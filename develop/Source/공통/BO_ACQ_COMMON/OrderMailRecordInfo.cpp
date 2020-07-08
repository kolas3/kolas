// OrderMailRecordInfo.cpp: implementation of the COrderMailRecordInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderMailRecordInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrderMailRecordInfo::COrderMailRecordInfo()
{

}

COrderMailRecordInfo::~COrderMailRecordInfo()
{
	INT cnt = m_arrRecordDataInfo.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		delete m_arrRecordDataInfo.GetAt(i);
	}
	m_arrRecordDataInfo.RemoveAll();
}

VOID COrderMailRecordInfo::SetOrderMailDataInfo(COrderMailDataInfo * pDataInfo)
{
	COrderMailDataInfo * pOrderDataInfo = NULL;
	pOrderDataInfo = new COrderMailDataInfo();
	pDataInfo->CopyData(pOrderDataInfo);
	m_arrRecordDataInfo.Add(pOrderDataInfo);
}

INT COrderMailRecordInfo::UpdateMailSendDate(CESL_DataMgr *pDM, CString strLastWorkMsg, CString strTodayDate, INT nOpType)
{
	INT cnt = m_arrRecordDataInfo.GetSize();
	for(INT i=0 ; i<cnt ; i++)
		m_arrRecordDataInfo.GetAt(i)->UpdateMailSendDate(pDM, strLastWorkMsg, strTodayDate, nOpType);
	return 0;
}
