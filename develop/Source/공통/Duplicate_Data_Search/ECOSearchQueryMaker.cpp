// WorkDupSearchQueryMaker.cpp: implementation of the CWorkDupSearchQueryMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ECOSearchQueryMaker.h"
#include "..\\BO_SPECIES_API\\ISBNChecker.h"
#include "../../../공통/BO_SPECIES_API/ISBNChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECOSearchQueryMaker::CECOSearchQueryMaker(CESL_Mgr * pMgr) : CQueryMaker(pMgr)
{
	
}

CECOSearchQueryMaker::~CECOSearchQueryMaker()
{

}

INT CECOSearchQueryMaker::GetSearchQuery(CString &strQuery, CESL_DataMgr * pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	if(!CheckValid()) return -1;
	if(pKeyDM == NULL) return -1;
	if(nIdx < 0) return -1;
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	CString fieldNames[10] = {
//		_T("본표제"), _T("저작자"), _T("발행자"), _T("발행년"), _T("낱권ISBN"), _T("매체구분"), 
//		_T("권서명"), _T("편권차"), _T("별치기호"), _T("등록구분")
//	};
	CString fieldNames[11] = {
		_T("본표제"), _T("총서명"), _T("저작자"), _T("발행자"), _T("발행년"), _T("낱권ISBN"), _T("매체구분"), 
		_T("권서명"), _T("편권차"), _T("별치기호"), _T("등록구분")
	};
	//===================================================
	INT ids = -1;
	
	m_ArrQuery.RemoveAll();
	
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	for(INT i=0 ; i<10 ; i++)
//	{
//		if(i < 8)
//		{
	for(INT i=0 ; i<11 ; i++)
	{
		if(i < 9)
		{
	//===================================================
			ids = MakeQuery(fieldNames[i], pKeyDM, nIdx, pBind );
			if(ids < 0) return -1;
		}
		else if(m_nDupSearchType == HOLD_DUP_SEARCH)
		{
			ids = MakeQuery(fieldNames[i], pKeyDM, nIdx, pBind );
			if(ids < 0) return -1;
		}
	}
	ids = GetQuery(strQuery, pKeyDM);
	if(ids < 0) return -1;
	return 0;
}

INT CECOSearchQueryMaker::GetTitleSearchQuery(CString &strQuery, CESL_DataMgr * pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	m_ArrQuery.RemoveAll();

	CString strArticle;
	CString strTitle = pKeyDM->GetCellData(_T("IBS_본표제"), nIdx);
	GetArticleData(strTitle, strArticle);
	
	//키워드 생성
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTitle);
	//한자한글 변환
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strTitle);
	if(!strArticle.IsEmpty())
	{
		//키워드 생성
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strArticle);
		//한자한글 변환
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strArticle);
	}
	CString strOption;
	
	if(!strArticle.IsEmpty())
		strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*|%s*') "), strTitle, strArticle+strTitle);
	else
		strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*') "), strTitle);
	
	m_ArrQuery.Add(strOption);
	
	INT ids = GetQuery(strQuery, pKeyDM, TRUE);
	if(ids < 0) return -1;
	
	return 0;
}

CString CECOSearchQueryMaker::GetTableName()
{	
	CString strTblName = m_strTableName;
	// 테이블명에 들어가야 함.
	CString strTmpData = m_ArrQuery.GetAt(0);
	
	//===================================================
	//2009.12.01 UPDATE BY PJW : 서명에 'IN'이 들어가 있을경우를 대비해서 비교를 해준다.
//	INT nPos = strTmpData.Find(_T("IN"));
//	CString strTmpData1 = strTmpData.Mid(nPos-1);
	CString strTmpData1;
	INT nPos1 = strTmpData.Find(_T("IN"));
	INT nPos2 = strTmpData.Find(_T("ECOSEARCH"));
	if( nPos1 > nPos2 && 0 < nPos2 )
	{
		strTmpData1 = strTmpData;
	}
	else
	{
		strTmpData1 = strTmpData.Mid(nPos1-1);
	}
	//===================================================
	if ( m_bOpenbase )
	{
		CString strOpenbase;
		//strTmpData1.Replace(_T("I.REC_KEY"), _T("SPECIES_KEY"));
		strTmpData1.Replace(_T("WHERE"), _T(""));
		strTmpData1.Replace(_T("AND"), _T(""));
		strOpenbase.Format(_T("SPECIES_KEY IN ( %s "), strTmpData1 );		
		strTblName.Replace(_T("%WHERE%"), _T(" WHERE ") + strOpenbase);
		strTblName.Replace(_T("%AND%"), _T(" AND ") + strOpenbase);
	}
	else
	{
		strTblName.Replace(_T("%WHERE%"), _T(" WHERE SPECIES_KEY ") + strTmpData1);
		strTblName.Replace(_T("%AND%"), _T(" AND SPECIES_KEY ") + strTmpData1);
	}
	return strTblName;
}

INT CECOSearchQueryMaker::MakeQuery(CString strFieldName, CESL_DataMgr *pKeyDM, INT nIdx, EDBM_BIND_DATA_TYPE *pBind)
{
	INT searchType, searchQueryType;
	searchType = searchQueryType = -1;
	BOOL articleSearchYN, useYN ;
	articleSearchYN = useYN = FALSE;
	CString articleData, tmpcondition;
	articleData = tmpcondition = _T("");

	CString searchPlace, searchField, tmpsrcstr, DMAlias, CMAlias;
	searchPlace = searchField = _T("");
	INT ids = m_SearchKey.GetFieldKeySettingInfo(strFieldName, &useYN, &searchType, &articleSearchYN);
	if(ids < 0) return -1000;
	if(!useYN) return 0;
	ids = m_SearchKey.GetDMCMAlias(strFieldName, DMAlias, CMAlias);
	if(ids < 0) return -1000;
	ids = pKeyDM->GetCellData(DMAlias, nIdx, tmpsrcstr);
	if(tmpsrcstr.GetLength() == 0) return 0;
	ids = m_SearchKey.GetSearchFieldKeySettingInfo(strFieldName, searchPlace, searchField);
	if(ids < 0) return -1000;

	if(searchPlace == _T("IDX_BO_TBL")) searchPlace = _T("I");
	else searchPlace = _T("B");
	searchQueryType = GetSearchType(searchField);
	
	ChangeECOSEARCHFieldName(searchField);
	
	if(articleSearchYN && searchType != 2)
		ids = GetArticleData(tmpsrcstr, articleData);

	// 2009.11.13 ADD BY PDJ : 역할어 제거

	
	if(searchType == 0)
	{
		if(searchQueryType == 0 || searchQueryType == 1)
		{
			//키워드 생성
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(tmpsrcstr);
			//한자한글 변환
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(tmpsrcstr);
			if(!articleData.IsEmpty())
			{
				//키워드 생성
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(articleData);
				//한자한글 변환
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(articleData);
			}
			if(tmpsrcstr.IsEmpty()) return 0;
		}
		
		if(searchQueryType == 0)
		{
			if(articleSearchYN && articleData.GetLength() != 0)
				tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s*|%s*') "), searchField, tmpsrcstr, articleData+tmpsrcstr);
			else
			{
				tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s*') "), searchField, tmpsrcstr);
				/*
				tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, :%s*) "), searchField, searchField);
				CString strBindVar;
				strBindVar.Format(_T(":%s"), searchField );
				AddEDBMBindDataByString(pBind, STRING , strBindVar.GetBuffer(0) , tmpsrcstr.GetBuffer(0) );
				*/
			}
		}
		else if(searchQueryType == 1)
			tmpcondition.Format(_T(" AND %s='%s'"), searchPlace+_T(".")+searchField, tmpsrcstr);
		else
		{
			CString strTmpQuery;	strTmpQuery.Empty();
			CStringArray arrData;
			CString strTmpData;
			ParsingString(_T(" "), tmpsrcstr, &arrData, TRUE);
			INT cnt = arrData.GetSize();
			tmpsrcstr.Empty();
			for(INT i=0 ; i<cnt ; i++)
			{
				if(tmpsrcstr.IsEmpty()) tmpsrcstr = arrData.GetAt(i) + _T("*");
				else tmpsrcstr += _T("|") + arrData.GetAt(i) + _T("*");
				if(i==10) break;
			}
			tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s*') "), searchField, tmpsrcstr);
			/*
			tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, :%s*) "), searchField, searchField);
			CString strBindVar;
			strBindVar.Format(_T(":%s"), searchField );
			AddEDBMBindDataByString(pBind, STRING , strBindVar.GetBuffer(0) , tmpsrcstr.GetBuffer(0) );
			*/

		}
		m_ArrQuery.Add(tmpcondition);
	}
	else if(searchType == 1)
	{
		if(searchQueryType == 0 || searchQueryType == 1)
		{
			//키워드 생성
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(tmpsrcstr);
			//한자한글 변환
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(tmpsrcstr);
			if(!articleData.IsEmpty())
			{
				//키워드 생성
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(articleData);
				//한자한글 변환
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(articleData);
			}
			if(tmpsrcstr.IsEmpty()) return 0;
		}
		
		if(searchQueryType == 0)
		{
			if(articleSearchYN && articleData.GetLength() != 0)
				tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s|%s') "), searchField, tmpsrcstr, articleData+tmpsrcstr);
			else
				tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s') "), searchField, tmpsrcstr);
		}
		else if(searchQueryType == 1)
			tmpcondition.Format(_T(" AND %s='%s'"), searchPlace+_T(".")+searchField, tmpsrcstr);
		else
		{
			CString strTmpQuery;	strTmpQuery.Empty();
			CStringArray arrData;
			CString strTmpData;
			ParsingString(_T(" "), tmpsrcstr, &arrData, TRUE);
			INT cnt = arrData.GetSize();
			tmpsrcstr.Empty();
			
			for(INT i=0 ; i<cnt ; i++)
			{
				if( searchField == _T("BO_ST_CODE") )
				{

					CISBNChecker isbnChecker;
					CString sPair = _T("");
					CString strISBNFrom, strOutISBN, strCheckDigit, strISBNTo, strTemp;
					CString sISBN = arrData.GetAt(i);
					INT nLength = sISBN.GetLength();
					if( nLength == 10 )
					{
						sISBN = _T("978") + sISBN;
						sISBN = sISBN.Left(12);
					}

					else if( nLength == 13 )
					{
						sISBN = sISBN.Mid(3);
						sISBN = sISBN.Left(9);
					}
					
					nLength = sISBN.GetLength();
			
					switch( nLength )
					{
					
					case 9:
					{
						strTemp = sISBN.Left(2);
						isbnChecker.ReturnISBNCheckDigit( sISBN.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
						strOutISBN.ReleaseBuffer();
						strCheckDigit.ReleaseBuffer();			
						strOutISBN.Replace(_T("-"), _T(""));
						strISBNTo.Format(_T("%s%s%s"), strTemp, strOutISBN, strCheckDigit);
						
						//strMsg.Format(_T("(%s)로 생성되었습니다."), strISBNTo);
						//MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION);
					}
					break;
					case 12:
					{			
						isbnChecker.ReturnISBNCheckDigit( sISBN.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
						strOutISBN.ReleaseBuffer();
						strOutISBN.Replace(_T("-"),_T(""));
						strCheckDigit.ReleaseBuffer();
						strTemp = sISBN.Left(3);
						CString strTemp2 = sISBN.Mid(3,2);
						strISBNTo.Format(_T("%s%s%s%s"), strTemp, strTemp2, strOutISBN, strCheckDigit);
						
						//strMsg.Format(_T("(%s)로 생성되었습니다."), strISBNTo);
						//MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONINFORMATION);
					}
					break;

					default:
						{
						//	strMsg = _T("잘못된 자리수가 입력되었습니다");
						//	MessageBox(m_pParnetMgr->GetSafeHwnd(), strMsg, _T("ISBN 체크"), MB_OK | MB_ICONWARNING);
							;
						}
						break;
					}

					sPair = strISBNTo;

					if(tmpsrcstr.IsEmpty())
					{
						tmpsrcstr = arrData.GetAt(i);
						if( sPair.GetLength() > 0 )
							tmpsrcstr += _T("|") + sPair;
	
					}		
					else
					{
						tmpsrcstr += _T("|") + arrData.GetAt(i);
							if( sPair.GetLength() > 0 )
							tmpsrcstr += _T("|") + sPair;
					}
				}
				else
				{
					if(tmpsrcstr.IsEmpty())
						tmpsrcstr = arrData.GetAt(i);
					else tmpsrcstr += _T("|") + arrData.GetAt(i);
				}
				
			}
			tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s') "), searchField, tmpsrcstr);
		}
		m_ArrQuery.Add(tmpcondition);
	}
	else if(searchType == 2)
	{
		//영문표제일 경우
		DelArticleFromStr(tmpsrcstr, tmpsrcstr);
		WordHeaderCuttingData(tmpsrcstr, tmpsrcstr);
		tmpcondition.Format(_T(" AND &ECOSEARCH(I.REC_KEY, %s, '%s') "), searchField, tmpsrcstr);
		m_ArrQuery.Add(tmpcondition);
	}
	return 0;
}

INT CECOSearchQueryMaker::GetQuery(CString &strQuery, CESL_DataMgr *pKeyDM, BOOL bTitleSearch/*=FALSE*/)
{
	INT nCnt = m_ArrQuery.GetSize();
	if(nCnt == 0) return -1;
	if(pKeyDM == NULL) return -1;
	
	INT ids, nPos;
	
	CString strTmpData, strTmpQuery;
	//ECOSEARCH가 아닌 쿼리는 솎아내야 한다.
	for(INT i=nCnt-1 ; i>=0 ; i--)
	{
		strTmpData = m_ArrQuery.GetAt(i);
		nPos = strTmpData.Find(_T("ECOSEARCH"));
		if(nPos < 0)
		{
			m_ArrQuery.RemoveAt(i);
			strQuery += strTmpData;
		}
	}

	nCnt = m_ArrQuery.GetSize();

	CString strTemp;
	if ( pKeyDM->m_bOpenbase ) 
	{
		for(i=0 ; i<nCnt ; i++)
		{
			if ( strQuery.IsEmpty() )
			{
				strQuery = m_ArrQuery.GetAt(i);
			}
			else
			{
				strTemp = m_ArrQuery.GetAt(i);
				strTemp.Replace(_T("AND"), _T("INTERSECT"));
				strQuery += strTemp;
			}
		}
	}
	else
	{
		for(i=0 ; i<nCnt ; i++)
		{
			strTmpData = m_ArrQuery.GetAt(i);
			
			if(i == 0)
			{
				if(!bTitleSearch && nCnt-1 > i)
					strTmpData.Replace(_T("I.REC_KEY"), _T("ECO_KEY"));
				pKeyDM->GetECOSEARCHReplace(strTmpData);
				strTmpData.TrimRight();
			}
			else if(i > 0 && nCnt-1 > i)
			{			
				strTmpData.Replace(_T("I.REC_KEY"), _T("ECO_KEY"));
				pKeyDM->GetECOSEARCHReplace(strTmpData);
				strTmpData.TrimRight();
				strTmpData += _T("&");
				strTmpData.Replace(_T(")&"), _T(" ##)"));
			}
			else if(nCnt-1 == i)
			{
				pKeyDM->GetECOSEARCHReplace(strTmpData);
				//FROM 과 Where 절 사이에 있는 테이블 정보에 , dual을 Join한다.
				ids = GetDualJoinTableReplace(strTmpData);
				if(ids < 0) return -1;
				strTmpData.TrimRight();
				strTmpData += _T("&");
				strTmpData.Replace(_T(")&"), _T(" ##)"));
			}
			m_ArrQuery.SetAt(i, strTmpData);
		}
		
		
		strTmpQuery.Empty();
		for(i=nCnt-1 ; i>=0 ; i--)
		{
			if(strTmpQuery.IsEmpty())
				strTmpQuery = m_ArrQuery.GetAt(i);
			else
			{
				strTmpData = m_ArrQuery.GetAt(i);
				strTmpQuery.Replace(_T("##)"), strTmpData+_T(")"));
			}
		}
	}
	
	strQuery += strTmpQuery;	
	return 0;
}

INT CECOSearchQueryMaker::ChangeECOSEARCHFieldName(CString &strSearchField)
{
	if(strSearchField == _T("IDX_TITLE")) strSearchField = _T("BO_IDX_TITLE");
	else if(strSearchField == _T("IDX_AUTHOR")) strSearchField = _T("BO_IDX_AUTHOR");
	else if(strSearchField == _T("IDX_PUBLISHER")) strSearchField = _T("BO_IDX_PUBLISHER");
	else if(strSearchField == _T("ST_CODE")) strSearchField = _T("BO_ST_CODE");
	else if(strSearchField == _T("IDX_VOL")) strSearchField = _T("BO_IDX_VOL");
	else if(strSearchField == _T("IDX_VOL_TITLE")) strSearchField = _T("BO_IDX_VOL_TITLE");
	else if(strSearchField == _T("IDX_SE_SHELF_CODE")) strSearchField = _T("BO_IDX_SE_SHELF_CODE");
	else if(strSearchField == _T("REG_CODE")) strSearchField = _T("BO_REG_CODE");
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	else if(strSearchField == _T("IDX_ALL_ITEM")) strSearchField = _T("BO_IDX_ALLITEM");
	//===================================================
	return 0;
}

INT CECOSearchQueryMaker::GetDualJoinTableReplace(CString &strSrc)
{
	if(strSrc.IsEmpty()) return 0;

	INT nFromIdx = strSrc.Find(_T("FROM"), 0);
	INT nWhereIdx = strSrc.Find(_T("WHERE"), 0);
	
	if(nFromIdx < 0 || nWhereIdx < 0) return -1;

	nFromIdx = nFromIdx + 4;
	if(nFromIdx > nWhereIdx) return -1;

	CString strTmpData, strReplaceStr;
	strReplaceStr = strSrc.Mid(nFromIdx, (nWhereIdx)-nFromIdx);
	strTmpData = strReplaceStr + _T(", DUAL ");
	
	strSrc.Replace(strReplaceStr, strTmpData);
	return 0;
}
