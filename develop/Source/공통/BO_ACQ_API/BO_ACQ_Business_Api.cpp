// BO_ACQ_Business_Api.cpp: implementation of the CBO_ACQ_Business_Api class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_ACQ_Business_Api.h"
#include "SortCondition.h"
#include "../BO_SPECIES_API/BO_SPECIES.h"
#include "BO_DeleteDlg.h"
#include "../BO_DATA_MANAGER/BO_DataConverter.h"

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("단행수서API"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_ACQ_Business_Api::CBO_ACQ_Business_Api(CESL_Mgr * pParentMgr):CSPF_Work(pParentMgr)
{
	m_pDataProcessingDlg = NULL;
}

CBO_ACQ_Business_Api::~CBO_ACQ_Business_Api()
{
	DeleteDataProcessingDlg();
}

INT CBO_ACQ_Business_Api::SPFExecutePath(CString strPath)
{
	if( strPath.IsEmpty() == TRUE )		return -1;

	if		( strPath == _T("책To종")			)			return MakeSpeciesInfoByBook();
	else if	( strPath == _T("책To수서정보")		)			return MakePurchaseInfoByBook();
	else if	( strPath == _T("책To권")			)			return MakeVolumeInfoByBook();
	else if	( strPath == _T("책To출력용권")		)			return MakePrintVolumeInfoByBook();
	else if	( strPath == _T("종DM삭제")			)			return DeleteSpeciesData();
	else if	( strPath == _T("권DM삭제")			)			return DeleteVolumeData();
	else if	( strPath == _T("DM자료삭제")		)			return DeleteDMData();
	else if	( strPath == _T("삭제")				)			return DeleteData();
	else if	( strPath == _T("종삭제")			)			return DeleteDataSpecies();
	else if	( strPath == _T("권삭제")			)			return DeleteDataVolume();
	else if	( strPath == _T("책삭제")			)			return DeleteDataBook();
	else if	( strPath == _T("미납자료삭제")		)			return DeleteMissingData();
	else if	( strPath == _T("구입처리")			)			return PurchaseData();
	else if	( strPath == _T("기증처리")			)			return DonateData();
	else if	( strPath == _T("자료복원")			)			return ProcessRecycleData(0, FALSE);
	else if	( strPath == _T("등록인계")			)			return RegTransfer(_T("BOR111N"));
	else if	( strPath == _T("복본대상")			)			return RegTransfer(_T("BOC111O"));
	else if	( strPath == _T("분류대상")			)			return RegTransfer(_T("BOC112O"));
	else if	( strPath == _T("정리대상")			)			return RegTransfer(_T("BOC113N"));
	else if	( strPath == _T("주문처리")			)			return OrderData();
	else if	( strPath == _T("심의처리")			)			return ReviewData();
	else if	( strPath == _T("검수처리")			)			return CheckInOrderData();
	else if	( strPath == _T("검수완료후등록대상처리"))		return CheckInComplete(_T("BOA114O"));
	else if	( strPath == _T("검수완료후등록인계"))			return CheckInComplete(_T("BOR111N"));
	else if	( strPath == _T("발주일련번호부여")	)			return GiveOrderSerialNo();
	else if	( strPath == _T("주문일련번호확인")	)			return CheckGiveOrderSerialNo();
	else if	( strPath == _T("그룹자료삭제")		)			return DeleteDataByDM();
	else if	( strPath == _T("납품처리")			)			return SetBookCountOnGrid();
	else if	( strPath == _T("납품취소")			)			return SetBookCountOnGrid();
	else if	( strPath == _T("구입처리_부모적용"))			return PurchaseDataApplyParent();
	else if	( strPath == _T("기증처리_부모적용"))			return DonateDataApplyParent();
	else if	( strPath == _T("선정자료주문취소")	)			return SelDataOrderCancel();
	else if	( strPath == _T("차수자료주문취소")	)			return DivNoOrderCancel();
	else if	( strPath == _T("그룹정보변경")		)			return ChangeGroup();
	else if	( strPath == _T("선정자료검수취소")	)			return CheckInCancelBySelData();
	else if	( strPath == _T("차수자료검수취소")	)			return CheckInCancelByDivNo();
	else if	( strPath == _T("구입정보오류수정")	)			return AdjustPurchaseDataError();
	else if	( strPath == _T("책색인정보동기화")	)			return SynchronizedBookToIndex();
	else if	( strPath == _T("색인재구축")		)			return RestructIndexField();
	else if	( strPath == _T("정렬필드설정")		)			return MakeSortNo();
	
	return -1;
}

INT CBO_ACQ_Business_Api::DeleteDataVolume()
{
	BOOL bRecycleYN = FALSE;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	INT nTempData = -1;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strVolSortNoAlias, strTmpData;
	CStringArray * parrSpeciesKey = NULL;
	CStringArray * parrAcqKey = NULL;
	CStringArray * parrVolSortNo = NULL;
	CGroupInfo * pGroup = NULL;
	CArray<INT, INT> * parrIdx = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("ACQ_TYPE"), nTempData);
	nAcqType = (ACQ_TYPE)nTempData;
	GetTempData(_T("재활용처리여부"), bRecycleYN);
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	GetTempData(_T("권일련번호알리아스"), strVolSortNoAlias);
	parrIdx = (CArray<INT, INT> *)GetTempPtr(_T("삭제인덱스"));
	parrSpeciesKey = (CStringArray*)GetTempPtr(_T("종키리스트"));
	parrAcqKey = (CStringArray*)GetTempPtr(_T("수서정보키리스트"));
	parrVolSortNo = (CStringArray*)GetTempPtr(_T("권일련번호리스트"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("그룹정보"));
	if(parrSpeciesKey != NULL) parrSpeciesKey->RemoveAll();
	if(parrAcqKey != NULL) parrAcqKey->RemoveAll();
	if(parrVolSortNo != NULL) parrVolSortNo->RemoveAll();
	
	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(bRecycleYN == TRUE)
	{
		strTmpData.Format(_T("%d건의 자료를 재활용처리중입니다."), nSelCnt);
	}
	else
	{
		strTmpData.Format(_T("%d건의 자료를 삭제중입니다."), nSelCnt);
	}
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	CString strSpeciesKey, strAccuKey, strVolSortNo;
	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		pDM->GetCellData(strVolSortNoAlias, i, strVolSortNo);
		
		ids = api.BO_VOLUME_DELETE(strSpeciesKey, strAccuKey, strVolSortNo, nSpeciesType, nAcqType, bRecycleYN, pGroup);
		if(ids < 0) return -1;

		parrIdx->Add(i);
		
		if(parrSpeciesKey != NULL) parrSpeciesKey->Add(strSpeciesKey);
		if(parrAcqKey != NULL && !strAccuKey.IsEmpty()) parrAcqKey->Add(strAccuKey);
		if(parrVolSortNo != NULL && !strVolSortNo.IsEmpty()) parrVolSortNo->Add(strVolSortNo);
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();

	return 0;
}

INT CBO_ACQ_Business_Api::DeleteDataSpecies()
{
	BOOL bRecycleYN = TRUE;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	INT nTempData = -1;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strDelOption, strTmpData;
	CArray<INT, INT> * parrIdx = NULL;
	CStringArray * parrSpeciesKey = NULL;
	CStringArray * parrAcqKey = NULL;
	CGroupInfo * pGroup = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("ACQ_TYPE"), nTempData);
	nAcqType = (ACQ_TYPE)nTempData;
	GetTempData(_T("재활용처리여부"), bRecycleYN);
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	parrIdx = (CArray<INT, INT> *)GetTempPtr(_T("삭제인덱스"));
	parrSpeciesKey = (CStringArray*)GetTempPtr(_T("종키리스트"));
	parrAcqKey = (CStringArray*)GetTempPtr(_T("수서정보키리스트"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("그룹정보"));

	if(parrAcqKey != NULL) parrAcqKey->RemoveAll();

	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(bRecycleYN == TRUE)
	{
		strTmpData.Format(_T("%d건의 자료를 재활용처리중입니다."), nSelCnt);
	}
	else
	{
		strTmpData.Format(_T("%d건의 자료를 삭제중입니다."), nSelCnt);
	}
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	CString strSpeciesKey, strAccuKey;
	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		if(nSpeciesType == BO_REG_INVALID || nSpeciesType == BO_CAT)
		{
			strTmpData = pDM->GetCellData(_T("BB_수입구분"), i);
			if(strTmpData == _T("1")) nAcqType = PURCHASE;
			else if(strTmpData == _T("2")) nAcqType = DONATE;
			else nAcqType = ACQ_TYPE_INVALID;
		}

		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		if(!strAccuKeyAlias.IsEmpty()) pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		if(bRecycleYN)
			ids = api.BO_SPECIES_RECYCLE(strSpeciesKey, strAccuKey, (INT)nAcqType, pGroup);
		else
			ids = api.BO_SPECIES_DELETE(strSpeciesKey, strAccuKey, (INT)nAcqType);
		if(ids < 0) return -1;
		parrIdx->Add(i);
		
		if(parrSpeciesKey != NULL) parrSpeciesKey->Add(strSpeciesKey);
		if(parrAcqKey != NULL && !strAccuKey.IsEmpty()) parrAcqKey->Add(strAccuKey);
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();

	return 0;
}

INT CBO_ACQ_Business_Api::DeleteMissingData()
{
	BOOL bRecycleYN = FALSE;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	INT nTempData = -1;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strVolSortNoAlias, strTmpData;
	CStringArray * parrSpeciesKey = NULL;
	CStringArray * parrAcqKey = NULL;
	CStringArray * parrVolSortNo = NULL;
	CGroupInfo * pGroup = NULL;
	CArray<INT, INT> * parrIdx = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("ACQ_TYPE"), nTempData);
	nAcqType = (ACQ_TYPE)nTempData;
	GetTempData(_T("재활용처리여부"), bRecycleYN);
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	GetTempData(_T("권일련번호알리아스"), strVolSortNoAlias);
	parrIdx = (CArray<INT, INT> *)GetTempPtr(_T("삭제인덱스"));
	parrSpeciesKey = (CStringArray*)GetTempPtr(_T("종키리스트"));
	parrAcqKey = (CStringArray*)GetTempPtr(_T("수서정보키리스트"));
	parrVolSortNo = (CStringArray*)GetTempPtr(_T("권일련번호리스트"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("그룹정보"));
	if(parrSpeciesKey != NULL) parrSpeciesKey->RemoveAll();
	if(parrAcqKey != NULL) parrAcqKey->RemoveAll();
	if(parrVolSortNo != NULL) parrVolSortNo->RemoveAll();
	
	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(bRecycleYN == TRUE)
	{
		strTmpData.Format(_T("%d건의 자료를 재활용처리중입니다."), nSelCnt);
	}
	else
	{
		strTmpData.Format(_T("%d건의 자료를 삭제중입니다."), nSelCnt);
	}
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	CString strSpeciesKey, strOldSpeciesKey, strAccuKey, strOldAccuKey, strVolSortNo;
	
	CString strQuery, strBookKey, strOldBookKey, strBookCount, strVolCount, strResult;
	INT nCnt;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		pDM->GetCellData(strSpeciesKeyAlias, i, strOldSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strOldAccuKey);
		pDM->GetCellData(strVolSortNoAlias, i, strVolSortNo);

		strQuery.Format(_T("SELECT COUNT(distinct VOL_SORT_NO) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), 
			            strOldSpeciesKey);
		pDM->GetOneValueQuery(strQuery, strVolCount);
		pDM->GetCellData(_T("책수"), i, strBookCount);
		nCnt = _ttoi(strBookCount);
		strQuery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = %s"), 
			            strOldSpeciesKey, strVolSortNo);
		pDM->GetOneValueQuery(strQuery, strResult);

		if(bRecycleYN && _ttoi(strVolCount) > 1 && _ttoi(strResult) <= nCnt)
		{
			strQuery.Format(_T("REC_KEY = %s"), strOldSpeciesKey);
			ids = api.m_pDM_SPECIES->RefreshDataManager(strQuery);
			if(ids < 0) return -1;

			strQuery.Format(_T("REC_KEY = %s"), strOldSpeciesKey);
			ids = api.m_pDM_IDX_BO->RefreshDataManager(strQuery);
			if(ids < 0) return -1;

			ids = api.SetAccuDM(TRUE);
			if(ids < 0) return -1;
			strQuery.Format(_T("REC_KEY = %s"), strOldAccuKey);
			ids = api.m_pDM_ACCU->RefreshDataManager(strQuery);
			if(ids < 0) return -1;

			strQuery.Format(_T("SPECIES_KEY = %s AND VOL_SORT_NO = %s AND WORKING_STATUS = 'BOA113O' AND BOOK_APPENDIX_FLAG = 'B' ORDER BY REPRESENT_BOOK_YN"), 
							strOldSpeciesKey, strVolSortNo);
			ids = api.m_pDM_BOOK->RefreshDataManager(strQuery);
			if(ids < 0) return -1;
			INT nRowCount = api.m_pDM_BOOK->GetRowCount();
			CStringArray arrOldBookKey;   arrOldBookKey.RemoveAll();
			for(INT Row=0; Row < nRowCount; Row++)
			{
				strOldBookKey = api.m_pDM_BOOK->GetCellData(_T("BB_책KEY"), Row);
				arrOldBookKey.Add(strOldBookKey);
			}

			CString strBookKey, strPurchaseInfoKey, strPrice;
			DOUBLE dPrice = 0.0;
			CArray <CString, CString> RemoveAliasList;
			RemoveAliasList.RemoveAll();

			ids = api.m_pDM_SPECIES->MakeRecKey(strSpeciesKey); 
			if(ids < 0) return -1;
			ids = api.m_pDM_SPECIES->MakeRecKey(strPurchaseInfoKey); 
			if(ids < 0) return -1;

			ids = pDM->StartFrame();
			if(ids < 0) return -1;

			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("DELETE_DATE"), _T("STRING"), GetTodayDate());
			api.m_pDM_SPECIES->SetCellData(_T("BS_종KEY"), strSpeciesKey, 0);
			api.m_pDM_SPECIES->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);	
			api.m_pDM_SPECIES->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);	
			api.m_pDM_SPECIES->MakeQueryByDM(RemoveAliasList, api.m_pDM_SPECIES, _T("BO_DELETE_SPECIES_TBL"), 0, pDM);
			api.m_pDM_SPECIES->FreeData();
			api.m_pDM_SPECIES->FreeDBFieldData();

			pDM->InitDBFieldData();
			api.m_pDM_IDX_BO->SetCellData(_T("IBS_REC_KEY"), strSpeciesKey, 0);
			api.m_pDM_IDX_BO->SetCellData(_T("IBS_종구분값"), _T("2"), 0);	
			api.m_pDM_IDX_BO->SetCellData(_T("IBS_삭제일"), GetTodayDate(), 0);	
			api.m_pDM_IDX_BO->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);	
			api.m_pDM_IDX_BO->MakeQueryByDM(RemoveAliasList, api.m_pDM_IDX_BO, _T("IDX_BO_TBL"), 0, pDM);
			api.m_pDM_IDX_BO->FreeData();

			api.m_pDM_ACCU->GetCellData(_T("BP_가격"), 0, strPrice);
			strPrice.Replace(_T(","), _T(""));
			
			TCHAR* pEndPtr = NULL;
			dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

			dPrice = dPrice * nCnt;
			strPrice.Format(_T("%.2f"), dPrice);
			api.m_pDM_ACCU->SetCellData(_T("BP_수서정보KEY"), strPurchaseInfoKey, 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_종KEY"), strSpeciesKey, 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_입력구분"), _T("3"), 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_발주책수"), strBookCount, 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_납품책수"), _T("0"), 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_미납책수"), strBookCount, 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_발주가격"), strPrice, 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_납품가격"), _T("0"), 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_총권수"), _T("1"), 0);
			api.m_pDM_ACCU->SetCellData(_T("BP_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
			api.m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, api.m_pDM_ACCU, _T("BO_PURCHASEINFO_TBL"), 0, pDM);
			api.m_pDM_ACCU->FreeData();

			for(INT j=0; j < arrOldBookKey.GetSize(); j++)
			{
				strQuery.Format(_T("REC_KEY = %s"), arrOldBookKey.GetAt(j));
				ids = api.m_pDM_BOOK->RefreshDataManager(strQuery);
				if(ids < 0) return -1;

				strOldBookKey = api.m_pDM_BOOK->GetCellData(_T("BB_책KEY"), 0);
				strQuery = api.m_pDM_BOOK->GetCellData(_T("BB_편권차"), 0);
				strQuery = api.m_pDM_BOOK->GetCellData(_T("BB_권일련번호"), 0);

				ids = api.m_pDM_SPECIES->MakeRecKey(strBookKey); 
				if(ids < 0) return -1;
				pDM->InitDBFieldData();
				pDM->AddDBFieldData(_T("DELETE_DATE"), _T("STRING"), GetTodayDate());
				RemoveAliasList.RemoveAll();
				RemoveAliasList.Add(_T("BB_원부KEY"));
				RemoveAliasList.Add(_T("BB_복본기호"));
				RemoveAliasList.Add(_T("BB_등록번호"));
				RemoveAliasList.Add(_T("BB_원본등록번호"));
				RemoveAliasList.Add(_T("BB_일련번호"));
				api.m_pDM_BOOK->SetCellData(_T("BB_책KEY"), strBookKey, 0);
				api.m_pDM_BOOK->SetCellData(_T("BB_종KEY"), strSpeciesKey, 0);
				api.m_pDM_BOOK->SetCellData(_T("BB_수서정보KEY"), strPurchaseInfoKey, 0);
				api.m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), pGroup->GetGroupNo(), 0);	
				api.m_pDM_BOOK->SetCellData(_T("BB_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);	
				api.m_pDM_BOOK->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);	
				api.m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, api.m_pDM_BOOK, _T("BO_DELETE_BOOK_TBL"), 0, pDM);
				api.m_pDM_BOOK->FreeData();

				ids = api.BO_BOOK_DELETE(strOldSpeciesKey, strOldAccuKey, strOldBookKey, nSpeciesType, nAcqType, FALSE, pGroup);
				if(ids < 0) return -1;
			}

			ids = pDM->SendFrame();
			if(ids < 0) return -1;
			pDM->EndFrame();

		}
		else
		{
			strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = %s AND WORKING_STATUS = 'BOA113O' ORDER BY REPRESENT_BOOK_YN"), 
							strOldSpeciesKey, strVolSortNo);
			tmpDM.RestructDataManager(strQuery);

			for(INT j=0; j < nCnt; j++)
			{
				strBookKey = tmpDM.GetCellData(j, 0);
				ids = api.BO_BOOK_DELETE(strOldSpeciesKey, strOldAccuKey, strBookKey, nSpeciesType, nAcqType, bRecycleYN, pGroup);
				if(ids < 0) return -1;
			}
		}

		parrIdx->Add(i);
		
		if(parrSpeciesKey != NULL) parrSpeciesKey->Add(strOldSpeciesKey);
		if(parrAcqKey != NULL && !strAccuKey.IsEmpty()) parrAcqKey->Add(strOldAccuKey);
		if(parrVolSortNo != NULL && !strVolSortNo.IsEmpty()) parrVolSortNo->Add(strVolSortNo);
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();

	return 0;
}

INT CBO_ACQ_Business_Api::DeleteSelectRowAndBookDM(CESL_DataMgr * pDM, CESL_DataMgr * pBookDM, CArray<INT, INT> * parrIdx, DELETE_TYPE nDelType)
{
	INT cnt = parrIdx->GetSize();
	if(cnt == 0) return 0;
	CString tmpstr, DelAlias1, DelAlias2, DelData1, DelData2;
	INT rowidx, ids;
	if(nDelType == SPECIES_DELETE)
	{
		DelAlias1 = _T("IBS_종KEY");	DelAlias2 = _T("BP_수서정보KEY");
	}
	else if(nDelType == VOLUME_DELETE)
	{	
		DelAlias1 = _T("BP_수서정보KEY");	DelAlias2 = _T("BB_권일련번호");
	}
	for(INT i=cnt-1 ; i>=0 ; i--)
	{
		rowidx = parrIdx->GetAt(i);
		
		DelData1 = pDM->GetCellData(DelAlias1, rowidx);
		DelData2 = pDM->GetCellData(DelAlias2, rowidx);
		
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0) return -1001;
		if(pBookDM)
		{
			ids = DeleteRow(pBookDM, DelData1, DelAlias1, DelData2, DelAlias2);
			if(ids < 0) return -1001;
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteSelectRow(CESL_DataMgr * pDM, CStringArray * parrIdx, CString strPath)
{
	INT cnt = parrIdx->GetSize();
	if(cnt == 0) return 0;
	CString tmpstr;
	INT rowidx, ids;
	for(INT i=cnt-1 ; i>=0 ; i--)
	{
		tmpstr = parrIdx->GetAt(i);
		rowidx = _ttoi(tmpstr.GetBuffer(0));
		if(strPath == _T("중복자료삭제"))
			DeleteDuplicateRow(pDM, rowidx);
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteSelectRow(CESL_DataMgr * pDM, CArray<INT, INT> * parrIdx, CString strPath)
{
	INT cnt = parrIdx->GetSize();
	if(cnt == 0) return 0;
	
	INT rowidx, ids;
	for(INT i=cnt-1 ; i>=0 ; i--)
	{
		rowidx = parrIdx->GetAt(i);
		if(strPath == _T("중복자료삭제"))
			DeleteDuplicateRow(pDM, rowidx);
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteDuplicateRow(CESL_DataMgr *pDM, INT rowidx)
{
	INT idx, cnt;
	CString tmpstr;
	pDM->GetCellData(_T("식별번호"), rowidx, tmpstr);
	if(tmpstr != _T("**"))
	{
		cnt = pDM->GetRowCount();
		for(INT i=rowidx+1 ; i<cnt ; i++)
		{
			pDM->GetCellData(_T("식별번호"), i, tmpstr);
			if(tmpstr != _T("**"))
			{
				idx = i;
				break;
			}
		}
		if(i == cnt)
			idx = cnt;
		idx--;
		while(idx > rowidx)
		{
			pDM->DeleteRow(idx--);
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::FindSameSpeciesKeyCnt(CESL_DataMgr * pBook_DM, INT *nStart, INT nRowCnt, CString strDivAlias, CString strSpeciesKey)
{
	INT i = *nStart;
	i++;
	INT cnt = 0;
	CString strTmpData;
	while(i < nRowCnt)
	{
		pBook_DM->GetCellData(strDivAlias, i, strTmpData);
		if(strTmpData == strSpeciesKey)
		{
			cnt++;
			i++;
		}
		else break;
	}
	*nStart = i;
	return cnt;
}

INT CBO_ACQ_Business_Api::MakeOtherInfoByBook(CESL_DataMgr * pTargetDM, CESL_DataMgr * pSourceDM, INT startIdx, INT endIdx, CString strAlias, CTextProgressCtrl * pProgressCtrl/*=NULL*/)
{
	CBO_SPECIES api(m_pParentMgr);
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strSpeciesWorkingStatus;
	INT ids, cnt, nDBookCnt, nTmp1, nTmp2;
	LONG nMinSortNo = LONG_MAX;
	DOUBLE nTotalPrice = 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;

	if(pProgressCtrl)
	{
		pProgressCtrl->SetRange(0, endIdx-startIdx)	;
		pProgressCtrl->SetPos(0);
		pProgressCtrl->SetWindowText(_T(""));
	}

	for(INT i=startIdx ; i<=endIdx; i++)
	{
		if(pProgressCtrl)
		{
			pProgressCtrl->StepIt();
		}
		pSourceDM->GetCellData(strAlias, i, strTmpKey);
		if(!strKey.IsEmpty() && strKey != strTmpKey)
		{
			ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
			if(ids < 0) break;
			strTmpData.Format(_T("%d"), cnt);
			pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);

			strTmpData = TruncPrice(pTargetDM, nTotalPrice);
			pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);

			pTargetDM->SetCellData(_T("UDF_변환종가격"), strTmpData, ids);	
			pTargetDM->SetCellData(_T("UDF_가격"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
			strTmpData.Format(_T("%ld"), nMinSortNo);
			pTargetDM->SetCellData(_T("UDF_일련번호"), strTmpData, ids);

			strSpeciesWorkingStatus.Empty();
			strKey = strTmpKey;
			cnt = 1;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);
			strTmpData.Replace(_T(","), _T(""));

			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strSpeciesWorkingStatus = pSourceDM->GetCellData(_T("BB_자료상태"), i);
			strTmpData = pSourceDM->GetCellData(_T("BB_일련번호"), i);
			nMinSortNo = _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			cnt++;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);
			strTmpData.Replace(_T(","), _T(""));
			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strTmpData = pSourceDM->GetCellData(_T("BB_자료상태"), i);
			strKey = strTmpKey;
			if(strTmpData.IsEmpty()) continue;
			nTmp1 = api.GetWorkingStatusPriority(strSpeciesWorkingStatus);
			nTmp2 = api.GetWorkingStatusPriority(strTmpData);
			if(nTmp1 < nTmp2) strSpeciesWorkingStatus = strTmpData;
			else if(nTmp1 == nTmp2) 
			{
				if(strSpeciesWorkingStatus.Compare(strTmpData) < 0) strSpeciesWorkingStatus = strTmpData;
			}
			strTmpData = pSourceDM->GetCellData(_T("BB_일련번호"), i);
			nTmp1 = _ttoi(strTmpData.GetBuffer(0));
			if(nMinSortNo > nTmp1) nMinSortNo = nTmp1;
		}
	}
	if(i > endIdx)
	{
		ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
		strTmpData.Format(_T("%d"), cnt);
		pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);
		strTmpData = TruncPrice(pTargetDM, nTotalPrice);
		pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);		
		pTargetDM->SetCellData(_T("UDF_변환종가격"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_가격"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
	}
	return 0;
}

INT CBO_ACQ_Business_Api::MakeVolumeInfoByBook()
{
	CBO_SPECIES api(m_pParentMgr);

	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pVolume_DM = NULL;
	CTextProgressCtrl * pProgressCtrl = NULL;

	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pVolume_DM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	pProgressCtrl = (CTextProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));

	if(!pBook_DM || !pVolume_DM) return -1;
	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CBO_DataConverter Converter;
	Converter.SetDivAlias(_T("BP_수서정보KEY"));
	Converter.SetProgressCtrl(pProgressCtrl);
	Converter.SetSourceDM(pBook_DM);
	Converter.SetTargetDM(pVolume_DM);
	INT ids = Converter.ConvertToVolumeInfo(FALSE);
	return ids;
}

INT CBO_ACQ_Business_Api::MakeSpeciesInfoByBook()
{
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pSpecies_DM = NULL;
	CTextProgressCtrl * pProgressCtrl = NULL;


	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pProgressCtrl = (CTextProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));
	if(!pBook_DM || !pSpecies_DM) return -1;

	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CBO_DataConverter Converter;
	Converter.SetDivAlias(_T("IBS_종KEY"));
	Converter.SetProgressCtrl(pProgressCtrl);
	Converter.SetSourceDM(pBook_DM);
	Converter.SetTargetDM(pSpecies_DM);
	INT ids = Converter.ConvertToSpeciesInfo();
	return ids;
}

INT CBO_ACQ_Business_Api::MakePurchaseInfoByBook()
{
	INT nRowCnt, ids;
	CESL_DataMgr* pBook_DM = NULL;
	CESL_DataMgr* pSpecies_DM = NULL;
	CTextProgressCtrl* pProgressCtrl = NULL;
	CBO_DataConverter Converter;

	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pProgressCtrl = (CTextProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));

	if( pBook_DM == NULL || pSpecies_DM == NULL )	return -1;

	nRowCnt = pBook_DM->GetRowCount();
	if( nRowCnt == 0 )	return 0;

	Converter.SetDivAlias(_T("BP_수서정보KEY"));
	Converter.SetProgressCtrl(pProgressCtrl);
	Converter.SetSourceDM(pBook_DM);
	Converter.SetTargetDM(pSpecies_DM);
	
	ids = Converter.ConvertToSpeciesInfo();
	return ids;	
}

INT CBO_ACQ_Business_Api::MakePurchaseInfoByBook(CESL_DataMgr * pBook_DM, CESL_DataMgr * pSpecies_DM)
{
	if(!pBook_DM || !pSpecies_DM) return -1;
	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CBO_DataConverter Converter;
	Converter.SetDivAlias(_T("BP_수서정보KEY"));
	Converter.SetProgressCtrl(NULL);
	Converter.SetSourceDM(pBook_DM);
	Converter.SetTargetDM(pSpecies_DM);
	INT ids = Converter.ConvertToSpeciesInfo();
	return ids;
}

INT CBO_ACQ_Business_Api::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	if( idx < 0) return -1000;

	INT col, nCol;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	INT nCurRow, nColCnt;
	pTargetDM->InsertRow(-1);
	nCurRow = pTargetDM->GetRowCount() - 1;
	nColCnt = pSourceDM->m_nCols;
	if(nColCnt < 0) nColCnt = pSourceDM->GetRefColumnCount();

	for (col = 0; col < nColCnt; col++)
	{
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol >= 0)	pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(idx, col));
	}
	
	return nCurRow;
}

INT CBO_ACQ_Business_Api::DeleteSpeciesData()
{
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;

	CESL_DataMgr * pSpecies_DM = NULL;
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pVolume_DM = NULL;
	INT nDelIdx, nTempData;
	nDelIdx = nTempData = -1;
	
	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pVolume_DM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("삭제인덱스"), nDelIdx);
	
	CString strSpeciesKey;
	pSpecies_DM->GetCellData(_T("IBS_종KEY"), nDelIdx, strSpeciesKey);
	pSpecies_DM->DeleteRow(nDelIdx);
	if(pVolume_DM)
		DeleteRow(pVolume_DM, strSpeciesKey, _T("IBS_종KEY"));
	if(pBook_DM)
		DeleteRow(pBook_DM, strSpeciesKey, _T("IBS_종KEY"));
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteVolumeData()
{
	CESL_DataMgr * pSpecies_DM = NULL;
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pVolume_DM = NULL;
	INT nDelIdx = -1;

	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pVolume_DM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	GetTempData(_T("삭제인덱스"), nDelIdx);

	CString strSpeciesKey, strVolSortNo, strBookCnt;
	pVolume_DM->GetCellData(_T("IBS_종KEY"), nDelIdx, strSpeciesKey);
	pVolume_DM->GetCellData(_T("BB_권일련번호"), nDelIdx, strVolSortNo);
	pVolume_DM->GetCellData(_T("UDF_책수"), nDelIdx, strBookCnt);

	pVolume_DM->DeleteRow(nDelIdx);
	if(pBook_DM)
		DeleteRow(pBook_DM, strSpeciesKey, _T("IBS_종KEY"), strVolSortNo, _T("BB_권일련번호"));
	if(pSpecies_DM)
	{
		CString strTmpData;
		INT idx = FindIndex(pSpecies_DM, _T("IBS_종KEY"), strSpeciesKey);
		if(idx < 0) return 0;
		CString strSpeciesBookCnt = pSpecies_DM->GetCellData(_T("UDF_책수"), idx);
		INT speciesBookCnt = _ttoi(strSpeciesBookCnt.GetBuffer(0));
		INT volumeBookCnt = _ttoi(strBookCnt.GetBuffer(0));
		if(speciesBookCnt > volumeBookCnt)
		{
			strTmpData.Format(_T("%d"), speciesBookCnt - volumeBookCnt);
			pSpecies_DM->SetCellData(_T("UDF_책수"), strTmpData, idx);
		}
		else
			pSpecies_DM->DeleteRow(idx);
	}
	return 0;
}

INT CBO_ACQ_Business_Api::FindIndex(CESL_DataMgr *pDM, CString strAlias, CString strKey, INT nIdx/*=0*/)
{
	INT rowCnt = pDM->GetRowCount();
	CString tmpstr;
	for(INT i=nIdx ; i<rowCnt ; i++)
	{
		tmpstr = pDM->GetCellData(strAlias, i);
		if(tmpstr == strKey) return i;
	}
	return -1;
}

INT CBO_ACQ_Business_Api::DeleteRow(CESL_DataMgr *pDM, CString strKey, CString strAlias, CString strKey1/*=_T("")*/, CString strAlias1 /*=_T("")*/)
{
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=nRowCnt ; i>=0; i--)
	{
		pDM->GetCellData(strAlias, i, strTmpData);
		if(strTmpData == strKey)
		{
			if(strAlias1.IsEmpty()) pDM->DeleteRow(i);
			else
			{
				pDM->GetCellData(strAlias1, i, strTmpData);
				if(strTmpData = strKey1) pDM->DeleteRow(i);
			}
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::PurchaseData()
{
	INT ids, nTmpData = 0;
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	STYPE = (SPECIES_TYPE)nTmpData;
	
	switch( STYPE )
	{
	case BO_BASIC:
	case BO_IMPORT:		ids = PurchaseAcqObjData(FALSE);
		break;
	case BO_REQUEST:	ids = ProcessRequestData(TRUE, FALSE);
		break;
	case BO_RECYCLE:	ids = ProcessRecycleData(PURCHASE, FALSE);
		break;
	default:			ids = -1;
		break;
	}

	return ids;
}

INT CBO_ACQ_Business_Api::PurchaseAcqObjData(BOOL bApplyParent)
{
	CESL_DataMgr * pParentDM = NULL;
	CESL_DataMgr * pParentBookDM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_Grid * pMainGrid = NULL;
	CGroupInfo * pGroupInfo = NULL;
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	INT nTmpData = 0;
	
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	STYPE = (SPECIES_TYPE)nTmpData;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("BOOKDM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroupInfo = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));

	if(pGroupInfo == NULL && m_pParentMgr->m_pInfo->GetKIIUPClientVersion() != 'L')
	{
		m_pParentMgr->ESLAfxMessageBox(_T("차수정보가 올바르지 않습니다."));
		return -1;
	}

	if(bApplyParent)
	{
		pParentDM = (CESL_DataMgr*)GetTempPtr(_T("ParentDM"));
		pParentBookDM = (CESL_DataMgr*)GetTempPtr(_T("ParentBookDM"));
		pMainGrid = (CESL_Grid*)GetTempPtr(_T("ParentGrid"));
	}

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	CStringArray arrIdx;		arrIdx.RemoveAll();
	CString strSpeciesKey, strWorkingStatus, strTmpData;
	if(STYPE == BO_BASIC) strWorkingStatus = _T("BOT111O");
	else if(STYPE == BO_IMPORT) strWorkingStatus = _T("BOT112O");
	else if(STYPE == SPECIES_TYPE_INVALID) strWorkingStatus = _T("");

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(STYPE == BO_BASIC) strTmpData.Format(_T("%d건의 기초자료를 구입처리중입니다."), nSelCnt);
	else if(STYPE == BO_IMPORT) strTmpData.Format(_T("%d건의 반입자료를 구입처리중입니다."), nSelCnt);
	else if(STYPE == SPECIES_TYPE_INVALID) strTmpData.Format(_T("%d건의 기초/반입자료를 구입처리중입니다."), nSelCnt);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);
	
	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-10000);
	api.InitDataMgr();

	pDM->StartFrame();
	ids = -1;

	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strSpeciesKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		ids = api.BO_SPECIES_ACQOBJ_PURCHASE(pDM, pGroupInfo, strSpeciesKey, strWorkingStatus, pParentDM, pParentBookDM, pMainGrid);
		if(ids < 0) return -2000;
		
		arrIdx.Add(strSpeciesKey);
		i = pGrid->SelectGetNext();

	}
	DeleteDataProcessingDlg();
	
	ids = pDM->SendFrame();
	if(ids < 0) return -3000;
	pDM->EndFrame();

	InitTempData();	InitTempPtr();
	SetTempPtr(_T("종DM"), (DWORD*)pDM);
	SetTempPtr(_T("권DM"), (DWORD*)NULL);
	SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	SetTempPtr(_T("삭제데이터"), (DWORD*)&arrIdx);
	SetTempData(_T("비교Alias"), _T("IBS_종KEY"));
	ids = DeleteDMData();
	if(ids < 0) return -4000;

	return 0;
}

INT CBO_ACQ_Business_Api::ProcessRequestData(BOOL isPurchase, BOOL bApplyParent)
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pParentDM = NULL;
	CESL_DataMgr * pParentBookDM = NULL;
	CESL_Grid * pMainGrid = NULL;
	CGroupInfo * pGroupInfo = NULL;
	CStringArray * parrFurnishKey;

	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroupInfo = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));
	parrFurnishKey = (CStringArray*)GetTempPtr(_T("FurnishKey"));

	if(bApplyParent)
	{
		pParentDM = (CESL_DataMgr*)GetTempPtr(_T("ParentDM"));
		pParentBookDM = (CESL_DataMgr*)GetTempPtr(_T("ParentBookDM"));
		pMainGrid = (CESL_Grid*)GetTempPtr(_T("ParentGrid"));		
	}

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;
	
	parrFurnishKey->RemoveAll();

	CString strAcqKey, strAcqCode, strFurnishKey, strQuery, strTmpData;
	CBO_SPECIES api(m_pParentMgr);
	if(isPurchase) api.SetOpenMode(5000);
	else api.SetOpenMode(5100);
	api.InitDataMgr();
 
	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(isPurchase) strTmpData.Format(_T("%d건의 비치희망자료를 구입처리중입니다."), nSelCnt);
	else strTmpData.Format(_T("%d건의 비치희망자료를 기증처리중입니다."), nSelCnt);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CString TITLE, AUTHOR, PUBLISHER, PUBLISH_YEAR, APPLICANT_NAME, PRICE, CURRENCY_CODE;
	CString strSubjectCode, strAutclassNoType;
	
	CString REMARK;
	
	CString ISBN, PRIORITY_YN;

	INT count = 0;
	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		pDM->GetCellData(_T("수서정보KEY"), i, strAcqKey);
		pDM->GetCellData(_T("수입구분"), i, strAcqCode);
		if(!(strAcqKey.IsEmpty()))
		{
			pGrid->SetReverse(i);
			if(IDCANCEL == m_pParentMgr->ESLAfxMessageBox(_T("신청된 자료입니다. 재신청 하시겠습니까?"), MB_OKCANCEL))
			{
				i = pGrid->SelectGetNext();
				if(i < 0) return -1;
				continue;
			}
		}

		pDM->GetCellData(_T("비치희망KEY"), i, strFurnishKey);
		parrFurnishKey->Add(strFurnishKey);

		pDM->GetCellData(_T("본표제"), i, TITLE);
		pDM->GetCellData(_T("저작자"), i, AUTHOR);
		pDM->GetCellData(_T("발행자"), i, PUBLISHER);
		pDM->GetCellData(_T("발행년"), i, PUBLISH_YEAR);
		pDM->GetCellData(_T("신청자"), i, APPLICANT_NAME);
		pDM->GetCellData(_T("가격"), i, PRICE);
		pDM->GetCellData(_T("화폐구분"), i, CURRENCY_CODE);
		pDM->GetCellData(_T("ISBN"), i, ISBN);
		pDM->GetCellData(_T("우선정리자료"), i, PRIORITY_YN);

		CString strAppClass, strAppCode;
		pDM->GetCellData(_T("신청구분"), i, strAppClass);
		pDM->GetCellData(_T("신청코드"), i, strAppCode);
		pDM->GetCellData(_T("취소사유"), i, REMARK);

		pDM->GetCellData(_T("분류표구분"), i, strAutclassNoType);
		pDM->GetCellData(_T("주제구분"), i, strSubjectCode);		
		
		if( _T("Y") == api.m_strUseCoverYN )
			api.m_strSPECIES_KEY.Empty();
		
		CString BOOKCNT;
		pDM->GetCellData(_T("책수"),i,BOOKCNT);

		CString strVolInfo;
		pDM->GetCellData(_T("권정보"), i, strVolInfo);

		ids = api.BO_FURNISH_DATA_PROCESS(TITLE, 
										  AUTHOR, 
										  PUBLISHER, 
										  PUBLISH_YEAR, 
										  APPLICANT_NAME, 
										  PRICE, 
										  CURRENCY_CODE, 
										  ISBN, 
										  PRIORITY_YN, 
										  strFurnishKey, 
										  strAcqKey, 
										  strAcqCode, 
										  pGroupInfo, 
										  isPurchase, 
										  pParentDM, 
										  pParentBookDM, 
										  pMainGrid,
										  strAppClass,
										  strAppCode,
										  REMARK,
										  strAutclassNoType,
										  strSubjectCode
										  ,BOOKCNT
										  ,strVolInfo
										  );

		if(ids < 0) return -2000;
		
		pDM->SetCellData(_T("비치상태"), _T("2"), i);
		pDM->SetCellData(_T("비치상태설명"), _T("처리중"), i);
		
		pDM->SetCellData(_T("비치일자"), _T(""), i);	
		
		CString strUpdateAcqKey;
		strUpdateAcqKey = api.m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"),0);
		if(strUpdateAcqKey.IsEmpty())
		{
			pDM->SetCellData(_T("수서정보KEY"), _T("-1"), i);
		}
		else
		{
			pDM->SetCellData(_T("수서정보KEY"), strUpdateAcqKey, i);
		}
		
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();
	return 0;	
}

INT CBO_ACQ_Business_Api::ProcessRecycleData(INT nAcqType, BOOL bApplyParent)
{
	ACQ_TYPE ATYPE = (ACQ_TYPE)nAcqType;

	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pParentDM = NULL;
	CESL_DataMgr * pParentBookDM = NULL;
	CESL_Grid * pMainGrid = NULL;
	CGroupInfo * pGroupInfo = NULL;
	
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("BOOKDM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroupInfo = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));	
	
	if(bApplyParent)
	{
		pParentDM = (CESL_DataMgr*)GetTempPtr(_T("ParentDM"));
		pParentBookDM = (CESL_DataMgr*)GetTempPtr(_T("ParentBookDM"));
		pMainGrid = (CESL_Grid*)GetTempPtr(_T("ParentGrid"));
	}
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;


	CStringArray arrIdx;		arrIdx.RemoveAll();
	CString strSpeciesKey, strWorkingStatus, strTmpData;
	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(ATYPE == PURCHASE) strTmpData.Format(_T("%d건의 재활용자료를 구입처리중입니다."), nSelCnt);
	else if(ATYPE == DONATE) strTmpData.Format(_T("%d건의 재활용자료를 기증처리중입니다."), nSelCnt);
	else strTmpData.Format(_T("%d건의 재활용자료를 복원중입니다."), nSelCnt);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-6000);
	api.InitDataMgr();

	pDM->StartFrame();
	ids = -1;

	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strSpeciesKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		if(ATYPE == PURCHASE || ATYPE == DONATE)
			ids = api.BO_RECYCLE_DATA_PROCESS(pGroupInfo, strSpeciesKey, (INT)ATYPE, pParentDM, pParentBookDM, pMainGrid);
		else 
			ids = api.BO_RECYCLE_DATA_RESTORE(strSpeciesKey);

		if(ids < 0) return -2000;
		else if(ids == 0)
		{
			arrIdx.Add(strSpeciesKey);
		}
		
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();

	InitTempData();	InitTempPtr();
	SetTempPtr(_T("종DM"), (DWORD*)pDM);
	SetTempPtr(_T("권DM"), (DWORD*)NULL);
	SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	SetTempPtr(_T("삭제데이터"), (DWORD*)&arrIdx);
	SetTempData(_T("비교Alias"), _T("IBS_종KEY"));
	ids = DeleteDMData();
	if(ids < 0) return -4000;

	return 0;	
}

INT CBO_ACQ_Business_Api::DonateData()
{
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	CESL_DataMgr * pDM = NULL;
	CGroupInfo * pGroupInfo = NULL;
	INT nTmpData = 0;
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGroupInfo = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));
	STYPE = (SPECIES_TYPE)nTmpData;

	CString strQuery, strValue;
	strQuery.Format(_T("SELECT MAX(SERIAL_NO) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='2' AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
		pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
	pDM->GetOneValueQuery(strQuery, strValue);
	nTmpData = _ttoi(strValue);
	pGroupInfo->SetLastSerialNo(nTmpData);

	INT ids = -1;
	if(STYPE == BO_BASIC || STYPE == BO_IMPORT)
		ids = DonateAcqObjData(FALSE);
	else if(STYPE == BO_REQUEST)
		ids = ProcessRequestData(FALSE, FALSE);
	else if(STYPE == BO_RECYCLE)
		ids = ProcessRecycleData(DONATE, FALSE);

	return ids;
}

INT CBO_ACQ_Business_Api::DonateAcqObjData(BOOL bApplyParent)
{
	CESL_DataMgr * pMainDM = NULL;
	CESL_DataMgr * pMainBookDM = NULL;
	CESL_Grid * pMainGrid = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CESL_Grid * pGrid = NULL;
	CGroupInfo * pGroupInfo = NULL;
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	INT nTmpData = 0;
	
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	STYPE = (SPECIES_TYPE)nTmpData;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("BOOKDM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroupInfo = (CGroupInfo*)GetTempPtr(_T("GroupInfo"));
	if(bApplyParent)
	{
		pMainDM = (CESL_DataMgr*)GetTempPtr(_T("ParentDM"));
		pMainBookDM = (CESL_DataMgr*)GetTempPtr(_T("ParentBookDM"));
		pMainGrid = (CESL_Grid*)GetTempPtr(_T("ParentGrid"));
	}

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	CStringArray arrIdx;		arrIdx.RemoveAll();
	CString strSpeciesKey, strWorkingStatus, strTmpData;
	if(STYPE == BO_BASIC) strWorkingStatus = _T("BOT111O");
	else if(STYPE == BO_IMPORT) strWorkingStatus = _T("BOT112O");

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(STYPE == BO_BASIC) strTmpData.Format(_T("%d건의 기초자료를 기증처리중입니다."), nSelCnt);
	else if(STYPE == BO_IMPORT) strTmpData.Format(_T("%d건의 반입자료를 기증처리중입니다."), nSelCnt);
	else if(STYPE == SPECIES_TYPE_INVALID) strTmpData.Format(_T("%d건의 기초/반입자료를 기증처리중입니다."), nSelCnt);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-11000);
	api.InitDataMgr();

	pDM->StartFrame();
	ids = -1;

	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strSpeciesKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		ids = api.BO_SPECIES_ACQOBJ_DONATE(pDM, pGroupInfo, strSpeciesKey, strWorkingStatus, pMainDM, pMainBookDM, pMainGrid);
		if(ids < 0) return -2000;
		
		arrIdx.Add(strSpeciesKey);
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();
	pGroupInfo->UpdateLastSerialNo(_T("2"), m_pParentMgr, pDM);

	ids = pDM->SendFrame();
	if(ids < 0) return -3000;
	pDM->EndFrame();

	InitTempData();	InitTempPtr();
	SetTempPtr(_T("종DM"), (DWORD*)pDM);
	SetTempPtr(_T("권DM"), (DWORD*)NULL);
	SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	SetTempPtr(_T("삭제데이터"), (DWORD*)&arrIdx);
	SetTempData(_T("비교Alias"), _T("IBS_종KEY"));
	ids = DeleteDMData();
	if(ids < 0) return -4000;

	return 0;
}

INT CBO_ACQ_Business_Api::RegTransfer(CString strWorkingStatus)
{
	CBO_SPECIES api(m_pParentMgr);
	CESL_DataMgr * pBookDM = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	CString strProcUnit, strSpeciesKeyAlias, strAccuKeyAlias, strVolSortNoAlias;
	strVolSortNoAlias.Empty();
	BOOL bRFIDSetYN = FALSE;
	INT nTmpData;
	GetTempData(_T("RFID_SET_YN"), bRFIDSetYN);
	GetTempData(_T("처리단위"), strProcUnit);
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	GetTempData(_T("ACQ_TYPE"), nTmpData);
	ACQ_TYPE nAcqType = (ACQ_TYPE)nTmpData;

	if(strProcUnit == _T("권"))
	{
		GetTempData(_T("권일련번호알리아스"), strVolSortNoAlias);
	}
	else if(strProcUnit == _T("그룹단위"))
	{
		return RegTransferByGroup();
	}
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	CStringArray arrIdx;		arrIdx.RemoveAll();

	CString strQuery, strSpeciesKey, strAccuKey, strVolSortNo, strTmpData;
	strVolSortNo.Empty();

	INT nPos = 1;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정하신 자료를 인계처리 하고 있습니다."));
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());

	pDM->StartFrame();

	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		if(!strVolSortNoAlias.IsEmpty())
			pDM->GetCellData(strVolSortNoAlias, i, strVolSortNo);

		if(strWorkingStatus == _T("BOR111N"))
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS='%s', REG_TRANSFER_DATE='%s' WHERE SPECIES_KEY=%s AND ACQ_KEY=%s"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strWorkingStatus, GetTodayDate(), strSpeciesKey, strAccuKey);
		else 
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS='%s', CAT_TRANSFER_DATE='%s', REG_TRANSFER_DATE='%s' WHERE SPECIES_KEY=%s AND ACQ_KEY=%s"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strWorkingStatus, GetTodayDate(), GetTodayDate(), strSpeciesKey, strAccuKey);

		if(!strVolSortNo.IsEmpty())
			strQuery += _T(" AND VOL_SORT_NO = ") + strVolSortNo; 
		if(nAcqType == PURCHASE)
			strQuery += _T(" AND WORKING_STATUS = 'BOA114O';");
		else if(nAcqType == DONATE)
			strQuery += _T(" AND WORKING_STATUS = 'BOA211O';");
		
		pDM->AddFrame(strQuery);
		strQuery.Format(
			_T("update idx_bo_tbl set working_status='%s', LAST_WORK='%s' where WORKING_STATUS<'%s' AND WORKING_STATUS<'BOC' and rec_key=%s;"),
			strWorkingStatus, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strWorkingStatus, strSpeciesKey);
		pDM->AddFrame(strQuery);

		strTmpData.Format(_T("%d"), i);
		arrIdx.Add(strTmpData);
		i = pGrid->SelectGetNext();
	}
	
	m_pDataProcessingDlg->SetMessage(_T("선정자료를 인계처리 중입니다."));
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();

	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	DeleteSelectRow(pDM, &arrIdx, _T(""));
	
	return 0;	
}

CString CBO_ACQ_Business_Api::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CBO_ACQ_Business_Api::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CBO_ACQ_Business_Api::OrderData()
{
	CString strGroupNo, strAcqYear, strQuery, strWorkCode;
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹정보"));
	GetTempData(_T("수입년도"), strAcqYear);
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGroupCollection->GetGroupInfo(strGroupNo);
	strWorkCode = pGroupCollection->GetWorkCode();

	pDM->StartFrame();

	CString strBookKeyList;
	CString strValue;
	strBookKeyList = _T("");

	INT nTotalRowCnt = pDM->GetRowCount();
	INT nBookKeyCntFlag = 0;
	INT i;

	for ( i= 0 ; i<pDM->GetRowCount() ; i++ )
	{
		strValue  = pDM->GetCellData(_T("선정"), i ); 
		if ( strValue == _T("V") )
		{
			strValue  = pDM->GetCellData(_T("IBS_종KEY"), i ); 
			if ( !strBookKeyList.IsEmpty() ) strBookKeyList += _T(",");
			strBookKeyList += _T("'") + strValue + _T("'"); 
		}
	}
	
	if ( strBookKeyList.IsEmpty() ) return -1;

	ConvertRecKeyToSubQuery(strBookKeyList, _T("BO_BOOK_TBL"), _T("SPECIES_KEY"));

	if(strWorkCode.IsEmpty())
		strQuery.Format(
		_T("UPDATE BO_BOOK_TBL SET LAST_WORK = '%s', WORKING_STATUS = 'BOA112N', PREV_WORKING_STATUS = 'BOA111N' WHERE ACQ_CODE='1' AND ACQ_YEAR = '%s' AND SEQ_NO IN (%s) AND WORKING_STATUS = 'BOA111N' AND SPECIES_KEY IN (%s);"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strGroupNo, strBookKeyList );
	else
		strQuery.Format(
		_T("UPDATE BO_BOOK_TBL SET LAST_WORK = '%s', WORKING_STATUS = 'BOA112N', PREV_WORKING_STATUS = 'BOA111N' WHERE ACQ_CODE='1' AND ACQ_YEAR = '%s' AND SEQ_NO IN (%s) AND WORKING_STATUS = 'BOA111N' AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE (SPECIES_CLASS='0' OR SPECIES_CLASS='1') AND WORK_CODE='%s' AND REC_KEY IN (%s) );"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strGroupNo, strWorkCode, strBookKeyList );
	pDM->AddFrame(strQuery);

	if(strWorkCode.IsEmpty())
		strQuery.Format(
		_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK = '%s', ORDER_DATE = '%s' WHERE ORDER_YEAR = '%s' AND ORDER_SEQ_NO IN (%s) AND SPECIES_KEY IN (%s);"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate(), strAcqYear, strGroupNo, strBookKeyList);
	else 
		strQuery.Format(
		_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK = '%s', ORDER_DATE = '%s' WHERE ORDER_YEAR = '%s' AND ORDER_SEQ_NO IN (%s) AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE (SPECIES_CLASS='0' OR SPECIES_CLASS='1') AND WORK_CODE='%s' AND REC_KEY IN (%s));"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate(), strAcqYear, strGroupNo, strWorkCode, strBookKeyList);
	pDM->AddFrame(strQuery);

	if(strWorkCode.IsEmpty())
		strQuery.Format(
		_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK = '%s', WORKING_STATUS = '1', ORDER_DATE = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO IN (%s) AND WORK_CODE IS NULL;"),

		m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate(), strAcqYear, strGroupNo);
	else
		strQuery.Format(
		_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK = '%s', WORKING_STATUS = '1', ORDER_DATE = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO IN (%s) AND WORK_CODE='%s';"),

		m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate(), strAcqYear, strGroupNo, strWorkCode);
	pDM->AddFrame(strQuery);

	if(strWorkCode.IsEmpty())
		strQuery.Format(
		_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOA112N', LAST_WORK='%s' ")
		_T("WHERE WORKING_STATUS<'BOA112N' AND ")
		_T("REC_KEY IN ")
		_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO IN (%s)) AND REC_KEY IN (%s);"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strGroupNo, strBookKeyList);
	else
		strQuery.Format(
		_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOA112N', LAST_WORK='%s' ")
		_T("WHERE WORKING_STATUS<'BOA112N' AND WORK_CODE='%s' AND ")
		_T("REC_KEY IN ")
		_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO IN (%s)) AND REC_KEY IN (%s);"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), strWorkCode, strAcqYear, strGroupNo, strBookKeyList);
	pDM->AddFrame(strQuery);

	INT ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	return 0;
}

INT CBO_ACQ_Business_Api::ReviewData()
{
	CString strGroupNo, strAcqYear, strWorkCode;
	GetTempData(_T("그룹번호"), strGroupNo);
	GetTempData(_T("수입년도"), strAcqYear);
	GetTempData(_T("업무구분"), strWorkCode);
	CBO_SPECIES api(m_pParentMgr);
	CString strQuery, strDMPK;
	CESL_DataMgr * pDM = NULL;
	pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	pDM->InitDMPKFromFile(_T("DM_중복본조사키")); 
	pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	pDM->StartFrame();

	strQuery.Format(
		_T("UPDATE BO_PURCHASEINFO_TBL SET PURCHASE_CHECK_YN = '%s', ORDER_CHECK_DATE = '%s' WHERE ORDER_YEAR = '%s' AND ORDER_SEQ_NO IN (%s) AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SPECIES_CLASS='0' AND WORK_CODE='%s');"),

		_T("Y"), GetTodayDate(), strAcqYear, strGroupNo, strWorkCode);
	pDM->AddFrame(strQuery);

	strQuery.Format(
		_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET REVIEW_CHECK_YN = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO IN (%s) AND WORK_CODE='%s';"),

		_T("Y"), strAcqYear, strGroupNo, strWorkCode);
	pDM->AddFrame(strQuery);


	INT ids = pDM->SendFrame();
	if(ids < 0)
	{
		if(pDM != NULL)
		{
			delete pDM;
			pDM = NULL;
		}
		return -1;
	}
	ids = pDM->EndFrame();
	if(ids < 0)
	{
		if(pDM != NULL)
		{
			delete pDM;
			pDM = NULL;
		}
		return -1;
	}

	if(pDM != NULL)
	{
		delete pDM;
		pDM = NULL;
	}
	
	return 0;
	
}

INT CBO_ACQ_Business_Api::CheckInOrderData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	CESL_DataMgr * pSourceDM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("그리드"));
	INT nGridCol = -1;
	BOOL isStatusChange = FALSE;
	GetTempData(_T("그리드컬럼번호"), nGridCol);
	INT nRowCnt = pDM->GetRowCount();
	GetTempData(_T("상태변경여부"), isStatusChange);
	CString strWorkingStatus;
	GetTempData(_T("자료상태"), strWorkingStatus);
	BOOL bRFIDSetYN = FALSE;
	GetTempData(_T("RFID_SET_YN"), bRFIDSetYN);

	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BP_수서정보KEY");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	pSourceDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	pSourceDM->StartFrame();

	CString strAcqKey, strVolSortNo, strPrevAcqKey, strDiscRate, strTmpData;
	INT nStartIdx, nEndIdx, nDBookCnt;

	INT nPos = 1;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정하신 자료를 검수처리 하고 있습니다."));
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	nStartIdx = nEndIdx = nDBookCnt = 0;
	strPrevAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), 0);
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		pGrid->GetAt(i, nGridCol, strTmpData);
		nDBookCnt = _ttoi(strTmpData.GetBuffer(0));
		strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		if(strPrevAcqKey != strAcqKey)
		{	
			strDiscRate = pDM->GetCellData(_T("BP_할인율"), i-1);

			if ( strDiscRate == _T("") ) strDiscRate = _T("0");
			
			strTmpData.Format(
				_T("UPDATE BO_PURCHASEINFO_TBL SET DELIVERY_BOOK_CNT=GetAcqBookCount(%s, 'Y'), MISSING_BOOK_CNT=GetAcqBookCount(%s, 'N'), DELIVERY_PRICE=GetSpeciesPrice(%s, %s, 'Y') WHERE REC_KEY=%s;"), 
				strPrevAcqKey, strPrevAcqKey, strPrevAcqKey, strDiscRate, strPrevAcqKey);
			pSourceDM->AddFrame(strTmpData);
			strPrevAcqKey = strAcqKey;
		}
			
		nEndIdx = FindDMIndex(_T("BP_수서정보KEY"), strAcqKey, nStartIdx, pSourceDM);

		pDM->GetCellData(_T("BB_권일련번호"), i, strVolSortNo);
		CheckDeliveryBook(nStartIdx, nEndIdx, nDBookCnt, strVolSortNo, pSourceDM, strWorkingStatus);
		nStartIdx = nEndIdx = 0;
		pGrid->GetAt(i, nGridCol, strTmpData);
		pDM->SetCellData(_T("UDF_납품책수"), strTmpData, i); 
		pGrid->GetAt(i, nGridCol+1, strTmpData);
		pDM->SetCellData(_T("UDF_미납책수"), strTmpData, i); 
	}
	
	strDiscRate = pDM->GetCellData(_T("BP_할인율"), i-1);
	strTmpData.Format(
		_T("UPDATE BO_PURCHASEINFO_TBL SET DELIVERY_BOOK_CNT=GetAcqBookCount(%s, 'Y'), MISSING_BOOK_CNT=GetAcqBookCount(%s, 'N'), DELIVERY_PRICE=GetSpeciesPrice(%s, %s, 'Y') WHERE REC_KEY=%s;"), 
		strPrevAcqKey, strPrevAcqKey, strPrevAcqKey, strDiscRate, strPrevAcqKey);
	pSourceDM->AddFrame(strTmpData);

	m_pDataProcessingDlg->SetMessage(_T("검수처리 중입니다."));

	CBO_SPECIES api(m_pParentMgr);

	INT ids = pSourceDM->SendFrame();
	
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = pSourceDM->EndFrame();
	if(ids < 0) return -1;

	if(isStatusChange)
	{
		nRowCnt = pSourceDM->GetRowCount();
		for(i=nRowCnt-1 ; i>=0 ; i--)
		{
			strTmpData = pSourceDM->GetCellData(_T("BB_납품여부"), i);
			if(strTmpData == _T("Y"))
				pSourceDM->DeleteRow(i);
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::FindDMIndex(CString strAlias, CString strValue, INT &nStartIdx, CESL_DataMgr * pDM)
{
	BOOL isSame = FALSE;
	INT rowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(strAlias, i);
		if(strValue == strTmpData && !isSame)
		{
			nStartIdx = i;
			isSame = TRUE;
		}
		else if(strValue != strTmpData && isSame)
		{
			break;
		}
	}
	
	return i;
}

INT CBO_ACQ_Business_Api::CheckDeliveryBook(INT nStartIdx, INT nEndIdx, INT nDBookCnt, CString strVolSortNo, CESL_DataMgr * pSourceDM, CString strWorkingStatus)
{
	INT cnt = 0;
	CString strTmpData, strQuery;
	
	for(INT i=nStartIdx ; i< nEndIdx ; i++)
	{
		strTmpData = pSourceDM->GetCellData(_T("BB_권일련번호"), i);
		if(cnt >= nDBookCnt)
		{
			if(strTmpData == strVolSortNo)
			{	
				strTmpData = pSourceDM->GetCellData(_T("BB_납품여부"), i);
				pSourceDM->SetCellData(_T("BB_납품여부"), _T("N"), i);
				strTmpData = pSourceDM->GetCellData(_T("BB_책KEY"), i);
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET CHECKIN_DATE=NULL, DELIVERY_YN='N' WHERE REC_KEY=%s;"), strTmpData);
				pSourceDM->AddFrame(strQuery);
				cnt++;
			}
		}
		else if(cnt < nDBookCnt)
		{
			if(strTmpData == strVolSortNo)
			{	
				strTmpData = pSourceDM->GetCellData(_T("BB_납품여부"), i);
				if(strTmpData != _T("Y"))
				{
					pSourceDM->SetCellData(_T("BB_납품여부"), _T("Y"), i);
				}
				strTmpData = pSourceDM->GetCellData(_T("BB_책KEY"), i);
				if(strWorkingStatus == _T("BOA112N") || strWorkingStatus == _T("BOA114O"))
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='%s', CHECKIN_DATE='%s', DELIVERY_YN='Y' WHERE REC_KEY=%s;"), 
					strWorkingStatus, GetTodayDate(), strTmpData);
				else if(strWorkingStatus == _T("BOR111N"))
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='%s', REG_TRANSFER_DATE='%s', CHECKIN_DATE='%s', DELIVERY_YN='Y' WHERE REC_KEY=%s;"), 
					strWorkingStatus, GetTodayDate(), GetTodayDate(), strTmpData);
				else if(strWorkingStatus == _T("BOC111N") || strWorkingStatus == _T("BOC112O") || strWorkingStatus == _T("BOC113N"))
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='%s', CAT_TRANSFER_DATE='%s', REG_TRANSFER_DATE='%s', CHECKIN_DATE='%s', DELIVERY_YN='Y' WHERE REC_KEY=%s;"), 
					strWorkingStatus, GetTodayDate(), GetTodayDate(), GetTodayDate(), strTmpData);
				pSourceDM->AddFrame(strQuery);
				cnt++;
			}			
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::CheckInComplete(CString strWorkingStatus)
{
	BOOL bRFIDSetYN = FALSE;
	CBO_SPECIES m_cApi(m_pParentMgr);
	CESL_DataMgr * pDM_BOOK = NULL;
	CGroupCollection * pGroupCollection = NULL;
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹정보"));
	pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	GetTempData(_T("RFID_SET_YN"), bRFIDSetYN);

	CString strBookKeyList;
	CString strValue;
	strBookKeyList = _T("");
	for ( int i= 0 ; i<pDM_BOOK->GetRowCount() ; i++ )
	{
		strValue  = pDM_BOOK->GetCellData(_T("선정"), i ); 
		if ( strValue == _T("V") )
		{
			strValue  = pDM_BOOK->GetCellData(_T("BB_책KEY"), i ); 
			if ( !strBookKeyList.IsEmpty() ) strBookKeyList += _T(",");
			strBookKeyList += _T("'") + strValue + _T("'"); 
		}
	}
	
	if ( strBookKeyList.IsEmpty() ) return -1;
	ConvertRecKeyToSubQuery(strBookKeyList, _T("BO_BOOK_TBL"), _T("REC_KEY"));

	CString strQuery, strTmpData, strGroupInfo, strWorkCode;
	pGroupCollection->GetGroupInfo(strGroupInfo);
	strWorkCode = pGroupCollection->GetWorkCode();
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND B.REC_KEY IN (%s)"),
			pGroupCollection->GetAcqYear(), strGroupInfo, strBookKeyList);
	else
		strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE='%s' AND B.REC_KEY IN (%s)"),
			pGroupCollection->GetAcqYear(), strGroupInfo, strWorkCode, strBookKeyList);
	pDM_BOOK->RefreshDataManager(strQuery);
	INT nRowCnt = pDM_BOOK->GetRowCount(); 
	if(nRowCnt == 0) return -1;

	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_그룹번호");						nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");						nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM_BOOK->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	pDM_BOOK->StartFrame();
	
	INT nSerialNo = 1;
	CString strSpeciesKey, strSerialNo, strPrevGroupNo, strGroupNo, strMsg;

	INT nPos = 1;
	ShowDataProcessingDlg();
	strMsg.Format(_T("%d건의 자료를 검수완료 처리하고 있습니다."), nRowCnt);
	strTmpData.Format(strMsg);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	for( i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg->GetCancel())
		{
			pDM_BOOK->FreeFrame();
			return 0;
		}
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		strGroupNo = pDM_BOOK->GetCellData(_T("BP_그룹번호"), i);
		if(strPrevGroupNo != strGroupNo)
		{
			nSerialNo = 1;
			strPrevGroupNo = strGroupNo;
		}
		
		strTmpData = pDM_BOOK->GetCellData(_T("BB_납품여부"), i);
		strSpeciesKey = pDM_BOOK->GetCellData(_T("IBS_종KEY"), i);
		if(strTmpData == _T("Y"))
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
			strSerialNo.Format(_T("%d"), nSerialNo++);
			if(strWorkingStatus == _T("BOA114O"))
				strQuery.Format(
					_T("UPDATE BO_BOOK_TBL SET CHECKIN_DATE='%s', WORKING_STATUS='%s', SERIAL_NO=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
					GetTodayDate(), strWorkingStatus, strSerialNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
			else if(strWorkingStatus == _T("BOR111N"))
				strQuery.Format(
					_T("UPDATE BO_BOOK_TBL SET CHECKIN_DATE='%s',REG_TRANSFER_DATE='%s', WORKING_STATUS='%s', SERIAL_NO=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
					GetTodayDate(), GetTodayDate(), strWorkingStatus, strSerialNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);

			pDM_BOOK->AddFrame(strQuery);
			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='%s', LAST_WORK='%s' ")
				_T("WHERE WORKING_STATUS<'%s' AND WORKING_STATUS<'BOC' AND ")
				_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY=%s);"),
				strWorkingStatus, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strWorkingStatus, strTmpData);
			pDM_BOOK->AddFrame(strQuery);
			strQuery.Format(
				_T("UPDATE BO_PURCHASEINFO_TBL SET CHECKIN_DATE='%s', LAST_WORK='%s' WHERE REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE REC_KEY=%s);"),
				GetTodayDate(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
			pDM_BOOK->AddFrame(strQuery);
		}
		else
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
			strSerialNo.Format(_T("%d"), nSerialNo++);
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOA113O', SERIAL_NO=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strSerialNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
			pDM_BOOK->AddFrame(strQuery);
			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOA113O', LAST_WORK='%s' ")
				_T("WHERE WORKING_STATUS<'BOA113O' AND ")
				_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY=%s);"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
			pDM_BOOK->AddFrame(strQuery);
		}	
	}

	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET CHECKIN_COMPLETE_DATE='%s', LAST_WORK='%s', WORKING_STATUS='2' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO IN (%s);"),
			GetTodayDate(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroupCollection->GetAcqYear(), strGroupInfo);
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET CHECKIN_COMPLETE_DATE='%s', LAST_WORK='%s', WORKING_STATUS='2' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO IN (%s) AND WORK_CODE='%s';"),
			GetTodayDate(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroupCollection->GetAcqYear(), strGroupInfo, strWorkCode);
	pDM_BOOK->AddFrame(strQuery);

	m_pDataProcessingDlg->SetMessage(_T("검수완료 처리 중입니다."));

	INT ids = pDM_BOOK->SendFrame();

	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = pDM_BOOK->EndFrame();
	if(ids < 0) return -1;
	return 0;
}

INT CBO_ACQ_Business_Api::GiveOrderSerialNo()
{
	
 	CString strGroupNo, strAcqYear, strWorkCode;
	CESL_DataMgr * pDM_ACCU = (CESL_DataMgr*)GetTempPtr(_T("SpeciesDM"));
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BookDM"));

	INT ids;
	INT nErrCnt = 0;

	pDM_BOOK->SORT(_T("BP_수서정보KEY"));

	INT nRowCnt = pDM_ACCU->GetRowCount();
	INT nNum = 1;
	CString strNum, strDivNo, strTmpData, strValue, strQuery;


	pDM_ACCU->StartFrame();

	BOOL bFindYN = FALSE;
	INT nPos = 1;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("%d건의 자료에 대하여 주문일련번호를 부여합니다."), nRowCnt);
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	CMapStringToString mapDivnoVSOrderSerialNo;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if ( pDM_ACCU->GetCellData ( _T("BP_선정비선정"), i ) != _T("Y") ) continue;

		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		strNum.Empty();
		strTmpData = pDM_ACCU->GetCellData(_T("BP_그룹번호"), i);
		if(strTmpData.IsEmpty()) 
		{
			nErrCnt++;
			continue;
		}

		bFindYN = mapDivnoVSOrderSerialNo.Lookup(strTmpData, strValue);
		if(bFindYN) 
		{
			nNum = _ttoi(strValue.GetBuffer(0)) + 1;
			strValue.Format(_T("%d"), nNum);
			mapDivnoVSOrderSerialNo.SetAt(strTmpData, strValue);
		}
		else 
		{
			strValue.Format(_T("%d"), 1);
			mapDivnoVSOrderSerialNo.SetAt(strTmpData, strValue);
			nNum = 1;
		}
		
		strTmpData = pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		strNum.Format(_T("%d"), nNum);
		pDM_ACCU->SetCellData(_T("BP_발주일련번호"), strNum, i);

		ids = FindIndex(pDM_BOOK, _T("BP_수서정보KEY"), strTmpData, 0);
		while(ids >= 0)
		{
			pDM_BOOK->SetCellData(_T("BP_발주일련번호"), strNum, ids);
			ids = FindIndex(pDM_BOOK, _T("BP_수서정보KEY"), strTmpData, ids+1);
		}

		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK = '%s', SERIAL_NO = %s WHERE REC_KEY = %s;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strNum, strTmpData);
		pDM_ACCU->AddFrame(strQuery);
		
		if(strNum.IsEmpty())
		{
			strNum = pDM_ACCU->GetCellData(_T("BP_일련번호"), i);
			if(strNum.IsEmpty()) strNum = _T("NULL");
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET SERIAL_NO=%s, LAST_WORK='%s' WHERE ACQ_KEY=%s;"),
				strNum, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
		}
		else
		{
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET SERIAL_NO=%s, LAST_WORK='%s' WHERE ACQ_KEY=%s;"),
				strNum, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strTmpData);
		}
		pDM_ACCU->AddFrame(strQuery);

	}
	CBO_SPECIES api(m_pParentMgr);
	ids = pDM_ACCU->SendFrame();
	DeleteDataProcessingDlg();
	if(ids <0) return -1;
	ids = pDM_ACCU->EndFrame();
	if(ids < 0) return -1;

	return nErrCnt;
}

INT CBO_ACQ_Business_Api::CheckValidationOrderProcess(CESL_DataMgr * pDM, CGroupCollection *pGroupCollection)
{
	CGroupInfo * pGroup = NULL;
	INT nGroupCnt = pGroupCollection->GetGroupCount();
	CGroupCollection DeleteGroupCollection(m_pParentMgr);

	CString strMsg, strHoldDupSearchYN, strWorkDupSearchYN, strReviewYN, strNextProcess;
	m_pParentMgr->GetManageValue(_T("수서"), _T("단행"), _T("복본조사수행여부"), _T(""), strHoldDupSearchYN);
	m_pParentMgr->GetManageValue(_T("수서"), _T("단행"), _T("중복조사수행여부"), _T(""), strWorkDupSearchYN);
	m_pParentMgr->GetManageValue(_T("수서"), _T("단행"), _T("심의처리수행여부"), _T(""), strReviewYN);
	
	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		pGroup = pGroupCollection->GetGroupInfo(i);
		if(!pGroup->CheckValidationOrderProcess(strHoldDupSearchYN, strWorkDupSearchYN, strReviewYN, strNextProcess, pDM))
		{
			strMsg.Format(
				_T("차수정보 : %s-%s는 주문작업을 수행하기 위하여 %s 작업을 수행하여야 합니다."),
				pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strNextProcess);
			m_pParentMgr->ESLAfxMessageBox(strMsg);
			CGroupInfo pTmpGroup;
			pGroup->CopyData(&pTmpGroup);
			DeleteGroupCollection.AddGroupInfo(&pTmpGroup);
		}
	}
	pGroupCollection->DeleteThisGroupInfo(&DeleteGroupCollection);
	return 0;
}

BOOL CBO_ACQ_Business_Api::CheckValidationOrderProcess(CESL_DataMgr * pDM, CGroupInfo *pGroup)
{
	if(!pGroup) return FALSE;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(
		_T("SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND (PROVIDER_CODE IS NULL OR ORDER_PRICE IS NULL OR ORDER_PRICE=0 OR CURRENCY_CODE IS NULL) REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s)"),
		pGroup->GetAcqYear(), pGroup->GetGroupInfo(), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 0) return FALSE;
	return TRUE;
}

INT CBO_ACQ_Business_Api::CheckGiveOrderSerialNo()
{
	CArray<INT, INT> arrIdx;
	arrIdx.RemoveAll();
	CGroupCollection * pGroupCollection = NULL;
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹정보"));
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CGroupInfo * pGroup;
	CString strMsg;
	INT cnt = pGroupCollection->GetGroupCount();
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = pGroupCollection->GetGroupInfo(i);
		if(!CheckGiveOrderSerialNo(pDM, pGroup))
		{
			strMsg.Format(
				_T("차수정보 : %s-%s의 자료는 주문일련번호가 부여되어있지 않습니다.\n해당 차수는 주문할 수 없습니다."),
				pGroup->GetAcqYear(), pGroup->GetGroupInfo());
			m_pParentMgr->ESLAfxMessageBox(strMsg);
			arrIdx.Add(i);
		}
	}

	cnt = arrIdx.GetSize();
	for( i=cnt-1 ; i>=0 ; i--)
		pGroupCollection->RemoveAt(arrIdx.GetAt(i));

	return 0;
}

BOOL CBO_ACQ_Business_Api::CheckGiveOrderSerialNo(CESL_DataMgr * pDM, CGroupInfo *pGroup)
{
	if(!pGroup) return FALSE;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString strQuery, strWorkCode;
	strWorkCode = pGroup->GetWorkCode();
	if(strWorkCode.IsEmpty())
		strQuery.Format(			
			_T("SELECT P.REC_KEY FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B WHERE I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND (P.SERIAL_NO IS NULL OR P.SERIAL_NO = 0) AND I.WORK_CODE IS NULL AND B.WORKING_STATUS = 'BOA111N'"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());			
	else
		strQuery.Format(
			_T("SELECT P.REC_KEY FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B WHERE I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND (P.SERIAL_NO IS NULL OR P.SERIAL_NO = 0) AND I.WORK_CODE = '%s' AND B.WORKING_STATUS = 'BOA111N'"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);			

	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 0) return FALSE;
	return TRUE;
}

INT CBO_ACQ_Business_Api::DeleteDataByDM()
{
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("DM_BOOK"));
	CESL_DataMgr * pDM_SPECIES = (CESL_DataMgr*)GetTempPtr(_T("DM_SPECIES"));
	CESL_DataMgr * pDM_VOLUME = (CESL_DataMgr*)GetTempPtr(_T("DM_VOLUME"));
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GROUPINFO"));
	CString strAcqYear, strGroupInfo, strWorkCode;
	INT nRowCnt = -1;
	INT i = 0;
	if(pDM_BOOK != NULL)
	{
		nRowCnt = pDM_BOOK->GetRowCount();
		CString strAcqYear, strGroupInfo;
		for(i=nRowCnt-1 ; i>=0; i--)
		{
			strAcqYear = pDM_BOOK->GetCellData(_T("BP_수입년도"), i);
			strGroupInfo = pDM_BOOK->GetCellData(_T("BP_그룹번호"), i);
			strWorkCode = pDM_BOOK->GetCellData(_T("IBS_업무구분"), i);
			if(pGroupCollection->IsIncludeGroupInfo(strAcqYear, strGroupInfo, strWorkCode))
				pDM_BOOK->DeleteRow(i);
		}
	}
	if(pDM_SPECIES != NULL)
	{
		nRowCnt = pDM_SPECIES->GetRowCount();
		for(i=nRowCnt-1 ; i>=0; i--)
		{
			strAcqYear = pDM_SPECIES->GetCellData(_T("BP_수입년도"), i);
			strGroupInfo = pDM_SPECIES->GetCellData(_T("BP_그룹번호"), i);
			strWorkCode = pDM_SPECIES->GetCellData(_T("IBS_업무구분"), i);
			if(pGroupCollection->IsIncludeGroupInfo(strAcqYear, strGroupInfo, strWorkCode))
				pDM_SPECIES->DeleteRow(i);
		}
	}
	if(pDM_VOLUME != NULL)
	{
		nRowCnt = pDM_VOLUME->GetRowCount();
		for(i=nRowCnt-1 ; i>=0; i--)
		{
			strAcqYear = pDM_VOLUME->GetCellData(_T("BP_수입년도"), i);
			strGroupInfo = pDM_VOLUME->GetCellData(_T("BP_그룹번호"), i);
			strWorkCode = pDM_VOLUME->GetCellData(_T("IBS_업무구분"), i);
			if(pGroupCollection->IsIncludeGroupInfo(strAcqYear, strGroupInfo, strWorkCode))
				pDM_VOLUME->DeleteRow(i);
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::RegTransferByGroup()
{
	return 0;	
}

INT CBO_ACQ_Business_Api::SetBookCountOnGrid()
{
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	CString strGetCol, strSetCol, strClearCol;
	INT nGetColIdx, nClearColIdx, nSetColIdx;
	GetTempData(_T("GET_COLUMN"), strGetCol);
	GetTempData(_T("SET_COLUMN"), strSetCol);
	GetTempData(_T("CLEAR_COLUMN"), strClearCol);

	nGetColIdx = GetGridColumnIdx(pGrid, strGetCol);
	nSetColIdx = GetGridColumnIdx(pGrid, strSetCol);
	nClearColIdx = GetGridColumnIdx(pGrid, strClearCol);

	if(nGetColIdx < 0 || nSetColIdx < 0 || nClearColIdx < 0)
		return -1;
	
	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	CString strData;
	while(i >= 0)
	{
		pGrid->GetAt(i, nGetColIdx, strData);
		pGrid->SetAt(i, nClearColIdx, _T("0"));
		pGrid->SetAt(i, nSetColIdx, strData);
		i = pGrid->SelectGetNext();
	}
	return 0;
}

INT CBO_ACQ_Business_Api::PurchaseDataApplyParent()
{
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	INT nTmpData = 0;
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	STYPE = (SPECIES_TYPE)nTmpData;
	
	INT ids = -1;
	if(STYPE == BO_BASIC || STYPE == BO_IMPORT || STYPE == SPECIES_TYPE_INVALID)
		ids = PurchaseAcqObjData(TRUE);
	else if(STYPE == BO_REQUEST)
		ids = ProcessRequestData(TRUE, TRUE);
	else if(STYPE == BO_RECYCLE)
		ids = ProcessRecycleData(PURCHASE, TRUE);

	return ids;
}

INT CBO_ACQ_Business_Api::DonateDataApplyParent()
{
	SPECIES_TYPE STYPE = SPECIES_TYPE_INVALID;
	INT nTmpData = 0;
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	STYPE = (SPECIES_TYPE)nTmpData;
	
	INT ids = -1;
	if(STYPE == BO_BASIC || STYPE == BO_IMPORT || STYPE == SPECIES_TYPE_INVALID)
		ids = DonateAcqObjData(TRUE);
	else if(STYPE == BO_REQUEST)
		ids = ProcessRequestData(FALSE, TRUE);
	else if(STYPE == BO_RECYCLE)
		ids = ProcessRecycleData(DONATE, TRUE);

	return ids;
}

BOOL CBO_ACQ_Business_Api::CheckValidationOrderProcess(CESL_DataMgr *pDM_ACCU, INT nIdx)
{
	CString strProvider, strCurrencyCode, strPrice;
	strPrice = pDM_ACCU->GetCellData(_T("UDF_종가격"), nIdx);
	strCurrencyCode = pDM_ACCU->GetCellData(_T("BP_화폐구분"), nIdx);
	if(strPrice.IsEmpty() || strPrice == _T("0")) return FALSE;
	if(strCurrencyCode.IsEmpty()) return FALSE;
	return TRUE;
}

INT CBO_ACQ_Business_Api::SelDataOrderCancel()
{
	BOOL bRFIDUseYN = FALSE;
	CString strBookCntAlias;
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBOOK_DM = NULL;
	CESL_Grid * pGrid = NULL;
	CGroupInfo * pGroup = NULL;
	CStringArray * pAcqKeyArray = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pBOOK_DM = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("GROUP"));
	pAcqKeyArray = (CStringArray*)GetTempPtr(_T("ACQKEYLIST"));
	GetTempData(_T("RFID_USE_YN"), bRFIDUseYN);
	GetTempData(_T("BOOK_CNT_ALIAS"), strBookCntAlias);

	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	INT nRowCnt = pGrid->SelectGetCount();
	
	CStringArray arrIdx;
	arrIdx.RemoveAll();
	CString strTmpData, strAcqKey, strQuery, strSpeciesKey, strBookCnt;
	CString strTitle, strAuthor, strPublisher, strMsg;
	INT nLastSerialNo;
	if(pGroup)
	{
		strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s)"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		pDM->GetOneValueQuery(strQuery, strTmpData);
		nLastSerialNo = _ttoi(strTmpData.GetBuffer(0));
	}

	INT nPos = 1;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정하신 자료를 주문취소 처리하고 있습니다."));
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	pDM->StartFrame();

	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		strBookCnt = pDM->GetCellData(strBookCntAlias, i);
		
		CheckBookDeleteField(pBOOK_DM, strAcqKey, _T(""), _ttoi(strBookCnt.GetBuffer(0)));

		pAcqKeyArray->Add(strAcqKey);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		if(pGroup)
		{
			strQuery.Format(
				_T("UPDATE BO_PURCHASEINFO_TBL ")
				_T("SET DELIVERY_PRICE=ORDER_PRICE, DELIVERY_BOOK_CNT=ORDER_BOOK_CNT, ORDER_YEAR='%s', ")
				_T("ORDER_SEQ_NO=%s, ORDER_WORK_NO=%s, ORDER_SERIAL_NO=%s, SERIAL_NO=NULL, LAST_WORK='%s', ")
				_T("ORDER_DATE=NULL WHERE REC_KEY = %s;"),
				pGroup->GetAcqYear(), pGroup->GetGroupInfo(), pGroup->GetWorkNo(), strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey);
			pDM->AddFrame(strQuery);

			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL ")
				_T("SET DELIVERY_YN='Y', SEQ_NO=%s, ACQ_YEAR='%s', LAST_WORK='%s', WORKING_STATUS='BOA111N', ")
				_T("PREV_WORKING_STATUS='BOA112N' WHERE ACQ_KEY=%s;"),
				pGroup->GetGroupInfo(), pGroup->GetAcqYear(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey);
			pDM->AddFrame(strQuery);

			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOA111N', WORK_CODE='%s',  LAST_WORK='%s' WHERE REC_KEY=%s;"), 
				pGroup->GetWorkCode(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
			pDM->AddFrame(strQuery);
		
			strQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET INPUT_DATE='%s', WORK_CODE='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				GetTodayDate(), pGroup->GetWorkCode(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}
		else
		{
			strQuery.Format(
				_T("UPDATE BO_PURCHASEINFO_TBL ")
				_T("SET DELIVERY_PRICE=ORDER_PRICE, DELIVERY_BOOK_CNT=ORDER_BOOK_CNT, ")
				_T("ORDER_SEQ_NO=NULL, ORDER_WORK_NO=NULL, ORDER_SERIAL_NO=NULL, SERIAL_NO=NULL, LAST_WORK='%s', ")
				_T("ORDER_DATE=NULL WHERE REC_KEY = %s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey);
			pDM->AddFrame(strQuery);

			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL ")
				_T("SET DELIVERY_YN='Y', SEQ_NO=NULL, LAST_WORK='%s', WORKING_STATUS='BOA111N', ")
				_T("PREV_WORKING_STATUS='BOA112N' WHERE ACQ_KEY=%s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey);
			pDM->AddFrame(strQuery);

			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOA111N', LAST_WORK='%s' WHERE REC_KEY=%s;"), 
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
			pDM->AddFrame(strQuery);
		
			strQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET INPUT_DATE='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				GetTodayDate(), m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}
		strTmpData.Format(_T("%d"), i);
		arrIdx.Add(strTmpData);
		i = pGrid->SelectGetNext();
	}
	if(pGroup)
	{
		pGroup->SetLastSerialNo(nLastSerialNo);
		pGroup->UpDateLastSerialNo(m_pParentMgr, pDM);
	}
	ids = pDM->SendFrame();
	if(ids < 0) return ids;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	DeleteSelectRow(pDM, &arrIdx, _T(""));
	
	nRowCnt = pBOOK_DM->GetRowCount();
	for(i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(pBOOK_DM->GetCellData(_T("UDF_삭제여부"), i) == _T("Y"))
			pBOOK_DM->DeleteRow(i);
	}

	return 0;
}

INT CBO_ACQ_Business_Api::DivNoOrderCancel()
{
	CESL_DataMgr * pDM = NULL;
	CGroupInfo * pGroup = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("Group"));

	CString strQuery, strWorkCode, strTmpData;
	strWorkCode = pGroup->GetWorkCode();
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORKNO IS NOT NULL AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE IS NULL)"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo()
			);
	else
		strQuery.Format(
			_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORKNO IS NOT NULL AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE='%s')"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode
			);
	pDM->GetOneValueQuery(strQuery, strTmpData);
	INT ids = _ttoi(strTmpData.GetBuffer(0));
	if(ids > 0) return ids;

	pDM->StartFrame();

	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='0', LAST_WORK='%s', ORDER_DATE=NULL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='0', LAST_WORK='%s', ORDER_DATE=NULL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE = '%s';"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK='%s', ORDER_DATE=NULL, DELIVERY_PRICE=ORDER_PRICE, DELIVERY_BOOK_CNT=ORDER_BOOK_CNT WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE IS NULL);"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK='%s', ORDER_DATE=NULL, DELIVERY_PRICE=ORDER_PRICE, DELIVERY_BOOK_CNT=ORDER_BOOK_CNT WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE='%s');"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET DELIVERY_YN='Y', WORKING_STATUS='BOA111N', PREV_WORKING_STATUS='BOA112N', LAST_WORK='%s' WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE IS NULL);"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET DELIVERY_YN='Y', WORKING_STATUS='BOA111N', PREV_WORKING_STATUS='BOA112N', LAST_WORK='%s' WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE='%s');"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return ids;
}

INT CBO_ACQ_Business_Api::ChangeGroup()
{
	INT nTmpData = -1;
	GetTempData(_T("SPECIES_TYPE"), nTmpData);
	SPECIES_TYPE nSTYPE = (SPECIES_TYPE)nTmpData;
	GetTempData(_T("ACQ_TYPE"), nTmpData);
	ACQ_TYPE nATYPE = (ACQ_TYPE)nTmpData;
	if(nATYPE == ACQ_TYPE_INVALID)
		return ChangeGroupAcqObj(nSTYPE);
	else if(nATYPE == PURCHASE)
		return ChangeGroupPurchase(nSTYPE);
	else if(nATYPE == DONATE)
		return ChangeGroupDonate(nSTYPE);
	else if(nATYPE == OTHER_ACQ_TYPE)
		return ChangeGroupOtherAcqType(nSTYPE);
	return -1;
}

INT CBO_ACQ_Business_Api::ChangeGroupPurchase(SPECIES_TYPE nSTYPE)
{
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pDM_BOOK = NULL;
	CESL_Grid * pGrid = NULL;
	CGroupInfo * pGroup = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("GROUP"));
	
	INT nBookSerialNo, nPurchaseSerialNo, nIdx;
	nBookSerialNo = nPurchaseSerialNo = nIdx = 0;
	CString strWorkingStatus, strSpeciesKey, strAcqKey, strTmpData, strQuery;
	CString strAcqYear, strGroupNo, strWorkNo, strBookSerialNo, strPurchaseSerialNo, strWorkCode;
	
	if(nSTYPE == BO_PURCHASE) strWorkingStatus = _T("BOA111N");
	else return -1;
	
	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	if(pGroup)
	{
		nBookSerialNo = pGroup->GetBookLastSerialNo();
		nPurchaseSerialNo = pGroup->GetLastSerialNo();
		strAcqYear = pGroup->GetAcqYear();
		strGroupNo = pGroup->GetGroupInfo();
		strWorkNo = pGroup->GetWorkNo();
		strWorkCode = pGroup->GetWorkCode();
	}
	pDM->StartFrame();
	
	while(i >= 0)
	{
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		if(strSpeciesKey.IsEmpty() || strAcqKey.IsEmpty()) 
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		if(!pGroup)
		{
			strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
			strGroupNo = _T("NULL");
			strWorkNo = _T("NULL");
			strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), i);
		}
		
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', ACQ_YEAR='%s', SEQ_NO=%s, SERIAL_NO=%d ")
			_T("WHERE SPECIES_KEY=%s AND ACQ_KEY=%s;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strGroupNo, ++nBookSerialNo, strSpeciesKey, strAcqKey);
		pDM->AddFrame(strQuery);
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK='%s', ORDER_YEAR='%s', ORDER_SEQ_NO=%s, ")
			_T("ORDER_WORK_NO=%s, ORDER_SERIAL_NO=%d, SERIAL_NO=NULL WHERE SPECIES_KEY=%s AND REC_KEY=%s;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strGroupNo,
			strWorkNo, ++nPurchaseSerialNo, strSpeciesKey, strAcqKey);
		pDM->AddFrame(strQuery);
		if(pGroup)
		{
			strQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET LAST_WORK='%s', WORK_CODE='%s' WHERE REC_KEY=%s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetWorkCode(), strSpeciesKey);
			pDM->AddFrame(strQuery);
			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET LAST_WORK='%s', WORK_CODE='%s' WHERE REC_KEY=%s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetWorkCode(), strSpeciesKey);			
			pDM->AddFrame(strQuery);
		}
		
		pDM->SetCellData(_T("BP_수입년도"), strAcqYear, i);
		if(strGroupNo == _T("NULL")) strTmpData = _T("");
		else strTmpData = strGroupNo;
		pDM->SetCellData(_T("BP_그룹번호"), strTmpData, i);
		if(strWorkNo == _T("NULL")) strTmpData = _T("");
		else strTmpData = strWorkNo;
		pDM->SetCellData(_T("BP_작업번호"), strTmpData, i);
		strTmpData.Format(_T("%d"), nPurchaseSerialNo);
		pDM->SetCellData(_T("BP_일련번호"), strTmpData, i);
		pDM->SetCellData(_T("IBS_업무구분"), strWorkCode, i);
		pDM->SetCellData(_T("BP_발주일련번호"), _T(""), i);

		nIdx = FindIndex(pDM_BOOK, _T("BP_수서정보KEY"), strAcqKey, nIdx);
		while(nIdx >= 0)
		{
			pDM_BOOK->SetCellData(_T("BP_수입년도"), strAcqYear, nIdx);
			if(strGroupNo == _T("NULL")) strTmpData = _T("");
			else strTmpData = strGroupNo;
			pDM_BOOK->SetCellData(_T("BP_그룹번호"), strTmpData, nIdx);
			if(strWorkNo == _T("NULL")) strTmpData = _T("");
			else strTmpData = strWorkNo;
			pDM_BOOK->SetCellData(_T("BP_작업번호"), strTmpData, nIdx);
			strTmpData.Format(_T("%d"), nPurchaseSerialNo);
			pDM_BOOK->SetCellData(_T("BP_일련번호"), strTmpData, nIdx);
			pDM_BOOK->SetCellData(_T("IBS_업무구분"), strWorkCode, nIdx);
			pDM_BOOK->SetCellData(_T("BP_발주일련번호"), _T(""), nIdx);
			nIdx = FindIndex(pDM_BOOK, _T("BP_수서정보KEY"), strAcqKey, nIdx+1);
		}
		
		i = pGrid->SelectGetNext();
	}

	if(pGroup)
	{
		pGroup->SetLastSerialNo(nPurchaseSerialNo);
		pGroup->UpdateLastSerialNo(_T("1"), m_pParentMgr, pDM);
	}

	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CBO_ACQ_Business_Api::ChangeGroupDonate(SPECIES_TYPE nSTYPE)
{
	return 0;
}

INT CBO_ACQ_Business_Api::ChangeGroupOtherAcqType(SPECIES_TYPE nSTYPE)
{
	return 0;
}

INT CBO_ACQ_Business_Api::ChangeGroupAcqObj(SPECIES_TYPE nSTYPE)
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CGroupInfo * pGroup = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("GROUP"));
	
	CString strWorkingStatus, strTmpData, strQuery;
	if(nSTYPE == BO_BASIC) strWorkingStatus = _T("BOT111O");
	else if(nSTYPE == BO_IMPORT) strWorkingStatus = _T("BOT112O");

	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();

	pDM->StartFrame();

	while(i >= 0)
	{
		strTmpData = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(nSTYPE == BO_BASIC || nSTYPE == BO_IMPORT)
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', ACQ_YEAR='%s', SEQ_NO=%s WHERE SPECIES_KEY=%s AND WORKING_STATUS='%s' AND (ACQ_CODE='0' OR ACQ_CODE IS NULL);"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupNo(), strTmpData, strWorkingStatus);
		}
		else if(nSTYPE == BO_RECYCLE)
		{
			strQuery.Format(_T("UPDATE BO_DELETE_BOOK_TBL SET LAST_WORK='%s', ACQ_YEAR='%s', SEQ_NO=%s WHERE SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG='B';"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupNo(), strTmpData);
		}
		pDM->SetCellData(_T("UDF_그룹명"), pGroup->GetGroupInfo(), i);
		pDM->AddFrame(strQuery);
		i = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return 0;
}

INT CBO_ACQ_Business_Api::DeleteDMData()
{
	CESL_DataMgr * pSpecies_DM = NULL;
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pVolume_DM = NULL;
	CStringArray * pArrDelData = NULL;
	CString strCompAlias;

	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pVolume_DM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pArrDelData = (CStringArray*)GetTempPtr(_T("삭제데이터"));
	GetTempData(_T("비교Alias"), strCompAlias);
	
	if(pArrDelData == NULL) return 0;

	INT nDelDataCnt = pArrDelData->GetSize();
	for(INT i=0 ; i<nDelDataCnt ; i++)
	{
		if(pSpecies_DM)
			DeleteRow(pSpecies_DM, pArrDelData->GetAt(i), strCompAlias);
		if(pVolume_DM)
			DeleteRow(pVolume_DM, pArrDelData->GetAt(i), strCompAlias);
		if(pBook_DM)
			DeleteRow(pBook_DM, pArrDelData->GetAt(i), strCompAlias);
	}

	return 0;
}

VOID CBO_ACQ_Business_Api::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(m_pParentMgr);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();

	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_Business_Api::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CBO_ACQ_Business_Api::GetGridColumnIdx(CESL_Grid * pGrid, CString strColHeader)
{
	INT colCnt = pGrid->GetColCount();
	INT width, alignment, nPos;
	CString tmpstr;
	for(INT i=0 ; i<colCnt ; i++)
	{
		pGrid->GetColumnInfo(i, tmpstr, width, alignment);
		nPos = tmpstr.Find(strColHeader);
		if(nPos >= 0) return i;
	}
	return -1;
}

INT CBO_ACQ_Business_Api::CheckCancelRow(CESL_Grid *pGrid, CStringArray *pArrCancelIdx)
{
	INT nCnt = pArrCancelIdx->GetSize();
	if(nCnt == 0) return 0;
	INT nIdx;
	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpData = pArrCancelIdx->GetAt(i);
		nIdx = _ttoi(strTmpData.GetBuffer(0));
		pGrid->SetReverse(nIdx);
		pGrid->SetAt(nIdx, 0, _T("V"));
	}
	return 0;
}

INT CBO_ACQ_Business_Api::CheckInCancelBySelData()
{
	BOOL bRFIDUseYN = FALSE;
	CString strBookCntAlias;
	CBO_SPECIES api(m_pParentMgr);
	CESL_DataMgr * pPRINT_DM = (CESL_DataMgr*)GetTempPtr(_T("PRINT_DM"));
	CESL_DataMgr * pBOOK_DM = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_Grid * pGRID = (CESL_Grid*)GetTempPtr(_T("GRID"));
	GetTempData(_T("BOOK_CNT_ALIAS"), strBookCntAlias);
	GetTempData(_T("RFID_USE_YN"), bRFIDUseYN);
	if(pPRINT_DM == NULL || pBOOK_DM == NULL || pGRID == NULL)
		return -1;

	pPRINT_DM->StartFrame();

	INT i = pGRID->SelectGetHeadPosition();
	INT nRowCnt = pGRID->SelectGetCount();
	if(i < 0) return -1;
	CString strDiscRate, strSpeciesKey, strAcqKey, strVolSortNo, strTmpData, strQuery, strRFIDSerial;
	CString strTile, strVol, strAuthor, strPublisher, strMsg, strBookCnt;
	CStringArray arrIdx;	arrIdx.RemoveAll();

	INT nPos = 1;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정하신 자료를 미납처리 하고 있습니다."));
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		strSpeciesKey = pPRINT_DM->GetCellData(_T("IBS_종KEY"), i);
		strAcqKey = pPRINT_DM->GetCellData(_T("BP_수서정보KEY"), i);
		strVolSortNo = pPRINT_DM->GetCellData(_T("BB_권일련번호"), i);
		strDiscRate = pPRINT_DM->GetCellData(_T("BP_할인율"), i);
		strBookCnt = pPRINT_DM->GetCellData(strBookCntAlias, i);
		
		CheckBookDeleteField(pBOOK_DM, strAcqKey, strVolSortNo, _ttoi(strBookCnt.GetBuffer(0)));

		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET CHECKIN_DATE=NULL, LAST_WORK='%s' WHERE REC_KEY = %s;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey);
		pPRINT_DM->AddFrame(strQuery);
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET CHECKIN_DATE=NULL, DELIVERY_YN='N', LAST_WORK='%s', WORKING_STATUS='BOA113O', PREV_WORKING_STATUS='BOA114O' WHERE ACQ_KEY=%s AND VOL_SORT_NO=%s;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqKey, strVolSortNo);
		pPRINT_DM->AddFrame(strQuery);
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET DELIVERY_BOOK_CNT=GetAcqBookCount(%s, 'Y'), MISSING_BOOK_CNT=GetAcqBookCount(%s, 'N'), DELIVERY_PRICE=GetSpeciesPrice(%s, %s, 'Y') WHERE REC_KEY=%s;"), 
			strAcqKey, strAcqKey, strAcqKey, strDiscRate, strAcqKey);
		pPRINT_DM->AddFrame(strQuery);
		strTmpData.Format(_T("%d"), i);
		arrIdx.Add(strTmpData);
		i = pGRID->SelectGetNext();
	}
	m_pDataProcessingDlg->SetMessage(_T("미납처리 중입니다."));

	INT ids = pPRINT_DM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	ids = pPRINT_DM->EndFrame();
	if(ids < 0) return -1;

	DeleteSelectRow(pPRINT_DM, &arrIdx, _T(""));
	
	nRowCnt = pBOOK_DM->GetRowCount();
	for(i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(pBOOK_DM->GetCellData(_T("UDF_삭제여부"), i) == _T("Y"))
			pBOOK_DM->DeleteRow(i);
	}
	
	return 0;
}

INT CBO_ACQ_Business_Api::CheckInCancelByDivNo()
{
	CESL_DataMgr * pDM = NULL;
	CGroupInfo * pGroup = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("Group"));

	CString strQuery, strWorkCode, strTmpData;
	strWorkCode = pGroup->GetWorkCode();

	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORKING_STATUS>'BOC' AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE IS NULL)"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo()
			);
	else
		strQuery.Format(
			_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORKING_STATUS>'BOC' AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE='%s')"),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode
			);
	pDM->GetOneValueQuery(strQuery, strTmpData);
	INT ids = _ttoi(strTmpData.GetBuffer(0));
	if(ids > 0) return ids;

	pDM->StartFrame();

	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1', LAST_WORK='%s', CHECKIN_COMPLETE_DATE=NULL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL;"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='1', LAST_WORK='%s', CHECKIN_COMPLETE_DATE=NULL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE = '%s';"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK='%s', CHECKIN_DATE=NULL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE IS NULL);"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET LAST_WORK='%s', CHECKIN_DATE=NULL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE='%s');"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOA112N', PREV_WORKING_STATUS='BOA114O', LAST_WORK='%s' WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE IS NULL);"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	else
		strQuery.Format(
			_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOA112N', PREV_WORKING_STATUS='BOA114O', LAST_WORK='%s' WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_CODE='%s');"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
	pDM->AddFrame(strQuery);
	
	ids = pDM->SendFrame();
	if(ids < 0) return ids;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return 0;
}

INT CBO_ACQ_Business_Api::CheckBookDeleteField(CESL_DataMgr *pBOOK_DM, CString strAcqKey, CString strVolSortNo, INT nBookCnt)
{
	if(pBOOK_DM == NULL) return -1;
	INT nRowCnt = pBOOK_DM->GetRowCount();
	if(nRowCnt == 0) return 0;
	if(nBookCnt < 1) return 0;

	CString strSrcAcqKey, strSrcVolSortNo;
	INT nCnt = 0;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strVolSortNo.IsEmpty()) strSrcVolSortNo.Empty();
		else strSrcVolSortNo = pBOOK_DM->GetCellData(_T("BB_권일련번호"), i);
		strSrcAcqKey = pBOOK_DM->GetCellData(_T("BP_수서정보KEY"), i);
		if(strSrcAcqKey == strAcqKey && strSrcVolSortNo == strVolSortNo)
		{
			pBOOK_DM->SetCellData(_T("UDF_삭제여부"), _T("Y"), i);
			++nCnt;
			if(nBookCnt == nCnt) return 0;
		}
	}
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteData()
{
	DELETE_TYPE nDeleteType = SPECIES_DELETE;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	INT nTempData = -1;
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strPriceAlias, strBookCntAlias, strDelOption, strTmpData;
	CStringArray * parrSpeciesKey = NULL;
	CStringArray * parrAcqKey = NULL;
	CStringArray * parrVolSortNo = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("BookDM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("ACQ_TYPE"), nTempData);
	nAcqType = (ACQ_TYPE)nTempData;
	GetTempData(_T("삭제타입"), nTempData);
	nDeleteType = (DELETE_TYPE)nTempData;
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("삭제종류"), strDelOption);
	parrSpeciesKey = (CStringArray*)GetTempPtr(_T("종키리스트"));
	parrAcqKey = (CStringArray*)GetTempPtr(_T("수서정보키리스트"));
	parrVolSortNo = (CStringArray*)GetTempPtr(_T("권일련번호리스트"));
	
	CString strYN;
	CString strSelect;
	CString strBsKey;
	CString strQuery;
	CString strCnt;
	INT nRowCnt = 0;
	nRowCnt = pDM->GetRowCount();
	for ( INT i = 0 ; i < nRowCnt ; i++ )
	{
		pDM->GetCellData(_T("BB_대표책여부"), i, strYN);
		pGrid->GetAt(i, 0, strSelect);

		if( strYN == _T("Y") && strSelect == _T("V"))
		{
			pDM->GetCellData(_T("IBS_종KEY"), i, strBsKey);
			strQuery.Format( _T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL BB WHERE SPECIES_KEY = %s")
							 , strBsKey ); 
			pDM->GetOneValueQuery( strQuery, strCnt );
			if( _ttoi(strCnt) > 1 )
			{
				AfxMessageBox( _T("선정된 자료 중 대표책이 포함되어 있습니다.") );
				break;
			}
		}
	}
	
	CBO_DeleteDlg dlg(m_pParentMgr);
	dlg.SetMainDM(pDM);
	dlg.SetMainBookDM(pBookDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetDeleteType(nDeleteType);
	if(strDelOption.IsEmpty())
		dlg.SetDeleteOption(1);
	else if(strDelOption == _T("중복자료삭제"))
		dlg.SetDeleteOption(-1);
	dlg.SetVolSortNoArray(parrVolSortNo);
	dlg.SetAcqKeyArray(parrAcqKey);
	dlg.SetSpeciesKeyArray(parrSpeciesKey);
	dlg.SetBookCntAlias(strBookCntAlias);
	dlg.SetPriceAlias(strPriceAlias);
	dlg.SetAcqType(nAcqType);
	dlg.SetSpeciesType(nSpeciesType);
	dlg.DoModal();	
	if(!dlg.GetDeleteFlag()) return 1000;
	return 0;
}

INT CBO_ACQ_Business_Api::DeleteDataBook()
{
	BOOL bRecycleYN = FALSE;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	DELETE_TYPE nDeleteType = DELETE_TYPE_INVALID;
	INT nTempData = -1;
	INT nTempDeleteType = 0;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strBookKeyAlias, strRegNoAlias, strTmpData;
	CStringArray * parrSpeciesKey = NULL;
	CStringArray * parrAcqKey = NULL;
	CStringArray * parrVolSortNo = NULL;
	CGroupInfo * pGroup = NULL;
	CArray<INT, INT> * parrIdx = NULL;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	GetTempData(_T("SPECIES_TYPE"), nTempData);
	nSpeciesType = (SPECIES_TYPE)nTempData;
	GetTempData(_T("ACQ_TYPE"), nTempData);
	nAcqType = (ACQ_TYPE)nTempData;
	GetTempData(_T("재활용처리여부"), bRecycleYN);
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	GetTempData(_T("책키알리아스"), strBookKeyAlias);

	parrIdx = (CArray<INT, INT> *)GetTempPtr(_T("삭제인덱스"));
	parrSpeciesKey = (CStringArray*)GetTempPtr(_T("종키리스트"));
	parrAcqKey = (CStringArray*)GetTempPtr(_T("수서정보키리스트"));
	parrVolSortNo = (CStringArray*)GetTempPtr(_T("권일련번호리스트"));
	pGroup = (CGroupInfo*)GetTempPtr(_T("그룹정보"));
	if(parrSpeciesKey != NULL) parrSpeciesKey->RemoveAll();
	if(parrAcqKey != NULL) parrAcqKey->RemoveAll();
	if(parrVolSortNo != NULL) parrVolSortNo->RemoveAll();

	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;

	INT nSelCnt = pGrid->SelectGetCount();
	INT nPos = 1;
	ShowDataProcessingDlg();
	if(bRecycleYN == TRUE)
	{
		strTmpData.Format(_T("%d건의 자료를 재활용처리중입니다."), nSelCnt);
	}
	else
	{
		strTmpData.Format(_T("%d건의 자료를 삭제중입니다."), nSelCnt);
	}
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nSelCnt);

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	CString strSpeciesKey, strAccuKey, strBookRecKey;
	while(i >= 0)
	{
		if(m_pDataProcessingDlg->GetCancel()) break;
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		pDM->GetCellData(strBookKeyAlias, i, strBookRecKey);
		
		ids = api.BO_BOOK_DELETE(strSpeciesKey, strAccuKey, strBookRecKey, nSpeciesType, nAcqType, bRecycleYN, pGroup);
		if(ids < 0) return -1;
		parrIdx->Add(i);

		if(parrSpeciesKey != NULL) parrSpeciesKey->Add(strSpeciesKey);
		if(parrAcqKey != NULL && !strAccuKey.IsEmpty()) parrAcqKey->Add(strAccuKey);
		i = pGrid->SelectGetNext();
	}
	DeleteDataProcessingDlg();

	return 0;
}

INT CBO_ACQ_Business_Api::AdjustPurchaseDataError()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	if(pDM == NULL || pGrid == NULL) return -1;

	CBO_SPECIES api(m_pParentMgr);
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	pDM->StartFrame();
	while(i >= 0)
	{
		api.MakeAccuInfo(pDM, pDM, i, TRUE);

		i = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return 0;
}

INT CBO_ACQ_Business_Api::SynchronizedBookToIndex()
{
	CBO_SPECIES api(m_pParentMgr);
	
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	if(pDM == NULL) return -1;
	CESL_DataMgr tmpDM, tmpBookDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpBookDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strQuery ;
	strQuery.Format(_T("SELECT DISTINCT(I.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY = B.SPECIES_KEY AND I.IDX_VOL IS NULL AND B.VOL IS NOT NULL"));
	tmpDM.RestructDataManager(strQuery);
	
	CString strKey, strMsg;
	INT nBookCnt;
	INT nCnt = tmpDM.GetRowCount();
	if(nCnt == 0) 
	{
		m_pParentMgr->ESLAfxMessageBox(_T("동기화할 자료가 없습니다."));
		return 2000;
	}
	strMsg.Format(_T("%d건의 책-색인 동기화가 안된 자료가 있습니다.\n동기화 작업을 수행 하시겠습니까?"), nCnt);
	if(m_pParentMgr->ESLAfxMessageBox(strMsg, MB_YESNO) == IDNO) return 1000;
	
	pDM->StartFrame();
	CString strIdxVol, strIdxVolTitle, strVol, strVolTitle, strBookKey;
	for(INT i=0 ; i<nCnt ; i++)
	{
		strIdxVol.Empty();	strIdxVolTitle.Empty();
		strKey = tmpDM.GetCellData(i, 0);
		strQuery.Format(_T("SELECT REC_KEY, VOL, VOL_TITLE FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"), strKey);
		tmpBookDM.RestructDataManager(strQuery);
		nBookCnt = tmpBookDM.GetRowCount();
		for(INT j=0 ; j<nBookCnt ; j++)
		{
			strBookKey = tmpBookDM.GetCellData(j, 0);
			strVol = tmpBookDM.GetCellData(j, 1);
			strVolTitle = tmpBookDM.GetCellData(j, 2);
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strVol);			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strVol);
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strVolTitle);		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strVolTitle);
			if(strIdxVol.IsEmpty()) strIdxVol = strVol;
			else strIdxVol += _T(" ") + strVol;
			if(strIdxVolTitle.IsEmpty()) strIdxVolTitle = strVolTitle;
			else strIdxVolTitle += _T(" ") + strVolTitle;
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET VOL_INDEX='%s', VOL_TITLE_INDEX='%s' WHERE REC_KEY=%s;"), 
				strVol, strVolTitle, strBookKey);
			pDM->AddFrame(strQuery);
		}
		strQuery.Format(
			_T("UPDATE IDX_BO_TBL SET	IDX_VOL='%s', ")
									_T("IDX_VOL_TITLE='%s', ")
									_T("USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
									_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
									_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
									_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
									_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
									_T("LAST_WORK='%s' WHERE rec_key=%s;"), 
			strIdxVol, strIdxVolTitle, strKey, strKey, strKey, strKey, strKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strKey);
		pDM->AddFrame(strQuery);
			
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			pDM->AddFrame(strQuery);
		}
	}

	INT	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return ids;
}

INT CBO_ACQ_Business_Api::RestructIndexField()
{
	CBO_SPECIES api(m_pParentMgr);
	
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	if(pDM == NULL || pGrid == NULL) return -1;
	
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
	DM_Index.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	pDM->StartFrame();
	CStringArray ArrData;
	
	CString strMarc, strKey, strQuery, strIdxVol, strIdxVolTitle;
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CMarc marc;
	pDM->StartFrame();
	while(i >= 0)
	{
		strMarc = pDM->GetCellData(_T("BS_MARC"), i);
		strKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		strIdxVol = pDM->GetCellData(_T("IBS_권차색인"), i);
		if(!strIdxVol.IsEmpty())
		{
			ArrData.RemoveAll();
			api.ParsingString(_T(" "), strIdxVol, &ArrData, TRUE);
			INT nCnt = ArrData.GetSize();
			strIdxVol.Empty();
			for(INT idx=0 ; idx<nCnt ; idx++)
			{
				if(strIdxVol.IsEmpty()) strIdxVol = ArrData.GetAt(idx);
				else strIdxVol += _T(" ") + ArrData.GetAt(idx);
			}
		}

		strIdxVolTitle = pDM->GetCellData(_T("IBS_권서명색인"), i);
		if(!strIdxVolTitle.IsEmpty())
		{
			ArrData.RemoveAll();
			api.ParsingString(_T(" "), strIdxVolTitle, &ArrData, TRUE);
			INT nCnt = ArrData.GetSize();
			strIdxVolTitle.Empty();
			for(INT idx=0 ; idx<nCnt ; idx++)
			{
				if(strIdxVolTitle.IsEmpty()) strIdxVolTitle = ArrData.GetAt(idx);
				else strIdxVolTitle += _T(" ") + ArrData.GetAt(idx);
			}
		}

		if(strMarc.IsEmpty()) 
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		strQuery.Format(_T("REC_KEY = %s"), strKey);
		DM_Index.RefreshDataManager(strQuery);
		if(DM_Index.GetRowCount() < 1)
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		DM_Index.SetCellData(_T("IBS_검색용편권차"), strIdxVol, 0);
		DM_Index.SetCellData(_T("IBS_검색용권서명"), strIdxVolTitle, 0);
		api.APPLY_IndexItem(&DM_Index, &marc, 0);
		DM_Index.MakeQueryByDM(RemoveAliasList, &DM_Index, DM_Index.TBL_NAME, 0, pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			pDM->AddFrame(strQuery);
		}
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return ids;
}

INT CBO_ACQ_Business_Api::MakePrintVolumeInfoByBook()
{
	CBO_SPECIES api(m_pParentMgr);
	
	CString strDivAlias;
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pVolume_DM = NULL;
	CTextProgressCtrl * pProgressCtrl = NULL;
	
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pVolume_DM = (CESL_DataMgr*)GetTempPtr(_T("권DM"));
	pProgressCtrl = (CTextProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));
	GetTempData(_T("구분키Alias"), strDivAlias);
	
	if(!pBook_DM || !pVolume_DM) return -1;
	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	CBO_DataConverter Converter;
	Converter.SetSourceDM(pBook_DM);
	Converter.SetTargetDM(pVolume_DM);
	Converter.SetDivAlias(strDivAlias);
	Converter.SetProgressCtrl(pProgressCtrl);
	INT ids = Converter.ConvertToVolumeInfo(TRUE);
	return ids;
}

INT CBO_ACQ_Business_Api::MakeSortNo()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = _T("BB_수입구분");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	

	CString strCode, strDivKeyAlias;
	CString codes[6];
	INT nStartRow, ids;
	nStartRow = 0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("입수구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		nStartRow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("입수구분"), codes, 6, nStartRow);
		strCode = codes[2];
		strCode.TrimLeft();	strCode.TrimRight();
		if(strCode == _T("1") || strCode == _T("2")) strDivKeyAlias = _T("BB_수서정보KEY");
		else strDivKeyAlias = _T("IBS_종KEY");

		ids = SetSortNo(pDM, _T("BB_수입구분"), strCode, strDivKeyAlias);
		if(ids < 0) return -1;
		nStartRow++;
	}

	return 0;
}

INT CBO_ACQ_Business_Api::SetSortNo(CESL_DataMgr *pDM, CString strCodeAlias, CString strCode, CString strDivKeyAlias)
{
	INT nStartIdx, nLastIdx;
	INT ids = FindFirstEndIdx(pDM, strCodeAlias, strCode, nStartIdx, nLastIdx);
	if(ids < 0) return 0;
	
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = strDivKeyAlias;				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC, nStartIdx, nLastIdx);	

	INT nSCnt, nNext;
	nNext = nStartIdx;
	CString strTmpKey;
	while(nLastIdx >= nStartIdx)
	{
		pDM->GetCellData(strDivKeyAlias, nStartIdx, strTmpKey);
		nSCnt = FindSameKeyCnt(pDM, nNext, nLastIdx, strDivKeyAlias, strTmpKey);
		SetSortNo(pDM, nStartIdx, nStartIdx+nSCnt);
		nStartIdx = nNext;
	}

	return 0;
}

INT CBO_ACQ_Business_Api::FindFirstEndIdx(CESL_DataMgr *pDM, CString strCodeAlias, CString strCode, INT &nStartIdx, INT &nLastIdx)
{
	BOOL bFind = FALSE;
	CString strTmpData;
	nStartIdx = nLastIdx = -1;
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(strCodeAlias, i);
		if(!bFind && strTmpData == strCode)
		{
			bFind = TRUE;
			nStartIdx = i;
		}
		else if(bFind && strTmpData != strCode)
		{
			nLastIdx = i-1;
			break;
		}
	}

	if(!bFind) return -1;
	if(nLastIdx < 0) nLastIdx = i-1;
	return 0;
}

INT CBO_ACQ_Business_Api::FindSameKeyCnt(CESL_DataMgr *pDM, INT &nNext, INT nLastIdx, CString strDivKeyAlias, CString strTmpKey)
{
	INT i = nNext;
	i++;
	INT cnt = 0;
	CString strTmpData;
	while(i <= nLastIdx)
	{
		pDM->GetCellData(strDivKeyAlias, i, strTmpData);
		if(strTmpData == strTmpKey)
		{
			cnt++;
			i++;
		}
		else break;
	}
	nNext = i;
	return cnt;
}

INT CBO_ACQ_Business_Api::SetSortNo(CESL_DataMgr *pDM, INT nStartIdx, INT nEndIdx)
{
	CString strTmpData;
	LONG nMinSortNo, nTmpData;
	nMinSortNo = LONG_MAX;
	for(INT i=nStartIdx ; i<=nEndIdx ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BB_일련번호"), i);
		nTmpData = _ttol(strTmpData.GetBuffer(0));
		if(nTmpData < nMinSortNo) nMinSortNo = nTmpData;
	}
	for(i=nStartIdx ; i<=nEndIdx ; i++)
	{
		strTmpData.Format(_T("%ld"), nMinSortNo);
		pDM->SetCellData(_T("UDF_일련번호"), strTmpData, i);
	}
	return 0;
}

CString CBO_ACQ_Business_Api::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), strValue);

	if(strValue == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(strValue == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

INT CBO_ACQ_Business_Api::ConvertRecKeyToSubQuery(CString &strRecKeyList, CString strTableName, CString strKeyName)
{
	
	CString strTemp;
 	CString strRecKey;
 	CString strBookKeyTempList;
 	CString strQueryWhere;
 	INT nCnt;
 	INT nLocation;
 	INT nStartLocation;
	INT i ; 
	strBookKeyTempList = strRecKeyList;
	strBookKeyTempList.Replace(_T("'"), _T(""));
	nCnt = 0;
	nLocation = 0;
	strQueryWhere.Format(_T(""));
	while( 1 )
	{
		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
		++nCnt;
		if( 0 > nLocation )
		{
			break;			
		}
	}
	if ( (nCnt % 1000) == 0 )
	{
		nCnt = (nCnt/1000);
	}
	else
	{
		nCnt = (nCnt/1000)+1;
	}
	nStartLocation = strBookKeyTempList.GetLength()/nCnt;

	for( i = 0 ; i < nCnt ; i++)
	{
		if( i == (nCnt -1) )
		{
			strRecKey = strBookKeyTempList;
		}
		else
		{
			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
			strRecKey = strBookKeyTempList.Mid(0, nLocation );
			strTemp.Format(_T("%s,"), strRecKey);
			strBookKeyTempList.Replace( strTemp, _T("") );
		}
	
		if( strQueryWhere.IsEmpty() )
		{
			strQueryWhere.Format(_T("%s IN (%s)"), strKeyName, strRecKey);
		}
		else
		{
			strTemp.Format(_T("%s OR %s IN( %s )"), strQueryWhere,strKeyName, strRecKey );
			strQueryWhere = strTemp;
		}
	}
	strRecKeyList.Format(_T("SELECT %s FROM %s WHERE %s"), strKeyName, strTableName,strQueryWhere);
	return 0;
}
