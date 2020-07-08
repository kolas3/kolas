// Kolas2up_MobileLog.cpp: implementation of the CKolas2up_MobileLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_MobileLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_MobileLog::CKolas2up_MobileLog()
{
	strREC_KEY = _T("");
	cQUERY_TYPE = 'I';
	strTABLE_NAME = _T("");
	strFIELDS = _T("");
	strVALUES = _T("");
	strREC_KEY_FIELDNAME = _T("");
	cREC_KEY_FIELDTYPE = 'I';
	strREC_KEY_FIELDVALUE = _T("-1");
	cEXE_FLAG = 'R';		// Ready ´ë±â
	strREMARK = _T("");
	strLOGDATE = _T("");
}

CKolas2up_MobileLog::~CKolas2up_MobileLog()
{

}

INT CKolas2up_MobileLog::SetLogDate(VOID)
{
	CTime t = CTime::GetCurrentTime();
	//strLOGDATE.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	strLOGDATE = _T("SYSDATE");
	return 0;
}