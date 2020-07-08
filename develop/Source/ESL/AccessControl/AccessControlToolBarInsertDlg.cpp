// AccessControlToolBarInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlToolBarInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolBarInsertDlg dialog


CAccessControlToolBarInsertDlg::CAccessControlToolBarInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlToolBarInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlToolBarInsertDlg)
	m_sAlias = _T("");
	m_sCaption = _T("");
	m_sDisabled = _T("");
	m_sHot = _T("");
	m_iIdx = 0;
	m_sNormal = _T("");
	m_sSM = _T("");
	m_ToolTip = _T("");
	//}}AFX_DATA_INIT

	MODE = 1;			// 입력모드 1, 수정모드 -1
	m_strACC = _T("");
	// KOL.UDF.022
	m_cSTYLE = ' ';
	m_cTYPE = ' ';
}


VOID CAccessControlToolBarInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlToolBarInsertDlg)
	DDX_Control(pDX, IDC_cACC_KEY, m_cACC_KEY);
	DDX_Control(pDX, IDC_cACC_SYSKEY, m_cACC_SYSKEY);
	DDX_Text(pDX, IDC_eALIAS, m_sAlias);
	DDX_Text(pDX, IDC_eCAPTION, m_sCaption);
	DDX_Text(pDX, IDC_eDISABLED, m_sDisabled);
	DDX_Text(pDX, IDC_eHOT, m_sHot);
	DDX_Text(pDX, IDC_eIDX, m_iIdx);
	DDX_Text(pDX, IDC_eNORMAL, m_sNormal);
	DDX_Text(pDX, IDC_eSM, m_sSM);
	DDX_Text(pDX, IDC_TOOLTIP, m_ToolTip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlToolBarInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlToolBarInsertDlg)
	ON_BN_CLICKED(IDOK, OnbOK)
	ON_BN_CLICKED(IDCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bTYPECLEAR, OnbTYPECLEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolBarInsertDlg message handlers

VOID CAccessControlToolBarInsertDlg::OnbOK() 
{
	UpdateData(true);
	CButton *radio1 = (CButton *)GetDlgItem(IDC_ACC_RADIO1);
	CButton *radio2 = (CButton *)GetDlgItem(IDC_ACC_RADIO2);
	CButton *radio3 = (CButton *)GetDlgItem(IDC_ACC_RADIO3);
	CButton *radio4 = (CButton *)GetDlgItem(IDC_ACC_RADIO4);
	CButton *radio5 = (CButton *)GetDlgItem(IDC_ACC_RADIO7);

	if (radio1->GetCheck()) m_cSTYLE = 'B';		// Button style
		else m_cSTYLE = 'S';					// separator style
	m_cTYPE = ' ';
	if (radio3->GetCheck()) m_cTYPE = 'R';		// Read only type
	if (radio4->GetCheck()) m_cTYPE = 'I';		// Invisible type
	if (radio5->GetCheck()) m_cTYPE = 'D';		// Access Denied type
	
	CString strSYSKEY, strKEY;
	m_cACC_SYSKEY.GetWindowText(strSYSKEY);
	strSYSKEY.TrimLeft();		strSYSKEY.TrimRight();		strSYSKEY.MakeUpper();
	m_cACC_KEY.GetWindowText(strKEY);
	strKEY.TrimLeft();		strKEY.TrimRight();		strKEY.MakeUpper();
	if (strSYSKEY.Find(_T("없음")) == -1) {
		m_strACC.Format(_T("%s+%s"), strSYSKEY, strKEY);
	} else {
		m_strACC = _T("");
	}

	CDialog::OnOK();		// 다이알 로그를 닫는다.
}

VOID CAccessControlToolBarInsertDlg::OnbCANCEL() 
{
	CDialog::OnCancel();
}

BOOL CAccessControlToolBarInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	NewButtonList.Add(IDC_bOK, this, IDB_OK_U, IDB_OK_D, IDB_OK_F);
	NewButtonList.Add(IDC_bCANCEL, this, IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);

	m_cACC_SYSKEY.ResetContent();
	m_cACC_SYSKEY.AddString(_T("단축키 없음"));
	m_cACC_SYSKEY.AddString(_T("CTRL"));
	m_cACC_SYSKEY.AddString(_T("ALT"));
	m_cACC_SYSKEY.SetCurSel(0);

	CString str;
	m_cACC_KEY.ResetContent();
	for (INT i = 'A'; i <= 'Z'; i++) {
		str.Format(_T("%c"), i);
		m_cACC_KEY.AddString(str);
	}
	m_cACC_KEY.SetCurSel(0);

	CButton *radio1 = (CButton *)GetDlgItem(IDC_ACC_RADIO1);
	CButton *radio2 = (CButton *)GetDlgItem(IDC_ACC_RADIO2);
	CButton *radio3 = (CButton *)GetDlgItem(IDC_ACC_RADIO3);
	CButton *radio4 = (CButton *)GetDlgItem(IDC_ACC_RADIO4);
	CButton *radio5 = (CButton *)GetDlgItem(IDC_ACC_RADIO7);

	radio1->SetCheck(false);
	radio2->SetCheck(false);
	radio3->SetCheck(false);
	radio4->SetCheck(false);
	radio5->SetCheck(false);

	if (MODE == 1) {			// 입력모드 - 화면상태를 초기값으로
		radio1->SetCheck(true);
		GetDlgItem(IDC_eALIAS)->EnableWindow(true);
	}
	if (MODE == -1) {			// 수정모드 - 화면상태를 수정값으로
		if (m_cSTYLE == 'S') {
			radio2->SetCheck(true);			
		} else {
			radio1->SetCheck(true);
		}
		if (m_cTYPE == 'R') radio3->SetCheck(true);
		if (m_cTYPE == 'I') radio4->SetCheck(true);
		if (m_cTYPE == 'D') radio5->SetCheck(true);
		GetDlgItem(IDC_eALIAS)->EnableWindow(false);

		// 단축키 설정
		m_strACC.TrimLeft();			m_strACC.TrimRight();
		m_strACC.MakeUpper();

		CString strSYSKEY, strKEY;
		INT nCutPos = m_strACC.Find(_T("+"), 0);
		if (nCutPos == -1) {
			m_cACC_KEY.SelectString(0, _T("단축키 없음"));
		} else {
			strSYSKEY = m_strACC.Left(nCutPos);
			strSYSKEY.TrimLeft();			strSYSKEY.TrimRight();
			strSYSKEY.MakeUpper();
			strKEY = m_strACC.Mid(nCutPos+1);
			strKEY.TrimLeft();				strKEY.TrimRight();
			strKEY.MakeUpper();

			m_cACC_SYSKEY.SelectString(0, strSYSKEY);
			m_cACC_KEY.SelectString(0, strKEY);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlToolBarInsertDlg::OnbTYPECLEAR() 
{
	CButton *radio3 = (CButton *)GetDlgItem(IDC_ACC_RADIO3);
	CButton *radio4 = (CButton *)GetDlgItem(IDC_ACC_RADIO4);
	CButton *radio5 = (CButton *)GetDlgItem(IDC_ACC_RADIO7);

	radio3->SetCheck(false);
	radio4->SetCheck(false);
	radio5->SetCheck(false);
}
