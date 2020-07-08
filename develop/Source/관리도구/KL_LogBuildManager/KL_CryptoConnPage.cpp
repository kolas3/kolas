// KL_CryptoConnPage.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoConnPage.h"
#include "KL_CryptoInfo.h"

#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKL_CryptoInfo* theInfo;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoConnPage dialog
IMPLEMENT_DYNAMIC(CKL_CryptoConnPage, CPropertyPage)

CKL_CryptoConnPage::CKL_CryptoConnPage()
	: CPropertyPage(CKL_CryptoConnPage::IDD)
{
	m_bConnect = FALSE;
}


void CKL_CryptoConnPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoConnPage)	
	DDX_Control(pDX, IDC_EDT_RESULT, m_edtResult);
	DDX_Control(pDX, IDC_EDT_USER, m_edtUser);
	DDX_Control(pDX, IDC_EDT_PASS, m_edtPass);
	DDX_Control(pDX, IDC_EDT_SID, m_edtSID);
	DDX_Control(pDX, IDC_EDT_KOLAS_USER, m_edtKolasUser);
	DDX_Control(pDX, IDC_EDT_KOLAS_USERID, m_edtKolasUserID);
	DDX_Control(pDX, IDC_BTN_TEST, m_btnConnTest);
	//}}AFX_DATA_MAP
}

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoConnPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	InitResultControl();

#ifdef CRYPTO_DEV
	m_edtUser.SetWindowText(_T("KULM"));
	m_edtPass.SetWindowText(_T("KULM#"));
	m_edtSID.SetWindowText(_T("NL_LOC_BUSAN"));
	m_edtKolasUser.SetWindowText(_T("KOLAS1"));
	m_btnConnTest.EnableWindow(TRUE);
#endif 

	return TRUE;
}

BEGIN_MESSAGE_MAP(CKL_CryptoConnPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKL_CryptoConnPage)
	ON_EN_CHANGE(IDC_EDT_USER, OnChangeEdtUser)
	ON_EN_CHANGE(IDC_EDT_PASS, OnChangeEdtPass)
	ON_EN_CHANGE(IDC_EDT_SID, OnChangeEdtSid)
	ON_EN_CHANGE(IDC_EDT_KOLAS_USER, OnChangeEdtKolasUser)
	ON_EN_CHANGE(IDC_EDT_KOLAS_USERID, OnChangeEdtKolasUserID)
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnConnTest)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoConnPage message handlers

/*=============================================================
	@ Function    :	OnChangeEdtUser
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnChangeEdtUser() 
{
	m_btnConnTest.EnableWindow( IsConnectInput() );
}

/*=============================================================
	@ Function    :	OnChangeEdtPass
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnChangeEdtPass() 
{
	m_btnConnTest.EnableWindow( IsConnectInput() );
}

/*=============================================================
	@ Function    :	OnChangeEdtSid
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnChangeEdtSid() 
{
	m_btnConnTest.EnableWindow( IsConnectInput() );
}

/*=============================================================
	@ Function    :	OnChangeEdtKolasUser
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnChangeEdtKolasUser() 
{
	m_btnConnTest.EnableWindow( IsConnectInput() );
}

/*=============================================================
	@ Function    :	OnChangeEdtKolasUserID
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnChangeEdtKolasUserID() 
{
	m_btnConnTest.EnableWindow( IsConnectInput() );
}


/*=============================================================
	@ Function    :	OnBtnConnTest
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::OnBtnConnTest() 
{
	CString strConnectInfo = MakeConnectInfo();
	CString strKolasUserInfo;
	CString strKolasUserID;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(strConnectInfo);

	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT SYSDATE FROM DUAL"));
	int ids = dm.GetOneValueQuery(strQuery, strResult);
	if (ids > 0)
	{		
		m_edtKolasUser.GetWindowText(strKolasUserInfo);
		strQuery.Format(_T("SELECT COUNT(1) FROM %s.CO_LOAN_USER_TBL"), strKolasUserInfo);
		// KOLAS쪽 디비를 확인한다
		ids = dm.GetOneValueQuery(strQuery, strResult);	
		if (ids > 0)
		{
			// KOLAS의 사용자 정보를 구성한다
			m_edtKolasUserID.GetWindowText(strKolasUserID);
			strQuery.Format(_T("SELECT M.NAME,M.PERMITION,M.REC_KEY FROM %s.MN_USER_TBL M WHERE M.ID=\'%s\'"), strKolasUserInfo, strKolasUserID);

			ids = dm.RestructDataManager(strQuery);
			if (dm.GetRowCount() != 1) ids = -1001;

			if (ids >= 0)
			{
				// 디비 접속을 성공했다.
				m_edtResult.SetWindowText(_T("접속성공"));
				// 결과를 보관
				theInfo->SetConnectInfo(strConnectInfo);
				theInfo->SetKolasUserInfo(strKolasUserInfo);
	
				theInfo->SetUserID(strKolasUserID);
				theInfo->SetUserName(dm.GetCellData(0, 0));
				theInfo->SetUserPK(dm.GetCellData(0, 2));

				CString strHost;
				TCHAR szHostName[255];
				HOSTENT *pHostent;
				if (gethostname((LPSTR)(LPCTSTR)szHostName, 255) == 0)
				{
					pHostent = gethostbyname((LPSTR)(LPCTSTR)szHostName);
					if (pHostent)
					{
						strHost.Format(_T("%d.%d.%d.%d"), 
										(BYTE)pHostent->h_addr_list[0][0], 
										(BYTE)pHostent->h_addr_list[0][1], 
										(BYTE)pHostent->h_addr_list[0][2], 
										(BYTE)pHostent->h_addr_list[0][3]);
					}		
				}
				theInfo->SetLocalIP(strHost);

				m_bConnect = TRUE;
				return;
			}
		}
	}

	// 디비 접속에 실패했다.
	m_edtResult.SetWindowText(_T("접속실패!!"));
	// 이전 결과를 제거한다
	theInfo->SetConnectInfo(_T(""));
	theInfo->SetKolasUserInfo(_T(""));

	theInfo->SetUserID(_T(""));
	theInfo->SetUserName(_T(""));
	theInfo->SetUserPK(_T(""));
	theInfo->SetLocalIP(_T(""));

	m_bConnect = FALSE;
}

/*=============================================================
	@ Function    :	InitResultControl
	@ Description :
=============================================================*/
void CKL_CryptoConnPage::InitResultControl()
{
	CFont* pFont = GetFont();
	if (pFont != NULL)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);

		// 강조체 추가
		lf.lfWeight = FW_BOLD;
		
		m_ftResult.DeleteObject();
		m_ftResult.CreateFontIndirect(&lf);

		m_edtResult.SetFont(&m_ftResult);
	}
}

/*=============================================================
	@ Function    :	IsConnectInput
	@ Description :
=============================================================*/
BOOL CKL_CryptoConnPage::IsConnectInput()
{
	CString strValue;
	m_edtUser.GetWindowText(strValue);
	if (strValue.IsEmpty())
		return FALSE; // 사용자명을 입력하지 않았다

	strValue.Empty();
	m_edtPass.GetWindowText(strValue);
	if (strValue.IsEmpty())
		return FALSE; // 비밀번호를 입력하지 않았다

	strValue.Empty();
	m_edtSID.GetWindowText(strValue);
	if (strValue.IsEmpty())
		return FALSE; // SID를 입력하지 않았다

	strValue.Empty();
	m_edtKolasUser.GetWindowText(strValue);
	if (strValue.IsEmpty())
		return FALSE; // KOLAS 사용자명을 입력하지 않았다

	strValue.Empty();
	m_edtKolasUserID.GetWindowText(strValue);
	if (strValue.IsEmpty())
		return FALSE; // KOLAS 사용자ID를 입력하지 않았다

	return TRUE;
}

/*=============================================================
	@ Function    :	MakeConnectInfo
	@ Description :
=============================================================*/
CString CKL_CryptoConnPage::MakeConnectInfo()
{
	CString strUser;
	m_edtUser.GetWindowText(strUser);

	CString strPass;
	m_edtPass.GetWindowText(strPass);

	CString strSID;
	m_edtSID.GetWindowText(strSID);

	CString strConnectInfo;
	strConnectInfo.Format(_T("%s@%s/%s[]"), strUser, strSID, strPass);
	return strConnectInfo;
}

/*=============================================================
	@ Function    :	OnCtlColor
	@ Description :
=============================================================*/
HBRUSH CKL_CryptoConnPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_EDT_RESULT)
	{
		if (m_bConnect)
		{
			pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
		}
		else
		{
			pDC->SetTextColor(RGB(0xFF, 0x00, 0x00));
		}
	}
	
	return hbr;
}

