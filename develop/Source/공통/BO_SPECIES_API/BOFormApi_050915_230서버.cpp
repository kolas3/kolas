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
	m_pParnetMgr = pParentMgr;
	m_nAppMode = MODE_INSERT;
	m_pCopyCodeMgr = NULL;
	if (m_pParnetMgr != NULL)
	{
		LoadRegCode();
	}
}

CBOFormApi::~CBOFormApi()
{

}

//////////////////////////////////////////////////////////////////////
// CBOFormApi Operations
INT CBOFormApi::LoadRegCode()
{
	if (m_pParnetMgr == NULL) return -1;

	INT nCodeCount = m_pParnetMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	INT nStartRow = 0;
	INT nCnt = 0;
	CString strArrCode[6];
	for (INT i=0 ; nCnt<nCodeCount ; i++)
	{
		nStartRow = m_pParnetMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), strArrCode, 6, nStartRow);
		if(strArrCode[5] == _T("Y"))
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

	// '/'로 나눈다.
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
		

		if (strData.GetLength() < 2) return -1;
		strRegCode = strData.Left(2);

		// 등록된 등록구분인지 체크
		if (!IsValidRegCode(strRegCode)) return -1;

		if (strData.GetLength() == 2)
			strCount = _T("1");
		else
			strCount = strData.Mid(2);

		INT nLength = strCount.GetLength();
		for (INT i = 0; i < nLength; i++)
		{
			if (!_istdigit(strCount.GetAt(i))) return -1;
		}

		nCount = _ttoi(strCount);
		if (nCount <= 0) return -1;

		// Insert Data to Map
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

	// 기존 등록구분에 있는데 새로운 등록구분에 없으면 TRUE 리턴
	INT nCount = arrExistRegCode.GetSize();
	for (idx = 0 ; idx < nCount; idx++)
	{
		regCode = arrExistRegCode.GetAt(idx);

		if (!IsInRegCode(pArrRegCode, regCode.m_strRegCode)) return TRUE;
	}

	// 새로운 등록구분 체크
	nCount = pArrRegCode->GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		
		// 기존 등록구분에 있으면 개수를 비교한다.
		if (IsInRegCode(&arrExistRegCode, regCode.m_strRegCode))
		{
			nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);
/*			if (regCode.m_nCount < nBookCount)
				return TRUE;
*/		}
		// 기존 등록구분에 없으면 FALSE return
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

	// 등록구분과 개수에 맞게 책 조절
	CRegCode regCode;
	INT nBookCount;
	for (INT idx = 0; idx < nRegCodeCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		nBookCount = GetBookCount(pDM_Book, regCode.m_strRegCode, strVol);

		// 삭제
		if (regCode.m_nCount < nBookCount)
		{
			RemoveBook(pDM_Book, regCode.m_strRegCode, strVol, nBookCount - regCode.m_nCount);
		}
		// 추가
		else if (regCode.m_nCount > nBookCount)
		{
			AddBook(pDM_Book, regCode.m_strRegCode, strVol, regCode.m_nCount - nBookCount);
		}
	}

	// 데이터 정리
	nDMRowCount = pDM_Book->GetRowCount();
	for (idx = nDMRowCount - 1; idx >= 0; idx--)
	{
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// RegCode 랑 비교한다. 없는 regCode면 삭제한다.
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
	}

	// 복본기호 부여
	CString strSeShelfCode, strDupCode;
	nDMRowCount = pDM_Book->GetRowCount();
	for(idx = 0 ; idx < nDMRowCount ; idx++)
	{
		// 책과 권이 아니면 넘어간다.
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
		// 복본기호가 c1이면 삭제한다. c1을 없앤다.
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
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL BB , LH_STORAGE_V01 CL ")
						_T("WHERE BB.REC_KEY = %s  AND ")
						_T("BB.REC_KEY = CL.BOOK_KEY AND CL.STATUS IN ( '0' , '2' , '3' )")
						, strRecKey);
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

	// 책 키를 가져온다.
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

	// 키가 없으면 리턴
	if (strKey.IsEmpty()) return 0;

	// 키를 물고 있는 부록을 지운다.
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT i = nRowCount - 1; i >= 0; i--)
	{
		// 부록이 아니면 넘어간다.
		if (!IsAppendix(pDM_Book, i)) continue;

		// 키가 다르면 넘어간다.
		if (pDM_Book->GetCellData(_T("BB_부모책KEY"), i) != strKey) continue;

		// 쿼리 플래그
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
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		ids = pDM_Book->GetCellData(_T("BB_권일련번호"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();
}

INT CBOFormApi::GetBookCount(CESL_DataMgr *pDM_Book, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	if (pDM_Book == NULL) return -1;

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, lpszVol)) continue;

		// 등록구분 비교
		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx) != strRegCode) continue;
		
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
		// 책과 권이 아니면 넘어간다.
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

	// 표준 책을 찾고
	INT nStandardRowIdx = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		nStandardRowIdx = idx;
		break;
	}
	
	CString strQueryFlag;
	CString strDupCode, strSeShelfCode;
	INT nNewRow = -1;
	for (idx = 0; idx < nCount; idx++)
	{
		// 복사해서 로우 생성한다.
		nNewRow = CopyDM(pDM_Book, nStandardRowIdx, pDM_Book, -1);
		if (nNewRow < 0) continue;

		// 등록구분 세팅
		pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, nNewRow);

		// 등록번호 빈값으로 세팅
		pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), nNewRow);

		// 부록 플래그 추가
		pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), nNewRow);

		// 쿼리 플래그 세팅
		strQueryFlag = GetQueryFlag(MODE_INSERT);
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nNewRow);
/*
		// 복본기호 넣기
		//INT nDupNumber = GetLastDupCodeNumber(pDM_Book, strVol);
		INT nDupNumber ;
		if(m_pCopyCodeMgr == NULL) continue;
		else
		{
			strSeShelfCode = pDM_Book->GetCellData(_T("BB_별치기호_코드"), nNewRow);
			m_pCopyCodeMgr->SetSrcInfo(strVol, strRegCode, strSeShelfCode);
			nDupNumber = GetLastDupCodeNumber(pDM_Book, nNewRow);
			if (nDupNumber < 0) continue;
			strDupCode.Format(_T("%d"), ++nDupNumber);
			pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, nNewRow);		
		}
*/
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
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		// 등록구분 비교
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
		
		// Accept List
		if (!IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
			
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

		// 거부 리스트에 있는지 확인
		// Default Ignore List
		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		// User Ignore List
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
		
		// Accept List
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

		// 거부 리스트에 있는지 확인
		// Default Ignore List
		if (IsStringInArray(&m_arrDefaultIgnoreAlias, strAlias)) continue;
		// User Ignore List
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
		// 부록이 아니면
		if (!IsAppendix(pDM_Book, idx, strVol)) continue;
		
		// 부모키 비교
		if (!strParentKey.IsEmpty())
			if (pDM_Book->GetCellData(_T("BB_부모책KEY"), idx) != strParentKey) continue;
		
		// 부록 DM에 넣는다.
		if (pDM_Appendix->InsertRow(-1) < 0) continue;
		
		nAppendixRowCount = pDM_Appendix->GetRowCount();
		CopyDM(pDM_Book, idx, pDM_Appendix, nAppendixRowCount - 1);

		// 책 rowidx를 appendix의 UDF 필드에 넣는다.
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
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;
		
		// 가격 가져오기
		ids = pDM_Book->GetCellData(_T("BB_가격"), idx, strTemp);
		if (ids < 0 || strTemp.IsEmpty()) 
		{
			nResult ++;
			continue;
		}

		// 가격 합하기
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

	// 책에 가격을 적용 시킨다.
	INT n = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		// 마지막 책이면 마지막 가격을 넣는다.
		n++;
		
		if (n == nBookCount)
			pDM_Book->SetCellData(_T("BB_가격"), strLastBookPrice, idx);
		else
			pDM_Book->SetCellData(_T("BB_가격"), strBookPrice, idx);
		// 가격사항을 변경하였으므로 DB_FLAG의 값을 _T("U")로 설정한다. 이때 이미 임의의 값(U/I/D)으로 설정되어 있다면 
		// 변경하지 않는다.
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

	strTemp.Format(_T("%d"), n++);
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		// 권차 적용
		pDM_Book->SetCellData(_T("BB_편권차"), strTemp, idx);
		strTemp.Format(_T("%d"), n++);
		
		strVolSortNo.Format(_T("%d"), (idx+1)*10);
		
		//권호일련번호 적용
		pDM_Book->SetCellData(_T("BB_권일련번호"), strVolSortNo, idx);
		
		// 복본기호 초기화
		pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);
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
	if (strISBNFrom.IsEmpty()) return -1;

	strISBNTo.Empty();

	CISBNChecker isbnChecker;
	INT ids = -1;

	CString strSet;

	// () 제거..
	INT nFind = strISBNFrom.Find('(');
	if (nFind >= 0)
	{
		strSet = strISBNFrom.Mid(nFind);
		strISBNFrom = strISBNFrom.Mid(0, nFind);
	}
	if (strISBNFrom.IsEmpty()) return -1;

	// '-' 제거
	CString strTemp = strISBNFrom;
	strISBNFrom.Empty();
	isbnChecker.RemoveDashFromISBN(strTemp.GetBuffer(0), strISBNFrom.GetBuffer(MAX_PATH));
	strISBNFrom.ReleaseBuffer();

	if (strISBNFrom.IsEmpty()) return -1;
	
/*
	// 외국 ISBN은 안함
	if (strISBNFrom.Left(2) != _T("89")) 
	{
		MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("89로 시작하는 번호를 입력하십시오!"), _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
		return -1;
	}
*/
	// 길이가 10, 9에 따라
	INT nLength = strISBNFrom.GetLength();

	INT iResult = -1;
	if (nLength == 10)
	{
		ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
		if (ids < 0) return -1;

		if (iResult == 0)
		{
			MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("올바른 ISBN이 입력되었습니다."), _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
			strISBNTo += strSet;
			return 0;
		}
		else if (iResult == 1)
		{
		}
		else if (iResult == -2)
		{
			ids = MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("ISBN이 잘못되었습니다. 수정하시겠습니까?"), _T("ISBN 체크"), MB_YESNO | MB_ICONQUESTION);
			if (ids == IDYES)
			{
				strISBNTo += strSet;
			}
			else
				return -1;
		}
	}
	else if (nLength == 9)
	{
		CString strOutISBN;
		CString strCheckDigit;
		isbnChecker.ReturnISBNCheckDigit(strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE);
		strOutISBN.ReleaseBuffer();
		strCheckDigit.ReleaseBuffer();
		strTemp = strISBNFrom.Left(2);
		strISBNTo.Format(_T("%s-%s-%s%s"), strTemp, strOutISBN, strCheckDigit, strSet);

		CString strMsg;
		strMsg.Format(_T("(%s)로 생성되었습니다."), strISBNTo);
		MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("잘못된 자리수가 입력되었습니다"), _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
		return -1;
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

		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strVol)) continue;

		// 복본기호를 가져온다.
		strTemp = pDM_Book->GetCellData(_T("BB_복본기호"), idx);
		if (strTemp.IsEmpty()) strTemp = _T("1");

		// 크기 비교 1char .. number
		//_stscanf(strTemp, _T("%c%d"), &ch, &nNumber);
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
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		nDupNo = 1;
		strVolMaster = pDM_Book->GetCellData(_T("BB_편권차"), idx);
		while (++idx < nRowCount)
		{
			//if (++idx >= nRowCount) break;
			if (!IsBook(pDM_Book, idx)) continue;

			strVolSlave = pDM_Book->GetCellData(_T("BB_편권차"), idx);
			if (strVolMaster != strVolSlave) 
			{
				idx--;
				break;
			}

			// 복본기호 추가
			strDupCode.Format(_T("%d"), ++nDupNo);
			pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, idx);

			// DB FALG 수정
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

	// 부록을 찾고 
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 부록이 아니면 넘어간다.
		if (!IsAppendix(pDM_Book, idx)) continue;

		// 등록구분 코드 가져온다.
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (strRegCode.IsEmpty()) continue;

		// 부모 책의 등록번호가 없으면 넘어간다.
		strParentBookKey = pDM_Book->GetCellData(_T("BB_부모책KEY"), idx);
		if (strParentBookKey.IsEmpty()) continue;

		ids = GetParentBookIndex(pDM_Book, strParentBookKey);
		if (ids < 0) continue;

		strParentBookRegNoMaster = pDM_Book->GetCellData(_T("BB_등록번호"), ids);
		if (strParentBookRegNoMaster.GetLength() < 3) continue;


		nRegNo = 0;

		// 부록 등록번호 추가
		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(2), ++nRegNo);
		pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

		// DB FALG 수정
		CString strQueryFlag = GetQueryFlag(MODE_UPDATE, pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

		while (++idx < nRowCount)
		{
			// 부록이 아니면 넘어간다.
			if (!IsAppendix(pDM_Book, idx)) continue;

			// 등록구분 코드 가져온다.
			strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
			if (strRegCode.IsEmpty()) continue;

			// 부모 책의 등록번호가 없으면 넘어간다.
			strParentBookKey = pDM_Book->GetCellData(_T("BB_부모책KEY"), idx);
			if (strParentBookKey.IsEmpty()) continue;

			ids = GetParentBookIndex(pDM_Book, strParentBookKey);
			if (ids < 0) continue;

			strParentBookRegNoSlave = pDM_Book->GetCellData(_T("BB_등록번호"), ids);
			if (strParentBookRegNoSlave.GetLength() < 3) continue;
			if (strParentBookRegNoMaster != strParentBookRegNoSlave) 
			{
				idx--;
				break;
			}

			// 부록 등록번호 추가
			strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(2), ++nRegNo);
			pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

			// DB FALG 수정
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
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		// Key 비교한다.
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

	// 편권차가 있으면 체크
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

inline BOOL CBOFormApi::IsAppendix(CESL_DataMgr *pDM_Book, INT idx, LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_책부록플래그"), idx) != _T("A")) return FALSE;

	if (pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D")) return FALSE;

	// 편권차가 있으면 체크
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

	// nIdx 가 -1이고 대표책이 없을경우 첫번째 책을 대표책으로 한다.

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, nIdx) < 0)
		{
			// 대표책이 없을경우
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx)) break;
			nIdx = idx;
		}		
	}
	else
	{
		// nIdx 의 데이터가 책인지 확인한다.
		if (!IsBook(pDM_Book, nIdx)) return -1;
	}

	// 대표책 IDX만 빼고 나머지는 N으로 세팅해 준다.
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

	// 복본기호
	CorrectBookDM_DupCode(pDM_Book);

	// 대표책 : -1이면 자동처리이기 때문에 수정이 된다.
	SetRepresentationBook(pDM_Book);

	// 등록번호 : ???
	// TODO

	return 0;
}

INT CBOFormApi::CorrectBookDM_DupCode(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	// 권단위로 복본기호를 적용한다.

	INT ids = -1;
	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	INT nVolCnt;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
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

		// 책과 권이 아니면 넘어간다.
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

INT CBOFormApi::ApplySpeciesPriceToVolPrice(CESL_DataMgr *pDM_Book, INT nPrice, CString &strTotalPrice)
{
	if (pDM_Book == NULL) return -1;

	INT nVolCount = GetVolumeCount(pDM_Book);
	//INT nVolCount = pDM_Book->GetRowCount();
	if (nVolCount <= 0) return -1;

	if (nPrice < 0) return -1;

	INT nVolPrice = 0;
	INT nLastVolPrice = 0;

	if (nPrice >= nVolCount) 
	{
		nVolPrice = nPrice / (nVolCount * 10);
		nVolPrice *= 10;
		nLastVolPrice = nPrice - (nVolPrice * nVolCount);
		if (nLastVolPrice == 0)
			nLastVolPrice = nVolPrice;
		else
			nLastVolPrice = nLastVolPrice + nVolPrice;
	}

	CString strVolPrice;
	CString strLastVolPrice, strVolSortNo, strTmpData;

	strVolPrice.Format(_T("%d"), nVolPrice);
	strLastVolPrice.Format(_T("%d"), nLastVolPrice);

	if (nLastVolPrice < 0) return -1;

	// 권에 가격을 적용 시킨다.
	INT nTotalPrice = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;
	strVolSortNo = pDM_Book->GetCellData(_T("BB_권일련번호"), 0);
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		strTmpData = pDM_Book->GetCellData(_T("BB_권일련번호"), idx);
		if(strVolSortNo != strTmpData)
		{
			strVolSortNo = strTmpData;
		}
		
		if (idx == nRowCount-1)
		{
			pDM_Book->SetCellData(_T("BB_가격"), strLastVolPrice, idx);
			nTotalPrice += _ttoi(strLastVolPrice.GetBuffer(0));
		}
		else
		{
			pDM_Book->SetCellData(_T("BB_가격"), strVolPrice, idx);
			nTotalPrice += _ttoi(strVolPrice.GetBuffer(0));
		}
		// 가격사항을 변경하였으므로 DB_FLAG의 값을 _T("U")로 설정한다. 이때 이미 임의의 값(U/I/D)으로 설정되어 있다면 
		// 변경하지 않는다.
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	
	strTotalPrice.Format(_T("%d"), nTotalPrice);
	return 0;
}

INT CBOFormApi::ApplyVolPrice(CESL_DataMgr *pDM_Book, INT nPrice, CString &strTotalPrice, CString &str950$b)
{

	if (pDM_Book == NULL) return -1;

	INT nVolCount = GetVolumeCount(pDM_Book);
	if (nPrice < 0) return -1;

	str950$b.Format(_T("%d"), nPrice*nVolCount);
	
	CString strVolPrice, strVolSortNo, strTmpData;

	strVolPrice.Format(_T("%d"), nPrice);
	
	// 권에 가격을 적용 시킨다.
	INT n = 0;
	INT nTotalPrice = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		pDM_Book->SetCellData(_T("BB_가격"), strVolPrice, idx);
		nTotalPrice += _ttoi(strVolPrice.GetBuffer(0));
		
		// 가격사항을 변경하였으므로 DB_FLAG의 값을 _T("U")로 설정한다. 이때 이미 임의의 값(U/I/D)으로 설정되어 있다면 
		// 변경하지 않는다.
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx).IsEmpty())
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), idx);
	}
	
	strTotalPrice.Format(_T("%d"), nTotalPrice);
	return 0;
}

INT CBOFormApi::DisplayISBN(CESL_ControlMgr *pCM, CString strAlias)
{
	CISBNChecker isbnChecker;
	CString strData, strConvData;
	pCM->GetControlMgrData(strAlias, strData);
	if(strData.IsEmpty()) return 0;
	strData.Replace(_T("-"), _T(""));
	
	TCHAR    *szExtraStrPointer;		// szUnFormattedISBN에서 '('이후의 문자열
	TCHAR    szTemp[128];			// 임시 문자열
	TCHAR    szTempISBN[128];			// 임시 ISBN
	TCHAR    szCorrectFormattedISBN[MAX_PATH];			

	INT nFirstDash = 2;
    INT nThirdDash = 11;
	INT nSecondDash;
	INT ISBNData;

	// '('이후의 문자열을 따로 보관한다.
    szExtraStrPointer = _tcschr( strData.GetBuffer(0), '(' );
	_tcscpy(szTempISBN, strData.GetBuffer(0));
    
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
	
	if ( szExtraStrPointer != NULL ) 
		_tcscpy( &szCorrectFormattedISBN[13], szExtraStrPointer );
	pCM->SetControlMgrData(strAlias, CString(szCorrectFormattedISBN));
	return 0;
}


INT CBOFormApi::SaveDefaultValue(CString strMarc, LPCTSTR lpszFileName)
{
	CMarcDefaultValueMgr mgr(m_pParnetMgr);
	mgr.SetFileName(lpszFileName);
	return mgr.SaveDefaultValue(strMarc);
/*

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::modeCreate))
		return -1;

	INT ids = -1;
	file.WriteString(strMarc);
	
	file.Close();
	return 0;
*/
}

INT CBOFormApi::LoadDefaultValue(CString &strMarc, LPCTSTR lpszFileName)
{
	CMarcDefaultValueMgr mgr(m_pParnetMgr);
	mgr.SetFileName(lpszFileName);
	return mgr.LoadDefaultValue(strMarc);
	
/*
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead))
		return -1;

	INT ids = -1;
	file.ReadString(strMarc);
	
	file.Close();
	return 0;
*/
}

INT CBOFormApi::SetVolSortNo(CCopyCodeMgr *pCopyCodeMgr, CESL_DataMgr *pDM_Book)
{
	if(pCopyCodeMgr == NULL || pDM_Book == NULL) return -1;
	INT nRowCnt = pDM_Book->GetRowCount();
	if(nRowCnt == 0) return 0;

	//먼저 권일련번호 필드를 초기화 한다.
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pDM_Book->SetCellData(_T("BB_권일련번호"), _T(""), i);
		pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), i);
	}

	//책을 반복하여 권일련번호와 복본기호를 부여한다.
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
		
		// Multi Combo Control 인 경우
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

		// 복본인지 검사한다.
		if (!m_pCopyCodeMgr->IsCopy(pDM_Book, idx)) continue;
		//자기 자신이면 검사하지 않는다.
		if(idx == nCurIdx) continue;

		// 복본기호를 가져온다.
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
	wmemset(&OFN, 0, sizeof(OPENFILENAME));
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

INT CBOFormApi::MakeBookDM(CString strQuery, CESL_DataMgr *pDM_BOOK)
{
	CESL_DataMgr * pDM = new CESL_DataMgr;
	pDM->InitDMPKFromFile(_T("DMUP_단행_종_책EBOOK정보"));
	pDM->SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	pDM->SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);
	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0)
	{
		if(pDM)
		{
			delete pDM;
			pDM = NULL;
		}
		return -1;
	}
	// 기본적으로 복사하지 말아야 할 필드를 지운다.
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
