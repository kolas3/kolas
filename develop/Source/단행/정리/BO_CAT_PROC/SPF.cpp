// SPF.cpp: implementation of the CSPF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "BO_CAT_PROC_1200.h"
#include "..\..\..\공동목록\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPF::CSPF(CESL_Mgr* pParent) : CSPF_Work(pParent)
{

}

CSPF::~CSPF()
{

}

INT CSPF::SPFExecutePath(CString strPath){
	
EFS_BEGIN
	if (strPath.IsEmpty()) return -1;
	if (strPath == _T("정리완료")){
		return CatComplete();
	}
	return 0;

EFS_END
return -1;

}

INT CSPF::CatComplete() {

EFS_BEGIN

	CBO_CAT_PROC_1200 *pParent = (CBO_CAT_PROC_1200*)m_pParentMgr;
	CString strDMAlias;
	this->GetTempData(_T("DMAlias"), strDMAlias);
	CESL_DataMgr *pDM;
	pDM = pParent->FindDM(strDMAlias);
	CString sRec_key = pDM->GetCellData( _T("REC_KEY") , pParent->m_idx );

	CString sShelf_Date;
	pParent->GetControlData(_T("CM_BO_CAT_PROC_1200"), _T("배가일자"), sShelf_Date);

	CESL_DataMgr *pDmSpecies;
	pDmSpecies = pParent->FindDM(_T("DM_BO_CAT_PROC_1200_SPECIES"));
	if ( pDmSpecies == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_1200_SPECIES error") );
		return -1;
	}
	INT iSpeciesCount = pDmSpecies->GetRowCount();

	pDM->StartFrame();
	CString sYear;
	CString sQuery;
	for ( INT i = 1 ; i <= iSpeciesCount ; i ++ ) {
		CString sStatus;
		pDmSpecies->GetCellData( _T("BS_자료상태"), i-1 , sStatus );

		CString sSpeciesKey;
		pDmSpecies->GetCellData( _T("BS_종KEY") , i-1 , sSpeciesKey );
		
		CString sControlCode;
		pDmSpecies->GetCellData( _T("BS_제어자료구분") , i-1 , sControlCode );

		if ( sControlCode.IsEmpty() ) {
			AfxMessageBox( _T("제어자료구분이 없는 자료가 있습니다.") );
			return -1;
		}
		
		CString sNumber;
		pDmSpecies->GetCellData( _T("번호") , i-1 , sNumber );

		CString sControlNo;
		pParent->GetControlData( _T("CM_BO_CAT_PROC_1200"), _T("현재년도"), sYear );		
		sControlNo.Format( _T("%s%s%05s") , sControlCode, sYear , sNumber );

		CString sStreamMarc = pDmSpecies->GetCellData( _T("BS_MARC"), i-1 );
		CMarc pMarc;
		pParent->m_pInfo->pMarcMgr->Decoding(sStreamMarc, &pMarc);

		pParent->m_pInfo->pMarcMgr->DeleteField( &pMarc, _T("001") );

		CString s001tag = _T("001") + sControlNo;
		pParent->m_pInfo->pMarcMgr->InsertField( &pMarc, s001tag );
		pParent->m_pInfo->pMarcMgr->Encoding(&pMarc, sStreamMarc);
		
		sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s', CONTROL_NO = '%s', LAST_WORK='%s' WHERE REC_KEY = '%s';") , sStreamMarc, sControlNo , m_pParentMgr->GetWorkLogMsg(_T("배가완료"),WORK_LOG), sSpeciesKey );
		pDM->AddFrame( sQuery );

		sQuery.Format( _T("UPDATE IDX_BO_TBL SET SPECIES_CLASS = '1', WORKING_STATUS = '%s', CONTROL_NO = '%s' , LAST_WORK='%s' WHERE REC_KEY = '%s';") , _T("BOL112N"), sControlNo, m_pParentMgr->GetWorkLogMsg(_T("배가완료"),WORK_LOG), sSpeciesKey );
		pDM->AddFrame( sQuery );
		
	}
	CESL_DataMgr * pDM_1200;
	pDM_1200 = pParent->FindDM( _T("DM_BO_CAT_PROC_1200") );
	if ( NULL == pDM_1200 ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1200 error") );
		return -1;
	}
	INT iRowCount = pDM_1200->GetRowCount();
	for ( i = 1 ; i <= iRowCount ; i ++ ) {
		CString sDivisionValue;
		pDM_1200->GetCellData( i-1 , 0 , sDivisionValue );
		if ( sDivisionValue == _T("") ) {
			continue;
		}
		CString sControlNo;
		pDM_1200->GetCellData( _T("BS_제어번호") , i-1 , sControlNo );
		INT iContorlNo = _ttoi ( sControlNo.GetBuffer(0) );
		sControlNo.Format( _T("%d") , iContorlNo-1 );
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s', LAST_WORK='%s' WHERE DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;") , sControlNo , m_pParentMgr->GetWorkLogMsg(_T("배가완료"),WORK_LOG), sDivisionValue );
		pDM->AddFrame( sQuery );
		if ( sControlNo == _T("1") ) {
			CString sLastRecKey;
			pDM->MakeRecKey( sLastRecKey );
			sQuery.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) VALUES ('%s','CO','제어번호','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);"), sLastRecKey, sDivisionValue, sYear, _T("1"), m_pParentMgr->GetWorkLogMsg(_T("배가완료"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("배가완료"),WORK_LOG) );
			pDM->AddFrame( sQuery );
		}
	}
	
	INT ids;
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return -1;
	}

	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}

	CButton *pPrint01;
	pPrint01 = (CButton *)pParent->GetDlgItem(IDC_CHK_CENTER_UPLOAD);
	if ( pPrint01->GetCheck() == 1 ) {
		CString BasicDM = _T("DM_CENTERUPLOAD_01");
		CString DupDM   = _T("DM_CENTERUPLOAD_복본자료");
		CString pParentCM;

		CESL_DataMgr *pDmBasic = pParent->FindDM( BasicDM );
		if ( pDmBasic == NULL ) return -1;

		CString strWhere = _T("I.REC_KEY in (") + pParent->sRecKey + _T(")");
		if (pDmBasic->RefreshDataManager(strWhere) < 0)	return -1;

		INT iBasicCount = pDmBasic->GetRowCount();
		CBO_CAT_CENTERUPLOAD dlg(pParent) ;
		dlg.SetDMAlias( BasicDM, DupDM, pParentCM) ;	
		dlg.CenterUpload( FALSE ) ;
	}

	AfxMessageBox( _T("배가완료 했습니다.") );		

	return 0;

EFS_END
	return -1;
}