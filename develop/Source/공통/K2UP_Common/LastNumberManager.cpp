// LastNumberManager.cpp: implementation of the CLastNumberManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LastNumberManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("마지막번호관리"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLastNumberManager::CLastNumberManager(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{

}

CLastNumberManager::~CLastNumberManager()
{

}

INT CLastNumberManager::GetLastNumber(CString strKindCode, CString strFormCode, CString strCreateYear, CString strDivVal, CString &strLastNum)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	CString tmpquery;
	if(strDivVal.IsEmpty())
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// 통합시스템 적용
		tmpquery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='%s' AND MAKE_YEAR='%s' ")
						 _T("AND DIVISION_VALUE IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE")
						 ,strKindCode, strFormCode, strCreateYear );
//		tmpquery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = '%s' AND MAKE_YEAR = '%s' AND DIVISION_VALUE IS NULL"),
//			strKindCode, strFormCode, strCreateYear );
		//--2008.10.16 UPDATE BY PWR --}}
	}
	else 
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// 통합시스템 적용
		tmpquery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='%s' AND MAKE_YEAR='%s' ")
						 _T("AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						 , strKindCode,	strFormCode, strCreateYear,	strDivVal);
//		tmpquery.Format(
//			_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = '%s' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '%s'"),
//			strKindCode,
//			strFormCode,
//			strCreateYear,
//			strDivVal);
		//--2008.10.16 UPDATE BY PWR --}}
	}
	tmpDM.GetOneValueQuery(tmpquery, strLastNum);
	if(strLastNum.IsEmpty())
	{
		CString strRecKey;
		tmpDM.StartFrame();
		tmpDM.MakeRecKey(strRecKey);
		//++2008.10.16 UPDATE BY PWR {{++
		// 통합시스템 적용
		tmpquery.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL")
						 _T("(REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR, DIVISION_VALUE, ")
						 _T("LAST_NUMBER, CREATE_DATE, CREATE_WORKER, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
						 _T("VALUES('%s', '%s', '%s', '%s', '%s', 0, '%s', '%s', '%s', '%s', UDF_MANAGE_CODE);")
						 , strRecKey, strKindCode, strFormCode, strCreateYear, strDivVal,GetTodayDate()
						 , m_pParentMgr->m_pInfo->USER_ID, m_pParentMgr->GetWorkLogMsg(WORK_LOG)
						 , m_pParentMgr->GetWorkLogMsg(WORK_LOG) );
//		tmpquery.Format(
//			_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR, DIVISION_VALUE, LAST_NUMBER, CREATE_DATE, CREATE_WORKER, FIRST_WORK, LAST_WORK) VALUES('%s', '%s', '%s', '%s', '%s', 0, '%s', '%s', '%s', '%s');"),
//			strRecKey, 
//			strKindCode,
//			strFormCode, 
//			strCreateYear, 
//			strDivVal,
//			GetTodayDate(),
//			m_pParentMgr->m_pInfo->USER_ID,
//			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
//			m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		//--2008.10.16 UPDATE BY PWR --}}
		tmpDM.AddFrame(tmpquery);
		INT ids = tmpDM.SendFrame();
		if(ids < 0) return -2000;
		ids = tmpDM.EndFrame();
		if(ids < 0) return -2000;
		strLastNum = _T("0");
	}
	return 0;
}

INT CLastNumberManager::SetLastNumber(CString strKindCode, CString strFormCode, CString strCreateYear, CString strDivVal, CString strLastNum)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	tmpDM.StartFrame();
	CString tmpquery;
	if(strDivVal.IsEmpty())
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// 통합시스템 적용
		tmpquery.Format( _T("UPDATE CO_LAST_NUMBER_TBL ")
						 _T("SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='%s' AND MAKE_YEAR='%s' ")
						 _T("AND DIVISION_VALUE IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , strLastNum, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strKindCode
						 , strFormCode, strCreateYear );
//		tmpquery.Format(
//			_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = '%s' AND MAKE_YEAR = '%s' AND DIVISION_VALUE IS NULL;"),
//			strLastNum,
//			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
//			strKindCode,
//			strFormCode,
//			strCreateYear);
		//--2008.10.16 UPDATE BY PWR --}}
	}
	else 
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// 통합시스템 적용
		tmpquery.Format( _T("UPDATE CO_LAST_NUMBER_TBL ")
						 _T("SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='%s' AND MAKE_YEAR='%s' ")
						 _T("AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , strLastNum, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strKindCode
						 , strFormCode, strCreateYear, strDivVal );
//		tmpquery.Format(
//			_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = '%s' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '%s';"),
//			strLastNum,
//			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
//			strKindCode,
//			strFormCode,
//			strCreateYear,
//			strDivVal);
		//--2008.10.16 UPDATE BY PWR --}}
	}
	tmpDM.AddFrame(tmpquery);
	INT ids = tmpDM.SendFrame();
	if(ids < 0) return -2000;
	ids = tmpDM.EndFrame();
	if(ids < 0) return -2000;

	return 0;
}

CString CLastNumberManager::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}