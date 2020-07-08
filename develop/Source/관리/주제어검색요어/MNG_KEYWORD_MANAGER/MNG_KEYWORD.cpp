// MNG_KEYWORD.cpp: implementation of the CMNG_KEYWORD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNG_KEYWORD.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNG_KEYWORD::CMNG_KEYWORD()
{
	
}

CMNG_KEYWORD::~CMNG_KEYWORD()
{

}


INT CMNG_KEYWORD::SearchKeyword(CESL_DataMgr *pDM, CString sKeyword)
{
EFS_BEGIN
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}

	if ( sKeyword == _T("") )
	{
		return -1;
	}

	CString sWhere;
	sWhere.Format(_T("SUBJECT_NAME LIKE '%s%%'"), sKeyword);

	INT ids = pDM->RefreshDataManager(sWhere);
	if ( ids<0 )
		return -1;

	return 0;
EFS_END
	return -1;
}


INT CMNG_KEYWORD::SearchRelatedKeyword(CESL_DataMgr *pDM, CString sRecKey)
{
EFS_BEGIN
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}

	if ( sRecKey == _T("") )
	{
		return -1;
	}
	
	CString sWhere;
	sWhere.Format(_T(" SUBJECT_NAME_KEY = %s "), sRecKey);

	INT ids = pDM->RefreshDataManager(sWhere);
	if ( ids<0 )
		return -1;

	return 0;

EFS_END
	return -1;
}
