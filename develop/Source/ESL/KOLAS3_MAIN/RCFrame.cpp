// RCFrame.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include "KOLAS3_MAIN.h"
#include "KOLAS3_MAINApi.h"
#include "RCFrame.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCFrame

IMPLEMENT_DYNCREATE(CRCFrame, CFrameWnd)

CRCFrame::CRCFrame()
{
}

CRCFrame::~CRCFrame()
{

}


BEGIN_MESSAGE_MAP(CRCFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CRCFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_SET_PROGRESS_POS, OnSetProgressPos)
	ON_MESSAGE(WM_SET_PROGRESS_TEXT, OnSetProgressText)
	ON_MESSAGE(WM_SET_PROGRESS_SHOW, OnSetProgressShow)
	ON_MESSAGE(WM_SET_STATUS_TEXT, OnSetStatusText)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_STATUS_SLIDER,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CRCFrame Operations

VOID CRCFrame::SetHanEngStatusItems()
{
    HWND hWnd	= GetSafeHwnd();
    HIMC hIMC	= ImmGetContext(hWnd);
    BOOL bOpen	= ImmGetOpenStatus(hIMC);

    if (bOpen)
        m_wndStatusBar.SetPaneText(4, _T("한글") );
    else 
        m_wndStatusBar.SetPaneText(4, _T("영문") );

	ImmReleaseContext(hWnd,hIMC);
}

VOID CRCFrame::ShowCurrentStatus()
{
	CTime time = CTime::GetCurrentTime();
	HWND hWnd = GetSafeHwnd();

	CString strMsg;

	INT nHour = time.GetHour();
	if (nHour < 12) {
		strMsg.Format(	_T("%04d년 %02d월 %02d일 오전 %02d:%02d"), 
						time.GetYear(), time.GetMonth(), time.GetDay(),
						nHour, time.GetMinute()
					);
	} else {
		if (nHour >= 13) nHour -= 12;
		strMsg.Format(	_T("%04d년 %02d월 %02d일 오후 %02d:%02d"), 
						time.GetYear(), time.GetMonth(), time.GetDay(),
						nHour, time.GetMinute()
					);
	}
	m_wndStatusBar.SetPaneText(3, strMsg );

	SetHanEngStatusItems();
}

/////////////////////////////////////////////////////////////////////////////
// CRCFrame message handlers

INT CRCFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// status bar
	if (!m_wndStatusBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_BOTTOM) ||
	!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// progress bar
	if(!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,20,10), &m_wndStatusBar, ID_STATUS_PROGRESS2))
		return -1;

	m_wndProgressBar.SetRange(0, 10000);
	m_wndProgressBar.SetPos(0);
	m_wndProgressBar.ShowWindow(SW_HIDE);

/*
	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_SLIDER, SBPS_NOBORDERS, 100); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL,120);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL,80);
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL,50);
*/
	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_SLIDER, SBPS_NOBORDERS, 100); 
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL,400);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL,165);
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL,30);

	m_wndStatusBar.SetPaneText(2, _T(""));
	m_wndStatusBar.SetPaneText(3, _T(""));
	m_wndStatusBar.SetPaneText(4, _T(""));
		
	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	_tcscpy(logFont.lfFaceName,_T("굴림체"));
	logFont.lfHeight = 12;
	logFont.lfCharSet = DEFAULT_CHARSET;

	m_fontStatusBar.CreateFontIndirect(&logFont);
	m_wndStatusBar.SetFont(&m_fontStatusBar);

	SetTimer(0, 500, NULL);

	EnableDocking(CBRS_ALIGN_ANY);
	
	return 0;
}

VOID CRCFrame::OnTimer(UINT nIDEvent) 
{
	// Status Bar refresh
	if (nIDEvent == 0) 		
		ShowCurrentStatus();
	
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CRCFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	LoadWindowPosition(cs);
	
	return CFrameWnd::PreCreateWindow(cs);
}


BOOL CRCFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CView *pView = GetActiveView();
	if ( pView->GetSafeHwnd() != NULL ) pView->SendMessage(WM_COMMAND, wParam, lParam);
	
	return CFrameWnd::OnCommand(wParam, lParam);
}

BOOL CRCFrame::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_HANGUL ) 
	{
		SetHanEngStatusItems();
	}		
	return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CRCFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	UINT nIDLast = m_nIDLastMessage;
	m_nFlags &= ~WF_NOPOPMSG;

	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL)
	{
		CString strMsg;
		CString strMessage;

		// set the message bar text
		if (lParam != 0)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
   			m_strStatusBar = (LPCTSTR)lParam;
			strMsg = m_strStatusBar;
		}
		else if (wParam != 0)
		{
			// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
			if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
				wParam = AFX_IDS_PREVIEW_CLOSE;

			// get message associated with the ID indicated by wParam
            if (wParam == AFX_IDS_IDLEMESSAGE)
				strMsg = m_strStatusBar;
			else
			{
				GetMessageString(wParam, strMessage);
				strMsg = strMessage;
			}
		}

		pMessageBar->SetWindowText(strMsg);

		// update owner of the bar in terms of last message selected
		CFrameWnd* pFrameWnd = pMessageBar->GetParentFrame();
		if (pFrameWnd != NULL)
		{
			m_nIDLastMessage = (UINT)wParam;
			m_nIDTracking = (UINT)wParam;
		}
	}

	m_nIDLastMessage = (UINT)wParam;    	// new ID (or 0)
	m_nIDTracking = (UINT)wParam;       	// so F1 on toolbar buttons work

	return nIDLast;
}

BOOL CRCFrame::LoadWindowPosition(CREATESTRUCT& cs)
{
	CWinApp *pApp = AfxGetApp();

	INT ids = -1;
	CRect rect;

	if ((rect.left = pApp->GetProfileInt(_T("RC_FRAME"), _T("left"), -1)) != -1 &&
		(rect.top = pApp->GetProfileInt(_T("RC_FRAME"), _T("top"), -1)) != -1 &&
		(rect.right = pApp->GetProfileInt(_T("RC_FRAME"), _T("right"), -1)) != -1 &&
		(rect.bottom = pApp->GetProfileInt(_T("RC_FRAME"), _T("bottom"), -1)) != -1
		)
	{
		// width 
		cs.cx = rect.right - rect.left;
		// height
		cs.cy = rect.bottom - rect.top;

		// window가 밖으로 나가는지...
		INT x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		INT y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);


		cs.x = min(rect.left, x);
		cs.y = min(rect.top, y);
	}
	else
		return FALSE;

	return TRUE;
}

VOID CRCFrame::OnClose() 
{
	// 프레임의 위치를 저장한다.
	CWinApp *pApp = AfxGetApp();
		
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	pApp->WriteProfileInt(_T("RC_FRAME"), _T("top"), wp.rcNormalPosition.top);
	pApp->WriteProfileInt(_T("RC_FRAME"), _T("left"), wp.rcNormalPosition.left);
	pApp->WriteProfileInt(_T("RC_FRAME"), _T("right"), wp.rcNormalPosition.right);
	pApp->WriteProfileInt(_T("RC_FRAME"), _T("bottom"), wp.rcNormalPosition.bottom);
		
	CFrameWnd::OnClose();
}

VOID CRCFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;

	CRect   rc;
    m_wndStatusBar.GetItemRect(1,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
                                 rc.top-1,
                                 rc.Width(),
                                 rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);

	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);

	INT nPaneSize_0 = cx-750;
	if (nPaneSize_0 < 50) {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , 50); 
	} else {
		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS , nPaneSize_0); 
	}
}

LRESULT CRCFrame::OnSetProgressPos(WPARAM wParam, LPARAM lParam)
{
	m_wndProgressBar.SetRange(0, (short)lParam);
	m_wndProgressBar.SetPos((short)wParam);

	return 0;
}

LRESULT CRCFrame::OnSetProgressText(WPARAM wParam, LPARAM lParam)
{
	//m_wndProgressBar.SetRange(0, (short)lParam);
	CString strText;
	strText.Format(_T("%s"), ((CString*)wParam)->GetBuffer(0));
	m_wndProgressBar.SetWindowText(strText);
	m_wndProgressBar.SetPos((short)lParam);

	return 0;
}

LRESULT CRCFrame::OnSetProgressShow(WPARAM wParam, LPARAM lParam)
{
	m_wndProgressBar.ShowWindow((BOOL)wParam);
	return 0;
}

LRESULT CRCFrame::OnSetStatusText(WPARAM wParam, LPARAM lParam)
{
	TCHAR *pBuf = (TCHAR*)lParam;
	CString str;
	if (pBuf) str.Format(_T("%s"), pBuf);
		else str = _T("");

	m_wndStatusBar.SetPaneText(2, str );

	return 0;
}
