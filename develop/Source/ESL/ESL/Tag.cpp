// Tag.cpp: implementation of the CTag class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Tag.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTag::CTag()
{
	m_strItemData.Empty();
	m_strFirstIndicator = _T(" ");
	m_strSecondIndicator = _T(" ");
	m_strFieldTerminator.Format(_T("%c"), FIELD_TERMINATOR);
}
CTag *CTag::Clone()
{
	EFS_BEGIN
	
	CTag *pTag = new CTag;
	pTag->SetFieldTerminator(GetFieldTerminator());
	pTag->SetFirstIndicator(GetFirstIndicator());
	pTag->SetSecondIndicator(GetSecondIndicator());
	pTag->SetItemData(GetItemData());
	pTag->SetTagCode(GetTagCode());
	
	POSITION pos = m_listSubfield.GetHeadPosition();
	while (pos != NULL)
	{
		CSubfield *pSubfield = (CSubfield*)m_listSubfield.GetNext(pos);
		if (pSubfield == NULL) continue;
		pTag->AddSubfield(pSubfield->Clone());
	}
	return pTag;

	EFS_END
	return NULL;
}
CTag::~CTag()
{
	FreeSubfieldList();
}
INT CTag::FreeSubfieldList()
{
	EFS_BEGIN
	
	INT nListCount = m_listSubfield.GetCount();
	if (nListCount == 0) return 0;
	CSubfield *pSubfield;
	for (INT i = 0; i < nListCount; i++)
	{
		pSubfield = (CSubfield*)m_listSubfield.RemoveHead();
		if (pSubfield) delete pSubfield;
	}
	m_listSubfield.RemoveAll();
	return nListCount;

	EFS_END
	return -1;
}
INT CTag::AddSubfield(CSubfield *pSubfield)
{
	EFS_BEGIN
	
	if (!pSubfield) return -1;
	m_listSubfield.AddTail((CObject*)pSubfield);
	return 0;

	EFS_END
	return -1;
}
CSubfield *CTag::FindSubfield(CString strSubfield)
{
	EFS_BEGIN
	
	CSubfield *pSubfield;
	POSITION pos = m_listSubfield.GetHeadPosition();
	while (pos != NULL)
	{
		pSubfield = (CSubfield*)m_listSubfield.GetNext(pos);
		if (pSubfield != NULL && pSubfield->GetSubfieldCode() == strSubfield)
			return pSubfield;
	}
	return NULL;

	EFS_END
	return NULL;
}
INT CTag::SortSubfield_Special(CMapStringToOb *pMap)
{
	EFS_BEGIN
	
	if (pMap == NULL) return -1;
	CStringArray *pArr = NULL;
	if (!pMap->Lookup(GetTagCode(), (CObject*&)pArr)) return -1;
	if (pArr == NULL) return -1;
	// 일단 일련적으로 정렬한다.
	SortSubfield_Bubble();
	//우선순위로 정렬한다.
	CObList tempList;
	CSubfield *pSubfield = NULL;
	POSITION pos = m_listSubfield.GetTailPosition();
	while (pos)
	{
		pSubfield = (CSubfield*)m_listSubfield.GetPrev(pos);
		if (pSubfield == NULL) continue;
		tempList.AddTail((CObject*)pSubfield);
	}
	m_listSubfield.RemoveAll();
	CString strSubfieldCode;
	INT nCount = pArr->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strSubfieldCode = pArr->GetAt(idx);
		if(strSubfieldCode.IsEmpty()) continue;
		strSubfieldCode.Replace('$', SUBFIELD_CODE);
		pos = tempList.GetTailPosition();
		while (pos)
		{
			POSITION posOld = pos;
			pSubfield = (CSubfield*)tempList.GetPrev(pos);
			if (pSubfield == NULL) continue;
			if (strSubfieldCode != pSubfield->GetSubfieldCode()) continue;
			
			// 임시 리스트에서 제거하고 서브필드 리스트에 넣는다.
			tempList.RemoveAt(posOld);
			m_listSubfield.AddTail((CObject*)pSubfield);
		}
	}
	// 임시 리스트의 나머지를 서브필드 리스트에 넣는다.
	nCount = tempList.GetCount();
	for (idx = 0; idx < nCount; idx++)
	{
		pSubfield = (CSubfield*)tempList.RemoveTail();
		if (pSubfield == NULL) continue;
		m_listSubfield.AddTail((CObject*)pSubfield);
	}
	return 0;

	EFS_END
	return -1;
}
INT CTag::SortSubfield(INT nOrder /*= SORT_ASC*/)
{
	EFS_BEGIN
	
	return SortSubfield_Bubble(nOrder);

	EFS_END
	return -1;
}
INT CTag::SortSubfield_Bubble(INT nOrder /*= SORT_ASC*/)
{
	EFS_BEGIN
	
	INT nCount = m_listSubfield.GetCount();
	INT nCount2 = nCount;
	for (INT i = 0; i < nCount; i++)
	{
		for (INT j = 0; j < nCount2 - 1; j++)
		{
			CSubfield *pSubfieldMaster = (CSubfield*)m_listSubfield.GetAt(m_listSubfield.FindIndex(j));
			if (pSubfieldMaster == NULL) continue;
			CSubfield *pSubfieldSlave = (CSubfield*)m_listSubfield.GetAt(m_listSubfield.FindIndex(j + 1));
			if (pSubfieldSlave == NULL) continue;
			// asc 
			if (nOrder == SORT_ASC)
			{
				if (pSubfieldMaster->GetSubfieldCode() > pSubfieldSlave->GetSubfieldCode())
				{
					// Swap
					m_listSubfield.SetAt(m_listSubfield.FindIndex(j), (CObject*)pSubfieldSlave);
					m_listSubfield.SetAt(m_listSubfield.FindIndex(j + 1), (CObject*)pSubfieldMaster);
				}
			}
			// desc
			else if (nOrder == SORT_DESC)
			{
				if (pSubfieldMaster->GetSubfieldCode() < pSubfieldSlave->GetSubfieldCode())
				{
					m_listSubfield.SetAt(m_listSubfield.FindIndex(i), (CObject*)pSubfieldSlave);
					m_listSubfield.SetAt(m_listSubfield.FindIndex(j + 1), (CObject*)pSubfieldMaster);
				}
			}
		}
		nCount2 --;
	}
	return 0;

	EFS_END
	return -1;
}
INT CTag::SortSubfield_Selection(INT nOrder /*= SORT_ASC*/)
{
	EFS_BEGIN
	INT nCount = m_listSubfield.GetCount();
	for (INT i = 0; i < nCount - 1; i++)
	{
		CSubfield *pSubfieldMaster = (CSubfield*)m_listSubfield.GetAt(m_listSubfield.FindIndex(i));
		if (pSubfieldMaster == NULL) continue;
		for (INT j = i + 1; j < nCount; j++)
		{
			CSubfield *pSubfieldSlave = (CSubfield*)m_listSubfield.GetAt(m_listSubfield.FindIndex(j));
			if (pSubfieldSlave == NULL) continue;
			
			// asc
			if (nOrder == SORT_ASC)
			{
				if (pSubfieldMaster->GetSubfieldCode() > pSubfieldSlave->GetSubfieldCode())
				{
					m_listSubfield.SetAt(m_listSubfield.FindIndex(i), (CObject*)pSubfieldSlave);
					m_listSubfield.SetAt(m_listSubfield.FindIndex(j), (CObject*)pSubfieldMaster);
					pSubfieldMaster = pSubfieldSlave;
				}
			}
			// desc
			else if (nOrder == SORT_DESC)
			{
				if (pSubfieldMaster->GetSubfieldCode() < pSubfieldSlave->GetSubfieldCode())
				{
					m_listSubfield.SetAt(m_listSubfield.FindIndex(i), (CObject*)pSubfieldSlave);
					m_listSubfield.SetAt(m_listSubfield.FindIndex(j), (CObject*)pSubfieldMaster);
					pSubfieldMaster = pSubfieldSlave;
				}
			}
		}
	}
	return 0;

	EFS_END
	return -1;
}
