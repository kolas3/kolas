// CheckinVolMoreExp.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolMoreExp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolMoreExp dialog


CCheckinVolMoreExp::CCheckinVolMoreExp(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinVolMoreExp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolMoreExp)
	m_strMoreExpCnt = _T("");
	//}}AFX_DATA_INIT
}

CCheckinVolMoreExp::~CCheckinVolMoreExp()
{
}


VOID CCheckinVolMoreExp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolMoreExp)
	DDX_Text(pDX, IDC_edtCHECKIN_VOL_MORE_EXP_CNT, m_strMoreExpCnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolMoreExp, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolMoreExp)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolMoreExp message handlers

VOID CCheckinVolMoreExp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_strMoreExpCnt.IsEmpty()) {
		AfxMessageBox(_T("추가 예측 건수가 입력되지 않았습니다!"));
		return;
	}

	INT nCnt = _ttoi(m_strMoreExpCnt.GetBuffer(0));
	

	if(nCnt<1) {
		AfxMessageBox(_T("추가 예측 건수가 잘 못 입력되었습니다!"));
		return;
	}


	CDialog::OnOK();
}

HBRUSH CCheckinVolMoreExp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinVolMoreExp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
