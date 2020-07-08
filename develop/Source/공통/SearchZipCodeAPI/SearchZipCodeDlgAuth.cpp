#include "Stdafx.h"
#include "SearchZipCodeDlgAuth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSearchZipCodeDlgAuth Dialog

CSearchZipCodeDlgAuth::CSearchZipCodeDlgAuth(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
}

CSearchZipCodeDlgAuth::~CSearchZipCodeDlgAuth()
{
}

void CSearchZipCodeDlgAuth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCodeDlgAuth)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchZipCodeDlgAuth, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCodeDlgAuth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgAuth message handlers


BOOL CSearchZipCodeDlgAuth::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSearchZipCodeDlgAuth::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	ASSERT(GetDlgItem(IDC_EDT_AUTH));
	GetDlgItem(IDC_EDT_AUTH)->SetWindowText(m_strAuthKey);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSearchZipCodeDlgAuth::OnOK()
{
	ASSERT(GetDlgItem(IDC_EDT_AUTH));
	GetDlgItem(IDC_EDT_AUTH)->GetWindowText(m_strAuthKey);

	if (!m_strAuthKey.IsEmpty())
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(_T("인증키를 입력하십시오"), MB_ICONINFORMATION);
	}
}
