// ErrorAdjustApi.cpp: implementation of the CErrorAdjustApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustApi.h"
#include "ESL_CharSetMgr.h"
#include "MarcAdjustCondition.h"
#include "MarcAdjustDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("오류수정API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustApi::CErrorAdjustApi(CESL_Mgr * m_pParentMgr) : CSPF_Work(m_pParentMgr)
{
	m_pDM_ACCU = NULL;
}

CErrorAdjustApi::~CErrorAdjustApi()
{
	if(m_pDM_ACCU)
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
}

INT CErrorAdjustApi::SPFExecutePath(CString strPath)
{
	if(strPath.IsEmpty())
		return -1;
	if(strPath == _T("책To종"))
		return MakeSpeciesInfoByBook();
	else if(strPath == _T("구입정보구축"))
		return AdjustPurchaseDataError();
	else if(strPath == _T("책색인동기화"))
		return SynchronizedBookToIndex();
	else if(strPath == _T("색인재구축"))
		return RestructIndexField();
	else if(strPath == _T("마크수정"))
		return MarcUpdate();
	else if(strPath == _T("쓰레기자료삭제"))
		return DeleteWasteData();
	else if(strPath == _T("재활용자료복원"))
		return RecycleDataRestore();
	else if(strPath == _T("등록번호설정IN대출정보"))
		return SetRegNoInLoanInfo();
	else if(strPath == _T("책부록플래그설정"))
		return SetAppendixBookFlag();
	else if(strPath == _T("비치희망색인재구축"))
		return RestructRequestIndexData();
	else if(strPath == _T("종정보오류수정"))
		return AdjustSpeciesError();
	else if(strPath == _T("관리번호오류수정"))
		return DeleteDataBook();
	return -1;
}

INT CErrorAdjustApi::MakeSpeciesInfoByBook()
{
	CESL_DataMgr * pBook_DM = NULL;
	CESL_DataMgr * pSpecies_DM = NULL;
	CProgressCtrl * pProgressCtrl = NULL;
	pBook_DM = (CESL_DataMgr*)GetTempPtr(_T("책DM"));
	pSpecies_DM = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pProgressCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCtrl"));
	
	if(!pBook_DM || !pSpecies_DM) return -1;
	INT nRowCnt = pBook_DM->GetRowCount();
	if(nRowCnt == 0) return 0;
	pSpecies_DM->FreeData();
	pBook_DM->SORT(_T("IBS_종KEY"));
	
	MakeOtherInfoByBook(pSpecies_DM, pBook_DM, 0, nRowCnt-1, _T("IBS_종KEY"), pProgressCtrl);
	
	return 0;
}

INT CErrorAdjustApi::MakeOtherInfoByBook(CESL_DataMgr * pTargetDM, CESL_DataMgr * pSourceDM, INT startIdx, INT endIdx, CString strAlias, CProgressCtrl * pProgressCtrl/*=NULL*/)
{
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strSpeciesWorkingStatus;
	INT ids, cnt, nDBookCnt, nTmp1, nTmp2;
	ids = cnt = nDBookCnt = nTmp1 = nTmp2 = 0;
	DOUBLE nTotalPrice = 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	if(pProgressCtrl)
	{
		pProgressCtrl->SetRange32(0, endIdx-startIdx)	;
		pProgressCtrl->SetPos(0);
	}
	
	for(INT i=startIdx ; i<=endIdx; i++)
	{
		if(pProgressCtrl)
		{
			pProgressCtrl->SetPos(i+1);
		}
		pSourceDM->GetCellData(strAlias, i, strTmpKey);
		if(!strKey.IsEmpty() && strKey != strTmpKey)
		{
			ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
			if(ids < 0) break;
			strTmpData.Format(_T("%d"), cnt);
			pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);
			strTmpData.Format(_T("%.0f"), nTotalPrice);
			pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_가격"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
			
			strSpeciesWorkingStatus.Empty();
			strKey = strTmpKey;
			cnt = 1;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);
			//nTotalPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strSpeciesWorkingStatus = pSourceDM->GetCellData(_T("BB_자료상태"), i);
		}
		else 
		{
			cnt++;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);
			//nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strTmpData = pSourceDM->GetCellData(_T("BB_자료상태"), i);
			strKey = strTmpKey;
			if(strTmpData.IsEmpty()) continue;
			if(nTmp1 < nTmp2) strSpeciesWorkingStatus = strTmpData;
			else if(nTmp1 == nTmp2) 
			{
				if(strSpeciesWorkingStatus.Compare(strTmpData) < 0) strSpeciesWorkingStatus = strTmpData;
			}
		}
	}
	if(i > endIdx)
	{
		ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
		strTmpData.Format(_T("%d"), cnt);
		pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);
		strTmpData.Format(_T("%.0f"), nTotalPrice);
		pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
	}
	return 0;
}

INT CErrorAdjustApi::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	if( idx < 0) return -1000;
	
	INT col, nCol;
	
	// 사용할 필드 정보를 구축한다.
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT nCurRow, nColCnt;
	pTargetDM->InsertRow(-1);
	nCurRow = pTargetDM->GetRowCount() - 1;
	nColCnt = pSourceDM->m_nCols;
	if(nColCnt < 0) nColCnt = pSourceDM->GetRefColumnCount();
	
	for (col = 0; col < nColCnt; col++) {
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol >= 0)	pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(idx, col));
	}
	
	return nCurRow;
}

INT CErrorAdjustApi::SetAccuDM(CESL_DataMgr * pDM_ACCU, BOOL bPurchase)
{
	CString strDMAlias, strDMPK;
	if(bPurchase) strDMAlias = _T("DMUP_단행_종_구입정보");
	else strDMAlias = _T("DMUP_단행_종_기증정보");
	
	if(m_pDM_ACCU != NULL) 
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
	m_pDM_ACCU = new CESL_DataMgr;
	m_pDM_ACCU->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	m_pDM_ACCU->InitDMPKFromFile(strDMAlias);
	m_pDM_ACCU->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	return 0;
}

INT CErrorAdjustApi::Apply_MARC_950(CString strSource, CString strPrice, CString strCurrencyCode)
{
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) {
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		} else {
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}
	szBufDigit[nBufDigitSize] = '\0';
	strPrice.Format(_T("%s"), szBufDigit);
	strPrice.TrimLeft();		strPrice.TrimRight();
	szBufNotDigit[nBufNotDigitSize] = '\0';
	strCurrencyCode.Format(_T("%s"), szBufNotDigit);
	strCurrencyCode.TrimLeft();			strCurrencyCode.TrimRight();
	strCurrencyCode = strCurrencyCode.Left(3);
	
	return 0;
}

INT CErrorAdjustApi::GetGroupInfo(CESL_DataMgr *pDM, INT nIdx, CString &strOrderLastSerialNo, CString &strWorkNo, CString &strLastSerialNo)
{
	CString strWorkCode, strGroupInfo, strAcqYear, strWorkStatus, strAcqCode;
	CString strQuery, strValue, strKey;
	strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), nIdx);
	strGroupInfo = pDM->GetCellData(_T("BB_그룹번호"), nIdx);
	strAcqYear = pDM->GetCellData(_T("BB_수입년도"), nIdx);
	strWorkStatus = pDM->GetCellData(_T("BB_자료상태"), nIdx);
	strAcqCode = pDM->GetCellData(_T("BB_수입구분"), nIdx);
		
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	if(strAcqCode == _T("1"))
	{
		strQuery.Format(_T("SELECT WORKING_STATUS, LAST_WORK_NO, REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s"),
			strAcqYear, strGroupInfo);
		tmpDM.RestructDataManager(strQuery);
		strValue = tmpDM.GetCellData(0, 0);
		if(strValue == _T("0")) strWorkStatus = _T("BOA111N");
		else if(strValue == _T("1")) strWorkStatus = _T("BOA112N");
		else if(strValue == _T("2"))
		{
			if(strWorkStatus == _T("BOA111N") || strWorkStatus == _T("BOA112N"))
				strWorkStatus = _T("BOA114O");
			strQuery.Format(_T("SELECT MAX(SERIAL_NO) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s"),
				strAcqCode, strGroupInfo);
			pDM->GetOneValueQuery(strQuery, strValue);
			strOrderLastSerialNo = strValue;
		}
		strValue = tmpDM.GetCellData(0, 1);
		strKey = tmpDM.GetCellData(0, 2);
		CStringArray tmpArr;	tmpArr.Add(strValue);
		strWorkNo = strValue;
		strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY=%s AND WORK_NO=%s"), 
			strKey, strValue);
		pDM->GetOneValueQuery(strQuery, strValue);
		strLastSerialNo = strValue;
	}
	return 0;
}

CString CErrorAdjustApi::GetTodayYear()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

CString CErrorAdjustApi::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CErrorAdjustApi::MakeAccuInfo(CESL_DataMgr *pDM_OUT, CESL_DataMgr *pDM, INT nIdx, BOOL isPurchase)
{
	CString strMarc;
	CESL_DataMgr * pDM_ACCU = NULL;
	SetAccuDM(pDM_ACCU, isPurchase);
	
	m_pDM_ACCU->InsertRow(-1);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strRecKey, strTmpData, strAcqkey, strPrice, strCurrencyCode, strPriceOtherInfo, strInputType;
	CString strBookCnt, strSpeciesKey;
	CMarc marc;
	strMarc = pDM->GetCellData(_T("BS_MARC"), nIdx);
	m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTmpData);
	Apply_MARC_950(strTmpData, strPrice, strCurrencyCode);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strPriceOtherInfo);
	
	strInputType = _T("0");
	strAcqkey = pDM->GetCellData(_T("BB_수서정보KEY"), nIdx);
	strBookCnt = pDM->GetCellData(_T("UDF_책수"), nIdx);
	strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), nIdx);
	
	CString strOrderSerialNo, strWorkNo, strLastSerialNo;
	INT ids = GetGroupInfo(pDM, nIdx, strOrderSerialNo, strWorkNo, strLastSerialNo);
	if(ids < 0) return -1;
	
	if(isPurchase)
	{
		ids = pDM_OUT->MakeRecKey(strAcqkey); 
		if(ids < 0) return ids;
		
		CString strTmpData;
		m_pDM_ACCU->SetCellData(_T("BP_수서정보KEY"), strAcqkey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_발주책수"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_납품책수"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_미납책수"), _T("0"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_주문정산타입"), _T("종가격"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), pDM->GetCellData(_T("BB_수입년도"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), pDM->GetCellData(_T("BB_그룹번호"), nIdx), 0);
		strTmpData = strWorkNo;
		if(strTmpData == _T("사용안함")) strTmpData = _T("0");
		m_pDM_ACCU->SetCellData(_T("BP_작업번호"), strTmpData, 0);
		INT nSerialNo = _ttoi(strLastSerialNo.GetBuffer(0));
		strTmpData.Format(_T("%d"), nSerialNo+1);
		m_pDM_ACCU->SetCellData(_T("BP_발주일련번호"), strTmpData, 0);
		m_pDM_ACCU->SetCellData(_T("BP_총권수"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_입력일자"), pDM->GetCellData(_T("BB_입력일"), 0), 0);
		m_pDM_ACCU->SetCellData(_T("BP_입력구분"), strInputType, 0);
		INT nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		if(nOrderSerialNo < 0) m_pDM_ACCU->SetCellData(_T("BP_일련번호"), _T(""), 0);
		else
		{
			strTmpData.Format(_T("%d"), nOrderSerialNo+1);
			m_pDM_ACCU->SetCellData(_T("BP_일련번호"), strTmpData, 0);
		}
		m_pDM_ACCU->SetCellData(_T("BP_발주가격"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_납품가격"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_할인율"), _T("0"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_발주일자"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_검수일"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_환율"), _T("1"), 0);
		m_pDM_ACCU->SetCellData(_T("BP_종KEY"), strSpeciesKey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_가격"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_화폐구분"), strCurrencyCode, 0);
		m_pDM_ACCU->SetCellData(_T("BP_가격잡정보"), strPriceOtherInfo, 0);
		m_pDM_ACCU->SetCellData(_T("BP_주제구분_코드"), pDM->GetCellData(_T("BS_주제구분"), 0), 0);
		ids = m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;

		strTmpData.Format(
			_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s);"),
			nSerialNo, 
			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			strWorkNo,
			pDM->GetCellData(_T("BB_수입년도"), nIdx),
			pDM->GetCellData(_T("BB_그룹번호"), nIdx)
			);
		pDM_OUT->AddFrame(strTmpData);
		strTmpData.Format(
			_T("UPDATE BO_BOOK_TBL SET ACQ_KEY=%s, LAST_WORK='%s' WHERE SPECIES_KEY=%s AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND ACQ_KEY IS NULL;"),
			strAcqkey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, pDM->GetCellData(_T("BB_수입년도"), nIdx), pDM->GetCellData(_T("BB_그룹번호"), nIdx));
		pDM_OUT->AddFrame(strTmpData);
	}
	else
	{
		ids = m_pDM_ACCU->MakeRecKey(strAcqkey);
		if(ids < 0) return ids;

		m_pDM_ACCU->SetCellData(_T("BP_수서정보KEY"), strAcqkey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_발주책수"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), pDM->GetCellData(_T("BB_수입년도"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), pDM->GetCellData(_T("BB_그룹번호"), nIdx), 0);
		m_pDM_ACCU->SetCellData(_T("BP_총권수"), strBookCnt, 0);
		m_pDM_ACCU->SetCellData(_T("BP_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
		m_pDM_ACCU->SetCellData(_T("BP_입력일자"), pDM->GetCellData(_T("BB_입력일"), 0), 0);
		m_pDM_ACCU->SetCellData(_T("BP_검수일"), GetTodayDate(), 0);
		m_pDM_ACCU->SetCellData(_T("BP_입력구분"), strInputType, 0);
		m_pDM_ACCU->SetCellData(_T("BP_종KEY"), strSpeciesKey, 0);
		m_pDM_ACCU->SetCellData(_T("BP_가격"), strPrice, 0);
		m_pDM_ACCU->SetCellData(_T("BP_화폐구분"), strCurrencyCode, 0);
		m_pDM_ACCU->SetCellData(_T("BP_가격잡정보"), strPriceOtherInfo, 0);
		m_pDM_ACCU->SetCellData(_T("BP_주제구분_코드"), pDM->GetCellData(_T("BS_주제구분"), 0), 0);
		ids = m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, 0, pDM_OUT);
		if(ids < 0) return ids;
		strTmpData.Format(
			_T("UPDATE BO_BOOK_TBL SET ACQ_KEY=%s, LAST_WORK='%s' WHERE SPECIES_KEY=%s AND ACQ_CODE='2' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND ACQ_KEY IS NULL;"),
			strAcqkey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey, pDM->GetCellData(_T("BB_수입년도"), nIdx), pDM->GetCellData(_T("BB_그룹번호"), nIdx));
		pDM_OUT->AddFrame(strTmpData);
	}

	
	return 0;
}

INT CErrorAdjustApi::AdjustPurchaseDataError()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		ids = MakeAccuInfo(pDM, pDM, i, TRUE);
		if(ids < 0) return -1;
		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}

	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	DeleteSelectRow(pDM, &ArrIdx);
	
	return 0;
}

INT CErrorAdjustApi::SynchronizedBookToIndex()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;

	CESL_DataMgr tmpBookDM;
	tmpBookDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();

	CString strSpeciesKey, strQuery, strBookKey, strVol, strVolTitle, strIdxVol, strIdxVolTitle;
	CStringArray ArrBookInfo;	ArrBookInfo.RemoveAll();
	INT nBookCnt;
	pDM->StartFrame();
	
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strSpeciesKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
		}
		strQuery.Format(_T("SELECT REC_KEY, VOL, VOL_TITLE FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"), strSpeciesKey);
		ids = tmpBookDM.RestructDataManager(strQuery);
		if(ids < 0) return -1;
		strIdxVol.Empty();	strIdxVolTitle.Empty();
		
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
			_T("LAST_WORK='%s' where rec_key=%s;"), 
			strIdxVol, strIdxVolTitle, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(
// 			_T("INSERT INTO IDX_TMP_BO_TBL ")
// 			_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 			_T("FROM IDX_BO_TBL ")
// 			_T("WHERE REC_KEY=%s);"),
// 			strSpeciesKey);
// 		pDM->AddFrame(strQuery);
		//=====================================================
		
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}

		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids < 0) return ids;
	DeleteSelectRow(pDM, &ArrIdx);

	return 0;
}

INT CErrorAdjustApi::DeleteSelectRow(CESL_DataMgr * pDM, CArray<INT, INT> * pArrIdx)
{
	INT cnt = pArrIdx->GetSize();
	if(cnt == 0) return 0;
	
	INT rowidx, ids;
	for(INT i=cnt-1 ; i>=0 ; i--)
	{
		rowidx = pArrIdx->GetAt(i);
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

BOOL CErrorAdjustApi::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
		return FALSE;
}

INT CErrorAdjustApi::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

INT CErrorAdjustApi::RestructIndexField()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;
	
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
	DM_Index.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}	
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	CStringArray ArrData;
	
	CString strMarc, strKey, strQuery;
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CMarc marc;
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strMarc = pDM->GetCellData(_T("BS_MARC"), i);
		strKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strKey.IsEmpty())
		{
			i = pGrid->SelectGetNext();
			continue;
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
		APPLY_IndexItem(&DM_Index, &marc, 0);
		DM_Index.MakeQueryByDM(RemoveAliasList, &DM_Index, DM_Index.TBL_NAME, 0, pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
			pDM->AddFrame(strQuery);
		}
		ArrIdx.Add(i);

		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}

INT CErrorAdjustApi::APPLY_IndexItem(CESL_DataMgr *pDM_IDX_BO, CMarc *pMarc, INT nRowIdx) 
{
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, pMarc);
	
	INT nIndexToolKit = 0;
	CString IndexToolKit[20];
	CString IndexToolKitCommand[20];
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용저작자");			IndexToolKitCommand[nIndexToolKit] = _T("저작자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_범위검색용발행년도");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행년도");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용범위검색발행자");	IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행자");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용정렬용본표제");		IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용키워드");			IndexToolKitCommand[nIndexToolKit] = _T("키워드");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용발행자");			IndexToolKitCommand[nIndexToolKit] = _T("발행자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용본표제");			IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_저작자사항");				IndexToolKitCommand[nIndexToolKit] = _T("저작자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행자사항");				IndexToolKitCommand[nIndexToolKit] = _T("발행자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_표제사항");				IndexToolKitCommand[nIndexToolKit] = _T("표제사항");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행년도사항");			IndexToolKitCommand[nIndexToolKit] = _T("발행년도사항");			nIndexToolKit++;

	INT i;
	CString strTag, strData, strIdxTitle, strIdxAuthor, strIdxPublihser, strIdxKeyword;
	for (i = 0; i < nIndexToolKit; i++) {
		strData.Empty();
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
		pDM_IDX_BO->SetCellData(IndexToolKit[i], strData, nRowIdx);
	}
	
	//IDX_ALL_ITEM필드를 채우기 위해서 검색용본표제, 저작자, 발행자, 키워드 를 추출한다.
	pDM_IDX_BO->GetCellData(_T("IBS_검색용본표제"), nRowIdx, strIdxTitle);
	pDM_IDX_BO->GetCellData(_T("IBS_검색용저작자"), nRowIdx, strIdxAuthor);
	pDM_IDX_BO->GetCellData(_T("IBS_검색용발행자"), nRowIdx, strIdxPublihser);
	pDM_IDX_BO->GetCellData(_T("IBS_검색용키워드"), nRowIdx, strIdxKeyword);

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_검색용저작자"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_검색용저작자"), strData, nRowIdx);
		strIdxAuthor = strData;
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_저작자"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		// 규칙1 : 245$d
		// 규칙1이 없을경우 규칙2 : 도서(형식구분TX)->100, 비도서(형식구분!=TX)->700

		// 구칙1 적용
		strData = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		pDM_IDX_BO->SetCellData(_T("IBS_저작자"), strData, nRowIdx);
		
		CString strFORM_CODE;
		// 규칙2 or 3적용
		if (strData.GetLength() == 0) {
			pDM_IDX_BO->GetCellData(_T("IBS_형식구분_코드"), nRowIdx, strFORM_CODE);
			strData = _T("");
			if (strFORM_CODE == _T("TX")) {
				m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_저작자"), strData, nRowIdx);
			} else {
				m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strData);
				pDM_IDX_BO->SetCellData(_T("IBS_저작자"), strData, nRowIdx);
			}
		}
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_본표제"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_검색용본표제"), nRowIdx, strData);
		pDM_IDX_BO->SetCellData(_T("IBS_본표제"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_검색용범위검색발행자"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("발행자"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_검색용범위검색발행자"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_발행자"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_발행자사항"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_발행자"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("발행자"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_발행자"), strData, nRowIdx);
//		pDM_IDX_BO->SetCellData(_T("IBS_발행자사항"), strData, nRowIdx);
	}

	strData = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_발행년"), nRowIdx, strData);
	if (strData.GetLength() == 0) {
		strData = _T("");
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(_T("범위검색용발행년도"), strData);
		pDM_IDX_BO->SetCellData(_T("IBS_발행년"), strData, nRowIdx);
	}
	
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
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	pDM_IDX_BO->SetCellData(_T("IBS_전체검색용색인어"), strIdxAllItem, nRowIdx);
	return 0;
}

INT CErrorAdjustApi::DeleteUnlessWord(CString &strData)
{
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("역할어"), _T("역할어"));
	INT nFind = strData.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strData.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if (CharSetMgr_UnlessWord.Find(str1, str2) == 0)
		{
			strData.Delete(nFind, strData.GetLength() - nFind);
		}
	}
	return 0;
}

INT CErrorAdjustApi::DeleteWasteData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	
	if(pDM == NULL || pGrid == NULL) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CString strSpeciesKey, strQuery;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		//1. 검색엔진 동기화를 위해 큐테이블에 로그 쓰기
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(
// 			_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 			_T("ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"),
// 			strSpeciesKey);
// 		pDM->AddFrame(strQuery);
		//=====================================================
		
		//종 테이블에서 책 없는 자료 삭제
		strQuery.Format(
			_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY = %s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//구입정보삭제
		strQuery.Format(
			_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//기증정보삭제
		strQuery.Format(
			_T("DELETE FROM BO_DONINFO_TBL WHERE SPECIES_KEY=%s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//색인 테이블에서 책 없는 자료 삭제
		strQuery.Format(
			_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY = %s;"), strSpeciesKey);
		pDM->AddFrame(strQuery);
		//ECO_SEARCH에 적용
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
			pDM->AddFrame(strQuery);
		}
		//목차초록 삭제
		ids = BO_TOC_ABSTRACT_DELETE(pDM, strSpeciesKey);
		if(ids < 0) return -1;
		
		ArrIdx.Add(i);
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if(ids < 0) return -1;
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}

INT CErrorAdjustApi::BO_TOC_ABSTRACT_DELETE(CESL_DataMgr *pDM_OUT, CString strSpeciesKey)
{
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
	pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(!strValue.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s;"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}
	
	return 0;
}

BOOL CErrorAdjustApi::IsSameSeShelfCode(CESL_DataMgr *pDM_BOOK)
{
	if(pDM_BOOK == NULL) return FALSE;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	if(nRowCnt < 2) return TRUE;
	
	CString strSeShelfCode, strTmpData;
	strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), 0);
	strSeShelfCode.TrimLeft();	strSeShelfCode.TrimRight();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), i);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		if(strSeShelfCode != strTmpData) return FALSE;
	}
	return TRUE;
}

INT CErrorAdjustApi::Restruct049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	BOOL bSameSeShelfCode = FALSE;
	bSameSeShelfCode = IsSameSeShelfCode(pDM_BOOK);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("049"));
	pDM_BOOK->SORT(_T("BB_등록번호"));
	CString strRegNo, strVol, strSeShelfCode, strCopyCode, str049;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strCopyCode = pDM_BOOK->GetCellData(_T("BB_복본기호"), i);
		strRegNo = pDM_BOOK->GetCellData(_T("BB_등록번호"), i);
		strVol = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
		strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), i);
		if(strRegNo.IsEmpty()) continue;
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$l"), strRegNo);
		if(!strVol.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$v"), strVol);
		if(!bSameSeShelfCode && !strSeShelfCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
		if(!strCopyCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$c"), strCopyCode);
	}
	
	if(bSameSeShelfCode)
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty())
		{
			strSeShelfCode = pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), 0);
			if(!strSeShelfCode.IsEmpty())
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
			str049 = _T("0") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}		
	}
	else
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty()) 
		{
			str049 = _T("1") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}
	}
	
	return 0;	
}

INT CErrorAdjustApi::RestructBookBy049MarcTag(CESL_DataMgr *pDM_BOOK, CMarc *pMarc, BOOL b049InfoInsertYN/*=FALSE*/)
{
	BOOL bSameShelfCode = FALSE;
	CString str049, strTmpData, strInd, strVol, strRegNo, strCopyCode, strRepresentShelfCode, strShelfCode, strRegCode;
	m_pParentMgr->m_pInfo->pMarcMgr->GetField(pMarc, _T("049"), str049);
	if(str049.IsEmpty()) return 0;
	
	str049.Replace(SUBFIELD_CODE, '$');
	str049.Replace(FIELD_TERMINATOR, ' ');
	str049.Replace(_T("$l"), _T("#$l"));
	CStringArray arr049;	arr049.RemoveAll();
	ParsingString(_T("#"), str049, &arr049, FALSE);
	INT n049Cnt = arr049.GetSize();
	INT n049SubFieldCnt = -1;
	INT nRegNo;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//===================================================================

	for(INT i=0 ; i<n049Cnt ; i++)
	{
		CStringArray arr049SubField;	arr049SubField.RemoveAll();
		strTmpData = arr049.GetAt(i);
		if(strTmpData.Left(1) == _T("0"))
		{
			bSameShelfCode = TRUE;
			continue;
		}
		else if(strTmpData.Left(1) == _T("1"))
		{
			continue;
		}
		ParsingString(_T("$"), strTmpData, &arr049SubField, FALSE);
		n049SubFieldCnt = arr049SubField.GetSize();
		strVol.Empty();	strCopyCode.Empty();	strShelfCode.Empty();	strRegNo.Empty();
		for(INT j=0 ; j<n049SubFieldCnt ; j++)
		{
			strTmpData = arr049SubField.GetAt(j);
			strTmpData.TrimLeft();	strTmpData.TrimRight();
			strInd = strTmpData.Left(1);
			strInd.MakeLower();
			if(strInd == _T("v")) strVol = strTmpData.Mid(1);
			else if(strInd == _T("c")) strCopyCode = strTmpData.Mid(1);
			else if(strInd == _T("f"))
			{
				strShelfCode = strTmpData.Mid(1);
				if(!strShelfCode.IsEmpty()) strRepresentShelfCode = strShelfCode;
			}
			else if(strInd == _T("l")) 
			{	
				strTmpData = strTmpData.Mid(1);
				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 				strRegCode = strTmpData.Left(2);
// 				if(strTmpData.GetLength() >= 3)
// 				{
// 					nRegNo = _ttoi(strTmpData.Mid(3).GetBuffer(0));
// 					strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);
// 				}
				
				strRegCode = strTmpData.Left(nRegCodeLength);
				if(strTmpData.GetLength() > nRegCodeLength)
				{
					strRegNo = m_pParentMgr->m_pInfo->MakeRegNo(strRegCode,strTmpData.Mid(nRegCodeLength));
				} 
				//===================================================================
			}
		}
		Set049ToBookDM(pDM_BOOK, strRegNo, strVol, strCopyCode, strShelfCode, b049InfoInsertYN, i-1);
	}
	//0490일경우 대표 별치기호를 모든 책에 적용한다.
	if(bSameShelfCode)
	{
		for(i=0 ; i<n049Cnt ; i++)
		{
			pDM_BOOK->SetCellData(_T("BB_별치기호_코드"), strRepresentShelfCode, i);
		}
	}
	
	return 0;
}

INT CErrorAdjustApi::Set049ToBookDM(CESL_DataMgr *pDM_BOOK, CString strRegNo, CString strVol, CString strCopyCode, CString strShelfCode, BOOL b049InfoInsertYN/*=FALSE*/, INT nIdx/*=-1*/)
{
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strSrcVol, strSrcVolCode, strSrcCopyCode, strSrcShelfCode;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strSrcVolCode = pDM_BOOK->GetCellData(_T("BB_권책기호"), i);
		strSrcVol = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
		strSrcCopyCode = pDM_BOOK->GetCellData(_T("BB_복본기호"), i);
		if((strSrcVol == strVol || strSrcVolCode == strVol) && strCopyCode == strSrcCopyCode)
		{
			if(pDM_BOOK->GetCellData(_T("BB_등록번호"), i).IsEmpty())
			{
				pDM_BOOK->SetCellData(_T("BB_등록번호"), strRegNo, i);
				pDM_BOOK->SetCellData(_T("BB_별치기호_코드"), strShelfCode, i);
				break;
			}
		}
	}
	
	return 0;
}

INT CErrorAdjustApi::RecycleDataRestore()
{
	CESL_DataMgr * pDM_INDEX = NULL;
	CESL_DataMgr * pDM_SPECIES = NULL;
	CESL_DataMgr * pDM_BOOK = NULL;

	pDM_INDEX = (CESL_DataMgr*)GetTempPtr(_T("색인DM"));
	pDM_SPECIES = (CESL_DataMgr*)GetTempPtr(_T("종DM"));
	pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("책DM"));

	if(pDM_SPECIES == NULL || pDM_INDEX == NULL || pDM_BOOK == NULL)
		return -1;

	pDM_SPECIES->StartFrame();
	//색인-종 작업상태 조정
	CString strWorkStatus = pDM_INDEX->GetCellData(_T("IBS_작업상태INFO"), 0);
	CString strSpeciesKey = pDM_INDEX->GetCellData(_T("IBS_REC_KEY"), 0);
	if(strWorkStatus.Left(3) == _T("BOL"))
	{
		pDM_INDEX->SetCellData(_T("IBS_종구분값"), _T("1"), 0);
		pDM_SPECIES->SetCellData(_T("BS_작업상태구분"), _T("0"), 0);
	}
	else
	{
		pDM_INDEX->SetCellData(_T("IBS_종구분값"), _T("0"), 0);
		pDM_SPECIES->SetCellData(_T("BS_작업상태구분"), _T("1"), 0);
	}

	CArray<CString, CString> RemoveAliasArray;
	RemoveAliasArray.RemoveAll();
	//RemoveAliasArray.Add(_T("BS_MARC"));
	RemoveAliasArray.Add(_T("IBS_삭제일"));
	RemoveAliasArray.Add(_T("IBS_REC_KEY"));
	INT ids = -1;
	CString strQuery;
	//색인 종 색인 임시테이블 Insert
	ids = pDM_INDEX->MakeQueryByDM(RemoveAliasArray, pDM_INDEX, pDM_INDEX->TBL_NAME, 0, pDM_SPECIES, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	if(ids < 0) return ids;
	ids = pDM_SPECIES->MakeQueryByDM(RemoveAliasArray, pDM_SPECIES, _T("BO_SPECIES_TBL"), 0, pDM_SPECIES);
	if(ids < 0) return ids;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	strQuery.Format(
// 		_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 		_T("ESL_SEQ.NEXTVAL, %s, 'I', SYSDATE);"),
// 		strSpeciesKey);
// 	ids = pDM_SPECIES->AddFrame(strQuery);
// 	if(ids < 0) return ids;
	//=====================================================
	

	//책 Insert
	CString strRegNo, strAccessionRecKey, strBookKey;
	INT nBookCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nBookCnt ; i++)
	{
		ids = pDM_BOOK->MakeQueryByDM(RemoveAliasArray, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_SPECIES);
		if(ids < 0) return ids;
		strRegNo = pDM_BOOK->GetCellData(_T("BB_등록번호"), i);
		strAccessionRecKey = pDM_BOOK->GetCellData(_T("BB_원부번호KEY"), i);
		strBookKey = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		strQuery.Empty();
		if(!strRegNo.IsEmpty() && !strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO='%s', ACCESSION_REC_KEY=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strRegNo, strAccessionRecKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		else if(strRegNo.IsEmpty() && !strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY=%s, LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strAccessionRecKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		else if(!strRegNo.IsEmpty() && strAccessionRecKey.IsEmpty())
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strRegNo, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strBookKey);
		}
		if(!strQuery.IsEmpty())
		{
			ids = pDM_SPECIES->AddFrame(strQuery);
			if(ids < 0) return ids;
		}
		if(!strRegNo.IsEmpty())
		{
			ids = DeleteToDropAccessionNoTbl(strRegNo, pDM_SPECIES);
			if(ids < 0) return ids;
		}
	}

	//책정보로 색인 재구축
	strQuery.Format(
		_T("UPDATE IDX_BO_TBL SET	")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("LAST_WORK = '%s' where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	ids = pDM_SPECIES->AddFrame(strQuery);
	if(ids < 0) return ids;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	strQuery.Format(
// 		_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 		_T("WHERE REC_KEY=%s);"),
// 		strSpeciesKey);
// 	pDM_SPECIES->AddFrame(strQuery);
	//=====================================================
	
	// ECOSEARCH용 색인 생성
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
	}
	
	// 재활용에 있는 종/책 삭제
	strQuery.Format(_T("DELETE FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
	ids = pDM_SPECIES->AddFrame(strQuery);
	if(ids < 0) return ids;

	for(i=0 ; i<nBookCnt ; i++)
	{
		strBookKey = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		strQuery.Format(_T("DELETE FROM BO_DELETE_BOOK_TBL WHERE REC_KEY=%s;"), strBookKey);
		ids = pDM_SPECIES->AddFrame(strQuery);
		if(ids < 0) return -1;
	}

	// 마크 갱신
	CString strMarc = pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	pDM_SPECIES->StartFrame();
	pDM_SPECIES->InitDBFieldData();
	pDM_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM_SPECIES->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	
	// SENDFRAME
	ids = pDM_SPECIES->SendFrame();
	if(ids < 0) return ids;
	ids = pDM_SPECIES->EndFrame();
	if(ids < 0) return ids;
	
	pDM_SPECIES->EndFrame();
	return 0;
}

INT CErrorAdjustApi::DeleteToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out)
{
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
	CString query, strTmpData;
	
	CString strRecKey;
	query.Format(
		_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE PUBLISH_FORM_CODE='%s' AND REG_CODE='%s' AND REG_NO=%d;"),
		_T("MO"),
		strCode,
		nNumber);
	INT ids = pDM_Out->AddFrame(query);
	if(ids < 0) return ids;
	return 0;
}

INT CErrorAdjustApi::SetRegNoInLoanInfo()
{
	CESL_DataMgr * pDM_INDEX = NULL;
	CESL_DataMgr * pDM_BOOK = NULL;
	pDM_BOOK = (CESL_DataMgr *)GetTempPtr(_T("책DM"));
	pDM_INDEX = (CESL_DataMgr *)GetTempPtr(_T("색인DM"));
	if(pDM_INDEX == NULL || pDM_BOOK == NULL) return -1;

	CString strSpeciesKey, strQuery;
	strSpeciesKey = pDM_INDEX->GetCellData(_T("IBS_REC_KEY"), 0);
	strQuery.Format(
		_T("SELECT DISTINCT BOOK_KEY, REG_NO FROM LH_STORAGE_V01 WHERE SPECIES_KEY=%s AND STATUS IN ('0', '1', '2')"),
		strSpeciesKey);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM_INDEX->CONNECTION_INFO);
	INT ids = tmpDM.RestructDataManager(strQuery);

	INT nRowCnt = tmpDM.GetRowCount();
	INT nBookCnt = pDM_BOOK->GetRowCount();
	if(nRowCnt == 0 || nBookCnt == 0) return 0;
	
	CString strBookKey, strRegNo;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strBookKey = tmpDM.GetCellData(i, 0);
		strRegNo = tmpDM.GetCellData(i, 1);
		for(INT j=0 ; j<nBookCnt ; j++)
		{
			if(strBookKey == pDM_BOOK->GetCellData(_T("BB_책KEY"), j))
			{
				pDM_BOOK->SetCellData(_T("BB_등록번호"), strRegNo, j);
			}
		}
	}
	return 0;
}

INT CErrorAdjustApi::SetAppendixBookFlag()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL) return -1;
	
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, 1);
	}

	CString strQuery;
	strQuery.Format(
		_T("UPDATE BO_BOOK_TBL SET BOOK_APPENDIX_FLAG='B', LAST_WORK='%s' WHERE ")
		_T("BOOK_APPENDIX_FLAG IS NULL AND WORKING_STATUS < 'BOL' AND WORKING_STATUS > 'BOR';"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG));
	INT ids = pDM->StartFrame();
	if(ids < 0) return -1;
	ids = pDM->AddFrame(strQuery);
	if(ids < 0) return -1;
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(1);
	}
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();

	return 0;
}

INT CErrorAdjustApi::RestructRequestIndexData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strTmpData, strQuery;
	CArray <CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("비치희망KEY"));
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strTmpData = pDM->GetCellData(_T("본표제"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("본표제인덱스"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("저작자"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("저작자인덱스"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("발행자"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("발행자인덱스"), strTmpData, i);
		strTmpData = pDM->GetCellData(_T("신청자"), i);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pDM->SetCellData(_T("신청자인덱스"), strTmpData, i);

		strTmpData = pDM->GetCellData(_T("비치희망KEY"), i);
		
		ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, i, pDM, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
		if(ids < 0) return -1;
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return 0;
}

INT CErrorAdjustApi::AdjustSpeciesError()
{
	INT ids;
	INT nSpeciesType = -1;		
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	
	if(nSpeciesType == 0 || nSpeciesType == 1)
		ids = AdjustSpeciesInfoError();
	else if(nSpeciesType == 2)
		ids = DeleteDupAddSpeciesInfo();
	else if(nSpeciesType == 3)
		ids = SyncIndexToBookWorkingStatus();
	return ids;
}

INT CErrorAdjustApi::SyncIndexToBookWorkingStatus()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strKey, strQuery;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(!strKey.IsEmpty())
		{
			//1. 검색엔진 동기화를 위해 큐테이블에 로그 쓰기
			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 			strQuery.Format(
// 				_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 				_T("ESL_SEQ.NEXTVAL, %s, 'I', SYSDATE);"),
// 				strKey);
// 			pDM->AddFrame(strQuery);	
			//=====================================================
			
			//색인 자료상태 Update
			strQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS=TESTWORKSTATUS(%s) WHERE REC_KEY = %s;"), strKey, strKey);
			pDM->AddFrame(strQuery);
			//ECO_SEARCH에 적용
			if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strKey);
				pDM->AddFrame(strQuery);
			}
		}
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::DeleteDupAddSpeciesInfo()
{
	CString strQuery;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	if(pDM == NULL || pGrid == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CString strKey, strValue;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(!strKey.IsEmpty())
		{
			strQuery.Format(
				_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), strKey);
			pDM->GetOneValueQuery(strQuery, strValue);
			if(_ttoi(strValue.GetBuffer(0)) > 0)
			{
				strQuery.Format(_T("[%s : %s : %d] 자료는 책정보를 포함하고 있습니다. 다시 확인해 주십시요"),
					pDM->GetCellData(_T("IBS_본표제"), i), pDM->GetCellData(_T("IBS_종KEY"), i), i);
			}
			else
			{
				//1. 검색엔진 동기화를 위해 큐테이블에 로그 쓰기
				//=====================================================
				//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 				strQuery.Format(
// 					_T("INSERT INTO idx_tmp_bo_tbl VALUES (")
// 					_T("ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"),
// 					strKey);
// 				pDM->AddFrame(strQuery);
				//=====================================================
				
				//종 테이블에서 책 없는 자료 삭제
				strQuery.Format(
					_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY = %s;"), strKey);
				pDM->AddFrame(strQuery);
				//구입정보삭제
				strQuery.Format(
					_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE SPECIES_KEY=%s;"), strKey);
				pDM->AddFrame(strQuery);
				//기증정보삭제
				strQuery.Format(
					_T("DELETE FROM BO_DONINFO_TBL WHERE SPECIES_KEY=%s;"), strKey);
				pDM->AddFrame(strQuery);
				//색인 테이블에서 책 없는 자료 삭제
				strQuery.Format(
					_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY = %s;"), strKey);
				pDM->AddFrame(strQuery);
				//ECO_SEARCH에 적용
				if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
				{
					strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strKey);
					pDM->AddFrame(strQuery);
				}
				//목차초록 삭제
				ids = BO_TOC_ABSTRACT_DELETE(pDM, strKey);
				//원본자료 복본플래그 재설정
				strQuery.Format(
					_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG=NULL WHERE REC_KEY IN ")
					_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
					strKey);
				pDM->AddFrame(strQuery);
				strQuery.Format(
					_T("UPDATE IDX_BO_TBL SET DUP_YN=NULL WHERE REC_KEY IN ")
					_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
					strKey);
				pDM->AddFrame(strQuery);
				if(ids < 0) return -1;			
			}
		}
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::AdjustSpeciesInfoError()
{
	INT nSpeciesType = -1;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	if(pDM == NULL || pGrid == NULL || nSpeciesType < 0) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	CString strKey;
	// StartFrame
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		
		strKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(!strKey.IsEmpty())
		{
			ids = RestructSpeciesInfo(strKey, pDM, nSpeciesType);
		}

		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::RestructSpeciesInfo(CString strKey, CESL_DataMgr * pDM_OUT, INT nSpeciesType)
{
	if(pDM_OUT == NULL) return -1;

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	if(nSpeciesType == 0)
		DM_SPECIES.InitDMPKFromFile(_T("DMUP_단행_종_종정보"));
	else if(nSpeciesType == 1)
		DM_SPECIES.InitDMPKFromFile(_T("DMUP_단행_종API_삭제종정보"));
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		
	CESL_DataMgr DM_IDX_BO;
	DM_IDX_BO.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	DM_IDX_BO.InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
	DM_IDX_BO.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	if(nSpeciesType == 0)
		DM_BOOK.InitDMPKFromFile(_T("DMUP_단행_종_책정보"));
	else if(nSpeciesType == 1)
		DM_BOOK.InitDMPKFromFile(_T("DMUP_단행_종API_삭제책정보"));
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids;
	CString strQuery, strMarc, strTmpData;
	strQuery.Format(_T("REC_KEY = %s"), strKey);
	ids = DM_IDX_BO.RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	strQuery.Format(_T("SPECIES_KEY = %s"), strKey);
	ids = DM_BOOK.RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	if(DM_IDX_BO.GetRowCount() == 0 || DM_BOOK.GetRowCount() == 0) return 0;
	strMarc.Empty();
	ids = MakeMarc(&DM_IDX_BO, &DM_BOOK, strMarc);
	if(ids < 0) return -1;

	DM_SPECIES.InsertRow(-1);
	DM_SPECIES.SetCellData(_T("BS_MARC"), strMarc, 0);
	m_pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("제어자료구분"), strTmpData);
	DM_SPECIES.SetCellData(_T("BS_제어자료구분"), strTmpData, 0);
	strTmpData.Empty();
	
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_형식구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_형식구분_코드"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_자료구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_자료구분_코드"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_보조등록구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_보조등록구분_코드"), strTmpData, 0);	
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_업무구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_업무구분"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_BOOK.GetCellData(_T("BB_이용제한구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_이용제한구분_코드"), strTmpData, 0);	
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_이용대상구분_코드"), 0);
	DM_SPECIES.SetCellData(_T("BS_이용대상구분_코드"), strTmpData, 0);
	strTmpData.Empty();
	strTmpData = DM_IDX_BO.GetCellData(_T("IBS_분류표구분"), 0);
	DM_SPECIES.SetCellData(_T("BS_분류표구분"), strTmpData, 0);
	
	//종정보설정
	DM_SPECIES.SetCellData(_T("BS_종KEY"), strKey, 0);
	DM_SPECIES.SetCellData(_T("BS_작업자"), m_pParentMgr->GetUserID(), 0);
	DM_SPECIES.SetCellData(_T("BS_작업상태구분"), _T("1"), 0);
	DM_SPECIES.SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	DM_SPECIES.SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	DM_SPECIES.SetCellData(_T("BS_입력일"), GetTodayDate(), 0);

	CArray <CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	ids = DM_SPECIES.MakeQueryByDM(RemoveAliasList, &DM_SPECIES, DM_SPECIES.TBL_NAME, 0, pDM_OUT);

	return ids;
}

INT CErrorAdjustApi::MakeMarc(CESL_DataMgr *pDM_IDX_BO, CESL_DataMgr *pDM_BOOK, CString &strMarc)
{
	INT nSpeciesMarcRelation = 0;
	CString SpeciesMarcRelation_Species[255];
	CString SpeciesMarcRelation_Marc[255];
	
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_발행년");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_면장수");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_물리적특성");	SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_크기");			SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_딸림자료");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$e");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_면장수");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_물리적특성");	SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_크기");			SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$c");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_딸림자료");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("300$e");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_판사항");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("250$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_발행지");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$a");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("IBS_발행자");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$b");			nSpeciesMarcRelation++;
	SpeciesMarcRelation_Species[nSpeciesMarcRelation] = _T("BB_발행년");		SpeciesMarcRelation_Marc[nSpeciesMarcRelation] = _T("260$c");			nSpeciesMarcRelation++;

	CMarc marc;
	if (pDM_IDX_BO == NULL) return -1;
	if (pDM_BOOK == NULL) return -3;
	
	INT nIdxRowIdx = 0;
	INT nREPRESENTBOOKRowIdx = 0;
	CString strDM, strData1, strData2, strData3;

	CESL_DataMgr DM_VOLUME;
	DM_VOLUME.SetCONNECTION_INFO(pDM_IDX_BO->CONNECTION_INFO);
	DM_VOLUME.InitDMPKFromFile(_T("DMUP_단행_권_권정보"));
	DM_VOLUME.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	CONVERT_BOOK_to_VOLUME(pDM_BOOK, &DM_VOLUME);
	
	//마크메니져 그룹설정
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_형식구분_코드"), nIdxRowIdx);
	if ( strData1 == _T("TX") )
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("단행"));
	else if ( strData1 == _T("VM") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("비도서_시청각자료"));
	else if ( strData1 == _T("CF") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("비도서_컴퓨터파일"));
	else if ( strData1 == _T("MP") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("비도서_지도자료"));
	else if ( strData1 == _T("MU") ) 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("비도서_녹음자료"));
	else 
		m_pParentMgr->m_pInfo->pMarcMgr->SetMarcGroup(_T("단행"));
	
	// 리더 정보 세팅 (긴급추가임)
	CString strLeader;
	marc.GetLeader(strLeader);
	// 레코드 상태 없으면 넣는다. 5번째
	if (strLeader.GetAt(5) == ' ') strLeader.SetAt(5, 'n');
	// 레코드 형태 없으면 넣는다. 6번째
	if (strLeader.GetAt(6) == ' ') strLeader.SetAt(6, 'a');
	// 서지수준 없으면 넣는다. 7반째
	if (strLeader.GetAt(7) == ' ') strLeader.SetAt(7, 'm');

	// 일반적인 데이타 처리
	for (INT i = 0; i < nSpeciesMarcRelation; i++) {
		strDM = SpeciesMarcRelation_Species[i].Left(2);
		strData1.Empty();
		if (strDM == _T("IB")) pDM_IDX_BO->GetCellData(SpeciesMarcRelation_Species[i], nIdxRowIdx, strData1);
		if (strDM == _T("BB")) pDM_BOOK->GetCellData(SpeciesMarcRelation_Species[i], nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, SpeciesMarcRelation_Marc[i], strData1, _T(""), MODIFY_ITEM);
		else m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, SpeciesMarcRelation_Marc[i]);
	}
	//008발행년 필드 적용
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("008"));
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_발행년"), 0);
	Apply_MARC_950(strData1, strData2, strData3);
	if(strData2.GetLength() == 4)
	{	
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("008@6"), _T("s") );
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem( &marc, _T("008@7-10"), strData2);		
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("008@38-39"), _T("  ") );
	}
	
	// 본표제
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_본표제"), nIdxRowIdx, strData1);	
	Set_TITLE(&marc, strData1);
	// 저작자
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_저작자"), nIdxRowIdx, strData1);
	if (strData1.GetLength() == 0) pDM_BOOK->GetCellData(_T("BB_저작자"), nREPRESENTBOOKRowIdx, strData1);
	Set_AUTHOR(&marc, strData1);

	// 편권차
	strData1 = pDM_IDX_BO->GetCellData(_T("IBS_편권차"), nIdxRowIdx);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("245$n"));
	if(!strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("245$n"), strData1);
	}
	
	// 편제(권서명)
	if (DM_VOLUME.GetRowCount() == 1) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("245$p"));
		strData1 = _T("");
		pDM_BOOK->GetCellData(_T("BB_편제명부록명"), nREPRESENTBOOKRowIdx, strData1);
		if (strData1.GetLength() > 0) 
		{
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("245$p"), strData1);
		}
	}
	
	//발행자
	strData1 = _T("");
	pDM_IDX_BO->GetCellData(_T("IBS_발행자"), nIdxRowIdx, strData1);
	Set_PUBLIHSER(&marc, strData1);

	Apply_MARC_700Author(&marc);

	SetISBN_to_Marc_ByVolume(&marc, &DM_VOLUME, _T("020"), _T("  "));		// IBS_낱권_ISBN is 020 blank blank
	// 특수한 데이타 처리 - 020(세트)
	SetISBN_to_Marc(&marc, pDM_IDX_BO, nIdxRowIdx, _T("020"), _T("1 "));		// IBS_세트_ISBN is 020 1 blank

	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if(ids < 0) return -1;
	
	return 0;
}

INT CErrorAdjustApi::CONVERT_BOOK_to_VOLUME(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_VOLUME)
{
	if (!pDM_BOOK || !pDM_VOLUME) return -35001;
	
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	INT nRowCount = pDM_BOOK->GetRowCount();
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("D")) continue;
		if (pDM_BOOK->GetCellData(_T("BB_책부록플래그"), i) == _T("A")) continue;
		IsNewVolume = TRUE;
		ids = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i, curVOL_SORT);
		for (j = 0; j < i; j++) {
			if (pDM_BOOK->GetCellData(_T("BB_책부록플래그"), j) == _T("A")) continue;
			if (pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), j) == _T("D")) continue;
			ids = pDM_BOOK->GetCellData(_T("BB_권일련번호"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}
	
	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx = 0;
	CString strData;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_VOLUME->InsertRow(-1);
		// BOOK의 한 Row를 VOLUME으로 복사한다.
		CopyDMRowToDMByAlias(pDM_BOOK, CopyRowIdxList.GetAt(i), pDM_VOLUME, RowIdx);
		// 복사된 VOLUME의 같은 책수를 구하여 VOLUME의 같은책수UDF 에 넣어준다.
		ids = pDM_VOLUME->GetCellData(_T("BB_편권차"), RowIdx, curVOL_SORT);
		strData.Format(_T("%d"), GetEqualBookCount(pDM_BOOK, curVOL_SORT));
		ids = pDM_VOLUME->SetCellData(_T("BB_책수"), strData, RowIdx);
		
		RowIdx++;
	}
	
	return 0;
}

INT CErrorAdjustApi::Set_TITLE(CMarc *pMarc, CString strData)
{
	if (pMarc == NULL) return -1;
	//if (strData.GetLength() == 0) return 0;
	//서명관련 245필드  $a, $x, $b 클리어
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$a"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$x"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$b"));
	//m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("245"));
	
	INT pos = strData.Find(_T("="), 0);
	CString strData1, strData2, strData3;
	if (pos < 0) 
	{
		strData1 = strData;
		pos = strData.Find(_T(":"), 0);
		if(pos < 0)
		{
			pos = strData.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
		}
		else
		{
			strData1 = strData.Left(pos);
			strData2 = strData.Mid(pos+1);
			pos = strData1.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData2, _T(""), MODIFY_ITEM);
		}
	}
	else 
	{
		strData1 = strData.Left(pos);
		strData2 = strData.Mid(pos+1);
		pos = strData1.Find(_T("("));
		if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
		else 
		{
			pos = strData.Find(_T(")"));
			if(pos < 0) 
			{
				strData1.Replace(_T("("), _T(""));
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			}
			else
			{
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
			}
		}
		pos = strData2.Find(_T(":"), 0);
		if(pos < 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData2, _T(""), MODIFY_ITEM);
		else 
		{
			strData1 = strData2.Left(pos);
			strData3 = strData2.Mid(pos+1);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData1, _T(""), MODIFY_ITEM);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData3, _T(""), MODIFY_ITEM);
		}
	}
	return 0;
}

INT CErrorAdjustApi::Set_AUTHOR(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$d"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$e"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(';', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		if (i == 0) 
		{
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$d"), strData1);
		}
		else
		{
			if (strData1.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$e"), strData1);
		}
		if (pos == -1) break;
	}
	return 0;
}

INT CErrorAdjustApi::Set_PUBLIHSER(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("260$b"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(':', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("260$b"), strData1);
		if (pos == -1) break;
	}
	return 0;
}

INT CErrorAdjustApi::Apply_MARC_700Author(CMarc *pMarc)
{
	if (pMarc == NULL) return -1;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("700"));

	CString strData, strTmpData;
	CArray<CString, CString&> StrArrayFrom;
	CArray<CString, CString&> StrArrayOrigin1;
	CArray<CString, CString&> StrArrayOrigin2;

	INT ids = -1;
	INT nCountFrom = -1;
	INT nCountOrigin1 = -1;
	INT nCountOrigin2 = -1;

	/**
		245$e -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// 존재하는지 체크한다.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	/**
		245$d -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// 존재하는지 체크한다.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	return 0;
}

INT CErrorAdjustApi::SetISBN_to_Marc_ByVolume(CMarc *pMarc, CESL_DataMgr *pDM_VOLUME, CString strTag, CString strInd)
{
	// _T("020  ") 를 모두 날린다.
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("020"), ' ', ' ');
	
	CString strData1=_T(""), strData2=_T(""), strData3=_T(""), strData4= _T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T(""), strMarcData4=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol, strCurrencyCode, strTmpData;
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	Apply_MARC_950(strData1, strTmpData, strCurrencyCode);
	
	INT nVolumes = pDM_VOLUME->GetRowCount();
	for (INT i = 0; i < nVolumes; i++) {
		strNewIndexData = _T("");
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_VOLUME->GetCellData(_T("BB_낱권ISBN"), i, strData1);
		pDM_VOLUME->GetCellData(_T("BB_편권차"), i, strVol);
		strData1.Replace(_T("-"), _T(""));
		if(!strVol.IsEmpty() && !strData1.IsEmpty()) strData1 += _T("(") + strVol + _T(")");
		pDM_VOLUME->GetCellData(_T("BB_낱권ISBN부가기호"), i, strData2);
		pDM_VOLUME->GetCellData(_T("BB_낱권오류ISBN"), i, strData3);
		strPrice = _T("");
		pDM_VOLUME->GetCellData(_T("BB_가격"), i, strData4);
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		else continue;
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
		if (strData4.GetLength() > 0) strMarcData4.Format(_T("$c%s%s"), strCurrencyCode, strData4);
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0 && strData4.GetLength() == 0) 
		{
			;
		} 
		else 
		{
			strNewIndexData.Format(_T("020  %s%s%s%s"), strMarcData1, strMarcData2, strMarcData3, strMarcData4);
		}
		if (strNewIndexData.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CErrorAdjustApi::SetISBN_to_Marc(CMarc *pMarc, CESL_DataMgr *pDM_IDX_BO, INT nRowIdx, CString strTag, CString strInd)
{
	CString strData1=_T(""), strData2=_T(""), strData3=_T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol;
	if (strTag == _T("020") && strInd == _T("  ")) {
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_낱권ISBN"), nRowIdx, strData1);
		strData1.Replace(_T("-"), _T(""));
		pDM_IDX_BO->GetCellData(_T("IBS_낱권ISBN부가기호"), nRowIdx, strData2);
		pDM_IDX_BO->GetCellData(_T("IBS_낱권오류ISBN"), nRowIdx, strData3);
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) {
			;
		} else {
			strNewIndexData.Format(_T("020  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
		}
	}
	if (strTag == _T("020") && strInd == _T("1 ")) {
		strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
		strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
		pDM_IDX_BO->GetCellData(_T("IBS_세트_ISBN1"), nRowIdx, strData1);
		strData1.Replace(_T("-"), _T(""));
		if(!strData1.IsEmpty()) strData1 += _T("(세트)");
		pDM_IDX_BO->GetCellData(_T("IBS_세트_ISBN2"), nRowIdx, strData2);
		pDM_IDX_BO->GetCellData(_T("IBS_세트_ISBN3"), nRowIdx, strData3);
		
		if (strData1.GetLength() > 0) strMarcData1.Format(_T("$a%s"), strData1);
		if (strData2.GetLength() > 0) strMarcData2.Format(_T("$g%s"), strData2);
		if (strData3.GetLength() > 0) strMarcData3.Format(_T("$z%s"), strData3);
		
		if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) {
			;
		} else {
			strNewIndexData.Format(_T("0201  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
		}
	}
	
	CArray <CString, CString&> Array;
	Array.RemoveAll();
	CString strField;
	
	CString strTmpInd;
	m_pParentMgr->m_pInfo->pMarcMgr->GetField(pMarc, strTag, strField, &Array);
	INT nSize = Array.GetSize();
	if (nSize == 0) {		// Marc has not any isbn field data
		m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	} else {						// Marc has some isbn field data
		BOOL bIsUpdate = FALSE;
		for (INT i = 0; i < nSize; i++) {
			if (Array.GetAt(i).GetLength() < 2) continue;
			strTmpInd = Array.GetAt(i).Left(2);
			
			if (strTmpInd == strInd) {
				strIndexData.Format(_T("%s%s"),  strTag, Array.GetAt(i));
				m_pParentMgr->m_pInfo->pMarcMgr->UpdateField(pMarc, strIndexData, strNewIndexData);
				bIsUpdate = TRUE;
				break;
			}
		}
		if (bIsUpdate == FALSE) m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}

INT CErrorAdjustApi::CopyDMRowToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

INT CErrorAdjustApi::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL)
{
	if (!pDM_BOOK) return 0;
	INT nEqualBookCount = 0;
	CString tempVOL;
	INT nBookCount = pDM_BOOK->GetRowCount();
	INT ids;
	
	for (INT i = 0; i < nBookCount; i++) {
		if (pDM_BOOK->GetCellData(_T("BB_책부록플래그"), i) == _T("A")) continue;
		ids = pDM_BOOK->GetCellData(_T("BB_편권차"), i, tempVOL);
		if (strVOL == tempVOL) nEqualBookCount++;
	}
	
	return nEqualBookCount;
}

INT CErrorAdjustApi::BO_BOOK_DELETE(CString strSpeciesKey,
								CString strAccuKey,
								CString strBookRecKey,
								INT nAcqType,
								CESL_DataMgr * pDM_OUT)
{
	INT nRowIdx = 0;
	
	CString strOption, strQuery, strKey, strRegNo, strMarc, strTmpData, strBookQuery;
	strKey = strSpeciesKey;
	
	if(!CheckHaveOtherBookInfo(pDM_OUT, strSpeciesKey, strAccuKey))    
	{
		return BO_SPECIES_DELETE(strSpeciesKey, strAccuKey, nAcqType, pDM_OUT);
	}
	// 해당 종의 책DM을 구성한다.
	strOption.Format(_T("DELETE BO_BOOK_TBL WHERE REC_KEY=%s;"), strBookRecKey);
	pDM_OUT->AddFrame(strOption);
	//이용제한구분/관리구분/자료실/등록구분 구분값의 구분값 concatenation
	strOption.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("LAST_WORK='%s' where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strSpeciesKey);
	pDM_OUT->AddFrame(strOption);	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	strQuery.Format(
// 		_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 		_T("WHERE REC_KEY=%s);"),
// 		strSpeciesKey);
// 	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	
	//ECOSEARCH를 위해서 색인잡기
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strOption.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strOption);
	}
	
	return 0;
}

BOOL CErrorAdjustApi::CheckHaveOtherBookInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey)
{
	CString strQuery;
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1"))
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ACQ_KEY=%s"),
		strSpeciesKey, strAccuKey);
	else
		strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"),
		strSpeciesKey);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 1) return TRUE;
	else if(tmpDM.GetRowCount() == 1) return FALSE;
	return FALSE;
}

BOOL CErrorAdjustApi::CheckHaveOtherAccuInfo(CESL_DataMgr * pDM, CString strSpeciesKey, CString strAccuKey)
{
	CString strQuery, strTmpData;
	if(strAccuKey.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("(SPECIES_KEY=%s AND ACQ_KEY IS NOT NULL AND ACQ_KEY != -1) ")
			_T("OR (SPECIES_KEY=%s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR')"),
			strSpeciesKey, strSpeciesKey);
	}
	else if(strAccuKey == _T("ALL"))
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("SPECIES_KEY = %s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR'"),
			strSpeciesKey);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ")
			_T("(SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY != %s)) ")
			_T("OR (SPECIES_KEY=%s AND WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOR')"),
			strSpeciesKey, strAccuKey, strSpeciesKey);
	}
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	strTmpData = tmpDM.GetCellData(0, 0);
	INT nTmpData = _ttoi(strTmpData.GetBuffer(0));
	if(nTmpData > 0)
		return TRUE;
	else
		return FALSE;
	
	return FALSE;
}

INT CErrorAdjustApi::BO_SPECIES_DELETE(CString strSpeciesKey, 
								   CString strAccuKey, 
								   INT nAcqType,
								   CESL_DataMgr * pDM_OUT)
{
	if (strSpeciesKey.GetLength() == 0) return -31001;	
	INT nRowIdx = 0;

	INT ids;
	CArray <CString, CString> RemoveAliasList;
	BOOL bCreateSpeciesKey = FALSE;

	CString strOption, strQuery, strKey, strBookQuery, strTmpData;
	strKey = strSpeciesKey;

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	DM_BOOK.InitDMPKFromFile(_T("DMUP_단행_종_책정보"));
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	// 해당 종의 책DM을 구성한다.
	if(!strAccuKey.IsEmpty() && strAccuKey != _T("-1") && strAccuKey != _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND ACQ_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey, strAccuKey);
	else if(strAccuKey == _T("ALL"))
		strOption.Format(_T("SPECIES_KEY = %s AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	else
		strOption.Format(_T("SPECIES_KEY = %s AND (ACQ_KEY IS NULL OR ACQ_KEY = -1) AND (WORKING_STATUS<'BOL' OR WORKING_STATUS>'BOR')"), strSpeciesKey);
	ids = DM_BOOK.RefreshDataManager(strOption);
	if(ids < 0) return -31003;

	//삭제할 책정보가 없다면 return
	if(DM_BOOK.GetRowCount() == 0) return 1000;
	
	// 구성된 Book정보의 DB_FLAG를 _T("D")로 설정한다.
	ids = DM_BOOK.GetRowCount();
	for(INT idx=0 ; idx<ids ; idx++)
	{
		DM_BOOK.SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	
	// 삭제종에 Insert할 Insert문을 구성한다.
	RemoveAliasList.RemoveAll();
		
	if(!CheckHaveOtherAccuInfo(pDM_OUT, strSpeciesKey, strAccuKey))
	{
		strQuery.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return -1;
		strQuery.Format(_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY=%s;"), strSpeciesKey);
		ids = pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return -1;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), strSpeciesKey, _T("D"));	
		//=====================================================
		
		BO_TOC_ABSTRACT_DELETE(pDM_OUT, strSpeciesKey);
	}
	
	// 삭제책정보에 Insert할 Insert문을 구성한다.
	// 삭제책정보를 구성할때 각 책에 부여되어진 등록번호를 CStringArray에 저장한다.
	INT nRowCount = DM_BOOK.GetRowCount();
	MakeBookQueryFrame(&DM_BOOK, strSpeciesKey, strAccuKey, pDM_OUT);
	
	//구입/기증정보 변경
	if(nAcqType == 1 || nAcqType == 2)
	{
		DeleteAccuInfo(pDM_OUT, strAccuKey, nAcqType);
	}
	else if(nAcqType == 0 && !strAccuKey.IsEmpty())
	{
		if(DM_BOOK.GetCellData(_T("BB_수입구분_코드"), 0) == _T("1")) nAcqType = 1;
		else if(DM_BOOK.GetCellData(_T("BB_수입구분_코드"), 0) == _T("2")) nAcqType = 2;
		DeleteAccuInfo(pDM_OUT, strAccuKey, nAcqType);
	}
	
	if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
	}
	
	return 0;
}

INT CErrorAdjustApi::BO_INDEX_TEMP_INSERT(CESL_DataMgr * pDM_OUT, CString strTblName, CString strSpeciesKey, CString strWorkFlag)
{
	if(strSpeciesKey.IsEmpty()) return 0;
	CString strIDXTEMPKEY;
	//pDM_OUT->MakeRecKey(strIDXTEMPKEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	if(!strSpeciesKey.IsEmpty())
		pDM_OUT->AddDBFieldData(_T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
	pDM_OUT->AddDBFieldData(_T("IDX_WORK_FLAG"), _T("STRING"), strWorkFlag);
	pDM_OUT->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), GetTodayDate());
	INT ids = pDM_OUT->MakeInsertFrame(strTblName);
	if(ids < 0) return -1;
	return 0;
}

INT CErrorAdjustApi::MakeBookQueryFrame(CESL_DataMgr *pDM_BOOK, CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT)
{
	INT ids, i;
	CString strQuery, strDBFLAG, strBook_key;
	INT nRowCount = pDM_BOOK->GetRowCount();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	// 이제 I/U/D에 따라서 Insert/Update/Delete를 구성한다.
	CString IsREPRESENT_BOOK_YN;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			pDM_OUT->MakeRecKey(strBook_key); 
			pDM_BOOK->SetCellData(_T("BB_책KEY"), strBook_key, i);
		}
		IsREPRESENT_BOOK_YN = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_대표책여부"), i, IsREPRESENT_BOOK_YN);
		IsREPRESENT_BOOK_YN.TrimLeft();		IsREPRESENT_BOOK_YN.TrimRight();
		IsREPRESENT_BOOK_YN.MakeUpper();
		if (IsREPRESENT_BOOK_YN != _T("Y") && IsREPRESENT_BOOK_YN != _T("N")) pDM_BOOK->SetCellData(_T("BB_대표책여부"), _T("N"), i);
	}
	//APPLY_SetAppendixInfo_to(pDM_BOOK, pDM_BOOK);

	//편권차색인을 만들기 위해서 그리고 ISBN입력시 특수기호를 제거하기 위하여
	CString volIndex, strEAISBN, strBookAppendixFlag;
	//여기까지
	for (i = 0; i < nRowCount; i++) {
		RemoveAliasList.RemoveAll();
		strDBFLAG = _T("");
		ids = pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			RemoveAliasList.Add(_T("BB_원본등록번호"));
			pDM_BOOK->SetCellData(_T("BB_수서정보KEY"), strPurchaseinfo_key, i);
			pDM_BOOK->SetCellData(_T("BB_종KEY"), strSpecies_key, i);
			pDM_BOOK->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_BOOK->SetCellData(_T("BB_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			if (pDM_BOOK->GetCellData(_T("BB_가격"), i).GetLength() == 0) pDM_BOOK->SetCellData(_T("BB_가격"), _T("0"), i);
			//편권차색인을 만듬 과 낱권ISBN의 특수기호 제거함
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_낱권ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_낱권ISBN"), strEAISBN, i);
			volIndex = pDM_BOOK->GetCellData(_T("BB_편제명부록명"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_편제명부록명색인"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				pDM_BOOK->SetCellData(_T("BB_편권차색인"), volIndex, i);
			}
			strBook_key = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFLAG == _T("U")) {
			RemoveAliasList.Add(_T("BB_책KEY"));
			RemoveAliasList.Add(_T("BB_원본등록번호"));
			ids = pDM_BOOK->GetCellData(_T("BB_책KEY"), i, strBook_key);
			pDM_BOOK->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			//편권차색인을 만듬 과 낱권ISBN의 특수기호 제거함
			strEAISBN = pDM_BOOK->GetCellData(_T("BB_낱권ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			pDM_BOOK->SetCellData(_T("BB_낱권ISBN"), strEAISBN, i);			
			volIndex = pDM_BOOK->GetCellData(_T("BB_편제명부록명"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_편제명부록명색인"), volIndex, i);
			}
			volIndex = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				pDM_BOOK->SetCellData(_T("BB_편권차색인"), volIndex, i);
			}
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_BOOK, _T("BO_BOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBook_key);
			strBook_key = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		}
		if (strDBFLAG == _T("D")) {
			ids = pDM_BOOK->GetCellData(_T("BB_책KEY"), i, strBook_key);
			if(strBook_key.IsEmpty()) strBook_key = _T("-1");
			strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY=%s;"), strBook_key);
			pDM_OUT->AddFrame(strQuery);		
		}
	}

	return 0;
}

INT CErrorAdjustApi::InsertToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out, BOOL IsCheckInsert)
{
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d"), strCode, nNumber);
		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	pDM_Out->MakeRecKey(strRecKey);
	query.Format(
		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO) VALUES(%s, 'MO', '%s', %d);"),
		strRecKey,
		strCode,
		nNumber);
	pDM_Out->AddFrame(query);
	return 0;
}

INT CErrorAdjustApi::DeleteAccuInfo(CESL_DataMgr *pDM_OUT, CString strAccuKey, INT nAcqType)
{
	CString strQuery, strTmpData, strPrice, strDiscRate, strSpeciesKey;
	if(strAccuKey == _T("ALL"))
	{
		strSpeciesKey = pDM_OUT->GetCellData(_T("BS_종KEY"), 0);
	}
		
	if(nAcqType == 0) return 0;
	else if(nAcqType == 1) 
	{
		if(!strAccuKey.IsEmpty())
			strQuery.Format(_T("DELETE BO_PURCHASEINFO_TBL WHERE REC_KEY=%s;"), strAccuKey);
	}
	else if(nAcqType == 2)
	{
		if(!strAccuKey.IsEmpty())
			strQuery.Format(_T("DELETE BO_DONINFO_TBL WHERE REC_KEY=%s;"), strAccuKey);
	}
	
	pDM_OUT->AddFrame(strQuery);
	return 0;	
}

INT CErrorAdjustApi::DeleteDataBook()
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CString strSpeciesKeyAlias, strAccuKeyAlias, strBookKeyAlias, strRegNoAlias, strTmpData;
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));	
	GetTempData(_T("종키알리아스"), strSpeciesKeyAlias);
	GetTempData(_T("수서정보키알리아스"), strAccuKeyAlias);
	GetTempData(_T("책키알리아스"), strBookKeyAlias);
	
	INT nCnt = 0;
	
	if(!pDM || !pGrid) return -1000;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1000;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}
	pDM->StartFrame();
	CString strSpeciesKey, strAccuKey, strBookRecKey;
	while(i >= 0)
	{	
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}
		pDM->GetCellData(strSpeciesKeyAlias, i, strSpeciesKey);
		pDM->GetCellData(strAccuKeyAlias, i, strAccuKey);
		pDM->GetCellData(strBookKeyAlias, i, strBookRecKey);
		ids = BO_BOOK_DELETE(strSpeciesKey, strAccuKey, strBookRecKey, 0, pDM);
		if(ids < 0) return -1;
		ArrIdx.Add(i);
		if(nCnt == 50)
		{
			ids = pDM->SendFrame();
			if(ids < 0) return ids;
			ids = pDM->EndFrame();
			if(ids < 0) return ids;
			pDM->StartFrame();
			nCnt = 0;
		}
		else
			nCnt++;
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	for(i=ArrIdx.GetSize()-1 ; i>=0 ; i--)
	{
		pDM->DeleteRow(ArrIdx.GetAt(i));
	}
	
	return 0;
}

INT CErrorAdjustApi::MarcUpdate()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CProgressCtrl * pPrgCtrl = (CProgressCtrl*)GetTempPtr(_T("ProgressCTRL"));
	if(pDM == NULL || pGrid == NULL) return -1;
		
	CMarcAdjustCondition Cond(m_pParentMgr);
	CMarcAdjustDlg dlg(m_pParentMgr);
	dlg.SetCondition(&Cond);
	if(IDOK != dlg.DoModal()) return 0;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT nSelCnt = pGrid->SelectGetCount();
	if(pPrgCtrl)
	{
		pPrgCtrl->SetPos(0);
		pPrgCtrl->SetRange(0, nSelCnt);
	}	
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	
	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	// StartFrame
	pDM->StartFrame();
	CStringArray ArrData;
	
	CString strMarc, strKey, strQuery, strTmpData;
	
	CMarc marc;
	pDM->StartFrame();
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			ids = pPrgCtrl->GetPos();
			pPrgCtrl->SetPos(++ids);
		}

		ids = Cond.Update(pDM, i);
		if(ids >= 0)
			ArrIdx.Add(i);
		
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	pDM->EndFrame();
	DeleteSelectRow(pDM, &ArrIdx);
	return ids;
}
