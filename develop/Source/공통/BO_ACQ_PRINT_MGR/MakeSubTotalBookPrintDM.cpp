// MakeSubTotalBookPrintDM.cpp: implementation of the CMakeSubTotalBookPrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSubTotalBookPrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSubTotalBookPrintDM::CMakeSubTotalBookPrintDM(CESL_Mgr * pParentMgr) : CMakeSubTotalPrintDM(pParentMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = FALSE;
}

CMakeSubTotalBookPrintDM::~CMakeSubTotalBookPrintDM()
{
	
}

BOOL CMakeSubTotalBookPrintDM::CheckTotalInfoAlias()
{
	if(m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty())
		return FALSE;

	return TRUE;
}

INT CMakeSubTotalBookPrintDM::InitMakePrintDM()
{
	if(m_pPrintDM == NULL || m_pPrintBookDM == NULL) return -1;
	if(m_nPrintType < 0) return -1;
	
	//  합계정보를 나타낼 알리아스 정의
	//
	switch( m_nPrintType )
	{
	// 자료구분별 출력 DM 추가
	// 2005-09-05 강봉수
	//
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
	case BO_ACQ_PURCHASE_DATA_DEPT:
	// 이상 끝.
	case BO_ACQ_PURCHASE_DATA_REGCODE:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("IBS_발행년");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
	}
	break;

	default :	return -1;
		break;
	}	
	return 0;	
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//  - 책소계출력물 DM을 구성한다.
//
// [ 미니스펙 ]
// 1. 프로그래스 컨트롤을 생성한다.
// 2. 정렬여부를 판단하여 소스 DM을 정렬한다.
// 3. 원본 책DM정보를 복사한다
// 4. UDF컬럼들을 설정한다.
// 5. UDF_일련번호를 설정한다.
// 6. 출력용 SubTotal PrintDM을 구축한다.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalBookPrintDM::MakePrintDM()
{
	//================================================================
	// 변수 정의
	//================================================================
	INT ids, i, nGroupCnt;
	CGroupInfo* pGroup = NULL;

	if( m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL )	return -1;

	if( CheckTotalInfoAlias() == FALSE )
	{
		if(IDNO == m_pParentMgr->ESLAfxMessageBox(_T("경고:합계정보를 나타낼 알리아스를 정의하지 않았습니다.\n출력작업을 계속 진행 하시겠습니까?"), MB_YESNO))
			return 1000;
	}

	//================================================================
	// 1. 프로그래스 컨트롤을 생성한다.
	//================================================================
	ShowDataProcessingDlg();
	

	// 2006-01-19 강봉수 - 소스DM을 정렬하지 않고 출력DM을 정렬한다.
	/*
	//================================================================
	// 2. 정렬여부를 판단하여 소스 DM을 정렬한다.
	//================================================================
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintBookDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintBookDM);
	if( ids < 0 )
	{
		if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) )
			return 0;
	}
	*/


	//================================================================
	// 3. 원본 책DM정보를 복사한다
	//================================================================
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, m_pPrintDM->GetRowCount());
	}
	
	m_pPrintDM->FreeData();

	for( i=0 ; i<m_pPrintBookDM->GetRowCount() ; i++ )
	{
		if( m_pDataProcessingDlg )	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		if( CheckPrintData(m_pPrintBookDM->GetCellData(m_strDivAlias, i)) == FALSE )	continue;
		
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )		return -1;

		ids = CopyDM(m_pPrintBookDM, i, m_pPrintDM, pRow);
		if( ids < 0 )	return -1;
	}

	ids = m_pPrintDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;

	//================================================================
	// 4. UDF컬럼들을 설정한다.
	//================================================================
	SetUDFDMFieldData(m_pPrintDM);
	
	//================================================================
	// 4-1. 정렬여부를 판단하여 출력DM을 정렬한다.
	//================================================================
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintDM);
	else							ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if( ids < 0 )
	{
		if( IDNO == m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) )
			return 0;
	}
	
	//================================================================
	// 5. UDF_일련번호를 설정한다.
	//================================================================
	ids = SetDataSerialNo(m_pPrintDM);
	if(ids < 0) return ids;


	//================================================================
	// 6. 출력용 SubTotal PrintDM을 구축한다.
	//================================================================	
	//nRowCnt = m_pPrintDM->GetRowCount();
	nGroupCnt = m_pGroupCollection->GetGroupCount();
	for( i=0 ; i<nGroupCnt ; i++ )
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);

		ids = MakeSubTotalRow(m_pPrintDM, pGroup);
		if( ids < 0 )	return -1;
		if( m_strSortCondition.IsEmpty() == FALSE )
		{
			ids = MakeSubTotalRowByGroup(m_pPrintDM, pGroup);
			if( ids < 0 )	return -1;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - UDF로 정의된 칼럼을 설정한다. 
//
//////////////////////////////////////////////////////////////////////
INT CMakeSubTotalBookPrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM)
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, j, ids, nColCnt, nRowCnt, nRow = -1;
	DOUBLE dPrice;	
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	TCHAR* pEndPtr = NULL;

	// 2005-01-17
	// 가격자릿수 설정값 받아오기.
	if( m_strPostNum.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
		if( m_strPostNum.IsEmpty() == TRUE )		m_strPostNum = _T("0");
	}
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{	
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}


	nColCnt = pDM->GetRefColumnCount();
	nRowCnt = pDM->GetRowCount();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		if( nRow != i )
		{
			CMarc marc;
			strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
			ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
			if( ids < 0 )	return -1;

			strTmpData.Empty();	
			ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("UDF_총서사항"), strTmpData, i);

			strTmpData.Empty();	
			ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("IBS_판사항"), strTmpData, i);

			strTmpData.Empty();	 
			strTmpData = pDM->GetCellData(_T("IBS_본표제"), i);
			ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("UDF_표제사항"), strTmpData, i);

			nRow = i;
		}
		
		for( j=0 ; j<nColCnt ; j++ )
		{
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if( FIELD_ALIAS == _T("UDF_차수정보") )
			{	
				INT nGroupNo, nWorkNo, nSerialNo;
				strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), i);
				strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				strWorkNo = pDM->GetCellData(_T("BP_작업번호"), i);
				nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
				strSerialNo = pDM->GetCellData(_T("BP_일련번호"), i);
				nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
				str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
				pDM->SetCellData(FIELD_ALIAS, str, i);
				str.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
				pDM->SetCellData(_T("UDF_출력차수정보"), str, i);
			}
			else if( FIELD_ALIAS == _T("BB_가격") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				// 2004.09.30
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				//2005-01-17
				if		( m_strPostNum == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strPostNum == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else							strTmpData.Format(_T("%.0f"), dPrice);

				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환가격"), strTmpData, i);
			}
			// 책 발주가격
			else if( FIELD_ALIAS == _T("BB_발주가격_책") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if		( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
					else if	( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
					else									strTmpData.Format(_T("%.0f"), dPrice);
				}
				pDM->SetCellData(_T("UDF_할인가"), strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인가격"), strTmpData, i);
			}
			else if( FIELD_ALIAS == _T("UDF_단가") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if( _T("\\") == pDM->GetCellData(_T("BP_화폐구분"), i) )
					{
						strTmpData.Format(_T("%d"), (INT)dPrice);
					}
					else
					{
						if		( m_strPostNum == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
						else if	( m_strPostNum == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
						else								strTmpData.Format(_T("%d"), (INT)dPrice);
					}
				}
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			/*
			else if( FIELD_ALIAS == _T("UDF_할인가") )
			{
				strTmpData = pDM->GetCellData(_T("BB_가격"), i);				
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dExchangeRate = 0.0; 
				strExchangeRate = pDM->GetCellData(_T("BP_환율"), i);
				dExchangeRate = (DOUBLE)atof(strExchangeRate.GetBuffer(0));
				dDisc_Price = (dPrice - ((dPrice*dDisc_Rate)/100)) * dExchangeRate;
				//2005-01-17
				if		( m_strTruncPrice == _T("1") )			strTmpData.Format(_T("%.1f"), dDisc_Price);
				else if	( m_strTruncPrice == _T("2") )			strTmpData.Format(_T("%.2f"), dDisc_Price);
				else										strTmpData.Format(_T("%.0f"), dDisc_Price);

				pDM->SetCellData(_T("UDF_할인가"), strTmpData, i);
				
				m_pParentMgr->GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), strIs64);
				if( _T("Y") == strIs64 )
				{
					INT nOrderPrice = 0;
					dDisc_Price /= 10;
					nOrderPrice = (INT)(dDisc_Price+0.5);
					nOrderPrice *= 10;
					strTmpData.Format(_T("%d"), nOrderPrice);
				}
				else
				{
					strTmpData.Format(_T("%.0f"), dDisc_Price);  // 2005-02-02
				}			

				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인가격"), strTmpData, i);
			}
			*/			
			else if( FIELD_ALIAS == _T("BB_등록구분") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), strTmpData, strTmpData);
				
				//===================================================
				//2009.05.20 UPDATE BY LKS : 구입대상자료목록(등록구분별) 출력인 경우 변환 없이 출력
				
				//DEL pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				if(m_nPrintType != BO_ACQ_PURCHASE_DATA_REGCODE)
					pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				//===================================================
			}
			// 도서분류코드1(사용자정의코드1) 값을 설명으로 변경하여 DM에 저장 (64)
			// 2005-09-05 강봉수
			//
			else if( FIELD_ALIAS == _T("BS_사용자정의코드1") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("도서분류코드1"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 이상 끝.

			// 학처별 코드값을 설명으로 변경하여 DM에 저장 (64)
			// 2005-09-26 강봉수
			//
			else if( FIELD_ALIAS == _T("UDF_학처명") )
			{
				strTmpData = pDM->GetCellData(_T("BP_학처명"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학부"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 이상 끝.
			// 부서별 코드값을 설명으로 변경하여 DM에 저장 (국방대)
			// 2005-11-10 강봉수
			//
			else if( FIELD_ALIAS == _T("UDF_부서명") )
			{
				strTmpData = pDM->GetCellData(_T("BP_부서코드"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("직원부서"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 이상 끝.
		}
	}	
	return 0;
}

INT CMakeSubTotalBookPrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	strAlias[nCondColumns] = _T("BP_일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	return 0;
}

INT CMakeSubTotalBookPrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpHeader = NULL;	
		CStringArray * pTmpTail = NULL;	
		CMapStringToString * pMap = NULL;
		CStringArray SortConditionHeader;	SortConditionHeader.RemoveAll();
		CStringArray SortConditionTail;		SortConditionTail.RemoveAll();
		CMapStringToString SortConditionMap;	SortConditionMap.RemoveAll();

		pTmpHeader = m_pSortItem->GetHeadSortList();
		ids = CopySortItem(pTmpHeader, &SortConditionHeader);
		if(ids < 0) return -1;
		pTmpTail = m_pSortItem->GetTailSortList();
		ids = CopySortItem(pTmpTail, &SortConditionTail);
		if(ids < 0) return -1;
		pMap = m_pSortItem->GetConvertUDFToRealAliasMap();
		ids = CopySortConditionMap(pMap, &SortConditionMap);
		if(ids < 0) return -1;
		
		if(!m_strSortCondition.IsEmpty()) pTmpHeader->Add(m_strSortCondition);
		pMap->SetAt(_T("UDF_종가격"), _T("UDF_변환가격"));
		pMap->SetAt(_T("UDF_할인종가격"), _T("UDF_변환할인가격"));
		pTmpTail->Add(_T("BB_권일련번호"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionHeader, pTmpHeader);
		CopySortItem(&SortConditionTail, pTmpTail);
		CopySortConditionMap(&SortConditionMap, pMap);

		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}
	
	return 0;
}
