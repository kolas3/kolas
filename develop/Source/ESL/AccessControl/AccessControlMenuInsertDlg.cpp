// AccessControlMenuInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlMenuInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlMenuInsertDlg dialog


CAccessControlMenuInsertDlg::CAccessControlMenuInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlMenuInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlMenuInsertDlg)
	m_sAlias = _T("");
	m_iIdx = 0;
	m_sCaption = _T("");
	m_sTarget = _T("");
	m_sTitle = _T("");
	//}}AFX_DATA_INIT

	pWnd = (CAccessControlWorkView *)pParent;
	MODE = 1;		// 입력모드는 1, 수정모드는 -1

	// KOL.UDF.022
	VISIBLE = ' ';
	STYLE = ' ';
	ACTION = ' ';
	FLAG = ' ';
	NAME = _T("");
}


VOID CAccessControlMenuInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlMenuInsertDlg)
	DDX_Text(pDX, IDC_eALIAS, m_sAlias);
	DDX_Text(pDX, IDC_eIDX, m_iIdx);
	DDX_Text(pDX, IDC_eCAPTION, m_sCaption);
	DDX_Text(pDX, IDC_eTARGET, m_sTarget);
	DDX_Text(pDX, IDC_eTITLE, m_sTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlMenuInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlMenuInsertDlg)
	ON_BN_CLICKED(IDCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDOK, OnbOK)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlMenuInsertDlg message handlers

VOID CAccessControlMenuInsertDlg::OnbOK() 
{
	UpdateData(true);
	CButton *radio1 = (CButton *)GetDlgItem(IDC_ACC_RADIO1);		// Visible
	CButton *radio2 = (CButton *)GetDlgItem(IDC_ACC_RADIO2);		// Invisible
	CButton *radio3 = (CButton *)GetDlgItem(IDC_ACC_RADIO3);		// String
	CButton *radio4 = (CButton *)GetDlgItem(IDC_ACC_RADIO4);		// Separator
	CButton *radio5 = (CButton *)GetDlgItem(IDC_ACC_RADIO5);		// AutoLoad
	CButton *radio6 = (CButton *)GetDlgItem(IDC_ACC_RADIO6);		// Load DLL
	CButton *radio7 = (CButton *)GetDlgItem(IDC_ACC_RADIO7);		// Sub Popup menu
	if (radio1->GetCheck()) VISIBLE = 'T';
		else VISIBLE = 'F';
	if (radio3->GetCheck()) STYLE = 'S';
	if (radio4->GetCheck()) STYLE = 'P';
	if (radio5->GetCheck()) STYLE = 'A';
	if (radio6->GetCheck()) ACTION = 'D';
		else ACTION = 'P';
	
	if (MODE == 1) {		// 입력 모드일 때만 중복 검사
		CString Qry,Count;
		Qry.Format(_T("SELECT COUNT(*) FROM ESL_MENU_TBL WHERE ALIAS = '%s' AND GROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c'"), m_sAlias, pWnd->MenuGroupAlias, NAME, FLAG);
			pWnd->m_pMenuDM->GetOneValueQuery(Qry,Count);

		if( 0 != _ttoi(Count.GetBuffer(0)))
		{
			AfxMessageBox(_T(" ALIAS 는 모든 메뉴 안에서 중목 될수 없습니다."));
			return;
		}
	}
	CDialog::OnOK();
}

VOID CAccessControlMenuInsertDlg::OnbCANCEL() 
{
	CDialog::OnCancel();
}

BOOL CAccessControlMenuInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	NewButtonList.Add(IDC_bOK, this, IDB_OK_U, IDB_OK_D, IDB_OK_F);
	NewButtonList.Add(IDC_bCANCEL, this, IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);

	// TODO: Add extra initialization here
	CButton *radio1 = (CButton *)GetDlgItem(IDC_ACC_RADIO1);		// Visible
	CButton *radio2 = (CButton *)GetDlgItem(IDC_ACC_RADIO2);		// Invisible
	CButton *radio3 = (CButton *)GetDlgItem(IDC_ACC_RADIO3);		// String
	CButton *radio4 = (CButton *)GetDlgItem(IDC_ACC_RADIO4);		// Separator
	CButton *radio5 = (CButton *)GetDlgItem(IDC_ACC_RADIO5);		// AutoLoad
	CButton *radio6 = (CButton *)GetDlgItem(IDC_ACC_RADIO6);		// Load DLL
	CButton *radio7 = (CButton *)GetDlgItem(IDC_ACC_RADIO7);		// Sub Popup menu
	if (MODE == 1) {		// 입력모드
		radio1->SetCheck(true);
		radio2->SetCheck(false);
		radio3->SetCheck(true);
		radio4->SetCheck(false);
		radio5->SetCheck(false);
		radio6->SetCheck(true);
		radio7->SetCheck(false);
		GetDlgItem(IDC_eALIAS)->EnableWindow(true);
	}
	if (MODE == -1) {		// 수정모드
		radio1->SetCheck(false);
		radio2->SetCheck(false);
		radio3->SetCheck(false);
		radio4->SetCheck(false);
		radio5->SetCheck(false);
		radio6->SetCheck(false);
		radio7->SetCheck(false);

		if (VISIBLE == 'T') radio1->SetCheck(true);
			else radio2->SetCheck(true);
		if (STYLE == 'S') radio3->SetCheck(true);
		if (STYLE == 'P') radio4->SetCheck(true);
		if (STYLE == 'A') radio5->SetCheck(true);
		if (ACTION == 'D') radio6->SetCheck(true);
			else radio7->SetCheck(true);
		GetDlgItem(IDC_eALIAS)->EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
