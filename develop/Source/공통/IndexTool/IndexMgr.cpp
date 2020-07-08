// IndexMgr.cpp: implementation of the CIndexMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexMgr::CIndexMgr()
{
	m_pMarc = NULL;
	m_pMarcMgr = NULL;
}

CIndexMgr::~CIndexMgr()
{
	FreeAllData();
}

INT CIndexMgr::LoadReferenceFromFile()
{
	LoadIndexReference(CFG_FILE_PATH_MONO,&mapMono);
	LoadIndexReference(CFG_FILE_PATH_SERIAL,&mapSerial);
	LoadIndexReference(CFG_FILE_PATH_NONBOOK,&mapNonBook);

	LoadDefaultReference(CFG_FILE_PATH_DEFAULT_MONO,&m_arrayUseDeaultMono);
	LoadDefaultReference(CFG_FILE_PATH_DEFAULT_SERIAL,&m_arrayUseDeaultSerial);
	LoadDefaultReference(CFG_FILE_PATH_DEFAULT_NONBOOK,&m_arrayUseDeaultNonBook);

	return 0;
}

INT CIndexMgr::LoadIndexReference(LPCTSTR lpszFileName,CMapStringToOb* pMap)
{
	
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	CString strLine;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	if(nSuccess)
	{
		CString strTag;
		CString strTagType;
		CString strSubfield;
		CString strIndicator;
		CString strIndicatorType;
		CString strIndicatorOption;
		CString strTmp;
		INT nPos = -1;

		structIndex* index = NULL;
		structIndexTag* tag = NULL;
		structIndexSubfield* subfield = NULL;
		structIndexIndicator* indicator = NULL;

		
		while(file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			if(strLine.IsEmpty()) continue;
			
			
			if(strLine.GetAt(0) == '=')
			{
			
				strLine = strLine.Mid(1);
				strLine.TrimLeft();
				strLine.TrimRight();
				if(strLine.IsEmpty())
				{	
					AfxMessageBox(_T("ERROR!!! IndexMgr: 색인 알리아스가 없는 색인이 존재합니다!"));
					return -1;
				}
				
				index = new structIndex;
				if(!index) return -1;
				
				pMap->SetAt(strLine,(CObject*)index); 
			
			}
			else if(strLine.GetAt(0) == '[')
			{
				strLine.Remove('[');
				strLine.Remove(']');
				if(strLine.IsEmpty()) return -1;
				
				nPos = strLine.Find(_T("|"));
				if(nPos>0)
				{
					strTag = strLine.Left(nPos);
					strTagType = strLine.Mid(nPos+1);
				}
				else
				{
					strTag = strLine;
					strTagType = _T("");
				}
				
				tag = new structIndexTag;
				if(!tag) return -1;
				tag->strTag = strTag;
				tag->strType = strTagType;

				if(index) index->IndexTagList.AddTail((CObject*)tag);

			}
			else if(strLine.GetAt(0) == '<')
			{
				strLine.Remove('<');
				strLine.Remove('>');
				if(strLine.IsEmpty()) return -1;
				
				strSubfield = strLine;

				subfield = new structIndexSubfield;
				if(!subfield) return -1;
				subfield->strSubfield = strSubfield;
				
				if(tag) tag->IndexSubfieldList.AddTail((CObject*)subfield);

			}
			else
			{
					
				nPos = strLine.Find(_T("|"));
				
				if(nPos>0)
				{
					strIndicator = strLine.Left(nPos);

					strTmp = strLine.Mid(nPos+1);

					nPos = strTmp.Find(_T("|"));

					if(nPos>0)
					{
						strIndicatorType = strTmp.Left(nPos);
						strIndicatorOption = strLine.Mid(nPos+1);
					}
					else
					{
						strIndicatorType = strTmp;
						strIndicatorOption = DEFAULT_INDICATOR_SINGLE_OPTION;
					}

				}
				else
				{
					strIndicator = strLine;
					strIndicatorType = DEFAULT_INDICATOR_TYPE;
					strIndicatorOption = DEFAULT_INDICATOR_SINGLE_OPTION;
				}


				indicator = new structIndexIndicator;
				if(!indicator) return -1;
				indicator->strIndicator = strIndicator;
				indicator->strType = strIndicatorType;
				indicator->strSingleOption = DEFAULT_INDICATOR_SINGLE_OPTION;

				if(subfield) subfield->IndexIndicatorList.AddTail((CObject*)indicator);

			}

		}
		

		file.Close();
	}

	return 0;
}

INT CIndexMgr::LoadDefaultReference(LPCTSTR lpszFileName,CStringArray* pArray)
{
	CStdioFile file;
	CString strLine;
	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	if(nSuccess)
	{
		while(file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			if(strLine.IsEmpty()) continue;
		
			pArray->Add(strLine);
		}
	}

	return 0;
}

VOID CIndexMgr::FreeAllData()
{
	CString strKey;

	CObject* pObject = NULL;
	structIndex* index = NULL;
	structIndexTag* tag = NULL;
	structIndexSubfield* subfield = NULL;
	structIndexIndicator* indicator = NULL;

	
	POSITION pos = mapMono.GetStartPosition();
	POSITION posTag;
	POSITION posSubfield;
	POSITION posIndicator;

	while(pos)
	{
		mapMono.GetNextAssoc(pos, strKey, pObject);
		index = (structIndex*)pObject;
		
		posTag = index->IndexTagList.GetHeadPosition();
		
		while(posTag)
		{
			pObject = index->IndexTagList.GetNext(posTag);
			tag = (structIndexTag*)pObject;
			
			posSubfield = tag->IndexSubfieldList.GetHeadPosition();

			while(posSubfield) 
			{
				pObject = tag->IndexSubfieldList.GetNext(posSubfield);
				subfield = (structIndexSubfield*)pObject;
				
				posIndicator = subfield->IndexIndicatorList.GetHeadPosition();

				while(posIndicator)
				{
					pObject = subfield->IndexIndicatorList.GetNext(posIndicator);
					indicator = (structIndexIndicator*)pObject;
					if(indicator) 
					{
						delete indicator;
						indicator = NULL;
					}
				}

				if(subfield)
				{
					subfield->IndexIndicatorList.RemoveAll();
					delete subfield;
					subfield = NULL;
				}
			}
				
			if(tag)
			{
				tag->IndexSubfieldList.RemoveAll();
				delete tag;
				tag = NULL;
			}

		}
		
		if(index)
		{
			index->IndexTagList.RemoveAll();
			delete index;
			index = NULL;

		}
	}
	
	mapMono.RemoveAll();


}

INT CIndexMgr::Init(CMarcMgr *pMarcMgr, CMarc *pMarc)
{
	if (pMarcMgr == NULL) return -1;
	if (pMarc == NULL) return -1;
	
	INT ids = LoadReferenceFromFile();
	if(ids<0) return -1;

	m_pMarc = pMarc;
	m_pMarcMgr = pMarcMgr;
	m_pMarcMgr->SortMarc(m_pMarc);

	m_MonoIndex.SetMarcMgr(pMarcMgr);
	m_MonoIndex.SetMarc(pMarc);
	
	m_NonBookIndex.SetMarcMgr(pMarcMgr);
	m_NonBookIndex.SetMarc(pMarc);

	m_SerialIndex.SetMarcMgr(pMarcMgr);
	m_SerialIndex.SetMarc(pMarc);

	return 0;
}


INT CIndexMgr::GetIndex(CString strIndexAlias, CString &strIndexItem)
{	
	INT ids = -1;

	strIndexItem.Empty();

	///////////////////////////////////////////////////////////////////
	
	CString strMarcGroup;
	strMarcGroup = m_pMarcMgr->GetMarcGroup();
	
	CObject* pObject = NULL;
	structIndex* pIndex = NULL;

	if(CheckUseDeault(strIndexAlias,strMarcGroup))
	{
		if (_T("단행") == strMarcGroup)
			ids = m_MonoIndex.GetIndex(strIndexAlias, strIndexItem);
		else if (strMarcGroup.Find(_T("비도서")) >= 0)
			ids = m_NonBookIndex.GetIndex(strIndexAlias, strIndexItem);
		else if (_T("연속") == strMarcGroup)
			ids = m_SerialIndex.GetIndex(strIndexAlias, strIndexItem);
		else 
			return -1;

	}
	else 
	{
	
		ids = mapMono.Lookup(strIndexAlias,pObject);
	
		if(ids<0) return ids;
		pIndex = (structIndex*)pObject;

		ids = ExtractIndex(pIndex,strIndexItem,strMarcGroup);
	}

	if (ids < 0)
		return ids;

	m_Index.TrimAll(strIndexItem);

	if (ids == INDEX_SEARCH)
	{
		strIndexItem.MakeUpper();
		m_Index.RemoveSpecialChar(strIndexItem);
		m_Index.ConvertHanjaToHangul(strIndexItem);
	}
	else if (ids == INDEX_OUTPUT)
	{
			
	}

	return 0;
}

INT CIndexMgr::ExtractIndex(structIndex* pIndex,CString &strIndexItem,CString strMarcGroup)
{
	
	CArray<CString, CString&> arrayItemData;
	CObject* pObject = NULL;
	structIndexTag* pIndexTag = NULL;
	structIndexSubfield* pIndexSub = NULL;
	structIndexIndicator* pIndexInd = NULL;
	POSITION posTag;
	POSITION posSubfield;
	POSITION posIndexSub;
	POSITION posIndexInd;
	CString strTag;
	CString strSubfieldCode;
	CString strTagType;
	CString strTmpIndexItem;
	CString strInd,strFirstInd,strSecondInd;
	CString strToken,strTokenCode;
	BOOL bCheck = FALSE;
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapCheckDup;
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapSubfield;

	INT ids = -1;

	POSITION pos = pIndex->IndexTagList.GetHeadPosition();
	
	while(pos)
	{
		pObject = pIndex->IndexTagList.GetNext(pos);
		pIndexTag = (structIndexTag*)pObject;
		if(!pIndexTag) continue;

		strTagType = pIndexTag->strType;
		
		if(_T("U") == strTagType) continue;
		if(_T("G") == strTagType)
		{
			CString strNoSubfieldTag;
			strNoSubfieldTag.Format(_T("%s*"),pIndexTag->strTag);

			ids = m_pMarcMgr->GetItem(m_pMarc,strNoSubfieldTag,strIndexItem);
			return ids;
		}

		posTag = m_pMarc->m_listTag.GetHeadPosition();
		while (posTag)
		{
			CTag *pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			
			strTag = pTag->GetTagCode();

			if(pIndexTag->strTag == strTag)
			{
				strFirstInd = pTag->GetFirstIndicator();
				strSecondInd = pTag->GetSecondIndicator();
				strInd = strFirstInd + strSecondInd;
				strInd.Replace(_T(" "),_T("_"));
				
				posIndexSub = pIndexTag->IndexSubfieldList.GetHeadPosition();
				while(posIndexSub)
				{
					pObject = pIndexTag->IndexSubfieldList.GetNext(posIndexSub);
					pIndexSub = (structIndexSubfield*)pObject;
					if(!pIndexSub) continue;
					
					posSubfield = pTag->m_listSubfield.GetHeadPosition();
					while (posSubfield)
					{
						CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
						if (pSubfield == NULL) continue;
										
						strSubfieldCode = pSubfield->GetSubfieldCode();
						strSubfieldCode.Replace(SUBFIELD_CODE, '$');
						if(pIndexSub->strSubfield == strSubfieldCode)
						{
							posIndexInd = pIndexSub->IndexIndicatorList.GetHeadPosition();
							
							bCheck = FALSE;

							while(posIndexInd)
							{
								 pObject = pIndexSub->IndexIndicatorList.GetNext(posIndexInd);
								 pIndexInd = (structIndexIndicator*)pObject;
								 
								 if(pIndexInd->strIndicator == strInd || pIndexInd->strIndicator == _T("AL"))
								 {
									if(bCheck) continue;	 

									strTokenCode = strTag + strSubfieldCode;

									ids = m_pMarcMgr->GetItem(m_pMarc,
															   strTokenCode,
															   strToken,
															   &arrayItemData);
									if(ids<0) continue;
													
									strTmpIndexItem = GetIndexToken(arrayItemData,
																	 pIndexInd->strType,
																	 pIndexInd->strSingleOption,
																     mapCheckDup);	
									
									if(!strTagType.IsEmpty())
									{
										m_Index.TrimAll(strTmpIndexItem);
										mapSubfield.SetAt(strSubfieldCode,strTmpIndexItem);
									
									}
									else
									{
										strIndexItem += strTmpIndexItem;

									}

									bCheck = TRUE;

								 }
								
							}
							

						}
						
						
					}

				}

			}

		}
		

		if(!strTagType.IsEmpty()) 
		{
			if(mapSubfield.GetCount()>0)
			{
				strIndexItem += GetTagTypeIndex(strTagType, mapSubfield);
				mapSubfield.RemoveAll();
			}
		}

	}

	return 0;
}


CString CIndexMgr::GetIndexToken(CArray<CString, CString&>& arrayItemData,
								 CString strIndexType,
								 CString strSingleOption,
								 CMap<CString, LPCTSTR, CString, LPCTSTR>& mapCheckDup)
{
	CString strResult;	
	CString strUniqueKey;
	CString strToken1,strToken2,strToken3;

	BOOL bCheck = FALSE;

	if(_T("A") == strIndexType)
	{
		
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
		
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}
		
			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
			
		}
		
	}
	else if(_T("B") == strIndexType) 
	{
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			m_Index.RemoveAllParenthesis(strToken1);
			
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}

			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
		}

	}
	else if(_T("C") == strIndexType)
	{
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			if (strToken1.Left(1) == _T("("))
			{
				m_Index.RemoveOneParenthesis(strToken1);
			}

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}
			
			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
		}

	}
	else if(_T("D") == strIndexType)
	{
	
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}
					
			m_Index.RemoveChar(strToken1, '(');
			m_Index.RemoveChar(strToken1, ')');		
			

			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;

		}

	}
	else if(_T("E") == strIndexType)
	{
		
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}

			strToken2 = strToken1;
					
			m_Index.RemoveChar(strToken2, '(');
			m_Index.RemoveChar(strToken2, ')');					
			strResult += _T(" ") + strToken2;

			if (m_Index.RemoveAllParenthesis(strToken1) > 0)
			{
				if(_T("S") == strSingleOption)
				{
					if(i==0) return strToken1;
					else return _T("");
				}
				
				strResult += _T(" ") + strToken1;

			}

		}
		
	}
	else if(_T("F") == strIndexType)
	{
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			
			INT nFind = strToken1.Find(_T(":"));
			if (nFind > 0)
			{
				strToken1 = strToken1.Mid(nFind + 1);
			}

			m_Index.RemoveAllParenthesis(strToken1);
			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}

			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
		}
	}
	else if(_T("G") == strIndexType)
	{
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			
			INT nFind = strToken1.Find(_T(":"));
			if (nFind > 0)
			{
				strToken1 = strToken1.Mid(nFind + 1);
			}

			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}

			m_Index.RemoveChar(strToken1, '(');
			m_Index.RemoveChar(strToken1, ')');	
			
			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
		}
	}
	else if(_T("H") == strIndexType)
	{
		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
			
			
			INT nFind = strToken1.Find(_T(":"));
			if (nFind > 0)
			{
				strToken1 = strToken1.Mid(nFind + 1);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
			}

			m_Index.RemoveSpace(strToken1);
			m_Index.TrimAll(strToken1);

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}

			strToken2 = strToken1;
					
			m_Index.RemoveChar(strToken2, '(');
			m_Index.RemoveChar(strToken2, ')');					
			strResult += _T(" ") + strToken2;

			if (m_Index.RemoveAllParenthesis(strToken1) > 0)
			{
				if(_T("S") == strSingleOption)
				{
					if(i==0) return strToken1;
					else return _T("");
				}
				
				strResult += _T(" ") + strToken1;
			}
		}
	}
	else if(_T("I") == strIndexType)
	{

		INT n = arrayItemData.GetSize();
		for (INT i = 0; i < n; i++)
		{
			strToken1 = arrayItemData.GetAt(i);
		
			strToken1.MakeUpper();
			m_Index.TrimAll(strToken1);
			

			INT nFind = strToken1.ReverseFind(' ');
			if (nFind > 0)
			{
				CString str1 = strToken1.Mid(nFind);
				CString str2 = str1;
				str1.TrimLeft();
				str1.TrimRight();
						
				if (m_Index.m_CharSetMgr_RoleWord.Find(str1, str2) == 0)
				{
					strToken1.Delete(nFind, strToken1.GetLength() - nFind);
				}
			}

			bCheck = mapCheckDup.Lookup(strToken1, strUniqueKey);
			if ( bCheck == FALSE ) {
				mapCheckDup.SetAt( strToken1, strToken1 );
			}
			else 
			{
				continue;
			}
		
			m_Index.RemoveSpace(strToken1);

			if(_T("S") == strSingleOption)
			{
				if(i==0) return strToken1;
				else return _T("");
			}

			strResult += _T(" ") + strToken1;
		}
		
	}


	
	return strResult;
}

CString CIndexMgr::GetTagTypeIndex(CString strTagType, 
								   CMap<CString, LPCTSTR, CString, LPCTSTR>& mapSubfield)
{
	CString strResult;

	if(_T("A") == strTagType)
	{
		CStringArray arraySpecialSubfield;
		arraySpecialSubfield.Add(_T("$a"));
		arraySpecialSubfield.Add(_T("$b"));
		arraySpecialSubfield.Add(_T("$f"));

		strResult = GetTagTypeIndexSpecialSub(arraySpecialSubfield,mapSubfield);
	}
	else if(_T("B") == strTagType)
	{
		CStringArray arraySpecialSubfield;
		arraySpecialSubfield.Add(_T("$a"));
		arraySpecialSubfield.Add(_T("$b"));
		arraySpecialSubfield.Add(_T("$x"));
		arraySpecialSubfield.Add(_T("$y"));
		arraySpecialSubfield.Add(_T("$z"));

		strResult = GetTagTypeIndexSpecialSub(arraySpecialSubfield,mapSubfield);
	}
	else if(_T("C") == strTagType)
	{
		CStringArray arraySpecialSubfield;
		arraySpecialSubfield.Add(_T("$a"));
		arraySpecialSubfield.Add(_T("$p"));
		arraySpecialSubfield.Add(_T("$x"));
		arraySpecialSubfield.Add(_T("$y"));
		arraySpecialSubfield.Add(_T("$z"));

		strResult = GetTagTypeIndexSpecialSub(arraySpecialSubfield,mapSubfield);
	}
	else if(_T("D") == strTagType)
	{
		CStringArray arraySpecialSubfield;
		arraySpecialSubfield.Add(_T("$a"));
		arraySpecialSubfield.Add(_T("$x"));
		arraySpecialSubfield.Add(_T("$y"));
		arraySpecialSubfield.Add(_T("$z"));

		strResult = GetTagTypeIndexSpecialSub(arraySpecialSubfield,mapSubfield);
	}
	else if(_T("E") == strTagType)
	{
		
		CString strKey;
		CString strData1,strData2,strData3;
		INT ids = -1;
		BOOL bCheck = mapSubfield.Lookup(_T("$p"), strData1);

		if ( bCheck == FALSE ) {
			
			POSITION pos = mapSubfield.GetStartPosition();

			while(pos) 
			{
				mapSubfield.GetNextAssoc(pos, strKey, strData1);

				if(!strData1.IsEmpty())
				{
					strResult += _T(" ") + strData1;
				}

			}
		}
		else 
		{
			POSITION pos = mapSubfield.GetStartPosition();

			while(pos) 
			{
				mapSubfield.GetNextAssoc(pos, strKey, strData1);
				if(_T("$a") == strKey || _T("$p") == strKey) continue;

				if(!strData1.IsEmpty())
				{
					strResult += _T(" ") + strData1;
				}

			}

			mapSubfield.Lookup(_T("$a"),strData1);
			mapSubfield.Lookup(_T("$p"),strData2);
			
			if(!strData1.IsEmpty())
			{
				strData3 += _T(" ") + strData1 + strData2;
				
				strResult += strData3;
				strResult += _T(" ") + strData2;
			}
			else 
			{		
				
				strResult += _T(" ") + strData2 + strData2;
			}
		}

	}
	else if(_T("F") == strTagType) 
	{
		
		CString strKey;
		CString strData1,strData2,strData3,strData4;
		INT ids = -1;
		BOOL bCheck = mapSubfield.Lookup(_T("$b"), strData1);

		if ( bCheck == FALSE ) {
			CStringArray arraySpecialSubfield;
			arraySpecialSubfield.Add(_T("$a"));
			arraySpecialSubfield.Add(_T("$k"));
			arraySpecialSubfield.Add(_T("$g"));

			strResult = GetTagTypeIndexSpecialSub(arraySpecialSubfield,mapSubfield);
		}
		else 
		{
			BOOL bCheck = FALSE;
			POSITION pos = mapSubfield.GetStartPosition();

			while(pos) 
			{
				mapSubfield.GetNextAssoc(pos, strKey, strData1);
				bCheck = FALSE;

				if(_T("$a") == strKey || _T("$b") == strKey || _T("$k") == strKey || _T("$g") == strKey)
				{
					strData2 += strData1;
					bCheck = TRUE;
				}
				
				if(_T("$b") == strKey || _T("$k") == strKey || _T("$g") == strKey)
				{
					strData3 += strData1;
				}
				

				if(!strData1.IsEmpty() && !bCheck)
				{
					strResult += _T(" ") + strData1;
				}

			}

			m_Index.RemoveSpace(strData2);
			m_Index.RemoveSpace(strData3);

			if(!strData2.IsEmpty())
				strResult += _T(" ") + strData2;

			if(!strData3.IsEmpty())
				strResult += _T(" ") + strData3;

		}
	}
	

	return strResult;
}

CString CIndexMgr::GetTagTypeIndexSpecialSub(CStringArray& arraySpecialSubfield,
											 CMap<CString, LPCTSTR, CString, LPCTSTR>& mapSubfield)
{
	CString strResult;
	
	POSITION pos = mapSubfield.GetStartPosition();
	CString strData1,strData2;
	CString strKey;
	CString strSpecialSub;
	BOOL bCheck = FALSE;
	INT ids = -1;
	INT nSize = arraySpecialSubfield.GetSize();

	while(pos)
	{
		mapSubfield.GetNextAssoc(pos, strKey, strData1);
		
		bCheck = FALSE;
		m_Index.TrimAll(strData1);

		for(INT i=0;i<nSize;i++)
		{
			strSpecialSub = arraySpecialSubfield.GetAt(i);
			if(strKey == strSpecialSub)
			{
				if(!strData1.IsEmpty())
					strData2 += strData1;
				
				bCheck = TRUE;
			}
		}
		
		if(!bCheck)
		{
			if(!strData1.IsEmpty())
				strResult += _T(" ") + strData1;
		}
	}
	
	m_Index.RemoveSpace(strData2);
	if(!strData2.IsEmpty())
		strResult += _T(" ") + strData2;

	return strResult;
}

BOOL CIndexMgr::CheckUseDeault(CString strIndexAlias,CString strMarcGroup)
{
	
	CString strAlias;
	
	if (_T("단행") == strMarcGroup)
	{
		INT nSize = m_arrayUseDeaultMono.GetSize();
		
		for(INT i=0;i<nSize;i++)
		{
			strAlias = m_arrayUseDeaultMono.GetAt(i);
			if(strAlias == strIndexAlias) return TRUE;
		}
	}
	else if (strMarcGroup.Find(_T("비도서")) >= 0)
	{
		INT nSize = m_arrayUseDeaultNonBook.GetSize();
		
		for(INT i=0;i<nSize;i++)
		{
			strAlias = m_arrayUseDeaultMono.GetAt(i);
			if(strAlias == strIndexAlias) return TRUE;
		}		
	}
	else if (_T("연속") == strMarcGroup)
	{
		INT nSize = m_arrayUseDeaultSerial.GetSize();
		
		for(INT i=0;i<nSize;i++)
		{
			strAlias = m_arrayUseDeaultMono.GetAt(i);
			if(strAlias == strIndexAlias) return TRUE;
		}	
	}

	
	return FALSE;
}
