// MNG_LIB_HOLIDAY_MGR_04.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIB_HOLIDAY_MGR_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_04 dialog


CMNG_LIB_HOLIDAY_MGR_04::CMNG_LIB_HOLIDAY_MGR_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_LIB_HOLIDAY_MGR_04::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOLIDAY_MGR_04)
	m_strDesc = _T("");
	//}}AFX_DATA_INIT

	m_nMonth = 1;
	m_nDay = 1;

	MODE = _T("INSERT");
}
CMNG_LIB_HOLIDAY_MGR_04::~CMNG_LIB_HOLIDAY_MGR_04()
{
	
}


VOID CMNG_LIB_HOLIDAY_MGR_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOLIDAY_MGR_04)
	DDX_Control(pDX, IDC_04_cMONTH, m_cMonth);
	DDX_Control(pDX, IDC_04_cDAY, m_cDay);
	DDX_Text(pDX, IDC_04_eDESC, m_strDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOLIDAY_MGR_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOLIDAY_MGR_04)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_04_cMONTH, OnSelchange04cMONTH)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_04 message handlers

BOOL CMNG_LIB_HOLIDAY_MGR_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitComboBox();
	CWnd *pWnd = GetDlgItem(IDC_bAPPLY);
	if (MODE == _T("INSERT")) {
		SetWindowText(_T("공휴일 입력"));
		if (pWnd) pWnd->SetWindowText(_T("입력"));
	}
	if (MODE == _T("MODIFY")) {
		SetWindowText(_T("공휴일 수정"));
		if (pWnd) pWnd->SetWindowText(_T("수정"));
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_LIB_HOLIDAY_MGR_04::InitComboBox(VOID)
{
	INT i;
	CString strMsg;

	m_cMonth.ResetContent();
	for (i = 1; i <= 12; i++) {
		strMsg.Format(_T("%d"), i);
		m_cMonth.AddString(strMsg);
		if (m_nMonth == i) m_cMonth.SetCurSel(i-1);
	}

	m_cDay.ResetContent();
	for (i = 1; i <= GetMaxDayofMonth(m_nMonth); i++) {
		strMsg.Format(_T("%d"), i);
		m_cDay.AddString(strMsg);
		if (m_nDay == i) m_cDay.SetCurSel(i-1);
	}

	return 0;
}

VOID CMNG_LIB_HOLIDAY_MGR_04::OnbAPPLY() 
{
	UpdateData(TRUE);
	CString str;
	m_cMonth.GetWindowText(str);
	m_nMonth = _ttoi(str);

	m_cDay.GetWindowText(str);
	m_nDay = _ttoi(str);

	CDialog::OnOK();
}

VOID CMNG_LIB_HOLIDAY_MGR_04::OnbCLOSE() 
{
	CDialog::OnCancel();	
}

INT CMNG_LIB_HOLIDAY_MGR_04::GetMaxDayofMonth(INT nMonth)
{
	if (nMonth < 1 || nMonth > 12) return -1;
    INT max_day[]= { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	return max_day[nMonth-1];
}

VOID CMNG_LIB_HOLIDAY_MGR_04::OnSelchange04cMONTH() 
{
	INT i;
	CString strMonth, strMsg;
	UpdateData(TRUE);
	m_cMonth.GetWindowText(strMonth);
	strMonth.TrimLeft();		strMonth.TrimRight();
	if (strMonth.GetLength() > 0) {
		CString strDay;
		m_cDay.GetWindowText(strDay);
		strDay.TrimLeft();		strDay.TrimRight();
		INT nDay = 1;
		if (strDay.GetLength() > 0) nDay = _ttoi(strDay);
		INT nMonth = _ttoi(strMonth);
		m_cDay.ResetContent();
		INT nSel = 0;
		for (i = 1; i <= GetMaxDayofMonth(nMonth); i++) {
			strMsg.Format(_T("%d"), i);
			m_cDay.AddString(strMsg);
			if (i == nDay) nSel = i-1;
		}
		m_cDay.SetCurSel(nSel);		
	}
}



HBRUSH CMNG_LIB_HOLIDAY_MGR_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}