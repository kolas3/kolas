// AccessControlSelectionMenuModifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlSelectionMenuModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionMenuModifyDlg dialog


CAccessControlSelectionMenuModifyDlg::CAccessControlSelectionMenuModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSelectionMenuModifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSelectionMenuModifyDlg)
	m_strCAPTION = _T("");
	m_nIdx = 0;
	//}}AFX_DATA_INIT
}


VOID CAccessControlSelectionMenuModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSelectionMenuModifyDlg)
	DDX_Text(pDX, IDC_eCAPTION, m_strCAPTION);
	DDX_Text(pDX, IDC_eIDX, m_nIdx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSelectionMenuModifyDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSelectionMenuModifyDlg)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionMenuModifyDlg message handlers

VOID CAccessControlSelectionMenuModifyDlg::OnbOK() 
{
	CDialog::OnOK();
}

VOID CAccessControlSelectionMenuModifyDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}
