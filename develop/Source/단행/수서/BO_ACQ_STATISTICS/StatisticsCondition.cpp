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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CStatisticsCondition::CStatisticsCondition(CESL_Mgr * pMgr)
{
	m_pMgr = pMgr;
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
	CString strOption, strTmpData;

	switch( m_nAcqType )
	{
	case PURCHASE:
		{
			m_strWhereStmt = 
				_T("SELECT P.SUBJECT_CODE, COUNT(DISTINCT(I.REC_KEY)), COUNT(DISTINCT(B.REC_KEY)), SUM(GETPRICEWON(B.ACQ_KEY, B.PRICE, 'Y', UDF_MANAGE_CODE)) ")
				_T("FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B ")
				// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//_T("WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND I.MANAGE_CODE = UDF_MANAGE_CODE");
				_T("WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND B.MANAGE_CODE = UDF_MANAGE_CODE");
				// KOL.RED.2018.210 ---------------------------------------------------------------------------
				
		}
		break;
		
	case DONATE:
		{
			if( m_bDonatorFlag == TRUE )
			{
				m_strWhereStmt = 
					_T("SELECT P.SUBJECT_CODE, COUNT(DISTINCT(I.REC_KEY)), COUNT(DISTINCT(B.REC_KEY)), SUM(B.PRICE) ")
					_T("FROM IDX_BO_TBL I, BO_DONINFO_TBL P, BO_BOOK_TBL B, CO_DONATOR_TBL D ")
					_T("WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY=D.REC_KEY(+) ")
					// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					//_T("AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND I.MANAGE_CODE = UDF_MANAGE_CODE");
					_T("AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND B.MANAGE_CODE = UDF_MANAGE_CODE");
					// KOL.RED.2018.210 ---------------------------------------------------------------------------
					
			}
			else
			{
				m_strWhereStmt = 
					_T("SELECT P.SUBJECT_CODE, COUNT(DISTINCT(I.REC_KEY)), COUNT(DISTINCT(B.REC_KEY)), SUM(B.PRICE) ")
					_T("FROM IDX_BO_TBL I, BO_DONINFO_TBL P, BO_BOOK_TBL B ")
					// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					//_T("WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND I.MANAGE_CODE = UDF_MANAGE_CODE");
					_T("WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND B.MANAGE_CODE = UDF_MANAGE_CODE");
					// KOL.RED.2018.210 ---------------------------------------------------------------------------
			}
		}
		break;
		
	default:	
		{
			m_strWhereStmt = 
			_T("SELECT SUBSTR(B.CLASS_NO, 1,1), COUNT(DISTINCT(I.REC_KEY)) , COUNT(DISTINCT(B.REC_KEY)), SUM(B.PRICE) ")
			_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
			// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND I.MANAGE_CODE = UDF_MANAGE_CODE");
			_T("WHERE I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.WORKING_STATUS<'BOT' AND B.MANAGE_CODE = UDF_MANAGE_CODE");
			// KOL.RED.2018.210 ---------------------------------------------------------------------------			
		}
		break;
	}


	strTmpData.Format(_T("%d"), m_nAcqType);
	switch ( m_nAcqType )
	{
		//===================================================
		//2009.10.07 ADD BY LKS : 추가되는 수입구분이 쿼리에 추가되도록 수정
		//===================================================
//이전소스===================================================================
//DEL		case PURCHASE:
//DEL		case DONATE:
//DEL			strOption.Format(_T(" AND B.ACQ_CODE='%s'"), strTmpData);
//DEL			break;
//DEL		default:
//DEL			strOption.Format(_T(" AND B.ACQ_CODE='%s'"), strTmpData);
//DEL			break;
//===========================================================================

		case EXCHANGE:
		case ACQ_TYPE_INVALID:
			strOption.Format(_T(" AND ( B.ACQ_KEY IS NULL OR B.ACQ_KEY < 1 )"));
			break;
		default:
			strOption.Format(_T(" AND B.ACQ_CODE='%s'"), strTmpData);
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
	
	if(m_Date1.IsEmpty() && !(m_Date2.IsEmpty()))
		strOption.Format(_T(" AND %s <= '%s'"), fieldName, m_Date2);
	else if(!(m_Date1.IsEmpty()) && m_Date2.IsEmpty())
		strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
	else if(!(m_Date1.IsEmpty()) && !(m_Date2.IsEmpty()))
	{
		if(m_Date1 == m_Date2)
			strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
		else
			strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), fieldName, m_Date1, fieldName, m_Date2);
	}
	return strOption;
}

CString CStatisticsCondition::GetStatType()
{
	CString strTmpData;
	if(m_nStatType == STATISTICS_BY_DIVNO && m_nAcqType == PURCHASE)
	{
		strTmpData = m_pGroupCollection->GetWorkingStatus();
		if(strTmpData == _T("구입대상") || strTmpData == _T("전체자료") || strTmpData.IsEmpty())
			strTmpData = _T("구입자료통계");
		else if(strTmpData == _T("주문"))
			strTmpData = _T("주문자료통계");
		else if(strTmpData == _T("검수완료"))
			strTmpData = _T("검수완료자료통계");
	} 
	else if(m_nStatType == STATISTICS_BY_DIVNO && m_nAcqType == DONATE)
	{
		strTmpData = _T("기증자료통계");
	}
	else if(m_nStatType == STATISTICS_BY_DATE && m_nAcqType == PURCHASE)
	{
		m_MapStatData.Lookup(_T("DATE_FIELD"), strTmpData);
		if(strTmpData == _T("B.INPUT_DATE"))
			strTmpData = _T("구입자료통계");
		else if(strTmpData == _T("B.REG_DATE"))
			strTmpData = _T("구입등록자료통계");
		else if(strTmpData == _T("P.ORDER_DATE"))
			strTmpData = _T("구입주문자료통계");
		else if(strTmpData == _T("P.CHECKIN_DATE"))
			strTmpData = _T("구입검수자료통계");			
	}
	else if(m_nStatType == STATISTICS_BY_DATE && m_nAcqType == DONATE)
	{
		m_MapStatData.Lookup(_T("DATE_FIELD"), strTmpData);
		if(strTmpData == _T("B.INPUT_DATE"))
			strTmpData = _T("기증자료통계");
		else if(strTmpData == _T("B.REG_DATE"))
			strTmpData = _T("기증등록자료통계");
	}
	else if(m_nStatType == STATISTICS_BY_DIVNO_DELIVERY)
	{
		strTmpData = _T("검수자료통계");
	}
	else if(m_nStatType == STATISTICS_BY_DIVNO_MISSING)
	{
		strTmpData = _T("미검수자료통계");
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
		m_MapStatData.Lookup(_T("날짜1"), strDate1);
		m_MapStatData.Lookup(_T("날짜2"), strDate2);
		strTmpData += strDate1;
		if(!strDate2.IsEmpty()) strTmpData += _T(" - ") + strDate2;
	}
	return strTmpData;
}
