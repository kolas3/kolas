// BL_BOSHELF.cpp: implementation of the CBL_BOSHELF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_BOSHELF.h"

#include "BlLocProgressbarDlg.h"

#include "..\\OpenSanFile\\InputSanFileDlg.h"

#include "Kolas2up_MobileApi.h"

#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"

#include "direct.h"
#include "..\..\..\공동목록\check_dup_api\rs_sql.h"
#include "..\..\include\공동목록\CU_ComApi.h"
#include "..\..\..\공동목록\Check_Dup_api\SearchMake.h" 


#include "KolisNetService.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_BOSHELF::CBL_BOSHELF( CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	m_pLocCommonAPI = new CLocCommonAPI(m_pParentMgr);
}

CBL_BOSHELF::~CBL_BOSHELF()
{
	if( m_pLocCommonAPI != NULL ) delete m_pLocCommonAPI;
	m_pLocCommonAPI = NULL;
}

INT CBL_BOSHELF::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( _T("배가자료관리_검색") ) == 0 )
	{
		ids = SearchBO_LOC_1310();
		if( ids == 1 ) return -1;
	}
	if( sPathAlias.Compare( _T("RFID검색") ) == 0 )
	{
		ids = SearchBO_LOC_1310FromRFID();
		if( ids == 1 ) return -1;
	}
	else if( sPathAlias.Compare( _T("배가자료관리_파일로검색") ) == 0 )
	{
		ids = SearchFileBO_LOC_1310();
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

	return ids;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::GetSqlBO_LOC_1130( CString &sWhere , INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sParentDMAlias = _T("DM_BO_LOC_1100");
	CString sBookKey;
	ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_종KEY") , sBookKey , nIndex ); 
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::GetSqlBO_LOC_1130( CString &sWhere , INT nIndex )") );

	if( sBookKey.IsEmpty() )
	{
		AfxMessageBox( _T("종키가 존재하지 않는 자료입니다.") );
		return 1;		
	}

	sWhere.Format( _T("SPECIES_KEY = %s") , sBookKey );
	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::SearchBO_LOC_1310()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_1300");
	CString sCMAlias = _T("CM_BO_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );

	CString sWhere;
	ids = GetSqlBO_LOC_1310( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	if( ids > 0 ) return 1;	
		
	DWORD START, FINISH;
	START = GetTickCount();
		ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - 딘헹, 배가자료관리 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );	
	
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

INT CBL_BOSHELF::SearchBO_LOC_1310FromRFID()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_1300");
	CString sCMAlias = _T("CM_BO_LOC_1300");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	CString sWhere;
	GetTempData( _T("WHERE") , sWhere );
	
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

INT CBL_BOSHELF::GetSqlBO_LOC_1310( CString &sWhere )
{	
EFS_BEGIN

	INT ids;
	
	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	CStringList whereList;
	CString sBuf;

	INT nSFieldCnt = 0;

	const INT nCtrlCnt = 29;
	CString sCMAlias = _T("CM_BO_LOC_1310");
	CString sCtrlAlias[ nCtrlCnt + 1 ] =
	{
		_T("배가상태")  , _T("자료실구분")   , _T("이용제한구분")      , _T("배가변경사유")      ,_T("등록구분") , 
		_T("등록번호1") , _T("등록번호2")    , _T("별치기호1")         , _T("별치기호1")         , _T("분류기호구분") , 
		_T("분류기호1") , _T("분류기호2")    , _T("배가일자1")         , _T("배가일자2")         ,_T("발행년1") , 
		_T("발행년2")   , _T("코드변경구분") , _T("배가상태변경종류1") , _T("배가상태변경종류2") , _T("본표제") , 
		_T("저작자")    , _T("발행자")       , _T("관리구분")              , _T("미대출기간1")           , _T("미대출기간2") , 
		_T("도서기호1")     , _T("도서기호2")        , _T("복본여부")              , /*추가*/_T("검색구분")      , /*PDW 추가*/_T("배가변경자")		
	};
	CString sGetData[ nCtrlCnt + 1 ];

	for( INT i = 0 ; i < nCtrlCnt + 1 ; i++ )
	{
		INT nComboColumnIndex = 0;
		
		if ( i == 2 )
			nComboColumnIndex = 1;
		// ============================================================================

		ids = m_pParentMgr->GetControlData( sCMAlias , sCtrlAlias[i] , sGetData[i] , -1 , nComboColumnIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );

		sGetData[i].TrimRight();
	}
	
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
		sWhere += _T(" BB.WORKING_STATUS IN (") + CString(HOLD_SEARCH_CONDITION) + _T(", 'BOL217O' ) ");
	}

	if( !sGetData[ 28 ].IsEmpty() && sGetData[ 28 ].Compare( _T("전체") ) != 0 )
	{
		if( _T("책") == sGetData[ 28 ] )
			sWhere += _T(" AND BB.BOOK_APPENDIX_FLAG != 'A' ");
		else if( _T("부록") == sGetData[ 28 ] )
			sWhere += _T(" AND BB.BOOK_APPENDIX_FLAG = 'A' ");
	}

	sGetData[1].TrimRight();
	if( (sGetData[1].Compare( _T("0") ) != 0) && (!sGetData[1].IsEmpty()) && (sGetData[1].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_LOC_CODE = '") + sGetData[1] + _T("'");
	}

	sGetData[2].TrimRight();
	if( !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("0"))!=0) && (sGetData[2].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.USE_LIMIT_CODE = '") + sGetData[2] + _T("'");
	}

	sGetData[4].TrimRight();
	if( !sGetData[4].IsEmpty() && (sGetData[4].Compare(_T("적용안함") )!=0))
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
			nSFieldCnt++;
		}
		else
		{
			sWhere += _T(" 1=1 ");
		}
	}
	
	if( !sGetData[12].IsEmpty() && (sGetData[12].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[13].IsEmpty() )
			sWhere += _T("( BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[13]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");
		else
			sWhere += _T("( BB.SHELF_DATE BETWEEN to_date('")+ sGetData[12]+ _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[12]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )");

		nSFieldCnt++;
	}

	if( !sGetData[29].IsEmpty() && (sGetData[29].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T("( BB.SHELF_WORKER = '")+ sGetData[29]+ _T("' )"); 

		nSFieldCnt++;
	}

	if( !sGetData[14].IsEmpty() && (sGetData[14].Compare(_T("적용안함") )!=0) )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[15].IsEmpty() )
		{
			sWhere += _T(" BB.PUBLISH_YEAR BETWEEN '")+ sGetData[14]+ _T("' AND '")+sGetData[15]+_T("' ");
		}
		else
		{
			sWhere += _T(" BB.PUBLISH_YEAR = '")+ sGetData[14]+_T("' "); 
		}
		nSFieldCnt++;
	}

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
			nSFieldCnt++;
		}
	}

	CString sModeAlias;
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );

	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );

	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	if( sModeAlias.Compare( _T("상세검색") ) == 0 )
	{
		if( sWhere.IsEmpty() || nSFieldCnt == 0 )
		{
			INT ids = MessageBox( m_pParentMgr->m_hWnd , _T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
				_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
				, _T("검색 경고") , MB_ICONWARNING | MB_YESNO );
			if( ids == IDNO )
				return 1;		
		}
		return 0;
	}

	sGetData[7].TrimRight();
	
	if( !sGetData[7].IsEmpty() && (sGetData[7].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 0 == sGetData[7].Compare(_T("EMP")) || 0 == sGetData[7].Compare(_T("없음")) )
		{
			sWhere += _T(" BB.SEPARATE_SHELF_CODE IS NULL "); 
		}
		else
		{			
			sWhere += _T(" BB.SEPARATE_SHELF_CODE = '")+ sGetData[7]+_T("' "); 
		}
	}

	sGetData[9].TrimRight();
	if( !sGetData[9].IsEmpty() && (sGetData[9].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" IB.CLASS_NO_TYPE = '") + sGetData[9] + _T("'");
	}
	
	if( !sGetData[3].IsEmpty() && (sGetData[3].Compare(_T("적용안함") )!=0))
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.SHELF_CHANGE_REASON_CODE = '") + sGetData[3] + _T("' ");

		nSFieldCnt++;
	}

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
		nSFieldCnt++;
	}

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
		nSFieldCnt++;
	}

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
		nSFieldCnt++;

	}

	CMakeSearchData MakeSearchData(m_pParentMgr);

	bool bIsEqualSearch;
	if( !sGetData[19].IsEmpty() )
	{
		bIsEqualSearch = FALSE;

		if( sGetData[19].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
			
		MakeSearchData.GetKeyWord(sGetData[19]);
		if( !bIsEqualSearch ) 
			sGetData[19] += _T("*");

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		
		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH(  , BO_IDX_TITLE, '") + sGetData[19] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , BO_IDX_TITLE, '%s' ) "), sGetData[ 19 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[19] + _T("',NULL) > 0 ");

		nSFieldCnt++;
	}
	if( !sGetData[20].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		if( sGetData[20].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		MakeSearchData.GetKeyWord(sGetData[20]);

		if( !bIsEqualSearch ) 
			sGetData[20] += _T("*");

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH(  , BO_IDX_AUTHOR, '") + sGetData[20] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '%s' ) "), sGetData[ 20 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[20] + _T("',NULL) > 0 ");


		nSFieldCnt++;
	}
	if( !sGetData[21].IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		if( sGetData[21].GetAt(0) == '\"' )
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[21]);
		if( !bIsEqualSearch ) 
			sGetData[21] += _T("*");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( 1 == m_pParentMgr->m_pInfo->GetSearchIndexType() )
		{
			if( fp )
				sWhere += _T(" &ECOSEARCH(  , BO_IDX_PUBLISHER , '") + sGetData[21] + _T("') ");
			else
			{
				sWhere = sWhere.Left( sWhere.GetLength() - 4 );
				sBuf.Format( _T(" &ECOSEARCH( , BO_IDX_PUBLISHER, '%s' ) "), sGetData[ 21 ] );
				whereList.AddTail( sBuf );
			}
		}
		else
			sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER , '") + sGetData[21] + _T("',NULL) > 0 ");

		nSFieldCnt++;

	}	

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

	CString sYear;
	m_pParentMgr->GetControlData( sCMAlias , _T("원부작성년도"), sYear );
	if( !sYear.IsEmpty() )
	{
		if( sWhere.GetLength() )
			sWhere += _T(" AND ");

		sBuf.Format( _T(" ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' )"), sYear );
		sWhere += sBuf;
	}

	if( !sGetData[23].IsEmpty() )
	{	
		INT ids = MessageBox( m_pParentMgr->m_hWnd , _T("다른조건이 사용되지 않은경우 검색시간이 오래 걸릴수 있습니다.\r\n")
			_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
			, _T("검색 경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;			
	
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

		nSFieldCnt++;

		sWhere += _T(" ) = 0 ) ");
	}
	
	INT cnt = whereList.GetCount();
	if( !fp && cnt )
	{
		if( 1 == cnt )
			sBuf.Format( _T(" AND IB.REC_KEY IN %s  "), whereList.GetAt( whereList.FindIndex( 0 ) ) );
		else
		{
			sBuf = _T(" AND IB.REC_KEY IN ( ") +  whereList.GetAt( whereList.FindIndex( 0 ) );
			for( INT i = 1; i < cnt; i++ )
				sBuf = sBuf + _T("  INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );						
			sBuf += _T(" ) ");
		}

		sWhere += sBuf;
	}
	
	if( fp )
		fclose(fp);
	
	if( sWhere.IsEmpty() || nSFieldCnt == 0 )
	{
		INT ids = MessageBox( m_pParentMgr->m_hWnd , _T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
			_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
			, _T("검색 경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;		
	}

	return 0;

EFS_END
	return -1;
}

INT CBL_BOSHELF::SearchFileBO_LOC_1310()
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

	BOOL bChkDiscardDamage = FALSE;
	CString strStartAcqDate = _T("");
	CString strEndAcqDate = _T("");
	CString strStartLastLoanDate = _T("");
	CString strEndLastLoanDate = _T("");

	CString strAcqDateQuery = _T("");

	bChkDiscardDamage = SanFileDlg.GetChkDiscardDamage();
	strStartAcqDate = SanFileDlg.GetStartAcqDate();
	strEndAcqDate = SanFileDlg.GetEndAcqDate();
	strStartLastLoanDate = SanFileDlg.GetStartLastLoanDate();
	strEndLastLoanDate = SanFileDlg.GetEndLastLoanDate();

	CString strLastLoanDateQuery = _T("");

	if( TRUE == bChkDiscardDamage )
	{
		if( !strStartAcqDate.IsEmpty() )
		{
			strAcqDateQuery.Format(_T(" BB.INPUT_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS') AND"), strStartAcqDate, strEndAcqDate);
		}
		
		if( !strStartLastLoanDate.IsEmpty() )
		{
			strLastLoanDateQuery.Format(_T("LOAN_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), strStartLastLoanDate, strEndLastLoanDate);
		}
	}

	CESL_DataMgr pDM;

	ids = SanFileDlg.GetSanFileDM( &pDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
	
	CESL_DataMgr TmpParentDM;
	CESL_DataMgr *pTmpParentDM = &TmpParentDM;

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_1300") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
	pParentDM->FreeData();

	CLocCommonAPI::MakeCopyDM( m_pParentMgr , pParentDM , pTmpParentDM );

	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	INT nRowCount = pDM.GetRowCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		CString sRegNo;
		ids = pDM.GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );

		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");

		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (pDM.GetRowCount()-1)) ) 
		{
			CString strBookKeyQuery = _T("");
			CString sWhere = _T("");
			if( !strStartLastLoanDate.IsEmpty() )
			{
				strBookKeyQuery.Format(_T("SELECT DISTINCT BOOK_KEY FROM CO_LOAN_TBL WHERE %s AND REG_NO IN( %s ) UNION SELECT DISTINCT BOOK_KEY FROM LS_WORK_T01 WHERE %s AND REG_NO IN( %s )"), strLastLoanDateQuery, sRegNoList, strLastLoanDateQuery, sRegNoList);
				sWhere = _T(" BB.WORKING_STATUS IN (")+CString(HOLD_SEARCH_CONDITION) +_T(")")  + _T(" AND BB.REC_KEY IN ( ") + strBookKeyQuery + _T(")");
			}
			else
			{
				sWhere = _T(" BB.REG_NO IN (") + sRegNoList + _T(")") + _T(" AND BB.WORKING_STATUS IN (")+CString(HOLD_SEARCH_CONDITION) +_T(")") ;
			}

			if( !strAcqDateQuery.IsEmpty() )
			{
				sWhere = strAcqDateQuery + sWhere;
			}
			//===================================================
			
			ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pTmpParentDM , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
			
			ids = AddDM( pParentDM , pTmpParentDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );

			sRegNoList.Empty();
		}
	}
	
	CStdioFile fInputFile;
	CString sLine;
	INT nSortOrder = 1;
	fInputFile.Open( _T("..\\tunning\\TemporaryRegNoFile.txt") , CFile::modeRead | CFile::typeBinary );
		TCHAR cUni;
		fInputFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fInputFile.SeekToBegin();
		}
	
	while( fInputFile.ReadString(sLine) )
	{
		for ( INT nIdx = 0 ; nIdx < pParentDM->GetRowCount() ; nIdx++ )
		{
			sLine.TrimLeft();
			sLine.TrimRight();
			if ( pParentDM->GetCellData(_T("BB_등록번호"), nIdx ) == sLine )
			{
				CString strSortOrder;
				strSortOrder.Format(_T("%d"), nSortOrder );
				pParentDM->SetCellData(_T("정렬순서"), strSortOrder, nIdx );
				nSortOrder++;
				break;
			}			
		}
	}

	pParentDM->SORT(_T("정렬순서"));
	CString sCMAlias = _T("CM_BO_LOC_1300");
	ids = GridDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );

	CString sRegNo;

	CStdioFile fFile;

	TCHAR sPath[500];
	_tgetcwd(sPath,500);
	CString ssPath = sPath;
	ssPath = ssPath.Mid(0,ssPath.GetLength()-4 );
	CString sFileName = ssPath + CString(_T("\\검색에실패한등록번호\\검색에실패한등록번호.txt"));

	_tmkdir(_T("..\\검색에실패한등록번호"));
	CFileException ex;
	ids = fFile.Open( _T("..\\검색에실패한등록번호\\검색에실패한등록번호.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary  , &ex );
	if( ids == 0 )
	{
		TCHAR sFEMsg[500];
		ex.GetErrorMessage(sFEMsg ,500);
		CString sEMsg;
		sEMsg.Format(_T("파일을 생성하는데 실패했습니다.[%s]") , sFEMsg  );
		AfxMessageBox(sEMsg);
		return 0;
	}

	if( 0 == fFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fFile.Write( &cUni, sizeof(TCHAR));
	}
	
	bool bIsNotEqual = FALSE;

	for( i = 0 ; i < nRowCount ; i++ )
	{
		ids = pDM.GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );

		ids = CLocCommonAPI::GetCellData( m_pParentMgr , pParentDM , _T("BB_등록번호") , sRegNo , _T("BB_등록번호") , sRegNo );
		if( ids == 1 )
		{
			bIsNotEqual = TRUE;
			fFile.WriteString(sRegNo);
			fFile.WriteString(_T("\r\n"));
		}
		
	}

	fFile.Close();

	if( bIsNotEqual )
	{
		CString sMsg;
		sMsg.Format(_T("불러오지 못한 등록번호는 '%s'파일에 기록하였습니다.") , sFileName );
		AfxMessageBox( sMsg );		
	}

	CString sMsg;
	sMsg.Format( _T("책수 : %d") , pParentDM->GetRowCount() );
	m_pParentMgr->GetParentFrame()->SendMessage( WM_USER + 2004, NULL, ( LPARAM )sMsg.GetBuffer( 0 ) );

	return 0;

	EFS_END
	return -1;

}

// 코드 일괄 변경
INT CBL_BOSHELF::ChangeCode()
{
	EFS_BEGIN

	INT ids;
	INT nGetRowCount = 0;
	CString sParentCMAlias = _T("CM_BO_LOC_1300");
	CString sParentDMAlias = _T("DM_BO_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::ChangeCode()") );

	CString sCenterUploadYN;
	CString sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
		_T(" WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='센터업로드사용여부'")
		_T(" AND VALUE_NAME='CENTER_UPLOAD' AND MANAGE_CODE = UDF_MANAGE_CODE ");
	pParentDM->GetOneValueQuery(sQuery, sCenterUploadYN);

	INT nIndex;

	CString sCodeNameCode;
	CString sCodeName;
	CString sCodeValue;
	CString sDMColumnAlias;
	ids = GetInputControlData( sCodeNameCode , sCodeName , sCodeValue , sDMColumnAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::ChangeCode()") );
	if( ids > 0 ) return 1;

	pGrid->SelectGetTailPosition();
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;
	
	nGetRowCount= pGrid->SelectGetCount();

	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
	ProgressBar->Create( m_pParentMgr );
	ProgressBar->CenterWindow( m_pParentMgr );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nGetRowCount-1;
	if( nGetRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);

	bool bIsChangeDeleteBook;
	CString sChangeDeleteBook;
	CString sCenterUpdate;	
	
	GetTempData( _T("제적자료변경허용") , sChangeDeleteBook );
	GetTempData( _T("센터변경")			, sCenterUpdate );	

	if( sChangeDeleteBook.Compare(_T("Y")) == 0 )
		bIsChangeDeleteBook = TRUE;
	else
		bIsChangeDeleteBook = FALSE;

	CString sRepresentBookYN;
	CString sWorkingStatus;

	CString sUploadStatus;

	INT repairBindingCnt = 0;
	
	INT nNoChangeCnt=0;
	INT nNoDeleteBookChangeCnt = 0;
	INT nChangeCnt=0;
	INT	nCenterBatchUpdateCnt = 0;
	INT nOneByOneCheckCnt = 0;
	INT nUploadNoCnt = 0;
	for( INT i = 0 ; i < nGetRowCount ; i++ )
	{
		ProgressBar->ChangeProgressBar(i);
		nIndex = pGrid->SelectGetIdx();

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_센터업로드") , sUploadStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );


		if ( (sWorkingStatus.Compare(_T("BOL218O"))==0 || sWorkingStatus.Compare(_T("BOL219O"))==0 ) && !(sCodeNameCode == _T("0") && sCodeValue == _T("BOL112N" )) )
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;									
		}	

		if ( sWorkingStatus.Compare(_T("BOL213O"))==0 && sCodeNameCode == _T("0") )
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;									
		}	

		if( (sWorkingStatus.Compare(_T("BOL211O"))==0) || 
			(sWorkingStatus.Compare(_T("BOL212O"))==0) || 
			(sWorkingStatus.Compare(_T("BOL214O"))==0) ||			
			(sWorkingStatus.Compare(_T("BOL411O"))==0) || 
			(sWorkingStatus.Compare(_T("BOL511O"))==0) || 			
			(sWorkingStatus.Compare(_T("BOL611O"))==0) || 
			(sWorkingStatus.Compare(_T("BOL215O"))==0)		)		
		{
			pGrid->SelectGetPrev();
			nNoChangeCnt++;
			continue;
		}

		if( _ttoi( pParentDM->GetCellData( _T("수리제본"), nIndex ) ) )
		{
			pGrid->SelectGetPrev();
			repairBindingCnt++;
			continue;
		}

		if( _ttoi(sCodeNameCode) == 1 )
		{			
			if( sWorkingStatus.Compare(_T("BOL115O")) == 0 )
			{
				if( !bIsChangeDeleteBook )
				{
					if( sCodeValue.Compare(_T("CL")) != 0 )
					{
						pGrid->SelectGetPrev();
						nNoDeleteBookChangeCnt++;
						continue;			
					}
				}
			}
		}

		CString sBookRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::ChangeCode()") );

		CString sSpeciesRecKey;
		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BS_종KEY") , sSpeciesRecKey , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_BOSHELF::ChangeCode()") );

		ids = m_pParentMgr->GetDataMgrData( sParentDMAlias , _T("BB_대표책값") , sRepresentBookYN , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeCode()") );

		sRecKeyList = sBookRecKey;

		CString sInsetLogYN;
		ids = ChangeCodeDBUpdate( sCodeNameCode, sCodeName, sCodeValue, sRecKeyList, sSpeciesRecKey, sRepresentBookYN, sInsetLogYN  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_BOSHELF::ChangeCode()") );
		sRecKeyList.Empty();	


		if ( sCenterUploadYN == _T("Y") && sCodeValue.Compare(_T("BOL218O")) != 0  && sCodeValue.Compare(_T("BOL219O")) != 0)

		{

			if ( !( sUploadStatus == _T("0") || sUploadStatus == _T("5") ) )
			{
				nUploadNoCnt++;
			}
			else
			{
				if ( sCenterUpdate == _T("Y") )
				{
					
					if(ChangeLocalUseLimit(sCodeName, sCodeValue, nIndex) == TRUE)
					{
						nCenterBatchUpdateCnt ++;
					}
					else
					{
						nOneByOneCheckCnt++;
					}
					
				}
				else
				{
					if ( sInsetLogYN == _T("Y") || sCenterUpdate == _T("Y") )
					{
						nOneByOneCheckCnt++;
					}
				}
			}
		}
		
		ids = DMCodeChangeUpdate( sDMColumnAlias , sCodeValue , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_BOSHELF::ChangeCode()") );

		ids = CMGridCodeChangeUpdate( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_BOSHELF::ChangeCode()") );

		if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
		nChangeCnt++;
	}

	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;

	CString sMsg;
	CString strTmp;
	if( (nChangeCnt > 0) && (nNoChangeCnt > 0) )
		sMsg.Format( _T("총 '%d'건의 자료중 처리된 자료가 '%d'건이며 \r\n")
					_T("처리되지 못한 자료가 '%d'건입니다."),
		nChangeCnt+nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt , nChangeCnt , nNoChangeCnt+repairBindingCnt+nNoDeleteBookChangeCnt);
	else if( (nChangeCnt > 0) && (nNoChangeCnt < 1) && ( repairBindingCnt < 1 ) && ( nNoDeleteBookChangeCnt < 1 ) )
		sMsg.Format( _T("모두 '%d'건이 처리되었습니다.") , nChangeCnt );

	if( (nNoChangeCnt > 0) )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("■ 대출중인 자료이므로 '%d'건이처리되지 못했습니다.") , nNoChangeCnt );
		sMsg += strTmp;		
	}		

	if( repairBindingCnt > 0 )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("■ 미처리자료(수리제본) : %d 건"), repairBindingCnt );
		sMsg += strTmp;		
	}

	if( nNoDeleteBookChangeCnt > 0 )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("■ 미처리자료(제적) : %d 건"), nNoDeleteBookChangeCnt );
		sMsg += strTmp;		
	}

	if ( _T("Y") != sCenterUploadYN ) 
	{
		if( !sMsg.IsEmpty() ) AfxMessageBox(sMsg);
		return 0;
	}

	if( nOneByOneCheckCnt > 0 && sCenterUpdate == _T("Y") )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("◆ 센터이용제한구분값 건별 변경 대상(다권본) : %d 건"), nOneByOneCheckCnt );
		sMsg += strTmp;		
	}

	if( nCenterBatchUpdateCnt > 0 && sCenterUpdate == _T("Y") )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("◆ 센터에 자동으로 변경한 자료(단권본) : %d 건"), nCenterBatchUpdateCnt );
		sMsg += strTmp;		
	}

	if( nUploadNoCnt > 0 && sCenterUpdate == _T("Y") )
	{
		if ( !sMsg.IsEmpty() ) sMsg += _T("\n");
		strTmp.Format( _T("◆ 센터에 올리지 않은 자료 : %d 건"), nUploadNoCnt );
		sMsg += strTmp;		
	}	


	if( !sMsg.IsEmpty() ) AfxMessageBox(sMsg);
	
	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_1340");

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
	else if( sCodeNameCode.Compare( _T("3") ) == 0 )
	{
		sCodeName = _T("ACQ_CODE");
		sCodeComboAlias = _T("수입구분코드");
		sDMColumnAlias = _T("수입구분");
	}
	else if( sCodeNameCode.Compare( _T("4") ) == 0 )
	{
		sCodeName = _T("USE_OBJECT_CODE");
		sCodeComboAlias = _T("이용대상구분코드");
		sDMColumnAlias = _T("이용대상구분");
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , sCodeComboAlias , sCodeValue , -1 , 0  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetInputControlData( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue , CString &sDMColumnAlias )") );

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

INT CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode, CString &sCodeName, CString &sCodeValue, CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN, CString &sInsetLogYN)
{
	EFS_BEGIN

	INT ids;

	sInsetLogYN = _T("N");

	CString sDMAlias = _T("DM_BO_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	CString sLogColumnName;

	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
		sLogColumnName = _T("LAST_SHELF_STATUS_CHANGE_DATE");

	else if( sCodeNameCode.Compare(_T("1")) == 0 ) 
		sLogColumnName = _T("LAST_USELIMITCODE_CHANGE_DATE");

	else if( sCodeNameCode.Compare(_T("2")) == 0 )
		sLogColumnName = _T("LAST_MANAGE_CODE_CHANGE_DATE");	

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pDM );

	CString strQuery;
	CString strTmp;
	INT		nOldServiceBookCnt;
	INT		nNewServiceBookCnt;

	strQuery.Format(_T("SELECT ECO$F_GET_SERVICE_VOL_COUNT$I(%s) FROM DUAL"), sSpeciesKey);
	pDM->GetOneValueQuery(strQuery, strTmp );
	nOldServiceBookCnt = _ttoi(strTmp.GetBuffer(0));

	pDM->StartFrame();
	pDM->InitDBFieldData();	

	CString strBookUpdateQuery;
	CString strBookUpdateField;
	CString strBookField;
	CString strWorkingStatus;
	strBookField.Format(_T("%s='%s'"), sCodeName, sCodeValue );
	strBookUpdateField = strBookField;
	if ( sCodeName == _T("WORKING_STATUS") && sCodeValue == _T("BOL115O") )
	{
		strBookField.Format(_T(",USE_LIMIT_CODE='CL'") );
		strBookUpdateField += strBookField;
	}

	if( !sLogColumnName.IsEmpty() )
	{
		strBookField.Format(_T(",%s=sysdate"), sLogColumnName );
		strBookUpdateField += strBookField;
	}
	strBookField.Format(_T(",LAST_WORK='%s'"), m_pParentMgr->GetWorkLogMsg( _T("코드일괄변경"), __WFILE__, __LINE__ ));

	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
	{		
		strWorkingStatus.Format(_T("'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL411O', 'BOL511O','BOL611O'") );	
	}
	else if( sCodeNameCode.Compare(_T("1")) == 0 ) 
	{
		strWorkingStatus.Format(_T("'BOL211O', 'BOL212O', 'BOL214O', 'BOL215O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O','BOL611O'") );			
	}
	else
	{
		strWorkingStatus.Format(_T("'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O', 'BOL611O'") );		
	}

	if ( sCodeName == _T("MANAGE_CODE") )
	{
		if(!strBookUpdateField.IsEmpty()) strBookUpdateField += _T(", ");
		strBookUpdateField += _T(" ACCESSION_REC_KEY = NULL ");
	}
	
	strBookUpdateQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s WHERE REC_KEY IN ( %s ) AND WORKING_STATUS NOT IN( %s );"), strBookUpdateField, sRecKeyList, strWorkingStatus );

	MobileApi.AddFrame(strBookUpdateQuery);

	CString sAppendixChange;
	GetTempData( _T("부록변경")			, sAppendixChange );	
	if ( sAppendixChange == _T("Y") )
	{
		CString strAppendixUpdateQuery;
		CString strUpdateField;
		CString strField;

		strField.Format(_T("%s='%s'"), sCodeName, sCodeValue );
		strUpdateField = strField;

		if ( sCodeName == _T("WORKING_STATUS") && sCodeValue == _T("BOL115O") )
		{
			strField.Format(_T(",USE_LIMIT_CODE='CL'") );
			strUpdateField += strField;
		}

		if( !sLogColumnName.IsEmpty() )
		{
			strField.Format(_T(",%s=sysdate"), sLogColumnName );
			strUpdateField += strField;
		}

		strAppendixUpdateQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s WHERE SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG='A' AND WORKING_STATUS NOT IN ( %s );"), strUpdateField, sSpeciesKey, strWorkingStatus );
			
		MobileApi.AddFrame(strAppendixUpdateQuery);
	}

	
	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );
	pDM->EndFrame();	
	MobileApi.EndLog();

	if( sCodeNameCode.Compare(_T("3")) == 0 )
		return 0;
	
	if( sCodeNameCode.Compare(_T("1")) == 0 || sCodeName == _T("WORKING_STATUS") && sCodeValue == _T("BOL115O") )
	{
		ids = ChangeUseLimitCode( sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ChangeCodeDBUpdate") );		
	}

	strQuery.Format(_T("SELECT ECO$F_GET_SERVICE_VOL_COUNT$I(%s) FROM DUAL"), sSpeciesKey);
	pDM->GetOneValueQuery(strQuery, strTmp );
	nNewServiceBookCnt = _ttoi(strTmp.GetBuffer(0));

	if ( nOldServiceBookCnt != nNewServiceBookCnt )
	{
		InsertUseLimitTempLog(sSpeciesKey);	
		sInsetLogYN = _T("Y");
	}

	CString sSpeciesCodeValue;
	if( sCodeNameCode.Compare(_T("0")) == 0 ) 
	{
		pDM->StartFrame();
		CLocCommonAPI::MakeIdxWokingStatusFrame( pDM , sSpeciesKey , 0 , m_pParentMgr );
		pDM->SendFrame();
		pDM->EndFrame();
		return 0;
	}

	CKolas2up_MobileApi MobileApi2;
	MobileApi2.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi2.StartLog();
	MobileApi2.SetDM( pDM );
	
	pDM->StartFrame();

	if( sRepresentBookYN.Compare(_T("Y"))==0 )
	{
		if ( sCodeName == _T("USE_OBJECT_CODE") ) sCodeName = _T("USE_OBJ_CODE");
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( sCodeName , _T("STRING") , sCodeValue );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		MobileApi2.MakeUpdateFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	}

	CString sSpeciesFunc;
	sSpeciesFunc.Format( _T("GET_DISP_BO_BOOK_INFO(%s,'MANAGE_CODE')") , sSpeciesKey );
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("NUMERIC") , sSpeciesFunc );

	MobileApi2.MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 0 , m_pParentMgr , &MobileApi2 );
	
	ids = MobileApi2.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCodeDBUpdate( CString &sCodeNameCode , CString &sCodeName , CString &sCodeValue  , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();

	return 0;	

	EFS_END
	return -1;

}

INT CBL_BOSHELF::ChangeShelf( INT nMode )
{
	EFS_BEGIN

	INT ids;

	INT nPreIndex;

	CString sShelfDate;
	CString sShelfWorker;

	CString sDMAlias = _T("DM_BO_LOC_1300");
	CString sCMAlias = _T("CM_BO_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

	CESL_Grid *pGrid = ((CESL_Grid*)pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

	nPreIndex = pGrid->GetIdx();

	CString sGetInputData[6];
	ids = GetInputDataBO_LOC_1330( sGetInputData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
	if( ids > 0 ) return ids;


	INT nIndex;
	INT nMaxUpdateRecordCnt = 50;
	CString sRecKeyList;
	CString sSpeciesKey;
	CString sRepresentBookYN;

	INT nNoChangeCnt=0;
	INT nChangeCnt=0;
	CString sWorkingStatus;

	if( nMode == 0 )
	{
		nIndex = pGrid->GetIdx();

		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		
	
		if( (sWorkingStatus.Compare(_T("BOL212O"))==0) || (sWorkingStatus.Compare(_T("BOL213O"))==0) || (sWorkingStatus.Compare(_T("BOL511O"))==0) || (sWorkingStatus.Compare(_T("BOL611O"))==0) ||
			(sWorkingStatus.Compare(_T("BOL214O"))==0) || (sWorkingStatus.Compare(_T("BOL215O"))==0) || (sWorkingStatus.Compare(_T("BOL218O"))==0) || (sWorkingStatus.Compare(_T("BOL219O"))==0) ||
			(sWorkingStatus.Compare(_T("BOL411O"))==0))
		{
			nNoChangeCnt++;
		}
		else
		{
			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_종KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
			
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_대표책값") , sRepresentBookYN , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeCode()") );

			sRecKeyList = sBookRecKey;
			ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey , sRepresentBookYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
			sRecKeyList.Empty();

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			nChangeCnt++;
		}
	}

	else
	{
		INT nGetRowCount = pGrid->SelectGetCount();

		CBlLocProgressBarDlg *ProgressBar;
		ProgressBar = new CBlLocProgressBarDlg( m_pParentMgr );
		ProgressBar->Create( m_pParentMgr );
		ProgressBar->CenterWindow( m_pParentMgr );
		ProgressBar->ShowWindow( SW_SHOW );
		INT nUpper = nGetRowCount-1;
		if( nGetRowCount == 1 ) nUpper = 1;
		ProgressBar->InitRange(nUpper);

		pGrid->SelectGetTailPosition();

		for( INT i = 0 ; i < nGetRowCount ; i++ )
		{
			ProgressBar->ChangeProgressBar(i);
			
			nIndex = pGrid->SelectGetIdx();

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_배가상태") , sWorkingStatus , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
			CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

			if( (sWorkingStatus.Compare(_T("BOL212O"))==0) || (sWorkingStatus.Compare(_T("BOL214O"))==0) || (sWorkingStatus.Compare(_T("BOL215O"))==0) || 				
				(sWorkingStatus.Compare(_T("BOL218O"))==0) || (sWorkingStatus.Compare(_T("BOL219O"))==0) || 
				(sWorkingStatus.Compare(_T("BOL511O"))==0) || (sWorkingStatus.Compare(_T("BOL611O"))==0) || (sWorkingStatus.Compare(_T("BOL411O"))==0) )
			{
				nNoChangeCnt++;
				pGrid->SelectGetPrev();
				continue;
			}

			CString sBookRecKey;
			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_책KEY") , sBookRecKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BS_종KEY") , sSpeciesKey , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_대표책값") , sRepresentBookYN , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::ChangeCode()") );

			sRecKeyList += sBookRecKey;

			ids = RecordChangeShelfProc( sGetInputData , sRecKeyList , sSpeciesKey , sRepresentBookYN  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
			sRecKeyList.Empty();

			ids = DMUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );
			
			ids = CMGridUpdate( sGetInputData , nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

			if( i !=  nGetRowCount-1 ) pGrid->SelectGetPrev();
			nChangeCnt++;
		}
		ProgressBar->DestroyWindow();
		if( ProgressBar != NULL ) delete ProgressBar;
	}

	m_pParentMgr->AllControlDisplay( _T("CM_BO_LOC_1330") , nPreIndex );

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

INT CBL_BOSHELF::GetInputDataBO_LOC_1330( CString *sGetInputData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_1330");

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
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::GetInputDataBO_LOC_1330( CString *sGetInputData )") );

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


INT CBL_BOSHELF::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList , CString sSpeciesKey , CString sRepresentBookYN )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_1300");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );


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

	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	MobileApi.MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKeyList );

	CString sSpceciesFunc;
	sSpceciesFunc.Format( _T("GET_DISP_BO_BOOK_INFO(%s,'SHELF_LOC_CODE')") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("idx_hold_place") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	sSpceciesFunc.Format( _T("GET_BO_MAIN_SHELF_CODE(%s)") , sSpeciesKey);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("main_place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("place_info") , _T("NUMERIC") , sSpceciesFunc );
	pDM->MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	if( sRepresentBookYN.Compare(_T("Y")) == 0 )
	{
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , sGetInputData[0] );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	}

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , 0 , m_pParentMgr , &MobileApi );

	ids = MobileApi.SendFrame();
	if( ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::RecordChangeShelfProc( CString *sGetInputData , CString &sRecKeyList )") );

	pDM->EndFrame();

	MobileApi.EndLog();

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_1300");

	TCHAR *sDMColumnAlias[5] =
	{
		_T("BB_배가기호") , _T("BB_배가변경일자") , _T("BB_배가변경지시자") , _T("BB_배가변경사유") , _T("BB_배가변경사유내용")
	};

	for( INT i = 0 ; i < 5 ; i++ )
	{
		ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias[i] , sGetInputData[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}
	
	CString sCodeDesc;
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sGetInputData[0] , sCodeDesc  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	ids = m_pParentMgr->SetDataMgrData( sDMAlias , _T("BB_자료실명") , sCodeDesc , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );
	return 0;


	EFS_END
	return -1;

}

INT CBL_BOSHELF::CMGridUpdate( CString *sGetInputData , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_1300");
	CString sGridAlias = _T("MainGrid");
	CString sDMAlias;	

	if( sCMAlias.Compare(_T("CM_BO_LOC_1300"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1300");		
	}
	else if( sCMAlias.Compare(_T("CM_BO_LOC_1100"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1100");		
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	TCHAR *sDMColumnAlias[4] =
	{
		_T("BB_배가기호") , _T("BB_자료실명") , _T("BB_배가변경사유내용"), _T("BB_배가변경일자") 
	};

	INT nColIndex[4] = 
	{
		-1 , -1 , -1, -1 
	};

	CString sData[4];

	sData[0] = sGetInputData[0];
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sGetInputData[0] , sData[1]  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );
	sData[2] = sGetInputData[4];
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("BB_배가변경일자") , sData[3] , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::ChangeShelf( INT nMode )") );

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , sCMAlias , sGridAlias , sDMColumnAlias , 4 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	for( INT i = 0 ; i < 4 ; i++ )
	{
		if( nColIndex[i] == -1 ) continue;
		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}

	return 0;


	EFS_END
	return -1;

}

INT CBL_BOSHELF::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )
{
	EFS_BEGIN

	INT ids;
	CString sGetData;

	const INT nColumnCnt = pSourceDM->GetColCount();
	CString *sColumnAlias;
	sColumnAlias = NULL;
	sColumnAlias = new CString[nColumnCnt];

	ids = pSourceDM->GetAllAlias( sColumnAlias , nColumnCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

	INT nRowCount = pSourceDM->GetRowCount();
	for( INT i = 0 ; i < nRowCount ;i++ )
	{
		for( INT j = 0 ; j < pDestDM->GetColCount() ; j++ )
		{
			if( j == 0 ) pDestDM->InsertRow( -1 );
			ids = pSourceDM->GetCellData( i , j , sGetData );
			if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );

			ids = pDestDM->SetCellData( sColumnAlias[j] , sGetData , pDestDM->GetRowCount()-1);			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::AddDM( CESL_DataMgr *pDestDM , CESL_DataMgr *pSourceDM )") );
			sGetData.Empty();
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::GetWorkingStatusDescToCode( CString &sCodeValue , INT nMode )
{
	EFS_BEGIN

	INT ids;

	ids = m_pLocCommonAPI->GetWorkingStatusDescToCode( sCodeValue , nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetWorkingStatusDescToCode") );
	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::GridDisplay( CString sCMAlias )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias;
	CString sFieldAlias;

	if( sCMAlias.Compare(_T("CM_BO_LOC_1300"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1300");
		sFieldAlias = _T("BB_자료실명");
	}
	else if( sCMAlias.Compare(_T("CM_BO_LOC_1100"))==0)
	{
		sDMAlias = _T("DM_BO_LOC_1100");
		sFieldAlias = _T("BB_배가위치부호");
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , sFieldAlias  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

	CMarcMgr* pMarcMgr = m_pParentMgr->m_pInfo->pMarcMgr;
	if( sCMAlias.Compare(_T("CM_BO_LOC_1300"))==0) 
	{
		CMarc Marc;
		CString sMarc;
		CString strField;

		CString sCodeValue;
		for( INT i = 0 ; i < pDM->GetRowCount() ;i++ )
		{
			ids = pDM->GetCellData( _T("BB_배가상태") , i , sCodeValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );
			ids = GetWorkingStatusDescToCode( sCodeValue , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );
			ids = pDM->SetCellData( _T("BB_배가상태") , sCodeValue , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

			CString data[ 4 ];
			data[ 0 ] = pDM->GetCellData( _T("PAGE"), i );
			data[ 1 ] = pDM->GetCellData( _T("PHYSICAL_PROPERTY"), i );
			data[ 2 ] = pDM->GetCellData( _T("BOOK_SIZE"), i );
			data[ 3 ] = pDM->GetCellData( _T("ACCOMP_MAT"), i );
			CString output;

			if( !data[ 0 ].IsEmpty() )
				output += data[ 0 ];
			if( !data[ 1 ].IsEmpty() )
				output = output + _T(":") + data[ 1 ];
			if( !data[ 2 ].IsEmpty() )
				output = output + _T(";") + data[ 2 ];
			if( !data[ 3 ].IsEmpty() )
				output = output + _T("+") + data[ 3 ];
			pDM->SetCellData( _T("형태사항"), output, i );

		}
		
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("관리구분") , _T("BB_관리구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("이용제한구분") , _T("BB_이용제한구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("형식구분") , _T("형식구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );
		
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("입수구분") , _T("수입구분")  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );
		
	}

	if( sCMAlias.Compare(_T("CM_BO_LOC_1300"))==0)
	{
		ids = MakeShelfCode( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

		ids = DisplayCopyBook( sDMAlias );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

	}

	ids = m_pParentMgr->AllControlDisplay( sCMAlias , -1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CBL_BOSHELF::GridDisplay( CString sCMAlias )") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::MakeShelfCode( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::MakeShelfCode(m_pParentMgr,sDMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeShelfCode") );

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::DisplayCopyBook( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DisplayCopyBook( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sCopyCode;
	CString sTmpCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		sCopyCode.Empty();

		ids = pDM->GetCellData( _T("BB_원/복본구분") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );
		
		if( !sTmpCode.IsEmpty() && (sTmpCode.Compare(_T("0"))!=0)) 
			sCopyCode = _T("복본");
		else
			sCopyCode = _T("원본");

		ids = pDM->SetCellData( _T("BB_원/복본구분") , sCopyCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );
	}

	return 0;


	EFS_END
	return -1;

}

INT CBL_BOSHELF::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )
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
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::GetUseLimitBossCode( CESL_DataMgr *pSpeciesDM , CString &sSpeciesCodeValue )") );
		
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

INT CBL_BOSHELF::CMGridCodeChangeUpdate(INT nIndex)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_1300");

	CString sCMAlias = _T("CM_BO_LOC_1300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );

	const INT nCnt = 6;

	TCHAR *sDMFieldAlias[nCnt] =
	{
		_T("BB_관리구분") , _T("BB_배가상태") , _T("BB_이용제한구분") , _T("수입구분"), _T("BB_이용대상구분"), _T("제적일자")
	};

	INT nColIndex[nCnt] =
	{
		-1,-1,-1,-1,-1
	};

	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMFieldAlias , nCnt , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") ); 

	CString sData;
	
	for( INT i = 0 ; i < 6 ; i++ )
	{
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , sDMFieldAlias[i] , sData , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") ); 

		if( nColIndex[i] == -1 || nColIndex[i] == 0) continue;

		ids = pGrid->SetAt( nIndex , nColIndex[i] , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::DMUpdate( CString *sGetInputData , INT nIndex )") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_1300");

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
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("이용제한구분") , sCodeValue , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("수입구분")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("입수구분") , sCodeValue , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}
	else if( sDMColumnAlias.Compare(_T("이용대상구분")) == 0 )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(  _T("이용대상구분") , sCodeValue , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}	

	ids = m_pParentMgr->SetDataMgrData( sDMAlias , sDMColumnAlias , sCodeValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );

	if ( sCodeValue == _T("제적자료") )
	{
		CString result;
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

		ids = m_pParentMgr->SetDataMgrData( sDMAlias , _T("BB_이용제한구분") , _T("사서제한") , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );

		ids = m_pParentMgr->SetDataMgrData( sDMAlias , _T("제적일자") , result , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DMCodeChangeUpdate( CString sDMColumnAlias , CString sCodeValue , INT nIndex )") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::DetailView()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_1300");
	CString sParentDMAlias = _T("DM_BO_LOC_1300");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BOSHELF::ChangeCode()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BOSHELF::ChangeCode()") );

	if( pParentDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("자료를 검색해주십시오") );
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

	CBOBookDetailViewerDlg Dlg(m_pParentMgr);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();

	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::ChangeUseLimitCode( CString sSpeciesKey )
{
	EFS_BEGIN

	INT ids;
	CString sPreUseLimitCode , sNewUseLimitCode;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_1300_USE_LIMIT_SPECIES"));
	if( pUseLimitDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeUseLimitCode") );

	CString sQuery;
	sQuery.Format( _T("SELECT USE_LIMIT_CODE FROM IDX_BO_TBL WHERE REC_KEY = %s") , sSpeciesKey );
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pUseLimitDM , sQuery , sPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeUseLimitCode") );

	sQuery.Format( _T(" SPECIES_KEY = %s ") , sSpeciesKey );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pUseLimitDM , sQuery );

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

	IsEqual = CompareUseLimtCodes( saPreUseLimitCode , saNewUseLimitCode );

	if( IsEqual ) return 0;

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.StartLog();
	MobileApi.SetDM( pUseLimitDM );

	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sNewUseLimitCode );
	MobileApi.MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );	

	CString sBossPreUseLimitCode , sBossNewUseLimitCode;
	ids = GetBossUseLimitCode( sBossPreUseLimitCode , saPreUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("ChangeUseLimitCode") );
	ids = GetBossUseLimitCode( sBossNewUseLimitCode , saNewUseLimitCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("ChangeUseLimitCode") );

	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("USE_LIMIT_CODE") , _T("STRING") , sBossNewUseLimitCode );
	MobileApi.AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	MobileApi.MakeUpdateFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

	CLocCommonAPI::MakeIdxTmpTblInsertFrame( pUseLimitDM , sSpeciesKey , 0 , m_pParentMgr , &MobileApi );	

	ids = MobileApi.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ChangeUseLimitCode") );
	ids = pUseLimitDM->EndFrame();

	MobileApi.EndLog();
	return 0;

	EFS_END
	return -1;

}

INT CBL_BOSHELF::MakeArrayUseLimitCode( CString sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

INT CBL_BOSHELF::GetBossUseLimitCode( CString &sUseLimitCode , CArray <CString,CString> &saUseLimitCode )
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

bool CBL_BOSHELF::CompareUseLimtCodes( CArray <CString,CString> &saPreUseLimitCode , CArray <CString,CString> &saNewUseLimitCode )
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

INT CBL_BOSHELF::InsertUseLimitTempLog(CString sSpeciesKey)
{
	EFS_BEGIN

	CString sRecKey;
	CString sQuery;
	CString LogYN;
	INT		ids;

	CESL_DataMgr *pUseLimitDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_1300_USE_LIMIT_SPECIES"));
	if( pUseLimitDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ChangeUseLimitCode") );

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
	MobileApi.SetDM( pUseLimitDM );
	MobileApi.StartLog();	

	sQuery.Format(_T("SELECT UPDATE_YN FROM BO_USE_LIMIT_UPDATE_TBL WHERE SPECIES_KEY=%s "), sSpeciesKey );
	pUseLimitDM->GetOneValueQuery(sQuery, LogYN );
	if ( LogYN == _T("Y") ) 
	{
		pUseLimitDM->StartFrame();
		pUseLimitDM->InitDBFieldData();
		pUseLimitDM->AddDBFieldData( _T("UPDATE_YN") , _T("STRING") , _T("N") );
		pUseLimitDM->AddDBFieldData( _T("CREATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		MobileApi.MakeUpdateFrame( _T("BO_USE_LIMIT_UPDATE_TBL") , _T("SPECIES_KEY") , _T("NUMERIC") , sSpeciesKey );
		pUseLimitDM->SendFrame();
		pUseLimitDM->EndFrame();
		
		return 0;
	}	
	else if ( LogYN == _T("N") ) 
	{
		pUseLimitDM->StartFrame();
		pUseLimitDM->InitDBFieldData();		
		pUseLimitDM->AddDBFieldData( _T("CREATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		MobileApi.MakeUpdateFrame( _T("BO_USE_LIMIT_UPDATE_TBL") , _T("SPECIES_KEY") , _T("NUMERIC") , sSpeciesKey );
		pUseLimitDM->SendFrame();
		pUseLimitDM->EndFrame();

		return 0;
	}

	pUseLimitDM->StartFrame();
	pUseLimitDM->InitDBFieldData();
	pUseLimitDM->AddDBFieldData( _T("SPECIES_KEY") , _T("NUMERIC") , sSpeciesKey );
	pUseLimitDM->AddDBFieldData( _T("CREATE_DATE") , _T("NUMERIC") , _T("SYSDATE") );
	pUseLimitDM->AddDBFieldData( _T("WORKER") , _T("STRING") , m_pParentMgr->GetUserID() );
	pUseLimitDM->AddDBFieldData( _T("UPDATE_YN") , _T("STRING") , _T("N") );
	pUseLimitDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );


	MobileApi.MakeRecKey( sRecKey );
	MobileApi.AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	MobileApi.MakeInsertFrame( _T("BO_USE_LIMIT_UPDATE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	
	ids = MobileApi.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("ChangeUseLimitCode") );

	pUseLimitDM->EndFrame();

	return 0;

	EFS_END

	return -1;
}

BOOL CBL_BOSHELF::ChangeLocalUseLimit(CString sCodeName, CString sCodeValue,INT nIndex){

	if ( sCodeName == _T("WORKING_STATUS") && sCodeValue == _T("BOL115O"))
	{
			sCodeValue = _T("DL");
	}
	else if( sCodeName != _T("USE_LIMIT_CODE") )
	{
			return FALSE;
	}	
	
	CString serviceyn = _T("Y");
	CString strLocaControlNo = _T("");
	CString strLocaRegNo = _T("");
	CString strLocaCopyNo = _T("");
	CString sDMAlias = _T("DM_BO_LOC_1300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );	
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BOSHELF::ChangeLocalUseLimit( CString sDMAlias )") );

	pDM->GetCellData(_T("제어번호"), nIndex, strLocaControlNo);
	pDM->GetCellData(_T("BB_등록번호") , nIndex, strLocaRegNo);
	pDM->GetCellData(_T("IBS_청구_복본기호") , nIndex, strLocaCopyNo);
	CString strShelfCode = _T("");
	CString strVolCode = _T("");
	pDM->GetCellData(_T("IBS_청구_별치기호_코드"), nIndex, strShelfCode);
	pDM->GetCellData(_T("IBS_청구_권책기호") , nIndex, strVolCode);

	if (sCodeValue == _T("CL") || sCodeValue == _T("CD") || sCodeValue == _T("DL"))
	{
		serviceyn = _T("N");
	}


	if(!CKolisNetService::setLocalBookRegNoInfo(m_pParentMgr,m_pParentMgr->m_pInfo->LIB_CODE,strLocaControlNo,strLocaRegNo,strLocaCopyNo,sCodeValue,serviceyn,_T("U"),strShelfCode,strVolCode)){
		return FALSE;
	}
	return TRUE;
}
INT  CBL_BOSHELF::GetCodeSum(INT* nGM, INT* nCA, INT* nCD, INT* nIZ, INT* nCL, INT* nDL)
{
	INT  ids;
	INT  nRowCount;

	CString szUseLimitCode;

	(*nGM) = 0;
	(*nCA) = 0;
	(*nCD) = 0;
	(*nIZ) = 0;
	(*nCL) = 0;
	(*nDL) = 0;
     

    CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_소장마스터") ) ;
	if(pDM == NULL) AfxMessageBox (_T("FindDM(m_szHoldingDMAlias ) Error~!")) ;
    
    nRowCount = pDM->GetRowCount () ;

	for (INT i =0; i < nRowCount; i++) 
	{
        

		ids = pDM->GetCellData (_T("이용제한구분"),i, szUseLimitCode);
		if (ids<0) return -2;

		if (szUseLimitCode == _T("CD")) 
		{
			(*nCD)++;
		} 
		else if (szUseLimitCode == _T("CL")) 
		{
			(*nCL)++;
		} 
		else if (szUseLimitCode == _T("CA"))
		{
			(*nCA)++;
		}
		else if (szUseLimitCode == _T("IZ"))
		{
			(*nIZ)++;
		}
		else if (szUseLimitCode == _T("DL"))
		{
			(*nDL)++;
		}
		else 
		{
			(*nGM)++;
		}
	}

	return 0;
}