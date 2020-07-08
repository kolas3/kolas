// ESL_CodeItemList.cpp: implementation of the CESL_CodeItemList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_CodeItemList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_CodeItemList::CESL_CodeItemList()
{
	CLASS_NAME = _T("");
	DEFAULT_VALUE = -1;
}

CESL_CodeItemList::~CESL_CodeItemList()
{
	FreeCodeItem();
}

INT CESL_CodeItemList::FreeCodeItem(VOID)
{
	INT nCount = LIST.GetCount();
	CESL_CodeItem *pData;
	POSITION pos = LIST.GetHeadPosition();
	INT i;
	for (i = 0; i < nCount; i++) {
		pData = NULL;
		pData = (CESL_CodeItem*)LIST.GetNext(pos);
		if (pData) {
			delete pData;
		}
	}
	LIST.RemoveAll();
	return 0;
}

INT CESL_CodeItemList::AddCodeItem(CString strCode, CString strDescription, TCHAR cActive)
{
	CESL_CodeItem *pData = new CESL_CodeItem;
	if (pData == NULL) return -1;
	pData->CODE = strCode;
	pData->DESCRIPTION = strDescription;
	pData->ACTIVE = cActive;

	LIST.AddTail((CObject*)pData);
	return 0;
}

VOID CESL_CodeItemList::SetDefault(VOID)
{
	DEFAULT_VALUE = LIST.GetCount()-1;
	return;
}

INT CESL_CodeItemList::GetDefaultCode(CString &strData)
{
	if (DEFAULT_VALUE == -1) return -1;
	POSITION pos = LIST.FindIndex(DEFAULT_VALUE);
	if (pos == NULL) return -2;
	CESL_CodeItem *pCodeItem = (CESL_CodeItem *)LIST.GetAt(pos);
	if (pCodeItem == NULL) return -3;
	strData = pCodeItem->CODE;

	return 0;
}
