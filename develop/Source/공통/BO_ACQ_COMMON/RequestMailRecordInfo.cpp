// RequestMailRecordInfo.cpp: implementation of the CRequestMailRecordInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RequestMailRecordInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRequestMailRecordInfo::CRequestMailRecordInfo()
{

}

CRequestMailRecordInfo::~CRequestMailRecordInfo()
{

}

INT CRequestMailRecordInfo::UpdateMailSendDate(CESL_DataMgr *pDM, CString strWorkLogMsg, CString strTodayDate)
{
	CString strQuery;
	strQuery.Format(
		_T("UPDATE BO_FURNISH_TBL SET MAIL_SEND_DATE='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
		strTodayDate, strWorkLogMsg, m_strRecKey);
	pDM->AddFrame(strQuery);
	return 0;
}

INT CRequestMailRecordInfo::SetMailSendDateInfo(CESL_DataMgr *pDM, CString strTodayDate)
{
	if(pDM == NULL) return 0;
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("∫Òƒ°»Ò∏¡KEY"), i);
		if(strTmpData == m_strRecKey)
		{
			pDM->SetCellData(_T("∏ﬁ¿œπﬂº€¿œ"), strTodayDate, i);
			break;
		}
	}
	return 0;
}
