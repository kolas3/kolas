// MakePrintDM.cpp: implementation of the CMakePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakePrintDM.h"

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

INT CMakePrintDM::MakeVolumeDM(CESL_DataMgr * pDM, CESL_DataMgr * pBookDM, CString strDivAlias, BOOL bPrintVolume/*=TRUE*/)
{
	if(pBookDM == NULL) return -1;
	if(pBookDM->GetRowCount() == 0) return 0;
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 권정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}
	pDM->FreeData();
	CBO_ACQ_Business_Api api(m_pParentMgr);
	api.InitTempData();	api.InitTempPtr();
	api.SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	api.SetTempPtr(_T("권DM"), (DWORD*)pDM);
	api.SetTempData(_T("구분키Alias"), strDivAlias.GetBuffer(0));
	if(m_pDataProcessingDlg)
		api.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	else
		api.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	INT ids;
	if(bPrintVolume)
		ids = api.SPFExecutePath(_T("책To출력용권"));
	else
		ids = api.SPFExecutePath(_T("책To권"));

	return ids;
}

INT CMakePrintDM::MakeSpeciesDM(CESL_DataMgr * pDM, CESL_DataMgr * pBookDM, BOOL bIndex)
{
	if(pBookDM == NULL) return -1;
	if(pBookDM->GetRowCount() == 0) return 0;
	
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력용 종정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	}
	pDM->FreeData();
	CBO_ACQ_Business_Api api(m_pParentMgr);
	api.InitTempData();	api.InitTempPtr();
	api.SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	api.SetTempPtr(_T("종DM"), (DWORD*)pDM);
	if(m_pDataProcessingDlg)
		api.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	else
		api.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	INT ids;
	if(bIndex)
		ids = api.SPFExecutePath(_T("책To종"));
	else
		ids = api.SPFExecutePath(_T("책To수서정보"));
	return ids;
}
INT CMakePrintDM::MakePrintDM()
{
/*
	INT ids;
	if(m_pPrintBookDM == NULL) return -1;
	if(m_pPrintBookDM->GetRowCount() == 1000) return 0;

	ids = CheckValid();
	if(ids < 0) return ids;

	if(	m_nPrintType == BO_ACQ_BASIC_DATA || m_nPrintType == BO_ACQ_IMPORT_DATA || 
		m_nPrintType == BO_ACQ_DONATE_DATA || m_nPrintType == BO_ACQ_NONDONATE_DATA ||
		m_nPrintType == BO_ACQ_DONATE_REGOBJ_DATA || m_nPrintType == BO_ACQ_PURCHASE_DATA ||
		m_nPrintType == BO_ACQ_RECYCLE_DATA)
		ids = MakeSpeciesVolumePrintDM(m_pPrintBookDM);
	//else if()
*/
	return 0;
}

INT CMakePrintDM::CheckValid()
{
/*
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintSourceDM == NULL) return -1;
	if(m_pPrintBookDM == NULL) return -1;
	if(m_pPrintObjDM == NULL) return -1;
	if(m_strDivAlias.IsEmpty()) return -1;
*/	
	return 0;
}

VOID CMakePrintDM::SetTotalInfoAlias(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_cTotalInfo.SetAliasInfo(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
}

INT CMakePrintDM::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	
	INT ids = -1;
	
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
	
	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strData, strTmpData;
	
	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	INT startpos = -1;
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;
		
		strAlias = pRef->FIELD_ALIAS;
		
		// 거부 리스트에 있는지 확인
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
			
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

INT CMakePrintDM::InsertVolumeInfo(CESL_DataMgr *pDM, CESL_DataMgr *pDM_OBJ, INT nCurIdx, CString strKey)
{
	CArray<INT, INT> IdxArray;	IdxArray.RemoveAll();
	
	CString strTMpData;
	strTMpData.Format(_T("Key Value : %s"), strKey);
	FileLog(strTMpData);
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
	LONG nKeyValue = (LONG)_ttol(strKey.GetBuffer(0));
	INT nPivot = pDM->GetRowCount()/2;
	if(nPivot == 0) nPivot = 1;
	
	INT nIdx = FindVolume(pDM, nKeyValue, nPivot, 1, pDM->GetRowCount());

	if(nIdx < 0) return -1;
	else GetMultiVolumeIdx(pDM, nIdx, pIdxArray);
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
	CString strKey = pDM->GetCellData(m_strDivAlias, nIdx);
	CString strTmpData ;
	for(INT i=nIdx-1 ; i>=0 ; i--)
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if(strTmpData == strKey) AddIdxByOrdered(pIdxArray, i);
		else break;
	}
	for(i=nIdx+1 ; i<pDM->GetRowCount() ; i++)
	{
		strTmpData = pDM->GetCellData(m_strDivAlias, i);
		if(strTmpData == strKey) AddIdxByOrdered(pIdxArray, i);
		else break;
	}
	AddIdxByOrdered(pIdxArray, nIdx);
	
	return 0;
}

INT CMakePrintDM::FindVolume(CESL_DataMgr *pDM, LONG nKeyValue, INT nPivot, LONG nFirstValue, LONG nLastValue)
{	
	CString strTmpData;
	strTmpData.Format(_T("초기값:\t%d\t\tPivot:\t%d\t\t마지막값:\t%d"), nFirstValue, nPivot, nLastValue);
	FileLog(strTmpData);
	if(nFirstValue > nLastValue) 
	{
		FileLog(_T("Not Found!!"));
		return -1;
	}
/*
	if(nFirstValue == nLastValue)
	{
		FileLog(_T()
		return nFirstValue;
	}
*/

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
		FileLog(_T("Found!!"));
		return nPivot-1;
	}

	return -1;
}

INT CMakePrintDM::FileLog(CString str)
{
/*
	FILE * f = _tfopen(_T("binary_search.log"), _T("a+t"));
	if (!f) return -1;
	_ftprintf(f, _T("%s\n"), str);
	fclose(f);
	*/
	return 0;
}

INT CMakePrintDM::SetTotalInfo(CESL_DataMgr * pDM, CPrintTotalInfo * pTotalInfo, INT nRowIdx, CString strTitle)
{
	if(pTotalInfo == NULL) return -1;
	
	CString strTmpData;
	pDM->SetCellData(m_strTotalInfoTitle, strTitle, nRowIdx);
	
	if(!m_strTotalBookCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalBookCnt(strTmpData);
		pDM->SetCellData(m_strTotalBookCntAlias, strTmpData+_T("책"), nRowIdx);
	}
	if(!m_strTotalPriceAlias.IsEmpty())
	{
		pTotalInfo->GetTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalPriceAlias, strTmpData, nRowIdx);
	}
	if(!m_strTotalDiscPriceAlias.IsEmpty())
	{
		pTotalInfo->GetDiscTotalPrice(strTmpData);
		pDM->SetCellData(m_strTotalDiscPriceAlias, strTmpData, nRowIdx);
	}
	if(!m_strTotalSpeciesCntAlias.IsEmpty())
	{
		pTotalInfo->GetTotalSpeciesCnt(strTmpData);
		pDM->SetCellData(m_strTotalSpeciesCntAlias, strTmpData+_T("종"), nRowIdx);
	}
	
	return 0;
}

BOOL CMakePrintDM::CheckTotalInfoAlias()
{
	if(m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalSpeciesCntAlias.IsEmpty()	||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty())
		return FALSE;
	return TRUE;
}

INT CMakePrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM, BOOL bOrder)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str; 
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nColCnt = pDM->GetRefColumnCount();
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		for(INT j=0 ; j<nColCnt ; j++)
		{
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(FIELD_ALIAS == _T("UDF_차수정보") && !bOrder)
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
			}
			else if(FIELD_ALIAS == _T("UDF_차수정보") && bOrder)
			{	
				INT nGroupNo, nOrderSerialNo;
				strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), i);
				strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				strOrderSerialNo = pDM->GetCellData(_T("BP_발주일련번호"), i);
				nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
				str.Format(_T("%s-%s-%04d-%05d"), strAcqYear, strWorkCode, nGroupNo, nOrderSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo;
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_입력구분설명"))
			{
				strTmpData = pDM->GetCellData(_T("BP_입력구분"), i);
				if(strTmpData == _T("0")) str = _T("화면입력");
				else if(strTmpData == _T("1")) str = _T("재활용반입");
				else if(strTmpData == _T("2")) str = _T("희망자료반입");
				else if(strTmpData == _T("3")) str = _T("파일반입");
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_주제구분설명"))
			{
				strTmpData = pDM->GetCellData(_T("IBS_분류표구분"), i);
				if(strTmpData.IsEmpty()) str = _T("");
				else if(strTmpData == _T("1"))
				{
					strTmpData = pDM->GetCellData(_T("BP_주제구분"), i);
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), strTmpData, str);
				}
				else if(strTmpData == _T("2"))
				{
					strTmpData = pDM->GetCellData(_T("BP_주제구분"), i);
					m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), strTmpData, str);
				}
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_구입처부호설명"))
			{
				strTmpData = pDM->GetCellData(_T("BP_구입처부호"), i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_할인가"))
			{
				CString strTmpData = pDM->GetCellData(_T("UDF_가격"), i);
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData.Replace(_T(","), _T(""));
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
/*
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BB_가격"), i);
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("UDF_책수"), i);
				nBookCnt = _ttoi(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price*nBookCnt);
*/
				pDM->SetCellData(_T("UDF_할인가"), strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("UDF_가격"))
			{
				CString strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
/*
				INT nBookCnt = 0;
				DOUBLE dPrice;
				strTmpData = pDM->GetCellData(_T("BB_가격"), i);
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("UDF_책수"), i);
				nBookCnt = _ttoi(strTmpData.GetBuffer(0));
				strTmpData.Format(_T("%.0f"), dPrice*nBookCnt);
				pDM->SetCellData(_T("UDF_가격"), strTmpData, i);
*/
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BP_발주가격"))
			{
				CString strTmpData;
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_발주가격"), i);				
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인종가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BP_가격"))
			{
				CString strTmpData;
				strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환종가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("UDF_종가격"))
			{
				CString strTmpData;
				INT nBookCnt = 0;
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("UDF_종가격"), i);
				strTmpData.Replace(_T(","), _T(""));
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				pDM->SetCellData(_T("UDF_할인종가격"), strTmpData, i);				
			}
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
