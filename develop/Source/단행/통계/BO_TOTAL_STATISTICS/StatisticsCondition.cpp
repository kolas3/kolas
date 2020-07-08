// StatisticsCondition.cpp: implementation of the CStatisticsCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatisticsCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
//=====================================================
//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// #define _PROPERTY_WORKING_STATUS _T("'BOL112N','BOL113O','BOL114O','BOL115O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O'")
// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
#define _PROPERTY_WORKING_STATUS				_T("'BOL112N','BOL113O','BOL114O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL611O','BOL252N'")
#define _REMOVE_BRING_PROPERTY_WORKING_STATUS	_T("'BOL112N','BOL113O','BOL114O','BOL115O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL611O','BOL252N'")
//#define _PROPERTY_WORKING_STATUS				_T("'BOL112N','BOL113O','BOL114O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL252N'")
//#define _REMOVE_BRING_PROPERTY_WORKING_STATUS	_T("'BOL112N','BOL113O','BOL114O','BOL115O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL511O','BOL252N'")
//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
#define _PROPERTY_WORKING_STATUS				_T("'BOL112N','BOL113O','BOL114O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O','BOL252N'")
#define _REMOVE_BRING_PROPERTY_WORKING_STATUS	_T("'BOL112N','BOL113O','BOL114O','BOL115O','BOL116O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL216O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O','BOL252N'")
/*//END-------------------------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CStatisticsCondition::CStatisticsCondition(CESL_Mgr * pMgr)
{
	m_pMgr = pMgr;
	//===================================================
	//2009.04.08 ADD BY PJW : 제적자료 포함유무 값 추가
	m_strRemoveBookBringYN = _T("Y");
	//===================================================
	//===================================================
	//2009.04.09 ADD BY PJW : 등록번호를 조건에 추가함.
	m_strStartRegNo	= _T("");
	m_strEndRegNo	= _T("");
	//===================================================


	m_strManageCode = _T("");
	m_strReference = _T("");
}

// 소멸자
CStatisticsCondition::~CStatisticsCondition()
{

}

// 관리값으로 정해진 세로열의 코드값 갯수를 리턴한다.
INT CStatisticsCondition::GetVerticalDescCount()
{
	INT cnt = m_pMgr->m_pInfo->pCodeMgr->GetCodeCount(m_strVerticalDesc);
	return cnt;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
INT CStatisticsCondition::SetWhereStmt()
{
	CString strOption, strTmpData, strGroup;

	if ( m_strClassNoType == _T("등록구분") ) strGroup = _T("B.REG_CODE");
	else if ( m_strClassNoType == _T("수입구분") ) strGroup = _T("B.ACQ_CODE");
	else strGroup = _T("SUBSTR(B.CLASS_NO, 1, 1)");

	//===================================================
	//2010.05.20 ADD BY PJW : 통합관리자일 경우 관리구분 조건 추가함
	//2010.05.20 UPDATE BY PJW : MANAGE_CODE는 따로 만들어서 넣어준다.
//	if( _T("S") == m_pMgr->m_pInfo->USER_PRIVILEGE )
//	{
//		strManageCode.Format(_T("'%s'"), m_strManageCode);
//	}
//	else
//	{
//		strManageCode.Format(_T("UDF_MANAGE_CODE"));
//	}

	CString strManageCode;
	if( _T("S") == m_pMgr->m_pInfo->USER_PRIVILEGE )
	{
		if( !m_strManageCode.IsEmpty() )
		{
			// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//strManageCode.Format(_T("AND I.MANAGE_CODE = '%s'"), m_strManageCode);
			strManageCode.Format(_T("AND B.MANAGE_CODE = '%s'"), m_strManageCode);
			// KOL.RED.2018.210 ---------------------------------------------------------------------------
			
		}
	}
	else
	{
		// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strManageCode.Format(_T("AND I.MANAGE_CODE = UDF_MANAGE_CODE"), m_strManageCode);
		strManageCode.Format(_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE"));
		// KOL.RED.2018.210 ---------------------------------------------------------------------------
		
	}
	//===================================================

	// 13/11/20 박대우 : 2012년도 기능개선패치 수정작업(20130604005)
	//                   자료실 조건이 잘못됨

/*//BEFORE-----------------------------------------------------------------------
	//===================================================
	// 2012.10.18 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (자료실 조건 쿼리)
	CString strReference;
	if( !m_strReference.IsEmpty()&& !m_strReference.Compare(_T("전체"))==0 )
	{
		strReference.Format(_T("AND B.SHELF_LOC_CODE = '%s'AND B.SHELF_LOC_CODE IS NOT NULL "), m_strReference);
	}
	else if( m_strReference.Compare(_T("전체"))==0)
	{
		strReference.Format(_T("") );
	}
	else
	{
		strReference.Format(_T("AND B.SHELF_LOC_CODE IN ('001','002','003','004','005','006','007','008','009','010','011') AND B.SHELF_LOC_CODE IS NOT NULL"));
	}
	//===================================================
*///AFTER------------------------------------------------------------------------
	CString strReference;
	if (m_strReference.Compare(_T("전체")) == 0)
	{
		strReference.Format(_T(""));
	}
	else
	{
		if (!m_strReference.IsEmpty())
		{
			strReference.Format(_T("AND B.SHELF_LOC_CODE IS NOT NULL AND B.SHELF_LOC_CODE = '%s'"), m_strReference);
		}
		else
		{
			strReference.Format(_T("AND B.SHELF_LOC_CODE IS NOT NULL"));
		}
	}
/*//END------------------------------------------------------------------------*/

	//===================================================
	//2009.03.09 UPDATE BY PJW : 가격을 구할때는 소수점 반올림한 후 합한다.
	//							 SUM(B.PRICE) -> SUM(ROUND(B.PRICE))
	switch( m_nAcqType )
	{
	case PURCHASE:
		{
			m_strWhereStmt.Format( 
				_T("SELECT %s, COUNT(DISTINCT(I.REC_KEY)), COUNT(DISTINCT(B.REC_KEY)), SUM(ROUND(B.PRICE)) ")
				_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
				//===================================================
				//2010.05.20 UPDATE BY PJW : MANAGE_CODE는 따로 만들어서 넣어준다.
//				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND I.MANAGE_CODE = %s AND B.WORKING_STATUS<'BOT'"), strGroup, strManageCode );
				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' %s AND B.WORKING_STATUS<'BOT'"), strGroup, strManageCode );
				//===================================================
				
		}
		break;
		
	case DONATE:
		{
			m_strWhereStmt.Format( 
				_T("SELECT %s, COUNT(DISTINCT(I.REC_KEY)), COUNT(DISTINCT(B.REC_KEY)), SUM(ROUND(B.PRICE)) ")
				_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
				//===================================================
				//2010.05.20 UPDATE BY PJW : MANAGE_CODE는 따로 만들어서 넣어준다.
//				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND I.MANAGE_CODE = %s AND B.WORKING_STATUS<'BOT'"), strGroup, strManageCode );
				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' %s AND B.WORKING_STATUS<'BOT'"), strGroup, strManageCode );
				//===================================================				
		}
		break;
		
	default:	
		{
			//===================================================
			//2009.04.08 UPDATE BY PJW : 제적자료 포함유무에 따라서 WORKING_STATUS를 달리 한다.
			if( m_strRemoveBookBringYN == _T("Y") )
			{
				m_strWhereStmt.Format( 
				_T("SELECT %s, COUNT(DISTINCT(I.REC_KEY)) , COUNT(DISTINCT(B.REC_KEY)), SUM(ROUND(B.PRICE)) ")
				_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
				//===================================================
				//2010.05.20 UPDATE BY PJW : MANAGE_CODE는 따로 만들어서 넣어준다.
//				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND I.MANAGE_CODE = %s AND B.WORKING_STATUS IN(%s)"), strGroup, strManageCode, _REMOVE_BRING_PROPERTY_WORKING_STATUS  );
				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' %s AND B.WORKING_STATUS IN(%s) %s "), strGroup, strManageCode, _REMOVE_BRING_PROPERTY_WORKING_STATUS, strReference  );
				//===================================================				
			}
			else
			{
				m_strWhereStmt.Format( 
				_T("SELECT %s, COUNT(DISTINCT(I.REC_KEY)) , COUNT(DISTINCT(B.REC_KEY)), SUM(ROUND(B.PRICE)) ")
				_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
				//===================================================
				//2010.05.20 UPDATE BY PJW : MANAGE_CODE는 따로 만들어서 넣어준다.
//				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND I.MANAGE_CODE = %s AND B.WORKING_STATUS IN(%s)"), strGroup, strManageCode, _PROPERTY_WORKING_STATUS  );
				_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' %s AND B.WORKING_STATUS IN(%s) %s "), strGroup, strManageCode, _PROPERTY_WORKING_STATUS,strReference  );
				//===================================================		
			}
			//===================================================
		}
		break;
	}
	//===================================================

	//===================================================
	//2009.04.09 ADD BY PJW : 등록번호를 조건에 추가함.
	strOption.Empty();

	if( !m_strStartRegNo.IsEmpty() && !m_strEndRegNo.IsEmpty() )
	{
		strOption.Format(_T(" AND B.REG_NO BETWEEN '%s' AND '%s'"), m_strStartRegNo, m_strEndRegNo);
	}
	else if( !m_strStartRegNo.IsEmpty() )
	{
		strOption.Format(_T(" AND B.REG_NO = '%s'"), m_strStartRegNo);
	}
	else if( !m_strEndRegNo.IsEmpty() )
	{
		strOption.Format(_T(" AND B.REG_NO = '%s'"), m_strEndRegNo);
	}
	
	if( !strOption.IsEmpty() )
	{
		m_strWhereStmt += strOption;
		strOption.Empty();
	}
	//===================================================
	


	strTmpData.Format(_T("%d"), m_nAcqType);
	switch ( m_nAcqType )
	{
		case PURCHASE:
		case DONATE:
			strOption.Format(_T(" AND B.ACQ_CODE='%s'"), strTmpData);
			break;
		default:			
			break;
	}
	m_strWhereStmt += strOption;
	

	if		( m_strClassNoType.Left(3) == _T("KDC") )	strOption.Format(_T(" AND I.CLASS_NO_TYPE='1'"));
	else if	( m_strClassNoType.Left(3) == _T("DDC") )	strOption.Format(_T(" AND I.CLASS_NO_TYPE='2'"));
	else if ( m_strClassNoType.Left(2) == _T("LC")	)	strOption.Format(_T(" AND I.CLASS_NO_TYPE IN (3, 7)"));	
	else												strOption.Empty();
	m_strWhereStmt += strOption;

	if( m_nStatType == STATISTICS_BY_DIVNO || 
		m_nStatType == STATISTICS_BY_DIVNO_DELIVERY || 
		m_nStatType == STATISTICS_BY_DIVNO_MISSING	)
	{
		m_strWhereStmt += GetGroupQuery();
	}
	else if( m_nStatType == STATISTICS_BY_DATE ) 
	{
		m_strWhereStmt += GetDateQuery();
	}

	return 0;
}

CString CStatisticsCondition::GetGroupQuery()
{
	CString strWhere, strOption, strGroupInfo, strWorkCode, strGroupFieldName;
	if(m_nAcqType == PURCHASE) strGroupFieldName = _T("P.ORDER_SEQ_NO");
	else if(m_nAcqType == DONATE) strGroupFieldName = _T("P.RECEIPT_NO");
	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	
	if(m_nAcqType == PURCHASE)
	{
		if(strWorkCode.IsEmpty())
			strOption.Format(_T(" AND P.ORDER_YEAR = '%s' AND %s IN (%s) AND I.WORK_CODE IS NULL"), m_pGroupCollection->GetAcqYear(), strGroupFieldName, strGroupInfo);
		else
			strOption.Format(_T(" AND P.ORDER_YEAR = '%s' AND I.WORK_CODE='%s' AND %s IN (%s)"), m_pGroupCollection->GetAcqYear(), strWorkCode, strGroupFieldName, strGroupInfo);
	}
	else
	{
		strOption.Format(_T(" AND P.ACQ_YEAR = '%s' AND %s IN (%s)"), m_pGroupCollection->GetAcqYear(), strGroupFieldName, strGroupInfo);
	}

	if(m_nStatType == STATISTICS_BY_DIVNO_DELIVERY)
		strOption += _T(" AND B.DELIVERY_YN = 'Y'");
	else if(m_nStatType == STATISTICS_BY_DIVNO_MISSING)
		strOption += _T(" AND B.DELIVERY_YN = 'N'");

	strWhere += strOption;
	return strWhere;
}

CString CStatisticsCondition::GetDateQuery()
{
	CString strOption = _T("");
	CString fieldName, m_Date1, m_Date2;
	m_MapStatData.Lookup(_T("DATE_FIELD"), fieldName);
	m_MapStatData.Lookup(_T("날짜1"), m_Date1);
	m_MapStatData.Lookup(_T("날짜2"), m_Date2);

	//if(m_Date1.IsEmpty() && !(m_Date2.IsEmpty()))
	//		strOption.Format(_T(" AND %s <= '%s'"), fieldName, m_Date2);
	//else if(!(m_Date1.IsEmpty()) && m_Date2.IsEmpty())
	//		strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
	//else if(!(m_Date1.IsEmpty()) && !(m_Date2.IsEmpty()))    FIELDNAME 적용
	//{
	//	if(m_Date1 == m_Date2)
	//		strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
	//	else
	//		strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), fieldName, m_Date1, fieldName, m_Date2);
	//}

	// 2012.10.18 UPDATE BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (자료실 조건 쿼리)
	// 배가상태 구분안함 조건이 들어갈 경우 배가일 조건을 적용하지 않기 위해 수정
	if(!m_strReference.Compare(_T("전체"))==0 )
	{
		if(m_Date1.IsEmpty() && !(m_Date2.IsEmpty()))
			strOption.Format(_T(" AND %s <= '%s'"), fieldName, m_Date2);
		else if(!(m_Date1.IsEmpty()) && m_Date2.IsEmpty())
			strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
		else if(!(m_Date1.IsEmpty()) && !(m_Date2.IsEmpty()) )
		{
			if(m_Date1 == m_Date2)
				strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
			else
				strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), fieldName, m_Date1, fieldName, m_Date2);
		}
	}
	///////////////////////////////////////////////

	return strOption;
}

CString CStatisticsCondition::GetStatType()
{
	CString strTmpData;
	if(m_nStatType == STATISTICS_BY_DIVNO && m_nAcqType == PURCHASE)
	{
		strTmpData = m_pGroupCollection->GetWorkingStatus();
		if(strTmpData == _T("구입대상") || strTmpData == _T("전체자료") || strTmpData.IsEmpty())
			strTmpData = _T("구입자료자산통계");
		else if(strTmpData == _T("주문"))
			strTmpData = _T("주문자료자산통계");
		else if(strTmpData == _T("검수완료"))
			strTmpData = _T("검수완료자료자산통계");
	} 
	else if(m_nStatType == STATISTICS_BY_DIVNO && m_nAcqType == DONATE)
	{
		strTmpData = _T("기증자료자산통계");
	}
	else if(m_nStatType == STATISTICS_BY_DATE && m_nAcqType == PURCHASE)
	{
		m_MapStatData.Lookup(_T("DATE_FIELD"), strTmpData);
		if(strTmpData == _T("B.INPUT_DATE"))
			strTmpData = _T("구입자료자산통계");
		else if(strTmpData == _T("B.REG_DATE"))
			strTmpData = _T("구입등록자료자산통계");
		else if(strTmpData == _T("P.ORDER_DATE"))
			strTmpData = _T("구입주문자료자산통계");
		else if(strTmpData == _T("P.CHECKIN_DATE"))
			strTmpData = _T("구입검수자료자산통계");			
	}
	else if(m_nStatType == STATISTICS_BY_DATE && m_nAcqType == DONATE)
	{
		m_MapStatData.Lookup(_T("DATE_FIELD"), strTmpData);
		if(strTmpData == _T("B.INPUT_DATE"))
			strTmpData = _T("기증자료자산통계");
		else if(strTmpData == _T("B.REG_DATE"))
			strTmpData = _T("기증등록자료자산통계");
	}
	else if(m_nStatType == STATISTICS_BY_DIVNO_DELIVERY)
	{
		strTmpData = _T("검수자료자산통계");
	}
	else if(m_nStatType == STATISTICS_BY_DIVNO_MISSING)
	{
		strTmpData = _T("미검수자료자산통계");
	}
	else
	{
		strTmpData = _T("자산통계");
	}

	return strTmpData;
}

CString CStatisticsCondition::GetCondition()
{
	CString strTmpData, strGroupInfo, strDate1, strDate2, strWorkCode;
	if(m_nStatType == STATISTICS_BY_DIVNO || m_nStatType == STATISTICS_BY_DIVNO_DELIVERY || m_nStatType == STATISTICS_BY_DIVNO_MISSING)
	{
		if(m_nAcqType == PURCHASE) strTmpData = _T("차수번호:");
		else if(m_nAcqType == DONATE) strTmpData = _T("접수번호:");
		strTmpData += m_pGroupCollection->GetAcqYear();
		strWorkCode = m_pGroupCollection->GetWorkCode();
		strTmpData += _T("-") + strWorkCode;
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		strTmpData += _T("-") + strGroupInfo;
	}
	else if(m_nStatType == STATISTICS_BY_DATE)
	{
		m_MapStatData.Lookup(_T("DATE_FIELD"), strTmpData);
		if(strTmpData == _T("B.INPUT_DATE"))
			strTmpData = _T("입력일:");
		else if(strTmpData == _T("B.REG_DATE"))
			strTmpData = _T("등록일:");
		else if(strTmpData == _T("P.ORDER_DATE"))
			strTmpData = _T("주문일:");
		else if(strTmpData == _T("P.CHECKIN_DATE"))
			strTmpData = _T("검수일:");	
		else if(strTmpData == _T("B.SHELF_DATE"))
			strTmpData = _T("배가일:");	
		m_MapStatData.Lookup(_T("날짜1"), strDate1);
		m_MapStatData.Lookup(_T("날짜2"), strDate2);
		strTmpData += strDate1;
		if(!strDate2.IsEmpty()) strTmpData += _T(" - ") + strDate2;
	
	}
	return strTmpData;
}
