// NewRecKeyList.cpp: implementation of the CNewRecKeyList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewRecKeyList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewRecKeyList::CNewRecKeyList()
{
	DATALIST.RemoveAll();
}

CNewRecKeyList::~CNewRecKeyList()
{
	FreeNewRecKey();
}

INT CNewRecKeyList::FreeNewRecKey(VOID)
{
	INT nData = DATALIST.GetCount();
	if (nData == 0) return 0;
	CNewRecKey * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CNewRecKey*)DATALIST.RemoveHead();
		if (pData) delete pData;
	}
	DATALIST.RemoveAll();
	return 0;
}

INT CNewRecKeyList::AddNewRecKey(CString strMobile, CString strMain)
{
	CNewRecKey * pData = new CNewRecKey;
	if (pData == NULL) return -1;
	pData->MOBILE = strMobile;
	pData->MAIN = strMain;

	DATALIST.AddTail((CObject*)pData);
	return 0;
}

BOOL CNewRecKeyList::FindRecKey(CString strMobile, CString &strMain)
{
	INT nData = DATALIST.GetCount();
	if (nData == 0) return FALSE;
	CNewRecKey * pData = NULL;
	POSITION pos = DATALIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CNewRecKey*)DATALIST.GetNext(pos);
		if (pData == NULL) continue;

		if (pData->MOBILE == strMobile) {
			strMain.Format(_T("%s"), pData->MAIN);
			return TRUE;
		}
	}
	return FALSE;
}

CNewRecKey *CNewRecKeyList::AddNewRecKey(VOID)
{
	CNewRecKey * pData = new CNewRecKey;
	if (pData == NULL) return NULL;

	DATALIST.AddTail((CObject*)pData);
	return pData;
}