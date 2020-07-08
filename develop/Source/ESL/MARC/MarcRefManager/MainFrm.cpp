// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MarcManager.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MarcManagerView.h"
#include "MarcManagerWorkView.h"
#include "MarcManagerGroupSelect.h"
#include "MarcManagerEditor.h"
#include "..\MarcToolKit\ApplyMarcRelationDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDITOR_MODE, OnEditorMode)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_APPLY_CHANGE, OnApplyChange)
	ON_WM_CLOSE()
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
	
}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
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

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// CG: The following block was added by the Splitter Bar component.
	{
		// 화면을 분할 하고
		if (!m_wndSplitter.CreateStatic(this, 1, 2))
		{
			TRACE0("Failed to create splitter bar ");
			return FALSE;    // failed to create
		}

		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMarcManagerView), CSize(260, 200), pContext);
		m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMarcManagerWorkView), CSize(200, 200), pContext);
        
		//타이머 설정을 통해 그룹을 선택하게 한다 
		SetTimer(0,300,NULL);

		return TRUE;
	}
}

CView * CMainFrame::GetFirstView(VOID)
{
	CDocument *pDoc = GetActiveDocument();
	if (!pDoc) return NULL;
	POSITION pd =  pDoc->GetFirstViewPosition();
	return pDoc->GetNextView(pd);
}

CView * CMainFrame::GetSecondView(VOID)
{
	CDocument *pDoc = GetActiveDocument();
	if (!pDoc) return NULL;
	POSITION pd =  pDoc->GetFirstViewPosition();
	CView * pView = pDoc->GetNextView(pd);
	if (!pView) return NULL;
	return pDoc->GetNextView(pd);
}

VOID CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
        KillTimer(0);
        CMarcManagerGroupSelect dlg(this);
	    if(dlg.DoModal() == IDOK) m_sMarcGroup = dlg.m_sGroupCode;
  		
		CMarcManagerView *pView = (CMarcManagerView *)GetFirstView();
		pView->m_sMarcGroup = m_sMarcGroup;
        pView->m_sGroupCodeOnView = m_sMarcGroup;
		//pView->OnbTagSearch();
		pView->UpdateData(FALSE);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

VOID CMainFrame::OnEditorMode() 
{
	// TODO: Add your command handler code here
	CMarcManagerEditor dlg(this);
	dlg.DoModal();
}

VOID CMainFrame::OnExport() 
{
	// TODO: Add your command handler code here
	CApplyMarcRelationDlg dlg(this);
	dlg.DoModal();
}

VOID CMainFrame::OnApplyChange() 
{
	// TODO: Add your command handler code here
	CApplyMarcRelationDlg *pDlg = new CApplyMarcRelationDlg(this);
	if (!pDlg->Create(this)) goto END;
	if (MessageBox(_T("이 작업을 하는데 약간의 시간이 걸립니다. 계속하시겠습니까?"), NULL, MB_YESNO | MB_ICONQUESTION) != IDYES) goto END;

	if (pDlg->ApplyMarcRelation_Simple(((CMarcManagerView*)GetFirstView())->m_sMarcGroup) < 0) goto END;
	
	MessageBox(_T("변경적용 완료 되었습니다"));

END:
	if (pDlg)
		delete pDlg;
}

VOID CMainFrame::OnClose() 
{
	CESL_DataMgr dm1;
	
	if (dm1.ORACLE_GetConnectedDB() == TRUE) {
		dm1.ORACLE_Commit();
		dm1.ORACLE_DisconnectDB();
	}


	CFrameWnd::OnClose();
}
