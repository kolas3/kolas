// MakeVolumePrintDM.cpp: implementation of the CMakeVolumePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeVolumePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeVolumePrintDM::CMakeVolumePrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{
	m_cVolumeTotalInfo.InitTotalInfo();
}

CMakeVolumePrintDM::~CMakeVolumePrintDM()
{

}

INT CMakeVolumePrintDM::InitMakePrintDM()
{
	switch( m_nPrintType )
	{	
	case BO_ACQ_CHECKIN_DATA:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalDeliveryBookCntAlias = _T("UDF_납품책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		m_strTotalSpeciesCntAlias = _T("BP_화폐구분");
	}
		break;
	case BO_ACQ_NONCHECKIN_DATA:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalMissingBookCntAlias = _T("UDF_미납책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		m_strTotalSpeciesCntAlias = _T("BP_화폐구분");
	}
		break;
	case BO_ACQ_CHECKINING_DATA:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalMissingBookCntAlias = _T("UDF_미납책수");
		m_strTotalDeliveryBookCntAlias = _T("UDF_납품책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		m_strTotalSpeciesCntAlias = _T("BP_화폐구분");
	}
		break;
	case BO_ACQ_MISSING_DATA:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalMissingBookCntAlias = _T("UDF_미납책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행자");
	}
		break;
	case BO_ACQ_PURCHASE_REGOBJ_DATA:
	{
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행자");
	}
		break;
	//m_nPrintType < 0
	default:	return -1;
		break;
	}

	SetTotalInfoAlias(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias, m_strTotalMissingBookCntAlias, m_strTotalDeliveryBookCntAlias);
	return 0;
}

INT CMakeVolumePrintDM::CheckValid()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pSourceBookDM == NULL) return -1;
	if(m_strDivAlias.IsEmpty()) return -1;
	return 0;
}

INT CMakeVolumePrintDM::MakePrintDM()
{
	INT ids ;
	switch( m_nPrintType )
	{
	case BO_ACQ_CHECKIN_DATA:			ids = MakeCheckInRelativeData(TRUE);
		break;
	case BO_ACQ_NONCHECKIN_DATA:		ids = MakeCheckInRelativeData(FALSE);
		break;
//	case BO_ACQ_CHECKIN_REQUEST_DATA:	ids = MakeCheckInRequestData();
//		break;
	case BO_ACQ_CHECKINING_DATA:		ids = MakeCheckInListData();
		break;
	case BO_ACQ_MISSING_DATA :
	case BO_ACQ_PURCHASE_REGOBJ_DATA:	ids = MakeVolumeListData();
		break;
	default:	return 486;
		break;
	}
	
	return ids;
}

INT CMakeVolumePrintDM::MakeCheckInRelativeData(BOOL bCheckIn)
{
	INT i, ids, nRowCnt;
	DOUBLE dPrice;
	CString strTmpData, strTmpAlias, strPrice, strDiscRate, strTmpPrice, strIs64;
	
	TCHAR* pEndPtr = NULL;

	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();

	ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if( ids < 0 )	return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	if( bCheckIn == TRUE )	strTmpAlias = _T("UDF_납품책수");
	else strTmpAlias = _T("UDF_미납책수");

	nRowCnt = m_pPrintDM->GetRowCount();
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	// 2005-01-17
	// 가격자릿수 설정값 받아오기.
	if( m_strPostNum.IsEmpty() == TRUE )
	{
		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
//DEL 		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
		if( m_strPostNum.IsEmpty() == TRUE )	m_strPostNum = _T("0");
	}

	// 2006-01-18 강봉수 - 발주가격단위설정값 
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}

	for( i=nRowCnt-1 ; i>=0 ; i-- )
	{
		if( m_pDataProcessingDlg )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);

		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if( strTmpData.IsEmpty() || strTmpData == _T("0") )
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		if( !CheckPrintData(m_pPrintDM->GetCellData(m_strDivAlias, i), m_pPrintDM->GetCellData(_T("BB_권일련번호"), i)) )
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		// 가격설정
		strPrice = m_pPrintDM->GetCellData(_T("BB_가격"), i);
		//dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr) * _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//2005-01-17
		//strTmpPrice.Format(_T("%.0f"), dPrice);
		if		( m_strPostNum == _T("1") )		strTmpPrice.Format(_T("%.1f"), dPrice);
		else if	( m_strPostNum == _T("2") )		strTmpPrice.Format(_T("%.2f"), dPrice);
		else 									strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), strTmpPrice, i);
		
		// 2006-01-19 강봉수 - 가격정책사항반영, 발주가격설정(계산을 하지 않고 DB에서 받아온다.
		strPrice = m_pPrintDM->GetCellData(_T("BB_발주가격_책"), i);
		strPrice.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr) * _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		if		( m_strTruncPrice == _T("1") )	strTmpPrice.Format(_T("%.1f"), dPrice);
		else if	( m_strTruncPrice == _T("2") )	strTmpPrice.Format(_T("%.2f"), dPrice);
		else 									strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_변환할인가격"), strTmpPrice, i);
	}
	
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);

	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if( ids < 0 )	return ids;
	
	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// 총계정보를 넣아야쥐
		m_pPrintDM->InsertRow(-1);
		// 2006-01-23 강봉수 - 한줄띄지않고 총계를 출력한다.(국방대요구사항)
		// m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"));
	}

	return 0;
}

INT CMakeVolumePrintDM::MakeCheckInRequestData()
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if(ids < 0) return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	CString strTmpData, strTmpAlias, strAcqKey, strQuery;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	
	strTmpAlias = _T("UDF_납품책수");
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if(strTmpAlias.IsEmpty() || strTmpAlias == _T("0"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strTmpData = m_pPrintDM->GetCellData(_T("BP_입력구분"), i);
		if(strTmpAlias != _T("2"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strAcqKey = m_pPrintDM->GetCellData(_T("BP_수서정보KEY"), i);
		if(strAcqKey.IsEmpty() || !CheckPrintData(strAcqKey, m_pPrintDM->GetCellData(_T("BB_권일련번호"), i))) 
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}

		strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_KEY = %s"), strAcqKey);
//DEL 		strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE ACQ_KEY = %s"), strAcqKey);
		tmpDM.RestructDataManager(strQuery);
		if(tmpDM.GetRowCount() > 0) 
		{
			strTmpData = tmpDM.GetCellData(0, 0);
			m_pPrintDM->SetCellData(_T("UDF_신청자"), strTmpData, i);
			strTmpData = tmpDM.GetCellData(0, 1);
			m_pPrintDM->SetCellData(_T("UDF_신청일"), strTmpData, i);
		}
	}
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);

	return ids;
}

INT CMakeVolumePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return SetDataSerialNo(pDM);
}

INT CMakeVolumePrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	INT ids;
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	if(m_pSortItem && m_bDataSortYN)
	{
		CStringArray * pTmpTail = NULL;	
		CStringArray SortConditionTail;	SortConditionTail.RemoveAll();
		pTmpTail = m_pSortItem->GetTailSortList();
		ids = CopySortItem(pTmpTail, &SortConditionTail);
		if(ids < 0) return -1;
		
		pTmpTail->Add(_T("BB_권일련번호"));
		ids = m_pSortItem->DataSort(pDM);
		CopySortItem(&SortConditionTail, pTmpTail);
		if(ids < 0) return -1;
	}
	else
	{
		return PrintDataSortByBasicInfo(pDM);
	}
	
	return SetDataSerialNo(pDM);
}

INT CMakeVolumePrintDM::MakeCheckInListData()
{
	INT ids, nRowCnt;
	DOUBLE dPrice;
	CString strAcqKey, strVolSortNo, strPrice, strDiscRate, strTmpData, strTmpPrice, strIs64;
	
	TCHAR* pEndPtr = NULL;

	nRowCnt = m_pSourceBookDM->GetRowCount();	
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	// 2005-01-26
	// 가격자릿수 설정값 받아오기.
	if( m_strPostNum.IsEmpty() == TRUE )
	{
		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
//DEL 		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
		if(m_strPostNum.IsEmpty() == TRUE )		m_strPostNum = _T("0");
	}
	// 2006-01-19 강봉수 - 발주가격단위설정값
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		m_pPrintDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if(m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}

	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if( m_pDataProcessingDlg )	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		if( !CheckPrintData(m_pSourceBookDM->GetCellData(m_strDivAlias, i), m_pSourceBookDM->GetCellData(_T("BB_권일련번호"), i)) )	continue;
		
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )	continue;
		
		ids = CopyDM(m_pSourceBookDM, i, m_pPrintDM, pRow);
		if( ids < 0 )	return -1;

		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_납품책수")));
		strPrice = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("BB_가격")));
		strDiscRate = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("BP_할인율")));
		//dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr) * _tcstod(strTmpData.GetBuffer(0), &pEndPtr);


		//2005-01-26
		if		( m_strPostNum == _T("1") )		strTmpPrice.Format(_T("%.1f"), dPrice);
		else if	( m_strPostNum == _T("2") )		strTmpPrice.Format(_T("%.2f"), dPrice);
		else									strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환가격")), strTmpPrice);
		
		// 2006-01-19 강봉수 - 가격정책사항반영, 권 발주가격 = 책발주가격의 합
		strTmpPrice = m_pSourceBookDM->GetCellData(_T("UDF_발주가격_권"), i);
		strTmpPrice.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpPrice);
		
		//===================================================
		//2009.10.21 ADD BY LKS : 검수대상자료출력시 할인가격 적용안되는 오류 수정
		//strPrice를 사용하도록 수정되어있음. 이유를 모르겠음. 원래소스로 돌릴경우 원인 파악!!
		//기존소스
		//dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

		dPrice = _tcstod(strTmpPrice.GetBuffer(0), &pEndPtr);
		//===================================================

		if		( m_strTruncPrice == _T("1") )	strTmpPrice.Format(_T("%.1f"), dPrice);
		else if	( m_strTruncPrice == _T("2") )	strTmpPrice.Format(_T("%.2f"), dPrice);
		else									strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환할인가격")), strTmpPrice);
	
		/*
		// 2005-01-26 환율도 계산한다.
		CString strExchangeRate;
		DOUBLE dExchangeRate = 0.0; 
		strExchangeRate = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("BP_환율")));
		dExchangeRate = (DOUBLE)atof(strExchangeRate.GetBuffer(0));
		dPrice = (dPrice - (dPrice * ((DOUBLE)atof(strDiscRate.GetBuffer(0))/100))) * dExchangeRate;
		
		if( _T("Y") == strIs64 )
		{
			INT nPrice = 0;
			dPrice /= 10;
			nPrice = (INT)(dPrice+0.5);
			nPrice *= 10;
			strTmpPrice.Format(_T("%d"), nPrice);
		}
		else
		{
		strTmpPrice.Format(_T("%.0f"), dPrice);
		}

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환할인가격")), strTmpPrice);
		*/
	}
	
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	
	//SetTotalInfoAlias(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias, m_strTotalMissingBookCntAlias, m_strTotalDeliveryBookCntAlias);
	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if(ids < 0) return ids;

	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// 총계정보를 넣아야쥐
		m_pPrintDM->InsertRow(-1);
		SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"));
	}

	return 0;
}

INT CMakeVolumePrintDM::MakeVolumeListData()
{
	INT i, ids, nRowCnt;
	DOUBLE dPrice = 0.0;
	CString strAcqKey, strVolSortNo, strTmpData, strPrice;

	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	
	nRowCnt = m_pSourceBookDM->GetRowCount();
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	for( i=nRowCnt-1 ; i>=0 ; i-- )
	{
		if( m_pDataProcessingDlg )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);		
		
		if( !CheckPrintData(m_pSourceBookDM->GetCellData(m_strDivAlias, i), m_pSourceBookDM->GetCellData(_T("BB_권일련번호"), i)) )
		{
			continue;
		}
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )	continue;
		ids = CopyDM(m_pSourceBookDM, i, m_pPrintDM, pRow);
		if( ids < 0 ) return -1;
	}
	
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;
	
	if( m_bDataSortYN == TRUE )		ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	
	ids = m_cVolumeTotalInfo.SetTotalInfo(m_pPrintDM);
	if( ids < 0 )		return ids;
	
	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// 총계정보를 넣아야쥐
		m_pPrintDM->InsertRow(-1);
	SetTotalInfo(m_pPrintDM, &m_cVolumeTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"));
	}

	return 0;
}

VOID CMakeVolumePrintDM::SetTotalInfoAlias(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias,
										   CString strMissingBookCntAlias, CString strDeliveryBookCntAlias)
{
	m_cVolumeTotalInfo.SetAliasInfo(strBookCntAlias, strPriceAlias, strDiscPriceAlias, strMissingBookCntAlias, strDeliveryBookCntAlias);
}

INT CMakeVolumePrintDM::SetTotalInfo(CESL_DataMgr * pDM, CVolumePrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle)
{
	if( pTotalInfo == NULL ) return -1;
	
	DOUBLE dPrice = 0.0;
	CString strTmpData;
	pDM->SetCellData(m_strTotalInfoTitle, strTitle, nRowIdx);
	
	// 2006-01-18 강봉수 - 발주가격단위설정값 
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}
	
	if( m_strTotalSpeciesCntAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalSpeciesCnt(strTmpData);
		pDM->SetCellData(m_strTotalSpeciesCntAlias, strTmpData+_T("종"), nRowIdx);
	}
	if( m_strTotalBookCntAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalBookCntAlias, strTmpData+_T("책"), nRowIdx);
	}
	if( m_strTotalMissingBookCntAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalMissingBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalMissingBookCntAlias, strTmpData+_T("책"), nRowIdx);
	}
	if( m_strTotalDeliveryBookCntAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalDeliveryBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalDeliveryBookCntAlias, strTmpData+_T("책"), nRowIdx);
	}
	
	//===================================================
	//2009.05.20 UPDATE BY LKS : 미검수자료 목록출력시 가격항목의 총계가 나오지 않는 오류 수정 
	
	if( m_strTotalPriceAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalPriceAlias, strTmpData, nRowIdx);
	}
	//===================================================
	
	if( m_strTotalDiscPriceAlias.IsEmpty() == FALSE )
	{
		//pTotalInfo->GetDB_DiscTotalPrice(strTmpData, m_strTruncPrice);		
		//pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);
		
		
		pTotalInfo->GetDiscTotalPrice(strTmpData, m_strTruncPrice);
		pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);
		/*
		if( strTitle == _T("총계") )
		{
			// 2006-01-23 강봉수 - 오차범위 산출을 위하여 DB값을 가져온다.
			// 오차를 산출한다.(DB값 - 항목의 합)
			ids = pDM->InsertRow(-1);
			if( ids < 0 )	return ids;
			strTmpData.Replace(_T(","),_T(""));			
			dPrice = (DOUBLE)atof(strTmpData);
			dPrice = dPrice - pTotalInfo->GetDB_DiscTotalPrice();
			if( dPrice > 0 )	strTmpData.Format(_T("+%.2f"), dPrice);
			else				strTmpData.Format(_T("%.2f"), dPrice);
			pDM->SetCellData(m_strTotalInfoTitle, _T("오차"), nRowIdx+1 );
			pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx+1);
		}
		*/
	}
	
	return 0;
}