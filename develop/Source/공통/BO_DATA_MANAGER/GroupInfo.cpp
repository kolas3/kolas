// GroupInfo.cpp: implementation of the CGroupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupInfo::CGroupInfo(CESL_Mgr * pParentMgr) : CBO_DataManager(pParentMgr)
{
	m_MapDMAliasMap.RemoveAll();
}

CGroupInfo::~CGroupInfo()
{

}

INT CGroupInfo::GetOtherInfo(CString strAlias, CString &strData)
{
	CString strValue;
	BOOL bLookup = m_MapDMAliasMap.Lookup(strAlias, strValue);
	if(!bLookup) return -1;
	INT ids = GetCellData(strValue, strData);
	if(ids < 0) return -1;
	return 0;
}
INT CGroupInfo::GetAcqYear(CString &strAcqYear)
{
	return GetOtherInfo(_T("수입년도"), strAcqYear);
}

INT CGroupInfo::GetGroupInfo(CString &strGroupInfo)
{
	return GetOtherInfo(_T("그룹번호"), strGroupInfo);
}

INT CGroupInfo::GetGroupCreateDate(CString &strGroupCreateDate)
{
	return GetOtherInfo(_T("그룹생성일"), strGroupCreateDate);
}

INT CGroupInfo::GetGroupCreator(CString &strGroupCreator)
{
	return GetOtherInfo(_T("그룹생성자"), strGroupCreator);
}

INT CGroupInfo::GetRecKey(CString &strRecKey)
{
	return GetOtherInfo(_T("REC_KEY"), strRecKey);
}

INT CGroupInfo::GetBookLastSerialNo(CString &strBookSearialNo)
{
	return GetOtherInfo(_T("책일련번호"), strBookSearialNo);
}

INT CGroupInfo::SetOtherInfo(CString strAlias, CString strData)
{
	CString strValue;
	BOOL bLookup = m_MapDMAliasMap.Lookup(strAlias, strValue);
	if(!bLookup) return -1;
	INT ids = SetCellData(strValue, strData);
	if(ids < 0) return -1;
	return 0;
}

INT CGroupInfo::SetAcqYear(CString strAcqYear)
{
	return SetOtherInfo(_T("수입년도"), strAcqYear);
}

INT CGroupInfo::SetGroupInfo(CString strGroupInfo)
{
	return SetOtherInfo(_T("그룹번호"), strGroupInfo);
}

INT CGroupInfo::SetGroupCreateDate(CString strGroupCreateDate)
{
	return SetOtherInfo(_T("그룹생성일"), strGroupCreateDate);
}

INT CGroupInfo::SetGroupCreator(CString strGroupCreator)
{
	return SetOtherInfo(_T("그룹생성자"), strGroupCreator);
}

INT CGroupInfo::SetRecKey(CString strRecKey)
{
	return SetOtherInfo(_T("REC_KEY"), strRecKey);
}

INT CGroupInfo::SetBookLastSerialNo(CString strBookSearialNo)
{
	return SetOtherInfo(_T("책일련번호"), strBookSearialNo);
}

INT CGroupInfo::CopyData(CGroupInfo * pGroup)
{
	if(pGroup == 0) return -1;
	INT ids = SetDM(pGroup->GetDM(), 0);
	if(ids < 0) return -1;
	return 0;
}

INT CGroupInfo::ReplaceAlias(CStringArray * pArrAlias)
{
	if(pArrAlias == NULL) return -1;
	INT nCnt = pArrAlias->GetSize();
	if(nCnt == 0) return 0;

	INT ids;
	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = ReplaceAlias(pArrAlias->GetAt(i), strTmpData);
		if(ids < 0) return -1;
		pArrAlias->SetAt(i, strTmpData);
	}
	return 0;
}

INT CGroupInfo::ReplaceAlias(CString strAlias, CString &strDMAlias)
{
	BOOL bLookup = m_MapDMAliasMap.Lookup(strAlias, strDMAlias);
	if(!bLookup) return -1;
	return 0;
}