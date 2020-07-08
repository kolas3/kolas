// SeExpectGetPatternName.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectGetPatternName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectGetPatternName dialog


CSeExpectGetPatternName::CSeExpectGetPatternName(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectGetPatternName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectGetPatternName)
	m_strPATTERN_NAME = _T("");
	//}}AFX_DATA_INIT
}


CSeExpectGetPatternName::~CSeExpectGetPatternName()
{
}


VOID CSeExpectGetPatternName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectGetPatternName)
	DDX_Text(pDX, IDC_edtEXP_PATTERN_NAME_GETTER, m_strPATTERN_NAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectGetPatternName, CDialog)
	//{{AFX_MSG_MAP(CSeExpectGetPatternName)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectGetPatternName message handlers

VOID CSeExpectGetPatternName::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_strPATTERN_NAME.IsEmpty()) {
		AfxMessageBox(_T("패턴명이 입력되지 않았습니다!"));
		return;
	}

	CDialog::OnOK();
}

HBRUSH CSeExpectGetPatternName::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSeExpectGetPatternName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
