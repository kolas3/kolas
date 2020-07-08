// MarcAdjFrame.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjFrame.h"

#include "imm.h"
#include <locale.h>

#include "MarcAdjMarcEditView.h"
#include "MarcAdjMarcGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjFrame

IMPLEMENT_DYNCREATE(CMarcAdjFrame, CFrameWnd)

CMarcAdjFrame::CMarcAdjFrame()
{
	m_pMarcAdjMgr = NULL;
}

CMarcAdjFrame::~CMarcAdjFrame()
{

}


BEGIN_MESSAGE_MAP(CMarcAdjFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMarcAdjFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_USER + 1000, OnSetFocusVol)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GRID_UPDATE, OnMarcGridUpdate)
	ON_MESSAGE(WM_EDIT_UPDATE, OnMarcEditUpdate)
	ON_MESSAGE(WM_CODE_UPDATE, OnCodeUpdate)
	ON_MESSAGE(WM_VOLUME_UPDATE, OnVolumeUpdate)

	ON_MESSAGE(WM_AC_AUTHOR, OnACDlgOpen)
	ON_MESSAGE(WM_AC_TITLE, OnACDlgOpen)
	ON_MESSAGE(WM_AC_SUBJECT, OnACDlgOpen)

	ON_MESSAGE( WM_USER + 1027 , OnTouchPad )

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

VOID CMarcAdjFrame::OnSetFocusVol()
{
	m_pMarcAdjMgr->GetResourceMgr()->m_dlgVolume.SetFocus();
}

VOID CMarcAdjFrame::OnSetFocusGridEdit()
{
	m_pMarcAdjMgr->GetResourceMgr()->GetMarcEditor()->SetEditFocus();
}
/////////////////////////////////////////////////////////////////////////////
// CMarcAdjFrame Operations
VOID CMarcAdjFrame::LoadWindowPosition(CREATESTRUCT &cs)
{
	CWinApp *pApp = AfxGetApp();

	INT ids = -1;
	CRect rect;

	if ((rect.left = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("left"), -1)) != -1 &&
		(rect.top = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("top"), -1)) != -1 &&
		(rect.right = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("right"), -1)) != -1 &&
		(rect.bottom = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("bottom"), -1)) != -1
		)
	{
		cs.cx = rect.right - rect.left;
		cs.cy = rect.bottom - rect.top;

		INT x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		INT y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);

		cs.x = min(rect.left, x);
		cs.y = min(rect.top, y);
	}

	INT nStyle = -1;
	if ((nStyle = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("showCmd"), -1)) != -1)
	{
		if (nStyle == SW_SHOWMAXIMIZED)
			cs.style |= WS_MAXIMIZE;
	}
}

VOID CMarcAdjFrame::SaveWindowPosition()
{
	CWinApp *pApp = AfxGetApp();
		
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("top"), wp.rcNormalPosition.top);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("left"), wp.rcNormalPosition.left);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("right"), wp.rcNormalPosition.right);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("bottom"), wp.rcNormalPosition.bottom);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("showCmd"), wp.showCmd);

	INT cxCur = 100;
	INT cxMin = 100;
	m_wndSplitter.GetColumnInfo(0, cxCur, cxMin);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("col0"), cxCur);
	m_wndSplitter.GetColumnInfo(1, cxCur, cxMin);
	pApp->WriteProfileInt(_T("MARC_ADJ_FRAME"), _T("col1"), cxCur);
}

VOID CMarcAdjFrame::ShowStatusInfo()
{
	HWND hWnd	= GetSafeHwnd();
    HIMC hIMC	= ImmGetContext(hWnd);
    BOOL bOpen	= ImmGetOpenStatus(hIMC);

    if (bOpen)
        m_wndStatusBar.SetPaneText(3, _T("한글") );
    else 
        m_wndStatusBar.SetPaneText(3, _T("영문") );

    CTime time = CTime::GetCurrentTime();
	CString strTime;

	INT nHour = time.GetHour();
	if (nHour < 12) {
		strTime.Format(	_T("%04d년 %02d월 %02d일 오전 %02d:%02d"), 
						time.GetYear(), time.GetMonth(), time.GetDay(),
						nHour, time.GetMinute()
					);
	} else {
		if (nHour >= 13) nHour -= 12;
		strTime.Format(	_T("%04d년 %02d월 %02d일 오후 %02d:%02d"), 
						time.GetYear(), time.GetMonth(), time.GetDay(),
						nHour, time.GetMinute()
					);
	}
    
    m_wndStatusBar.SetPaneText(2, strTime);
	
	ImmReleaseContext(hWnd,hIMC);
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjFrame message handlers

BOOL CMarcAdjFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~(FWS_ADDTOTITLE);

	LoadWindowPosition(cs);
	
	return CFrameWnd::PreCreateWindow(cs);
}

INT CMarcAdjFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("MARC 정리"));

	HICON hIcon = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_CAT_MARC_COMPLETE));

	SetIcon(hIcon, FALSE);
	
	if (!m_wndStatusBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_BOTTOM) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NOBORDERS, 100); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL,210);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL,30);

	EnableDocking(CBRS_ALIGN_ANY);

	m_pMarcAdjMgr->CreateResource(this);

	_tsetlocale(LC_TIME, _T("korea"));
	SetTimer(0, 500, NULL);
	
	return 0;
}

BOOL CMarcAdjFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2))	
	{
		TRACE0("Failed to create splitter bar ");
		return FALSE;    // failed to create
	}
	
	CRect rcClient;
	GetClientRect(rcClient);

	INT cxCur0 = rcClient.Width() / 2;
	INT cxCur1 = cxCur0;

	CWinApp *pApp = AfxGetApp();
	cxCur0 = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("col0"), cxCur0);
	cxCur1 = pApp->GetProfileInt(_T("MARC_ADJ_FRAME"), _T("col1"), cxCur1);

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMarcAdjMarcGridView), CSize(cxCur0, rcClient.Height()), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMarcAdjMarcEditView), CSize(cxCur1, rcClient.Height()), pContext);

	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

UINT CMarcAdjFrame::OnCodeUpdate(WPARAM wParam, LPARAM lParam)
{
	return m_pMarcAdjMgr->OnCodeUpdate();
}

UINT CMarcAdjFrame::OnVolumeUpdate(WPARAM wParam, LPARAM lParam)
{
	return m_pMarcAdjMgr->OnVolumeUpdate();
}

UINT CMarcAdjFrame::OnMarcGridUpdate(WPARAM wParam, LPARAM lParam)
{
	return m_pMarcAdjMgr->OnMarcGridUpdate();
}

UINT CMarcAdjFrame::OnMarcEditUpdate(WPARAM wParam, LPARAM lParam)
{
	return m_pMarcAdjMgr->OnMarcEditUpdate();
}

BOOL CMarcAdjFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	static BOOL bRun = FALSE;

	if(FALSE == bRun)
	{
		bRun = TRUE;

		TRACE(_T("[[%d]]\r\n"), wParam);
		if (m_pMarcAdjMgr->OnCommand((UINT)wParam))
		{
			bRun = FALSE;
			return TRUE;
		}		
		bRun = FALSE;
	}
	else TRUE;

	return CFrameWnd::OnCommand(wParam, lParam);
}

BOOL CMarcAdjFrame::OnCmdMsg(UINT nID, INT nCode, VOID* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_pMarcAdjMgr->OnCmdMsg(nID))
		return TRUE;
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

VOID CMarcAdjFrame::OnClose() 
{
	SaveWindowPosition();

	CCatESL_Mgr *pCatMgr = m_pMarcAdjMgr->GetCatESLMgr();
	if ( m_pMarcAdjMgr->GetRecievedRegNoArray()->GetSize() >= 1 ) {
		pCatMgr->GetCatApi()->InsertToDropAccessionNoTbl(m_pMarcAdjMgr->GetRecievedRegNoArray(), pCatMgr->m_pDM_Species, TRUE);
	}
	m_pMarcAdjMgr->GetRecievedRegNoArray()->RemoveAll();
	
	CFrameWnd::OnClose();
}

VOID CMarcAdjFrame::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0)
		ShowStatusInfo();
	
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CMarcAdjFrame::PreTranslateMessage(MSG* pMsg) 
{
	CString strKey;
    INT nIds;

	if (pMsg->message == WM_KEYDOWN)
	{		
		if ( GetKeyState(VK_CONTROL) < 0 )
		{
			strKey.Format(_T("CTRL+%c"), pMsg->wParam);
			if (strKey == _T("CTRL+S")) 
			{
				pMsg->wParam = 0;
				CMarcEditCtrl *pMarcEditCtrl = m_pMarcAdjMgr->GetResourceMgr()->m_MarcEditor.GetEditCtrl();
				pMarcEditCtrl->ApplyEdit();			
				nIds = m_pMarcAdjMgr->CMD_Save();
			
				return TRUE;				
			}
		}
		
		if( pMsg->wParam == VK_HANGUL )
		{
			ShowStatusInfo();
			return TRUE;
		}
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

UINT CMarcAdjFrame::OnACDlgOpen(WPARAM wParam, LPARAM lParam)
{
	if ( wParam == 0 )
		m_pMarcAdjMgr->OnACDlgOpen(0);
	else if ( wParam == 3 )
		m_pMarcAdjMgr->OnACDlgOpen(3);
	else if ( wParam == 4 )
		m_pMarcAdjMgr->OnACDlgOpen(4);
	
	return -1;
}


void CMarcAdjFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	if ( m_pMarcAdjMgr != NULL && 
		m_pMarcAdjMgr->GetResourceMgr() != NULL && 
		m_pMarcAdjMgr->GetResourceMgr()->GetMarcEditor() != NULL )
	{
		m_pMarcAdjMgr->GetResourceMgr()->GetMarcEditor()->MoveAdjustWindow();
	}
	
}

LRESULT CMarcAdjFrame::OnTouchPad(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("%s\r\n"), wParam);

	if(NULL != m_pMarcAdjMgr)
	{
		UINT uID = m_pMarcAdjMgr->OnTouchPad(wParam, lParam);
		if(0 < uID)
		{
			OnCommand((WPARAM)uID, NULL);
		}
	}
	return 0;
}