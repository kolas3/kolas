// ESLDlg_Work.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_Work.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Work dialog


CESLDlg_Work::CESLDlg_Work(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_Work::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_Work)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bInit = FALSE;
}


VOID CESLDlg_Work::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_Work)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_Work, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_Work)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_Work message handlers

BOOL CESLDlg_Work::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bInit = TRUE;	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


