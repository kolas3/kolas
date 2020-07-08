// ToolBarBtnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "ToolBarBtnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarBtnDlg dialog


CToolBarBtnDlg::CToolBarBtnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarBtnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CToolBarBtnDlg::~CToolBarBtnDlg()
{
	m_BGBrush.DeleteObject();
}
void CToolBarBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarBtnDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarBtnDlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_MAIN1, OnBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarBtnDlg message handlers

void CToolBarBtnDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAIN1);
	if(pWnd)
	{
		pWnd->MoveWindow(0, 0, cx, cy);
	}
}

BOOL CToolBarBtnDlg::Create(CWnd* pWnd)
{
	BOOL bResult = CDialog::Create(IDD, pWnd);

	m_BGBitmap.LoadBitmap(IDB_TOOLBAR_BK);	
	m_BGBrush.CreatePatternBrush(&m_BGBitmap);

	return bResult;
}

BOOL CToolBarBtnDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
// 	return CDialog::OnEraseBkgnd(pDC);

	CRect rect;
	GetClientRect(&rect);
		
	pDC->FillRect(&rect, &m_BGBrush);

	return TRUE;
}

BOOL CToolBarBtnDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CToolBarBtnDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
}

void CToolBarBtnDlg::OnBtn() 
{
	// TODO: Add your control notification handler code here
	GetParent()->SendMessage(WM_USER+100, NULL, NULL);
}

BOOL CToolBarBtnDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}
