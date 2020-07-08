// CenterUpload.cpp: implementation of the CCenterUpload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CenterUpload.h"
#include "ECOOCIComApi.h"

#include "KolisNetService.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCenterUpload::CCenterUpload(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)	
{
	m_strLibID = _T("") ;
	m_strMode  = _T("") ;
}

CCenterUpload::~CCenterUpload()
{

}
	
INT CCenterUpload::Check_Direct_Upload(CString &strReturnMode) 
{
	
	m_strMode = _T("N");
	
	if(m_strMode != _T("Y") && m_strMode != _T("N"))
	{
		strReturnMode.Format (_T("%s"),_T("CHECK_LIBCODE")) ;
		return -100 ;
	}

	strReturnMode.Format (_T("%s"),m_strMode) ;

	return 0 ;
}

INT  CCenterUpload::Input_Temp_Mater_tbl(CString strSpeciesKey ,CString strMarc, CString strUploadStatus, CString strControlNo, CString strRegNoList, CString strLimitCodeList, CString strSpeciesLimitCode ) 
{

	CString strTableName ;
	CString strReckey ;
	CString strCreateLib ;
	CString strLocalMarc ;
	CString strQuery;
 
	CString strMarcType = _T("W");
	if (!CKolisNetService::GetSequenceNo(this, strReckey))
	{
		return -1;
	}
	if (!CKolisNetService::MakeTempMasterUpload(this, strReckey, this->m_pInfo->LIB_CODE, strMarc, strControlNo, strMarcType ,strRegNoList, strLimitCodeList, strSpeciesLimitCode) )
	{
		return -1 ;
	}

	return 0 ;
}

INT CCenterUpload::Update_Temp_Lib_Statistics_Tbl(INT nSpeciesCnt , CString strFlag /*= _T("U")*/)
{
	CString strRecKey;
	if (!CKolisNetService::GetSequenceNo(this, strRecKey))
	{
		
		AfxMessageBox(_T("임시서지 정보 입력 실패")) ;
		return -1;
	}

	CString strSpeciesCount;
	strSpeciesCount.Format(_T("%d"), nSpeciesCnt);
	
	if (!CKolisNetService::MakeStatiticsTempUpload(this, strRecKey, this->m_pInfo->LIB_CODE, strFlag, CKolisNetService::GetSysdate(), strSpeciesCount))
	{
		AfxMessageBox(_T("임시서지 통계 정보 입력 실패")) ;
		return -1;
	}

	return 0;
}
