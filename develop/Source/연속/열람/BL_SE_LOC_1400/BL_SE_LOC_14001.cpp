// BL_SE_LOC_14001.cpp: implementation of the CBL_SE_LOC_1400 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_14001.h"

#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_1400::CBL_SE_LOC_1400(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_sMainDMAlias = _T("DM_SE_LOC_1400");
}

CBL_SE_LOC_1400::~CBL_SE_LOC_1400()
{

}

INT CBL_SE_LOC_1400::SPFExcutePath( CString sPathAlias )
{

	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1410();
	}
	if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchSE_LOC_1410FromRFID();
	}
	if( sPathAlias.Compare( REG_REPAIR ) == 0 )
	{
		ids = RegRepair();
	}
	if( sPathAlias.Compare( CANCEL_REPAIR ) == 0 )
	{
		ids = CancelRepair();
	}
	if( sPathAlias.Compare( REQUEST_REPAIR ) == 0 )
	{
		ids = RequestRepair();
	}
	if( sPathAlias.Compare( COMPLETE_REPAIR ) == 0 )
	{
		ids = CompleteRepair();
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


// 검색
INT CBL_SE_LOC_1400::SearchSE_LOC_1410()
{
	EFS_BEGIN

	INT ids;

	// Where절 구성
	CString sWhere;
	ids = GetWhereSE_LOC_1410(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );
	if( ids > 0 ) return 1;

	DWORD START, FINISH;
	START = GetTickCount();
	

	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchSE_LOC_1410") );

	FINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - 수리제본 자료 찾기 검색시 걸린 시간 :  [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	// 화면에 보여주기
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchSE_LOC_1410") );

	return 0;

	EFS_END
	return -1;

}

// RFID검색
INT CBL_SE_LOC_1400::SearchSE_LOC_1410FromRFID()
{
	EFS_BEGIN
		
		INT ids;
	
	// Where절 구성
	CString sWhere;
	GetTempData(_T("WHERE") , sWhere);
	
	
	// Refresh DataManager
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( m_sMainDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchSE_LOC_1410") );
	
	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchSE_LOC_1410") );
	
	// 화면에 보여주기
	ids = ViewMainGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchSE_LOC_1410") );
	
	return 0;
	
	EFS_END
		return -1;
	
}

// Where절 생성
INT CBL_SE_LOC_1400::GetWhereSE_LOC_1410( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1410");	// 검색 CM Alias

	// 1. 기본 Where절 생성
	const INT nCnt = 18;
	CString sControlAlias[nCnt] =
	{
		_T("자료실명") , _T("차수년도") , _T("차수번호1") , _T("차수번호2") , _T("제본등록일자1") , 
		_T("제본등록일자2") , _T("제본의뢰일자1") , _T("제본의뢰일자2") , _T("제본완료일자1") , _T("제본완료일자2") , 
		_T("등록번호구분") , _T("등록번호1") , _T("등록번호2") , _T("서명") , _T("발행자") , 
		_T("발행일1") , _T("발행일2") , _T("상태구분")
	};

	CString sControlData[nCnt];	// 입력값

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWhereSE_LOC_1410") );

	// Control에서 입력값 가져오기
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sControlData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetWhereSE_LOC_1410") );

		// 콘트롤이 디스에이블이라면 검색 조건에 넣지 않는다.
		CESL_Control_Element *pCE = ((CESL_Control_Element*)pCM->FindControlElement( sControlAlias[i] ));
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereSE_LOC_1410") );
		
		if( !m_pParentMgr->GetDlgItem( pCE->CTRL_ID )->IsWindowEnabled() )
			sControlData[i].Empty();

		sControlData[i].TrimLeft(); 
		sControlData[i].TrimRight();
	}

	// 필수 입력 항목 검사
	// 2003년 6월 16일 조병걸 수정 자료실을 필수 입력항목으로 지정하지 않음
	/*
	if( sControlData[0].IsEmpty() )
	{
		AfxMessageBox( _T("자료실을 선택해주십시오.") );
		return 1;
	}
	*/

	TCHAR *sEcoDBFieldName[nCnt-1] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("R.SEQ_YEAR") , _T("R.SEQ_NO") , _T("R.SEQ_NO") , _T("R.REG_DATE") ,
		_T("R.REG_DATE") , _T("R.REQUEST_DATE") , _T("R.REQUEST_DATE") , _T("R.COMPLETE_DATE") , _T("R.COMPLETE_DATE") , 
		_T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_PUBLISHER") , 
		_T("B.PUBLISH_DATE") , _T("B.PUBLISH_DATE")
	};

	TCHAR *sCatDBFieldName[nCnt-1] = 
	{
		_T("B.SHELF_LOC_CODE") , _T("R.SEQ_YEAR") , _T("R.SEQ_NO") , _T("R.SEQ_NO") , _T("R.REG_DATE") ,
		_T("R.REG_DATE") , _T("R.REQUEST_DATE") , _T("R.REQUEST_DATE") , _T("R.COMPLETE_DATE") , _T("R.COMPLETE_DATE") , 
		_T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO") , _T("I.IDX_TITLE") , _T("I.IDX_PUBLISHER") , 
		_T("B.PUBLISH_DATE") , _T("B.PUBLISH_DATE")
	};

	
	INT nDBFieldType[nCnt-1] = 
	{
		ONESTRING , ONESTRING , TWONUMERIC , ETC , TWODATE , 
		ETC , TWODATE , ETC , TWODATE , ETC , 
		THREESTRING , TWOSTRING , TWOSTRING , ONEINDEXSEARCH , ONEINDEXSEARCH ,
		TWODATE , ETC
	};

	TCHAR** sDBFieldName;
	/// ECO Search라면
	if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		sDBFieldName = sEcoDBFieldName;
	else
		sDBFieldName = sCatDBFieldName;
	

	// Where절 생성
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nCnt-1 , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereSE_LOC_1410") );


	
	/// 테스트를 위해 임시 적용
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
		
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	
	// Where절 생성
	ids = CLocCommonAPI::MakeSql( sControlData , sDBFieldName , nDBFieldType , nCnt-1 , sWhere , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-2 , _T("GetWhereBO_LOC_1410") );
	
	
	
	CString sEcoSearchWhere;
	INT nEcoSearchWhere = 0 ;
	if( bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		CMakeSearchData MakeSearchData(m_pParentMgr);	
		
		BOOL bIsEqualSearch ;
		/// 서명 검색 , 발행자 검색
		// 평상시는 우절단 검색 생성
		if( !sControlData[13].IsEmpty() )
		{
			if( sControlData[13].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sControlData[13]);
			if( !bIsEqualSearch ) 
				sControlData[13] += _T("*");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sControlData[13] + _T("') ");
			nEcoSearchWhere++;
		}
		
		if( !sControlData[14].IsEmpty() )
		{
			if( sControlData[14].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sControlData[14]);
			if( !bIsEqualSearch ) 
				sControlData[14] += _T("*");
			
			if( !sEcoSearchWhere.IsEmpty() )
				sEcoSearchWhere += _T(" INTERSECT ");
			
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '") + sControlData[14] + _T("') ");
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
	

	// + 자료의 상태 
	// 제적중이 아니고 대출중이 아니고 제본중이 아닌 비치 자료
	CString sWhere2;
	if( sControlData[17].Compare(_T("제본등록대상자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL214O' ) ");
	}
	else if( sControlData[17].Compare(_T("제본처리중자료")) == 0 )
	{
		sWhere2 = _T(" B.WORKING_STATUS IN ( 'SEL213O' ) ");
	}
	else if( sControlData[17].Compare(_T("제본등록자료")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '1' ");
	}
	else if( sControlData[17].Compare(_T("제본의뢰자료")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '2' ");
	}
	else if( sControlData[17].Compare(_T("제본완료자료")) == 0 )
	{
		sWhere2 = _T(" R.STATUS = '3' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	return 0;

	EFS_END
	return -1;

}


// 화면에 보여주기
// 1. 코드 -> 설명으로 Convert
// 2. 그리드 View
INT CBL_SE_LOC_1400::ViewMainGrid()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1400");
	CString sDMAlias = _T("DM_SE_LOC_1400");

	// 1. 코드 -> 설명
	// 청구기호 , 상태구분
	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-1 , _T("ViewMainGrid") );

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMainGrid") );

	CString sWorkingStatus;	// 자료상태
	CString sRepairStatus;	// 제본상태
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("자료상태") , i , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMainGrid") );
		
		ids = pDM->GetCellData( _T("제본상태") , i , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMainGrid") );

		if( sRepairStatus.Compare( _T("1") ) == 0 ) 
			sRepairStatus = _T("등록");
		else if( sRepairStatus.Compare( _T("2") ) == 0 )
			sRepairStatus = _T("의뢰");
		else if( sRepairStatus.Compare( _T("3") ) == 0 )
			sRepairStatus = _T("완료");
		else
		{
			ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMainGrid") );

			sRepairStatus = sWorkingStatus;
		}
	
		ids = pDM->SetCellData( _T("제본상태") , sRepairStatus  , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );

		ids = pDM->SetCellData( _T("자료상태") , sWorkingStatus, i  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMainGrid") );
		
	}


	// 2. 그리드 View
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-7 , _T("ViewMainGrid") );

	// 3. 자료실 View
	CString sShelfLocCode;
	ids = m_pParentMgr->GetControlData( _T("CM_SE_LOC_1410") , _T("자료실명") , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("ViewMainGrid") );
	
	ids = m_pParentMgr->SetControlData( _T("CM_SE_LOC_1400") , _T("자료실") , sShelfLocCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-9 , _T("ViewMainGrid") );

	return 0;

	EFS_END
	return -1;

}

// 제본 등록
/// 선정된 자료에 대해 일괄 처리 
/// 1. 등록대상 자료인지 확인 
/// 2. 책상태 UPDATE , 수리제본 정보 INSERT
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
INT CBL_SE_LOC_1400::RegRepair()
{
	EFS_BEGIN

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

	// 제본등록일자 입력값 검사
	CString sRegDate;
	ids = GetInputRegDate( sRegDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepair") );

	if( sRegDate.IsEmpty() )
	{
		AfxMessageBox( _T("제본등록일을 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	INT nSuccessCnt = 0;	// 수리제본등록에 성공한 자료 수 
	CString sRepairStatus; // 제본상태

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

		ids = pDM->GetCellData( _T("제본상태") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepair") );

		if( sRepairStatus.Compare( _T("연속비치자료") ) != 0  && 
			( sRepairStatus.Compare(_T("완료"))!=0 ) && 
			( sRepairStatus.Compare(_T("취소"))!=0 ) && 
			( sRepairStatus.Compare(_T("파손자료"))!=0 ))
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 제본 등록 DB 작업
		ids = RegRepairDBProc( nIndex , &TmpDM , sRegDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepair") );
		if( ids > 0 ) return ids;

		// 제본 등록 DM 작업
		ids = RegRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepair") );

		// 제본 등록 화면 작업
		ids = RegRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 제본 등록하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 제본등록할수 없는 상태이기 때문에 등록에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료가 제본 등록하였습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 등록할수 없는 상태이기 때문에 등록에 실패하였습니다.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// 제본등록 DB 작업
/// 1. 책정보 UPDATE
/// 2. 수리제본정보INSERT
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_SE_LOC_1400::RegRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sRegDate )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("자료상태") , _T("수리제본자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RegRepairDBProc") ); 

	// 프레임 생성
	// 2. 수리제본정보 INSERT
	CString sRepairRecKey;
	ids = pDM->MakeRecKey( sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("RegRepairDBProc") ); 

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->AddDBFieldData( _T("BOOK_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") );
	pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , _T("SE") );
	pDM->AddDBFieldData( _T("REG_DATE") , _T("STRING") , sRegDate );
	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
	pDM->MakeInsertFrame( _T("CO_REPAIR_BINDING_TBL") );
	pDM->InitDBFieldData();

	// 2.1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("제본상태") , _T("등록"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("수리제본정보KEY"), sRepairRecKey , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("등록일"), sRegDate , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("발행형태구분"), _T("단행") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("차수년도") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("차수번호") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("의뢰일") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("완료일") , _T("") , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// 제본 등록 DM작업
/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_1400::RegRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// 제본등록 화면 그리드작업
/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_1400::RegRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


// 프로그래스바 생성
CLocProgressBarDlg* CBL_SE_LOC_1400::CreateProgressBarDlg( INT nUpper )
{
	EFS_BEGIN

	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;

	EFS_END
	return NULL;

}

// 프로그래스바 종료
INT CBL_SE_LOC_1400::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	EFS_BEGIN

	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;

	EFS_END
	return -1;

}


/// 화면작업
/// 메인 그리드 포인터 참조
CESL_Grid* CBL_SE_LOC_1400::GetMainGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_SE_LOC_1400");
	CString sGridAlias = _T("GRID");

	// CM 참조
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;

	EFS_END
	return NULL;

}

INT CBL_SE_LOC_1400::GetInputRegDate( CString &sRegDate )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1420");
	CString sControlAlias = _T("제본등록일자");

	ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias , sRegDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputRegDate") );


	return 0;

	EFS_END
	return -1;

}

/// 그리드에 선택된 자료가 있는지 검사
INT CBL_SE_LOC_1400::GetSelectMainGridCnt()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();

	EFS_END
	return -1;

}


/// 제본 완료 PATH
/// 선정된 자료에 대해 일괄 처리 
/// 1. 제본의뢰 자료인지 확인 
/// 2. 책상태 UPDATE , 수리제본 정보 UPDATE
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요	
INT CBL_SE_LOC_1400::CompleteRepair()
{
	EFS_BEGIN

	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 제본완료일자 참조
	CString sCompleteDate;
	ids = GetInputCompleteDate( sCompleteDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepair") );

	if( sCompleteDate.IsEmpty() )
	{
		AfxMessageBox( _T("제본완료일을 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepair") );

	INT nSuccessCnt = 0;	// 수리제본완료에 성공한 자료 수 
	CString sRepairStatus;	// 제본상태

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

		ids = pDM->GetCellData( _T("제본상태") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CompleteRepair") );

		if( sRepairStatus.Compare(_T("의뢰"))!=0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 제본 등록 DB 작업
		ids = CompleteRepairDBProc( nIndex , &TmpDM , sCompleteDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CompleteRepair") );
		if( ids > 0 ) return ids;

		// 제본 등록 DM 작업
		ids = CompleteRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepair") );

		// 제본 등록 화면 작업
		ids = CompleteRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CompleteRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 제본 완료되었습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 제본 완료할수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료가 제본 완료되었습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 완료할수 없는 상태이기 때문에 작업에 실패하였습니다.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 DB 작업
/// 1. 책정보 UPDATE
/// 2. 수리제본정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록
INT CBL_SE_LOC_1400::CompleteRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sCompleteDate )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CompleteRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CompleteRepairDBProc") ); 

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("자료상태") , _T("연속비치자료") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , _T("SEL213O") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CompleteRepairDBProc") ); 

	// 2. 수리제본정보 UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("수리제본정보KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CompleteRepairDBProc") ); 

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("3") );
	pDM->AddDBFieldData( _T("COMPLETE_DATE") , _T("STRING") , sCompleteDate );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("제본상태") , _T("완료"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CompleteRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("완료일"), sCompleteDate , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CompleteRepairDBProc") ); 

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CompleteRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 DM작업
/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_1400::CompleteRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 화면 그리드작업
/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_1400::CompleteRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CompleteRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CompleteRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


/// 제본완료일자 참조
INT CBL_SE_LOC_1400::GetInputCompleteDate( CString &sCompleteDate )
{
	EFS_BEGIN

	sCompleteDate = CLocCommonAPI::GetDBTime(m_pParentMgr).Format(_T("%Y/%m/%d"));
	return 0;

	EFS_END
	return -1;

}



/// 등록 취소 PATH
/// 선정된 자료에 대해 일괄 처리 
/// 1. 제본등록 자료인지 확인 
/// 2. 책상태 UPDATE , 수리제본 정보 UPDATE
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요	
INT CBL_SE_LOC_1400::CancelRepair()
{
	EFS_BEGIN

	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepair") );

	INT nSuccessCnt = 0;	// 수리제본완료에 성공한 자료 수 
	CString sRepairStatus;	// 제본상태

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

		ids = pDM->GetCellData( _T("제본상태") , nIndex , sRepairStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRepair") );

		if( sRepairStatus.Compare(_T("등록"))!=0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		// 등록 취소 DB 작업
		ids = CancelRepairDBProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepair") );
		if( ids > 0 ) return ids;

		// 등록 취소 DM 작업
		ids = CancelRepairDMProc( nIndex  , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepair") );

		// 등록 취소 화면 작업
		ids = CancelRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록 취소되었습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록 취소할수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료가 등록 취소되었습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 등록 취소할수 없는 상태이기 때문에 작업에 실패하였습니다.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 DB 작업
/// 1. 책정보UPDATE
/// 2. 수리제본정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록
INT CBL_SE_LOC_1400::CancelRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey,sPreWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("이전자료상태") , 0 , sPreWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sPreWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sPreWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , _T("SEL213O") );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();
	
	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sPreWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("자료상태") , sPreWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , _T("SEL213O") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelRepairDBProc") ); 

	// 2. 수리제본정보 UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("수리제본정보KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CancelRepairDBProc") ); 

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("4") );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("제본상태") , _T("취소"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelRepairDBProc") ); 

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CancelRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 DM작업
/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_1400::CancelRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// 제본완료 화면 그리드작업
/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_1400::CancelRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}



/// 제본 의뢰 PATH
/// 선정된 자료에 대해 일괄 처리 
/// 1. 수리제본 정보 UPDATE
/// 2. DM 변경
/// 3. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요	
INT CBL_SE_LOC_1400::RequestRepair()
{
	EFS_BEGIN

	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepair") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	CString sInputData[3];	// 0 : 차수년도 , 1 : 차수번호 , 2 : 의뢰일자
	ids = GetRequestInputData( sInputData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	// DM 참조 선택
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepair") );

	INT nSuccessCnt = 0;	// 제본의뢰에 성공한 자료 수 
	CString sRepairStatus;	// 제본상태

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

		// 제본 의뢰 DB 작업
		ids = RequestRepairDBProc( nIndex , &TmpDM , sInputData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RequestRepair") );
		if( ids > 0 ) return ids;

		// 제본 의뢰 DM 작업
		ids = RequestRepairDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RequestRepair") );

		// 제본 의뢰 화면 작업
		ids = RequestRepairGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RequestRepair") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	if( nSuccessCnt > 0 )
	{
		ids = UpdateLastNumber();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepair") )
	}

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 제본 의뢰되었습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 제본 의뢰할수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료가 제본 의뢰되었습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 제본 의뢰할수 없는 상태이기 때문에 작업에 실패하였습니다.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;

	EFS_END
	return -1;

}


/// 제본의뢰 DB 작업
/// 1. 수리제본정보 UPDATE
/// 2. TmpDM에 변경된 정보 기록
INT CBL_SE_LOC_1400::RequestRepairDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString *sInputData)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RequestRepairDBProc") ); 

	pDM->StartFrame();

	// 2. 수리제본정보 UPDATE
	CString sRepairRecKey;
	ids = pTmpDM->GetCellData( _T("수리제본정보KEY") , 0 , sRepairRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RequestRepairDBProc") ); 

	pDM->AddDBFieldData( _T("SEQ_YEAR") , _T("STRING") , sInputData[0] );
	pDM->AddDBFieldData( _T("SEQ_NO") , _T("NUMERIC") , sInputData[1] );
	pDM->AddDBFieldData( _T("REQUEST_DATE") , _T("STRING") , sInputData[2] );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("2") );
	pDM->MakeUpdateFrame( _T("CO_REPAIR_BINDING_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRepairRecKey );
	pDM->InitDBFieldData();

	// 2.1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("제본상태") , _T("의뢰"), 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("차수년도") , sInputData[0], 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("차수번호") , sInputData[1] , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("의뢰일") , sInputData[2], 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RequestRepairDBProc") ); 

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("RequestRepairDBProc") ); 

	return 0;

	EFS_END
	return -1;

}




/// 제본의뢰 DM작업
/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_SE_LOC_1400::RequestRepairDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}


/// 제본으뢰 화면 그리드작업
/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_SE_LOC_1400::RequestRepairGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RequestRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RequestRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}


/// 제본의뢰 입력 정보 참조
/// 0:차수년도 , 1:차수번호 , 2:제본의뢰일자
INT CBL_SE_LOC_1400::GetRequestInputData( CString *sInputData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1430");
	// 차수년도 , 차수번호 , 의뢰일자가 들어가 있는지 검사
	const INT nCnt = 3;
	CString sControlAlias[nCnt] = 
	{
		_T("차수년도") , _T("차수번호") , _T("제본의뢰일자") 
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sInputData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetRequestInputData") );
		sInputData[i].TrimLeft(); sInputData[i].TrimRight();
	}

	return 0;

	EFS_END
	return -1;

}

/// 등록 상태가 아닌 자료의 'V'체크 해제
INT CBL_SE_LOC_1400::SelectCheckRegStatus()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1400");

	CESL_Grid* pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SelectCheckRegStatus") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();

	CString sRepairStatus;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("제본상태") , sRepairStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SelectCheckRegStatus") );
		
		if( sRepairStatus.Compare( _T("등록") ) == 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}

		pGrid->SetAt( nIndex , 0 , _T("") );
	
		pGrid->SelectGetNext();
	}

	return 0;

	EFS_END
	return -1;

}


/// 차수 번호 생성
/// 마지막 번호관리에서 가져온다.
INT CBL_SE_LOC_1400::MakeSeqNo( CString sYear , CString &sSeqNo )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_LAST_NUMBER_TBL");
	CString sPublishFormCode = _T("SE");
	CString sKindCode = _T("BND_NO");
	CString sSql;
	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMaxCheckNumber") );

	INT nCurrentYear = (CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear();

	// 해당 년의 마지막 등록번호가 존재하는지 검사
	sSql.Format( _T("SELECT COUNT(*) FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );


	CString sCount;
	ids = pDM->GetOneValueQuery( sSql , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("AutoSetUserNo") );

	// 없을 경우
	if( _ttoi( sCount ) < 1 )
	{
		CString sYear;
		sYear.Format( _T("%d") , nCurrentYear );

		CString sRecKey;
		pDM->MakeRecKey( sRecKey );

		pDM->StartFrame();

		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishFormCode );
		pDM->AddDBFieldData( _T("KIND_CODE") , _T("STRING") , sKindCode );
		pDM->AddDBFieldData( _T("MAKE_YEAR") , _T("STRING") , sYear );
		pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("STRING") , _T("0") );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );


		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
		pDM->MakeInsertFrame( sTableName );
		pDM->SendFrame();
		pDM->EndFrame();
	}	

	// 해당 년의 마지막 번호 가져오기
	sSql.Format( _T("SELECT LAST_NUMBER FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );

	ids = pDM->GetOneValueQuery( sSql , sSeqNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AutoSetUserNo") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_1400::UpdateLastNumber()
{
	EFS_BEGIN
	
	INT ids;
	
	CString sTableName = _T("CO_LAST_NUMBER_TBL");
	CString sPublishFormCode = _T("SE");
	CString sKindCode = _T("BND_NO");
	CString sSql;
	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMaxCheckNumber") );
	
	INT nCurrentYear = (CLocCommonAPI::GetDBTime(m_pParentMgr)).GetYear();
	
	// 해당 년의 마지막 등록번호가 존재하는지 검사
	sSql.Format( _T("SELECT COUNT(*) FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );
	
	
	CString sCount;
	ids = pDM->GetOneValueQuery( sSql , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-2 , _T("AutoSetUserNo") );
	
	// 없을 경우
	CString sRecKey;
	if( _ttoi( sCount ) < 1 )
	{
		CString sYear;
		sYear.Format( _T("%d") , nCurrentYear );
		
		pDM->MakeRecKey( sRecKey );
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishFormCode );
		pDM->AddDBFieldData( _T("KIND_CODE") , _T("STRING") , sKindCode );
		pDM->AddDBFieldData( _T("MAKE_YEAR") , _T("STRING") , sYear );
		pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("STRING") , _T("1") );
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );
		
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
		pDM->MakeInsertFrame( sTableName );
		pDM->SendFrame();
		pDM->EndFrame();
	}	
	
	// 해당 년의 마지막 번호 가져오기
	sSql.Format( _T("SELECT REC_KEY FROM %s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR=%d AND PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' ") , 
		sTableName , nCurrentYear , sPublishFormCode , sKindCode );
	
	ids = pDM->GetOneValueQuery( sSql , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AutoSetUserNo") );

	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("LAST_NUMBER") , _T("NUMERIC") , _T("LAST_NUMBER+1") );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AutoSetUserNo") );
	pDM->EndFrame();

	return 0;

	EFS_END
	return -1;
}
