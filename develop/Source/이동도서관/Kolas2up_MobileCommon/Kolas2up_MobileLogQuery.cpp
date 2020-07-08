// Kolas2up_MobileLogQuery.cpp: implementation of the CKolas2up_MobileLogQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_MobileLogQuery.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_MobileLogQuery::CKolas2up_MobileLogQuery()
{
	strRecKey	= _T("");
	strQuery	= _T("");
	FLAG		= 'N';
	strMsg		= _T("");

	m_arrAddonQuery.RemoveAll();
}

CKolas2up_MobileLogQuery::~CKolas2up_MobileLogQuery()
{

}

INT CKolas2up_MobileLogQuery::MakeQueryExecuteLog(CString &strResult)
{
	strResult = _T("");
	if (strRecKey.GetLength() == 0) return -1;
	//strResult.Format(_T("UPDATE MOBILE_LOG_TBL SET EXE_FLAG='%c', REMARK='%s' WHERE REC_KEY=%s;"), FLAG, strMsg, strRecKey);
	strResult.Format(_T("DELETE FROM QUERY_LOG_TBL WHERE REC_KEY=%s;"), strRecKey);
	return 0;
}