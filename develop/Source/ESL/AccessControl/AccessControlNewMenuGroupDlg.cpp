// AccessControlNewMenuGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlNewMenuGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlNewMenuGroupDlg dialog


CAccessControlNewMenuGroupDlg::CAccessControlNewMenuGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlNewMenuGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlNewMenuGroupDlg)
	m_idx = 0;
	m_sName = _T("");
	m_strCaption = _T("");
	//}}AFX_DATA_INIT

	m_strGroupType = _T(" ");
	MODE = 1;
	m_cUSERFLAG = 'U';
}


VOID CAccessControlNewMenuGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlNewMenuGroupDlg)
	DDX_Text(pDX, IDC_eNAME, m_sName);
	DDX_Text(pDX, IDC_eCAPTION, m_strCaption);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAccessControlNewMenuGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlNewMenuGroupDlg)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_ckGROUPTYPE, OnckGROUPTYPE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlNewMenuGroupDlg message handlers

VOID CAccessControlNewMenuGroupDlg::OnbOK() 
{
	UpdateData(true);
	if (m_sName.GetLength() == 0) {
		AfxMessageBox(_T("이름을 입력하십시오."));
		return;
	}
	CButton *btn = (CButton*)GetDlgItem(IDC_ckGROUPTYPE);
	if (btn->GetCheck()) m_strGroupType = _T("T");
		else m_strGroupType = _T(" ");
	
	CDialog::OnOK();
}

VOID CAccessControlNewMenuGroupDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

BOOL CAccessControlNewMenuGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (MODE == -1) {
		CButton *pBtn= (CButton*)GetDlgItem(IDC_ckGROUPTYPE);
		if (pBtn) {
			if (m_strGroupType == _T("T")) {
				pBtn->SetCheck(TRUE);
			} else {
				pBtn->SetCheck(FALSE);
			}
			
		}
	}
	if (m_cUSERFLAG == 'S') SetReadOnly(FALSE);
		else SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlNewMenuGroupDlg::OnckGROUPTYPE() 
{

}

INT CAccessControlNewMenuGroupDlg::SetReadOnly(BOOL bFlag)
{
	UINT nArr_ResourceID[32];
	INT nResourceID = 0;
	nArr_ResourceID[nResourceID++] = IDC_eNAME;
	nArr_ResourceID[nResourceID++] = IDC_ckGROUPTYPE;
	nArr_ResourceID[nResourceID++] = IDC_eCAPTION;

	CWnd *pWnd = NULL;
	for (INT i = 0; i < nResourceID; i++) {
		pWnd = GetDlgItem(nArr_ResourceID[i]);
		if (pWnd) pWnd->EnableWindow(!bFlag);
	}
	return 0;
}