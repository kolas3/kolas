// CO_CENTER_USER_USE.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_USE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_USE dialog


CCO_CENTER_USER_USE::CCO_CENTER_USER_USE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_USE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	EFS_BEGIN

	if( InitESL_Mgr(_T("CO_CENTER_USER_USE")) < 0 )
		ERROR_MSG_RETURN_VOID2( -1 , _T("CCO_CENTER_USER_USE") )

	EFS_END
}

CCO_CENTER_USER_USE::~CCO_CENTER_USER_USE()
{

}

VOID CCO_CENTER_USER_USE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_USE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_USE, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_USE)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_USE message handlers


INT CCO_CENTER_USER_USE::IsUseCenterUser( CString &sLibKey , CString &sLibCode )
{
//	OnShowDMViewer(0);

	// 이동 도서관용은 센터업로드 불가능
	if( m_pInfo->MODE == 10000 || (m_pInfo->MODE == 30000) )
		return 6;

	/*

	CESL_DataMgr* pLocalDM = FindDM(_T("DM_LOCAL_LIB_INFO"));
	if( pLocalDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseCenterUser") );

	ids = pLocalDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("IsUseCenterUser") );
	
	if( pLocalDM->GetRowCount() == 0 )
		return 1;

	ids = pLocalDM->GetCellData( _T("도서관부호") , 0 , sLibCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("IsUseCenterUser") );

  */

	/// 자관 도서관이 센터 도서관이 사용 가능한지 알아본다.

	CString sValue;
	CESL_DataMgr* pLocalDM = FindDM(_T("DM_LOCAL_LIB_INFO"));
	if( pLocalDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseCenterUser") );

//	GetManageValue( _T("기타") , _T("공통") , _T("대출자공유관리") , _T("센터도서관사용여부") , sValue );

//	sValue.TrimLeft(); sValue.TrimRight();

//	if( sValue.Compare(_T("N")) == 0 || sValue.IsEmpty() )
//		return 7;

	
	

	const INT nFieldCnt = 1;
	CString sField[nFieldCnt] = 
	{
		_T("LIB_CODE")
	};
	CString sLibCodes[nFieldCnt];

	CLocCommonAPI::GetLibInfo( sField , sLibCodes , nFieldCnt , this  );

	sLibCode = sLibCodes[0];

	if( sLibCode.IsEmpty() ) return 2;
	

	CString sWhere;
	sWhere.Format( _T("CODE = '%s'") , sLibCode );
/*
	CESL_DataMgr* pCenterDM = FindDM(_T("DM_CENTER_USE"));
	if( pCenterDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("IsUseCenterUser") );
	
	ids = pCenterDM->RefreshDataManager(sWhere);
	if( ids < 0 ) return 3;
	if( ids > 0 ) return 4;

	if( pCenterDM->GetRowCount() < 1 ) return 5;

	ids = pCenterDM->GetCellData( _T("도서관정보KEY") , 0 , sLibKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("IsUseCenterUser") );
*/
	return 0;
}