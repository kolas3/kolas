// BLBookCheckProc.cpp: implementation of the CBLBookCheckProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLBookCheckProc.h"
#include <direct.h>
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define MAX_BLOB_BUFFER 20000000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLBookCheckProc::CBLBookCheckProc( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	
	// 화면 공통 작업 CLASS 설정
	m_pDBProc = new CDBProc( m_pParentMgr );
	if( m_pDBProc == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -1 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	// DB공통 작업 CLASS 설정
	m_pScreenPlay = new CScreenPlay( m_pParentMgr );
	if( m_pScreenPlay == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -2 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	m_pLocCommonAPI = new CLocCommonAPI( m_pParentMgr );
	if( m_pLocCommonAPI == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -3 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );


	// 장서 점검 대상 자료 DM , CM 설정
	m_sBookDMAlias = _T("DM_BO_LOC_2100");
	m_sBookCMAlias = _T("CM_BO_LOC_2100");

	m_pBookDM = m_pParentMgr->FindDM( m_sBookDMAlias );
	if( m_pBookDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -4 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );
	
	m_pBookCM = m_pParentMgr->FindCM( m_sBookCMAlias );
	if( m_pBookDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -5 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// 장서 점검 대상 자료 검색 CM설정
	m_sSearchBookCMAlias = _T("CM_BO_LOC_2120");

	// 등록번호에 관련된 DM
	m_sParentRegNoDMAlias = _T("DM_BO_LOC_2100_REG_NO");
	m_sChildRegNoDMAlias = _T("DM_BO_LOC_2110_TMP_REG_NO");

	m_pRegNoDM = m_pParentMgr->FindDM( m_sParentRegNoDMAlias );
	if( m_pRegNoDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -6 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// 기타 정보 DM
	m_sInDBEtcDMAlias = _T("DM_BO_LOC_2100_ETC");
	m_pInDBEtcDM = m_pParentMgr->FindDM( m_sInDBEtcDMAlias );
	if( m_pInDBEtcDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -7 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	m_sNotInDBEtcDMAlias = _T("DM_BO_LOC_2100_ETC_NOT_IN_DB");
	m_pNotInDBEtcDM = m_pParentMgr->FindDM( m_sNotInDBEtcDMAlias );
	if( m_pNotInDBEtcDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -7 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// 장서 점검 번호 DM
	m_sCheckNumberDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT");
	m_pCheckNumberDM = m_pParentMgr->FindDM( m_sCheckNumberDMAlias );
	if( m_pCheckNumberDM == NULL )ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -8 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	m_sForFileDM = _T("DM_BO_LOC_2100_FOR_MAKE_FILE");
	m_pForFileDM = m_pParentMgr->FindDM( m_sForFileDM );
	if( m_pForFileDM == NULL )ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -9 , _T("CBLBookCheckProc( CESL_Mgr *pWnd )") );

	// 첫화면의 기타 정보 CM
	m_sEtcInfoCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");

	// 다른 자료실에 존재하는 자료를 체크하기 위한 DM
	m_sTmpEtcInfoDMAlias = _T("DM_BO_LOC_2100_ETC_TMP");

}

CBLBookCheckProc::~CBLBookCheckProc()
{
	if( m_pDBProc != NULL ) delete m_pDBProc;
	if( m_pScreenPlay != NULL ) delete m_pScreenPlay;
	if( m_pLocCommonAPI != NULL ) delete m_pLocCommonAPI;
	m_pDBProc = NULL;
	m_pScreenPlay = NULL;
	m_pLocCommonAPI = NULL;
}

INT CBLBookCheckProc::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( BOOK_SEARCH ) == 0 )
	{
		ids = SearchBook();
	}
	else if( sPathAlias.Compare( OPEN_REGNO_EDIT ) == 0 )
	{
		ids = OpenRegNoEdit();
	}
	else if( sPathAlias.Compare( SAVE_REGNO_EDIT ) == 0 )
	{
		ids = SaveRegNoEdit();
	}
	else if( sPathAlias.Compare( BOOK_CHECK ) == 0 )
	{
		ids = BookCheck();
	}
	else if( sPathAlias.Compare( INITIAL_BOOK_CHECK ) == 0 )
	{
		ids = InitBookCheckYN();
	}
	else if( sPathAlias.Compare( INIT_BOOK_CHECK_NUMBER ) == 0 )
	{
		ids = InitBookCheckNumber();
	}
	else if( sPathAlias.Compare( OPEN_BOOK_CHECK_NO_LIST ) == 0 )
	{
		ids = OpenBookCheckNoList();
	}
	else if( sPathAlias.Compare( CHANGE_CHECK_NO ) == 0 )
	{
		ids = ChangeCheckNo();
	}
	else if( sPathAlias.Compare( BOOK_CHECK_END ) == 0 )
	{
		ids = BookCheckEnd();
	}
	else if( sPathAlias.Compare( CHANGE_BOOK_CHECK_YN ) == 0 )
	{
		ids = ChangeBookCheckYN();
	}
	else if( sPathAlias.Compare( OPTION_SELECT ) == 0 )
	{
		ids = OptionSelect();
	}
	else if( sPathAlias.Compare( MOVE_GRID ) == 0 )
	{
		ids = MoveGrid();
	}
	else if( sPathAlias.Compare( SEARCH_BOOK_CHECK_N ) == 0 )
	{
		ids = SearchBookCheckN();
	}
	else if( sPathAlias.Compare( DETAIL_VIEW ) == 0 )
	{
		ids = DetailView();
	}
	else if( sPathAlias.Compare( DETAIL_VIEW2 ) == 0 )
	{
		ids = DetailView2();
	}
	else 
	{
		return -1;
	}

	if( ids < 0 ) 
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다.") );

	return 0;

	EFS_END
	return -1;

}

// 장서점검 대상 자료 검색
INT CBLBookCheckProc::SearchBook()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;		//WHERE절
	// 1. SQL문 생성
	ids = GetSqlFromControl( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	// 2. DB에서 SELECT해온다.
	ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	// 3. 결과를 화면에 보여준다.
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBook()") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}



// 장서점검 대상 자료 검색 화면 -> WHERE절 생성
INT CBLBookCheckProc::GetSqlFromControl( CString &sWhere )
{
	EFS_BEGIN

	INT nMode;	// 0: 등록번호로 검색 , 1:청구기호로 검색

	// 0. 모드 설정
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( m_sSearchBookCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSqlFromControl( CString &sWhere )") );

	CESL_Control_Element *ebtnMode = pCM->FindControlElement( _T("등록번호체크") );
	if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSqlFromControl( CString &sWhere )") );

	CButton *btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));

	if( btnMode->GetCheck() == 1 ) 
		nMode = 0;
	else
		nMode = 1;

	INT nControlCnt;			// 이용될 컨트롤 수
	TCHAR **sControlFieldAlias;	// 컨트롤 알리아스
	sControlFieldAlias = NULL;
	TCHAR **sDBFieldName;		// DB 필드 이름
	sDBFieldName = NULL;
	INT *nDBFieldType;		// DB 필드 타입
	nDBFieldType = NULL;
	CString *sGetData;			// 콘트롤의 값
	sGetData = NULL;

	// 1. 컨트롤의 값을 얻어온다.
	// 2. DB Field명을 정해준다.	
	// 3. DB Field Type을 정해준다.
	if( nMode == 0 )
		nControlCnt = 4;
	else
		nControlCnt = 8;

	sControlFieldAlias = new TCHAR*[nControlCnt];
	sDBFieldName = new TCHAR*[nControlCnt];
	nDBFieldType = new INT[nControlCnt];
	sGetData = new CString[nControlCnt];

	INT ids = GetSearchConitionFromControl( nMode , sControlFieldAlias , sDBFieldName , nDBFieldType );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetSqlFromControl( CString &sWhere )") );

	// Control의 데이터값을 얻어온다.
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( m_sSearchBookCMAlias ,  sControlFieldAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlFromControl( CString &sWhere )") );
	}

	
	// 4. Where절을 생성한다.
	ids = m_pDBProc->MakeSql( sGetData , sDBFieldName , nDBFieldType , nControlCnt , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetSqlFromControl( CString &sWhere )") );

	// 장서 점검값도 WHERE절에 포함을 시킨다.
	CString sBookCheckYN;
	CString sSpecialWhere;
	for( INT j = 0 ; j < 2 ; j++ )
	{
		ids = m_pScreenPlay->GetBookCheckInitYN( j , sBookCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetSqlFromControl( CString &sWhere )") );

		if( sBookCheckYN.IsEmpty() ) continue;

		if( sSpecialWhere.GetLength() > 1 ) sSpecialWhere += _T(" OR ");
		sSpecialWhere += _T(" BB.BOOK_CHECK_YN = '") + sBookCheckYN + _T("'");

		sBookCheckYN.Empty();
	}

	if( !sSpecialWhere.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T("( ") + sSpecialWhere + _T(" )");
	}

	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SEARCH_CONDITION)+ _T(") ");

	// 기본으로 이전 배가 상태가 실시간 배가 자료인것은 검색이 되지 않게 한다.
	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) ");
	
	///@20071105 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실을 다수 입력 받을 수 있도록 변경	
	CString strShelfCode = GetShelfLocCode();
	if( !strShelfCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += " BB.SHELF_LOC_CODE IN (" + strShelfCode + ")";
	}
	///@--}}

	// 2009.02.25 ADD BY PDJ : MANAGE_CODE처리 추가
	sWhere += _T("AND BB.MANAGE_CODE=UDF_MANAGE_CODE");

	// 동적 할당한 메모리를 DELETE시킨다. 
	
	for( i = 0 ; i < nControlCnt ; i++ )
	{
		delete sControlFieldAlias[i];
		delete sDBFieldName[i];
	}
	
	delete []sControlFieldAlias;
	delete []sDBFieldName;
	delete []nDBFieldType;
	delete []sGetData;
	
	return 0;

	EFS_END
	return -1;

}

// nMode에 따라 0:등록번호 검색 1:청구기호로 검색
// 검색하는 컨트롤 필드 알리아스와 DBField Name과 Field Type을 구하는 함수
INT CBLBookCheckProc::GetSearchConitionFromControl( INT nMode , TCHAR **sControlFieldAlias , TCHAR **sDBFieldName , INT *nDBFieldType )
{
	EFS_BEGIN

	///@20071205 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실 조건을 별도로 입력한다.
	if( nMode == 0 )
	{
		const INT nTmpControlCnt = 3;
		TCHAR *sTmpControlFieldAlias[nTmpControlCnt] = 
		{
			/*_T("자료실명") , */_T("등록구분") , _T("등록번호1") , _T("등록번호2") 
		};
		TCHAR *sTmpDBFieldName[nTmpControlCnt] = 
		{
			/*_T("BB.SHELF_LOC_CODE") , */_T("BB.REG_CODE") , _T("BB.REG_NO") , _T("BB.REG_NO") 
		};
		INT nTmpDBFieldType[nTmpControlCnt] = 
		{
			/*ONESTRING , */THREESTRING , ETC , ETC
		};
		for( INT i = 0 ; i < nTmpControlCnt ; i++ )
		{
			sControlFieldAlias[i] = new TCHAR[_tcsclen(sTmpControlFieldAlias[i])+1];
			_tcscpy( sControlFieldAlias[i] , sTmpControlFieldAlias[i] );
			sDBFieldName[i] = new TCHAR[_tcsclen(sTmpDBFieldName[i])+1];
			_tcscpy( sDBFieldName[i] , sTmpDBFieldName[i] );
			nDBFieldType[i] = nTmpDBFieldType[i];
		}

	}
	else
	{
		const INT nTmpControlCnt = 4;
		TCHAR *sTmpControlFieldAlias[nTmpControlCnt] = 
		{
			/*_T("자료실명") , */_T("별치기호1") , _T("별치기호2") ,  _T("분류기호1") , _T("분류기호2") 
		};
		TCHAR *sTmpDBFieldName[nTmpControlCnt] = 
		{
			/*_T("BB.SHELF_LOC_CODE") , */_T("IB.SE_SHELF_CODE") , _T("IB.SE_SHELF_CODE")  , _T("IB.CLASS_NO") , _T("IB.CLASS_NO") 
		};
		INT nTmpDBFieldType[nTmpControlCnt] = 
		{
			/*ONESTRING , */TWOSTRING , ETC , TWOSTRING , ETC 
		};
		for( INT i = 0 ; i < nTmpControlCnt ; i++ )
		{
			sControlFieldAlias[i] = new TCHAR[_tcsclen(sTmpControlFieldAlias[i])+1];
			_tcscpy( sControlFieldAlias[i] , sTmpControlFieldAlias[i] );
			sDBFieldName[i] = new TCHAR[_tcsclen(sTmpDBFieldName[i])+1];
			_tcscpy( sDBFieldName[i] , sTmpDBFieldName[i] );
			nDBFieldType[i] = nTmpDBFieldType[i];
		}
	}
	///@--}}
	return 0;

	EFS_END
	return -1;

}


// 부모 등록번호 DM에서 자식 등록번호 DM으로 카피하는 함수
INT CBLBookCheckProc::OpenRegNoEdit()
{
	EFS_BEGIN

	//ids = m_pLocCommonAPI->CopyDM( m_sParentRegNoDMAlias , m_sChildRegNoDMAlias );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenRegNoEdit()") );

	return 0;

	EFS_END
	return -1;

}

// 자식 등록번호 DM에서 부모 등록번호 DM으로 카피하는 함수
INT CBLBookCheckProc::SaveRegNoEdit()
{
	EFS_BEGIN

	INT ids;

	//ids = m_pLocCommonAPI->CopyDM( m_sChildRegNoDMAlias , m_sParentRegNoDMAlias );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SaveRegNoEdit()") );


	// 화면에 등록번호수를 갱신한다.
	CESL_DataMgr *pRegNoDM = m_pParentMgr->FindDM( m_sParentRegNoDMAlias );
	if( pRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SaveRegNoEdit()") );

	CString sRegNoCnt;
	sRegNoCnt.Format( _T("%d") , pRegNoDM->GetRowCount() );

	ids = m_pParentMgr->SetControlData( m_sEtcInfoCMAlias , _T("등록번호수") , sRegNoCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SaveRegNoEdit()") ); 


	return 0;

	EFS_END
	return -1;

}

HANDLE				g_hEvent_Thread_BookCheck_1;
// 장서 체크 PATH
INT CBLBookCheckProc::BookCheck()
{
	EFS_BEGIN

	// 쓰레드로 동작하는 함수 생성
	CWinThread* pwin_hThread =	AfxBeginThread( BookCheckThread, this  );

	return 0;

	EFS_END
	return -1;

}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// 메인 그리드 결과 보이기
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------
void CBLBookCheckProc::Display_BookCheckProc()
{
	m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias , &m_cMapData_ETC ,&m_cMapData_NoDB );
}

// 장서 체크
INT CBLBookCheckProc::BookCheckProc()
{
	EFS_BEGIN

	INT ids;

	// 화면 작업 : 프로그래스바 초기화
	// DM 작업 : 기타 정보 DM 초기화
	ids = InitBookCheck();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;

	// DM 작업 : 등록번호가 같은 장서점검 대상자료의 초기값을 'Y'로 변경하고 
	//           존재하지 않는 등록번호는 기타 정보DM에 입력한다.
	// 화면 작업 : 그리드의 값도 같이 변경한다.
	ids = ChangeRegNoCheckY();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;

	// 자료실 결정
	CString sShelfLocCode;
	///@20071205 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실을 복수개 선택할 수 있도록 변경
	/*//before	
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO") , _T("자료실명") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckProc()") );
	*///after
	sShelfLocCode = GetShelfLocCode();
	///@--}}
	
	ids = m_pParentMgr->SetDataMgrData( _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT") , _T("대상자료실") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckProc()") );
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 결과 수집후 메인 DM 에 넣기 , 쓰레드 종료 메세지 보내기
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
	ViewGridBookCheck();
	SendMessage( m_pParentMgr->m_hWnd , WM_USER + 2011 , 0 , 0 );

	/*
	// 결과물 생성
	// 소장 자료 , 기타 자료 , 분실 자료 , 비정상 자료수를 화면에 보여줌
	ids = MakeResultBookCheck();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckProc()") );
	if( ids > 0 ) return ids;
	*/
	return 0;

	EFS_END
	return -1;

}


// 프로그래스바 초기화
// 기타 정보 DM 초기화
INT CBLBookCheckProc::InitBookCheck()
{
	EFS_BEGIN

	INT ids;

	
	INT nMaxCnt;
	if( m_bIsRegNoInDM )
		nMaxCnt = m_pRegNoDM->GetRowCount();
	else
		nMaxCnt = m_regNo.GetSize();
	
	if( nMaxCnt < 1 )
	{
		AfxMessageBox( _T("비교할 등록번호가 존재하지 않습니다.\r\n등록번호 편집을 먼저 하십시오.") );
		return 1;
	}

	// 프로그래스바 초기화
	ids = m_pScreenPlay->InitProgressBar( nMaxCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck()") );


	// 기타 정보 DM 초기화
	m_pInDBEtcDM->FreeData();
	m_pNotInDBEtcDM->FreeData();

	return 0;

	EFS_END
	return -1;

}

// 등록번호목록과 장서점검 대상자료 목록을 비교
INT CBLBookCheckProc::ChangeRegNoCheckY()
{
	EFS_BEGIN

	INT ids;

	CString sRegNoAlias = _T("등록번호") ;
	// 등록번호 DM
	INT nRegNoDMRowCount;
	if( m_bIsRegNoInDM )
		nRegNoDMRowCount = m_pRegNoDM->GetRowCount();
	else
		nRegNoDMRowCount = m_regNo.GetSize();

	if( !nRegNoDMRowCount )
		return 0;
	// 비소장자료 DM
	INT nBookDMRowCount = m_pBookDM->GetRowCount();

	CString sRegNo;
	CString sBookCheckRegNo;

	bool bEqual = FALSE;	//소장 자료가 존재하는가?

	CString sPublishForm;



	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 원래 m_pBookDM DM (맨처음 그리드의 매칭DM) 카피본으로 셀렉트.

	/*
	// 등록번호를 조건으로 SELECT할 DM
	CESL_DataMgr *pSelectDM = m_pParentMgr->FindDM( _T("DM_BLBookCheckProc_SELECT") );
	if( !pSelectDM )
		return -1;
	*/
	m_pBookDM->FreeData();
	CESL_DataMgr* pSelectDM = new CESL_DataMgr();
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , m_pBookDM , pSelectDM );	
	// 입력순서대로 그리드에 뿌리기 위한 임시 저장소 초기화
	m_cMapData_ETC.Clear();
	m_cMapData_NoDB.Clear();
	m_cMapData_ALL.Clear();
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	// 있는 자료를 UPDATE할 DM
	CESL_DataMgr *pUpdateDM = m_pParentMgr->FindDM( _T("DM_BLBookCheckProc_UPDATE") );
	if( !pUpdateDM )
		return -1;

	// 시간재기
	CTime tStart = CTime::GetCurrentTime();
	
	INT startIndex, lastIndex;
	INT loopCnt = ( nRegNoDMRowCount % DEVIDED_CHECK_CNT ) ? ( nRegNoDMRowCount / DEVIDED_CHECK_CNT ) + 1 : ( nRegNoDMRowCount / DEVIDED_CHECK_CNT );
	CString query;
	CString shelfLocCode = GetShelfLocCode();
	for( INT i = 0; i < loopCnt; i++ )
	{
		// 짤라보낼 시작 index
		startIndex = i * DEVIDED_CHECK_CNT;
		// 짤라보낼   끝 index
		lastIndex  = startIndex + DEVIDED_CHECK_CNT - 1;
		lastIndex  = ( lastIndex > nRegNoDMRowCount - 1 ) ? nRegNoDMRowCount - 1 : lastIndex;

		// 짤라서 검색할 쿼리를 만든다.
		query = MakeSelectQuery( m_pRegNoDM, startIndex, lastIndex );

		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// 바뀐 DM 으로 셀렉트
		//ids = pSelectDM->RefreshDataManager( query );
		ids = m_pDBProc->MakeDMFromDB( _T("") , query , NULL , pSelectDM );
		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
		
		// 등록번호를 검사한다. 
		CheckRegNo( pSelectDM, pUpdateDM, startIndex, lastIndex, shelfLocCode );

		if( m_bBreak )
			return 100;
	}
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	delete	pSelectDM;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	CTime tEnd = CTime::GetCurrentTime();
	CTimeSpan ts = tEnd - tStart;
	m_sBookCheckSuccessMsg.Format( _T("\n\n\n")
				_T("시작시간	: %04d/%02d/%02d - %02d:%02d:%02d\n")
				_T("종료시간	: %04d/%02d/%02d - %02d:%02d:%02d\n")
				_T("소요시간	: %4d분 %02d초"),
				tStart.GetYear(), tStart.GetMonth(), tStart.GetDay(), tStart.GetHour(), tStart.GetMinute(), tStart.GetSecond(),
				tEnd.GetYear(), tEnd.GetMonth(), tEnd.GetDay(), tEnd.GetHour(), tEnd.GetMinute(), tEnd.GetSecond(),
				ts.GetTotalMinutes(), ts.GetSeconds() );

	
	
	/*
	for( INT i = 0 ; i < nRegNoDMRowCount ; i++ )
	{
		//--------------------------------------------------------------
		// 프로그래스 바 셋팅
		m_pScreenPlay->ProgressSetPos(i+1);
		//--------------------------------------------------------------
		// 멈출 것인지 조사--------------------------------------------------
		if( m_bBreak ) return 100;
		//--------------------------------------------------------------------

		bEqual = FALSE;

		ids = m_pRegNoDM->GetCellData( sRegNoAlias , i , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeRegNoCheckY()") );
		
		ids = IsBoBookTbl( sRegNo , sPublishForm);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeRegNoCheckY()") );
		if( ids == 0 )
		{
			// 소장 자료 셋팅
			ids = SettingInLib( sRegNo , sPublishForm );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeRegNoCheckY()") );
			bEqual = TRUE;
		}

		// 기타 정보 DM INSERT
		if( bEqual ) continue;
		
		if( ids == 1)
		{
			// TMP ETC DM -ADD-> ETC DM
			ids = m_pLocCommonAPI->AddDM( m_sTmpEtcInfoDMAlias , m_sInDBEtcDMAlias , m_pParentMgr );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeRegNoCheckY()") ); 
		}else if( ids == 2 )
		{
			m_pNotInDBEtcDM->InsertRow(-1);

			ids = m_pNotInDBEtcDM->SetCellData( sRegNoAlias , sRegNo , m_pNotInDBEtcDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeRegNoCheckY()") ); 
		}
	}
	*/

	return 0;

	EFS_END
	return -1;

}

// 등록번호가 sRegNo인 자료가 DB에 존재하는 지 조사
// return 0 : 소장자료이다.
//        음수 : 에러
//        1 : 기타 자료 ( 다른 자료실에 존재 )
//        2 : 존재하지 않는 자료
INT CBLBookCheckProc::IsBoBookTbl( CString sRegNo , CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sControlAlias = _T("자료실명");

	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsBoBookTbl()") ); 
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();

	ids = m_pDBProc->IsRegNo( m_pBookDM , sRegNo , sShelfLocCode , sPublishForm );
	return ids;

	EFS_END
	return -1;

}

// 결과물 생성
// 소장 자료 , 기타 자료 , 분실 자료 , 비정상 자료수를 화면에 보여줌
INT CBLBookCheckProc::MakeResultBookCheck()
{
	EFS_BEGIN

	INT ids;

	INT nBookCheckY = 0;	// 소장자료
	INT nBookCheckN = 0;	// 분실자료
	INT nBookCheckX = 0;	// 비정상자료
	INT nEtc = 0;			// 기타 자료
	INT nTotalRegNoCnt = 0;		// 등록번호 자료수
	INT nTotalBookCheckCnt =0;	// 장서점검 대상 자료수

	if( m_bIsRegNoInDM )
		nTotalRegNoCnt = m_pRegNoDM->GetRowCount();
	else
		nTotalRegNoCnt = m_regNo.GetSize();

	nTotalBookCheckCnt = m_pBookDM->GetRowCount();
	
	ids = GetBookCheckCnt( nBookCheckY , nBookCheckN , nBookCheckX );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeResultBookCheck()") ); 

//	nEtc = m_pEtcDM->GetRowCount();

	ids = m_pScreenPlay->ViewBookCheckResult( nBookCheckY , nBookCheckN , nBookCheckX , nEtc , nTotalRegNoCnt , nTotalBookCheckCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeResultBookCheck()") ); 

	return 0;

	EFS_END
	return -1;

}

// 소장 자료 , 분실 자료 , 비정상 자료를 얻어오는 함수
INT CBLBookCheckProc::GetBookCheckCnt( INT &nBookCheckY , INT &nBookCheckN , INT &nBookCheckX )
{
	EFS_BEGIN

	INT ids;

	INT nRowCount = m_pBookDM->GetRowCount();

	CString sBookCheck;

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pBookDM->GetCellData( _T("초기화구분") , i , sBookCheck );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookCheckCnt( INT &nBookCheckY , INT &nBookCheckN , INT &nBookCheckX )") ); 

		if( sBookCheck.Compare( _T("Y") )==0 ) 
			nBookCheckY++;
		else if( sBookCheck.Compare( _T("N") )==0 ) 
			nBookCheckN++;
		else if( sBookCheck.Compare( _T("X") )==0 ) 
			nBookCheckX++;
	}

	return 0;

	EFS_END
	return -1;

}

bool CBLBookCheckProc::GetThreadState()
{
	EFS_BEGIN

	return m_bStop;

	EFS_END
	return FALSE;

}

INT CBLBookCheckProc::SetStopThread()
{
	EFS_BEGIN

	m_bBreak = TRUE;
	return 0;	

	EFS_END
	return -1;

}

INT CBLBookCheckProc::SetBookCheckButton()
{
	EFS_BEGIN

	m_pScreenPlay->SetBookCheckButton();

	return 0;

	EFS_END
	return -1;

}

// 소장 자료일 경우 초기화 구분값을 Y로 변경한다.
// DB작업
INT CBLBookCheckProc::SettingInLib( CString sRegNo , CString sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = m_pDBProc->ChangeBookCheckYN( m_pBookDM , _T("Y") , sRegNo , sPublishForm , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck") );

	return 0;

	EFS_END
	return -1;

}

// 쓰레드 함수
UINT BookCheckThread(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBLBookCheckProc *pObj = (CBLBookCheckProc *)lParam;
	if ( pObj == NULL )
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->BookCheckProc();
	if (ids == 100 ) sErrorMsg = _T("장서 체크가 중지되었습니다.");
	else if (ids < 0) sErrorMsg.Format(_T("장서 체크 에러 발생-%d"),ids );
	else if (ids == 0) sErrorMsg = _T("장서 체크가 완료되었습니다.") + pObj->m_sBookCheckSuccessMsg;
	if( !sErrorMsg.IsEmpty() ) AfxMessageBox(sErrorMsg);


	pObj->m_bStop = TRUE;


	SetEvent( g_hEvent_Thread_BookCheck_1 );
	return ids;
}


// 초기화
INT CBLBookCheckProc::InitBookCheckYN()
{
	EFS_BEGIN

	INT ids;
	// 1. SQL문 생성 Frame 생성 , DB에 보낸다.
	ids = MakeInitBookCheckFrame( m_pBookDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheck") );
	if( ids > 0 ) return ids;

	// 2. 그리드 초기화
	ids = InitGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitBookCheck") );
	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::InitGrid()
{
	EFS_BEGIN

	INT ids;

	m_pBookDM->FreeData();
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitGrid") );

	return 0;

	EFS_END
	return -1;

}

// 초기화 UPDATE FRAME 생성
INT CBLBookCheckProc::MakeInitBookCheckFrame( CESL_DataMgr *pDM )
{
	EFS_BEGIN

	INT ids;

	// 콘트롤에서 값을 가져온다.
	///@20071205 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실을 다수 선택할 수 있도록 변경
	/*//before	
	CString sCMAlias = _T("CM_BO_LOC_2150");
	CString sParentCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");

	const INT nCnt = 1;
	TCHAR *sControlAlias[nCnt] = 
	{
		_T("자료실명") 
	};

	CString sGetData[nCnt];
	for( INT i=0 ; i<nCnt ;i++ )
	{
		ids = m_pParentMgr->GetControlData( sParentCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}
	*///after
	CString sCMAlias = _T("CM_BO_LOC_2150");
	CString sParentCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sGetData[1];
	sGetData[0] = GetShelfLocCode();
	///@--}}
	

	CString sUpdateFrame;
	CString sUpdate;
	CString sWhere;

	//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
	CESL_ControlMgr* pMainCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_2100"));
	if( pMainCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 4 , _T("MakeInitBookCheckFrame") ); 
	
	CESL_Control_Element* pIncludeAppendix = pMainCM->FindControlElement( _T("부록포함체크") );
	if( pIncludeAppendix == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 5 , _T("MakeInitBookCheckFrame") ); 
	
	CButton* btn1 = (CButton*)(m_pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
	if( btn1 == NULL ) {
		btn1 = (CButton*)(m_pParentMgr->pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		if( btn1 == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , - 6 , _T("MakeInitBookCheckFrame") ); 
	}


	TCHAR *sWorkingStatus[8] = 
	{
		_T(HOLD_SEARCH_CONDITION),
		// 17/07/20 김혜영 : 책나래대출자료 상태 추가 
/*//BEFORE-----------------------------------------------------------------------------
		//=====================================================
		//2009.03.26 UPDATE BY PJW :  단행 상호대차 자료상태 추가
// 		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O'"),
		// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL511O'"), 
		//_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O'"), 
		//=====================================================
*///AFTER------------------------------------------------------------------------------
		// 18.09.27 JMJ KOL.RED.2018.008
		_T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O'"), 
/*//END-------------------------------------------------------------------------------*/		
		_T("'BOL112N'") ,
		_T("'BOL115O'") , 
		_T("'BOL252N'") , 
		_T("'BOL113O'") , 
		_T("'BOL114O'") ,
		_T("'BOL116O'")
	};
	
	pDM->StartFrame();

	// 1. 전체
	// 단행 자료만...
	if( sGetData[0].IsEmpty() )
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sWorkingStatus[0] );
		else
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_APPENDIX_FLAG='B' AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sWorkingStatus[0] );
	else 
	{
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'N' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sWorkingStatus[0]);
		else		
		sWhere.Format( _T("WHERE BOOK_APPENDIX_FLAG='B' AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sWorkingStatus[0]);
	}

	sUpdateFrame = sUpdate + sWhere;
	pDM->AddFrame( sUpdateFrame );

	// 연속 간행 자료만...
	TCHAR *sSEWorkingStatus[8] = 
	{
		_T(HOLD_SE_SEARCH_CONDITION) , 
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			_T("'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O'") ,
			// 18.09.27 JMJ KOL.RED.2018.008
			_T("'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL316O','SEL317O','SEL411O','SEL511O'") ,
			//=====================================================
			_T("'SEL212N'") ,
			_T("'SEL215O'") , 
			_T("'SEL216O'") , 
			_T("'SEL213O'") , 
			_T("'SEL214O'") ,
			_T("'BOL217O'")
	};
	if( sGetData[0].IsEmpty() )
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_TYPE IN ('B','C','A') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sSEWorkingStatus[0] );
		else
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' WHERE BOOK_TYPE IN ('B','C') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sSEWorkingStatus[0] );
	else 
	{
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'N' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C','A')  AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sSEWorkingStatus[0]);
		else
		sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C')  AND SHELF_LOC_CODE IN (%s) AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE ;") , sGetData[0] , sSEWorkingStatus[0]);
	}
	
	sUpdateFrame = sUpdate + sWhere;
	pDM->AddFrame( sUpdateFrame );

	const INT nButtonCnt = 7;
	CString sCheckButtonAlias[nButtonCnt] = 
	{
		_T("대출"),_T("배가"),_T("제적"),_T("분실"),_T("수리/제본"),_T("파손"),_T("재정리")
	};

	CESL_Control_Element *ebtnMode;
	CButton *btnMode;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );
	for( INT i = 1 ; i < nButtonCnt+1 ; i++ )
	{

		ebtnMode = pCM->FindControlElement( sCheckButtonAlias[i-1] );
		if( ebtnMode == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3, _T("IsCanCheck()") );
		btnMode = (CButton*)(m_pParentMgr->GetDlgItem( ebtnMode->CTRL_ID ));

		if( btnMode->GetCheck() != 1 ) continue;

		// 단행
		sUpdate.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'Y' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sWorkingStatus[i] );
		else
		sWhere.Format( _T("WHERE BOOK_APPENDIX_FLAG='B' AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sWorkingStatus[i] );

		if( !sGetData[0].IsEmpty() )
			sWhere +=  _T("AND SHELF_LOC_CODE IN (")+sGetData[0]+_T(")");
		
		sUpdateFrame = sUpdate + sWhere + _T(";");

		pDM->AddFrame( sUpdateFrame );

		// 연속
		sUpdate.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'Y' ") );
		//2020.03.27 JOB.2020.0021 add by KYJ : 장서점검 부록 선택 옵션 추가
		if ( btn1->GetCheck() == TRUE )
			sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C','A') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sSEWorkingStatus[i] );
		else
		sWhere.Format( _T("WHERE BOOK_TYPE IN ('B','C') AND WORKING_STATUS IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE") , sSEWorkingStatus[i] );

		if( !sGetData[0].IsEmpty() )
			sWhere +=  _T("AND SHELF_LOC_CODE IN (")+sGetData[0]+_T(")");
		
		sUpdateFrame = sUpdate + sWhere + _T(";");
		
		pDM->AddFrame( sUpdateFrame );
		
	}
	
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeInitBookCheckFrame( CESL_DataMgr *pDM )") );

	ids = pDM->EndFrame();

	return 0;

	EFS_END
	return -1;

}

// 점검 번호 초기화
INT CBLBookCheckProc::InitBookCheckNumber()
{
	EFS_BEGIN

	INT ids;

	CString sCheckNumber;

	// 1. DB에서 새로 부여될 점검번호값 가져오기
	ids = m_pDBProc->GetMaxCheckNumber( m_pBookDM , sCheckNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitBookCheckNumber()") );
	
	// DM 에 인서트
	sCheckNumber.Format( _T("%d") , _ttoi( sCheckNumber ) + 1 );

	CString sShelfLocCode;
	if( m_pCheckNumberDM->GetRowCount() > 0 )
	{
		ids = m_pCheckNumberDM->GetCellData( _T("대상자료실") , 0 , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("InitBookCheckNumber") );
	}

	m_pCheckNumberDM->FreeData();
	m_pCheckNumberDM->InsertRow(-1);
	ids = m_pCheckNumberDM->SetCellData( _T("점검번호") , sCheckNumber , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("InitBookCheckNumber()") );

	// 점검년도도..
	CString sCheckYear;
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	sCheckYear.Format(_T("%d") ,(CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData)).GetYear());
// 	sCheckYear.Format(_T("%d") ,(CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear());	
	
	ids = m_pCheckNumberDM->SetCellData( _T("장서점검년도") , sCheckYear , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InitBookCheckNumber()") );
	
	ids = m_pCheckNumberDM->SetCellData( _T("대상자료실") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("InitBookCheckNumber()") );

	// 2. 화면에 보여주기
	ids = m_pScreenPlay->ViewCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("InitBookCheckNumber()") );

	return 0;

	EFS_END
	return -1;

}


// 점검 번호 변경의 화면이 열리때 일어나는 플로우
INT CBLBookCheckProc::OpenBookCheckNoList()
{
	EFS_BEGIN

	INT ids;
	
	// 1. DM 구성하기
	ids = m_pDBProc->MakeCheckNoDMToDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenBookCheckNoList()") );

	// 2. 화면에 보여주기
	ids = m_pScreenPlay->ViewCheckNoList();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OpenBookCheckNoList()") );

	return 0;

	EFS_END
	return -1;

}

// 점검 번호 변경
INT CBLBookCheckProc::ChangeCheckNo()
{
	EFS_BEGIN

	INT ids;

	// DM 이 존재하는 지 검사
	CString sDMAlias = _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT_LIST");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( sDMAlias );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeCheckNo()") );

	if( pTmpDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("변경될 점검 번호가 존재하지 않습니다.") );
		return 1;
	}

	// 선택된것을 점검번호 DM으로 이동
	INT nIndex = m_pScreenPlay->GetBookCheckSelectIndex();
	const INT nCopyCnt = 4;
	TCHAR *sFieldAlias[nCopyCnt] = 
	{
		_T("장서점검년도") , _T("점검번호") , _T("장서점검KEY") , _T("대상자료실")
	};

	CString sData;

	for( INT i = 0 ; i < nCopyCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sFieldAlias[i] , nIndex , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeCheckNo()") );

		if( i == 3 )
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분") , sData , sData );

		ids = m_pCheckNumberDM->SetCellData( sFieldAlias[i] , sData , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeCheckNo()") );
	}

	// 점검 번호를 보여줌.
	ids = m_pScreenPlay->ViewCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeCheckNo()") );

	// 초기화면의 변경에서 라면 
	// 이미 과거에 한번 점검했다면 그 내용을 불러온다.
	// 나머지 부분에서는 이 일을 하지 않는다.
	if( m_nLoadSPF == 0 )
	{
		ids = LoadPreBookCheckInfo();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeCheckNo()") );
	}

	return 0;

	EFS_END
	return -1;

}


// 장서 점검 완료
INT CBLBookCheckProc::BookCheckEndProc()
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;

	// 화면 ProgressBar , 로그 기록 ----------------------------------------------------------------
	ids = m_pScreenPlay->InitResultScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );


	// 1. 점검 번호 생성
	ids = MakeBookCheckNo( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );


	// 2. 분실 자료 처리
	ids = LostBookProc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckEndProc()") ); 
	

	// 3. 결과 파일 생성
	ids = MakeBookCheckResulFile();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckEndProc()") ); 


	// 4. 점검 테이블 업데이트
	//    기타 테이블 인서트	
	ids = BookCheckResultDBUpdate();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckEndProc()") ); 
	

	// 화면 ProgressBar , 로그 기록 ----------------------------------------------------------------
	ids = m_pScreenPlay->SetResultScreen( 100 , _T("작업 완료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );

	/*
	// 점검번호 생신하기
	ids = InitBookCheckNumber();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("BookCheckEndProc()") );
	*/
	
	return 0;

	EFS_END
	return -1;

}

// 점검 번호 생성
INT CBLBookCheckProc::MakeBookCheckNo( CString &sRecKey )
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 1 , _T("점검 번호 생성중..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("BookCheckEndProc()") );

	// 점검 번호가 존재하는지 검사
	ids = m_pCheckNumberDM->GetCellData( _T("장서점검KEY") , 0 , sRecKey );
	if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );


	if( sRecKey.IsEmpty() )
	{
		CString sCheckNo;
		ids = m_pCheckNumberDM->GetCellData( _T("점검번호") , 0 , sCheckNo );
		if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckEndProc()") );
		CString sCheckYear;
		ids = m_pCheckNumberDM->GetCellData( _T("장서점검년도") , 0 , sCheckYear );
		if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckEndProc()") );
		
		// 점검 번호가 DB에 존재하지 않는다면 생성
		ids = m_pDBProc->MakeBookCheckResultRecord( m_pBookDM , sCheckNo , sCheckYear , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckEndProc()") );

		// DM에 입력
		ids = m_pCheckNumberDM->SetCellData( _T("장서점검KEY") , sRecKey , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckEndProc()") );
	}

	ids = m_pScreenPlay->SetResultScreen( 10 , _T("점검 번호 생성 완료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("BookCheckEndProc()") );

	return 0;

	EFS_END
	return -1;

}

// 결과 파일 생성
INT CBLBookCheckProc::MakeBookCheckResulFile()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 21 , _T("장서 점검 결과 파일 생성중..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );
/*
	// 1. 소장 목록 파일 생성
	ids = MakeResultFile(0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResulFile()") );
*/
	// 2. 분실 목록 파일 생성
	ids = MakeResultFile(1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeBookCheckResulFile()") );

	// 3. 기타 목록 파일 생성
	ids = MakeEtcResultFile();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeBookCheckResulFile()") );

	ids = m_pScreenPlay->SetResultScreen( 80 , _T("장서 점검 결과 파일 생성 완료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -17 , _T("MakeBookCheckResulFile()") );

	return 0;

	EFS_END
	return -1;

}

// 점검 테이블 업데이트 
INT CBLBookCheckProc::BookCheckResultDBUpdate()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 81 , _T("결과 파일 업로드중..") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -18 , _T("BookCheckResultDBUpdate()") );

	// 결과 파일 UPLOAD
	INT nCnt = 4;
	CString sFileName;
	CString sCheckNoKey;
	ids = m_pCheckNumberDM->GetCellData( _T("장서점검KEY") , 0 , sCheckNoKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("BookCheckResultDBUpdate()") );
	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_2100_BOOK_CHECK_RESULT") , _T("대상자료실") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("BookCheckResultDBUpdate()") ); 
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();

	for( INT i = 1 ; i < nCnt ; i++ )
	{
		ids = GetFileName( i , sFileName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("BookCheckResultDBUpdate()") );
		ids = m_pDBProc->UploadResultFile( m_pBookDM , i , sFileName , sCheckNoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("BookCheckResultDBUpdate()") );
	}

	ids = m_pDBProc->EndBookCheckDBProc( m_pInDBEtcDM , m_pNotInDBEtcDM , sCheckNoKey , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("BookCheckResultDBUpdate()") );

	ids = m_pScreenPlay->SetResultScreen( 100 , _T("결과 파일 업로드 완료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -19, _T("BookCheckResultDBUpdate()") );

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::BookCheckEnd()
{
	EFS_BEGIN

	// 쓰레드로 동작하는 함수 생성
	AfxBeginThread( BookCheckEndThread, this );

	return 0;

	EFS_END
	return -1;

}

// 분실 자료 처리
INT CBLBookCheckProc::LostBookProc()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->SetResultScreen( 11 , _T("분실 자료 처리중...") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("LostBookProc()") );

	///@20071105 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실을 다수 선택할 수 있게 변경
	/*//before	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO") , _T("자료실명") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LostBookProc()") ); 

	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	*///after
	CString sShelfLocCode = GetShelfLocCode();
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	///@--}}	

	CString sWorkingStatus;

	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("분실자료처리") , sWorkingStatus , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("LostBookProc()") ); 

	ids = m_pDBProc->UpdateLostBookWorkingStatus( m_pBookDM , sShelfLocCode , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("LostBookProc()") ); 
	
	ids = m_pScreenPlay->SetResultScreen( 20 , _T("분실 자료 처리 완료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("LostBookProc()") );

	return 0;

	EFS_END
	return -1;

}

// 결과 파일 생성
INT CBLBookCheckProc::MakeResultFile(INT nMode)
{
	EFS_BEGIN

	INT ids;

	ids = m_pDBProc->GetDBForFile( m_pForFileDM , nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeResultFile()") ); 

	CString sFileName;
	ids = GetFileName( nMode , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pForFileDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeResultFile()") ); 

	CString sMsg;
	INT nProgress;
	if( nMode == 0 )
	{
		nProgress = 40;
		sMsg.Format( _T("소장 목록 파일이 '") );
	}
	else if( nMode == 1 )
	{
		nProgress = 60;
		sMsg.Format( _T("분실 자료 목록 파일이 '") );
	}
	sMsg = sMsg + sFileName + _T("' 에 생성되었습니다.");
	ids = m_pScreenPlay->SetResultScreen( nProgress , sMsg );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );


	return 0;

	EFS_END
	return -1;

}


// 쓰레드 함수
UINT BookCheckEndThread(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBLBookCheckProc *pObj = (CBLBookCheckProc *)lParam;
	if ( pObj == NULL )
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->BookCheckEndProc();
	if (ids == 100 ) sErrorMsg = _T("장서 점검 완료 중지");
	else if (ids < 0) sErrorMsg.Format(_T("장서 점검 완료 에러 발생-%d"),ids );
	else if (ids == 0) sErrorMsg = _T("장세 점검이 완료되었습니다.");

	pObj->m_bStop = TRUE;

	if (ids < 0  && ids != 100){
		AfxMessageBox(sErrorMsg);
		return 0;
	}
	if( ids > 0 ) 
		return 0;

	return 0;
}


// 만들어질 파일 이름을 얻어오는 함수
// nMode 0: 소장목록 파일 , 1: 분실목록 파일 , 2: 기타목록 파일 , 3: 기타목록 파일2
INT CBLBookCheckProc::GetFileName( INT nMode , CString &sFileName )
{
	EFS_BEGIN

	INT ids;

	CString sPath = _T("..\\BookCheck");
	_tmkdir( sPath );

	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tCurrent = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr,strGetData);
// 	CTime tCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);

	// 점검번호
	CString sCheckNo;
	
	ids = m_pCheckNumberDM->GetCellData( _T("점검번호") , 0 , sCheckNo );
	if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetFileName()") );

	sPath += _T("\\") + sCheckNo;
	_tmkdir( sPath );

	// 현재일
	sPath += _T("\\") + tCurrent.Format(_T("%Y%m%d"));
	_tmkdir( sPath );

	if( nMode == 0 ) 
		sFileName = sPath + _T("\\소장목록.txt");
	else if( nMode == 1 )
		sFileName = sPath + _T("\\분실목록.txt");
	else if( nMode == 2 )
		sFileName = sPath + _T("\\기타목록1.txt");
	else if( nMode == 3 )
		sFileName = sPath + _T("\\기타목록2.txt");

	return 0;

	EFS_END
	return -1;

}

// 기타 파일 생성 
INT CBLBookCheckProc::MakeEtcResultFile()
{
	EFS_BEGIN

	INT ids;
	
	CString sFileName;
	ids = GetFileName( 2 , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeEtcResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pInDBEtcDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeEtcResultFile()") ); 

	CString sMsg;
	sMsg.Format( _T("기타 파일1이 '%s'에 생성되었습니다.") , sFileName );

	ids = m_pScreenPlay->SetResultScreen( 70 , sMsg  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );


	ids = GetFileName( 3 , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeEtcResultFile()") ); 

	ids = SanFileAPI::PortableScannerFileSave( m_pNotInDBEtcDM , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeEtcResultFile()") ); 

	sMsg.Format( _T("기타 파일2가 '%s'에 생성되었습니다.") , sFileName );

	ids = m_pScreenPlay->SetResultScreen( 80 , sMsg  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("MakeBookCheckResulFile()") );

	return 0;

	EFS_END
	return -1;

}

// 값변경
// 그리드에 체크가 되어 있는 부분을 변경
INT CBLBookCheckProc::ChangeBookCheckYN()
{
	EFS_BEGIN

	INT ids;


	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckYN()") ); 

	INT nRowCount = pGrid->GetCount();

	if( nRowCount < 1 ) return 0;

	INT nMode;
	ids = m_pScreenPlay->GetBookChangeMode( nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN()") ); 

	ids = m_pScreenPlay->ChangeBookCheckPrgressBarInit( nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeBookCheckYN()") ); 

	CString sCheck;
	CString sBookCheckYN;
	CString sRegNo;
	CString sBookKey;

	TCHAR *sDMColumnAlias[1] = { _T("선정") };

	INT nColIndex[1] = { -1 };

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN") );

	INT nCheckColIndex = nColIndex[0];

	CString sPublishForm;

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	CString sCheckResult_NoDB;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pScreenPlay->ChangeBookCheckPrgressBarSetPos(i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeBookCheckYN()") ); 

		ids = pGrid->GetAt( i , nCheckColIndex , sCheck );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ChangeBookCheckYN()") ); 

		if( sCheck.Compare(_T("V"))!=0 )
		{
			m_pBookDM->SetCellData( _T("선정") , _T("") , i );
		}
		if( sCheck.Compare(_T("V"))!=0 ) continue;
		
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// "점검결과" 가  "DB없음" 이면 안함.
		m_pBookDM->GetCellData( _T("점검결과") , i , sCheckResult_NoDB );
		if( sCheckResult_NoDB == _T("DB없음") ) 
		{
			continue;
		}
		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
		
		ids = m_pBookDM->SetCellData( _T("선정") , _T("V") , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("초기화구분") , i , sBookCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ChangeBookCheckYN()") ); 

		if( nMode == 0 ) sBookCheckYN = _T("Y");
		else if( nMode == 1 ) sBookCheckYN = _T("N");
		else if( nMode == 2 )
		{
			if( sBookCheckYN.Compare(_T("Y"))==0)
				sBookCheckYN = _T("N");
			else if( sBookCheckYN.Compare(_T("N"))==0)
				sBookCheckYN = _T("Y");
		}
		ids = m_pBookDM->SetCellData( _T("초기화구분") , sBookCheckYN , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("등록번호") , i , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("책KEY") , i , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 

		ids = m_pBookDM->GetCellData( _T("단행/연속구분") , i , sPublishForm );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9, _T("ChangeBookCheckYN()") ); 
		

		ids = m_pDBProc->ChangeBookCheckYN( m_pBookDM , sBookCheckYN , sBookKey , sPublishForm , FALSE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("ChangeBookCheckYN()") ); 
	}

	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("ChangeBookCheckYN()") );  

	ids = m_pScreenPlay->ChangeBookCheckPrgressBarEnd();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("ChangeBookCheckYN()") ); 

	return 0;

	EFS_END
	return -1;

}

// 일괄 선정
INT CBLBookCheckProc::OptionSelect()
{
	EFS_BEGIN

	INT ids;

	// 검색 옵션..
	// DM을 읽어나가며 검색 조건에 걸리는 것을 체크한다.
	INT nRowCount = m_pBookDM->GetRowCount();

	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeBookCheckYN()") ); 

	CString sCheck;
	ids = m_pScreenPlay->GetCheckMode( sCheck );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OptionSelect") ); 

	TCHAR *sDMColumnAlias[1] = { _T("선정") };
	INT nColIndex[1] = { -1 };

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = IsCanCheck(i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OptionSelect") );
		
		if( ids > 0 ) continue;

		nColIndex[0] = -1;

		ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridCheckYN") );

		if( nColIndex[0] == -1 ) continue;

		pGrid->SetAt( i , nColIndex[0]  , sCheck );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::IsCanCheck( INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sShelfLocCode;
	CString sWorkingStatus;
	CString sLoanDate;
	CString sReturnDate;


	// 1. 자료실 비교
	ids = m_pBookDM->GetCellData( _T("자료실") , nIndex , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsCanCheck()") );
	sShelfLocCode.TrimRight(); sShelfLocCode.TrimLeft();

	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분") , sShelfLocCode , sShelfLocCode );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsCanCheck()") );

	ids = m_pScreenPlay->IsCanCheck( 0 , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;


	// 2. 배가 상태 비교
	ids = m_pBookDM->GetCellData( _T("배가상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsCanCheck()") );

	ids = m_pLocCommonAPI->GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsCanCheck()") );

	ids = m_pScreenPlay->IsCanCheck( 1 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;

	// 3. 대출 자료 비교
	ids = m_pBookDM->GetCellData( _T("대출일") , nIndex , sLoanDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("IsCanCheck()") );
	ids = m_pBookDM->GetCellData( _T("반납일") , nIndex , sReturnDate  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("IsCanCheck()") );
	//    대출일 비교
	ids = m_pScreenPlay->IsCanCheck( 2 , sLoanDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;
	//    반납일 비교
	ids = m_pScreenPlay->IsCanCheck( 3 , sReturnDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("IsCanCheck()") );
	if( ids > 0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::MoveGrid()
{
	EFS_BEGIN

	CString sMove1 , sMove2;

	INT ids;

	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("이동번호1") , sMove1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveGrid()") );
	ids = m_pParentMgr->GetControlData( m_sEtcInfoCMAlias , _T("이동번호2") , sMove2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveGrid()") );
	
	if( sMove2.Compare(_T("0"))==0 ) return 0;

	if( _ttoi(sMove1) < 0 ) return 0;

	ids = m_pScreenPlay->MoveGrid( _ttoi(sMove1) );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MoveGrid()") );

	return 0;

	EFS_END
	return -1;

}


// 비소장 자료로 선택된 자료 찾기
INT CBLBookCheckProc::SearchBookCheckN()
{
	EFS_BEGIN

	INT ids;

	// 1. SQL문 생성
	CString sCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	///@20071105 PDW
	///@{{++
	///@K3 요구사항 수정
	///@자료실을 다수 선택할 수 있도록 변경
	/*//before	
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("자료실명") , sShelfLocCode , -1 , 0 );	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBookCheckN()") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	*///after
	CString sShelfLocCode = GetShelfLocCode();	
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	///@--}}
	//CString sWhere = _T(" ( BB.BOOK_CHECK_YN = 'N' OR BB.BOOK_CHECK_YN IS NULL ) AND ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) AND BB.WORKING_STATUS IN (")HOLD_SEARCH_CONDITION_T(",")HOLD_SE_SEARCH_CONDITION_T(") ");
	CString sWhere = _T("( BB.BOOK_CHECK_YN = 'N' OR BB.BOOK_CHECK_YN IS NULL ) AND ( BB.PREV_WORKING_STATUS NOT IN ('SEL112N') OR BB.PREV_WORKING_STATUS IS NULL ) AND BB.WORKING_STATUS IN (")_T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION) _T(") ");
	if( !sShelfLocCode.IsEmpty() ) sWhere += _T(" AND BB.SHELF_LOC_CODE IN (" + sShelfLocCode +_T(")"));

	// 2009.02.25 ADD BY PDJ : MANAGE_CODE처리 추가
	sWhere += _T("AND BB.MANAGE_CODE=UDF_MANAGE_CODE");

	// 2. DB에서 SELECT해온다.


	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 기존 메인 DM 을 유지하고 , 추가한다.
	// m_sBookDMAlias 대신해서 Row 데이터 받을 MapData_Mem
	//ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere );
	CBLMapData_Mem	cMapData_NotHave; // 비소장
	ids = m_pDBProc->MakeDMFromDB( m_sBookDMAlias , sWhere , &cMapData_NotHave );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBookCheckN()") );
	if( ids > 0 ) return ids;
	// 원래 메인 DM 에 대신 받아온 Row 추가한다.
	cMapData_NotHave.CopyToDM( m_pBookDM );
	cMapData_NotHave.Clear();
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------



	// 3. 결과를 화면에 보여준다.
	ids = m_pScreenPlay->MainGridDisplay( m_sBookCMAlias , m_sBookDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBookCheckN()") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::DetailView()
{
	EFS_BEGIN

	if( m_pBookDM->GetRowCount() < 0 ) return 0;

	CESL_Grid *pGrid = m_pScreenPlay->GetMainGridPtr();
	if( pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DetailView()") );

	if( pGrid->GetMouseRow() < 1 ) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	m_pBookDM->GetCellData( _T("종KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );
	
	// 단행 연속 구분
	CString sPublishForm;
	m_pBookDM->GetCellData( _T("단행/연속구분") , nIndex , sPublishForm );
	if( sPublishForm.Compare(_T("MO"))==0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("SE"))==0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::DetailView2()
{
	EFS_BEGIN

	if( m_pInDBEtcDM->GetRowCount() < 0 ) return 0;

	CESL_Grid *pGrid = m_pScreenPlay->GetInDBGridPtr();
	if( pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DetailView()") );

	if( pGrid->GetMouseRow() < 1 ) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	m_pInDBEtcDM->GetCellData( _T("종KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );

	// 단행 연속 구분
	CString sPublishForm;
	m_pInDBEtcDM->GetCellData( _T("단행/연속구분") , nIndex , sPublishForm );
	if( sPublishForm.Compare(_T("MO"))==0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("SE"))==0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow );
		Dlg.DoModal();
	}

	return 0;

	EFS_END
	return -1;

}

// 이전 장서 점검 정보 로드하기

INT CBLBookCheckProc::LoadPreBookCheckInfo()
{
	EFS_BEGIN

	INT ids;

	// 비소장 자료 찾기
	// 기타자료1 찾기
	// 기타자료2 찾기
	ids = MakeBookCheckResultDMFromPreBookCheckInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LoadPreBookCheckInfo") );
	if( ids > 0 ) return 1;


	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, 1000, NULL );
	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, 999, NULL );
	AfxMessageBox( _T("장서점검 번호 변경을 완료하였습니다.") );
	
	// 그리드에 보여주기
	ids = AllControlDisplayGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("LoadPreBookCheckInfo") );

	
	
	return 0;

	EFS_END
	return -1;

}

// 이전 장서점검 이력 DB에서 얻어와 DM구성하기
INT CBLBookCheckProc::MakeBookCheckResultDMFromPreBookCheckInfo()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;
	CString sInfo[nCnt][2] =
	{
		{ _T("LOST_LIST_FILE") , _T("DM_BO_LOC_2100") } , 
		{ _T("ETC_LIST_FILE") , _T("DM_BO_LOC_2100_ETC") }, 
		{ _T("ETC_LIST_FILE2") , _T("DM_BO_LOC_2100_ETC_NOT_IN_DB") }
	};
	
	CESL_DataMgr *pDstDM;		// 결과 DM
	CESL_DataMgr *pRegNoDM;		// 등록번호 DM
	

	pRegNoDM = MakeTempRegNoDM();
	if( pRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );

	
	INT nFindLostList = AfxMessageBox( _T("비소장자료를 함께 검색하시겠습니까? \n검색시 이전 비소장자료이기때문에 현재정보와 다를 수 있습니다.") , MB_YESNO );
	CString currentJob[ 3 ] = {
		_T("[ 1 / 3 : 비소장자료를 검색중             ]"), 
		_T("[ 2 / 3 : 다른자료실에 존재하는 자료 검색 ]"), 
		_T("[ 3 / 3 : DB에 존재하지 않는 자료 검색    ]") };
	//SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, nCnt, NULL );

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		//SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, i, NULL );
		if( !i && nFindLostList != IDYES )
			continue;
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 2, ( LPARAM )&currentJob[ i ] );
		
		pRegNoDM->FreeData();

		pDstDM = m_pParentMgr->FindDM( sInfo[i][1] );
		if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
		pDstDM->FreeData();

		if( i == 2 )
		{
			ids = MakeRegNoFromBLOB( sInfo[i][0] , pDstDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );			
		}
		else
		{
			ids = MakeRegNoFromBLOB( sInfo[i][0] , pRegNoDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
	
			ids = MakeDMFromRegNoSql( pDstDM , pRegNoDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );
			if( ids > 0 ) return 1;
		}
	}

	// 메모리 프리
	if( pRegNoDM != NULL ) 
		delete pRegNoDM;

	pRegNoDM = NULL;

	return 0;

	EFS_END
	return -1;

}

// 템프 등록번호 DM 초기화하기
CESL_DataMgr* CBLBookCheckProc::MakeTempRegNoDM()
{
	EFS_BEGIN

	INT ids;

	// DM 메모리 구성
	CESL_DataMgr *pTmpDM = new CESL_DataMgr;
	

	// DM 레퍼런스 구성
	pTmpDM->InitDataMgrRef(1);		
	pTmpDM->m_nCols = 1;
	ids = pTmpDM->SetDataMgrRef( 0 , _T("등록번호") , _T("") , _T("")  , _T("")  , _T("") );			
	if( ids < 0 ) return NULL;

	return pTmpDM;

	EFS_END
	return NULL;

}



// DB BO_BOOK_CHK_RESULT_TBL의 sFieldName을 가져와 pRegNoDM에 등록번호를 추가하여 구성
INT CBLBookCheckProc::MakeRegNoFromBLOB(CString sFieldName , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	CString sFileName = _T("BookCheckTmp.txt");	// 템프 파일 이름

	// 1. 장서 점검 REC_KEY 가져오기
	//    REC_KEY가 없다면 SKIP
	
	
	CString currentJob = _T("장서점검 파일을 가져오는 중입니다.");
	SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 1, ( LPARAM )&currentJob );
	
	ids = MakeRegNoFileFromDBLOBField( sFileName , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFromBLOB") );
	if( ids > 0 ) return 0;
	

	// 2. 파일에 있는 내용을 pRegNoDM에 구성
	ids = SanFileAPI::RegNoFileLoad( pRegNoDM , m_pParentMgr , sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeRegNoFromBLOB") );

	return 0;

	EFS_END
	return -1;

}

// 현재 선택된 장서 체크 REC_KEY를 얻어온다.
INT CBLBookCheckProc::GetBookCheckRecKey(CString &sRecKey)
{
	EFS_BEGIN

	INT ids;

	CString sFieldAlias = _T("장서점검KEY");

	ids = m_pCheckNumberDM->GetCellData( sFieldAlias , 0 , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetBookCheckRecKey()") );
	sRecKey.TrimRight(); sRecKey.TrimLeft();

	return 0;

	EFS_END
	return -1;

}

// 장서점검 결과 테이블의 sFieldName을 가져와 sFileName에 기록한다.
INT CBLBookCheckProc::MakeRegNoFileFromDBLOBField( CString sFileName , CString sFieldName )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("BO_BOOK_CHK_RESULT_TBL");	// 장서 점검 결과 테이블

	CString sRecKey;	// 장서점검 테이블 REC_KEY

	// 파일 내용을 비운다.
	CFile fd;
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("장서 점검 Tmp File Open Error~"));		
		return 1;
	}
/*
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
*/	
	fd.Write(_T(""), 0);
	fd.Close();
	//_tremove(sFileName);

	ids = GetBookCheckRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFileFromDBLOBField()") );
	if( sRecKey.IsEmpty() ) return 1;
	
	// 2. 장서 점검 결과 파일을 DB에서 가져오기
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);

	INT nSize;
	ids = m_pCheckNumberDM->LOBGet( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey , sFieldName , pBuffer , &nSize );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeRegNoFileFromDBLOBField") );
	if ( nSize == 0 ) 
	{
		free( pBuffer );
		return 0;	
	}

	
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("장서 점검 Tmp File Open Error~"));
		free( pBuffer );
		return 1;
	}
	
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
	
	fd.WriteHuge(pBuffer, nSize);
	free( pBuffer );

	fd.Close();

	return 0;

	EFS_END
	return -1;

}

// pRegNoDM으로 등록번호를 조건으로 하는 SQL을 구성하여 pDstDM을 구성
INT CBLBookCheckProc::MakeDMFromRegNoSql( CESL_DataMgr *pDstDM , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	INT nRegNoRowCount = pRegNoDM->GetRowCount();

	CESL_DataMgr TmpDstDM;
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDstDM , &TmpDstDM );	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromRegNoSql") );

	INT nLimitCnt = 50;
	bool IsSend = FALSE;

	CString sRecKeyList;
	CString sWhere;
	CString sRecKey;

	CString currentJob[ 2 ] = {
		_T("단행 자료 정보를 검색중입니다."),
		_T("연속 자료 정보를 검색중입니다.") };
	// 단행 자료 , 연속 자료에서 검색한다.
	for( INT j = 0 ; j < 2 ; j++ )
	{
		if( j == 1)
		{
			TmpDstDM.TBL_NAME = _T("SE_BOOK_TBL BB , IDX_SE_TBL IB , SE_VOL_TBL V");
			TmpDstDM.CONDITION = _T("BB.VOL_KEY=V.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.BOOK_TYPE IN ('B','C','A') ");
			
			CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
			for( INT k = 0 ; k < TmpDstDM.RefList.GetCount() ; k++ )
			{
				sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
				ids = TmpDstDM.GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromDB") );
				
				if( sFieldAlias.Compare(_T("단행/연속구분")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , _T("SE")  , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
				else if( sFieldAlias.Compare(_T("가격")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , _T("V.PRICE") , sFieldType  , sInitValue  , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
				else if( sFieldAlias.Compare(_T("발행년")) == 0 )
				{
					ids = TmpDstDM.SetDataMgrRef( k , sFieldAlias , _T("IB.PUB_YEAR") , sFieldType  , sInitValue , sDataType );			
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );
				}
			}
		}
		
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 809, 1, ( LPARAM )&currentJob[ j ] );
		SendMessage( m_pParentMgr->m_hWnd, WM_USER + 807, nRegNoRowCount, NULL );
		for( INT i = 0 ; i < nRegNoRowCount ; i++ )
		{
			SendMessage( m_pParentMgr->m_hWnd, WM_USER + 808, i, NULL );
			
			IsSend=FALSE;

			if( i == nRegNoRowCount-1 ) IsSend = TRUE;
			if( (i!=0) && (i%nLimitCnt==0) ) IsSend = TRUE;


			ids = pRegNoDM->GetCellData( _T("등록번호") , i , sRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromRegNoSql") );

			if( !sRecKey.IsEmpty() )
			{
				if( !sRecKeyList.IsEmpty() ) sRecKeyList += _T(",");
				sRecKeyList += _T("'") + sRecKey + _T("'");
			}
			
			if( !IsSend ) continue;

			if( sRecKeyList.IsEmpty() ) continue;

			sWhere.Format(_T(" BB.WORKING_STATUS IN (%s) AND BB.REG_NO IN ( %s ) "), _T(HOLD_SEARCH_CONDITION)_T(",")_T(HOLD_SE_SEARCH_CONDITION) , sRecKeyList );
			ids = TmpDstDM.RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromRegNoSql") );

			ids = CLocCommonAPI::AddDM( &TmpDstDM , pDstDM , m_pParentMgr );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromRegNoSql") );

			sRecKeyList.Empty();
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBLBookCheckProc::AllControlDisplayGrid()
{
	EFS_BEGIN

	INT ids;

	ids = m_pScreenPlay->AllControlDisplayGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("AllControlDisplayGrid") );

	return 0;

	EFS_END
	return -1;

}

CString CBLBookCheckProc::MakeSelectQuery( CESL_DataMgr *pDM, INT startIndex, INT lastIndex )
{
	CString query, tmp, buf;

	tmp = _T(" REG_NO IN(");
	for( INT i = startIndex; i < lastIndex + 1; i++ )
	{
		if( m_bIsRegNoInDM )
			buf.Format( _T(" '%s',"), pDM->GetCellData( _T("등록번호"), i ) );
		else
			buf.Format( _T(" '%s',"), m_regNo.GetAt( i ) );

		tmp += buf;
	}
	buf = tmp.Left( tmp.GetLength() - 1 ) + _T(" )");
	query = buf;
	

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 메인 그리드의 DM 으로 셀렉트를 할꺼라서 
	// 아래 쿼리는 안한다.
	/*
	query.Format( 
		_T("SELECT 'M', B.REG_NO, I.TITLE_INFO, B.SHELF_LOC_CODE, ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE) AS CALL_NO  FROM BO_BOOK_TBL B, IDX_BO_TBL I WHERE B.SPECIES_KEY = I.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND %s ")
		_T("UNION ")
		_T("SELECT 'S', B.REG_NO, I.TITLE_INFO, B.SHELF_LOC_CODE, ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE) AS CALL_NO  fROM SE_BOOK_TBL B, IDX_SE_TBL I WHERE B.SPECIES_KEY = I.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND %s "),
		buf, buf );
	*/

	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return query;
}


INT CBLBookCheckProc::CheckRegNo( CESL_DataMgr *pSelectDM, CESL_DataMgr *pUpdateDM, INT startIndex, INT lastIndex, CString shelfLocCode )
{
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 해쉬 저장용..
	CBLMapData_Mem	cMapData_pDMSelect;
	cMapData_pDMSelect.Add_SelectQuery( pSelectDM , _T("등록번호") );
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	CString query, regNo, compareRegNo;
	
	INT type;	// 1 : 소장자료일때
				// 2 : DB에 없음
				// 3 : 자료실 다름
	
	pUpdateDM->StartFrame();
	pUpdateDM->InitDBFieldData();

	CString strTargetLoc;
	
	INT idx = 0;
	// 짤라서 체크할 카운트 만큼 loop
	for( INT j = startIndex; j < lastIndex + 1; j++ )
	{
		m_pScreenPlay->ProgressSetPos( j + 1 );
		
		// 검색비교될 등록번호
		if( m_bIsRegNoInDM )
			regNo = m_pRegNoDM->GetCellData( _T("등록번호"), j );
		else
			regNo = m_regNo.GetAt( j );
			
		type = 2;
			
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//
		// 해쉬를 사용하지 않아서 너무 느림
		// SQL In 을 200 -> 1000개 가까이 늘리고 (현재 900)
		// 해쉬 저장하고 찾는 방식으로 교체
		_BLMapData_Value* pFind = cMapData_pDMSelect.Find( regNo );
		idx = -1;
		if( pFind != NULL )
		{
			idx = pFind->row_idx;
			if( shelfLocCode.IsEmpty() )
			{
				type = 1;
			}else{
				// dm 자료실인댁스 : 11
				strTargetLoc = pFind->GetArrData(11);
					if( -1 == shelfLocCode.Find(strTargetLoc) )
						type = 3;
					// 자료실과 등록번호가 일치할때					
					else						
						type = 1;
			}
		}
			
		//// 검색된 레코드 개수만큼 loop
		//for( INT k = 0; k < pSelectDM->GetRowCount(); k++ )
		//{
		//	compareRegNo = pSelectDM->GetCellData( k, 1 );

		//	// 자료실을 선택하지 않고 등록번호가 일치할때
		//	if( shelfLocCode.IsEmpty() )
		//	{
		//		if( regNo == compareRegNo )
		//		{
		//			type = 1;
		//			idx = k;
		//		}
		//	}
		//	else
		//	{
		//		if( regNo == compareRegNo )
		//		{
		//			///@20071205 PDW
		//			///@{{++
		//			///@K3 요구사항 수정
		//			///@자료실을 다수 선택할 수 있도록 변경
		//			/*//before	
		//			if( shelfLocCode == pSelectDM->GetCellData( k, 3 ) )
		//			// 자료실과 등록번호가 일치할때					
		//				type = 1;
		//			// 자료실이 틀리고 등록번호가 일치할때
		//			else
		//				type = 3;
		//			*///after					
		//			// 자료실이 틀리고 등록번호가 일치할때
		//			strTargetLoc = pSelectDM->GetCellData( k, 3 );
		//			if( -1 == shelfLocCode.Find(strTargetLoc) )
		//				type = 3;
		//			// 자료실과 등록번호가 일치할때					
		//			else						
		//				type = 1;
		//			///@--}}
		//				
		//			idx = k;
		//		}
		//	}
		//	
		//	// 쓰레드 중지하기
		//	if( m_bBreak )
		//		return 100;			
		//}

		//
		// KOL.RED.2018.201 ---------------------------------------------------------------------------

		UpdateRegNoInfo( pUpdateDM, type, regNo, pSelectDM, idx );		
	}

	pUpdateDM->SendFrame();
	pUpdateDM->EndFrame();
	
	cMapData_pDMSelect.Clear();

	// 자료실명
	return 0;
}

///@20071205 PDW
///@{{++
///@K3 요구사항 수정
///@자료실을 복수 선택가능하도록 변경
/*//before	
CString CBLBookCheckProc::GetShelfLocCode()
{
	CString sShelfLocCode;
	m_pParentMgr->GetControlData( _T("CM_BO_LOC_2100_ETC_INFO"), _T("자료실명") ,sShelfLocCode ,-1 ,0 );
	return sShelfLocCode;
}
*///after
CString CBLBookCheckProc::GetShelfLocCode()
{
	CString sShelfLocCode;
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)m_pParentMgr->FindControl(_T("CM_BO_LOC_2100_ETC_INFO"), _T("자료실조건"));
	if( NULL == pCMCB )	return "";

	INT nCount = pCMCB->GetCount();

	CString strData;

	for( INT i=0;i<nCount;i++ )
	{
		if( !sShelfLocCode.IsEmpty() )
			sShelfLocCode += " , ";
				
		if( 0 > pCMCB->GetData( strData , i , 0 ) ) return "";

		strData.TrimLeft();strData.TrimRight();
		if( strData.IsEmpty() )	continue;

		sShelfLocCode += "'" + strData + "'";
	}

	return sShelfLocCode;
}
///@--}}


VOID CBLBookCheckProc::UpdateRegNoInfo( CESL_DataMgr *pUpdateDM, INT type, CString regNo, CESL_DataMgr *pSelectDM, INT idx )
{
	// 자료실이 다른 등록번호일때를 위한 변수
	const int cnt = 4;
	CString alias[ cnt ] = {
		_T("등록번호"), _T("서명"), _T("자료실"), _T("청구기호") };
	CString data[ cnt ];
	
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	int  alias_count = pSelectDM->GetColCount();
	CString *sel_alias_str = new CString[alias_count];
	CString *sel_Data_str = new CString[alias_count];
	pSelectDM->GetAllAlias(sel_alias_str, alias_count);
	int i=0;
	for( i = 0 ; i < alias_count ; i ++ )
	{
		CString str = sel_alias_str[i];
		str += _T("\n");
		OutputDebugString( str );
	}

	//메인 그리드 DM 의 "단행/연속구분" -> 22 컬럼
	TCHAR tch_MO_SE;
	if( idx > -1 )
	{
		CString sMO_SE = pSelectDM->GetCellData( idx, 22 );
		if( sMO_SE.GetLength() > 0 )
		{
			tch_MO_SE = sMO_SE.GetAt( 0 );
		}
	}
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
	
	CESL_DataMgr *pDM = NULL;
	CString query;
	switch( type )
	{
		case 1 :	// 소장자료일때

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			//
			//switch( pSelectDM->GetCellData( idx, 0 ).GetAt( 0 ) )
			switch( tch_MO_SE )
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------
			{
				case 'M' :	// 단행일때
					query.Format( _T("UPDATE BO_BOOK_TBL SET BOOK_CHECK_YN = 'Y' WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), regNo );
					break;
				case 'S' :	// 연속일때
					query.Format( _T("UPDATE SE_BOOK_TBL SET BOOK_CHECK_YN = 'Y' WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), regNo );
					break;
				default :
					break;
			}
			pUpdateDM->AddFrame( query );
			break;
		case 2 :	// DB에 없음	- DM_BO_LOC_2100_ETC_NOT_IN_DB
			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC_NOT_IN_DB") );
			pDM->SetCellData( _T("등록번호"), regNo, -1 );

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			//
			m_cMapData_NoDB.Add( regNo, sel_Data_str , 0 );	// 없는등록번호  임시저장
			m_cMapData_ALL.Add( regNo, sel_Data_str , 0 );
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------
			break;
		case 3 :	// 자료실 다름	- DM_BO_LOC_2100_ETC

			// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// 기존 DM 안씀
			/*
			data[ 0 ] = pSelectDM->GetCellData( idx, 1 );
			data[ 1 ] = pSelectDM->GetCellData( idx, 2 );
			data[ 2 ] = pSelectDM->GetCellData( idx, 3 );
			data[ 3 ] = pSelectDM->GetCellData( idx, 4 );
			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC") );
			pDM->SetCellData( alias, 4, data, -1 );
			*/

			pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_2100_ETC") );
			pDM->SetCellData( _T("등록번호"), regNo, -1 );

			pSelectDM->GetCellData( sel_alias_str , alias_count , sel_Data_str , idx );
			m_cMapData_ETC.Add( regNo, sel_Data_str , alias_count ); // 다른 자료실 임시저장
			m_cMapData_ALL.Add( regNo, sel_Data_str , alias_count );
			//
			// KOL.RED.2018.201 ---------------------------------------------------------------------------


			break;
		default :
			break;
	}
	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// 
	delete [] sel_alias_str;
	delete [] sel_Data_str;
	//
	// KOL.RED.2018.201 ---------------------------------------------------------------------------
}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// 위함수에서 분류해서 저장한 데이터를 입력된 순서에 따라서 그리드에 보이기.
void	CBLBookCheckProc::ViewGridBookCheck()
{
	// 한그리드에 보일때 다른 자료실 Row 데이터 이후에 
	// 없는 Row 데이터 보이기.
	//ViewGridBookCheck_MapData( &m_cMapData_ETC );
	//ViewGridBookCheck_MapData( &m_cMapData_NoDB );

	// 무조건 순서대로보일 꺼면 아래 코드
	ViewGridBookCheck_MapData( &m_cMapData_ALL );
}
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// 해쉬 저장소에 있는거를 ( 분류된것들 ) 
// m_pBookDM 메인 그리드 DM 에 넣는다.
// 
void	CBLBookCheckProc::ViewGridBookCheck_MapData( CBLMapData_Mem* pMapData )
{
	int  alias_count = m_pBookDM->GetColCount();
	CString *sel_alias_str = new CString[alias_count];
	CString *sel_Data_str  = new CString[alias_count];
	m_pBookDM->GetAllAlias(sel_alias_str, alias_count);

	INT nRegNoDMRowCount;
	if( m_bIsRegNoInDM )	nRegNoDMRowCount = m_pRegNoDM->GetRowCount();
	else					nRegNoDMRowCount = m_regNo.GetSize();

	CString			regNo;
	CArray<CString,CString&>	*arr_Data;
	for( int i = 0 ; i < nRegNoDMRowCount ; i++ )
	{
		if( m_bIsRegNoInDM )	regNo = m_pRegNoDM->GetCellData( _T("등록번호"), i );
		else					regNo = m_regNo.GetAt( i );

		arr_Data = pMapData->Find_ArrData( regNo );

		if( arr_Data == NULL )
		{
			// ERR 
			continue;
		}
	
		if( arr_Data->GetSize() > 0 )
		{
			for( int o = 0 ; o < alias_count ; o++ )
			{
				sel_Data_str[o] = arr_Data->GetAt(o);
			}
			m_pBookDM->SetCellData( sel_alias_str, alias_count, sel_Data_str ,-1 );
		}else{
			// 배열 데이터가 없는건 DB에 없는거
			// 위함수에서 문자열 배열에 아무것도 안넣었다.
			// 추가 처리 할꺼면 여기서..
			m_pBookDM->SetCellData( _T("등록번호"), regNo, -1 );
		}
	}

	delete [] sel_alias_str;
	delete [] sel_Data_str;

}
//
// KOL.RED.2018.201 ---------------------------------------------------------------------------


CString CBLBookCheckProc::ConvertPrice( CString price )
{
	return m_pScreenPlay->ConvertPrice( price );
}
