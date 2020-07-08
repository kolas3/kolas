// RFIDBookInfo.cpp: implementation of the CRFIDBookInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFIDBookInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRFIDBookInfo::CRFIDBookInfo()
{
	nSpeciesType = 0;
	strLocale = _T("1");
	strBookKey = _T("");
	strAccessionNo = _T("");
	strTitle = _T("");
	strAuthor = _T("");
	strPublisher = _T("");
	strShelfStatus = _T("");
	strShelfLocCode = _T("");
	strRFIDSerialNo = _T("");
	strCallNo = _T("");
	strLibCode = _T("123456");
	strStatus = _T("0");
	strBoSe = _T("B");
	strLoan = _T("0");
	strSecurity = _T("1");
	strTemp = _T("");
}

CRFIDBookInfo::~CRFIDBookInfo()
{

}
