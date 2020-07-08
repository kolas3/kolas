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
// 생성자
CMakeSubTotalPrintDM::CMakeSubTotalPrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = FALSE;
	m_bKOLAS2Format = FALSE;
	m_pSortItem = NULL;
	m_nPrintDataType = -1;
	m_strTruncPrice.Empty();
}
// 소멸자
CMakeSubTotalPrintDM::~CMakeSubTotalPrintDM()
{
	if(m_bDeletPrintBookDM && m_pPrintBookDM)
	{
		delete m_pPrintBookDM;
		m_pPrintBookDM = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM구성시 무시되어야할 알리아스를 정의하고, 토탈정보에대한 알리아스를 정의한다.
//
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalPrintDM::InitMakePrintDM()
{
	if( m_pPrintDM == NULL )	 return -1;

	m_pPrintBookDM = new CESL_DataMgr;
	if( m_pPrintBookDM == NULL )	 return -1;
	
	m_pPrintBookDM->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	m_pPrintBookDM->InitDMPKFromFile(m_strBookDMAlias);
	m_pPrintBookDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	m_bDeletPrintBookDM = TRUE;

	if( m_nPrintType < 0 )	 return -1;
	else if( m_bKOLAS2Format == TRUE )
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalSpeciesCntAlias = _T("UDF_출판사항");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		//m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");   // 2005-03-03
	}
	else
	{
		switch( m_nPrintType )
		{
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:	
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
				m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
				m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
				m_strTotalInfoTitle = _T("UDF_일련번호");
				m_strTotalSpeciesCntAlias = _T("IBS_발행년");
				m_strTotalBookCntAlias = _T("UDF_책수");
				m_strTotalPriceAlias = _T("UDF_변환가격");
				//m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
				m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");  // 2005-02-03
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				m_IgnoreAlias.Add(_T("UDF_출력접수번호정보"));
				m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
				m_IgnoreAlias.Add(_T("BP_기증자명"));
				m_strTotalInfoTitle = _T("UDF_일련번호");
				m_strTotalSpeciesCntAlias = _T("IBS_발행년");
				m_strTotalBookCntAlias = _T("UDF_책수");
				m_strTotalPriceAlias = _T("UDF_변환가격");
				//m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
				m_strTotalDiscPriceAlias = _T("UDF_변환가격");  // 2005-02-03
			}
			break;

		// 2005-01-31
		case BO_ACQ_ORDER_DATA_MEDIACODE:
		case BO_ACQ_PURCHASE_DATA_DEPT:
		case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		case BO_ACQ_ORDER_DATA_ORDER:
			{
				m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
				m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
				m_IgnoreAlias.Add(_T("UDF_구입처부호설명"));
				m_strTotalInfoTitle = _T("UDF_일련번호");
				m_strTotalSpeciesCntAlias = _T("IBS_발행년");
				//m_strTotalBookCntAlias = _T("UDF_책수");
				if( m_nPrintType == BO_ACQ_ORDER_DATA_ORDER )
				{
					m_strTotalBookCntAlias = _T("UDF_변환가격");
				}
				else
				{
					m_strTotalBookCntAlias = _T("UDF_책수");
				}
				m_strTotalPriceAlias = _T("UDF_변환가격");
				m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
			}
			break;

		default:
			break;
		}
	}
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM을 구성한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalPrintDM::MakePrintDM()
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids;
	BOOL bOrderPrint = FALSE;
	CESL_DataMgr* pBookDM = NULL;
	CESL_DataMgr BookDM, SpeciesDM;

	if( m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL )		return -1;

	switch( m_nPrintType )
	{
	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		break;
	default:
		{
			if( CheckTotalInfoAlias() == FALSE )
			{
				if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("경고:합계정보를 나타낼 알리아스를 정의하지 않았습니다.\n출력작업을 계속 진행 하시겠습니까?"), MB_YESNO) )
					return 1000;
			}
		}
		break;
	}
	
	//================================================================
	// 1. Progress Ctrl 을 생성한다.
	//================================================================
	ShowDataProcessingDlg();


	//================================================================
	// 2. 책 DM 을 설정한다.
	//================================================================
	BookDM.SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	BookDM.InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	BookDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);


	//================================================================
	// 3. 종 DM 을 설정한다.
	//================================================================
	SpeciesDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	SpeciesDM.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	SpeciesDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	//책 검색
	//================================================================
	// 4. 멤버변수 DM알리아스를 이용하여 책 DM 포인터를 가져온다.
	//================================================================
	pBookDM = MakePrintBookDM(&BookDM);
	if( pBookDM == NULL )	return -1;

	//종자료 구축
	//================================================================
	// 5. 책DM에서 종DM을 구축한다.
	//================================================================
	SpeciesDM.FreeData();
	ids = MakeSpeciesDM(&SpeciesDM, pBookDM, FALSE);

	// 종자료정렬
	if( m_nPrintType >= BO_ACQ_ORDER_DATA_DIVNO )	 bOrderPrint = TRUE;
	
	//===================================================
	//2009.05.06 UPDATE BY LKS : 함수를 위로올림 --> 정렬전에 DATA SET!!
	
	// UDF컬럼들을 설정한다.
	SetUDFDMFieldData(&SpeciesDM, bOrderPrint);
	//===================================================

	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(&SpeciesDM);
	else							ids = PrintDataSortByBasicInfo(&SpeciesDM);

	if( ids < 0 )
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO)
			return 0;
	}
	
//DEL	// UDF컬럼들을 설정한다.
//DEL	SetUDFDMFieldData(&SpeciesDM, bOrderPrint);
	
	// 그리드 선정자료 적용
	ids = ApplyGridSelectData(&SpeciesDM);
	if(ids < 0) return ids;
	//일련번호 설정
	ids = SetDataSerialNo(&SpeciesDM);
	if(ids < 0) return ids;
	
	
	//SubTotal PrintDM구축
	CGroupInfo* pGroup = NULL;
	INT nRowCnt = SpeciesDM.GetRowCount();
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);
		if( pGroup == NULL )	continue;
		ids = MakeSubTotalRow(&SpeciesDM, pGroup);
		if( ids < 0 )			continue;
		if( m_strSortCondition.IsEmpty() == FALSE )
		{
			ids = MakeSubTotalRowByGroup(&SpeciesDM, pGroup);
			if(ids < 0) return -1;
		}
	}

	// 출력용 권DM생성
	ids = MakeVolumeDM(m_pPrintBookDM, pBookDM, _T("BP_수서정보KEY"));
	if(ids < 0) return -1;

	CString strTmpData;
	
	// 권 DM을 구분 알리아스로 정렬한다.
	INT nCondColumns = 0;
	CString strAlias1[2];
	INT nCond1[2];
	strAlias1[nCondColumns] = _T("BP_수서정보KEY");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias1[nCondColumns] = _T("BB_권일련번호");			nCond1[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintBookDM->SORT(nCondColumns, strAlias1, nCond1, ESL_DATAMGR_SORT_ASC);
	
	// 출력DM를 Clear시킨다.
	m_pPrintDM->FreeData();
	
	// 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
	nRowCnt = SpeciesDM.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(i=0 ; i<nRowCnt ; i++)
	{
		if( m_pDataProcessingDlg )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strKey = SpeciesDM.GetCellData(_T("BP_수서정보KEY"), i);
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) return -1;
		ids = CopyDM(&SpeciesDM, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintBookDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_단가")), _T(""));
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	// Kolas2출력물에 맞게 데이터 조정
	if(m_bKOLAS2Format)
	{
		ids = SetKOLAS2FormatData(m_pPrintDM);
		if(ids < 0) return -1;
	}
	
	return 0;
}

CString CMakeSubTotalPrintDM::GetGroupCollectionQuery(INT nAcqType)
{
	if(m_pGroupCollection->GetGroupCount() == 0 )
		return _T("");
	
	CString strQuery, strGroupInfo;
	
	CString strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	if(strGroupInfo  == _T("전체자료")) return _T("");

	if(strWorkCode.IsEmpty( ))
	{
		//2007.05.16 UPDATE PJW : 기증자료 , 구입자료 구분 		
		if( nAcqType == 2  )
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND B.BOOK_APPENDIX_FLAG='B' AND P.ACQ_YEAR='%s' AND P.RECEIPT_NO  IN (%s) "),
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
		else
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) "),//AND I.WORK_CODE IS NULL_T(",
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
	}
	else
	{
		//2007.05.16 UPDATE PJW : 기증자료 , 구입자료 구분 
		if( nAcqType == 2 )
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND B.BOOK_APPENDIX_FLAG='B' AND P.ACQ_YEAR='%s' AND P.RECEIPT_NO  IN (%s) AND I.WORK_CODE='%s' "),
			m_pGroupCollection->GetAcqYear(), strGroupInfo);
		}
		else
		{
			strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE='%s'"),
			m_pGroupCollection->GetAcqYear(), strGroupInfo, strWorkCode);
		}
	}
	return strQuery;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRow(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup, CString strExtraField/*=_T("")*/, CString strExtraValue/*=_T("")*/)
{
	INT nStartIdx, nLastIdx, nDataCnt;
	CString strTmpData, strSubTotalPrice;
	CPrintTotalInfo TotalInfo;
	
	nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strExtraField, strExtraValue);
	if( nStartIdx < 0 || nLastIdx < 0 ) return -1;


	nDataCnt = -1;
	if( pSpeciesDM->GetRowCount() -1 == nLastIdx )		pSpeciesDM->InsertRow(-1);
	else												pSpeciesDM->InsertRow(nLastIdx+1);
	
	if( strExtraField.IsEmpty() == TRUE )	strTmpData = _T("총계");
	else									strTmpData = _T("소계");

	switch( m_nPrintType )
	{
	case BO_ACQ_PURCHASE_DATA_REGCODE:
		{
			TotalInfo.SetAliasInfo(m_cTotalInfo.GetBookCntAlias(), m_cTotalInfo.GetPriceAlias(), m_cTotalInfo.GetDiscPriceAlias());	
		}
		break;

	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		{
			TotalInfo.SetAcqType(2);
		}
	default:
		{
			TotalInfo.SetAliasInfo(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias);	
		}
		break;
	}
	TotalInfo.SetTotalInfo(pSpeciesDM, nStartIdx, nLastIdx+1);
	SetTotalInfo(pSpeciesDM, &TotalInfo, nLastIdx+1, strTmpData, TotalInfo.GetDiscTotalPrice_Display());
	
	return nLastIdx+2;
}

INT CMakeSubTotalPrintDM::MakeSubTotalRowByGroup(CESL_DataMgr *pSpeciesDM, CGroupInfo *pGroup)
{
	INT nStartIdx = FindStartIdx(pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	INT nLastIdx = FindLastIdx(nStartIdx, pSpeciesDM, pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	if(nStartIdx < 0 || nLastIdx < 0) return -1;
	
	CString strSortValue,strTmp;
	INT i = nStartIdx;
	while( i<=nLastIdx )
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
			if( strExtraField.IsEmpty() == TRUE )	return i;
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
		strTmpData = pDM->GetCellData(_T("UDF_일련번호"), i);
		strTmpData.Replace(_T(" "), _T(""));
		if(strTmpData == _T("소계") || strTmpData == _T("분류")) continue;
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
	INT ids;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDivValue = pDM->GetCellData(_T("UDF_일련번호"), i);
		
		if(strDivValue == _T("소계") || strDivValue == _T("총계"))		continue;
		else if(strDivValue.IsEmpty()) 
		{
			pDM->SetCellData(_T("UDF_출판사항"), pDM->GetCellData(_T("IBS_발행년"), i), i);
			strTmpData.Format(_T("%s"), pDM->GetCellData(_T("UDF_표제사항"), i));
			pDM->SetCellData(_T("UDF_서명저자사항"), strTmpData, i);
		}
		else
		{
			CMarc marc;
			strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
			if(!strTmpData.IsEmpty())
			{
				ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
				if(ids < 0) SetDefaultKOLAS2FormatData(pDM, i);
				ids = SetKOLAS2FormatData(&marc, pDM, i);
				if(ids < 0) SetDefaultKOLAS2FormatData(pDM, i);
			}
			else SetDefaultKOLAS2FormatData(pDM, i);
		}
	}
	return 0;
}

INT CMakeSubTotalPrintDM::SetDefaultKOLAS2FormatData(CESL_DataMgr *pDM, INT nIdx)
{
	CString strTmpData;
	strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_본표제"), nIdx), pDM->GetCellData(_T("IBS_저작자"), nIdx));
	pDM->SetCellData(_T("UDF_서명저자사항"), strTmpData, nIdx);
	strTmpData.Format(_T("%s, %s"), pDM->GetCellData(_T("IBS_발행자"), nIdx), pDM->GetCellData(_T("IBS_발행년"), nIdx));
	pDM->SetCellData(_T("UDF_출판사항"), strTmpData, nIdx);
	return 0;
}

INT CMakeSubTotalPrintDM::SetKOLAS2FormatData(CMarc * pMarc, CESL_DataMgr *pDM, INT nIdx)
{
	if(pMarc == NULL) return -1;

	CString strTmpData;
	strTmpData.Format(_T("%s / %s"), pDM->GetCellData(_T("IBS_본표제"), nIdx), pDM->GetCellData(_T("IBS_저작자"), nIdx));
	INT ids = m_MarcConvertMgr.GetTitleStatement(pMarc, strTmpData);
	if(ids < 0) return -1;
	pDM->SetCellData(_T("UDF_서명저자사항"), strTmpData, nIdx);
	strTmpData.Format(_T("%s, %s"), pDM->GetCellData(_T("IBS_발행자"), nIdx), pDM->GetCellData(_T("IBS_발행년"), nIdx));
	pDM->SetCellData(_T("UDF_출판사항"), strTmpData, nIdx);
	return 0;
}

INT CMakeSubTotalPrintDM::PrintDataSortByBasicInfo(CESL_DataMgr * pDM)
{
	if(pDM == NULL) return -1;

	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if( m_strSortCondition.IsEmpty() == FALSE )
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_nPrintType >= BO_ACQ_ORDER_DATA_DIVNO)
	{
		strAlias[nCondColumns] = _T("BP_발주일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	else
	{
		strAlias[nCondColumns] = _T("BP_일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	return 0;
}

INT CMakeSubTotalPrintDM::PrintDataSortByUserSelect(CESL_DataMgr * pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if( m_strSortCondition.IsEmpty() == FALSE )
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpData = NULL;	
		CMapStringToString * pMap = NULL;
		CStringArray SortConditionHeader;	SortConditionHeader.RemoveAll();
		CMapStringToString SortConditionMap;	SortConditionMap.RemoveAll();

		pTmpData = m_pSortItem->GetHeadSortList();
		ids = CopySortItem(pTmpData, &SortConditionHeader);
		if(ids < 0) return -1;
		pMap = m_pSortItem->GetConvertUDFToRealAliasMap();
		ids = CopySortConditionMap(pMap, &SortConditionMap);
		if(ids < 0) return -1;
		
		if( m_strSortCondition.IsEmpty() == FALSE )		pTmpData->Add(m_strSortCondition);
		pMap->SetAt(_T("UDF_종가격"), _T("UDF_가격"));
		pMap->SetAt(_T("UDF_할인종가격"), _T("UDF_할인가"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionHeader, pTmpData);
		CopySortConditionMap(&SortConditionMap, pMap);
		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
CESL_DataMgr* CMakeSubTotalPrintDM::MakePrintBookDM(CESL_DataMgr * pBookDM)
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids = -1;

	if(pBookDM == NULL) return NULL;
	
	//================================================================
	// 출력데이터 타입에 따라 책DM을 구축한다.
	// - 0 : 출력데이터 재검색
	// - 1 : 바탕화면검색데이터 사용
	// - 2 : 그리드 선정자료
	//================================================================
	switch( m_nPrintDataType )
	{
	case 0:
	{
		ids = ReSearchPrintBookDM(pBookDM);
		if( ids < 0 )	 return NULL;
		return pBookDM;
	}
		break;
	case 1:
	case 2:		return m_pParentMgr->FindDM(m_strBookDMAlias);
		break;
	}

	return NULL;
}

INT CMakeSubTotalPrintDM::ReSearchPrintBookDM(CESL_DataMgr * pBookDM)
{
	INT ids = -1;
	CString strQuery;


	if ( pBookDM->DB_MGR_ALIAS.Find(_T("기증")) >= 0 )
	{
		strQuery = GetGroupCollectionQuery(2);
	}
	else
	{
		strQuery = GetGroupCollectionQuery(1);
	}
	
	if(strQuery.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("주어진 그룹정보로 쿼리를 생성할 수 없습니다. 다시 검색해 주십시요."));
		return 1000;
	}
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 책정보를 구축중입니다."));
		ids = pBookDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	}
	else 
		ids = pBookDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	return 0;
}