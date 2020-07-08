// HolydayInfo.cpp : implementation file
//

#include "stdafx.h"
#include "HolydayInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHolydayInfo dialog


CHolydayInfo::CHolydayInfo(CESL_Mgr* pParent /*=NULL*/)
	: CShareInfoCenterUp(pParent)
{
	//{{AFX_DATA_INIT(CHolydayInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	/// 매핑 테이블 생성하기
	m_pUClassInfoMapping = new CTableFieldMapping( this , LIB_HOLIDAY_ENV  );
	
}

CHolydayInfo::~CHolydayInfo()
{
	delete m_pUClassInfoMapping;
}

VOID CHolydayInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHolydayInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHolydayInfo, CDialog)
	//{{AFX_MSG_MAP(CHolydayInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHolydayInfo message handlers


INT CHolydayInfo::SetData(UPLOAD_SHARE_INFO UploadInfo , CString sLibCode ) 
{
	EFS_BEGIN
		
	m_ShareInfo = UploadInfo.UploadShareLoanInfo;
	
	CShareInfoCenterUp::SetData( UploadInfo , sLibCode );
	
	return 0;
	
	EFS_END
	return -1;
	
}





INT CHolydayInfo::UploadData()
{
	EFS_BEGIN

	INT ids;

	CString sType = m_ShareInfo.sType;

	// [3001] 이미 Temp Table에 존재하는지 알아보기
	ids = IsTempTable( m_ShareInfo.sUpdatedTableRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadData") );
	if( ids == 0 )
	{
		InsertTempTable(m_ShareInfo,_UP_ERROR_CODE_IS_TEMP_TBL);
		return 0;
	}

	ids = InitLibKey();
	if( ids != 0 ) return 10006;
				
	
	// [3002]
	/// INSERT 인지
	if( sType.Compare(_T("I")) == 0 )
	{
		ids = InsertEnv();
	}
	/// UPDATE 인지
	else if( sType.Compare(_T("U")) == 0 )
	{
		ids = UpdateEnv();
	}
	/// DELETE 인지 알아보기
	else if( sType.Compare(_T("D")) == 0 )
	{
		ids = DeleteEnv();
	}
	else
	{
		return 10001;
	}



	return ids;

	EFS_END
	return -1;

}

INT CHolydayInfo::InsertEnv()
{
	EFS_BEGIN

	INT ids;

	// 1. CLINET 에서 정보 구해오기
	ids = MakeLoaclEnvInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. 직급별 환경 정보 INSERT
	ids = InsertLocalEnvToCenter();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	
	return 0;

	EFS_END
	return -1;

}


INT CHolydayInfo::UpdateEnv()
{
	EFS_BEGIN

	INT ids;

	// 1. CLINET 에서 정보 구해오기
	ids = MakeLoaclEnvInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. 직급별 환경 정보 UPDATE
	ids = UpdateLocalEnvToCenter();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}



INT CHolydayInfo::DeleteEnv()
{
	EFS_BEGIN

	INT ids;
	
	/// 직급별 환경 정보가 존재하는지 확인
	CString sCenterEnvKey;
	ids = IsCenterLoanEnv(sCenterEnvKey);
	if( ids < 0 ) return ids;		
	if( ids > 1 ) return ids;
			
	/// 존재하지 않는다.
	if( ids != 1 ) return 0;

	/// 존재 한다면 삭제
	ids = DeleteCenterEnvInfo(sCenterEnvKey);
	if( ids != 0 ) return ids;
	

	return 0;

	EFS_END
	return -1;

}

/// CLINET 에서 정보 구해오기
INT CHolydayInfo::MakeLoaclEnvInfo()
{
	EFS_BEGIN

	INT ids;

	// 로컬 정보
	CESL_DataMgr* pDM = FindDM(_T("DM_CLIENT_HOLYDAY_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLoaclEnvIno") );

	CString sWhere;
	sWhere.Format( _T("REC_KEY = %s") , m_ShareInfo.sUpdatedTableRecKey );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeLoaclEnvIno") );

	if( pDM->GetRowCount() == 0 )
		return 10021;


	return 0;

	EFS_END

	return -1;
}

/// 직급별 환경 정보 INSERT
INT CHolydayInfo::InsertLocalEnvToCenter()
{
	EFS_BEGIN

	INT ids;

	// [4101] 센터에 이미 업로드가 된 자료인지 알아보는 함수
	CString sCenterEnvKey;
	ids = IsCenterLoanEnv( sCenterEnvKey );
	if( ids < 0 ) return ids;
	if( ids > 1 ) return ids;

	// 존재한다면 
	if( ids == 1 )
	{
		ids = DeleteCenterEnvInfo( sCenterEnvKey );
		if( ids != 0 ) return ids;
	}


	// [4103] INSERT
	ids = UploadUClassDelayEnv( sCenterEnvKey );
	if( ids < 0 ) return ids;

	return 0;
	
	EFS_END
		
	return -1;
}

/// 직급별 환경 정보 UPDATE
INT CHolydayInfo::UpdateLocalEnvToCenter()
{
	EFS_BEGIN
	
	INT ids;

	// [4101] 센터에 이미 업로드가 된 자료인지 알아보는 함수
	CString sCenterEnvKey;
	ids = IsCenterLoanEnv( sCenterEnvKey );
	if( ids < 0 ) return ids;
	if( ids > 1 ) return ids;
	
	// 존재하지 않는다면 INSERT
	if( ids == 0 )
	{
		// [4103] INSERT
		ids = UploadUClassDelayEnv( sCenterEnvKey );
		if( ids < 0 ) return ids;
	}

	// [4203] UPDATE
	ids = UpdateUClassDelayEnv( sCenterEnvKey );
	if( ids != 0 ) return ids;
	

	return 0;
	
	EFS_END
		
	return -1;
}

/// [4102] 직급별 환경 정보 삭제
INT CHolydayInfo::DeleteCenterEnvInfo( CString sCenterEnvKey )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format(_T("DELETE FROM CT_LIB_HOLIDAY_TBL WHERE REC_KEY = %s ;")
		, sCenterEnvKey );
	
//	ids = m_pCS_SQL->LOAN_UnSelect( sSql );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	
	return 0;
	
	EFS_END
		
	return -1;
}

/// [4101] 직급별 환경 정보가 존재하는지 확인
INT CHolydayInfo::IsCenterLoanEnv( CString &sCenterEnvKey )
{
	EFS_BEGIN

	INT ids;
		
	CString sSql;
	sSql.Format(_T("SELECT REC_KEY FROM CT_LIB_HOLIDAY_TBL WHERE LIB_INFO_KEY = %s AND CLIENT_REC_KEY = %s ")
				, m_sLibKey , m_ShareInfo.sUpdatedTableRecKey );

//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sCenterEnvKey , FALSE );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}

	sCenterEnvKey.TrimLeft(); sCenterEnvKey.TrimRight();

	/// 존재한다면
	if( !sCenterEnvKey.IsEmpty() ) return 1;
		
	/// 존재하지 않는다면
	return 0;
	
	EFS_END
		
	return -1;
}

// [4103] INSERT
INT CHolydayInfo::UploadUClassDelayEnv( CString &sCenterEnvKey )
{
	EFS_BEGIN

	INT ids;
	
	CString sTableName = _T("CT_LIB_HOLIDAY_TBL");
	
	CESL_DataMgr* pLocalLoanDM = FindDM( _T("DM_CLIENT_HOLYDAY_INFO") );
	if( pLocalLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertCenterLoanInfo") );
	
	/// 매핑 필드 구성
	CESL_DataMgr* pMappingDM = m_pUClassInfoMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertCenterLoanInfo") );
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("소스필드") , i , sSrcField );
		pMappingDM->GetCellData( _T("적용필드") , i , sDstField );
		pMappingDM->GetCellData( _T("타입")		, i , sType );
		
		pLocalLoanDM->GetCellData( sSrcField , 0 , sData );
		
		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
	/// RecKey 구성 --------------------------------
	ids = CShareInfoCenterUp::MakeCenterRecKey(sCenterEnvKey);
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	pMappingDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sCenterEnvKey);
	/// --------------------------------------------
	
	/// 나머지 필드 구성
	pMappingDM->AddDBFieldData( _T("LIB_INFO_KEY") , _T("NUMERIC") , m_sLibKey );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[4103]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("FIRST_WORK")		, _T("STRING") , sFirstWork );
	

	CString sInsertQuery;
	/// Insert쿼리 만들기
	pMappingDM->MakeInsertFrameQuery( sTableName , sInsertQuery );

	/*
	/// Insert 쿼리
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
	*/

	/// 센터 데몬에 쿼리 날리기 --------------------
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , FALSE );	
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	/// --------------------------------------------
	
	pMappingDM->EndFrame();
	
	return 0;

	EFS_END

	return -1;
}

// [4203] UPDATE
INT CHolydayInfo::UpdateUClassDelayEnv( CString sCenterEnvKey )
{
	EFS_BEGIN

		
	INT ids;
	
	CString sTableName = _T("CT_LIB_HOLIDAY_TBL");
	
	CESL_DataMgr* pLocalLoanDM = FindDM( _T("DM_CLIENT_HOLYDAY_INFO") );
	if( pLocalLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertCenterLoanInfo") );
	
	/// 매핑 필드 구성
	CESL_DataMgr* pMappingDM = m_pUClassInfoMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertCenterLoanInfo") );
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("소스필드") , i , sSrcField );
		pMappingDM->GetCellData( _T("적용필드") , i , sDstField );
		pMappingDM->GetCellData( _T("타입")		, i , sType );

		if( !IsInUpdatedFields( m_asUpdatedFields , sSrcField ) ) continue;
		
		pLocalLoanDM->GetCellData( sSrcField , 0 , sData );

		
		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
		
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[4203]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("LAST_WORK")		, _T("STRING") , sFirstWork );
	
	CString sInsertQuery;

	/// Insert쿼리 만들기
	pMappingDM->MakeUpdateFrameQuery( sTableName , _T("REC_KEY") , _T("NUMERIC") , sCenterEnvKey , sInsertQuery );

/*	
	/// Insert 쿼리
	POSITION pos = pMappingDM->frame.FindIndex(0);
	sInsertQuery = *((CString*)(pMappingDM->frame.GetAt(pos)));
*/	
	/// 센터 데몬에 쿼리 날리기 --------------------
//	ids = m_pCS_SQL->LOAN_UnSelect( sInsertQuery , FALSE );	
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	/// --------------------------------------------
	
	pMappingDM->EndFrame();
	
	return 0;
	
	
	EFS_END
		
	return -1;	
}
