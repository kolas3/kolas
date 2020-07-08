// BO_BusinessData.cpp: implementation of the CBO_BusinessData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_BusinessData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_BusinessData::CBO_BusinessData(CESL_Mgr * pParentMgr) : CBO_DataManager(pParentMgr)
{

}

CBO_BusinessData::~CBO_BusinessData()
{

}

INT CBO_BusinessData::DeleteUnlessWord(CString &strData)
{
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("역할어"), _T("역할어"));
	INT nFind = strData.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strData.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if (CharSetMgr_UnlessWord.Find(str1, str2) == 0)
		{
			strData.Delete(nFind, strData.GetLength() - nFind);
		}
	}
	return 0;
}

INT CBO_BusinessData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	return 0;
}

INT CBO_BusinessData::ApplyDMOneCell(CESL_DataMgr * pDM, INT nRowIdx, INT nColIdx, CString strFieldName)
{
	if(pDM == NULL || m_pDM == NULL) return -1;
	
	INT nColCnt = m_pDM->GetRefColumnCount();
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		m_pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		if(FIELD_NAME == strFieldName)
		{
			pDM->SetCellData(nRowIdx, nColIdx, m_pDM->GetCellData(0, i));
			break;
		}
	}
	return 0;
}

INT CBO_BusinessData::ApplyMarcToData(CMarc * pMarc)
{
	return 0;
}

INT CBO_BusinessData::ApplyDataToMarc(CMarc * pMarc)
{
	return 0;
}

INT CBO_BusinessData::Get_TITLE(CMarc *pMarc, CString &strData1, CString &strData2)
{
	if(pMarc == NULL || m_pParentMgr == NULL) return -1;
	
	strData1.Empty();			strData2.Empty();
	INT ids;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), strData1);
	if(ids < 0) return -1;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$x"), strData2);
	if(ids < 0) return -1;
	//부서명 처리
	CString strTmpData;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$b"), strTmpData);
	if(ids < 0) return -1;
	if(strTmpData.GetLength() > 0)
	{
		if(strData2.GetLength() > 0)
			strData2 += _T(":") + strTmpData;
		else 
			strData1 += _T(":") + strTmpData;
	}
	return 0;
}

INT CBO_BusinessData::Get_AUTHOR(CMarc *pMarc, CString &strAuthor)
{
	if (pMarc == NULL || m_pParentMgr == NULL) return -1;
	strAuthor.Empty();
	CString strMarcData1, strData, strMarcData2, strTmpData, strTmpData1;
	INT ids;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strMarcData1);
	if(ids < 0) return -1;
	strData.Format(_T("%s"), strMarcData1);
	CArray <CString, CString&> arrData;
	arrData.RemoveAll();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strMarcData2, &arrData);
	if(ids < 0) return -1;
	for (INT i = 0; i < arrData.GetSize(); i++) {
		strTmpData1 = arrData.GetAt(i);
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), strTmpData1);
		else strTmpData.Format(_T("%s;%s"), strData, strTmpData1);
		strData = strTmpData;
	}
	strAuthor = strData;
	return 0;
}

INT CBO_BusinessData::Get_PUBLISHER(CMarc *pMarc, CString &strPublisher)
{
	if (pMarc == NULL || m_pParentMgr == NULL) return -1;
	strPublisher.Empty();
	CString strMarcData, strTmpData, strData;
	CArray <CString , CString&> strMarcArrayData;
	INT ids;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$b"), strMarcData, &strMarcArrayData);
	if(ids < 0) return -1;
	for (INT i = 0; i < strMarcArrayData.GetSize(); i++) {
		if (strData.GetLength() == 0) strTmpData.Format(_T("%s"), strMarcArrayData.GetAt(i));
		else strTmpData.Format(_T("%s:%s"), strData, strMarcArrayData.GetAt(i));
		strData = strTmpData;
	}
	strPublisher = strData;
	return 0;
}

INT CBO_BusinessData::FindItemByIndicator(CMarc *pMarc, CString strTag, CString strInd, CString &strData)
{
	if(pMarc == NULL || m_pParentMgr == NULL) return -1;
	strData = _T("");
	CArray <CString, CString&> IndArray;
	CArray <CString, CString&> Array;
	
	CString strTmp, strTmpInd;
	INT ids;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, strTag, strTmp, &Array, &IndArray);
	if(ids < 0) return -1;
	INT nArrayRow = IndArray.GetSize();
	for (INT i = 0; i < nArrayRow; i++) {
		strTmpInd = IndArray.GetAt(i).Left(2);
		if (strTmpInd == strInd) {
			strData = Array.GetAt(i);
			return IndArray.GetSize();
		}
	}
	return -1;
}

INT CBO_BusinessData::GetDigitData(CString strSrc, CString &strDigit)
{
	INT nSize = strSrc.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	for (INT i = 0; i < nSize; i++) 
	{
		ch = strSrc.GetAt(i);
		if (isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		}
	}
	szBufDigit[nBufDigitSize] = '\0';
	strDigit.Format(_T("%s"), szBufDigit);
	strDigit.TrimLeft();		strDigit.TrimRight();
	
	return 0;
}

INT CBO_BusinessData::GetNoDigitData(CString strSrc, CString &strNoDigit)
{
	INT nSize = strSrc.GetLength();
	TCHAR ch;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) 
	{
		ch = strSrc.GetAt(i);
		if (!isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}
	szBufNotDigit[nBufNotDigitSize] = '\0';
	strNoDigit.Format(_T("%s"), szBufNotDigit);
	strNoDigit.TrimLeft();			strNoDigit.TrimRight();
	return 0;
}

INT CBO_BusinessData::Set_TITLE(CMarc *pMarc, CString strData)
{
	if (pMarc == NULL) return -1;
	//서명관련 245필드 클리어
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("245"));
	
	INT pos = strData.Find(_T("="), 0);
	CString strData1, strData2, strData3;
	if (pos < 0) {
		strData1 = strData;
		pos = strData.Find(_T(":"), 0);
		if(pos < 0)
		{
			pos = strData.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
		}
		else
		{
			strData1 = strData.Left(pos);
			strData2 = strData.Mid(pos+1);
			pos = strData1.Find(_T("("));
			if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			else 
			{
				pos = strData.Find(_T(")"));
				if(pos < 0) 
				{
					strData1.Replace(_T("("), _T(""));
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				}
				else
				{
					m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
					m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
				}
			}
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData2, _T(""), MODIFY_ITEM);
		}
	}
	else {
		strData1 = strData.Left(pos);
		strData2 = strData.Mid(pos+1);
		pos = strData1.Find(_T("("));
		if(pos < 0 || pos != 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
		else 
		{
			pos = strData.Find(_T(")"));
			if(pos < 0) 
			{
				strData1.Replace(_T("("), _T(""));
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
			}
			else
			{
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$a"), strData1, _T(""), MODIFY_ITEM);
				m_pParentMgr->m_pInfo->pMarcMgr->SetTagIndicator(pMarc, _T("245"), _T("2"), _T("0"));
			}
		}
		pos = strData2.Find(_T(":"), 0);
		if(pos < 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData2, _T(""), MODIFY_ITEM);
		else 
		{
			strData1 = strData2.Left(pos);
			strData3 = strData2.Mid(pos+1);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$x"), strData1, _T(""), MODIFY_ITEM);
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$b"), strData3, _T(""), MODIFY_ITEM);
		}
	}
	return 0;
}

INT CBO_BusinessData::Set_AUTHOR(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$d"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$e"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(';', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		if (i == 0) 
		{
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$d"), strData1);
		}
		else
		{
			if (strData1.GetLength() > 0) m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$e"), strData1);
		}
		if (pos == -1) break;
	}
	return 0;
}

INT CBO_BusinessData::Set_PUBLIHSER(CMarc *pMarc, CString strIndexData)
{
	if (pMarc == NULL) return -1;
	INT pos = -1;
	INT nPrevPos = 0;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("260$b"));
	CString strData1;
	for (INT i = 0; true; i++) {
		strData1 = _T("");
		pos = strIndexData.Find(':', nPrevPos);
		if (pos == -1) strData1 = strIndexData.Mid(nPrevPos);
		else strData1 = strIndexData.Mid(nPrevPos, pos - nPrevPos);
		
		nPrevPos = pos+1;
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("260$b"), strData1);
		if (pos == -1) break;
	}
	return 0;
}
