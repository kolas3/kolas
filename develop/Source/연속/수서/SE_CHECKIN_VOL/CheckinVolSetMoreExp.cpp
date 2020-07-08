// CheckinVolSetMoreExp.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolSetMoreExp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSetMoreExp dialog


CCheckinVolSetMoreExp::CCheckinVolSetMoreExp(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinVolSetMoreExp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolSetMoreExp)
	m_strMORE_EXP_NO = _T("");
	m_bAUTO_USE_YN = FALSE;
	m_bDEFAULT_USE_YN = FALSE;
	m_strDEFAULT_EXP_NO = _T("");
	//}}AFX_DATA_INIT

	//color ref init
	CR_exp = RGB(255,255,255);
	CR_acq = RGB(255,255,255);
	CR_lack = RGB(255,255,255);
	CR_miss = RGB(255,255,255);
	CR_delay = RGB(255,255,255);
	CR_nonpub = RGB(255,255,255);

}

CCheckinVolSetMoreExp::~CCheckinVolSetMoreExp()
{
}


VOID CCheckinVolSetMoreExp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolSetMoreExp)
	DDX_Text(pDX, IDC_edtCHECKIN_VOL_SET_MORE_EXP_NO, m_strMORE_EXP_NO);
	DDX_Check(pDX, IDC_chkCHECKIN_VOL_SET_MORE_EXP_YN, m_bAUTO_USE_YN);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR1, m_bCH_exp);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR2, m_bCH_acq);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR3, m_bCH_lack);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR4, m_bCH_miss);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR5, m_bCH_delay);
	DDX_Control(pDX, IDC_btnCHECKIN_VOL_SET_COLOR6, m_bCH_nonpub);
	DDX_Check(pDX, IDC_chkCHECKIN_VOL_SET_DEFAULT_MORE_EXP_YN, m_bDEFAULT_USE_YN);
	DDX_Text(pDX, IDC_edtCHECKIN_VOL_SET_DEFAULT_MORE_EXP_NO, m_strDEFAULT_EXP_NO);
	//}}AFX_DATA_MAP

	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR1, CR_exp);
	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR2, CR_acq);
	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR3, CR_lack);
	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR4, CR_miss);
	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR5, CR_delay);
	DDX_ColorPicker(pDX, IDC_btnCHECKIN_VOL_SET_COLOR6, CR_nonpub);

}


BEGIN_MESSAGE_MAP(CCheckinVolSetMoreExp, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolSetMoreExp)
	ON_BN_CLICKED(IDC_chkCHECKIN_VOL_SET_MORE_EXP_YN, OnchkCHECKINVOLSETMOREEXPYN)
	ON_BN_CLICKED(IDC_chkCHECKIN_VOL_SET_DEFAULT_MORE_EXP_YN, OnchkCHECKINVOLSETDEFAULTMOREEXPYN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSetMoreExp message handlers

BOOL CCheckinVolSetMoreExp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	//기본 추가 예측 설정
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCHECKIN_VOL_SET_DEFAULT_MORE_EXP_NO); 

	if(pEdit)
	{
		if(!m_bDEFAULT_USE_YN) pEdit->SetReadOnly();
	}


	//추가 예측 설정
	pEdit = (CEdit*)GetDlgItem(IDC_edtCHECKIN_VOL_SET_MORE_EXP_NO); 
	
	if(pEdit)
	{
		if(!m_bAUTO_USE_YN) pEdit->SetReadOnly();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCheckinVolSetMoreExp::OnchkCHECKINVOLSETMOREEXPYN() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCHECKIN_VOL_SET_MORE_EXP_NO); 
	
	if(pEdit)
	{
		if(m_bAUTO_USE_YN)
		{
			pEdit->SetReadOnly(FALSE);
		}
		else
		{
			pEdit->SetReadOnly();
		}
	}


}

VOID CCheckinVolSetMoreExp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_bDEFAULT_USE_YN)
	{
		if(m_strDEFAULT_EXP_NO.IsEmpty()) 
		{
			AfxMessageBox(_T("기본 추가 예측 건수가 입력되지 않았습니다!"));
			return;
		}
	}

	CDialog::OnOK();
}

VOID CCheckinVolSetMoreExp::OnchkCHECKINVOLSETDEFAULTMOREEXPYN() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCHECKIN_VOL_SET_DEFAULT_MORE_EXP_NO); 
	
	if(pEdit)
	{
		if(m_bDEFAULT_USE_YN)
		{
			pEdit->SetReadOnly(FALSE);
		}
		else
		{
			pEdit->SetReadOnly();
		}
	}

}

HBRUSH CCheckinVolSetMoreExp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
