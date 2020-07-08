#include "stdafx.h"
#include "BL_LOC_NON_DB_BOOK_PROC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CBL_LOC_NON_DB_BOOK_PROC::CBL_LOC_NON_DB_BOOK_PROC(CESL_Mgr *pWnd , CLoanShareManager* pLoanShareManager ,  CLonaManageValue *pLona)
	: CSPF_Work(pWnd)
{	
	m_pLoanShareManager = pLoanShareManager;

	m_sNonDBDMAlias = _T("DM_BO_LOC_3192");
	m_sNonDBGridCMAlias = _T("CM_BO_LOC_3192");
	m_sNonDBCMAlias = _T("CM_BO_LOC_3192_EDIT_INFO");
	m_sNonDBGridAlias = _T("긴급대출자료목록");

	m_pNonDBDM = m_pParentMgr->FindDM( m_sNonDBDMAlias );
	if( m_pNonDBDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_NON_DB_BOOK_PROC") );

	m_pNonDBCM = m_pParentMgr->FindCM( m_sNonDBCMAlias );
	if( m_pNonDBCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_NON_DB_BOOK_PROC") );
	
	m_pNonDBGridCM = m_pParentMgr->FindCM( m_sNonDBGridCMAlias );
	if( m_pNonDBGridCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_NON_DB_BOOK_PROC") );

	m_pLoanSpfWork = new CBL_LOC_LOAN_PROC(m_pParentMgr,m_pLoanShareManager,pLona);
}

CBL_LOC_NON_DB_BOOK_PROC::~CBL_LOC_NON_DB_BOOK_PROC()
{
	if( m_pLoanSpfWork != NULL ) delete m_pLoanSpfWork;
}


INT CBL_LOC_NON_DB_BOOK_PROC::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( INIT_SCREEN ) == 0 )
	{
		ids = InitScreen();
	}
	else if( sPathAlias.Compare( INISERT_NONBOOK ) == 0 )
	{
		ids = InsertNonBook();
	}
	else if( sPathAlias.Compare( EDIT_NONBOOK ) == 0 )
	{
		ids = EditNonBook();
	}
	else if( sPathAlias.Compare( DELETE_NONBOOK ) == 0 )
	{
		ids = DeleteNonBook();
	}
	else if( sPathAlias.Compare( LOAN_NONBOOK ) == 0 )
	{
		ids = LoanNonBook();
	}
	else if( sPathAlias.Compare( CLEAR_NONBOOK ) == 0 )
	{
		ids = ClearScreen();
	}
	else if( sPathAlias.Compare( MOVE_TO_CAT ) == 0 )
	{
		ids = MoveToCat();
	}
	else
	{
		ids = 0;
	}

	if( ids > 0 ) 
		return ids;

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_NON_DB_BOOK_PROC::InitScreen()
{
	EFS_BEGIN

	INT ids;
	
	ids = MakeDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitScreen") );
	
	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitScreen") );
	
	ids = ViewDetailInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InitScreen") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::MakeDM()
{
	EFS_BEGIN

	INT ids;

	m_pNonDBDM->FreeData();

	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , m_pNonDBDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDM") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::ViewScreen()
{
	EFS_BEGIN

	INT ids;

	m_pNonDBGridCM->AllControlClear();

	
	CString sWorkingCode;
	for( INT i = 0 ; i < m_pNonDBDM->GetRowCount() ; i++ )
	{
		ids = m_pNonDBDM->GetCellData( _T("책상태") , i , sWorkingCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewScreen") );

		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingCode , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewScreen") );

		ids = m_pNonDBDM->SetCellData( _T("책상태") , sWorkingCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewScreen") );

	}

	ids = m_pNonDBGridCM->AllControlDisplay();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewScreen") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::ViewDetailInfo(INT nIndex/*=-1*/)
{
	EFS_BEGIN

	INT ids;

	m_pNonDBCM->AllControlClear();

	if( nIndex == -1 )
		nIndex = 0;

	INT nRowCount = m_pNonDBDM->GetRowCount();

	if( nRowCount < 1 )
	{
		m_pNonDBCM->AllControlClear();
		return 0;
	}

	CString sWorkingCode;
	ids = m_pNonDBDM->GetCellData( _T("책상태") , nIndex , sWorkingCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewScreen") );

	ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingCode , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewScreen") );

	ids = m_pNonDBDM->SetCellData( _T("책상태") , sWorkingCode , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewScreen") );


	ids = m_pNonDBCM->AllControlDisplay( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewDetailInfo") );

	SetSelectGrid( nIndex );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::InsertNonBook()
{
	EFS_BEGIN

	INT ids;

	ids = IsDuplicateRegNo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	if( ids > 0 ) return 1;

	ids = InsertNonBookDBProc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InsertNonBook") );


	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InsertNonBook") );

	ids = ViewDetailInfo(m_pNonDBDM->GetRowCount()-1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InsertNonBook") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::InsertNonBookDBProc()
{
	EFS_BEGIN

	INT ids;

	
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , m_pNonDBDM );

	pMobileApi->StartFrame();

	CString sWorkingStatus = _T("BOL112N");
	CString sRegCode;
	CString sRecKey;

	ids = pMobileApi->MakeRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBookDBProc") );

	ids = m_pNonDBCM->GetControlMgrData( _T("등록번호") , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InsertNonBookDBProc") );

	if( sRegCode.GetLength() < 3 )
	{
		AfxMessageBox(_T("잘못된 등록번호입니다. 확인하시고 입력하십시오") );
		return 1;
	}

	ids = CLocCommonAPI::MakeRegNoSpaceToZero( m_pParentMgr , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );

	ids = m_pNonDBCM->SetControlMgrData( _T("등록번호") , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("InsertNonBook") );

	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	sRegCode = sRegCode.Left(nRegCodeLength);
	
	m_pNonDBDM->StartFrame();
	m_pNonDBDM->InitDBFieldData();

	ids = m_pNonDBDM->MakeControlMgrFiledData( (CObject*)m_pNonDBCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("InsertNonBookDBProc") );

	ids = m_pNonDBDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sWorkingStatus , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("InsertNonBookDBProc") );

	ids = m_pNonDBDM->AddDBFieldData( _T("REG_CODE") , _T("STRING") , sRegCode , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("InsertNonBookDBProc") );
	
	CString sLog;
	
	sLog.Format( _T("%s") , m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
	m_pNonDBDM->AddDBFieldData( _T("FIRST_WORK") , _T("STRING") , sLog , TRUE );

	ids = pMobileApi->MakeInsertFrame( _T("CO_NON_DB_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("InsertNonBookDBProc") );

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("InsertNonBookDBProc") );
	ids = m_pNonDBDM->EndFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13, _T("InsertNonBookDBProc") );
	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );
	
	m_pNonDBDM->InsertRow(-1);
	INT nInsertIndex = m_pNonDBDM->GetRowCount() -1;

	const INT nCnt = 11;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("형식구분") , _T("관리구분") , _T("등록번호") , _T("별치기호") , 
		_T("본표제") , _T("이용대상구분") , _T("이용제한구분") , _T("가격") ,
		_T("저작자") , _T("출판사"), _T("청구기호")
	};

	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pNonDBCM->GetControlMgrData( sDMFieldAlias[i] , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("InsertNonBookDBProc") );

		ids = m_pNonDBDM->SetCellData( sDMFieldAlias[i] , sData , nInsertIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("InsertNonBookDBProc") );
	}

	
	ids = m_pNonDBDM->SetCellData( _T("등록구분") , sRegCode , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("InsertNonBookDBProc") );
	
	ids = m_pNonDBDM->SetCellData( _T("책KEY") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -17 , _T("InsertNonBookDBProc") );
	
	ids = m_pNonDBDM->SetCellData( _T("책상태") , sWorkingStatus , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -18 , _T("InsertNonBookDBProc") );


	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::EditNonBook()
{
	EFS_BEGIN

	if( m_pNonDBDM->GetRowCount() < 1 ) return 0;

	INT ids;

	ids = IsDuplicateRegNo(1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("EditNonBook") );

	if( ids > 0 ) return 1;
	
	INT nIndex;
	ids = GetCurrentPos(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("EditNonBook") );

	ids = EditNonBookDBProc(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("EditNonBook") );


	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("EditNonBook") );

	ids = ViewDetailInfo(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("EditNonBook") );

	AfxMessageBox( _T("수정되었습니다.") );
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::EditNonBookDBProc( INT nIndex )
{
	EFS_BEGIN

	CString sRegCode;
	CString regNo;
	CString sWorkingStatus;
	CString sRecKey;

	INT ids;

	ids = m_pNonDBDM->GetCellData( _T("책KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	ids = m_pNonDBCM->GetControlMgrData( _T("상태") , sWorkingStatus , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InsertNonBook") );

	ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InsertNonBook") );

	ids = m_pNonDBCM->GetControlMgrData( _T("등록번호") , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InsertNonBook") );

	if( sRegCode.GetLength() < 3 )
	{
		AfxMessageBox(_T("잘못된 등록번호입니다. 확인하시고 입력하십시오") );
		return 1;
	}

	ids = CLocCommonAPI::MakeRegNoSpaceToZero( m_pParentMgr , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );

	ids = m_pNonDBCM->SetControlMgrData( _T("등록번호") , sRegCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("InsertNonBook") );

	regNo = sRegCode;
	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;

	sRegCode = sRegCode.Left(nRegCodeLength);

	m_pNonDBDM->StartFrame();
	m_pNonDBDM->InitDBFieldData();

	ids = m_pNonDBDM->MakeControlMgrFiledData( ( CObject * )m_pNonDBCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("InsertNonBook") );

	ids = m_pNonDBDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sWorkingStatus , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("InsertNonBook") );

	ids = m_pNonDBDM->AddDBFieldData( _T("REG_CODE") , _T("STRING") , sRegCode , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("InsertNonBook") );
	
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , m_pNonDBDM );
	
	CString sLog;
	sLog.Format( _T("[%s] ID : %s") , CTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M")) , m_pParentMgr->GetUserID() );
	
	m_pNonDBDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	
	ids = pMobileApi->MakeUpdateFrame( _T("CO_NON_DB_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("InsertNonBook") );
	
	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("InsertNonBook") );
	ids = m_pNonDBDM->EndFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("InsertNonBook") );
	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );
	
	const INT ctrlCnt = 10;
	CString alias[ ctrlCnt ] = {
		_T("가격"), _T("관리구분"), _T("별치기호"), _T("본표제"), _T("이용대상구분"), _T("이용제한구분"), _T("저작자"), _T("출판사"), _T("형식구분"), _T("청구기호") };
	CString data[ ctrlCnt ];
	for( INT i = 0; i < ctrlCnt; i++ )
		m_pNonDBCM->GetControlMgrData( alias[ i ], data[ i ] );
	m_pNonDBDM->SetCellData( alias, ctrlCnt, data, nIndex );
	m_pNonDBDM->SetCellData( _T("등록구분"), sRegCode, nIndex );
	m_pNonDBDM->SetCellData( _T("등록번호"), regNo,	   nIndex );
	
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::DeleteNonBook()
{
	EFS_BEGIN

	if( m_pNonDBDM->GetRowCount() < 1 ) return 0;

	INT ids;
	INT nIndex;
	ids = GetCurrentPos(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	ids = DeleteNonBookDBProc( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InsertNonBook") );
	if( ids > 0 ) return 0;

	ids = m_pNonDBDM->DeleteRow( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InsertNonBook") );

	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );

	ids = ViewDetailInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );

	AfxMessageBox( _T("삭제되었습니다.") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::DeleteNonBookDBProc( INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sRecKey;
	CString sTableName = _T("CO_NON_DB_BOOK_TBL");
	CString sWorkingStatus;

	ids = m_pNonDBDM->GetCellData( _T("책상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );
#ifdef __K2UP__
	if( sWorkingStatus.Compare( _T("긴급대출자료") ) == 0 || sWorkingStatus.Compare( _T("BOL214O") ) == 0)
#else
	if( sWorkingStatus.Compare( _T("긴급대출자료") ) == 0 || sWorkingStatus.Compare( _T("BOL224O") ) == 0)
#endif
	{
		AfxMessageBox( _T("대출중인 자료이기 때문에 삭제할 수 없습니다.") );
		return 1;
	}

	if( IDYES != AfxMessageBox( _T("긴급자료 삭제시 대출이력도 함께 삭제됩니다.\r\n삭제하시겠습니까?"), MB_YESNO ) )
		return 1;


	ids = m_pNonDBDM->GetCellData( _T("책KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , m_pNonDBDM );

	m_pNonDBDM->StartFrame();

	m_pNonDBDM->InitDBFieldData();

	ids = pMobileApi->MakeDeleteFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	
	
	ids = pMobileApi->MakeDeleteFrame( _T("LS_WORK_T01"), _T("BOOK_KEY"), _T("NUMERIC"), sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );
	ids = pMobileApi->MakeDeleteFrame( _T("CO_LOAN_TBL"), _T("BOOK_KEY"), _T("NUMERIC"), sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InsertNonBook") );

	ids = m_pNonDBDM->EndFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InsertNonBook") );

	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );


	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::LoanNonBook()
{
	EFS_BEGIN

	if( m_pNonDBDM->GetRowCount() < 1 ) return 0;

	CString sRecKey;
	INT ids;

	INT nIndex;
	ids = GetCurrentPos(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LoanNonBook") );

	ids = m_pNonDBDM->GetCellData( _T("책KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("LoanNonBook") );

	CString sWorkingStatus;
	ids = m_pNonDBDM->GetCellData( _T("책상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("LoanNonBook") );
	
#ifdef __K2UP__
	if( sWorkingStatus.Compare(_T("BOL214O")) == 0 || (sWorkingStatus.Compare(_T("긴급대출자료"))==0)) 
#else
	if( sWorkingStatus.Compare(_T("BOL224O")) == 0 || (sWorkingStatus.Compare(_T("긴급대출자료"))==0)) 
#endif
	{
		AfxMessageBox(_T("이미 대출된 자료이기 때문에 대출할 수 없습니다.") );
		return 0;
	}

	CString sRegNo;
	ids = m_pNonDBDM->GetCellData( _T("등록번호") , nIndex , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("LoanNonBook") );
	

	ids = m_pLoanSpfWork->NoDBLoan( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("LoanNonBook") );
	if( ids > 0 ) return 1;

	ids = SucessLoan(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("LoanNonBook") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::GetCurrentPos( INT &nIndex )
{
	EFS_BEGIN

	CESL_Grid *pGrid = (CESL_Grid*)m_pNonDBGridCM->FindControl( m_sNonDBGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentPos") );

	nIndex = pGrid->GetIdx();

	if( nIndex > -1 ) pGrid->SetReverse( nIndex );

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::SetSelectGrid( INT nIndex )
{
	EFS_BEGIN

	CESL_Grid *pGrid = (CESL_Grid*)m_pNonDBGridCM->FindControl( m_sNonDBGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentPos") );

	if( nIndex > -1 )
	{
		pGrid->SetReverse( nIndex );
		pGrid->SetTopRow( nIndex+1 );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::SucessLoan( INT nIndex)
{
	EFS_BEGIN

	INT ids;
#ifdef __K2UP__
	CString sWorkingStatus = _T("BOL214O");
#else
	CString sWorkingStatus = _T("BOL224O");
#endif

	ids = m_pNonDBDM->SetCellData( _T("책상태") , sWorkingStatus , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SucessLoan") );

	ViewScreen();

	ViewDetailInfo(nIndex);

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_NON_DB_BOOK_PROC::IsDuplicateRegNo(INT nMode)
{
	EFS_BEGIN

	INT ids;

	CString sRegNo;
	CString sPreRegNo;

	ids = m_pNonDBCM->GetControlMgrData( _T("등록번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsDuplicateRegNo") );

	ids = CLocCommonAPI::MakeRegNoSpaceToZero( m_pParentMgr , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsDuplicateRegNo") );

	ids = m_pNonDBCM->SetControlMgrData( _T("등록번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsDuplicateRegNo") );

	CString sSql;
	sSql.Format( _T("SELECT COUNT(1) FROM CO_NON_DB_BOOK_TBL WHERE REG_NO = '%s' AND MANAGE_CODE = '%s' ") , sRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE );


	if( nMode == 1 )
	{
		INT nIndex;
		ids = GetCurrentPos( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsDuplicateRegNo") );
		ids = m_pNonDBDM->GetCellData( _T("등록번호") , nIndex , sPreRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsDuplicateRegNo") );
		sSql += _T(" AND REG_NO NOT IN ('") + sPreRegNo + _T("') ");
	}

	CString sValue;
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , m_pNonDBDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6, _T("IsDuplicateRegNo") );

	INT nEqualBookCnt=0;

	nEqualBookCnt += _ttoi(sValue);

	
	sSql.Format( _T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND MANAGE_CODE = '%s' ") , sRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE );


	if( nMode == 1 )
	{
		sSql += _T(" AND REG_NO NOT IN ('") + sPreRegNo + _T("') ");
	}

	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , m_pNonDBDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6, _T("IsDuplicateRegNo") );

	nEqualBookCnt += _ttoi(sValue);

	
	sSql.Format( _T("SELECT COUNT(1) FROM SE_BOOK_TBL WHERE REG_NO = '%s' AND MANAGE_CODE = '%s' ") , sRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE );

	
	if( nMode == 1 )
	{
		sSql += _T(" AND REG_NO NOT IN ('") + sPreRegNo + _T("') ");
	}
	
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , m_pNonDBDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6, _T("IsDuplicateRegNo") );
	
	nEqualBookCnt += _ttoi(sValue);
	

	if( nEqualBookCnt > 0 )
	{
		CString sMsg;
		sMsg.Format(_T("중복된 등록번호를 가진 자료가 존재하므로 추가할 수 없습니다."));
		AfxMessageBox( sMsg );
		return 1;
	}
	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_NON_DB_BOOK_PROC::ClearScreen()
{
	EFS_BEGIN

	m_pNonDBCM->AllControlClear();
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_NON_DB_BOOK_PROC::MoveToCat()
{
	EFS_BEGIN

	if( m_pNonDBDM->GetRowCount() < 1 ) return 0;
	
	INT ids;
	INT nIndex;		
	ids = GetCurrentPos(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveToCat") );

	
	CString sWorkingStatus;
	ids = m_pNonDBDM->GetCellData( _T("책상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertNonBook") );

#ifdef __K2UP__
	if( sWorkingStatus.Compare( _T("긴급대출자료") ) == 0 || sWorkingStatus.Compare( _T("BOL214O") ) == 0)
#else
	if( sWorkingStatus.Compare( _T("긴급대출자료") ) == 0 || sWorkingStatus.Compare( _T("BOL224O") ) == 0)
#endif
	{
		AfxMessageBox( _T("대출중인 자료이기 때문에 정리중으로 이동하실수 없습니다.") );
		return 1;
	}
		
	ids = MoveToCatDBProc(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveToCat") );	
	
	ids = m_pNonDBDM->DeleteRow( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InsertNonBook") );
	
	ids = ViewScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );
	
	ids = ViewDetailInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InsertNonBook") );
	
	return 0;

	EFS_END

	return -1;
}

INT CBL_LOC_NON_DB_BOOK_PROC::MoveToCatDBProc( INT nIndex )
{
	EFS_BEGIN
		
	INT ids;

	CString sWorkingStatus,sNewWorkingStatus;
	CString sRecKey;
	CString sTableName = _T("CO_NON_DB_BOOK_TBL");

	ids = m_pNonDBDM->GetCellData( _T("책상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveToCatDBProc") );

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	ids = m_pNonDBDM->GetCellData( _T("책KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveToCatDBProc") );
	
	sNewWorkingStatus = _T("BOC211O");
	
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , m_pNonDBDM );

	m_pNonDBDM->StartFrame();

	m_pNonDBDM->InitDBFieldData();

	m_pNonDBDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sNewWorkingStatus , TRUE );
	
#ifdef __K2UP__
	m_pNonDBDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") ,  sWorkingStatus , TRUE );
#endif	
	
	CString sLog;
	sLog.Format( _T("[%s] ID : %s") , CTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M")) , m_pParentMgr->GetUserID() );
	
	m_pNonDBDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );

	ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("MoveToCatDBProc") );

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveToCatDBProc") )

	m_pNonDBDM->EndFrame();
	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );
	return 0;

	EFS_END

	return -1;
}