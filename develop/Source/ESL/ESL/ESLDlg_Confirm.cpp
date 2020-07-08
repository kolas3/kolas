// ESLDlg_Confirm.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_Confirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Confirm dialog


CESLDlg_Confirm::CESLDlg_Confirm(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_Confirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_Confirm)
	m_msg = _T("");
	//}}AFX_DATA_INIT
	m_show = true;
}


VOID CESLDlg_Confirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_Confirm)
	DDX_Text(pDX, IDC_STATIC_MSG, m_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_Confirm, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_Confirm)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Confirm message handlers

VOID CESLDlg_Confirm::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::EndDialog(IDYES);
}

VOID CESLDlg_Confirm::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CESLDlg_Confirm::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		if (m_show) m_show = false;
			else m_show = true;
		GetDlgItem(IDI_ICON_INFO)->ShowWindow(m_show);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CESLDlg_Confirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetTimer(0, 700, NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
