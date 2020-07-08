// FixedFieldInfo.cpp: implementation of the CFixedFieldInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FixedFieldInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFixedFieldInfo::CFixedFieldInfo()
{

}

CFixedFieldInfo::CFixedFieldInfo(LPCTSTR lpszGroupName, LPCTSTR lpszItemName)
{
	m_strGroupName = lpszGroupName;
	m_strItemName = lpszItemName;
}

CFixedFieldInfo::CFixedFieldInfo(LPCTSTR lpszGroupName, LPCTSTR lpszSubGroupName, LPCTSTR lpszItemName)
{
	m_strGroupName = lpszGroupName;
	m_strSubGroupName = lpszSubGroupName;
	m_strItemName = lpszItemName;
}

CFixedFieldInfo::~CFixedFieldInfo()
{
	FreeItem();
}

VOID CFixedFieldInfo::FreeItem()
{
	CFixedFieldInfoItem *pItem = NULL;

	POSITION pos = m_listItem.GetHeadPosition();
	while (pos)
	{
		pItem = static_cast<CFixedFieldInfoItem*>(m_listItem.GetNext(pos));
		delete pItem;
	}

	m_listItem.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// CFixedFieldInfo Operations

VOID CFixedFieldInfo::Init()
{
	FreeItem();

	m_strGroupName.Empty();
	m_strSubGroupName.Empty();
}

POSITION CFixedFieldInfo::AddItem(LPCTSTR lpszCode, LPCTSTR lpszDesc)
{
	CFixedFieldInfoItem *pItem = new CFixedFieldInfoItem;
	pItem->m_strCode = lpszCode;
	pItem->m_strDesc = lpszDesc;
	
	return m_listItem.AddTail(pItem);
}

POSITION CFixedFieldInfo::GetHeadPositionItem()
{
	return m_listItem.GetHeadPosition();
}

VOID CFixedFieldInfo::GetNextItem(POSITION &rPosition, CString &strCode, CString &strDesc)
{
	strCode.Empty();
	strDesc.Empty();

	CFixedFieldInfoItem *pItem = static_cast<CFixedFieldInfoItem*>(m_listItem.GetNext(rPosition));
	if (pItem)
	{
		strCode = pItem->m_strCode;
		strDesc = pItem->m_strDesc;
	}
}
