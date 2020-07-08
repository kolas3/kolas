// ESLDlg_Error.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_Error.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Error dialog


CESLDlg_Error::CESLDlg_Error(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_Error::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_Error)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CESLDlg_Error::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_Error)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_Error, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_Error)
	ON_BN_CLICKED(IDC_COM_bOK, OnbOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Error message handlers

BOOL CESLDlg_Error::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_SQL_ERROR_EDIT)->SetWindowText(m_msg);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CESLDlg_Error::OnbOK() 
{
	CDialog::OnCancel();
}
