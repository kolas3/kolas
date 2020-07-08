// ErrorAdjustFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorAdjustFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustFrame

IMPLEMENT_DYNCREATE(CErrorAdjustFrame, CFrameWnd)

CErrorAdjustFrame::CErrorAdjustFrame()
{
	m_pErrorAdjustMgr = NULL;
}

CErrorAdjustFrame::~CErrorAdjustFrame()
{
}


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

BEGIN_MESSAGE_MAP(CErrorAdjustFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CErrorAdjustFrame)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustFrame message handlers

VOID CErrorAdjustFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveWindowPosition();

	CFrameWnd::OnClose();
}

VOID CErrorAdjustFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

INT CErrorAdjustFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	EnableDocking(CBRS_ALIGN_ANY);
	m_pErrorAdjustMgr->CreateResource(this);
	
	return 0;
}

BOOL CErrorAdjustFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	LoadWindowPosition(cs);
	return CFrameWnd::PreCreateWindow(cs);
}

BOOL CErrorAdjustFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pErrorAdjustMgr->OnCommand((UINT)wParam))
	return TRUE;
	
	return CFrameWnd::OnCommand(wParam, lParam);
}

VOID CErrorAdjustFrame::LoadWindowPosition(CREATESTRUCT &cs)
{
	CWinApp *pApp = AfxGetApp();
	
	INT ids = -1;
	CRect rect;
	
	if ((rect.left = pApp->GetProfileInt(_T("ERROR_ADJUST_FRAME"), _T("left"), -1)) != -1 &&
		(rect.top = pApp->GetProfileInt(_T("ERROR_ADJUST_FRAME"), _T("top"), -1)) != -1 &&
		(rect.right = pApp->GetProfileInt(_T("ERROR_ADJUST_FRAME"), _T("right"), -1)) != -1 &&
		(rect.bottom = pApp->GetProfileInt(_T("ERROR_ADJUST_FRAME"), _T("bottom"), -1)) != -1
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
}

VOID CErrorAdjustFrame::SaveWindowPosition()
{
	// 프레임의 위치를 저장한다.
	CWinApp *pApp = AfxGetApp();
	
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	pApp->WriteProfileInt(_T("ERROR_ADJUST_FRAME"), _T("top"), wp.rcNormalPosition.top);
	pApp->WriteProfileInt(_T("ERROR_ADJUST_FRAME"), _T("left"), wp.rcNormalPosition.left);
	pApp->WriteProfileInt(_T("ERROR_ADJUST_FRAME"), _T("right"), wp.rcNormalPosition.right);
	pApp->WriteProfileInt(_T("ERROR_ADJUST_FRAME"), _T("bottom"), wp.rcNormalPosition.bottom);
}

BOOL CErrorAdjustFrame::OnCmdMsg(UINT nID, INT nCode, VOID* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pErrorAdjustMgr->OnCmdMsg(nID))
	return TRUE;
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
