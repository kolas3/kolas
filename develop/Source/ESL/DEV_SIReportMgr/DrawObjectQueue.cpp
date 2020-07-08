// DrawObjectQueue.cpp: implementation of the CDrawObjectQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawObjectQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawObjectQueue::CDrawObjectQueue()
{

}

CDrawObjectQueue::~CDrawObjectQueue()
{
	//FreeLineList();
}
/*
INT CDrawObjectQueue::FreeLineList(VOID)
{
	INT count = LineList.GetCount();
	if (count == 0) return 0;
	CSIReportObject_Line * pData;
	for (INT i = 0; i < count; i++) {
		pData = (CSIReportObject_Line*)LineList.RemoveHead();
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	LineList.RemoveAll();
	return 0;
}

INT CDrawObjectQueue::CommitLineList(CDC *pDC) 
{
	
	INT count = LineList.GetCount();
	if (count == 0) return -1;
	CSIReportObject_Line * pData;
	POSITION pos;
	pos = LineList.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		pData = NULL;
		pData = (CSIReportObject_Line*)LineList.GetNext(pos);
		if (!pData) continue;
		if (pDC) {
			pDC->MoveTo(pData->rect.left, pData->rect.top);
			pDC->LineTo(pData->rect.right, pData->rect.bottom);
		}
	}
	FreeLineList();
	return count;
}

INT CDrawObjectQueue::AddLine(INT left, INT top, INT right, INT bottom)
{
	CSIReportObject_Line * pData = new CSIReportObject_Line;
	if (!pData) return -1;
	pData->rect.left = left;
	pData->rect.top = top;
	pData->rect.right = right;
	pData->rect.bottom = bottom;

	LineList.AddTail((CObject*)pData);
	return 0;
}
*/