// BOFormApi.cpp: implementation of the CBOFormApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BOFormApi.h"
#include "ISBNChecker.h"
#include "MarcDefaultValueMgr.h"
#include "../COMMON_EBOOK_API/EBookViewMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBOFormApi::CBOFormApi(CESL_Mgr *pParentMgr  /*= NULL*/)
{
	m_nAppMode = MODE_INSERT;
	m_pCopyCodeMgr = NULL;
	m_pParnetMgr = pParentMgr;
	if( m_pParnetMgr != NULL )		LoadRegCode();
}

CBOFormApi::~CBOFormApi()
{

}

//////////////////////////////////////////////////////////////////////
// CBOFormApi Operations
INT CBOFormApi::LoadRegCode()
{
	INT nCodeCount, nStartRow, nCnt, i;
	CString strArrCode[6];

	if( m_pParnetMgr == NULL )		return -1;

	nCodeCount = m_pParnetMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	nStartRow = nCnt = 0;	
	for( i=0 ; nCnt<nCodeCount ; i++ )
	{
		nStartRow = m_pParnetMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), strArrCode, 6, nStartRow);
		if( strArrCode[5] == _T("Y") )
		{
			m_arrRegCode.Add(strArrCode[2]);
			nCnt++;
		}
		
		nStartRow++;
	}

	return 0;
}

INT CBOFormApi::FetchRegCode(CString strRegCodeField, CArray<CRegCode, CRegCode> *pArrRegCode)
{
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
		
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
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
	
	CRegCode regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);

		pArrRegCode->Add(regCode);
	}

	return 0;
}

BOOL CBOFormApi::IsValidRegCode(CString strRegCode)
{
	if (m_pParnetMgr == NULL) return FALSE;
	if (strRegCode.IsEmpty()) return FALSE;

	return IsStringInArray(&m_arrRegCode, strRegCode);
}

BOOL CBOFormApi::IsRegCodeModify(CArray<CRegCode, CRegCode> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pArrRegCode == NULL || pDM_Book == NULL) return FALSE;

	INT ids = -1;
	CArray<CRegCode, CRegCode> arrExistRegCode;
	ids = GetRegCodeInfo(pDM_Book, &arrExistRegCode, strVol);

	INT idx = 0;
	INT nBookCount = 0;
	INT nRegCodeCount = 0;
	CRegCode regCode;

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
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

INT CBOFormApi::MakeBookFromRegCode(CArray<CRegCode, CRegCode> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pArrRegCode == NULL || pDM_Book == NULL) return -1;
	
	INT ids = -1;
	INT nDMRowCount = pDM_Book->GetRowCount();
	if (nDMRowCount == 0) return -1;

	INT nRegCodeCount = pArrRegCode->GetSize();
	if (nRegCodeCount == 0) return -1;

	CString strRegCode;
	CString strQueryFlag;

	CRegCode regCode;
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

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
	}

	CString strSeShelfCode, strDupCode;
	nDMRowCount = pDM_Book->GetRowCount();
	for(idx = 0 ; idx < nDMRowCount ; idx++)
	{
		if (!IsBook(pDM_Book, idx, strVol)) continue;
		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		if(strQueryFlag == _T("D")) continue;

		INT nDupNumber ;
		if(m_pCopyCodeMgr == NULL) continue;
		else
		{
			if(pDM_Book->GetCellData(_T("BB_복본기호"), idx).IsEmpty())
			{
				m_pCopyCodeMgr->SetSrcInfo(pDM_Book->GetCellData(_T("BB_편권차"), idx), 
											pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx), 
											pDM_Book->GetCellData(_T("BB_별치기호_코드"), idx));
				nDupNumber = GetLastDupCodeNumber(pDM_Book, idx, idx);
				if (nDupNumber < 0) continue;
				strDupCode.Format(_T("%d"), ++nDupNumber);
				pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, idx);		
			}
		}
	}

	nDMRowCount = pDM_Book->GetRowCount();
	for(idx = nDMRowCount-1 ; idx >= 0 ; idx--)
	{
		if (pDM_Book->GetCellData(_T("BB_복본기호"), idx) == _T("1"))
			pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);
	}

	return 0;
}

BOOL CBOFormApi::IsInRegCode(CArray<CRegCode, CRegCode> *pArrRegCode, CString strRegCode)
{
	if (pArrRegCode == NULL || strRegCode.IsEmpty()) return FALSE;

	INT nCount = pArrRegCode->GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		if (pArrRegCode->GetAt(i).m_strRegCode == strRegCode)
			return TRUE;
	}

	return FALSE;
}

INT CBOFormApi::DeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
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
}

INT CBOFormApi::CheckDeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
	if (pDM_Book == NULL) return -1;

	CString strQuery;
	CString strResult;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) == strVol)
		{
			if (CheckDeleteBook(pDM_Book, idx) < 0)
				return -1;
		}
	}

	return 0;
}

INT CBOFormApi::CheckDeleteBook(CESL_DataMgr *pDM_Book, INT nRow)
{
	if (pDM_Book == NULL) return -1;

	CString strQuery;
	CString strResult;
	CString strRecKey = pDM_Book->GetCellData(_T("BB_책KEY"), nRow);
	if (!strRecKey.IsEmpty())
	{
		strQuery.Format(_T("SELECT SUM(COUNT) FROM ")
						_T("(SELECT COUNT(1) AS COUNT FROM BO_BOOK_TBL BB , LH_STORAGE_V01 CL ")
						_T("WHERE BB.REC_KEY = %s  AND ")
						_T("BB.REC_KEY = CL.BOOK_KEY AND CL.STATUS IN ( '0' , '2' , '3' ) AND BB.MANAGE_CODE = UDF_MANAGE_CODE ")
						_T("UNION ")
						_T("SELECT COUNT(1) AS COUNT FROM BO_BOOK_TBL BB2 ")					
						_T("WHERE BB2.REC_KEY = %s AND BB2.WORKING_STATUS IN ('BOL218O', 'BOL219O') AND BB2.MANAGE_CODE = UDF_MANAGE_CODE)")
						, strRecKey, strRecKey);
		if (pDM_Book->GetOneValueQuery(strQuery, strResult) >= 0)
		{
			if (_ttoi(strResult) > 0)
			{
				return -1;
			}
		}
	}
	return 0;
}

CString CBOFormApi::GetBookKey(CESL_DataMgr *pDM_Book, INT nIdx)
{
	if (pDM_Book == NULL) return _T("");

	CString strKey = pDM_Book->GetCellData(_T("BB_책KEY"), nIdx);
	if (strKey.IsEmpty())
		strKey = pDM_Book->GetCellData(_T("BB_UNIQUE_KEY"), nIdx);

	return strKey;
}

INT CBOFormApi::DeleteBookDMRow(CESL_DataMgr *pDM_Book, INT idx, CString strQueryFlag /*=_T("")*/)
{
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
}

INT CBOFormApi::GetVolumeCount(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_권일련번호"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();
}

INT CBOFormApi::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	INT nReturnCnt, nRowCount, i;
	
	if( pDM_Book == NULL )	return -1;

	nReturnCnt = 0;
	nRowCount = pDM_Book->GetRowCount();
	for( i=0; i<nRowCount; i++ )
	{
		if( IsBook(pDM_Book, i, lpszVol) == FALSE )		continue;

		if( strRegCode.IsEmpty() == FALSE )
			if( pDM_Book->GetCellData(_T("BB_등록구분_코드"), i) != strRegCode )	continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;
}

INT CBOFormApi::GetRegCodeInfo(CESL_DataMgr *pDM_Book, CArray<CRegCode, CRegCode> *pArrRegCode, LPCTSTR lpszVol /*= NULL*/)
{
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

	CRegCode regCode;
	POSITION pos = mapRegCode.GetStartPosition();
	while (pos)
	{
		regCode.Init();
		mapRegCode.GetNextAssoc(pos, regCode.m_strRegCode, regCode.m_nCount);
		pArrRegCode->Add(regCode);
	}

	return 0;
}

INT CBOFormApi::AddBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
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
	CString strDupCode, strSeShelfCode;
	INT nNewRow = -1;
	for (idx = 0; idx < nCount; idx++)
	{
		nNewRow = CopyDM(pDM_Book, nStandardRowIdx, pDM_Book, -1);
		if (nNewRow < 0) continue;

		pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, nNewRow);

		pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), nNewRow);

		pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), nNewRow);

		strQueryFlag = GetQueryFlag(MODE_INSERT);
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), nNewRow);
	}

	return 0;
}

INT CBOFormApi::RemoveBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
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
}

INT CBOFormApi::CopyDMInArray(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CStringArray *pArrIgnoreAlias, CString strToAliasHeader /*= _T("")*/)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	if(pArrIgnoreAlias == NULL)	 return -1;
	if(pArrIgnoreAlias->GetSize() == 0) return 0;

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
		
		if (!IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
			
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
}

INT CBOFormApi::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
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
}

INT CBOFormApi::MakeDMFromCMInArray(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, CStringArray *pArrIgnoreAlias, INT nIdx /*= -1*/, CString strToAliasHeader /*= _T("")*/)
{
	if (pCM == NULL || pDM == NULL) return -1;
	if(pArrIgnoreAlias == NULL) return -1;
	if(pArrIgnoreAlias->GetSize() == 0) return 0;
	
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
	CString strData, strTmpData;
	CESL_Control_Element *pCE = NULL;
	INT startpos = -1;
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;
		
		if (!IsStringInArray(pArrIgnoreAlias, strAlias)) continue;	
		
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
}

INT CBOFormApi::MakeDMFromCM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, INT nIdx /*= -1*/, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
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
	CString strData, strTmpData;
	CESL_Control_Element *pCE = NULL;
	INT startpos = -1;
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
}

BOOL CBOFormApi::IsStringInArray(CStringArray *pArr, CString str)
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

CString CBOFormApi::GetQueryFlag(INT nWorkMode, CString strQueryFlag /*= _T("")*/)
{
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
		else if(strQueryFlag == _T("N"))
		    return _T("I");

	}
	
	return _T("");
}

INT CBOFormApi::MakeAppendixDM(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Appendix, CString strVol /*= _T("")*/, CString strParentKey /*=_T("")*/)
{
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
}

INT CBOFormApi::GetBookPriceSum(CESL_DataMgr *pDM_Book, INT &nSum)
{
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
}

INT CBOFormApi::ApplySpeciesPriceToBookPrice(CESL_DataMgr *pDM_Book, INT nPrice)
{

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
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	
	return 0;
}

INT CBOFormApi::ApplyCompleteWorks(CESL_DataMgr *pDM_Book, CString &strVol)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	INT n = 1;
	CString strTemp, strVolSortNo;
	CString strRegCode, strSepShelfCode;
	INT nCopyCode;
	CString strCopyCode;

	strTemp.Format(_T("%d"), n++);

	CCopyCodeMgr BookCopyCodeMgr;
	BOOL		 bCopyCheck;
	BookCopyCodeMgr.Init(pDM_Book->CONNECTION_INFO);
	
	nCopyCode = 1;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		pDM_Book->SetCellData(_T("BB_편권차"), strTemp, idx);
		strTemp.Format(_T("%d"), n++);
		
		strVolSortNo.Format(_T("%d"), (idx+1)*10);
		
		pDM_Book->SetCellData(_T("BB_권일련번호"), strVolSortNo, idx);

		if ( idx  == 0 )
		{
			pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);		
		}
		else
		{
			pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx, strRegCode );
			pDM_Book->GetCellData(_T("BB_별치기호_코드"), idx, strSepShelfCode );		
			
			BookCopyCodeMgr.SetSrcInfo(strVolSortNo, strRegCode, strSepShelfCode);
			bCopyCheck = BookCopyCodeMgr.IsCopy(pDM_Book, idx);
			if ( bCopyCheck ) 
			{
				nCopyCode ++;
				strCopyCode.Format(_T("%d"), nCopyCode);
				pDM_Book->SetCellData(_T("BB_복본기호"), strCopyCode, idx);		
			}
			else 
			{
				pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);
			}
		}
	}

	return 0;
}

CString CBOFormApi::ConvertWORKING_STATUSToCode(CString strDesc)
{
	CString result = _T("");
	
	if (strDesc == _T("기초자료")) result = _T("BOT111O");
	if (strDesc == _T("반입자료")) result = _T("BOT112O");
	if (strDesc == _T("구입대상자료")) result = _T("BOA111N");
	if (strDesc == _T("주문자료")) result = _T("BOA112N");
    if (strDesc == _T("미납자료")) result = _T("BOA113O");
	if (strDesc == _T("등록인계대상자료")) result = _T("BOA114O");
	if (strDesc == _T("기증접수자료")) result = _T("BOA211O");
	if (strDesc == _T("기증부당자료")) result = _T("BOA212O");
	if (strDesc == _T("등록인계자료")) result = _T("BOR111N");
	if (strDesc == _T("등록자료")) result = _T("BOR112N");

	return result;
}

VOID CBOFormApi::InitDefaultAlias()
{
	m_arrDefualtValueAlias.RemoveAll();
}

INT CBOFormApi::AddDefaultAlias(LPCTSTR lpszAlias)
{
	return m_arrDefualtValueAlias.Add(lpszAlias);
}

INT CBOFormApi::SaveDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
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
}

INT CBOFormApi::SaveDefaultValue(CESL_DataMgr *pDM, LPCTSTR lpszFileName)
{
	if (pDM == NULL) return -1;
	
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::typeBinary))
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
		if(strAlias.Left(2) != _T("BP")) continue;
		ids = pDM->GetCellData(strAlias, 0, strValue);
		if (ids < 0) continue;
		
		strLine.Format(_T("%s\t=\t%s\r\n"), strAlias, strValue);

		file.WriteString(strLine);
	}

	file.Close();
	
	return 0;
}

INT CBOFormApi::LoadDefaultValue(CESL_ControlMgr *pCM, LPCTSTR lpszFileName)
{
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
}

INT CBOFormApi::LoadDefaultValue(CESL_DataMgr *pDM, LPCTSTR lpszFileName)
{
	if (pDM == NULL) return -1;

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
		if(strAlias.Left(2) != _T("BP")) continue;
		
		ids = pDM->SetCellData(strAlias, strValue, 0);
	}

	file.Close();

	return 0;
}

INT CBOFormApi::CheckISBN(CString strISBNFrom, CString &strISBNTo)
{
	INT nLength, iResult, nFind, ids = -1;
	CString strSet, strTemp, strTemp2, strMsg, strOutISBN, strCheckDigit;
	CISBNChecker isbnChecker;
	

	strISBNTo.Empty();

	nFind = strISBNFrom.Find('(');
	if( nFind >= 0 )
	{
		strSet = strISBNFrom = strISBNFrom.Left(nFind);
	}
	if( strISBNFrom.IsEmpty() == TRUE )		return -1;

	strTemp = strISBNFrom;
	strISBNFrom.Empty();
	isbnChecker.RemoveDashFromISBN(strTemp.GetBuffer(0), strISBNFrom.GetBuffer(MAX_PATH));
	strISBNFrom.ReleaseBuffer();

	if( strISBNFrom.IsEmpty() == TRUE )		return -1;
	
	iResult = -1;
	nLength = strISBNFrom.GetLength();
	switch( nLength )
	{
	case 9:
		{
			isbnChecker.ReturnISBNCheckDigit( strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
			strOutISBN.ReleaseBuffer();
			strCheckDigit.ReleaseBuffer();
			strTemp = strISBNFrom.Left(2);
			strISBNTo.Format(_T("%s-%s-%s%s"), strTemp, strOutISBN, strCheckDigit, strSet);
			
			strMsg.Format(_T("(%s)로 생성되었습니다."), strISBNTo);
			MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION);
		}
		break;
	case 10:
		{
			ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
			if( ids < 0 )	return -1;

			if( iResult == 0 )
			{
				strMsg = _T("올바른 ISBN이 입력되었습니다.");
				MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION );
				strISBNTo += strSet;
			}
			else if (iResult == -2)
			{
				strMsg = _T("ISBN이 잘못되었습니다. 수정하시겠습니까?");
				if( IDYES == MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_YESNO | MB_ICONQUESTION) )
				{
					strISBNTo += strSet;
				}
				else
				{
					return -1;
				}
			}
			else
			{}
		}
		break;

	case 12:
		{			
			isbnChecker.ReturnISBNCheckDigit( strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
			strOutISBN.ReleaseBuffer();
			strCheckDigit.ReleaseBuffer();
			strTemp = strISBNFrom.Left(3);
			strTemp2 = strISBNFrom.Mid(3,2);
			strISBNTo.Format(_T("%s-%s-%s-%s%s"), strTemp, strTemp2, strOutISBN, strCheckDigit, strSet);
			
			strMsg.Format(_T("(%s)로 생성되었습니다."), strISBNTo);
			MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION);
		}
		break;
	case 13:
		{
			ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
			if( ids < 0 )	return -1;

			if( iResult == 0 )
			{
				strMsg = _T("올바른 ISBN이 입력되었습니다.");
				MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION );
				strISBNTo += strSet;
			}
			else if (iResult == -2)
			{
				strMsg = _T("ISBN이 잘못되었습니다. 수정하시겠습니까?");
				if( IDYES == MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_YESNO | MB_ICONQUESTION) )
				{
					strISBNTo += strSet;
				}
				else
				{
					return -1;
				}
			}
			else
			{}
		}
		break;

	default:
		{
			strMsg = _T("잘못된 자리수가 입력되었습니다");
			MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
			return -1;
		}
		break;
	}

	return 0;
}

INT CBOFormApi::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CString strVol)
{
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
		if (strTemp.IsEmpty()) strTemp = _T("1");

		_stscanf(strTemp, _T("%d"), &nNumber);
				
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;
}

INT CBOFormApi::AutoApplyDupCode(CESL_DataMgr *pDM_Book)
{
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

			strDupCode.Format(_T("%d"), ++nDupNo);
			pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, idx);

			CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}


	return 0;
}

INT CBOFormApi::AutoApplyAppendixRegNo(CESL_DataMgr *pDM_Book)
{
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
		
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

		if (strParentBookRegNoMaster.GetLength() <= nRegCodeLength) continue;

		nRegNo = 0;

		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
	
		pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

		CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

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
}

INT CBOFormApi::GetParentBookIndex(CESL_DataMgr *pDM_Book, CString strParentBookKey)
{
	if (pDM_Book == NULL || strParentBookKey.IsEmpty()) return -1;


	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx <nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		if (GetBookKey(pDM_Book, idx) == strParentBookKey)
			return idx;
	}

	return -1;
}

VOID CBOFormApi::InitDefaultIgnoreAlias()
{
	m_arrDefaultIgnoreAlias.RemoveAll();
}

INT CBOFormApi::AddDefaultIgnoreAlias(LPCTSTR lpszAlias)
{
	return m_arrDefaultIgnoreAlias.Add(lpszAlias);
}

inline BOOL CBOFormApi::IsBook(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_책부록플래그"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

inline BOOL CBOFormApi::IsAppendix(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_책부록플래그"), idx) != _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

INT CBOFormApi::GetRepresentationBook(CESL_DataMgr *pDM_Book, INT &nIdx)
{
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
}

INT CBOFormApi::SetRepresentationBook(CESL_DataMgr *pDM_Book, INT nIdx /*= -1*/)
{
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
}

INT CBOFormApi::CorrectBookDM(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	CorrectBookDM_DupCode(pDM_Book);

	SetRepresentationBook(pDM_Book);

	return 0;
}

INT CBOFormApi::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book)
{
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
}

INT CBOFormApi::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book, CString strVol)
{
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
}

INT CBOFormApi::EmbededVolSortNo(CESL_DataMgr * pDM_Book, CString strVolSortNo)
{
	INT volSortNo, tmpVolSortNo;
	CString strTmpData;
	volSortNo = _ttoi(strVolSortNo);
	INT cnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM_Book->GetCellData(_T("BB_권일련번호"), i, strTmpData);
		tmpVolSortNo = _ttoi(strTmpData);
		if(volSortNo <= tmpVolSortNo)
		{
			strTmpData.Format(_T("%d"), ((tmpVolSortNo/10)+1)*10);
			pDM_Book->SetCellData(_T("BB_권일련번호"), strTmpData, i);
			strTmpData = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i);
			if(strTmpData.IsEmpty())
			{
				strTmpData = GetQueryFlag(MODE_UPDATE);
				pDM_Book->SetCellData(_T("BB_DB_FLAG"), strTmpData, i);
			}
		}
	}
	return 0;
}

INT CBOFormApi::GetLastVolSortNo(CESL_DataMgr * pDM_Book, CString &strVolSortNo)
{
	INT volSortNo, tmpVolSortNo;
	CString strTmpData;
	INT cnt = pDM_Book->GetRowCount();
	volSortNo = -1;
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM_Book->GetCellData(_T("BB_권일련번호"), i, strTmpData);
		tmpVolSortNo = _ttoi(strTmpData);
		if(volSortNo < tmpVolSortNo) volSortNo = tmpVolSortNo;
	}

	strVolSortNo.Format(_T("%d"), ((volSortNo/10)+1)*10);
	return 0;
}

INT CBOFormApi::ApplySpeciesPriceToVolPrice(CESL_DataMgr *pDM_Book, DOUBLE nPrice, CString &strTotalPrice)
{
	if (pDM_Book == NULL) return -1;

	INT nVolCount = GetVolumeCount(pDM_Book);
	if (nVolCount <= 0) return -1;

	if (nPrice < 0) return -1;

	DOUBLE nVolPrice = 0; 
	DOUBLE nLastVolPrice = 0;
	TCHAR* pEndPtr = NULL;

	if (nPrice >= nVolCount) 
	{
		CString strPrice;
		strPrice.Empty();
		nVolPrice = nPrice / nVolCount;
		strPrice = TruncPrice(pDM_Book, nVolPrice);

		nVolPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

		nLastVolPrice = nPrice - (nVolPrice * nVolCount);
		if (nLastVolPrice == 0)
			nLastVolPrice = nVolPrice;
		else
			nLastVolPrice = nLastVolPrice + nVolPrice;
	}
	if (nLastVolPrice < 0) return -1;

	CString strVolPrice;
	CString strLastVolPrice, strVolSortNo, strTmpData;

	strVolPrice = TruncPrice(pDM_Book, nVolPrice);
	strLastVolPrice = TruncPrice(pDM_Book, nLastVolPrice);

	DOUBLE nTotalPrice = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;
	strVolSortNo = pDM_Book->GetCellData(_T("BB_권일련번호"), 0);
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		strTmpData = pDM_Book->GetCellData(_T("BB_권일련번호"), idx);
		if(strVolSortNo != strTmpData)
		{
			strVolSortNo = strTmpData;
		}
		
		if (idx == nRowCount-1)
		{
			pDM_Book->SetCellData(_T("BB_가격"), strLastVolPrice, idx);		
			nTotalPrice += _tcstod(strLastVolPrice.GetBuffer(0), &pEndPtr);
		}
		else
		{
			pDM_Book->SetCellData(_T("BB_가격"), strVolPrice, idx);
			nTotalPrice += _tcstod(strVolPrice.GetBuffer(0), &pEndPtr);
		}

		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	
	strTotalPrice = TruncPrice(pDM_Book, nTotalPrice);
	return 0;
}

INT CBOFormApi::ApplyVolPrice(CESL_DataMgr *pDM_Book, DOUBLE nPrice, CString &strTotalPrice, CString &str950$b)
{

	if (pDM_Book == NULL) return -1;

	INT nVolCount = GetVolumeCount(pDM_Book);
	if (nPrice < 0) return -1;

	CString strVolPrice, strVolSortNo, strTmpData;

	strVolPrice.Format(_T("%.2f"), nPrice);
	
	INT n = 0;
	DOUBLE nTotalPrice = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx)) continue;

		pDM_Book->SetCellData(_T("BB_가격"), strVolPrice, idx);
		
		TCHAR* pEndPtr = NULL;
		nTotalPrice += _tcstod(strVolPrice.GetBuffer(0), &pEndPtr);
	
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	
	str950$b.Format(_T("%.2f"), nPrice*nVolCount);
	strTotalPrice = TruncPrice(pDM_Book, nTotalPrice);

	return 0;
}

INT CBOFormApi::DisplayISBN(CESL_ControlMgr *pCM, CString strAlias)
{
	CISBNChecker isbnChecker;
	CString strData, strConvData;
	pCM->GetControlMgrData(strAlias, strData);
	if(strData.IsEmpty()) return 0;
	strData.Replace(_T("-"), _T(""));
	
	TCHAR    *szExtraStrPointer;
	TCHAR    szTemp[128];		
	TCHAR    szTempISBN[128];	
	TCHAR    szCorrectFormattedISBN[MAX_PATH];			

	INT nFirstDash = 2;
    INT nThirdDash = 11;
	INT nSecondDash;
	INT ISBNData;

	INT nLength;
	szExtraStrPointer = _tcschr( strData.GetBuffer(0), '(' );	
	if ( szExtraStrPointer != NULL )
	{
		nLength = strData.GetLength() - _tcsclen(szExtraStrPointer);
		_tcscpy(szTempISBN, strData.Left(nLength).GetBuffer(0));	
	}
	else
	{
		_tcscpy(szTempISBN, strData.GetBuffer(0));
	}
    
	if( _tcsclen(szTempISBN) > 10 )
	{
		INT nAddDash = 3;
		nFirstDash = 2 + nAddDash + 1;
		nThirdDash = 11 + nAddDash + 1;

		wmemcpy( szTemp , &szTempISBN[2+nAddDash] , 2 );
		szTemp[2] = 0;
		ISBNData = _ttoi( szTemp );

		if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5 + nAddDash +1;
		if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6 + nAddDash +1;
		if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7 + nAddDash +1;
		if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8 + nAddDash +1;
		if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9 + nAddDash +1;

		wmemcpy( szCorrectFormattedISBN                  , szTempISBN                , nAddDash );
		wmemcpy( szCorrectFormattedISBN +(nAddDash +1)   , szTempISBN+nAddDash       , 2 );
		wmemcpy( szCorrectFormattedISBN +(nFirstDash+1)  , szTempISBN+nAddDash+2     , nSecondDash-nFirstDash-1 );
		wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-2), nThirdDash-nSecondDash-1 );
		wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN+nAddDash+9     , 1 );

		szCorrectFormattedISBN[nAddDash]	= '-';
		szCorrectFormattedISBN[nFirstDash]	= '-';
		szCorrectFormattedISBN[nSecondDash] = '-';
		szCorrectFormattedISBN[nThirdDash]	= '-';
		szCorrectFormattedISBN[17]			= 0	 ;

		if( szExtraStrPointer != NULL ) 
		{
			_tcscpy( szCorrectFormattedISBN+17, szExtraStrPointer );
		}
	}
	else
	{
		wmemcpy( szTemp , &szTempISBN[2] , 2 );
		szTemp[2] = 0;
		ISBNData = _ttoi( szTemp );

		if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5;
		if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6;
		if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7;
		if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8;
		if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9;

		wmemcpy( szCorrectFormattedISBN                  , szTempISBN                 , 2 );
		wmemcpy( szCorrectFormattedISBN +(nFirstDash +1) , szTempISBN +2              , nSecondDash-nFirstDash-1 );
		wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-1) , nThirdDash-nSecondDash-1 );
		wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN +9              , 1 );

		szCorrectFormattedISBN[nFirstDash]	= '-';
		szCorrectFormattedISBN[nSecondDash] = '-';
		szCorrectFormattedISBN[nThirdDash]	= '-';
		szCorrectFormattedISBN[13]			= 0	 ;


		if( szExtraStrPointer != NULL ) 
		{
			_tcscpy( &szCorrectFormattedISBN[13], szExtraStrPointer );
		}
	}

	pCM->SetControlMgrData(strAlias, CString(szCorrectFormattedISBN));
	return 0;
}


INT CBOFormApi::SaveDefaultValue(CString strMarc, LPCTSTR lpszFileName)
{
	CMarcDefaultValueMgr mgr(m_pParnetMgr);
	mgr.SetFileName(lpszFileName);
	return mgr.SaveDefaultValue(strMarc);
}

INT CBOFormApi::LoadDefaultValue(CString &strMarc, LPCTSTR lpszFileName)
{
	CMarcDefaultValueMgr mgr(m_pParnetMgr);
	mgr.SetFileName(lpszFileName);
	return mgr.LoadDefaultValue(strMarc);
}

INT CBOFormApi::SetVolSortNo(CCopyCodeMgr *pCopyCodeMgr, CESL_DataMgr *pDM_Book)
{
	if(pCopyCodeMgr == NULL || pDM_Book == NULL) return -1;
	INT nRowCnt = pDM_Book->GetRowCount();
	if(nRowCnt == 0) return 0;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pDM_Book->SetCellData(_T("BB_권일련번호"), _T(""), i);
		pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), i);
	}

	INT nVolSortNo, nCopyCode;
	nVolSortNo = 10;	nCopyCode = 1;
	CString strRegCode, strVol, strSeShelfCode, strTmpData;
	strRegCode = strVol = strSeShelfCode = _T("-1");
	CString strTmpRegCode, strTmpVol, strTmpSeShelfCode;
	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpRegCode = pDM_Book->GetCellData(_T("BB_등록구분"), i);
		strTmpVol = pDM_Book->GetCellData(_T("BB_편권차"), i);
		strTmpSeShelfCode = pDM_Book->GetCellData(_T("BB_별치기호"), i);

	}
	return 0;
}

INT CBOFormApi::AllControlClear(CESL_ControlMgr *pCM, CStringArray *pIgnoreAlias)
{
EFS_BEGIN
	if(pCM == NULL) return -1;
	INT cecount = pCM->Control_List.GetCount();
	INT nAliasCnt = 0;
	if(pIgnoreAlias) nAliasCnt = pIgnoreAlias->GetSize();
	CESL_Control_Element * pCE = NULL;
	CString strCtrlAlias, strTmpData;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if(pCE == NULL) continue;
		strCtrlAlias = pCE->CTRL_ALIAS;
		for(INT j=0 ; j<nAliasCnt ; j++)
		{
			strTmpData = pIgnoreAlias->GetAt(j);
			if(strCtrlAlias == strTmpData) break;
		}
		
		if (pCE->CTRL_TYPE == _T("M")) {
			if (pCE->CTRL_ID < 0) return 0;
			if (!pCE->Control) return -1;
			CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox*)pCE->Control;
			if(!pControl->IsWindowEnabled()) pControl->EnableWindow(TRUE);
		}	
		else if(pCE->CTRL_TYPE == _T("E"))
		{
			if(j < nAliasCnt) continue;
			if (pCE->CTRL_ID < 0) return 0;
			if (!pCE->Control) return -1;
			CESL_ControlEdit * pControl = (CESL_ControlEdit*)pCE->Control;
			if((pControl->GetStyle() & ES_READONLY) && j == nAliasCnt) pControl->SetReadOnly(FALSE);
		}
		else if (pCE->CTRL_TYPE == _T("G"))
		{
			if (pCE->CTRL_ID < 0) return 0;
			if (!pCE->Control) return -1;
			CESL_Grid * pControl = (CESL_Grid*)pCE->Control;
			if(!pControl->IsWindowEnabled()) pControl->EnableWindow(TRUE);
		}			
		else if (pCE->CTRL_TYPE == _T("D"))
		{
			if (pCE->CTRL_ID < 0) return 0;
			if (!pCE->Control) return -1;
			CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)pCE->Control;
			pControl->ECO_SetReadOnly(FALSE);
		}	

		if(j == nAliasCnt)
			pCE->Clear();	
	}
	return 0;

EFS_END
return -1;
}

INT CBOFormApi::AllControlDisplay(CESL_ControlMgr *pCM, CStringArray *pIgnoreAlias, INT nIdx/*=-1*/)
{
	EFS_BEGIN

	INT cecount = pCM->Control_List.GetCount();
	INT nAliasCnt = pIgnoreAlias->GetSize();
	CESL_Control_Element * pCE = NULL;
	CString strCtrlAlias, strTmpData;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if(pCE == NULL) continue;
		strCtrlAlias = pCE->CTRL_ALIAS;
		for(INT j=0 ; j<nAliasCnt ; j++)
		{
			strTmpData = pIgnoreAlias->GetAt(j);
			if(strCtrlAlias == strTmpData) break;
		}
		if(j < nAliasCnt) continue;
		pCE->Display(nIdx);
	}
	return 0;

	EFS_END
	return -1;
}

INT CBOFormApi::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, INT nCurIdx, INT nEndIdx/*=-1*/, CString strRegNo/*=_T("-1")*/)
{
	if (pDM_Book == NULL) return -1;

	INT nRowCount;
	if(nEndIdx > -1) 
		nRowCount = nEndIdx;
	else
		nRowCount = pDM_Book->GetRowCount();
	
	if(strRegNo.IsEmpty()) return 0;
	
	INT nDupNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		CString strTemp;
		INT nNumber = 0;

		if (!m_pCopyCodeMgr->IsCopy(pDM_Book, idx)) continue;
		if(idx == nCurIdx) continue;
		if(strRegNo != _T("-1"))
		{
			strTemp = pDM_Book->GetCellData(_T("BB_등록번호"), idx);
			if(strTemp.IsEmpty()) continue;
		}

		strTemp = pDM_Book->GetCellData(_T("BB_복본기호"), idx);
		if (strTemp.IsEmpty()) strTemp = _T("1");

		nNumber = _ttoi(strTemp.GetBuffer(0));	
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;
}


INT CBOFormApi::EBookFileOpen(CESL_Mgr * pMgr, INT nMode, CESL_ControlMgr * pCM)
{
	EFS_BEGIN

	CString strAliasHeader, strFileSize;
	if(pCM->CM_ALIAS == _T("CMUP_단행_종_색인정보")) strAliasHeader = _T("IBS");
	else strAliasHeader = _T("BB");

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = pMgr->m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if(nMode == EBOOK_COVER)
	{
		OFN.lpstrFilter = _T("JPEG(*.jpg)\0*.jpg\0GIF(*.gif)\0*.gif\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("E-BOOK 표지파일 열기");
	}
	else if(nMode == EBOOK_FILE)
	{
		OFN.lpstrFilter = _T("모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("E-BOOK 원본파일 열기");
	}
	else if(nMode == EBOOK_EXECUTER)
	{
		OFN.lpstrFilter = _T("실행파일(*.exe)\0*.exe\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("E-BOOK 실행파일 열기");
	}
		
	if (GetOpenFileName(&OFN) != 0)
	{
		if(nMode == EBOOK_COVER)
			pCM->SetControlMgrData(strAliasHeader+_T("_이미지파일"), OFN.lpstrFile);
		else if(nMode == EBOOK_FILE)
			pCM->SetControlMgrData(strAliasHeader+_T("_원본파일"), OFN.lpstrFile);
		else if(nMode == EBOOK_EXECUTER)
			pCM->SetControlMgrData(strAliasHeader+_T("_실행파일"), OFN.lpstrFile);
	}

	SetCurrentDirectory(szBuffer);
	return 0;
	EFS_END
return -1;
}

INT CBOFormApi::MakeBookDM(CString strQuery, CESL_DataMgr *pDM_BOOK, EDBM_BIND_DATA_TYPE *pBind)
{
	CESL_DataMgr * pDM = new CESL_DataMgr;
	pDM->InitDMPKFromFile(_T("DMUP_단행_종_책EBOOK정보"));		
	pDM->SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	pDM->SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);
	INT ids = pDM->RefreshDataManager(strQuery, NULL, pBind);
	if(ids < 0)
	{
		if(pDM)
		{
			delete pDM;
			pDM = NULL;
		}
		return -1;
	}

	InitDefaultIgnoreAlias();
	
	pDM_BOOK->FreeData();
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		CopyDM(pDM, i, pDM_BOOK, -1);
	}
	if(pDM)
	{
		delete pDM;
		pDM = NULL;
	}
	
	return 0;
}

INT CBOFormApi::EBookView(CString strBookKey, CString strExeFile, CString strFile)
{
	CString strMsg;
	CEBookViewMgr mgr;
	if(strFile.IsEmpty()) return -1;

	INT ids = mgr.EBookView(strBookKey, strExeFile, strFile);
	if(ids < 0)
	{
		strMsg.Format(_T("[%s]파일을 실행할 수 없습니다."), strFile);
		m_pParnetMgr->ESLAfxMessageBox(strMsg);
	}
	return 0;
}

VOID CBOFormApi::DeleteEBookTempFolder()
{
	CEBookViewMgr mgr;
	mgr.DeleteEBookTempFolder();
}

CString CBOFormApi::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '가격절사단위설정'"), strValue);
	
	if(strValue == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(strValue == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

