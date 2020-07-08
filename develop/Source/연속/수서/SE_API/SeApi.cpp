// SeApi.cpp: implementation of the CSeApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "SeDuplicatedVolDlg.h"
#include "SeAppedixExpDlg.h"
#include "SeSpeciesDelete.h"
#include "..\..\..\공통\IndexToolKit\IndexTool.h"
#include "..\..\..\공통\IndexToolKit\Index.h"
#include "..\..\..\연속\열람\장서관리\BL_SE_LOC_1500\BL_SE_LOC_15.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeApi::CSeApi(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
}

CSeApi::~CSeApi()
{

}

INT CSeApi::SPECIES_PURCHASE_INSERT(CESL_DataMgr* pDM_SPECIES, CESL_DataMgr* pDM_IDX, CESL_DataMgr* pDM_PURCHASE, CESL_DataMgr* pDM_SUB_HISTORY, CMarc* pMarc, INT nRowIdx)
{
	// 연속-종-구입의 입력(복본이 아니라는 가정) 
	// 색인에 1 row insert
	// 종에 1 row insert
	// 구입정보에 1 row insert
	// 구독이력정보에 1row insert 
	// 소장정보에 1 row insert(REC_KEY) 
	
	
	if(!pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY || !pMarc)
	{
		AfxMessageBox(_T("연속-종-구입의 입력 관련정보를 찾을 수 없습니다!"));
		return -1;
	}
	
	//프레임에만 사용되는 DM free
	pDM_IDX->FreeData();
	pDM_SUB_HISTORY->FreeData();
	
	// 색인 DM에 1row insert
	pDM_IDX->InsertRow(-1);
	
	// 구독 이력 DM에 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// 구독 이력에 REC_KEY만들어 준다
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("구독이력KEY"),strSUB_HISTORY_KEY,nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_구독이력정보KEY"),strSUB_HISTORY_KEY,nRowIdx);


	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),nRowIdx); 	
	

	//구입정보로 구독이력정보를 만든다 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default 값을 적용 시킨다.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//구입 적용을 시킨다 
	pDM_SPECIES->SetCellData(_T("SS_구입여부"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_기증여부"),_T("N"),nRowIdx);
	
	

	//종과 마크를 가지고 색인을 구성한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	//종에 홀딩KEY 를 설정한다
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_홀딩KEY"),strHoldingKey,nRowIdx);


	//startframe
	pDM_SPECIES->StartFrame();

	// 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// 색인에 Insert할 Insert문을 구성한다.
	pDM_IDX->SetCellData(_T("ISS_종KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// 구입관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// 구독이력정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);


	// 소장정보에 rec_key만 insert한다 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	
	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	


	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 저장 실패!!!!"));
		return -1000;		// Species Insert Error
	}
	
	return 0;
}


INT CSeApi::SPECIES_PURCHASE_INSERT_DISCONTINUE(CESL_DataMgr* pDM_SPECIES,
												CESL_DataMgr* pDM_IDX, 
												CESL_DataMgr* pDM_PURCHASE, 
												CESL_DataMgr* pDM_SUB_HISTORY, 
												CMarc* pMarc, 
												CMarc* pOldMarc,
												CString strOldSpeciesKey,
												CString strDiscontinueReason,
												INT nRowIdx)
{
	// 연속-종-구입의 입력 및 기존 폐간정보 UPDATE 
	// 색인에 1 row insert
	// 종에 1 row insert
	// 구입정보에 1 row insert
	// 구독이력정보에 1 row insert 
	// 소장정보에 1 row insert(REC_KEY) 
	// 기존 종에 MARC update
	// 기존 색인에 간행 상태 update('d' (폐간) 으로)
	
	
	if(!pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY || !pMarc || !pOldMarc || strOldSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("연속-종-구입의 입력 - 폐간처리 관련정보를 찾을 수 없습니다!"));
		return -1;
	}
	
	//프레임에만 사용되는 DM free
	pDM_IDX->FreeData();
	pDM_SUB_HISTORY->FreeData();
	
	// 색인 DM에 1row insert
	pDM_IDX->InsertRow(-1);
	
	// 구독 이력 DM에 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// 구독 이력에 REC_KEY만들어 준다
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("구독이력KEY"),strSUB_HISTORY_KEY,nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_구독이력정보KEY"),strSUB_HISTORY_KEY,nRowIdx);


	CArray <CString, CString> RemoveAliasList;

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),nRowIdx); 	
	

	//구입정보로 구독이력정보를 만든다 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default 값을 적용 시킨다.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//구입 적용을 시킨다 
	pDM_SPECIES->SetCellData(_T("SS_구입여부"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_기증여부"),_T("N"),nRowIdx);
	
	
	//종과 마크를 가지고 색인을 구성한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	//종에 홀딩KEY 를 설정한다
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_홀딩KEY"),strHoldingKey,nRowIdx);

	//startframe
	pDM_SPECIES->StartFrame();

	// 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// 색인에 Insert할 Insert문을 구성한다.
	pDM_IDX->SetCellData(_T("ISS_종KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// 구입관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// 구독이력정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);


	// 소장정보에 rec_key만 insert한다 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	
	//기존 종 색인 UPDATE 
	CString strQuery;
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET PUB_STATUS = 'd' WHERE REC_KEY = %s;"),strOldSpeciesKey);
	pDM_SPECIES->AddFrame(strQuery);
	
	
	//기존 종 폐간 사유 UPDATE
	if(!strDiscontinueReason.IsEmpty())
	{
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET DISCONTINUE_REASON = '%s' WHERE REC_KEY = %s;"),strDiscontinueReason,strOldSpeciesKey);
		pDM_SPECIES->AddFrame(strQuery);
	}
	

	//선행 및 후속 저록 insert
	CString strWorkLogMsg;
	CString strREC_KEY1,strREC_KEY2;

	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM_SPECIES->MakeRecKey(strREC_KEY1);
	pDM_SPECIES->MakeRecKey(strREC_KEY2);
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_PREV_SPECIES_TBL (REC_KEY,SPECIES_KEY,PREV_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY1,strSpecies_key,strOldSpeciesKey,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);
	
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_NEXT_SPECIES_TBL (REC_KEY,SPECIES_KEY,NEXT_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY2,strOldSpeciesKey,strSpecies_key,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);

	

	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	SetIdxTmpSeTbl(pDM_SPECIES, strOldSpeciesKey, _T("I"));

	
	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 저장 실패!!!!"));
		return -1000;		// Species Insert Error
	}
	
	//기존 마크관련 update
	ids = UpdateSpeciesMarcbyCMarc(pDM_SPECIES,strOldSpeciesKey,pOldMarc);
	if (ids < 0) {
		AfxMessageBox(_T("마크 업데이트 실패!!!!"));
		return -1200;		// Species Insert Error
	}


	return 0;
}

INT CSeApi::SPECIES_DONATE_INSERT(CESL_DataMgr* pDM_SPECIES, CESL_DataMgr* pDM_IDX, CESL_DataMgr* pDM_DONATE, CMarc* pMarc, INT nRowIdx)
{
	// 연속-종-기증의 입력(복본이 아니라는 가정)
	// 색인에 1 row insert
	// 종에 1 row insert
	// 기증정보에 1 row insert
	// 소장정보에 1 row insert (REC_KEY만)

	CArray <CString, CString> RemoveAliasList;

	if(!pDM_SPECIES || !pDM_IDX || !pDM_DONATE || !pMarc)
	{
		AfxMessageBox(_T("연속-종-기증의 입력 관련정보를 찾을 수 없습니다!"));
		return -1;
	}
	
	//프레임에만 사용되는 DM free
	pDM_IDX->FreeData();


	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_기증관리KEY"),0); 	
	
	// 색인 DM에 1row insert
	pDM_IDX->InsertRow(-1);
	

	//Default 값을 적용 시킨다.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	

	//기증적용을 시킨다 
	pDM_SPECIES->SetCellData(_T("SS_기증여부"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_구입여부"),_T("N"),nRowIdx);
	
	
	//종과 마크를 가지고 색인을 구성한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);
	
	
	//종에 홀딩KEY 를 설정한다
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_홀딩KEY"),strHoldingKey,nRowIdx);


	//startframe
	pDM_SPECIES->StartFrame();
	
	// 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// 색인에 Insert할 Insert문을 구성한다.
	pDM_IDX->SetCellData(_T("ISS_종KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// 기증관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// 소장정보에 rec_key만 insert한다 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	

	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));


	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 저장 실패!!!!"));
		return -2000;		// Species Insert Error
	}
	

	return 0;
}

INT CSeApi::SPECIES_DONATE_INSERT_DISCONTINUE(CESL_DataMgr* pDM_SPECIES, 
										      CESL_DataMgr* pDM_IDX, 
										      CESL_DataMgr* pDM_DONATE, 
										      CMarc* pMarc,
										      CMarc* pOldMarc,
										      CString strOldSpeciesKey,
											  CString strDiscontinueReason,
										      INT nRowIdx)
{
	// 연속-종-기증의 입력 및 기존 종 폐간 처리 
	// 색인에 1 row insert
	// 종에 1 row insert
	// 기증정보에 1 row insert
	// 소장정보에 1 row insert (REC_KEY만)
	// 기존 종에 MARC update
	// 기존 색인에 간행 상태 update('d' (폐간) 으로)

	CArray <CString, CString> RemoveAliasList;

	if(!pDM_SPECIES || !pDM_IDX || !pDM_DONATE || !pMarc || !pOldMarc || strOldSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("연속-종-기증의 입력 - 폐간처리 관련정보를 찾을 수 없습니다!"));
		return -1;
	}
	
	//프레임에만 사용되는 DM free
	pDM_IDX->FreeData();


	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_기증관리KEY"),0); 	
	
	// 색인 DM에 1row insert
	pDM_IDX->InsertRow(-1);
	

	//Default 값을 적용 시킨다.
	SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetInsertDefaultValue_IDX(pDM_IDX, nRowIdx);
	

	//기증적용을 시킨다 
	pDM_SPECIES->SetCellData(_T("SS_기증여부"),_T("Y"),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_구입여부"),_T("N"),nRowIdx);
	
	
	//종과 마크를 가지고 색인을 구성한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);
	

	//종에 홀딩KEY 를 설정한다
	CString strHoldingKey;
	pDM_SPECIES->MakeRecKey(strHoldingKey);
	pDM_SPECIES->SetCellData(_T("SS_홀딩KEY"),strHoldingKey,nRowIdx);

	
	//startframe
	pDM_SPECIES->StartFrame();
	

	// 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES);


	// 색인에 Insert할 Insert문을 구성한다.
	pDM_IDX->SetCellData(_T("ISS_종KEY"), strSpecies_key, nRowIdx);
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES);
	

	// 기증관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);


	// 소장정보에 rec_key만 insert한다 
 	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHoldingKey);
	pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
	
	

	//기존 종 색인 UPDATE 
	CString strQuery;
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET PUB_STATUS = 'd' WHERE REC_KEY = %s;"),strOldSpeciesKey);
	pDM_SPECIES->AddFrame(strQuery);
	

	
	//기존 종 폐간 사유 UPDATE
	if(!strDiscontinueReason.IsEmpty())
	{
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET DISCONTINUE_REASON = '%s' WHERE REC_KEY = %s;"),strDiscontinueReason,strOldSpeciesKey);
		pDM_SPECIES->AddFrame(strQuery);
	}
	

	//선행 및 후속 저록 insert
	CString strWorkLogMsg;
	CString strREC_KEY1,strREC_KEY2;

	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM_SPECIES->MakeRecKey(strREC_KEY1);
	pDM_SPECIES->MakeRecKey(strREC_KEY2);
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_PREV_SPECIES_TBL (REC_KEY,SPECIES_KEY,PREV_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY1,strSpecies_key,strOldSpeciesKey,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);
	
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_NEXT_SPECIES_TBL (REC_KEY,SPECIES_KEY,NEXT_SPECIES_KEY,FIRST_WORK,LAST_WORK) VALUES (%s,%s,%s,'%s','%s');"),strREC_KEY2,strOldSpeciesKey,strSpecies_key,strWorkLogMsg,strWorkLogMsg);
	pDM_SPECIES->AddFrame(strQuery);

	
	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));
	SetIdxTmpSeTbl(pDM_SPECIES, strOldSpeciesKey, _T("I"));


	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 저장 실패!!!!"));
		return -2000;		// Species Insert Error
	}
	

	//기존 마크관련 update
	ids = UpdateSpeciesMarcbyCMarc(pDM_SPECIES,strOldSpeciesKey,pOldMarc);
	if (ids < 0) {
		AfxMessageBox(_T("마크 업데이트 실패!!!!"));
		return -2200;		// Species Insert Error
	}
	
	return 0;
}

INT CSeApi::SPECIES_PURCHASE_UPDATE(CESL_DataMgr *pDM_SPECIES, 
									CESL_DataMgr *pDM_IDX, 
									CESL_DataMgr *pDM_PURCHASE,
									CESL_DataMgr* pDM_SUB_HISTORY, 
									CESL_DataMgr* pDM_SPE_HISTORY, 
									CMarc* pMarc, 
									INT nRowIdx)
{
	// 연속-종-구입-수정의 수정 
	// 종에 1 row update
	// 색인에 1 row update
	// 구입정보에 1 row update
	
	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strPurchase_key,strSubHistory_key;

	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),nRowIdx); 	
	strSubHistory_key = pDM_SUB_HISTORY->GetCellData(_T("구독이력KEY"),nRowIdx);

	//구입정보로 구독이력정보를 만든다 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);


	//Default 값을 적용 시킨다.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetModifyDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx);
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetModifyDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx);


	//종과 마크를 가지고 색인정보를 수정한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	// 종에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 색인에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 구입정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SP_구입관리KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPurchase_key);
	

	// 구독이력정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("구독이력KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strSubHistory_key);


    // 종이력정보에 Insert문을 구성한다
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}
	

	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));

	
	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 수정실패!!!"));
		return -3000;	
	}

	return 0;
}

INT CSeApi::SPECIES_PURCHASE_UPDATE_ACQ_CHANGE(CESL_DataMgr *pDM_SPECIES, 
											   CESL_DataMgr *pDM_IDX, 
											   CESL_DataMgr *pDM_PURCHASE, 
											   CESL_DataMgr* pDM_SUB_HISTORY, 
											   CESL_DataMgr* pDM_SPE_HISTORY,
											   CMarc* pMarc, INT nRowIdx)
{
	// 종 화면에서 수입구분이 변화된상황  
	// 종에 1 row update
	// 색인에 1 row update
	// 구입정보에 1 row insert
	// 구독이력정보에 1row insert

	//프레임에만 사용되는 DM free
	pDM_SUB_HISTORY->FreeData();
	
	// 구독 이력 DM에 1row insert
	pDM_SUB_HISTORY->InsertRow(-1);

	// 구독 이력에 REC_KEY만들어 준다
	CString strSUB_HISTORY_KEY;
	pDM_SUB_HISTORY->MakeRecKey(strSUB_HISTORY_KEY);
	pDM_SUB_HISTORY->SetCellData(_T("구독이력KEY"),strSUB_HISTORY_KEY,nRowIdx);
	
	// 구독 이력KEY 를 구입정보에 넣어준다 
	pDM_PURCHASE->SetCellData(_T("SP_구독이력정보KEY"),strSUB_HISTORY_KEY,nRowIdx);

	//구입정보로 구독이력정보를 만든다 
	MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,nRowIdx);

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strPurchase_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strPurchase_key = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),nRowIdx); 	
	

	//Default 값을 적용 시킨다.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetInsertDefaultValue_PURCHASE(pDM_PURCHASE, nRowIdx); //구입은 입력으로 setting한다.
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);
	SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY, nRowIdx); //구독이력은 입력으로 setting한다.


	//종과 마크를 가지고 색인정보를 수정한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);


	// 종에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 색인에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 구입관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);

	
	// 구독이력정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), nRowIdx , pDM_SPECIES);
	
	 // 종이력정보에 Insert문을 구성한다
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}

	
	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 수정실패!!!"));
		return -3000;	
	}
	
	return 0;

	
}

INT CSeApi::SPECIES_DONATE_UPDATE(CESL_DataMgr *pDM_SPECIES, 
								  CESL_DataMgr *pDM_IDX, 
								  CESL_DataMgr *pDM_DONATE,
								  CESL_DataMgr* pDM_SPE_HISTORY,
								  CMarc* pMarc, 
								  INT nRowIdx)
{
	// 연속-종-기증 의수정 
	// 종에 1 row update
	// 색인에 1 row update
	// 기증정보에 1 row update

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_기증관리KEY"),0); 	
	
	//Default 값을 적용 시킨다.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetModifyDefaultValue_DONATE(pDM_DONATE, nRowIdx);
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);

	//종과 마크를 가지고 색인정보를 수정한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	// 종에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("MARC"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 색인에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 기증정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SD_기증관리KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strDonate_key);

    
	// 종이력정보에 Insert문을 구성한다
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}
	
	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSpecies_key, _T("I"));


	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 수정실패!!!"));
		return -4000;	
	}
	
	return 0;

}

INT CSeApi::SPECIES_DONATE_UPDATE_ACQ_CHANGE(CESL_DataMgr *pDM_SPECIES, 
											 CESL_DataMgr *pDM_IDX, 
											 CESL_DataMgr *pDM_DONATE,
											 CESL_DataMgr* pDM_SPE_HISTORY,
											 CMarc* pMarc, 
											 INT nRowIdx)
{
	// 종화면에서 수입 구분이 변화된 상태 
	// 종에 1 row update
	// 색인에 1 row update
	// 기증정보에 1 row insert

	CArray <CString, CString> RemoveAliasList;
	pDM_SPECIES->StartFrame();

	INT ids;
	CString strSpecies_key, strDonate_key;
	strSpecies_key = pDM_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx); 
	strDonate_key = pDM_DONATE->GetCellData(_T("SD_기증관리KEY"),0); 	
	
	//Default 값을 적용 시킨다.
	SetModifyDefaultValue_SPECIES(pDM_SPECIES,nRowIdx);
	SetInsertDefaultValue_DONATE(pDM_DONATE, nRowIdx); //기증은 insert
	SetModifyDefaultValue_IDX(pDM_IDX, nRowIdx);


	//종과 마크를 가지고 색인정보를 수정한다
	MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,nRowIdx);

	// 종에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	//RemoveAliasList.Add(_T("MARC"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	RemoveAliasList.Add(_T("SS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), nRowIdx , pDM_SPECIES,_T("REC_KEY"), _T("NUMERIC"), strSpecies_key);

	// 색인에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("ISS_종KEY"));
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), nRowIdx, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);


	// 기증관리정보에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_DONATE, _T("SE_DONATE_MANAGE_TBL"), nRowIdx , pDM_SPECIES);
	
	
	// 종이력정보에 Insert문을 구성한다
	for(INT i=0;i<pDM_SPE_HISTORY->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPE_HISTORY, _T("SE_SPECIES_HISTORY_TBL"), i , pDM_SPECIES);
	}


	// 종관련 Send Frame
	ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("종 수정실패!!!"));
		return -4000;	
	}

	return 0;

}


INT CSeApi::SPECIES_DELETE(CString strSPECIES_KEY,
						   CESL_DataMgr* pDM_SPECIES,
						   CESL_DataMgr* pDM_SPECIES_DELETE, 
						   CESL_DataMgr* pDM_VOL,
						   CESL_DataMgr* pDM_VOL_DELETE,
						   CESL_DataMgr* pDM_BOOK,
						   CESL_DataMgr* pDM_DROP_NO,
						   INT nACQ_TYPE)
{
	//종 삭제 함수 
	//다른 수입구분을 확인해 종을 삭제할 것인 지를 판단한다.
	//종 1 row delete 삭제종 1 row insert
	//색인의 작업 상태 update
	//권 n row delete 삭제권 n row insert
	//그외 관련 delete
	
	if(!pDM_SPECIES || !pDM_SPECIES_DELETE || !pDM_VOL || !pDM_VOL_DELETE || strSPECIES_KEY.IsEmpty())
	{
		AfxMessageBox(_T("종 삭제 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strQuery,strACQ_KEY,strWhere;
	INT nDeleteSpeciesOption = 0;

	
	//프레임 관련 DM을 FREE 한다
	pDM_SPECIES->FreeData();
	pDM_SPECIES_DELETE->FreeData();
	pDM_VOL->FreeData();
	pDM_VOL_DELETE->FreeData();
	pDM_BOOK->FreeData();
	pDM_DROP_NO->FreeData();
	

  	//다른쪽 수입 구분을 파악한다. 
	if(1 == nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	
	CString tmp;
	pDM_SPECIES->GetOneValueQuery(strQuery,tmp);
	//다른 다른 수입 구분이 존재하지 않는 경우 
	if(tmp.IsEmpty()) nDeleteSpeciesOption = 1;

	
	//삭제에 필요한 기본값을 가지고 온다
	CString strToday;
	CString strWorkLog;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);


	pDM_SPECIES->StartFrame();

	const INT cnt = 5;
	CString strData[cnt]; //ACQ_KEY, 입수 예측 TYPE, 입수예측정보KEY,  발행 예측 TYPE, 발행일예측정보KEY

  	if(1 == nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
		
	pDM_SPECIES->GetValuesQuery(strQuery,strData,cnt);
	
	
	//소장정보 KEY;
	CString strHoldingKey;
	strQuery.Format(_T("SELECT HOLDING_KEY FROM SE_SPECIES_TBL WHERE REC_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetOneValueQuery(strQuery,strHoldingKey);


	//구입 또는 기증관리 KEY
	strACQ_KEY = strData[0];


	//입수 예측 정보
	INT ntype = -1;
	CString strTBL_NAME;
	if(!strData[1].IsEmpty() && !strData[2].IsEmpty())
	{
		strTBL_NAME = GetExpTable(strData[1]);
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strData[2]);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//발행일 예측 정보 
	ntype = -1;
	if(!strData[3].IsEmpty() && !strData[4].IsEmpty()) 
	{
		strTBL_NAME = GetExpTable(strData[3]);
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strData[4]);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//부록 예측 정보 
	if(!strACQ_KEY.IsEmpty()) {
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	}

	//권호 예측 정보 
	if(!strACQ_KEY.IsEmpty()) {
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	//구독 이력 정보 
	if(1 == nACQ_TYPE) 
	{
		if(!strACQ_KEY.IsEmpty()) {
			strQuery.Format(_T("DELETE FROM SE_SUBSCRIPTION_HISTORY_TBL WHERE PURCHASE_MANAGE_KEY = %s;"),strACQ_KEY); 
			pDM_SPECIES->AddFrame(strQuery);
		}
	}
	
	
	//클레임 이력 정보를 삭제한다. 
	if(!strACQ_KEY.IsEmpty()) {
		
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);
	}
	
	
	//책관련 정보 삭제 -- 등록번호가 있는 책은 누락 등록번호 TBL에 INSERT한다 (제본책 제외)
	
	/*
	strWhere.Format(_T("B.BOOK_TYPE != 'B' AND B.ACQ_CODE = '%d' AND B.ACCESSION_REC_KEY = C.REC_KEY (+)"),nACQ_TYPE);
	pDM_BOOK->RefreshDataManager(strWhere);
	*/

	//책 누락능록번호 처리 
	//MAKE_DROP_ACCESSION_NO_by_BOOK(pDM_SPECIES,pDM_BOOK,pDM_DROP_NO);
	

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//누락 등록 번호 처리 2004-03-04수정
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	pDM_SPECIES->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s', UDF_MANAGE_CODE")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,nACQ_TYPE,strSPECIES_KEY);
// 	pDM_SPECIES->AddFrame(strQuery);

	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s', UDF_MANAGE_CODE")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,nACQ_TYPE,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	//=====================================================
	
	


	//책정보 삭제
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE BOOK_TYPE != 'B' AND ACQ_CODE = '%d' AND SPECIES_KEY = %s;"),nACQ_TYPE,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	


	//수입구분별 권호정보
	if(!strACQ_KEY.IsEmpty()) {

		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);
	
	}
	
	//구입 - 기증 정보
	if(1 == nACQ_TYPE) 
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	else if(2 == nACQ_TYPE) 
		strQuery.Format(_T("DELETE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddFrame(strQuery);

	
	//권정보를 가져온다.
	strWhere.Format(_T("V.VOL_TYPE = 'V' AND V.SPECIES_KEY = %s"),strSPECIES_KEY); //제본 권호가 아닌 권호
	pDM_VOL->RefreshDataManager(strWhere);
	

	//권을 삭제할 것인지에 대한 여부를 판단 해 삭제할 경우는 삭제권으로 MOVE
	//종 삭제가 가능한가여부 판단 	
	//관련 테이블(기사,기사색인,목차,초록,기사목차삭제)
	CString strUDF_KEY,strOtherAcqCount,strVOL_KEY;
	INT nOtherAcqCount = 0;
	INT nRowDeleteVol = 0;
	CArray<CString,CString> RemoveAliasList;

	for(INT i=0;i<pDM_VOL->GetRowCount();i++)
	{
		
		strUDF_KEY =_T("");
		strOtherAcqCount = _T("");

		strOtherAcqCount = pDM_VOL->GetCellData(_T("UDF_기타수입구분책수"),i);
		nOtherAcqCount = _ttoi(strOtherAcqCount.GetBuffer(0));
	
		if(1 == nACQ_TYPE) { 
			strUDF_KEY = pDM_VOL->GetCellData(_T("UDF_기증수입구분별권KEY"),i);
		} else if(2 == nACQ_TYPE) {
			strUDF_KEY = pDM_VOL->GetCellData(_T("UDF_구입수입구분별권KEY"),i);
		}
		
		if(strUDF_KEY.IsEmpty() && 0 == nOtherAcqCount)
		{
			//권삭제
			strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),i);
						
			pDM_VOL_DELETE->InsertRow(-1);	
			nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
			CopyDMToDMByAlias(pDM_VOL,i,pDM_VOL_DELETE,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_삭제일자"),strToday,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_최초작업"),strWorkLog,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_마지막작업"),strWorkLog,nRowDeleteVol);

			RemoveAliasList.RemoveAll();
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_SPECIES);

			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

			//색인 권호명 setting
			MakeEcoIndexVol(pDM_SPECIES,strVOL_KEY,FALSE);

			//가사,목차,초록관련 삭제
			GetArticleDeleteQuery(pDM_SPECIES,strVOL_KEY);
		}


		if(0 != nOtherAcqCount && 1 == nDeleteSpeciesOption) //다른 수입구분이 존재하는 경우
		{
			nDeleteSpeciesOption = 0;
		}

	}

		
	if(1 == nDeleteSpeciesOption) //종을 삭제 해야 하는 상황
	{
		//종을 삭제종으로 MOVE 
		strWhere.Format(_T("REC_KEY = %s"),strSPECIES_KEY);
		pDM_SPECIES->RefreshDataManager(strWhere);
		pDM_SPECIES_DELETE->InsertRow(-1);
		CopyDMToDMByAlias(pDM_SPECIES,0,pDM_SPECIES_DELETE,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_삭제일자"),strToday,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_최초작업"),strWorkLog,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_마지막작업"),strWorkLog,0);
		pDM_SPECIES_DELETE->SetCellData(_T("SS_작업상태구분"),_T("3"),0); //작업 상태 구분을 삭제로 표시

		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES_DELETE, _T("SE_DELETE_SPECIES_TBL"), 0 , pDM_SPECIES);
		
		strQuery.Format(_T("DELETE FROM SE_SPECIES_TBL WHERE REC_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddFrame(strQuery);

		
		if(!strHoldingKey.IsEmpty())
		{
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_HOLDING_TBL WHERE REC_KEY = %s;"),strHoldingKey);
			pDM_SPECIES->AddFrame(strQuery);
		}

		//색인 update
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '3' WHERE REC_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//IDX_TMP_SE_TBL 설정
		SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("D"));
		

		//제본관련(제본 플래그 책, 제본책, 제본권호,제본정보) delete 
		//제본책 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
						_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s AND BOOK_TYPE = 'B');"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		
		
		//제본 플래그 책
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE SPECIES_KEY = %s AND BOOK_TYPE = 'B';"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//제본권호 (삭제 권 정보로 이동)
		pDM_VOL->FreeData();
		pDM_VOL_DELETE->FreeData();


		//제본 권호 정보를 가져온다.
		strWhere.Format(_T("V.VOL_TYPE = 'B' AND V.SPECIES_KEY = %s"),strSPECIES_KEY); 
		pDM_VOL->RefreshDataManager(strWhere);
		
		for(INT i=0;i<pDM_VOL->GetRowCount();i++)
		{
			//제본권 삭제(이동)
			strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),i);
				
			pDM_VOL_DELETE->InsertRow(-1);	
			nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
			CopyDMToDMByAlias(pDM_VOL,i,pDM_VOL_DELETE,nRowDeleteVol);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
			pDM_VOL_DELETE->SetCellData(_T("SV_삭제일자"),strToday,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_최초작업"),strWorkLog,nRowDeleteVol);
			pDM_VOL_DELETE->SetCellData(_T("SV_마지막작업"),strWorkLog,nRowDeleteVol);

			RemoveAliasList.RemoveAll();
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_SPECIES);

			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_SPECIES->AddFrame(strQuery);
	
		}

		//제본 정보
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_BINDING_INFO_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);

		
		//선행저록
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_PREV_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//후속저록
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_NEXT_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//종 이력정보
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_SPECIES_HISTORY_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
		pDM_SPECIES->AddFrame(strQuery);

	}

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if(ids<0) return -1000;

	return 0;
}



INT CSeApi::SPECIES_CAT_DELETE(CString strSPECIES_KEY,
							   CESL_DataMgr* pDM_SPECIES,
						       CESL_DataMgr* pDM_SPECIES_DELETE, 
						       CESL_DataMgr* pDM_VOL)
{
	//정리에의 종 삭제 함수 --> 수서에도 적용
	//모든 수입구분의 자료를 삭제한다
	//종 1 row delete 삭제종 1 row insert
	//색인의 작업 상태 update
	//권 n row delete 삭제권 n row insert
	//그외 관련 delete
	
	if(!pDM_SPECIES || !pDM_SPECIES_DELETE || !pDM_VOL || strSPECIES_KEY.IsEmpty())
	{
		AfxMessageBox(_T("종 삭제 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strQuery,strACQ_KEY,strWhere,strTBL_NAME;


	/////////////////////////////////////////////////
	//종 정보를 보여준 후 한번더 삭제 의사를 묻는다//
	/////////////////////////////////////////////////
	CSeSpeciesDelete dlg(m_pMgr);
	dlg.m_strSpeciesKey = strSPECIES_KEY;
	if(dlg.DoModal() != IDOK) return -1001; //procedure cancel


	///////////////////////////////////////////////////////////
	//해당 종내의 책 대출, 수리제본, 일반제본 상태를 확인한다//
	///////////////////////////////////////////////////////////

	//대출
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s)")
					,strSPECIES_KEY);

	INT nCount = 0;
	CString strCount;

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("해당 종 내에 대출 혹은 예약중인 책이 있습니다!\n확인하시고 다시 삭제해 주십시요!"));
			return -1002; //verifying fail
		}

	}

	//수리제본
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s)")
					,strSPECIES_KEY);

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("해당 종 내에 수리제본에 등록 또는 의뢰한 책이 있습니다!\n확인하시고 다시 삭제해 주십시요!"));
			return -1002; //verifying fail
		}

	}

	//일반 제본
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ('SEB112N','SEB113N') AND SPECIES_KEY = %s")
					,strSPECIES_KEY);

	pDM_SPECIES->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			AfxMessageBox(_T("해당 종 내에 연속 제본의 결정 혹은 발주 상태의 책이 있습니다!\n확인하시고 다시 삭제해 주십시요!"));
			return -1002; //verifying fail
		}

	}

	
	//프레임 관련 DM을 FREE 한다
	pDM_SPECIES->FreeData();
	pDM_SPECIES_DELETE->FreeData();
	pDM_VOL->FreeData();
	
	
	//삭제에 필요한 기본 값을 가져온다 
	CString strToday;
	CString strWorkLog;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);


	////////////////////////////////////////////////////////////////
	//구입, 기증 관리 테이블에서 삭제에 필요한 KEY들을 가지고 온다//
	////////////////////////////////////////////////////////////////

	const INT cnt = 5;
	CString strPurData[cnt]; //ACQ_KEY, 입수 예측 TYPE, 입수예측정보KEY,  발행 예측 TYPE, 발행일예측정보KEY
	CString strDonData[cnt]; 
	//구입관리 데이터 
	strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetValuesQuery(strQuery,strPurData,cnt);
	//기증관리 데이터 
	strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_TYPE,ACQ_EXP_KEY,PUB_EXP_TYPE,PUB_EXP_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->GetValuesQuery(strQuery,strDonData,cnt);
	
	
	pDM_SPECIES->StartFrame();


	//////////////////////////////////////////////////////////////////////////////////
	//구입 관리와 연결된  테이블 삭제 (예측,구독이력,클레임,수입구분별 권,구입관리) //
	//////////////////////////////////////////////////////////////////////////////////

	if(!strPurData[0].IsEmpty())
	{
		
		strACQ_KEY = strPurData[0];
		
		//입수일 예측 정보
		if(!strPurData[1].IsEmpty() && !strPurData[2].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strPurData[1]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strPurData[2]);
			pDM_SPECIES->AddFrame(strQuery);
		}
	
		//발행일 예측 정보 
		if(!strPurData[3].IsEmpty() && !strPurData[4].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strPurData[3]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strPurData[4]);
			pDM_SPECIES->AddFrame(strQuery);
		}


		//부록 예측 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//권호 예측 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		
		
		//구독 이력 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_SUBSCRIPTION_HISTORY_TBL WHERE PURCHASE_MANAGE_KEY = %s;"),strACQ_KEY); 
		pDM_SPECIES->AddFrame(strQuery);
	
		
		//클레임 이력 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
		

		//수입구분별 권 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	
		//구입관리 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_MANAGE_TBL WHERE REC_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);	

	}


	///////////////////////////////////////////////////////////////
	//기증 관리와 연결된  테이블 삭제 (예측,클레임,수입구분별 권)//
	///////////////////////////////////////////////////////////////

	if(!strDonData[0].IsEmpty())
	{
		
		strACQ_KEY = strDonData[0];
		
		//입수일 예측 정보
		if(!strDonData[1].IsEmpty() && !strDonData[2].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strDonData[1]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '0';"),strTBL_NAME,strDonData[2]);
			pDM_SPECIES->AddFrame(strQuery);
		}
	
		//발행일 예측 정보 
		if(!strDonData[3].IsEmpty() && !strDonData[4].IsEmpty()) 
		{
			strTBL_NAME = GetExpTable(strDonData[3]);
			pDM_SPECIES->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '1';"),strTBL_NAME,strDonData[4]);
			pDM_SPECIES->AddFrame(strQuery);
		}


		//부록 예측 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//권호 예측 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);

		
		//클레임 이력 정보
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s);"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);


		//수입구분별 권 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);
	

		//기증관리 정보 
		pDM_SPECIES->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_DONATE_MANAGE_TBL WHERE REC_KEY = %s;"),strACQ_KEY);
		pDM_SPECIES->AddFrame(strQuery);		

	}


	
	//////////////////////////////////////////////////////////////
	//책관련정보 삭제(책,대출관련,누락등록번호 처리,부록,제본책)//
	//////////////////////////////////////////////////////////////
	

	//대출 메일 정보
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

   
	//대출 정보 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	

	//배달 자료 정보 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

		
	//수리 제본 정보 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//부록정보 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);	
	pDM_SPECIES->AddFrame(strQuery);
	

	//제본책 
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

	

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락 등록 번호 처리 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
//	pDM_SPECIES->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strSPECIES_KEY);
// 	pDM_SPECIES->AddFrame(strQuery);

		pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
	//===================================================== 

	//책정보 삭제
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);	
	pDM_SPECIES->AddFrame(strQuery);



	///////////////////////////////////////////////////////////////////
	//권호 관련 삭제(권호,삭제권입력,기사,기사색인,목차초록,권호색인)//
	///////////////////////////////////////////////////////////////////
	
	//권호 정보를 가져온다.
	strWhere.Format(_T("V.SPECIES_KEY = %s"),strSPECIES_KEY); 
	pDM_VOL->RefreshDataManager(strWhere);
	
	CString strVOL_KEY;
	INT nRowDeleteVol = 0;

	CArray<CString,CString> RemoveAliasList;
	
	//삭제권호에 입력
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s', UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE SPECIES_KEY = %s);"),strToday,strWorkLog,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	for(INT i=0;i<pDM_VOL->GetRowCount();i++)
	{
		
		strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),i);
		
		if(strVOL_KEY.IsEmpty()) continue;

		//색인 권호명 setting
		MakeEcoIndexVol(pDM_SPECIES,strVOL_KEY,FALSE);

	}


	//기사,목차,초록관련 삭제
	GetArticleDeleteQuerybySpecies(pDM_SPECIES,strSPECIES_KEY);
	

	//권호 정보
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

	
	///////////////////////////////////////////////////////////////////////////////////////
	//종 관련 삭제(소장정보, 종, 선행저록, 후속저록,종 이력정보, 제본정보, 색인 업데이트)//
	///////////////////////////////////////////////////////////////////////////////////////

	
	//소장정보
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_HOLDING_TBL WHERE REC_KEY IN (SELECT HOLDING_KEY FROM SE_SPECIES_TBL ")
					_T("WHERE REC_KEY = %s);"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);



	//제본 정보
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_INFO_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//선행저록
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_PREV_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//후속저록
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_NEXT_SPECIES_TBL WHERE (REC_KEY = %s OR SPECIES_KEY = %s);"),strSPECIES_KEY,strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//종 이력정보
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_SPECIES_HISTORY_TBL WHERE SPECIES_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);


	//종을 삭제종으로 MOVE 
	strWhere.Format(_T("REC_KEY = %s"),strSPECIES_KEY);
	pDM_SPECIES->RefreshDataManager(strWhere);
	pDM_SPECIES_DELETE->InsertRow(-1);
	CopyDMToDMByAlias(pDM_SPECIES,0,pDM_SPECIES_DELETE,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_삭제일자"),strToday,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_최초작업"),strWorkLog,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_마지막작업"),strWorkLog,0);
	pDM_SPECIES_DELETE->SetCellData(_T("SS_작업상태구분"),_T("3"),0); //작업 상태 구분을 삭제로 표시

	RemoveAliasList.RemoveAll();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES_DELETE, _T("SE_DELETE_SPECIES_TBL"), 0 , pDM_SPECIES);
	

	//종삭제
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_SPECIES_TBL WHERE REC_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);

		
	//색인 update
	pDM_SPECIES->InitDBFieldData();
	strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '3' WHERE REC_KEY = %s;"),strSPECIES_KEY);
	pDM_SPECIES->AddFrame(strQuery);
		

	//IDX_TMP_SE_TBL 설정
	SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("D"));
		

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	if(ids<0) return -1003; //db oepration error

	return 0;
}




INT CSeApi::GetArticleDeleteQuery(CESL_DataMgr* pDM_OUT, CString strVOL_KEY)
{
	//권호키로 관련 기사정보들을 삭제하는 퀴리를 만드는 함수 
	CString strQuery;

	//2003/03/22 추가
	//IDX_TMP_SE_TOC_TBL에 삭제될 SE_ARTICLE_TBL를 INSERT한다
	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TOC_TBL ")
					_T("(SELECT %s.NEXTVAL,REC_KEY,'D','%s' FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);")
					,DEFAULT_SEQ_NAME,GetTodayDate(),strVOL_KEY);

	pDM_OUT->AddFrame(strQuery);


	//SE_ABSTRACTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ABSTRACTS_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//SE_ARTICLE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TOC_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//SE_ARTICLE_TBL 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//IDX_SE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM IDX_SE_TOC_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);


	//SE_CONTENTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CONTENTS_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_OUT->AddFrame(strQuery);

	return 0;
}

INT CSeApi::GetArticleDeleteQuerybySpecies(CESL_DataMgr* pDM_OUT, CString strSPECIES_KEY)
{
	
	//종키로 관련 기사정보들을 삭제하는 퀴리를 만드는 함수 (정리용)
	CString strQuery;


	//2003/03/22 추가
	//IDX_TMP_SE_TOC_TBL에 삭제될 SE_ARTICLE_TBL를 INSERT한다
	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TOC_TBL ")
					_T("(SELECT %s.NEXTVAL,REC_KEY,'D','%s' FROM SE_ARTICLE_TBL WHERE VOL_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY = %s));")
					,DEFAULT_SEQ_NAME,GetTodayDate(),strSPECIES_KEY);

	pDM_OUT->AddFrame(strQuery);
	


	//SE_ABSTRACTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ABSTRACTS_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s));"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//SE_ARTICLE_TOC_TBL		
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TOC_TBL WHERE ARTICLE_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s));"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);

	

	//SE_ARTICLE_TBL 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ARTICLE_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//IDX_SE_TOC_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM IDX_SE_TOC_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);



	//SE_CONTENTS_TBL
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CONTENTS_TBL WHERE VOL_KEY ")
					_T("IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE SPECIES_KEY= %s);"),strSPECIES_KEY);
	pDM_OUT->AddFrame(strQuery);


	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_INSERT(CESL_DataMgr* pDM, 
								   CMarc* pMarc, 
								   CESL_DataMgr* pDM_SEQ_NO, 
								   CESL_DataMgr* pDM_WORK_NO, 
								   BOOL IsNewWorkNo, 
								   INT nRowIdx,
								   BOOL bIsDepartMngUse)
{
	
	// 연속-구입대상자료 입력 
	if(!pDM || !pMarc || !pDM_SEQ_NO || !pDM_WORK_NO)
	{
		AfxMessageBox(_T("구입대상자료 입력 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}


	if(0 == pDM_SEQ_NO->GetRowCount())
	{
		AfxMessageBox(_T("차수 정보를 찾을 수 없습니다!"));
		return -2;
	}

	//프레임 관련 free
	pDM_WORK_NO->FreeData();


	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM->MakeRecKey(strREC_KEY);

	pDM->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
	pDM->SetCellData(_T("입력일자"),GetTodayDate(),nRowIdx);
	pDM->SetCellData(_T("최초작업"),strWorkLog,nRowIdx);		
	pDM->SetCellData(_T("마지막작업"),strWorkLog,nRowIdx);	


	//입력 구분 처리 
	CString strINPUT_TYPE;
	strINPUT_TYPE = pDM->GetCellData(_T("입력구분"),nRowIdx);	
	if(strINPUT_TYPE.IsEmpty())
	{
		pDM->SetCellData(_T("입력구분"),_T("0"),nRowIdx);	 //화면 입력
	}
	
	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);

	//차수번호, 작업번호 처리
	BOOL bIsInsertWorkNo = FALSE;
	CString strLastWorkNo = _T("");

	if(IsNewWorkNo) //새 작업 번호 
	{
		strLastWorkNo = GetLastWorkNo(pDM_SEQ_NO,strSEQ_NO_KEY);

		pDM->SetCellData(_T("차수내작업번호"),strLastWorkNo,nRowIdx);
		pDM->SetCellData(_T("차수일련번호"),_T("1"),nRowIdx);

		pDM_WORK_NO->InsertRow(-1);
		CString strWorkKey;
		pDM_WORK_NO->MakeRecKey(strWorkKey);
		
		pDM_WORK_NO->SetCellData(_T("REC_KEY"),strWorkKey,0);
		pDM_WORK_NO->SetCellData(_T("차수관리KEY"),strSEQ_NO_KEY,0);
		pDM_WORK_NO->SetCellData(_T("작업번호"),strLastWorkNo,0);
		pDM_WORK_NO->SetCellData(_T("마지막일련번호"),_T("1"),0);		
		pDM_WORK_NO->SetCellData(_T("생성일"),GetTodayDate(),0);
		pDM_WORK_NO->SetCellData(_T("생성자"),m_pMgr->m_pInfo->USER_ID,0);
		pDM_WORK_NO->SetCellData(_T("최초작업"),strWorkLog,0);		
		pDM_WORK_NO->SetCellData(_T("마지막작업"),strWorkLog,0);	

		bIsInsertWorkNo = TRUE; //insert

	}
	else //기존 작업번호 
	{
		CString strWorkNo = _T("");
		strWorkNo = pDM->GetCellData(_T("차수내작업번호"),nRowIdx);
		
		CString strSerialWorkNo = _T("");
		strSerialWorkNo = GetSerialWorkNo(pDM_WORK_NO,strSEQ_NO_KEY,strWorkNo);
		pDM->SetCellData(_T("차수일련번호"),strSerialWorkNo,nRowIdx);
	}	


	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	
	// 구입 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM);
	

	//새 작업 번호 
	if(bIsInsertWorkNo) 
	{
		RemoveAliasList.RemoveAll();
		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , pDM);
	}


	// 구입 종관련 Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}

	//부서 지정자료 사용시 마크에서 서명과 발행자를 뽑아 각각의 필드에 넣어준다
	if(bIsDepartMngUse)
	{
		CString strTitle;
		CString strPublisher;
		CString strQuery;

		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("245$a"),strTitle);
		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("260$b"),strPublisher);
		strQuery.Format(_T("UPDATE SE_PURCHASE_SPECIES_TBL SET TITLE = '%s', PUBLISHER = '%s' ")
			            _T("WHERE REC_KEY = %s;"),strTitle,strPublisher,strREC_KEY);
		
		ids = pDM->ExecuteQuery(strQuery,1);

		if (ids < 0) {
			return -3001;	
		}

	}

	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_UPDATE(CESL_DataMgr* pDM, 
								   CMarc* pMarc, 
								   CESL_DataMgr* pDM_SEQ_NO, 
								   CESL_DataMgr* pDM_WORK_NO, 
								   BOOL IsNewWorkNo, 
								   BOOL IsWorkNoChanged,
								   INT nRowIdx,
								   BOOL bIsDepartMngUse)
{
	// 연속-구입대상자료 수정 
	if(!pDM || !pMarc || !pDM_SEQ_NO || !pDM_WORK_NO)
	{
		AfxMessageBox(_T("구입대상자료 수정 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}


	if(0 == pDM_SEQ_NO->GetRowCount())
	{
		AfxMessageBox(_T("차수 정보를 찾을 수 없습니다!"));
		return -2;
	}
	
	
	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strREC_KEY = pDM->GetCellData(_T("REC_KEY"),nRowIdx);

	pDM->SetCellData(_T("마지막작업"),strWorkLog,nRowIdx);	

	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	

	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	//차수번호, 작업번호 처리
	BOOL bIsInsertWorkNo = FALSE;
	CString strLastWorkNo = _T("");

	if(IsNewWorkNo) //새 작업 번호 
	{
		strLastWorkNo = GetLastWorkNo(pDM_SEQ_NO,strSEQ_NO_KEY);

		pDM->SetCellData(_T("차수내작업번호"),strLastWorkNo,nRowIdx);
		pDM->SetCellData(_T("차수일련번호"),_T("1"),nRowIdx);

		pDM_WORK_NO->InsertRow(-1);
		CString strWorkKey;
		pDM_WORK_NO->MakeRecKey(strWorkKey);
		
		pDM_WORK_NO->SetCellData(_T("REC_KEY"),strWorkKey,0);
		pDM_WORK_NO->SetCellData(_T("차수관리KEY"),strSEQ_NO_KEY,0);
		pDM_WORK_NO->SetCellData(_T("작업번호"),strLastWorkNo,0);
		pDM_WORK_NO->SetCellData(_T("마지막일련번호"),_T("1"),0);		
		pDM_WORK_NO->SetCellData(_T("생성일"),GetTodayDate(),0);
		pDM_WORK_NO->SetCellData(_T("생성자"),m_pMgr->m_pInfo->USER_ID,0);
		pDM_WORK_NO->SetCellData(_T("최초작업"),strWorkLog,0);		
		pDM_WORK_NO->SetCellData(_T("마지막작업"),strWorkLog,0);	

		bIsInsertWorkNo = TRUE; //insert

	}
	else //기존 작업번호 (작업번호가 수정 되었는 지를 판단한다 )
	{
		
		if(IsWorkNoChanged)
		{

			CString strWorkNo = _T("");
			strWorkNo = pDM->GetCellData(_T("차수내작업번호"),nRowIdx);
			
			CString strSerialWorkNo = _T("");
			strSerialWorkNo = GetSerialWorkNo(pDM_WORK_NO,strSEQ_NO_KEY,strWorkNo);
			pDM->SetCellData(_T("차수일련번호"),strSerialWorkNo,nRowIdx);

		}

	}	


	// 구입 종에 update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("REC_KEY"));
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM, _T("REC_KEY") , _T("NUMERIC") , strREC_KEY);
	
	
	//새 작업 번호 
	if(bIsInsertWorkNo) 
	{
		RemoveAliasList.RemoveAll();
		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , pDM);
	}

	// 구입 종관련 Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("구입 종 수정 실패!!!!"));
		return -4000;	
	}

	//부서 지정자료 사용시 마크에서 서명과 발행자를 뽑아 각각의 필드에 넣어준다
	if(bIsDepartMngUse)
	{
		CString strTitle;
		CString strPublisher;
		CString strQuery;

		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("245$a"),strTitle);
		m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("260$b"),strPublisher);
		strQuery.Format(_T("UPDATE SE_PURCHASE_SPECIES_TBL SET TITLE = '%s', PUBLISHER = '%s' ")
			            _T("WHERE REC_KEY = %s;"),strTitle,strPublisher,strREC_KEY);
		
		ids = pDM->ExecuteQuery(strQuery,1);

		if (ids < 0) {
			return -4001;	
		}

	}
	
	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_DELETE(CESL_DataMgr* pDM, INT nRowIdx)
{
	
	CString strREC_KEY;
	strREC_KEY = pDM->GetCellData(_T("REC_KEY"),nRowIdx);
	if(strREC_KEY.IsEmpty()) return -1;

	CString strQuery;
	strQuery.Format(_T("DELETE FROM SE_PURCHASE_SPECIES_TBL WHERE REC_KEY = %s;"),strREC_KEY);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids<0) return -7000;


	return 0;
}

INT CSeApi::SPECIES_PUR_OBJ_FILE_IMPORT(CESL_DataMgr* pDM,
										CMarc* pMarc, 
										INT nRowIdx)
{

	// 연속-구입대상자료 입력  (파일 반입)
	CString strREC_KEY;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	pDM->MakeRecKey(strREC_KEY);

	pDM->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
	pDM->SetCellData(_T("입력일자"),GetTodayDate(),nRowIdx);
	pDM->SetCellData(_T("최초작업"),strWorkLog,nRowIdx);		
	pDM->SetCellData(_T("마지막작업"),strWorkLog,nRowIdx);	

	//입력 구분 처리 
	CString strINPUT_TYPE;
	strINPUT_TYPE = pDM->GetCellData(_T("입력구분"),nRowIdx);	
	if(strINPUT_TYPE.IsEmpty())
	{
		pDM->SetCellData(_T("입력구분"),_T("0"),nRowIdx);	 //화면 입력
	}

	CArray<CString, CString> RemoveAliasList;
	pDM->StartFrame();
	
	// 구입 종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("SE_PURCHASE_SPECIES_TBL"), nRowIdx , pDM);
	

	// 구입 종관련 Send Frame
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}
	
	return 0;
}

INT CSeApi::SPECIES_ORDER(CESL_DataMgr* pDM_PUR_SPECIES, 
						  CESL_DataMgr* pDM_SPECIES, 
						  CESL_DataMgr* pDM_IDX,
						  CESL_DataMgr* pDM_PURCHASE,
						  CESL_DataMgr* pDM_SUB_HISTORY,
						  CMarc* pMarc,
						  INT nRowIdx)
{

	// 주문처리 함수 
	// 기존 종의 존재 여부를 판단해 없으면 만들어 주고 있으면 구입관리 정보를 갱신한다.

	if(!pDM_PUR_SPECIES || !pDM_SPECIES || !pDM_IDX || !pDM_PURCHASE || !pDM_SUB_HISTORY)
	{
		AfxMessageBox(_T("주문관련 정보를 찾을 수 없습니다!"));
		return -1;
	}
	
	//프레임 관련 DM free
	pDM_SPECIES->FreeData();
	pDM_IDX->FreeData();
	pDM_PURCHASE->FreeData();
	pDM_SUB_HISTORY->FreeData();


	CString strSPECIES_KEY = _T("");
	CString strWhere;
	CArray<CString,CString> RemoveAliasList;
	strSPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("SS_종KEY"),nRowIdx);
	CString strQuery;

	CString strSEQ_NO = _T("");
	strSEQ_NO =	pDM_PUR_SPECIES->GetCellData(_T("차수번호"),nRowIdx);


	INT ids = 0;

	BOOL bDepartmentOrderUse = IsDepartMngUse(pDM_SPECIES);

	if(!strSPECIES_KEY.IsEmpty()) //기존의 종이 존재하는 상태 (구입 정보 자료와 구독이력만 처리해 준다);
	{
		
		strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
		pDM_PURCHASE->RefreshDataManager(strWhere);
		if(0 == pDM_PURCHASE->GetRowCount()) 
		{
			AfxMessageBox(_T("해당 종의 구입정보를 찾을 수 없습니다!"));
			return -2;
		}
		
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_PURCHASE,0);
		pDM_SUB_HISTORY->InsertRow(-1);
		
		
		//구입정보로 구독이력정보를 만든다 
		MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,0);
		
		CString strPREV_HISTORY_KEY,strHISTORY_KEY,strPURCHASE_KEY,strPUR_SPECIES_KEY;
		strPUR_SPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("REC_KEY"),nRowIdx);
		strPURCHASE_KEY = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),0);
		strPREV_HISTORY_KEY = pDM_PURCHASE->GetCellData(_T("구독이력KEY"),0);
		pDM_SPECIES->MakeRecKey(strHISTORY_KEY);
		

		pDM_SUB_HISTORY->SetCellData(_T("구독이력KEY"),strHISTORY_KEY,0);
		pDM_PUR_SPECIES->SetCellData(_T("현재구독이력KEY"),strHISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("이전구독이력KEY"),strPREV_HISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("주문일자"),GetTodayDate(),nRowIdx);
		pDM_PURCHASE->SetCellData(_T("SP_구독이력정보KEY"),strHISTORY_KEY,0);
		
		SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY,0);
		
		//프레임
		pDM_SPECIES->StartFrame();
		

		//구입종에 update 문을 구성한다.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("REC_KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PUR_SPECIES, _T("SE_PURCHASE_SPECIES_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPUR_SPECIES_KEY);

		
		// 구독이력정보에 insert 문을 구성한다.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL") , 0 , pDM_SPECIES);
		
		
		//구입정보에 update문을 구성한다.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("SP_구입관리KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL") , 0 , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPURCHASE_KEY);

	
		//해당 차수에 update문을 구성한다 (작업 상태를 주문으로 만든다.) 
		if(!strSEQ_NO.IsEmpty())
		{
			//++2008.10.17 UPDATE BY PWR {{++
			// 통합시스템 적용
			strQuery.Format( _T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1' ")
							 _T("WHERE SEQ_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;"), strSEQ_NO );
			// strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS = '1' WHERE SEQ_NO = %s;"),strSEQ_NO);
			//--2008.10.17 UPDATE BY PWR --}}
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}
		

		//기존 부서별 주문 정보를 DELETE 하고 새로운 부서별 주문 정보로 대체한다. 
		if(bDepartmentOrderUse)
		{
			strQuery.Format(_T("DELETE FROM SE_DEPARTMENT_ORDER_TBL WHERE PURCHASE_MANAGE_KEY = %s'"),strPURCHASE_KEY);
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);


			strQuery.Format(_T("UPDATE SE_DEPARTMENT_ORDER_TBL SET PURCHASE_MANAGE_KEY = %s ")
							_T("WHERE PURCHASE_SPECIES_KEY = %s;"),strPURCHASE_KEY,strPUR_SPECIES_KEY);

			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}


		ids = pDM_SPECIES->SendFrame();
		pDM_SPECIES->EndFrame();
		if(ids<0) return -5000;

	}
	else //새로 종을 입력해야 하는 상태 
	{

		pDM_SPECIES->InsertRow(-1);
		pDM_IDX->InsertRow(-1);
		pDM_PURCHASE->InsertRow(-1);
		pDM_SUB_HISTORY->InsertRow(-1);

		//종과 구입 정보에 구입종 정보를 copy	
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_SPECIES,0);
		CopyDMToDMByAlias(pDM_PUR_SPECIES,nRowIdx,pDM_PURCHASE,0);
	

		//KEY값들을 설정 해 준다.
		CString strPUR_SPECIES_KEY;
		strPUR_SPECIES_KEY = pDM_PUR_SPECIES->GetCellData(_T("REC_KEY"),nRowIdx);

		CString strSPECIES_KEY,strPURCHASE_KEY,strHISTORY_KEY,strACQ_EXP_KEY,strPUB_EXP_KEY,strHOLDING_KEY;
		pDM_SPECIES->MakeRecKey(strSPECIES_KEY);
		pDM_SPECIES->MakeRecKey(strPURCHASE_KEY);
		pDM_SPECIES->MakeRecKey(strHISTORY_KEY);
		pDM_SPECIES->MakeRecKey(strACQ_EXP_KEY);
		pDM_SPECIES->MakeRecKey(strPUB_EXP_KEY);
		pDM_SPECIES->MakeRecKey(strHOLDING_KEY);
	

		pDM_SPECIES->SetCellData(_T("SS_종KEY"),strSPECIES_KEY,0);
		pDM_SPECIES->SetCellData(_T("SS_홀딩KEY"),strHOLDING_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_구입관리KEY"),strPURCHASE_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_입수예측정보KEY"),strACQ_EXP_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_발행예측정보KEY"),strPUB_EXP_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_종KEY"),strSPECIES_KEY,0);
		pDM_PURCHASE->SetCellData(_T("SP_구독이력정보KEY"),strHISTORY_KEY,0);
		pDM_SUB_HISTORY->SetCellData(_T("구독이력KEY"),strHISTORY_KEY,0);
		pDM_PUR_SPECIES->SetCellData(_T("SS_종KEY"),strSPECIES_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("현재구독이력KEY"),strHISTORY_KEY,nRowIdx);
		pDM_PUR_SPECIES->SetCellData(_T("이전구독이력KEY"),strHISTORY_KEY,nRowIdx);

		
		//구입정보로 구독이력정보를 만든다 
		MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(pDM_PURCHASE, pDM_SUB_HISTORY ,0);


		//Default 값을 적용 시킨다.
		SetInsertDefaultValue_SPECIES(pDM_SPECIES,pMarc,0);
		SetInsertDefaultValue_PURCHASE(pDM_PURCHASE,0);
		SetInsertDefaultValue_IDX(pDM_IDX,0);
		SetInsertDefaultValue_SUB_HISTORY(pDM_SUB_HISTORY,0);
		

		//구입 적용을 시킨다 
		pDM_SPECIES->SetCellData(_T("SS_구입여부"),_T("Y"),0);
		pDM_SPECIES->SetCellData(_T("SS_기증여부"),_T("N"),0);
		
	
		//종과 마크를 가지고 색인을 구성한다
		MAKE_INDEX_by_SPECIES_MARC(pDM_SPECIES,pMarc,pDM_IDX,0);
		

		//주문일자 적용 
		pDM_PUR_SPECIES->SetCellData(_T("주문일자"),GetTodayDate(),nRowIdx);


		//프레임
		pDM_SPECIES->StartFrame();
 

		// 종에 Insert할 Insert문을 구성한다.
		RemoveAliasList.RemoveAll();
		CString strStreamMarc;
		m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
		pDM_SPECIES->SetCellData(_T("MARC"),strStreamMarc,0);
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SPECIES, _T("SE_SPECIES_TBL"), 0 , pDM_SPECIES);


		// 색인에 Insert할 Insert문을 구성한다.
		pDM_IDX->SetCellData(_T("ISS_종KEY"), strSPECIES_KEY, 0);
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_IDX, _T("IDX_SE_TBL"), 0, pDM_SPECIES);
		

		// 구입관리정보에 Insert할 Insert문을 구성한다.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PURCHASE, _T("SE_PURCHASE_MANAGE_TBL"), 0 , pDM_SPECIES);


		// 구독이력정보에 Insert할 Insert문을 구성한다.
		RemoveAliasList.RemoveAll();
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_SUB_HISTORY, _T("SE_SUBSCRIPTION_HISTORY_TBL"), 0 , pDM_SPECIES);


		// 소장정보에 rec_key만 insert한다 
 		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strHOLDING_KEY);
		pDM_SPECIES->MakeInsertFrame(_T("SE_HOLDING_TBL"));
		
		
		//구입종에 update 문을 구성한다.
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add(_T("REC_KEY"));
		pDM_SPECIES->InitDBFieldData();
		pDM_SPECIES->MakeQueryByDM(RemoveAliasList, pDM_PUR_SPECIES, _T("SE_PURCHASE_SPECIES_TBL") , nRowIdx , pDM_SPECIES, _T("REC_KEY") , _T("NUMERIC") , strPUR_SPECIES_KEY);
		
	
		//해당 차수에 update문을 구성한다 (작업 상태를 주문으로 만든다.) 
		if(!strSEQ_NO.IsEmpty())
		{
			//++2008.10.17 UPDATE BY PWR {{++
			// 통합시스템 적용
			strQuery.Format( _T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1' ")
							 _T("WHERE SEQ_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;"), strSEQ_NO );
			// strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS = '1' WHERE SEQ_NO = %s;"),strSEQ_NO);
			//--2008.10.17 UPDATE BY PWR --}}
			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);

		}

		//IDX_TMP_SE_TBL 설정
		SetIdxTmpSeTbl(pDM_SPECIES, strSPECIES_KEY, _T("I"));


		//부서별 주문 정보에 PURCHASE_MANAGE_KEY를 설정한다.
		if(bDepartmentOrderUse)
		{
			strQuery.Format(_T("UPDATE SE_DEPARTMENT_ORDER_TBL SET PURCHASE_MANAGE_KEY = %s ")
							_T("WHERE PURCHASE_SPECIES_KEY = %s;"),strPURCHASE_KEY,strPUR_SPECIES_KEY);

			pDM_SPECIES->InitDBFieldData();
			pDM_SPECIES->AddFrame(strQuery);


		}

		// 종관련 Send Frame
		ids = pDM_SPECIES->SendFrame();
		pDM_SPECIES->EndFrame();
		if (ids < 0) return -6000;	

	}

	return 0;
}

INT CSeApi::SPECIES_ORDER_CANCEL(CESL_DataMgr* pDM_PUR_SPECIES,
								 CESL_DataMgr* pDM_PURCHASE,
								 CESL_DataMgr* pDM_SUB_HISTORY,
								 INT nRowIdx)
{
	//주문 취소 함수
	//주문 취소에는 종이 삭제되야할 경우와 그렇지 않아야 할 경우가 존재한다.
	
	/*
	1)종이 삭제 될 경우: 
		-해당 종KEY로 책이 존재하지 않고, 
		-해당 종KEY의 기증관리 테이블이 존재하지 않고,
		-현재 구독이력 KEY와 이전 구독이력 KEY가 같을 때 
	
		->이 경우에는 종, 구입관리, 소장 정보, 구독이력을 삭제. 
		->색인은 삭제여부는 미정 

	2)종이 삭제 되지 않을 경우:
		-1)번 상황에 배리 될 경우.
		
		->이 경우에는 이전 구독이력 KEY 정보가 실제 할 경우 이전 구독이력 정보로 
		구입관리 정보를 roll back 시킨 후 현재 구독이력 정보를 지운다. 
	*/
	
	
	






	return 0;
}

INT CSeApi::CHECK_IN(CESL_DataMgr* pDM_IDX,
					 INT nIdx_IDX, 
					 CESL_DataMgr* pDM_WORK,
					 INT nIdx_WORK, 
					 CESL_DataMgr* pDM_ACQ,
					 INT nACQ_TYPE,
					 CESL_DataMgr* pDM_VOL,
					 CESL_DataMgr* pDM_ACQ_VOL,
					 CESL_DataMgr* pDM_BOOK,
					 CArray<CString,CString>& arrayVolExpKeyList,
					 CArray<CString,CString>& arrayOtherVolExpKeyList,
					 CESL_DataMgr* pDM_BOOK_INSERT,
					 CESL_DataMgr* pDM_APPENDIX,
					 CString strACQ_YEAR,
					 CString strRECEIPT_NO,
					 BOOL bIsDepartMngUse,
					 CESL_DataMgr* pDM_DEPART) 
{

	//체크인 함수 
	//입수예측 자료를 기반으로 
	//권(1 or 0 row), 수입구분별권(1 row) , 책(n row)을 입력한다 
	
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("체크인 관련 정보를 찾을 수 없습니다"));
		return -1;
	}

	//프레임에만 사용되는 DM을 FREE한다 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();
	pDM_BOOK_INSERT->FreeData();


	//먼저 권호 복본조사를 한다 
	BOOL bIsNewVol = TRUE;
	if(InspectDuplicateVol(pDM_WORK,nIdx_WORK,pDM_VOL)<0)
	{
		return -1;
	}
	
	if(pDM_VOL->GetRowCount()>0) bIsNewVol = FALSE;
	
	
	//권호 구분을 파악해 미발행, 혹은 합병호에 따른 처리를 한다.
	//1: 일반 2: 합병호 3: 미발행
	BOOL bIsNonPublish = FALSE;
	CString strVOLUME_CLASS = _T("");
	strVOLUME_CLASS = pDM_WORK->GetCellData(_T("SV_권호구분"),nIdx_WORK);
	if(_T("3") == strVOLUME_CLASS) bIsNonPublish = TRUE; //미발행

	
	INT nTotolBook = 0;
	CString strCHECKIN_STATUS = _T("");
	INT nACQ_BOOK_CNT = 0;
	INT nMISS_BOOK_CNT = 0;


	if(bIsNonPublish) //미발행 
	{
		strCHECKIN_STATUS = _T("1");
	}
	else 
	{
		
		//부록예측이 존재하는지를 파악한다
		CheckAppendix(pDM_WORK,nIdx_WORK,nACQ_TYPE,pDM_APPENDIX,pDM_BOOK_INSERT);


		//작업정보에서 책수를 조사해 상태 값을 파악한다.
		CString strACQ_BOOK_CNT = _T("");
		CString strMISS_BOOK_CNT = _T("");
		strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수책수"),nIdx_WORK);
		strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_결호책수"),nIdx_WORK);
		nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

		if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
		{
			AfxMessageBox(_T("ERROR: 입수 결호 책수가 모두 0 입니다!"));
			return -1;
		}

		
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //입수
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //결호
		else 
			strCHECKIN_STATUS = _T("3"); //미달	

		nTotolBook = nACQ_BOOK_CNT + nMISS_BOOK_CNT;

	}

	//마크 구조체를 만든다 
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);


	//색인과 작업정보에서  권호정보를 추출한다. (복본조사에 걸리지 않았을 경우) 
    if(bIsNewVol) 
	{
		MAKE_VOL_by_IDX_WORK(pDM_IDX,
							 nIdx_IDX,
							 pDM_WORK,
							 nIdx_WORK,
							 pDM_VOL,
							 &Marc,
							 strVOLUME_CLASS);
	}

	//색인과 수입구분정보, 작업정보에서  수입구분별 권호정보를 추출한다
	MAKE_ACQ_VOL_by_IDX_ACQ_WORK(pDM_ACQ,
								 nACQ_TYPE,
								 pDM_WORK,
								 nIdx_WORK,
								 pDM_ACQ_VOL,
								 strCHECKIN_STATUS);

	
	//책입력을 구성한다 
	if(!bIsNonPublish)
	{
		MAKE_BOOK_by_ACQ_WORK(pDM_ACQ,
							  nACQ_TYPE,
							  pDM_WORK,
							  nIdx_WORK,
							  pDM_BOOK,
							  nACQ_BOOK_CNT,
							  nMISS_BOOK_CNT,
							  strACQ_YEAR,
							  strRECEIPT_NO,
							  bIsDepartMngUse,
							  pDM_DEPART);
	}
	
	//각 정보의 연계된 foreign KEY를 설정한다 
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),0);
	CString strACQ_VOL_KEY = _T("");
	strACQ_VOL_KEY = pDM_ACQ_VOL->GetCellData(_T("SA_수입구분별권KEY"),0);
	
	//권KEY 설정
	pDM_ACQ_VOL->SetCellData(_T("SA_권KEY"),strVOL_KEY,0);
	
		
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();

	if(bIsNewVol)
	{
		//권 query
		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL"), 0, pDM_VOL);
	
		//색인 권호명 setting
		MakeEcoIndexVol(pDM_VOL,strVOL_KEY);
	}

	//수입구분별권 query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL"), 0, pDM_VOL);


	CString sCopyCode = _T("");
	//책 query
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->SetCellData(_T("SB_권KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_수입구분별권KEY"),strACQ_VOL_KEY,i);

		if( i > 0)
		{
			sCopyCode.Format(_T("%d"),i+1);
			pDM_BOOK->SetCellData(_T("SB_복본기호"),sCopyCode,i);
		}
		
		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("SE_BOOK_TBL"), i , pDM_VOL);
	}
	

	//부록 query
	INT nTotalAppendixBook = pDM_BOOK_INSERT->GetRowCount();
	if(nTotalAppendixBook>0)
	{
		
		INT nTotalAppendix = pDM_APPENDIX->GetRowCount();
		CString strUDF;

		for(i=0;i<nTotalAppendix;i++)
		{
			strUDF = _T("");
			strUDF = pDM_APPENDIX->GetCellData(_T("UDF_상태"),i);
			if(_T("I") != strUDF) continue;
			
			
			pDM_VOL->SetCellData(_T("권KEY"),strVOL_KEY,i);
			RemoveAliasList.RemoveAll();
			pDM_VOL->InitDBFieldData();
			pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_APPENDIX, _T("SE_APPENDIX_BOOK_TBL"), i , pDM_VOL);
			pDM_APPENDIX->SetCellData(_T("UDF_상태"),_T(""),i); //다시 빈칸으로 
		}
		
		//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선		
		//KOL.RED.019-002 JY 연속간행물 기능 안정화
		//ADD===============================================================
		CString strSBWorkingStatus = _T("");
		pDM_BOOK->GetCellData(_T("SB_자료상태"),0,strSBWorkingStatus);
		//==================================================================
		
		for(i=0;i<nTotalAppendixBook;i++)
		{
			pDM_BOOK_INSERT->SetCellData(_T("SB_권KEY"),strVOL_KEY,i);
			pDM_BOOK_INSERT->SetCellData(_T("SB_수입구분별권KEY"),strACQ_VOL_KEY,i);
			//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
			//KOL.RED.019-002 JY 연속간행물 기능 안정화
			//ADD===============================================================
			pDM_BOOK_INSERT->SetCellData(_T("SB_자료상태"),strSBWorkingStatus,i);
			//==================================================================
			
			RemoveAliasList.RemoveAll();
			pDM_VOL->InitDBFieldData();
			pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL"), i , pDM_VOL);
		}
	}

					

	//예측자료에 대한 update 처리
	UpdateExpectData(pDM_VOL,
		             pDM_ACQ,
					 nACQ_TYPE,
					 pDM_WORK,
					 nIdx_WORK,
					 arrayVolExpKeyList,
					 arrayOtherVolExpKeyList);
				


	CString strQuery;

	//최종권호일련번호 
	CString strLAST_VOL_SORT_NO,strSPECIES_KEY;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_최종권호일련번호"),nIdx_IDX);
	strSPECIES_KEY = pDM_IDX->GetCellData(_T("ISS_종KEY"),nIdx_IDX);
	strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LAST_VOL_SORT_NO = %s WHERE REC_KEY = %s;"),strLAST_VOL_SORT_NO,strSPECIES_KEY);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->AddFrame(strQuery);
	

	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);


	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -1000;	
	}
	

	
	//입수일 refresh
	if(_T("2") != strCHECKIN_STATUS && !bIsNonPublish) //결호가 아니고,  미발행이 아닌 자료에 대해서
	{
		CString strACQ_DATE;
		strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_입수일"),nIdx_WORK);
		
		//설정된 입수일이 없는 경우는 오늘 날짜를 넣어준다.
		if(strACQ_DATE.IsEmpty())
		{
			pDM_WORK->SetCellData(_T("UDF_입수일"),GetTodayDate(),nIdx_WORK);
		}

	}

	//부록수 refresh
	if(!bIsNonPublish)
	{
		CString strAppendixCnt;
		strAppendixCnt.Format(_T("%d"),nTotalAppendixBook);
		pDM_WORK->SetCellData(_T("UDF_부록수"),strAppendixCnt,nIdx_WORK);
	}

	//기사수 refresh
	if(!bIsNonPublish)
	{
		pDM_WORK->SetCellData(_T("UDF_기사수"),_T("0"),nIdx_WORK);
	}
	
	//성공시 필요한자료(권, 수입구분별권)을 pDM_WORK에 넣어준다
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);


	return 0;
}

INT CSeApi::ACQ_DELAY(CESL_DataMgr* pDM_IDX,
					 INT nIdx_IDX, 
					 CESL_DataMgr* pDM_WORK,
					 INT nIdx_WORK, 
					 CESL_DataMgr* pDM_ACQ,
					 INT nACQ_TYPE,
					 CESL_DataMgr* pDM_VOL,
					 CESL_DataMgr* pDM_ACQ_VOL,
					 CESL_DataMgr* pDM_BOOK,
					 CArray<CString,CString>& arrayVolExpKeyList,
					 CArray<CString,CString>& arrayOtherVolExpKeyList)
					 
					 
{

	//입수지연 함수 
	//입수예측 자료를 기반으로 
	//권(1 or 0 row), 수입구분별권(1 row) , 결호책(n row)을 입력한다 
	
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("관련 DM 을 찾을 수 없습니다"));
		return -1;
	}

	//프레임에만 사용되는 DM을 FREE한다 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();


	//작업정보에서 책수를 조사해 상태 값을 파악한다.
	CString strMISS_BOOK_CNT = _T("");
	INT nMISS_BOOK_CNT;
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_결호책수"),nIdx_WORK);
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	if(0 == nMISS_BOOK_CNT)
	{
		AfxMessageBox(_T("ERROR: 결호 책수가 0 입니다!"));
		return -1;
	}

	CString strCHECKIN_STATUS;
	strCHECKIN_STATUS = _T("2"); //결호
	INT nTotolBook = nMISS_BOOK_CNT;
	

	//마크 구조체를 만든다 
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);


	//색인과 작업정보에서  권호정보를 추출한다.
    MAKE_VOL_by_IDX_WORK(pDM_IDX,nIdx_IDX,pDM_WORK,nIdx_WORK,pDM_VOL,&Marc);


	//색인과 수입구분정보, 작업정보에서  수입구분별 권호정보를 추출한다
	MAKE_ACQ_VOL_by_IDX_ACQ_WORK(pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,pDM_ACQ_VOL,strCHECKIN_STATUS);

	
	//책입력을 구성한다 
	MAKE_BOOK_by_ACQ_WORK(pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,pDM_BOOK,0,nMISS_BOOK_CNT,_T(""),_T(""));

	
	//각 정보의 연계된 foreign KEY를 설정한다 
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),0);
	CString strACQ_VOL_KEY = _T("");
	strACQ_VOL_KEY = pDM_ACQ_VOL->GetCellData(_T("SA_수입구분별권KEY"),0);
	

	//권KEY 설정
	pDM_ACQ_VOL->SetCellData(_T("SA_권KEY"),strVOL_KEY,0);
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	//권 query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL"), 0, pDM_VOL);
	
	
	//색인 권호명 setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);


	//수입구분별권 query
	RemoveAliasList.RemoveAll();
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL"), 0, pDM_VOL);


	//책 query
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->SetCellData(_T("SB_권KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_수입구분별권KEY"),strACQ_VOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_책종류"),_T("L"),i);


		RemoveAliasList.RemoveAll();
		pDM_VOL->InitDBFieldData();
		pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("SE_BOOK_TBL"), i , pDM_VOL);
	}

	//예측자료에 대한 update 처리
	UpdateExpectData(pDM_VOL,pDM_ACQ,nACQ_TYPE,pDM_WORK,nIdx_WORK,arrayVolExpKeyList,arrayOtherVolExpKeyList);

	//최종권호일련번호 
	CString strLAST_VOL_SORT_NO,strSPECIES_KEY;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_최종권호일련번호"),nIdx_IDX);
	strSPECIES_KEY = pDM_IDX->GetCellData(_T("ISS_종KEY"),nIdx_IDX);
	CString strQuery;
	strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LAST_VOL_SORT_NO = %s WHERE REC_KEY = %s;"),strLAST_VOL_SORT_NO,strSPECIES_KEY);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->AddFrame(strQuery);

	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -1000;	
	}
	
	return 0;
}

INT CSeApi::VOL_UPDATE(CESL_DataMgr* pDM_IDX,
					   INT nIdx_IDX, 
					   CESL_DataMgr* pDM_WORK,
					   INT nIdx_WORK, 
					   CESL_DataMgr* pDM_ACQ,
					   INT nACQ_TYPE,
					   CESL_DataMgr* pDM_VOL,
					   CESL_DataMgr* pDM_ACQ_VOL,
					   CESL_DataMgr* pDM_BOOK,
					   CESL_DataMgr* pDM_BOOK_INSERT,
					   CString strACQ_YEAR,
					   CString strRECEIPT_NO,
					   BOOL bIsLightVersion,
					   BOOL bIsDepartMngUse,
					   CESL_DataMgr* pDM_DEPART) 
{
	
	//권 수정  함수 
	//작업 DM 자료를 기반으로 
	//권(1 row), 수입구분별권(1 row) update 
	//책(n row) insert or delete or update - 책삭제시에는 자료상태를 확인하여 지울수 없는 경우를 고려한다 
	if(!pDM_IDX || !pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK)
	{
		AfxMessageBox(_T("관련 DM 을 찾을 수 없습니다"));
		return -1;
	}

	//프레임에만 사용되는 DM을 FREE한다 
	pDM_VOL->FreeData();
	pDM_ACQ_VOL->FreeData();
	pDM_BOOK->FreeData();
	pDM_BOOK_INSERT->FreeData();


	//작업정보에서 권, 수입구분 정보를 가져온다.
	pDM_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK, nIdx_WORK, pDM_VOL,0);
	pDM_ACQ_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK, nIdx_WORK, pDM_ACQ_VOL,0);
	
	
	//SV_권KEY
	CString strVOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);

	//SA_수입구분별권KEY
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nIdx_WORK);

	
	//SV_권호명
	CString strVOL_TITLE;
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_권호명"),nIdx_WORK);
	
	
	//SV_부차적권호명 
	CString strOTHER_VOL_TITLE;
	strOTHER_VOL_TITLE = pDM_WORK->GetCellData(_T("SV_부차적권호명"),nIdx_WORK);
	

	//SV_색인권호명
	CString strIDX_VOL_TITLE;
	CArray<CString,CString> arrayList;
	arrayList.Add(strVOL_TITLE);
	arrayList.Add(strOTHER_VOL_TITLE);
	strIDX_VOL_TITLE = MakeKeyWord(arrayList);
	pDM_VOL->SetCellData(_T("SV_색인권호명"),strIDX_VOL_TITLE,0);


	//기존정보를 파악한다.
	//BOOK_TYPE  C:일반  M:결호  L:지연 
	CString strWhere;
	strWhere.Format(_T("ACQ_TYPE_VOL_KEY = %s AND (BOOK_TYPE = 'C' OR BOOK_TYPE = 'M' OR BOOK_TYPE = 'L')"),strACQ_VOL_KEY); 
	pDM_BOOK->RefreshDataManager(strWhere);
	INT nDB_ACQ_BOOK_CNT = 0;
	INT nDB_MISS_BOOK_CNT = 0;
	INT nDeletePossible = 0;
	CString strBOOK_TYPE;
	CString strWORKING_STATUS;
	CString strSHELF_LOC_CODE;
	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strSHELF_LOC_CODE = _T("");
		strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_배가위치_코드"),i);

		strBOOK_TYPE = _T("");
		strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
		CString strResultCode;
		if(_T("C") == strBOOK_TYPE) {
			
			strWORKING_STATUS = _T("");
			strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_자료상태"),i);
			//자료상태가 실시간, 제본, 등록 대상일 경우는 삭제가 가능하다  
			if(_T("SEL111O") == strWORKING_STATUS || _T("SEB111N") == strWORKING_STATUS || _T("SER111N") == strWORKING_STATUS) nDeletePossible++;
			nDB_ACQ_BOOK_CNT++;
			
			//부서별 지정자료 추가.. HS
			INT ids = GetNextPriorityShelfLocCode(strResultCode,
											  strSHELF_LOC_CODE,
											  pDM_DEPART,
											  pDM_BOOK);
			if(ids<0) continue;

			pDM_BOOK->SetCellData(_T("SB_배가위치_코드"),strResultCode,i);
		}	

		if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) nDB_MISS_BOOK_CNT++;	
	}
	INT nDB_TOTAL_BOOK_CNT = nDB_ACQ_BOOK_CNT + nDB_MISS_BOOK_CNT;

	
	//권호구분
	//1: 일반 2: 합병호 3: 미발행
	BOOL bIsNonPublish = FALSE;
	CString strVOLUME_CLASS = _T("");
	strVOLUME_CLASS = pDM_WORK->GetCellData(_T("SV_권호구분"),nIdx_WORK);
	if(_T("3") == strVOLUME_CLASS) bIsNonPublish = TRUE; //미발행
	

	if(nDB_TOTAL_BOOK_CNT != 0 && bIsNonPublish) //신규로 미발행 처리된 경우 
	{
		return VOL_UPDATE_MAKE_NON_PUBLISH(pDM_VOL,pDM_ACQ_VOL,pDM_WORK,nIdx_WORK);
	}



	//작업정보에서 책수를 조사해 상태 값을 파악한다.
	CString strACQ_BOOK_CNT = _T("");
	CString strMISS_BOOK_CNT = _T("");
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수책수"),nIdx_WORK);
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_결호책수"),nIdx_WORK);
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	CString strCHECKIN_STATUS;
	if(0 == nMISS_BOOK_CNT) 
		strCHECKIN_STATUS = _T("1"); //입수
	else if(0 == nACQ_BOOK_CNT) 
		strCHECKIN_STATUS = _T("2"); //결호
	else 
		strCHECKIN_STATUS = _T("3"); //미달	
	
	//입수상태 처리 
	pDM_ACQ_VOL->SetCellData(_T("SA_입수상태"),strCHECKIN_STATUS,0);
		

	//변화된 책수와 기존 책수를 비교한다.
	INT nWORK_TOTAL_BOOK_CNT = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	INT nTotalBookGap = nWORK_TOTAL_BOOK_CNT - nDB_TOTAL_BOOK_CNT;
	INT nAcqBookGap = nACQ_BOOK_CNT - nDB_ACQ_BOOK_CNT;
	INT nMissBookGap = nMISS_BOOK_CNT - nDB_MISS_BOOK_CNT;
	

	CString msg;
	if(nAcqBookGap<0) //입수책 감소 
	{
		if(nDeletePossible + nAcqBookGap <0) //감소되야할 입수책이 가능한 입수책보다 큰 경우 
		{
			
			if(bIsLightVersion)
			{
				msg.Format(_T("[ %s ] 정보 처리중 삭제 할 책 중에 이미 인계된 책이 있어 \n") 
							_T("작업을 진행 할 수 없습니다!\n")
							_T("이러한 책들은 책정보 화면에서 확인하신 후 삭제하실 수 있습니다"),strVOL_TITLE);   
				
			}
			else
			{
				msg.Format(_T("[ %s ] 정보 처리중 삭제 할 책 중에 [실시간 대상],[제본 대상], [등록 대상]\n") 
							_T("이외의 책이 있어 작업을 진행 할 수 없습니다!\n")
							_T("이러한 책들은 책정보 화면에서 확인하신 후 삭제하실 수 있습니다"),strVOL_TITLE);   
				
			}
			AfxMessageBox(msg);   
			return -1;
		}
	}
	

	if(nTotalBookGap>0) //총 책수가 늘은 경우 
	{
		for(i=0;i<nTotalBookGap;i++)
		{
			pDM_BOOK_INSERT->InsertRow(-1);
			pDM_BOOK_INSERT->SetCellData(_T("UDF_상태"),_T("I"),i); //insert
		}

		//책기본정보 구성 
		VOL_UPDATE_NEW_BOOK(pDM_ACQ,nACQ_TYPE,
			                pDM_WORK,nIdx_WORK,
							pDM_BOOK_INSERT,
							bIsDepartMngUse);
	}
		
	if(0 != nAcqBookGap || 0 != nMissBookGap) //책수 변화가 있을 경우 
	{
		//책의 상태를 정리해 pDM_BOOK_INSERT 에 copy한다. 
		VOL_UPDATE_SET_BOOK_STATUS(pDM_WORK,
								   nIdx_WORK,
								   pDM_BOOK,
								   pDM_BOOK_INSERT,
								   nACQ_TYPE,
								   strACQ_YEAR,
								   strRECEIPT_NO,
								   bIsDepartMngUse,
								   pDM_DEPART);
	}
	
	
	//마지막 작업 setting
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	

	//프레임 구성 
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	// 권정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SV_권KEY"));
	pDM_VOL->SetCellData(_T("SV_마지막작업"),strWorkLog,0);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strVOL_KEY);
	
	
	//색인 권호명 setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);

	
	// 수입구분별권정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SA_수입구분별권KEY"));
	pDM_ACQ_VOL->SetCellData(_T("SA_마지막작업"),strWorkLog,0);
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strACQ_VOL_KEY);
	
	
	
	if(0 != nAcqBookGap || 0 != nMissBookGap) //책수 변화가 있을 경우 
	{
		CString strUDF;
		CString strQuery;
		CString strBOOK_KEY;

		//책정보 쿼리를 구성한다 
		for(INT i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
		{
			strUDF = _T("");
			strUDF = pDM_BOOK_INSERT->GetCellData(_T("UDF_상태"),i);
			
			if(_T("I") == strUDF) //insert
			{

				RemoveAliasList.RemoveAll();
				pDM_VOL->InitDBFieldData();
				pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL"), i , pDM_VOL);

			}
			else if(_T("U") == strUDF) //update
			{

				strBOOK_KEY = _T("");
				RemoveAliasList.RemoveAll();
				RemoveAliasList.Add(_T("SB_책KEY"));
				strBOOK_KEY = pDM_BOOK_INSERT->GetCellData(_T("SB_책KEY"),i);
				pDM_VOL->InitDBFieldData();
				pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_BOOK_INSERT, _T("SE_BOOK_TBL") , i , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strBOOK_KEY);

			}
			else if(_T("D") == strUDF) //delete 
			{
				strBOOK_KEY = _T("");
				strBOOK_KEY = pDM_BOOK_INSERT->GetCellData(_T("SB_책KEY"),i);
				strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
				pDM_VOL->InitDBFieldData();
				pDM_VOL->AddFrame(strQuery);
			}
		}
	

		//ISS_종KEY
		CString strSPECIES_KEY;
		strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);

		//색인 UPDATE
		APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	}
	


	//지연자료 refresh
	//입수 상태가 미달 혹은 결호인 경우는 지연 자료를 결호자료로 만든다
	CString strUDF;
	strUDF = pDM_WORK->GetCellData(_T("UDF_입수상태"),nIdx_WORK);

	if(_T("지연") != strUDF)
	{
		if(_T("결호") == strUDF || _T("미달") == strUDF)
		{
			CString strQuery;
			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET BOOK_TYPE = 'M' WHERE BOOK_TYPE = 'L' AND ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
			pDM_VOL->InitDBFieldData();
			pDM_VOL->AddFrame(strQuery);
			
			//WORK DM에서 UDF_지연수를 0으로 만든다
			pDM_WORK->SetCellData(_T("UDF_지연수"),_T("0"),nIdx_WORK);
			
		}
		
	}
	

	//입수일 refresh
	if(_T("2") != strCHECKIN_STATUS && !bIsNonPublish) //결호가 아니고,  미발행이 아닌 자료에 대해서
	{
		CString strACQ_DATE;
		strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_입수일"),nIdx_WORK);
		
		//설정된 입수일이 없는 경우는 오늘 날짜를 넣어준다.
		if(strACQ_DATE.IsEmpty())
		{
			pDM_WORK->SetCellData(_T("UDF_입수일"),GetTodayDate(),nIdx_WORK);
		}

	}
	
	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -2000;	
	}
	

	//성공시 필요한자료(권, 수입구분별권)을 pDM_WORK에 넣어준다
	if(_T("지연") != strUDF)
	{
		if(_T("결호") == strUDF || _T("미달") == strUDF)
		{
			pDM_WORK->SetCellData(_T("UDF_지연수"),_T("0"),nIdx_WORK);
		}
		
	}
	
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);

	return 0;
}

INT CSeApi::VOL_UPDATE_MAKE_NON_PUBLISH(CESL_DataMgr* pDM_VOL,
										CESL_DataMgr* pDM_ACQ_VOL,
										CESL_DataMgr* pDM_WORK,
										INT nIdx_WORK)
{
	//신규로 DB 정보를 미발행 처리된 경우에 사용하는 함수
	//SV_권KEY
	CString strVOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);

	//SA_수입구분별권KEY
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nIdx_WORK);
	
	//입수상태는 입수 
	pDM_ACQ_VOL->SetCellData(_T("SA_입수상태"),_T("1"),0);
	
	//프레임 구성 
	CArray<CString,CString> RemoveAliasList;
	pDM_VOL->StartFrame();
	

	// 권정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SV_권KEY"));
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_VOL, _T("SE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strVOL_KEY);
	

	//색인 권호명 setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY);


	// 수입구분별권정보에 Update문을 구성한다.
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("SA_수입구분별권KEY"));
	pDM_VOL->InitDBFieldData();
	pDM_VOL->MakeQueryByDM(RemoveAliasList, pDM_ACQ_VOL, _T("SE_ACQ_TYPE_VOL_TBL") , 0 , pDM_VOL, _T("REC_KEY") , _T("NUMERIC") , strACQ_VOL_KEY);
	

	//책정보를 모두 삭제한다
	CString strQuery;
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	// Send Frame
	INT ids = pDM_VOL->SendFrame();
	pDM_VOL->EndFrame();
	if (ids < 0) {
		return -2000;	
	}
	

	//refresh
	pDM_WORK->SetCellData(_T("UDF_지연수"),_T("0"),nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_입수일"),_T(""),nIdx_WORK);
	CopyDMToDMByAlias(pDM_VOL,0,pDM_WORK,nIdx_WORK);
	CopyDMToDMByAlias(pDM_ACQ_VOL,0,pDM_WORK,nIdx_WORK);


	return 0;
}

INT CSeApi::VOL_DELETE_VERIFY(CESL_DataMgr* pDM_WORK,INT nIdx_WORK)
{
	//체크인에서 사용되는 권 삭제 FLOW 중에서 해당권호에 
	//삭제 할수 없는 책이 있는지에 대한 검사.

	if(!pDM_WORK) return -1;
	CString strVOL_KEY,strACQ_VOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nIdx_WORK);
	if(strVOL_KEY.IsEmpty() || strACQ_VOL_KEY.IsEmpty()) return -2;
	

	CString strQuery;
	CString strResult = _T("");

	//해당 수입구분별권KEY를 통해 삭제할 수 없는 책이 있는 지를 조사한다
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
					 _T("WHERE ACQ_TYPE_VOL_KEY=%s ")
					 _T("AND WORKING_STATUS NOT IN ('SEB111N','SEL111O','SER111N','SEA112N','SEA212N') ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strACQ_VOL_KEY );
	// strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s AND WORKING_STATUS NOT IN ('SEB111N','SEL111O','SER111N','SEA112N','SEA212N')"),strACQ_VOL_KEY);
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_WORK->GetOneValueQuery(strQuery,strResult);
	INT nIllegalCnt = _ttoi(strResult.GetBuffer(0));

	if(nIllegalCnt>0) 
	{
		return -1001; //삭제할 수 없는 책 존재
	}
	
	/* 기사 부록은 권호와 함께 삭제 시킨다
	CString strArticleCnt;
	INT nArticleCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strArticleCnt);
	nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
	if(nArticleCnt>0)
	{
		return -1002; //기사 정보 존재  
	}
				
	CString strAppendixCnt;
	INT nAppendixCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strAppendixCnt);
	nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
	if(nAppendixCnt>0)
	{
		return -1003; //부록 정보 존재 
	}
	*/

	CString strArticleCnt;
	INT nArticleCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strArticleCnt);
	nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
				
	CString strAppendixCnt;
	INT nAppendixCnt = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVOL_KEY);
	pDM_WORK->GetOneValueQuery(strQuery,strAppendixCnt);
	nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

	INT nReturnOption = 0;

	if(nArticleCnt>0) nReturnOption = 1;
	
	if(nAppendixCnt>0) {
		if(0 == nReturnOption) nReturnOption = 2;
		else nReturnOption = 3;
	}
	
	if(1 == nReturnOption) return -1002; //기사 존재
	if(2 == nReturnOption) return -1003; //부록 존재 
	if(3 == nReturnOption) return -1004; //기사 및 부록 존재 

	return 0;
}

INT CSeApi::VOL_DELETE(CESL_DataMgr* pDM_WORK,
					   INT nIdx_WORK, 
					   INT nACQ_TYPE)
{
	
	// 체크인 화면에서 사용되는 권 삭제 함수 
	// 다른 수입구분 정보를 파악해서 권까지 삭제할 것인지를 판단 
	// 해당권호의 책 상태를 파악해 지울수 없는 경우는 return
	// 권(1 row or 0 row delete)
	// 수입구분별권 (1row delete)
	// 책 , 부록 , 클레임 이력 (n row delete) 
	
	if(!pDM_WORK) return -1;
	CString strVOL_KEY,strACQ_VOL_KEY;
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nIdx_WORK);
	if(strVOL_KEY.IsEmpty() || strACQ_VOL_KEY.IsEmpty()) return -2;
	
	CString strQuery;
	CString strResult = _T("");


	//해당 권KEY로 구입일 경우 기증, 기증일 경우 구입의 수입구분별 권 정보를 확인한다.
	if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s AND ACQ_CODE = '2'"),strVOL_KEY);
	}
	else if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s AND ACQ_CODE = '1'"),strVOL_KEY);
	}

	strResult = _T("");
	pDM_WORK->GetOneValueQuery(strQuery,strResult);
	
	strResult.TrimLeft();
	strResult.TrimRight();

	pDM_WORK->StartFrame();

	if(strResult.IsEmpty()) { //권호 까지 삭제
		

		CString strToday = GetTodayDate();
		CString strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

		//클레임 이력 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN (SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);

		//책 (부록 포함)
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		

		//부록책 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					    _T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);


		//수입구분별권 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
	

		//삭제권호에 입력
		//===================================================
		//2009.07.06 UPDATE BY PJW : 쿼리에 UDF_MANAGE_CODE추가함
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		//===================================================


		//권 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
		pDM_WORK->AddFrame(strQuery);
		

		//기사,목차,초록관련 삭제
		GetArticleDeleteQuery(pDM_WORK,strVOL_KEY);


		//색인권호명
		MakeEcoIndexVol(pDM_WORK,strVOL_KEY,FALSE);

	} 
	else //수입구분별권 정보만 삭제 
	{
		
		//클레임 이력 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);
	
		//책 (부록이 아닌 책만 삭제)
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE ACQ_TYPE_VOL_KEY = %s AND BOOK_TYPE != 'A';"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);


		//수입구분별권 
		pDM_WORK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strACQ_VOL_KEY);
		pDM_WORK->AddFrame(strQuery);

	}
	
	
	//ISS_종KEY
	CString strSPECIES_KEY;
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);

	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_WORK, strSPECIES_KEY);

	INT ids = pDM_WORK->SendFrame();
	pDM_WORK->EndFrame();
	
	if(ids<0) return -1003; //쿼리 에러 

	return 0;
}


INT CSeApi::VOL_CAT_DELETE(CESL_DataMgr* pDM_OUT,
						   CString strSPECIES_KEY,
						   CString strVOL_KEY,
						   CString strVOL_TYPE,
						   CString strVolTitle,
						   CStringArray& arrayChildBookKeys,
						   BOOL bOnlyAddFrame,
						   BOOL bOnlyVerify)
{

	//정리에서 사용하는 권삭제 함수 
	//권 타입을 판단해 각각의 서브 함수를 이용한다
	
	if(!pDM_OUT || strSPECIES_KEY.IsEmpty() || strVOL_KEY.IsEmpty() || strVOL_TYPE.IsEmpty() || strVolTitle.IsEmpty())
	{
		return -1;
	}
	
	INT ids = -1;

	if(_T("B") == strVOL_TYPE && 0 == arrayChildBookKeys.GetSize())
	{
		AfxMessageBox(_T("제본권호의 하위 책키 정보가 입력되지 않았습니다!")); 
		return -1002; //verifying fail
	}


	if(!bOnlyAddFrame) pDM_OUT->StartFrame();

	if(_T("V") == strVOL_TYPE) //일반
	{


		ids = VOL_CAT_DELETE_GENERAL(pDM_OUT,
									 strSPECIES_KEY,
									 strVOL_KEY,
									 strVolTitle,
									 bOnlyVerify);
								
	}
	else if(_T("B") == strVOL_TYPE) //제본 
	{


		ids = VOL_CAT_DELETE_BINDING(pDM_OUT,
									 strSPECIES_KEY,
									 strVOL_KEY,
									 strVolTitle,
									 arrayChildBookKeys,
									 bOnlyVerify);

	}

	
	if(ids<0) {
		if(!bOnlyAddFrame) pDM_OUT->EndFrame();
		return -1002; //verifying fail
	}

	if(bOnlyAddFrame)
	{
		return 0;
	}

	ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	if(ids<0) return -1003; //Transaction error 

	return 0;

}


INT CSeApi::VOL_CAT_DELETE_GENERAL(CESL_DataMgr* pDM_VOL,
								   CString strSPECIES_KEY,
								   CString strVOL_KEY,
						           CString strVolTitle,
							       BOOL bOnlyVerify)
{
	//정리에서 사용할 권 삭제 함수의 서브 함수
	//일반 권호를 삭제할 때 사용
	//수입 구분에 관계 없이 해당 권호와 관련한 모든 정보를 삭제한다
	//해당 권호의 하위 책들의 상태를 먼저 점검한다

	if(!pDM_VOL) return -1;

		
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;

	///////////////////////////////////////////////////////////////
	//해당 권내의 책중에 제본이 되어 있는 책이 있는 지를 확인한다//
	///////////////////////////////////////////////////////////////

	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BINDED_BOOK_YN = 'Y'")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);

	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 제본되어 있는 책이 있습니다. \n 해당 제본책을 먼저 삭제해 주신 후\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	///////////////////////////////////////////////////////////
	//해당 권내의 책 대출, 수리제본, 일반제본 상태를 확인한다//
	///////////////////////////////////////////////////////////

	//대출
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 대출 혹은 예약중인 책이 있습니다!\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//수리제본
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 수리제본에 등록 또는 의뢰한 책이 있습니다!\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//일반 제본
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ")
					_T("('SEB112N','SEB113N') AND VOL_KEY = %s"),strVOL_KEY);
				
	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 연속 제본의 결정 혹은 발주 상태의 책이 있습니다!\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//////////////////////////////////////////////////////////
	//트랜잭션이 아닌 경우에는 삭제 가능에 대한 검증만 한다.//
	//////////////////////////////////////////////////////////
	if(bOnlyVerify) return 0;



	CString strWorkLog,strToday;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strToday = GetTodayDate();
	


	//////////////////////////////////////////////////////////////
	//책관련정보 삭제(책,대출관련,누락등록번호 처리,부록,제본책)//
	//////////////////////////////////////////////////////////////
	

	//대출 메일 정보
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

   
	//대출 정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	

	//배달 자료 정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

		
	//수리 제본 정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);


	//부록정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_APPENDIX_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);	
	pDM_VOL->AddFrame(strQuery);
	

	//제본책 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BINDING_BOOK_TBL WHERE BOOK_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락 등록 번호 처리 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	pDM_VOL->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strVOL_KEY);
// 	pDM_VOL->AddFrame(strQuery);

	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s));")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//=====================================================
	
					
	

	//책정보 삭제
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE SE_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);	
	pDM_VOL->AddFrame(strQuery);



	///////////////////////////////////////////////////
	//수입 구분별 권호 정보(클레임이력,수입구분별 권)//
	///////////////////////////////////////////////////

	//클레임 이력 정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY IN ")
					_T("(SELECT REC_KEY FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s);"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
		

	//수입구분별 권 정보 
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	///////////////////////////////////////////////////////////////////
	//권호 관련 삭제(권호,삭제권입력,기사,기사색인,목차초록,권호색인)//
	///////////////////////////////////////////////////////////////////
	

	//삭제권호에 입력
	//===================================================
	//2009.07.06 UPDATE BY PJW : 쿼리에 UDF_MANAGE_CODE추가함
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//===================================================
	

	//색인 권호명 setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY,FALSE);


	//기사,목차,초록관련 삭제
	GetArticleDeleteQuery(pDM_VOL,strVOL_KEY);
	

	//권호 정보
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	

	return 0;
}

INT CSeApi::VOL_CAT_DELETE_BINDING(CESL_DataMgr* pDM_VOL,
								   CString strSPECIES_KEY,
								   CString strVOL_KEY,
						           CString strVolTitle,
								   CStringArray& arrayChildBookKeys,
							       BOOL bOnlyVerify)
{

	//정리에서 사용할 권 삭제 함수의 서브 함수
	//제본 권호를 삭제할 때 사용
	//해당 권호의 하위 책들의 상태를 먼저 점검한다

	if(!pDM_VOL) return -1;

		
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;

	///////////////////////////////////////////////////////////
	//해당 권내의 책 대출, 수리제본, 일반제본 상태를 확인한다//
	///////////////////////////////////////////////////////////

	//대출
	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 대출 혹은 예약중인 책이 있습니다!\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
		
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	//수리제본
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY IN (SELECT REC_KEY FROM SE_BOOK_TBL WHERE VOL_KEY = %s)")
					,strVOL_KEY);

	pDM_VOL->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			msg.Format(_T("[ %s ] 권호 내에 수리제본에 등록 또는 의뢰한 책이 있습니다!\n")
					   _T("확인하시고 다시 삭제해 주십시요!"),strVolTitle);
			AfxMessageBox(msg);
			return -1002; //verifying fail
		}

	}

	
	
	CString strWorkLog,strToday;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	strToday = GetTodayDate();

	INT ids = -1;


	/////////////////////////////////////////////////////////
	//하위 책 관련정보 삭제(제본책과 물려있는 책 및 제본책)//
	/////////////////////////////////////////////////////////
	
	CString strBOOK_KEY;
	INT nSize = arrayChildBookKeys.GetSize();

	for(INT i=0;i<nSize;i++)
	{
		strBOOK_KEY = arrayChildBookKeys.GetAt(i);
		if(strBOOK_KEY.IsEmpty()) continue;

		ids = BOOK_CAT_DELETE_BINDING(pDM_VOL,
									  strBOOK_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strVolTitle,
									  _T("V"),
								      FALSE,
									  bOnlyVerify);
		
		if(ids<0) 
		{
			return -1002; //verifying error
		}


	}


	//////////////////////////////////////////////////////////
	//트랜잭션이 아닌 경우에는 삭제 가능에 대한 검증만 한다.//
	//////////////////////////////////////////////////////////
	if(bOnlyVerify) return 0;



	///////////////////////////////////////////////////////////////////
	//권호 관련 삭제(권호,삭제권입력,기사,기사색인,목차초록,권호색인)//
	///////////////////////////////////////////////////////////////////
	

	//삭제권호에 입력
	pDM_VOL->InitDBFieldData();
	//===================================================
	//2009.07.06 UPDATE BY PJW : 쿼리에 UDF_MANAGE_CODE추가함
	strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
					_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
					_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
					_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
					_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
					_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);
	//===================================================
	

	//색인 권호명 setting
	MakeEcoIndexVol(pDM_VOL,strVOL_KEY,FALSE);


	//권호 정보
	pDM_VOL->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
	pDM_VOL->AddFrame(strQuery);

	
	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_VOL, strSPECIES_KEY);
	

	return 0;
}

INT CSeApi::VOL_DELETE_EXP(CESL_DataMgr* pDM_WORK,
						   INT nIdx_WORK,
					       CESL_DataMgr* pDM_ACQ,
					       INT nACQ_TYPE,
					       CArray<CString,CString>& arrayVolExpKeyList,
					       CArray<CString,CString>& arrayOtherVolExpKeyList)
{
	//예측 권호에 대한 삭제함수로 예측정보에대한 update만한다. 
	
	if(!pDM_WORK || !pDM_ACQ) {
		AfxMessageBox(_T("예측 정보 삭제 관련 DM을 찾을 수 없습니다!"));
		return -1;
	}

	pDM_WORK->StartFrame();
	
	INT ids;
	ids = UpdateExpectData(pDM_WORK,
						   pDM_ACQ,
					       nACQ_TYPE,
					       pDM_WORK,
					       nIdx_WORK, 
					       arrayVolExpKeyList,
					       arrayOtherVolExpKeyList);

	if(ids<0) return -1001;
	
	ids = pDM_WORK->SendFrame();
	pDM_WORK->EndFrame();
	if(ids<0) return -1002;
	


	return 0;
}

INT CSeApi::VOL_UPDATE_NEW_BOOK(CESL_DataMgr* pDM_ACQ,
								INT nACQ_TYPE, 
								CESL_DataMgr* pDM_WORK,
								INT nIdx_WORK, 
								CESL_DataMgr* pDM_BOOK,
								BOOL bIsDepartMngUse) 
{
	
	//권호 수정 함수에서 사용하는 함수로 새로운 책이 입력되었을 경우 
	//기본 정보를 setting한다
	
	
	//종KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);
	

	//권KEY
	CString strVOL_KEY = _T("");	
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);


	//수입구분별권KEY
	CString strACQ_VOL_KEY = _T("");	
	strACQ_VOL_KEY = pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nIdx_WORK);	
	

	//등록구분
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_등록구분_코드"),nIdx_WORK);

	//이용제한구분
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_이용제한구분_코드"),nIdx_WORK);
	
	//관리구분
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_관리구분_코드"),nIdx_WORK);
	
	//배가위치코드
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_배가위치_코드"),nIdx_WORK);
	
	//분류기호구분
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_분류기호구분_코드"),nIdx_WORK);
	
	//수입구분 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//입력일 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();

	//최초작업, 마지막 작업
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	CString strREC_KEY;
	
	//인계 여부 설정
	CString strQuery;
	CString strResult;
	CString strTRANSFER_YN;
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
				_T("WHERE GROUP_1='L' AND GROUP_2='수서' AND CLASS_ALIAS='실시간대상인계사용여부' ")
				_T("AND MANAGE_CODE=UDF_MANAGE_CODE");
	// strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '수서' AND CLASS_ALIAS='실시간대상인계사용여부'");
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_BOOK->GetOneValueQuery(strQuery,strResult);
	
	if(_T("Y") == strResult)
		strTRANSFER_YN = _T("N");
	else
		strTRANSFER_YN = _T("Y");


	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		pDM_BOOK->MakeRecKey(strREC_KEY);		
		pDM_BOOK->SetCellData(_T("SB_책KEY"),strREC_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_종KEY"),strSPECIES_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_권KEY"),strVOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_수입구분별권KEY"),strACQ_VOL_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_수입구분"),strACQ_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_등록구분_코드"),strREG_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_이용제한구분_코드"),strUSE_LIMIT_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_관리구분"),strMANAGE_CODE,i);
		
		if(!bIsDepartMngUse)
			pDM_BOOK->SetCellData(_T("SB_배가위치_코드"),strSHELF_LOC_CODE,i);
		
		pDM_BOOK->SetCellData(_T("SB_분류기호구분_코드"),strCLASS_NO_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_입력일"),strINPUT_DATE,i);
		pDM_BOOK->SetCellData(_T("SB_최초작업"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_마지막작업"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_제본책여부"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_레이블출력구분"),_T("1"),i); //default
		pDM_BOOK->SetCellData(_T("SB_장서점검"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_인계여부"),strTRANSFER_YN,i); //manage value
	}

	return 0;
}

INT CSeApi::VOL_UPDATE_SET_BOOK_STATUS(CESL_DataMgr* pDM_WORK,
									   INT nIdx_WORK,
									   CESL_DataMgr* pDM_BOOK,
									   CESL_DataMgr* pDM_BOOK_INSERT,
									   INT nACQ_TYPE,
									   CString strACQ_YEAR,
									   CString strRECEIPT_NO,
									   BOOL bIsDepartMngUse,
					                   CESL_DataMgr* pDM_DEPART)
{
	
	//권수정시에 책의 상태값을 정리해주는 함수이다. 
	//BOOK_TYPE  C:일반  M:결호  L:지연
	INT nDB_ACQ_BOOK_CNT = 0;
	INT nDB_MISS_BOOK_CNT = 0;
	CString strBOOK_TYPE;
	
	//DB책수 
	INT nBookRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0;i<nBookRowCnt;i++)
	{
		strBOOK_TYPE = _T("");
		strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
		if(_T("C") == strBOOK_TYPE) nDB_ACQ_BOOK_CNT++;
		if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE ) nDB_MISS_BOOK_CNT++;	
		pDM_BOOK->SetCellData(_T("UDF_삭제우선순위"),_T("Z"),i); //일단 삭제 우선 순위를 Z 로 부여(삭제불가) 
	}
	
	//작업책수.
	CString strACQ_BOOK_CNT = _T("");
	CString strMISS_BOOK_CNT = _T("");
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	strACQ_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수책수"),nIdx_WORK);
	strMISS_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_결호책수"),nIdx_WORK);
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));	
	
	//책수 차이 
	INT nAcqBookGap = nACQ_BOOK_CNT - nDB_ACQ_BOOK_CNT;
	INT nMissBookGap = nMISS_BOOK_CNT - nDB_MISS_BOOK_CNT;
	

	//책들의 자료상태와 종정보, 수입정보의 값과 비교 추가해야할 우선 순위와 
	//삭제 우선 순위(DM_체크인_책.UDF_삭제우선순위)를 정해논다 
	CString strREAD_FIRST_YN;
	CString strWORKING_STATUS;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_열람우선여부"),nIdx_WORK);

	for(i=0;i<nBookRowCnt;i++)
	{
		strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_자료상태"),i);
			
		if(_T("SEL111O") == strWORKING_STATUS) //실시간 대상 
			pDM_BOOK->SetCellData(_T("UDF_삭제우선순위"),_T("A"),i); //우선 순위를 A 로 부여 
		else if(_T("SEB111N") == strWORKING_STATUS) //제본 대상 
			pDM_BOOK->SetCellData(_T("UDF_삭제우선순위"),_T("B"),i); //우선 순위를 B 로 부여 
		else if(_T("SER111N") == strWORKING_STATUS) //등록 대상 
			pDM_BOOK->SetCellData(_T("UDF_삭제우선순위"),_T("C"),i); //우선 순위를 C 로 부여 
	}

	INT nRow;
	
	
	if(nAcqBookGap<0) //////////////////////////////// 1) 입수책수가 줄은 경우 
	{
		if(nMissBookGap>0) //결호책이 늘었으면 
		{
			//결호 책이 늘은 만큼 까지 입수책을 결호책으로 만든다. 
			for(i = 0;i<nMissBookGap;i++) 
			{
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				MoveBookToMissBook(pDM_BOOK,nACQ_TYPE,nRow); //(입수,추가->결호) 
				pDM_BOOK->SetCellData(_T("UDF_상태"),_T("U"),nRow); // update 
				nAcqBookGap++; 
				nMissBookGap--;
				if(0 == nAcqBookGap || 0==nMissBookGap) break; 
			}

			if(nAcqBookGap<0) //결호책으로 옮기고도 줄은 책수가 남았으면 남은 만큼 삭제  
			{
				while(1)
				{
					nRow = GetDeleteHighestPriority(pDM_BOOK);
					pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),nRow); // delete 
					nAcqBookGap++;
					if(0 == nAcqBookGap) break;
				}

			}

			if(nMissBookGap>0) //늘은 결호책이 더 있으면 입력된 책을 결호책으로 만든다
			{
				for(i = 0;i<pDM_BOOK_INSERT->GetRowCount();i++) 
				{
					MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(입수,추가->결호)
				}
			}
				
			
		}
		else if(nMissBookGap<0) //결호책이 줄었으면 
		{
			while(1) //입수책삭제 
			{
				CString strDeleteKey;
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),nRow); // delete 
				nAcqBookGap++;
				if(0 == nAcqBookGap) break;
			}
			

			for(INT i=0;i<nBookRowCnt;i++) //결호책 삭제 
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
					pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),nRow); // delete 
					nMissBookGap++;
				}
				if(0 == nMissBookGap) break;
			}

		}
		else //결호책이 변하지 않았으면 
		{
			while(1) //입수책삭제 
			{
				nRow = GetDeleteHighestPriority(pDM_BOOK);
				pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),nRow); // delete 
				nAcqBookGap++;
				if(0 == nAcqBookGap) break;
			}
		}
	}
	else if(nAcqBookGap>0) //////////////////////////////// 2) 입수책수가 늘어난 경우 
	{
		
		if(nMissBookGap>0) //결호책이 늘었으면 
		{
			
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
				if(nAcqBookGap>0)
				{
					//입력된 책에서 입수 책수가 늘어난 만큼 입수책으로 만들어준다 
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(결호,추가 -> 입수)
					nAcqBookGap--;
				}
				else
				{
					//입력된 책에서 결호 책수가 늘어난 만큼 결호책으로 만들어준다 
					MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(입수,추가 -> 결호)
				}
			}
				
			
		}
		else if(nMissBookGap<0) //결호책이 줄었으면 
		{
			
			//결호책이 줄은 만큼 결호책을 입수책으로 옮긴다. 
			for(i=0;i<nBookRowCnt;i++)
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE)
				{
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(결호,추가 -> 입수)

					pDM_BOOK->SetCellData(_T("UDF_상태"),_T("U"),i); // update 
					nMissBookGap++;
					nAcqBookGap--;
					if(0 == nMissBookGap || 0 == nAcqBookGap) break;					
				}
			}
			
			//결호 책이 모두 줄여지지 않았으면 남은 만큼 삭제  
			if(nMissBookGap<0)
			{

				for(i=0;i<nBookRowCnt;i++)
				{
					strBOOK_TYPE = _T("");
					strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
					if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
						pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),i); // delete 
						nMissBookGap++;
					}
					if(0 == nMissBookGap) break;
				}
			}

			//추가로 입력 된 책은 모두 입수 책으로 만든다 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(결호,추가 -> 입수)
			}
			
		}
		else //결호책이 변하지 않았으면 
		{
			
			//추가로 입력 된 책은 모두 입수 책으로 만든다 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				
					MoveBookToAcqBook(pDM_WORK,
									  nIdx_WORK,
									  pDM_BOOK,
									  pDM_BOOK_INSERT,
									  pDM_BOOK_INSERT,
									  nACQ_TYPE,
									  strACQ_YEAR,
									  strRECEIPT_NO,
									  i,
									  bIsDepartMngUse,
									  pDM_DEPART); //(결호,추가 -> 입수)
			}
		}

	} 
	else if(0 == nAcqBookGap) //////////////////////////////// 3) 입수책수가 변함 없는 경우  
	{
	
		if(nMissBookGap>0) //결호책이 늘었으면 
		{
			//추가로 입력 된 책은 모두 결호 책으로 만든다 
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				MoveBookToMissBook(pDM_BOOK_INSERT,nACQ_TYPE,i); //(입수,추가->결호)
			}
			
		}
		else if(nMissBookGap<0) //결호책이 줄었으면 
		{
			
			//결호책을 줄어든 만큼  만큼 삭제  
			for(i=0;i<nBookRowCnt;i++)
			{
				strBOOK_TYPE = _T("");
				strBOOK_TYPE = pDM_BOOK->GetCellData(_T("SB_책종류"),i);
				if(_T("M") == strBOOK_TYPE || _T("L") == strBOOK_TYPE) {
					pDM_BOOK->SetCellData(_T("UDF_상태"),_T("D"),i); // delete 
					nMissBookGap++;
				}
				if(0 == nMissBookGap) break;
			}
		}

	}

	
	//책 DM 을 통합한다 
	INT nRowIdx;
	for(i=0;i<nBookRowCnt;i++)
	{
		pDM_BOOK_INSERT->InsertRow(-1);
		nRowIdx = pDM_BOOK_INSERT->GetRowCount()-1;
		CopyDMToDMByAlias(pDM_BOOK,i,pDM_BOOK_INSERT,nRowIdx);
	}


	return 0;
}


INT CSeApi::BOOK_DELETE_VERIFY(CESL_DataMgr* pDM_OUT,
							   CString strBOOK_KEY,
							   CString strVOL_KEY,
							   CString strBOOK_TYPE,
							   CString strBooktitle)
{



	//대출
	CString strQuery;
	CString msg;
	INT nCount = 0;
	CString strCount;


	strQuery.Format(_T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3','5') ")
					_T("AND BOOK_KEY = %s"),strBOOK_KEY);

	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ]  부록이 대출 혹은 예약중입니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ]  책이 대출 혹은 예약중입니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);
			}
		
			AfxMessageBox(msg);
			return -1;
		}

	}

	//수리제본
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_REPAIR_BINDING_TBL WHERE STATUS IN ('1','2') ")
					_T("AND BOOK_KEY = %s"),strBOOK_KEY);

	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ] 부록이 수리제본에 등록 또는 의뢰 중에 있습니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ] 책이 수리제본에 등록 또는 의뢰 중에 있습니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);
			}

			AfxMessageBox(msg);
			return -1;
		}

	}

	//일반 제본
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKING_STATUS IN ")
					_T("('SEB112N','SEB113N') AND REC_KEY = %s"),strBOOK_KEY);
				
	pDM_OUT->GetOneValueQuery(strQuery,strCount);
		
	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
		if(nCount>0)
		{
			if(_T("A") == strBOOK_TYPE)
			{
				msg.Format(_T("[ %s ] 부록이 연속 제본의 결정 혹은 발주 상태에 있습니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);
			}
			else
			{
				msg.Format(_T("[ %s ] 책이 연속 제본의 결정 혹은 발주 상태에 있습니다!\n")
						   _T("확인하시고 다시 삭제해 주십시요!"),strBooktitle);

			}

			AfxMessageBox(msg);
			return -1;
		}

	}


	return 0;
}


INT CSeApi::BOOK_CAT_DELETE(CESL_DataMgr* pDM_OUT,
							CString strBOOK_KEY,
							CString strACQ_TYPE_VOL_KEY,
							CString strVOL_KEY,
							CString strSPECIES_KEY,
							CString strBOOK_TYPE,
							CString strBooktitle,
							BOOL bOnlyAddFrame,
							BOOL bOnlyVerify)
{
	//정리에서 사용하는 책삭제 함수 
	//책 타입을 판단해 각각의 서브 함수를 이용한다
	
	if(!pDM_OUT || strBOOK_KEY.IsEmpty() || strVOL_KEY.IsEmpty() || strSPECIES_KEY.IsEmpty() || strBOOK_TYPE.IsEmpty())
	{
		return -1;
	}
	
	INT ids = -1;

	ids = BOOK_DELETE_VERIFY(pDM_OUT,
							 strBOOK_KEY,
							 strVOL_KEY,
							 strBOOK_TYPE,
							 strBooktitle);

	if(ids<0)
	{
		return -1002; //verifying fail
	}


	if(!bOnlyAddFrame) pDM_OUT->StartFrame();


	if(_T("C") == strBOOK_TYPE) //일반
	{


		ids = BOOK_CAT_DELETE_GENERAL(pDM_OUT,
									  strBOOK_KEY,
								      strACQ_TYPE_VOL_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strBooktitle,
									  _T("C"),
									  bOnlyVerify);
		

	}
	else if(_T("B") == strBOOK_TYPE) //제본 
	{


		ids = BOOK_CAT_DELETE_BINDING(pDM_OUT,
									  strBOOK_KEY,
								      strVOL_KEY,
								      strSPECIES_KEY,
									  strBooktitle,
									  _T("B"),
									  TRUE,
									  bOnlyVerify);
		

	}
	else if(_T("A") == strBOOK_TYPE) //부록
	{

		ids = BOOK_CAT_DELETE_APPENDIX(pDM_OUT,
									   strBOOK_KEY,
								       strSPECIES_KEY,
									   bOnlyVerify);
	}
	
	if(ids<0) 
	{
		if(!bOnlyAddFrame) 
		{
			pDM_OUT->EndFrame();
		}

		//===================================================
		//2009.05.11 UPDATE BY PJW : 부록을 삭제하지 않을 경우는 정상리턴한다.
// 		return -1002; //verifying fail
		if(ids == -1004) 
		{
			return ids; //verifying fail
		}
		else
		{
			return -1002; //verifying fail
		}
		//===================================================
	}

	if(bOnlyAddFrame)
	{
		return 0;
	}


	ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	if(ids<0) return -1003; //Transaction error 

	return 0;
}

INT CSeApi::BOOK_CAT_DELETE_BINDING(CESL_DataMgr* pDM_OUT,
								    CString strBOOK_KEY,
								    CString strVOL_KEY,
								    CString strSPECIES_KEY,
									CString strtitle,
									CString strDeleteOption,
									BOOL bVolDelete,
									BOOL bOnlyVerify)
{
	//권,책삭제 함수의 서브함수 
	//제본책과 그에 연결된 일반책을 삭제하는 쿼리를 트랜잭션 안에 넣는다
	

	//제본책 테이블에서 묶여있는 다른 책들의 KEY를 가지고 와서 하위 책먼저 삭제시킨다
	CString strQuery;
	CString strBookKeys;

	strQuery.Format(_T("SELECT BOOK_KEYS FROM SE_BINDING_BOOK_TBL ")
					_T("WHERE BOOK_KEY = %s"),strBOOK_KEY);

	
	pDM_OUT->GetOneValueQuery(strQuery,strBookKeys);

	CStringArray arrayBookKeys;
	ParsingKeyString(_T(","),strBookKeys, &arrayBookKeys);

	CString strBindedBookKey;
	CString strBindedBookAcqTypeVolKey;
	CString strBindedBookVolKey;
	CString strResult[2];
	INT ids = -1;

	INT nSize = arrayBookKeys.GetSize();
	CString msg;
	
	if(0 == nSize)
	{
		
		if(_T("V") == strDeleteOption) //권호 삭제 
		{
			msg.Format(_T("[ %s ] 권호에 속해 있는 제본 책 중에 \n")
						_T("묶여 있는 일반 책 정보를 찾을 수 없는 자료가 있어\n")
						_T("삭제할 수 없습니다!"),strtitle);
		}
		else if(_T("B") == strDeleteOption) //책 삭제
		{
			msg.Format(_T("[ %s ] 제본 책에 묶여 있는 일반 책정보를 찾을 수 없어\n")
						_T("삭제할 수 없습니다!"),strtitle);
		}

		AfxMessageBox(msg);
		return -1;

	}

	for(INT i=0;i<nSize;i++)
	{
		
		strBindedBookKey = arrayBookKeys.GetAt(i);
		strBindedBookKey.TrimLeft();
		strBindedBookKey.TrimRight();
		if(strBindedBookKey.IsEmpty()) continue;

		strQuery.Format(_T("SELECT ACQ_TYPE_VOL_KEY, VOL_KEY FROM SE_BOOK_TBL ")
						_T("WHERE REC_KEY = %s"),strBindedBookKey);

		pDM_OUT->GetValuesQuery(strQuery,strResult,2);


		strBindedBookAcqTypeVolKey = strResult[0];
		strBindedBookVolKey = strResult[1];
		
		if(!strBindedBookVolKey.IsEmpty())
		{

			ids = BOOK_CAT_DELETE_GENERAL(pDM_OUT,
										  strBindedBookKey,
										  strBindedBookAcqTypeVolKey,
										  strBindedBookVolKey,
										  strSPECIES_KEY,
										  strtitle,
										  strDeleteOption,
										  bOnlyVerify);

			if(ids<0) return -1;
		}
	}
	
	//검증만 하는 경우는 return
	if(bOnlyVerify) return 0;
	

	//다른 제본책이 없으면 자신의 제본 권호 까지 삭제한다
	if(bVolDelete)
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						_T("WHERE VOL_KEY = %s"),strVOL_KEY);

		CString strAnotherBookCnt;
		INT nAnotherBookCnt;

		pDM_OUT->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBookCnt = _ttoi(strAnotherBookCnt.GetBuffer(0));

		if(0 == nAnotherBookCnt)
		{
			//제본 권호 정보
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			pDM_OUT->AddFrame(strQuery);
		

			//색인 권호명 setting
			MakeEcoIndexVol(pDM_OUT,strVOL_KEY,FALSE);
		}
	}


	//////////////////////////////////////////////////////////////
	//책관련정보 삭제(책,대출관련,누락등록번호 처리,부록,제본책)//
	//////////////////////////////////////////////////////////////
	
	
	//대출 메일 정보
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//대출 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//배달 자료 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//수리 제본 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	//제본 책
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BINDING_BOOK_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락 등록 번호 처리 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString strWorkLog;
// 	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
// 
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================

	//책
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);

	return 0;
}



INT CSeApi::BOOK_CAT_DELETE_GENERAL(CESL_DataMgr* pDM_OUT,
								    CString strBOOK_KEY,
								    CString strACQ_TYPE_VOL_KEY,
								    CString strVOL_KEY,
								    CString strSPECIES_KEY,
								    CString strtitle,
									CString strDeleteOption,
									BOOL bOnlyVerify)
{

	//권,책삭제 함수의 서브함수 
	//일반책만을 삭제하는 쿼리를 트랜잭션 안에 넣는다
	//정리에서 입력하여 수입구분별권KEY(strACQ_TYPE_VOL_KEY)가 없는 경우는 권호만 삭제한다. 
	INT nACQ_BOOK_CNT = 0;
	INT nMISS_BOOK_CNT = 0;

	CString strWorkLog,strToday;
	strToday = GetTodayDate();
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	CString strQuery;

	if(!strACQ_TYPE_VOL_KEY.IsEmpty())
	{
		CString strResult[3];
		strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL ")
						_T("WHERE REC_KEY = %s"),strACQ_TYPE_VOL_KEY);

		pDM_OUT->GetValuesQuery(strQuery,strResult,3);
		
		nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
		nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
		
		nACQ_BOOK_CNT--;
	}


	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //수입구분별 권 정보를 삭제 해야하는 상황 
	{
		
		if(!strACQ_TYPE_VOL_KEY.IsEmpty())
		{

			//클레임 이력 정보
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strACQ_TYPE_VOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
		

			//수입구분별 권호 정보
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strACQ_TYPE_VOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);

		}

		//권 삭제까지 해야 하는 지를 판단한다(해당 권에 물려 있는 다른 책이 있는지)
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						_T("WHERE VOL_KEY = %s AND REC_KEY != %s AND BOOK_TYPE != 'A'")
						,strVOL_KEY,strBOOK_KEY);


		pDM_OUT->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));

		CString strDeleteAppendixYN;
		strDeleteAppendixYN = _T("N");
		if(0 == nAnotherBook) //권 까지 삭제 
		{
			
			//부록이 있는지 판단해 에러처리 
			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'")
							,strVOL_KEY);

			pDM_OUT->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

			if(nAppendixCnt>0) //권을 삭제하는 상황인데 부록이 권에 붙어 있는 경우 
			{
				
				CString msg;

				
				if(_T("C") == strDeleteOption) //일반 책삭제
				{
					//===================================================
					//2009.05.11 UPDATE BY PJW : 부록이 있는자료는 물어본후 처리하도록 한다.
//					msg.Format(_T("[ %s ] 책의 해당 권호에 부록자료가 있어 삭제 할 수 없습니다!\n")
//								_T("부록자료를 먼저 삭제하여 주십시요."),strtitle);
//					AfxMessageBox(msg);
//					return -1; 
					msg.Format(_T("[ %s ] 책의 해당 권호의 부록도 함께 삭제 됩니다.\n")
								_T("삭제 하시겠습니까?"),strtitle);
					if ( IDYES == AfxMessageBox(msg, MB_YESNO) )
					{
						strDeleteAppendixYN = _T("Y");
					}
					else
					{
						return -1004; 
					}
					//===================================================
					
				}
				else if(_T("V") == strDeleteOption) //제본 권 삭제를 통한 삭제
				{
					msg.Format(_T("[ %s ] 제본권호에 속한 부록자료가 있습니다.\n")
								_T("체크인에서 부록자료를 먼저 삭제하신 후 이 자료를 삭제하십시요"),strtitle);
					AfxMessageBox(msg);
					return -1; 
				}
				else if(_T("B") == strDeleteOption) //제본 책 삭제를 통한 삭제
				{
					msg.Format(_T("[ %s ] 제본책에 속한 부록자료가 있습니다.\n")
								_T("체크인에서 부록자료를 먼저 삭제하신 후 이 자료를 삭제하십시요"),strtitle);
					AfxMessageBox(msg);
					return -1; 
				}

				
			}

			///////////////////////////////////////////////////////////////////
			//권호 관련 삭제(권호,삭제권입력,기사,기사색인,목차초록,권호색인)//
			///////////////////////////////////////////////////////////////////
			

			//삭제권호에 입력
			//===================================================
			//2009.07.06 UPDATE BY PJW : 쿼리에 UDF_MANAGE_CODE추가함
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("INSERT INTO SE_DELETE_VOL_TBL ")
							_T("(SELECT REC_KEY,SPECIES_KEY,VOL_TYPE,TITLE,FORM_CODE,MEDIA_CODE,VOL_SORT_NO,")
							_T("VOL_TITLE,OTHER_VOL_TITLE,ALTERNATIVE_VOL_TITLE,IDX_VOL_TITLE,PUBLISH_DATE,")
							_T("CURRENCY_CODE,PRICE,PRICE_CHARACTER,PAGE,PHYSICAL_PROPERTY,BOOK_SIZE,ACCOMP_MAT,")
							_T("REMARK,VOLUME_CLASS,CONTENTS_YN,CONTENTS_KEY,'%s',FIRST_WORK,'%s',UDF_MANAGE_CODE ")
							_T("FROM SE_VOL_TBL WHERE REC_KEY = %s);"),strToday,strWorkLog,strVOL_KEY);
			if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
			//===================================================
			

			//색인 권호명 setting
			if(!bOnlyVerify) MakeEcoIndexVol(pDM_OUT,strVOL_KEY,FALSE);


			//기사,목차,초록관련 삭제
			if(!bOnlyVerify) GetArticleDeleteQuery(pDM_OUT,strVOL_KEY);
			

			//권호 정보
			pDM_OUT->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVOL_KEY);
			if(!bOnlyVerify) 
			{
				pDM_OUT->AddFrame(strQuery);
				//===================================================
				//2009.05.11 ADD BY PJW : 권을 삭제할경우 부록도 삭제한다.
				if ( strDeleteAppendixYN == _T("Y") )
				{
					strQuery.Format(_T("DELETE FROM SE_APPENDIX_BOOK_TBL WHERE VOL_KEY = %s;"),strVOL_KEY);						
					pDM_OUT->AddFrame(strQuery);
				}
				//===================================================
			}
			

		}	

	}
	else
	{
		
		//책수 및 입수 상태 update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //입수
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //결호
		else 
			strCHECKIN_STATUS = _T("3"); //미달
		
		
		pDM_OUT->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s',LAST_WORK = '%s' ")
						_T("WHERE REC_KEY = %s;"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strWorkLog,strACQ_TYPE_VOL_KEY);
		if(!bOnlyVerify) pDM_OUT->AddFrame(strQuery);
		
	}

	
	//검증만 하는 경우는 return
	if(bOnlyVerify) return 0;


	//////////////////////////////////////////////////////////////
	//책관련정보 삭제(책,대출관련,누락등록번호 처리,부록,제본책)//
	//////////////////////////////////////////////////////////////
	

	//대출 메일 정보
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//대출 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//배달 자료 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//수리 제본 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

	
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락 등록 번호 처리 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//===================================================== 
	


	//책 DELETE
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);


	return 0;
}


INT CSeApi::BOOK_CAT_DELETE_APPENDIX(CESL_DataMgr* pDM_OUT,
									 CString strBOOK_KEY,
									 CString strSPECIES_KEY,
									 BOOL bOnlyVerify)
{

	//권,책삭제 함수의 서브함수 
	//부록책만을 삭제하는 쿼리를 트랜잭션 안에 넣는다

	
	//검증만 하는 경우는 return
	if(bOnlyVerify) return 0;


	CString strQuery;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	//대출 메일 정보
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_LOAN_MAIL_SEND_TBL WHERE LOAN_KEY IN ")
					_T("(SELECT REC_KEY FROM LH_STORAGE_V01 WHERE BOOK_KEY = %s);")
					,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

   
	//대출 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE LS_WORK_T01 WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	

	//배달 자료 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_DELIVERY_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);

		
	//수리 제본 정보 
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE CO_REPAIR_BINDING_TBL WHERE BOOK_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//부록책 DELETE
	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);


	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_OUT, strSPECIES_KEY);


	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락 등록 번호 처리 
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	pDM_OUT->InitDBFieldData();
// 	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
// 					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,2),SUBSTR(REG_NO,3,10),'%s','%s',UDF_MANAGE_CODE ")
// 					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
// 					_T("AND REC_KEY = %s);")
// 					,DEFAULT_SEQ_NAME,strWorkLog,strWorkLog,strBOOK_KEY);
// 	pDM_OUT->AddFrame(strQuery);

	pDM_OUT->InitDBFieldData();
	strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(SELECT %s.NEXTVAL,'SE',NULL,NULL,SUBSTR(REG_NO,0,%d),SUBSTR(REG_NO,%d,%d),'%s','%s',UDF_MANAGE_CODE ")
					_T("FROM SE_BOOK_TBL WHERE (ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57) ")
					_T("AND REC_KEY = %s);")
					,DEFAULT_SEQ_NAME,nRegCodeLength,nRegCodeLength+1, 12-nRegCodeLength,strWorkLog,strWorkLog,strBOOK_KEY);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================  
	


	return 0;
}

INT CSeApi::BOOK_DELETE(CESL_DataMgr* pDM_BOOK,
						CESL_DataMgr* pDM_VOL,
					    CESL_DataMgr* pDM_VOL_DELETE,
						INT nIdx_BOOK,
						BOOL bIsVerified)
{

	
	//책삭제 함수
	//수입구분별 권 정보, 권 정보를 파악해 변경사항을 적용해 준다 
	
	if(!pDM_BOOK || !pDM_VOL || !pDM_VOL_DELETE) return -1;
	


	//프레임에서만 사용 되는 데이터 객체
	pDM_VOL->FreeData();
	pDM_VOL_DELETE->FreeData();
	
	if(!bIsVerified)
	{
		if(VerifyBookDeleteProc(pDM_BOOK,nIdx_BOOK)<0) return -1004; //verify 문제 
	}

	CString strBookKey,strVolKey,strAcqTypeVolKey,strAcqCode,strRegNo;
	CString strAccessionKey,strAccessionNo,strAccessionYear;
	
	strBookKey = pDM_BOOK->GetCellData(_T("SB_책KEY"),nIdx_BOOK);
	strVolKey = pDM_BOOK->GetCellData(_T("SB_권KEY"),nIdx_BOOK);
	strAcqTypeVolKey = pDM_BOOK->GetCellData(_T("SB_수입구분별권KEY"),nIdx_BOOK);
	strAcqCode = pDM_BOOK->GetCellData(_T("SB_수입구분"),nIdx_BOOK);
	strRegNo = pDM_BOOK->GetCellData(_T("SB_등록번호"),nIdx_BOOK);
	strAccessionKey = pDM_BOOK->GetCellData(_T("SB_등록원부KEY"),nIdx_BOOK);
	
	CString strQuery;

	if(!strAccessionKey.IsEmpty())
	{
		CString strAccessData[2];
		
		strQuery.Format(_T("SELECT ACCESSION_REC_NO,ACCESSION_REC_MAKE_YEAR FROM CO_ACCESSION_REC_TBL WHERE REC_KEY = %s"),strAccessionKey);
		pDM_BOOK->GetValuesQuery(strQuery,strAccessData,2);
		strAccessionNo = strAccessData[0];
		strAccessionYear = strAccessData[1];
	}

	CString strResult[3];
	
	strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
	pDM_BOOK->GetValuesQuery(strQuery,strResult,3);
	
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
	
	nACQ_BOOK_CNT--;

	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	pDM_BOOK->StartFrame();

	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //수입구분별 권 정보를 삭제 해야하는 상황 
	{

		//클레임 이력 정보 delete 
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s;"),strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
	

		//수입구분별 권호 정보
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s;"),strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
	

		//권 삭제까지 해야 하는 지를 판단한다
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		//++2008.10.17 UPDATE BY PWR {{++
		// 통합시스템 적용
		strQuery.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
						 _T("WHERE VOL_KEY=%s AND ACQ_CODE!='%s' AND BOOK_TYPE NOT IN ('B','A') ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strVolKey, strAcqCode );
		// strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND ACQ_CODE != '%s' AND BOOK_TYPE NOT IN ('B','A')"),strVolKey,strAcqCode);
		//--2008.10.17 UPDATE BY PWR --}}
		pDM_BOOK->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));


		if(0 == nAnotherBook) //권 까지 삭제 
		{
			
			//삭제 권호로 이동
			//권정보를 가져온다.
			CString strWhere;
			strWhere.Format(_T("REC_KEY = %s"),strVolKey); 
			pDM_VOL->RefreshDataManager(strWhere);
			
			INT nRowCnt = pDM_VOL->GetRowCount();
	
			if(nRowCnt>0)
			{
				CString strWorkLog;
				INT nRowDeleteVol;
				CArray<CString,CString> RemoveAliasList;
				strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
				
				pDM_VOL_DELETE->InsertRow(-1);	
				nRowDeleteVol = pDM_VOL_DELETE->GetRowCount()-1;
				CopyDMToDMByAlias(pDM_VOL,0,pDM_VOL_DELETE,nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_삭제일자"),GetTodayDate(),nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_최초작업"),strWorkLog,nRowDeleteVol);
				pDM_VOL_DELETE->SetCellData(_T("SV_마지막작업"),strWorkLog,nRowDeleteVol);
					
				//삭제권 insert
				RemoveAliasList.RemoveAll();
				pDM_BOOK->InitDBFieldData();
				pDM_BOOK->MakeQueryByDM(RemoveAliasList, pDM_VOL_DELETE, _T("SE_DELETE_VOL_TBL"), nRowDeleteVol , pDM_BOOK);
				
				//권 delete
				pDM_BOOK->InitDBFieldData();
				strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s;"),strVolKey);
				pDM_BOOK->AddFrame(strQuery);
				
				//색인 권호명 setting
				MakeEcoIndexVol(pDM_BOOK,strVolKey,FALSE);

			}

		}	


	}
	else
	{
		
		//책수 및 입수 상태 update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //입수
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //결호
		else 
			strCHECKIN_STATUS = _T("3"); //미달
		
		
		pDM_BOOK->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s',LAST_WORK = '%s' ")
						_T("WHERE REC_KEY = %s;"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strWorkLog,strAcqTypeVolKey);
		pDM_BOOK->AddFrame(strQuery);
		
	}
	
	
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락등록번호 처리
	if(!strRegNo.IsEmpty())
	{
		CString strRegOnlyCode;
		CString strRegOnlyNo;
		INT nRegOnlyNo;


		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strRegOnlyCode = strRegNo.Left(2);
// 		nRegOnlyNo = _ttoi(strRegNo.Mid(2).GetBuffer(0));
// 		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
		
		strRegOnlyCode = strRegNo.Left(nRegCodeLength);
		nRegOnlyNo = _ttoi(strRegNo.Mid(nRegCodeLength).GetBuffer(0));
		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
		//=====================================================
		
			
		//등록 번호가 가등록 번호인지를 체크한다.(등록구분이 숫자이면 가등록 번호)
		if(!IsDigitString(strRegOnlyCode))
		{

			pDM_BOOK->InitDBFieldData();
			//++2008.10.17 UPDATE BY PWR {{++
			// 통합시스템 적용
			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK, MANAGE_CODE) ")
							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s','%s',UDF_MANAGE_CODE);"),
							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
//			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
//							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK) ")
//							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s','%s');"),
//							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
			//--2008.10.17 UPDATE BY PWR --}}
			
			pDM_BOOK->AddFrame(strQuery);
		}
	}


	pDM_BOOK->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s;"),strBookKey);
	pDM_BOOK->AddFrame(strQuery);
	
	
	//종KEY
	CString strSPECIES_KEY;
	strSPECIES_KEY = pDM_BOOK->GetCellData(_T("SB_종KEY"),nIdx_BOOK);

	//색인 UPDATE
	APPLY_IndexItem_Book(pDM_BOOK, strSPECIES_KEY);
	

	INT ids = pDM_BOOK->SendFrame();
	pDM_BOOK->EndFrame();	
	
	if(ids<0) return -1005; //쿼리 에러
	
	return 0;
}


INT CSeApi::VerifyBookDeleteProc(CESL_DataMgr* pDM_BOOK,INT nIdx_BOOK)
{
	
	//모든 책이 삭제 되었을 경우 -- 기사, 부록 유무 판별
	CString strAcqVolKey;
	CString strVolKey;
	CString msg;
	CString strQuery;
	strAcqVolKey = pDM_BOOK->GetCellData(_T("SB_수입구분별권KEY"),nIdx_BOOK);
	strVolKey = pDM_BOOK->GetCellData(_T("SB_권KEY"),nIdx_BOOK);


    //입수책과 결호책을 가지고 온다
	INT nMissBookCnt = 0;
  	INT nAcqBookCnt = 0;
	CString strResult[2];

	strQuery.Format(_T("SELECT ACQ_BOOK_CNT, MISS_BOOK_CNT FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqVolKey);
	pDM_BOOK->GetValuesQuery(strQuery,strResult,2);

	if(!strResult[0].IsEmpty()) nAcqBookCnt = _ttoi(strResult[0].GetBuffer(0));
	if(!strResult[1].IsEmpty()) nMissBookCnt = _ttoi(strResult[1].GetBuffer(0));		
		
	if(1 == nAcqBookCnt && 0 == nMissBookCnt) //해당 수입구분별 권호 삭제 
	{
	
		//다른 수입 구분이 있는지를 조사
		CString strResult;
		INT nAcqVolCnt = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s"),strVolKey);
		pDM_BOOK->GetOneValueQuery(strQuery,strResult);			
		if(!strResult.IsEmpty()) nAcqVolCnt = _ttoi(strResult.GetBuffer(0));
			
		if(nAcqVolCnt>1) //다른 수입구분이 있는 경우 (해당 권호 삭제 메시지)
		{
			msg.Format(_T("해당책 삭제시 남은 책이 없게 되어\n") 
						_T("권호 정보가 사라집니다\n")
						_T("그래도 삭제 하시겠습니까?"));
				
			if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;

		}
		else  //다른 수입구분이 없는 경우 (해당 권호의 기사와 부록을 확인 후 삭제 메시지)
		{
				
			CString strArticleCnt;
			INT nArticleCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM_BOOK->GetOneValueQuery(strQuery,strArticleCnt);
			nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
			if(nArticleCnt>0)
			{
				AfxMessageBox(_T("해당 권호에  기사정보가 있어 해당 책을 삭제 할수 없습니다!\n")
							   _T("먼저 기사정보를 삭제하신 후 삭제 하실 수 있습니다."));					  
				return -1002;
			}
				
			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVolKey);
			pDM_BOOK->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
			if(nAppendixCnt>0)
			{
				AfxMessageBox(_T("해당 권호에 부록 정보가 있어 해당 책을 삭제 할수 없습니다!\n")
									_T("먼저 부록 정보를 삭제하신 후 삭제 하실 수 있습니다."));	
				return -1003;
			}

			msg.Format(_T("해당책 삭제시 남은 책이 없게 되어\n") 
			_T("권호 정보가 사라집니다\n")
			_T("그래도 삭제 하시겠습니까?"));
				
			if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;
		}

	}


	return 0;
}

				
/*
INT CSeApi::BOOK_DELETE(CESL_DataMgr* pDM, 
						CString strBookKey, 
						CString strVolKey, 
						CString strAcqTypeVolKey,
						CString strAcqCode,
						CString strRegNo,
						CString strAccessionNo,
						CString strAccessionYear)
{
	//책삭제 함수 - 수서 이후 단계에서 사용한다. 
	//수입구분별 권 정보, 권 정보를 파악해 변경사항을 적용해 준다 
	if(!pDM || strBookKey.IsEmpty() || strVolKey.IsEmpty() || strAcqTypeVolKey.IsEmpty() || strAcqCode.IsEmpty()) return -1;

	CString strQuery;
	CString strResult[3];
	
	strQuery.Format(_T("SELECT ACQ_BOOK_CNT,MISS_BOOK_CNT,CHECKIN_STATUS FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
	pDM->GetValuesQuery(strQuery,strResult,3);
	
	INT nACQ_BOOK_CNT;
	INT nMISS_BOOK_CNT;
	nACQ_BOOK_CNT = _ttoi(strResult[0].GetBuffer(0));
	nMISS_BOOK_CNT = _ttoi(strResult[1].GetBuffer(0));
	
	nACQ_BOOK_CNT--;

	pDM->StartFrame();

	if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT) //수입구분별 권 정보를 삭제 해야하는 상황 
	{
		
		pDM->InitDBFieldData();
		strQuery.Format(_T("DELETE FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqTypeVolKey);
		pDM->AddFrame(strQuery);
		

		//권 삭제까지 해야 하는 지를 판단한다
		CString strAnotherBookCnt = _T("");
		INT nAnotherBook = 0;
		strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND ACQ_CODE != '%s' AND BOOK_TYPE NOT IN ('B','A')"),strVolKey,strAcqCode);
		pDM->GetOneValueQuery(strQuery,strAnotherBookCnt);
		nAnotherBook = _ttoi(strAnotherBookCnt.GetBuffer(0));

		if(0 == nAnotherBook) //권 까지 삭제 (해당권에 기사와 부록이 연결되어 있는 지를 검사한다)
		{
			
			CString strArticleCnt;
			INT nArticleCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM->GetOneValueQuery(strQuery,strArticleCnt);
			nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
			
			if(nArticleCnt>0)
			{
				AfxMessageBox(_T("해당 권호에 기사 정보가 있어 삭제 할수 없습니다")); 
				pDM->EndFrame();
				return -1000;
			}

			CString strAppendixCnt;
			INT nAppendixCnt;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_APPENDIX_BOOK_TBL WHERE VOL_KEY = %s"),strVolKey);
			pDM->GetOneValueQuery(strQuery,strAppendixCnt);
			nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));

			if(nAppendixCnt>0)
			{
				AfxMessageBox(_T("해당 권호에 부록이 있어 삭제 할수 없습니다")); 
				pDM->EndFrame();
				return -1001;
			}

			pDM->InitDBFieldData();
			strQuery.Format(_T("DELETE FROM SE_VOL_TBL WHERE REC_KEY = %s"),strVolKey);
			pDM->AddFrame(strQuery);
			
			//삭제 권호 로 이동
			

		}	


	}
	else
	{
		
		//책수 및 입수 상태 update
		CString strCHECKIN_STATUS;
		if(0 == nMISS_BOOK_CNT) 
			strCHECKIN_STATUS = _T("1"); //입수
		else if(0 == nACQ_BOOK_CNT) 
			strCHECKIN_STATUS = _T("2"); //결호
		else 
			strCHECKIN_STATUS = _T("3"); //미달
		
		
		pDM->InitDBFieldData();
		strQuery.Format(_T("UPDATE SE_ACQ_TYPE_VOL_TBL SET ACQ_BOOK_CNT = %d, CHECKIN_STATUS = '%s' WHERE REC_KEY = %s"),nACQ_BOOK_CNT,strCHECKIN_STATUS,strAcqTypeVolKey);
		pDM->AddFrame(strQuery);
		
	}
	

	//누락등록번호 처리
	if(!strRegNo.IsEmpty())
	{
		CString strRegOnlyCode;
		CString strRegOnlyNo;
		INT nRegOnlyNo;
		CString strWorkLog;
		strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

		strRegOnlyCode = strRegNo.Left(2);
		nRegOnlyNo = _ttoi(strRegNo.Mid(2).GetBuffer(0));
		strRegOnlyNo.Format(_T("%d"),nRegOnlyNo);
			
		//등록 번호가 가등록 번호인지를 체크한다.(등록구분이 숫자이면 가등록 번호)
		if(!IsDigitString(strRegOnlyCode))
		{

			pDM->InitDBFieldData();
			strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
							_T("(REC_KEY,PUBLISH_FORM_CODE,ACCESSION_REC_MAKE_YEAR,ACCESSION_REC_NO,REG_CODE,REG_NO,FIRST_WORK,LAST_WORK) ")
							_T("VALUES (%s.NEXTVAL,'SE','%s','%s','%s','%s','%s',%s');"),
							DEFAULT_SEQ_NAME,strAccessionYear,strAccessionNo,strRegOnlyCode,strRegOnlyNo,strWorkLog,strWorkLog);
			
			pDM->AddFrame(strQuery);
		}
	}


	pDM->InitDBFieldData();
	strQuery.Format(_T("DELETE FROM SE_BOOK_TBL WHERE REC_KEY = %s"),strBookKey);
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();	

	return ids;
}
*/  

INT CSeApi::GetDeleteHighestPriority(CESL_DataMgr* pDM_BOOK)
{
	//책DM 중 삭제할 수 있는 우선 순위를 얻어온다 
	
	CString strCompare; // 비교 우선 순위 
	CString strPriority; //현재 최고 우선 순위 
	INT nHighestPriorityIdx = -1; //최고 우선 순위 rowidx

	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strCompare = _T("");
		strCompare = pDM_BOOK->GetCellData(_T("UDF_삭제우선순위"),i);
		
		if(0 == i) 
		{
			if(_T("Z") != strCompare)
			{
				strPriority = strCompare;
				nHighestPriorityIdx = 0;
			}
		}
		else
		{
			if(nHighestPriorityIdx<0)
			{
				if(_T("Z") != strCompare)
				{
					strPriority = strCompare;
					nHighestPriorityIdx = i;
				}
			}
			else
			{
				if(strPriority>strCompare) {
					strPriority = strCompare;
					nHighestPriorityIdx = i;
				}
					
			}
		}
	}
	
	pDM_BOOK->SetCellData(_T("UDF_삭제우선순위"),_T("Z"),nHighestPriorityIdx);

	return nHighestPriorityIdx;
}

INT CSeApi::MoveBookToMissBook(CESL_DataMgr* pDM_BOOK,INT nACQ_TYPE,INT nRowIdx)
{
	//해당 row의 책을 결호책으로 setting 한다.
	
	if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA112N"),nRowIdx);  //구입 결호   	
	else pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA212N"),nRowIdx);	 //기증 결호 
	
	pDM_BOOK->SetCellData(_T("SB_책종류"),_T("M"),nRowIdx); //결호책 
	pDM_BOOK->SetCellData(_T("SB_입수일"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_실시간인계일"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_제본인계일"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_등록인계일"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_수입년도"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_접수번호"),_T(""),nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_접수일련번호"),_T(""),nRowIdx); 

	return 0;
}

INT CSeApi::MoveBookToAcqBook(CESL_DataMgr* pDM_WORK,
							  INT nIdx_WORK,
							  CESL_DataMgr* pDM_BOOK,
							  CESL_DataMgr* pDM_BOOK_INSERT,
							  CESL_DataMgr* pDM_BOOK_OUT,
							  INT nACQ_TYPE,
							  CString strACQ_YEAR,
							  CString strRECEIPT_NO,
							  INT nRowIdx,
							  BOOL bIsDepartMngUse,
					          CESL_DataMgr* pDM_DEPART)
{
	//해당 row의 책을 입수책으로 setting 한다.
	//열람우선여부, 제본여부, 등록 책수를 확인해 WORKING_STATUS를 정한다. 

	
	//공통
	CString strLAST_SERIAL_NO;
	CString strToday = GetTodayDate();
	pDM_BOOK_OUT->SetCellData(_T("SB_책종류"),_T("C"),nRowIdx); //일반책 
	pDM_BOOK_OUT->SetCellData(_T("SB_입수일"),strToday,nRowIdx); 
	pDM_BOOK_OUT->SetCellData(_T("SB_수입년도"),strACQ_YEAR,nRowIdx); 
	pDM_BOOK_OUT->SetCellData(_T("SB_접수번호"),strRECEIPT_NO,nRowIdx); 
	strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	pDM_BOOK_OUT->SetCellData(_T("SB_접수일련번호"),strLAST_SERIAL_NO,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_실시간인계일"),strToday,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_제본인계일"),strToday,nRowIdx); 
	pDM_BOOK->SetCellData(_T("SB_등록인계일"),strToday,nRowIdx); 

	//자료 상태 
	//종자료의 제본 여부 
	CString strBINDING_YN;
	strBINDING_YN = pDM_WORK->GetCellData(_T("SS_제본여부"),nIdx_WORK);

	//수입구분의 열람우선여부,등록책수  
	CString strREAD_FIRST_YN,strREG_BOOK_CNT;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_열람우선여부"),nIdx_WORK);
	strREG_BOOK_CNT = pDM_WORK->GetCellData(_T("UDF_등록책수"),nIdx_WORK);
	INT nREG_BOOK_CNT = 0;
	if(!strREG_BOOK_CNT.IsEmpty()) nREG_BOOK_CNT = _ttoi(strREG_BOOK_CNT.GetBuffer(0));
	

	if(_T("Y") == strREAD_FIRST_YN) //실시간 처리 되어 있는 경우는 무조건 실시간 처리한다  
	{
		pDM_BOOK_OUT->SetCellData(_T("SB_자료상태"),_T("SEL111O"),nRowIdx);    	
	}
	else //실시간 처리되어 있지 않은 경우  
	{
		
		//수입 구분 정보의 등록 책수와 현재 책의 제본 및 등록 책수를 비교한다 
		if(nREG_BOOK_CNT>0) 
		{
			
			CString strWORKING_STATUS;
			INT nCurrentBindBook = 0;
			INT nCurrentRegBook = 0;

			//기존 데이터에서 제본 및 등록 대상인 책을 파악한다 
			for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
			{
				strWORKING_STATUS = pDM_BOOK->GetCellData(_T("SB_자료상태"),i);
				if(_T("SEB111N") == strWORKING_STATUS) nCurrentBindBook++;
				if(_T("SER111N") == strWORKING_STATUS) nCurrentRegBook++;
			}
			
			for(i=0;i<pDM_BOOK_INSERT->GetRowCount();i++)
			{
				strWORKING_STATUS = pDM_BOOK_INSERT->GetCellData(_T("SB_자료상태"),i);
				if(_T("SEB111N") == strWORKING_STATUS) nCurrentBindBook++;
				if(_T("SER111N") == strWORKING_STATUS) nCurrentRegBook++;
			}
			
			nREG_BOOK_CNT -= nCurrentBindBook;
			nREG_BOOK_CNT -= nCurrentRegBook;
			
			if(nREG_BOOK_CNT>0) //더 등록 혹은 제본해야할 책이 남아 있으면 
			{
				//제본 여부를 판단한다.
				if(_T("Y") == strBINDING_YN) pDM_BOOK_OUT->SetCellData(_T("SB_자료상태"),_T("SEB111N"),nRowIdx); 
				else pDM_BOOK_OUT->SetCellData(_T("SB_자료상태"),_T("SER111N"),nRowIdx); 	
			}
			else //아니면 실시간 
			{
				pDM_BOOK_OUT->SetCellData(_T("SB_자료상태"),_T("SEL111O"),nRowIdx);  
			}

		}
		else //실시간 처리 
		{
			pDM_BOOK_OUT->SetCellData(_T("SB_자료상태"),_T("SEL111O"),nRowIdx);    	
		}
	}

	if(bIsDepartMngUse)
	{
		//기본 배가위치코드
		CString strSHELF_LOC_CODE = _T("");
	    strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_배가위치_코드"),nIdx_WORK);

		CString strResultCode;
		INT ids = -1;

		ids = GetNextPriorityShelfLocCode(strResultCode,
										  strSHELF_LOC_CODE,
										  pDM_DEPART,
										  pDM_BOOK,
										  pDM_BOOK_INSERT,
										  2);

		if(ids>=0) pDM_BOOK_OUT->SetCellData(_T("SB_배가위치_코드"),strResultCode,nRowIdx);
	}


	return 0;
}

INT CSeApi::InspectDuplicateVol(CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_VOL)
{
	//권호 복본 조사 함수 
	//SV_권호명을 IDX_VOL_TITLE와 catsearch검색한 후 결과가 있으면 결과를 보여준다 
	CString strVOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_권호명"),nIdx_WORK);
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);
	
	if(strVOL_TITLE.IsEmpty()) return 0;

	CString strQuery;

/*
if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	strQuery.Format(_T("SPECIES_KEY = %s AND CATSEARCH(IDX_VOL_TITLE, '%s*', NULL)>0"),strSPECIES_KEY,strVOL_TITLE);
else
	strQuery.Format(_T("SPECIES_KEY = %s AND &ECOSEARCH(REC_KEY,SE_VOL_TITLE, '%s*)"),strSPECIES_KEY,strVOL_TITLE);
*/
	
	//like검색으로 전환 
	strQuery.Format(_T("SPECIES_KEY = %s AND IDX_VOL_TITLE LIKE '%%%s%%'"),strSPECIES_KEY,strVOL_TITLE);

	pDM_VOL->RefreshDataManager(strQuery);

	
	if(pDM_VOL->GetRowCount()>0) //복본된 권호가 있으면 확인 창을 띄운다 
	{
		CSeDuplicatedVolDlg dlg(m_pMgr);
		dlg.m_strVOL_TITLE = strVOL_TITLE;
		INT nCheck = dlg.DoModal();

		if(nCheck == IDOK) //기존 권호 입력 
		{
			INT nIdx = dlg.m_nCurrentIdx;
			CString strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"),nIdx);
			strQuery.Format(_T("REC_KEY = %s"),strVOL_KEY);
			pDM_VOL->RefreshDataManager(strQuery); //기존 권호를 가지고 온다 

		}
		else if(nCheck == IDCANCEL) //작업 중지 
		{
			return -1;
		}
		else //새 권호로 입력 
		{
			pDM_VOL->FreeData();
		}

	}

	return 0; 
}

INT CSeApi::MAKE_INDEX_by_SPECIES_MARC(CESL_DataMgr* pDM_SPECIES,CMarc* pMarc,CESL_DataMgr* pDM_IDX,INT nRowIdx)
{
	
	//pDM_SPECIES 알리아스에 따라서 pDM_IDX에 복사
	CString field_alias, field_name, field_type, data_type, strData;
	INT npos = 0;
	CString strtmpalias;

	INT nColumn = pDM_SPECIES->RefList.GetCount();
	for(INT col = 0; col < nColumn; col++) {   
		strData = _T("");
		pDM_SPECIES->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, nRowIdx, strData);
		
		npos = field_alias.Find(_T("_"));
		if(npos<0) continue;
		
		strtmpalias.Format(_T("ISS_%s"),field_alias.Mid(npos+1));
	
		pDM_IDX->SetCellData(strtmpalias, strData, nRowIdx);
	}


	//IndexToolKit을 사용해 MARC에서 색인정보를 추출한다
	APPLY_IndexItem(pDM_IDX,pMarc,nRowIdx); 


	//2004.02.24 수정(종의 SHELF_LOC_CODE -> 색인의 PLACE_INFO, MAIN_PLACE_INFO)
	//(SS_배가위치_코드-> ISS_자료있는곳INFO_코드, ISS_주자료있는곳INFO_코드)
	strData = pDM_SPECIES->GetCellData(_T("SS_배가위치_코드"),nRowIdx);
	pDM_IDX->SetCellData(_T("ISS_자료있는곳INFO_코드"), strData, nRowIdx);
	pDM_IDX->SetCellData(_T("ISS_주자료있는곳INFO_코드"), strData, nRowIdx);	



	return 0;
}

INT CSeApi::MAKE_EXP_WORK_by_IDX_ACQ(CESL_DataMgr* pDM_IDX,INT nIdx_IDX,CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_ACQ, INT nACQ_TYPE)
{
	//체크인 바탕 정보(색인, 수입구분)를 통해 예측정보를 위한 
	//권과 수입구분별 권 정보를 만들어 낸다. 
	

///////////////////////////////////권정보 
	
	//형태 구분
	CString strFORM_CODE_IDX = _T("");
	strFORM_CODE_IDX = pDM_IDX->GetCellData(_T("ISS_형식구분_코드"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_형식구분_코드"),strFORM_CODE_IDX,nIdx_WORK);


	//매체구분
	CString strMEDIA_CODE_IDX = _T("");
	strMEDIA_CODE_IDX = pDM_IDX->GetCellData(_T("ISS_매체구분_코드"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_매체구분_코드"),strMEDIA_CODE_IDX,nIdx_WORK);


	//화폐 구분	
	CString strCURRENCY_CODE_IDX = _T("");
	strCURRENCY_CODE_IDX = pDM_IDX->GetCellData(_T("SS_화폐구분_코드"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_화폐구분"),strCURRENCY_CODE_IDX,nIdx_WORK);


	//가격 
	CString strPRICE_IDX = _T("");
	strPRICE_IDX = pDM_IDX->GetCellData(_T("SS_가격"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_가격"),strPRICE_IDX,nIdx_WORK);


	//가격성격 
	CString strPRICE_CHARACTER_IDX = _T("");
	strPRICE_CHARACTER_IDX = pDM_IDX->GetCellData(_T("SS_가격성격"),nIdx_IDX);
	pDM_WORK->SetCellData(_T("SV_가격성격"),strPRICE_CHARACTER_IDX,nIdx_WORK);


	//종에서 마크 정보를 가지고 온다.
	CString strMarcStream;
	strMarcStream = pDM_IDX->GetCellData(_T("MARC"),nIdx_IDX);
	CMarc Marc;
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pMgr->m_pInfo->pMarcMgr->Decoding(strMarcStream, &Marc);

	
	//권호표제(245$a)
	CString strTITLE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTITLE_IDX);
	pDM_WORK->SetCellData(_T("SV_권호표제"),strTITLE_IDX,nIdx_WORK);

	if(strTITLE_IDX.IsEmpty())
	{
		CString strTITLE_IDX = _T("");
		strTITLE_IDX = pDM_IDX->GetCellData(_T("ISS_본표제"),nIdx_IDX);
		pDM_WORK->SetCellData(_T("SV_권호표제"),strTITLE_IDX,nIdx_WORK);
	}


	//면장수 
	CString strPAGE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$a"),strPAGE_IDX);
	pDM_WORK->SetCellData(_T("SV_면장수"),strPAGE_IDX,nIdx_WORK);


	//물리적특성 
	CString strPHYSICAL_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$b"),strPHYSICAL_IDX);
	pDM_WORK->SetCellData(_T("SV_물리적특성"),strPHYSICAL_IDX,nIdx_WORK);


	//책크기 
	CString strSIZE_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$c"),strSIZE_IDX);
	pDM_WORK->SetCellData(_T("SV_책크기"),strSIZE_IDX,nIdx_WORK);


	//딸림자료 
	CString strACCOMP_IDX = _T("");
	m_pMgr->m_pInfo->pMarcMgr->GetItem(&Marc,_T("300$d"),strACCOMP_IDX);
	pDM_WORK->SetCellData(_T("SV_딸림자료"),strACCOMP_IDX,nIdx_WORK);



//////////////////수입구분별 권 정보 
	//수입구분
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	pDM_WORK->SetCellData(_T("SA_수입구분"),strACQ_TYPE,nIdx_WORK);


	//수입구분KEY , 입수처 ,입수예정책수  
	CString strACQ_KEY = _T("");
	CString strPROVIDER = _T("");
	CString strACQ_EXP_BOOK_CNT = _T("");
	if(1 == nACQ_TYPE)
	{
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),0);
		strPROVIDER = pDM_ACQ->GetCellData(_T("SP_구입처_코드"),0);
		strACQ_EXP_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_입수책수"),0);
	} 
	else 
	{
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SD_기증관리KEY"),0);
		strPROVIDER = pDM_ACQ->GetCellData(_T("SD_기증자명"),0);
		strACQ_EXP_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_입수책수"),0);
	}

	pDM_WORK->SetCellData(_T("SA_수입구분KEY"),strACQ_KEY,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_입수처"),strPROVIDER,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_입수예정책수"),strACQ_EXP_BOOK_CNT,nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),nIdx_WORK);
	pDM_WORK->SetCellData(_T("SA_결호책수"),_T("0"),nIdx_WORK);
	

////////////////////기타정보
	
	//색인 - 종 정보 	
	const INT cnt = 11;
	CString alias[cnt] = {
		_T("ISS_종KEY"),
		_T("ISS_제어번호"),
		_T("SS_등록구분_코드"),
		_T("SS_이용제한구분_코드"),
		_T("SS_관리구분_코드"),
		_T("SS_배가위치_코드"),
		_T("SS_분류기호구분_코드"),
		_T("SS_제본여부"),
		_T("ISS_형식구분_코드"),
		_T("ISS_매체구분_코드"),
		_T("SS_화폐구분_코드")};
	
	CString data[cnt];
	pDM_IDX->GetCellData(alias,cnt,data,nIdx_IDX);
	pDM_WORK->SetCellData(alias,cnt,data,nIdx_WORK);
		

	//열람우선여부,등록책수 
	CString strREAD_FIRST_YN;
	CString strREG_BOOK_CNT;
	
	if(1 == nACQ_TYPE)
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SP_열람우선여부"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_등록책수"),0);
	} 
	else 
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SD_열람우선여부"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_등록책수"),0);
	}

	pDM_WORK->SetCellData(_T("UDF_열람우선여부"),strREAD_FIRST_YN,nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_등록책수"),strREG_BOOK_CNT,nIdx_WORK);
	

	return 0;
}

INT CSeApi::MAKE_DB_WORK_by_IDX_ACQ(CESL_DataMgr* pDM_IDX,INT nIdx_IDX,CESL_DataMgr* pDM_WORK,INT nIdx_WORK,CESL_DataMgr* pDM_ACQ, INT nACQ_TYPE)
{
	
	//색인 - 종 정보 	
	const INT cnt = 11;
	CString alias[cnt] = {
		_T("ISS_종KEY"),
		_T("ISS_제어번호"),
		_T("SS_등록구분_코드"),
		_T("SS_이용제한구분_코드"),
		_T("SS_관리구분_코드"),
		_T("SS_배가위치_코드"),
		_T("SS_분류기호구분_코드"),
		_T("SS_제본여부"),
		_T("ISS_형식구분_코드"),
		_T("ISS_매체구분_코드"),
		_T("SS_화폐구분_코드")};
	
	CString data[cnt];
	pDM_IDX->GetCellData(alias,cnt,data,nIdx_IDX);
	pDM_WORK->SetCellData(alias,cnt,data,nIdx_WORK);
		

	//열람우선여부,등록책수 
	CString strREAD_FIRST_YN;
	CString strREG_BOOK_CNT;
	
	if(1 == nACQ_TYPE)
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SP_열람우선여부"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SP_등록책수"),0);
	} 
	else 
	{
		strREAD_FIRST_YN = pDM_ACQ->GetCellData(_T("SD_열람우선여부"),0);
		strREG_BOOK_CNT = pDM_ACQ->GetCellData(_T("SD_등록책수"),0);
	}

	pDM_WORK->SetCellData(_T("UDF_열람우선여부"),strREAD_FIRST_YN,nIdx_WORK);
	pDM_WORK->SetCellData(_T("UDF_등록책수"),strREG_BOOK_CNT,nIdx_WORK);


	return 0;
}

INT CSeApi::MAKE_VOL_by_IDX_WORK(CESL_DataMgr* pDM_IDX,
								 INT nIdx_IDX,
								 CESL_DataMgr* pDM_WORK,
								 INT nIdx_WORK,
								 CESL_DataMgr* pDM_VOL,
								 CMarc* pMarc,
								 CString strVOLUME_CLASS)
{
	pDM_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK,nIdx_WORK,pDM_VOL,0);
	
	//REC_KEY
	CString strVOL_KEY = _T("");
	pDM_VOL->MakeRecKey(strVOL_KEY);
	pDM_VOL->SetCellData(_T("SV_권KEY"),strVOL_KEY,0);

	//종KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);
	pDM_VOL->SetCellData(_T("SV_종KEY"),strSPECIES_KEY,0);
	
	
	//색인권호명
	CString strVOL_TITLE = _T("");
	CString strOTHER_VOL_TITLE = _T("");
	CString strIDX_VOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_권호명"),nIdx_WORK);
	strOTHER_VOL_TITLE = pDM_WORK->GetCellData(_T("SV_부차적권호명"),nIdx_WORK);
	CArray<CString,CString> arrayList;
	arrayList.Add(strVOL_TITLE);
	arrayList.Add(strOTHER_VOL_TITLE);
	strIDX_VOL_TITLE = MakeKeyWord(arrayList);
	pDM_VOL->SetCellData(_T("SV_색인권호명"),strIDX_VOL_TITLE,0);


	//최종권호일련 번호(체크인 바탕 종 정보) 
	CString strLAST_VOL_SORT_NO = _T("");
	INT nLAST_VOL_SORT_NO;
	strLAST_VOL_SORT_NO = pDM_IDX->GetCellData(_T("SS_최종권호일련번호"),nIdx_IDX);
	nLAST_VOL_SORT_NO = _ttoi(strLAST_VOL_SORT_NO.GetBuffer(0));
	nLAST_VOL_SORT_NO += 10;
	strLAST_VOL_SORT_NO.Format(_T("%d"),nLAST_VOL_SORT_NO);
	pDM_VOL->SetCellData(_T("SV_권호일련번호"),strLAST_VOL_SORT_NO,0);
	pDM_IDX->SetCellData(_T("SS_최종권호일련번호"),strLAST_VOL_SORT_NO,nIdx_IDX); 

	
	//권호유형
	pDM_VOL->SetCellData(_T("SV_권호유형"),_T("V"),0);


	//권호구분
	pDM_VOL->SetCellData(_T("SV_권호구분"),strVOLUME_CLASS,0);
	
	
	//최초작업, 마지막 작업
	pDM_VOL->SetCellData(_T("SV_최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);
	pDM_VOL->SetCellData(_T("SV_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);


	return 0;
}

INT CSeApi::MAKE_ACQ_VOL_by_IDX_ACQ_WORK(CESL_DataMgr* pDM_ACQ,INT nACQ_TYPE, CESL_DataMgr* pDM_WORK, INT nIdx_WORK, CESL_DataMgr* pDM_ACQ_VOL,CString strCHECKIN_STATUS)
{
	pDM_ACQ_VOL->InsertRow(-1);
	CopyDMToDMByAlias(pDM_WORK,nIdx_WORK,pDM_ACQ_VOL,0);
	
	//REC_KEY
	CString strACQ_VOL_KEY = _T("");
	pDM_ACQ->MakeRecKey(strACQ_VOL_KEY);
	pDM_ACQ_VOL->SetCellData(_T("SA_수입구분별권KEY"),strACQ_VOL_KEY,0);
	
	//입수상태 
	pDM_ACQ_VOL->SetCellData(_T("SA_입수상태"),strCHECKIN_STATUS,0);


	//최초작업, 마지막 작업
	pDM_ACQ_VOL->SetCellData(_T("SA_최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);
	pDM_ACQ_VOL->SetCellData(_T("SA_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),0);


	return 0;
}

INT CSeApi::MAKE_BOOK_by_ACQ_WORK(CESL_DataMgr* pDM_ACQ,
								  INT nACQ_TYPE, 
								  CESL_DataMgr* pDM_WORK,
								  INT nIdx_WORK, 
								  CESL_DataMgr* pDM_BOOK, 
								  INT nACQ_BOOK_CNT,
								  INT nMISS_BOOK_CNT,
								  CString strACQ_YEAR,
					              CString strRECEIPT_NO,
								  BOOL bIsDepartMngUse,
								  CESL_DataMgr* pDM_DEPART) 
{
	
	//종KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);
	
	//등록구분
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_등록구분_코드"),nIdx_WORK);

	//이용제한구분
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_이용제한구분_코드"),nIdx_WORK);
	
	//관리구분
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_관리구분_코드"),nIdx_WORK);
	
	//배가위치코드
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_배가위치_코드"),nIdx_WORK);
	
	//분류기호구분
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_분류기호구분_코드"),nIdx_WORK);
	
	//수입구분 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//입력일 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();
//	COleDateTime t = COleDateTime::GetCurrentTime();
	//Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());	
	//strINPUT_DATE = GetTodayDate();
//	strINPUT_DATE.Format(_T("TO_DATE(%04d/%02d/%02d %02d:%02d:%02d),'YYYY/MM/DD HH24:MI:SS')"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());	

	//입수일 
	CString strACQ_DATE;
	strACQ_DATE = pDM_WORK->GetCellData(_T("UDF_입수일"),nIdx_WORK);
	if(strACQ_DATE.IsEmpty()) strACQ_DATE = GetTodayDate();
	
	
	//인계 여부 설정
	CString strQuery;
	CString strResult;
	CString strTRANSFER_YN;
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
				_T("WHERE GROUP_1='L' AND GROUP_2='수서' AND CLASS_ALIAS='실시간대상인계사용여부' ")
				_T("AND MANAGE_CODE=UDF_MANAGE_CODE");
	// strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '수서' AND CLASS_ALIAS='실시간대상인계사용여부'");
	//--2008.10.17 UPDATE BY PWR --}}
	pDM_BOOK->GetOneValueQuery(strQuery,strResult);
	
	if(_T("Y") == strResult)
		strTRANSFER_YN = _T("N");
	else
		strTRANSFER_YN = _T("Y");

	//책수 
	INT nTotolBook = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	
	//최초작업, 마지막 작업
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	
	CString strREC_KEY;
	//책수 설정
	for(INT i=0;i<nTotolBook;i++)
	{
		pDM_BOOK->InsertRow(-1);
		pDM_BOOK->MakeRecKey(strREC_KEY);		

		//공통
		pDM_BOOK->SetCellData(_T("SB_책KEY"),strREC_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_종KEY"),strSPECIES_KEY,i);
		pDM_BOOK->SetCellData(_T("SB_수입구분"),strACQ_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_등록구분_코드"),strREG_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_이용제한구분_코드"),strUSE_LIMIT_CODE,i);
		pDM_BOOK->SetCellData(_T("SB_관리구분"),strMANAGE_CODE,i);
		
		if(!bIsDepartMngUse)
			pDM_BOOK->SetCellData(_T("SB_배가위치_코드"),strSHELF_LOC_CODE,i);
		
		pDM_BOOK->SetCellData(_T("SB_분류기호구분_코드"),strCLASS_NO_TYPE,i);
		pDM_BOOK->SetCellData(_T("SB_입력일"),strINPUT_DATE,i);
		pDM_BOOK->SetCellData(_T("SB_최초작업"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_마지막작업"),strWorkLog,i);
		pDM_BOOK->SetCellData(_T("SB_제본책여부"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_레이블출력구분"),_T("1"),i); //default
		pDM_BOOK->SetCellData(_T("SB_장서점검"),_T("N"),i); //default
		pDM_BOOK->SetCellData(_T("SB_인계여부"),strTRANSFER_YN,i); //manage value

	}
	
	
	//자료 상태(WORKING_STATUS) 처리-->접수 번호 처리 추가   
	
	//종자료의 제본 여부 
	CString strBINDING_YN;
	strBINDING_YN = pDM_WORK->GetCellData(_T("SS_제본여부"),nIdx_WORK);
	

	//수입구분의 열람우선여부,등록책수  
	CString strREAD_FIRST_YN,strREG_BOOK_CNT;
	strREAD_FIRST_YN = pDM_WORK->GetCellData(_T("UDF_열람우선여부"),nIdx_WORK);
	strREG_BOOK_CNT = pDM_WORK->GetCellData(_T("UDF_등록책수"),nIdx_WORK);

	
	INT nREG_BOOK_CNT = 0;
	if(!strREG_BOOK_CNT.IsEmpty()) nREG_BOOK_CNT = _ttoi(strREG_BOOK_CNT.GetBuffer(0));
	
	CString strLAST_SERIAL_NO = _T("");
	
	if(0 == nACQ_BOOK_CNT) //1) 결호자료 
	{
		for(INT i=0;i<nTotolBook;i++)
		{
			if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA112N"),i);  //구입 결호   	
			else pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA212N"),i);	 //기증 결호 
			pDM_BOOK->SetCellData(_T("SB_책종류"),_T("M"),i); //결호책 
		}

	}
	else // 2)입수된 자료가 있을 경우  
	{
		if(_T("Y") == strREAD_FIRST_YN) //열람 우선 여부 
		{
			BOOL IsAssigned = FALSE;

			for(INT i=0;i<nTotolBook;i++)
			{
				if(!IsAssigned) {
					pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEL111O"),i);   //실시간   	
					pDM_BOOK->SetCellData(_T("SB_책종류"),_T("C"),i); //일반책 
					pDM_BOOK->SetCellData(_T("SB_입수일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_수입년도"),strACQ_YEAR,i); 
					pDM_BOOK->SetCellData(_T("SB_접수번호"),strRECEIPT_NO,i); 
					pDM_BOOK->SetCellData(_T("SB_실시간인계일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_제본인계일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_등록인계일"),strACQ_DATE,i); 
					strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
					pDM_BOOK->SetCellData(_T("SB_접수일련번호"),strLAST_SERIAL_NO,i); 
				}

				if(IsAssigned)	{
				
					if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA112N"),i);  //구입 결호     	
					else pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA212N"),i);	//기증 결호 
					pDM_BOOK->SetCellData(_T("SB_책종류"),_T("M"),i); //결호책 	
				}

				if(nACQ_BOOK_CNT == i+1) IsAssigned = TRUE;
			}
		
		}
		else //열람 우선 여부를 택하지 않았을 경우 
		{
			BOOL IsAssigned = FALSE;

			for(INT i=0;i<nTotolBook;i++)
			{
				
				if(!IsAssigned) {
				
					if(nREG_BOOK_CNT>0) //등록 책수 만큼 
					{
						if(_T("Y") == strBINDING_YN) //제본여부 파악
						{
							pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEB111N"),i);   //제본책   	
							pDM_BOOK->SetCellData(_T("SB_책종류"),_T("C"),i); //일반책 
						}
						else
						{
							pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SER111N"),i);   //등록책 	
							pDM_BOOK->SetCellData(_T("SB_책종류"),_T("C"),i); //일반책 
						}
						nREG_BOOK_CNT--;
					}
					else //실시간 
					{
						pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEL111O"),i);   //실시간   	
						pDM_BOOK->SetCellData(_T("SB_책종류"),_T("C"),i); //일반책 
					}

					pDM_BOOK->SetCellData(_T("SB_입수일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_수입년도"),strACQ_YEAR,i); 
					pDM_BOOK->SetCellData(_T("SB_접수번호"),strRECEIPT_NO,i); 
					strLAST_SERIAL_NO = GetReceiptSerialNo(pDM_BOOK, nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
					pDM_BOOK->SetCellData(_T("SB_접수일련번호"),strLAST_SERIAL_NO,i); 
					pDM_BOOK->SetCellData(_T("SB_실시간인계일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_제본인계일"),strACQ_DATE,i); 
					pDM_BOOK->SetCellData(_T("SB_등록인계일"),strACQ_DATE,i); 

				}
						
				if(IsAssigned)	{
						if(1 == nACQ_TYPE) pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA112N"),i);  //구입 결호     	
						else pDM_BOOK->SetCellData(_T("SB_자료상태"),_T("SEA212N"),i);	//기증 결호 
						pDM_BOOK->SetCellData(_T("SB_책종류"),_T("M"),i); //결호책 	
					}

				if(nACQ_BOOK_CNT == i+1) IsAssigned = TRUE;
				
			}
		}
	}


	//부서별 지정기능
	if(bIsDepartMngUse)
	{
		CString strResultCode;
		CString strBookType;
		INT ids = -1;
		
		for(i=0;i<nTotolBook;i++)
		{

			strBookType = pDM_BOOK->GetCellData(_T("SB_책종류"),i); 
			if(_T("C") != strBookType) continue;

			ids = GetNextPriorityShelfLocCode(strResultCode,
											  strSHELF_LOC_CODE,
											  pDM_DEPART,
											  pDM_BOOK);

			if(ids<0) continue;

			pDM_BOOK->SetCellData(_T("SB_배가위치_코드"),strResultCode,i);

		}


	}

	

	return 0;
}

INT CSeApi::MAKE_SUB_HISTORY_by_PURCHASE_MANAGE(CESL_DataMgr* pDM_PURCHASE, CESL_DataMgr* pDM_SUB_HISTORY, INT nRowIdx)
{
	
	//구입관리KEY
	CString strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구입관리KEY"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("구입관리KEY"),strtmp,nRowIdx);
	strtmp = _T("");

	//구입처_코드 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구입처_코드"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("구입처_코드"),strtmp,nRowIdx);
	strtmp = _T("");


	//구독가격 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구독가격"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("구독가격"),strtmp,nRowIdx);
	strtmp = _T("");


	//구독시작일 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구독시작일"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("구독시작일"),strtmp,nRowIdx);
	strtmp = _T("");


	//구독만기일 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구독만기일"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("구독만기일"),strtmp,nRowIdx);
	strtmp = _T("");

	
	//발행부수 
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_발행부수"),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("발행부수"),strtmp,nRowIdx);
	strtmp = _T("");


	return 0;
}

INT CSeApi::MAKE_DROP_ACCESSION_NO_by_BOOK(CESL_DataMgr* pDM_OUT,CESL_DataMgr* pDM_BOOK,CESL_DataMgr* pDM_DROP_NO)
{
	
	CString strREG_CODE,strREG_NO,strALL_REG_NO,strACCESSIOM_NO,strACCESSION_YEAR,strREC_KEY;
	CArray<CString,CString> RemoveAliasList;
	INT nREG_NO = 0;
	INT nRowIdx = 0;
	CString strWorkLog;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	for(INT i=0;i<pDM_BOOK->GetRowCount();i++)
	{
		strREG_CODE = _T("");
		strACCESSIOM_NO = _T("");
		strACCESSION_YEAR = _T("");

		strALL_REG_NO = pDM_BOOK->GetCellData(_T("등록번호"),i);
		if(strALL_REG_NO.IsEmpty()) continue;
		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strREG_CODE = strALL_REG_NO.Left(2);
// 		nREG_NO = _ttoi(strALL_REG_NO.Mid(2).GetBuffer(0));
// 		strREG_NO.Format(_T("%d"),nREG_NO);

		strREG_CODE = strALL_REG_NO.Left(nRegCodeLength);
		nREG_NO = _ttoi(strALL_REG_NO.Mid(nRegCodeLength).GetBuffer(0));
		strREG_NO.Format(_T("%d"),nREG_NO);
		//=====================================================
		

		//등록 번호가 가등록 번호인지를 체크한다.(등록구분이 숫자이면 가등록 번호)
		if(IsDigitString(strREG_CODE)) continue;

		strACCESSIOM_NO = pDM_BOOK->GetCellData(_T("원부번호"),i);
		strACCESSION_YEAR = pDM_BOOK->GetCellData(_T("원부작성년도"),i);
		
		pDM_DROP_NO->InsertRow(-1);
		nRowIdx = pDM_DROP_NO->GetRowCount()-1;
		pDM_DROP_NO->MakeRecKey(strREC_KEY); 
		pDM_DROP_NO->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("발행형태구분"),_T("SE"),nRowIdx);
		pDM_DROP_NO->SetCellData(_T("원부작성년도"),strACCESSION_YEAR,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("원부번호"),strACCESSIOM_NO,nRowIdx);
		
		pDM_DROP_NO->SetCellData(_T("등록구분"),strREG_CODE,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("등록번호"),strREG_NO,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("최초작업"),strWorkLog,nRowIdx);
		pDM_DROP_NO->SetCellData(_T("마지막작업"),strWorkLog,nRowIdx);

		pDM_OUT->InitDBFieldData();
		RemoveAliasList.RemoveAll();
		pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_DROP_NO, _T("CO_DROP_ACCESSION_NO_TBL"), nRowIdx , pDM_OUT);
	}


	return 0;
}

INT CSeApi::CheckAppendix(CESL_DataMgr* pDM_WORK,
					  INT nIdx_WORK,
					  INT nACQ_TYPE,
					  CESL_DataMgr* pDM_APPENDIX, 
					  CESL_DataMgr* pDM_BOOK)
{
	
	//부록예측이 되어 있는지를 확인후 정보를 보여준후 입력할 것인지를 결정한다.
	INT nRowCnt = pDM_APPENDIX->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	CString strVOL_TITLE = _T("");
	strVOL_TITLE = pDM_WORK->GetCellData(_T("SV_권호명"),nIdx_WORK);

	CSeAppedixExpDlg dlg(m_pMgr);
	dlg.m_strVOL_TITLE = strVOL_TITLE;
	dlg.DoModal();
	
	CString strUDF;
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strUDF = pDM_APPENDIX->GetCellData(_T("UDF_상태"),i);
		if(_T("I") != strUDF) continue;
		
		MAKE_APPENDIX_BOOK_by_WORK(pDM_WORK,nIdx_WORK,nACQ_TYPE,pDM_APPENDIX,i,pDM_BOOK);
	}

	return 0;
}

INT CSeApi::MAKE_APPENDIX_BOOK_by_WORK(CESL_DataMgr* pDM_WORK,
									   INT nIdx_WORK,
									   INT nACQ_TYPE,
									   CESL_DataMgr* pDM_APPENDIX,
									   INT nIdx_APPEN,
									   CESL_DataMgr* pDM_BOOK,
									   BOOL bOption)
{
	//작업 DM 으로 부록책과, 부록 정보를 구성한다. 
	
	////////////////////////////////////부록 책 
	//종KEY
	CString strSPECIES_KEY = _T("");
	strSPECIES_KEY = pDM_WORK->GetCellData(_T("ISS_종KEY"),nIdx_WORK);
	
	//권KEY
	CString strVOL_KEY = _T("");
	strVOL_KEY = pDM_WORK->GetCellData(_T("SV_권KEY"),nIdx_WORK);

	//등록구분
	CString strREG_CODE = _T("");
	strREG_CODE = pDM_WORK->GetCellData(_T("SS_등록구분_코드"),nIdx_WORK);

	//이용제한구분
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = pDM_WORK->GetCellData(_T("SS_이용제한구분_코드"),nIdx_WORK);
	
	//관리구분
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = pDM_WORK->GetCellData(_T("SS_관리구분_코드"),nIdx_WORK);
	
	//배가위치코드
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = pDM_WORK->GetCellData(_T("SS_배가위치_코드"),nIdx_WORK);
	
	//분류기호구분
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = pDM_WORK->GetCellData(_T("SS_분류기호구분_코드"),nIdx_WORK);
	
	//수입구분 
	CString strACQ_TYPE;
	strACQ_TYPE.Format(_T("%d"),nACQ_TYPE);
	
	//수입년도 
	CString strACQ_YEAR;
	strACQ_YEAR = GetCurrentYear();


	//입력일 
	CString strINPUT_DATE;
	strINPUT_DATE = GetTodayDate();

	//최초작업, 마지막 작업
	CString strWorkLog = _T("");
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);
	CString strBOOK_KEY;
	INT idx_book;
	
	//작업자
	CString strWorker = m_pMgr->m_pInfo->USER_ID;

	pDM_BOOK->InsertRow(-1);
	idx_book = pDM_BOOK->GetRowCount()-1;

	pDM_BOOK->MakeRecKey(strBOOK_KEY);		
	pDM_BOOK->SetCellData(_T("SB_책KEY"),strBOOK_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_종KEY"),strSPECIES_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_권KEY"),strVOL_KEY,idx_book);
	pDM_BOOK->SetCellData(_T("SB_수입구분"),strACQ_TYPE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_수입년도"),strACQ_YEAR,idx_book);
	pDM_BOOK->SetCellData(_T("SB_등록구분_코드"),strREG_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_이용제한구분_코드"),strUSE_LIMIT_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_관리구분"),strMANAGE_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_배가위치_코드"),strSHELF_LOC_CODE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_분류기호구분_코드"),strCLASS_NO_TYPE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_입력일"),strINPUT_DATE,idx_book);
	pDM_BOOK->SetCellData(_T("SB_최초작업"),strWorkLog,idx_book);
	pDM_BOOK->SetCellData(_T("SB_마지막작업"),strWorkLog,idx_book);
	pDM_BOOK->SetCellData(_T("SB_제본책여부"),_T("N"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_레이블출력구분"),_T("1"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_장서점검"),_T("N"),idx_book); //default
	pDM_BOOK->SetCellData(_T("SB_책종류"),_T("A"),idx_book); //부록책 
	pDM_BOOK->SetCellData(_T("SB_인계여부"),_T("Y"),idx_book); //default 
	pDM_BOOK->SetCellData(_T("SB_작업자"),strWorker,idx_book); 


	////////////////////////////////////부록 
	CString strAPPENDIX_KEY;
	pDM_APPENDIX->MakeRecKey(strAPPENDIX_KEY);	
	pDM_APPENDIX->SetCellData(_T("부록KEY"),strAPPENDIX_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("책KEY"),strBOOK_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("권KEY"),strVOL_KEY,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("최초작업"),strWorkLog,nIdx_APPEN);
	pDM_APPENDIX->SetCellData(_T("마지막작업"),strWorkLog,nIdx_APPEN);
	
	if(!bOption)
	{
		pDM_APPENDIX->SetCellData(_T("단행본등록여부"),_T("N"),nIdx_APPEN); //default
	}

	return 0;
}

INT CSeApi::UpdateExpectData(CESL_DataMgr* pDM_OUT,
							 CESL_DataMgr* pDM_ACQ,
							 INT nACQ_TYPE,
							 CESL_DataMgr* pDM_WORK,
							 INT nIdx_WORK, 
							 CArray<CString,CString>& arrayVolExpKeyList,
							 CArray<CString,CString>& arrayOtherVolExpKeyList)
{

	CString strPUB_EXP_KEY,strACQ_EXP_KEY,strPUB_EXP_TYPE,strACQ_EXP_TYPE,strTBL_NAME,strQuery;

	if(1 == nACQ_TYPE)
	{
		strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_발행예측정보KEY"),0); 
		strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_입수예측정보KEY"),0);
		strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_발행예측구분"),0);
		strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_입수예측구분"),0);
	}
	else
	{
		strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_발행예측정보KEY"),0); 
		strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_입수예측정보KEY"),0);
		strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_발행예측구분"),0);
		strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_입수예측구분"),0);
	}
	
	CString strNEW_PUB_EXP_DATE,strNEW_ACQ_EXP_DATE,strACQ_LAST_COUNT,strPUB_LAST_COUNT;
	strNEW_PUB_EXP_DATE = pDM_WORK->GetCellData(_T("UDF_다음발행예정일"),nIdx_WORK);
	strNEW_ACQ_EXP_DATE = pDM_WORK->GetCellData(_T("UDF_다음입수예정일"),nIdx_WORK);
	strACQ_LAST_COUNT = pDM_WORK->GetCellData(_T("UDF_입수예측된갯수"),nIdx_WORK);
	strPUB_LAST_COUNT = pDM_WORK->GetCellData(_T("UDF_발행예측된갯수"),nIdx_WORK);
	if(strACQ_LAST_COUNT.IsEmpty()) strACQ_LAST_COUNT = _T("0");
	if(strPUB_LAST_COUNT.IsEmpty()) strPUB_LAST_COUNT = _T("0");


	//발행일 
	if(!strNEW_PUB_EXP_DATE.IsEmpty())
	{
		strTBL_NAME = GetExpTable(strPUB_EXP_TYPE);
		strQuery.Format(_T("UPDATE %s SET EXP_START_DATE = '%s',LAST_ACQ_EXP_COUNT = %s WHERE REC_KEY = %s;"),strTBL_NAME,strNEW_PUB_EXP_DATE,strPUB_LAST_COUNT,strPUB_EXP_KEY);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	}
	
	//입수일 
	if(!strNEW_ACQ_EXP_DATE.IsEmpty())
	{
		strTBL_NAME = GetExpTable(strACQ_EXP_TYPE);
		strQuery.Format(_T("UPDATE %s SET EXP_START_DATE = '%s',LAST_ACQ_EXP_COUNT = %s WHERE REC_KEY = %s;"),strTBL_NAME,strNEW_ACQ_EXP_DATE,strACQ_LAST_COUNT,strACQ_EXP_KEY);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	}
	
	CString strACQ_EXP_NO;
	INT ids = 0; 

	//권호
	strACQ_EXP_NO = pDM_WORK->GetCellData(_T("UDF_권호예측시작수"),nIdx_WORK);
	if(!strACQ_EXP_NO.IsEmpty()) {
		ids = UpdateVolExpectData(pDM_OUT,strACQ_EXP_NO,_T("1"),arrayVolExpKeyList);
		if(ids<0) return -1;
	}

		
	//부차적 권호 
	strACQ_EXP_NO = pDM_WORK->GetCellData(_T("UDF_부차적권호예측시작수"),nIdx_WORK);
	if(!strACQ_EXP_NO.IsEmpty()) {
		ids = UpdateVolExpectData(pDM_OUT,strACQ_EXP_NO,_T("2"),arrayOtherVolExpKeyList);		
		if(ids<0) return -1;
	}

	return 0;
}

INT CSeApi::UpdateVolExpectData(CESL_DataMgr* pDM_OUT, CString strACQ_EXP_NO, CString strFORMAT_TYPE, CArray<CString,CString>& arrayVolExpKeyList)
{
	
	INT nPos = strACQ_EXP_NO.Find(_T(","));
	CString strREC_KEY;
	CString strQuery;
	

	if(nPos<0) //LEVEL 이 하나 있는 경우 
	{
		INT nSize = arrayVolExpKeyList.GetSize();
	
		if(nSize == 0) 
		{
			AfxMessageBox(_T("권호 예측 KEY를 찾을 수 없습니다"));
			return -1;
		}
		
		strREC_KEY = arrayVolExpKeyList.GetAt(0);
		
		strACQ_EXP_NO.TrimLeft();
		strACQ_EXP_NO.TrimRight();
		strREC_KEY.TrimLeft();
		strREC_KEY.TrimRight();
		
		strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),strACQ_EXP_NO,strREC_KEY,strFORMAT_TYPE);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddFrame(strQuery);
	
	}
	else //LEVEL이 여러개 있는 경우 
	{
		CString tmp;
		
		for(INT i=0;i<arrayVolExpKeyList.GetSize(); i++)
		{
			INT nLevel, nLimit;

			if(nPos<0) 
			{
				tmp = strACQ_EXP_NO;
			}
			else
			{
				tmp = strACQ_EXP_NO.Left(nPos);
				strACQ_EXP_NO = strACQ_EXP_NO.Mid(nPos+1);
			}
			
			tmp.TrimLeft();
			tmp.TrimRight();

			strREC_KEY = arrayVolExpKeyList.GetAt(i);
		
			strREC_KEY.TrimLeft();
			strREC_KEY.TrimRight();
			
			CString sQuery, sLevel, sLimit;
			sQuery.Format(_T("SELECT INC_UNIT_LEVEL FROM SE_VOL_EXP_TBL WHERE REC_KEY = %s AND FORMAT_TYPE = %s"),strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->GetOneValueQuery(sQuery,sLevel);
			
			sQuery.Format(_T("SELECT INC_UNIT_LIMIT FROM SE_VOL_EXP_TBL WHERE REC_KEY = %s AND FORMAT_TYPE = %s"),strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->GetOneValueQuery(sQuery,sLimit);

			if(!sLevel.IsEmpty())
			{
				nLevel = _ttoi(sLevel);
				nLimit = _ttoi(sLimit);
				if( nLimit > nLevel)
					nLevel++;
				else if( nLimit == nLevel )
					nLevel = 1;

				sLevel.Format(_T("%d"),nLevel);

				strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s', INC_UNIT_LEVEL = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),tmp,sLevel,strREC_KEY,strFORMAT_TYPE);
			}
			else
				strQuery.Format(_T("UPDATE SE_VOL_EXP_TBL SET ACQ_EXP_NO = '%s' WHERE REC_KEY = %s AND FORMAT_TYPE = %s;"),tmp,strREC_KEY,strFORMAT_TYPE);
			pDM_OUT->InitDBFieldData();
			pDM_OUT->AddFrame(strQuery);

			nPos = strACQ_EXP_NO.Find(_T(","));
		}
	}


	return 0;
}

INT CSeApi::SetInsertDefaultValue_SPECIES(CESL_DataMgr* pDM_SPECIES, CMarc* pMarc, INT nRowIdx)
{
	
	//default value
	pDM_SPECIES->SetCellData(_T("SS_입력구분"),_T("0"),nRowIdx); //화면입력
	pDM_SPECIES->SetCellData(_T("SS_입력일"),GetTodayDate(),nRowIdx); //입력일
	pDM_SPECIES->SetCellData(_T("SS_최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_SPECIES->SetCellData(_T("SS_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);


	
	//제어번호 setting
	CString strControlCode,strControlNo;
	strControlCode = pDM_SPECIES->GetCellData(_T("SS_제어자료구분_코드"),nRowIdx);
	strControlNo = GetControlNo(pDM_SPECIES, strControlCode);
	pDM_SPECIES->SetCellData(_T("SS_제어번호"),strControlNo,nRowIdx);
	


	//마크 제어번호 
	//연속설정
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pMgr->m_pInfo->pMarcMgr->SetItem(pMarc,_T("001*"),strControlNo);

	

	//수입년도 
	pDM_SPECIES->SetCellData(_T("SS_수입년도"),GetCurrentYear(),nRowIdx);
	

	
	//작업상태구분
	pDM_SPECIES->SetCellData(_T("SS_작업상태구분"),_T("1"),nRowIdx); //작업 종으로 입력
	

	return 0;
}

INT CSeApi::SetInsertDefaultValue_IDX(CESL_DataMgr* pDM_IDX, INT nRowIdx)
{
	//종구분값(작업상태구분)
	pDM_IDX->SetCellData(_T("ISS_종구분값"),_T("0"),nRowIdx); //작업 종으로 입력
		
	return 0;
}

INT CSeApi::SetInsertDefaultValue_PURCHASE(CESL_DataMgr* pDM_PURCHASE, INT nRowIdx)
{
	CString strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_입수책수"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_입수책수"),_T("1"), nRowIdx); 

	strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_등록책수"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_등록책수"),_T("0"), nRowIdx);

	strtmp = _T("");
	strtmp = pDM_PURCHASE->GetCellData(_T("SP_구독가격"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_PURCHASE->SetCellData(_T("SP_구독가격"),_T("0"), nRowIdx);

	pDM_PURCHASE->SetCellData(_T("SP_최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_PURCHASE->SetCellData(_T("SP_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetInsertDefaultValue_DONATE(CESL_DataMgr *pDM_DONATE, INT nRowIdx)
{
	CString strtmp = _T("");
    
	strtmp = _T("");
	strtmp = pDM_DONATE->GetCellData(_T("SD_입수책수"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_DONATE->SetCellData(_T("SD_입수책수"),_T("1"), nRowIdx); 

	strtmp = _T("");
	strtmp = pDM_DONATE->GetCellData(_T("SD_등록책수"),nRowIdx);
	if(strtmp.IsEmpty()) pDM_DONATE->SetCellData(_T("SD_등록책수"),_T("0"), nRowIdx);

	pDM_DONATE->SetCellData(_T("SD_최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_DONATE->SetCellData(_T("SD_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetInsertDefaultValue_SUB_HISTORY(CESL_DataMgr* pDM_SUB_HISTORY, INT nRowIdx)
{
	
	pDM_SUB_HISTORY->SetCellData(_T("최초작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	pDM_SUB_HISTORY->SetCellData(_T("마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}


INT CSeApi::SetModifyDefaultValue_SPECIES(CESL_DataMgr* pDM_SPECIES, INT nRowIdx)
{
	pDM_SPECIES->SetCellData(_T("SS_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::SetModifyDefaultValue_IDX(CESL_DataMgr* pDM_IDX, INT nRowIdx)
{
	
	return 0;
}

INT CSeApi::SetModifyDefaultValue_PURCHASE(CESL_DataMgr* pDM_PURCHASE,INT nRowIdx)
{
	pDM_PURCHASE->SetCellData(_T("SP_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::SetModifyDefaultValue_SUB_HISTORY(CESL_DataMgr* pDM_SUB_HISTORY,INT nRowIdx)
{
	pDM_SUB_HISTORY->SetCellData(_T("마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);

	return 0;
}

INT CSeApi::SetModifyDefaultValue_DONATE(CESL_DataMgr *pDM_DONATE, INT nRowIdx)
{
	pDM_DONATE->SetCellData(_T("SD_마지막작업"),m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),nRowIdx);
	return 0;
}

INT CSeApi::FileLog(CString str)
{
	FILE *fDebug = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));

	FILE * f = _tfopen(_T("_SE_API.log"), _T("a+b"));
	if (!f) return -1;
	
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(f, 0, SEEK_END);		
		if( 0 == ftell(f) )
		{
			/*UNCHANGE*/fputc(0xFF, f);
			/*UNCHANGE*/fputc(0xFE, f);
		}
		// ------------------------------------------------------------------------------
		
	_ftprintf(f, _T("%s\r\n"), str);
	fclose(f);
	return 0;
}

CString CSeApi::GetTodayDate()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CSeApi::GetCurrentYear()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;

}

INT CSeApi::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	INT pos;
	CString strTmpData;	
	CIndexTool* pIndexTool	= NULL;
	CIndex* pIndex			= NULL;
	
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if( !pIndexTool )	return -1;

	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return -1;

	if( strSource.GetLength() == 0 )	return 0;
	strSource.TrimLeft();	strSource.TrimRight();
	pos = strSource.Find(_T(" "), 0);	
	while( pos > 0 )
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		strSource.TrimLeft();	strSource.TrimRight();

		pIndex->MakeIndex(strTmpData);
		if( CheckIsHave(pTargetArray, strTmpData) == FALSE )		pTargetArray->Add(strTmpData);
		
		pos = strSource.Find(_T(" "), 0);
	}

	pIndex->MakeIndex(strSource);
	if( CheckIsHave(pTargetArray, strSource) == FALSE )		pTargetArray->Add(strSource);

	return 0;
}

BOOL CSeApi::CheckIsHave(CStringArray *codeValue, CString code)
{

	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;

}

INT CSeApi::ParsingKeyString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		
		strTmpData.TrimLeft();
		strTmpData.TrimRight();

		if(!strTmpData.IsEmpty()) pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}

	strSource.TrimLeft();
	strSource.TrimRight();
	if(!strSource.IsEmpty()) pTargetArray->Add(strSource);
	
	return 0;
}

CString CSeApi::GetExpTable(CString strType)
{

	CString strTBL_NAME = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 'W':
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 'D':
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 'E':
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	default:
		break;
	}

	return strTBL_NAME;
}

CString CSeApi::GetControlNo(CESL_DataMgr* pDM, CString strControlCode)
{
	//제어 번호를 얻는 함수 
	pDM->StartFrame();
	
	//해당년
	CString strYear;
	strYear = GetCurrentYear();

	CString strLastNumber,strQuery,strControlNo;

	
	strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//해당년도의 해당 제어번호를 SELECT하고 없을 경우는 INSERT한다 
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='CON_NO' AND PUBLISH_FORM_CODE='CO' AND DIVISION_VALUE='%s' ")
					 _T("AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strControlCode, strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'CON_NO' AND PUBLISH_FORM_CODE = 'CO' AND DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s'"),strControlCode,strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);

	INT nControlNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nControlNo = _ttoi(strLastNumber.GetBuffer(0));
	}

	nControlNo++;

	if( -4007 == ids)	//검색결과가 없으면 INSERT 
	{	
		
		if(!strControlCode.IsEmpty())
		{
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,DIVISION_VALUE,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
							_T("VALUES	(%s.NEXTVAL,'CON_NO','CO','%s','%s',1,'%s','%s','%s',UDF_MANAGE_CODE);"), DEFAULT_SEQ_NAME, strYear,strControlCode,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
			pDM->ExecuteQuery(strQuery,1);
		}
		
	}
	else	//있으면 UPDATE
	{
		//++2008.10.17 UPDATE BY PWR {{++
		// 통합시스템 적용
		strQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d, LAST_WORK='%s' ")
						 _T("WHERE MAKE_YEAR='%s' AND KIND_CODE='CON_NO' AND DIVISION_VALUE='%s' ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , nControlNo, m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI), strYear, strControlCode );
		// strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'CON_NO' AND DIVISION_VALUE = '%s';"),nControlNo,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),strYear,strControlCode);
		//--2008.10.17 UPDATE BY PWR --}}
		pDM->ExecuteQuery(strQuery,1);

	}

	strControlNo.Format(_T("%s%s%05d"),strControlCode,strYear,nControlNo);

	pDM->EndFrame();

	return strControlNo;
}


CString CSeApi::GetLastReceiptNo(CESL_DataMgr* pDM, INT nACQ_TYPE, CString strACQ_YEAR)
{
	//마지막 접수번호를 얻는 함수 
	
	// 2005-04-11 주석처리
	// pDM->StartFrame();
	CString strYear;
	strYear = strACQ_YEAR;

	CString strLastNumber,strQuery,strReceiptNo,strKindCode;

	if(1 == nACQ_TYPE)
		strKindCode = _T("CK1_NO"); //구입 접수번호 
	else if(2 == nACQ_TYPE)
		strKindCode = _T("CK2_NO"); //기증 접수번호 
	
	// 2005-04-11 주석처리
	// strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	// pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//해당년도의 마지막 접수번호를 SELECT하고 없을 경우는 INSERT한다 
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='%s' AND PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strKindCode, strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = '%s' AND PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s'"),strKindCode,strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);
	
	INT nReceiptNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nReceiptNo = _ttoi(strLastNumber.GetBuffer(0));
		nReceiptNo++;
	}
	else
	{
		nReceiptNo = 1;
	}

	// 2005-04-11 주석처리
	//if( -4007 == ids)	//검색결과가 없으면 INSERT 
	//{
	//	strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK)")
	//					_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s');"), DEFAULT_SEQ_NAME,strKindCode, strYear,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
	//	pDM->ExecuteQuery(strQuery,1);
	//}
	//else	//있으면 UPDATE
	//{
	//	strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),nReceiptNo,m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),strYear,strKindCode);
	//	pDM->ExecuteQuery(strQuery,1);
	//}
	//
	//pDM->EndFrame();
	
	strReceiptNo.Format(_T("%d"),nReceiptNo);

	return strReceiptNo;
}

  
CString CSeApi::GetReceiptSerialNo(CESL_DataMgr* pDM, INT nACQ_TYPE, CString strACQ_YEAR, CString strRECEIPT_NO)
{
	/*
	//접수 일련 번호를 얻는 함수 
	CString strLAST_SERIAL_NO = _T("");
	
	pDM->StartFrame();
	
	CString strQuery;
	
	strQuery.Format(_T("LOCK TABLE SE_RECEIPT_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	CString strDBLastNo = _T("");

	//해당 접수번호로 최종 번호를 SELECT한다. 
	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	INT ids =  pDM->GetOneValueQuery(strQuery,strDBLastNo,FALSE);
	INT nLastNo = _ttoi(strDBLastNo.GetBuffer(0));
	nLastNo++;


	strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO = %d WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),nLastNo,nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	pDM->ExecuteQuery(strQuery,1);
	pDM->EndFrame();
	
	strLAST_SERIAL_NO.Format(_T("%d"),nLastNo);
	return strLAST_SERIAL_NO;
	*/

	//modified by loveisverb 2004/09/08
	//락을 제외


	//해당 접수번호로 최종 번호를 SELECT한다. 
	CString strQuery;
	CString strDBLastNo,strLAST_SERIAL_NO;
	INT nLastNo = 0;
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL ")
					_T("WHERE ACQ_CODE='%d' AND ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE")
					, nACQ_TYPE, strACQ_YEAR, strRECEIPT_NO );
	// strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strDBLastNo);
	if(!strDBLastNo.IsEmpty()) nLastNo = _ttoi(strDBLastNo.GetBuffer(0));
	nLastNo++;


	pDM->StartFrame();
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO=%d ")
					_T("WHERE ACQ_CODE='%d' AND ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE;")
					,nLastNo, nACQ_TYPE, strACQ_YEAR, strRECEIPT_NO );
	// strQuery.Format(_T("UPDATE SE_RECEIPT_TBL SET LAST_SERIAL_NO = %d WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),nLastNo,nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
	//--2008.10.17 UPDATE BY PWR --}}
	pDM->AddFrame(strQuery);
	pDM->SendFrame();
	pDM->EndFrame();

	strLAST_SERIAL_NO.Format(_T("%d"),nLastNo);
	return strLAST_SERIAL_NO;
}

CString CSeApi::GetSerialWorkNo(CESL_DataMgr* pDM, CString strSEQ_NO_KEY, CString strWorkNo)
{
	// 작업 일련 번호를 얻는 함수 
	CString strSerialWorkNo = _T("");
	INT nSerialNo = 0;
	CString strQuery;

	pDM->StartFrame();

	
	strQuery.Format(_T("LOCK TABLE SE_PURCHASE_WORK_NO_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s AND WORK_NO = %s"),strSEQ_NO_KEY,strWorkNo);
	INT ids =  pDM->GetOneValueQuery(strQuery,strSerialWorkNo,FALSE);

	if(ids<0)
	{
		pDM->EndFrame();
		return _T("");
	}
	
	nSerialNo = _ttoi(strSerialWorkNo.GetBuffer(0));
	nSerialNo++;
	
	strQuery.Format(_T("UPDATE SE_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d WHERE PURCHASE_SEQ_NO_KEY = %s AND WORK_NO = %s;"),nSerialNo,strSEQ_NO_KEY,strWorkNo);
	pDM->ExecuteQuery(strQuery,1);

	pDM->EndFrame();
	strSerialWorkNo.Format(_T("%d"),nSerialNo);


	return strSerialWorkNo;
}

CString CSeApi::GetLastWorkNo(CESL_DataMgr* pDM, CString strSEQ_NO_KEY)
{
	//차수번호내 마지막 작업 번호 얻는 함수 
	CString strLastWorkNo = _T("");
	INT nLastWorkNo = 0;
	CString strQuery;
	
	pDM->StartFrame();
	
	
	strQuery.Format(_T("LOCK TABLE SE_PURCHASE_SEQ_NO_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	strQuery.Format(_T("SELECT LAST_WORK_NO FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),strSEQ_NO_KEY);
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastWorkNo,FALSE);

	if(ids<0)
	{
		pDM->EndFrame();
		return _T("");
	}
	
	nLastWorkNo = _ttoi(strLastWorkNo.GetBuffer(0));
	nLastWorkNo++;
	
	strQuery.Format(_T("UPDATE SE_PURCHASE_SEQ_NO_TBL SET LAST_WORK_NO = %d WHERE REC_KEY = %s;"),nLastWorkNo,strSEQ_NO_KEY);
	pDM->ExecuteQuery(strQuery,1);
	
	pDM->EndFrame();
	strLastWorkNo.Format(_T("%d"),nLastWorkNo);

	return strLastWorkNo;
}

INT CSeApi::UpdateSpeciesMarcbyCMarc(CESL_DataMgr *pDM_SPECIES, CString strSpecies_key, CMarc *pMarc)
{

	if (!pDM_SPECIES) return-1;
	if (!pMarc) return -2;

	CString strStreamMarc;
	m_pMgr->m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
	pDM_SPECIES->StartFrame();
	
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, -1);
	
	pDM_SPECIES->MakeUpdateFrame(pDM_SPECIES->TBL_NAME,_T("REC_KEY"),_T("NUMERIC"),strSpecies_key);

	INT ids = pDM_SPECIES->SendFrame();
	pDM_SPECIES->EndFrame();
	return ids;

}

INT CSeApi::APPLY_IndexItem(CESL_DataMgr *pDM_IDX, CMarc *pMarc, INT nRowIdx) 
{
	//연속설정
	m_pMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));


	//INDEX TOOL KIT 이용 기본 색인어 생성 
	INT nIndexToolKit = 0;
	CString IndexToolKit[MAX_TEMP_STRING_ARRAY_SIZE];
	CString IndexToolKitCommand[MAX_TEMP_STRING_ARRAY_SIZE];
		
	
	//기본 사항  (ISS_전체검색용색인어 제외)
	IndexToolKit[nIndexToolKit] = _T("ISS_검색용본표제");			IndexToolKitCommand[nIndexToolKit] = _T("서명파일");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_키워드");					IndexToolKitCommand[nIndexToolKit] = _T("키워드파일");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_검색용저작자");			IndexToolKitCommand[nIndexToolKit] = _T("저작자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_검색용발행자");			IndexToolKitCommand[nIndexToolKit] = _T("발행자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_범위검색용본표제");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용본표제");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_범위검색용발행자");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행자");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_범위검색용발행년도");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행년도");	nIndexToolKit++;
	
	//2004.9.2 loveisverb수정
	//IndexToolKit[nIndexToolKit] = _T("ISS_표제사항");				IndexToolKitCommand[nIndexToolKit] = _T("표제사항INFO");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_표제사항");				IndexToolKitCommand[nIndexToolKit] = _T("표제사항");			nIndexToolKit++;

	IndexToolKit[nIndexToolKit] = _T("ISS_저작자사항");				IndexToolKitCommand[nIndexToolKit] = _T("저작자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_발행자사항");				IndexToolKitCommand[nIndexToolKit] = _T("발행자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_발행년도사항");			IndexToolKitCommand[nIndexToolKit] = _T("발행년도사항");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_본표제");					IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_저작자");					IndexToolKitCommand[nIndexToolKit] = _T("저작자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_발행자");					IndexToolKitCommand[nIndexToolKit] = _T("발행자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_발행년도2");				IndexToolKitCommand[nIndexToolKit] = _T("발행년");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_발행지");					IndexToolKitCommand[nIndexToolKit] = _T("발행지");				nIndexToolKit++;

	
	//이외의 부호 값 
	IndexToolKit[nIndexToolKit] = _T("ISS_ISBN");					IndexToolKitCommand[nIndexToolKit] = _T("표준부호_ISBN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_STRN");					IndexToolKitCommand[nIndexToolKit] = _T("표준부호_STRN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_RNSTRN");					IndexToolKitCommand[nIndexToolKit] = _T("표준부호_RNSTRN");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_국가서지부호");			IndexToolKitCommand[nIndexToolKit] = _T("표준부호_국가서지부호");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_저작권등록번호");			IndexToolKitCommand[nIndexToolKit] = _T("표준부호_저작권등록번호");				nIndexToolKit++;	
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호KDCP");			IndexToolKitCommand[nIndexToolKit] = _T("KDCP");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호KDC");			IndexToolKitCommand[nIndexToolKit] = _T("KDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호LC");				IndexToolKitCommand[nIndexToolKit] = _T("LC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호NDC");			IndexToolKitCommand[nIndexToolKit] = _T("NDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호CEC");			IndexToolKitCommand[nIndexToolKit] = _T("CEC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호COC");			IndexToolKitCommand[nIndexToolKit] = _T("COC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호CWC");			IndexToolKitCommand[nIndexToolKit] = _T("CWC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호DDC");			IndexToolKitCommand[nIndexToolKit] = _T("DDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_분류기호UDC");			IndexToolKitCommand[nIndexToolKit] = _T("UDC");					nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_한국대학부호");			IndexToolKitCommand[nIndexToolKit] = _T("한국대학부호");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_한국정부기관부호");		IndexToolKitCommand[nIndexToolKit] = _T("정부기관부호");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_본문언어");				IndexToolKitCommand[nIndexToolKit] = _T("본문언어");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("ISS_요약문언어");				IndexToolKitCommand[nIndexToolKit] = _T("요약문언어");			nIndexToolKit++;

	CString strData;	

	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return -1;

	pIndexTool->Init(m_pMgr->m_pInfo->pMarcMgr,pMarc);

	for(INT i=0;i<nIndexToolKit;i++) {
		strData = _T("");
		pIndexTool->GetIndex(IndexToolKitCommand[i], strData);
		pDM_IDX->SetCellData(IndexToolKit[i], strData, nRowIdx);
	}

	//008 TAG 정보 입력
	//간행상태
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@6"),strData);
	pDM_IDX->SetCellData(_T("ISS_간행상태"), strData, nRowIdx);
	strData = _T("");
	
	//창간년,폐간년  
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@7-10"),strData);
	pDM_IDX->SetCellData(_T("ISS_창간년"), strData, nRowIdx);
	strData = _T("");

	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@11-14"),strData);
	pDM_IDX->SetCellData(_T("ISS_폐간년"), strData, nRowIdx);
	strData = _T("");
	

	//간행빈도(부정기간(빈칸)의 경우 _T("_") 처리를 해준다 
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@18"),strData);
	
	strData.TrimLeft();
	strData.TrimRight();

	if(strData.IsEmpty())
	{
		pDM_IDX->SetCellData(_T("ISS_간행빈도"), _T("_"), nRowIdx);
	}
	else
	{
		pDM_IDX->SetCellData(_T("ISS_간행빈도"), strData, nRowIdx);
	}
	
	strData = _T("");
	

	//정규성
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@19"),strData);
	pDM_IDX->SetCellData(_T("ISS_정규성"), strData, nRowIdx);
	strData = _T("");
	
	//이외의 값들 
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("022$a"),strData);
	pDM_IDX->SetCellData(_T("ISS_ISSN"), strData, nRowIdx);
	strData = _T("");
	
	m_pMgr->m_pInfo->pMarcMgr->GetItem(pMarc,_T("030$a"),strData);
	pDM_IDX->SetCellData(_T("ISS_CODEN"), strData, nRowIdx);
	strData = _T("");


	//전체검색용색인어
	//IDX_ALL_ITEM필드를 채우기 위해서 검색용본표제, 저작자, 발행자, 키워드 를 추출한다.
	CString strIdxTitle,strIdxAuthor,strIdxPublihser,strIdxKeyword;
	pDM_IDX->GetCellData(_T("ISS_검색용본표제"), nRowIdx, strIdxTitle);
	pDM_IDX->GetCellData(_T("ISS_검색용저작자"), nRowIdx, strIdxAuthor);
	pDM_IDX->GetCellData(_T("ISS_검색용발행자"), nRowIdx, strIdxPublihser);
	pDM_IDX->GetCellData(_T("ISS_키워드"), nRowIdx, strIdxKeyword);

	// IDX_ALL_ITEM필드 설정 - 검색용본표제, 검색용저자, 검색용 발행자, 검색용색인어
	CString strIdxAllItem ;
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublihser + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if( strIdxAllItem.IsEmpty() == TRUE )	
		{
			strIdxAllItem = TargetArray.GetAt(i);
		}
		else 
		{
			strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
		}
	}

	pDM_IDX->SetCellData(_T("ISS_전체검색용색인어"), strIdxAllItem, nRowIdx);


	return 0;
}

INT CSeApi::APPLY_IndexItem_Book(CESL_DataMgr* pDM_OUT, CString strSpeciesKey)
{

	//색인의 책정보관련 정보 구성
	CString strQuery;

	//등록 구분(REG_CODE)
	CString strRegCode;
	strRegCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'REG_CODE') FROM DUAL)"),strSpeciesKey);
	

	//이용제한구분(USE_LIMIT_CODE)
	CString strUseLimitCode;
	strUseLimitCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'USE_LIMIT_CODE') FROM DUAL)"),strSpeciesKey);
	

	//관리구분(MANAGE_CODE)
	CString strManageCode;
	strManageCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'MANAGE_CODE') FROM DUAL)"),strSpeciesKey);


	//자료실(IDX_PLACE)
	CString strIdxPlace;
	strIdxPlace.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SHELF_LOC_CODE') FROM DUAL)"),strSpeciesKey);

	
	//별치기호(IDX_SE_SHELF_CODE)
	CString strSeparateCode;
	strSeparateCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SEPARATE_SHELF_CODE') FROM DUAL)"),strSpeciesKey);
	

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, USE_LIMIT_CODE = %s,")
					_T(" MANAGE_CODE = %s, IDX_PLACE = %s, IDX_SE_SHELF_CODE = %s")
					_T(" WHERE REC_KEY = %s;"),
					strRegCode,strUseLimitCode,strManageCode,strIdxPlace,strSeparateCode,strSpeciesKey);
	

//	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);

	//색인 변화 적용
	SetIdxTmpSeTbl(pDM_OUT, strSpeciesKey, _T("I"));

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET MANAGE_CODE = UDF_MANAGE_CODE ")
					_T("WHERE REC_KEY = %s;"),
					strSpeciesKey);	

//	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);

	return 0;
}

INT CSeApi::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}


CString CSeApi::MakeKeyWord(CArray <CString, CString> &IndexList)
{
	//해당리스트의 문자들을 빈칸을 제거하고 한자를 한글로 만들어 연결 시킨다.
	INT nSize;
	CString strKEYWORD = _T("");
	nSize = IndexList.GetSize();
	if(0 == nSize) return  strKEYWORD;
	
	INT nCount = 0;
	CString strData;
	
	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return strKEYWORD;

	CIndex* pIndex = NULL;
	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return strKEYWORD;


	for(INT i=0;i<nSize;i++) 
	{
		strData = _T("");
		strData = IndexList.GetAt(i);
		if(!strData.IsEmpty()) 
		{
			if(nCount) strKEYWORD += _T(" ");
			pIndex->TrimAll(strData);
			pIndex->RemoveSpecialChar(strData);
			pIndex->ConvertHanjaToHangul(strData);
			strKEYWORD += strData;
			nCount++;
		}
	}

	return strKEYWORD;
}

VOID CSeApi::MakeIndexWord(CString &strData)
{
	//해당 데이터의 공백을 지우고, 트림하고, 특수기호를 없앤후,  한자를 한글로 만든다.
	if(strData.IsEmpty()) return;

	strData.Replace(_T(" "),_T(""));

	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pMgr->m_pInfo->pIndexTool;
	if(!pIndexTool) return;

	CIndex* pIndex = NULL;
	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return;


	strData.MakeUpper();
	pIndex->TrimAll(strData);
	pIndex->RemoveSpecialChar(strData);
	pIndex->ConvertHanjaToHangul(strData);
}


VOID CSeApi::SetSerialMarcReader(CMarc* pMarc)
{
	//연속 마크 MarcReader 설정( 05 : n , 06 : a ,07 : s)
	
	CString strLeader;
	pMarc->GetLeader(strLeader);
	strLeader.SetAt(5,'n');
	strLeader.SetAt(6,'a');
	strLeader.SetAt(7,'s');
	pMarc->SetLeader(strLeader.GetBuffer(0));
	
}


INT CSeApi::BOOK_TRANSFER_TO_REAL_TIME(CESL_DataMgr* pDM, CString strBookKey)
{
	//실시간 인계 처리 
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("실시간 인계 처리  관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET TRANSFER_YN = 'Y',IMM_LOC_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_REG(CESL_DataMgr* pDM, CString strBookKey)
{
	//등록대상(SER111N) 처리 
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("등록대상 처리 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N',TRANSFER_YN = 'Y',REG_TRANSFER_DATE = '%s', LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();	

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_BINDING(CESL_DataMgr* pDM, CString strBookKey)
{
	//제본대상(SEB111N) 처리
	if(!pDM || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("제본대상 처리 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}


	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEB111N',TRANSFER_YN = 'Y',BINDING_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),GetTodayDate(),strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	return ids;
}

INT CSeApi::BOOK_TRANSFER_TO_SHELF(CESL_DataMgr* pDM, 
								   CString strPrevWorkingStatus, 
								   CString strSpeciesKey, 
								   CString strVolKey, 
								   CString strBookKey)
{
	//실시간 배가 처리
	//열람에서 제공하는 api를 사용해 목차 및 기사를 작업 -> 소장으로 만들어 놓는다
	if(!pDM || strPrevWorkingStatus.IsEmpty() || strSpeciesKey.IsEmpty() || strVolKey.IsEmpty() || strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("실시간 배가 관련 자료를 찾을 수 없습니다!"));
		return -1;
	}


	CString strQuery = _T("");
	CString strWorkLogMsg;
	strWorkLogMsg = m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI);

	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEL112N',")
					_T("TRANSFER_YN = 'Y',")
					_T("PREV_WORKING_STATUS = '%s',")
					_T("SHELF_DATE = '%s',")
					_T("SHELF_WORKER = '%s',")
					_T("LAST_WORK = '%s' ")
					_T("WHERE REC_KEY = %s;"),strPrevWorkingStatus,GetTodayDate(),m_pMgr->m_pInfo->USER_ID,strWorkLogMsg,strBookKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	

	CBL_SE_LOC_15 BL(m_pMgr);
	BL.SettingSpeciesField( strSpeciesKey , strVolKey );


	return ids;	
}

VOID CSeApi::SetIdxTmpSeTbl(CESL_DataMgr* pDM, CString strSpeciesKey, CString strMode)
{
	//색인에 변화가 생겼을 때 IDX_TMP_SE_TBL 에 설정해준다
	if(!pDM || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TBL (REC_KEY,SPECIES_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
// 					_T("VALUES (%s.NEXTVAL,%s,'%s','%s');"),DEFAULT_SEQ_NAME,strSpeciesKey,strMode,GetTodayDate());
// 
// 	//pDM->InitDBFieldData();
// 	pDM->AddFrame(strQuery);
	//=====================================================
	
	

	if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	{

	}
	else
	{
		//ecosearch를 위한 procedure
		strQuery.Format(_T("ECO$P_MAKE_SE$I(%s);"),strSpeciesKey);
		pDM->AddFrame(strQuery);
	}

}

VOID CSeApi::MakeEcoIndexVol(CESL_DataMgr* pDM, CString strVolKey, BOOL bIsInsert)
{

	if(ORA_CATSEARCH == m_pMgr->m_pInfo->GetSearchIndexType())
	{
			return;
	}
	else
	{
		//ecosearch를 위한 procedure

		CString strQuery;	
		pDM->InitDBFieldData();
		if(bIsInsert)
		{
			strQuery.Format(_T("ECO$P_MAKE_SEVOL$I(%s);"),strVolKey);
		}
		else 
		{
			strQuery.Format(_T("ECO$P_DELETE_SEVOL$I(%s);"),strVolKey);
		}

		pDM->AddFrame(strQuery);
	}
}

BOOL CSeApi::IsLightVersion(CESL_DataMgr* pDM)
{
	TCHAR szValue;
	
	szValue = m_pMgr->m_pInfo->GetKIIUPClientVersion();

	if('L' == szValue) return TRUE;

	return FALSE;
}

BOOL CSeApi::IsDepartMngUse(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'SEDEPARTMNG'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}

BOOL CSeApi::IsCoverImageUse(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}


BOOL CSeApi::IsDigitString(CString str)
{
	if(str.IsEmpty()) return TRUE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;

}

CString CSeApi::GetWorkingStatusDesc(CString strCode)
{
	CString code[64];
	CString desc[64];
	INT cnt = 0;

	code[cnt] = _T("SEA111N");	desc[cnt] = _T("구입입수자료"); cnt++;
	code[cnt] = _T("SEA112N");	desc[cnt] = _T("구입결호자료"); cnt++;
	code[cnt] = _T("SEA211N");	desc[cnt] = _T("기증입수자료"); cnt++;
	code[cnt] = _T("SEA212N");	desc[cnt] = _T("기증결호자료"); cnt++;
	code[cnt] = _T("SEB111N");	desc[cnt] = _T("제본인계자료"); cnt++;
	code[cnt] = _T("SEB112N");	desc[cnt] = _T("제본결정자료"); cnt++;
	code[cnt] = _T("SEB113N");	desc[cnt] = _T("제본발주자료"); cnt++;	
	code[cnt] = _T("SEB114N");	desc[cnt] = _T("제본검수자료"); cnt++;	
	code[cnt] = _T("SEL111O");	desc[cnt] = _T("실시간인계자료"); cnt++;	
	code[cnt] = _T("SEL112N");	desc[cnt] = _T("실시간배가자료"); cnt++;	
	code[cnt] = _T("SEL113O");	desc[cnt] = _T("실시간폐기자료"); cnt++;	
	code[cnt] = _T("SER111N");	desc[cnt] = _T("등록인계자료"); cnt++;
	code[cnt] = _T("SER112N");	desc[cnt] = _T("등록자료"); cnt++;	
	code[cnt] = _T("SER113O");	desc[cnt] = _T("가원부번호부여자료"); cnt++;
	code[cnt] = _T("SEC111N");	desc[cnt] = _T("정리대상자료"); cnt++;
	code[cnt] = _T("SEC112O");	desc[cnt] = _T("열람인계대상자료"); cnt++;
	code[cnt] = _T("SEL211O");	desc[cnt] = _T("열람인계자료"); cnt++;
	code[cnt] = _T("SEL212N");	desc[cnt] = _T("배가자료"); cnt++;
	code[cnt] = _T("SEL213O");	desc[cnt] = _T("수리제본자료"); cnt++;
	code[cnt] = _T("SEL214O");	desc[cnt] = _T("파손자료"); cnt++;
	code[cnt] = _T("SEL215O");	desc[cnt] = _T("제적자료"); cnt++;
	code[cnt] = _T("SEL216O");	desc[cnt] = _T("분실자료"); cnt++;
	code[cnt] = _T("SEL217O");	desc[cnt] = _T("재정리자료"); cnt++;
	code[cnt] = _T("SEL311O");	desc[cnt] = _T("관외대출자료"); cnt++;
	code[cnt] = _T("SEL312O");	desc[cnt] = _T("관내대출자료"); cnt++;
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	code[cnt] = _T("SEL411O");	desc[cnt] = _T("연속책두레업무진행자료"); cnt++;
	code[cnt] = _T("SEL511O");	desc[cnt] = _T("통합도서타관반납"); cnt++;
	//=====================================================
	code[cnt] = _T("SEL313O");	desc[cnt] = _T("특별대출자료"); cnt++;
	code[cnt] = _T("SEL314O");	desc[cnt] = _T("긴급대출자료"); cnt++;
    code[cnt] = _T("SEL315O");  desc[cnt] = _T("배달자료"); cnt++; 


	CString strDesc = _T("");
	for(INT i=0;i<cnt;i++)
	{
		if(code[i] == strCode)
		{
			strDesc = desc[i];
			break;
		}
	
	}

	return strDesc;
}

INT CSeApi::GetNextPriorityShelfLocCode(CString& strResult,
									    CString strDefaultCode,
									    CESL_DataMgr* pDM_DEPART,
									    CESL_DataMgr* pDM_BOOK,
									    CESL_DataMgr* pDM_BOOK_INSERT,
									    INT nMode)
{

	if(1 == nMode)
	{
		if(!pDM_DEPART || !pDM_BOOK) return -1;
	}
	else if(2 == nMode)
	{
		if(!pDM_DEPART || !pDM_BOOK || !pDM_BOOK_INSERT) return -1;
	}

	if(1 == nMode) //체크인
	{
		INT nRowBook = pDM_BOOK->GetRowCount();
		INT nRowDepart = pDM_DEPART->GetRowCount();

		if(0 == nRowDepart || 0 == nRowBook) //부서별 설정이 되어있지 않음
		{
			strResult = strDefaultCode;
			return 0;
		}

		strResult.Empty();
		CString strCode;
		CString strExpBookCnt;
		CString strCurCode;
		CString strBookType;
		INT nExpBookCnt = 0;
		INT nCurAssignCount = 0;

		for(INT i=0;i<nRowDepart;i++)
		{
			nExpBookCnt = 0;
			nCurAssignCount = 0;
			strCode = pDM_DEPART->GetCellData(_T("부서코드"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			if(strCode.IsEmpty()) strCode = strDefaultCode;
			strExpBookCnt = pDM_DEPART->GetCellData(_T("입수예정책수"),i);
			if(!strExpBookCnt.IsEmpty()) nExpBookCnt = _ttoi(strExpBookCnt.GetBuffer(0));
			
			
			for(INT j=0;j<nRowBook;j++)
			{
				strBookType = pDM_BOOK->GetCellData(_T("SB_책종류"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK->GetCellData(_T("SB_배가위치_코드"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			if(nCurAssignCount >= nExpBookCnt) 
			{
				continue;
			}
			else
			{
				strResult = strCode;
				return 0;
			}

		}	
	}
	else if(2 == nMode) //권수정
	{

		INT nRowBook = pDM_BOOK->GetRowCount();
		INT nRowBookInsert = pDM_BOOK_INSERT->GetRowCount();
		INT nRowDepart = pDM_DEPART->GetRowCount();
		INT nTotalBook = nRowBook + nRowBookInsert;

		if(0 == nRowDepart || 0 == nTotalBook) //부서별 설정이 되어있지 않음
		{
			strResult = strDefaultCode;
			return 0;
		}

		strResult.Empty();
		CString strCode;
		CString strExpBookCnt;
		CString strCurCode;
		CString strBookType;
		INT nExpBookCnt = 0;
		INT nCurAssignCount = 0;

		for(INT i=0;i<nRowDepart;i++)
		{
			nExpBookCnt = 0;
			nCurAssignCount = 0;
			strCode = pDM_DEPART->GetCellData(_T("부서코드"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			if(strCode.IsEmpty()) strCode = strDefaultCode;
			strExpBookCnt = pDM_DEPART->GetCellData(_T("입수예정책수"),i);
			if(!strExpBookCnt.IsEmpty()) nExpBookCnt = _ttoi(strExpBookCnt.GetBuffer(0));
			
			
			for(INT j=0;j<nRowBook;j++)
			{
				strBookType = pDM_BOOK->GetCellData(_T("SB_책종류"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK->GetCellData(_T("SB_배가위치_코드"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			for(j=0;j<nRowBookInsert;j++)
			{
				strBookType = pDM_BOOK_INSERT->GetCellData(_T("SB_책종류"),j);
				if(_T("C") != strBookType) continue;

				strCurCode = pDM_BOOK_INSERT->GetCellData(_T("SB_배가위치_코드"),j);
				if(strCode == strCurCode) nCurAssignCount++;
			}

			if(nCurAssignCount >= nExpBookCnt) 
			{
				continue;
			}
			else
			{
				strResult = strCode;
				return 0;
			}

		}	

	}
	
	return -1;

}