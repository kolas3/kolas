// BO_DataConverter.cpp: implementation of the CBO_DataConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_DataConverter.h"
#include "../K2UP_Common/WorkingStatus.h"
#include "../COMMON_CONVERSION_MGR/PriceInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CBO_DataConverter::CBO_DataConverter()
{
	m_pSourceDM = NULL;
	m_pTargetDM = NULL;
	m_pPrgCtrl = NULL;
}

// 소멸자
CBO_DataConverter::~CBO_DataConverter()
{

}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종정보DM을 구축한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToSpeciesInfo()
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL || m_strDivideAlias.IsEmpty() == TRUE ) 	return -1;	
	if( m_pSourceDM->GetRowCount() == 0 )	return 0;
	
	m_pSourceDM->SORT(m_strDivideAlias);
	
	return ConvertSourceInfo(0, m_pSourceDM->GetRowCount()-1, m_strDivideAlias);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 권정보DM을 구축한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToVolumeInfo(BOOL bPrintData)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL || m_strDivideAlias.IsEmpty() == TRUE )	return -1;
	if( m_pSourceDM->GetRowCount() == 0 )	return 0;

	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = m_strDivideAlias;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;

	if( bPrintData == FALSE )
	{
		strAlias[nCondColumns] = _T("BB_대표책여부");		nCond[nCondColumns] = ESL_DATAMGR_SORT_DESC;	nCondColumns++;
	}
	m_pSourceDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	INT cnt, nStart, nSCnt, nNext, ids;
	CString strSpeciesKey, strTmpSpeciesKey, strVolSortNo, strTmpVolSortNo, strTmpData;
	cnt = nStart = nSCnt = nNext = 0;
	if( m_pPrgCtrl )
	{
		m_pPrgCtrl->SetRange(0, m_pSourceDM->GetRowCount());
		m_pPrgCtrl->SetPos(0);
	}
	
	while( m_pSourceDM->GetRowCount() > nStart )
	{
		m_pSourceDM->GetCellData(m_strDivideAlias, nStart, strTmpSpeciesKey);
		nSCnt = FindSameDataCnt(&nNext, m_pSourceDM->GetRowCount(), strTmpSpeciesKey);
		if( bPrintData == TRUE )	ids = ConvertToPrintVolume(nStart, nStart+nSCnt);
		else						ids = ConvertToVolume(nStart, nStart+nSCnt);
		if( ids < 0 )	 return -1;

		nStart = nNext;
		if( m_pPrgCtrl )	m_pPrgCtrl->SetPos(nStart);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 책DM으로 종DM을 구축한다.
// [ 미니스펙 ]
// 1. Progress Ctrl을 초기화한다.
// 2. 해당알리아스의 칼럼 인덱스를 찾는다.
//		- UDF_책수, UDF_종가격, UDF_가격, UDF_자료상태설명, UDF_일련번호
// 3. 가격절사단위 설정값을 가져온다.
// 4. 인자로 온 범위 만큼 수서정보키를 가져와 DM을 복사하고 2번에서 설정된 값들을 DM에 추가 저장한다.
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertSourceInfo(INT nStart, INT nEnd, CString strAlias)
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids, cnt, nDBookCnt, nTmp1, i;
	INT nCol_UDF_BOOKCOUNT, nCol_UDF_SPECIESPRICE, nCol_UDF_PRICE, nCol_UDF_WORKING_STATUS, nCol_UDF_SERIALNO, nCol_UDF_REPRESENTBOOKPRICE ;
	LONG nMinSortNo;
	DOUBLE nTotalPrice;
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strBookPrice;
	CWorkingStatus cWorkingStatus(_T(""));
	
	nMinSortNo		= LONG_MAX;
	nTotalPrice		= 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	//================================================================
	// 1. Progress Ctrl을 초기화한다.
	//================================================================
	if( m_pPrgCtrl )
	{
		m_pPrgCtrl->SetRange(0, nEnd-nStart)	;
		m_pPrgCtrl->SetPos(0);
		m_pPrgCtrl->SetWindowText(_T(""));
	}

	//================================================================
	// 2. 해당알리아스의 칼럼 인덱스를 찾는다.
	//		- UDF_책수, UDF_종가격, UDF_가격, UDF_자료상태설명, UDF_일련번호
	// 2006.03.15 KBS - UDF_단가를 대표책의 가격으로 설정한다.
	//================================================================
	nCol_UDF_BOOKCOUNT		= m_pTargetDM->FindColumn(_T("UDF_책수")		);
	nCol_UDF_SPECIESPRICE	= m_pTargetDM->FindColumn(_T("UDF_종가격")		);
	nCol_UDF_PRICE			= m_pTargetDM->FindColumn(_T("UDF_가격")		);
	nCol_UDF_WORKING_STATUS = m_pTargetDM->FindColumn(_T("UDF_자료상태설명"));
	nCol_UDF_SERIALNO		= m_pTargetDM->FindColumn(_T("UDF_일련번호")	);
	nCol_UDF_REPRESENTBOOKPRICE = m_pTargetDM->FindColumn(_T("UDF_단가")	);
	
	//================================================================
	// 2005-02-03
	// 3. 가격절사단위 설정값을 가져온다.
	//================================================================
	if( m_strPosNumber.IsEmpty() == TRUE )
	{
		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL "
										_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서' ")
//DEL 										 _T("WHERE GROUP_2 = '수서' ")
										 _T("AND GROUP_3 = '단행' ") 
										 _T("AND CLASS_ALIAS = '가격절사단위설정'")), m_strPosNumber);
		
		if( m_strPosNumber.IsEmpty() == TRUE )			m_strPosNumber = _T("0");
	}


	//================================================================
	// 4. 인자로 온 범위 만큼 수서정보키를 가져와 DM을 복사하고 2번에서 설정된 값들을 DM에 추가 저장한다.
	//================================================================
	strBookPrice.Empty();
	for( i=nStart; i<=nEnd; i++ )
	{
		if( m_pPrgCtrl )	m_pPrgCtrl->StepIt();
		
		// 수서정보키를 가져온다.
		m_pSourceDM->GetCellData(strAlias, i, strTmpKey);
		// 새로운 종 입력
		if( strKey.IsEmpty() == FALSE && strKey != strTmpKey )
		{
			// 소스DM -> 타겟DM으로 복사한다.
			DWORD* pRow = (DWORD*)AddToTargetDM(i-1);
			if( pRow == NULL )		break;

			// 위에서 설정된 UDF항목들을 설정한다.
			strTmpData.Format(_T("%d"), cnt);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCOUNT, strTmpData);
			// 2004.12.03
			strTmpData = TruncPrice(m_pTargetDM, nTotalPrice);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SPECIESPRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_PRICE, strTmpData);
			
			strTmpData.Format(_T("%ld"), nMinSortNo);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SERIALNO, strTmpData);

			m_pTargetDM->SetCellData(pRow, nCol_UDF_WORKING_STATUS, cWorkingStatus.GetWorkingStatus());
			// 2006.03.14 KBS - UDF_단가의 값을 대표책의 가격으로 한다.
			m_pTargetDM->SetCellData(pRow, nCol_UDF_REPRESENTBOOKPRICE, strBookPrice);

			strKey = strTmpKey;
			cnt = 1;
			strTmpData = m_pSourceDM->GetCellData(_T("BB_가격"), i);
			//nTotalPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			cWorkingStatus.Init(m_pSourceDM->GetCellData(_T("BB_자료상태"), i));
			strTmpData = m_pSourceDM->GetCellData(_T("BB_일련번호"), i);
			nMinSortNo = _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			cnt++;
			strTmpData = m_pSourceDM->GetCellData(_T("BB_가격"), i);
			//nTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			
			strTmpData = m_pSourceDM->GetCellData(_T("BB_일련번호"), i);
			nTmp1 = _ttoi(strTmpData.GetBuffer(0));
			if( nMinSortNo > nTmp1 )	nMinSortNo = nTmp1;

			strTmpData = m_pSourceDM->GetCellData(_T("BB_자료상태"), i);

			strKey = strTmpKey;
			if( strTmpData.IsEmpty() == TRUE )	continue;
			cWorkingStatus.SetWorkingStatusIfMax(strTmpData);
		}
		// 2006.03.14 KBS - UDF_단가의 값을 대표책의 가격으로 한다.
		if ( _T("Y") == m_pSourceDM->GetCellData(_T("BB_대표책여부"), i) )
		{
			m_pSourceDM->GetCellData(_T("UDF_단가"), i, strBookPrice);
		}
	}

	if( i > nEnd )
	{
		DWORD* pRow = (DWORD*)AddToTargetDM(i-1);
		if( pRow != NULL )
		{
			strTmpData.Format(_T("%d"), cnt);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCOUNT, strTmpData);
			// 2004.12.03
			strTmpData = TruncPrice(m_pTargetDM, nTotalPrice);

			m_pTargetDM->SetCellData(pRow, nCol_UDF_SPECIESPRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_PRICE, strTmpData);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_WORKING_STATUS, cWorkingStatus.GetWorkingStatus());
			strTmpData.Format(_T("%ld"), nMinSortNo);
			m_pTargetDM->SetCellData(pRow, nCol_UDF_SERIALNO, strTmpData);
			// 2006.03.14 KBS - UDF_단가의 값을 대표책의 가격으로 한다.
			m_pTargetDM->SetCellData(pRow, nCol_UDF_REPRESENTBOOKPRICE, strBookPrice);
		}
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로온 idx번째의 로우의 칼럼을 DM복사한다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::AddToTargetDM(INT idx)
{
	if( m_pTargetDM == NULL || m_pSourceDM == NULL || idx < 0 )		return -1000;		
	
	DWORD* pRow = (DWORD*)m_pTargetDM->InsertRow(-2);
	if( pRow == NULL )	return -1000;

	// 사용할 필드 정보를 구축한다.
	INT col, nCol, nCurRow, nColCnt;	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	nCurRow = m_pTargetDM->GetRowCount() - 1;
	nColCnt = m_pSourceDM->m_nCols;
	if( nColCnt < 0 )	nColCnt = m_pSourceDM->GetRefColumnCount();
	
	for( col = 0; col < nColCnt; col++ ) 
	{
		if( m_pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE) != 0 ) 
		{
			continue;
		}
		// 2006.03.14 KBS - UDF_단가의 값을 대표책의 가격으로 한다.
		if( FIELD_ALIAS == _T("UDF_단가") )		continue;

		nCol = m_pTargetDM->FindColumn(FIELD_ALIAS);
		if( nCol >= 0 )
		{
			m_pTargetDM->SetCellData(pRow, nCol, m_pSourceDM->GetCellData(idx, col));
		}
	}
	
	return (INT)pRow;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 알리아스의 값이 인자값과 같은 DM의 갯수를 세어 리턴한다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::FindSameDataCnt(INT *nStart, INT nRowCnt, CString strSpeciesKey)
{
	INT i, cnt = 0;
	CString strTmpData;

	i = *nStart;
	i++;
	while( i < nRowCnt )
	{
		m_pSourceDM->GetCellData(m_strDivideAlias, i, strTmpData);
		if( strTmpData == strSpeciesKey )
		{
			cnt++;
			i++;
		}
		else break;
	}
	*nStart = i;

	return cnt;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 출력용 권DM을 구축한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToPrintVolume(INT nBeginIdx, INT nRowCnt)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL )	return -1;
	
	INT i, j, ids, nDBookCnt, nBookCnt;
	DOUBLE dOrderPrice, dPrice, dDiscRate, dExchangeRate;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG, strData, strVol, strTmpData, strTmpData1, strTmpData2, strPrice;	
	CPriceInfoConversionMgr PriceConvertMgr;
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	// 2006-01-16 강봉수 - 가격정책사항반영, 책발주가격의 합을 권발주가격에 적용한다.
	// 발주가격 소수점적용여부를 검사한다.	
	strVol = _T("");
	for( i=nBeginIdx; i<=nRowCnt; i++ )
	{
		strTmpData.Empty();
		m_pSourceDM->GetCellData(_T("BB_권일련번호"), i, curVOL_SORT);
		// 권일련번호가 같다면
		if( strVol != curVOL_SORT )
		{
			CopyRowIdxList.Add(i);
			strVol = curVOL_SORT;
			}
		}

	// 2005-02-03
	if( m_strPosNumber.IsEmpty() == TRUE )
	{
		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPosNumber);
//DEL 		m_pSourceDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPosNumber);
		if( m_strPosNumber.IsEmpty() == TRUE )		m_strPosNumber = _T("0");
	}

	for( i=0; i<CopyRowIdxList.GetSize(); i++ ) 
	{
		// BOOK의 한 Row를 VOLUME으로 복사한다.
		DWORD *pRow = (DWORD*)AddToTargetDM(CopyRowIdxList.GetAt(i));
		if(pRow == NULL) return -1;

		// 2006-01-18 강봉수 - 가격정책사항반영, 권발주가격을 저장한다.
		dOrderPrice = 0.0;
		m_pSourceDM->GetCellData(_T("BB_권일련번호"), i, curVOL_SORT);
		for( j=nBeginIdx; j<nRowCnt; j++ )
		{
			m_pSourceDM->GetCellData(_T("BB_권일련번호"), j, tempVOL_SORT);
			if( curVOL_SORT == tempVOL_SORT )
			{
				m_pSourceDM->GetCellData(_T("BB_발주가격_책"), j, strTmpData);
				//dOrderPrice += (DOUBLE)atof(strTmpData);
				TCHAR* pEndPtr = NULL;
				dOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			}
		}
		strPrice.Empty();
		strPrice.Format(_T("%lf"), dOrderPrice);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_발주가격_권")), strPrice);
		//-----------------------------------------------------------------

		// 복사된 VOLUME의 같은 책수를 구하여 VOLUME의 같은책수UDF 에 넣어준다.
		curVOL_SORT = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_권일련번호")));
		nBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_책수")), strData);
		strData.Format(_T("%d"), nDBookCnt);
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_납품책수")), strData);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);	
		ids = m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_미납책수")), strData);
		//IBS본표제에 편권차와 권서명 표시
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_편권차")));
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_권서명")));
		strData.Format(_T(". %s , %s"), strTmpData1, strTmpData2);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_표제사항")), strData);
		// 2005-02-04 IBS_서명저작자사항과 IBS_발행사항 추가.
		// 서명저작자사항
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_표제사항"))); 
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_저작자")));
		strData.Empty();
		if(strTmpData2.IsEmpty()) strData = strTmpData1;
		else  strData = strTmpData1 + _T(" / ") + strTmpData2;
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_서명저작자사항")), strData);
		//IBS_저작자 에 책의 저작자를 표시
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_저작자")));
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_저작자")), strData);
		//발행년에 책의 발행년 표시
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_발행년")));
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_발행년")), strData);
		// 2005-02-04 발행자사항
		strTmpData1 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_발행자")));
		strTmpData2 = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_발행년")));
		if(strTmpData2.IsEmpty()) strData = strTmpData1;
		else  strData = strTmpData1 + _T(", ") + strTmpData2;
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_발행자사항")), strData);
				
		//가격정보를 설정한다.
		//dPrice			= (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_가격"))).GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BB_가격"))).GetBuffer(0), &pEndPtr);

		//dDiscRate = (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_할인율"))).GetBuffer(0));
		dDiscRate = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_할인율"))).GetBuffer(0), &pEndPtr);

		//dExchangeRate = (DOUBLE)atof(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_환율"))).GetBuffer(0));
		dExchangeRate = _tcstod(m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_환율"))).GetBuffer(0), &pEndPtr);

		// UDF_단가 = BB_가격+콤마설정
		// 2006-04-01 KBS - 국방대 요구사항 단가필드에는 화폐구분에따라 소수점을 적용한다.
		if( _T("\\") == m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("BP_화폐구분"))) )
		{
			strData.Format(_T("%d"), (INT)dPrice);
		}
		else	strData = TruncPrice(m_pTargetDM, dPrice);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_단가")), strData);
		// UDF_변환가격/변환종가격 = BB_가격*책수
		strData = TruncPrice(m_pTargetDM, dPrice*nBookCnt);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_변환가격")), strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_변환종가격")), strData);

		// 출력용 DM에서 발주가격정보를 설정하지 않는다.
		/*
		//strData.Format(_T("%.2f"), dPrice*nBookCnt - ((dPrice*nBookCnt)*(dDiscRate/100)));		
		strData.Format(_T("%.0f"), (dPrice*nBookCnt - ((dPrice*nBookCnt*dDiscRate)/100)) * dExchangeRate);
		PriceConvertMgr.ConversionPriceInfoToDesc(strData, strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_변환할인가격")), strData);
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_변환할인종가격")), strData);
		*/
		//자료상태정보를 설정한다.
		strData = m_pTargetDM->GetCellData(pRow, m_pTargetDM->FindColumn(_T("IBS_자료상태")));
		strData = strData.Left(3);
		if(strData == _T("BOA")) strData = _T("수서자료");
		else if(strData == _T("BOC")) strData = _T("정리자료");
		else if(strData == _T("BOR")) strData = _T("등록자료");
		else if(strData == _T("BOL")) strData = _T("소장자료");
		m_pTargetDM->SetCellData(pRow, m_pTargetDM->FindColumn(_T("UDF_자료상태설명")), strData);
		//단가정보 변경	
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 같은 알리아스로 나뉜 DM을 권별 정보로 나누어 TargetDM에 INSERT 한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CBO_DataConverter::ConvertToVolume(INT nBeginIdx, INT nRowCnt)
{
	if( m_pSourceDM == NULL || m_pTargetDM == NULL )	return -1;
	
	INT i, j, ids;
	DOUBLE dOrderPrice = 0.0;
	CString curVOL_SORT, strVol, tempVOL_SORT, strDBFLAG, strTmpData, strPrice, strRecKeys;	
	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();
	
	// 2006-01-16 강봉수 - 가격정책사항반영, 책발주가격의 합을 권발주가격에 적용한다.
	// 발주가격 소수점적용여부를 검사한다.	
	strVol = _T("");
	for( i = nBeginIdx; i <= nRowCnt; i++ )
	{
		strTmpData.Empty();
		m_pSourceDM->GetCellData(_T("BB_권일련번호"), i, curVOL_SORT);		
		if( strVol != curVOL_SORT )
		{
			CopyRowIdxList.Add(i);
			strVol = curVOL_SORT;
		}
	}
	
	INT nCol_VOL_SORTNO				= m_pTargetDM->FindColumn(_T("BB_권일련번호")	);
	INT nCol_UDF_BOOKCNT			= m_pTargetDM->FindColumn(_T("UDF_책수")		);
	INT nCol_UDF_DELIVERY_BOOKCNT	= m_pTargetDM->FindColumn(_T("UDF_납품책수")	);
	INT nCol_UDF_MISSING_BOOKCNT	= m_pTargetDM->FindColumn(_T("UDF_미납책수")	);
	INT nCol_PUB_YEAR				= m_pTargetDM->FindColumn(_T("IBS_발행년")		);
	INT nCol_ORDERPRICE_VOL			= m_pTargetDM->FindColumn(_T("UDF_발주가격_권")	);
	INT nCol_BOOK_RECKEY			= m_pTargetDM->FindColumn(_T("BB_책KEY")	);

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT nDBookCnt, nBookCnt;
	CString strData;
	for (i = 0; i < nCopyRowIdxList; i++) 
	{
		// BOOK의 한 Row를 VOLUME으로 복사한다.
		DWORD *pRow = (DWORD*)AddToTargetDM(CopyRowIdxList.GetAt(i));
		if( pRow == NULL )		return -1;

		// 2006-01-18 강봉수 - 가격정책사항반영, 권발주가격을 저장한다.
		dOrderPrice = 0.0;
		strRecKeys.Empty();
		m_pSourceDM->GetCellData(_T("BB_권일련번호"), CopyRowIdxList.GetAt(i), curVOL_SORT);
		for( j=nBeginIdx; j<=nRowCnt; j++ )
		{
			m_pSourceDM->GetCellData(_T("BB_권일련번호"), j, tempVOL_SORT);
			if( curVOL_SORT == tempVOL_SORT )
			{
				m_pSourceDM->GetCellData(_T("BB_발주가격_책"), j, strTmpData);
				//dOrderPrice += (DOUBLE)atof(strTmpData);
				TCHAR* pEndPtr = NULL;
				dOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			}
			// 2006-01-24 강봉수 - 가격정책사항반영, 책키값을 저장한다. ( 오차출력 )
			m_pSourceDM->GetCellData(_T("BB_책KEY"), j, strTmpData);
			if( strRecKeys.IsEmpty() == TRUE )	strRecKeys = strTmpData;
			else								strRecKeys += _T(", ") + strTmpData;
		}
		strPrice.Empty();
		strPrice.Format(_T("%lf"), dOrderPrice);
		m_pTargetDM->SetCellData(pRow, nCol_ORDERPRICE_VOL, strPrice);
		m_pTargetDM->SetCellData(pRow, nCol_BOOK_RECKEY, strRecKeys);
		//-----------------------------------------------------------------
		
		// 복사된 VOLUME의 같은 책수를 구하여 VOLUME의 같은책수UDF 에 넣어준다.
		curVOL_SORT = m_pTargetDM->GetCellData(pRow, nCol_VOL_SORTNO);
		nBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt);
		nDBookCnt = GetEqualBookCount(m_pSourceDM, curVOL_SORT, nBeginIdx, nRowCnt, _T("Y"));
		strData.Format(_T("%d"), nBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_BOOKCNT, strData);
		strData.Format(_T("%d"), nDBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_DELIVERY_BOOKCNT, strData);
		strData.Format(_T("%d"), nBookCnt - nDBookCnt);
		m_pTargetDM->SetCellData(pRow, nCol_UDF_MISSING_BOOKCNT, strData);
		strData = m_pTargetDM->GetCellData(pRow, nCol_PUB_YEAR);
	}

	ids = m_pTargetDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	return 0;
}

INT CBO_DataConverter::GetEqualBookCount(CESL_DataMgr *pDM_BOOK, CString &strVOL, INT nBeginIdx, INT nRowCnt, CString strDeliveryYN/*=_T("")*/)
{
	if( pDM_BOOK == NULL )		return 0;

	INT ids, i, nEqualBookCount = 0;
	CString tempVOL, strTmpData;
	for( i=nBeginIdx; i<=nRowCnt; i++ )
	{
		ids = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i, tempVOL);
		pDM_BOOK->GetCellData(_T("BB_납품여부"), i, strTmpData);
		if( strVOL == tempVOL )
		{
			if( strDeliveryYN.IsEmpty() == TRUE )								nEqualBookCount++;
			else if( !strDeliveryYN.IsEmpty() && strDeliveryYN == strTmpData )	nEqualBookCount++;
		}	
	}
	
	return nEqualBookCount;
}

CString CBO_DataConverter::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 가격정책에 따라 절사단위설정값을 읽어와서 지정된 자릿수만큼 표기하도록 한다.
	INT nPos = -1;
	CString strTmpData, strTmp, strValue;
	

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-31
	if		( m_strPosNumber == _T("1") )	strTmpData = strTmp.Left(nPos+2);
	else if	( m_strPosNumber == _T("2") )	strTmpData = strTmp.Left(nPos+3);
	else									strTmpData = strTmp.Left(nPos);

	return strTmpData;
}
