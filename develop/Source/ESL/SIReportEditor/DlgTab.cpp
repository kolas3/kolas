// DlgTab.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTab dialog


CDlgTab::CDlgTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab)
	//}}AFX_DATA_INIT
}


VOID CDlgTab::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab)
	DDX_Control(pDX, IDC_TAB_VIEW, m_tabCtrl);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgTab, CDialog)
	//{{AFX_MSG_MAP(CDlgTab)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VIEW, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab message handlers

BOOL CDlgTab::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// Initialize tab ctrl
	m_imageListTab.Create(IDB_TAB, 24, 1, RGB(255,255,255));
	
	m_tabCtrl.SetImageList(&m_imageListTab);
	
	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE | TCIF_TEXT;

	tcItem.iImage = 0;
	tcItem.pszText = _T("Control");
	m_tabCtrl.InsertItem(0, &tcItem);

	tcItem.iImage = 1;
	tcItem.pszText = _T("Sheet");
	m_tabCtrl.InsertItem(2, &tcItem);

	// Init dialog
	m_dlgControlBox.Create(IDD_CONTROLBOX, this);
	m_dlgSheetView.Create(IDD_SHEET_VIEW, this);

	m_dlgControlBox.ShowWindow(SW_SHOW);
	m_dlgControlBox.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgTab::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (m_tabCtrl.GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	m_tabCtrl.MoveWindow(rect);

	rect.top += 40;
	rect.left += 20;
	rect.right -= 20;
	rect.bottom -= 20;
	
	m_dlgControlBox.MoveWindow(rect);
	m_dlgSheetView.MoveWindow(rect);


EFS_END
}

VOID CDlgTab::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	if (m_tabCtrl.GetCurSel() == 0)
	{
		m_dlgControlBox.SetFocus();
		m_dlgControlBox.ShowWindow(SW_SHOW);
		m_dlgSheetView.ShowWindow(SW_HIDE);
		
	}
	else if (m_tabCtrl.GetCurSel() == 1)
	{
		m_dlgSheetView.SetFocus();
		m_dlgControlBox.ShowWindow(SW_HIDE);
		m_dlgSheetView.ShowWindow(SW_SHOW);
	}

	*pResult = 0;

EFS_END
}
