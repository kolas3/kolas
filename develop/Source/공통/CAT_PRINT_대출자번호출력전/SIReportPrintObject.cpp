// SIReportPrintObject.cpp: implementation of the CSIReportPrintObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportPrintObject.h"
#include "..\..\ESL\SIReportData\SIReportConstant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportPrintObject::CSIReportPrintObject()
{
	nMAX_BOOKCARD_LINE = 16;
}

CSIReportPrintObject::~CSIReportPrintObject()
{

}

INT CSIReportPrintObject::SetAt(INT nRowIdx, CString str, BOOL bAlignBottom, INT bHAlign)
{
	if (nRowIdx < 0 || nRowIdx >= nMAX_BOOKCARD_LINE) return -1;
	LINE[nRowIdx].Format(_T("%s"), str);
	V_ALIGN[nRowIdx] = bAlignBottom ? ALIGN_BOTTOM : ALIGN_VCENTER;
	// 2005.11.11 ADD BY PDJ
	// 가로 정렬기능 추가
	H_ALIGN[nRowIdx] = bHAlign;

	return 0;
}

