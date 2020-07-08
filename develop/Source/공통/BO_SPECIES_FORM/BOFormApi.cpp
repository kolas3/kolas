// BOFormApi.cpp: implementation of the CBOFormApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BOFormApi.h"

//#include _T("..\K2UP_Common\ISBNChecker.h")
#include "..\BO_SPECIES_API\ISBNChecker.h"

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
	CString strArrCode[6];
	for (INT i = 0; i < nCodeCount; i++) {
		nStartRow = m_pParnetMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), strArrCode, 6, nStartRow);

		m_arrRegCode.Add(strArrCode[2]);
		
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
		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
// 		if (strData.GetLength() < 2) return -1;
// 		strRegCode = strData.Left(2);
		if (strData.GetLength() < nRegCodeLength) return -1;
		strRegCode = strData.Left(nRegCodeLength);
		//===================================================================

		// 등록된 등록구분인지 체크
		if (!IsValidRegCode(strRegCode)) return -1;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if (strData.GetLength() == 2)
// 			strCount = _T("1");
// 		else
// 			strCount = strData.Mid(2);
		if (strData.GetLength() == nRegCodeLength)
			strCount = _T("1");
		else
			strCount = strData.Mid(nRegCodeLength);
		//===================================================================

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
			if (regCode.m_nCount < nBookCount)
				return TRUE;
		}
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

		// 복본기호가 c1이면 삭제한다. c1을 없앤다.
		if (pDM_Book->GetCellData(_T("BB_복본기호"), idx) == _T("c1"))
			pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// RegCode 랑 비교한다. 없는 regCode면 삭제한다.
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
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

		ids = pDM_Book->GetCellData(_T("BB_편권차"), idx, strVol);
		
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
	CString strDupCode;
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

		// 복본기호 넣기
		INT nDupNumber = GetLastDupCodeNumber(pDM_Book, strVol);
		if (nDupNumber < 0) continue;
		strDupCode.Format(_T("c%d"), ++nDupNumber);
		pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, nNewRow);		
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
/*		//책의 저자에는 245$d만 들어감
		if(strAlias == _T("BB_저작자"))
		{
			startpos = strData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData.Mid(0, strData.Find(_T(";")));
		}
		//대표책을 색인에 반영시에는 245$d만 대치함
		else if(strAlias == _T("IBS_저작자"))
		{
			pDM_To->GetCellData(_T("IBS_저작자"), nToIdx, strTmpData);
			startpos = strTmpData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData + strTmpData.Mid(strTmpData.Find(_T(";")));
		}
*/		ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;
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
/*		//책의 저자에는 245$d만 들어감
		if(strAlias == _T("BB_저작자") && bSwitchHeader)
		{
			startpos = strData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData.Mid(0, strData.Find(_T(";")));
		}
		//대표책을 색인에 반영시에는 245$d만 대치함
		else if(strAlias == _T("IBS_저작자") && bSwitchHeader)
		{
			pDM->GetCellData(_T("IBS_저작자"), nIdx, strTmpData);
			startpos = strTmpData.Find(_T(";"), 0);
			if(startpos > 0) strData = strData + strTmpData.Mid(strTmpData.Find(_T(";")));
		}				
		else if(strAlias == _T("IBS_저작자") && !bSwitchHeader)
		{
			//CM에 입력한 저작자 사항의 역할어를 삭제하여 색인DM에 설정한다.
			DeleteUnlessWord
		}
		*/
		//만약 설정하고자 하는 DM cell에 이미 값이 있다면 설정하지 않음
//		if(pDM->GetCellData(strAlias, nIdx).IsEmpty())
//		{
			ids = pDM->SetCellData(strAlias, strData, nIdx);
			if (ids < 0) continue;
//		}
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
	CString strTemp;

	strTemp.Format(_T("%d"), n++);
	strVol = strTemp;
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx)) continue;

		// 권차 적용
		pDM_Book->SetCellData(_T("BB_편권차"), strTemp, idx);
		strTemp.Format(_T("%d"), n++);

		// 복본기호 초기화
		pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);
	}

	return 0;
}

CString CBOFormApi::ConvertWORKING_STATUSToCode(CString strDesc)
{
	CString result = _T("");
	
	if (strDesc == _T("구입대상자료")) result = _T("BOA111N");
	if (strDesc == _T("검수대상자료")) result = _T("BOA113N");
	if (strDesc == _T("기증대상자료")) result = _T("BOA131N");
	if (strDesc == _T("편목대상자료")) result = _T("BOC113N");
    if (strDesc == _T("검수완료자료")) result = _T("BOA141N");

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
	

	// 외국 ISBN은 안함
	if (strISBNFrom.Left(2) != _T("89")) 
	{
		MessageBox(m_pParnetMgr->GetSafeHwnd(), _T("89로 시작하는 번호를 입력하십시오!"), _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
		return -1;
	}

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

		strISBNTo.Format(_T("89-%s-%s%s"), strOutISBN, strCheckDigit, strSet);

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
		if (strTemp.IsEmpty()) continue;

		// 크기 비교 1char .. number
		_stscanf(strTemp, _T("%c%d"), &ch, &nNumber);
				
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
			strDupCode.Format(_T("c%d"), ++nDupNo);
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

		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if (strParentBookRegNoMaster.GetLength() < 3) continue;
		INT nRegCodeLength;
		nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

		strParentBookRegNoMaster = pDM_Book->GetCellData(_T("BB_등록번호"), ids);
		if (strParentBookRegNoMaster.GetLength() <= nRegCodeLength) continue;
		//===================================================================


		nRegNo = 0;

		// 부록 등록번호 추가
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(2), ++nRegNo);
		strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
		//===================================================================

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
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			if (strParentBookRegNoSlave.GetLength() < 3) continue;
			if (strParentBookRegNoSlave.GetLength() <= nRegCodeLength) continue;
			//===================================================================
			if (strParentBookRegNoMaster != strParentBookRegNoSlave) 
			{
				idx--;
				break;
			}

			// 부록 등록번호 추가
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
			// strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
			strRegNo.Format(_T("%s%s_%d"), strRegCode, strParentBookRegNoMaster.Mid(nRegCodeLength), ++nRegNo);
			//===================================================================
			pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

			// DB FALG 수정			
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
		}
	}

	return 0;
}