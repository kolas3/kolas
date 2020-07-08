// PaymentRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PaymentRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaymentRunDlg dialog


CPaymentRunDlg::CPaymentRunDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPaymentRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaymentRunDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPayment = 0;
}
CPaymentRunDlg::~CPaymentRunDlg()
{
}

void CPaymentRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaymentRunDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaymentRunDlg, CDialog)
	//{{AFX_MSG_MAP(CPaymentRunDlg)
	ON_BN_CLICKED(IDC_btnPR_YES, OnbtnPRYES)
	ON_BN_CLICKED(IDC_btnPR_NO, OnbtnPRNO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaymentRunDlg message handlers

BOOL CPaymentRunDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	CString strValue;
	strValue.Format(_T("%d"), m_nPayment);
	GetDlgItem(IDC_edtPR_PAYMENT)->SetWindowText(strValue);

	if(_T("S") == m_pInfo->USER_PRIVILEGE)
	{
		((CButton*)GetDlgItem(IDC_edtPR_PAYMENT))->EnableWindow(TRUE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaymentRunDlg::OnbtnPRYES() 
{
	CString strValue;
	GetDlgItem(IDC_edtPR_PAYMENT)->GetWindowText(strValue);
	if(strValue.IsEmpty())
	{
		MessageBox(_T("납입금액을 입력해 주십시오."), _T("납입처리"));
	}
	if(1 > _ttoi(strValue))
	{
		MessageBox(_T("납입금액을 입력해 주십시오."), _T("납입처리"));
	}
	m_nPayment = _ttoi(strValue);
	OnOK();
}

void CPaymentRunDlg::OnbtnPRNO() 
{
	OnCancel();
}

BOOL CPaymentRunDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnPRNO();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
