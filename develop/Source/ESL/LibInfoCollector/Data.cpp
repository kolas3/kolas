// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData(CString strDataAlias)
{
	m_strDataAlias = strDataAlias;
}

CData::CData()
{
}

CData::~CData()
{
	m_Map.RemoveAll();
}


void CData::SetAt(CString strAlias, CString strValue)
{
	m_Map.SetAt(strAlias, strValue);	
}

BOOL CData::GetAt(CString strAlias, CString &strValue)
{
	strValue.Empty();
	return m_Map.Lookup(strAlias, strValue);
}

BOOL CData::Remove(CString strAlias)
{
	return m_Map.RemoveKey(strAlias);
}

POSITION CData::GetStartPosition()
{
	return m_Map.GetStartPosition();
}

void CData::GetNextData(POSITION &pos, CString &strAlias, CString &strValue)
{
	m_Map.GetNextAssoc(pos, strAlias, strValue);
}

CString CData::GetAt(CString strAlias)
{
	CString strValue;
	GetAt(strAlias, strValue);

	return strValue;
}
CDataMgr::CDataMgr()
{
	
}
CDataMgr::CDataMgr(CString strAlias)
{
	m_strAlias = strAlias;
}

CDataMgr::~CDataMgr()
{
	RemoveAll();
}

void CDataMgr::RemoveAll()
{
	INT nCnt = 0;
	const INT MAX_CNT = m_Data.GetCount();
	for(nCnt = MAX_CNT-1; nCnt > 0; nCnt--)
	{
		DeleteData(nCnt);
	}
}
INT CDataMgr::GetData(CString strAlias, INT nIdx, CString& strValue)
{	
	POSITION pos = m_Data.FindIndex(nIdx);
	if(!pos)
	{
		return -1000;
	}	

	CData* pData = (CData*)m_Data.GetAt(pos);
	if(!pData)
	{			
		return -1010;
	}

	if(!pData->GetAt(strAlias, strValue))
	{
		return -1020;
	}

	return 0;	
}

INT CDataMgr::SetData(CString strAlias, CString strValue, INT nIdx)
{
	POSITION pos = m_Data.FindIndex(nIdx);
	if(!pos)
	{
		return -1000;
	}	

	CData* pData = (CData*)m_Data.GetAt(pos);
	if(!pData)
	{			
		return -1010;
	}

	pData->SetAt(strAlias, strValue);

	return 0;
}

INT CDataMgr::InsertData(INT nIdx)
{
	if(-1 == nIdx)
	{
		CData* pData = new CData;
		m_Data.AddTail((CObject*)pData);
	}

	POSITION pos = m_Data.FindIndex(nIdx);
	if(!pos)
	{
		return -1000;
	}	

	CData* pData = new CData(m_strAlias);
	m_Data.InsertBefore(pos, (CObject*)pData);

	return 0;
}

INT CDataMgr::DeleteData(INT nIdx)
{
	POSITION pos = m_Data.FindIndex(nIdx);
	if(!pos)
	{
		return -1000;
	}

	CData* pData = (CData*)m_Data.GetAt(pos);
	if(!pData)
	{
		return -1010;
	}

	delete pData;
	pData = NULL;
	m_Data.RemoveAt(pos);

	return 0;
}

INT CDataMgr::GetCount()
{
	return m_Data.GetCount();
}

CString CDataMgr::GetDataAlias()
{
	return m_strAlias;
}