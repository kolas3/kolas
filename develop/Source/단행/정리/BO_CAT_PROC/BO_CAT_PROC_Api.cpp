// BO_CAT_PROC_Api.cpp: implementation of the CBO_CAT_PROC_Api class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_CAT_PROC_Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_CAT_PROC_Api::CBO_CAT_PROC_Api(CESL_Mgr *pParentMgr  /*= NULL*/)
{
	m_pParnetMgr = pParentMgr;
	m_nAppMode = MODE_INSERT;

	if (m_pParnetMgr != NULL)
	{
		LoadRegCode();
	}
}

CBO_CAT_PROC_Api::~CBO_CAT_PROC_Api()
{

}

INT CBO_CAT_PROC_Api::LoadRegCode()
{
EFS_BEGIN

	if (m_pParnetMgr == NULL) return -1;

	INT nCodeCount = m_pParnetMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	INT nStartRow = 0;
	CString strArrCode[6];
	for (INT i = 0; i < nCodeCount; i++) {
		nStartRow = m_pParnetMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), strArrCode, 6, nStartRow);

		m_arrRegCode.Add(strArrCode[2]);
		
		nStartRow++;
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::FetchRegCode(CString strRegCodeField, CArray<CRegCodeProc, CRegCodeProc> *pArrRegCode)
{
EFS_BEGIN

	if(pArrRegCode == NULL) return -1;
	if (strRegCodeField.IsEmpty()) return -1;

	strRegCodeField.TrimLeft();
	strRegCodeField.TrimRight();

	CMap<CString, LPCTSTR, INT, INT> mapRegCode;
	CString strRegCode;
	CString strCount;
	CString strData;
	INT nCount = 0;
	INT nTempCount = 0;

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	while (TRUE)
	{
		INT nFind = strRegCodeField.Find('/');
		if (nFind < 0)
		{
			nFind = strRegCodeField.GetLength() - 1;
			strData  = strRegCodeField;
		}
		else
		{		
			strData = strRegCodeField.Mid(0, nFind);
		}
		
		if (strData.GetLength() < nRegCodeLength) return -1;
		strRegCode = strData.Left(nRegCodeLength);

		if (!IsValidRegCode(strRegCode)) return -1;

		if (strData.GetLength() == nRegCodeLength)
			strCount = _T("1");
		else
			strCount = strData.Mid(nRegCodeLength);
		
		INT nLength = strCount.GetLength();
		for (INT i = 0; i < nLength; i++)
		{
			if (!_istdigit(strCount.GetAt(i))) return -1;
		}

		nCount = _ttoi(strCount);
		if (nCount <= 0) return -1;

		if (mapRegCode.Lookup(strRegCode, nTempCount))
		{
			mapRegCode.SetAt(strRegCode, nTempCount + nCount);
		}
		else
		{
			mapRegCode.SetAt(strRegCode, nCount);
		}
		
		strRegCodeField = strRegCodeField.Mid(nFind + 1);
		if (strRegCodeField.IsEmpty()) break;
	}
	
	CRegCodeProc regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);

		pArrRegCode->Add(regCode);
	}

	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_Api::IsValidRegCode(CString strRegCode)
{
EFS_BEGIN

	if (m_pParnetMgr == NULL) return FALSE;
	if (strRegCode.IsEmpty()) return FALSE;

	return IsStringInArray(&m_arrRegCode, strRegCode);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_Api::IsRegCodeModify(CArray<CRegCodeProc, CRegCodeProc> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
EFS_BEGIN

	if (pArrRegCode == NULL || pDM_Book == NULL) return FALSE;

	INT ids = -1;
	CArray<CRegCodeProc, CRegCodeProc> arrExistRegCode;
	ids = GetRegCodeInfo(pDM_Book, &arrExistRegCode, strVol);

	INT idx = 0;
	INT nBookCount = 0;
	INT nRegCodeCount = 0;
	CRegCodeProc regCode;

	INT nCount = arrExistRegCode.GetSize();
	for (idx = 0 ; idx < nCount; idx++)
	{
		regCode = arrExistRegCode.GetAt(idx);

		if (!IsInRegCode(pArrRegCode, regCode.m_strRegCode)) return TRUE;
	}

	nCount = pArrRegCode->GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		
		if (IsInRegCode(&arrExistRegCode, regCode.m_strRegCode))
		{
			nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);
			if (regCode.m_nCount < nBookCount)
				return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_Api::MakeBookFromRegCode(CArray<CRegCodeProc, CRegCodeProc> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
EFS_BEGIN

	if (pArrRegCode == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	INT nDMRowCount = pDM_Book->GetRowCount();
	if (nDMRowCount == 0) return -1;

	INT nRegCodeCount = pArrRegCode->GetSize();
	if (nRegCodeCount == 0) return -1;

	CString strRegCode;
	CString strQueryFlag;

	CRegCodeProc regCode;
	INT nBookCount;
	for (INT idx = 0; idx < nRegCodeCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);

		if (regCode.m_nCount < nBookCount)
		{
			RemoveBook(pDM_Book, regCode.m_strRegCode, strVol, nBookCount - regCode.m_nCount);
		}
		else if (regCode.m_nCount > nBookCount)
		{
			AddBook(pDM_Book, regCode.m_strRegCode, strVol, regCode.m_nCount - nBookCount);
		}
	}

	nDMRowCount = pDM_Book->GetRowCount();
	for (idx = nDMRowCount - 1; idx >= 0; idx--)
	{
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		if (pDM_Book->GetCellData(_T("BB_복본기호"), idx) == _T("c1"))
			pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
	}

	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_Api::IsInRegCode(CArray<CRegCodeProc, CRegCodeProc> *pArrRegCode, CString strRegCode)
{
EFS_BEGIN

	if (pArrRegCode == NULL || strRegCode.IsEmpty()) return FALSE;

	INT nCount = pArrRegCode->GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		if (pArrRegCode->GetAt(i).m_strRegCode == strRegCode)
			return TRUE;
	}

	return FALSE;

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_Api::DeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
EFS_BEGIN

	if (pDM_Book == NULL || strVol.IsEmpty()) return -1;
	
	INT nRowCount = pDM_Book->GetRowCount();

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) == strVol)
		{
			DeleteBookDMRow(pDM_Book, idx);
		}
	}

	return 0;

EFS_END
return -1;

}

CString CBO_CAT_PROC_Api::GetBookKey(CESL_DataMgr *pDM_Book, INT nIdx)
{
EFS_BEGIN

	if (pDM_Book == NULL) return _T("");

	CString strKey = pDM_Book->GetCellData(_T("BB_책KEY"), nIdx);
	if (strKey.IsEmpty())
		strKey = pDM_Book->GetCellData(_T("BB_UNIQUE_KEY"), nIdx);

	return strKey;

EFS_END
return _T("");

}

INT CBO_CAT_PROC_Api::DeleteBookDMRow(CESL_DataMgr *pDM_Book, INT idx, CString strQueryFlag /*=_T("")*/)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strQueryFlag.IsEmpty())
	{
		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
	}

	CString strKey = GetBookKey(pDM_Book, idx);
	
	if (strQueryFlag == _T("D")) 
	{
		
	}
	else if (strQueryFlag == _T("I"))
	{
		pDM_Book->DeleteRow(idx);
	}
	else if (strQueryFlag == _T("U"))
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	else
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}

	if (strKey.IsEmpty()) return 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT i = nRowCount - 1; i >= 0; i--)
	{
		if (!IsAppendix(pDM_Book, i)) continue;

		if (pDM_Book->GetCellData(_T("BB_부모책KEY"), i) != strKey) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i);

		if (strQueryFlag == _T("D"))
		{
		}
		else if (strQueryFlag == _T("I"))
		{
			pDM_Book->DeleteRow(i);
		}
		else if (strQueryFlag == _T("U"))
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
		else
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetVolumeCount(CESL_DataMgr *pDM_Book)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_편권차"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, lpszVol)) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetRegCodeInfo(CESL_DataMgr *pDM_Book, CArray<CRegCodeProc, CRegCodeProc> *pArrRegCode, LPCTSTR lpszVol /*= NULL*/)
{
EFS_BEGIN

	if (pDM_Book == NULL || pArrRegCode == NULL) return -1;

	CMap<CString, LPCTSTR, INT, INT> mapRegCode;

	INT ids = -1;
	CString strRegCode;
	INT nRegCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, lpszVol)) continue;

		ids = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx, strRegCode);
		if (ids < 0) continue;

		if (strRegCode.IsEmpty()) continue;
		if (!IsValidRegCode(strRegCode)) continue;

		if (mapRegCode.Lookup(strRegCode, nRegCount))
			mapRegCode.SetAt(strRegCode, ++nRegCount);
		else
			mapRegCode.SetAt(strRegCode, 1);
	}

	CRegCodeProc regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);
		pArrRegCode->Add(regCode);
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::AddBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nStandardRowIdx = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		nStandardRowIdx = idx;
		break;
	}
	
	CString strQueryFlag;
	CString strDupCode;
	INT nNewRow = -1;
	for (idx = 0; idx < nCount; idx++)
	{
		nNewRow = CopyDM(pDM_Book, nStandardRowIdx, pDM_Book, -1);
		if (nNewRow < 0) continue;

		pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, nNewRow);

		pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), nNewRow);

		pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), nNewRow);

		strQueryFlag = GetQueryFlag(MODE_INSERT);
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nNewRow);
		
		INT nDupNumber = GetLastDupCodeNumber(pDM_Book, strVol);
		if (nDupNumber < 0) continue;
		strDupCode.Format(_T("c%d"), ++nDupNumber);
		pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, nNewRow);		
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::RemoveBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	CString strQueryFlag;
	INT nDeletedCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		if (pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx) != strRegCode) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		
		if (DeleteBookDMRow(pDM_Book, idx, strQueryFlag) < 0) continue;

		if (nCount == ++nDeletedCount) break;
	}

	return nDeletedCount;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
EFS_BEGIN

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
	CString strData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
		if (ids < 0) continue;

		if (bSwitchHeader)
		{
			strAlias = strAlias.Mid(strAlias.Find(_T("_")));
			strAlias = strToAliasHeader + strAlias;
		}

		ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::MakeDMFromCM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, INT nIdx /*= -1*/, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
EFS_BEGIN

	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	BOOL bSwitchHeader = FALSE;
	if (!strToAliasHeader.IsEmpty())
	{
		bSwitchHeader = TRUE;
	}
	
	CString strAlias;
	CString strData;
	CESL_Control_Element *pCE = NULL;
	
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;

		ids = pCM->GetControlMgrData(strAlias, strData);
		if (ids < 0) continue;

		if (bSwitchHeader)
		{
			strAlias = strAlias.Mid(strAlias.Find(_T("_")));
			strAlias = strToAliasHeader + strAlias;
		}
				
		ids = pDM->SetCellData(strAlias, strData, nIdx);
		if (ids < 0) continue;
	}
	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_Api::IsStringInArray(CStringArray *pArr, CString str)
{
EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

EFS_END
return FALSE;

}

CString CBO_CAT_PROC_Api::GetQueryFlag(INT nWorkMode, CString strQueryFlag /*= _T("")*/)
{
EFS_BEGIN

	if (strQueryFlag.IsEmpty())
	{
		if (nWorkMode == MODE_INSERT)
		{
			return _T("I");
		}
		else if (nWorkMode == MODE_UPDATE)
		{
			if (m_nAppMode == MODE_INSERT)
				return _T("I");
			else if (m_nAppMode == MODE_UPDATE)
				return _T("U");
		}
	}
	else
	{
		if (strQueryFlag == _T("I"))
			return _T("I");
		else if (strQueryFlag == _T("U"))
			return _T("U");
	}
	
	return _T("");

EFS_END
return _T("");

}

INT CBO_CAT_PROC_Api::MakeAppendixDM(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Appendix, CString strVol /*= _T("")*/, CString strParentKey /*=_T("")*/)
{
EFS_BEGIN

	if (pDM_Book == NULL || pDM_Appendix == NULL) return -1;
	
	pDM_Appendix->FreeData();

	CString strIdx;
	INT nAppendixRowCount = -1;
	INT nColCount = pDM_Book->RefList.GetCount();
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsAppendix(pDM_Book, idx, strVol)) continue;
		
		if (!strParentKey.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_부모책KEY"), idx) != strParentKey) continue;
		
		if (pDM_Appendix->InsertRow(-1) < 0) continue;
		
		nAppendixRowCount = pDM_Appendix->GetRowCount();
		CopyDM(pDM_Book, idx, pDM_Appendix, nAppendixRowCount - 1);

		strIdx.Format(_T("%d"), idx);
		pDM_Appendix->SetCellData(_T("BB_BOOK_IDX"), strIdx, nAppendixRowCount - 1);
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetBookPriceSum(CESL_DataMgr *pDM_Book, INT &nSum)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	
	nSum = 0;
	INT ids = -1;
	INT nResult = 0;
	CString strTemp;


	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0 ; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;
		
		ids = pDM_Book->GetCellData(_T("BB_가격"), idx, strTemp);
		if (ids < 0 || strTemp.IsEmpty()) 
		{
			nResult ++;
			continue;
		}

		nSum += _ttoi(strTemp);		
	}

	return nResult;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::ApplySpeciesPriceToBookPrice(CESL_DataMgr *pDM_Book, INT nPrice)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nBookCount = GetBookCount(pDM_Book);
	if (nBookCount <= 0) return -1;

	if (nPrice < 0) return -1;

	INT nBookPrice = 0;
	INT nLastBookPrice = 0;

	if (nPrice >= nBookCount) 
	{
		nBookPrice = nPrice / nBookCount;
		nLastBookPrice = nPrice - (nBookPrice * nBookCount);
		if (nLastBookPrice == 0)
			nLastBookPrice = nBookPrice;
		else
			nLastBookPrice = nLastBookPrice + nBookPrice;
	}

	CString strBookPrice;
	CString strLastBookPrice;

	strBookPrice.Format(_T("%d"), nBookPrice);
	strLastBookPrice.Format(_T("%d"), nLastBookPrice);

	if (nLastBookPrice < 0) return -1;

	INT n = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		n++;
		
		if (n == nBookCount)
			pDM_Book->SetCellData(_T("BB_가격"), strLastBookPrice, idx);
		else
			pDM_Book->SetCellData(_T("BB_가격"), strBookPrice, idx);
	}
	

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::ApplyCompleteWorks(CESL_DataMgr *pDM_Book, CString &strVol)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	INT n = 1;
	CString strTemp;

	strTemp.Format(_T("%d"), n++);
	strVol = strTemp;
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		pDM_Book->SetCellData(_T("BB_편권차"), strTemp, idx);
		strTemp.Format(_T("%d"), n++);

		pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);
	}

	return 0;

EFS_END
return -1;

}

CString CBO_CAT_PROC_Api::ConvertWORKING_STATUSToCode(CString strDesc)
{
EFS_BEGIN

	CString result = _T("");
	
	if (strDesc == _T("구입대상자료")) result = _T("BOA111N");
	if (strDesc == _T("검수대상자료")) result = _T("BOA113N");
	if (strDesc == _T("기증대상자료")) result = _T("BOA131N");
	if (strDesc == _T("편목대상자료")) result = _T("BOC113N");
    if (strDesc == _T("검수완료자료")) result = _T("BOA141N");

	return result;

EFS_END
return _T("");

}

VOID CBO_CAT_PROC_Api::InitDefaultAlias()
{
EFS_BEGIN

	m_arrDefualtValueAlias.RemoveAll();

EFS_END
}

INT CBO_CAT_PROC_Api::AddDefaultAlias(LPCTSTR lpszAlias)
{
EFS_BEGIN

	return m_arrDefualtValueAlias.Add(lpszAlias);

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::SaveDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
EFS_BEGIN

	if (pCM == NULL) return -1;
	
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;

	if( 0 == file.GetLength() )
	{			
	TCHAR cUni = 0xFEFF;
	file.Write( &cUni, sizeof(TCHAR));
	}

	INT ids = -1;
	CString strAlias;
	CString strValue;
	CString strLine;
	INT nRows = m_arrDefualtValueAlias.GetSize();
	for (INT idx = 0; idx < nRows; idx++)
	{
		strLine.Empty();
		strAlias = m_arrDefualtValueAlias.GetAt(idx);
		ids = pCM->GetControlMgrData(strAlias, strValue);
		if (ids < 0) continue;
		
		strLine.Format(_T("%s\t=\t%s\r\n"), strAlias, strValue);

		file.WriteString(strLine);
	}

	file.Close();
	
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::LoadDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
EFS_BEGIN

	if (pCM == NULL) return -1;

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return -1;
		
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

	INT ids = -1;

	CString strLine;
	CString strAlias;
	CString strValue;

	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		strAlias.Empty();
		strValue.Empty();

		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;
		
		ids = _stscanf(strLine, _T("%s\t=\t%[^\n]"), strAlias.GetBuffer(256), strValue.GetBuffer(256));
		
		if (ids == _TEOF) continue;

		strAlias.ReleaseBuffer();
		strValue.ReleaseBuffer();

		ids = pCM->SetControlMgrData(strAlias, strValue);
	}

	file.Close();

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CString strVol)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	INT nDupNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		CString strTemp;
		INT nNumber = 0;
		TCHAR ch = ' ';

		if (!IsBook(pDM_Book, idx, strVol)) continue;

		strTemp = pDM_Book->GetCellData(_T("BB_복본기호"), idx);
		if (strTemp.IsEmpty()) continue;

		_stscanf(strTemp, _T("%c%d"), &ch, &nNumber);
				
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::AutoApplyDupCode(CESL_DataMgr *pDM_Book)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();

	pDM_Book->SORT(_T("BB_편권차"));

	CString strVolMaster;
	CString strVolSlave;
	CString strDupCode;
	INT nDupNo = -1;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		nDupNo = 1;
		strVolMaster = pDM_Book->GetCellData(_T("BB_편권차"), idx);
		while (++idx < nRowCount)
		{
			if (!IsBook(pDM_Book, idx)) continue;

			strVolSlave = pDM_Book->GetCellData(_T("BB_편권차"), idx);
			if (strVolMaster != strVolSlave) 
			{
				idx--;
				break;
			}

			strDupCode.Format(_T("c%d"), ++nDupNo);
			pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, idx);

			CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}


	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::AutoApplyAppendixRegNo(CESL_DataMgr *pDM_Book)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();

	pDM_Book->SORT(_T("BB_부모책KEY"));

	CString strRegCode;
	CString strParentBookKey;
	CString strParentBookRegNoMaster;
	CString strParentBookRegNoSlave;
	CString strRegNo;
	
	INT		ids = -1;
	INT		nRegNo = -1;

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsAppendix(pDM_Book, idx)) continue;

		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (strRegCode.IsEmpty()) continue;

		strParentBookKey = pDM_Book->GetCellData(_T("BB_부모책KEY"), idx);
		if (strParentBookKey.IsEmpty()) continue;

		ids = GetParentBookIndex(pDM_Book, strParentBookKey);
		if (ids < 0) continue;

		strParentBookRegNoMaster = pDM_Book->GetCellData(_T("BB_등록번호"), ids);
			
		if (strParentBookRegNoMaster.GetLength() <= nRegCodeLength) continue;
		nRegNo = 0;

		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
	
		pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

		CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
		while (++idx < nRowCount)
		{
			if (!IsAppendix(pDM_Book, idx)) continue;

			strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
			if (strRegCode.IsEmpty()) continue;

			strParentBookKey = pDM_Book->GetCellData(_T("BB_부모책KEY"), idx);
			if (strParentBookKey.IsEmpty()) continue;

			ids = GetParentBookIndex(pDM_Book, strParentBookKey);
			if (ids < 0) continue;

			strParentBookRegNoSlave = pDM_Book->GetCellData(_T("BB_등록번호"), ids);
			
			if (strParentBookRegNoSlave.GetLength() <= nRegCodeLength) continue;
			if (strParentBookRegNoMaster != strParentBookRegNoSlave) 
			{
				idx--;
				break;
			}

			strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);

			pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

			CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::GetParentBookIndex(CESL_DataMgr *pDM_Book, CString strParentBookKey)
{
EFS_BEGIN

	if (pDM_Book == NULL || strParentBookKey.IsEmpty()) return -1;


	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx <nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		if (GetBookKey(pDM_Book, idx) == strParentBookKey)
			return idx;
	}

	return -1;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_Api::InitDefaultIgnoreAlias()
{
EFS_BEGIN

	m_arrDefaultIgnoreAlias.RemoveAll();

EFS_END
}

INT CBO_CAT_PROC_Api::AddDefaultIgnoreAlias(LPCTSTR lpszAlias)
{
EFS_BEGIN

	return m_arrDefaultIgnoreAlias.Add(lpszAlias);

EFS_END
return -1;

}

inline BOOL CBO_CAT_PROC_Api::IsBook(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_책부록플래그"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

inline BOOL CBO_CAT_PROC_Api::IsAppendix(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_책부록플래그"), idx) != _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

INT CBO_CAT_PROC_Api::GetRepresentationBook(CESL_DataMgr *pDM_Book, INT &nIdx)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		if (pDM_Book->GetCellData(_T("BB_대표책여부"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::SetRepresentationBook(CESL_DataMgr *pDM_Book, INT nIdx /*= -1*/)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, nIdx) < 0)
		{
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx)) break;
			nIdx = idx;
		}		
	}
	else
	{
		if (!IsBook(pDM_Book, nIdx)) return -1;
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData(_T("BB_대표책여부"), _T("Y"), idx);
		else
			pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), idx);
	}
	
	return nIdx;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::CorrectBookDM(CESL_DataMgr *pDM_Book)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	CorrectBookDM_DupCode(pDM_Book);

	SetRepresentationBook(pDM_Book);

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT ids = -1;
	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	INT nVolCnt;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_편권차"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	if (mapVolume.GetCount() == 0) return 0;


	POSITION pos = mapVolume.GetStartPosition();
	while (pos)
	{
		mapVolume.GetNextAssoc(pos, strVol, nVolCnt);
		
		CorrectBookDM_DupCode(pDM_Book, strVol);
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_Api::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book, CString strVol)
{
EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	
	CString strDupCode;
	INT n = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strDupCode.Empty();

		if (!IsBook(pDM_Book, idx, strVol)) continue;

		if (n != 0) 
			strDupCode.Format(_T("c%d"), ++n);

		pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, idx);
	}

	return 0;

EFS_END
return -1;

}