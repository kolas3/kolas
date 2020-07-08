// WorkingStatus.cpp: implementation of the CWorkingStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkingStatus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkingStatus::CWorkingStatus(CString strWorkingStatus)
{
	m_MapPririty.RemoveAll();
	m_MapPririty.SetAt(_T("BOT"), _T("1"));
	m_MapPririty.SetAt(_T("BOA"), _T("2"));
	m_MapPririty.SetAt(_T("BOR"), _T("3"));
	m_MapPririty.SetAt(_T("BOC"), _T("4"));
	m_MapPririty.SetAt(_T("BOL"), _T("5"));
	
	m_bErrorOccured = Init(strWorkingStatus);
}

CWorkingStatus::~CWorkingStatus()
{

}

INT CWorkingStatus::Compare(CString strWorkingStatus)
{
	if(strWorkingStatus.IsEmpty()) return 1;
	if(strWorkingStatus.GetLength() > 7) return 1;
	CString strTmpData;
	INT nPriority;
	if(!m_MapPririty.Lookup(strWorkingStatus.Left(3), strTmpData))
		return 1;
	nPriority = _ttoi(strTmpData.GetBuffer(0));
	
	if(nPriority > m_nPriority) return 1;
	else if(nPriority < m_nPriority) return -1;
	
	return strWorkingStatus.Compare(m_strWorkingStatus);
}

BOOL CWorkingStatus::Init(CString strWorkingStatus)
{
	if(strWorkingStatus.IsEmpty()) return FALSE;
	m_strWorkingStatus = strWorkingStatus;
	m_strHead = strWorkingStatus.Left(3);
	if(m_strHead.GetLength() != 3) return FALSE;
	CString strTmpData;
	if(!m_MapPririty.Lookup(m_strHead, strTmpData))
		return FALSE;

	m_nPriority = _ttoi(strTmpData.GetBuffer(0));
	return TRUE;
}

CString CWorkingStatus::GetMaxWorkingStatus(CESL_DataMgr *pDM, CString strAlias)
{
	if(pDM == NULL || strAlias.IsEmpty()) return _T("");
	
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return _T("");

	CString strTmpData;
	if(!Init(pDM->GetCellData(strAlias, 0))) return _T("");

	for(INT i=1 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(strAlias, i);
		if(Compare(strTmpData) > 0)
			if(!Init(strTmpData)) return _T("");
	}

	return m_strWorkingStatus;
}

CString CWorkingStatus::GetMinWorkingStatus(CESL_DataMgr *pDM, CString strAlias)
{
	if(pDM == NULL || strAlias.IsEmpty()) return _T("");
	
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return _T("");
	
	CString strTmpData;
	if(!Init(pDM->GetCellData(strAlias, 0))) return _T("");
	
	for(INT i=1 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(strAlias, i);
		if(Compare(strTmpData) < 0)
			if(!Init(strTmpData)) return _T("");
	}
	
	return m_strWorkingStatus;
}

INT CWorkingStatus::SetWorkingStatusIfMax(CString strWorkingStatus)
{
	if(Compare(strWorkingStatus) > 0)
		if(!Init(strWorkingStatus)) return -1;
	return 0;
}

INT CWorkingStatus::SetWorkingStatusIfMin(CString strWorkingStatus)
{
	if(Compare(strWorkingStatus) < 0)
		if(!Init(strWorkingStatus)) return -1;
	return 0;
}