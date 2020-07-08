// Index_Mono.cpp: implementation of the CIndex_Mono class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Index_Mono.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndex_Mono::CIndex_Mono() 
{

}

CIndex_Mono::~CIndex_Mono()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

INT CIndex_Mono::GetIndex(CString strIndexAlias, CString &strIndexItem)
{
	if (CIndex::GetIndex(strIndexAlias, strIndexItem) < 0) 
		return -1;

	if (strIndexAlias == _T("서명파일"))
		return GetIndex_BookTitleFile(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("저작자"))
		return GetIndex_Author(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("분류기호파일"))
		return GetIndex_ClassCode(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("키워드파일"))
		return GetIndex_Keyword(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("주제명파일"))
		return GetIndex_Subject(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("각종표준번호"))
		return GetIndex_StandardCode(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("발행자"))
		return GetIndex_Publisher(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("한국대학부호"))
		return GetIndex_KRUnivCode(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("정부기관부호"))
		return GetIndex_GovCode(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("인문"))
		return GetIndex_SealWord(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("요약문언어"))
		return GetIndex_SummaryWord(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("본문언어"))
		return GetIndex_TextWord(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("학위논문학과"))
		return GetIndex_SchoolCourse(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("발행년"))
		return GetIndex_PublishYear(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("ISBN"))
		return GetIndex_ISBN(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("본표제"))
		return GetIndex_BookTitleFile(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("범위검색용본표제"))
		return GetIndex_ITITLE(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("키워드"))
		return GetIndex_KEYWORD(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("범위검색용발행자"))
		return GetIndex_IPUBLISHER(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("표준부호_ISBN"))
		return GetIndex_ST_CODE(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표준부호_ISSN"))
		return GetIndex_ST_ISSN(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표준부호_STRN"))
		return GetIndex_ST_STRN(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표준부호_RNSTRN"))
		return GetIndex_ST_RNSTRN(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표준부호_국가서지번호"))
		return GetIndex_CBN(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표준부호_저작권등록번호"))
		return GetIndex_CAN(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("기타부호_청구기호_별치기호"))
		return GetIndex_SE_SHELF_CODE(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("기타부호_청구기호_분류기호"))
		return GetIndex_CLASS_NO(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("기타부호_청구기호_도서기호"))
		return GetIndex_BOOK_CODE(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("기타부호_청구기호_권책기호"))
		return GetIndex_VOL_CODE(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("기타부호_청구기호_권책기호_DISP"))
		return GetIndex_VOL_CODE_DISP(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("기타부호_제어번호"))
		return GetIndex_CONTROL_NO(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("KDCP"))
		return GetIndex_KDCP_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("KDC"))
		return GetIndex_KDCCLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("DDC"))
		return GetIndex_DDC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("CEC"))
		return GetIndex_CEC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("CWC"))
		return GetIndex_CWC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("COC"))
		return GetIndex_COC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("UDC"))
		return GetIndex_UDC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("NDC"))
		return GetIndex_NDC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("LC"))
		return GetIndex_LC_CLASS(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("ETC"))
		return GetIndex_ETC_CLASS(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("판종색인"))
		return GetIndex_EDIT(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("판종"))
		return Get_EDIT_INFO(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("범위검색용발행년도"))
		return GetIndex_IPUB_YEAR(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("발행년도"))
		return GetIndex_PublishYear(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("장정"))
		return GetIndex_JANGJUNG(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("표제사항INFO"))
		return GetIndex_BookTitleInfo(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("표제사항"))
		return GetIndex_TITLE_INFO(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("저작자사항"))
		return GetIndex_AUTHOR_INFO(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("발행자사항"))
		return GetIndex_PUB_INFO(strIndexAlias, strIndexItem);
	else if (strIndexAlias == _T("발행년도사항"))
		return GetIndex_PUB_YEAR_INFO(strIndexAlias, strIndexItem);

	else if (strIndexAlias == _T("발행지"))
		return GetIndex_PUB_PLACE_INFO(strIndexAlias, strIndexItem);
	
	m_strLastError = _T("strIndexAlias is not index alias");
	return -1;
}

INT CIndex_Mono::MakeFullIndex( CString strIndexAlias, CString &strIndexItem )
{
	CString strAllItem = _T("");
	CString strProcessItem = _T("");
	CString strTempItem = _T("");
	
	INT nPos = 0;

	if( _T("본표제") == strIndexAlias )
	{
		strProcessItem = strIndexItem;
		nPos = strProcessItem.FindOneOf(_T(") "));
		strTempItem = strProcessItem;
		RemoveSpace(strTempItem);
		strAllItem = strTempItem;
		while(0 < nPos )
		{
			strTempItem = strProcessItem.Left(nPos);
			strProcessItem = strProcessItem.Mid(nPos+1);
			strProcessItem.TrimLeft();	strProcessItem.TrimRight();

			strAllItem = strAllItem + _T(" ") + strTempItem;
			strTempItem = strProcessItem;
			RemoveSpace(strTempItem);
			strAllItem = strAllItem + _T(" ") + strTempItem;

			nPos = strProcessItem.FindOneOf(_T(") "));
		}
		strIndexItem = strAllItem;
	}
	else
	{
		RemoveSpace(strIndexItem);
	}
	return 0;
}


INT CIndex_Mono::GetIndex_BookTitleFile(CString strIndexAlias, CString &strIndexItem)
{		
	INT ids;
	CArray<CString, CString&> arrayItemData;

	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapCheckDup;

	CTag *pTag = GetMarc()->FindTag(_T("245"));
	if (pTag != NULL) 
	{
		CString strFirstInd = pTag->GetFirstIndicator();
		CString strSecondInd = pTag->GetSecondIndicator();

		strTemp1.Empty();
		strTemp2.Empty();
		arrayItemData.RemoveAll();
		
		ids = GetMarcMgr()->GetItem(GetMarc(), _T("245$a"), strTemp1, &arrayItemData);
		if (ids >= 0)
		{
			INT n = arrayItemData.GetSize();
			for (INT i = 0; i < n; i++)
			{
				strTemp1 = arrayItemData.GetAt(i);
				
				strTemp1.MakeUpper();					
				
				MakeFullIndex( strIndexAlias, strTemp1 );
				TrimAll(strTemp1);
				UnicodeSpecialConvert(strTemp1);	
				
				CString strUniqueKey;
				BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
				if ( bCheck == FALSE ) {
					mapCheckDup.SetAt( strTemp1, strTemp1 );
				}
				else {
					continue;
				}
				
				strTemp2 = strTemp1;
				
				strTemp2.Replace(_T("[외]"),_T(""));
				strTemp2.Replace(_T("[外]"),_T(""));
				strTemp2.Replace(_T("[et al.]"),_T(""));
				
				RemoveChar(strTemp2, '(');
				RemoveChar(strTemp2, ')');					
				strIndexItem += _T(" ") + strTemp2;
				
				if (RemoveAllParenthesis(strTemp1) > 0)
					strIndexItem += _T(" ") + strTemp1;
			}
			
			arrayItemData.RemoveAll();
			ids = GetMarcMgr()->GetItem(GetMarc(), _T("245$x"), strTemp1, &arrayItemData);
			if (ids >= 0)
			{
				INT n = arrayItemData.GetSize();
				for (INT i = 0; i < n; i++)
				{
					strTemp1 = arrayItemData.GetAt(i);
					
					strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
					TrimAll(strTemp1);
					RemoveAllParenthesis(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
					
					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}
					
					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}

		strTemp1.Empty();
		strTemp2.Empty();
		arrayItemData.RemoveAll();
		
		CString strLeader;
		GetMarc()->GetLeader(strLeader);
		if( _T("c") != strLeader.Mid(18,1) )
		{
			ids = GetMarcMgr()->GetItem(GetMarc(), _T("245$b"), strTemp1, &arrayItemData);
			if (ids >= 0)
			{
				INT n = arrayItemData.GetSize();
				for (INT i = 0; i < n; i++)
				{
					strTemp1 = arrayItemData.GetAt(i);

					strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					if (strTemp1.Left(1) == _T("("))
					{
						RemoveOneParenthesis(strTemp1);
					}

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
		
		arrayItemData.RemoveAll();
		ids = GetMarcMgr()->GetItem(GetMarc(), _T("245$p"), strTemp1, &arrayItemData);
		if (ids >= 0)
		{
			INT n = arrayItemData.GetSize();
			for (INT i = 0; i < n; i++)
			{
				strTemp1 = arrayItemData.GetAt(i);

				strTemp1.MakeUpper();
				
				MakeFullIndex( strIndexAlias, strTemp1 );
				TrimAll(strTemp1);
				UnicodeSpecialConvert(strTemp1);	

				if (strTemp1.Left(1) == _T("("))
				{
					RemoveOneParenthesis(strTemp1);
				}

				CString strUniqueKey;
				BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
				if ( bCheck == FALSE ) {
					mapCheckDup.SetAt( strTemp1, strTemp1 );
				}
				else {
					continue;
				}

				strIndexItem += _T(" ") + strTemp1;
			}
		}

	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		
		if (pTag->GetTagCode() == _T("130") || pTag->GetTagCode() == _T("210") ||
			pTag->GetTagCode() == _T("222") || pTag->GetTagCode() == _T("240") ||
			pTag->GetTagCode() == _T("242") || pTag->GetTagCode() == _T("243") ||
			pTag->GetTagCode() == _T("246") || pTag->GetTagCode() == _T("247") ||
			pTag->GetTagCode() == _T("730") || pTag->GetTagCode() == _T("930") || 
			pTag->GetTagCode() == _T("940")	|| pTag->GetTagCode() == _T("949") )
		{
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			strTemp1.Empty();
			strTemp2.Empty();
			strTemp3.Empty();
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');

				if ( pTag->GetTagCode() == _T("240") || pTag->GetTagCode() == _T("242") ||
					 pTag->GetTagCode() == _T("246") || pTag->GetTagCode() == _T("247") )
				{
					if (strFirstInd == _T("0"))
					{
						continue;
					}
				}

				if ( pTag->GetTagCode() == _T("246") )
				{
					if (strFirstInd == _T("2"))
					{
						continue;
					}
				}

				if (strSubfieldCode == _T("$a"))
				{	
					strTemp1 = pSubfield->GetItemData();
					
					if (strSecondInd == _T("1") || pTag->GetTagCode() == _T("246") )
					{
						RemoveAllParenthesis(strTemp1);
						MakeIndex(strTemp1);
					}
					else if (strFirstInd == _T("2"))
					{
						strTemp2 = strTemp1;
						MakeIndex(strTemp1);						
						if (RemoveAllParenthesis(strTemp2) > 0)
						strTemp1 += _T(" ") + strTemp2;
					}
					else
					{						
						MakeIndex(strTemp1);
					}

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}
				}
			}

			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');

				if (strSubfieldCode == _T("$p"))
				{	
					strTemp2 = pSubfield->GetItemData();
					RemoveAllParenthesis(strTemp2);
					MakeIndex(strTemp2);

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp2, strTemp2 );
					}
					else {
						continue;
					}

					if (!strTemp1.IsEmpty())
						strTemp3 += _T(" ") + strTemp1 + _T(" ") + strTemp1 + strTemp2;

					strTemp3 += _T(" ") + strTemp2;
					
				}
			}
			if (strTemp3.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
			else
				strIndexItem += strTemp3;
		}
	}
	
	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != _T("440")) continue;

		CString strFirstInd = pTag->GetFirstIndicator();
		CString strSecondInd = pTag->GetSecondIndicator();

		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield != NULL)
		{
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;
			CString strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');
			
			strTemp1 = pSubfield->GetItemData();		

			if (strSubfieldCode == _T("$a") || strSubfieldCode ==_T("$x") || strSubfieldCode == _T("$p"))
			{
				if (strFirstInd == _T("0") && strSecondInd == _T("0"))
				{
					strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
					TrimAll(strTemp1);
					RemoveAllParenthesis(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
				else if (strFirstInd == _T("1") && strSecondInd == _T("0"))
				{
					strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					if (strSubfieldCode == _T("$x"))
					{
						if (strTemp1.Left(1) == _T("("))
						{
							RemoveOneParenthesis(strTemp1);
						}

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp1, strTemp1 );
						}
						else {
							continue;
						}

						strIndexItem += _T(" ") + strTemp1;
					}
					else
					{
						strTemp2 = strTemp1;
						
						RemoveChar(strTemp2, '(');
						RemoveChar(strTemp2, ')');

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp2, strTemp2 );
							strIndexItem += _T(" ") + strTemp2;
						}						

						if (RemoveAllParenthesis(strTemp1) > 0) {
							CString strUniqueKey;
							BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
							if ( bCheck == FALSE ) {
								mapCheckDup.SetAt( strTemp1, strTemp1 );
							}
							else {
								continue;
							}
							strIndexItem += _T(" ") + strTemp1;
						}
					}
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("830") ) 
		{

			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				strTemp1 = pSubfield->GetItemData();		

				if (strSubfieldCode == _T("$a") || strSubfieldCode ==_T("$p"))
				{
					strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
					
					TrimAll(strTemp1);
					RemoveAllParenthesis(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != _T("500")) continue;
		
		CString strFirstInd = pTag->GetFirstIndicator();
		CString strSecondInd = pTag->GetSecondIndicator();
		
		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield != NULL)
		{
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;
			CString strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');
			if (strSubfieldCode == _T("$c") ||
				strSubfieldCode == _T("$b") ||
				strSubfieldCode == _T("$e") ||
				strSubfieldCode == _T("$f") ||
				strSubfieldCode == _T("$g") ||
				strSubfieldCode == _T("$h") ||
				strSubfieldCode == _T("$i") ||
				strSubfieldCode == _T("$j") ||
				strSubfieldCode == _T("$k") ||
				strSubfieldCode == _T("$m") ||
				strSubfieldCode == _T("$r")
				)
			{
			
				strTemp1 = pSubfield->GetItemData();		
				
				if (strFirstInd == _T("0")) continue;

				if (strFirstInd == _T("1") && strSecondInd == _T("0"))
				{
					INT nFind = strTemp1.Find(_T(":"));
					if (nFind > 0)
					{
						strTemp1 = strTemp1.Mid(nFind + 1);

						strTemp1.MakeUpper();
						
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						RemoveAllParenthesis(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp1, strTemp1 );
						}
						else {
							continue;
						}

						strIndexItem += _T(" ") + strTemp1;

					}
					
                    else if (!strTemp1.IsEmpty () && !(nFind>0))
					{
					    strTemp1.MakeUpper();
					
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						RemoveAllParenthesis(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp1, strTemp1 );
						}
						else {
							continue;
						}

						strIndexItem += _T(" ") + strTemp1;				
                    }
				}
				else if (strFirstInd == _T("1") && strSecondInd == _T("1"))
				{
					INT nFind = strTemp1.Find(_T(":"));
					if (nFind > 0)
					{
						strTemp1 = strTemp1.Mid(nFind + 1);

						strTemp1.MakeUpper();
						
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						RemoveAllParenthesis(strTemp1);
						// 2009.03.20 ADD BY PDJ : Unicode Special Convert
						UnicodeSpecialConvert(strTemp1);	

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp1, strTemp1 );
						}
						else {
							continue;
						}

						strIndexItem += _T(" ") + strTemp1;

					}
                    else if (!strTemp1.IsEmpty () && !(nFind>0))
					{
					    strTemp1.MakeUpper();
						
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						RemoveAllParenthesis(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp1, strTemp1 );
						}
						else {
							continue;
						}

						strIndexItem += _T(" ") + strTemp1;				
                    }
				}
				else if (strFirstInd == _T("2") && strSecondInd == _T("0"))
				{
					INT nFind = strTemp1.Find(_T(":"));
					if (nFind > 0)
					{
						strTemp1 = strTemp1.Mid(nFind + 1);

						strTemp1.MakeUpper();					
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						strTemp2 = strTemp1;
						
						RemoveChar(strTemp2, '(');
						RemoveChar(strTemp2, ')');

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp2, strTemp2 );
							strIndexItem += _T(" ") + strTemp2;
						}						

						if (RemoveAllParenthesis(strTemp1) > 0) {
							CString strUniqueKey;
							BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
							if ( bCheck == FALSE ) {
								mapCheckDup.SetAt( strTemp1, strTemp1 );
							}
							else {
								continue;
							}

							strIndexItem += _T(" ") + strTemp1;
						}
					}
                    else if (!strTemp1.IsEmpty () && !(nFind>0))
					{
					    strTemp1.MakeUpper();						
					MakeFullIndex( strIndexAlias, strTemp1 );
						TrimAll(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						strTemp2 = strTemp1;
						
						RemoveChar(strTemp2, '(');
						RemoveChar(strTemp2, ')');

						CString strUniqueKey;
						BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
						if ( bCheck == FALSE ) {
							mapCheckDup.SetAt( strTemp2, strTemp2 );
							strIndexItem += _T(" ") + strTemp2;
						}						

						if (RemoveAllParenthesis(strTemp1) > 0) {
							CString strUniqueKey;
							BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
							if ( bCheck == FALSE ) {
								mapCheckDup.SetAt( strTemp1, strTemp1 );
							}
							else {
								continue;
							}

							strIndexItem += _T(" ") + strTemp1;
						}			
                    }
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != _T("507")) continue;
		
		CString strFirstInd = pTag->GetFirstIndicator();
		CString strSecondInd = pTag->GetSecondIndicator();
		
		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield != NULL)
		{
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;
			CString strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');
			if (strSubfieldCode != _T("$t")) continue;
			
			strTemp1 = pSubfield->GetItemData();		
			
			if (strFirstInd == _T("0")) continue;

			if ( (strFirstInd == _T("2") && strSecondInd == _T("0")) || strFirstInd == _T("1") )
			{
				strTemp1.MakeUpper();				
					MakeFullIndex( strIndexAlias, strTemp1 );
				TrimAll(strTemp1);
				UnicodeSpecialConvert(strTemp1);	

				strTemp2 = strTemp1;
				
				RemoveChar(strTemp2, _T('('));
				RemoveChar(strTemp2, _T(')'));

				CString strUniqueKey;
				BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
				if ( bCheck == FALSE ) {
					mapCheckDup.SetAt( strTemp2, strTemp2 );
					strIndexItem += _T(" ") + strTemp2;
				}

				if (RemoveAllParenthesis(strTemp1) > 0) {
					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
			}			
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != _T("740")) continue;
		
		CString strFirstInd = pTag->GetFirstIndicator();
		CString strSecondInd = pTag->GetSecondIndicator();
		
		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield != NULL)
		{
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;
			CString strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');
			if (strSubfieldCode != _T("$a")) continue;
			
			strTemp1 = pSubfield->GetItemData();
			strTemp1.MakeUpper();			
					MakeFullIndex( strIndexAlias, strTemp1 );
			TrimAll(strTemp1);
			UnicodeSpecialConvert(strTemp1);	
			
			strSecondInd.TrimLeft();	strSecondInd.TrimRight(); 
			if (strSecondInd == _T("0") || strSecondInd == _T("2") || strSecondInd == _T("")) 
			{
				CString strUniqueKey;
				BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
				if ( bCheck == FALSE ) {
					mapCheckDup.SetAt( strTemp1, strTemp1 );
				}
				else {
					continue;
				}

				strIndexItem += _T(" ") + strTemp1;
			}
			else if (strSecondInd == _T("1"))
			{
					MakeFullIndex( strIndexAlias, strTemp1 );					
				RemoveAllParenthesis(strTemp1);

				CString strUniqueKey;
				BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
				if ( bCheck == FALSE ) {
					mapCheckDup.SetAt( strTemp1, strTemp1 );
				}
				else {
					continue;
				}

				strIndexItem += _T(" ") + strTemp1;
			}			
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_Author(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;	
	POSITION posTag;

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapCheckDup;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("100") || pTag->GetTagCode() == _T("700") || 
			pTag->GetTagCode() == _T("800") || pTag->GetTagCode() == _T("900") )
		{
			strTemp1.Empty();
			strTemp2.Empty();
			strTemp3.Empty();
		
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

				}
				else if (strSubfieldCode == _T("$b"))
				{
					strTemp2 = pSubfield->GetItemData();
					strTemp2.MakeUpper();
					RemoveSpace(strTemp2);
					TrimAll(strTemp2);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp2, strTemp2 );
					}
					else {
						continue;
					}
				}
				else if (strSubfieldCode == _T("$c"))
				{
					strTemp3 = pSubfield->GetItemData();
					strTemp3.MakeUpper();
					RemoveSpace(strTemp3);
					TrimAll(strTemp3);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp3, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp3, strTemp3 );
					}
					else {
						continue;
					}
				}	
			}						
			strIndexItem += _T(" ") + strTemp1 + strTemp2 + strTemp3;										
		}
	}

	CString strPrevSubField, strAppendIndexItem;
	BOOL b$bCheck;
	
	strAppendIndexItem=_T("");
	posTag = GetMarc()->m_listTag.GetHeadPosition();	
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("110") || pTag->GetTagCode() == _T("710") || 
			pTag->GetTagCode() == _T("910") || pTag->GetTagCode() == _T("910") )
		{
			strTemp1.Empty();
			strTemp2.Empty();
			strTemp3.Empty();
			strIndexItem += _T(" ");
		
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			b$bCheck=FALSE;
			strPrevSubField=_T("");
			strAppendIndexItem=_T("");
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$b") || strSubfieldCode == _T("$k") || strSubfieldCode == _T("$g"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += strTemp1;										

					if ( b$bCheck )
					{
						strAppendIndexItem += strTemp1;						
					}					
				}					
				
				if ( strPrevSubField == _T("$a") && strSubfieldCode == _T("$b") ) 
				{
					strAppendIndexItem = _T(" ")+strTemp1;
					b$bCheck = TRUE;
				}
				strPrevSubField = strSubfieldCode;
			}

			strIndexItem += strAppendIndexItem;


			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$b"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;


				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("111") || pTag->GetTagCode() == _T("711") || 
			pTag->GetTagCode() == _T("811") || pTag->GetTagCode() == _T("911") )
		{
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}
					strIndexItem += _T(" ") + strTemp1;
				}				
			}


			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$e"))
				{
					strTemp2 = pSubfield->GetItemData();
					strTemp2.MakeUpper();
					RemoveSpace(strTemp2);
					TrimAll(strTemp2);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp2, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp2, strTemp2 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp2;
				}				
			}	
			strIndexItem += _T(" ") + strTemp1 + strTemp2;			
		}
	}
	
	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("507"))
		{
			strTemp1.Empty();
					
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();
			
			if (strFirstInd == _T("0")) continue;		
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("245"))
		{
			strTemp1.Empty();
					
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$d"))
				{
					strTemp1 = pSubfield->GetItemData();					
					strTemp1.Replace(_T("[외]"),_T(""));
					strTemp1.Replace(_T("[外]"),_T(""));
					strTemp1.Replace(_T("[et al.]"),_T(""));

					INT nFind = strTemp1.Find(_T(": "));
					if( nFind > -1)
					{
						strTemp1 = strTemp1.Mid(nFind+2);
					}

					strTemp1.MakeUpper();
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
					
					nFind = strTemp1.ReverseFind(' ');
					if (nFind > 0)
					{
						CString str1 = strTemp1.Mid(nFind);
						CString str2 = str1;
						str1.TrimLeft();
						str1.TrimRight();
						
						if (m_CharSetMgr_RoleWord.Find(str1, str2) == 0)
						{
							strTemp1.Delete(nFind, strTemp1.GetLength() - nFind);
						}
					}
					RemoveSpace(strTemp1);

					CString strUniqueKey;
					BOOL bCheck = mapCheckDup.Lookup(strTemp1, strUniqueKey);
					if ( bCheck == FALSE ) {
						mapCheckDup.SetAt( strTemp1, strTemp1 );
					}
					else {
						continue;
					}

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
	}
	

	return 0;
}

INT CIndex_Mono::GetIndex_ClassCode(CString strIndexAlias, CString &strIndexItem)
{		
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("056"))
		{
			strTemp1.Empty();
					
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("082"))
		{
			strTemp1.Empty();
					
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					INT nFind = strTemp1.Find(_T("/"));
					strTemp2 = strTemp1.Left(nFind);
					TrimAll(strTemp2);

					strIndexItem += _T(" ") + strTemp2;

					strTemp1 = strTemp1.Mid(nFind + 1);
					TrimAll(strTemp1);

					while (TRUE)
					{
						nFind = strTemp1.Find(_T("/"));
						if (nFind < 0)
						{
							strIndexItem += _T(" ") + strTemp2 + strTemp1;
							break;
						}
						else
						{
							strTemp3 = strTemp1.Left(nFind);
							TrimAll(strTemp3);
							strIndexItem += _T(" ") + strTemp2 + strTemp3;
							strTemp1 = strTemp1.Mid(nFind + 1);
						}
					}
				}
			}
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("085"))
		{
			strTemp1.Empty();
					
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strSecondInd != _T(" ")) continue;
			if (strFirstInd == _T("0") || strFirstInd == _T("2") || strFirstInd == _T("3")) 
			{
			
				POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
				while (posSubfield != NULL)
				{
					CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
					if (pSubfield == NULL) continue;
					CString strSubfieldCode = pSubfield->GetSubfieldCode();
					strSubfieldCode.Replace(SUBFIELD_CODE, '$');
					
					if (strSubfieldCode == _T("$a"))
					{
						strTemp1 = pSubfield->GetItemData();
						strTemp1.MakeUpper();
						RemoveSpace(strTemp1);
						TrimAll(strTemp1);
						UnicodeSpecialConvert(strTemp1);	

						if (strFirstInd == _T("3"))
							RemoveChar(strTemp1, '-');
						strIndexItem += _T(" ") + strTemp1;
					}
				}
			}
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_Keyword(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("653"))
		{
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					strIndexItem += _T(" ") + strTemp1;
				}
			}
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_Subject(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("600"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$b") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$y") || strSubfieldCode == _T("$z"))				
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					TrimAll(strTemp1);					
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("610"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$b") || strSubfieldCode == _T("$k") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$y") || strSubfieldCode == _T("$z"))				
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					TrimAll(strTemp1);					
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("611"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$k") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$y") || strSubfieldCode == _T("$z"))				
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("630"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$p") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$y") || strSubfieldCode == _T("$z"))				
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("650") || pTag->GetTagCode() == _T("651"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$x") || strSubfieldCode == _T("$y") || strSubfieldCode == _T("$z"))				
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_StandardCode(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("015"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") || strSubfieldCode == _T("$b"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					RemoveChar(strTemp1, '-');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("017"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveAllParenthesis(strTemp1);
					RemoveChar(strTemp1, '.');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("020"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("022"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveChar(strTemp1, '-');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("027"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveChar(strTemp1, '-');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("027"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveChar(strTemp1, '-');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("088"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd != _T(" ") || strSecondInd != _T(" ")) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 += pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					RemoveChar(strTemp1, '-');
					RemoveChar(strTemp1, '/');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
				}
			}

			if (!strTemp1.IsEmpty())
				strIndexItem += _T(" ") + strTemp1;
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_Publisher(CString strIndexAlias, CString &strIndexItem)
{

	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("260"))
		{
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$b"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					strIndexItem += _T(" ") + strTemp1;
				}
			}				
		}
	}

	INT nPaserIdx;
	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("550"))
		{
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
					nPaserIdx = strTemp1.Find(_T(":"),0);
					if ( nPaserIdx > 0 )
					{
						strIndexItem += _T(" ") + strTemp1.Mid(nPaserIdx);						
					}
					else
					{
						strIndexItem += _T(" ") + strTemp1;
					}					
				}
			}				
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_KRUnivCode(CString strIndexAlias, CString &strIndexItem)
{
	

	GetMarcMgr()->GetItem(GetMarc(), _T("008@26-27"), strIndexItem);

	return 0;
}

INT CIndex_Mono::GetIndex_GovCode(CString strIndexAlias, CString &strIndexItem)
{
	

	GetMarcMgr()->GetItem(GetMarc(), _T("008@38-39"), strIndexItem);

	return 0;
}

INT CIndex_Mono::GetIndex_SealWord(CString strIndexAlias, CString &strIndexItem)
{

	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("590"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			if (strFirstInd == _T("1") && strSecondInd == _T(" "))
			{

				POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
				while (posSubfield != NULL)
				{
					CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
					if (pSubfield == NULL) continue;
					CString strSubfieldCode = pSubfield->GetSubfieldCode();
					strSubfieldCode.Replace(SUBFIELD_CODE, '$');
					
					if (strSubfieldCode == _T("$b"))
					{
						strTemp1 = pSubfield->GetItemData();

						INT nFind = strTemp1.Find(_T(":"));
						if (nFind > 0)
						{
							strTemp1 = strTemp1.Mid(nFind + 1);

							strTemp1.MakeUpper();
							RemoveSpace(strTemp1);
							TrimAll(strTemp1);
							UnicodeSpecialConvert(strTemp1);	
							strIndexItem += _T(" ") + strTemp1;
						}
					}
				}		
			}
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_SummaryWord(CString strIndexAlias, CString &strIndexItem)
{

	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("041"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				strTemp2.Empty();
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$b"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					INT n = 0;
					INT nCount = strTemp1.GetLength();
					for (INT i = 0; i < nCount; i++)
					{
						strTemp2 +=	strTemp1.GetAt(i);
						n++;

						if (n % 3 == 0 && i + 1 < nCount)
							strTemp2 += _T(",");
					}
				}
				
				if (!strTemp2.IsEmpty())
					strIndexItem += _T(" ") + strTemp2;
			}				
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_TextWord(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("041"))
		{
			strTemp1.Empty();
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				strTemp2.Empty();
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveSpace(strTemp1);
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	
					strIndexItem += _T(" ") + strTemp1;
				}
			}		
		}
	}

	GetMarcMgr()->GetItem(GetMarc(), _T("008@35-37"), strTemp2);
	strTemp1.MakeUpper();
	RemoveSpace(strTemp1);
	TrimAll(strTemp1);
	UnicodeSpecialConvert(strTemp1);	
	
	if (!strTemp2.IsEmpty())
		strIndexItem += _T(" ") + strTemp2;
	
	return 0;
}

INT CIndex_Mono::GetIndex_SchoolCourse(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag;

	posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("502"))
		{
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$c"))
				{
					strTemp1 = pSubfield->GetItemData();
					strTemp1.MakeUpper();
					RemoveChar(strTemp1, '(');
					RemoveChar(strTemp1, ')');
					TrimAll(strTemp1);
					UnicodeSpecialConvert(strTemp1);	

					strTemp2 = strTemp1;
					RemoveSpace(strTemp2);
					strIndexItem += _T(" ") + strTemp2;
					
					INT nFind = strTemp1.Find(' ');
					if (nFind > 0)
					{
						strIndexItem += _T(" ") + strTemp1.Left(nFind);
						strIndexItem += _T(" ") + strTemp1.Mid(nFind + 1);
					}					
				}
			}		
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_PublishYear(CString strIndexAlias, CString &strIndexItem)
{
	CString strTemp1, strTemp2, strTemp3;

	CString strPubYearType;
	
	CString strPubYear1;

	CString strPubYear2;

	GetMarcMgr()->GetItem(GetMarc(), _T("008@06"), strPubYearType);
	if (strPubYearType.IsEmpty()) return -1;

	GetMarcMgr()->GetItem(GetMarc(), _T("008@7-10"), strPubYear1);
	if (strPubYear1.IsEmpty()) return -1;

	GetMarcMgr()->GetItem(GetMarc(), _T("008@11-14"), strPubYear2);
	if (strPubYear2.IsEmpty()) return -1;

	if (strPubYearType == _T("b"))
	{
		return 0;
	}
	else if (strPubYearType == _T("e"))
	{
		strIndexItem = strPubYear1;
	}
	else if (strPubYearType == _T("s"))
	{
		strIndexItem = strPubYear1;
	}
	else if (strPubYearType == _T("t"))
	{
		strIndexItem = strPubYear1;
	}
	else if (strPubYearType == _T("i"))
	{
		strIndexItem = strPubYear1;
		if (strPubYear1 != strPubYear2)
		{
			strIndexItem += _T("-") + strPubYear2;
		}
	}
	else if (strPubYearType == _T("k"))
	{
		strIndexItem = strPubYear1;
		if (strPubYear1 != strPubYear2)
		{
			strIndexItem += _T("-") + strPubYear2;
		}
	}
	else if (strPubYearType == _T("m"))
	{
		if (strPubYear1 == _T("    "))
			strPubYear1.Empty();
	
		CTime time;
		time = CTime::GetCurrentTime();
		if (strPubYear2 == _T("9999"))
			strPubYear2.Format(_T("%d"), time.GetYear());

		strIndexItem = strPubYear1;
		if (strPubYear1 != strPubYear2)
		{
			if (strPubYear1.IsEmpty())
				strIndexItem = strPubYear2;
			else
				strIndexItem += _T("-") + strPubYear2;
		}
	}
	else if (strPubYearType == _T("p"))
	{
		strIndexItem = strPubYear2 + _T(" \\\\ ") + strPubYear1;
	}
	else if (strPubYearType == _T("r"))
	{
		strIndexItem = strPubYear1;
	}
	else if (strPubYearType == _T("n"))
	{
		return 0;
	}
	else if (strPubYearType == _T("q"))
	{
		strIndexItem = strPubYear1;
		if (strPubYear1 != strPubYear2)
		{
			strIndexItem += _T("-") + strPubYear2;
		}
	}
	else if (strPubYearType == _T("c"))
	{
	}
	else if (strPubYearType == _T("d"))
	{
	}
	else if (strPubYearType == _T("u"))
	{
	}

	return 0;
}

INT CIndex_Mono::GetIndex_ISBN(CString strIndexAlias, CString &strIndexItem)
{	
	CString strTemp1, strTemp2, strTemp3;
	POSITION posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("020"))
		{
			strTemp1.Empty();
			strTemp2.Empty();
			strTemp3.Empty();

			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');

				if (strSubfieldCode == _T("$a"))
				{
					strTemp2 = pSubfield->GetItemData();
				}
				else if (strSubfieldCode == _T("$g"))
				{
					strTemp2 = _T("-") + pSubfield->GetItemData();
				}
				else if (strSubfieldCode == _T("$z"))
				{
					strTemp2 = _T("-") + pSubfield->GetItemData();
				}

				strIndexItem += _T(" ") + strTemp1 + strTemp2 + strTemp3;
			}		
		}
	}

	return 0;
}

INT CIndex_Mono::GetIndex_ITITLE(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("245$a"), strIndexItem);
	if (ids < 0) return -1;
	
	strIndexItem.Replace(_T("[외]"),_T(""));
	strIndexItem.Replace(_T("[外]"),_T(""));
	strIndexItem.Replace(_T("[et al.]"),_T(""));
	
	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_KEYWORD(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("653$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_IPUBLISHER(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$b"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_ST_CODE(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("020$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_ST_ISSN(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("022$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_ST_STRN(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("027$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_ST_RNSTRN(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("088$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_CBN(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("015$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_CAN(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("017$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_SE_SHELF_CODE(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("049$f"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_CLASS_NO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$a"), strIndexItem);
	if (ids < 0) return -1;

	return 1;
}

INT CIndex_Mono::GetIndex_BOOK_CODE(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_VOL_CODE(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$c"), strIndexItem);
	if (ids < 0) return -1;

	return 1;
}

INT CIndex_Mono::GetIndex_VOL_CODE_DISP(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$c"), strIndexItem);
	if (ids < 0) return -1;

	return 1;
}

INT CIndex_Mono::GetIndex_CONTROL_NO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("001*"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_KDCP_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("0"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				MakeIndex(strIndexItem);
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_KDCCLASS(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	CArray <CString ,CString&> arrClass ;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("056$a"), strIndexItem,&arrClass);
	if (ids < 0) return -1;
    
	
    if(arrClass.GetSize () >1) 
	{
		strIndexItem.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strIndexItem.IsEmpty())
				strIndexItem = arrClass.GetAt (i) ;
			else
				strIndexItem += _T(" ") + arrClass.GetAt (i) ;
		}
	}
	return 1;
}

INT CIndex_Mono::GetIndex_DDC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
    CArray <CString ,CString&> arrClass ;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("082$a"), strIndexItem,&arrClass);
	if (ids < 0) return -1;
	
    if(arrClass.GetSize () >1) 
	{
		strIndexItem.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strIndexItem.IsEmpty())
				strIndexItem = arrClass.GetAt (i) ;
			else
				strIndexItem += _T(" ") + arrClass.GetAt (i) ;
		}
	}
	return 1;
}

INT CIndex_Mono::GetIndex_CEC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("2"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_CWC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("3"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_COC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("4"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_UDC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("080$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_NDC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("1"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_LC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T("7"))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_ETC_CLASS(CString strIndexAlias, CString &strIndexItem)
{
	CTag *pTag = NULL;
	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() == _T("085"))
		{
			if (pTag->GetFirstIndicator() == _T(" "))
			{
				CSubfield *pSubfield = pTag->FindSubfield(_T("$a"));
				if (pSubfield == NULL) continue;

				strIndexItem = pSubfield->GetItemData();
				return 0;
			}
		}
	}

	return -1;
}

INT CIndex_Mono::GetIndex_EDIT(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("250$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::Get_EDIT_INFO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("250$a"), strIndexItem);	
	if (ids < 0) return -1;
	
	return 0;
}

INT CIndex_Mono::GetIndex_IPUB_YEAR(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	CString sPubYear;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$c"), sPubYear);
	CString sYear;
	for ( INT j = 0 ; j < sPubYear.GetLength() ; j++ ) {
		CString sOneWord;
		sOneWord = sPubYear.Mid( j, 1 );
		if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
			sYear = sYear + sOneWord;			
		}		
	}
	if ( sYear.GetLength() == 4 ) {
		strIndexItem = sYear;
	}
	else {
		ids = m_pMarcMgr->GetItem(m_pMarc, _T("008@7-10"), strIndexItem);
		if (ids < 0) return -1;
	}

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_JANGJUNG(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("300$a"), strIndexItem);
	if (ids < 0) return -1;

	MakeIndex(strIndexItem);
	return 0;
}

INT CIndex_Mono::GetIndex_BookTitleInfo(CString strIndexAlias, CString &strIndexItem)
{
	CString strPunctuationMark;
	POSITION posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("245"))
		{
			INT n = 0;
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();

			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				strPunctuationMark.Empty();

				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');

				if (pSubfield->GetPunctuationMark() == _T(".") ||
					pSubfield->GetPunctuationMark() == _T(","))
				{
					strPunctuationMark = pSubfield->GetPunctuationMark() + _T(" ");
				}
				else if (!pSubfield->GetPunctuationMark().IsEmpty())
				{
					strPunctuationMark.Format(_T(" %s "), pSubfield->GetPunctuationMark());
				}

				if (n == 0)
					strPunctuationMark.Empty();
				
				strIndexItem += strPunctuationMark + pSubfield->GetItemData();
				
				n++;
			}		
		}
	}

	return INDEX_OUTPUT;
}

INT CIndex_Mono::GetIndex_TITLE_INFO(CString strIndexAlias, CString &strIndexItem)
{
	CString strPunctuationMark;
	POSITION posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("245"))
		{
			INT n = 0;
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				strPunctuationMark.Empty();
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$b") ||
					strSubfieldCode == _T("$n") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$p") )
				{
					
					if (pSubfield->GetPunctuationMark() == _T(".") ||
						pSubfield->GetPunctuationMark() == _T(","))
					{
						strPunctuationMark = pSubfield->GetPunctuationMark() + _T(" ");
					}
					else if (!pSubfield->GetPunctuationMark().IsEmpty())
					{
						strPunctuationMark.Format(_T(" %s "), pSubfield->GetPunctuationMark());
					}
					
					if (n == 0)
					strPunctuationMark.Empty();
				
					strIndexItem += strPunctuationMark + pSubfield->GetItemData();
					
					n++;
				}
			}		
		}
	}

	return INDEX_OUTPUT;
}

INT CIndex_Mono::GetIndex_AUTHOR_INFO(CString strIndexAlias, CString &strIndexItem)
{
	CString strPunctuationMark;
	POSITION posTag = GetMarc()->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		CTag *pTag = (CTag*)GetMarc()->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() == _T("245"))
		{
			INT n = 0;
			CString strFirstInd = pTag->GetFirstIndicator();
			CString strSecondInd = pTag->GetSecondIndicator();
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				strPunctuationMark.Empty();
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				CString strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				if (strSubfieldCode == _T("$d") ||
					strSubfieldCode == _T("$e") )
				{	
					
					if (pSubfield->GetPunctuationMark() == _T(".") ||
						pSubfield->GetPunctuationMark() == _T(","))
					{
						strPunctuationMark = pSubfield->GetPunctuationMark() + _T(" ");
					}
					else if (!pSubfield->GetPunctuationMark().IsEmpty())
					{
						strPunctuationMark.Format(_T(" %s "), pSubfield->GetPunctuationMark());
					}
					
					if (n == 0)
					strPunctuationMark.Empty();
				
					strIndexItem += strPunctuationMark + pSubfield->GetItemData();
					
					n++;
				}
			}		
		}
	}

	return INDEX_OUTPUT;
}

INT CIndex_Mono::GetIndex_PUB_INFO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$b"), strIndexItem);
	if (ids < 0) return -1;

	return INDEX_OUTPUT;
}

INT CIndex_Mono::GetIndex_PUB_YEAR_INFO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$c"), strIndexItem);
	if (ids < 0) return -1;

	return INDEX_OUTPUT;
}

INT CIndex_Mono::GetIndex_PUB_PLACE_INFO(CString strIndexAlias, CString &strIndexItem)
{
	INT ids;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$a"), strIndexItem);
	if (ids < 0) return -1;

	return INDEX_OUTPUT;
}
