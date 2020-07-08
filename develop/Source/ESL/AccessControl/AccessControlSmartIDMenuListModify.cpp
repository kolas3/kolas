// AccessControlSmartIDMenuListModify.cpp : implementation file
//

#include "stdafx.h"
#include "accesscontrol.h"
#include "AccessControlSmartIDMenuListModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListModify dialog


CAccessControlSmartIDMenuListModify::CAccessControlSmartIDMenuListModify(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSmartIDMenuListModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSmartIDMenuListModify)
	m_strTITLE_DESC = _T("");
	//}}AFX_DATA_INIT
	
	// KOL.UDF.022
	m_nWorkingStatus = 0;
	m_pWorkingStatus = NULL;
	m_pWorkingStatusDesc = NULL;
	m_strWorkingStatus = _T("");
}


VOID CAccessControlSmartIDMenuListModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSmartIDMenuListModify)
	DDX_Control(pDX, IDC_cWORKING_STATUS, m_cWorkingStatus);
	DDX_Text(pDX, IDC_eTITLE_DESC, m_strTITLE_DESC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSmartIDMenuListModify, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSmartIDMenuListModify)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_CBN_SELCHANGE(IDC_cWORKING_STATUS, OnSelchangecWORKINGSTATUS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListModify message handlers

BOOL CAccessControlSmartIDMenuListModify::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

BOOL CAccessControlSmartIDMenuListModify::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitComboBox();
	ShowWorkingStatusDesc();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlSmartIDMenuListModify::OnbOK() 
{
	UpdateData(TRUE);
	m_cWorkingStatus.GetWindowText(m_strWorkingStatus);
	m_strWorkingStatus.TrimLeft();		m_strWorkingStatus.TrimRight();

	m_strTITLE_DESC.TrimLeft();			m_strTITLE_DESC.TrimRight();

	CDialog::OnOK();
}

VOID CAccessControlSmartIDMenuListModify::OnbCANCEL() 
{
	CDialog::OnCancel();
}

VOID CAccessControlSmartIDMenuListModify::InitComboBox(VOID)
{
	m_cWorkingStatus.ResetContent();
	
	INT nSelect = -1;
	for (INT i = 0; i < m_nWorkingStatus; i++) {
		m_cWorkingStatus.AddString(m_pWorkingStatus[i]);
		if (m_strWorkingStatus == m_pWorkingStatus[i]) nSelect = i;
	}
	if (nSelect >= 0) m_cWorkingStatus.SetCurSel(nSelect);

	UpdateData(FALSE);
	return;
}

VOID CAccessControlSmartIDMenuListModify::ShowWorkingStatusDesc(VOID)
{
	CString str;
	CString strDesc = _T("");

	UpdateData(TRUE);
	INT nSelect = m_cWorkingStatus.GetCurSel();
	str = m_pWorkingStatus[nSelect];
	str.TrimLeft();		str.TrimRight();

	for (INT i = 0; i < m_nWorkingStatus; i++) {
		if (m_pWorkingStatus[i] == str) {
			strDesc = m_pWorkingStatusDesc[i];
			break;
		}
	}

	GetDlgItem(IDC_STATIC_WORKING_STATUS_DESC)->SetWindowText(strDesc);

}

VOID CAccessControlSmartIDMenuListModify::OnSelchangecWORKINGSTATUS() 
{
	ShowWorkingStatusDesc();	
}

