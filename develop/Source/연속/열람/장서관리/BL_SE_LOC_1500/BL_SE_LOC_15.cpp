// BL_SE_LOC_15.cpp: implementation of the CBL_SE_LOC_15 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_15.h"

#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "..\\..\\..\\수서\\SE_API\\SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_15::CBL_SE_LOC_15(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_sMainDMAlias  = _T("DM_SE_LOC_1500");
}

CBL_SE_LOC_15::~CBL_SE_LOC_15()
{

}

INT CBL_SE_LOC_15::SPFExcutePath( CString sPathAlias )
{
	INT ids;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1510();
	}
	else if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1510FromRFID();
	}
	else if( sPathAlias.Compare( RECORD_SHELF_DATE ) == 0 )
	{
		ids = RecordShelfDate();
	}
	else if( sPathAlias.Compare( RECORD_TMP_REG_NO ) == 0 )
	{
		ids = RecordRegNo();
	}
	else if( sPathAlias.Compare( CANCEL_SHELF ) == 0 )
	{
		ids = CancelShelf();
	}
	else if( sPathAlias.Compare( CANCEL_TMP_REG_NO ) == 0 )
	{
		ids = CancelRegNo();
	}
	else if( sPathAlias.Compare( CHANGE_BOX_NAME ) == 0 )
	{
		ids = ChangeBoxName();
	}
	else if( sPathAlias.Compare( RESTORE_DATA ) == 0 )
	{
		ids = RestoreData();
	}
	else if( sPathAlias.Compare( CANCEL_RESTORE ) == 0 )
	{
		ids = CancelRestore();
	}
	else if( sPathAlias.Compare( REMOVE_DATA ) == 0 )
	{
		ids = RemoveData();
	}
	else if( sPathAlias.Compare( REMOVE_COMPLETE ) == 0 )
	{
		ids = RemoveComplete();
	}
	else if( sPathAlias.Compare( CANCEL_REMOVE ) == 0 )
	{
		ids = CancelRemove();
	}
	else if( sPathAlias.Compare( ONE_BOOK_SHELF_CHANGE ) == 0 )
	{
		ids = OneBookChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_BOOK_SHELF_CHANGE ) == 0 )
	{
		ids = AllBookChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_SPECIES_SHELF_CHANGE ) == 0 )
	{
		ids = AllSpeciesChangeShelf();
	}
	else if( sPathAlias.Compare( ALL_SPECIES_CODE_CHANGE ) == 0 )
	{
		ids = ChangeSpeciesCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( ALL_BOOK_CODE_CHANGE ) == 0 )
	{
		ids = ChangeCode();
		if( ids == 1 ) return -1;
	}
	if( ids < 0 ) 
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다.") );

	return ids;
}

// 검색
INT CBL_SE_LOC_15::SearchSE_LOC_1510()
{
	INT ids;

	// Where절 구성
	CString sWhere;
	ids = GetWhereSE_LOC_1510(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );
	if( ids > 0 ) return 1;


	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );

	DWORD START, FINISH;
	START = GetTickCount();
	

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_1410") );

	FINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - 실시간열람 찾기 검색시 걸린 시간 :  [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	//===================================================
	//2010.01.14 ADD BY PJW : 검색결과에 따른 메세지 보여준다.
	if( pDM->GetRowCount() <= 0)
	{
		AfxMessageBox(_T("검색된 결과가 없습니다."));
	}
	else
	{
		AfxMessageBox(_T("검색을 완료 하였습니다."));
	}
	//===================================================
	
	// 화면에 보여주기
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_1410") );

	return 0;
}


// 검색
INT CBL_SE_LOC_15::SearchSE_LOC_1510FromRFID()
{
	INT ids;
	
	CString sWhere;
	
	GetTempData( _T("WHERE") , sWhere );
	
	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchBO_LOC_1410") );
	
	

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_1410") );

	
	
	// 화면에 보여주기
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_1410") );
	
	return 0;
}

// Where절 생성
// 050329_검색조건에 수입구분 추가 idx => 24    -_-+++
INT CBL_SE_LOC_15::GetWhereSE_LOC_1510( CString &sWhere )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1510");	// 검색 CM Alias

	// 1. 기본 Where절 생성
	const INT nCnt = 31;//30
	CString sControlAlias[nCnt] =
	{
		_T("자료실명") , _T("인계일자1") , _T("인계일자2") , _T("폐기일자1") , _T("폐기일자2") , 
		_T("함번호1") , _T("함번호2") , _T("접수번호1") , _T("접수번호2"), 
		_T("간행빈도") , _T("분류기호구분") , _T("분류기호1") , 
		_T("분류기호2") ,  _T("가등록번호1") , _T("가등록번호2") , _T("제어번호1") , 
		_T("제어번호2") , _T("서명1") , _T("서명2") , _T("발행자1") , _T("발행자2") , 
		_T("발행일1") , _T("발행일2") , _T("ISSN") , _T("입수구분"), _T("배가일자1") , 
		_T("배가일자2") , _T("복귀일자1") , _T("복귀일자2") ,  _T("자료상태") , _T("ISSN_까지")
	};

	CString sControlData[nCnt];	// 입력값

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWhereBO_LOC_1410") );

	// Control에서 입력값 가져오기
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sControlData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetWhereBO_LOC_1410") );

		// 콘트롤이 디스에이블이라면 검색 조건에 넣지 않는다.
		CESL_Control_Element *pCE = ((CESL_Control_Element*)pCM->FindControlElement( sControlAlias[i] ));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_1410") );
		
		if( !m_pParentMgr->GetDlgItem( pCE->CTRL_ID )->IsWindowEnabled() )
			sControlData[i].Empty();

		if( sControlAlias[i].Compare(_T("분류기호구분")) == 0 )
			sControlData[i].Empty();

		sControlData[i].TrimLeft(); 
		sControlData[i].TrimRight();
	}

	// 필수 입력 항목 검사
	// 2003년 6월 16일 조병걸 수정 : 자료실을 필수 입력 항목으로 검사하지 않는다.
	/*
	if( sControlData[0].IsEmpty() )
	{
		AfxMessageBox( _T("자료실을 선택해주십시오.") );
		return 1;
	}
	*/

	TCHAR *sEcoDBFieldName[25] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.DISUSE_PROC_DATE") , _T("B.DISUSE_PROC_DATE")  ,
		_T("S.BOX_NAME") , _T("S.BOX_NAME") , _T("B.RECEIPT_NO") , _T("B.RECEIPT_NO"), 
		_T("I.PUB_FREQ") , _T("B.CLASS_NO_TYPE") , _T("B.CLASS_NO") , 
		_T("B.CLASS_NO")  , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.CONTROL_NO") ,
		//_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("V.PUBLISH_DATE") , _T("V.PUBLISH_DATE") , _T("I.ST_ISSN") , _T("B.ACQ_CODE")
	};


	TCHAR *sCatDBFieldName[25] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.IMM_LOC_TRANSFER_DATE") , _T("B.DISUSE_PROC_DATE") , _T("B.DISUSE_PROC_DATE")  ,
		_T("S.BOX_NAME") , _T("S.BOX_NAME") , _T("B.RECEIPT_NO") , _T("B.RECEIPT_NO"), 
		_T("I.PUB_FREQ") , _T("B.CLASS_NO_TYPE") , _T("B.CLASS_NO") , 
		_T("B.CLASS_NO")  , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.CONTROL_NO") ,
		//_T("I.CONTROL_NO") , _T("I.SE_IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("I.CONTROL_NO") , _T("I.IDX_TITLE") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , _T("I.SE_IDX_PUBLISHER") ,
		_T("V.PUBLISH_DATE") , _T("V.PUBLISH_DATE") , _T("I.ST_ISSN") , _T("B.ACQ_CODE")
	};

	/// 2004년 5월 20일 조병걸 수정
	/// 함번호를 우절단 검색 가능하도록 해달라는 요청에 의해
	
	INT nDBFieldType[25] = 
	{
		ONESTRING , TWODATE , ETC , TWODATE , ETC ,	// 5
		ETC , ETC , TWONUMERIC , ETC ,				// 9
		ONESTRING , ONESTRING , TWOSTRING ,			// 12
		ETC  , TWOSTRING , ETC , TWOSTRING ,		// 16
		ETC , ONEINDEXSEARCH , ETC , ONEINDEXSEARCH , ETC , // 21
		TWODATE , ETC , ETC	, ONESTRING					// 24
	};

	TCHAR** sDBFieldName;
	/// ECO Search라면
	if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		sDBFieldName = sEcoDBFieldName;
	else
		sDBFieldName = sCatDBFieldName;
	

	// 검색 윈도우의 모드가 _T("자세히 >>") 모드일 경우만 검색 조건에 추가
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::GetSqlBO_LOC_1110( CString &sWhere )") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	
	INT nMakeCnt;
	if( sModeAlias.Compare( _T("상세검색") ) == 0 )
		nMakeCnt = 9;
	else
		nMakeCnt = 25;

	// Where절 생성
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nMakeCnt , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereBO_LOC_1410") );


	/// 2004년 5월 20일 조병걸 수정
	/// 함번호를 우절단 검색 가능하도록 해달라는 요청에 의해
	if( !sControlData[5].IsEmpty() )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");

		if( !sControlData[6].IsEmpty() )
		{
			sWhere += _T(" S.BOX_NAME BETWEEN '") + sControlData[5] + _T("' AND '") + sControlData[6] +_T("' ");
		}
		else
		{
			sWhere += _T(" S.BOX_NAME LIKE '") + sControlData[5] + _T("%' ");
		}
	}



	/// 17 하고 19
	/// 테스트를 위해 임시 적용
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	

	
	CString sEcoSearchWhere;
	INT nEcoSearchWhere = 0;
	if( nMakeCnt > 9 && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		CMakeSearchData MakeSearchData(m_pParentMgr);	
		
		BOOL bIsEqualSearch ;
		/// 서명 검색 , 발행자 검색
		// 평상시는 우절단 검색 생성
		if( !sControlData[17].IsEmpty() )
		{
			if( sControlData[17].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			else
				bIsEqualSearch = FALSE;

			
			MakeSearchData.GetKeyWord(sControlData[17]);
			if( !bIsEqualSearch ) 
				sControlData[17] += _T("*");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sControlData[17] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sControlData[19].IsEmpty() )
		{
			if( sControlData[19].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			else
				bIsEqualSearch = FALSE;
			
			MakeSearchData.GetKeyWord(sControlData[19]);
			if( !bIsEqualSearch ) 
				sControlData[19] += _T("*");
			
			if( !sEcoSearchWhere.IsEmpty() )
				sEcoSearchWhere += _T(" INTERSECT ");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '") + sControlData[19] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sEcoSearchWhere.IsEmpty() )
		{
			if( !sWhere.IsEmpty() )
				sWhere += _T(" AND ");
			if( nEcoSearchWhere == 1 )
				sWhere += _T(" I.REC_KEY IN ") + sEcoSearchWhere ;
			else
				sWhere += _T(" I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
		}
		
	}
	

	


	// ISSN
	CString sISSN;
	if( !sControlData[23].IsEmpty() )
	{
		sISSN = sControlData[23];
		if( !sControlData[30].IsEmpty() )
			sISSN += sControlData[30];
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND I.ST_ISSN = '") + sISSN + _T("' ");
	}

	// + 자료의 상태 
	// 제적중이 아니고 대출중이 아니고 제본중이 아닌 비치 자료
	CString sWhere2;
	if( sControlData[29].Compare(_T("실시간인계자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL111O'  ");
	}
	else if( sControlData[29].Compare(_T("실시간자료(배가자료+대출자료)")) == 0 )
	{
		//=====================================================
		//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 		sWhere2 = _T(" ( ( B.WORKING_STATUS IN ('SEL311O','SEL312O','SEL313O','SEL314O','SEL315O')  AND B.PREV_WORKING_STATUS = 'SEL112N') or B.WORKING_STATUS = 'SEL112N' ) ");
		// 18.09.27 JMJ KOL.RED.2018.008
		sWhere2 = _T(" ( ( B.WORKING_STATUS IN ('SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O')  AND B.PREV_WORKING_STATUS = 'SEL112N') or B.WORKING_STATUS = 'SEL112N' ) ");
		//=====================================================
	}
	else if( sControlData[29].Compare(_T("실시간배가자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("대출자료")) == 0 )
	{
		//=====================================================
		//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O' )  AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	// 18.09.27 JMJ KOL.RED.2018.008
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O' )  AND B.PREV_WORKING_STATUS = 'SEL112N' ");
		//=====================================================
	}
	else if( sControlData[29].Compare(_T("복귀자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ('SEB111N','SER111N') AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("등록인계자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SER111N' AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("제본인계자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEB111N' AND B.PREV_WORKING_STATUS = 'SEL112N' ");
	}
	else if( sControlData[29].Compare(_T("폐기자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS = 'SEL113O' AND B.PREV_WORKING_STATUS = 'SEL112N'");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	// 배가일자
	sWhere2.Empty();
	if( !sControlData[25].IsEmpty() )
	{
		if( sControlData[26].IsEmpty() )
		{
			sWhere2.Format( _T("( (B.SHELF_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.SHELF_CHANGE_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[25] , sControlData[25] , sControlData[25] , sControlData[25] 
							);
		}
		else
		{
			sWhere2.Format( _T("( (B.SHELF_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.SHELF_CHANGE_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[25] , sControlData[26] , sControlData[25] , sControlData[26] 
							);
		}

	}
	if( !sWhere2.IsEmpty() )
	{
		sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	// 복귀일자
	sWhere2.Empty();
	if( !sControlData[27].IsEmpty() )
	{
		if( sControlData[28].IsEmpty() )
		{
			sWhere2.Format( _T("( (B.BINDING_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.REG_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[27] , sControlData[27] , sControlData[27] , sControlData[27] 
							);
		}
		else
		{
			sWhere2.Format( _T("( (B.BINDING_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) OR ")
							_T("( B.REG_TRANSFER_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')) ) ") ,
							sControlData[27] , sControlData[28] , sControlData[27] , sControlData[28] 
							);
		}

	}
	if( !sWhere2.IsEmpty() )
	{
		sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	return 0;
}


// 화면에 보여주기
// 1. 코드 -> 설명으로 Convert
// 2. 그리드 View
INT CBL_SE_LOC_15::ViewMainGrid()
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sDMAlias = _T("DM_SE_LOC_1500");

	// 1. 코드 -> 설명
	// 청구기호 , 상태구분
	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-1 , _T("ViewMainGrid") );


	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	// 이용제한구분
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("이용제한구분") , _T("이용제한구분") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );
	
	// 관리구분
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("관리구분") , _T("관리구분") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );
	
	// 자료실
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , _T("자료실") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );

	// 간행빈도
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("간행빈도코드") , _T("간행빈도") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("ViewMainGrid") );

	CString sWorkingStatus;	// 자료상태
	CString sBackPath;		// 복귀 경로
	CString sAppendix;      // 부록여부
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// 자료 상태 변경
		ids = pDM->GetCellData( _T("자료상태") , i , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMainGrid") );

		ids = pDM->SetCellData( _T("자료상태") , sWorkingStatus, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );

		// 복귀 경로 표시
		ids = pDM->GetCellData( _T("복귀경로") , i , sBackPath );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );

		if( sBackPath.Compare(_T("Y")) == 0 )
			sBackPath=_T("제본인계자료");
		else
			sBackPath=_T("등록인계자료");
		
		ids = pDM->SetCellData( _T("복귀경로") , sBackPath, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );		

		ids = pDM->GetCellData( _T("부록명") , i , sAppendix );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		if( sAppendix != _T("") )
			sAppendix = _T("부록");

		ids = pDM->SetCellData(_T("부록여부"),sAppendix, i);
	}

	CString strMsg;		
	if( pDM->GetRowCount() == 0 )
		strMsg.Format( _T("검색된 자료가 존재하지 않습니다.") );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));		
	

	// 2. 그리드 View
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-7 , _T("ViewMainGrid") );

	// 3. 종 책수 표시
	ids = DisplaySpeciesBookCnt();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("ViewMainGrid") );


	

	return 0;
}

/// 메인 화면에 종수 책수를 뿌려준다.
INT CBL_SE_LOC_15::DisplaySpeciesBookCnt()
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	// 책수 구해오기
	CString sBookCnt;
	sBookCnt.Format( _T("%d") , pDM->GetRowCount() );
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1500_ETC") , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ViewMainGrid") );

	// 종수 구해오기
	CString sSpeciesCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( m_pParentMgr , pDM , _T("종정보KEY") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("ViewMainGrid") );
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1500_ETC") , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ViewMainGrid") );

	return 0;

}

/// 그리드에 선택된 자료가 있는지 검사
INT CBL_SE_LOC_15::GetSelectMainGridCnt()
{
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();
}


/// 화면작업
/// 메인 그리드 포인터 참조
CESL_Grid* CBL_SE_LOC_15::GetMainGridPtr()
{
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("메인그리드");

	// CM 참조
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}



// 배가일 기록
/// 선정된 자료에 대해 일괄 처리 
/// 1. 배가일을 기록할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::RecordShelfDate()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 배가일기록정보 입력값 검사 - 배가일자,배가담당자
	CString sShelfDate,sWorker;
	ids = GetInputShelfRecordData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("배가일자를 입력해주십시오.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("작업자를 입력해주십시오.") );
		return 3;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("실시간인계자료") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 배가일 기록 DB작업
		ids = RecordShelfDateDBProc( nIndex , &TmpDM , sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// 제본 등록 DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 제본 등록 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n배가일 기록 자료 : '%d'") , nSelectCnt , nSuccessCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n실패한 자료 : '%d'(배가일 기록 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , nSelectCnt , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n배가일 기록 자료 : '%d'\r\n")
						   _T("실패한 자료 : '%d'(자료상태가 배가일 기록 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );
	
	AfxMessageBox( sResultMsg );

	return 0;
}

/// 배가일 기록 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::RecordShelfDateDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate,CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordShelfDateDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordShelfDateDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordShelfDateDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordShelfDateDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("실시간배가자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordShelfDateDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RecordShelfDateDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordShelfDateDBProc") ); 

	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 

	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 

	return 0;
}

/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_15::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	// 김용배 추가
	ids = pDM->SetCellData( _T("등록번호"), pTmpDM->GetCellData( _T("가등록번호"), 0 ), nDMIndex );
	if( ids < 0 ) 
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 
	
	return 0;
}

/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_15::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;
}


/// 배가일 기록에 필요한 입력값을 가져온다.
INT CBL_SE_LOC_15::GetInputShelfRecordData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1530");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가일자") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가담당자") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;

}


// 프로그래스바 생성
CLocProgressBarDlg* CBL_SE_LOC_15::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// 프로그래스바 종료
INT CBL_SE_LOC_15::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}




// 가등록번호 부여
/// 선정된 자료에 대해 일괄 처리 
/// 1. 가등록번호를 기록할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::RecordRegNo()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNo") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNo") );

		if( sWorkingStatus.Compare( _T("실시간인계자료") ) != 0 && ( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0 ) )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 배가일 기록 DB작업
		ids = RecordRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) 
		{
			DeleteProgressBarDlg( pProgressBar );
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
		}
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료에 등록번호를 부여하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록번호를 부여 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 등록번호를 부여 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// 가등록번호 부여 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::RecordRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// 5163인지 select
	CString strIs5163;
	m_pParentMgr->GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , strIs5163 );

	// 050116_소정추가 ++++++++++++++++++++++++++++++++++++++++++++++++
	// 5163일경우 가등록번호 구하기( S0/HS + 2005 + 6자리 일련번호)
	// 공공 도서관 ( 2005 + 8자리 일련번호)
	if ( _T("Y") == strIs5163 )
		ids = MakeTmpRegNo_5163(sRegNo);
	else 
		ids = MakeTmpRegNo(sRegNo);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNoDBProc") );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , sRegNo );

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("가등록번호") , sRegNo , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;
}


/// 가등록번호 생성 & 참조 DB작업
INT CBL_SE_LOC_15::MakeTmpRegNo( CString &sRegNo )
{
	INT ids;

	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( _T("DM_TMP") );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	sYear.Format( _T("%d") , CLocCommonAPI::GetDBTime(m_pParentMgr).GetYear() );

	// 마지막 등록번호 얻어옴
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sYear);
	ids = pTmpDM->GetOneValueQuery( sSql , sTailRegNo );
	// 2008.06.06 REM BY PDJ : 값이 없을 경우 - 떨어지므로 리턴하면 안됨.
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") );

	// 마지막 등록번호가 존재하지 않을 경우 생성
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = pTmpDM->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

		sTailRegNo = _T("1");
	}

	sRegNo.Format( _T("%s%08s"),sYear,sTailRegNo);
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-001 JY 연속간행물 기능 안정화
	//ADD=================================================================================================
	sSql.Empty();
	sSql.Format( 
		_T("SELECT (SELECT MAX(REG_NO) FROM SE_BOOK_TBL WHERE REG_NO LIKE '%s________'  AND MANAGE_CODE = UDF_MANAGE_CODE) AS REG_NO ")
		_T("FROM DUAL WHERE ")
		_T("EXISTS (SELECT REG_NO FROM SE_BOOK_TBL WHERE REG_NO = '%s'  AND MANAGE_CODE = UDF_MANAGE_CODE) ")
	,sYear,sRegNo);

	CString strValue = _T("");
	pTmpDM->GetOneValueQuery( sSql , strValue );

	if ( !strValue.IsEmpty() ) {
		int tmpValue = _ttoi(strValue.Right(4));
		sTailRegNo.Format( _T("%d"), tmpValue );
		sRegNo.Format( _T("%s%08d"),sYear,tmpValue +1 );
	}
	
	sSql.Empty();
	//====================================================================================================

	// 마지막 등록번호 ++
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
					, _ttoi(sTailRegNo)+1 , sYear 
					);

	ids = pTmpDM->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

	return 0;
}


/// 가등록번호 생성 & 참조 DB작업(050116_소정추가)
INT CBL_SE_LOC_15::MakeTmpRegNo_5163( CString &sRegNo )
{
	INT ids;

	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM( _T("DM_TMP") );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	sYear.Format( _T("%d") , CLocCommonAPI::GetDBTime(m_pParentMgr).GetYear() );

	// 마지막 등록번호 얻어옴
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sYear);
	pTmpDM->GetOneValueQuery( sSql , sTailRegNo );

	// 마지막 등록번호가 존재하지 않을 경우 생성
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = pTmpDM->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

		sTailRegNo = _T("1");
	}

	CString strRegCode = _T("");
	CESL_DataMgr *pTmpRegNoDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1500_TMP_REGNO") );
	if( pTmpRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeTmpRegNo") ); 
	strRegCode = pTmpRegNoDM->GetCellData(_T("가등록구분"), 0);
	//ids = m_pParentMgr->GetDataMgrData(_T("DM_SE_LOC_1500_TMP_REGNO"), _T("가등록구분"), strRegCode);

	sRegNo.Format( _T("%s%s%06s"),strRegCode, sYear,sTailRegNo);

	// 마지막 등록번호 ++
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
					, _ttoi(sTailRegNo)+1 , sYear 
					);

	ids = pTmpDM->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeTmpRegNo") ); 

	return 0;
}



/// 배가 취소
/// 선정된 자료에 대해 일괄 처리 
/// 1. 배가취소할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::CancelShelf()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}


	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 배가 취소 DB작업
		ids = CancelShelfDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 배가취소하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 배가취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 배가취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// 배가 취소 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::CancelShelfDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelShelfDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelShelfDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelShelfDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelShelfDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL111O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T("") );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("실시간인계자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유코드") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유설명") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelShelfDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelShelfDBProc") ); 

	// 4. 종정보의 SPECIES_CLASS 구성
	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;

}


/// 가등록번호 취소
/// 선정된 자료에 대해 일괄 처리 
/// 1. 가등록번호 취소할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::CancelRegNo()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNo") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNo") );

		if( sWorkingStatus.Compare( _T("실시간인계자료") ) != 0 && ( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0 ) )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 가등록번호 취소 DB작업
		ids = CancelRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료에 등록번호를 삭제하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록번호를 삭제 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 등록번호를 삭제 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}
/// 가등록번호 쉬소 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::CancelRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T("") );

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("가등록번호") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;

}


/// 함번호 변경
/// 1. 함번호 변경할수 있는 자료인지 검사
/// 2. 종정보 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
INT CBL_SE_LOC_15::ChangeBoxName()
{
	INT ids;

	INT nCurrentIndex;
	CString sBoxName;
	GetTempData( _T("선택인덱스") , nCurrentIndex );
	GetTempData( _T("함번호") , sBoxName );

	// 그리드 포인터 참조
	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNo") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태


	CESL_DataMgr TmpDM;

	// 함번호 변경 DB작업
	ids = ChnageBoxNameDBProc( nCurrentIndex , &TmpDM , sBoxName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNo") );
	if( ids > 0 ) return ids;

	// DM 작업
	ids = DBChangeBoxNameDMProc( nCurrentIndex , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RecordRegNo") );

	// 화면 작업
	ids = DBChangeBoxNameGridProc(nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNo") );


	CString sResultMsg;
	sResultMsg.Format(_T("함번호가 변경되었습니다."));
	AfxMessageBox( sResultMsg );

	return 0;
}

/// 함번호 변경 DB 작업
/// 1. 종정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::ChnageBoxNameDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sBoxName)
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordRegNoDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordRegNoDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordRegNoDBProc") ); 

	pDM->StartFrame();

	CString sSpeciesRecKey ;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordRegNoDBProc") ); 

	// 프레임 생성
	// 1. 종정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("BOX_NAME") , _T("STRING") , sBoxName );

	pDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("함번호") , sBoxName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RecordRegNoDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RecordRegNoDBProc") ); 

	return 0;
}


/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_15::DBChangeBoxNameDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	CString sSpeciesKey;
	CString sTmpSpeciesKey;
	ids = pDM->GetCellData( _T("종정보KEY") , nDMIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	CString sBoxName;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("종정보KEY") , i , sTmpSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		if( sTmpSpeciesKey.Compare(sSpeciesKey) != 0 ) continue;

		ids = pTmpDM->GetCellData( _T("함번호") , 0 , sBoxName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		ids = pDM->SetCellData( _T("함번호") , sBoxName , i );

	}

	return 0;
}

/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_15::DBChangeBoxNameGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	CString sSpeciesKey;
	CString sTmpSpeciesKey;
	ids = pDM->GetCellData( _T("종정보KEY") , nDMIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("종정보KEY") , i , sTmpSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

		if( sTmpSpeciesKey.Compare(sSpeciesKey) != 0 ) continue;

		ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , i , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );
	}


	return 0;
}



/// 데이터 복귀
/// 선정된 자료에 대해 일괄 처리 
/// 1. 데이터 복귀할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::RestoreData()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 데이터 복귀 정보 입력값 검사 - 복귀 일자
	CString sRestoreDate;
	ids = GetInputRestoreDate( sRestoreDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRestoreDate.IsEmpty() )
	{
		AfxMessageBox( _T("복귀일자를 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nCopySuccessCnt = 0;	// 제본인계에 작업에 성공한 자료 수 
	INT nRegSuccessCnt = 0;		// 등록인계에 작업에 성공한 자료 수 
	CString sWorkingStatus;			// 자료상태
	CString sRestoreWorkingStatus;	// 복귀경로

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0  )
		{
			pGrid->SelectGetNext();
			continue;
		}

		ids = pDM->GetCellData( _T("복귀경로") , nIndex , sRestoreWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		//  DB작업
		ids = RestoreDataDBProc( nIndex , &TmpDM , sRestoreDate , sRestoreWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		if( sRestoreWorkingStatus.Compare(_T("등록인계자료")) == 0 )
			nRegSuccessCnt++;
		else if( sRestoreWorkingStatus.Compare(_T("제본인계자료")) == 0 )
			nCopySuccessCnt++;

		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nCopySuccessCnt>0 || (nRegSuccessCnt > 0)  )
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n제본인계자료 : '%d'\r\n등록인계자료 : '%d' ") 
						, nSelectCnt , nCopySuccessCnt , nRegSuccessCnt  );
	else
		sResultMsg.Format( _T("선택하신 자료중 복귀 가능한 자료가 존재하지 않습니다."));

	AfxMessageBox( sResultMsg );

	return 0;
}

/// 데이터 복귀 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::RestoreDataDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRestoreDate , CString sRestoreWorkingStatus )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RestoreDataDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RestoreDataDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RestoreDataDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RestoreDataDBProc") ); 

	CString sRestoreFieldName;
	if( sRestoreWorkingStatus.Compare(_T("제본인계자료"))==0 )
		sRestoreFieldName = _T("BINDING_TRANSFER_DATE");
	else
		sRestoreFieldName = _T("REG_TRANSFER_DATE");


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sRestoreWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sRestoreWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( sRestoreFieldName , _T("STRING") , sRestoreDate );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sRestoreWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , sRestoreWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("가등록번호") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유코드") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유설명") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RestoreDataDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RestoreDataDBProc") ); 

	// 4. 종정보의 SPECIES_CLASS 구성
	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}


/// 데이터 복귀 일자 참조
INT CBL_SE_LOC_15::GetInputRestoreDate( CString &sRestoreDate )
{
	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1520");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("복귀일자") , sRestoreDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sRestoreDate.TrimLeft(); sRestoreDate.TrimRight();

	return 0;
}


/// 복귀 취소
/// 선정된 자료에 대해 일괄 처리 
/// 1. 복귀취소할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::CancelRestore()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 복귀취소정보 입력값 검사 - 배가일자,배가담당자
	CString sShelfDate,sWorker;
	ids = CancelRestoreData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("배가일자를 입력해주십시오.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("작업자를 입력해주십시오.") );
		return 3;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("등록인계자료") ) != 0 && (sWorkingStatus.Compare( _T("제본인계자료") ) != 0))
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 복귀 취소 DB작업
		ids = CancelRestoreDBProc( nIndex , &TmpDM ,sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 복귀취소하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 복귀취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 복귀취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}


/// 복귀 취소 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::CancelRestoreDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRestoreDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRestoreDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRestoreDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRestoreDBProc") ); 

	CString sRestoreFieldName;
	if( sWorkingStatus.Compare(_T("제본인계자료"))==0 )
		sRestoreFieldName = _T("BINDING_TRANSFER_DATE");
	else
		sRestoreFieldName = _T("REG_TRANSFER_DATE");


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( sRestoreFieldName , _T("STRING") , _T("") );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("실시간배가자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRestoreDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelRestoreDBProc") ); 

	// 4. 종정보의 SPECIES_CLASS 구성
	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}


/// 복귀 취소에 필요한 입력값을 가져온다.
INT CBL_SE_LOC_15::CancelRestoreData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1520");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가일자") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가담당자") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;
}



/// 데이터 폐기
/// 선정된 자료에 대해 일괄 처리 
/// 1. 폐기할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::RemoveData()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 데이터 복귀 정보 입력값 검사 - 복귀 일자
	CString sRemoveDate;
	ids = GetInputRemoveDate( sRemoveDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRemoveDate.IsEmpty() )
	{
		AfxMessageBox( _T("폐기일자를 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus;			// 자료상태
	CString sRestoreWorkingStatus;	// 복귀경로

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0  )
		{
			pGrid->SelectGetNext();
			continue;
		}

		//  DB작업
		ids = RemoveDataDBProc( nIndex , &TmpDM , sRemoveDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;

		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSuccessCnt > 0   )
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d' ") 
						, nSelectCnt , nSuccessCnt);
	else
		sResultMsg.Format( _T("선택하신 자료중 폐기 가능한 자료가 존재하지 않습니다."));

	AfxMessageBox( sResultMsg );

	return 0;
}

/// 데이터 복귀 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::RemoveDataDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRemoveDate )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RemoveDataDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RemoveDataDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RemoveDataDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RemoveDataDBProc") ); 

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL113O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("DISUSE_PROC_DATE") , _T("STRING") , sRemoveDate );
	pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , _T(""));
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , _T(""));

	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("실시간폐기자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , _T("실시간배가자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("가등록번호") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경일자") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유코드") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유설명") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RemoveDataDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RemoveDataDBProc") ); 

	
	// 4. 종정보의 SPECIES_CLASS 구성
	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;
}

/// 데이터 완전 삭제(DB에서)
/// 모든 상태에 대해서 삭제 실행(단 대출 상태인 자료에 대해서는 삭제를 하지 않는다.)
/// (선정된 자료에 대해 일괄 처리)
/// 1. 삭제할 수 있는 자료인지 검사
///   1.1 정말 삭제를 원하는지 문의 후 실시
/// 2. 책상태 UPDATE
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::RemoveComplete()
{
	EFS_BEGIN

	INT ids = 0;
	INT nSelectCnt = 0;
	INT nIndex = 0;
	INT nSuccessCnt = 0;
	CString sMsg = _T("");
	CString sWorkingStatus = _T("");
	CString sBookKey = _T("");
	CString sAcqTypeVolKey = _T("");
	CString sVolKey = _T("");
	CString sSpeciesKey = _T("");
	CString sBookType = _T("");
	CString sVolTitle = _T("");
	CString sCMAlias = _T("CM_SE_LOC_1500");
	// 18.09.27 JMJ KOL.RED.2018.008
	CString sLoanStatus = _T("연속관외대출자료, 연속관내대출자료, 연속특별대출자료, 연속긴급대출자료, 연속배달자료, 연속책두레업무진행자료");

	CSeApi* pRemove = new CSeApi( m_pParentMgr );
	if ( NULL==pRemove ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("RemoveComplete") ); 

	// CM 참조
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("RemoveComplete") ); 

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("RemoveComplete") );

	pGrid->SelectMakeList();
	nIndex = pGrid->SelectGetTailPosition();
	nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}
	else if ( nSelectCnt > 0 ) 
	{
		// 1. 정말 삭제를 원하는지 문의
		sMsg.Format(_T("실제로 DB에서 데이터가 삭제되어 복구가 불가능합니다.\r\n")
					_T("%d개의 자료를 정말로 삭제하시겠습니까?"), nSelectCnt );
		ids = AfxMessageBox( sMsg, MB_YESNO );
		if ( IDNO==ids )	return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("RemoveComplete") );

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	// 책별로 Frame을 만들어서 DB에 update한다.
	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		// 자료상태가 대출이 아니면 삭제를 수행한다.
		ids = pDM->GetCellData( _T("자료상태"), nIndex, sWorkingStatus );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("RemoveComplete") );
		if ( sLoanStatus.Find( sWorkingStatus ) > -1 )	continue;

		ids = pDM->GetCellData( _T("책정보KEY"), nIndex, sBookKey );
		ids = pDM->GetCellData( _T("수입구분별권KEY"), nIndex, sAcqTypeVolKey );
		ids = pDM->GetCellData( _T("권호KEY"), nIndex, sVolKey );
		ids = pDM->GetCellData( _T("종정보KEY"), nIndex, sSpeciesKey );
		ids = pDM->GetCellData( _T("책유형"), nIndex, sBookType );
		ids = pDM->GetCellData( _T("권호명"), nIndex, sVolTitle );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("RemoveComplete") );

		// 2. 책상태 UPDATE
		// 수서의 책 삭제시 여러 수행동작을 링크를 걸어서 수행할 수 있게한다.
		ids = pRemove->BOOK_CAT_DELETE( pDM			, sBookKey		, sAcqTypeVolKey,
										sVolKey		, sSpeciesKey	, sBookType		,
										sVolTitle	, FALSE			, FALSE );
		if ( ids < 0 ) 
		{
			//===================================================
			//2009.05.11 ADD BY PJW : 일반자료일경우는 계속진행한다.
			if(ids == -1004) 
			{
				if ( (i+1)!=nSelectCnt )
				{
					nIndex = pGrid->SelectGetPrev( );
				}
				continue;
			}
			//===================================================
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5, _T("RemoveComplete") );
		}


		// 2.1 권/호의 배가상태 값 결정.
		ids = SettingSpeciesField( sSpeciesKey , sVolKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RemoveComplete") ); 

		// 3. DM 변경
		ids = pDM->DeleteRow( nIndex );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -7, _T("RemoveComplete") );

		nSuccessCnt++;

		if ( (i+1)!=nSelectCnt )
			nIndex = pGrid->SelectGetPrev( );
	}

	// 4. 그리드 변경
	ids = pCM->AllControlDisplay( );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -8, _T("RemoveComplete") );

	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );
	// 삭제 객체 죽임
	delete pRemove;
	pRemove = NULL;

	if( nSuccessCnt > 0   )
		sMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d' ") 
						, nSelectCnt , nSuccessCnt);
	else
		sMsg.Format( _T("선택하신 자료중 삭제 가능한 자료가 존재하지 않습니다."));

	AfxMessageBox( sMsg );

	return 0;

	EFS_END
	return -1;
}

/// 폐기 취소
/// 선정된 자료에 대해 일괄 처리 
/// 1. 폐기취소할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_15::CancelRemove()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 복귀취소정보 입력값 검사 - 배가일자,배가담당자
	CString sShelfDate,sWorker;
	ids = CancelRemoveData( sShelfDate,sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfDate.IsEmpty() )
	{
		AfxMessageBox( _T("배가일자를 입력해주십시오.") );
		return 2;
	}
	if( sWorker.IsEmpty() )
	{
		AfxMessageBox( _T("작업자를 입력해주십시오.") );
		return 3;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sWorkingStatus.Compare( _T("실시간폐기자료") ) != 0)
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 복귀 취소 DB작업
		ids = CancelRemoveDBProc( nIndex , &TmpDM ,sShelfDate , sWorker );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 폐기취소하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 폐기취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 폐기취소 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

}

/// 복귀 취소 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_15::CancelRemoveDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sWorker )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRemoveDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRemoveDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRemoveDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRemoveDBProc") ); 


	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL112N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sWorker );
	pDM->AddDBFieldData( _T("DISUSE_PROC_DATE") , _T("STRING") , _T(""));
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("실시간배가자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가담당자") , sWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRemoveDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelRemoveDBProc") ); 

	
	// 4. 종정보의 SPECIES_CLASS 구성
	// 4. 종정보의 SPECIES_CLASS 구성
	CString sSpeciesKey, sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("RecordShelfDateDBProc") ); 
	
	ids = SettingSpeciesField( sSpeciesKey , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("RecordShelfDateDBProc") ); 
	
	return 0;

}

/// 폐기 취소에 필요한 입력값을 가져온다.
INT CBL_SE_LOC_15::CancelRemoveData( CString &sShelfDate,CString &sWorker )
{
	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1580");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가일자") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sShelfDate.TrimLeft(); sShelfDate.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가담당자") , sWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sWorker.TrimLeft(); sWorker.TrimRight();

	return 0;

}

/// 폐기 일자 참조
INT CBL_SE_LOC_15::GetInputRemoveDate( CString &sRemoveDate )
{
	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1580");
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("폐기일자") , sRemoveDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputShelfRecordData") );
	sRemoveDate.TrimLeft(); sRemoveDate.TrimRight();

	return 0;

}


/// 낱권 배가변경
INT CBL_SE_LOC_15::OneBookChangeShelf()
{
	INT ids;
	INT nIndex;
	GetTempData( _T("선택인덱스") , nIndex );

	ids = OneBookChangeShelf( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );
	if( ids == 1000 )
	{
		AfxMessageBox( _T("자료 상태가 배가 변경할 수 없는 자료상태입니다. ")) ;
		return ids;
	}
	if( ids > 0 ) return ids;



	return 0;
}


/// DM의 레코드가 nIndex인 자료를 변경한다.
INT CBL_SE_LOC_15::OneBookChangeShelf( INT nIndex )
{
	INT ids;
	// 입력값이 올바르게 입력되어 있는지 검사

	CString sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate;
	
	ids = GetBookChangeShelfInputData( sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );

	if( sShelfLocCode.IsEmpty() )
	{
		AfxMessageBox( _T("변경될 배가 위치를 선택해주십시오") );
		return 1;
	}
	if( sShelfChangeDate.IsEmpty() )
	{
		AfxMessageBox( _T("배가 변경 일자를 입력해주십시오.") );
		return 2;
	}
	if( sShelfWorker.IsEmpty() )
	{
		AfxMessageBox( _T("배가 지시자를 입력해주십시오.") );
		return 3;
	}

	CString sDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OneBookChangeShelf") );


	CESL_DataMgr TmpDM;
	CString sWorkingStatus;

	ids = pDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("OneBookChangeShelf") );

	if( sWorkingStatus.Compare( _T("실시간배가자료") ) != 0)
		return 1000;


	ids = OneBookChangeShelfDBProc( nIndex ,&TmpDM ,sChangeReasonCode, sChangeReason , sShelfLocCode , sShelfWorker , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelf") );
	
	// DM 작업
	ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("OneBookChangeShelf") );

	// 화면 작업
	ids = DBInsertUpdateGridProc(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OneBookChangeShelf") );
	
	return 0;
}


/// 책배가변경화면에서 입력값을 가져온다.
INT CBL_SE_LOC_15::GetBookChangeShelfInputData( CString &sChangeReasonCode, CString &sChangeReason , CString &sShelfLocCode , CString &sShelfWorker , CString &sShelfChangeDate )
{
	CString sCMAlias = _T("CM_SE_LOC_1540");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("변경사유코드_입력") , sChangeReasonCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookChangeShelfInputData") );
	sChangeReasonCode.TrimLeft(); sChangeReasonCode.TrimRight();
	if( sChangeReasonCode.Compare(_T("적용안함")) == 0 ) sChangeReasonCode.Empty();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("변경사유내용_입력") , sChangeReason );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetBookChangeShelfInputData") );
	sChangeReason.TrimLeft(); sChangeReason.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("변경위치_입력") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetBookChangeShelfInputData") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("변경지시자_입력") , sShelfWorker );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetBookChangeShelfInputData") );
	sShelfWorker.TrimLeft(); sShelfWorker.TrimRight();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("변경날짜_입력") , sShelfChangeDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetBookChangeShelfInputData") );
	sShelfChangeDate.TrimLeft(); sShelfChangeDate.TrimRight();

	return 0;
}


/// DM의 레코드가 nIndex인 자료를 배가 변경한다.
INT CBL_SE_LOC_15::OneBookChangeShelfDBProc( INT nIndex , CESL_DataMgr* pTmpDM , CString sChangeReasonCode, CString sChangeReason , CString sShelfLocCode , CString sShelfWorker , CString sShelfChangeDate )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelfDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OneBookChangeShelfDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("OneBookChangeShelfDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sShelfBeroreLoc;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	ids = pTmpDM->GetCellData( _T("자료실") , 0 , sShelfBeroreLoc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분") , sShelfBeroreLoc , sShelfBeroreLoc );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sShelfWorker );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , sChangeReasonCode  );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , sShelfChangeDate );
	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sShelfLocCode );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , sChangeReason );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , sShelfBeroreLoc );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sShelfLocCode , sShelfLocCode );

	ids = pTmpDM->SetCellData( _T("배가담당자") , sShelfWorker , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유코드") , sChangeReasonCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경일자") , sShelfChangeDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("자료실") , sShelfLocCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배가변경사유설명") , sChangeReason , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OneBookChangeShelfDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("OneBookChangeShelfDBProc") ); 

	// 종에도 기록한다.
	CString sSpeciesKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	CString sSpceciesFunc;
	sSpceciesFunc.Format( _T("GET_DISP_SE_BOOK_INFO(%s,'SHELF_LOC_CODE')") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("idx_hold_place") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	
	sSpceciesFunc.Format( _T("GET_SE_MAIN_SHELF_CODE(%s)") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("main_place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr );

	// 색인 테이블도 함께 변경한다.
	CString sQuery;
	sQuery.Format( _T("UPDATE IDX_SE_TBL SET IDX_PLACE = GET_SE_BOOK_INFO( '%s', 'SHELF_LOC_CODE' ) WHERE REC_KEY = %s;"),
					sSpeciesKey, sSpeciesKey );
	pDM->AddFrame( sQuery );
	
	pDM->SendFrame();
	
	return 0;
}


/// 일괄 배가변경
INT CBL_SE_LOC_15::AllBookChangeShelf()
{
	INT ids;
	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllBookChangeShelf") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nSuccessCnt = 0;

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		ids = OneBookChangeShelf(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllBookChangeShelf") );
		if( ids == 1000 )
		{
			pGrid->SelectGetNext();
			continue;
		}
		else if( ids > 0 ) 
		{
			DeleteProgressBarDlg( pProgressBar );
			return 1;
		}

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}

	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 배가변경하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 배가변경 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 배가변경 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}


/// 종배가변경
INT CBL_SE_LOC_15::AllSpeciesChangeShelf()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 배가실 참조
	CString sShelfLocCode;
	ids = GetSpeciesChangeShelfInputData( sShelfLocCode);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sShelfLocCode.IsEmpty() )
	{
		AfxMessageBox( _T("배가실을 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		// 종배가변경 기록 DB작업
		ids = AllSpeciesChangeShelfDBProc( nIndex  , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	return 0;
}

/// 종배가변경 DB작업
INT CBL_SE_LOC_15::AllSpeciesChangeShelfDBProc( INT nIndex , CString sShelfLocCode )
{
	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OneBookChangeShelfDBProc") );

	pDM->StartFrame();

	CString sSpeciesRecKey ;
	ids = pDM->GetCellData( _T("종정보KEY") , nIndex , sSpeciesRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("OneBookChangeShelfDBProc") ); 

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sShelfLocCode );
	pDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesRecKey );
	pDM->InitDBFieldData();

	// 종배가실 변경시 색인 테이블의 배가실도 함께 변경한다. 
	CString sQuery;
	sQuery.Format( _T("UPDATE IDX_SE_TBL SET PLACE_INFO = '%s', MAIN_PLACE_INFO = '%s' WHERE REC_KEY = %s;"), 
					sShelfLocCode, sShelfLocCode, sSpeciesRecKey );
	pDM->AddFrame( sQuery );
	
	
	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("OneBookChangeShelfDBProc") ); 

	return 0;

}

/// 책배가변경화면에서 입력값을 가져온다.
INT CBL_SE_LOC_15::GetSpeciesChangeShelfInputData( CString &sShelfLocCode )
{
	CString sCMAlias = _T("CM_SE_LOC_1590");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배가실") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetBookChangeShelfInputData") );
	sShelfLocCode.TrimLeft(); sShelfLocCode.TrimRight();
	if( sShelfLocCode.Compare(_T("적용안함")) == 0 ) sShelfLocCode.Empty();

	return 0;
}



// 코드 일괄 변경
INT CBL_SE_LOC_15::ChangeCode()
{
	EFS_BEGIN

	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("메인그리드") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	INT nIndex;

	CString sCodeNameCode;	// 변경될 코드의 종류 코드 ( 이용제한구분 : 0 , 관리구분 : 1 )
	CString sCodeName;			// 변경될 코드의 필드명( USE_LIMIT_CODE , MANAGE_CODE )
	CString sCodeValue;			 // 콘트롤에서 얻어옴
	CString sDMColumnAlias;
	ids = GetInputControlData( sCodeNameCode , sCodeName , sCodeValue , sDMColumnAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeCode()") );
	if( ids > 0 ) return 1;

	// 그리드의 포지션을 마지막으로
	pGrid->SelectGetTailPosition();
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;		 // 변경될 레코드의 REC_KEY 리스트
	
	nGetRowCount= pGrid->SelectGetCount();

	
	// 프로그래스바를 생성하기 위해--------------------
	CLocProgressBarDlg *ProgressBar;
	ProgressBar = new CLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	// 선정된 자료
	CString sRepresentBookYN;
	CString sWorkingStatus;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// 프로그래스바 설정------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();

		// 대출 중인 자료는 변경할수 없다.
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("자료상태") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		
		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS값이 잘못들어감
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )
		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0))
		//=====================================================
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;
		}


		// UPDATE할 책테이블의 REC_KEY 참조
		CString sBookRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("책정보KEY") , sBookRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeCode()") );

		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("종정보KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("대표책값") , sRepresentBookYN , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeCode()") );



		sRecKeyList = sBookRecKey;

		ids = ChangeCodeDBUpdate( sCodeNameCode , sCodeName , sCodeValue  , sRecKeyList , sSpeciesRecKey , sRepresentBookYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeCode()") );
		sRecKeyList.Empty();
		
		// 데이터 메니져에서 UPDATE
		// 코드를 데이터 메니져에 입력
		ids = DMCodeChangeUpdate( sDMColumnAlias , sCodeValue , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::ChangeCode()") );

		// 그리드에 보여주는 작업
		ids = CMGridCodeChangeUpdate( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );


		if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
		nChangeCnt++;
	}
	// 프로그래스바 디스트로이------------------
	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;
	//----------------------------------

	CString sMsg;
	if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("선택 자료 : '%d'\r\n성공 자료 : '%d'\r\n실패 자료 : '%d'(대출중인 자료이기 때문에 처리되지 못했습니다.)"),
		nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
		sMsg.Format( _T("모두 '%d'건이 처리되었습니다.") , nChangeCnt );
	else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("모두 '%d'건이 대출중인 자료이기 때문에 처리되지 못했습니다.") , nNoChangeCnt );
	AfxMessageBox(sMsg);


	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_15::GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1595");

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("코드변경종류") , sCodeNameCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	CString sCodeComboAlias;
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		sCodeName = _T("USE_LIMIT_CODE");
		sCodeComboAlias = _T("이용제한구분코드");
		sDMColumnAlias = _T("이용제한구분");
	}
	else if( sCodeNameCode.Compare( _T("1") ) == 0 )
	{
		sCodeName = _T("MANAGE_CODE");
		sCodeComboAlias = _T("관리구분코드");
		sDMColumnAlias = _T("관리구분");
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , sCodeComboAlias , sCodeValue , -1 , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	// 배가 상태 코드는 다르게 한다.
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCodeValue , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );
	}

	if( sCodeValue.IsEmpty() || ( sCodeValue.Compare( _T("0") )== 0 ) )
	{
		AfxMessageBox( _T("변경될 구분값을 선택해주십시오") );
		return 1;
	}

	return 0;

	EFS_END
	return -1;

}

// 구분 코드 일괄 변경
INT CBL_SE_LOC_15::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	// LOG DB 컬럼 결정
	CString sLogColumnName;
	// 배가 상태
	// 이용제한 구분 
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
		sLogColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
	// 관리구분 
	else if( sCodeNameCode.Compare(_T("1")) == 0 )
		sLogColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( sCodeName , _T("STRING") , sCodeValue );
	pDM->AddDBFieldData( sLogColumnName , _T("NUMERIC") , _T("sysdate") );

	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );
	pDM->EndFrame();

	MobileApi.EndLog();

	CString sSpeciesCodeValue;
	if( sCodeNameCode.Compare(_T("1")) == 0 ) 
	{
		return 0;
	}

	
	// 이용제한 구분이면 다음을 실행한다.
	if( sCodeNameCode.Compare(_T("0")) == 0 )
	{
		ids = ChangeUseLimitCode( sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeCodeDBUpdate") );

		return 0;
	}

	CKolas2up_MobileApi MobileApi2;
	MobileApi2.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi2.StartLog();
	MobileApi2.SetDM( pDM );
	
	pDM->StartFrame();

	CString sSpeciesFunc;
	sSpeciesFunc.Format( _T("GET_DISP_SE_BOOK_INFO(%s,'MANAGE_CODE')") , sSpeciesKey );
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("NUMERIC") , sSpeciesFunc );
	MobileApi2.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr , &MobileApi2 );
	
	// QUERY SEND
	ids = MobileApi2.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();


	return 0;	

	EFS_END
	return -1;

}




/// 구분 코드 일괄 종변경
/// 종이 변경 -> 종만 변경
INT CBL_SE_LOC_15::ChangeSpeciesCode()
{
	EFS_BEGIN


	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("메인그리드") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	INT nIndex;

	CString sCodeNameCode;		// 변경될 코드의 종류 코드 ( 배가상태 : 0 , 이용제한구분 : 1 , 관리구분 : 2 )
	CString sCodeName;			// 변경될 코드의 필드명( WORKING_STATUS , USE_LIMIT_CODE , MANAGE_CODE )
	CString sCodeValue;			// 콘트롤에서 얻어옴
	CString sDMColumnAlias;
	ids = GetInputControlData( sCodeNameCode , sCodeName , sCodeValue , sDMColumnAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );
	if( ids > 0 ) return 1;

	// 그리드의 포지션을 마지막으로
	pGrid->SelectGetTailPosition();
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;		 // 변경될 레코드의 REC_KEY 리스트
	
	nGetRowCount= pGrid->SelectGetCount();

	
	// 프로그래스바를 생성하기 위해--------------------
	CLocProgressBarDlg *ProgressBar;
	ProgressBar = new CLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	// 선정된 자료
	CString sRepresentBookYN;
	CString sWorkingStatus;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// 프로그래스바 설정------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();


		// UPDATE할 책테이블의 REC_KEY 참조
		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("종정보KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

		pParentDM->StartFrame();

		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("SE_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		ids = pParentDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("") )

		pParentDM->EndFrame();

		if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
		nChangeCnt++;
	}
	// 프로그래스바 디스트로이------------------
	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;
	//----------------------------------

	CString sMsg;
	if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n실패 자료 : '%d'(대출중인 자료이기 때문에 처리되지 못했습니다.)"),
		nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
		sMsg.Format( _T("모두 '%d'건이 처리되었습니다.") , nChangeCnt );
	else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("모두 '%d'건이 대출중인 자료이기 때문에 처리되지 못했습니다.") , nNoChangeCnt );
	AfxMessageBox(sMsg);


	return 0;

	EFS_END

	return -1;
}


// 코드 일괄 변경중
// 변경되었을 경우 화면에 보여주는 작업
INT CBL_SE_LOC_15::CMGridCodeChangeUpdate(INT nIndex)
{
	EFS_BEGIN
		
		INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1500");
	
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("메인그리드");
	
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	
	const INT nCnt = 2;
	
	TCHAR *sDMFieldAlias[nCnt] =
	{
		_T("관리구분") , _T("이용제한구분")
	};
	
	INT nColIndex[nCnt] =
	{
		-1,-1
	};
	
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMFieldAlias , nCnt , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 
	
	CString sData;
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias[i] , sData , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 
		
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}
	
	return 0;
	
	EFS_END
	return -1;
	
}


// 코드 일괄 변경시 DM UPDATE
INT CBL_SE_LOC_15::DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )
{
	EFS_BEGIN
		
		INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1500");
	
	if( sDMColumnAlias.Compare(_T("관리구분")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("관리구분") , sCodeValue , sCodeValue   );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
		
	}
	else if( sDMColumnAlias.Compare(_T("자료상태")) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCodeValue , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("이용제한구분")) == 0 )
	{
		// 이용제한 구분
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("이용제한구분") , sCodeValue , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	
	ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias , sCodeValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	
	return 0;
	
	EFS_END
		return -1;
	
}



// 종 이용제한 구분 UPDATE
INT CBL_SE_LOC_15::ChangeUseLimitCode( CString sSpeciesKey )
{
	EFS_BEGIN

	INT ids;
	CString sPreUseLimitCode , sNewUseLimitCode;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_SE_LOC_1500_USE_LIMIT_SPECIES"));
	if( pUseLimitDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeUseLimitCode") );

	// 이전_이용제한구분 가져오기
	CString sQuery;
	sQuery.Format( _T("SELECT USE_LIMIT_CODE FROM IDX_SE_TBL WHERE REC_KEY = %s") , sSpeciesKey );
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pUseLimitDM , sQuery , sPreUseLimitCode );
//	ids = pUseLimitDM->GetOneValueQuery( sQuery , sPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeUseLimitCode") );

	// 새로운_이용제한구분 가져오기
	sQuery.Format( _T(" SPECIES_KEY = %s ") , sSpeciesKey );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pUseLimitDM , sQuery );
//	ids = pUseLimitDM->RefreshDataManager( sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeUseLimitCode") );


	CString sUseLimitCode;
	for( INT i = 0 ; i < pUseLimitDM->GetRowCount() ; i++ )
	{
		ids = pUseLimitDM->GetCellData( _T("이용제한구분") , i , sUseLimitCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ChangeUseLimitCode") );

		if( sNewUseLimitCode.GetLength() > 1 )
			sNewUseLimitCode += _T(" ");

		sNewUseLimitCode += sUseLimitCode;
	}

	CArray < CString , CString > saPreUseLimitCode;
	CArray < CString , CString > saNewUseLimitCode;

	ids = MakeArrayUseLimitCode( sPreUseLimitCode , saPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd , -5 , _T("ChangeUseLimitCode") );

	ids = MakeArrayUseLimitCode( sNewUseLimitCode , saNewUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd , -6 , _T("ChangeUseLimitCode") );

	BOOL IsEqual = TRUE;

	// 같으냐 틀리냐는 사이즈로 조정이 불가능하다.
	IsEqual = CompareUseLimtCodes( saPreUseLimitCode , saNewUseLimitCode );

	if( IsEqual ) return 0;

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pUseLimitDM );

	// 종 테이블에 새로운 이용제한구분을 입력한다.
	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sNewUseLimitCode );
	// 단행 색인 테이블 - 모든 이용제한 구분 리스트가 들어간다.
	MobileApi.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );	
//	pUseLimitDM->MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	// 단행 종 테이블 - 새로운 대표만 들어간다.
	CString sBossPreUseLimitCode , sBossNewUseLimitCode;
	ids = GetBossUseLimitCode( sBossPreUseLimitCode , saPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ChangeUseLimitCode") );
	ids = GetBossUseLimitCode( sBossNewUseLimitCode , saNewUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("ChangeUseLimitCode") );

	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sBossNewUseLimitCode );
//	pUseLimitDM->MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	MobileApi.MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pUseLimitDM , sSpeciesKey , 1 , m_pParentMgr , &MobileApi );
	

//	ids = pUseLimitDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ChangeUseLimitCode") );
	ids = pUseLimitDM->EndFrame();

	MobileApi.EndLog();

	// 이력정보를 남길 건인가를 알아본다.
	if( sBossPreUseLimitCode.Compare( sBossNewUseLimitCode ) == 0 ) return 0;

	// 이력 정보를 남긴다.
	CString sRecKey;

	CKolas2up_MobileApi MobileApi2;
	MobileApi2.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi2.SetDM( pUseLimitDM );
	MobileApi2.StartLog();

	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("SPECIES_KEY") , _T("NUMERIC") , sSpeciesKey );
	pUseLimitDM->AddDBFieldData( _T("CREATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
	pUseLimitDM->AddDBFieldData( _T("WORKER") , _T("STRING") , m_pParentMgr->GetUserID() );
	pUseLimitDM->AddDBFieldData( _T("UPDATE_YN") , _T("STRING") , _T("N") );
	pUseLimitDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );

//	pUseLimitDM->MakeRecKey( sRecKey );
	MobileApi2.MakeRecKey( sRecKey );
//	pUseLimitDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

//	pUseLimitDM->MakeInsertFrame( _T("BO_USE_LIMIT_UPDATE_TBL") );
	MobileApi2.AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
	MobileApi2.MakeInsertFrame( _T("SE_USE_LIMIT_UPDATE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	
//	ids = pUseLimitDM->SendFrame();
	ids = MobileApi2.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("ChangeUseLimitCode") );

	pUseLimitDM->EndFrame();

	// 끝~~~


	return 0;

	EFS_END
	return -1;

}



// 우선 순위가 높은것을 알아내준다.
INT CBL_SE_LOC_15::GetBossUseLimitCode( CString &sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
{
	EFS_BEGIN
		
		const INT nCodeCnt = 5;
	CString sOrder[nCodeCnt][2] = 
	{
		{ _T("CL") , _T("0") } , 
		{ _T("CD") , _T("1") } , 
		{ _T("CA") , _T("2") } , 
		{ _T("IZ") , _T("3") } , 
		{ _T("GM") , _T("4") }
	};
	
	INT nIndex;
	CString sTmpCode;
	INT nUseLimitCode=-1;
	for( INT i = 0 ; i < saUseLimitCode.GetSize() ; i++ )
	{
		sTmpCode = saUseLimitCode.GetAt(i);
		
		nIndex = -1;
		for( INT j = 0 ; j < nCodeCnt ; j++ )
		{
			if( sTmpCode.Compare( sOrder[j][0] ) == 0 )
			{
				nIndex = j;
			}
		}
		
		if( nIndex == -1 ) continue;
		
		if( i == 0 )
		{
			nUseLimitCode = _ttoi(sOrder[nIndex][1]);
		}
		else
		{
			if( _ttoi( sOrder[nIndex][1] ) > nUseLimitCode  ) 
				nUseLimitCode = _ttoi( sOrder[nIndex][1] );
		}
	}
	
	if( nUseLimitCode != -1 )
	{
		sUseLimitCode = sOrder[nUseLimitCode][0];
	}
	
	return 0;
	
	EFS_END
	return -1;
	
}



bool CBL_SE_LOC_15::CompareUseLimtCodes( CArray <CString,CString> &saPreUseLimitCode , CArray <CString,CString> &saNewUseLimitCode )
{
	EFS_BEGIN
		
		CString sPreUseLimitCode;
	CString sNewUseLimitCode;
	
	bool IsEqual;
	for( INT i = 0 ; i < saPreUseLimitCode.GetSize() ; i++ )
	{
		IsEqual = FALSE;
		
		sPreUseLimitCode = saPreUseLimitCode.GetAt(i);
		
		for( INT j = 0 ; j < saNewUseLimitCode.GetSize() ; j++ )
		{
			sNewUseLimitCode = saNewUseLimitCode.GetAt(j);
			
			if( sPreUseLimitCode.Compare(sNewUseLimitCode)==0 )
				IsEqual = TRUE;
		}
		if( !IsEqual ) return FALSE;
	}
	
	for( i = 0 ; i < saNewUseLimitCode.GetSize() ; i++ )
	{
		IsEqual = FALSE;
		
		sNewUseLimitCode = saNewUseLimitCode.GetAt(i);
		
		for( INT j = 0 ; j < saPreUseLimitCode.GetSize() ; j++ )
		{
			sPreUseLimitCode = saPreUseLimitCode.GetAt(j);
			
			if( sNewUseLimitCode.Compare(sPreUseLimitCode)==0 )
				IsEqual = TRUE;
		}
		if( !IsEqual ) return FALSE;
	}
	
	return TRUE;
	
	EFS_END
		return FALSE;
	
}




INT CBL_SE_LOC_15::MakeArrayUseLimitCode( CString sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
{
	EFS_BEGIN
		
		sUseLimitCode.TrimLeft();
	sUseLimitCode.TrimRight();
	
	if( sUseLimitCode.IsEmpty() ) return 0;
	
	INT nIndex = -1;
	
	bool IsEnd = FALSE;
	CString sCode;
	while(TRUE)
	{
		nIndex = sUseLimitCode.Find( _T(" ") );
		
		if( nIndex < 0 )
		{
			saUseLimitCode.Add( sUseLimitCode );
			IsEnd = TRUE;
		}
		else
		{
			sCode = sUseLimitCode.Mid( 0 , nIndex );
			sUseLimitCode = sUseLimitCode.Mid( nIndex+1 , sUseLimitCode.GetLength()-nIndex );
			
			saUseLimitCode.Add( sCode );
		}
		
		if( IsEnd ) break;
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}


/// 자료가 작업종인지 소장종인지 알아내어 종테이블에 셋팅한다.
/// 권호도 있다. 인덱스 테이블도 있다.
INT CBL_SE_LOC_15::SettingSpeciesField( CString sSpeciesKey , CString sVolKey )
{
	EFS_BEGIN

	INT ids;

	CString sSql;
	sSql.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s AND WORKING_STATUS IN (%s) ") , 
				sSpeciesKey ,
				_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL112N','SEL2160'")
				);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection());
	TmpDM.ParseCONNECTION_INFO();
	
	CString sValue;
	TmpDM.GetOneValueQuery( sSql , sValue );

	TmpDM.StartFrame();
	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("SPECIES_CLASS") , _T("STRING") , _T("1") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("SPECIES_CLASS") , _T("STRING") , _T("0") );
	}

	TmpDM.MakeUpdateFrame( _T("IDX_SE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	
	TmpDM.MakeUpdateFrame( _T("SE_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );


	// 권호 상태 설정
	// 권호KEY를 지닌 책들이 소장중이라면 소장으로 변경한다.
	sSql.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND WORKING_STATUS IN (%s) ") , 
		sVolKey ,
		_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL112N','SEL2160'")
		);
		
	TmpDM.GetOneValueQuery( sSql , sValue );

	TmpDM.InitDBFieldData();
	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	
	TmpDM.MakeUpdateFrame( _T("SE_CONTENTS_TBL") , _T("VOL_KEY") , _T("NUMERIC") , sVolKey );

	if( _ttoi(sValue) > 0 )
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	}
	else
	{
		TmpDM.AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("1") );
	}
	TmpDM.MakeUpdateFrame( _T("IDX_SE_TOC_TBL") , _T("VOL_KEY") , _T("NUMERIC") , sVolKey );

	CESL_DataMgr TmpDM2;
	TmpDM2.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString sGetArticleKey;
	sGetArticleKey.Format( _T("SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s ") , sVolKey );
	ids = TmpDM2.RestructDataManager(  sGetArticleKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -31 , _T("SettingSpeciesField") );
  
	if( TmpDM2.GetRowCount() > 0 )
	{
		CString sArticleKey;
		for( INT i = 0 ; i < TmpDM2.GetRowCount() ; i++ )
		{
			ids = TmpDM2.GetCellData( i , 0 , sArticleKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -32 , _T("SettingSpeciesField") );

			TmpDM.InitDBFieldData();
			TmpDM.AddDBFieldData( _T("ARTICLE_KEY") , _T("NUMERIC") , sArticleKey );
			TmpDM.AddDBFieldData( _T("IDX_WORK_FLAG") , _T("STRING") , _T("I") );
			TmpDM.AddDBFieldData( _T("INPUT_DATE") , _T("NUMERIC") , _T("SYSDATE") );	
			TmpDM.AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NEXTVAL") );
			TmpDM.MakeInsertFrame( _T("IDX_TMP_SE_TOC_TBL") );
		}
		
		TmpDM.InitDBFieldData();
	}

	/// 기사 색인 홈페이지 동기화


	CLocCommonAPI::MakeIdxWokingStatusFrame( &TmpDM , sSpeciesKey , 1 , m_pParentMgr );
//	CLocCommonAPI::MakeIdxTmpTblInsertFrame( &TmpDM , sSpeciesKey , 1 );
	
	ids = TmpDM.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("SettingSpeciesField") );
	
	TmpDM.EndFrame();

	return 0;

	EFS_END

	return -1;

}