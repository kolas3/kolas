// OrderMailDataInfo.cpp: implementation of the COrderMailDataInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderMailDataInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrderMailDataInfo::COrderMailDataInfo()
{

}

COrderMailDataInfo::~COrderMailDataInfo()
{

}

INT COrderMailDataInfo::UpdateMailSendDate(CESL_DataMgr *pDM, CString strWorkLogMsg, CString strTodayDate, INT nOpType)
{
	CString strQuery;
	if(nOpType > 0)
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', MAIL_SEND_DATE='%s' WHERE SPECIES_KEY=%s AND ACQ_KEY=%s AND VOL_SORT_NO=%s;"),
			strWorkLogMsg, strTodayDate, m_strSpeciesKey, m_strAcqKey, m_strVolSortNo);
	else
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', MAIL_SEND_DATE='%s' WHERE SPECIES_KEY=%s AND ACQ_KEY=%s AND VOL_SORT_NO=%s AND WORKING_STATUS='BOA111N';"),
			m_strSpeciesKey, m_strAcqKey, m_strVolSortNo);
	pDM->AddFrame(strQuery);
	return 0;
}

INT COrderMailDataInfo::CopyData(COrderMailDataInfo *pOrderDataInfo)
{
	if(pOrderDataInfo == NULL) return -1;
	pOrderDataInfo->SetAcqKey(m_strAcqKey);
	pOrderDataInfo->SetSpeciesKey(m_strSpeciesKey);
	pOrderDataInfo->SetVolSortNo(m_strVolSortNo);
	return 0;
}
