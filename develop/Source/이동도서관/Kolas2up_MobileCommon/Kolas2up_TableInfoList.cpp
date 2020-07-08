// Kolas2up_TableInfoList.cpp: implementation of the CKolas2up_TableInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_TableInfoList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_TableInfoList::CKolas2up_TableInfoList()
{

}

CKolas2up_TableInfoList::~CKolas2up_TableInfoList()
{
	FreeTableList();
}

INT CKolas2up_TableInfoList::FreeTableList(VOID)
{
	INT nData = TABLE_LIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_TableInfo * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_TableInfo*)TABLE_LIST.RemoveHead();
		if (pData) delete pData;
	}
	TABLE_LIST.RemoveAll();
	return 0;
}

CKolas2up_TableInfo * CKolas2up_TableInfoList::AddTableToList(VOID)
{
	CKolas2up_TableInfo *pData = new CKolas2up_TableInfo;
	if (pData == NULL) return NULL;
	TABLE_LIST.AddTail((CObject*)pData);
	return pData;
}

CKolas2up_TableInfo * CKolas2up_TableInfoList::ReadTableANDAddTableToList(CString strFileName)
{
	CKolas2up_TableInfo *pData = new CKolas2up_TableInfo;
	if (pData == NULL) return NULL;
	INT ids = pData->ReadTableFieldInfoFromFile(strFileName);
	if (ids < 0) {
		delete pData;
		pData = NULL;

		return NULL;
	}
	TABLE_LIST.AddTail((CObject*)pData);
	return pData;
}

INT CKolas2up_TableInfoList::AddTableToList(CKolas2up_TableInfo * pTableInfo)
{
	if (pTableInfo == NULL) return -1;
	TABLE_LIST.AddTail((CObject*)pTableInfo);
	return 0;
}

CKolas2up_TableInfo * CKolas2up_TableInfoList::GetTableInList(INT nIdx)
{
	INT nData = TABLE_LIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_TableInfo * pData;
	POSITION pos = TABLE_LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_TableInfo*)TABLE_LIST.GetNext(pos);
		if (i == nIdx) return pData;
	}
	return NULL;
}

INT CKolas2up_TableInfoList::GetCount(VOID)
{
	return TABLE_LIST.GetCount();
}

CKolas2up_TableInfo * CKolas2up_TableInfoList::FindTableByTableName(CString strTableName)
{
	INT nData = TABLE_LIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_TableInfo * pData;
	POSITION pos = TABLE_LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_TableInfo*)TABLE_LIST.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->TABLE_NAME == strTableName) return pData;
	}
	return NULL;
}