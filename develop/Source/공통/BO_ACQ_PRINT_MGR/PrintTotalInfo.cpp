// PrintTotalInfo.cpp: implementation of the CPrintTotalInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintTotalInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CPrintTotalInfo::CPrintTotalInfo()
{
	m_strBookCntAlias	= _T("");
	m_strPriceAlias		= _T("");
	m_strDiscPriceAlias = _T("");
	InitTotalInfo();
	m_strTruncPrice.Empty();

	// KOL.UDF.022 시큐어코딩 보완
	InitTotalInfo();
	m_nAcqType = -1;	
}
// 소멸자
CPrintTotalInfo::~CPrintTotalInfo()
{

}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 멤버변수들의 초기화
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::InitTotalInfo()
{
	m_dTotalPrice		= 0.0;
	m_dDiscTotalPrice_Display	= 0.0;
	m_dDiscTotalPrice	= 0.0;
	m_nTotalBookCnt		= 0;
	m_nTotalSpeciesCnt	= 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 가격절사단위설정에따라 _T("가격")을 표현하여 리턴한다.(콤마를 찍어줌) -> 변환할인가격
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalPrice(CString &strTotalPrice)
{
	//2005-01-17
	if		( m_strTruncPrice == _T("1") )	strTotalPrice.Format(_T("%.1f"), m_dTotalPrice);
	else if	( m_strTruncPrice == _T("2") )	strTotalPrice.Format(_T("%.2f"), m_dTotalPrice);
	else								strTotalPrice.Format(_T("%.0f"), m_dTotalPrice);

	// 콤마를 찍어줌
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 구입가격(발주가격)을 리턴한다.(콤마를 찍어줌)
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetDiscTotalPrice(CString &strDiscTotalPrice, CString strTruncUnit)
{
	// 육사에서는 구입가격 십원단위 처리
	// 2005.11.02 강봉수 추가	
	INT nPrice = 0;		
	
	if		( strTruncUnit == _T("0") )
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDiscTotalPrice);
	}
	else if	( strTruncUnit == _T("1") )
	{
		strDiscTotalPrice.Format(_T("%.1f"), m_dDiscTotalPrice);
	}
	else if	( strTruncUnit == _T("2") )
	{
		strDiscTotalPrice.Format(_T("%.2f"), m_dDiscTotalPrice);
	}
	else
	{
		strDiscTotalPrice.Format(_T("%.0f"), m_dDiscTotalPrice);
	}
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strDiscTotalPrice, strDiscTotalPrice);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 총종수를 인자로 리턴한다.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalSpeciesCnt(CString &strTotalSpeciesCnt)
{
	strTotalSpeciesCnt.Format(_T("%d"), m_nTotalSpeciesCnt);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 총책수를 인자로 리턴한다.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::GetTotalBookCnt(CString &strTotalBookCnt)
{
	strTotalBookCnt.Format(_T("%d"), m_nTotalBookCnt);
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 가격알리아스 값이 있는지 확인한다.
//////////////////////////////////////////////////////////////////////
BOOL CPrintTotalInfo::CheckTotalInfoAlias()
{
	if( m_strPriceAlias.IsEmpty() == TRUE )		return FALSE;
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 온 알리아스를 멤버변수로 설정한다.
//////////////////////////////////////////////////////////////////////
VOID CPrintTotalInfo::SetAliasInfo(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_strBookCntAlias	= strBookCntAlias;
	m_strPriceAlias		= strPriceAlias;
	m_strDiscPriceAlias = strDiscPriceAlias;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종, 책, 가격의 총합을 구하여 멤버변수에 저장한다. (소계자료)
//
// [ 미니스펙 ]
// 1. 재계산을 위하여 멤버변수를 초기화한다.
// 2. 가격표현범위 값을 처리하기 위하여 가격자릿수 설정값을 가져온다.
// 3. 인자로 온 DM의 범위값이 음수라면 범위를 전체로 설정한다.
// 4. 계산하려는 DM의 갯수가 1000개가 넘는다면 1000개씩 계산한다.
// 5. 나머지 로우에 해당하는 DM의 총계를 구한다.
//
//////////////////////////////////////////////////////////////////////
INT CPrintTotalInfo::SetTotalInfo(CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, q, nQuota, nCnt, nRow ;
	DOUBLE dTempPrice;
	CString strTmpData, strQuery, strDisPrice;
	CStringArray astrAcqKeys;

	TCHAR* pEndPtr = NULL;

	if( pDM == NULL )						return -1;
	if( pDM->GetRowCount() == 0 )			return  0;
	if( CheckTotalInfoAlias() == FALSE )	return -1;

	//================================================================
	// 1. 재계산을 위하여 멤버변수를 초기화한다.
	//================================================================
	InitTotalInfo();


	//================================================================
	// 2. 가격표현범위 값을 처리하기 위하여 가격자릿수 설정값을 가져온다.
	// 2005-01-17
	//================================================================
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strTruncPrice);
		// default : 소수점 적용안함.
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}


	//================================================================
	// 3. 인자로 온 DM의 범위값이 음수라면 범위를 전체로 설정한다.
	//================================================================
	if( nFirstIdx < 0 )		nFirstIdx = 0;
	if( nLastIdx < 0 )		nLastIdx = pDM->GetRowCount();

	//================================================================
	// 4. 계산하려는 DM의 갯수가 1000개가 넘는다면 1000개씩 계산한다.
	// 2005-04-15 by SM5ong 합계를 1000개씩 끊어서 계산하도록 한다.
	//================================================================
	// BEGIN:
	m_dDiscTotalPrice_Display = 0.0;
	nCnt = nRow = 0;	
	dTempPrice = 0.0;
	astrAcqKeys.RemoveAll();
	nQuota = (nLastIdx-nFirstIdx) / QUOTA_PRINT; // 몫
	if( nQuota > 0 )
	{
		// 1000단위로 끊어서 합계를 구한다.
		for( q=0; q<nQuota; q++ )  
		{
			if( q == 0 )	nCnt = nFirstIdx;
			else			nCnt = 0;
	
			for( i=nCnt ; i<QUOTA_PRINT ; i++ )
			{
				// 현재로우를 계산한다.
				nRow = (q*QUOTA_PRINT) + i;

				// 종수 증가
				m_nTotalSpeciesCnt++;
				
				// 책수 증가
				if( m_strBookCntAlias.IsEmpty() == FALSE )
				{
					//strTmpData = pDM->GetCellData(m_strBookCntAlias, nRow);
					strTmpData = pDM->GetCellData(_T("UDF_책수"), nRow);
					m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
				}
				else 
				{
					m_nTotalBookCnt++;
				}

				// 가격 총계 계산
				if( m_strPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strPriceAlias, nRow);
					strTmpData.Replace(_T(","), _T(""));
					//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
					m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
				}

				// 구입가격(발주가격) 계산을 위한 REC_KEY를 저장한다.
				if( m_strDiscPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strDiscPriceAlias, nRow);					
					if( strTmpData.IsEmpty() == TRUE )	continue;
					strTmpData.Replace(_T(","), _T(""));
					//m_dDiscTotalPrice_Display += (DOUBLE)atof(strTmpData);
					m_dDiscTotalPrice_Display += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
					
					// 수서정보KEY가 아닌 책의 REC_KEY로 구해와 책단위의 발주가격을 계산한다.
					strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), nRow);
					if( strTmpData.IsEmpty() == TRUE )	continue;

					astrAcqKeys.Add(strTmpData);
					//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
					//else									strDisPrice += _T(", ") + strTmpData;
				}				
			}

			// 저장된 REC_KEY를 이용하여 총계를 구한다.
			if( astrAcqKeys.GetSize() > 0 )
			{
				strDisPrice.Empty();
				strDisPrice = astrAcqKeys.GetAt(0);
				for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
				// 2006.02.01 강봉수 - 가격정책사항 최종 반영, 종발주가격의 합이 총계이다.
				if( m_nAcqType == 2 )
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
									_T("FROM BO_DONINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				else
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
									_T("FROM BO_PURCHASEINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				/*
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL ")
								_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
				*/
				pDM->GetOneValueQuery(strQuery, strTmpData);
				//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			astrAcqKeys.RemoveAll();
			
			// 2006-01-23 강봉수 - 가격정책사항반영, 구입가격 총계는 한번에 계산한다.
			/*
			// 저장된 REC_KEY를 이용하여 구입가격의 총합을 구한다.
			if( strDisPrice.IsEmpty() == FALSE )
			{
				strQuery.Format(_T("SELECT SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y')) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
				pDM->GetOneValueQuery(strQuery, strTmpData);
				dTempPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			}
			else dTempPrice = 0;

			// 총계에 더한다.
			m_dDiscTotalPrice += dTempPrice;
			*/
		}
	}

	//================================================================
	// 5. 나머지 로우에 해당하는 DM의 총계를 구한다.
	//================================================================
	dTempPrice = 0.0;
	if( nQuota==0 ) nCnt = nFirstIdx;
	else			nCnt = nQuota*QUOTA_PRINT;

	for( i=nCnt ; i<nLastIdx ; i++ )
	{
		// 종수 증가
		m_nTotalSpeciesCnt++;
		
		// 책수 증가
		if( m_strBookCntAlias.IsEmpty() == FALSE )
		{
			//strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			strTmpData = pDM->GetCellData(_T("UDF_책수"), i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}

		// 가격 합산
		if( m_strPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);			
			strTmpData.Replace(_T(","), _T(""));
			//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		}

		// 구입가격 총계 합산을 위한 REC_KEY 저장
		if( m_strDiscPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strDiscPriceAlias, i);			
			if( strTmpData.IsEmpty() == TRUE )	continue;
			strTmpData.Replace(_T(","), _T(""));
			//m_dDiscTotalPrice_Display += (DOUBLE)atof(strTmpData);
			m_dDiscTotalPrice_Display += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
			
			// 2005-02-02
			strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), i);
			//strTmpData = pDM->GetCellData(_T("BB_책KEY"), i);
			if( strTmpData.IsEmpty() == TRUE )	continue;

			astrAcqKeys.Add(strTmpData);
			//if( strDisPrice.IsEmpty()  == TRUE )	strDisPrice += strTmpData;
			//else									strDisPrice += _T(", ") + strTmpData;
		}
		
	}

	// 저장된 REC_KEY를 이용하여 총계를 구한다.
	if( astrAcqKeys.GetSize() > 0 )
	{
		strDisPrice.Empty();
		strDisPrice = astrAcqKeys.GetAt(0);
		for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);	
		// 2006.02.01 강봉수 - 가격정책사항 최종 반영, 종발주가격의 합이 총계이다.
		if( m_nAcqType == 2 )
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
							_T("FROM BO_DONINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		else
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
							_T("FROM BO_PURCHASEINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		/*
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
		*/
		pDM->GetOneValueQuery(strQuery, strTmpData);
		//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	else
	{
		dTempPrice = 0;
	}

	m_dDiscTotalPrice += dTempPrice;	
	// END:
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종, 책, 가격의 총합을 구하여 멤버변수에 저장한다.
//////////////////////////////////////////////////////////////////////
INT CPrintTotalInfo::SetTotalInfo(CMapStringToString * pArrData, CString strDivAlias, CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, q, nQuota, nCnt, nRow;
	DOUBLE dTempPrice;
	CString strTmpData, strTmpValue, strQuery, strDisPrice;
	CStringArray astrAcqKeys;

	if( pDM == NULL )						return -1;
	if( pDM->GetRowCount() == 0 )			return 0;	
	if( CheckTotalInfoAlias() == FALSE )	return -1;

	
	//================================================================
	// 1. 재계산을 위하여 멤버변수를 초기화 시킨다.
	//================================================================
	InitTotalInfo();


	//================================================================
	// 2. 소수점 적용을 위한 가격자릿수 설정값을 가져온다.
	// 2005-01-17
	//================================================================
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}
	

	//================================================================
	// 3. 계산할 범위가 음수일경우에 전체를 계산한다.
	//================================================================
	if( nFirstIdx < 0 )		nFirstIdx = 0;
	if( nLastIdx < 0 )		nLastIdx = pDM->GetRowCount();

	//================================================================
	// 4. 계산하려는 DM의 갯수가 1000개가 넘는다면 1000개씩 계산한다.
	// 2005-04-15 by SM5ong 합계를 1000개씩 끊어서 계산하도록 한다.
	//================================================================
	// BEGIN:
	nCnt = nRow = 0;
	astrAcqKeys.RemoveAll();
	nQuota = (nLastIdx-nFirstIdx) / QUOTA_PRINT; // 몫
	if( nQuota > 0 )
	{
		// 1000단위로 끊어서 합계를 구한다.
		for( q=0; q<nQuota; q++ )  
		{
			strDisPrice.Empty();
			dTempPrice = 0.0;
			if( q==0 )	nCnt = nFirstIdx;
			else		nCnt = 0;
	
			for( i=nCnt ; i<QUOTA_PRINT ; i++ )
			{
				// 현재 로우를 계산한다.
				nRow = (q * QUOTA_PRINT) + i;

				strTmpData = pDM->GetCellData(strDivAlias, nRow);
				if( pArrData && pArrData->GetCount() > 0 )
				{
					if( !pArrData->Lookup(strTmpData, strTmpValue) )	continue;
				}

				// 종수 증가
				m_nTotalSpeciesCnt++;
				
				// 책수 증가
				if( m_strBookCntAlias.IsEmpty()  == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strBookCntAlias, nRow);
					m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
				}
				else 
				{
					m_nTotalBookCnt++;
				}

				// 가격 계산
				if( m_strPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(m_strPriceAlias, nRow);
					strTmpData.Replace(_T(","), _T(""));
					//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
					TCHAR* pEndPtr = NULL;
					m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
				}

				// 총합의 계산을 위해 REC_KEY를 저장한다.
				if( m_strDiscPriceAlias.IsEmpty() == FALSE )
				{
					strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), nRow);
					//strTmpData = pDM->GetCellData(_T("BB_책KEY"), nRow);
					if( strTmpData.IsEmpty() == TRUE ) continue;

					astrAcqKeys.Add(strTmpData);
					//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
					//else									strDisPrice += _T(", ") + strTmpData;
				}
			}

			// 1000 개씩 잘라서 구입가격을 계산한다.
			if( astrAcqKeys.GetSize() > 0  )
			{
				strDisPrice.Empty();
				strDisPrice = astrAcqKeys.GetAt(0);
				for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
				// 2006.02.01 강봉수 - 가격정책사항 최종 반영, 발주가격의합으로 총계를 계산한다.
				if( m_nAcqType == 2 )
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
									_T("FROM BO_DONINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				else
				{
					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 					strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
									_T("FROM BO_PURCHASEINFO_TBL ")
									_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
				}
				/*
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
				
				// 쿼리문 변경 - 책발주가격의 합이 아닌 종발주가격의 합으로 가져온다.
				strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
								_T("FROM BO_BOOK_TBL ")
								_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
				*/
				pDM->GetOneValueQuery(strQuery, strTmpData);
				//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				TCHAR* pEndPtr = NULL;
				dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			astrAcqKeys.RemoveAll();

			/*
			if( strDisPrice.IsEmpty() == FALSE )
			{
				strQuery.Format(_T("SELECT SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, 'Y')) ")
								_T("FROM BO_BOOK_TBL B ")
								_T("WHERE b.ACQ_KEY in ( %s ) "), strDisPrice);
				pDM->GetOneValueQuery(strQuery, strTmpData);
				dTempPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			}
			else dTempPrice = 0;

			m_dDiscTotalPrice += dTempPrice;
			*/
		}
	}

	//================================================================
	// 5. 나머지 로우에 해당하는 DM의 총계를 구한다.
	//================================================================
	strDisPrice.Empty();
	dTempPrice = 0.0;
	if( nQuota==0 )	nCnt=nFirstIdx;
	else			nCnt=nQuota*QUOTA_PRINT;

	for( i=nCnt ; i<nLastIdx ; i++ )
	{
		strTmpData = pDM->GetCellData(strDivAlias, i);
		if( pArrData && pArrData->GetCount() > 0 )
		{
			if( !pArrData->Lookup(strTmpData, strTmpValue) )	continue;
		}

		m_nTotalSpeciesCnt++;
		
		if( m_strBookCntAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}

		if( m_strPriceAlias.IsEmpty() == FALSE )
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);
			strTmpData.Replace(_T(","), _T(""));
			//m_dTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
			TCHAR* pEndPtr = NULL;
			m_dTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
		}

		if( m_strDiscPriceAlias.IsEmpty() == FALSE )
		{
			// 수서정보KEY대신 책KEY로 설정한다.
			strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), i);
			//strTmpData = pDM->GetCellData(_T("BB_책KEY"), i);
			if( strTmpData.IsEmpty() == TRUE )	continue;

			astrAcqKeys.Add(strTmpData);
			//if( strDisPrice.IsEmpty() == TRUE )		strDisPrice += strTmpData;
			//else									strDisPrice += _T(", ") + strTmpData;
		}
	}

	if( astrAcqKeys.GetSize() > 0  )
	{
		strDisPrice.Empty();
		strDisPrice = astrAcqKeys.GetAt(0);
		for( i=1; i<astrAcqKeys.GetSize(); i++ )	strDisPrice += _T(", ") + astrAcqKeys.GetAt(i);
		// 2006.02.01 강봉수 - 가격정책사항 최종 반영, 발주가격의합으로 총계를 계산한다.
		if( m_nAcqType == 2 )
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(PRICE)) ")
							_T("FROM BO_DONINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		else
		{
			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE, UDF_MANAGE_CODE)) ")
//DEL 			strQuery.Format(_T("SELECT SUM(GET_ORDERPRICE(ORDER_PRICE)) ")
							_T("FROM BO_PURCHASEINFO_TBL ")
							_T("WHERE REC_KEY IN ( %s ) "), strDisPrice);
		}
		/*
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(b.ACQ_KEY, b.PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL B ")
						_T("WHERE b.ACQ_KEY IN ( %s ) "), strDisPrice);
		
		// 쿼리문 변경 - 책발주가격의 합이 아닌 종발주가격의 합으로 가져온다.
		strQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GETPRICEWON(ACQ_KEY, PRICE, 'Y'))) ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE ACQ_KEY IN ( %s ) "), strDisPrice);
		*/
		pDM->GetOneValueQuery(strQuery, strTmpData);
		//dTempPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dTempPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	else dTempPrice = 0;

	m_dDiscTotalPrice += dTempPrice;
	// END:

	return 0;
}

VOID CPrintTotalInfo::SetDiscTotalPrice(DOUBLE dTmpPrice)
{
	//20070820 UPDATE PJW :총합을 저장 dTmpPrice = m_dDiscTotalPrice; 로 변경 
	//총합을 저장할 변수를 받아와서 총합이 있는 변수에 넣어서 구입가격이 0.00으로 나왔음 
	//m_dDiscTotalPrice = dTmpPrice;
	//20070903 UPDATE PJW : 구입과 기증에서 총합에 사용되는 변수가 달라서 값이 제대로 들어가지 않았음.
	if( m_dDiscTotalPrice == 0.)
	{
		m_dDiscTotalPrice = dTmpPrice;
	}
	else
	{
		dTmpPrice = m_dDiscTotalPrice;		
	}
}