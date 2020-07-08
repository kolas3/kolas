// RENEWAL_PASSWORD.cpp : implementation file
//

// 2012.09.26 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
#include "stdafx.h"
#include "kolas3_main.h"
#include "RENEWAL_PASSWORD.h"
#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRENEWAL_PASSWORD dialog


CRENEWAL_PASSWORD::CRENEWAL_PASSWORD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRENEWAL_PASSWORD::IDD, pParent)
{
	m_pParent = pParent;
	//{{AFX_DATA_INIT(CRENEWAL_PASSWORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRENEWAL_PASSWORD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRENEWAL_PASSWORD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRENEWAL_PASSWORD, CDialog)
	//{{AFX_MSG_MAP(CRENEWAL_PASSWORD)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRENEWAL_PASSWORD message handlers


BOOL CRENEWAL_PASSWORD::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	( (CEdit*)GetDlgItem(IDC_edtPASSWORD1) )->SetLimitText(20);
	( (CEdit*)GetDlgItem(IDC_edtPASSWORD2) )->SetLimitText(20);
	( (CEdit*)GetDlgItem(IDC_edtPASSWORD3) )->SetLimitText(20);

	// KOL.RED.2018.216 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 삭제
//	CString strMsg = _T("비밀번호 사용기간이 6개월이상 경과 되었습니다.\r\n개인정보의 안전한 보호를 위해 새로운 비밀번호로\r\n변경할 것을 권장합니다.");
//	((CEdit*)GetDlgItem(IDC_staticINFO1))->SetWindowText(strMsg);
//	strMsg = _T("사용자 확인을 위한 이전 비밀번호와 변경할 새로운 비밀번호를 입력해주세요.");
//	((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
	// KOL.RED.2018.216 ---------------------------------------------------------------------------	
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRENEWAL_PASSWORD::OnOK() 
{
	INT ids;
	CString strMsg;
	CString strQuery;
	CString strResult;
	CString strPassword1, strPassword2, strPassword3;
	CString strCryptoPassword1, strCryptoPassword2, strCryptoPassword3;

	((CEdit*)GetDlgItem(IDC_edtPASSWORD1))->GetWindowText(strPassword1);
	((CEdit*)GetDlgItem(IDC_edtPASSWORD2))->GetWindowText(strPassword2);
	((CEdit*)GetDlgItem(IDC_edtPASSWORD3))->GetWindowText(strPassword3);

	strCryptoPassword1 = EncryptionPassword(strPassword1);
	strCryptoPassword2 = EncryptionPassword(strPassword2);
	strCryptoPassword3 = EncryptionPassword(strPassword3);

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(this->m_pInfo->CONNECTION_INFO);
	// 입력항목 체크
	if(_T("") == strPassword1)
	{
		strMsg = (_T("이전 비밀번호를 입력해주세요."));
		((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
		return;
	}
	else if(_T("") == strPassword2 || _T("") == strPassword3)
	{
		strMsg = (_T("새로운 비밀번호를 입력해주세요."));
		((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
		return;
	}

	// 이전비밀번호 일치여부
	strQuery.Format(_T("SELECT PASSWD FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK);
	ids = dm.GetOneValueQuery(strQuery, strResult);
	if (ids < 0) 
	{
		AfxMessageBox(_T("접속정보를 확인해주세요."));
		return;
	}
	if(strResult != strCryptoPassword1)
	{
		strMsg = (_T("이전 비밀번호가 일치하지 않습니다."));
		((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
		return;
	}

	// 새로운 비밀번호 변경여부
	if(strPassword2 == strPassword3)
	{
		if(strCryptoPassword1 == strCryptoPassword2)
		{
			strMsg = (_T("이전 비밀번호와 다른 비밀번호를 사용해주세요."));
			((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
			return;
		}
		//비밀번호 생성규칙
		INT ids1 = CLocCommonAPI::CheckUpperEnglish(strPassword2);
		INT ids2 = CLocCommonAPI::CheckNumber(strPassword2);
		INT ids3 = CLocCommonAPI::CheckSpecial(strPassword2);
		if(10 > strPassword2.GetLength() || 2 > (ids1+ids2+ids3))
		{
			strMsg = (_T("비밀번호는 영문, 숫자, 특수문자 중 2개이상을\r\n조합,10자리 이상을 사용해야 합니다."));
			((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
			return;
		}

		//DB에 저장
		dm.StartFrame();
		strQuery.Format(_T("UPDATE MN_USER_TBL SET PASSWD = '%s', PW_RENEWAL_DATE = SYSDATE WHERE REC_KEY = %s;"), strCryptoPassword2, this->m_pInfo->USER_PK);
		dm.AddFrame(strQuery);
		dm.SendFrame();
		dm.EndFrame();
	}
	else
	{
		strMsg = (_T("새로운 비밀번호를 잘못 입력하셨습니다."));
		((CStatic*)GetDlgItem(IDC_staticINFO2))->SetWindowText(strMsg);
		return;
	}
	
	CDialog::OnOK();
}

BOOL CRENEWAL_PASSWORD::PreTranslateMessage(MSG* pMsg) 
{
	// KOL.RED.2018.216 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*
	if( VK_RETURN == pMsg->wParam )
	{
		OnOK();
	}
	*/
	if		(WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		OnOK();
	}
	else if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)
	{
		// 강제 종료가 되지 않도록 처리
		return TRUE;
	}
	else if (WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		// 강제 종료가 되지 않도록 처리
		return TRUE;
	}
	// KOL.RED.2018.216 ---------------------------------------------------------------------------
	
	return CDialog::PreTranslateMessage(pMsg);
}


CString CRENEWAL_PASSWORD::EncryptionPassword(CString strPassword) 
{
	CString strResult;

	if(this->m_pInfo->m_bUserPwEncryption)
	{
		strResult = CLocCommonAPI::StringToSHA256(strPassword);
	}
	else
	{
		strResult = strPassword;
	}
	return strResult;
}

HBRUSH CRENEWAL_PASSWORD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// KOL.RED.2018.216 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*
	if(IDC_staticINFO2 == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }
	*/
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_staticINFO2:
		{
			pDC->SetTextColor(RGB(0,70,213));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	case IDC_STATIC_LAW:
		{
			pDC->SetTextColor(RGB(255,0,50));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	case IDC_STATIC_LAW2:
//	case IDC_STATIC_LAW3:
//	case IDC_STATIC_LAW4:
//	case IDC_STATIC_LAW5:
		{
			pDC->SetTextColor(RGB(150,0,30));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	}
	// KOL.RED.2018.216 ---------------------------------------------------------------------------

	return hbr;
}
