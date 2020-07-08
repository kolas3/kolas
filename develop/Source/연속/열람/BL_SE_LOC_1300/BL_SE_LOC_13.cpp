// BL_SE_LOC_13.cpp: implementation of the CBL_SE_LOC_13 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_SE_LOC_13.h"

// SanFile을 오픈하기 위해
#include "..\..\..\단행\열람\장서관리\OpenSanFile\\InputSanFileDlg.h"

// 이동 도서관을 위해
#include "Kolas2up_MobileApi.h"

// 인덱스 잡아내기 위해
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"


//#include _T("..\\..\\..\\공통\\Common_Data_Mgr\\Data_Mgr.h")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_SE_LOC_13::CBL_SE_LOC_13( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_pLocCommonAPI = new CLocCommonAPI(m_pParentMgr);
}

CBL_SE_LOC_13::~CBL_SE_LOC_13()
{
	if( m_pLocCommonAPI != NULL ) delete m_pLocCommonAPI;
	m_pLocCommonAPI = NULL;
}

INT CBL_SE_LOC_13::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;
	if( sPathAlias.Compare( _T("배가자료관리_검색") ) == 0 )
	{
		ids = SearchSE_LOC_1310();
		if( ids == 1 ) return -1;
	}
	if( sPathAlias.Compare( _T("RFID검색") ) == 0 )
	{
		ids = SearchSE_LOC_1310FromRFID();
		if( ids == 1 ) return -1;
	}	
	else if( sPathAlias.Compare( _T("배가자료관리_파일로검색") ) == 0 )
	{
		ids = SearchFileSE_LOC_1310();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_코드일괄변경") ) == 0 )
	{
		ids = ChangeCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_낱권변경") ) == 0 )
	{
		ids = ChangeShelf(0);
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_일괄변경") ) == 0 )
	{
		ids = ChangeShelf(1);
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_이용제한구분종변경") ) == 0 )
	{
		ids = ChangeSpeciesCode();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_상세보기") ) == 0 )
	{
		ids = DetailView();
		if( ids == 1 ) return -1;
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


//-----------------------------------------------------------------------------------------
// 이곳부터는 배가 관리에 사용되는 함수들
//-----------------------------------------------------------------------------------------
INT CBL_SE_LOC_13::SearchSE_LOC_1310()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	// 조건절 생성
	CString sWhere;
	ids = GetSqlSE_LOC_1310( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );
	
//	ids = pDM->RefreshDataManager( sWhere );
	
	DWORD START, FINISH;
	START = GetTickCount();
	
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - 연속, 배가일자기록 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );
	
	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	INT nRowCount = pDM->GetRowCount();

	CString sMsg;
	sMsg.Format( _T("책수 : %d") , nRowCount );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));

	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	return 0;

	EFS_END
	return -1;

}


INT CBL_SE_LOC_13::SearchSE_LOC_1310FromRFID()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	// 조건절 생성
	CString sWhere;
	GetTempData( _T("WHERE") , sWhere );
	
	//	ids = pDM->RefreshDataManager( sWhere );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	INT nRowCount = pDM->GetRowCount();
	if( nRowCount == 0 )
		AfxMessageBox( _T("검색된 자료가 존재하지 않습니다.") );
	CString sMsg;
	sMsg.Format( _T("책수 : %d") , nRowCount );
	m_pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));
	
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	return 0;
	
	EFS_END
	return -1;
	
}


/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-14 ~ 2005-12-14
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   함수설명   ] 
      [연속-열람] 장서관리 -검색화면 입력 값으로 조건절 만들기

   [  PARAMETERS  ]
      sWhere : [OUT] 조건절
   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패

   [   미니스펙   ]
      1. 검색화면에 입력된 값 가져오기 
	  2. 기본 조건절 생성하기 	                      
	  3. 추가 조건절 생성하기 
--------------------------------------------------------------------------------------------*/
INT CBL_SE_LOC_13::GetSqlSE_LOC_1310( CString &sWhere )
{	
EFS_BEGIN

	INT ids;

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	
	CStringList whereList;
	CString sBuf;	

	// ----------------------------------------------------------------------------
	// 1. 검색화면에 입력된 값 가져오기 
	// ----------------------------------------------------------------------------
	const INT nCtrlCnt = 29;
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sCtrlAlias[ nCtrlCnt + 1 ] =
	{
		_T("배가상태")  , _T("자료실구분")   , _T("이용제한구분")      , _T("배가변경사유")      ,_T("등록구분") , 
		_T("등록번호1") , _T("등록번호2")    , _T("별치기호1")         , _T("별치기호1")         , _T("분류기호구분") , 
		_T("분류기호1") , _T("분류기호2")    , _T("배가일자1")         , _T("배가일자2")         ,_T("발행년1") , 
		_T("발행년2")   , _T("코드변경구분") , _T("배가상태변경종류1") , _T("배가상태변경종류2") , _T("본표제") , 
		_T("저작자")    , _T("발행자")       , _T("관리구분")              , _T("미대출기간1")           , _T("미대출기간2") , 
		_T("도서기호1")     , _T("도서기호2")        , _T("복본여부")              , _T("검색구분")              , _T("배가변경자") /*PDW 추가*/
	};

	CString sGetData[ nCtrlCnt + 1 ];

	for( INT i = 0 ; i < nCtrlCnt + 1; i++ )
	{		
		INT nComboColumnIndex = 0;
		// 2005-12-15 UPD BY PDW(Jr) ==================================================
		// 자료실구분 코드 설명위치 변경 
		/* 수정전
		if( i == 1 || i==2 || i==9)
			nComboColumnIndex = 1;
		*/
		if( i==2 )
			nComboColumnIndex = 1;
		// ============================================================================

		ids = m_pParentMgr->GetControlData( sCMAlias , sCtrlAlias[i] , sGetData[i] , -1 , nComboColumnIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

		sGetData[i].TrimRight();
	}
	
	// ----------------------------------------------------------------------------
	// 2. 기본 조건절 생성하기 
	// ----------------------------------------------------------------------------
	// 배가상태
	CString sShelfState;
	if( !sGetData[0].IsEmpty() && (sGetData[0].Compare(_T("적용안함") )!=0) )
	{
		if( sGetData[0].Compare(_T("대출자료"))==0 )
		{
			GetWorkingStatusDescToCode( sGetData[0] ,0);
			sWhere += _T(" BB.WORKING_STATUS IN (") + sGetData[0] + _T(")");
		}
		else
		{
			GetWorkingStatusDescToCode( sGetData[0] ,0);
			sWhere += _T(" BB.WORKING_STATUS IN ('") + sGetData[0] + _T("')");
		}

	}
	else
	{
		// 2009.09.16 UPDATE BY KSJ : 책 상태 누락으로 인하여 수정 
		//DEL sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SE_SEARCH_CONDITION) + _T(", 'SEL312O') ");
		sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SE_SEARCH_CONDITION) + _T(", 'SEL317O') ");
	}

	// 책/부록 플래그
	if( !sGetData[ 28 ].IsEmpty() && sGetData[ 28 ].Compare( _T("전체") ) != 0 )
	{
		if( !sGetData[ 28 ].Compare( _T("책") ) )
			sWhere += _T(" AND  BB.BOOK_TYPE != 'A' ");
		else if( !sGetData[ 28 ].Compare( _T("부록") ) )
			sWhere += _T(" AND  BB.BOOK_TYPE = 'A' ");
	}
	
	
	// 배가변경자
	if( !sGetData[29].IsEmpty() && (sGetData[29].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T("( BB.SHELF_WORKER = '")+ sGetData[29]+ _T("' )"); 
	}

	// 2005-12-22 REM BY PDW(JR) ===========================================================
	// 제거됨 
	/*
	// 기본으로 이전 배가 상태가 실시간 배가 자료인것은 검색이 되지 않게 한다.
	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
	sWhere += _T(" BB.PREV_WORKING_STATUS NOT IN ('SEL112N') ");
	*/
	// =====================================================================================

	// 배가실
	sGetData[1].TrimRight();
	if( (!sGetData[1].IsEmpty()) && (sGetData[1].Compare(_T("적용안함") )!=0))
	//if( (sGetData[1].Compare( _T("0") ) != 0) && (!sGetData[1].IsEmpty()) && (sGetData[1].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_LOC_CODE = '") + sGetData[1] + _T("'");
	}

	//이용제한구분
	sGetData[2].TrimRight();
	if( !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("적용안함") )!=0))
	//if( !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("0"))!=0) && (sGetData[2].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.USE_LIMIT_CODE = '") + sGetData[2] + _T("'");
	}		
	
	// 등록번호구분 + 등록번호 ~
	sGetData[4].TrimRight();
	if( !sGetData[4].IsEmpty() && (sGetData[4].Compare(_T("적용안함") )!=0))
	//if( !sGetData[4].IsEmpty() && (sGetData[4].Compare(_T("0"))!=0) && (sGetData[4].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[5].IsEmpty() )
		{
			CString sRegNo1 = sGetData[4]+sGetData[5];
			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );
			if( !sGetData[6].IsEmpty() )
			{
				CString sRegNo2 = sGetData[4]+sGetData[6];
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere += _T(" BB.REG_NO BETWEEN '")+ sRegNo1 + _T("' AND '")+ sRegNo2 +_T("' ");
			}
			else
			{
				sWhere += _T(" BB.REG_NO = '")+ sRegNo1+_T("' "); 
			}
		}
		else
		{
			sWhere += _T("1=1");
//			sWhere += _T(" BB.REG_CODE = '")+ sGetData[4] +_T("' ");
		}
	}
	


	// 최초배가일자 ~
	if( !sGetData[12].IsEmpty() && (sGetData[12].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[13].IsEmpty() )
		{
			sWhere += _T("( (BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[13]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");
			sWhere += _T(" OR ");
			sWhere += _T(" (BB.SHELF_CHANGE_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[13]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) )");
		}
		else
		{
			sWhere += _T("( (BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[12]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");
			sWhere += _T(" OR ");
			sWhere += _T(" (BB.SHELF_CHANGE_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[12]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) )");
		}
	}

	// 관리구분
	// 16/04/12 김혜영: [코라스_기능개선]배가자료관리_조회시 종정보관리구분 제외
	// %KOLASIII%\cfg\Reference\DM\DM_BO_LOC_1300.DM에서 조건을 수정하여 아래의 구문이 필요하지 않음
	// IB.MANAGE_CODE=UDF_MANAGE_CODE ->  BB.MANAGE_CODE=UDF_MANAGE_CODE로 변경
/*// BEFORE -----------------------------------------------------------------------------------------
	sGetData[22].TrimRight();
	if( (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("적용안함") )!=0))
	//if( (sGetData[22].Compare( _T("0") ) != 0) && (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.MANAGE_CODE = '") + sGetData[22] + _T("'");
	}
*/// AFTER -----------------------------------------------------------------------------------------
	/*
	sGetData[22].TrimRight();
	if( (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("적용안함") )!=0))
	//if( (sGetData[22].Compare( _T("0") ) != 0) && (!sGetData[22].IsEmpty()) && (sGetData[22].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.MANAGE_CODE = '") + sGetData[22] + _T("'");
	}
	*/
//*/ END -------------------------------------------------------------------------------------------

	// 발행년 ~ 
	if( !sGetData[14].IsEmpty() && (sGetData[14].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[15].IsEmpty() )
		{
			sWhere += _T(" V.PUBLISH_DATE BETWEEN to_date('")+ sGetData[14]+ _T("/01/01 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[15]+_T("/12/31 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" V.PUBLISH_DATE BETWEEN to_date('")+ sGetData[14]+ _T("/01/01 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[14]+_T("/12/31 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
	}

	// 코드 변경 구분
	sGetData[16].TrimRight();
	if( !sGetData[16].IsEmpty() && (sGetData[16].Compare( _T("적용안함") ) != 0 ))
	{
		CString sDBColumnName;
		if( sGetData[16].Compare( _T("배가상태") ) == 0 ) sDBColumnName = _T("LAST_SHELF_STATUS_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("이용제한구분") ) == 0 ) sDBColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("관리구분") ) == 0 ) sDBColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");
		else if( sGetData[16].Compare( _T("자료실") ) == 0 ) sDBColumnName = _T("SHELF_CHANGE_DATE");

		if( !sGetData[17].IsEmpty() )
		{
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			if( !sGetData[18].IsEmpty() )
			{
				sWhere += _T(" BB.") + sDBColumnName + _T(" BETWEEN  to_date('")+ sGetData[17]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[18]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
			}
			else
			{
				sWhere += _T(" BB.") + sDBColumnName + _T(" BETWEEN  to_date('")+ sGetData[17]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[17]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
			}
		}
	}

	// ----------------------------------------------------------------------------
	// 3. 추가 조건절 생성하기 
	//    검색 윈도우의 모드가 _T("간략검색") 모드일 경우만 검색 조건에 추가
	// ----------------------------------------------------------------------------	
	CString sModeAlias;
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchSE_LOC_1310()") );

	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	if( sModeAlias.Compare( _T("상세검색") ) == 0 ) return 0;

	// 별치기호
	sGetData[7].TrimRight();
	if( !sGetData[7].IsEmpty() && (sGetData[7].Compare(_T("적용안함") )!=0))
	//if( !sGetData[7].IsEmpty() && (sGetData[7].Compare(_T("0"))!=0) && (sGetData[7].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SEPARATE_SHELF_CODE = '")+ sGetData[7]+_T("' "); 
	}

	// 분류기호구분 
	sGetData[9].TrimRight();
	if( !sGetData[9].IsEmpty() && (sGetData[9].Compare(_T("적용안함") )!=0))
	//if( !sGetData[9].IsEmpty() && (sGetData[9].Compare(_T("0"))!=0) && (sGetData[9].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.CLASS_NO_TYPE = '") + sGetData[9] + _T("'");
	}
	
	// 배가변경사유
	if( !sGetData[3].IsEmpty() && (sGetData[3].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_CHANGE_REASON_CODE = '") + sGetData[3] + _T("' ");
	}

	// 분류기호 ~
	if( !sGetData[10].IsEmpty() && (sGetData[10].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[11].IsEmpty() )
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '")+ sGetData[10]+ _T("' AND '")+sGetData[11]+_T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO = '")+ sGetData[10]+_T("' "); 
		}
	}

	// 도서기호
	if( !sGetData[25].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[25].IsEmpty() )
		{
			sWhere += _T(" BB.BOOK_CODE BETWEEN '")+ sGetData[25]+ _T("' AND '")+sGetData[26]+_T("' ");
		}
		else
		{
			sWhere += _T(" BB.BOOK_CODE = '")+ sGetData[25]+_T("' "); 
		}
	}

	// 복본기호
	if( sGetData[27].Compare( _T("적용안함") ) != 0 )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( sGetData[27].Compare( _T("원본") ) == 0 )
		{
			sWhere += _T(" ( BB.COPY_CODE = '0' OR BB.COPY_CODE IS NULL ) ");
		}
		else
		{
			sWhere += _T(" ( BB.COPY_CODE != '0' AND BB.COPY_CODE IS NOT NULL ) ") ;
		}

	}

	CMakeSearchData MakeSearchData(m_pParentMgr);

	/// 2003년 9월 1일 CATSEARCH -> ECOSEARCH 변환 , 작업자 : 조병걸
	bool bIsEqualSearch;
	// 본표제
	if( !sGetData[19].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// 평상시는 우절단 검색 생성
		if( sGetData[19].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[19]);
		if( !bIsEqualSearch ) 
			sGetData[19] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		//sWhere += _T(" IB.TITLE LIKE '") + sGetData[19] + _T("%'");

			
		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[19] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH(, SE_IDX_TITLE, '%s' ) "), sGetData[ 19 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[19] + _T("',NULL) > 0 ");


/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[19] + _T("') ");
#endif
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[19] + _T("',NULL) > 0 ");
#endif	
		*/
	}
	// 저작자
	if( !sGetData[20].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// 평상시는 우절단 검색 생성
		if( sGetData[20].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		MakeSearchData.GetKeyWord(sGetData[20]);
		
		if( !bIsEqualSearch ) 
			sGetData[20] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[20] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '%s' ) "), sGetData[ 20 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[20] + _T("',NULL) > 0 ");

/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[20] + _T("') ");
#endif
		//sWhere += _T(" IB.AUHOTR LIKE '") + sGetData[20] + _T("%'");
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[20] + _T("',NULL) > 0 ");
#endif
		*/

	}
	// 발행자
	if( !sGetData[21].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		// 평상시는 우절단 검색 생성
		if( sGetData[21].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[21]);
		if( !bIsEqualSearch ) 
			sGetData[21] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[21] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '%s' ) "), sGetData[ 21 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER , '") + sGetData[21] + _T("',NULL) > 0 ");


/*
#ifdef _ECOSEARCH_
		sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[21] + _T("') ");
#endif

		//sWhere += _T(" IB.PUBLISHER LIKE '") + sGetData[21] + _T("%'");
#ifndef _ECOSEARCH_
		sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER , '") + sGetData[21] + _T("',NULL) > 0 ");
#endif
		*/

	}
	
	// 미대출 자료 검색
	if( !sGetData[23].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		sWhere += _T("( ( SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.BOOK_KEY = BB.REC_KEY ");
		
		if( !sGetData[23].IsEmpty() )
		{
			sWhere += _T(" AND CL.LOAN_DATE BETWEEN to_date('")+ sGetData[23]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[24]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND CL.LOAN_DATE BETWEEN to_date('")+ sGetData[23]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[23]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}

		sWhere += _T(" ) = 0 ) ");
	}

	// 2005-12-14 UPD BY PDW(Jr) ==================================================
	// 원부번호 검색 
	CString sAccessionRegNoFrom, sAccessionRegNoTo;
	m_pParentMgr->GetControlData( sCMAlias , _T("원부번호부터"), sAccessionRegNoFrom );
	m_pParentMgr->GetControlData( sCMAlias , _T("원부번호까지"), sAccessionRegNoTo	 );

	if( !sAccessionRegNoFrom.IsEmpty() && !sAccessionRegNoTo.IsEmpty() )
	{
		if( sWhere.GetLength() ) 
			sWhere += _T(" AND ");

		sBuf.Format( _T("ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO BETWEEN '%s' AND '%s' )"), sAccessionRegNoFrom, sAccessionRegNoTo );
		sWhere += sBuf;
	}
	else if( !sAccessionRegNoFrom.IsEmpty() )
	{
		if( sWhere.GetLength() ) 
			sWhere += _T(" AND ");
		sBuf.Format( _T("ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO = '%s' )"), sAccessionRegNoFrom );
		sWhere += sBuf;
	}
		
	// 원부작성년도 검색 
	CString sYear;
	m_pParentMgr->GetControlData( sCMAlias , _T("원부작성년도"), sYear );
	if( !sYear.IsEmpty() )
	{
		if( sWhere.GetLength() )
			sWhere += _T(" AND ");

		sBuf.Format( _T(" ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' )"), sYear );
		sWhere += sBuf;
	}
	// ============================================================================

	INT cnt = whereList.GetCount();
	if( !fp && cnt )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		if( 1 == cnt )
			sBuf.Format( _T(" IB.REC_KEY IN %s "), whereList.GetAt( whereList.FindIndex( 0 ) ) );
		else
		{
			sBuf = _T(" IB.REC_KEY IN ( ") +  whereList.GetAt( whereList.FindIndex( 0 ) );
			for( INT i = 1; i < cnt; i++ )
				sBuf = sBuf + _T("  INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );	
					
			sBuf += _T(" ) ");
		}
		sWhere += sBuf;
	}
	//===================================================
	//2009.05.18 UPDATE BY PJW : 열려있는 경우만 닫는다.
// 	fclose(fp);
	else if( fp )
	{
		fclose(fp);
	}	
	//===================================================
	
	
	return 0;

EFS_END
	return -1;
}

INT CBL_SE_LOC_13::SearchFileSE_LOC_1310()
{
	EFS_BEGIN

	INT ids;
	CInputSanFileDlg SanFileDlg( m_pParentMgr );
	
	ids = SanFileDlg.DoModal();
	if( ids == IDCANCEL )
	{
		AfxMessageBox(_T("취소되었습니다."));
		return 0;
	}
	
	CESL_DataMgr pDM;

	ids = SanFileDlg.GetSanFileDM( &pDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	
	CESL_DataMgr TmpParentDM;
	CESL_DataMgr *pTmpParentDM;
	pTmpParentDM = &TmpParentDM;
	
	// = m_pParentMgr->FindDM( _T("DM_TMP_SE_LOC_1300") );
	//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( _T("DM_SE_LOC_1300") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
	pParentDM->FreeData();

	CLocCommonAPI::MakeCopyDM( m_pParentMgr , pParentDM , pTmpParentDM );

	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	INT nRowCount = pDM.GetRowCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		// SELECT할 등록번호 참조
		CString sRegNo;
		ids = pDM.GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

		// RECKEY리스트 생성
		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");

		// 레코드가 100개가 되었거나 마지막일 경우 SQL문을 날려줌
		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (pDM.GetRowCount()-1)) ) 
		{
			//===================================================
			//2009.05.21 UPDATE BY PJW : 책의 자료상태를 연속용으로 변경함
// 			CString sWhere = _T(" BB.REG_NO IN (") + sRegNoList + _T(")") + _T(" AND BB.WORKING_STATUS IN (")+CString(HOLD_SEARCH_CONDITION) +_T(")") ;
			CString sWhere = _T(" BB.REG_NO IN (") + sRegNoList + _T(")") + _T(" AND BB.WORKING_STATUS IN (")+CString(HOLD_SE_SEARCH_CONDITION) +_T(")") ;
			//===================================================
			
			ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pTmpParentDM , sWhere );
//			ids = pTmpParentDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );
			
			ids = AddDM( pParentDM , pTmpParentDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

			sRegNoList.Empty();
		}
	}
	
	CString sCMAlias = _T("CM_SE_LOC_1300");
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::SearchFileSE_LOC_1310()") );

	return 0;

	EFS_END
	return -1;

}

// 코드 일괄 변경
INT CBL_SE_LOC_13::ChangeCode()
{
	EFS_BEGIN

	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	INT nIndex;

	CString sCodeNameCode;	// 변경될 코드의 종류 코드 ( 배가상태 : 0 , 이용제한구분 : 1 , 관리구분 : 2 )
	CString sCodeName;			// 변경될 코드의 필드명( WORKING_STATUS , USE_LIMIT_CODE , MANAGE_CODE )
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
	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);
	//-----------------------------------------------

	bool bIsChangeDeleteBook;
	CString sChangeDeleteBook;
	GetTempData( _T("제적자료변경허용") , sChangeDeleteBook );
	
	if( sChangeDeleteBook.Compare(_T("Y")) == 0 )
		bIsChangeDeleteBook = TRUE;
	else
		bIsChangeDeleteBook = FALSE;
	
	// 선정된 자료
	CString sRepresentBookYN;
	CString sWorkingStatus;

	// 수리제본 도서 개수
	INT repairBindingCnt = 0;

	INT nNoChangeCnt=0;
	INT nNoDeleteBookChangeCnt = 0;
	INT nChangeCnt=0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		// 프로그래스바 설정------------------
		ProgressBar->ChangeProgressBar(i);
		//-----------------------------------
		nIndex = pGrid->SelectGetIdx();

		// 대출 중인 자료는 변경할수 없다.
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS값이 잘못들어감
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )
		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0)  || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
		//=====================================================
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;
		}

		// 수리제본 일경우
		if( _ttoi( pParentDM->GetCellData( _T("수리제본"), nIndex ) ) )
		{
			pGrid->SelectGetPrev();
			repairBindingCnt++;
			continue;
		}

		// 제적 자료일 경우 
		// 선택 옵션에 따라 변경하고 변경하지 않는다.
		// 이용제한 구분일 경우만
		if( _ttoi(sCodeNameCode) == 1 )
		{
			if( sWorkingStatus.Compare(_T("SEL215O")) == 0 )
			{
				if( !bIsChangeDeleteBook )
				{
					/// 사서제한으로 변경되는 거라면 변경한다.
					if( sCodeValue.Compare(_T("CL")) != 0 )
					{
						pGrid->SelectGetPrev();
						nNoDeleteBookChangeCnt++;
						continue;			
					}
				}
			}
		}
		
		// UPDATE할 책테이블의 REC_KEY 참조
		CString sBookRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeCode()") );

		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_종KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeCode()") );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_대표책값") , sRepresentBookYN , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeCode()") );



		// RECKEY리스트 생성
//		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRecKeyList += _T(",");
//		sRecKeyList += sBookRecKey;

		sRecKeyList = sBookRecKey;

		// 레코드가 MAX개가 되었거나 마지막일 경우 SQL문을 날려줌
//		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nGetRowCount-1)) ) 
//		{
			ids = ChangeCodeDBUpdate( sCodeNameCode , sCodeName , sCodeValue  , sRecKeyList , sSpeciesRecKey , sRepresentBookYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeCode()") );
			sRecKeyList.Empty();
//		}
		
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
		sMsg.Format( _T("총 '%d'건의 자료중 처리된 자료가 '%d'건이며 \r\n")
		_T("처리되지 못한 자료가 '%d'건입니다."),
		nChangeCnt+nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt , nChangeCnt , nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt);
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) && ( repairBindingCnt < 1 ) && ( nNoDeleteBookChangeCnt < 1 ) )
		sMsg.Format( _T("모두 '%d'건이 처리되었습니다.") , nChangeCnt );
	
	if( !sMsg.IsEmpty() )
		AfxMessageBox(sMsg);
	
	if( (nNoChangeCnt > 0) )
	{
		sMsg.Format( _T("대출중인 자료이므로 '%d'건이처리되지 못했습니다.") , nNoChangeCnt );
		AfxMessageBox(sMsg);
	}		
	
	if( repairBindingCnt > 0 )
	{
		sMsg.Format( _T("수리제본 상태이므로 '%d' 건이 처리되지 못했습니다."), repairBindingCnt );
		AfxMessageBox( sMsg );
	}
	
	if( nNoDeleteBookChangeCnt > 0 )
	{
		sMsg.Format( _T("제적 상태이므로 '%d' 건이 처리되지 못했습니다."), nNoDeleteBookChangeCnt );
		AfxMessageBox( sMsg );
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CBL_SE_LOC_13::GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1340");

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("코드변경종류") , sCodeNameCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

	CString sCodeComboAlias;
	if( sCodeNameCode.Compare( _T("0") ) == 0 )
	{
		sCodeName = _T("WORKING_STATUS");
		sCodeComboAlias = _T("배가상태코드");
		sDMColumnAlias = _T("BB_배가상태");
	}
	else if( sCodeNameCode.Compare( _T("1") ) == 0 )
	{
		sCodeName = _T("USE_LIMIT_CODE");
		sCodeComboAlias = _T("이용제한구분코드");
		sDMColumnAlias = _T("BB_이용제한구분");
	}
	else if( sCodeNameCode.Compare( _T("2") ) == 0 )
	{
		sCodeName = _T("MANAGE_CODE");
		sCodeComboAlias = _T("관리구분코드");
		sDMColumnAlias = _T("BB_관리구분");
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
		ids = GetWorkingStatusDescToCode( sCodeValue , 0 );
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
INT CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	// LOG DB 컬럼 결정
	CString sLogColumnName;
	// 배가 상태
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
		sLogColumnName = _T("LAST_SHELF_STATUS_CHANGE_DATE");
	// 이용제한 구분 
	else if( sCodeNameCode.Compare(_T("1")) == 0 ) 
		sLogColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");
	// 관리구분 
	else if( sCodeNameCode.Compare(_T("2")) == 0 )
		sLogColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( sCodeName , _T("STRING") , sCodeValue );
	pDM->AddDBFieldData( sLogColumnName , _T("NUMERIC") , _T("sysdate") );

//	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );
	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

/*
	CString sUpdateQuery;
	// UPDATE QUERY 생성
	sUpdateQuery.Format( _T("UPDATE SE_BOOK_TBL SET"
											_T(" %s = '%s' , %s = sysdate ")
											_T("WHERE REC_KEY IN ( %s) ;")) , 
										sCodeName , sCodeValue ,  sLogColumnName , sRecKeyList );
	pDM->AddFrame( sUpdateQuery );
*/
//	ids = pDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );
	pDM->EndFrame();

	MobileApi.EndLog();

	/// 배가 상태 변경일 경우
	CString sSpeciesCodeValue;
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
	{
		pDM->StartFrame();

		CLocCommonAPI::MakeIdxWokingStatusFrame( pDM , sSpeciesKey  , 1 , m_pParentMgr );

		pDM->SendFrame();
		pDM->EndFrame();

		return 0;
	}

	
	// 이용제한 구분이면 다음을 실행한다.
	if( sCodeNameCode.Compare(_T("1")) == 0 )
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

INT CBL_SE_LOC_13::ChangeShelf( INT nMode )
{
	EFS_BEGIN

	INT ids;

	INT nPreIndex;

	CString sShelfDate;
	CString sShelfWorker;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sCMAlias = _T("CM_SE_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	CESL_Grid *pGrid = ((CESL_Grid*)pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

	nPreIndex = pGrid->GetIdx();

	// 입력 내용을 참조
	CString sGetInputData[6];	// 변경위치 , 변경날짜 , 변경지시자 , 변경사유코드 , 변경사유 , 배가 변경전위치 
	ids = GetInputDataSE_LOC_1330( sGetInputData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
	if( ids > 0 ) return ids;


	INT nIndex;
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	CString sWorkingStatus;
	CString sSpeciesKey;

	// 낱권 변경
	if( nMode == 0 )
	{
		nIndex = pGrid->GetIdx();

		// 자료 상태가 대출 중인지 검사하여 대출중인 자료는 변경하지 못한다.
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		//=====================================================
		//2009.02.17 UPDATE BY PJW : WORKING_STATUS값이 잘못들어감
// 		if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 		(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )

		// 2012.11.07 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차
		// 관외대출자료는 배가변경 가능
		if( (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) || (sWorkingStatus.Compare(_T("SEL314O"))==0) || 
			// 18.09.27 JMJ KOL.RED.2018.008
			(sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
//		if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
//			(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0))
		//=====================================================	
		
		{
			nNoChangeCnt++;
		}
		else
		{
			// UPDATE할 책테이블의 REC_KEY 참조
			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_종KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			
			sRecKeyList = sBookRecKey;
			ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			sRecKeyList.Empty();

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			nChangeCnt++;
		}
	}
	// 일괄 변경
	else
	{
		INT nGetRowCount = pGrid->SelectGetCount();
		// 프로그래스바를 생성하기 위해--------------------
		CBlLocProgressBarDlg *ProgressBar;
		ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
		ProgressBar->Create( m_pParentMgr );
		ProgressBar->CenterWindow( m_pParentMgr );
		ProgressBar->ShowWindow( SW_SHOW );
		INT nUpper = nGetRowCount-1;
		if( nGetRowCount == 1 ) nUpper = 1;
		ProgressBar->InitRange(nUpper);
		//-----------------------------------------------

		// 그리드의 포지션을 마지막으로
		pGrid->SelectGetTailPosition();

		for( INT i = 0 ; i < nGetRowCount ; i++ )
		{
			// 프로그래스바 설정------------------
			ProgressBar->ChangeProgressBar(i);
			//-----------------------------------
			
			nIndex = pGrid->SelectGetIdx();

			// 자료 상태가 대출 중인지 검사하여 대출중인 자료는 변경하지 못한다.
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

			//=====================================================
			//2009.02.17 UPDATE BY PJW : WORKING_STATUS값이 잘못들어감
// 			if( sWorkingStatus.Compare(_T("SEL321O"))==0 || (sWorkingStatus.Compare(_T("SEL322O"))==0) || (sWorkingStatus.Compare(_T("SEL323O"))==0) ||
// 			(sWorkingStatus.Compare(_T("SEL324O"))==0) || (sWorkingStatus.Compare(_T("SEL325O"))==0) )

			// 2012.11.07 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차
			// 관외대출자료는 배가변경 가능
			if( (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) || (sWorkingStatus.Compare(_T("SEL314O"))==0) || 
				// 18.09.27 JMJ KOL.RED.2018.008
				(sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0) || (sWorkingStatus.Compare(_T("SEL411O"))==0) || (sWorkingStatus.Compare(_T("SEL611O"))==0) )
//			if( (sWorkingStatus.Compare(_T("SEL311O"))==0) || (sWorkingStatus.Compare(_T("SEL312O"))==0) || (sWorkingStatus.Compare(_T("SEL313O"))==0) ||
//				(sWorkingStatus.Compare(_T("SEL314O"))==0) || (sWorkingStatus.Compare(_T("SEL315O"))==0) || (sWorkingStatus.Compare(_T("SEL511O"))==0))
			//=====================================================
			
			{
				nNoChangeCnt++;
				pGrid->SelectGetPrev();
				continue;
			}


			// UPDATE할 책테이블의 REC_KEY 참조
			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_종KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			// RECKEY리스트 생성
//			if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRecKeyList += _T(",");
			sRecKeyList += sBookRecKey;

			// 레코드가 100개가 되었거나 마지막일 경우 SQL문을 날려줌
//			if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nGetRowCount-1)) ) 
//			{
				ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
				sRecKeyList.Empty();
//			}

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );
			
			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_SE_LOC_13::ChangeShelf( INT nMode )") );

			if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
			nChangeCnt++;
		}
		// 프로그래스바 디스트로이------------------
		ProgressBar->DestroyWindow();
		if( ProgressBar != NULL ) delete ProgressBar;
		//----------------------------------
	}

	m_pParentMgr->AllControlDisplay( _T("CM_SE_LOC_1330") , nPreIndex );

	CString sMsg;
	if( nMode == 1 )
	{
		if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("총 '%d'건의 자료중 처리된 자료가 '%d'건이며 대출중인 자료이기 때문에 처리되지 못한 자료가 '%d'건입니다."),
			nChangeCnt+nNoChangeCnt,nChangeCnt,nNoChangeCnt );
		else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
			sMsg.Format( _T("모두 '%d'건이 처리되었습니다.") , nChangeCnt );
		else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("모두 '%d'건이 대출중인 자료이기 때문에 처리되지 못했습니다.") , nNoChangeCnt );
	}
	else
	{
		if( (nChangeCnt > 0) && (nNoChangeCnt < 1) )
			sMsg.Format( _T("처리되었습니다.") );
		else if( (nChangeCnt < 1) && (nNoChangeCnt > 0) )
			sMsg.Format( _T("대출중인 자료이기 때문에 처리되지 못했습니다.") );
	}
	AfxMessageBox(sMsg);

	return 0;


	EFS_END
	return -1;

}

// 변경위치 , 변경날짜 , 변경지시자 , 변경사유코드 , 변경사유 , 배가 변경전위치 
INT CBL_SE_LOC_13::GetInputDataSE_LOC_1330( CString *sGetInputData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1330");

	TCHAR *sControlAlias[6] =
	{
		_T("변경위치_입력") , _T("변경날짜_입력") , _T("변경지시자_입력") , _T("변경사유코드_입력") , _T("변경사유내용_입력") , _T("배가위치")
	};

	INT nIndex = 1;
	for( INT i = 0 ; i < 6 ; i++ )
	{
		if( i == 3 ) nIndex = 0;
		else nIndex = 0;
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetInputData[i] , -1 , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GetInputDataSE_LOC_1330( CString *sGetInputData )") );

		sGetInputData[i].TrimLeft(); sGetInputData[i].TrimRight();

		if( sGetInputData[i].Compare(_T("적용안함")) == 0 ) sGetInputData[i] = _T("");

		if( sGetInputData[i].IsEmpty() ) 
		{
			if( i == 0 || i == 1 || i==2 )
			{
				CString sMsg;
				if( i == 0 ) sMsg = _T("변경위치를 선택해주십시오");
				if( i == 1 ) sMsg = _T("배가일을 입력하십시오");
				if( i == 2 ) sMsg = _T("변경지시자를 입력하십시오");
				AfxMessageBox( sMsg );
				return 1;
			}
		}
	}
	return 0;

	EFS_END
	return -1;

}


INT CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList , CString sSpeciesKey)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );


	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	pDM->StartFrame();
	
	CString sUpdateQuery;

	pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sGetInputData[0] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_DATE") , _T("STRING") , sGetInputData[1] );
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sGetInputData[2] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_REASON_CODE") , _T("STRING") , sGetInputData[3] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_ETC_REASON") , _T("STRING") , sGetInputData[4] );
	pDM->AddDBFieldData( _T("SHELF_CHANGE_BEFORE_LOC") , _T("STRING") , sGetInputData[5] );

	MobileApi.MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

	// 종에도 기록한다.
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

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 1 , m_pParentMgr ,  &MobileApi );
	
	// QUERY SEND
//	ids = pDM->SendFrame();
	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();

	return 0;

	EFS_END
	return -1;

}

// sGetInputData : 변경위치 , 변경날짜 , 변경지시자 , 변경사유코드 , 변경사유 , 배가 변경전위치 
INT CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1300");

	TCHAR *sDMColumnAlias[5] =
	{
		_T("BB_배가기호") , _T("BB_배가변경일자") , _T("BB_배가변경지시자") , _T("BB_배가변경사유") , _T("BB_배가변경사유내용") 
	};

	for( INT i = 0 ; i < 5 ; i++ )
	{
		ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias[i] , sGetInputData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}
	
	CString sCodeDesc;
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sGetInputData[0] , sCodeDesc  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	ids = m_pParentMgr->SetDataMgrData( sDMAlias , _T("BB_자료실명") , sCodeDesc , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	return 0;


	EFS_END
	return -1;

}

// 그리드 업데이트 변경위치 , 변경날짜 , 변경지시자 , 변경사유코드 , 변경사유 , 배가 변경전위치 
INT CBL_SE_LOC_13::CMGridUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	TCHAR *sDMColumnAlias[3] =
	{
		_T("BB_배가기호") , _T("BB_자료실명") , _T("BB_배가변경사유내용") 
	};

	INT nColIndex[3] = 
	{
		-1 , -1 , -1 
	};

	CString sData[3];

	sData[0] = sGetInputData[0];
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sGetInputData[0] , sData[1]  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	sData[2] = sGetInputData[4];

	// 컬럼 찾기 : 링크된 데이터 메니져의 알리아스로 찾는다.
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , sCMAlias , sGridAlias , sDMColumnAlias , 3 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	for( INT i = 0 ; i < 3 ; i++ )
	{
		if( nColIndex[i] == -1 ) continue;
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}

	return 0;


	EFS_END
	return -1;

}

//------------------------------------------------------------------------------------
// 공통으로 쓰이는 모듈
//------------------------------------------------------------------------------------

INT CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )
{
	EFS_BEGIN

	INT ids;
	CString sGetData;

	const INT nColumnCnt = pSourceDM->GetColCount();
	CString *sColumnAlias;
	sColumnAlias = NULL;
	sColumnAlias = new CString[nColumnCnt];

	ids = pSourceDM->GetAllAlias( sColumnAlias , nColumnCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

	INT nRowCount = pSourceDM->GetRowCount();
	for( INT i = 0 ; i < nRowCount ;i++ )
	{
		for( INT j = 0 ; j < pDestDM->GetColCount() ; j++ )
		{
			if( j == 0 ) pDestDM->InsertRow( -1 );
			ids = pSourceDM->GetCellData( i , j , sGetData );
			if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

			ids = pDestDM->SetCellData( sColumnAlias[j] , sGetData , pDestDM->GetRowCount()-1);			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );
			sGetData.Empty();
		}
	}

	return 0;

	EFS_END
	return -1;

}


// nMode 
// 0-> WORKING_STATUS 의 DESCRIPTION을 코드로 변경해준다.
// 1-> WORKING_STATUS 의 코드를 DESCRIPTION으로 변경해준다.
INT CBL_SE_LOC_13::GetWorkingStatusDescToCode( CString &sCodeValue , INT nMode )
{
	EFS_BEGIN

	INT ids;

	ids = m_pLocCommonAPI->GetWorkingStatusDescToCode( sCodeValue , nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWorkingStatusDescToCode") );
	return 0;

	EFS_END
	return -1;

}


// 배가 자료 관리의 그리드를 화면에 적용시 코드값을 설명값으로 변경시키는 작업을 한다.
INT CBL_SE_LOC_13::GridDisplay( CString sCMAlias )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias;
	CString sFieldAlias;

	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0)
	{
		sDMAlias = _T("DM_SE_LOC_1300");
		sFieldAlias = _T("BB_자료실명");
	}
	else if( sCMAlias.Compare(_T("CM_BO_LOC_1100"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1100");
		sFieldAlias = _T("BB_배가위치부호");
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	// 배가 위치 CODE -> 설명
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , sFieldAlias  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	// 배가 상태 변경
	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0) 
	{
		CString sCodeValue;
		for( INT i = 0 ; i < pDM->GetRowCount() ;i++ )
		{
			ids = pDM->GetCellData( _T("BB_배가상태") , i , sCodeValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
			ids = GetWorkingStatusDescToCode( sCodeValue , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
			ids = pDM->SetCellData( _T("BB_배가상태") , sCodeValue , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		}
		
		// 관리구분
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("관리구분") , _T("BB_관리구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
		// 이용제한 구분
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("이용제한구분") , _T("BB_이용제한구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );
		// 행태 구분
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("형식구분") , _T("형태사항")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	}

	if( sCMAlias.Compare(_T("CM_SE_LOC_1300"))==0)
	{
		// 청구기호 생성
		ids = MakeShelfCode( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		// 원복본구분 표시
		ids = DisplayCopyBook( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

		// 김용배 추가
		// 그리드에 보여줄때 형태사항 보여주기
		if( DisplayFormType( pDM ) )
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_SE_LOC_13::DisplayFormType( CESL_DataMgr *pDM )") );


	}

	ids = m_pParentMgr->AllControlDisplay( sCMAlias , -1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_SE_LOC_13::GridDisplay( CString sCMAlias )") );

	return 0;

	EFS_END
	return -1;

}

// 청구기호 적용하는 함수
INT CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
/*
	const INT nCnt = 5;
	TCHAR *sFieldAlias[nCnt] =
	{
		_T("IBS_청구_별치기호_코드") ,
		_T("IBS_청구_분류기호") ,
		_T("IBS_청구_도서기호") ,
		_T("IBS_청구_권책기호") ,
		_T("IBS_청구_복본기호") ,
	};


	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sShelfCode;
	CString sTmpCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		sShelfCode.Empty();
		// 청구기호 생성
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			ids = pDM->GetCellData( sFieldAlias[j] , i , sTmpCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
			
			if( !sTmpCode.IsEmpty() ) 
			{
				if( sShelfCode.GetLength() > 1 ) sShelfCode += _T("-");
				sShelfCode += sTmpCode;
			}
		}

		// 청구기호 적용
		ids = pDM->SetCellData( _T("청구기호") , sShelfCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
	}
*/
	return 0;

	EFS_END
	return -1;

}

// 원/복본 구분 표시
INT CBL_SE_LOC_13::DisplayCopyBook( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DisplayCopyBook( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sCopyCode;
	CString sTmpCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		sCopyCode.Empty();
		// 원복본 생성
		ids = pDM->GetCellData( _T("BB_원/복본구분") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
		
		if( !sTmpCode.IsEmpty() && (sTmpCode.Compare(_T("0"))!=0)) 
			sCopyCode = _T("복본");
		else
			sCopyCode = _T("원본");

		// 원/복본구분 적용
		ids = pDM->SetCellData( _T("BB_원/복본구분") , sCopyCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::MakeShelfCode( CString sDMAlias )") );
	}

	return 0;


	EFS_END
	return -1;

}

// 코드 일괄 변경중 
// 책테이블중 대표가 되는 코드를 얻어오는 함수
// 이용제한 구분 : 
//                 		_T("CL") , _T("CD") , _T("CA") , _T("IZ") , _T("GM")
INT CBL_SE_LOC_13::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )
{
	EFS_BEGIN

	INT ids;

	const INT nUseLimitCodeCnt = 5;
	INT nUseLimitCodeIndex;
	TCHAR *sUseLimitCode[nUseLimitCodeCnt] = 
	{
		_T("CL") , _T("CD") , _T("CA") , _T("IZ") , _T("GM")
	};



	CString sCode;
	nUseLimitCodeIndex = 0;
	for( INT i = 0 ; i < pSpeciesDM->GetRowCount() ; i++ )
	{
		ids = pSpeciesDM->GetCellData( _T("이용제한구분") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )") );
		
		if( sCode.IsEmpty() ) sCode = sUseLimitCode[0];
		
		for( INT j = 0 ; j < nUseLimitCodeCnt ; j++ )
		{
			if( sCode.Compare( sUseLimitCode[j]) == 0 ) break;
		}
		if( j == nUseLimitCodeCnt ) continue;

		if( nUseLimitCodeIndex < j ) nUseLimitCodeIndex = j;
	}
	sSpeciesCodeValue = sUseLimitCode[nUseLimitCodeIndex];

	return 0;

	EFS_END
	return -1;

}

// 코드 일괄 변경중
// 변경되었을 경우 화면에 보여주는 작업
INT CBL_SE_LOC_13::CMGridCodeChangeUpdate(INT nIndex)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");

	CString sCMAlias = _T("CM_SE_LOC_1300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") );

	const INT nCnt = 3;

	TCHAR *sDMFieldAlias[nCnt] =
	{
		_T("BB_관리구분") , _T("BB_배가상태") , _T("BB_이용제한구분")
	};

	INT nColIndex[nCnt] =
	{
		-1,-1,-1
	};

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMFieldAlias , nCnt , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::DMUpdate( CString *sGetInputData , INT nIndex )") ); 

	CString sData;
	
	for( INT i = 0 ; i < 3 ; i++ )
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
INT CBL_SE_LOC_13::DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1300");

	if( sDMColumnAlias.Compare(_T("BB_관리구분")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("관리구분") , sCodeValue , sCodeValue   );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
		
	}
	else if( sDMColumnAlias.Compare(_T("BB_배가상태")) == 0 )
	{
		ids = GetWorkingStatusDescToCode( sCodeValue , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("BB_이용제한구분")) == 0 )
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

INT CBL_SE_LOC_13::DetailView()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_SE_LOC_13::ChangeCode()") );

	// 검사
	if( pParentDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("자료를 검색해 주십시오") );
		return 0;
	}

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();

	if( nSelectCnt > 1 )
	{
		nMode = 1;
		pGrid->SelectGetHeadPosition();
	}
	else
	{
		nSelectCnt = pParentDM->GetRowCount();
		nStartPos = pGrid->GetIdx();
	}
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = i;
		
		pParentDM->GetCellData( _T("BS_종KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );

		if( nIndex == nTmpStartPos )
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	
	CSEBookDetailViewerDlg Dlg(m_pParentMgr);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();

	return 0;

	EFS_END
	return -1;

}

// 종 이용제한 구분 UPDATE
INT CBL_SE_LOC_13::ChangeUseLimitCode( CString sSpeciesKey )
{
	EFS_BEGIN

	INT ids;
	CString sPreUseLimitCode , sNewUseLimitCode;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_SE_LOC_1300_USE_LIMIT_SPECIES"));
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

INT CBL_SE_LOC_13::MakeArrayUseLimitCode( CString sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

// 우선 순위가 높은것을 알아내준다.
INT CBL_SE_LOC_13::GetBossUseLimitCode( CString &sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

bool CBL_SE_LOC_13::CompareUseLimtCodes( CArray <CString,CString> &saPreUseLimitCode , CArray <CString,CString> &saNewUseLimitCode )
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


/// 구분 코드 일괄 종변경
/// 종이 변경 -> 종만 변경
INT CBL_SE_LOC_13::ChangeSpeciesCode()
{
	EFS_BEGIN


	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_SE_LOC_1300");
	CString sParentDMAlias = _T("DM_SE_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
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
	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
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
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_종KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_SE_LOC_13::ChangeSpeciesCode()") );

		pParentDM->StartFrame();

		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("SE_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		// 2006-04-03 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 색인 누락 부분을 추가
		pParentDM->InitDBFieldData();
		pParentDM->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),sCodeValue);
		pParentDM->MakeUpdateFrame(_T("IDX_SE_TBL"),_T("REC_KEY"),_T("NUMERIC"),sSpeciesRecKey);

		CLocCommonAPI::MakeIdxTmpTblInsertFrame( pParentDM, sSpeciesRecKey, 1 , m_pParentMgr);
		// ------------------------------------------------------------------------------------------------------

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
		sMsg.Format( _T("총 '%d'건의 자료중 처리된 자료가 '%d'건이며 대출중인 자료이기 때문에 처리되지 못한 자료가 '%d'건입니다."),
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

INT CBL_SE_LOC_13::DisplayFormType( CESL_DataMgr *pDM )
{
	CString data[ 4 ];
	CString output;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		output.Empty();
		
		data[ 0 ] = pDM->GetCellData( _T("PAGE"), i );
		data[ 1 ] = pDM->GetCellData( _T("PHYSICAL_PROPERTY"), i );
		data[ 2 ] = pDM->GetCellData( _T("BOOK_SIZE") , i );
		data[ 3 ] = pDM->GetCellData( _T("ACCOMP_MAT"), i );
		
		if( !data[ 0 ].IsEmpty() )
			output += data[ 0 ];
		if( !data[ 1 ].IsEmpty() )
			output = output + _T(":") + data[ 1 ];
		if( !data[ 2 ].IsEmpty() )
			output = output + _T(";") + data[ 2 ];
		if( !data[ 3 ].IsEmpty() )
			output = output + _T("+") + data[ 3 ];

		pDM->SetCellData( _T("형태사항"), output, i );

		//pDM->SetCellData( _T("형태사항"), MakeFormType( pDM->GetCellData( _T("마크"), i ) ), i );
	}
	return 0;
}

CString CBL_SE_LOC_13::MakeFormType( CString marc )
{
	CMarcMgr* pMarcMgr = m_pParentMgr->m_pInfo->pMarcMgr;
	CMarc Marc;
	pMarcMgr->Decoding( marc , &Marc );

	CString formType;
	Marc.GetField( _T("300"), formType );
	formType.Replace( CString( 31 ) + _T("a") , _T("") );
	formType.Replace( CString( 31 ) + _T("b") , _T("") );
	formType.Replace( CString( 31 ) + _T("c") , _T("") );
	formType.Replace( CString( 31 ) + _T("d") , _T("") );
	formType.Replace( CString( 31 ) + _T("e") , _T("") );
	formType.Replace( CString( 30 ) , _T("") );
	formType.TrimLeft();
	formType.TrimRight();	

	return formType;
}
