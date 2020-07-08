// AcqGroupInfo.cpp: implementation of the CAcqGroupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("수서그룹정보"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupInfo::CAcqGroupInfo(CESL_Mgr * pParentMgr) : CGroupInfo(pParentMgr)
{

}

CAcqGroupInfo::~CAcqGroupInfo()
{

}

INT CAcqGroupInfo::InitGroupInfo(CString strConnectionInfo, CString strDMAlias)
{
	INT ids = InitDataManager(strConnectionInfo, strDMAlias);
	if(ids < 0) return -1;
	
	m_MapDMAliasMap.RemoveAll();
	m_MapDMAliasMap.SetAt(_T("수입년도"), _T("그룹생성년도"));
	m_MapDMAliasMap.SetAt(_T("책수"), _T("그룹책수"));
	m_MapDMAliasMap.SetAt(_T("REC_KEY"), _T("그룹KEY"));
	m_MapDMAliasMap.SetAt(_T("그룹번호"), _T("그룹번호"));
	m_MapDMAliasMap.SetAt(_T("그룹이름"), _T("그룹이름"));
	m_MapDMAliasMap.SetAt(_T("그룹생성일"), _T("그룹생성일자"));
	m_MapDMAliasMap.SetAt(_T("그룹생성자"), _T("그룹생성자"));
	m_MapDMAliasMap.SetAt(_T("책일련번호"), _T("책일련번호"));
	return 0;
}

BOOL CAcqGroupInfo::IsEqual(CGroupInfo * pGroup)
{
	CStringArray ArrAlias, ArrMyData, ArrOtherData;
	ArrAlias.RemoveAll();	ArrMyData.RemoveAll();	ArrOtherData.RemoveAll();
	ArrAlias.Add(_T("수입년도"));	ArrAlias.Add(_T("그룹번호"));	ArrAlias.Add(_T("그룹이름"));
	
	INT ids = ReplaceAlias(&ArrAlias);
	if(ids < 0) return FALSE;
	ids = GetCellData(&ArrAlias, &ArrMyData);
	if(ids < 0) return FALSE;
	ids = GetCellData(&ArrAlias, &ArrOtherData);
	if(ids < 0) return FALSE;
	
	INT nCnt = ArrMyData.GetSize();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(ArrMyData.GetAt(i) != ArrOtherData.GetAt(i))
			return FALSE;
	}
	return TRUE;
}

INT CAcqGroupInfo::InitBookSerialNo()
{
	CString strQuery, strAcqYear, strGroupNo;
	INT ids = GetAcqYear(strAcqYear);
	if(ids < 0) return -1;
	ids = GetGroupInfo(strGroupNo);
	if(ids < 0) return -1;

	if(!strAcqYear.IsEmpty() || strGroupNo.IsEmpty())
	{
		ids = SetBookLastSerialNo(_T("0"));
		if(ids < 0) return -1;
		return 0;
	}

	strQuery.Format(
		_T("SELECT MAX(SERIAL_NO) FROM BO_BOOK_TBL WHERE (ACQ_CODE IS NULL OR ACQ_CODE='-1') ")
		_T("AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
//DEL 		_T("AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
		strAcqYear, strGroupNo);
	CString strValue;
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if(ids < 0) return -1;
	ids = SetBookLastSerialNo(strValue);
	if(ids < 0) return -1;
	return 0;
}

INT CAcqGroupInfo::UpdateLastBookSerialNo()
{
	return 0;
}
