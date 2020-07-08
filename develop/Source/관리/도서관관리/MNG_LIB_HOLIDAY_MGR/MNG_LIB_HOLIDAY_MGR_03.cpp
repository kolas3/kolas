// MNG_LIB_HOLIDAY_MGR_03.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIB_HOLIDAY_MGR_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_03 dialog


CMNG_LIB_HOLIDAY_MGR_03::CMNG_LIB_HOLIDAY_MGR_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOLIDAY_MGR_03)
	//}}AFX_DATA_INIT
}

CMNG_LIB_HOLIDAY_MGR_03::~CMNG_LIB_HOLIDAY_MGR_03()
{
	
}



VOID CMNG_LIB_HOLIDAY_MGR_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOLIDAY_MGR_03)
	DDX_Control(pDX, IDC_cWEEK, m_cWeek);
	DDX_Control(pDX, IDC_cPERWEEK, m_cPerWeek);
	DDX_Control(pDX, IDC_cSTARTMONTH, m_cStartMonth);
	DDX_Control(pDX, IDC_cPERMONTH, m_cPerMonth);
	DDX_Control(pDX, IDC_cSTARTWEEK, m_cStartWeek);
	DDX_Control(pDX, IDC_cDAY, m_cDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOLIDAY_MGR_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOLIDAY_MGR_03)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_03 message handlers

VOID CMNG_LIB_HOLIDAY_MGR_03::OnbAPPLY() 
{
	UpdateData(TRUE);

	MODE = _T("MONTH");
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	if (pButton1) {
		if (pButton1->GetCheck() == TRUE) {
			MODE = _T("WEEK");
		}
	}

	CString str;
	if (MODE == _T("WEEK")) {
		m_cStartWeek.GetWindowText(str);
		m_nStartWeek = _ttoi(str);

		m_cPerWeek.GetWindowText(str);
		m_nPerWeek = _ttoi(str);

		m_cWeek.GetWindowText(str);
		if (str == _T("일요일")) m_nWeek = 1;
		if (str == _T("월요일")) m_nWeek = 2;
		if (str == _T("화요일")) m_nWeek = 3;
		if (str == _T("수요일")) m_nWeek = 4;
		if (str == _T("목요일")) m_nWeek = 5;
		if (str == _T("금요일")) m_nWeek = 6;
		if (str == _T("토요일")) m_nWeek = 7;
	}
	if (MODE == _T("MONTH")) {
		m_cStartMonth.GetWindowText(str);
		m_nStartMonth = _ttoi(str);

		m_cPerMonth.GetWindowText(str);
		m_nPerMonth = _ttoi(str);

		m_cDay.GetWindowText(str);
		m_nDay = _ttoi(str);
	}
	
	CDialog::OnOK();
}

VOID CMNG_LIB_HOLIDAY_MGR_03::OnbCLOSE() 
{
	UpdateData(TRUE);
	CDialog::OnCancel();
}

BOOL CMNG_LIB_HOLIDAY_MGR_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitComboBox();
	UpdateData(FALSE);

	MODE = _T("WEEK");
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	if (pButton1) pButton1->SetCheck(TRUE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_LIB_HOLIDAY_MGR_03::InitComboBox(VOID)
{	
	INT i;
	CString strMsg;

	m_cStartWeek.ResetContent();
	for (i = 1; i <= 52; i++) {
		strMsg.Format(_T("%d"), i);
		m_cStartWeek.AddString(strMsg);
	}
	m_cStartWeek.SetCurSel(0);

	m_cPerWeek.ResetContent();
	for (i = 1; i <= 52; i++) {
		strMsg.Format(_T("%d"), i);
		m_cPerWeek.AddString(strMsg);
	}
	m_cPerWeek.SetCurSel(0);

	m_cWeek.ResetContent();
	m_cWeek.AddString(_T("일요일"));
	m_cWeek.AddString(_T("월요일"));
	m_cWeek.AddString(_T("화요일"));
	m_cWeek.AddString(_T("수요일"));
	m_cWeek.AddString(_T("목요일"));
	m_cWeek.AddString(_T("금요일"));
	m_cWeek.AddString(_T("토요일"));
	m_cWeek.SetCurSel(0);



	m_cStartMonth.ResetContent();
	for (i = 1; i <= 12; i++) {
		strMsg.Format(_T("%d"), i);
		m_cStartMonth.AddString(strMsg);		
	}
	m_cStartMonth.SetCurSel(0);

	m_cPerMonth.ResetContent();
	for (i = 1; i <= 12; i++) {
		strMsg.Format(_T("%d"), i);
		m_cPerMonth.AddString(strMsg);		
	}
	m_cPerMonth.SetCurSel(0);

	m_cDay.ResetContent();
	for (i = 1; i <= 31; i++) {
		strMsg.Format(_T("%d"), i);
		m_cDay.AddString(strMsg);
	}
	m_cDay.SetCurSel(0);

	return 0;
}

HBRUSH CMNG_LIB_HOLIDAY_MGR_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
