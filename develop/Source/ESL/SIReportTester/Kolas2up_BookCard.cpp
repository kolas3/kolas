// Kolas2up_BookCard.cpp: implementation of the CKolas2up_BookCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportTester.h"
#include "Kolas2up_BookCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_BookCard::CKolas2up_BookCard()
{
	nMAX_BOOKCARD_LINE = 16;
}

CKolas2up_BookCard::~CKolas2up_BookCard()
{

}

INT CKolas2up_BookCard::SetAt(INT nRowIdx, CString str)
{
	if (nRowIdx < 0 || nRowIdx >= nMAX_BOOKCARD_LINE) return -1;
	LINE[nRowIdx].Format(_T("%s"), str);
	return 0;
}
