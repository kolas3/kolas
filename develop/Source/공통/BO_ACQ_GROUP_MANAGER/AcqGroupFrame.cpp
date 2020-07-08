// AcqGroupFrame.cpp : implementation file
//

#include "stdafx.h"
#include "AcqGroupFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupFrame

IMPLEMENT_DYNCREATE(CAcqGroupFrame, CFrameWnd)

CAcqGroupFrame::CAcqGroupFrame()
{
	m_pAcqGroupMgr = NULL;
}

CAcqGroupFrame::~CAcqGroupFrame()
{
}

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};

BEGIN_MESSAGE_MAP(CAcqGroupFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CAcqGroupFrame)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupFrame message handlers

BOOL CAcqGroupFrame::OnCmdMsg(UINT nID, INT nCode, VOID* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pAcqGroupMgr->OnCmdMsg(nID))
		return TRUE;
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CAcqGroupFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pAcqGroupMgr->OnCommand((UINT)wParam))
		return TRUE;
	return CFrameWnd::OnCommand(wParam, lParam);
}

VOID CAcqGroupFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveWindowPosition();
	CFrameWnd::OnClose();
}

INT CAcqGroupFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	EnableDocking(CBRS_ALIGN_ANY);
	m_pAcqGroupMgr->CreateResource(this);
	return 0;
}

VOID CAcqGroupFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CAcqGroupFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	LoadWindowPosition(cs);
	return CFrameWnd::PreCreateWindow(cs);
}

VOID CAcqGroupFrame::LoadWindowPosition(CREATESTRUCT &cs)
{
	CWinApp *pApp = AfxGetApp();
	
	INT ids = -1;
	CRect rect;
	
	if ((rect.left = pApp->GetProfileInt(_T("ACQ_GROUP_FRAME"), _T("left"), -1)) != -1 &&
		(rect.top = pApp->GetProfileInt(_T("ACQ_GROUP_FRAME"), _T("top"), -1)) != -1 &&
		(rect.right = pApp->GetProfileInt(_T("ACQ_GROUP_FRAME"), _T("right"), -1)) != -1 &&
		(rect.bottom = pApp->GetProfileInt(_T("ACQ_GROUP_FRAME"), _T("bottom"), -1)) != -1
		)
	{
		cs.cx = rect.right - rect.left;

		cs.cy = rect.bottom - rect.top;
		
		INT x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		INT y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		
		cs.x = min(rect.left, x);
		cs.y = min(rect.top, y);
	}
}

VOID CAcqGroupFrame::SaveWindowPosition()
{
	CWinApp *pApp = AfxGetApp();
	
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	pApp->WriteProfileInt(_T("ACQ_GROUP_FRAME"), _T("top"), wp.rcNormalPosition.top);
	pApp->WriteProfileInt(_T("ACQ_GROUP_FRAME"), _T("left"), wp.rcNormalPosition.left);
	pApp->WriteProfileInt(_T("ACQ_GROUP_FRAME"), _T("right"), wp.rcNormalPosition.right);
	pApp->WriteProfileInt(_T("ACQ_GROUP_FRAME"), _T("bottom"), wp.rcNormalPosition.bottom);
}
