// UniqueMsgList.cpp: implementation of the CUniqueMsgList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccessControl.h"
#include "UniqueMsgList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUniqueMsgList::CUniqueMsgList()
{
	FreeData();
}

CUniqueMsgList::~CUniqueMsgList()
{
	FreeData();
}

INT CUniqueMsgList::FreeData(VOID)
{
	UNIQUEID = 32101;
	INT count = LIST.GetCount();
	if (count == 0) return 0;
	CUniqueMsg * pData;
	for (INT i = 0; i < count; i++) {
		pData = (CUniqueMsg*)LIST.RemoveHead();
		if (pData) delete pData;
	}
	LIST.RemoveAll();
	return(count);
}

INT CUniqueMsgList::AddMsg(CString msg)
{
	CUniqueMsg *pUniqueMsg = new CUniqueMsg;
	if (!pUniqueMsg) return -1;
	
	INT result = UNIQUEID;
	pUniqueMsg->id = UNIQUEID;
	UNIQUEID += 1;

	pUniqueMsg->msg = msg;

	LIST.AddTail((CObject*)pUniqueMsg);
	return result;
}

CString CUniqueMsgList::GetMsg(INT id)
{
	INT count = LIST.GetCount();
	CUniqueMsg * pData = NULL;
	POSITION pos;
	pos = LIST.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		pData = NULL;
		pData = (CUniqueMsg*)LIST.GetNext(pos);
		if (!pData) continue;
		if (pData->id == id) return pData->msg;
	}
	return _T("");
}