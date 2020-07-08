// Kolas2up_Card.cpp: implementation of the CKolas2up_Card class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportTester.h"
#include "Kolas2up_Card.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_Card::CKolas2up_Card()
{
	for (INT i = 0; i < MAX_CARD_LINE; i++) {
		LINE[i] = _T("");
	}
}

CKolas2up_Card::~CKolas2up_Card()
{

}

INT CKolas2up_Card::SetAt(INT nRowIdx, CString str)
{
	LINE[nRowIdx].Format(_T("%s"), str);
	return 0;
}