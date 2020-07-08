// SIReportPrintObject.cpp: implementation of the CSIReportPrintObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportPrintObject.h"
#include "..\..\..\..\ESL\SIReportData\SIReportConstant.h"

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

INT CSIReportPrintObject::SetAt(INT nRowIdx, CString str, BOOL bAlignBottom /*= FALSE*/)
{
	if (nRowIdx < 0 || nRowIdx >= nMAX_BOOKCARD_LINE) return -1;
	LINE[nRowIdx].Format(_T("%s"), str);
	V_ALIGN[nRowIdx] = bAlignBottom ? ALIGN_BOTTOM : ALIGN_VCENTER;

	return 0;
}
