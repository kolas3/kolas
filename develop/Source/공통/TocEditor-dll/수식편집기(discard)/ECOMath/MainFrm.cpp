// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ECOMath.h"

#include "MainFrm.h"

#include "ECOMathView.h"
#include "SourceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(WM_ECOMATH, OnEcoMath)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SEND_SOOSIK, OnSendSoosik)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hParentWnd= NULL ;
}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	if (!m_wndMyBar.Create(this, IDD_DIALOGBAR, 
		CBRS_TOP, IDD_DIALOGBAR))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//	m_wndMyBar.EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndMyBar);

//	m_wndToolBar.ShowWindow(SW_HIDE) ;

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{

	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CECOMathView), CSize(100, 150), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSourceView), CSize(100, 100), pContext);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;
	cs.cx= 550;
	cs.cy= 350;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
VOID CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

VOID CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

#include "TocEDitorDef.h"

long CMainFrame::OnEcoMath(WPARAM wParam, LPARAM lParam)
{
	INT ret = -1;
//	CString sTmp;
	switch(wParam) {
		case ECOMATH_CONNECT :
//			AfxMessageBox(_T("CONNECT"));
			ret = 1;
			break;
		case ECOMATH_INIT :
			m_cSharedMemory.Open(_T("TestPool"), 20);
			ret = 1;
			break;
		case ECOMATH_INITOK :
			ret = 1;
			break;
		case ECOMATH_SET_HWND :
			m_hParentWnd= (HWND)lParam ;
//			sTmp.Format( _T("%d") , m_hParentWnd ) ;
//			AfxMessageBox(sTmp) ;
			if ( m_hParentWnd==NULL )	ret = -1;
			else						ret = 1;
			break;
		case ECOMATH_GETLEN :
			ret = Send2Toc(0);
			break;
		case ECOMATH_GETMATH :
			Send2Toc(1);
			ret = 1;
			break;
		case ECOMATH_EXIT :
			exit(0);
			break;
		default :
			ret = -1;
			break;
	}

	return ret;
}

INT CMainFrame::Send2Toc(INT op)
{
	CString buffer;
	CString send;
	CString msg;
	
	CWnd *pView = m_wndSplitter.GetPane(1, 0);
	pView ->GetWindowText(buffer);

	if(op == 0) {
		return buffer.GetLength();
	}

	for(INT loop = 0; loop < (buffer.GetLength() /20 + 1); loop++) {
		send = buffer.Mid(loop * 20, 20);
		
		switch(m_cSharedMemory.Write(send.GetBuffer(1), send.GetLength(), 10000))
		{
		case CSharedMemory::MEM_SUCCESS:
			break;
		case CSharedMemory::MEM_ERROR_CLOSED:
			AfxMessageBox(_T("Not opened"));
			break;
		case CSharedMemory::MEM_ERROR_TIMEOUT:
			AfxMessageBox(_T("Timeout"));
			break;
		case CSharedMemory::MEM_ERROR_OTHERPARTY:
			AfxMessageBox(_T("Other party disconnected"));
			m_cSharedMemory.Close();
			break;
		case CSharedMemory::MEM_ERROR_UNKNOWN:
			AfxMessageBox(_T("Unknown"));
			break;
		}
	}

//	AfxMessageBox(send);
	return 1;
}

VOID CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
/*
	if ( m_wndSplitter.m_hWnd!=NULL )
	{
		CString sSoosik ;
		CWnd* pWnd = NULL  ;
		pWnd= m_wndSplitter.GetPane(1,0) ;
		if ( pWnd != NULL )
		{
			pWnd->GetWindowText(sSoosik) ;
			if ( sSoosik != _T("$ $") ) {
				CString sMsg ;
				sMsg= _T("수식 태그 '") + sSoosik + _T("' 을(를) 에디터의 현재 위치에 넣겠습니까??") ;
				INT retval= AfxMessageBox(sMsg, MB_YESNO|MB_ICONQUESTION ) ;
				if ( retval==IDYES )
				{
					OnSendSoosik() ;
				}
			}
		}
	}
*/
	CFrameWnd::OnClose();
}

VOID CMainFrame::OnSendSoosik() 
{
	// TODO: Add your command handler code here
	if ( m_hParentWnd==NULL )
	{
		AfxMessageBox( _T("목차편집기 혹은 초록편집기가 없습니다.") , MB_ICONERROR ) ;
		return ;
	}

	::PostMessage(m_hParentWnd, WM_READ_ECOMATH, 0, 0) ;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE )
	{
		exit(-1) ;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
