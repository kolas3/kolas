// VerInfo.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "VerInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "RegistryEx.h"

/////////////////////////////////////////////////////////////////////////////
// CVerInfo property page

IMPLEMENT_DYNCREATE(CVerInfo, CPropertyPage)

CVerInfo::CVerInfo() : CPropertyPage(CVerInfo::IDD)
{
	//{{AFX_DATA_INIT(CVerInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CVerInfo::~CVerInfo()
{
}

VOID CVerInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CVerInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerInfo message handlers

BOOL CVerInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	LoadConfig() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CVerInfo::LoadConfig ()
{

	CButton * pTitlte = (CButton *)GetDlgItem(IDC_Text_Title) ;
	CButton * pVerDate = (CButton *)GetDlgItem(IDC_Text_Date) ;
	CButton * pVerInfo = (CButton *)GetDlgItem(IDC_Text_Version) ;
    CString strTitle ;
//DEL     CString m_strRegDepth ;
	strTitle = _T("KOLAS III");
	pTitlte->SetWindowText (strTitle + _T(" 프로그램 정보"));
	
    HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth;
	depth = _T("Software\\ECO\\KOLAS III");

//DEL 	CString depth = m_strRegDepth;
	CString strDate = _T("");
	CString strVerInfo = _T("") ;
	CString strInstallDate = _T("");
	CRegistryEx reg(hKey);
	if ( reg.VerifyKey( depth ) ) {
		 reg.Open( hKey, depth );
		 reg.Read( _T("SETUP_BUILD_DATE"), strDate);
		 reg.Read (_T("SETUP_BUILD_NUMBER"),strVerInfo);
		 reg.Read (_T("SETUP_INSTALL_DATE"),strInstallDate);
		 reg.Close();
	}
	
    pVerDate->SetWindowText (_T("설치 버젼 : ") + strDate + _T(" BUILD ") +strVerInfo) ;
    pVerInfo->SetWindowText (_T("설치 일자 : ") + strInstallDate );

	return 0 ;
}
