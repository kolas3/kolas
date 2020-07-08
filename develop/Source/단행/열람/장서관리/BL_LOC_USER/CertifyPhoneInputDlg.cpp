#include "Stdafx.h"
#include "CertifyPhoneInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CERTIFY_PHONE_INPUT_TIME_CHK	100
#define CERTIFY_PHONE_NUMBER_SIZE		6

CCertifyPhoneInputDlg::CCertifyPhoneInputDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCertifyPhoneInputDlg::IDD, pParent)
{
	
	m_nLimitTimer = 180;
}

CCertifyPhoneInputDlg::~CCertifyPhoneInputDlg()
{
}

void CCertifyPhoneInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertifyPhoneInputDlg)
	DDX_Control(pDX, IDC_EDT_CERTIFYNUM, m_edtCertifyNumber);
	 //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCertifyPhoneInputDlg, CDialog)
	//{{AFX_MSG_MAP(CCertifyPhoneInputDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDT_CERTIFYNUM, OnChangeCertifyNum)
	ON_BN_CLICKED(IDC_BTN_INPUT, OnbtnInput)	
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()




BOOL CCertifyPhoneInputDlg::PreTranslateMessage(MSG* pMsg) 
{	
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(pMsg->hwnd != GetDlgItem(IDCANCEL)->GetSafeHwnd())
		{
			OnbtnInput();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCertifyPhoneInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	m_edtCertifyNumber.SetLimitText(CERTIFY_PHONE_NUMBER_SIZE);

	SetTimer(CERTIFY_PHONE_INPUT_TIME_CHK, 1000, 0);
	
	return TRUE; 
}

HBRUSH CCertifyPhoneInputDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_STATIC_TIMER:
		{
			pDC->SetTextColor(RGB(255,10,10));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	}

	return hbr;
}

VOID CCertifyPhoneInputDlg::OnTimer(UINT nIDEvent) 
{	
	switch (nIDEvent)
	{
	case CERTIFY_PHONE_INPUT_TIME_CHK:
		{
			m_nLimitTimer--;

			if (m_nLimitTimer == 0)
			{
				KillTimer(CERTIFY_PHONE_INPUT_TIME_CHK);
				GetDlgItem(IDC_STATIC_TIMER)->SetWindowText(_T("시간경과"));
				GetDlgItem(IDC_BTN_INPUT)->EnableWindow(FALSE);
			}
			else
			{
				CString strTimer;
				strTimer.Format(_T("%d분%02d초"), m_nLimitTimer/60, m_nLimitTimer%60);
				GetDlgItem(IDC_STATIC_TIMER)->SetWindowText(strTimer);
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

VOID CCertifyPhoneInputDlg::OnChangeCertifyNum() 
{
	CString strCertifyNumber;
	m_edtCertifyNumber.GetWindowText(strCertifyNumber);

	GetDlgItem(IDC_BTN_INPUT)->EnableWindow( strCertifyNumber.GetLength() == CERTIFY_PHONE_NUMBER_SIZE );
}

VOID CCertifyPhoneInputDlg::OnbtnInput()
{
	m_edtCertifyNumber.GetWindowText(m_strCertifyNumber);

	if (m_strCertifyNumber.IsEmpty() || m_strCertifyNumber.GetLength() < CERTIFY_PHONE_NUMBER_SIZE)
	{
		GetDlgItem(IDC_STATIC_MYPIN_MSG)->SetWindowText(_T("6자리 인증번호를 입력해주시기 바랍니다."));
		m_edtCertifyNumber.SetFocus();
		return;
	}

	KillTimer(CERTIFY_PHONE_INPUT_TIME_CHK);

	CDialog::OnOK();
}