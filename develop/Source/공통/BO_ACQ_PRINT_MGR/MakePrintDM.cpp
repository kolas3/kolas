// MakePrintDM.cpp: implementation of the CMakePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakePrintDM.h"
#include "../BO_DATA_MANAGER/BO_DataConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakePrintDM::CMakePrintDM(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_pPrintDM = NULL;
	m_nPrintType = -1;
	m_IgnoreAlias.RemoveAll();
	m_pDataProcessingDlg = NULL;
	m_pArrDivValeu = NULL;
	m_strTruncPrice = _T("");
	m_strPostNum	= _T("");
}

CMakePrintDM::~CMakePrintDM()
{
	//프로그래스 컨트롤 숨기기
	HideDataProcessingDlg();
}

INT CMakePrintDM::InitMakePrintDM()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 책DM을 이용하여 권정보DM을 생성한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CMakePrintDM::MakeVolumeDM(CESL_DataMgr * pDM, 
							   CESL_DataMgr * pBookDM, 
							   CString strDivAlias, 
							   BOOL bPrintVolume/*=TRUE*/)
{
	//================================================================
	// 변수 정의
	//================================================================
	CBO_DataConverter Converter;

	//================================================================
	// 1. DM이 설정되어 있지 않다면 리턴한다.
	//================================================================
	if(pBookDM == NULL) return -1;

	//================================================================
	// 2. 출력할 DM에 내용이 없으면 리턴한다.
	//================================================================
	if(pBookDM->GetRowCount() == 0) return 0;
	

	//================================================================
	// 3. Progress Ctrl Dialog 창이 있다면 메시지 및 위치를 설정한다.
	//================================================================
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 권정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}

	//================================================================
	//  4. 출력용 DM을 구축하기 위해 출력용DM의 데이터를 비운다.
	//================================================================
	pDM->FreeData();
	
	
	//================================================================
	// 5. DM 및 구분알리아스를 
	//================================================================
	Converter.SetDivAlias(strDivAlias);
	Converter.SetProgressCtrl(m_pDataProcessingDlg->GetProgressCtrl());
	Converter.SetSourceDM(pBookDM);
	Converter.SetTargetDM(pDM);

	return (Converter.ConvertToVolumeInfo(bPrintVolume));
}

INT CMakePrintDM::MakeSpeciesDM(CESL_DataMgr * pDM, CESL_DataMgr * pBookDM, BOOL bIndex)
{
	INT ids;
	CString strAlias;
	CBO_DataConverter Converter;

	if( pBookDM == NULL )				return -1;
	if( pBookDM->GetRowCount() == 0 )	return 0;
	
	if( m_pDataProcessingDlg != NULL )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 종정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}

	pDM->FreeData();

	if( bIndex == TRUE )	strAlias = _T("IBS_종KEY");
	else strAlias = _T("BP_수서정보KEY");
	
	Converter.SetDivAlias(_T("IBS_종KEY"));
	Converter.SetProgressCtrl(NULL);
	Converter.SetSourceDM(pBookDM);
	Converter.SetTargetDM(pDM);

	ids = Converter.ConvertToSpeciesInfo();
	return ids;
}

INT CMakePrintDM::MakePrintDM()
{
	return 0;
}

INT CMakePrintDM::CheckValid()
{
	return 0;
}

VOID CMakePrintDM::SetTotalInfoAlias(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_cTotalInfo.SetAliasInfo(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM을 복사한다.
// [ 미니스펙 ]-생략
//////////////////////////////////////////////////////////////////////
INT CMakePrintDM::CopyDM(CESL_DataMgr *pDM_From, 
						 INT nFromIdx, 
						 CESL_DataMgr *pDM_To, 
						 DWORD * pRow, 
						 CString strToAliasHeader /*= _T("")*/, 
						 CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if( pDM_From == NULL || pDM_To == NULL )	return -1;
	
	INT ids, startpos;		
	BOOL bSwitchHeader = FALSE;
	CString strAlias, strData, strTmpData;
	
	if( pDM_From->GetRowCount()-1 < nFromIdx || nFromIdx < 0 )	return -1;	
	
	if( pRow == NULL )
	{
		pRow = (DWORD*)pDM_To->InsertRow(-2);
		if( pRow == NULL )	return -1;
	}	
	
	if( strToAliasHeader.IsEmpty() == FALSE )	bSwitchHeader = TRUE;
	
	CESL_DataMgr::reference* pRef = NULL;	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	startpos = -1;
	while( pos )
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if( pRef == NULL )		continue;
		
		strAlias = pRef->FIELD_ALIAS;
		
		// 거부 리스트에 있는지 확인
		if( pArrIgnoreAlias != NULL )
		{
			if( IsStringInArray(pArrIgnoreAlias, strAlias) )	continue;
		}
			ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
		if( ids < 0 )	continue;
			
			// 헤더 변경
		if( bSwitchHeader == TRUE )
			{
				strAlias = strAlias.Mid(strAlias.Find(_T("_")));
				strAlias = strToAliasHeader + strAlias;
			}
		ids = pDM_To->SetCellData(pRow, pDM_To->FindColumn(strAlias), strData);
		if( ids < 0 )	continue;
	}
	
	return (INT)pRow;
}

INT CMakePrintDM::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	
	INT ids = -1;
	BOOL bSwitchHeader = FALSE;
	CString strAlias, strData, strTmpData;
	CESL_DataMgr::reference *pRef = NULL;
	
	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;
	
	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;
	
	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	
	
	if( !strToAliasHeader.IsEmpty() )	bSwitchHeader = TRUE;	
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	INT startpos = -1;
	while( pos )
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if( pRef == NULL )	continue;
		
		strAlias = pRef->FIELD_ALIAS;
		// 거부 리스트에 있는지 확인
		if( pArrIgnoreAlias != NULL )
		{
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		}
			
			ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
			if (ids < 0) continue;
			
			// 헤더 변경
			if (bSwitchHeader)
			{
				strAlias = strAlias.Mid(strAlias.Find(_T("_")));
				strAlias = strToAliasHeader + strAlias;
			}
			ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
			if (ids < 0) continue;
	}
	
	return nToIdx;
}

BOOL CMakePrintDM::IsStringInArray(CStringArray *pArr, CString str)
{
	if (pArr == NULL || str.IsEmpty()) return FALSE;
	
	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;
	
	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}
	
	return FALSE;
}


INT CMakePrintDM::InsertObjInfo(CESL_DataMgr * pDM, 
								CESL_DataMgr * pDM_OBJ, 
								CString strKey, 
								PRINT_TYPE nType)
{
	if( strKey.IsEmpty() == TRUE )	return 1000;
	
	switch( nType )
	{
	case SPECIES_PRINT:		return InsertVolumeInfo(pDM, pDM_OBJ, strKey);
		break;
	case VOLUME_PRINT:		return InsertSpeciesInfo(pDM, pDM_OBJ, -1, strKey);
		break;
	case BOOK_PRINT:		return InsertSpeciesInfo(pDM, pDM_OBJ, -1, strKey);
		break;
	}
	
	return -1;
}

INT CMakePrintDM::InsertObjInfo(CESL_DataMgr * pDM, CESL_DataMgr * pDM_OBJ, INT nCurIdx, CString strKey, PRINT_TYPE nType)
{
	if(strKey.IsEmpty()) return 1000;

	if(nType == SPECIES_PRINT)
		return InsertVolumeInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	else if(nType == VOLUME_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	else if(nType == BOOK_PRINT)
		return InsertSpeciesInfo(pDM, pDM_OBJ, nCurIdx, strKey);
	return -1;
}

// 권정보를 삽입한다.
INT CMakePrintDM::InsertVolumeInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, CString strKey)
{
	CArray<INT, INT> IdxArray;	IdxArray.RemoveAll();
	
	INT ids, nCnt, i, nReturnVal = 0;
	
	ids = IsMultiVolume(pDM_OBJ, strKey, &IdxArray);
	if( ids < 0 )	return ids;
	
	if( IdxArray.GetSize() > 1 )
	{
		nCnt = IdxArray.GetSize();
		for( i=0 ; i<nCnt ; i++ )
		{
			DWORD* pRow = (DWORD*)pDM->InsertRow(-2);
			if( pRow == NULL )	return -1;
			ids = CopyDM(pDM_OBJ, IdxArray.GetAt(i), pDM, pRow, _T(""), &m_IgnoreAlias);
			if( ids < 0 )	return ids;
		}
		nReturnVal = 1000;
	}

	return nReturnVal;
}

INT CMakePrintDM::InsertVolumeInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, INT nCurIdx, CString strKey)
{
	CArray<INT, INT> IdxArray;	IdxArray.RemoveAll();
	
	CString strTMpData;
	strTMpData.Format(_T("Key Value : %s"), strKey);
	//=====================================================
	//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 	FileLog(strTMpData);
	//=====================================================
	
	INT ids = IsMultiVolume(pDM_OBJ, strKey, &IdxArray);
	if(ids < 0) return ids;

	if(IdxArray.GetSize() > 1)
	{
		INT nCnt = IdxArray.GetSize();
		for(INT i=0 ; i<nCnt ; i++)
		{
			pDM->InsertRow(-1);
			ids = CopyDM(pDM_OBJ, IdxArray.GetAt(i), pDM, pDM->GetRowCount()-1, _T(""), &m_IgnoreAlias);
			if(ids < 0) return ids;
		}
	}
	return 0;
}

INT CMakePrintDM::InsertSpeciesInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, INT nCurIdx, CString strKey)
{
	return 0;
}

INT CMakePrintDM::IsMultiVolume(CESL_DataMgr *pDM, CString strKey, CArray<INT, INT> * pIdxArray)
{
	INT nPivot, i, nIdx;
	LONG nKeyValue;
	CString strTmp;
	
	nKeyValue	= (LONG)_ttol(strKey.GetBuffer(0));
	nPivot		= pDM->GetRowCount()/2;
	if( nPivot == 0 )	nPivot = 1;	
	
	//=====================================================
	//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 	for( i=0 ; i<pDM->GetRowCount() ; i++ )
// 	{	
// 		strTmp.Format(_T("%d \t\t %s"), i, pDM->GetCellData(m_strDivAlias, i));
// 		FileLog(strTmp);
// 	}
	//=====================================================


	nIdx = FindVolume(pDM, nKeyValue, nPivot, 1, pDM->GetRowCount());
	if( nIdx < 0 )	return -1;
	
	GetMultiVolumeIdx(pDM, nIdx, pIdxArray);

	return 0;
}

INT CMakePrintDM::AddIdxByOrdered(CArray<INT, INT> *pIdxArray, INT nNewData)
{
	INT nCnt = pIdxArray->GetSize();
	if(nCnt == 0) pIdxArray->Add(nNewData);
	else
	{
		INT nTmpData;
		for(INT i=0 ; i<nCnt ; i++)
		{
			nTmpData = pIdxArray->GetAt(i);
			if(nTmpData >= nNewData)
			{
				pIdxArray->InsertAt(i, nNewData);
				break;
			}
		}
		if(i == nCnt) pIdxArray->Add(nNewData);
	}
	return 0;
}

INT CMakePrintDM::GetMultiVolumeIdx(CESL_DataMgr * pDM, INT nIdx, CArray<INT, INT> *pIdxArray)
{
	INT i;
	CString strKey, strTmpData;
	
	strKey = pDM->GetCellData(m_strDivAlias, nIdx);
	for( i=nIdx-1 ; i>=0 ; i-- )
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if( strTmpData == strKey )	AddIdxByOrdered(pIdxArray, i);
		else break;
	}
	for( i=nIdx+1 ; i<pDM->GetRowCount() ; i++ )
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if( strTmpData == strKey )	AddIdxByOrdered(pIdxArray, i);
		else break;
	}

	AddIdxByOrdered(pIdxArray, nIdx);
	
	return 0;
}

INT CMakePrintDM::FindVolume(CESL_DataMgr *pDM, LONG nKeyValue, INT nPivot, LONG nFirstValue, LONG nLastValue)
{	
	CString strTmpData;
	strTmpData.Format(_T("초기값:\t%d\t\tPivot:\t%d\t\t마지막값:\t%d"), nFirstValue, nPivot, nLastValue);
	//=====================================================
	//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 	FileLog(strTmpData);
	//=====================================================
	
	if(nFirstValue > nLastValue) 
	{
		//=====================================================
		//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 		FileLog(_T("Not Found!!"));
		//=====================================================
		
		return -1;
	}

	if(nPivot < 1 || nPivot > pDM->GetRowCount())
		return -1;

	CString strTmpKey = pDM->GetCellData(m_strDivAlias, nPivot-1);
	if(strTmpKey.IsEmpty())
		return -1;
	
	LONG nTmpData = _ttol(strTmpKey.GetBuffer(0));
	INT nWeight;

	if(nKeyValue > nTmpData)
	{
		nWeight = (nLastValue-nPivot)/2;
		if(nWeight == 0) nWeight = 1;
		INT nNextPivot = nPivot + nWeight;
		return FindVolume(pDM, nKeyValue, nNextPivot, nPivot+1, nLastValue);
		//return FindVolume(pDM, nKeyValue, nNextPivot, nPivot, nLastValue);
	}
	else if(nKeyValue < nTmpData)
	{
		nWeight = (nPivot-nFirstValue)/2;
		if(nWeight == 0) nWeight = 1;
		INT nNextPivot = nPivot - nWeight;
		return FindVolume(pDM, nKeyValue, nNextPivot, nFirstValue, nPivot-1);
		//return FindVolume(pDM, nKeyValue, nNextPivot, nFirstValue, nPivot);
	}
	else if(nKeyValue == nTmpData)
	{
		//=====================================================
		//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 		FileLog(_T("Found!!"));
		//=====================================================
		
		return nPivot-1;
	}

	return -1;
}

INT CMakePrintDM::FileLog(CString str)
{
	//=====================================================
	//2009.02.20 REM BY PJW : REC_KEY LOG 남기는 부분제거
// 	FILE * f = _tfopen(_T("binary_search.log"), _T("a+b"));
// 	if (!f) return -1;
// 	fseek(f, 0, SEEK_END);		
// 	if( 0 == ftell(f) )
// 	{
// 		/*UNCHANGE*/fputc(0xFF, f);
// 		/*UNCHANGE*/fputc(0xFE, f);
// 	}
// 		
// 	_ftprintf(f, _T("%s\r\n"), str);
// 	fclose(f);
	//=====================================================
	

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그룹별 소계 및 총계를 구한다.
// 
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CMakePrintDM::SetTotalInfo(CESL_DataMgr * pDM, CPrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle, DOUBLE dTotalOrderPrice/*=0*/)
{
	if( pTotalInfo == NULL )	return -1;	

	CString strTmpData, strIs64;
	DOUBLE dTmpData = 0.0;
	// 2006-01-18 강봉수 - 발주가격단위설정값 
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}
	
	pDM->SetCellData( m_strTotalInfoTitle, strTitle, nRowIdx );
	
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
	if( m_strTotalPriceAlias.IsEmpty() == FALSE )
	{
		pTotalInfo->GetTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalPriceAlias, strTmpData, nRowIdx);
	}
	
	if( m_strTotalDiscPriceAlias.IsEmpty() == FALSE )
	{
		// 2006.02.01 강봉수 - 가격정책사항 최종 반영, 출력물에 오차는 삭제하고 DB에 저장된 발주가격의 합으로(종단위합) 총계를 나타낸다.
		pTotalInfo->SetDiscTotalPrice(dTotalOrderPrice);
		pTotalInfo->GetDiscTotalPrice(strTmpData, m_strTruncPrice);
		pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);

		/*
		// 2006-01-23 강봉수 - 가격정책사항반영
		// 총합을 항목의 합으로 입력하고, DB값과 항목의합(인자로 온 값)과의 차이를 오차로 설정한다.
		//pTotalInfo->GetDiscTotalPrice(strTmpData, m_strTruncPrice);
		if		( m_strTruncPrice == _T("0") )	strTmpData.Format(_T("%.f"), dTotalOrderPrice);
		else if	( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dTotalOrderPrice);
		else if ( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dTotalOrderPrice);
		else									strTmpData.Format(_T("%.f"), dTotalOrderPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
		pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);
		
		if( strTitle == _T("총계") )
		{
			// 오차를 산출한다.(DB값 - 항목의 합)
			ids = pDM->InsertRow(-1);
			if( ids < 0 )	return ids;
			dTmpData = pTotalInfo->GetDiscTotalPrice();	// DB에 저장된 참값
			dTmpData = dTotalOrderPrice - dTmpData;
			if( dTmpData > 0 )	strTmpData.Format(_T("+%.2f"), dTmpData);
			else				strTmpData.Format(_T("%.2f"), dTmpData);
			pDM->SetCellData( m_strTotalInfoTitle, _T("오차"), nRowIdx+1 );
			pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx+1);		
	}
		*/
	}
	
	
	return 0;
}

BOOL CMakePrintDM::CheckTotalInfoAlias()
{
	if( m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalSpeciesCntAlias.IsEmpty()	||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty()		)
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - UDF정의 필드를 구축한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CMakePrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM, BOOL bOrder)
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids, i, j, nRow = -1;
	INT nColCnt = pDM->GetRefColumnCount();
	INT nRowCnt = pDM->GetRowCount();
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str, strIs64; 
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	m_MarcConvertMgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	
	TCHAR* pEndPtr = NULL;

	// 2006-01-18 강봉수 - 가격절사단위설정값 
	if( m_strPostNum.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), m_strPostNum);
		if( m_strPostNum.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}

	// 2006-01-18 강봉수 - 발주가격단위설정값 
	if( m_strTruncPrice.IsEmpty() == TRUE )
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
//DEL 		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '구입가격단위설정'"), m_strTruncPrice);
		if( m_strTruncPrice.IsEmpty() == TRUE )		m_strTruncPrice = _T("0");
	}

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
			
			// 2005-02-03
			strTmpData.Empty();	 
			strTmpData = pDM->GetCellData(_T("IBS_매체구분"), i);
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("매체구분"), strTmpData, str);
			pDM->SetCellData(_T("UDF_매체구분설명"), str, i);
			nRow = i;
		}

		for( j=0 ; j<nColCnt ; j++ )
		{
			strTmpData.Empty();
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);

			if( FIELD_ALIAS == _T("UDF_차수정보") )
			{	
				INT nGroupNo;
				strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), i);
				strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				
				if( bOrder == FALSE )
				{
					INT nWorkNo, nSerialNo;
					strWorkNo = pDM->GetCellData(_T("BP_작업번호"), i);
					nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
					strSerialNo = pDM->GetCellData(_T("BP_일련번호"), i);
					nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
					str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
					pDM->SetCellData(FIELD_ALIAS, str, i);
					str.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
					pDM->SetCellData(_T("UDF_출력차수정보"), str, i);
				}
				else
				{	
					INT nOrderSerialNo;
					strOrderSerialNo = pDM->GetCellData(_T("BP_발주일련번호"), i);
					nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
					str.Format(_T("%s-%s-%04d-%05d"), strAcqYear, strWorkCode, nGroupNo, nOrderSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo;
					pDM->SetCellData(FIELD_ALIAS, str, i);
					str.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
					pDM->SetCellData(_T("UDF_출력차수정보"), str, i);
				}
			}
			else if( FIELD_ALIAS == _T("UDF_접수번호정보"))
			{
				INT nGroupNo;
				strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
				strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				str.Format(_T("%s-%04d"), strAcqYear, nGroupNo);
				pDM->SetCellData(_T("UDF_출력접수번호정보"), str, i);
			}
			else if( FIELD_ALIAS == _T("UDF_입력구분설명") )
			{
				strTmpData = pDM->GetCellData(_T("BP_입력구분"), i);
				if     ( strTmpData == _T("0") )	str = _T("화면입력");
				else if( strTmpData == _T("1") )	str = _T("재활용반입");
				else if( strTmpData == _T("2") )	str = _T("희망자료반입");
				else if( strTmpData == _T("3") )	str = _T("파일반입");
				else								str = _T("");

				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if( FIELD_ALIAS == _T("UDF_주제구분설명") )
			{
				strTmpData	= pDM->GetCellData(_T("IBS_분류표구분"), i);
				str			= pDM->GetCellData(_T("BP_주제구분"), i);
				if		( strTmpData == _T("1") )
				{
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
				}
				else if	( strTmpData == _T("2") )
				{
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
				}
				else if	( strTmpData == _T("3") || strTmpData == _T("7") )
				{					
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
				}
				else/*( strTmpData.IsEmpty() == TRUE )*/		
				{
					str = _T("");
				}

				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if( FIELD_ALIAS == _T("UDF_구입처부호설명") )
			{
				strTmpData = pDM->GetCellData(_T("BP_구입처부호"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if( FIELD_ALIAS == _T("UDF_가격") )
			{
				// 종DM 컨버터시 입력됨
				/*
				DOUBLE dPrice;
				// 2005-01-26
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				dPrice = (DOUBLE)(atof(strTmpData.GetBuffer(0)));
				if		( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else									strTmpData.Format(_T("%.0f"), dPrice);				
				pDM->SetCellData(_T("UDF_가격"), strTmpData, i);
				*/
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);

				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{	
						//strTmpData.Replace(_T(","), _T(""));
						strTmpData.Format(_T("%d"), (INT)_ttoi(strTmpData));
				}
				
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환가격"), strTmpData, i);
			}
			else if( FIELD_ALIAS == _T("UDF_변환가격") )
			{
				strTmpData = pDM->GetCellData(_T("BP_가격"), i);
				strTmpData.Replace(_T(","), _T(""));
				//20070512 pjw수정 원화일경우 소수점제외
				/*
				if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{	
						strTmpData.Format(_T("%d"), (INT)_ttoi(str));
				}
				*/
				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{	
						strTmpData.Format(_T("%d"), (INT)_ttoi(strTmpData));
				}

				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			else if( FIELD_ALIAS == _T("BP_가격") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{
					//strTmpData.Replace(_T(","), _T(""));
					strTmpData.Format(_T("%d"), (INT)_ttoi(strTmpData));
				}

 				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환종가격"), strTmpData, i);
			}
			/*
			else if( FIELD_ALIAS == _T("UDF_종가격") )
			{
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("UDF_종가격"), i);
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				//2005-01-17
				if( m_strTruncPrice == _T("1") )		strTmpData.Format(_T("%.1f"), dDisc_Price);
				else if( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dDisc_Price);
				else									strTmpData.Format(_T("%.0f"), dDisc_Price);

				pDM->SetCellData(_T("UDF_할인종가격"), strTmpData, i);				
			}
			*/
			else if( FIELD_ALIAS == _T("UDF_단가") )
			{
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				strTmpData.Replace(_T(","), _T(""));
				DOUBLE dPrice = 0.0;
				//dPrice = atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), i) )
				{
					strTmpData.Format(_T("%d"), (INT)dPrice);
				}
				else
				{
					if		( m_strPostNum == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
					else if	( m_strPostNum == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
					else								strTmpData.Format(_T("%d"), (INT)dPrice);
				}
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}			
			
			// 권 발주가격
			else if( FIELD_ALIAS == _T("UDF_할인가") )
			{
				DOUBLE dPrice;
				strTmpData = pDM->GetCellData(_T("UDF_발주가격_권"), i);
				strTmpData.Replace(_T(","), _T(""));
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{
					strTmpData.Format(_T("%d"), (INT)dPrice);
				}else
				{
					if( dPrice == 0 )	strTmpData = _T("0");
					else
					{
						if		( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
						else if	( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
						else									strTmpData.Format(_T("%.0f"), dPrice);
					}
				}
				/*
				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if		( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
					else if	( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
					else									strTmpData.Format(_T("%.0f"), dPrice);
				}
				*/
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);				
				pDM->SetCellData(_T("UDF_변환할인가격"), str, i);
				
				// 2006-01-18 강봉수 - 가격정책사항반영, 권발주가격을 계산해서 가져온다.(표시형식 및 단위 설정 적용)
				
				DOUBLE  dDisc_Rate, dDisc_Price, dExchangeRate = 0.0; 
				CString strExchangeRate;
				
				strTmpData = pDM->GetCellData(_T("UDF_가격"), i);
				//dPrice = (DOUBLE)(atof(strTmpData.GetBuffer(0)));
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				//dDisc_Rate = (DOUBLE)(atof(strTmpData.GetBuffer(0)));
				dDisc_Rate = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				// 2005-01-26 환율도 계산한다.
				// dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strExchangeRate = pDM->GetCellData(_T("BP_환율"), i);
				//dExchangeRate = (DOUBLE)(atof(strExchangeRate.GetBuffer(0)));
				dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

				dDisc_Price = (dPrice - ((dPrice*dDisc_Rate)/100)) * dExchangeRate;
				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{
					strTmpData.Format(_T("%d"), (INT)dDisc_Price);
				}else
				{
					if	   ( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dDisc_Price);
					else if( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dDisc_Price);
					else									strTmpData.Format(_T("%.0f"), dDisc_Price);
				}
				//2005-01-17
				/*
				if	   ( m_strTruncPrice == _T("1") )	strTmpData.Format(_T("%.1f"), dDisc_Price);
				else if( m_strTruncPrice == _T("2") )	strTmpData.Format(_T("%.2f"), dDisc_Price);
				else									strTmpData.Format(_T("%.0f"), dDisc_Price);
				*/
				pDM->SetCellData(_T("UDF_할인가"), strTmpData, i);

				// 2005-02-03
				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{
					strTmpData.Format(_T("%d"), (INT)dDisc_Price);
				}else
				{
					strTmpData.Format(_T("%.0f"), dDisc_Price);
				}
				//strTmpData.Format(_T("%.0f"), dDisc_Price);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인가격"), strTmpData, i);
				
			}
			else if( FIELD_ALIAS == _T("BP_발주가격") )
			{
				DOUBLE dPrice;
				strTmpData = pDM->GetCellData(_T("BP_발주가격"), i);
				//dPrice = (DOUBLE)atof(strTmpData);
				dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

				//20070512 pjw수정 원화일경우 소수점제외
				if( _T("\\") ==  pDM->GetCellData(_T("BP_화폐구분"), nRow) )
				{
					strTmpData.Format(_T("%d"), (INT)dPrice);
				}else
				{
					if( dPrice == 0 )	strTmpData = _T("0");
					else
					{
						if		( m_strTruncPrice == _T("1") )		strTmpData.Format(_T("%.1f"), dPrice);
						else if ( m_strTruncPrice == _T("2") )		strTmpData.Format(_T("%.2f"), dPrice);
						else										strTmpData.Format(_T("%.0f"), dPrice);
					}
				}
				/*
				if( dPrice == 0 )	strTmpData = _T("0");
				else
				{
					if		( m_strTruncPrice == _T("1") )		strTmpData.Format(_T("%.1f"), dPrice);
					else if ( m_strTruncPrice == _T("2") )		strTmpData.Format(_T("%.2f"), dPrice);
					else										strTmpData.Format(_T("%.0f"), dPrice);
				}*/
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);				
				pDM->SetCellData(_T("UDF_변환할인종가격"), strTmpData, i);

				// 2006-01-18 강봉수 - 가격정책사항반영, 권발주가격을 계산해서 가져온다.(표시형식 및 단위 설정 적용)
				/*
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)(atof(strTmpData.GetBuffer(0)));
				strTmpData = pDM->GetCellData(_T("BP_발주가격"), i);
				dPrice = (DOUBLE)(atof(strTmpData.GetBuffer(0)));
				// 2005-01-31
				dDisc_Price = dPrice ;//- (dPrice * (dDisc_Rate/100));

				// 육사에서는 십원단위로 반올림한다.
				m_pParentMgr->GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), strIs64);
				if( _T("Y") == strIs64 )
				{
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
				pDM->SetCellData(_T("UDF_변환할인종가격"), strTmpData, i);
				*/
			}
			// 2005-03-04 신청자 추가.
			else if( FIELD_ALIAS == _T("UDF_신청자") )
			{
				CString strAcqKey, strQuery;
				strAcqKey.Empty();
				strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), i);
				if( strAcqKey.IsEmpty() == TRUE )	 continue;
				strQuery.Format(_T("SELECT APPLICANT_NAME FROM BO_FURNISH_TBL WHERE ACQ_KEY = %s"), strAcqKey);
				pDM->GetOneValueQuery(strQuery, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 도서분류코드1(사용자정의코드1) 값을 설명으로 변경하여 DM에 저장 (64)
			// 2005-09-05 강봉수
			//
			else if(FIELD_ALIAS == _T("UDF_사용자정의코드1"))
			{
				strTmpData = pDM->GetCellData(_T("BS_사용자정의코드1"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("도서분류코드1"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 이상 끝.

			// 학처별 코드값을 설명으로 변경하여 DM에 저장 (64)
			// 2005-09-26 강봉수
			//
			else if(FIELD_ALIAS == _T("UDF_학처명"))
			{
				strTmpData = pDM->GetCellData(_T("BP_학처명"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학부"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
			// 이상 끝.
			// 부서별 코드값을 설명으로 변경하여 DM에 저장 (국방대)
			// 2005-11-10 강봉수
			//
			else if(FIELD_ALIAS == _T("UDF_부서명"))
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


VOID CMakePrintDM::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) return;
	
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->SetMessage(_T(""));
}

VOID CMakePrintDM::HideDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
	}
}

INT CMakePrintDM::CopySortItem(CStringArray *pSourceArr, CStringArray *pTargetArr)
{
	if(pSourceArr == NULL || pTargetArr == NULL) return -1;
	if(pSourceArr->GetSize() == 0) return 0;
	
	pTargetArr->RemoveAll();
	for(INT i=0 ; i<pSourceArr->GetSize() ; i++)
	{
		pTargetArr->Add(pSourceArr->GetAt(i));
	}
	return 0;
}

INT CMakePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr * pDM)
{
	return 0;
}

INT CMakePrintDM::PrintDataSortByUserSelect(CESL_DataMgr * pDM)
{
	return 0;
}

INT CMakePrintDM::SetDataSerialNo(CESL_DataMgr *pDM)
{
	INT i, nSerialNo = 0;
	CString strTmpData;
	
	if( pDM == NULL )				return -1;
	if( pDM->GetRowCount() == 0 )	return 0;	
	
	for( i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		strTmpData.Format(_T("%d"), ++nSerialNo);
		pDM->SetCellData(_T("UDF_일련번호"), strTmpData, i);
	}

	return 0;
}

INT CMakePrintDM::CopySortConditionMap(CMapStringToString *pSource, CMapStringToString *pTarget)
{
	if(pSource == NULL || pTarget == NULL) return -1;
	pTarget->RemoveAll();
	CString strKey, strValue;
	POSITION pos = pSource->GetStartPosition();
	while( pos != NULL )
	{
		CString string;
		pSource->GetNextAssoc(pos, strKey, strValue);
		pTarget->SetAt(strKey, strValue);
	}
	return 0;	
}

BOOL CMakePrintDM::CheckPrintData(CString strValue, CString strExtraValue/*=_T("")*/)
{
	CString strTmpData;
	if(m_pArrDivValeu && m_pArrDivValeu->GetCount() > 0)
	{
		if(strExtraValue.IsEmpty())
		{
			if(!m_pArrDivValeu->Lookup(strValue, strTmpData))	return FALSE;
		}
		else
		{
			if(!m_pArrDivValeu->Lookup(strValue+_T("-")+strExtraValue, strTmpData))		return FALSE;
		}
	}
	return TRUE;
}

INT CMakePrintDM::ApplyGridSelectData(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	if(m_strDivAlias.IsEmpty()) return 0;

	if(m_pArrDivValeu == NULL || m_pArrDivValeu->GetCount() == 0) return 0;

	CString strTmpData;
	for(INT i=pDM->GetRowCount()-1 ; i>=0 ; i--)
	{
		if(!CheckPrintData(pDM->GetCellData(m_strDivAlias, i)))
			pDM->DeleteRow(i);
	}
	return 0;
}

INT CMakePrintDM::SetConvertTotalInfo(CESL_DataMgr * pDM, CPrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle)
{
	return 0;
}