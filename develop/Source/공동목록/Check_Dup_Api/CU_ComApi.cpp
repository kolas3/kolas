c//
// 현재 업로드의 문제점
// - LOCAL업로드시 수동으로 할 경우에도 서지MASTER를 UPDATE하는 경우가 발생
//

#include "stdafx.h"
#include "..\..\include\공동목록\CU_ComAPI.h"
#include   "..\..\include\공동목록\search_struct.h"
#include "..\..\include\공동목록\MarcConversion.h"
//#include _T("ViewUploadMarc.h")
#include "check_dup.h"
#include "..\..\esl\marc\Marcedit\MarcEditor.h"
#include "SearchMake.h"
#include "ViewUploadMarc.h"
#include "RS_SQL.h" 

//#include "..\\..\\공동목록\\RS_COMMON\\RS_DM_Mgr.h"

// 목차/초록 기능 사용여부 세팅
//#define __TOC_PROCESS__
#undef __TOC_PROCESS__

//
#define TEMP_UB_NO  _T("UB00000000000")
#define TEMP_UH_NO  _T("UH00000000000")

//2004-02-11 이학중 
#define STRING 1 
#define NUMBER 2


//
#define UPLOAD_INTERVAL_TIME  (1000*20)  // 10 sec.

// +++ private function
INT _GetControlNo( CESL_Mgr* pMgr, CESL_DataMgr* pDataMgr, CString f_icCentMatCode, CString f_icYear, CString &pControlNo );
INT _MakeLocalControlNoList( TCHAR* control_no, TCHAR* control_no_list );
INT _TocProcess( CESL_Mgr* pMgr, CString &sDispAlias, CString &sDBAlias, INT iRowIndex, CString &ub_control_no, CString *sDupDBAlias = NULL, INT iDupRowIndex = -1 );
// ---

//
TCHAR g_BibMarc[100000];
TCHAR g_HoldingMarc[100000];
TCHAR g_LocalMarc[100000];
TCHAR g_TempMarc[100000];

CString test_g_BibMarc ;


// for compare previous marc ( only used in file & batch upload )
BOOL g_bCheckInterval = FALSE;
// 
UINT g_bWaited        = 0;
TCHAR g_PrevMarc[100000];

//
// CU_ComApi내의 함수중 로컬종자료 DataMgr의 REC_KEY값 ALIAS를 세팅하고자할때 사용한다. ( default = _T("REC_KEY") )
TCHAR g_sRecKeyAlias[40] = _T("REC_KEY");

// LOCAL 소장자료에 UPLOAD상태를 UPDATE한다.
// 주의사항 : 함수내부에서 COMMIT수행함.
INT UpdateUploadStatus( CESL_Mgr* pMgr, CString &sDBAlias, INT iRowIndex, CString sUploadStatus )
{

    INT  ids;
	CString species_key;
    CESL_DataMgr *pDataMgr;

	pDataMgr = pMgr->FindDM (sDBAlias);
    if(pDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Fail")) ;
		return -1 ;
	}
	
	CString tmp_g_sRecKeyAlias ;
	tmp_g_sRecKeyAlias.Format (_T("%s"),g_sRecKeyAlias);
	
	ids = pDataMgr->GetCellData (tmp_g_sRecKeyAlias.GetBuffer(0),iRowIndex,species_key);
	if (ids) return -3;

	pDataMgr->StartFrame ();
	pDataMgr->InitDBFieldData();
	pDataMgr->AddDBFieldData( _T("CENTER_UPLOAD_STATUS"), _T("STRING"), sUploadStatus );
    ids = pDataMgr->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),species_key,-1);
	if (ids<0) return -4;
    
	ids = pDataMgr->SendFrame ();
	ids = pDataMgr->EndFrame ();
    return 0;  

}

INT UpdateBTPStatisticsInfo( CESL_Mgr* pMgr, CString sDispAlias, CString sDBAlias, INT SpeciesCnt,CString sUpFlag ,CString sClientType )
{

	CESL_DataMgr* pDM = pMgr->FindDM(sDBAlias);
	if(pDM == NULL)
	{
		AfxMessageBox(_T("UpdateStatisticsInfo FindDM() Fail"));
		return -1 ;
	}

	CString strRecKey;
	pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);

	CString strSpeciesCount;
	strSpeciesCount.Format(_T("%d"), SpeciesCnt);

	if (!CKolisNetService::MakeStatiticsBtp(pMgr, strRecKey, pMgr->m_pInfo->LIB_CODE, sClientType, _T("A"), sUpFlag, CKolisNetService::GetSysdate(), strSpeciesCount))
	{
		AfxMessageBox(_T("UpdateStatisticsInfo KOLIS-NET OPEN API Fail"));
		return -1;
	}

    return 0;
}