// LoanInfoCenterUp.cpp : implementation file
//

#include "stdafx.h"
#include "LoanInfoCenterUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoCenterUp dialog


CLoanInfoCenterUp::CLoanInfoCenterUp(CESL_Mgr* pParent /*=NULL*/)
	: CShareInfoCenterUp(pParent)
{
	//{{AFX_DATA_INIT(CLoanInfoCenterUp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	/// 매핑 테이블 생성하기
	m_pLoanInfoMapping  = new CTableFieldMapping( this , LOAN_INFO );
	
}

CLoanInfoCenterUp::~CLoanInfoCenterUp()
{	
	delete m_pLoanInfoMapping;
}

VOID CLoanInfoCenterUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanInfoCenterUp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanInfoCenterUp, CDialog)
	//{{AFX_MSG_MAP(CLoanInfoCenterUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoCenterUp message handlers


INT CLoanInfoCenterUp::SetData(UPLOAD_SHARE_INFO UploadInfo , CString sLibCode ) 
{
	EFS_BEGIN
		
	m_ShareInfo = UploadInfo.UploadShareLoanInfo;
	
	CShareInfoCenterUp::SetData( UploadInfo , sLibCode );
	
	return 0;
	
	EFS_END
	return -1;
	
}





INT CLoanInfoCenterUp::UploadData()
{
	EFS_BEGIN

	INT ids;

	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UploadData() [START]") );


	CString sType = m_ShareInfo.sType;

	// [3001] 이미 Temp Table에 존재하는지 알아보기
	ids = IsTempTable( m_ShareInfo.sUpdatedTableRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UploadData") );
	if( ids == 0 )
	{
		InsertTempTable(m_ShareInfo,_UP_ERROR_CODE_IS_TEMP_TBL);
		return 0;
	}

	// [3002]
	/// INSERT 인지
	if( sType.Compare(_T("I")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertLoan [START]") );
		ids = InsertLoan();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertLoan [END]") );
	}
	/// UPDATE 인지
	else if( sType.Compare(_T("U")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateLoan [START]") );
		ids = UpdateLoan();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateLoan [END]") );
	}
	/// DELETE 인지 알아보기
	else if( sType.Compare(_T("D")) == 0 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::DeleteLoan [START]") );
		ids = DeleteLoan();
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::DeleteLoan [END]") );
	}
	else
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UploadData [ERROR-10001 RETURN]") );
		return 10001;
	}


	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UploadData() [END]") );
	
	return ids;

	EFS_END
	return -1;

}

INT CLoanInfoCenterUp::InsertLoan()
{
	EFS_BEGIN

	INT ids;

	// 1. CLINET 에서 정보 구해오기
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::MakeLoaclLoanInfo() [START]") );
	ids = MakeLoaclLoanInfo();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::MakeLoaclLoanInfo() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. 대출/반납 정보 INSERT
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertLocalLoanToCenter() [START]") );
	ids = InsertLocalLoanToCenter();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertLocalLoanToCenter() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	
	return 0;

	EFS_END
	return -1;

}


INT CLoanInfoCenterUp::UpdateLoan()
{
	EFS_BEGIN

	INT ids;

	

	// 1. CLINET 에서 정보 구해오기
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::MakeLoaclLoanInfo [START]") );
	ids = MakeLoaclLoanInfo();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::MakeLoaclLoanInfo [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertUser") );
	if( ids > 0 ) return ids;
	
	// 2. 대출/반납 정보 UPDATE
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateLocalLoanToCenter [START]") );
	ids = UpdateLocalLoanToCenter();
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateLocalLoanToCenter [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertUser") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}



INT CLoanInfoCenterUp::DeleteLoan()
{
	EFS_BEGIN

	INT ids;
	
	/// 이용자가 존재하는지 확인
	CString sCenterLoanKey;
	ids = IsCenterLoanInfo(sCenterLoanKey);
	if( ids < 0 ) return ids;		
	if( ids > 1 ) return ids;
			
	/// 존재하지 않는다.
	if( ids != 1 ) return 0;

	/// 존재 한다면 삭제
	ids = DeleteCenterLoanInfo(sCenterLoanKey);
	if( ids != 0 ) return ids;
	

	return 0;

	EFS_END
	return -1;

}



INT CLoanInfoCenterUp::MakeLoaclLoanInfo()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_CLIENT_LOAN_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLoaclLoanInfo") );

	CString sWhere;
	sWhere.Format( _T("REC_KEY = %s") , m_ShareInfo.sUpdatedTableRecKey );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeLoaclLoanInfo") );

	if( pDM->GetRowCount() == 0 )
		return 10010;

	return 0;

	EFS_END

	return -1;

}

/// Local의 대출/반납 정보 센터에 INSERT
INT CLoanInfoCenterUp::InsertLocalLoanToCenter()
{
	EFS_BEGIN
		
	INT ids;

	/// [3101] 대출/반납 정보에 연결된 이용자가 존재하는가?
	CString sMasterUserKey;
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanUser() [START]") );	
	ids = IsCenterLoanUser(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanUser() [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertLocalLoanToCenter") );
	if( ids == 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("203") );
		return 10012;
	}
	if( ids > 1 ) return ids;

	
	/// [3102] 인서트 하려는 대출/반납 정보가 존재하는가?
	CString sCenterLoanKey;
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanInfo() [START]") );	
	ids = IsCenterLoanInfo(sCenterLoanKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanInfo() [END]") );	
	if( ids < 0 ) return ids;		
	if( ids > 1 ) return ids;

	/// 존재한다면
	/// [3103] 도서관별 대출/반납 정보 DELETE
	if( ids == 1 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::DeleteCenterLoanInfo() [START]") );	
		ids = DeleteCenterLoanInfo(sCenterLoanKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::DeleteCenterLoanInfo() [END]") );	
		if( ids != 0 ) return ids;
	}

	/// [3104] 도서관별 대출/반납 정보 INSERT
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertCenterLoanInfo() [START]") );	
	ids = InsertCenterLoanInfo(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertCenterLoanInfo() [END]") );	
	if( ids != 0 ) return ids;				
	
	return 0;
	
	EFS_END
		
	return -1;
	
}

/// Local의 대출/반납 정보 센터에 UPDATE
INT CLoanInfoCenterUp::UpdateLocalLoanToCenter()
{
	EFS_BEGIN

	INT ids;
		
	/// [3201][3101] 대출/반납 정보에 연결된 이용자가 존재하는가?
	CString sMasterUserKey;
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanUser [START]") );
	ids = IsCenterLoanUser(sMasterUserKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanUser [END]") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertLocalLoanToCenter") );
	if( ids == 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("203") );
		return 10012;
	}
	if( ids > 1 ) return ids;
	
	/// [3202][3102] 인서트 하려는 대출/반납 정보가 존재하는가?
	CString sCenterLoanKey;
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanInfo [START]") );
	ids = IsCenterLoanInfo(sCenterLoanKey);
	CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::IsCenterLoanInfo [END]") );
	if( ids < 0 ) return ids;		
	if( ids > 1 ) return ids;
	
	/// 존재한다면
	/// [3203] 도서관별 대출/반납 정보 Update
	if( ids == 1 )
	{
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateCenterLoanInfo [START]") );
		ids = UpdateCenterLoanInfo(sCenterLoanKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::UpdateCenterLoanInfo [END]") );
		if( ids != 0 ) return ids;
	}
	else
	{
		/// [3204][3104] 도서관별 대출/반납 정보 INSERT
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertCenterLoanInfo [START]") );
		ids = InsertCenterLoanInfo(sMasterUserKey);
		CShareInfoCenterUp::RecordLogTunning( _T("CLoanInfoCenterUp::InsertCenterLoanInfo [END]") );
		if( ids != 0 ) return ids;				
	}
	
	
		
	return 0;
	
	EFS_END
		
	return -1;
	
}



/// [3101] 대출/반납 정보에 연결된 이용자가 존재하는가?
INT CLoanInfoCenterUp::IsCenterLoanUser( CString &sMasterUserKey )
{
	EFS_BEGIN

	INT ids;

	CString sUserKey;

	CESL_DataMgr* pDM = FindDM( _T("DM_CLIENT_LOAN_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsCenterLoanUser") );
	
	ids = pDM->GetCellData( _T("USER_KEY") , 0 , sUserKey );

	CString sSql;
	sSql.Format( _T(" SELECT MASTER_USER_KEY FROM CT_LIBRARY_LOAN_USER_TBL WHERE CLIENT_REC_KEY = %s ")
		         _T(" AND LIB_CODE = '%s' ") , sUserKey , m_sLibCode );

//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sMasterUserKey , FALSE );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	
	sMasterUserKey.TrimLeft(); sMasterUserKey.TrimRight();

	/// 존재한다면 
	if( !sMasterUserKey.IsEmpty() ) return 1;
		
	/// 존재하지 않는다면
	return 0;
	
	EFS_END
		
	return -1;
	
}

/// [3102] 인서트 하려는 대출/반납 정보가 존재하는가?
INT CLoanInfoCenterUp::IsCenterLoanInfo( CString &sCenterLoanKey )
{
	EFS_BEGIN


	INT ids;

	CString sSql;
	sSql.Format( _T(" SELECT REC_KEY FROM CT_CO_LOAN_INFO_TBL WHERE CLIENT_REC_KEY = %s ")
		_T(" AND LIB_CODE = '%s' ") , m_ShareInfo.sUpdatedTableRecKey , m_sLibCode );
	
//	ids = m_pCS_SQL->LOAN_GetOneValue( sSql , sCenterLoanKey , FALSE );
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	
	sCenterLoanKey.TrimLeft(); sCenterLoanKey.TrimRight();
	
	/// 존재한다면 
	if( !sCenterLoanKey.IsEmpty() ) return 1;
	
	/// 존재하지 않는다면
	return 0;
	
	EFS_END
		
	return -1;
}


/// [3103] 도서관별 대출/반납 정보 DELETE
INT CLoanInfoCenterUp::DeleteCenterLoanInfo( CString sCenterLoanKey )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( _T("DELETE FROM CT_CO_LOAN_INFO_TBL WHERE REC_KEY = %s ;") , sCenterLoanKey );

//	ids = m_pCS_SQL->LOAN_UnSelect( sSql , FALSE );
	if( ids != 0 ) 
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	
	return 0;
	
	EFS_END
		
	return -1;
}

/// [3104] 도서관별 대출/반납 정보 INSERT
INT CLoanInfoCenterUp::InsertCenterLoanInfo( CString sMasterUserKey )
{
	EFS_BEGIN

	INT ids;
		
	CString sTableName = _T("CT_CO_LOAN_INFO_TBL");
	
	CESL_DataMgr* pLocalLoanDM = FindDM( _T("DM_CLIENT_LOAN_INFO") );
	if( pLocalLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertCenterLoanInfo") );
	
	/// 매핑 필드 구성
	CESL_DataMgr* pMappingDM = m_pLoanInfoMapping->GetMappingInfo();
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
	CString sRecKey;
	ids = CShareInfoCenterUp::MakeCenterRecKey(sRecKey);
	if( ids != 0 )
	{
		CShareInfoCenterUp::InsertTempTable( m_ShareInfo , _T("001") );
		return 10006;
	}
	pMappingDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey);
	/// --------------------------------------------
	
	/// 나머지 필드 구성
	pMappingDM->AddDBFieldData( _T("LIB_CODE") , _T("STRING") , m_sLibCode );
	pMappingDM->AddDBFieldData( _T("MASTER_USER_KEY")	, _T("NUMERIC") , sMasterUserKey );
	
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[3104]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("FIRST_WORK")		, _T("STRING") , sFirstWork );
	
	CString sInsertQuery;

	/// Insert쿼리 만들기
	pMappingDM->MakeInsertFrameQuery( sTableName , sInsertQuery);
	
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


/// [3203] 도서관별 대출/반납 정보 Update
INT CLoanInfoCenterUp::UpdateCenterLoanInfo( CString sCenterLoanKey)
{
	EFS_BEGIN

	INT ids;
		
	CString sTableName = _T("CT_CO_LOAN_INFO_TBL");
	
	CESL_DataMgr* pLocalLoanDM = FindDM( _T("DM_CLIENT_LOAN_INFO") );
	if( pLocalLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertCenterLoanInfo") );
	
	/// 매핑 필드 구성
	CESL_DataMgr* pMappingDM = m_pLoanInfoMapping->GetMappingInfo();
	if( pMappingDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertCenterLoanInfo") );
	
	pMappingDM->StartFrame();
	pMappingDM->InitDBFieldData();
	
	CString sSrcField , sDstField , sType , sData;
	for( INT i = 0 ; i < pMappingDM->GetRowCount() ; i++ )
	{
		pMappingDM->GetCellData( _T("소스필드") , i , sSrcField );
		pMappingDM->GetCellData( _T("적용필드") , i , sDstField );
		pMappingDM->GetCellData( _T("타입")		, i , sType );

		if( !IsInUpdatedFields( m_asUpdatedFields , sSrcField) ) continue;
		
		pLocalLoanDM->GetCellData( sSrcField , 0 , sData );

		if( sType.Compare(_T("NUMERIC")) == 0 && sData.IsEmpty() )
			sData = _T("NULL");
		
		pMappingDM->AddDBFieldData( sDstField , sType , sData , TRUE );
	}
	
	
	/// 나머지 필드 구성
	CString sFirstWork;
	sFirstWork = CESL_Mgr::GetWorkLogMsg( _T("[3203]") , __WFILE__ , __LINE__ );
	pMappingDM->AddDBFieldData( _T("LAST_WORK")	, _T("STRING") , sFirstWork );
	
	CString sInsertQuery;

	/// Update쿼리 만들기
	ids = pMappingDM->MakeUpdateFrameQuery( sTableName , _T("REC_KEY") , _T("NUMERIC") , sCenterLoanKey, sInsertQuery );
	
	/// Update 쿼리
/*
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
