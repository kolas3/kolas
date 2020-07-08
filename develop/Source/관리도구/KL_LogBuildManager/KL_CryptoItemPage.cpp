// KL_CryptoItemPage.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoItemPage.h"
#include "KL_CryptoInfo.h"

#include "ESL_Mgr.h"

#define TIMER_ANALYZE_LIBRARYINFO	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKL_CryptoInfo* theInfo;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoItemPage dialog
IMPLEMENT_DYNAMIC(CKL_CryptoItemPage, CPropertyPage)

CKL_CryptoItemPage::CKL_CryptoItemPage()
	: CPropertyPage(CKL_CryptoItemPage::IDD)
{
	m_bWarnning = FALSE;
}


void CKL_CryptoItemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoItemPage)
	DDX_Control(pDX, IDC_EDT_KOLAS_USER_RESULT, m_edtManagerResult);
	DDX_Control(pDX, IDC_EDT_LOAN_USER_RESULT, m_edtUserResult);
	DDX_Control(pDX, IDC_CHK_MANAGER_PASSWD, m_btnManagerPasswd);
	DDX_Control(pDX, IDC_CHK_USER_PASSWD, m_btnUserPasswd);
	DDX_Control(pDX, IDC_CHK_CARD_PASSWD, m_btnCardPasswd);	
	//}}AFX_DATA_MAP
}

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoItemPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

#ifdef CRYPTO_DEV
	// 최초 설정값을 체크상태로 만든다.
	m_btnManagerPasswd.SetCheck(TRUE);
	m_btnUserPasswd.SetCheck(TRUE);
	m_btnCardPasswd.SetCheck(TRUE);
	// 체크값을 반영하기 위해서 호출
	OnChkOption();
#endif
	return TRUE;
}

BEGIN_MESSAGE_MAP(CKL_CryptoItemPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKL_CryptoItemPage)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_MANAGER_PASSWD, OnChkOption)
	ON_BN_CLICKED(IDC_CHK_USER_PASSWD, OnChkOption)
	ON_BN_CLICKED(IDC_CHK_CARD_PASSWD, OnChkOption)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoItemPage message handlers

/*=============================================================
	@ Function    :	OnShowWindow
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		SetTimer(TIMER_ANALYZE_LIBRARYINFO, 100, NULL);
	}
}

/*=============================================================
	@ Function    :	OnTimer
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_ANALYZE_LIBRARYINFO:
		KillTimer(TIMER_ANALYZE_LIBRARYINFO);
		
		if (AnalyzeLibraryInfo())
		{
			m_bWarnning = FALSE;
			GetDlgItem(IDC_STATIC_INFO_STATUS)->SetWindowText(_T("도서관 DB정보 분석이 완료되었습니다"));			
		}
		else
		{
			m_bWarnning = TRUE;
			GetDlgItem(IDC_STATIC_INFO_STATUS)->SetWindowText(_T("도서관 DB정보를 분석하는 중에 문제가 발생했습니다!"));			
		}

		if (theInfo->GetCryptoDisableOption() == CRYPTO_OPT_ALL)
		{
			GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(_T("모든 항목들의 암호화 작업이 완료된 상태입니다"));			
		}
		else
		{
			GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(_T("암호화 항목을 선택해 주십시오"));
		}
		break;
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

/*=============================================================
	@ Function    :	AnalyzeLibraryInfo
	@ Description :
=============================================================*/
BOOL CKL_CryptoItemPage::AnalyzeLibraryInfo()
{	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	// 코라스3 사용자 수 확인
	CString strResult;
	int ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM MN_USER_TBL"), strResult);
	if (ids < 0)
	{
		// 접속 설정을 다시 해야한다.
		return FALSE;
	}
	// 결과를 에디트컨트롤에 반영
	m_edtManagerResult.SetWindowText(strResult);
	// 실행계획을 세우기 위해서 정보 저장.
	theInfo->SetUserCount((UINT)_ttoi(strResult));

	// 대출/반납 이용자 수 확인
	ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL"), strResult);
	if (ids < 0)
	{
		// 원인이??
		return FALSE;
	}
	// 결과를 에디트컨트롤에 반영
	m_edtUserResult.SetWindowText(strResult);
	// 실행계획을 세우기 위해서 정보 저장.
	theInfo->SetLoanUserCount((UINT)_ttoi(strResult));

	// 암호화를 적용한 항목이 있는지 확인
	{
		UINT nDisableOpt = 0;

		// 사용자 암호화 유무
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'USERPWENCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'USERPWENCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// 원인이??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnManagerPasswd.SetCheck(FALSE);
			m_btnManagerPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_USER_PASSWD;
		}

		// 이용자 암호화 유무
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'VISITORPWENCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'VISITORPWENCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// 원인이??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnUserPasswd.SetCheck(FALSE);
			m_btnUserPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_LOAN_USER_PASSWD;
		}
		
		// 회원증 암호화 유무
		strResult.Empty();
#ifdef CRYPTO_DEV
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN_TEST FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'CARDPWCRYPTION\'"), strResult);
#else
		ids = dm.GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE=\'CARDPWCRYPTION\'"), strResult);
#endif
		if (ids < 0 && ids != -4007)
		{
			// 원인이??
			return FALSE;
		}
		if (strResult.CompareNoCase(_T("Y")) == 0)
		{
			m_btnCardPasswd.SetCheck(FALSE);
			m_btnCardPasswd.EnableWindow(FALSE);

			nDisableOpt += (UINT)CRYPTO_OPT_LOAN_USER_CARD;
		}

		theInfo->SetCryptoDisableOption(nDisableOpt);

		// 변경된 내용이 있다면 적용
		OnChkOption();
	}
	
	
	return TRUE;
}

/*=============================================================
	@ Function    :	OnChkOption
	@ Description :
=============================================================*/
void CKL_CryptoItemPage::OnChkOption() 
{
	UINT nOpt = 0;
	
	if (m_btnManagerPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_USER_PASSWD;
	}
	if (m_btnUserPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_LOAN_USER_PASSWD;
	}
	if (m_btnCardPasswd.GetCheck())
	{
		nOpt += (UINT)CRYPTO_OPT_LOAN_USER_CARD;
	}

	theInfo->SetCryptoOption(nOpt);
}

/*=============================================================
	@ Function    :	OnCtlColor
	@ Description :
=============================================================*/
HBRUSH CKL_CryptoItemPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_INFO_STATUS:
		if (m_bWarnning)
		{
			pDC->SetTextColor(RGB(0xFF, 0x00, 0x00));
		}
		else
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
		}
		break;
	case IDC_STATIC_ITEM:
		if (theInfo->GetCryptoDisableOption() == CRYPTO_OPT_ALL)
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0xFF));
		}
		else
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
		}
		break;
	}

	return hbr;
}
