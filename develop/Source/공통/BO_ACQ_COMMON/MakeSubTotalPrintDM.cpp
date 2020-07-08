// MakeSubTotalPrintDM.cpp: implementation of the CMakeSubTotalPrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSubTotalPrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSubTotalPrintDM::CMakeSubTotalPrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = TRUE;
	m_bKOLAS2Format = FALSE;
}

CMakeSubTotalPrintDM::~CMakeSubTotalPrintDM()
{
	if(m_bDeletPrintBookDM && m_pPrintBookDM)
	{
		delete m_pPrintBookDM;
		m_pPrintBookDM = NULL;
	}
}

INT CMakeSubTotalPrintDM::InitMakePrintDM()
{
	if(m_pPrintDM == NULL) return -1;

	m_pPrintBookDM = new CESL_DataMgr;
	if(m_pPrintBookDM == NULL) return -1;
	m_pPrintBookDM->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	m_pPrintBookDM->InitDMPKFromFile(m_strBookDMAlias);
	m_pPrintBookDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	if(m_nPrintType < 0) return -1;
	else if(m_bKOLAS2Format)
	{
		m_IgnoreAlias.Add(_T("UDF_차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
		m_strTotalInfoTitle = _T("UDF_차수정보");
		m_strTotalSpeciesCntAlias = _T("UDF_출판사항");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
	}
	else if(m_nPrintType == BO_ACQ_ORDER_DATA_DIVNO || m_nPrintType == BO_ACQ_ORDER_DATA_PROVIDER || m_nPrintType == BO_ACQ_ORDER_DATA_SUBJECTCODE)
	{
		m_IgnoreAlias.Add(_T("UDF_차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
		m_strTotalInfoTitle = _T("UDF_차수정보");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
	}
	else if(m_nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nPrintType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE)
	{
		m_IgnoreAlias.Add(_T("UDF_차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
		m_strTotalInfoTitle = _T("UDF_차수정보");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
	}
	else if(m_nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || m_nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
		m_nPrintType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
	{
		m_IgnoreAlias.Add(_T("UDF_차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
		m_strTotalInfoTitle = _T("UDF_차수정보");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
	}
	return 0;
}

INT CMakeSubTotalPrintDM::MakePrintDM()
{
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pSpeciesDM = NULL;
	if(m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL)
		return -1;

	if(!CheckTotalInfoAlias())
	{
		if(IDNO == m_pParentMgr->ESLAfxMessageBox(_T("경고:합계정보를 나타낼 알리아스를 정의하지 않았습니다.\n출력작업을 계속 진행 하시겠습니까?"), MB_YESNO))
			return 1000;
	}

	CString strQuery;
	strQuery = GetGroupCollectionQuery();
	if(strQuery.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("주어진 그룹정보로 쿼리를 생성할 수 없습니다. 다시 검색해 주십시요."));
		return 1000;
	}
	
	//프로그래스 컨트롤을 보여준다.
	ShowDataProcessingDlg();
	
	pBookDM = new CESL_DataMgr;
	if(pBookDM == NULL) return -1;
	pBookDM->SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	pBookDM->InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	pBookDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	pSpeciesDM = new CESL_DataMgr;
	if(pSpeciesDM == NULL) return -1;
	pSpeciesDM->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	pSpeciesDM->InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	pSpeciesDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	//책 검색
	INT ids;
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 책정보를 구축중입니다."));
		ids = pBookDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	}
	else 
		ids = pBookDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	//종자료 구축
	pSpeciesDM->FreeData();
	ids = MakeSpeciesDM(pSpeciesDM, pBookDM, FALSE);

	// 종자료정렬
	BOOL bOrderPrint = FALSE;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	if(m_nPrintType >= BO_ACQ_ORDER_DATA_DIVNO)
	{
		strAlias[nCondColumns] = _T("BP_발주일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
		bOrderPrint = TRUE;
	}
	else
	{
		strAlias[nCondColumns] = _T("BP_일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
		bOrderPrint = FALSE;
	}
	
	pSpeciesDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	
	// UDF컬럼들을 설정한다.
	SetUDFDMFieldData(pSpeciesDM, bOrderPrint);
	
	//SubTotal PrintDM구축
	CGroupInfo * pGroup = NULL;
	INT nRowCnt = pSpeciesDM->GetRowCount();
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);
		ids = MakeSubTotalRow(pSpeciesDM, pGroup);
		if(ids < 0) return -1;
		if(!m_strSortCondition.IsEmpty())
		{
			ids = MakeSubTotalRowByGroup(pSpeciesDM, pGroup);
			if(ids < 0) return -1;
		}
	}

	// 출력용 권DM생성
	ids = MakeVolumeDM(m_pPrintBookDM, pBookDM, _T("BP_수서정보KEY"));
	if(ids < 0) return -1;
	CString strTmpData;
/*
	for(i=0 ; i<m_pPrintBookDM->GetRowCount() ; i++)
	{
		strTmpData.Format(_T("%d : %s"), i+1, m_pPrintBookDM->GetCellData(_T("BP_수서정보KEY"),  i));
		FileLog(strTmpData);	
	}
	FileLog(_T("=====================================================================\n\n"));	
*/
	
	// 권 DM을 구분 알리아스로 정렬한다.
	nCondColumns = 0;
	CString strAlias1[2];
	INT nCond1[2];
	strAlias1[nCondColumns] = _T("BP_수서정보KEY");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias1[nCondColumns] = _T("BB_권일련번호");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintBookDM->SORT(nCondColumns, strAlias1, nCond1, ESL_DATAMGR_SORT_ASC);
	
	// 출력DM를 Clear시킨다.
	m_pPrintDM->FreeData();
	
	// 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
	nRowCnt = pSpeciesDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strKey = pSpeciesDM->GetCellData(_T("BP_수서정보KEY"), i);
		m_pPrintDM->InsertRow(-1);
		ids = CopyDM(pSpeciesDM, i, m_pPrintDM, m_pPrintDM->GetRowCount()-1);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintBookDM, m_pPrintDM->GetRowCount()-1, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
	}

	//Kolas2출력물에 맞게 데이터 조정
	if(m_bKOLAS2Format)
	{
		ids = SetKOLAS2FormatData(m_pPrintDM);
		if(ids < 0) return -1;
	}
	
	return 0;
}

CString CMakeSubTotalPrintDM::GetGroupCollectionQuery()
{
	if(m_pGroupCollection->GetGroupCount() == 0)
		return _T("");
	
	CString strQuery, strGroupInfo;
	CString strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	if(strWorkCode.IsEmpty())
		strQuery.Format(
		_T("I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE IS NULL"),
		m_pGroupCollection->GetAcqYear(), strGroupInfo);
	else
		strQuery.Format(
		_T("I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE='%s'"),
		m_pGroupCollection->GetAcqYear(), strGroupInfo, strWorkCode);
	return strQuery;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRow(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup, CString strExtraField/*=_T("")*/, CString strExtraValue/*=_T("")*/)
{
	INT nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	INT nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	if(nStartIdx < 0 || nLastIdx < 0) return -1;

	CString strTmpData, strSubTotalPrice;
	INT nDataCnt = -1;
	if(pSpeciesDM->GetRowCount() -1 == nLastIdx) pSpeciesDM->InsertRow(-1);
	else pSpeciesDM->InsertRow(nLastIdx+1);
	
	if(strExtraField.IsEmpty()) strTmpData = _T("총 계");
	else strTmpData = _T("소 계");

	CPrintTotalInfo TotalInfo;
	TotalInfo.SetAliasInfo(m_cTotalInfo.GetBookCntAlias(), m_cTotalInfo.GetPriceAlias(), m_cTotalInfo.GetDiscPriceAlias());
	TotalInfo.SetTotalInfo(pSpeciesDM, nStartIdx, nLastIdx+1);
	SetTotalInfo(pSpeciesDM, &TotalInfo, nLastIdx+1, strTmpData);
	
	return nLastIdx+2;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRowByGroup(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup)
{
	INT nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	INT nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	if(nStartIdx < 0 || nLastIdx < 0) return -1;
	
	CString strSortValue;
	INT i = nStartIdx;
	while(i<=nLastIdx)
	{
		strSortValue = pSpeciesDM->GetCellData(m_strSortCondition, i);
		i = MakeSubTotalRow(pSpeciesDM, pGroup, m_strSortCondition, strSortValue);
		nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	
	return 0;
}

INT CMakeSubTotalPrintDM::FindStartIdx(CESL_DataMgr *pDM, 
									   CString strAcqYear, 
									   CString strGroupNo, 
									   CString strExtraField/*=_T("")*/, 
									   CString strExtraValue/*=_T("")*/)
{
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strAcqYear == pDM->GetCellData(_T("BP_수입년도"), i) && strGroupNo == pDM->GetCellData(_T("BP_그룹번호"), i))
		{
			if(strExtraField.IsEmpty())	return i;
			else
			{
				if(strExtraValue == pDM->GetCellData(strExtraField, i)) return i;
			}
		}
	}
	return -1;
}

INT CMakeSubTotalPrintDM::FindLastIdx(INT nStartIdx, 
									  CESL_DataMgr *pDM, 
									  CString strAcqYear, 
									  CString strGroupNo, 
									  CString strExtraField/*=_T("")*/, 
									  CString strExtraValue/*=_T("")*/)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt-1 == nStartIdx) return nStartIdx;
	else if(nRowCnt-1 < nStartIdx) return -1;
	
	CString strTmpData;
	for(INT i=nStartIdx ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("UDF_차수정보"), i);
		strTmpData.Replace(_T(" "), _T(""));
		if(strTmpData == _T("소계")) continue;
		if(strAcqYear != pDM->GetCellData(_T("BP_수입년도"), i) || strGroupNo != pDM->GetCellData(_T("BP_그룹번호"), i))
		{
			return i-1;
		}
		else
		{
			if(!strExtraField.IsEmpty() && strExtraValue != pDM->GetCellData(strExtraField, i)) return i-1;
		}
	}
	return i-1;
}

INT CMakeSubTotalPrintDM::SetKOLAS2FormatData(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	CString strDivValue, strTmpData;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDivValue = pDM->GetCellData(_T("UDF_차수정보"), i);
		
		if(strDivValue == _T("소 계") || strDivValue == _T("총 계"))
			continue;
		else if(strDivValue.IsEmpty()) 
		{
			pDM->SetCellData(_T("UDF_출판사항"), pDM->GetCellData(_T("IBS_발행년"), i), i);
			strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_본표제"), i), pDM->GetCellData(_T("IBS_저작자"), i));
			pDM->SetCellData(_T("UDF_서명저자사항"), strTmpData, i);
		}
		else
		{
			strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_본표제"), i), pDM->GetCellData(_T("IBS_저작자"), i));
			pDM->SetCellData(_T("UDF_서명저자사항"), strTmpData, i);
			strTmpData.Format(_T("%s, %s"), pDM->GetCellData(_T("IBS_발행자"), i), pDM->GetCellData(_T("IBS_발행년"), i));
			pDM->SetCellData(_T("UDF_출판사항"), strTmpData, i);
		}
	}
	return 0;
}
