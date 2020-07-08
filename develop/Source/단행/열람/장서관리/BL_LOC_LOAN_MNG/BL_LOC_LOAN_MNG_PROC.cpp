// BL_LOC_LOAN_MNG_PROC.cpp: implementation of the CBL_LOC_LOAN_MNG_PROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_LOC_LOAN_MNG_PROC.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h" //상세보기
#include "..\BL_LOC_LOAN\BO_LOC_3170.h"	//예약 만기일 부여
#include "..\BL_LOC_LOAN\BO_LOC_3170_ReserveInfo.h"	//한줄서기 예약시 예약 만기일 부여
#include "..\BL_LOC_LOAN\SMSPreviewDlg.h"	//SMS 발송
#include "..\BL_LOC_LOAN\KL_UserSearchDlg.h" // 통합도서서비스
#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"
// SBL
#include "SBL.h"
// 2011.08.31 ADD : 3000Api
#include "BO_LOC_3000Api.h"
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_LOC_LOAN_MNG_PROC::CBL_LOC_LOAN_MNG_PROC(CESL_Mgr *pWnd,CLonaManageValue *pLona,CLoanShareManager* pLoanShareManager)
	: CSPF_Work(pWnd)
{
	m_pLoan = pLona;
	m_pLoanShareManager = pLoanShareManager;

	m_pReservationInfo = NULL;
	m_pReservationInfo = new CBL_LOC_RESERVATOIN_PROC(m_pParentMgr,m_pLoanShareManager,pLona);

	m_strUserID = pWnd->GetUserID();
	m_strLibCode=_T("");

	// 2006-01-12 ADD BY PDW(Jr) ============================================================
	// 국방대 플래그
	m_pLoan->GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , m_sDefenceFlag );	
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();
	// ======================================================================================

	m_nUnmannedLoanStatus = 0;
}

CBL_LOC_LOAN_MNG_PROC::~CBL_LOC_LOAN_MNG_PROC()
{
	if( m_pReservationInfo != NULL ) delete m_pReservationInfo;
}

INT CBL_LOC_LOAN_MNG_PROC::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	AfxGetApp()->DoWaitCursor(1);
	if( sPathAlias.Compare( _T("대출자료관리_검색") ) == 0 )
	{
		ids = SearchLoanData();
	}
	else if( sPathAlias.Compare( _T("RFID검색") ) == 0 )
	{
		ids = SearchLoanDataFromRFID();
	}
	else if( sPathAlias.Compare( _T("회원정보") ) == 0 )
	{
		ids = ViewMemberInfo();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_수정") ) == 0 )
	{
		ids = ModifyLoanInfo();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_일괄수정") ) == 0 )
	{
		ids = ModifyAllLoanInfo();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_예약취소") ) == 0 )
	{
		ids = CancelReservation();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_예약만기일부여") ) == 0 )
	{
		ids = RecordExpireDate();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_예약처리_회원정보") ) == 0 )
	{
		ids = ViewMemberInfo2();
	}
	else if( sPathAlias.Compare( _T("대출자료관리_상세정보") ) == 0 )
	{
		ids = ViewDetail();
	}
	else if( sPathAlias.Compare( _T("메일") ) == 0 )
	{
		ids = OpenMail();
	}
	else if( sPathAlias.Compare( _T("무인반납처리") ) == 0 )
	{
		ids = ReturnFromStation();
	}

	//++2008.05.27 UPDATE BY PWR {{++
	// 타관반납 추가, 기존타관->분관
	// 분관반납자료처리
	else if( sPathAlias.Compare( _T("분관반납처리") ) == 0 )
	{
		ids = ReturnFromOtherManageCode();
	}
	// 타관반납자료처리
	else if( sPathAlias.Compare( _T("타관반납처리") ) == 0 )
	{
		ids = ReturnFromOtherLibCode();
	}
	/*
	else if( sPathAlias.Compare( _T("타관반납처리") ) == 0 )
	{
		ids = ReturnFromOtherManageCode();
	}
	*/
	//--2008.05.27 UPDATE BY PWR --}}

	/*
	else if( sPathAlias.Compare( _T("대출자료관리_반납확인메일보내기") ) == 0 )
	{
		ids = OpenMail(RESERVE_BOOK_RETURN);
	}
	*/
	else if( sPathAlias.Compare( _T("예약순서재부여") ) == 0 )
	{
		// 예약 기능 제한
		// 이동도서관일 경우
		if( (m_pParentMgr->m_pInfo->MODE == 10000) || (m_pParentMgr->m_pInfo->MODE == 30000) )
			return 0;

		ids = ReViewOrder();
	}
	if( ids < 0 ) 
	{
		ViewMessage( _T("오류가 발생하여 기능을 수행하지 못하였습니다.") );
		return -1;
	}
	return 0;
	
	AfxGetApp()->DoWaitCursor(-1);
	
	EFS_END
	return -1;

}


VOID CBL_LOC_LOAN_MNG_PROC::ViewMessage( CString sMsg )
{
	EFS_BEGIN

	MessageBox( m_pParentMgr->m_hWnd , sMsg , _T("대출자료관리") , MB_ICONINFORMATION | MB_OK );

	EFS_END
}

// 대출정보관리_검색 PATH
INT CBL_LOC_LOAN_MNG_PROC::SearchLoanDataFromRFID()
{
	EFS_BEGIN
	
	BEGIN_SBL();

	INT ids;
	
	CString sDMAlias[3] = { _T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") };
	CString sCMAlias = _T("CM_BO_LOC_3200");
	
	CESL_DataMgr* pDstDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200"));
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("SearchLoanData") );
	
	pDstDM->FreeData();

	CString sSerialNo;
	GetTempData( _T("WHERE") , sSerialNo );
	
	// DB작업
	CString sWhere;		// WHERE절
	CESL_DataMgr *pDM;	// DM MANAGER
	INT nDMCnt = 3; 
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		// 조건절 생성
//DEL 		if( i == 0 || i == 2 )
			sWhere.Format( _T("B.WORKNO = '%s' ") , sSerialNo);
//DEL 		else
//DEL 			sWhere.Format( _T("NB.WORKNO = '%s' ") , sSerialNo);
		
		if( sWhere.IsEmpty() ) continue;
		
		pDM = m_pParentMgr->FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchLoanData") );
		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanData") );

		if( pDM->GetRowCount() == 0 ) continue;
		
		ids = CLocCommonAPI::AddDM( pDM , pDstDM , m_pParentMgr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") )
	}

	// 2011.09.05 ADD : 3000Api
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;
		for(INT i = 0; i < pDstDM->GetRowCount(); i++)
		{
			pDstDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDstDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDstDM->GetCellData(_T("전화번호_자택"), i, strGetData);
			asDes3.Add(strGetData);
			pDstDM->GetCellData(_T("전화번호_근무지"), i, strGetData);
			asDes4.Add(strGetData);
			pDstDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDstDM->GetCellData(_T("주소_자택"), i, strGetData);
			asDes6.Add(strGetData);
			pDstDM->GetCellData(_T("주소_근무지"), i, strGetData);
			asDes7.Add(strGetData);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes6);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes7);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDstDM->GetRowCount(); i++)
				{
					pDstDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDstDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDstDM->SetCellData(_T("전화번호_자택"), asDes3.GetAt(i), i);
					pDstDM->SetCellData(_T("전화번호_근무지"), asDes4.GetAt(i), i);
					pDstDM->SetCellData(_T("휴대폰"), asDes5.GetAt(i), i);
					pDstDM->SetCellData(_T("주소_자택"), asDes6.GetAt(i), i);
					pDstDM->SetCellData(_T("주소_근무지"), asDes7.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
	}
	
	// 순위 정하기
	 ReViewOrder(_T("DM_BO_LOC_3200"));
	
	// 정렬하기(M_2004-1006_HAN...)
	const INT SORT_COND = 2;
	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
	CString sSortCond[SORT_COND] = { _T("대출자"), _T("대출일") };

	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );
	//pDstDM->SORT( _T("대출일") , TRUE );
	
	// 화면 작업
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") );
	
	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	if( pDstDM->GetRowCount() > 0 )	AfxMessageBox( _T("자료가 검색되었습니다.") );
	else							AfxMessageBox( _T("자료가 존재하지 않습니다.") );

	return 0;
	
EFS_END

	return -1;	
}

// 대출정보관리_검색 PATH

/*
@Description
 -
@Parameter
 -
@Return Value
 - 0 : 성공
 - 음수 : 실패
@Mini Spec
//DEL 2009.02.19 COMMENT BY CJY
//DEL 대출자료관리 검색속도 개선을 위한 로직변경
//DEL 기존 방식 
//DEL  1. 세개의 DM에서 각각 단행/연속/긴급자료를 검색한다.
//DEL  2. 검색된 내용을 DM_BO_LOC_3200에 추가시킨다.
//DEL 변경된 방식
//DEL  1. 세개의 DM에서 각각 테이블명과 검색조건을 얻어온다.
//DEL  2. DM_BO_LOC_3200에 해당 테이블명과 검색조건을 설정한후 SQL문을 얻어온다.
//DEL  3. 얻어온 SQL문을 UNION ALL로 묶은 후 검색한다
//DEL - 주의사항 -
//DEL 1. 검색은 DM_BO_LOC_3200로만 진행되기 때문에 DM_BO_LOC_3200_BO 등에 칼럼을 추가해도 의미가 없다.
//DEL 2. 하지만 테이블명이나 조인조건절은 가져오기 때문에 단행/연속/긴급DM을 지우면 안된다.
//DEL 3. RefreshDataMgr로는 UNION이 지원되지 않기 때문에 RestructDataMgr을 사용하였다.
//DEL    기존처럼 ALIAS를 통해서 GetCellData나 SetCellData를 하기 위해서는 
//DEL    SELECT되는 칼럼의 순서와 DM의 칼럼의 순서가 일치해야 되는 데,
//DEL    UDF를 사용하게 되면 칼럼갯수자체가 틀려지기 때문에 사용해선 안된다.
//DEL    UDF방식을 사용하려면 FIELD NAME 항목에 NULL로 설정해준다.
//DEL 4. 단행/연속/긴급 테이블에 따라 가져와야 될 항목이 다른 경우 아래에서 Replace를 통해 교체해주면 된다.
*/
INT CBL_LOC_LOAN_MNG_PROC::SearchLoanData()
{
	EFS_BEGIN

	BEGIN_SBL();

	INT ids;

	CString sDMAlias[3] = { _T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") };
	CString sCMAlias = _T("CM_BO_LOC_3200");

	CESL_DataMgr* pDstDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200"));
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("SearchLoanData") );

	pDstDM->FreeData();

	// DB작업
	CString sWhere;		// WHERE절
	CESL_DataMgr *pDM;	// DM MANAGER
	INT nDMCnt = 3; //일단은 단행 자료만 , 긴급대출자료만
	bool bFirstWaringMsg = FALSE;

	CString strFullQuery;
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		// 조건절 생성
		ids = GetSqlBO_LOC_3210( sWhere , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchLoanData") );
		if( ids == 1 ) continue;

		if( sWhere.IsEmpty() && !bFirstWaringMsg )
		{
			if( i == 0 )
			{
				ids = MessageBox( m_pParentMgr->m_hWnd , _T("검색 조건이 부족하여 시간이 오래 걸리거나 제대로 기능을 수행하지 못할수 있습니다. \r\n검색하시겠습니까?") , _T("경고") , MB_ICONWARNING | MB_YESNO );
				if( ids == IDNO )
					return 0;
				bFirstWaringMsg = TRUE;
			}
		}

		pDM = m_pParentMgr->FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchLoanData") );
		

		// 2009.02.11 ADD BY CJY : 단행/연속/긴급으로 각각 나누어 검색후 합치던 것을 UNION으로 통합
		CString strExtraCondition = pDM->EXTRA_CONDITION;
		CString strTblName = pDM->TBL_NAME;
		
		pDstDM->TBL_NAME = pDM->TBL_NAME;
		pDstDM->CONDITION = pDM->CONDITION;
		pDstDM->EXTRA_CONDITION = _T("");
		pDstDM->SetCONNECTION_INFO(_T(""));
			

		CString strQuery, strLSQuery, strCLQuery;

		INT aColIndex[256];	
		strQuery = strLSQuery = strCLQuery = pDstDM->MakeSelectQuery(sWhere, aColIndex);
	
		// VIEW검색을 UNION으로 나누어서 검색
		strLSQuery.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
		strCLQuery.Replace(_T("LH_STORAGE_V01"), _T("CO_LOAN_TBL"));

		strQuery = strLSQuery + _T(" UNION ALL ") + strCLQuery;

		//DEL 특정 칼럼에 대해 단행/연속/긴급에 따라 SELECT되는 항목이 다르다면 이곳에서 바꿔치기 한다.
		if(_T("DM_BO_LOC_3200_BO") == sDMAlias[i])
		{
			strQuery.Replace(_T("<$자료구분$>"),		_T("'단행'"));
			strQuery.Replace(_T("<$단행/연속구분$>"),	_T("'MO'"));
			strQuery.Replace(_T("<$발행년$>"),			_T("B.PUBLISH_YEAR"));
			strQuery.Replace(_T("<$가족ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));
			strQuery.Replace(_T("<$부록여부$>"),		_T("B.BOOK_APPENDIX_FLAG")); // 2009.05.13 ADD BY PWR : 부록여부 추가
		}
		else if(_T("DM_BO_LOC_3200_SE") == sDMAlias[i])
		{
			strQuery.Replace(_T("DECODE( B.BOOK_APPENDIX_FLAG, 'A', NULL, 'B', B.VOL_TITLE )"),	_T("NULL"));
			strQuery.Replace(_T("<$자료구분$>"),		_T("'연속'"));
			strQuery.Replace(_T("<$단행/연속구분$>"),	_T("'SE'"));
			strQuery.Replace(_T("<$발행년$>"),			_T("NULL"));
			strQuery.Replace(_T("<$가족ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));
			strQuery.Replace(_T("<$부록여부$>"),		_T("B.BOOK_TYPE")); // 2009.05.13 ADD BY PWR : 부록여부 추가
		}
		else if(_T("DM_BO_LOC_3200_NON_DB") == sDMAlias[i])
		{
			strQuery.Replace(_T("B.BOOK_CODE"),			_T("NULL"));
			strQuery.Replace(_T("B.COPY_CODE"),			_T("NULL"));
			strQuery.Replace(_T("B.VOL_CODE"),			_T("NULL"));
			strQuery.Replace(_T("I.AUTHOR_INFO"),		_T("B.AUTHOR"));
			strQuery.Replace(_T("I.IDX_TITLE"),			_T("B.TITLE"));
			strQuery.Replace(_T("I.PUB_INFO"),			_T("B.PUBLISHER"));
			strQuery.Replace(_T("I.REC_KEY"),			_T("NULL"));
			strQuery.Replace(_T("I.TITLE_INFO"),		_T("B.TITLE"));
			strQuery.Replace(_T("DECODE( B.BOOK_APPENDIX_FLAG, 'A', NULL, 'B', B.VOL_TITLE )"),	_T("NULL"));			
			strQuery.Replace(_T("<$자료구분$>"),		_T("'긴급'"));
			strQuery.Replace(_T("<$단행/연속구분$>"),	_T("'NB'"));
			strQuery.Replace(_T("<$발행년$>"),			_T("NULL"));
			strQuery.Replace(_T("<$가족ID$>"),			_T("/*SQL TUNING 2005.08 PDJ*/ /*+ RULE */ (SELECT /*+ INDEX_DESC (CLFI CO_LOAN_FAMILY_ID_IDX1) */ CLFI.ID FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLU.FID_KEY=CLFI.REC_KEY AND ROWNUM=1)"));			
			strQuery.Replace(_T("<$부록여부$>"),		_T("NULL")); // 2009.05.13 ADD BY PWR : 부록여부 추가
		}

		if(TRUE == strFullQuery.IsEmpty()) strFullQuery = strQuery;
		else strFullQuery = strFullQuery + _T(" UNION ALL ") + strQuery;		

		// 2009.02.11 DEL BY CJY : 단행 / 연속 / 긴급으로 나누어 검색하는 대신 한번에 검색하도록 변경되어 삭제
//DEL 		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanData") );

//DEL 		if( pDM->GetRowCount() == 0 ) continue;
//DEL 		ids = CLocCommonAPI::AddDM( pDM , pDstDM , m_pParentMgr );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanData") );

	}

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();

	strFullQuery = strFullQuery + _T(" ORDER BY NAME ASC, LOAN_DATE DESC");

//DEL DM_BO_LOC_3200에 UDF로 된 항목이 없다고 가정한다.
//DEL UDF로 지정되어야 하는 항목은 DATA_TYPE를 FIELD_NAME를 NULL로 설정
	pDstDM->RestructDataManager(strFullQuery);

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - 대출 자료 관리 자료 찾기 검색시 걸린 시간 :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));

	// 2007.08.20 update by pdw
	// 검색이후 정렬 순서에 따라 정렬되도록 변경
/*	// 정렬하기(M_2004-1006_HAN...)
	const INT SORT_COND = 2;
	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
	CString sSortCond[SORT_COND] = { _T("대출자"), _T("대출일") };

	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );
	//pDstDM->SORT( _T("대출일") , TRUE );
*/

	// 2009.02.20 DEL BY CJY : 데이터베이스 검색시 이미 정렬을 해서 가지고 온다.
// 	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
// 	if( !pCM )ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("SearchLoanData") );
	
// 	CString strSORT;
// 	pCM->GetControlMgrData( _T("정렬기준") , strSORT , -1 , 0 );
// 	const INT SORT_COND = 2;
// 	INT nSortCond[SORT_COND] = { ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_DESC };
// 	CString sSortCond[SORT_COND] = { _T("대출자"), strSORT };

//  	pDstDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );

	// 2011.08.31 ADD : 3000Api
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;

	// 2007.12.22 ADD BY PWR : 관리구분 내용 관리구분설명으로 변경
	INT nCnt = pDstDM->GetRowCount();
	CString sData, sDesc;
	for(i = 0; i < nCnt; i++)
	{
		ids = pDstDM->GetCellData(_T("관리구분"), i, sData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AllControlDisplayGrid") );
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );
		ids = pDstDM->SetCellData( _T("관리구분설명"), sDesc, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AllControlDisplayGrid") );		

		// 2011.08.31 ADD : 3000Api
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			// 1:주민등록번호, 2:E_MAIL, 3:전화번호_자택, 4:전화번호_근무지, 5:휴대폰, 6:주소_자택, 7:주소_근무지
			CString strGetData;
			pDstDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDstDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDstDM->GetCellData(_T("전화번호_자택"), i, strGetData);
			asDes3.Add(strGetData);
			pDstDM->GetCellData(_T("전화번호_근무지"), i, strGetData);
			asDes4.Add(strGetData);
			pDstDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDstDM->GetCellData(_T("주소_자택"), i, strGetData);
			asDes6.Add(strGetData);
			pDstDM->GetCellData(_T("주소_근무지"), i, strGetData);
			asDes7.Add(strGetData);
		}
	}

	// 2011.08.31 ADD : 3000Api
	if(0 < asDes1.GetSize() && TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes6);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes7);
					pApi->CloseSocket();
				}
				for(i = 0; i < nCnt; i++)
				{
					pDstDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDstDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDstDM->SetCellData(_T("전화번호_자택"), asDes3.GetAt(i), i);
					pDstDM->SetCellData(_T("전화번호_근무지"), asDes4.GetAt(i), i);
					pDstDM->SetCellData(_T("휴대폰"), asDes5.GetAt(i), i);
					pDstDM->SetCellData(_T("주소_자택"), asDes6.GetAt(i), i);
					pDstDM->SetCellData(_T("주소_근무지"), asDes7.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
	}

	// 화면 작업
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("SearchLoanData") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	if( pDstDM->GetRowCount() > 0 )	AfxMessageBox( _T("자료가 검색되었습니다.") );
	else							AfxMessageBox( _T("자료가 존재하지 않습니다.") );


	return 0;

	EFS_END
	return -1;

}

// 대출자료관리 WHERE절 생성
// nMode = 0 -> 단행
//         1 -> 긴급대출
//         2 -> 연속
/*
INT CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )
{
	EFS_BEGIN

	INT ids;

	sWhere.Empty();
	
	CString sCMAlias = _T("CM_BO_LOC_3210");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );

	// 자료 구분이 맞을 때만 SQL문을 생성
	CString sCMode[3] = { _T("단행") , _T("긴급") , _T("연속") };

	CString sMode;
	ids = pCM->GetControlMgrData( _T("자료구분") , sMode , -1 , 0 );

	if( sMode.Compare(_T("전체"))!=0 )
	{
		if( sMode.Compare( sCMode[nMode] ) != 0 )
			return 1;
	}

	// 검색 윈도우의 모드가 _T("상세검색") 모드일 경우만 검색 조건에 추가
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );

	// 2005.11.11 UPD BY PDW(jr) /////////////////////////////////////////////////////
	// 국방대 검색 조건 추가 
	// 수입구분과 배가일자를 검색조건에서 추가, 공공은 값을 입력받지 않는다. 
	// 국방대 교번 추가
	const INT nCtrlCnt = 45;
	CString sCtrlAlias[ nCtrlCnt ] =
	{
		_T("대출자료상태_전체")		, _T("대출자료상태_대출")		, _T("대출자료상태_반납")		, _T("대출자료상태_연체")	, _T("대출자료상태_반납연기")	,
		_T("대출자료상태_예약")		, _T("대출자료상태_예약취소")	, _T("대출자료상태_무인반납중")	, _T("대출형태")			, _T("관리구분")				, 
		_T("자료실명")				, _T("대출일1")					, _T("대출일2")					, _T("반납예정일1")			, _T("반납예정일2")				,
		_T("반납일1")				, _T("반납일2")					, _T("예약일1")					, _T("예약일2")				, _T("예약만기일1")				,
		_T("예약만기일2")			, _T("대출소속정보")			, _T("대출직급정보")			, _T("대출자번호1")			, _T("대출자번호2")				,
		_T("대출자1")				, _T("대출자2")					, _T("본표제")					, _T("저작자")				, _T("별치기호1")				,
		_T("별치기호2")				, _T("분류기호1")				, _T("분류기호2")				, _T("등록번호구분")		, _T("등록번호1")				,
		_T("등록번호2")				, _T("핸드폰")					, _T("대출자료상태_예약완료")   , _T("수입구분")            , _T("배가일1")                 ,
		_T("배가일2")               , _T("변경일1")                 , _T("변경일2")                 , _T("교번1")				, _T("교번2")
	};
	INT nBoFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC			, ETC			, ETC		, 
		ETC			, ETC		, ETC			, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO		, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE		, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING		, TWOSTRING		, ADDINFO	,
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, TWOSTRING	,
		ADDINFO		, TWOSTRING	, ADDINFO		, THREESTRING	, ADDINFO	,
		ADDINFO		, ADDINFO	, ETC           , ONESTRING     , TWODATE   ,
		ADDINFO     , TWODATE   , ADDINFO       , ADDINFO		, ADDINFO		
	};
	INT nNonDBFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC		, ETC			, ETC		, 
		ETC			, ETC		, ETC		, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO	, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE	, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING	, TWOSTRING		, ADDINFO	,
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, TWOSTRING	,
		ADDINFO		, ETC		, ETC		, THREESTRING	, ADDINFO	,
		ADDINFO		, ADDINFO	, ETC       , ETC           , ETC       ,
		ETC         , ETC       , ETC       , ADDINFO		, ADDINFO		
	};

	TCHAR *sBo_ECOSERARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , _T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") ,
		_T("CL.RETURN_DATE") , _T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") ,
		_T("CL.RESERVATION_EXPIRE_DATE") , _T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") ,
		_T("CLU.NAME") , _T("CLU.NAME") , _T("I.BO_IDX_TITLE") , _T("I.BO_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") ,
		_T("B.SEPARATE_SHELF_CODE") , _T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") ,
		_T("B.REG_NO"), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")		
	};

	TCHAR *sBo_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	TCHAR *sSe_ECOSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	TCHAR *sSe_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc")
	};

	
	TCHAR *sNonDBDBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("CL.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.NAME") , 
		_T("CLU.NAME") , _T("NB.TITLE") , _T("NB.AUTHOR") , _T("NB.SEPARATE_SHELF_CODE") , _T("NB.SEPARATE_SHELF_CODE") , 
		_T("etc") , _T("etc") , _T("NB.REG_NO") , _T("NB.REG_NO") , _T("NB.REG_NO"), _T(""), _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,_T("CLU.CLASS_NO") , _T("CLU.CLASS_NO")
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR **pDBFieldName;
	pDBFieldName = NULL;
	INT *pnDBFieldType;
	pnDBFieldType = NULL;

	if( nMode == 0 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sBo_ECOSERARCH_DBFieldName;
		else
			pDBFieldName = sBo_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}
	else if( nMode == 1 )
	{
		pDBFieldName = sNonDBDBFieldName;
		pnDBFieldType = nNonDBFieldType;
	}
	else if( nMode == 2 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sSe_ECOSEARCH_DBFieldName;
		else
			pDBFieldName = sSe_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}


	CString sGetData[ nCtrlCnt ];

	for( INT i = 0 ; i < nCtrlCnt ; i++ )
	{
		if( pnDBFieldType[i] == ETC ) continue;
		ids = pCM->GetControlMgrData( sCtrlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	}

	// 대출 형태
	if( sGetData[8].Compare(_T("적용안함")) == 0 )
		sGetData[8] = _T("");
	else if( sGetData[8].Compare(_T("일반")) == 0 )
		sGetData[8] = _T("0");
	else if( sGetData[8].Compare(_T("특별")) == 0 )
		sGetData[8] = _T("1");
	else if( sGetData[8].Compare(_T("관내")) == 0 )
		sGetData[8] = _T("2");
	else if( sGetData[8].Compare(_T("무인")) == 0 )
		sGetData[8] = _T("3");
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++
	// 장기 상태 추가
	else if( sGetData[8].Compare(_T("장기")) == 0 )
		sGetData[8] = _T("4");
	// ______________________________________________

	INT nCnt;
	if( sModeAlias.Compare( _T("상세검색") ) == 0 ) 
		nCnt = 27;
	else 
		nCnt = nCtrlCnt;
	ids = MakeWhereSql( nCnt , pnDBFieldType , pDBFieldName , sGetData , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );


	// New Ecosearch 적용
	// 테스트를 위해 임시 적용
	// 27 , 28  
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("r"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	CString sEcoSearchWhere;
	INT nEcoSearchCnt=0;
	if( nCnt > 27 && ( nMode == 0 || nMode == 2 ) && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		if( m_pParentMgr->m_pInfo->MODE == 10000 || ( m_pParentMgr->m_pInfo->MODE == 30000 ) )
		{
		}
		else
		{
			CMakeSearchData MakeSearchData(m_pParentMgr);	
			
			// 2005.02.19 ADD BY PDJ 
			// 초기화 필요
			// 값이 깨져서 TRUE가 되었음.
			BOOL bIsEqualSearch = FALSE;

			/// 서명 검색 , 발행자 검색
			// 평상시는 우절단 검색 생성
			if( !sGetData[27].IsEmpty() )
			{
				if( sGetData[27].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[27]);
				if( !bIsEqualSearch ) 
					sGetData[27] += _T("*");
				
				sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[27] + _T("') ");
				nEcoSearchCnt++;
			}
			
			if( !sGetData[28].IsEmpty() )
			{
				if( sGetData[28].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[28]);
				if( !bIsEqualSearch ) 
					sGetData[28] += _T("*");
				
				if( !sEcoSearchWhere.IsEmpty() )
					sEcoSearchWhere += _T(" INTERSECT ");
				
				sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				nEcoSearchCnt++;
			}
			
			if( !sEcoSearchWhere.IsEmpty() )
			{
				if( !sWhere.IsEmpty() )
					sWhere += _T(" AND ");
				if( nEcoSearchCnt == 1 )
					sWhere += _T("I.REC_KEY IN ") + sEcoSearchWhere ;
				else
					sWhere += _T("I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
			}
		}
	}
	
	
	// 김용배 추가
	// 핸드폰 번호 검색추가
	// IDX_HANDPHONE

	sGetData[ 36 ].TrimLeft();
	sGetData[ 36 ].TrimRight();
	sGetData[ 36 ].Replace( _T(" "), _T("") );
	sGetData[ 36 ].Replace( _T("-"), _T("") );
	if( !sGetData[ 36 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += ( _T("CLU.IDX_HANDPHONE like '%") + sGetData[ 36 ] + _T("%'") );
	}

	/// 20040223 조병걸 수정
	/// 집 우편번호 , 주소 추가
	CString sZipCode , sAddress;
	ids = pCM->GetControlMgrData( _T("우편번호") , sZipCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sZipCode.TrimLeft(); sZipCode.TrimRight();

	ids = pCM->GetControlMgrData( _T("주소") , sAddress , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sAddress.TrimLeft(); sAddress.TrimRight();

	if( !sZipCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += _T("CLU.H_ZIPCODE LIKE ('")+sZipCode+_T("%')");
	}

	if( !sAddress.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");
		
		sWhere += _T("CLU.H_ADDR1 LIKE ('%")+sAddress+_T("%')");
	}
	
	// 2005.11.11 ADD BY PDW(Jr) /////////////////////////////////////////////
	// 교번 추가
	sGetData[ 43 ].TrimLeft();sGetData[ 43 ].TrimRight();
	if( !sGetData[ 43 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sGetData[ 44 ].TrimLeft();sGetData[ 44 ].TrimRight();
		if( !sGetData[ 44 ].IsEmpty() ) 
			sClass = _T("CLU.CLASS_NO BETWEEN '") + sGetData[ 43 ] + _T("' AND '") + sGetData[ 44 ] + _T("'");
		else
			sClass = _T("CLU.CLASS_NO = '") + sGetData[ 43 ] + _T("'");
		
		sWhere += sClass;
	}

	// 체크 박스 설정
	INT nCheck;
	CESL_Control_Element *pCheckCE;

	CString sWhere2;

	// 무인 반납기 체크
	CString sStation,sDoor;
//	pCM->GetControlMgrData( _T("무인반납기") , sStation , -1 , 0 );
//	pCM->GetControlMgrData( _T("무인반납기") , sDoor , -1 , 1 );
	
	CComboBox* pCombo = (CComboBox*)(pCM->FindControl(_T("무인반납기")));
	INT nSSIdx = pCombo->GetCurSel();

	if( nSSIdx != 0 )
	{
		CESL_DataMgr* pStationList = m_pParentMgr->FindDM(_T("DM_무인반납기목록"));
		// 검색은 PC_NAME으로 해야한다.
		pStationList->GetCellData( _T("스테이션") , nSSIdx-1 , sStation );
		pStationList->GetCellData( _T("도어이름") , nSSIdx-1 , sDoor );
	}

	sStation.TrimLeft(); sStation.TrimRight();
	sDoor.TrimLeft(); sDoor.TrimRight();
	
	if( !sStation.IsEmpty() )
	{
		if( !sWhere2.IsEmpty() ) sWhere2 += _T(" AND ");
		if( !sDoor.IsEmpty() )
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' AND  CL.SELF_RETURN_DOOR = '")+sDoor+_T("' ");
		else
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}
	
	sWhere2.Empty();

	for( i = 0 ; i < 9 ; i++ )
	{
		if ( (m_pLoan->m_bVolReserve) && (9==(i+1)) )		i = 37;
		else if ( !(m_pLoan->m_bVolReserve) && (9==(i+1)) )	continue;

		pCheckCE= pCM->FindControlElement( sCtrlAlias[i] );
		if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck != 1 ) continue;
		
		// 전체 체크
		if( i == 0 )	return 0;
		// 대출 체크
		else if( i == 1 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '0')");
		}
		// 반납 체크
		else if( i == 2 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '1')");
		}
		// 연체 체크
		else if( i == 3 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");

			if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==30000) )	
				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(now(),'YYYY/MM/DD') ) ");
			else
				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
		}
		// 반납 연기 체크
		else if( i == 4 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '2')");
		}
		// 예약 체크 or 예약 체크 미지정
		else if( i == 5 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			if ( m_pLoan->m_bVolReserve )
				sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NULL)");
			else
				sWhere2 += (_T(" (CL.STATUS = '3')"));
		}
		// 예약 취소 체크
		else if( i == 6 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '4')");
		}	
		// 무인반납중 체크
		else if( i == 7 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '5')");
		}
		// 예약 체크 완료 체크
		else if ( 37==i )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NOT NULL)");
		}
	}
		
	if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");

	
	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	//if ( m_pLoan->m_bVolReserve )
	//{
	//	if( !sWhere.IsEmpty() && !m_sTimeCondition.IsEmpty() )
	//		sWhere = sWhere + _T(" AND ") + m_sTimeCondition;
	//}

	return 0;

	EFS_END
	return -1;

}
*/
INT CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )
{
	EFS_BEGIN

	INT ids;

	sWhere.Empty();
	
	CString sCMAlias = _T("CM_BO_LOC_3210");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSqlBO_LOC_3210") );

	// 자료 구분이 맞을 때만 SQL문을 생성
	CString sCMode[3] = { _T("단행") , _T("긴급") , _T("연속") };

	CString sMode;
	ids = pCM->GetControlMgrData( _T("자료구분") , sMode , -1 , 0 );

	if( sMode.Compare(_T("전체"))!=0 )
	{
		if( sMode.Compare( sCMode[nMode] ) != 0 )
			return 1;
	}

	// 검색 윈도우의 모드가 _T("상세검색") 모드일 경우만 검색 조건에 추가
	CString sModeAlias;
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSqlBO_LOC_3210") );
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );

	// 2010.11.29 ADD BY KSJ : 연체일 추가
	// 2007.05.25 UPDATE BY PDW(JR)
	// 예약취소일 추가
	const INT nCtrlCnt = 50;
	CString sCtrlAlias[ nCtrlCnt ] =
	{
		_T("대출자료상태_전체")		, _T("대출자료상태_대출")		, _T("대출자료상태_반납")		, _T("대출자료상태_연체")	, _T("대출자료상태_반납연기")	, // 4
		_T("대출자료상태_예약")		, _T("대출자료상태_예약취소")	, _T("대출자료상태_무인반납중")	, _T("대출형태")			, _T("관리구분")				, // 9
		_T("자료실명")				, _T("대출일1")					, _T("대출일2")					, _T("반납예정일1")			, _T("반납예정일2")				, // 14
		_T("반납일1")				, _T("반납일2")					, _T("예약일1")					, _T("예약일2")				, _T("예약만기일1")				, // 19
		_T("예약만기일2")			, _T("대출소속정보")			, _T("대출직급정보")			, _T("대출자번호1")			, _T("대출자번호2")				, // 24
		_T("대출자1")				, _T("대출자2")					, _T("본표제")					, _T("저작자")				, _T("별치기호1")				, // 29
		_T("별치기호2")				, _T("분류기호1")				, _T("분류기호2")				, _T("등록번호구분")		, _T("등록번호1")				, // 34
		_T("등록번호2")				, _T("핸드폰")					, _T("대출자료상태_예약완료")   , _T("수입구분")            , _T("배가일1")                 , // 39
		_T("배가일2")               , _T("변경일1")                 , _T("변경일2")                 , _T("교번1")				, _T("교번2")                   , // 44
		_T("군별")                  , _T("예약취소일1")             , _T("예약취소일2")				, _T("연체일1")				, _T("연체일2")					// 49
	};
	INT nBoFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC			, ETC			, ETC		, 
		ETC			, ETC		, ETC			, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO		, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE		, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING		, TWOSTRING		, ADDINFO	,
		// 17/01/18 김혜영 : 공통>대출/반납>대출자료관리의 별치기호 조건 검색시 오류
/*//BEFORE-----------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, TWOSTRING	,
*///AFTER------------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONEINDEXSEARCH, ONEINDEXSEARCH, ONESTRING	, 
/*//END-------------------------------------------------------------------------------*/
		ADDINFO		, TWOSTRING	, ADDINFO		, ADDINFO	    , ADDINFO   ,
		ADDINFO		, ADDINFO	, ETC           , ONESTRING     , TWODATE   ,
		ADDINFO     , TWODATE   , ADDINFO       , ADDINFO		, ADDINFO	,
		ADDINFO     , ADDINFO   , ADDINFO		, ADDINFO		, ADDINFO		
	};
	INT nNonDBFieldType[ nCtrlCnt ] = 
	{
		ETC			, ETC		, ETC		, ETC			, ETC		, 
		ETC			, ETC		, ETC		, ONESTRING		, ONESTRING	, 
		ONESTRING	, TWODATE	, ADDINFO	, TWODATE		, ADDINFO	,
		TWODATE		, ADDINFO	, TWODATE	, ADDINFO		, TWODATE	,
		ADDINFO		, ONESTRING	, ONESTRING	, TWOSTRING		, ADDINFO	,
		// 17/01/18 김혜영 : 공통>대출/반납>대출자료관리의 별치기호 조건 검색시 오류
/*//BEFORE-----------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, TWOSTRING	,
*///AFTER------------------------------------------------------------------------------
		TWOSTRING	, ADDINFO	, ONESTRING	, ONESTRING		, ONESTRING	, 
/*//END-------------------------------------------------------------------------------*/
		ADDINFO		, ETC		, ETC		, ADDINFO       , ADDINFO  	,
		ADDINFO		, ADDINFO	, ETC       , ETC           , ETC       ,
		ETC         , ETC       , ETC       , ADDINFO		, ADDINFO	,
		ADDINFO     , ADDINFO   , ADDINFO	, ADDINFO		, ADDINFO
	};

	// 2006-03-02 UPDATE BY PDW(Jr) =============================================================
	// 대출자명 검색 조건 NAME >> IDX_USER_NAME 변경
	// 자료실 CL.SHELF_LOC_CODE >> B.SHELF_LOC_CODE 변경

	TCHAR *sBo_ECOSERARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , _T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") ,
		_T("CL.RETURN_DATE") , _T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") ,
		_T("CL.RESERVATION_EXPIRE_DATE") , _T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") ,
		_T("CLU.IDX_USER_NAME") , _T("CLU.IDX_USER_NAME") , _T("I.BO_IDX_TITLE") , _T("I.BO_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") ,
		_T("B.SEPARATE_SHELF_CODE") , _T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") ,
		_T("B.REG_NO"),_T("") , _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,
		_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sBo_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sSe_ECOSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.SE_IDX_TITLE") , _T("I.SE_IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") ,_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR *sSe_CATSEARCH_DBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("B.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("I.IDX_TITLE") , _T("I.IDX_AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("I.CLASS_NO") , _T("I.CLASS_NO") , _T("B.REG_CODE") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("B.ACQ_CODE") , _T("B.SHELF_DATE") , _T("B.SHELF_DATE") , 
		_T("B.SHELF_CHANGE_DATE") , _T("B.SHELF_CHANGE_DATE") , _T("etc") , _T("etc") , _T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	
	TCHAR *sNonDBDBFieldName[ nCtrlCnt ] =
	{
		_T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,
		_T("etc") , _T("etc") , _T("etc") , _T("CL.LOAN_TYPE_CODE") , _T("B.MANAGE_CODE") , 
		_T("CL.SHELF_LOC_CODE") , 
		_T("CL.LOAN_DATE") , _T("CL.LOAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_PLAN_DATE") , _T("CL.RETURN_DATE") , 
		_T("CL.RETURN_DATE") ,_T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , _T("CL.RESERVATION_EXPIRE_DATE") , 
		_T("CLU.USER_POSITION_CODE") ,_T("CLU.USER_CLASS_CODE") , _T("CLU.USER_NO") , _T("CLU.USER_NO") , _T("CLU.IDX_USER_NAME") , 
		_T("CLU.IDX_USER_NAME") , _T("B.TITLE") , _T("B.AUTHOR") , _T("B.SEPARATE_SHELF_CODE") , _T("B.SEPARATE_SHELF_CODE") , 
		_T("etc") , _T("etc") , _T("B.REG_NO") , _T("B.REG_NO") , _T("B.REG_NO"), _T(""), _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") , _T("etc") ,_T("CLU.CLASS_NO") , _T("CLU.CLASS_NO") ,
		_T("CLU.USER_DEFINE_CODE1"), _T(""), _T(""), _T(""), _T("")
	};

	TCHAR **pDBFieldName;
	pDBFieldName = NULL;
	INT *pnDBFieldType;
	pnDBFieldType = NULL;

	if( nMode == 0 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sBo_ECOSERARCH_DBFieldName;
		else
			pDBFieldName = sBo_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}
	else if( nMode == 1 )
	{
		pDBFieldName = sNonDBDBFieldName;
		pnDBFieldType = nNonDBFieldType;
	}
	else if( nMode == 2 )
	{
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == 1 )
			pDBFieldName = sSe_ECOSEARCH_DBFieldName;
		else
			pDBFieldName = sSe_CATSEARCH_DBFieldName;
		pnDBFieldType = nBoFieldType;
	}


	CString sGetData[ nCtrlCnt ];

	for( INT i = 0 ; i < nCtrlCnt ; i++ )
	{
		if( pnDBFieldType[i] == ETC ) continue;

		// 2006-01-12 ADD BY PDW(Jr) ======================================================
		// 국방대 조건 삭제
		if ( m_sDefenceFlag != _T("Y") && ( i == 38 || i == 45 ) ) continue;
		// ================================================================================

		ids = pCM->GetControlMgrData( sCtrlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetSqlBO_LOC_3210") );
	}

	// 대출 형태
	if( sGetData[8].Compare(_T("적용안함")) == 0 )
		sGetData[8] = _T("");
	else if( sGetData[8].Compare(_T("일반")) == 0 )
		sGetData[8] = _T("0");
	else if( sGetData[8].Compare(_T("특별")) == 0 )
		sGetData[8] = _T("1");
	else if( sGetData[8].Compare(_T("관내")) == 0 )
		sGetData[8] = _T("2");
	else if( sGetData[8].Compare(_T("무인")) == 0 )
		sGetData[8] = _T("3");
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++
	// 장기 상태 추가
	else if( sGetData[8].Compare(_T("장기")) == 0 )
		sGetData[8] = _T("4");
	// ______________________________________________

	INT nCnt;
	if( sModeAlias.Compare( _T("상세검색") ) == 0 ) 
	{
		nCnt = 27;
	}
	else 
	{
		nCnt = nCtrlCnt;
	}
	ids = MakeWhereSql( nCnt , pnDBFieldType , pDBFieldName , sGetData , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlBO_LOC_3210") );

	///{{++
	/// 2008.03.03 add by pdw
	/// 등록번호 방식을 변경하였다
	if( 0 != sModeAlias.Compare(_T("상세검색")) )
	{
		/// 등록구분 선택은 필수
		if( !sGetData[33].IsEmpty() )
		{
			CString strRegNoAlias;

//DEL 			if( 0 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("B.REG_NO");
//DEL 			}
//DEL 			else if( 1 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("NB.REG_NO");
//DEL 			}
//DEL 			else if( 2 == nMode )
//DEL 			{
//DEL 				strRegNoAlias = _T("B.REG_NO");
//DEL 			}
			strRegNoAlias = _T("B.REG_NO");

			CESL_Control_Element* pElement = pCM->FindControlElement(_T("등록번호모드"));
			if( NULL == pElement ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetSqlBO_LOC_3210") );
			CButton* pCheck = (CButton*)m_pParentMgr->GetDlgItem(pElement->CTRL_ID);
			if( NULL == pCheck ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetSqlBO_LOC_3210") );
			INT nCheck = pCheck->GetCheck();
			/// 범위 검색이라면
			if( 0 == nCheck )
			{					
				if( !sGetData[34].IsEmpty() )
				{
					if( sGetData[35].IsEmpty() )
					{
						sGetData[35] = sGetData[34];
					}

					CString strRegNo1, strRegNo2;

					strRegNo1 = sGetData[33] + sGetData[34];
					strRegNo2 = sGetData[33] + sGetData[35];
						
					CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo1 );
					CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo2 );

					if( sWhere.GetLength() > 1 )
						sWhere += _T(" AND ");

					sWhere += ( strRegNoAlias + _T(" BETWEEN '") + strRegNo1 + _T("' AND '") + strRegNo2 + _T("'") );
				}
			}
			/// 구분 검색이라면
			/// 입력값은 ","로 구분되어 있다
			else if( 1 == nCheck )
			{
				CString strData;
				ids = pCM->GetControlMgrData( _T("등록번호확장"), strData, -1, 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("GetSqlBO_LOC_3210") );
				
				if( !strData.IsEmpty() )
				{
					CStringArray pArray;
					CLocCommonAPI::MakeArrayList(strData, _T(","), pArray);

					CString strRegNo;
					CString strRegNoWhere;
					INT     nArrayCnt = pArray.GetSize();					

					for( i=0; i<nArrayCnt; i++ )
					{
						strRegNo = sGetData[33] + pArray.GetAt(i);
						CLocCommonAPI::MakeRegNoSpaceToZero( strRegNo );

						if( !strRegNoWhere.IsEmpty() )
						{
							strRegNoWhere += _T(",");
						}

						strRegNoWhere += ( _T("'") + strRegNo + _T("'") );
					}
					
					if( !strRegNoWhere.IsEmpty() )
					{
						if( sWhere.GetLength() > 1 )
							sWhere += _T(" AND ");

						sWhere += ( strRegNoAlias + _T(" IN ( ") + strRegNoWhere + _T(" )") );
					}
				}
			}	
		}
	}
	///}}--

	// 2007.05.25 add by pdw(jr) {{++++++++++++++++++++++++++++++++++++++++++++++++
	// 예약취소일 추가
	sGetData[46].TrimLeft();
	sGetData[46].TrimRight();
	if( !sGetData[46].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sGetData[47].TrimLeft();
		sGetData[47].TrimRight();
		if( !sGetData[47].IsEmpty() )
		{
			//CString(_T(" BETWEEN to_date('")) + sGetData[i] + CString(_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")) + sGetData[i+1] +_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')");
			sWhere += ( _T("CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('") + sGetData[46] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[47] + _T("23:59:59','YYYY/MM/DD HH24:MI:SS')") );
		}
		else
		{
			sWhere += ( _T("CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('") + sGetData[46] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[46] + _T("23:59:59','YYYY/MM/DD HH24:MI:SS')") );
		}
	}
	// --------------------------------------------------------------------------}}

	// 2010.11.29 ADD BY KSJ : 연체일 추가
	sGetData[48].TrimLeft();	sGetData[48].TrimRight();
	sGetData[49].TrimLeft();	sGetData[49].TrimRight();
	if(sGetData[48].IsEmpty() && !sGetData[49].IsEmpty())
	{
		sGetData[48] = _T("1");
	}
	if(!sGetData[48].IsEmpty())
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString strTemp;
		// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------		
		strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[48]);
		sWhere += strTemp;

		if(!sGetData[49].IsEmpty())
		{
			strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[49]);
			sWhere += strTemp;
		}
*///AFTER------------------------------------------------------------------------------
		CString strValue = _T("");
		m_pLoan->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체기간휴관일포함유무"), strValue);
		if( !strValue.IsEmpty() && _T("Y") != strValue)		
		{
			strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD')-GET_HOLIDAYCOUNT(TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'), TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD'), '%s'))"),sGetData[48], m_pParentMgr->m_pInfo->MANAGE_CODE);
			sWhere += strTemp;

			if(!sGetData[49].IsEmpty())
			{
				strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD')-GET_HOLIDAYCOUNT(TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'), TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD'), '%s'))"),sGetData[49],m_pParentMgr->m_pInfo->MANAGE_CODE);
				sWhere += strTemp;
			}		
		}
		else
		{
			strTemp.Format(_T(" %s <= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[48]);
			sWhere += strTemp;

			if(!sGetData[49].IsEmpty())
			{
				strTemp.Format(_T(" AND %s >= (TO_DATE(NVL(CL.RETURN_DATE, SYSDATE),'YYYY/MM/DD')-TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD'))"),sGetData[49]);
				sWhere += strTemp;
			}
		}
/*//END-------------------------------------------------------------------------------*/
		
	}

	// New Ecosearch 적용
	// 테스트를 위해 임시 적용
	// 27 , 28  
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
	BOOL bNewEcoSearch = FALSE;
	if( fd == NULL )
		bNewEcoSearch = TRUE;
	else
		fclose(fd);
	
	CString sEcoSearchWhere;
	INT nEcoSearchCnt=0;
	if( nCnt > 27 && ( nMode == 0 || nMode == 2 ) && bNewEcoSearch && 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
	{
		//if( m_pParentMgr->m_pInfo->MODE == 10000 || ( m_pParentMgr->m_pInfo->MODE == 30000 ) )
		//{
		//}
		//else
		//{
			CMakeSearchData MakeSearchData(m_pParentMgr);	
			
			// 2005.02.19 ADD BY PDJ 
			// 초기화 필요
			// 값이 깨져서 TRUE가 되었음.
			BOOL bIsEqualSearch = FALSE;

			/// 서명 검색 , 발행자 검색
			// 평상시는 우절단 검색 생성
			if( !sGetData[27].IsEmpty() )
			{
				if( sGetData[27].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[27]);
				if( !bIsEqualSearch ) 
					sGetData[27] += _T("*");
				
				// 2007.07.12 update pdw
				// 연속은 색인이 다르다.
				if( nMode == 2 )
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sGetData[27] + _T("') ");
				}
				else
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[27] + _T("') ");
				}
				nEcoSearchCnt++;
			}
			
			if( !sGetData[28].IsEmpty() )
			{
				if( sGetData[28].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sGetData[28]);
				if( !bIsEqualSearch ) 
					sGetData[28] += _T("*");
				
				if( !sEcoSearchWhere.IsEmpty() )
					sEcoSearchWhere += _T(" INTERSECT ");
				
				// 2007.07.12 update pdw
				// 연속은 색인이 다르다.
				if( nMode == 2 )
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				}
				else
				{
					sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[28] + _T("') ");
				}
				nEcoSearchCnt++;
			}
			
			if( !sEcoSearchWhere.IsEmpty() )
			{
				if( !sWhere.IsEmpty() )
					sWhere += _T(" AND ");
				if( nEcoSearchCnt == 1 )
					sWhere += _T("I.REC_KEY IN ") + sEcoSearchWhere ;
				else
					sWhere += _T("I.REC_KEY IN ( ") + sEcoSearchWhere +_T(")");
			}
		//}
	}
	
	
	// 김용배 추가
	// 핸드폰 번호 검색추가
	// IDX_HANDPHONE

	sGetData[ 36 ].TrimLeft();
	sGetData[ 36 ].TrimRight();
	sGetData[ 36 ].Replace( _T(" "), _T("") );
	sGetData[ 36 ].Replace( _T("-"), _T("") );
	if( !sGetData[ 36 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += ( _T("CLU.IDX_HANDPHONE like '%") + sGetData[ 36 ] + _T("%'") );
	}

	/// 20040223 조병걸 수정
	/// 집 우편번호 , 주소 추가
	CString sZipCode , sAddress;
	ids = pCM->GetControlMgrData( _T("우편번호") , sZipCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sZipCode.TrimLeft(); sZipCode.TrimRight();

	ids = pCM->GetControlMgrData( _T("주소") , sAddress , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -201 , _T("GetSqlBO_LOC_3210") );
	sAddress.TrimLeft(); sAddress.TrimRight();

	if( !sZipCode.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		sWhere += _T("CLU.H_ZIPCODE LIKE ('")+sZipCode+_T("%')");
	}

	if( !sAddress.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");
		
		sWhere += _T("CLU.H_ADDR1 LIKE ('%")+sAddress+_T("%')");
	}
	
	// 2005.11.11 ADD BY PDW(Jr) /////////////////////////////////////////////
	// 교번 추가
	sGetData[ 43 ].TrimLeft();sGetData[ 43 ].TrimRight();
	if( !sGetData[ 43 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sGetData[ 44 ].TrimLeft();sGetData[ 44 ].TrimRight();
		if( !sGetData[ 44 ].IsEmpty() ) 
			sClass = _T("CLU.CLASS_NO BETWEEN '") + sGetData[ 43 ] + _T("' AND '") + sGetData[ 44 ] + _T("'");
		else
			sClass = _T("CLU.CLASS_NO = '") + sGetData[ 43 ] + _T("'");
		
		sWhere += sClass;
	}
	// 군별 추가 ////////////////////////////////////////////////
	sGetData[ 45 ].TrimLeft();sGetData[ 45 ].TrimRight();
	if( !sGetData[ 45 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		CString sClass = _T("");
		sClass = _T("CLU.USER_DEFINE_CODE1 = '") + sGetData[ 45 ] + _T("'");
		
		sWhere += sClass;
	}

	// 체크 박스 설정
	INT nCheck;
	CESL_Control_Element *pCheckCE;

	CString sWhere2;

	// 무인 반납기 체크
	CString sStation,sDoor;
//	pCM->GetControlMgrData( _T("무인반납기") , sStation , -1 , 0 );
//	pCM->GetControlMgrData( _T("무인반납기") , sDoor , -1 , 1 );
	
	CComboBox* pCombo = (CComboBox*)(pCM->FindControl(_T("무인반납기")));
	INT nSSIdx = pCombo->GetCurSel();

	if( nSSIdx != 0 )
	{
		CESL_DataMgr* pStationList = m_pParentMgr->FindDM(_T("DM_무인반납기목록"));
		// 검색은 PC_NAME으로 해야한다.
		pStationList->GetCellData( _T("스테이션") , nSSIdx-1 , sStation );
		pStationList->GetCellData( _T("도어이름") , nSSIdx-1 , sDoor );
	}

	sStation.TrimLeft(); sStation.TrimRight();
	sDoor.TrimLeft(); sDoor.TrimRight();
	
	if( !sStation.IsEmpty() )
	{
		if( !sWhere2.IsEmpty() ) sWhere2 += _T(" AND ");
		if( !sDoor.IsEmpty() )
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' AND  CL.SELF_RETURN_DOOR = '")+sDoor+_T("' ");
		else
			sWhere2 += _T(" CL.SELF_RETURN_STATION = '")+sStation+_T("' ");
	}

	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}
	
	sWhere2.Empty();

	for( i = 0 ; i < 9 ; i++ )
	{
		if ( (m_pLoan->m_bVolReserve) && (9==(i+1)) )		i = 37;
		else if ( !(m_pLoan->m_bVolReserve) && (9==(i+1)) )	continue;

		pCheckCE= pCM->FindControlElement( sCtrlAlias[i] );
		if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetSqlBO_LOC_3210") );
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck != 1 ) continue;
		
		// 전체 체크
		if( i == 0 )	return 0;
		// 대출 체크
		else if( i == 1 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS IN ('0', 'L'))");
//DEL 			sWhere2 += _T(" (CL.STATUS = '0')");
		}
		// 반납 체크
		else if( i == 2 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS IN ('1', 'R'))");
//DEL 			sWhere2 += _T(" (CL.STATUS = '1')");
		}
		// 연체 체크
		else if( i == 3 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");

			//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 			if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==30000) )	
//REMOVE▶ 				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(now(),'YYYY/MM/DD') ) ");
			//--2008.09.02 DEL BY LKS --}}
//			else

			// 18.09.27 JMJ KOL.RED.2018.008
			//연체중인 자료 검색 시 [책두레 반납연기] 상태 추가
			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L','D') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') ) ");
			//sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L','D') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') ) ");
//			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE 속도개선
//			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') ) ");
// 			sWhere2 += _T(" ( CL.STATUS IN ('0','2', 'L') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
//DEL 				sWhere2 += _T(" ( CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < to_char(SYSDATE,'YYYY/MM/DD') ) ");
		}
		// 반납 연기 체크
		else if( i == 4 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere2 += _T(" (CL.STATUS IN ('2', 'D'))");
			//sWhere2 += _T(" (CL.STATUS = '2')");
		}
		// 예약 체크 or 예약 체크 미지정
		else if( i == 5 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			//++2008.06.04 UPDATE BY PWR {{++
			// 무인예약대출시스템 사용시 상태값 구분되도록 검색조건을 변경
			if( m_pLoan->m_sUnmannedReserveSys == _T("Y") )
			{
				if ( m_pLoan->m_bVolReserve )
					sWhere2 += _T(" (CL.STATUS='3' AND CL.REG_NO IS NULL AND CL.UNMANNED_RESERVATION_LOAN IS NULL)");
				else
					sWhere2 += _T(" (CL.STATUS='3' AND CL.UNMANNED_RESERVATION_LOAN IS NULL)");
			}
			else
			{
				if ( m_pLoan->m_bVolReserve )
					sWhere2 += _T(" (CL.STATUS='3' AND CL.REG_NO IS NULL)");
				else
					sWhere2 += _T(" (CL.STATUS='3')");
			}
			/*
			if ( m_pLoan->m_bVolReserve )
				sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NULL)");
			else
				sWhere2 += (_T(" (CL.STATUS = '3')"));
			*/
			//--2008.06.04 UPDATE BY PWR --}}
		}
		// 예약 취소 체크
		else if( i == 6 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '4')");
		}	
		// 무인반납중 체크
		else if( i == 7 )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '5')");
		}
		// 예약 체크 완료 체크
		else if ( 37==i )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '3' AND CL.REG_NO IS NOT NULL)");
		}
	}

	//++2008.06.04 UPDATE BY PWR {{++
	// 분관반납자료	
	pCheckCE = pCM->FindControlElement( _T("대출자료상태_분관반납중") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );

	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS='6')");
		}
	}
	// 야간대출예약
	pCheckCE = pCM->FindControlElement( _T("대출자료상태_야간대출예약") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );

	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 += _T(" CL.NIGHT_RESERVATION_LOAN IN ('Y','O')");
		}
	}	
	// 타관반납자료
	pCheckCE = pCM->FindControlElement( _T("대출자료상태_타관반납중") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS='7')");
		}
	}
	// 무인대출예약
	pCheckCE = pCM->FindControlElement( _T("대출자료상태_무인대출예약") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");		
			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN IN ('Y','O') AND CL.STATUS='3')");
			
			if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");
	
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += sWhere2;

			return 0;
		}
	}
	// 야간대출예약신청
	pCheckCE = pCM->FindControlElement( _T("야간대출상태_신청") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'Y'");		

			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += _T(" CL.STATUS='3'");
		}
	}
	// 야간대출예약접수
	pCheckCE = pCM->FindControlElement( _T("야간대출상태_접수") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
			sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'O'");		

			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			sWhere += _T(" CL.STATUS='3'");
		}
	}
	// 무인대출예약신청
	pCheckCE = pCM->FindControlElement( _T("무인대출상태_신청") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN = 'Y' AND CL.STATUS='3')");
		}
	}
	// 무인대출대기 ( 상태값이 'O'이면서 예약만기일이 경과되지 않은 자료 )
	pCheckCE = pCM->FindControlElement( _T("무인대출상태_대기") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE 속도개선
			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE >= TO_DATE(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");
// 			sWhere2 += _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE >= TO_CHAR(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");
		}
	}
	// 무인대출지연 ( 상태값이 'O'이면서 예약만기일이 경과된 자료 )
	pCheckCE = pCM->FindControlElement( _T("무인대출상태_지연") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("GetSqlBO_LOC_3210") );
	if( (CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )) )
	{
		nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
		if( nCheck )
		{
			if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			// 2009.12.15 UPDATE BY PWR : TO_CHAR -> TO_DATE 속도개선
			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");	
// 			sWhere2 = sWhere2 + _T(" (CL.UNMANNED_RESERVATION_LOAN='O' AND CL.RESERVATION_EXPIRE_DATE < TO_CHAR(SYSDATE,'YYYY/MM/DD') AND CL.STATUS='3')");	
		}
	}
	/*
	pCheckCE= pCM->FindControlElement( _T("대출자료상태_야간대출예약") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
	if( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");		
			sWhere2 += _T(" CL.NIGHT_RESERVATION_LOAN IN ('Y','O') ");
	}

	pCheckCE= pCM->FindControlElement( _T("대출자료상태_타관반납중") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();
	if( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" OR ");
			sWhere2 += _T(" (CL.STATUS = '6')");
	}

	BOOL bCheck2;
	CString sWhere3;
	pCheckCE= pCM->FindControlElement( _T("야간대출상태_신청") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	nCheck = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();

	pCheckCE= pCM->FindControlElement( _T("야간대출상태_접수") );
	if( pCheckCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::GetSqlBO_LOC_3210( CString &sWhere , INT nMode )") );
	bCheck2 = ((CButton*)(m_pParentMgr->GetDlgItem( pCheckCE->CTRL_ID )))->GetCheck();

	if ( nCheck )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");		
		sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'Y' ");		

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" CL.STATUS='3' ");

	}
    else if ( bCheck2 )
	{
		if( sWhere2.GetLength() > 1 ) sWhere2 += _T(" AND ");
		sWhere2 = sWhere2 + _T(" CL.NIGHT_RESERVATION_LOAN = 'O' ");		

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" CL.STATUS='3' ");
	}	
	*/
	//--2008.06.04 UPDATE BY PWR --}}
	
	if( sWhere2.GetLength() > 1 ) sWhere2 = _T("( ") + sWhere2 + _T(") ");

	
	if( !sWhere2.IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += sWhere2;
	}

	//if ( m_pLoan->m_bVolReserve )
	//{
	//	if( !sWhere.IsEmpty() && !m_sTimeCondition.IsEmpty() )
	//		sWhere = sWhere + _T(" AND ") + m_sTimeCondition;
	//}

	return 0;

	EFS_END
	return -1;

}
// TOOL 
// WHERE절 생성
// [out]sWhere
// [in] nCnt -> 처리할 갯수
// [in] nFieldType -> TYPE
// [in] sDBFieldAlias -> DB Field Name 
// [in] sGetData -> 내용
INT CBL_LOC_LOAN_MNG_PROC::MakeWhereSql( INT nCnt , INT nFieldType[] , TCHAR **sDBFieldName , CString sGetData[] , CString &sWhere )
{
	EFS_BEGIN

	CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nFieldType , nCnt , sWhere ,m_pParentMgr);
/*
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();

		switch( nFieldType[i] )
		{
		case ONENUMERIC :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = ")) + sGetData[i] + CString(_T(" "));
			}
			break;
		case ONESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
			}
			break;
		case TWOSTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
				}
				else
				{
					sWhere += CString(_T("( ")) + sDBFieldName[i] + CString(_T(" BETWEEN '")) + sGetData[i] + CString(_T("' AND '")) + sGetData[i+1] + CString(_T("' ) "));
				}
				i++;
			}
			break;
		case ONEDATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += CString(_T(" to_char(")) + sDBFieldName[i]+ CString(_T(",'YYYY/MM/DD')")) + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
			}
			break;
		case TWODATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += CString(_T(" to_char(")) +sDBFieldName[i] + CString(_T(",'YYYY/MM/DD') = '")) + sGetData[i] + CString(_T("' "));
				}
				else
				{
					sWhere += CString(_T("( to_char(")) + sDBFieldName[i] + CString(_T(",'YYYY/MM/DD') BETWEEN '")) + sGetData[i] + CString(_T("' AND '")) + sGetData[i+1] + CString(_T("' ) "));
				}
				i++;
			}
			break;
		case THREESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( !sGetData[i+1].IsEmpty() )
				{
					// 등록 번호인 경우에는 특별한 짓을 한다.
					CString sData1 = sGetData[i]+sGetData[i+1];
					CString sData2 = sGetData[i]+sGetData[i+2];
					if( CString(_T("B.REG_CODE")).Compare(sDBFieldName[i])==0 || (CString(_T("NB.REG_NO")).Compare(sDBFieldName[i])==0))
					{
						CLocCommonAPI::MakeRegNoSpaceToZero( sData1);
						if( !sGetData[i+2].IsEmpty() )
							CLocCommonAPI::MakeRegNoSpaceToZero( sData2);
					}
					if( sGetData[i+2].IsEmpty() )
					{
						sWhere += sDBFieldName[i+1] + CString(_T(" = '")) + sData1 + CString(_T("' "));
					}
					else
					{
						sWhere += CString(_T("( ")) + sDBFieldName[i+1] + CString(_T(" BETWEEN '")) + sData1 +CString(_T("' AND '")) + sData2 + _T("' ) ");
					}
				}
				else
				{
					if( CString(_T("B.REG_CODE")).Compare(sDBFieldName[i])==0 )
						sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
					else if( CString(_T("NB.REG_NO")).Compare(sDBFieldName[i])==0 )
						sWhere += sDBFieldName[i] + CString(_T(" LIKE ('")) + sGetData[i] + CString(_T("%') "));
					else
						sWhere += sDBFieldName[i] + CString(_T(" LIKE ('")) + sGetData[i] + CString(_T("%') "));

					
				}
				i = i+2;
			}
			break;
		case ONEINDEXSEARCH :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += CString(_T(" ( CATSEARCH("))+sDBFieldName[i] +CString(_T(",'")) + sGetData[i] + CString(_T("',NULL) > 0 ) "));
			}
			break;
		default:
			break;
		}
	}
*/
	return 0;

	EFS_END
	return -1;

}


// 그리드의 화면을 보여준다.
INT CBL_LOC_LOAN_MNG_PROC::GridDisplay( CString sCMAlias )
{
	EFS_BEGIN

	INT ids;

	// 회원구분 , 대출 상태 , 메일 발송여부 , 청구기호를 보여준다.
	// 일단은 단행만 한다. , 긴급대출만 한다.

/*
	const INT nDMCnt = 3;

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3200_BO") , _T("DM_BO_LOC_3200_NON_DB") , _T("DM_BO_LOC_3200_SE") 
	};

*/
	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	const INT nDMCnt = 1;
	
	const INT nRCnt = 8;
	CString sReserveAlias[nRCnt] =
	{
		_T("IBS_청구_도서기호"),		_T("IBS_청구_분류기호"), _T("IBS_청구_복본기호"), 
		_T("IBS_청구_별치기호_코드"),	_T("IBS_청구_권책기호"), _T("배가상태"), 
		_T("이전자료상태"),				_T("등록번호")			
	};

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3200")
	};

	CString sMemberClass;
	CString sLoanStatus;
	CString sMailSendDate;
	CString sWorkingStatus;
	CString sRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	// 2008.06.04 ADD BY PWR : 무인대출예약상태값
	// ("Y":무인대출신청, "O, 예약만기일경과안된":무인대출대기, "O, 예약만기일경과":무인대출지연)
	CString sUnmannedReserveStatus;

	CESL_DataMgr *pDM;

	// 2009.10.30 ADD BY KSJ : 대출자번호가 주민번호일 경우 처리
	CString strValue1;
	ids = m_pLoan->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	strValue1.TrimLeft();		strValue1.TrimRight();		strValue1.MakeUpper();

	// 2010.08.27 ADD BY PWR : 속도개선
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	CTime tCurrent(tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), tTime.GetHour(), 0, 0);

	INT nCnt = 1;
	INT nRowCount;
	for( INT j = 0 ; j < nCnt ; j++ )
	{
		pDM = m_pParentMgr->FindDM( sDMAlias[j] );
		nRowCount = pDM->GetRowCount();
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("GridDisplay") );

		for( INT i = 0 ; i < nRowCount ; i++ )
		{
			// 2009.10.30 ADD BY KSJ : 대출자번호가 주민번호일 경우 처리
			CString strUserNo , strBuf , strCivilNo;
			if( _T("Y") != strValue1 )
			{
				ids = pDM->GetCellData( _T("대출자번호") , i , strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );

				// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용 할 경우 표시방법 변경
//DEL 				strBuf = strUserNo;
//DEL 				if( 13 <= strUserNo.GetLength() )
//DEL 				{
//DEL 					strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 				}
//DEL 				
//DEL 				ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
//DEL 				if( 0 == ids )
//DEL 				{
//DEL 					//ids = CLocCommonAPI::ChangeSecuriyCivilNo( NULL, sUserNo );
//DEL 					strUserNo.Format( _T("%s%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
//DEL 					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridDisplay") );
//DEL 
//DEL 					ids = pDM->SetCellData( _T("대출자번호") , strUserNo , i );
//DEL 					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
//DEL 				}
				CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pLoan->m_sIsUnityLoanService );
				ids = pDM->SetCellData( _T("대출자번호") , strUserNo , i );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
			}

			// 2010.01.12 ADD BY PWR : 주민등록번호 *표시
			ids = pDM->GetCellData( _T("주민등록번호") , i , strCivilNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
			// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용 할 경우 표시방법 변경
//DEL 			strUserNo.Replace(_T("-"), _T(""));
//DEL 			strBuf = strUserNo;
//DEL 			if( 13 <= strUserNo.GetLength() )
//DEL 			{
//DEL 				strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 			}
//DEL 			ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
//DEL 			if( 0 == ids )
//DEL 			{
//DEL 				strUserNo.Format( _T("%s-%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridDisplay") );
//DEL 				ids = pDM->SetCellData( _T("주민등록번호") , strUserNo , i );
//DEL 				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );
//DEL 			}
			ids = CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strCivilNo , m_pLoan->m_sIsUnityLoanService );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridDisplay") );
			ids = pDM->SetCellData( _T("주민등록번호") , strCivilNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridDisplay") );

			// 회원구분
			ids = pDM->GetCellData( _T("구분") , i , sMemberClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("GridDisplay") );
			
			ids = ConvertMemberClassCodeToDesc( 0 , sMemberClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("GridDisplay") ); 

			ids = pDM->SetCellData( _T("구분") , sMemberClass , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("GridDisplay") ); 

			// 대출상태
			ids = pDM->GetCellData( _T("대출상태") , i , sLoanStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("GridDisplay") );
			
			// 2009.02.16 ADD BY PWR : 한줄서기 예약상태에 등록번호는 책의 등록번호라 의미없어 지정여부로 변경
			CString sReserveMark;
			ids = pDM->GetCellData( _T("지정여부"), i, sReserveMark );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("GridDisplay") );
			// 한줄서기 예약 미지정이면 필요없는내용 지우기
			if( (m_pLoan->m_bVolReserve) && (0==sLoanStatus.Compare(_T("3"))) )
			{				
				// 2009.02.16 UPDATE BY PWR : 한줄서기 예약상태에 등록번호는 책의 등록번호라 의미없어 지정여부로 변경
				//if( sRegNo.IsEmpty() )
				if(sReserveMark == _T("X"))
				{
					for( INT k = 0 ; k < nRCnt ; k++ )
					{
						ids = pDM->SetCellData( sReserveAlias[k] , _T("") , i);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("GridDisplay") ); 
					}
				}
			}
			
			ids = pDM->GetCellData( _T("등록번호") , i , sRegNo );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

			ids = pDM->GetCellData( _T("예약만기일") , i , strReservationExpireDate );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

			ids = pDM->GetCellData( _T("예약취소일") , i , strReserveCancelDate );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );
			
			// 2008.06.04 ADD BY PWR : 무인대출예약 대출상태값 변경해서 보여주기
			CString sWhere, sValue, sRecKey;
			if( m_pLoan->m_sUnmannedReserveSys == _T("Y") )
			{
				m_nUnmannedLoanStatus = 0;
				ids = pDM->GetCellData( _T("대출/반납정보KEY"), i, sRecKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

				ids = pDM->GetCellData( _T("무인대출예약"), i, sUnmannedReserveStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GridDisplay") );

				if( sUnmannedReserveStatus == _T("Y") )
				{
					m_nUnmannedLoanStatus = 1;//무인대출예약신청
				}
				else if( sUnmannedReserveStatus == _T("O") )
				{
					//예약만기일을 경과 하면 대출지연
					// 2011.09.19 UPDATE BY PWR : 무인예약자료인데 예약만기일 없는 자료일때 에러나는 현상 수정
					if(13 <= strReservationExpireDate.GetLength())
					{
						CTime tExpireDate( _ttoi(strReservationExpireDate.Mid(0,4)), _ttoi(strReservationExpireDate.Mid(5,2)), _ttoi(strReservationExpireDate.Mid(8,2)),_ttoi(strReservationExpireDate.Mid(11,2)),0,0 ); 
						// 2010.08.27 DEL BY PWR : 속도개선
	// 					CTime tCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
	// 					tCurrent = CTime(tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), tCurrent.GetHour(), 0, 0 );
						if( tExpireDate < tCurrent )
							m_nUnmannedLoanStatus = 3;//무인대출지연
						else
							m_nUnmannedLoanStatus = 2;//무인대출대기
					}
				}
			}
			
			// 2008.04.16 UPDATE BY PDW : 예약취소타입 필드가 추가되면서 상태값을 부여주는 룰이 변경되었다
			if( _T("4") == sLoanStatus || 0 < sLoanStatus.Find(_T("예약취소")) )
			{
				CString strReturnCancelType;
				ids = pDM->GetCellData( _T("예약취소타입"), i, strReturnCancelType );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , _T("GridDisplay") );

				if( strReturnCancelType.IsEmpty() )
				{
					ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, sLoanStatus );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7, _T("GridDisplay") ); 
				}
				else
				{
					/*
					0	예약취소(만기)
					1	예약취소(사서)
					2	예약취소(이용자)
					3	예약취소(무인)
					*/
					if(strReturnCancelType == _T("0"))
						sLoanStatus = _T("예약취소(만기)");
					else if(strReturnCancelType == _T("1"))
						sLoanStatus = _T("예약취소(사서)");
					else if(strReturnCancelType == _T("2"))
						sLoanStatus = _T("예약취소(이용자)");
					else if(strReturnCancelType == _T("3"))
						sLoanStatus = _T("예약취소(무인)");
					else
						sLoanStatus = _T("예약취소");
					/*
					const int nTypeCnt = 5;
					CString strstrReturnCancelTypeDesc[nTypeCnt][2] = {
						{_T("0"),_T("예약취소(만기)")},   {_T("1"),_T("예약취소(사서)")},
						{_T("2"),_T("예약취소(이용자)")}, {_T("3"),_T("예약취소(무인)")},
						{_T(""),_T("예약취소")}
					};

					for( int k=0;k<nTypeCnt;k++ )
					{
						if( 0 == strstrReturnCancelTypeDesc[k][0].Compare(strReturnCancelType) )
						{
							sLoanStatus = strstrReturnCancelTypeDesc[k][1];
							break;
						}
					}
					*/
				}
			}
			else
			{
				ids = ConvertLoanStatusCodeToDesc( 0, sRegNo, sLoanStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7, _T("GridDisplay") ); 
			}

			ids = pDM->SetCellData( _T("대출상태") , sLoanStatus , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("GridDisplay") ); 

			// 청구기호 생성
			ids = MakeShelfCode( sDMAlias[j] , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("GridDisplay") ); 

			// 배가 상태
			if( j == 0 )
			{
				ids = pDM->GetCellData( _T("배가상태") , i , sWorkingStatus );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("GridDisplay") ); 

				CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

				ids = pDM->SetCellData( _T("배가상태") , sWorkingStatus , i );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("GridDisplay") ); 

			}

			/*
			///@20071105 PDW
			///@{{++
			///@K3 요구사항 수정작업
			///@대출자료관리 검색결과에 총서명을 추가한다.
			///@총서명은 마크데이터에서 추출해야 한다.
			CString strMarc;

			ids = pDM->GetCellData(_T("MARC"), i , strMarc);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 

			if( 0 < strMarc.GetLength() )
			{
				CString strTotalTitle;
				ids = pDM->GetMarcTagData( strMarc, _T("440$a"), strTotalTitle );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 

				ids = pDM->SetCellData( _T("총서명"), strTotalTitle, i );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::GridDisplay( sCMAlias )") ); 
			}
			///@--}}
			*/
		}

	}
	ids = m_pParentMgr->AllControlDisplay( sCMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay") );


	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - 대출 자료 관리 자료 찾기(그리드 디스플레이) 걸린 시간 :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	return 0;

	EFS_END
	return -1;

}

// 2006.12.06 ADD BY PDJ
/// 예약상태를 사용자 취소와 만기 자동취소로 구분하기 위한것
INT CBL_LOC_LOAN_MNG_PROC::ConvertLoanStatusCodeToDesc(CString strReservationExpireDate, CString strReserveCancelDate, CString &sLoanStatus )
{
	// 예약 만기일 보다 취소일이 더 크면 자동취소된것임
	// CASE 1. [다음날] 대출반납 화면에서 취소된 경우
	// CASE 2. Cron에 등록해서 배치로 돌린경우 
	//         단 12시 이전에 돌리는 경우 문제.?? 

	if ( strReserveCancelDate == _T("") ) 
	{
		// 2009.01.07 ADD BY PWR (취소일없는 취소이력 4로 보이는현상때문에)
		sLoanStatus = _T("예약취소(취소일없음)");
		return 0;
	}

	// 2007.04.11 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++++	
	if( strReservationExpireDate.IsEmpty() )
	{
		sLoanStatus = _T("예약취소(이용자)");
	}
	else
	{
		if ( strReserveCancelDate <= strReservationExpireDate )
		{
			sLoanStatus = _T("예약취소(이용자)");
		}
		else
		{
			sLoanStatus = _T("예약취소(만기)");
		}
	}
	// --------------------------------------------------------------------------}}
	
	
	return 0;
}

// 대출상태의 코드를 설명으로 또는 설명을 코드로 변경하는 함수
// nMode 0 : 코드->설명
// nMode 1 : 설명->코드
INT CBL_LOC_LOAN_MNG_PROC::ConvertLoanStatusCodeToDesc( INT nMode, CString &sRegNo, CString &sCode )
{
	EFS_BEGIN
	
	// 2007.02.22 UPDATE BY PDW
	// 분관반납자료 추가
	// 2008.06.04 UPDATE BY PWR
	// 타관반납, 무인대출예약 추가
	// 2008.12.15 UPDATE BY CJY
	// 책두레대출, 책두레반납, 책두레반납연기 추가
	const INT nCnt = 17;
	TCHAR *sLoanStatus[nCnt][2] = 
	{
		{_T("0"),_T("대출")}				,{_T("1"),_T("반납")}				,{_T("2"),_T("반납연기")}, 
		{_T("3"),_T("예약(미지정)")}		,{_T("3"),_T("예약(지정완료)")}		,{_T("3"),_T("예약(무인대출신청)")},
		{_T("3"),_T("예약(무인대출대기)")}	,{_T("3"),_T("예약(무인대출지연)")}	,{_T("4"),_T("예약취소")}, 
		{_T("4"),_T("예약취소(이용자)")}	,{_T("4"),_T("예약취소(만기)")}		,{_T("5"),_T("반납(무인반납중)")},
		{_T("6"),_T("반납(분관반납중)")}	,{_T("7"),_T("반납(타관반납중)")}	,{_T("L"),_T("책두레대출")}	,
		{_T("R"),_T("책두레반납")}			,{_T("D"),_T("책두레반납연기")}
		/*
		{_T("0"),_T("대출")}, {_T("1"),_T("반납")}, {_T("2"),_T("반납연기")}, 
		{_T("3"),_T("예약(미지정)")}, {_T("3"), _T("예약(지정완료)")}, {_T("4"),_T("예약취소")}, 
		{_T("4"),_T("예약취소(이용자)")}, {_T("4"),_T("예약취소(만기)")}, {_T("5"),_T("반납(무인반납중)")},
		{_T("6"),_T("반납(분관관반납중)")}
		*/
	};

	INT nCompareIndex = 0;
	INT nReturnIndex = 1;
	if( nMode == 1 )
	{
		nCompareIndex = 1;
		nReturnIndex = 0;
	}

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sCode.Compare( sLoanStatus[i][nCompareIndex] ) != 0 ) continue;

		if(i == 3)//예약
		{
			if(m_nUnmannedLoanStatus==1)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "예약(무인대출신청)";

				m_nUnmannedLoanStatus = 0;
			}
			else if(m_nUnmannedLoanStatus==2)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "예약(무인대출대기)";

				m_nUnmannedLoanStatus = 0;
			}
			else if(m_nUnmannedLoanStatus==3)
			{
				if( nReturnIndex == 0 )
					sCode = "3";
				else
					sCode = "예약(무인대출지연)";

				m_nUnmannedLoanStatus = 0;
			}
			else
			{
				if(m_pLoan->m_bVolReserve)
				{ // 한줄서기예약
					if( sRegNo.IsEmpty() )
					{
						if( nReturnIndex == 0 )
							sCode = "3";
						else
							sCode = "예약(미지정)";
					}
					else
					{
						if( nReturnIndex == 0 )
							sCode = "3";
						else
							sCode = "예약(지정완료)";
					}
				}
				else
				{ // 일반예약
					if( nReturnIndex == 0 )
						sCode = "3";
					else
						sCode = "예약";
				}
			}
		}
		else
		{
			sCode = sLoanStatus[i][nReturnIndex];
		}
	}

	return 0;

	EFS_END
	return -1;

}

// 회원구분의 코드를 설명으로 또는 설명을 코드로 변경하는 함수
// nMode 0 : 코드->설명
// nMode 1 : 설명->코드
INT CBL_LOC_LOAN_MNG_PROC::ConvertMemberClassCodeToDesc( INT nMode , CString &sMemberClass )
{
	EFS_BEGIN

	const INT nCnt = 2;
	TCHAR *sMemberClassCode[nCnt][2] = 
	{
		{_T("0"),_T("정회원")} , {_T("1"),_T("비회원")} 
	};

	INT nCompareIndex = 0;
	INT nReturnIndex = 1;
	if( nMode == 1 )
	{
		nCompareIndex = 1;
		nReturnIndex = 0;
	}

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sMemberClass.Compare( sMemberClassCode[i][nCompareIndex] ) != 0 ) continue;

		sMemberClass = sMemberClassCode[i][nReturnIndex];
	}

	return 0;

	EFS_END
	return -1;

}

// 회원 정보를 보여준다.
INT CBL_LOC_LOAN_MNG_PROC::ViewMemberInfo()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM(sDMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo") );
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo") );

	if( pDM->GetRowCount() < 1 ) return 0;

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMemberInfo") );

	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("대출자KEY") , sRecKey , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMemberInfo") );
	
	// 2007.04.02 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 탈퇴회원은 정보를 볼 수 없다.
	CString strUserNo;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("대출자번호") , strUserNo , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewMemberInfo") );
	strUserNo.TrimLeft();strUserNo.TrimRight();

	if( strUserNo.IsEmpty() )
	{
		CString strQuery, strResult;
		strQuery.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sRecKey );
		ids = pDM->GetOneValueQuery( strQuery, strResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewMemberInfo") );

		// 0:정상, 1:대출정지, 2:제적, 3:탈퇴
		if( strResult ==_T("3") )
		{
			AfxMessageBox( _T("탈퇴처리가 완료되어 조회할 정보가 없습니다."), MB_ICONSTOP );
			return 0;
		}
	}
	//---------------------------------------------------------------------------------------}}

	CString sGridAlias;
	ids = GetParentGridAlias( sGridAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMemberInfo") );
	
	CBO_LOC_3320 ViewMember(m_pParentMgr, m_pLoanShareManager, m_pLoan);
	ViewMember.SetMode(4,TRUE);
	ViewMember.SetRecKey( sRecKey );
	ViewMember.SetGridAlias( sGridAlias );
	ViewMember.m_nAction = ACTION_READ_F_LOANIFOMGR;
	ViewMember.DoModal();
	
	return 0;

	EFS_END
	return -1;

}

// 현재 가르키고 있는 그리드의 열을 리턴
INT CBL_LOC_LOAN_MNG_PROC::GetCurrentGridPos()
{
	EFS_BEGIN

	CString sCMAlias;
	CString sGridAlias;

	INT ids;

	ids = GetParentCMAlias( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );

	ids = GetParentGridAlias( sGridAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridPos") );
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetCurrentGridPos") );

	INT nIndex = pGrid->GetIdx();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetCurrentGridPos") );

	return nIndex;

	EFS_END
	return -1;

}

// 청구기호 적용하는 함수
INT CBL_LOC_LOAN_MNG_PROC::MakeShelfCode( CString sDMAlias , INT nRowIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeShelfCode") );
	
	INT nRowCount = pDM->GetRowCount();
	
	CString sShelfCode;

	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , pDM , nRowIndex , sShelfCode );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeShelfCode") );
	

	return 0;

	EFS_END
	return -1;

}

// 대출자료 관리 수정
INT CBL_LOC_LOAN_MNG_PROC::ModifyLoanInfo()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ModifyLoanInfoDBProc"));

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyLoanInfoDBProc"));

	// Mobile API 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	// 1. SQL문 생성
	ids = MakeModifySQL( pMobileApi );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyLoanInfo") );

	// 2. DB 적용
	ids = ModifyLoanInfoDBProc( pMobileApi );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ModifyLoanInfo") );

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 3. DM 적용
	ids = CMToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ModifyLoanInfo") );

	// 4. 그리드 적용
	ids = DMToGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ModifyLoanInfo") );

	return 0;

	EFS_END
	return -1;

}

// 대출자료관리_수정 SQL문생성
INT CBL_LOC_LOAN_MNG_PROC::MakeModifySQL(CKolas2up_MobileApi *pMobileApi)
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias;
	ids = GetCMAlias3320( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	const INT nCnt = 7;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출일") , _T("반납예정일") , _T("반납일") , _T("예약일") , _T("예약만기일") ,
		_T("연체료") , _T("배상비") 
	};
	TCHAR *sDBFieldName[nCnt] = 
	{
		_T("LOAN_DATE") , _T("RETURN_PLAN_DATE") , _T("RETURN_DATE") , _T("RESERVATION_DATE") , _T("RESERVATION_EXPIRE_DATE") , 
		_T("ARREAR") , _T("REPARATION")
	};
	TCHAR *sType[nCnt] = 
	{
		_T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , 
		_T("NUMERIC") , _T("NUMERIC")
	};

	// 2007.06.18 add by pdw
	CString strTime;
	CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
	if( !pTimeDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

		if( _T("연체료") == sControlAlias[i] ||
			_T("배상비") == sControlAlias[i]) continue;
			
		if( !sGetData[i].IsEmpty() )
		{
			if( _T("대출일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("대출시간"),0,strTime);
			}
			else if( _T("반납예정일") == sControlAlias[i] )
			{
				// 2009.08.05 UPDATE BY PWR
				strTime = _T("23:59:59");
				//pTimeDM->GetCellData(_T("반납예정시간"),0,strTime);
			}
			else if( _T("반납일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("반납시간"),0,strTime);
			}
			else if( _T("예약일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("예약시간"),0,strTime);
			}
			else if( _T("예약만기일") == sControlAlias[i] )
			{
				// 2009.08.05 UPDATE BY PWR
				strTime = _T("23:59:59");
				//pTimeDM->GetCellData(_T("예약만기시간"),0,strTime);
			}

			if( strTime.IsEmpty() )
				strTime = _T("00:00:00");

			CString strTmp;
			strTmp.Format(_T("TO_DATE('%s %s','YYYY/MM/DD HH24:MI:SS')"),sGetData[i], strTime);

			sGetData[i] = strTmp;
		}
	}


	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeModifySQL")); 

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeModifySQL"));

	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString sUpdateFields;
	for( INT j = 0 ; j < nCnt ; j++ )
	{
		if( sGetData[j].IsEmpty() )
			if( CString(_T("NUMERIC")).Compare(sType[j])==0)
				sGetData[j] = _T("NULL");
			else
			{
				sType[j] = _T("NUMERIC");
				sGetData[j] = _T("NULL");
			}
		ids = pDM->AddDBFieldData( sDBFieldName[j] , sType[j] , sGetData[j] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeModifySQL")); 

		// 2009.08.05 ADD BY PWR : 대출공유
		if(!sUpdateFields.IsEmpty())
		{
			sUpdateFields += _T(",");
		}
		sUpdateFields += sDBFieldName[j];
	}
	// 2009.08.05 ADD BY PWR : LAST_WORK
	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("대출자료관리_수정"), __WFILE__, __LINE__ ), TRUE );

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeModifySQL") );

	CString sRecKey;
	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("MakeModifySQL"));

	// 2009.06.09 UPDATE BY PWR : CO_LOAN_TBL로 이동된 자료일수 있으므로 같이 UPDATE한다.
	CString strQuery, strCnt;
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
	pDM->GetOneValueQuery(strQuery, strCnt, FALSE);
	if(0 < _ttoi(strCnt))
	{
		ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	}
	else
	{
		ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	}
	//ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("MakeModifySQL"));

// 	// 2009.08.05 ADD BY PWR : 대출공유
// 	UPLOAD_SHARE_INFO UploadShareInfo;
// 	UploadShareInfo.nType = 0;
// 	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
// 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
// 	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	return 0;

	EFS_END
	return -1;

}

// 대출자료 관리 수정 :: DB 적용
INT CBL_LOC_LOAN_MNG_PROC::ModifyLoanInfoDBProc(CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ModifyLoanInfoDBProc");

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	// 2005.5. ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		//++2008.10.15 UPDATE BY PWR {{++
// 		// 통합시스템 적용
// 		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
// //		//++2006.07.31 ADD BY PDW(Jr) {{++
// //		// 관리구분을 사용하는지 체크
// //		CString	strWhere, strManageCode, strManageValue;		
// //		
// //		strWhere.Format(_T("SELECT MAX(VALUE_NAME) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND ")
// //						_T(" GROUP_2='기타' AND ")
// //	                    _T(" GROUP_3='공통' AND ")
// //                        _T(" CLASS_ALIAS='관리구분별도서관설정'") );
// //		pDM->GetOneValueQuery( strWhere, strManageValue, FALSE );
// //		//--2006.07.31 ADD BY PDW(Jr) --}}
// //
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") || strManageValue == _T("N") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		//--2008.10.15 UPDATE BY PWR --}}
// 
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}


	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pMobileApi->EndFrame();

	INT nIndex = GetCurrentGridPos();
	if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// 2010.08.05 ADD BY PWR : 통합도서서비스 대출자료관리 수정
	if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
	{
		CString strGetData;
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("통합회원여부"), strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == strGetData)
		{
			CString strStatus;
			pDM->GetCellData(_T("대출상태"), nIndex, strStatus);
			CString strRegNo;
			pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
			ConvertLoanStatusCodeToDesc(1, strRegNo, strStatus);
			if(_T("3") != strStatus || _T("4") != strStatus)
			{
				pDM->GetCellData(_T("관리구분"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
					if(NULL == pTimeDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					CString strRecKey;
					ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, strRecKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					Writer.addElement(_T("로컬KEY"), strRecKey);
					Writer.addElement(_T("대출도서관"), m_pParentMgr->m_pInfo->LIB_CODE);
					pDM->GetCellData(_T("대출자번호"), nIndex, strGetData);
					Writer.addElement(_T("통합대출자번호"), strGetData);
					CString strLoanDate, strTime;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("대출일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("대출시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strLoanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("대출일"), strLoanDate);
					}
					CString strReturnDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("반납일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("반납시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReturnDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("반납일"), strReturnDate);
					}
					CString strReturnPlanDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("반납예정일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReturnPlanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("반납예정일"), strReturnPlanDate);
					}
					CString strReservationDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("예약일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("예약시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReservationDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("예약일"), strReservationDate);
					}
					CString strReservationExpireDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3220"), _T("예약만기일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReservationExpireDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("예약만기일"), strReservationExpireDate);
					}
					CFrameSender Sender(m_pParentMgr->m_pInfo);
					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리_수정"), __WFILE__, __LINE__ ));
				}
			}
		}
	}

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// 대출 공유 대출/반납 정보 , 대출자 변경시 센터 도서관에 알리기 
	// 2003년 11월 8일 작업
	CString sRecKey;
	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("LOAN_DATE,RETURN_PLAN_DATE,RETURN_DATE,RESERVATION_DATE,RESERVATION_EXPIRE_DATE,ARREAR,REPARATION");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------

	return 0;

EFS_END
return -1;
}

// 대출자료 관리 수정 :: CM에 있는 자료를 DM에 적용
INT CBL_LOC_LOAN_MNG_PROC::CMToDM()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias;
	CString sDMAlias;

	ids = GetCMAlias3320( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

	// 2007.06.18 add by pdw
	CString strTime;
	CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3220"));
	if( !pTimeDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeModifySQL"));

	const INT nCnt = 7;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출일") , _T("반납예정일") , _T("반납일") , _T("예약일") , _T("예약만기일") ,
		_T("연체료") , _T("배상비") 
	};

	CString sGetData[nCnt];

	INT nIndex = GetCurrentGridPos();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CMToDM") );

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CMToDM"));

		// 2007.06.18 add by pdw
		if( _T("연체료") == sControlAlias[i] ||
			_T("배상비") == sControlAlias[i]) continue;
			
		if( !sGetData[i].IsEmpty() )
		{
			if( _T("대출일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("대출시간"),0,strTime);
			}
			else if( _T("반납예정일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("반납예정시간"),0,strTime);
			}
			else if( _T("반납일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("반납시간"),0,strTime);
			}
			else if( _T("예약일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("예약시간"),0,strTime);
			}
			else if( _T("예약만기일") == sControlAlias[i] )
			{
				pTimeDM->GetCellData(_T("예약만기시간"),0,strTime);
			}

			// 2009.10.27 ADD BY PWR : 반납예정일, 예약만기일 시간 23:59:59입력
			if(_T("반납예정일") == sControlAlias[i] || _T("예약만기일") == sControlAlias[i])
			{
				strTime = _T("23:59:59");
			}
			else
			{
				if( strTime.IsEmpty() && 
					!( _T("예약만기일") == sControlAlias[i] || _T("반납예정일") == sControlAlias[i] ) )
					strTime = _T("00:00:00");
			}

			if( 8 < strTime.GetLength() )
				strTime = strTime.Mid(0,8);


			CString strTmp;
			strTmp.Format(_T("%s %s"),sGetData[i], strTime);
			strTmp.TrimLeft();strTmp.TrimRight();
			sGetData[i] = strTmp;
		}

		ids = m_pParentMgr->SetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CMToDM"));
	}

	return 0;

	EFS_END
	return -1;

}

// 대출자료 관리 수정 :: DM에 있는 자료를 그리드에 적용
INT CBL_LOC_LOAN_MNG_PROC::DMToGrid()
{
EFS_BEGIN

	INT ids;

	CString sCMAlias;
	CString sGridAlias;
	CString sDMAlias;

	ids = GetParentCMAlias( sCMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("DMToGrid"));
	}

	ids = GetParentDM( sDMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("DMToGrid"));
	}
	
	ids = GetParentGridAlias( sGridAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("DMToGrid"));
	}

	INT nIndex = GetCurrentGridPos();
	if( 0 > nIndex ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("DMToGrid") );
	}

	const INT nCnt = 5;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출일") , _T("반납예정일") , _T("반납일") , _T("예약일") , _T("예약만기일") ,
	};

	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1 , -1 , -1 
	};


	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( NULL == pCM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("DMToGrid"));
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("DMToGrid")); 
	}

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );

	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("DMToGrid") );
	}

	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("DMToGrid"));
		}
		
		if( 0 == sPublishForm.Compare(_T("N")) && (( 3 == i ) || ( 4 == i )) ) 
		{
			continue;
		}

		if( -1 == nColIndex[i] )
		{
			continue;
		}

		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("DMToGrid"));
		}
	}

	return 0;

EFS_END
return -1;
}


// 예약처리 :: 예약취소
INT CBL_LOC_LOAN_MNG_PROC::CancelReservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CancelReservation");

	// 1. 선택된 자료 예약취소 하기
	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");
	CString sCMAlias = _T("CM_BO_LOC_3240");
	CString sTableName = _T("LS_WORK_T01");

	// 현재 사용되고 있는 그리드 얻어오기
	CESL_Grid *pGrid = GetClientGridPtr();
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pGrid->SelectMakeList();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// 부모 그리드 얻어오기
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(NULL == pParentGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRowCount = pGrid->SelectGetCount();

	if(1 > nRowCount)
	{
 		ViewMessage(_T("자료를 선정해주십시오"));
		return 0;
	}
	ids = MessageBox( m_pParentMgr->m_hWnd , _T("선택된 자료를 취소하시겠습니까?") , _T("예약 취소") , MB_YESNO | MB_ICONQUESTION );
	if( IDNO == ids ) 
	{
		return 0;
	}

	// 현재 사용되는 DM 얻어오기
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	// 부모 DM 얻어오기
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if(NULL == pParentDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if ( TRUE == m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere, strManageCode;		
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 		pDM->GetOneValueQuery( strWhere , m_strLibCode , FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	pGrid->SelectGetTailPosition();
	INT nIndex;
	CString sRecKey;
	CString sStatus;
	CString sReservationBookCnt;
	CString sUserRecKey;
	CString sMailSql;

	INT nStatusColIndex = -1;
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("대출상태") , nStatusColIndex );

	CLocMailMgrAPI* pMailMgr;
	pMailMgr = NULL;

	if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE == 30000) ) ;
	else
	{
		if( m_pReservationInfo->m_pLoanProc->m_pCheckLoan->IsSendCancelMail() )
		{
			pMailMgr = new CLocMailMgrAPI( m_pParentMgr );
			pMailMgr->CreateMailInfo();
		}
	}

	INT		i , nCount = 0;
	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData( _T("대출상태") , nIndex , sStatus );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		// 2005.02.21 UPDATE BY PDJ
		// 기존 조건으로는 자료가 검색되지 않는다.
		//if( (0!=sStatus.Compare(_T("예약") )) || (0!=sStatus.Find(_T("예약(") ))  )
		if(	TRUE !=	( 0 == sStatus.Compare(_T("예약")) || 0 == sStatus.Find(_T("예약(")) ) )
		{
			pGrid->SelectGetPrev();
			continue;
		}

		// 2010.05.20 ADD BY PWR : 무인예약사용시 무인예약신청자료는 메시지, 대기와지연자료는 취소불가처리
		if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
		{
			CString strQuery, strValue, strMsg;;
			strQuery.Format(_T("SELECT UNMANNED_RESERVATION_LOAN FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
			pDM->GetOneValueQuery(strQuery, strValue);
			if(_T("Y") == strValue)
			{ // 무인예약신청
				strMsg.Format(_T("무인예약신청 자료입니다.\r\n예약취소 하시겠습니까?"));
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("경고"), MB_ICONWARNING | MB_YESNO))
				{
					pGrid->SelectGetPrev();
					continue;
				}
			}
			else if(_T("O") == strValue)
			{ // 무인예약대기, 지연
				strMsg = _T("무인예약대기 자료는 예약취소 할 수 없습니다.");
				MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("경고"), MB_ICONWARNING);
				pGrid->SelectGetPrev();
				continue;
			}
		}

		// Mobile API 적용
		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

		// DB UPDATE
		// LH_STORAGE_V01
		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("4") );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 		if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE==20000) )	
//REMOVE▶ 			ids = pDM->AddDBFieldData( _T("RESERVE_CANCEL_DATE") , _T("NUMERIC") , _T("now()") );
		//--2008.09.02 DEL BY LKS --}}
//		else

// 		// 2010.07.28 UPDATE BY PWR : 통합도서서비스일때는 취소일자 기록하여 적용되도록
// 		CString strDate;
// 		CString sReserveCancelDate;
// 		if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
// 		{
// 			strDate = (CLocCommonAPI::GetDBTime(m_pParentMgr)).Format(_T("%Y/%m/%d %H:%M:%S"));
// 			sReserveCancelDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strDate);
// 		}
// 		else
// 		{
// 			sReserveCancelDate = _T("SYSDATE");
// 		}
// 		ids = pDM->AddDBFieldData(_T("RESERVE_CANCEL_DATE"), _T("NUMERIC"), sReserveCancelDate);
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 		ids = pDM->AddDBFieldData( _T("RESERVE_CANCEL_DATE") , _T("NUMERIC") , _T("SYSDATE") );
 		if( 0 > ids ) 
 		{
 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1100 , _T("CancelReservation") );
 		}

		//++2008.04.16 ADD BY PDW {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 예약취소 타입 필드가 추가되었다
		ids = pDM->AddDBFieldData( _T("RESERVATION_CANCEL_TYPE"), _T("STRING"), _T("1") );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		//----------------------------------------------------------------------------------------}}

		//++2008.11.20 ADD BY PWR {{++
		// 예약취소시 R_DEVICE에 KOLASIII저장 (LH_STORAGE_V01에서 SELECT될수 있도록)
		ids = pDM->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		//--2008.11.20 ADD BY PWR --}}

		// 2009.08.05 ADD BY PWR : LAST_WORK
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("대출자료관리_예약취소"), __WFILE__, __LINE__ ), TRUE );

//		ids = pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		// CO_LOAN_USER_TBL
		// 대출자 정보
		ids = pDM->GetCellData( _T("대출자KEY") , nIndex , sUserRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = GetTotalReservationBookCnt( sUserRecKey , sReservationBookCnt );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		INT nReservationBookCnt = _ttoi(sReservationBookCnt);
		nReservationBookCnt = ( 0 > nReservationBookCnt-1 ) ? 0 : nReservationBookCnt-1;
		sReservationBookCnt.Format( _T("%d") , nReservationBookCnt);
	
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData( _T("RESERVATION_COUNT") , _T("NUMERIC") , sReservationBookCnt );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		// 2009.08.05 ADD BY PWR : LAST_WORK
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("대출자료관리_예약취소"), __WFILE__, __LINE__ ), TRUE );

//		ids = pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
		ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pDM->EndFrame();

// 		// 2010.07.28 ADD BY PWR : 통합도서서비스 대출자료관리 예약취소
// 		if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
// 		{
// 			CString sParentIndex;
// 			ids = pDM->GetCellData(_T("부모INDEX"), nIndex, sParentIndex);
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 			CString strGetData;
// 			ids = pParentDM->GetCellData(_T("관리구분"), _ttoi(sParentIndex), strGetData);
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 			if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
// 			{
// 				ids = pParentDM->GetCellData(_T("통합회원여부"), _ttoi(sParentIndex), strGetData);
// 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 				if(_T("Y") == strGetData)
// 				{
// 					CFrameWriter Writer(m_pParentMgr->m_pInfo);
// 					Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
// 					Writer.addRecord();
// 					ids = pParentDM->GetCellData(_T("대출자번호"), _ttoi(sParentIndex), strGetData);
// 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 					Writer.addElement(_T("통합대출자번호"), strGetData);
// 					Writer.addElement(_T("대출도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
// 					Writer.addElement(_T("반납도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
// 					Writer.addElement(_T("예약취소일"), sReserveCancelDate);
// 					ids = pParentDM->GetCellData(_T("등록번호"), _ttoi(sParentIndex), strGetData);
// 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 					Writer.addElement(_T("등록번호"), strGetData);
// 					Writer.addElement(_T("상태"), _T("4"));
// 					CFrameSender Sender(m_pParentMgr->m_pInfo);
// 					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리-예약취소"), __WFILE__, __LINE__));
// 				}
// 			}
// 		}

		// Mobile API delete
		CLocCommonAPI::MobileApiEndLog( pMobileApi );

		// --------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		// 대출 공유 대출/반납 정보 , 대출자 변경시 센터 도서관에 알리기 
		// 2003년 11월 8일 작업
		
		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS,RESERVE_CANCEL_DATE,RESERVATION_CANCEL_TYPE,R_DEVICE");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		// 2. 대출자 정보 Update
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sUserRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("RESERVATION_COUNT");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		// --------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		
		// 화면 UPDATE
		// GRID 업데이트
		pGrid->SelectGetPrev();

		CString sParentIndex;
		ids = pDM->GetCellData( _T("부모INDEX") , nIndex , sParentIndex );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pParentDM->SetCellData( _T("대출상태") , _T("예약취소") , _ttoi(sParentIndex) );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if ( TRUE == m_pLoan->m_bVolReserve )
		{
				/// 예약 순위 결정하기
				/// 예약 순위가 변경되어야 한다
				CString sStatus , sRSpeciesKey , sRVolInfoKey;
				ids = pDM->GetCellData( _T("종KEY") , nIndex , sRSpeciesKey);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				ids = pDM->GetCellData( _T("권호정보KEY") , nIndex , sRVolInfoKey );
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CESL_DataMgr* pParentDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3200") );
				if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentMgr->FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
				if(NULL == pParentGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				CString sSpeciesKey , sVolInfoKey;
				INT		j , nParentRowCnt;
				nParentRowCnt		=	pParentDM->GetRowCount();
				for( j=0 ; j<nParentRowCnt ; j++ )
				{
					ids = pParentDM->GetCellData( _T("대출상태") , j , sStatus );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pParentDM->GetCellData( _T("종KEY") , j , sSpeciesKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pParentDM->GetCellData( _T("권호정보KEY") , j , sVolInfoKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					// 2009.05.18 UPDATE BY KSJ : 예약취소시 그리드 갱신 안 되는 문제 수정
					//if( ( 0 == sStatus.Find(_T("예약(")) ) && ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					if( ( 0 == sStatus.Find(_T("예약")) ) && ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					{
						ReViewOrder( pParentDM , j );
						pParentGrid->DisplayLine(j);
					}
				}

				INT		nRowCnt;
				nRowCnt		=	pDM->GetRowCount();
				for( j=0 ; j<nRowCnt ; j++ )
				{
					ids = pDM->GetCellData( _T("종KEY") , j , sSpeciesKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pDM->GetCellData( _T("권호정보KEY") , j , sVolInfoKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					if( ( 0 == sSpeciesKey.Compare(sRSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(sRVolInfoKey) ) )
					{
						ReViewOrder( pDM , j );
						pGrid->DisplayLine(j);
					}
				}
		}
		else
		{
				if( -1 != nStatusColIndex )
				{
					ids = pParentGrid->SetAt( _ttoi(sParentIndex) , nStatusColIndex , _T("예약취소") );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				}
		}

		// 2006-11-21 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++
		// 필드명 오류
		//sMailSql.Format( _T(" CL.LOAN_KEY = %s ") , sRecKey );
		sMailSql.Format( _T(" CL.REC_KEY = %s ") , sRecKey );
		// -------------------------------------------------------------------

		if( m_pReservationInfo->m_pLoanProc->m_pCheckLoan->IsSendCancelMail() )
		{
			if( NULL != pMailMgr )
			{
				pMailMgr->MakeRecord( 6 , sMailSql , FALSE );
			}
		}

		pDM->DeleteRow( nIndex );
		nCount++;
	}

	if( NULL != pMailMgr )
	{
		delete pMailMgr;
	}

	pMailMgr = NULL;

	CString sMsg;
	if( 0 < nCount )
	{
		// 순서 재 부여
		ReViewOrder();

		sMsg.Format( _T("총 %d건이 예약 취소되었습니다.") , nCount );
		ViewMessage( sMsg );
		m_pParentMgr->AllControlDisplay(sCMAlias);
	}
// 	else
// 	{
// 		sMsg.Format( _T("자료를 선정해주십시오") , nCount );
// 		ViewMessage( sMsg );
// 	}

	return 0;

EFS_END
return -1;
}

// 총 예약 책수 GET
INT CBL_LOC_LOAN_MNG_PROC::GetTotalReservationBookCnt( CString sUserRecKey , CString &sReservationBookCnt )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	// 현재 사용되는 DM 얻어오기
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1, _T("GetTotalReservationBookCnt") );

	CString sSQL;
	sSQL.Format( _T("SELECT RESERVATION_COUNT FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s ") , sUserRecKey );
//	ids = pDM->GetOneValueQuery( sSQL , sReservationBookCnt );
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM , sSQL , sUserRecKey );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetTotalReservationBookCnt") );

	return 0;

	EFS_END
	return -1;

}

// 예약 만기일 부여
INT CBL_LOC_LOAN_MNG_PROC::RecordExpireDate()
{
EFS_BEGIN

	INT ids;

	// 1. 선택된 자료 예약취소 하기
	CString sDMAlias		= _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias	= _T("DM_BO_LOC_3200");

	// 현재 사용되고 있는 그리드 얻어오기
	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("RecordExpireDate") );
	}

	ids = pGrid->SelectMakeList();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("RecordExpireDate") );
	}

	// 부모 그리드 얻어오기
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if( NULL == pParentGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("RecordExpireDate") );
	}

	INT nRowCount = pGrid->SelectGetCount();

	// 현재 사용되는 DM 얻어오기
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("RecordExpireDate") );
	}

	// 부모 DM 얻어오기
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( NULL == pParentDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("RecordExpireDate") );
	}

	INT nExpireColIndex = -1;
	INT nParentExpireColIndex = -1;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("예약만기일") , nExpireColIndex );
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("예약만기일") , nParentExpireColIndex );

	// 2009.04.23 ADD BY KSJ : SMS 발송결과 추가
	INT		nSMSResultIdx = -1;
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("SMS발송결과") , nSMSResultIdx );


	pGrid->SelectGetTailPosition();
	
	INT nIndex;
	CString sRecKey;
	CString sBookKey;
	CString sRegNo;

	INT		nCount				= 0;
	INT		nFailReturn			= 0;
	INT		nFailFirstReserve	= 0;
	INT		nFailRC				= 0;	// 예약지정이 부여가 안 되어 있어서 실패한 자료 Cnt
	INT		nFailReserveExpireDay	= 0;	// 예약만기일이 이미 부여 되서 실패한 자료 Cnt
	// 2009.10.21 ADD BY KSJ : 관리구분이 다른 자료 예약만기일 부여 안 되도록 수정
	INT		nFailOtherManageCode = 0;	// 관리구분이 다른 자료라 실패한 자료 Cnt
	
	// 2009.04.22 ADD BY KSJ : SMS 발송결과 추가
	INT		nSendSMSCnt		=	0;		// SMS 발송 성공 Cnt
	INT		nFailSMSUseYN	=	0;		// 이용자가 SMS 수신 동의 안 해서 SMS발송 실패한 경우 Cnt
	INT		nFailPhone		=	0;		// 이용자의 폰번호가 없어서 SMS발송 실패한 경우 Cnt
	INT		nFailMakeMsg	=	0;		// 메세지 구성에 실패하여 SMS발송 실패한 경우 Cnt
	INT		nFailUnSelectProc	=	0;	// Procedure에서 실패한 경우 Cnt
	
	// 2009.05.18 ADD BY KSJ : SMS 발송 결과 메시지 사용 Flag 추가
	BOOL	bShowSMSSendResult = FALSE;

	// 2010.06.17 ADD BY PWR : 무인예약상태 추가
	INT nUnmennedCnt = 0;
	INT nFailUnmenned = 0;

	INT		i;
	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		if( 0 > nIndex ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("RecordExpireDate") );
		}

		ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("RecordExpireDate") );
		}

		ids = pDM->GetCellData( _T("대출책KEY") , nIndex , sBookKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("RecordExpireDate") );
		}

		//한줄서기 예약 일 경우
		/*
		if ( m_pLoan->m_bVolReserve )
		{
				ids = pDM->GetCellData( _T("등록번호") , nIndex , sRegNo );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8, _T("RecordExpireDate") );

				// 지정 완료된 자료인지 알아본다.
				if( sRegNo.IsEmpty() )
				{
					pGrid->SelectGetPrev();
					nFailRC++;
				}
		}
		*/

		// 2009.10.21 ADD BY KSJ : 관리구분이 다른 자료 예약만기일 부여 안 되도록 수정
		CString		strManageCode;
		pDM->GetCellData( _T("관리구분") , nIndex , strManageCode );
		if( m_pParentMgr->m_pInfo->MANAGE_CODE != strManageCode )
		{
			pGrid->SelectGetPrev();
			nFailOtherManageCode++;
			continue;
		}

		// 2010.06.17 ADD BY PWR : 무인대출예약상태추가
		BOOL bUnmanned = FALSE;
		CString	strUnmanned;
		ids = pDM->GetCellData(_T("무인대출예약"), nIndex, strUnmanned);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, _T("RecordExpireDate"));

		//일반예약 일 경우
		if ( !(m_pLoan->m_bVolReserve) )
		{
			// 1. 예약 만기일 부여하려는 책이 반납되었는지 조사
			ids = m_pReservationInfo->IsReturn( pDM , sBookKey );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1090 , _T("RecordExpireDate") );
			}

			if( 0 < ids ) 
			{
				pGrid->SelectGetPrev();
				nFailReturn++;
				continue;
			}

			// 2. 예약 만기일 부여하려는 사람이 예약된 사람중 제일 처음인지 조사
			ids = m_pReservationInfo->IsFirstReservation( pDM , sBookKey , sRecKey );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1100 , _T("RecordExpireDate") );
			}

			if( 0 < ids ) 
			{
				pGrid->SelectGetPrev();
				nFailFirstReserve++;
				continue;
			}

			// 2010.06.17 ADD BY PWR : 일반에약에서는 무인예약일경우 만기일부여안함
			if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
			{
				if(!strUnmanned.IsEmpty())
				{
					pGrid->SelectGetPrev();
					nFailUnmenned++;
					continue;
				}
			}
		}
		// 2010.06.17 ADD BY PWR : 한줄서기일때 무인예약일경우 처리
		else
		{
			if(_T("Y") == m_pLoan->m_sUnmannedReserveSys)
			{
				if(!strUnmanned.IsEmpty())
				{
					bUnmanned = TRUE;
				}
			}
		}

		//++2008.12.22 ADD BY KSJ{{++
		//자료 지정이 안 된 자료는, 예약만기일 부여 할 수 없음
		CString		strRegNo;
		pDM->GetCellData( _T("등록번호") , nIndex , strRegNo );
		if( _T("")	==	strRegNo )
		{
			pGrid->SelectGetPrev();
			nFailRC++;
			continue;
		}

		//예약만기일이 이미 부여 된 자료에, 예약만기일 부여 할 수 없음		
		CString		strExpireDate;
		pDM->GetCellData( _T("예약만기일") , nIndex , strExpireDate );
		if( _T("")	!=	strExpireDate )
		{
			pGrid->SelectGetPrev();
			nFailReserveExpireDay++;
			continue;
		}
		//--2008.12.22 ADD BY KSJ--}}

		// 3. 예약 만기일 얻어오기
		INT nExpireDay = 0;

		CString sPositionCode;
		pDM->GetCellData( _T("직급") , nIndex , sPositionCode );

		ids = m_pReservationInfo->GetReservationExpireDay( nExpireDay , sPositionCode );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1110 , _T("RecordExpireDate") );
		}

		CString sExpireDate;
		ids = m_pReservationInfo->MakeReservationExpireDate( nExpireDay , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1120 , _T("RecordExpireDate") );
		}
		
		// 4. DB 적용
		ids = m_pReservationInfo->ExpireDBProc( pDM , sRecKey , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1130 , _T("RecordExpireDate") );
		}

		// 화면 UPDATE
		// GRID 업데이트
		pGrid->SelectGetPrev();

		CString sParentIndex;
		ids = pDM->GetCellData( _T("부모INDEX") , nIndex , sParentIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1140 , _T("RecordExpireDate") );
		}

		INT		nParentIndex	=	_ttoi(sParentIndex);
		//++2009.01.29 ADD BY KSJ ++{{
		//예약지정 후 예약만기일 재부여 되는 문제로 인하여, DM_BO_LOC_3240_BO DM에 예약만기일 적용
		ids = pDM->SetCellData( _T("예약만기일") , sExpireDate , nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1150 , _T("RecordExpireDate") );
		}
		//--2009.01.29 ADD BY KSJ --}}

		ids = pParentDM->SetCellData( _T("예약만기일") , sExpireDate , nParentIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1160 , _T("RecordExpireDate") );
		}

		ids = pParentGrid->SetAt( nParentIndex , nParentExpireColIndex  , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1170 , _T("RecordExpireDate") );
		}
		
		ids = pGrid->SetAt( nIndex , nExpireColIndex , sExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1180 , _T("RecordExpireDate") );
		}

		nCount++;

		// 2010.06.17 ADD BY PWR : 무인예약일경우 처리
		if(TRUE == bUnmanned)
		{
			nUnmennedCnt++;
		}

		//===========================================================================
		// 2009.04.21 ADD BY KSJ : 예약만기일이 부여 됐다면 관리값에 따라 SMS 발송
		/// SMS System 사용여부
		CString strSMSSystemUseMode;
		ids = m_pParentMgr->m_pInfo->GetSMSUseMode();
		if( 1 == ids )
		{ //// SMS System 사용
			strSMSSystemUseMode	=	_T("Y");
		}
		else
		{ //// SMS System 사용 안 함
			strSMSSystemUseMode	=	_T("N");
		}

		// 2009.05.18 UPDATE BY KSJ : SMS 발송 결과 메시지 사용 Flag 추가
		if(		_T("Y") == strSMSSystemUseMode				// SMS System 사용여부
			&&	_T("Y") == m_pLoan->m_sReserveExpireSMS	)	// 예약만기일 부여 할 경우 SMS 발송 여부)
		{
			bShowSMSSendResult = TRUE;
		}

		if(		TRUE	==	bShowSMSSendResult
			&&	10000	!=	m_pParentMgr->m_pInfo->MODE		// 2009.05.14 ADD BY KSJ : 이동도서관일 경우 SMS 발송 안 함
			&&	30000	!=	m_pParentMgr->m_pInfo->MODE	)	
		{
			// 2010.06.17 ADD BY PWR : 무인예약자료는 SMS발송안함
			if(!strUnmanned.IsEmpty())
			{
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("무인예약자료는 장비에 넣으면 발송됩니다.") , nIndex );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1300 , _T("RecordExpireDate") );
				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("무인예약자료는 장비에 넣으면 발송됩니다.") );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1260 , _T("RecordExpireDate") );
				continue;
			}

			/// 이용자 SMS 수신여부
			CString		strSMSUseYN;
			ids = pParentDM->GetCellData( _T("SMS수신여부") , nParentIndex , strSMSUseYN );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1250 , _T("RecordExpireDate") );
			}

			if( _T("Y") != strSMSUseYN )
			{/// 이용자가 SMS 서비스 신청을 하지 않아서 SMS 발송 실패
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("수신 동의하지 않은 이용자 입니다.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1300 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("수신 동의하지 않은 이용자 입니다.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1260 , _T("RecordExpireDate") );
				}

				nFailSMSUseYN++;
				continue;
			}

			CESL_DataMgr TmpDM;
 			TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
 			TmpDM.SetManageCode(m_pParentMgr->m_pInfo->MANAGE_CODE);

			CString		strUserName, strUserKey, strLoanKey, strTitle, strReceiveCallNo, strSMSType;
			strSMSType	=	_T("SMS03");
			ids = pDM->GetCellData( _T("대출자"),			nIndex, strUserName);		// 회원이름
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1200 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("대출자KEY"),		nIndex, strUserKey);		// USER_KEY
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1210 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("대출/반납정보KEY"),	nIndex, strLoanKey);		// LOAN_KEY
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1220 , _T("RecordExpireDate") );
			}

			ids = pDM->GetCellData( _T("본표제"),			nIndex, strTitle);			// TITLE
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1230 , _T("RecordExpireDate") );
			}

			ids = pParentDM->GetCellData( _T("휴대폰"), nParentIndex, strReceiveCallNo);// 받는사람 번호
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1240 , _T("RecordExpireDate") );
			}
			
			CSMSPreviewDlg	SMSDlg(m_pParentMgr);
			SMSDlg.SetInfo( strUserKey, strLoanKey, strUserName, strTitle, sExpireDate, strReceiveCallNo, strSMSType);
			ids = SMSDlg.MakeMsg();
			if( 1030 == ids )
			{ /// 1030 : 핸드폰 번호 없어서 SMS 발송 실패
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("수신자 전화번호가 옳바르지 않습니다.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1310 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("수신자 전화번호가 옳바르지 않습니다.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1270 , _T("RecordExpireDate") );
				}

				nFailPhone++;
				continue;
			}
			else if( 0 > ids )
			{ /// 메세지 구성 실패로 인한 SMS 발송 실패
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("메세지 구성에 실패하였습니다.") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1320 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("메세지 구성에 실패하였습니다.") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1280 , _T("RecordExpireDate") );
				}

				nFailMakeMsg++;
				continue;
			}


			ids = SMSDlg.SendMsg();
			if( 0 == ids )
			{ /// SMS 발송 성공
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("성공") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1330 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("성공") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1290 , _T("RecordExpireDate") );
				}

				nSendSMSCnt++;
			}
			else 
			{ /// Procedure에서 발송 실패
				ids = pDM->SetCellData( _T("SMS발송결과") , _T("UnSelectProc Error") , nIndex );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1340 , _T("RecordExpireDate") );
				}

				ids = pGrid->SetAt( nIndex , nSMSResultIdx , _T("UnSelectProc Error") );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1290 , _T("RecordExpireDate") );
				}

				nFailUnSelectProc++;
			}
		}
		//===========================================================================
	}

	CString sMsg;
	if( 0 == nRowCount ) 
	{
		sMsg.Format( _T("자료를 먼저 선정해주십시오") );
	}
	else
	{
		sMsg.Format( _T("총 처리 건수 : %d ") , nRowCount );
		CString sTmp;
		// 2009.10.21 ADD BY KSJ : 관리구분이 다른 자료 예약만기일 부여 안 되도록 수정
		if( 0 < nFailOtherManageCode )
		{
			sTmp.Format(_T("\r\n타관 예약자료이기 때문에 예약 만기일 부여에 실패한 건수 : %d ") , nFailOtherManageCode );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailRC ) 
		{
			sTmp.Format(_T("\r\n지정완료 되지 않은 자료  : %d ") , nFailRC );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailReturn ) 
		{
			sTmp.Format(_T("\r\n반납이 되지 않아 예약 만기일 부여에 실패한 건수 : %d ") , nFailReturn );
		}

		sMsg += sTmp;
		sTmp.Empty();
		if( 0 < nFailFirstReserve ) 
		{
			sTmp.Format(_T("\r\n앞에 예약된 사람이 있기 때문에 예약 만기일 부여에 실패한 건수 : %d ") , nFailFirstReserve  );
		}

		sMsg += sTmp;
		sTmp.Empty();

		//++2009.01.09 ADD BY KSJ {{++
		//예약만기일이 이미 부여 된 자료에 대한 Message 추가
		if( 0 < nFailReserveExpireDay )  
		{
			sTmp.Format(_T("\r\n예약 만기일이 이미 부여 되었기 때문에 예약 만기일 부여에 실패한 건수 : %d ") , nFailReserveExpireDay  );
		}

		sMsg += sTmp;
		sTmp.Empty();
		//--2009.01.09 ADD BY KSJ --}}

		// 2010.06.17 ADD BY PWR : 무인예약일경우 처리
		if(0 < nFailUnmenned)
		{
			sTmp.Format(_T("\r\n무인예약자료라 실패한 건수 : %d ") , nFailReserveExpireDay  );
		}
		sMsg += sTmp;
		sTmp.Empty();

		if( 0 < nCount ) 
		{
			sTmp.Format(_T("\r\n성공한 건수 : %d ") , nCount );
		}

		sMsg += sTmp;
		sTmp.Empty();

		// 2009.05.14 ADD BY KSJ : 이동도서관일 경우 SMS 발송 안 함 ( 10000 : 이동도서관 , 30000 : 현재 사용 안 하는 MODE 값 )
		if( 10000 == m_pParentMgr->m_pInfo->MODE || 30000 == m_pParentMgr->m_pInfo->MODE )
		{/// 이동도서관일 경우
		}
		else
		{/// 이동도서관 아닐 경우
			// 2009.05.18 ADD BY KSJ : SMS 발송 결과는 SMS SYSTEM 사용하고 예약만기일 부여 할 경우 SMS 발송 사용 할 경우에만 보여줌
			if( TRUE == bShowSMSSendResult )
			{
				//==============================================================================
				// 2009.04.22 ADD BY KSJ : SMS 발송 결과 출력
				// 2010.06.17 UPDATE BY PWR : 무인자료는 SMS안보내므로
				if( 0 < (nCount - nUnmennedCnt) ) 
// 				if( 0 < nCount ) 
				{
					// 2010.06.17 UPDATE BY PWR : 무인자료는 SMS안보내므로
					sTmp.Format( _T("\n\r\n\rSMS발송 총 처리 건수 : %d ") , nCount - nUnmennedCnt);
// 					sTmp.Format( _T("\n\r\n\rSMS발송 총 처리 건수 : %d ") , nCount );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailSMSUseYN ) 
				{
					sTmp.Format( _T("\r\nSMS수신 신청하지 않아 발송 실패한 건수 : %d ") , nFailSMSUseYN );
					sMsg += sTmp;
					sTmp.Empty();
				}
				
				if( 0 < nFailPhone ) 
				{
					sTmp.Format( _T("\r\n수신자의 폰번호가 없어 발송 실패한 건수 : %d ") , nFailPhone );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailMakeMsg ) 
				{
					sTmp.Format( _T("\r\n메세지 구성에 실패하여 발송 실패한 건수 : %d ") , nFailMakeMsg );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nFailUnSelectProc ) 
				{
					sTmp.Format( _T("\r\nUnSelectProc Error 건수 : %d ") , nFailUnSelectProc );
					sMsg += sTmp;
					sTmp.Empty();
				}

				if( 0 < nSendSMSCnt ) 
				{
					sTmp.Format( _T("\r\nSMS발송 성공한 건수 : %d ") , nSendSMSCnt );
					sMsg += sTmp;
					sTmp.Empty();
				}
				//==============================================================================	
			}
		}
	}
	ViewMessage( sMsg );

	// 2010.06.17 ADD BY PWR : 무인예약대출자료 메시지 처리
	if(0 < nUnmennedCnt)
	{
		sMsg.Format( _T("[ %d ]건의 무인예약 자료가 처리 되었습니다."), nUnmennedCnt);
		ViewMessage(sMsg);
	}
	
	return 0;

EFS_END
return -1;
}


CESL_Grid* CBL_LOC_LOAN_MNG_PROC::GetClientGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3240");
	CString sGridAlias = _T("ReservationProcessGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	return pGrid;

	EFS_END
	return NULL;

}

CESL_Grid* CBL_LOC_LOAN_MNG_PROC::GetParentGridPtr()
{
	EFS_BEGIN

	INT ids;

	CString sParentCMAlias;
	CString sParentGridAlias;

	ids = GetParentCMAlias( sParentCMAlias );
	if( ids < 0 ) return NULL;

	ids = GetParentGridAlias( sParentGridAlias );
	if( ids < 0 ) return NULL;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sParentGridAlias ));

	return pGrid;

	EFS_END
	return NULL;

}

// 대출자료관리_예약처리_대출자상세보기
INT CBL_LOC_LOAN_MNG_PROC::ViewMemberInfo2()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewMemberInfo2") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetClientGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewMemberInfo2") );

	INT nIndex = pGrid->GetIdx();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewMemberInfo2") );

	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("대출자KEY") , sRecKey , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewMemberInfo2") );

	
	CBO_LOC_3320 ViewMember(m_pParentMgr, m_pLoanShareManager, m_pLoan);
	ViewMember.SetMode(5,TRUE);
	ViewMember.SetRecKey( sRecKey );
	ViewMember.m_nAction = ACTION_READ_F_LOANIFOMGR;
	ViewMember.DoModal();
	
	return 0;

	EFS_END
	return -1;

}

// 메일
INT CBL_LOC_LOAN_MNG_PROC::OpenMail()
{
	EFS_BEGIN

	
	INT ids;
	CString sDMAlias;
	GetParentDM(sDMAlias);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1, _T("OpenMail") );

	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OpenMail") );

	if( pGrid->GetCount() < 1 ) return 0;

	pGrid->SelectMakeList();
	INT nCount = pGrid->SelectGetCount();

	if( nCount < 1 ) 
	{
		AfxMessageBox( _T("메일 보낼 자료를 선택해 주십시오") );
		return 0;
	}

	/*
	if( nCount > 100 )
	{
		AfxMessageBox( _T("선정한 자료가 100건이 넘어 메일 발송을 할수 없습니다. 100건 이하로 선정해주십시오.") );
		return 0;
	}
	*/

	// 메일 종류 선택하기	
	CLocMailMgrAPI MailMgr(m_pParentMgr);

	ids = MailMgr.MakeRecord( -1 , pDM , pGrid , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OpenMail") );


	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::SetCheckParentMailSendYN( CString sLoanKey , CString sYN )
{
	EFS_BEGIN

	INT ids;

	CString sParentDMAlias = _T("DM_BO_LOC_3200");

	// 현재 사용되고 있는 그리드 얻어오기

	// 부모 그리드 얻어오기
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SetCheckMailSendYN") );

	// 부모 DM 얻어오기
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OSetCheckMailSendYN"));

	INT nParentColIndex = -1;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("메일발송") , nParentColIndex );

	CString sTmpLoanKey;
	for( INT i = 0 ; i < pParentDM->GetRowCount() ; i++ )
	{
		ids = pParentDM->GetCellData( _T("대출/반납정보KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		if( sTmpLoanKey.Compare(sLoanKey)!=0 ) continue;

		ids = pParentDM->SetCellData( _T("메일발송") , sYN , i );
		if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		if( nParentColIndex != -1 )
			pParentGrid->SetAt( i , nParentColIndex , sYN );

		return 0;
	
	}

	return 0;

	EFS_END
	return -1;

}


// 메세지 보여주기
INT CBL_LOC_LOAN_MNG_PROC::ShowMailResultMessage( INT nMailMode , INT nMailSendList , INT nDenyReasonStatus , INT nDenyReasonOrder , INT nDenyReasonWorkingStatus )
{

	return 0;

}

// nReason -> 0:대출상태가 예약땜시 혹은 대출상태가 아니라서 , 1:순위가 1이 아니기 땜시 , 2:비치자료가 아니기 땜시 
INT CBL_LOC_LOAN_MNG_PROC::MailAddRecord( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{

	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::AddRecordMailReturnPressing( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{
	EFS_BEGIN


	return 0;

	EFS_END
	return -1;

}

// 예약 자료 반납 확인 메일이다.
INT CBL_LOC_LOAN_MNG_PROC::AddRecordMailReserveBookReturn( INT nMailMode , CLocMailMgrAPI *MailMgr , CESL_DataMgr *pDM , INT nIndex ,  INT &nReason )
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

// 반납 확인 메일 보내기
INT CBL_LOC_LOAN_MNG_PROC::OpenMail(INT nMailMode )
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

// 부모 그리드와 예약 처리 그리드에 메일 발송 여부에 'Y'를 체크한다.
INT CBL_LOC_LOAN_MNG_PROC::SetCheckMailSendYN( CString sLoanKey , CString sYN )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");
	CString sCMAlias = _T("CM_BO_LOC_3240");

	// 현재 사용되고 있는 그리드 얻어오기
	CESL_Grid *pGrid = GetClientGridPtr();
	if(pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SetCheckMailSendYN") );
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SetCheckMailSendYN") );
	// 부모 그리드 얻어오기
	CESL_Grid *pParentGrid = GetParentGridPtr();
	if(pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SetCheckMailSendYN") );
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 ) return 0;

	// 현재 사용되는 DM 얻어오기
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5, _T("SetCheckMailSendYN") );
	// 부모 DM 얻어오기
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("OSetCheckMailSendYN"));

	INT nClientColIndex = -1;
	INT nParentColIndex = -1;
	CString  sParentRowIndex;

	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("메일발송") , nClientColIndex );
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pParentGrid , _T("메일발송") , nParentColIndex );

	CString sTmpLoanKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("대출/반납정보KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		if( sTmpLoanKey.Compare(sLoanKey)!=0 ) continue;

		ids = pDM->GetCellData( _T("부모INDEX") , i , sParentRowIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("OSetCheckMailSendYN"));

		ids = pDM->SetCellData( _T("메일발송") , sYN , i );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		ids = pParentDM->SetCellData( _T("메일발송") , sYN , _ttoi(sParentRowIndex) );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("OSetCheckMailSendYN"));

		if( nClientColIndex != -1 )
			pGrid->SetAt( i , nClientColIndex , sYN );
		if( nParentColIndex != -1 )
			pParentGrid->SetAt( _ttoi(sParentRowIndex) , nParentColIndex , sYN );
	
		return 0;
	}

	return 0;

	EFS_END
	return -1;

}

// 대출 자료 관리 일괄 수정
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()
{
	EFS_BEGIN

	INT ids;

	// 그리드에 선택된 행을 찾아내어 하나씩 DB에 UPDATE
	// DM에도 UPDATE
	// 그리드에도 UPDATE
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyAllLoanInfo") );

	pGrid->SelectMakeList();

	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("변경할 자료를 먼저 선정해주십시오") );
		return 0;
	}

	INT nIndex;
	pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB에 보내고 , DM에 보내고 , 그리드 보여줌
		ids = ModifyAllLoanInfoProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ModifyAllLoanInfo") );
		if( ids > 0 ) return 1;

		pGrid->SelectGetNext();
	}

	ViewMessage( _T("모두 수정되었습니다.") );

	return 0;

	EFS_END
	return -1;

}


// 대출 자료 관리 일괄 수정
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc( INT &nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;
	CString sParentDMAlias;

	ids = GetParentDM( sParentDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	CArray<CString,CString&> sChangeDMColumns;
	CArray<CString,CString&> sChangeDBColumns;
	CArray<CString,CString&> sChangeValues;
	CArray<CString,CString&> sValueTypes;
	CArray<CString,CString&> sColIndexs;

	INT nCnt = 0;
	ids = GetChangeColumns( &sChangeDMColumns , &sChangeDBColumns , &sChangeValues , &sValueTypes , &sColIndexs , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	if( nCnt == 0 ) 
	{
		ViewMessage( _T("수정할 수 있는 항목이 존재하지 않습니다. ") );
		return 1;
	}

	// 수정항목 체크
	INT	i;
	BOOL bChange;
	bChange = FALSE;
	for ( i=0 ; i<nCnt ; i++ )
	{
		if ( !sChangeValues.GetAt(i).IsEmpty() ) 
		{
			bChange = TRUE;
			break;
		}
	}	

	if ( bChange == FALSE )
	{
		ViewMessage( _T("수정할 항목을 1개 이상 입력하여야 합니다.") );
		return 100;
	}

	// DB 작업
	ids = ModifyAllLoanInfoDBProc( pDM , nIndex , &sChangeDBColumns , &sChangeValues , &sValueTypes , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	// DM 작업
	ids = ModifyAllLoanInfoDMProc( pDM , nIndex , &sChangeDMColumns , &sChangeValues , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	// 그리드 작업
	ids = ModifyAllLoanInfoGridProc( nIndex , &sColIndexs , &sChangeValues , nCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoProc()") );

	return 0;

	EFS_END
	return -1;

}

// 대출 자료 관리 일괄 수정시 UPDATE될 DM,DB 필드명과 변경될 값들을 얻어오는 함수
INT CBL_LOC_LOAN_MNG_PROC::GetChangeColumns( CArray<CString,CString&> *sChangeDMColumns , CArray<CString,CString&> *sChangeDBColumns , CArray<CString,CString&> *sChangeValues , CArray<CString,CString&> *sValueTypes , CArray<CString,CString&> *sColIndexs , INT &nCnt )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3260");
	CString sCMAlias = _T("CM_BO_LOC_3260_EDIT_INFO");

	const INT nColumnCnt = 7;
	TCHAR *sColumns[nColumnCnt][5] = 
	{
		{ _T("대출일") , _T("LOAN_DATE") , _T("대출일") , _T("NUMERIC") , _T("-1")} , 
		{ _T("반납예정일") , _T("RETURN_PLAN_DATE") , _T("반납예정일") , _T("NUMERIC") , _T("-1") } , 
		{ _T("반납일") , _T("RETURN_DATE") , _T("반납일") , _T("NUMERIC") , _T("-1") } , 
		{ _T("예약일") , _T("RESERVATION_DATE") , _T("예약일") , _T("NUMERIC") , _T("-1")} , 
		{ _T("예약만기일") , _T("RESERVATION_EXPIRE_DATE") , _T("예약만기일") , _T("NUMERIC") , _T("-1")} , 
		{ _T("연체료") , _T("ARREAR") , _T("연체료") , _T("NUMERIC") , _T("-1")} , 
		{ _T("배상비") , _T("REPARATION") , _T("배상비") , _T("NUMERIC") , _T("-1") } , 
	};

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetChangeColumns") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetChangeColumns") );

	CESL_Control_Element *pControlElement;
	BOOL bIsEnable;
	CString sGetData;

	CString sParentCMAlias;
	CString sParentGridAlias;
	GetParentCMAlias( sParentCMAlias );
	GetParentGridAlias( sParentGridAlias );

	CString sCol;
	CString strTime;
	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		pControlElement = pCM->FindControlElement( sColumns[i][2] );
		if( pControlElement == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetChangeColumns") );

		bIsEnable = (m_pParentMgr->GetDlgItem( pControlElement->CTRL_ID ))->IsWindowEnabled();

		if( !bIsEnable ) continue;

		ids = pCM->GetControlMgrData( sColumns[i][2] , sGetData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("GetChangeColumns") );
		sGetData.TrimLeft(); sGetData.TrimRight();

		sChangeDBColumns->Add( CString(sColumns[i][1]) );
		sChangeDMColumns->Add( CString(sColumns[i][0]) );
		sValueTypes->Add( CString(sColumns[i][3]) );

		strTime.Empty();

		// 2007.06.18 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
		// 시간 추가 됨.
		if(_T("대출일") == CString(sColumns[i][0]))
		{			
			ids = pDM->GetCellData(_T("대출시간"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("GetChangeColumns") );
		}
		else if(_T("반납예정일") == CString(sColumns[i][0]))
		{
//			ids = pDM->GetCellData(_T("반납예정시간"), 0, strTime );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
			strTime.Empty();
		}
		else if(_T("반납일") == CString(sColumns[i][0]))
		{
			ids = pDM->GetCellData(_T("반납시간"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
		}
		else if(_T("예약일") == CString(sColumns[i][0]))
		{
			ids = pDM->GetCellData(_T("예약시간"), 0, strTime );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
		}
		else if(_T("예약만기일") == CString(sColumns[i][0]))
		{
//			ids = pDM->GetCellData(_T("예약만기시간"), 0, strTime );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("GetChangeColumns") );
			strTime.Empty();
		}

		// 2009.09.11 ADD BY PWR : 반납예정일, 예약만기일 시간 23:59:59입력
		if(_T("반납예정일") == CString(sColumns[i][0]) || _T("예약만기일") == CString(sColumns[i][0]))
		{
			if(!sGetData.IsEmpty())
			{
				sGetData = sGetData + _T(" 23:59:59");
			}
		}
		else
		{
			if( strTime.IsEmpty() &&
				!( _T("반납예정일") == CString(sColumns[i][0]) || _T("예약만기일") == CString(sColumns[i][0]) ||
				   _T("연체료") == CString(sColumns[i][0])     || _T("배상비") == CString(sColumns[i][0]) ) )
				strTime = "00:00:00";
			if( !sGetData.IsEmpty() )
			{
				if( strTime.IsEmpty() )
				{
					sGetData = sGetData;
				}
				else
				{
					sGetData = sGetData + " " + strTime;
				}
			}
		}
		// ----------------------------------------------------------------------}}

		sChangeValues->Add( sGetData );

		INT nColIndex = -1;
		CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , sParentCMAlias  , sParentGridAlias , sColumns[i][0] , nColIndex );
		sCol.Format(_T("%d") , nColIndex );
		sColIndexs->Add( sCol );

		nCnt++;
	}
	

	return 0;

	EFS_END
	return -1;

}

// 일괄수정
// DB작업
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoDBProc( CESL_DataMgr *pDM , INT nIndex , CArray<CString,CString&> *sChangeDBColumns , CArray<CString,CString&> *sChangeValues , CArray<CString,CString&> *sValueTypes , INT nCnt )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ModifyAllLoanInfoDBProc");

	// Mobile API 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	pDM->StartFrame();

	pDM->InitDBFieldData();

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// //						_T("WHERE ID = '%s'"), m_strUserID );
// //		pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	// ---------------------------------
	// 대출 공유
	CString sUpdateFields;
	// ---------------------------------

	// 2007.06.18 add by pdw ++++++++++++++++++++++++++++++++++++++++++++
	CString strChangeDate;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;
			
		/*
		"연체료" , ARREAR
		"배상비" , REPARATION
		*/
		if( _T("ARREAR") != sChangeDBColumns->GetAt(i) && _T("REPARATION") != sChangeDBColumns->GetAt(i) )
		{
			strChangeDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"),sChangeValues->GetAt(i));
		}
		else
		{
			strChangeDate.Format(_T("%s"),sChangeValues->GetAt(i));
		}

		pDM->AddDBFieldData( sChangeDBColumns->GetAt(i) , sValueTypes->GetAt(i) , strChangeDate );

		// ---------------------------------
		// 대출 공유
		if( !sUpdateFields.IsEmpty() ) sUpdateFields += _T(",");
		sUpdateFields += sChangeDBColumns->GetAt(i);
		// ---------------------------------
	}

	// 2009.08.05 ADD BY PWR : LAST_WORK
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("대출자료관리_일괄수정"), __WFILE__, __LINE__ ), TRUE );

	CString sRecKey;
	pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );

	// 2009.06.09 UPDATE BY PWR : CO_LOAN_TBL로 이동된 자료일수 있으므로 같이 UPDATE한다.
	CString strQuery, strCnt;
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
	pDM->GetOneValueQuery(strQuery, strCnt, FALSE);
	if(0 < _ttoi(strCnt))
	{
		pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	}
	else
	{
		pMobileApi->MakeUpdateFrame(_T("CO_LOAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	}
	//pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );

	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->EndFrame();

	// 2010.08.05 ADD BY PWR : 통합도서서비스 대출자료관리 일괄수정
	if(_T("Y") == m_pLoan->m_sIsUnityLoanService)
	{
		CString strGetData;
		pDM->GetCellData(_T("통합회원여부"), nIndex, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == strGetData)
		{
			CString strStatus;
			pDM->GetCellData(_T("대출상태"), nIndex, strStatus);
			CString strRegNo;
			pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
			ConvertLoanStatusCodeToDesc(1, strRegNo, strStatus);
			if(_T("3") != strStatus && _T("4") != strStatus)
			{
				pDM->GetCellData(_T("관리구분"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					CESL_DataMgr* pTimeDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3260"));
					if(NULL == pTimeDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					CString strRecKey;
					ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, strRecKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					Writer.addElement(_T("로컬KEY"), strRecKey);
					Writer.addElement(_T("대출도서관"), m_pParentMgr->m_pInfo->LIB_CODE);
					pDM->GetCellData(_T("대출자번호"), nIndex, strGetData);
					Writer.addElement(_T("통합대출자번호"), strGetData);
					CString strLoanDate, strTime;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("대출일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("대출시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strLoanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("대출일"), strLoanDate);
					}
					CString strReturnDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("반납일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("반납시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReturnDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("반납일"), strReturnDate);
					}
					CString strReturnPlanDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("반납예정일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReturnPlanDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("반납예정일"), strReturnPlanDate);
					}
					CString strReservationDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("예약일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						pTimeDM->GetCellData(_T("예약시간"),0,strTime);
						if(strTime.IsEmpty()) strTime = _T("00:00:00");
						strReservationDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("예약일"), strReservationDate);
					}
					CString strReservationExpireDate;
					ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3260_EDIT_INFO"), _T("예약만기일"), strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(!strGetData.IsEmpty())
					{
						strTime = _T("23:59:59");
						strReservationExpireDate.Format(_T("%s %s"), strGetData, strTime);
						Writer.addElement(_T("예약만기일"), strReservationExpireDate);
					}
					CFrameSender Sender(m_pParentMgr->m_pInfo);
					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리-일괄수정"), __WFILE__, __LINE__ ));
				}
			}
		}
	}

	// Mobile API delete
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// 대출 공유 대출/반납 정보 , 대출자 변경시 센터 도서관에 알리기 
	// 2003년 11월 8일 작업
	
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	
	return 0;

EFS_END
return -1;
}

// 일괄 수정
// DM 작업
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoDMProc( CESL_DataMgr *pDM , INT nIndex , CArray<CString,CString&> *sChangeDMColumns , CArray<CString,CString&> *sChangeValues , INT nCnt )
{
	EFS_BEGIN

	INT ids;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;

		ids = pDM->SetCellData( sChangeDMColumns->GetAt(i) ,  sChangeValues->GetAt(i) , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ModifyAllLoanInfoDMProc") );
	}	

	return 0;

	EFS_END
	return -1;

}

// 일괄 수정
// 그리드 작업
INT CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfoGridProc( INT nIndex , CArray<CString,CString&> *nColIndexs , CArray<CString,CString&> *sChangeValues , INT nCnt )
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetParentGridPtr();

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( _ttoi(nColIndexs->GetAt(i)) < 0 ) continue;

		if( sChangeValues->GetAt(i).IsEmpty() )
			continue;
		pGrid->SetAt( nIndex , _ttoi(nColIndexs->GetAt(i)) , sChangeValues->GetAt(i) );
	}	

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentDM( CString &sDMAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_BO");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_NON_DB");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sDMAlias = _T("DM_BO_LOC_3200_SE");
	}
*/
	sDMAlias = _T("DM_BO_LOC_3200");

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentCMAlias( CString &sCMAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
	}
*/
	sCMAlias = _T("CM_BO_LOC_3200");
	return 0;


	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetParentGridAlias( CString &sGridAlias )
{
	EFS_BEGIN

/*	
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sGridAlias = _T("MainBOGrid");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sGridAlias = _T("MainNonDBGrid");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sGridAlias = _T("MainSEGrid");
	}
*/
	sGridAlias = _T("MainGrid");
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::GetCMAlias3320( CString &sCMAlias )
{
	EFS_BEGIN

/*
	CString sPublishForm;

	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetPublishForm") );

	if( sPublishForm.Compare(_T("M")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220");
	}
	else if( sPublishForm.Compare(_T("N")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220_NON_DB");
	}
	else if( sPublishForm.Compare(_T("S")) == 0 )
	{
		sCMAlias = _T("CM_BO_LOC_3220");
	}
*/

	sCMAlias = _T("CM_BO_LOC_3220");

	return 0;


	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_MNG_PROC::ViewDetail()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_3200");
	CString sParentDMAlias = _T("DM_BO_LOC_3200");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCode()") );
	if( pParentDM->GetRowCount() < 0 ) return 0;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::ChangeCode()") );

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	INT nIndex = pGrid->GetIdx();
	pParentDM->GetCellData( _T("종KEY") , nIndex , sSpeciesKey );
	sSpeciesKeyList.AddTail( sSpeciesKey );
	asGridRow.Add( nIndex );

	CString sPublishForm;
	pParentDM->GetCellData( _T("자료구분") , nIndex , sPublishForm );

	

	if( sPublishForm.Compare(_T("단행")) == 0 )
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , 0 );
		Dlg.DoModal();
	}
	else if( sPublishForm.Compare(_T("연속")) == 0 )
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , 0 );
		Dlg.DoModal();		
	}


	sSpeciesKeyList.RemoveAll();
	asGridRow.RemoveAll();

	return 0;

	EFS_END
	return -1;

}

// 예약 자료 순서 재부여
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder()
{
	EFS_BEGIN

	

	CString sDMAlias;

	GetParentDM( sDMAlias );

	ReViewOrder(sDMAlias);

	return 0;

	EFS_END
	return -1;

}

// 예약 자료 순서 재부여
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder( CString sDMAlias )
{
	EFS_BEGIN

	// 김용배 수정
	// 예약자료 순위 가져오는 로직을 query 한번 날려서 가져오도록 수정
	/*
	const INT reservateInfoCnt = 2;
	CString reservateInfoAlias[ reservateInfoCnt ] = {
		_T("대출/반납정보KEY"), _T("대출책KEY") };
	CString reservateInfoData[ reservateInfoCnt ];
	
	RESERVATE_ORDER_LIST ReservationOrderList;
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	INT DMRowCnt = pDM->GetRowCount();
	for( INT i = 0; i < DMRowCnt; i++ )
		if( 3 == _ttoi( pDM->GetCellData( _T("대출상태"), i ) ) )	// 검색된 대출/반납 자료에서 상태가 예약인 자료만 
		{
			pDM->GetCellData( reservateInfoAlias, reservateInfoCnt, reservateInfoData, i );
			// RESERVATE_ORDER_LIST 에 한 레코드를 추가한다
			ReservationOrderList.AddReservateInfo( reservateInfoData[ 0 ], reservateInfoData[ 1 ], i );
		}

	// 예약자료가 하나도 없다면 그냥 통과
	if( !ReservationOrderList.GetInfoCount() )
		return 0;

	// 예약자료가 있다면 DM : DM_BO_LOC_3200 에 예약자료 순위를 넣는다
	if( SetReservationRank( pDM, ReservationOrderList ) )
		return -1;
	*/
	
	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ReViewOrder") );

	CString sStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("대출상태") , i , sStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder") );

		if( !( (sStatus.Compare(_T("3"))==0) || (sStatus.Compare(_T("예약"))==0) || (0 == sStatus.Find(_T("예약("))) ) )
			continue;

		ids = ReViewOrder( pDM , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ReViewOrder") );
	}
	
	
	
	return 0;
	
	EFS_END
	return -1;
	
}

// 예약 자료 순서 재부여
INT CBL_LOC_LOAN_MNG_PROC::ReViewOrder( CESL_DataMgr *pDM , INT nIndex )
{
	EFS_BEGIN

	INT ids = 0;

	CString sBookKey = _T("");
	CString sLoanKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sOrder = _T("");
	CString sSql = _T("");

	// 예약 순위 정해주기
	// 조건 : 책KEY , 대출/반납정보KEY
	ids = pDM->GetCellData( _T("대출책KEY") , nIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sLoanKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("종KEY") , nIndex , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	ids = pDM->GetCellData( _T("권호정보KEY") , nIndex , sVolInfoKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") );

	CString sOrderDMAlias = _T("DM_BO_LOC_3200_RESERVATION_ORDER");

	CESL_DataMgr* pOrderDM = m_pParentMgr->FindDM( sOrderDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ReViewOrder") );
	pOrderDM->FreeData();

	if(sVolInfoKey == _T(""))
	{
		ids = pDM->SetCellData( _T("순위") , _T("Not VolInfo") , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-4 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

		return 0;
	}

	if ( m_pLoan->m_bVolReserve )
	{
		// GET_RESERVATE_RANK 
		// input = 현황키, SpeciesKey, VolInfoKey
		// output = ?/?/? ( rank/지정된책갯수/예약걸린책수 )
		sSql.Format( _T("SELECT GET_RESERVATE_RANK( %s , %s , %s ) FROM DUAL") , sLoanKey , sSpeciesKey , sVolInfoKey );
		ids = pOrderDM->GetOneValueQuery( sSql , sOrder );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-3 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 
	}
	else
	{
		sSql.Format( _T(" STATUS = '3' AND BOOK_KEY = %s") , sBookKey );
		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pOrderDM ,sSql );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-3 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

		CString sOrderLoanKey;
		for( INT i = 0 ; i < pOrderDM->GetRowCount() ; i++ )
		{
			ids = pOrderDM->GetCellData( _T("대출/반납정보KEY") , i , sOrderLoanKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ReViewOrder") );

			if( sLoanKey.Compare( sOrderLoanKey ) == 0 )
				sOrder.Format( _T("%d") , i+1 );
		}
	}

	ids = pDM->SetCellData( _T("순위") , sOrder , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-4 , _T("ReViewOrder( CESL_DataMgr *pDM , INT nIndex )") ); 

	return 0;

	EFS_END
	return -1;

}

/// 무인 반납중인 자료 처리
/// 그리드의 선정된 자료중 상태가 무인 반납중인 자료만 변경
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromStation()
{
	EFS_BEGIN

	INT ids;
	
	// 그리드에 선택된 행을 찾아내어 하나씩 DB에 UPDATE
	// DM에도 UPDATE
	// 그리드에도 UPDATE
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()") );
	
	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();
	
	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("변경할 자료를 먼저 선정해주십시오") );
		return 0;
	}
	
	INT nIndex;
	pGrid->SelectGetHeadPosition();

	INT nReturnCnt = 0;
	for( INT i = 0 ; i < nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB에 보내고 , DM에 보내고 , 그리드 보여줌
		ids = StationReturnProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_LOC_LOAN_MNG_PROC::ModifyAllLoanInfo()") );
		
		if( ids == 0 ) nReturnCnt++;
		pGrid->SelectGetNext();
	}
	
	if( nReturnCnt == 0 )
	{
		ViewMessage( _T("처리할 자료가 존재하지 않습니다.") );
		return 0;
	}
	
	if( nReturnCnt == nSelectCount )
	{
		// 2008.06.04 UPDATE BY PWR : 몇건의 자료가 반납처리 되었는지를 알 수 있도록
		CString strMsg;
		strMsg.Format( _T("[ %d ]건의 자료가 반납처리 되었습니다."), nReturnCnt );
		ViewMessage( strMsg );
	}
	else
	{
		CString sMsg;
		sMsg.Format( _T("처리할 자료 : %d \r\n처리하지 않은 자료(대출상태가 무인반납중이 아닌자료) : %d \r\n처리한 자료 : %d ") 
			, nSelectCount , nSelectCount-nReturnCnt , nReturnCnt );
		ViewMessage( sMsg );
	}
	
	return 0;
	
	EFS_END
	
	return -1;
}

// 18.09.27 JMJ KOL.RED.2018.008
// 책두레 자료가 무인반납기로 반납된 경우 LS_WORK_T01.STATUS가 1로 바뀌어버려
// 통계에서 일반자료반납으로 잡힌다. 성북도서관에서 책두레자료로 잡히도록 요청함.
// 책두레대출 후 책두레 반납되는 자료는 ILL_TRANS_TBL.APL_LIBCODE 가 널이 아니기 때문에
// 이를 확인하여 R로 할지 1로할지 결정한다.
INT CBL_LOC_LOAN_MNG_PROC::DecideLoanStatus( CString strLoanKey, CString& strLoanStatus )
{
	INT ids;
	CString strGetQuery, strTransNo;
	CESL_DataMgr TmpDM;
	
	strGetQuery.Empty();
	strTransNo.Empty();

	TmpDM.SetCONNECTION_INFO(_T(""));

	// 책두레를 사용하지 않는다면 1
	if ( m_pLoan->m_sBookCooperativeSys == _T("N") ) strLoanStatus = _T("1");
	// 책두레를 사용한다면 실제 책두레자료인지 확인
	else {
		strGetQuery.Format( _T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s AND APL_LIBCODE IS NOT NULL"), strLoanKey);
		ids = TmpDM.GetOneValueQuery(strGetQuery, strTransNo);
		if ( ids == -4007 ) strTransNo=_T("");
		else if(0 > ids ) {
			return -100;
		}

		if ( strTransNo.GetLength() > 0 ) strLoanStatus = _T("R");
		else strLoanStatus = _T("1");		
	}	

	return 0;
}

INT CBL_LOC_LOAN_MNG_PROC::StationReturnProc( INT nIndex )
{
EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("StationReturnProc"));
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("StationReturnProc"));

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere;		
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"));
// 		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	CString sStatus;
	pDM->GetCellData( _T("대출상태"), nIndex, sStatus );
	CString sRegNo;
	pDM->GetCellData( _T("등록번호"), nIndex, sRegNo );
	CString strReservationExpireDate;
	pDM->GetCellData( _T("예약만기일"), nIndex, strReservationExpireDate );
	CString strReserveCancelDate;
	pDM->GetCellData( _T("예약취소일"), nIndex, strReserveCancelDate );
	ConvertLoanStatusCodeToDesc( 1, sRegNo, sStatus );

	if( sStatus != _T("5") )	return 1;
		
	// 1. DB작업	
	CString sLoanKey;
	pDM->GetCellData( _T("대출/반납정보KEY"), nIndex, sLoanKey );

	// Mobile API 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr, pDM );
	pMobileApi->StartFrame();
	
	// 18.09.27 JMJ KOL.RED.2018.008
	ids = DecideLoanStatus(sLoanKey, sStatus);
	if ( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));

	//sStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS"), _T("STRING"), sStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sLoanKey );

	CString sPublishForm;
	CString sBookKey;
	CString sWorkingStatus;
	CString sPreWorkingStatus;
	CString sBookTableName;
	pDM->GetCellData( _T("이전자료상태"), nIndex, sPreWorkingStatus );
	pDM->GetCellData( _T("배가상태"), nIndex, sWorkingStatus );
	pDM->GetCellData( _T("대출책KEY"), nIndex, sBookKey );
	pDM->GetCellData( _T("자료구분"), nIndex, sPublishForm );

	// 2010.08.31 UPDATE BY PWR : 무인반납처리시 상태값 정상 적용(WORKING_STATUS:소장, PREV_WORKING_STATUS:무인반납중)
	if(_T("단행") == sPublishForm)
	{
		sBookTableName = _T("BO_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC 무인반납상태로 넣었다간 대출반납-개인별내역화면에서 복원하면 데이터 깨진다.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}
	else if(_T("연속") == sPublishForm)
	{
		sBookTableName = _T("SE_BOOK_TBL");
		CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus, 0);
		CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus, 0);
		if(_T("SEL112N") == sPreWorkingStatus || _T("SEL112N") == sWorkingStatus )
		{
			sWorkingStatus = _T("SEL112N");
		}
		else
		{
			sWorkingStatus = _T("SEL212N");
		}
		//KOL.RED.014 JMC 무인반납상태로 넣었다간 대출반납-개인별내역화면에서 복원하면 데이터 깨진다.
		sPreWorkingStatus = _T("SEL311O");
		//sPreWorkingStatus = _T("SEL316O");
	}
	else if(_T("긴급") == sPublishForm)
	{
		sBookTableName = _T("CO_NON_DB_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC 무인반납상태로 넣었다간 대출반납-개인별내역화면에서 복원하면 데이터 깨진다.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}
	else
	{
		sBookTableName = _T("BO_BOOK_TBL");
		sWorkingStatus = _T("BOL112N");
		//KOL.RED.014 JMC 무인반납상태로 넣었다간 대출반납-개인별내역화면에서 복원하면 데이터 깨진다.
		sPreWorkingStatus = _T("BOL211O");
		//sPreWorkingStatus = _T("BOL216O");
	}	
	pMobileApi->InitDBFieldData();
	// 2010.09.28 ADD BY KSJ : WORKING_STATUS , PREV_WORKING_STATUS 반대로 들어가는 문제 수정
//DEL 	pMobileApi->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sPreWorkingStatus);
//DEL 	pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sWorkingStatus);
	pMobileApi->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus);
	pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus);

// 	if( sPublishForm.Compare(_T("단행")) == 0 )
// 		sBookTableName = _T("BO_BOOK_TBL");
// 	else if( sPublishForm.Compare(_T("연속")) == 0 )
// 		sBookTableName = _T("SE_BOOK_TBL");
// 	else if( sPublishForm.Compare(_T("긴급")) == 0 )
// 		sBookTableName = _T("CO_NON_DB_BOOK_TBL");
// 	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus ,  0);
// 	CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus ,  0);
// 	pMobileApi->InitDBFieldData();
// 	pMobileApi->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), sPreWorkingStatus );
// 	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), sWorkingStatus );
	pMobileApi->MakeUpdateFrame( sBookTableName, _T("REC_KEY"), _T("NUMERIC"), sBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS 변동시 종의 WORKING_STATUS 업데이트 (박종운씨 요청사항)
	CString strSpeciesKey;
	pDM->GetCellData( _T("종KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, sPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2010.10.12 ADD BY KSJ : 통합회원시스템 적용
	CString strKLMemberYN;
	pDM->GetCellData( _T("통합회원여부"), nIndex, strKLMemberYN );
	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
	strKLMemberYN.MakeUpper();
	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
	{
		CString strGetData;
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		ids = pDM->GetCellData(_T("대출자번호"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("통합대출자번호"), strGetData);
		Writer.addElement(_T("대출도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("반납도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
		//DEL Writer.addElement(_T("등록번호"), sRegNo);
		Writer.addElement(_T("로컬KEY"), sLoanKey);
		ids = pDM->GetCellData(_T("반납일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("반납일"), strGetData);
		Writer.addElement(_T("상태"), _T("1"));
		ids = pDM->GetCellData(_T("회원상태"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("회원상태"), strGetData);
		ids = pDM->GetCellData(_T("대출정지일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("대출정지일"), strGetData);
		ids = pDM->GetCellData(_T("대출정지부여일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc"));
		Writer.addElement(_T("대출정지부여일"), strGetData);
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리-무인반납처리"), __WFILE__, __LINE__));		
	}

	// 대출 공유 대출/반납 정보 , 대출자 변경시 센터 도서관에 알리기 
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	// DM작업
	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus,  1);
	
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, sStatus );
	//if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("StationReturnProc") ); 

	ConvertLoanStatusCodeToDesc( 0, sRegNo, sStatus );
	
	pDM->SetCellData(_T("대출상태"), sStatus, nIndex );
	// KOL.RED.014 상태오류 수정.
	pDM->SetCellData(_T("배가상태"), sWorkingStatus, nIndex );
	pDM->SetCellData(_T("이전자료상태"), sPreWorkingStatus, nIndex );
	/*
	pDM->SetCellData(_T("배가상태"), sPreWorkingStatus, nIndex );
	pDM->SetCellData(_T("이전자료상태"), sWorkingStatus, nIndex );*/

	// 그리드 작업	
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출상태") , _T("배가상태") , _T("이전자료상태")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if(pGrid==NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("StationReturnProc")); 
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr, pGrid, sControlAlias, nCnt, nColIndex );
		
	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("StationReturnProc"));
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("StationReturnProc"));
	}

	// 2007.05.07 ADD BY PDW : 예약자료 처리
	// 1. 단행이나 연속만 가능하다.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey;
	CString strVolInfo;
	CString strRegNo;

	pDM->GetCellData( _T("단행/연속구분"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("대출책KEY"), nIndex, strBookKey );
	pDM->GetCellData( _T("권호정보KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("등록번호"), nIndex, strRegNo );
	// 2009.05.22 ADD BY PWR : 부록자료이면 예약처리 안하도록 수정
	CString strAppendixFlag;
	pDM->GetCellData(_T("부록여부"), nIndex, strAppendixFlag);
	if(strAppendixFlag != _T("B"))
	{
		return 0;
	}

	// 2. 예약자료가 있는지 확인한다.
	if( strSpeciesFlag == _T("MO") )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( strSpeciesFlag == _T("SE") )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	if( !strTableName.GetLength() ) return 0;

	if( !m_pLoan->m_bVolReserve )
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				         _T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
						 _T("AND BB.REC_KEY=%s AND CL.STATUS='3'"), strTableName, strBookKey );
	}
	else
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				         _T("WHERE CL.SPECIES_KEY=BB.SPECIES_KEY ")
						 _T("AND BB.SPECIES_KEY=%s ")
						 _T("AND ((CL.VOL_INFO=BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL)) ")
						 _T("AND CL.STATUS='3'"), strTableName, strSpeciesKey );
	}

	CString strResult;
	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -6, _T("StationReturnProc"));

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), strRegNo );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd, strMsg, _T("예약 처리"), MB_YESNO|MB_ICONQUESTION ) )
		{
			if( m_pLoan->m_bVolReserve )
			{ // 한줄서기예약
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr, m_pLoanShareManager, m_pLoan, 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey, 1 );
				
				// 2009.02.03 ADD BY KSJ : 한줄서기예약 처리방식 변경
				Dlg.m_strReturnBookKey = strBookKey;
				// 반납 된 책의 REG_NO
				Dlg.m_strReturnBookRegNo = sRegNo;
				// 0:예약상태, 1:예약처리
				Dlg.m_nReserveInfoMode = 1;
				Dlg.DoModal();
			}
			else
			{ // 일반예약
				CBO_LOC_3170 Dlg( m_pParentMgr, m_pLoanShareManager, m_pLoan, 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey, 1 );
				Dlg.DoModal();				
			}
		}
	}

	return 0;

EFS_END
return -1;
}

// 김용배 추가
// 예약자료 우선순위 쿼리 한방으로 끝내기!!!
INT CBL_LOC_LOAN_MNG_PROC::SetReservationRank( CESL_DataMgr *pLoanDM, RESERVATE_ORDER_LIST &ReservationOrderList )
{
	CArray< CString, CString >bookKeyList;
	ReservationOrderList.GetBookKeyList( bookKeyList );

	// LH_STORAGE_V01 에서 검색된 예약자료의 BOOK_KEY를 포함하는 모든 레코드를 가져오기 위한 QUERY 를 만든다.
	// ORDER BY BOOK_KEY, RESERVATION_DATE 이기 때문에 이전 BOOK_KEY 와 현재 BOOK_KEY 를 체크함으로써 우선순위 초기화를 한다. 
	// 또한 위로 부터 우선순위는 오름차순이다. 
	CString query = _T("SELECT REC_KEY, BOOK_KEY FROM LS_WORK_T01 ");
	CString where = _T("WHERE STATUS = '3' AND BOOK_KEY IN( ");
	for( INT i = 0; i < bookKeyList.GetSize(); i++ )
		where += ( bookKeyList.GetAt( i ) + _T(", ") );
	if( _T(", ") == where.Right( 2 ) )
		where = where.Left( where.GetLength() - 2 );
	query += ( where + _T(" ) ORDER BY BOOK_KEY, RESERVATION_DATE") );
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( pLoanDM->CONNECTION_INFO );
	
	dm.RestructDataManager( query );
	INT cnt = dm.GetRowCount();
	INT reservationInfoCnt = ReservationOrderList.GetInfoCount();
	CString loanKey, bookKey, pastBookKey;
	INT rank = 0;
	for( i = 0; i < cnt; i++ )
	{
		loanKey = dm.GetCellData( i, 0 );
		bookKey = dm.GetCellData( i, 1 );
		
		// 이전 BOOK_KEY 와 현재 BOOK_KEY 가 같지 않다면 우선순위는 1 로 초기화 된다.
		if( pastBookKey != bookKey )
			rank = 1;
		else
			rank++;	// 계속 같은 책이라면 우선순위는 증가한다. 

		// 이전에 검색된 모든 예약자료의 개수만큼 돌면서 대출키가 같은지 검사한다. 
		for( INT j = 0; j < reservationInfoCnt; j++ )
			// 대출키가 같다면 현재 우선순위가 적용된다. 
			if( loanKey == ReservationOrderList.GetLoanKey( j ) )
			{
				CString sRank;
				sRank.Format( _T("%d"), rank );
				pLoanDM->SetCellData( _T("순위"), sRank, ReservationOrderList.GetDMRow( j ) );
			}

		pastBookKey = bookKey;
	}

	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  타관반납자료 처리
/////////////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()
{
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	INT		nGridSelectCount;
	INT		nSelectIndex;
	INT		nSuccessCount = 0;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_Grid* pMainGrid = GetParentGridPtr();
	if( !pMainGrid ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

	pMainGrid->SelectMakeList();

	nGridSelectCount = pMainGrid->SelectGetCount();

	if( 1 > nGridSelectCount )
	{
		ViewMessage( _T("변경할 자료를 먼저 선정해주십시오") );
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	pMainGrid->SelectGetHeadPosition();

	for( INT i=0;i<nGridSelectCount;i++ )
	{
		nSelectIndex = pMainGrid->SelectGetIdx();

		ids = OtherManageCodeReturnProc( nSelectIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

		if( 0 == ids )
		{
			nSuccessCount++;
		}

		pMainGrid->SelectGetNext();
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	if( 0 == nSuccessCount )
	{
		ViewMessage( _T("처리할 자료가 존재하지 않습니다.") );
		return 0;
	}

	if( nSuccessCount == nGridSelectCount )
	{
		//ViewMessage( _T("모두 수정되었습니다.") );

		//++2008.03.16 ADD BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 분관반납처리시 "모두 수정되었습니다." -> "?건의 자료가 반납처리 되었습니다." 로 변경
		CString strMsg;
		strMsg.Format( _T("[ %d ]건의 자료가 반납처리 되었습니다."), nSuccessCount );
		ViewMessage( strMsg );
		//--2008.03.16 ADD BY PWR -----------------------------------------------------------}}
	}
	else
	{
		CString strMsg;
		strMsg.Format( _T("처리할 자료 : %d \r\n")
			           _T("처리하지 않은 자료(대출상태가 분관반납중이 아닌자료) : %d \r\n")
					   _T("처리한 자료 : %d "), 
					     nGridSelectCount , nGridSelectCount-nSuccessCount , nSuccessCount );
		ViewMessage( strMsg );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  타관반납자료 건별처리
//////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc(INT nIndex)
{
	//////////////////////////////////////////////////////////////////
	// Declare Variable
	//////////////////////////////////////////////////////////////////
	INT		ids;
	CString strDMAlias;
	CString strLoanStatus;
	CString strRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	CString strLoanKey;

	CString strBookKey;
	CString strPublishForm;	
	CString strWorkingStatus;
	CString strPreWorkingStatus;
	CString strBookTableName;

	CString strFuncName = _T("OtherManageCodeReturnProc");

	//////////////////////////////////////////////////////////////////
	// 1. 데이터 구성
	//////////////////////////////////////////////////////////////////
	ids = GetParentDM( strDMAlias );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , strFuncName);

	CESL_DataMgr* pDM = m_pParentMgr->FindDM( strDMAlias );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , strFuncName);

	ids = pDM->GetCellData( _T("대출상태") , nIndex , strLoanStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName);

	ids = pDM->GetCellData( _T("등록번호") , nIndex , strRegNo );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
	
	ConvertLoanStatusCodeToDesc( 1, strRegNo, strLoanStatus );

	if( _T("6") != strLoanStatus )
	{
		// 타관대출상태가 아닌 자료는 취소
		return 1;
	}

	// 2009.10.26 ADD BY PWR : 타관 이력 분관반납처리 안되도록
	CString strManageCode;
	ids = pDM->GetCellData(_T("관리구분"), nIndex, strManageCode);
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	}
	if(strManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
	{
		return 1;
	}

	ids = pDM->GetCellData( _T("예약만기일") , nIndex , strReservationExpireDate );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);			
	
	ids = pDM->GetCellData( _T("예약취소일") , nIndex , strReserveCancelDate );	
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , strLoanKey );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);

	ids = pDM->GetCellData( _T("이전자료상태") , nIndex , strPreWorkingStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , strFuncName);
	
	ids = pDM->GetCellData( _T("배가상태") , nIndex , strWorkingStatus );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , strFuncName);

	ids = pDM->GetCellData( _T("대출책KEY") , nIndex , strBookKey );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , strFuncName);

	ids = pDM->GetCellData( _T("자료구분") , nIndex , strPublishForm );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , strFuncName);

	if( strPublishForm.Compare(_T("단행")) == 0 )
	{
		strBookTableName = _T("BO_BOOK_TBL");
	}
	else if( strPublishForm.Compare(_T("연속")) == 0 )
	{
		strBookTableName = _T("SE_BOOK_TBL");
	}
	else if( strPublishForm.Compare(_T("긴급")) == 0 )
	{
		strBookTableName = _T("CO_NON_DB_BOOK_TBL");
	}

	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  0);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  0);

	//////////////////////////////////////////////////////////////////
	// 2. DB Update
	//////////////////////////////////////////////////////////////////
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	// 대출상태
	strLoanStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS") , _T("STRING") , strLoanStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );

	// 책상태
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , strPreWorkingStatus );
	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , strWorkingStatus );
	pMobileApi->MakeUpdateFrame( strBookTableName , _T("REC_KEY") , _T("NUMERIC") , strBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS 변동시 종의 WORKING_STATUS 업데이트 (박종운씨 요청사항)
	CString strSpeciesKey;
	pDM->GetCellData( _T("종KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, strPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , strFuncName);

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2013.02.05 DEL BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 14회 (통합도서서비스에서의 분관반납기능사용)
// 	// 2010.10.12 ADD BY KSJ : 통합회원시스템 적용
// 	CString strKLMemberYN;
// 	pDM->GetCellData( _T("통합회원여부"), nIndex, strKLMemberYN );
// 	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
// 	strKLMemberYN.MakeUpper();
// 	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
// 	{
// 		CString strGetData;
// 		CFrameWriter Writer(m_pParentMgr->m_pInfo);
// 		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
// 		Writer.addRecord();
// 		ids = pDM->GetCellData(_T("대출자번호"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("통합대출자번호"), strGetData);
// 		Writer.addElement(_T("대출도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
// 		Writer.addElement(_T("반납도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
// 		//DEL Writer.addElement(_T("등록번호"), sRegNo);
// 		Writer.addElement(_T("로컬KEY"), strLoanKey);
// 		ids = pDM->GetCellData(_T("반납일"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("반납일"), strGetData);
// 		Writer.addElement(_T("상태"), _T("1"));
// 		ids = pDM->GetCellData(_T("회원상태"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("회원상태"), strGetData);
// 		ids = pDM->GetCellData(_T("대출정지일"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("대출정지일"), strGetData);
// 		ids = pDM->GetCellData(_T("대출정지부여일"), nIndex, strGetData);
// 		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
// 		Writer.addElement(_T("대출정지부여일"), strGetData);
// 		CFrameSender Sender(m_pParentMgr->m_pInfo);
// 		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리-분관반납처리"), __WFILE__, __LINE__));		
// 	}

	// 센터업로드
	if( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format(_T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL WHERE ID = '%s'"), m_strUserID );
// //
// //		ids = pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"));		
// //		}
// //		else
// //		{
// //			strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 
// 		ids = pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	//////////////////////////////////////////////////////////////////
	// 3. Display
	//////////////////////////////////////////////////////////////////
	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  1);

	// 2009.02.24 UPDATE BY PWR : 예약취소일때만 
	if(strLoanStatus==_T("4"))
	{
		ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , strFuncName );
	}
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
	//if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()"));

	ConvertLoanStatusCodeToDesc( 0, strRegNo, strLoanStatus );

	ids = pDM->SetCellData( _T("대출상태") , strLoanStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -16 , strFuncName);
	
	ids = pDM->SetCellData( _T("배가상태") , strPreWorkingStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -17 , strFuncName);
	
	ids = pDM->SetCellData( _T("이전자료상태") , strWorkingStatus , nIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -18 , strFuncName);

	// 그리드 작업
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출상태") , _T("배가상태") , _T("이전자료상태")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName); 
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );
		
	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( strDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);
	}
	
	// 2007.05.07 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	예약자료 처리
	*/
	// 1. 단행이나 연속만 가능하다.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey2;
	CString strVolInfo;
	CString strRegNo2;

	pDM->GetCellData( _T("단행/연속구분"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("대출책KEY"), nIndex, strBookKey2 );
	pDM->GetCellData( _T("권호정보KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("등록번호"), nIndex, strRegNo2 );
	// 2009.05.13 ADD BY PWR : 부록자료이면 예약처리 안하도록 수정
	CString strAppendixFlag;
	pDM->GetCellData(_T("부록여부"), nIndex, strAppendixFlag);
	if(strAppendixFlag != _T("B"))
	{
		return 0;
	}

	// 2. 예약자료가 있는지 확인한다.
	//    쿼리 확인 한줄서기와 일반예약은 쿼리가 틀리다.
	if( strSpeciesFlag == _T("MO") )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( strSpeciesFlag == _T("SE") )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	
	if( !strTableName.GetLength() ) return 0;

//	{
		if( !m_pLoan->m_bVolReserve )
		{
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.BOOK_KEY=BB.REC_KEY ")
							   _T(" AND BB.REC_KEY = %s AND CL.STATUS = '3'"), strTableName, strBookKey2 );
		}
		else
		{
			/*
			SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE SPECIES_KEY = SPECIES_KEY 
  AND ( (CL2.VOL_INFO = BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) 
  AND STATUS = '3';
			*/
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.SPECIES_KEY = BB.SPECIES_KEY ")
							   _T(" AND BB.SPECIES_KEY = %s ")
							   _T(" AND ( (CL.VOL_INFO = BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) ")
							   _T(" AND CL.STATUS ='3'"), strTableName, strSpeciesKey );
		}
//	}

	CString strResult;

	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), strRegNo2 );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd , strMsg , _T("예약 처리") , MB_YESNO | MB_ICONQUESTION ) )
		{
			if( !m_pLoan->m_bVolReserve )
			{				
				CBO_LOC_3170 Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
				
			}
			else if( m_pLoan->m_bVolReserve )
			{
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
			}
		}
	}
	// ---------------------------------------------------------------------}}

	return 0;
}

// 2008.06.02 ADD BY PWR
/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  타관반납자료 처리
/////////////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherLibCode()
{
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	INT		nGridSelectCount;
	INT		nSelectIndex;
	INT		nSuccessCount = 0;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_Grid* pMainGrid = GetParentGridPtr();
	if( !pMainGrid ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

	pMainGrid->SelectMakeList();

	nGridSelectCount = pMainGrid->SelectGetCount();

	if( 1 > nGridSelectCount )
	{
		ViewMessage( _T("변경할 자료를 먼저 선정해주십시오") );
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	pMainGrid->SelectGetHeadPosition();

	for( INT i = 0; i < nGridSelectCount; i++ )
	{
		nSelectIndex = pMainGrid->SelectGetIdx();

		ids = OtherLibCodeReturnProc( nSelectIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -2, _T("CBL_LOC_LOAN_MNG_PROC::ReturnFromOtherManageCode()") );

		if( 0 == ids )
		{
			nSuccessCount++;
		}

		pMainGrid->SelectGetNext();
	}

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	if( 0 == nSuccessCount )
	{
		ViewMessage( _T("처리할 자료가 존재하지 않습니다.") );
		return 0;
	}

	if( nSuccessCount == nGridSelectCount )
	{
		CString strMsg;
		strMsg.Format( _T("[ %d ]건의 자료가 반납처리 되었습니다."), nSuccessCount );
		ViewMessage( strMsg );
	}
	else
	{
		CString strMsg;
		strMsg.Format( _T("처리할 자료 : %d \r\n")
			           _T("처리하지 않은 자료(대출상태가 타관반납중이 아닌자료) : %d \r\n")
					   _T("처리한 자료 : %d "), 
					     nGridSelectCount , nGridSelectCount-nSuccessCount , nSuccessCount );
		ViewMessage( strMsg );
	}

	return 0;
}

// 2008.06.02 ADD BY PWR
//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  타관반납자료 건별처리
//////////////////////////////////////////////////////////////////////
INT CBL_LOC_LOAN_MNG_PROC::OtherLibCodeReturnProc(INT nIndex)
{
	//////////////////////////////////////////////////////////////////
	// Declare Variable
	//////////////////////////////////////////////////////////////////
	INT		ids;
	CString strDMAlias;
	CString strLoanStatus;
	CString strRegNo;
	CString strReservationExpireDate;
	CString strReserveCancelDate;
	CString strLoanKey;

	CString strBookKey;
	CString strPublishForm;	
	CString strWorkingStatus;
	CString strPreWorkingStatus;
	CString strBookTableName;

	CString strFuncName = _T("OtherLibCodeReturnProc");

	//////////////////////////////////////////////////////////////////
	// 1. 데이터 구성
	//////////////////////////////////////////////////////////////////
	ids = GetParentDM( strDMAlias );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1010 , strFuncName);

	CESL_DataMgr* pDM = m_pParentMgr->FindDM( strDMAlias );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = pDM->GetCellData( _T("대출상태"), nIndex, strLoanStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1030 , strFuncName);

	ids = pDM->GetCellData( _T("등록번호"), nIndex, strRegNo );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1040 , strFuncName);
	
	ConvertLoanStatusCodeToDesc( 1, strRegNo, strLoanStatus );

	if( _T("7") != strLoanStatus )
	{
		// 타관대출상태가 아닌 자료는 취소
		return 1;
	}

	// 2009.10.26 ADD BY PWR : 타관 이력 타관반납처리 안되도록
	CString strManageCode;
	ids = pDM->GetCellData(_T("관리구분"), nIndex, strManageCode);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	if(strManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE) return 1;

	ids = pDM->GetCellData( _T("예약만기일"), nIndex, strReservationExpireDate );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1050 , strFuncName);
	
	ids = pDM->GetCellData( _T("예약취소일"), nIndex, strReserveCancelDate );	
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1060 , strFuncName);

	ids = pDM->GetCellData( _T("대출/반납정보KEY"), nIndex, strLoanKey );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1070 , strFuncName);

	ids = pDM->GetCellData( _T("이전자료상태"), nIndex, strPreWorkingStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1080 , strFuncName);
	
	ids = pDM->GetCellData( _T("배가상태"), nIndex, strWorkingStatus );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1090 , strFuncName);

	ids = pDM->GetCellData( _T("대출책KEY"), nIndex , strBookKey );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1100, strFuncName);

	ids = pDM->GetCellData( _T("자료구분"), nIndex, strPublishForm );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1110 , strFuncName);

	if( strPublishForm.Compare( _T("단행") ) == 0 )
	{
		strBookTableName = _T("BO_BOOK_TBL");
	}
	else if( strPublishForm.Compare( _T("연속") ) == 0 )
	{
		strBookTableName = _T("SE_BOOK_TBL");
	}
	else if( strPublishForm.Compare( _T("긴급") ) == 0 )
	{
		strBookTableName = _T("CO_NON_DB_BOOK_TBL");
	}

	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  0);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  0);

	//////////////////////////////////////////////////////////////////
	// 2. DB Update
	//////////////////////////////////////////////////////////////////
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	// 대출상태
	strLoanStatus = _T("1");
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("STATUS"), _T("STRING"), strLoanStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );

	// 책상태
	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), strPreWorkingStatus );
	pMobileApi->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), strWorkingStatus );
	pMobileApi->MakeUpdateFrame( strBookTableName, _T("REC_KEY"), _T("NUMERIC"), strBookKey );

	// 2009.03.11 ADD BY CJY : WORKING_STATUS 변동시 종의 WORKING_STATUS 업데이트 (박종운씨 요청사항)
	CString strSpeciesKey;
	pDM->GetCellData( _T("종KEY"), nIndex, strSpeciesKey );	
	AddIdxQueryFrame(pMobileApi, strPublishForm, strSpeciesKey );

	ids = pMobileApi->SendFrame();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1120 , strFuncName);

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	// 2010.10.12 ADD BY KSJ : 통합회원시스템 적용
	CString strKLMemberYN;
	pDM->GetCellData( _T("통합회원여부"), nIndex, strKLMemberYN );
	strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
	strKLMemberYN.MakeUpper();
	if(_T("Y")==m_pLoan->m_sIsUnityLoanService && _T("Y")==strKLMemberYN)
	{
		CString strGetData;
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		ids = pDM->GetCellData(_T("대출자번호"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("통합대출자번호"), strGetData);
		Writer.addElement(_T("대출도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("반납도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
		//DEL Writer.addElement(_T("등록번호"), sRegNo);
		Writer.addElement(_T("로컬KEY"), strLoanKey);
		ids = pDM->GetCellData(_T("반납일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("반납일"), strGetData);
		Writer.addElement(_T("상태"), _T("1"));
		ids = pDM->GetCellData(_T("회원상태"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("회원상태"), strGetData);
		ids = pDM->GetCellData(_T("대출정지일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("대출정지일"), strGetData);
		ids = pDM->GetCellData(_T("대출정지부여일"), nIndex, strGetData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, strFuncName);
		Writer.addElement(_T("대출정지부여일"), strGetData);
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("대출자료관리-타관반납처리"), __WFILE__, __LINE__));		
	}

	// 센터업로드
	if( TRUE == m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere, strManageCode;		
// 
// //		strWhere.Format( _T("SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL WHERE ID='%s'"), m_strUserID );
// //
// //		ids = pDM->GetOneValueQuery( strWhere, strManageCode, FALSE );
// //		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -13, _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()") );
// //
// //		if ( strManageCode == _T("") )
// //		{
// //			strWhere.Format( _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL") );		
// //		}
// //		else
// //		{
// //			strWhere.Format( _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE ='%s'"), strManageCode);		
// //		}
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"));
// 		ids = pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		if( 0 > ids ) 
// 		{
// 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1130 , _T("OtherLibCodeReturnProc") );
// 		}
		m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	}

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS");				
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	//////////////////////////////////////////////////////////////////
	// 3. Display
	//////////////////////////////////////////////////////////////////
	CLocCommonAPI::GetWorkingStatusDescToCode(strPreWorkingStatus ,  1);
	CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus ,  1);

	// 2009.02.24 UPDATE BY PWR : 예약취소일때만 
	if(strLoanStatus==_T("4"))
	{
		ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1140 , strFuncName);
	}
	//ids = ConvertLoanStatusCodeToDesc(strReservationExpireDate, strReserveCancelDate, strLoanStatus );
	//if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("CBL_LOC_LOAN_MNG_PROC::OtherManageCodeReturnProc()") );

	ConvertLoanStatusCodeToDesc( 0, strRegNo, strLoanStatus );

	ids = pDM->SetCellData( _T("대출상태"), strLoanStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1150 , strFuncName);
	
	ids = pDM->SetCellData( _T("배가상태") , strPreWorkingStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1160 , strFuncName);
	
	ids = pDM->SetCellData( _T("이전자료상태") , strWorkingStatus , nIndex );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1170 , strFuncName);

	// 그리드 작업
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("대출상태"), _T("배가상태"), _T("이전자료상태")
	};
	
	INT nColIndex[nCnt] = 
	{
		-1 , -1 , -1
	};
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if(NULL==pGrid) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1180 , strFuncName);
	
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sControlAlias , nCnt , nColIndex );
		
	CString sGetData[nCnt];
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( strDMAlias , sControlAlias[i] , sGetData[i] , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1190 , strFuncName);
				
		if( nColIndex[i] == -1 ) continue;
		
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sGetData[i] );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1200 , strFuncName);
	}
	
	// 2007.05.07 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	예약자료 처리
	*/
	// 1. 단행이나 연속만 가능하다.
	CString strSpeciesFlag;
	CString strQuery;
	CString strTableName;
	CString strBookKey2;
	CString strVolInfo;
	CString strRegNo2;

	pDM->GetCellData( _T("단행/연속구분"), nIndex, strSpeciesFlag );
	pDM->GetCellData( _T("대출책KEY"), nIndex, strBookKey2 );
	pDM->GetCellData( _T("권호정보KEY"), nIndex, strVolInfo );
	pDM->GetCellData( _T("등록번호"), nIndex, strRegNo2 );

	// 2. 예약자료가 있는지 확인한다.
	//    쿼리 확인 한줄서기와 일반예약은 쿼리가 틀리다.
	if( _T("MO") == strSpeciesFlag )
	{
		strTableName = _T("BO_BOOK_TBL");
	}
	else if( _T("SE") == strSpeciesFlag )
	{
		strTableName = _T("SE_BOOK_TBL");
	}
	
	if( TRUE != strTableName.GetLength() ) 
	{
		return 0;
	}

//	{
		if( TRUE != m_pLoan->m_bVolReserve )
		{
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.BOOK_KEY=BB.REC_KEY ")
							 _T(" AND BB.REC_KEY = %s AND CL.STATUS = '3'"), strTableName, strBookKey2 );
		}
		else
		{
			/*
			SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE SPECIES_KEY = SPECIES_KEY 
  AND ( (CL2.VOL_INFO = BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) 
  AND STATUS = '3';
			*/
			strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, %s BB ")
				             _T(" WHERE CL.SPECIES_KEY = BB.SPECIES_KEY ")
							 _T(" AND BB.SPECIES_KEY = %s ")
							 _T(" AND ( (CL.VOL_INFO = BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) ")
							 _T(" AND CL.STATUS ='3'"), strTableName, strSpeciesKey );
		}
//	}

	CString strResult;

	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1210 , strFuncName);

	if( 0 < _ttoi(strResult) )
	{
		CString strMsg;
		strMsg.Format( _T("[ %s ]는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), strRegNo2 );
		if( IDYES == MessageBox( m_pParentMgr->m_hWnd , strMsg , _T("예약 처리") , MB_YESNO | MB_ICONQUESTION ) )
		{
			if( TRUE != m_pLoan->m_bVolReserve )
			{				
				CBO_LOC_3170 Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
				
			}
			else if( TRUE == m_pLoan->m_bVolReserve )
			{
				CBO_LOC_3170_ReserveInfo Dlg( m_pParentMgr , m_pLoanShareManager , m_pLoan , 0 );
				Dlg.pMain = m_pParentMgr->pMain;
				Dlg.pParentMgr = (CESL_Mgr*)m_pParentMgr;
				Dlg.m_pInfo = m_pParentMgr->m_pInfo;
				Dlg.SetInfo(strSpeciesKey, strVolInfo, strSpeciesFlag, strBookKey2, 1 );
				Dlg.DoModal();
			}
		}
	}
	// ---------------------------------------------------------------------}}

	return 0;
}

/// 예약대출 상태변경
/// nMode : 0  >> 신청상태로 변경 'Y'
///       : 1  >> 접수상태로 변경 'O'
INT CBL_LOC_LOAN_MNG_PROC::ChangeNightLoanStatus(INT nMode)
{
EFS_BEGIN

	INT ids;
	
	CESL_Grid *pGrid = GetParentGridPtr();
	if( NULL == pGrid )
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("ChangeNightLoanStatus") );
	}

	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount < 1 ) 
	{
		ViewMessage( _T("변경할 자료를 먼저 선정해주십시오") );
		return 0;
	}

	INT nIndex;
	pGrid->SelectGetHeadPosition();

	INT		i , nReturnCnt = 0;
	for( i=0 ; i<nSelectCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		// DB에 보내고 , DM에 보내고 
		ids = ChangeNightLoanStatusProc( nMode, nIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("ChangeNightLoanStatus") );
		}
		
		if( 0 == ids ) 
		{
			nReturnCnt++;
		}

		pGrid->SelectGetNext();
	}
	
	if( 0 == nReturnCnt )
	{
		ViewMessage( _T("처리할 자료가 존재하지 않습니다.") );
		return 0;
	}
	
	if( nReturnCnt == nSelectCount )
	{
		ViewMessage( _T("모두 수정되었습니다.") );
	}
	else
	{
		CString sMsg;
		sMsg.Format( _T("처리할 자료 : %d \r\n처리하지 않은 자료(야간대출예약이 아닌 자료) : %d \r\n처리한 자료 : %d ") 
			, nSelectCount , nSelectCount-nReturnCnt , nReturnCnt );
		ViewMessage( sMsg );
	}
	
	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_MNG_PROC::ChangeNightLoanStatusProc(INT nMode, INT nIndex)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias;
	ids = GetParentDM( sDMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("ChangeNightLoanStatusProc"));
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("ChangeNightLoanStatusProc"));
	}

	CString sStatus;
	pDM->GetCellData(_T("야간대출예약") , nIndex , sStatus );
	if( _T("N") == sStatus )
	{
		return 1;	
	}

	// 1. DB작업
	
	CString sLoanKey;

	pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sLoanKey );

	// Mobile API 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	
	pMobileApi->StartFrame();

	if( nMode == 0 )
	{
		sStatus = _T("Y");
	}
	else
	{
		sStatus = _T("O");
	}

	pMobileApi->InitDBFieldData();
	pMobileApi->AddDBFieldData( _T("NIGHT_RESERVATION_LOAN") , _T("STRING") , sStatus );
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sLoanKey );
	ids = pMobileApi->SendFrame();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd , -1030 , _T("ChangeNightLoanStatusProc"));
	}
	
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	pDM->SetCellData(_T("야간대출예약") , sStatus , nIndex );
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_MNG_PROC::AddIdxQueryFrame( CKolas2up_MobileApi *pMobileApi, CString strPublishFormCode, CString sSpeciesKey )
{
EFS_BEGIN

	/// 이동도서관일 경우는 하지 않는다.
	if( ( 10000 == m_pParentMgr->m_pInfo->MODE ) || ( 30000 == m_pParentMgr->m_pInfo->MODE ) )
	{
		return 0;
	}

	CString sIDXTblName, sProcName;

	if( _T("단행") == strPublishFormCode || _T("MO") == strPublishFormCode)
	{
		sIDXTblName = _T("IDX_BO_TBL");			
		sProcName = _T("TESTWORKSTATUS");
	}
	else if( _T("연속") == strPublishFormCode || _T("SE") == strPublishFormCode)
	{
		sIDXTblName = _T("IDX_SE_TBL");			
		sProcName = _T("TESTSEWORKSTATUS");
	}
	else
	{
		return -1;
	}

	CString sQuery;
	sQuery.Format( _T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;") ,sIDXTblName, sProcName , sSpeciesKey , sSpeciesKey);	
	pMobileApi->AddFrame( sQuery );

	return 0;

EFS_END
return -1;
}