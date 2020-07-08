// MNG_USER_CHANGEPASSWORD.cpp : implementation file
//
#include "stdafx.h"
#include "MNG_USER_CHANGEPASSWORD.h"
#include "MNG_USER_MANAGER_03.h"
#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_CHANGEPASSWORD dialog


CMNG_USER_CHANGEPASSWORD::CMNG_USER_CHANGEPASSWORD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_USER_CHANGEPASSWORD::IDD, pParent)
{
	m_pParent = pParent;
	//{{AFX_DATA_INIT(CMNG_USER_CHANGEPASSWORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_USER_CHANGEPASSWORD::~CMNG_USER_CHANGEPASSWORD()
{
}


void CMNG_USER_CHANGEPASSWORD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_CHANGEPASSWORD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_CHANGEPASSWORD, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_CHANGEPASSWORD)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_CHANGEPASSWORD message handlers

BOOL CMNG_USER_CHANGEPASSWORD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMNG_USER_CHANGEPASSWORD::OnOK() 
{
	CString strMsg;
	CString strUserKey;
	CString strPassword;
	CString strPassword1;
	CString strPassword2;
	CString strQuery;
	INT ids;

	strUserKey = ((CMNG_USER_MANAGER_03*)m_pParent)->GetUpdatePK();
	((CEdit*)GetDlgItem(IDC_edtPASSWORD1))->GetWindowText(strPassword1);
	((CEdit*)GetDlgItem(IDC_edtPASSWORD2))->GetWindowText(strPassword2);
	
	if(_T("") == strPassword1 && _T("") == strPassword2)
	{
		strMsg = _T("비밀번호를 입력해주세요.");
		((CStatic*)GetDlgItem(IDC_staticINFO))->SetWindowText(strMsg);
		return;
	}
	if(strPassword1 != strPassword2)
	{
		strMsg = _T("비밀번호를 잘못 입력하셨습니다.");
		((CStatic*)GetDlgItem(IDC_staticINFO))->SetWindowText(strMsg);
		return;
	}
	ids = ((CMNG_USER_MANAGER_03*)m_pParent)->CheckPasswordCreateRule(strPassword1);
	if(0 > ids)
	{
		strMsg = _T("비밀번호는 영문, 숫자, 특수문자 중 2개이상을\r\n조합,10자리 이상을 사용해야 합니다.");
		((CStatic*)GetDlgItem(IDC_staticINFO))->SetWindowText(strMsg);
		return;
	}
	strPassword = strPassword1;
	if(m_pInfo->m_bUserPwEncryption)
	{
		strPassword = CLocCommonAPI::StringToSHA256(strPassword);
	}
	
	strPassword.MakeUpper();

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_03"));
	if(NULL == pDM)
	{
		AfxMessageBox( _T("FindDM Error!") );		
	}
	pDM->StartFrame();
	strQuery.Format(_T("UPDATE MN_USER_TBL SET PASSWD = '%s', PW_RENEWAL_DATE = SYSDATE WHERE REC_KEY = %s;"), strPassword, strUserKey);
	pDM->AddFrame(strQuery);
	pDM->SendFrame();
	pDM->EndFrame();

	((CMNG_USER_MANAGER_03*)m_pParent)->m_strCryptoPassword = strPassword;

	CDialog::OnOK();
}

BOOL CMNG_USER_CHANGEPASSWORD::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CMNG_USER_CHANGEPASSWORD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_staticINFO == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}
