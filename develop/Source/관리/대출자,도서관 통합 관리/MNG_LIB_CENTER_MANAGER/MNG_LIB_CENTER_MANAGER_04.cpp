// MNG_LIB_CENTER_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_04.h"
#include "MNG_LIB_CENTER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_04 dialog


CMNG_LIB_CENTER_MANAGER_04::CMNG_LIB_CENTER_MANAGER_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_LIB_CENTER_MANAGER_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_LIB_CENTER_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_04)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_04 message handlers


BOOL CMNG_LIB_CENTER_MANAGER_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGER_02")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMNG_LIB_CENTER_MANAGER_04::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		// TODO: Add your specialized code here and/or call the base class
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}

VOID CMNG_LIB_CENTER_MANAGER_04::OnbtnFIND() 
{
	EFS_BEGIN

	SearchCenterUser();	

	EFS_END
}

/// 센터 이용자 검색
INT CMNG_LIB_CENTER_MANAGER_04::SearchCenterUser()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;

	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchCenterUser") );
	pDM->SetCONNECTION_INFO(_T("kolas2@k2up_center/kolas2#[]"));

	ids = GetWhereSql(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );
	if( ids > 0 ) return ids;

	if( sWhere.IsEmpty() )
	{
		AfxMessageBox(_T("조건을 입력하시고 검색하십시오"));
		return 1;
	}

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );

	// 도서관코드 -> 설명
	ids = LibCodToDesc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchCenterUser") );

	ids = AllControlDisplay( _T("CM_CO_CENTER_USER_MANAGER_01") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchCenterUser") );

	return 0;

	EFS_END

	return -1;
}

/// WHERE절 구성하기
INT CMNG_LIB_CENTER_MANAGER_04::GetWhereSql( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	const INT nControlCnt = 8;

	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGER_02");

	CString sControlAlias[nControlCnt] =
	{
		_T("대출자번호1") , _T("대출자번호2") , _T("대출자명1") , _T("대출자명2") , _T("주민등록번호") , 
		_T("등록일자1") , _T("등록일자2") , _T("E_MAIL")	
	};

	TCHAR* sSearchFieldName[nControlCnt] =
	{
		_T("CMLU.USER_NO") , _T("CMLU.USER_NO") , _T("CMLU.NAME") , _T("CMLU.NAME") , _T("CMLU.CIVIL_NO") , 
		_T("CLU.REG_DATE") , _T("CLU.REG_DATE") , _T("CMLU.E_MAIL")	
	};

	INT nSearchType[nControlCnt] =
	{
		TWOSTRING , ETC , TWOSTRING , ETC , ONELIKESTRING , 
		TWODATE , ETC , ONELIKESTRING
	};

	CString sGetData[nControlCnt];

	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhereSql") );
	}

	ids = CLocCommonAPI::MakeSql( sGetData , sSearchFieldName , nSearchType , nControlCnt , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhereSql") );


	CString sLibName , sLibKey;

	ids = GetControlData( sCMAlias , _T("도서관명") , sLibName , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetWhereSql") );

	if( sLibName.Compare(_T("전체")) == 0 ) return 0;

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetWhereSql") );

	ids = CLocCommonAPI::GetCellData( this , pLibInfoDM , _T("도서관이름") , sLibName , _T("도서관KEY") , sLibKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("GetWhereSql") );

	if( !sLibKey.IsEmpty() )
	{	
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sWhere += _T(" CLU.LIB_KEY = ") + sLibKey;
	}

	return 0;


	EFS_END

	return -1;
}


/// 화면 구성
/// 컨트롤 초기화
INT CMNG_LIB_CENTER_MANAGER_04::InitControl()
{
	EFS_BEGIN

	INT ids;

	/// 도서관명 조건 구성
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbLIBNAME);

	pCombo->ResetContent();

	pCombo->AddString(_T("전체"));

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CString sLibName;
	for( INT i = 0 ; i < pLibInfoDM->GetRowCount() ; i++ )
	{
		ids = pLibInfoDM->GetCellData( _T("도서관이름") , i , sLibName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

		pCombo->AddString(sLibName);
	}

	pCombo->SetCurSel(0);
	
	return 0;

	EFS_END

	return -1;
}

VOID CMNG_LIB_CENTER_MANAGER_04::OnbtnCLOSE() 
{
	OnCancel();	
}


/// 도서관명 코드 -> 설명
INT CMNG_LIB_CENTER_MANAGER_04::LibCodToDesc()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pLibInfoDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pLibInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("LibCodToDesc") );
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("LibCodToDesc") );

	TCHAR* sAlias[2] =
	{
		_T("생성도서관") , _T("최종수정도서관")
	};

	CString sLibCode;
	CString sLibName;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		for( INT j = 0 ; j < 2 ; j++ )
		{
			ids = pDM->GetCellData( sAlias[j] , i , sLibCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("LibCodToDesc") );
			
			ids = CLocCommonAPI::GetCellData( this , pLibInfoDM , _T("도서관CODE") , sLibCode , _T("도서관이름") , sLibName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("LibCodToDesc") );
			
			ids = pDM->SetCellData( sAlias[j] , sLibName , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("LibCodToDesc") );
		}
	}

	return 0;

	EFS_END

	return -1;
}

