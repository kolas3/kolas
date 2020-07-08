// ProviderInfo.cpp: implementation of the CProviderInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProviderInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProviderInfo::CProviderInfo()
{

}

CProviderInfo::~CProviderInfo()
{

}

INT CProviderInfo::SetProviderInfo(CESL_DataMgr *pDM, CString strProviderCode)
{
	CString strQuery;
	strQuery.Format(_T("SELECT DESCRIPTION, EMAIL, TEL, ADDRESS FROM CD_CODE_TBL WHERE CLASS=2 AND TRIM(CODE) = '%s'"), strProviderCode);
	CESL_DataMgr tmpDM ;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);

	m_strProviderName = tmpDM.GetCellData(0, 0);
	m_strProviderEMail = tmpDM.GetCellData(0, 1);
	m_strProviderTel = tmpDM.GetCellData(0, 2);
	m_strProviderAddr = tmpDM.GetCellData(0, 3);
	return 0;
}
