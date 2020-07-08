// BO_CatApi1.cpp: implementation of the CCatApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatApi.h"

#include "CommonDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatApi::CCatApi(CESL_Mgr *pParentMgr)
{
	m_pParnetMgr = pParentMgr;

	m_pMarcMgr = NULL;

	m_LastSpeciesKey = _T("");

	if (m_pParnetMgr == NULL) return;

	m_pIndexTool = (CIndexTool*)pParentMgr->m_pInfo->pIndexTool;

	m_pDataMgr = NULL;

}

CCatApi::~CCatApi()
{

}

//////////////////////////////////////////////////////////////////////
// CCatApi Operations

INT CCatApi::Init()
{
	EFS_BEGIN

	m_pMarcMgr = m_pParnetMgr->m_pInfo->pMarcMgr;

	InitMemberData();

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::LoadRegCode()
{
	EFS_BEGIN

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

	EFS_END
	return -1;
}

INT CCatApi::FetchRegCode(CString strRegCodeField, 
						  CArray<CCatRegCode, 
						  CCatRegCode> *pArrRegCode)
{
	EFS_BEGIN

	if(pArrRegCode == NULL) return -1;
	if (strRegCodeField.IsEmpty()) return -1;

	strRegCodeField.TrimLeft();
	strRegCodeField.TrimRight();

	// '/'로 나눈다.
	CMap<CString, LPCTSTR, INT, INT> mapRegCode;
	CString strRegCode;
	CString strCount;
	CString strItemData;
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
			strItemData  = strRegCodeField;
		}
		else
		{		
			strItemData = strRegCodeField.Mid(0, nFind);
		}
		

		if (strItemData.GetLength() < nRegCodeLength) return -1;
		strRegCode = strItemData.Left(nRegCodeLength);

		if (!IsValidRegCode(strRegCode)) return -1;

		if (strItemData.GetLength() == nRegCodeLength)
			strCount = _T("1");
		else
			strCount = strItemData.Mid(nRegCodeLength);

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
	
	CCatRegCode regCode;
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

BOOL CCatApi::IsValidRegCode(CString strRegCode)
{
	EFS_BEGIN

	if (m_pParnetMgr == NULL) return FALSE;
	if (strRegCode.IsEmpty()) return FALSE;

	return IsStringInArray(&m_arrRegCode, strRegCode);


	EFS_END
	return FALSE;
}

BOOL CCatApi::IsRegCodeModify(CArray<CCatRegCode, 
							  CCatRegCode> *pArrRegCode, 
							  CESL_DataMgr *pDM_Book, 
							  CString strVol, 
							  CString strToAliasHeader /*= _T("")*/)
{
	EFS_BEGIN

	if (pArrRegCode == NULL || pDM_Book == NULL) return FALSE;

	INT ids = -1;
	CArray<CCatRegCode, CCatRegCode> arrExistRegCode;
	ids = GetRegCodeInfo(pDM_Book, &arrExistRegCode, _T("BB"), strVol);

	INT idx = 0;
	INT nBookCount = 0;
	INT nRegCodeCount = 0;
	CCatRegCode regCode;

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
			nBookCount = GetBookCount(pDM_Book, strToAliasHeader, regCode.m_strRegCode, strVol);
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

INT CCatApi::MakeBookFromRegCode(CArray<CCatRegCode, CCatRegCode> *pArrRegCode, CESL_DataMgr *pDM_Book, CString strVol)
{
	return 0;
}

BOOL CCatApi::IsInRegCode(CArray<CCatRegCode, 
						  CCatRegCode> *pArrRegCode, 
						  CString strRegCode)
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

INT CCatApi::DeleteVolume(CESL_DataMgr *pDM_Book, CString strVol)
{
	return 0;
}

CString CCatApi::GetBookKey(CESL_DataMgr *pDM_Book, 
							INT nIdx, 
							CString strToAliasHeader /*= _T("")*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return _T("");
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	CString strKey = pDM_Book->GetCellData(strToAliasHeader + _T("_책KEY"), nIdx);
	if (strKey.IsEmpty())
		strKey = pDM_Book->GetCellData(strToAliasHeader + _T("_UNIQUE_KEY"), nIdx);

	return strKey;

	EFS_END
	return _T("");
}

INT CCatApi::DeleteBookDMRow(CESL_DataMgr *pDM_Book, INT idx, CString strQueryFlag /*=_T("")*/)
{
	return 0;
}

INT CCatApi::GetVolumeCount(CESL_DataMgr *pDM_Book, 
							CString strToAliasHeader /*= _T("")*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strVol;
	CMap<CString, LPCTSTR, INT, INT> mapVolume;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		ids = pDM_Book->GetCellData( strToAliasHeader + _T("_편권차"), idx, strVol);
		
		mapVolume.SetAt(strVol, 1);
	}

	return mapVolume.GetCount();

	EFS_END
	return -1;
}

INT CCatApi::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CString strRegCode /*= _T("")*/, 
						  LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( strToAliasHeader + _T("_등록구분_코드"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

INT CCatApi::GetRegCodeInfo(CESL_DataMgr *pDM_Book, 
							CArray<CCatRegCode, 
							CCatRegCode> *pArrRegCode, 
							CString strToAliasHeader /*= _T("")*/, 
							LPCTSTR lpszVol /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pArrRegCode == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	CMap<CString, LPCTSTR, INT, INT> mapRegCode;

	INT ids = -1;
	CString strRegCode;
	INT nRegCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		ids = pDM_Book->GetCellData(strToAliasHeader + _T("_등록구분_코드"), idx, strRegCode);
		if (ids < 0) continue;

		if (strRegCode.IsEmpty()) continue;
		if (!IsValidRegCode(strRegCode)) continue;

		if (mapRegCode.Lookup(strRegCode, nRegCount))
			mapRegCode.SetAt(strRegCode, ++nRegCount);
		else
			mapRegCode.SetAt(strRegCode, 1);
	}

	CCatRegCode regCode;
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

INT CCatApi::RemoveBook(CESL_DataMgr *pDM_Book, CString strRegCode, CString strVol, INT nCount /*= 1*/)
{
	return 0;
}

INT CCatApi::CopyDM(CESL_DataMgr *pDM_From, 
					INT nFromIdx, 
					CESL_DataMgr *pDM_To, 
					INT nToIdx, 
					CString strToAliasHeader /*= _T("")*/, 
					CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_From == NULL || pDM_To == NULL) return -1;

	INT ids = -1;
	INT nFind = -1;

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

	CString strAlias;
	CString strItemData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		ids = pDM_From->GetCellData(strAlias, nFromIdx, strItemData);
		if (ids < 0) continue;

		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		if ( strAlias == _T("IBS_배가위치부호") ) continue;
		if ( strAlias == _T("BS_배가위치부호") ) continue;
		if ( strAlias == _T("BB_등록구분_코드") ) continue;
		if ( strAlias == _T("IBS_분류표구분") || strAlias == _T("BS_분류표구분") )
		{
			ids = pDM_To->SetCellData(_T("IBS_분류표구분_코드"), strItemData, nToIdx);
			ids = pDM_To->SetCellData(_T("BS_분류표구분_코드"), strItemData, nToIdx);
			ids = pDM_To->SetCellData(_T("IBS_분류표구분"), strItemData, nToIdx);
			ids = pDM_To->SetCellData(_T("BS_분류표구분"), strItemData, nToIdx);
		}
		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}

INT CCatApi::MakeDMFromCM(CESL_ControlMgr *pCM, 
						  CESL_DataMgr *pDM, 
						  INT nAppMode /*= -1*/,
						  INT nIdx /*= -1*/, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;
	INT nFind = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	CString strAlias;
	CString strItemData;
	CESL_Control_Element *pCE = NULL;
	
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		if (!reinterpret_cast<CWnd*>(pCM->FindControl(strAlias))->IsWindowEnabled()) continue;

		ids = pCM->GetControlMgrData(strAlias, strItemData);
		if (ids < 0) continue;

		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;

		if (pDM->GetCellData(strAlias, nIdx) != strItemData)
		{
			ids = pDM->SetCellData(strAlias, strItemData, nIdx);
			ids = pDM->SetCellData(strToAliasHeader + _T("_DB_FLAG"), GetQueryFlag(MNG_MODIFY, nAppMode, pDM->GetCellData(strToAliasHeader + _T("_DB_FLAG"), nIdx)), nIdx);
		}
		if (ids < 0) continue;
	}
	return 0;

	EFS_END
	return -1;
}

BOOL CCatApi::IsStringInArray(CStringArray *pArr, 
							  CString str)
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

BOOL CCatApi::IsStringInArray(CArray<CString, CString&> *pArr, 
							  CString str)
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

CString CCatApi::GetQueryFlag(INT nWorkMode, 
							  INT nAppMode,
							  CString strQueryFlag /*= _T("")*/)
{
	EFS_BEGIN

	if (strQueryFlag.IsEmpty())
	{
		if (nWorkMode == MNG_INSERT ) return _T("I");
		else if (nWorkMode == MNG_MODIFY )
		{
			if (nAppMode == MNG_INSERT || nAppMode == MNG_IMPORT ) return _T("I");
			else if (nAppMode == MNG_MODIFY )	return _T("U");
		}
	}
	else
	{
		return strQueryFlag;
	}
	
	return _T("");

	EFS_END
	return _T("");
}

INT CCatApi::GetBookPriceSum(CESL_DataMgr *pDM_Book, INT &nSum)
{
	return 0;
}

CString CCatApi::GetWorkingStatusDesc(CString strWorkingStatusCode)
{
	EFS_BEGIN

	CString strDesc;

	if (m_mapWorkingStatusCodeDesc.Lookup(strWorkingStatusCode, strDesc))
		return strDesc;
	else
		return _T("");

	return _T("");

	EFS_END
	return _T("");
}

INT CCatApi::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, CString strVol)
{
	return 0;
}

INT CCatApi::ApplyAppendixRegNo(CESL_DataMgr *pDM_Appendix)
{
	return 0;
}

BOOL CCatApi::IsBook(CESL_DataMgr *pDM_Book, 
					 INT idx, 
					 CString strToAliasHeader /*= _T("")*/, 
					 LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_책부록플래그"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

BOOL CCatApi::IsAppendix(CESL_DataMgr *pDM_Book, 
						 INT idx, 
						 CString strToAliasHeader /*= _T("")*/, 
						 LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_책부록플래그"), idx) != _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

INT CCatApi::ConvertBookToVolume(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume)
{
	return 0;
}

INT CCatApi::GetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT &nIdx)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		if (pDM_Book->GetCellData( strToAliasHeader + _T("_대표책여부"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;

	EFS_END
	return -1;
}

INT CCatApi::SetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT nIdx /*= -1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, strToAliasHeader, nIdx) < 0)
		{
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx, strToAliasHeader)) break;
			nIdx = idx;
		}		
	}
	else
	{
		if (!IsBook(pDM_Book, nIdx, strToAliasHeader)) return -1;
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("Y"), idx);
		else
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("N"), idx);
	}
	
	return nIdx;

	EFS_END
	return -1;
}

INT CCatApi::UpdateRepresentVolume(CESL_DataMgr *pDM_Book, CString strAlias, CString strItemData, INT nIdx/*=-1*/)
{
	return 0;
}

INT CCatApi::ApplyVolumeUpdate(CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Book, INT nVolumeIdx, CString strAlias, CString strItemData)
{
	return 0;
}

INT CCatApi::ConvertMarcToCatDM(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	return 0;
}

INT CCatApi::ConvertCatDMToMarc(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	return 0;
}

INT CCatApi::MakeTemp049Marc(CMarc *pSourceMarc, 
							 CMarc *pOutputMarc, 
							 CString &strFirstInd, 
							 CString &strDefaultShelfLocCode)
{
	EFS_BEGIN

	if (!pSourceMarc || !pOutputMarc) return -1;

	INT ids = -1;

	CTag *p049Tag = pSourceMarc->FindTag(_T("049"));
	if (p049Tag == NULL) return -1;

	CTag *pNewTag = NULL;
	CSubfield *pSubfield = NULL;

	POSITION pos = NULL;
	POSITION pos2 = NULL;

	CString strSubfieldCode;

	strFirstInd = p049Tag->GetFirstIndicator();

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapTempSubData;
	mapTempSubData.RemoveAll();

	pos = p049Tag->m_listSubfield.GetHeadPosition();
	while (pos)
	{
		pSubfield = reinterpret_cast<CSubfield*>(p049Tag->m_listSubfield.GetNext(pos));
		if (pSubfield == NULL) continue;
		
		strSubfieldCode = pSubfield->GetSubfieldCode();
		strSubfieldCode.Replace(SUBFIELD_CODE, '$');

		if(strSubfieldCode == _T("$l"))
		{
			pos2 = pos;
			pNewTag = new CTag();
			pOutputMarc->AddTag(pNewTag);
			pNewTag->SetTagCode(_T("049"));
			pNewTag->AddSubfield(pSubfield->Clone());
			
			if (pos2 == NULL) continue;

			pSubfield = reinterpret_cast<CSubfield*>(p049Tag->m_listSubfield.GetNext(pos2));
			while (pSubfield)
			{
				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$l")) break;

                if (strSubfieldCode == _T("$f"))
				{
					strDefaultShelfLocCode = pSubfield->GetItemData();
					mapTempSubData.SetAt(pSubfield->GetItemData(),pSubfield->GetItemData());
				}

				pNewTag->AddSubfield(pSubfield->Clone());

				if (pos2)
					pSubfield = reinterpret_cast<CSubfield*>(p049Tag->m_listSubfield.GetNext(pos2));
				else 
					pSubfield = NULL;
			}
		}
	}
    INT nMapCount = mapTempSubData.GetCount();
	if(nMapCount > 1 || strFirstInd == _T("1") )
    {
		p049Tag->SetFirstIndicator(_T("1"));
		strFirstInd = _T("1");
    }
	else
	{
        p049Tag->SetFirstIndicator(_T("0"));
		strFirstInd = _T("0");
	}

	return 0;

	EFS_END
	return -1;
}

BOOL CCatApi::IsRegNoInArray(CArray<CString, CString&> *pArr, 
							 CString str)
{
	if (pArr == NULL) return FALSE;

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	if (str.GetLength() <= nRegCodeLength) return FALSE;

	CString strRegNo1;
	CString strRegNo2;
	CString strTemp;

	INT nCount = pArr->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strTemp = pArr->GetAt(idx);
		if (strTemp.GetLength() <= nRegCodeLength) continue;
		
		strRegNo1 = m_pParnetMgr->m_pInfo->MakeRegNo(strTemp.Left(nRegCodeLength), strTemp.Mid(nRegCodeLength));
		strRegNo2 = m_pParnetMgr->m_pInfo->MakeRegNo(str.Left(nRegCodeLength), str.Mid(nRegCodeLength));
		if (strRegNo1 == strRegNo2) return TRUE;
	}

	return FALSE;

}

INT CCatApi::MergeDM(CESL_DataMgr *pDM_Source, CESL_DataMgr *pDM_Output, LPCTSTR lpszKeyAlias, INT nMode /*= 0*/)
{
	return 0;
}

INT CCatApi::Convert049ToBook(CMarc *pMarc, CESL_DataMgr *pDM_Book, BOOL bNewVolSortNo)
{
	return 0;
}

INT CCatApi::GetBookIdxByRegNo(CESL_DataMgr *pDM_Book, 
							   CString strRegNo, 
							   INT &nIdx, 
							   CString strToAliasHeader /*= _T("")*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	if (strRegNo.GetLength() <= nRegCodeLength) return -1;

	CString strRegCode = strRegNo.Left(nRegCodeLength);
	CString strRegNoTemp = strRegNo.Mid(nRegCodeLength);

	strRegNo= m_pParnetMgr->m_pInfo->MakeRegNo(strRegCode, strRegNoTemp);
	
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		CString sRegNo = pDM_Book->GetCellData( strToAliasHeader + _T("_등록번호"), idx);
		if ( sRegNo != _T("") && sRegNo.GetLength() < 12 ) {
			if ( sRegNo.GetLength() > nRegCodeLength ) 
			{
				sRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(sRegNo.Left(nRegCodeLength), sRegNo.Mid(nRegCodeLength));
			}
		}

		if (sRegNo == strRegNo)
		{
			nIdx = idx;
			return 0;
		}
	}
	
	return -1;

	EFS_END
	return -1;
}

INT CCatApi::DeleteRoleWord(CString &strSrc)
{
	EFS_BEGIN

	CESL_CharSetMgr *pCharSetMgr_RoleWord = NULL ;
	if (pCharSetMgr_RoleWord == NULL)
	{
		pCharSetMgr_RoleWord = new CESL_CharSetMgr;
		pCharSetMgr_RoleWord->InitData(_T("역할어"), _T("역할어"));
	}

	INT nFind = strSrc.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strSrc.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if (pCharSetMgr_RoleWord->Find(str1, str2) == 0)
		{
			strSrc.Delete(nFind, strSrc.GetLength() - nFind);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::DeleteSpecies(CStringArray *pArrStrSpeciesKey)
{
	return 0;
}

INT CCatApi::ApplyLastModifyData(CMarc *pMarc)
{
	EFS_BEGIN

	if (!pMarc) return -1;

	CString strLibCode;
	CString strItemData;

	INT ids = GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	
	if (ids < 0 || strLibCode.IsEmpty())
	{
		m_strLastErrMsg.Format(_T("도서관 부호가 없습니다. 수정해주세요"));
		return -1;
	}

	ids = m_pMarcMgr->GetItem(pMarc, _T("040$a"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pMarcMgr->SetItem(pMarc, _T("040$a"), strLibCode);
		m_pMarcMgr->SetItem(pMarc, _T("040$c"), strLibCode);
	}
	else if (strItemData != strLibCode)
	{
		m_pMarcMgr->DeleteItem(pMarc, _T("040$d"), strLibCode);
		m_pMarcMgr->SetItem(pMarc, _T("040$d"), strLibCode);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::ApplyDefaultValue(CESL_DataMgr *pDM_Species, 
							   CESL_DataMgr *pDM_Index, 
							   CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL 
		) return -1;

	INT ids = -1;

	ids = ApplyDefaultValue_Species(pDM_Species, pDM_Book);
	if (ids < 0) return ids;
	ids = ApplyDefaultValue_Index(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;
	ids = ApplyDefaultValue_Book(pDM_Book, pDM_Index);
	if (ids < 0) return ids;

	return 0;

	EFS_END
	return -1;
}

CString CCatApi::GetToday()
{
	EFS_BEGIN

	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;

	return _T("");

	EFS_END
	return _T("");
}

INT CCatApi::GetSpeciesUseLimitCode(CESL_DataMgr * pDM_Book, 
									CString &strUSELIMIT_CODE, 
									CString strToAliasHeader /*= _T("")*/, 
									INT mode /*=-1*/, 
									CString strPrev_USELIMIT_CODE /*=_T("")*/)
{
	EFS_BEGIN

	strUSELIMIT_CODE.Empty();
	INT rowCnt = pDM_Book->GetRowCount();
	CString strTmpData, strDBFlag;
	INT nPriority = -1;
	INT nMaxPriority = -1;
	if(mode > 0 && strPrev_USELIMIT_CODE.GetLength() > 0)
	{
		if(strPrev_USELIMIT_CODE == _T("GM")) nMaxPriority = 50;
		else if(strPrev_USELIMIT_CODE == _T("IZ")) nMaxPriority = 40;
		else if(strPrev_USELIMIT_CODE == _T("CA")) nMaxPriority = 30;
		else if(strPrev_USELIMIT_CODE == _T("CD")) nMaxPriority = 20;
		else if(strPrev_USELIMIT_CODE == _T("CL")) nMaxPriority = 10;
	}

	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_Book->GetCellData(_T(""), i, strTmpData);
		if(strTmpData.IsEmpty()) continue;
		strTmpData.TrimLeft();	strTmpData.TrimRight();		strTmpData.MakeUpper();
		if(strTmpData == _T("GM")) nPriority = 50;
		else if(strTmpData == _T("IZ")) nPriority = 40;
		else if(strTmpData == _T("CA")) nPriority = 30;
		else if(strTmpData == _T("CD")) nPriority = 20;
		else if(strTmpData == _T("CL")) nPriority = 10;
		if(nMaxPriority < nPriority) nMaxPriority = nPriority;
	}
	if(nMaxPriority < 0) return 0;
	else if(nMaxPriority == 50) strUSELIMIT_CODE = _T("GM");
	else if(nMaxPriority == 40) strUSELIMIT_CODE = _T("IZ");
	else if(nMaxPriority == 30) strUSELIMIT_CODE = _T("CA");
	else if(nMaxPriority == 20) strUSELIMIT_CODE = _T("CD");
	else if(nMaxPriority == 10) strUSELIMIT_CODE = _T("CL");

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::LogToFile(CString str)
{
	EFS_BEGIN

	FILE *fDebug = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fDebug == NULL) return 0;
	fclose(fDebug);

	FILE * f = _tfopen(_T("..\\tunning\\_BO_SPECIES_API.log"), _T("a+b"));
	if (!f) return -1;
	fseek(f, 0, SEEK_END);		
	if( 0 == ftell(f) )
	{
		/*UNCHANGE*/fputc(0xFF, f);
		/*UNCHANGE*/fputc(0xFE, f);
	}
	_ftprintf(f, _T("%s\r\n"), str);
	fclose(f);
	return 0;

	EFS_END
	return -1;
}

INT CCatApi::GetLibInfo(CString ArrFieldAttr[], 
						CString ArrResult[], 
						INT nFieldCount)
{
	if (!ArrFieldAttr || !ArrResult || nFieldCount < 1) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);

	CString strFieldAttr;
	for (INT idx = 0; idx < nFieldCount; idx++)
	{
		if (ArrFieldAttr[idx].IsEmpty()) continue;

		strFieldAttr += ArrFieldAttr[idx];
		if (idx != nFieldCount - 1)
			strFieldAttr += _T(", ");
	}

	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"), strFieldAttr );
	INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
	if (ids < 0) return -1;

	return 0;
}

INT CCatApi::MakeIndexArray(CStringArray *pArrStr, 
							LPCTSTR strSrcIndexField, 
							LPCTSTR lpszDelimit /*= _T(" ")*/)
{
	EFS_BEGIN

	CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strIndexItem;	
	
	LPTSTR lpszToken = _tcstok(const_cast<LPTSTR>(strSrcIndexField), lpszDelimit);
	while (lpszToken)
	{
		strIndexItem.Format(_T("%s"), lpszToken);
		pIndex->MakeIndex(strIndexItem);
		if (!IsStringInArray(pArrStr, strIndexItem))
		{
			pArrStr->Add(strIndexItem);
		}

		lpszToken = _tcstok(NULL, lpszDelimit);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::MakeIndexClassifyCodeConcatenationSQL(CESL_DataMgr * pIDX_DM, 
												   CESL_DataMgr * pBOOK_DM, 
												   CStringArray * Index_classfycodes, 
												   CStringArray * Book_classfycodes, 
												   INT cnt, 
												   CString &Index_Update_sql, 
												   CString strRecKey, 
												   CString strToAliasHeader /*= _T("")*/, 
												   INT mode /*=-1*/ )
{
	EFS_BEGIN

	CStringArray Index_field;
	CStringArray Index_value;
	INT nCol = -1;
	INT ids = -1;
	CString field_name, field_alias, field_type, init_value, data_type, value, pre_value;
	Index_Update_sql.Format(_T("UPDATE %s SET "), pIDX_DM->TBL_NAME);
	CString sub_sql = _T("");

	INT rowCnt = pBOOK_DM->GetRowCount();

	for(INT i=0 ; i<cnt ; i++)
	{
		nCol = pIDX_DM->FindColumn(Index_classfycodes->GetAt(i));
		if(nCol < 0) continue;
		pIDX_DM->GetColumnRefInfo(nCol,  field_alias, field_name, field_type, init_value, data_type);

		CStringArray codeValue;
		CString code, strDBFlag;
		for(INT j=0 ; j<rowCnt ; j++)
		{
			pBOOK_DM->GetCellData( strToAliasHeader + _T("_DB_FLAG"), j, strDBFlag);
			if(strDBFlag == _T("D")) continue;
			pBOOK_DM->GetCellData( Book_classfycodes->GetAt(i), j, code);
			if(!IsStringInArray(&codeValue, code))
				codeValue.Add(code);
		}
		value.Empty();
		for(j=0 ; j<codeValue.GetSize() ; j++)
		{
			if(!value.IsEmpty()) value += _T(" ");
			value += codeValue.GetAt(j);
		}

		if(mode > 0)
		{
			pIDX_DM->GetOneValueQuery(_T("SELECT ") + field_name + _T(" FROM ") + pIDX_DM->TBL_NAME + _T(" WHERE REC_KEY = ") + strRecKey, pre_value);
			if(!pre_value.IsEmpty()) MergeClassifyCodes(pre_value, value);
		}

		if(sub_sql.IsEmpty()) sub_sql = field_name + _T(" = '") + value + _T("'");
		else sub_sql += _T(", ") + field_name + _T(" = '") + value + _T("'");

	}
	Index_Update_sql += sub_sql + _T(" WHERE REC_KEY = ") + strRecKey + _T(";");
	
	return 0;


	EFS_END
	return -1;
}

INT CCatApi::MergeClassifyCodes(CString pre_value, 
								CString &value)
{
	EFS_BEGIN

	CStringArray codeValue;
	INT pos;
	CString code;
	while(!pre_value.IsEmpty())
	{
		pos = pre_value.Find(_T(" "), 0);
		if(pos < 0)
		{
			codeValue.Add(pre_value);
			break;
		}
		code = pre_value.Mid(0, pos);
		pre_value = pre_value.Mid(pos+1);
		codeValue.Add(code);
	}
	while(!value.IsEmpty())
	{
		pos = value.Find(_T(" "), 0);
		if(pos < 0)
		{
			if(!IsStringInArray(&codeValue, value))
				codeValue.Add(value);
			break;
		}
		code = value.Mid(0, pos);
		value = value.Mid(pos+1);
		if(!IsStringInArray(&codeValue, code))
			codeValue.Add(code);
	}
	value.Empty();
	for(INT i=0 ; i<codeValue.GetSize() ; i++)
	{
		if(!value.IsEmpty()) value += _T(" ");
		value += codeValue.GetAt(i);
	}
	return 0;

	EFS_END
	return -1;
}

INT CCatApi::ImportSpecies(CESL_DataMgr *pDM_Species, 
						   CESL_DataMgr *pDM_Index, 
						   CESL_DataMgr *pDM_Book,
						   CESL_DataMgr *pDM_Volume, 
						   CESL_DataMgr *pDM_Appendix /*= NULL*/, 
						   CStringArray *pArrStrAddFrame /*= NULL*/, 
						   LPCTSTR lpszShelfDate /*= NULL*/,
						   INT nAppMode
						   )
{
	return 0;
}

INT CCatApi::WebImportSpecies(CESL_DataMgr *pDM_Species, 
						   CESL_DataMgr *pDM_Index, 
						   CESL_DataMgr *pDM_Book,
						   CESL_DataMgr *pDM_Volume, 
						   CESL_DataMgr *pDM_Appendix /*= NULL*/, 
						   CStringArray *pArrStrAddFrame /*= NULL*/, 
						   LPCTSTR lpszShelfDate /*= NULL*/,
						   INT nAppMode
						   )
{
	return 0;
}

INT CCatApi::DMDPImportSpecies(CESL_DataMgr *pDM_Species, 
						   CESL_DataMgr *pDM_Index, 
						   CESL_DataMgr *pDM_Book,
						   CESL_DataMgr *pDM_Volume, 
						   CESL_DataMgr *pDM_Appendix /*= NULL*/, 
						   CStringArray *pArrStrAddFrame /*= NULL*/, 
						   LPCTSTR lpszShelfDate /*= NULL*/,
						   INT nAppMode
						   )
{
	return 0;
}

INT CCatApi::InsertToDropAccessionNoTbl(CStringArray *pArrStrRegNo, 
										CESL_DataMgr *pDM_Out, 
										BOOL bCommit)
{
	EFS_BEGIN

	if (pArrStrRegNo == NULL) return -1;

	INT ids = -1;

	INT nRowCount = pArrStrRegNo->GetSize();

	if (bCommit && nRowCount>0) { 
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	CString strRegNo;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strRegNo = pArrStrRegNo->GetAt(idx);
		if (strRegNo.IsEmpty()) continue;

		InsertToDropAccessionNoTbl(strRegNo, pDM_Out, bCommit);
	}

	if (bCommit && nRowCount>0) {	
		ids = pDM_Out->SendFrame();
		if (ids < 0) 
		{
			return ids;
		}
		ids = pDM_Out->EndFrame();
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::InsertToDropAccessionNoTbl(CString strRegNo, 
										CESL_DataMgr * pDM_Out, 
										BOOL IsCheckInsert)
{
	EFS_BEGIN

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));

	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(	_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL ")
						_T("WHERE REG_CODE='%s' AND REG_NO=%d AND MANAGE_CODE=UDF_MANAGE_CODE")
						, strCode, nNumber	);

		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	INT ids = pDM_Out->MakeRecKey(strRecKey);
	if (ids < 0) return ids;

	query.Format(	_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
					_T("(REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
					_T("VALUES(%s, 'MO', '%s', %d, '%s', '%s',UDF_MANAGE_CODE);")
					, strRecKey, strCode, nNumber
					, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)
					, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG) );

	ids = pDM_Out->AddFrame(query);
	if (ids < 0) return ids;
	return 0;

	EFS_END
	return -1;
}

INT CCatApi::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT_SPECIES, 
									   CString strTBL, 
									   CString strSpeciesKey, 
									   INT RowIdx, 
									   BOOL bCommit /*= TRUE*/)
{
	return 0;
}

CString CCatApi::GetPriceCode(CString strPrice, 
							  CString &strRealPrice)
{
	EFS_BEGIN

	CString strResult;

	INT nStartPrice = -1;

	TCHAR ch = 0;
	INT nLength = strPrice.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strPrice.GetAt(idx);
		if (_istdigit(ch))
		{
			nStartPrice = idx;
			break;
		}
		
		strResult += ch;
	}

	if (nStartPrice >= 0)
		strRealPrice = strPrice.Mid(nStartPrice);

	return strResult;

	return _T("");

	EFS_END
	return _T("");
}

INT CCatApi::LoadClassCodeVolumeDlg(LPCTSTR lpszFileName, 
									CString &str056, CString &str082, CString &str085)
{
	EFS_BEGIN

	if (lpszFileName == NULL) return -1;

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(lpszFileName, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
			return -1;
		else
		{
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		}
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	INT ids = -1;
	CString strValue;
	CString strAlias;
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == '#' || strLine.Left(1) == '.') continue;
		
		ids = _stscanf(strLine, _T("%s\t=\t%s"), strAlias.GetBuffer(256), strValue.GetBuffer(256));
		if (ids == _TEOF) continue;

		strAlias.ReleaseBuffer();
		
		if (strAlias == _T("056TAG"))
			str056 = strValue;
		else if (strAlias == _T("082TAG"))
			str082 = strValue;
		else if (strAlias == _T("085TAG"))
			str085 = strValue;

	}

	file.Close();

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::SaveClassCodeVolumeDlg(LPCTSTR lpszFileName, 
									CString n056, 
									CString n082,
									CString n085)
{
	EFS_BEGIN

	if (lpszFileName == NULL) return -1;

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) return -1;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strLine;
	strLine.Format(_T(".복복조사/분류 에서 각 분류기호( 056Tag, 082Tag, 085Tag LC ) 판사항($2)를 보관하는 파일.\r\n"));
	file.WriteString(strLine);

	strLine.Format(_T("056TAG\t=\t%s\r\n"), n056);
	file.WriteString(strLine);
	strLine.Format(_T("082TAG\t=\t%s\r\n"), n082);
	file.WriteString(strLine);
	strLine.Format(_T("085TAG\t=\t%s\r\n"), n085);
	file.WriteString(strLine);

	file.Close();

	return 0;

	EFS_END
	return -1;
}

INT CCatApi::ApplyShelfCodeData(CESL_DataMgr *pDM_Book, CMarc *pMarc, BOOL bCatHold /*=FALSE*/)
{
	return 0;
}

INT CCatApi::SyncVolumeBookAppendixDM(CESL_DataMgr *pDM_Volume
					   , CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_HoldBook
					   , CESL_DataMgr *pDM_Appendix, CESL_DataMgr *pDM_HoldAppendix, INT nAppMode)
{
	return 0;
}


CString CCatApi::GetCommonMatInfo(CString strKey, 
								  CString strFormCode)
{
	CString strReturn;
	if (strFormCode.IsEmpty()) return strReturn;

	if (strFormCode == _T("BK"))
	{
		CMap<CString, LPCTSTR, CString, LPCTSTR> mapCodeToCommonMat_TX;

		mapCodeToCommonMat_TX.SetAt(_T("ha"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hb"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hc"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hd"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("he"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hf"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hg"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_TX.SetAt(_T("hz"), _T("마이크로 형태자료"));

		mapCodeToCommonMat_TX.SetAt(_T("zm"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("zu"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("zz"), _T(""));

		mapCodeToCommonMat_TX.SetAt(_T("ta"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("tb"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("tc"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("td"), _T(""));
		mapCodeToCommonMat_TX.SetAt(_T("tz"), _T(""));

		mapCodeToCommonMat_TX.Lookup(strKey, strReturn);
		return strReturn;
	}
	else
	{
		CMap<CString, LPCTSTR, CString, LPCTSTR> mapCodeToCommonMat_NonBook;

		mapCodeToCommonMat_NonBook.SetAt(_T("ag"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("aj"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ak"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ar"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("as"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ay"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("az"), _T("지도"));
		mapCodeToCommonMat_NonBook.SetAt(_T("aq"), _T("모형"));

		mapCodeToCommonMat_NonBook.SetAt(_T("da"), _T("구체자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("db"), _T("구체자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("dc"), _T("구체자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("dz"), _T("구체자료"));

		mapCodeToCommonMat_NonBook.SetAt(_T("gc"), _T("필름스트립"));
		mapCodeToCommonMat_NonBook.SetAt(_T("gd"), _T("필름스트립"));
		mapCodeToCommonMat_NonBook.SetAt(_T("gf"), _T("필름스트립"));
		mapCodeToCommonMat_NonBook.SetAt(_T("go"), _T("필름스트립"));
		mapCodeToCommonMat_NonBook.SetAt(_T("gs"), _T("슬라이드"));
		mapCodeToCommonMat_NonBook.SetAt(_T("gt"), _T("트랜스페어런시"));
		mapCodeToCommonMat_NonBook.SetAt(_T("gz"), _T(""));

		mapCodeToCommonMat_NonBook.SetAt(_T("ha"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hb"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hc"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hd"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("he"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hf"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hg"), _T("마이크로 형태자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("hz"), _T("마이크로 형태자료"));

		mapCodeToCommonMat_NonBook.SetAt(_T("sd"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("se"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("sg"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("si"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("sq"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ss"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("st"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("sw"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("sz"), _T("녹음자료"));

		mapCodeToCommonMat_NonBook.SetAt(_T("kc"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("kd"), _T("도면"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ke"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("kf"), _T("사진"));
		mapCodeToCommonMat_NonBook.SetAt(_T("kg"), _T("사진"));
		mapCodeToCommonMat_NonBook.SetAt(_T("kh"), _T("사진"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ki"), _T("녹음자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("kj"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("kl"), _T("도면"));
		mapCodeToCommonMat_NonBook.SetAt(_T("kn"), _T("차트"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ko"), _T("플래시 카드"));
		mapCodeToCommonMat_NonBook.SetAt(_T("kz"), _T(""));

		mapCodeToCommonMat_NonBook.SetAt(_T("mc"), _T("영화"));
		mapCodeToCommonMat_NonBook.SetAt(_T("mf"), _T("영화"));
		mapCodeToCommonMat_NonBook.SetAt(_T("mr"), _T("영화"));
		mapCodeToCommonMat_NonBook.SetAt(_T("mz"), _T("영화"));

		mapCodeToCommonMat_NonBook.SetAt(_T("vc"), _T("비디오 녹화자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("vd"), _T("비디오 녹화자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("vf"), _T("비디오 녹화자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("vr"), _T("비디오 녹화자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("vz"), _T("비디오 녹화자료"));

		mapCodeToCommonMat_NonBook.SetAt(_T("zm"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("zu"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("zz"), _T(""));

		mapCodeToCommonMat_NonBook.SetAt(_T("ca"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cb"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cc"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cf"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("ch"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cj"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cm"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("co"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cr"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cu"), _T("전자자료"));
		mapCodeToCommonMat_NonBook.SetAt(_T("cz"), _T("전자자료"));

		mapCodeToCommonMat_NonBook.SetAt(_T("ta"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("tb"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("tc"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("td"), _T(""));
		mapCodeToCommonMat_NonBook.SetAt(_T("tz"), _T(""));

		mapCodeToCommonMat_NonBook.Lookup(strKey, strReturn);
		return strReturn;
	}
}

INT CCatApi::IsNumberCopyCode( CString strCode )
{
	if ( strCode.IsEmpty() ) return -1;

	CString strTmp=_T("");
	for(INT i=0; i<strCode.GetLength(); i++)
	{
		strTmp = strCode.GetAt(i);
		if ( strTmp<_T("0") || strTmp>_T("9")) return -1;
	}

	return 0;
}
