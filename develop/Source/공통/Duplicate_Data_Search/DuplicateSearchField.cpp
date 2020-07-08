// DuplicateSearchField.cpp: implementation of the CDuplicateSearchField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuplicateSearchField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplicateSearchField::CDuplicateSearchField()
{
	m_UseYN = FALSE;	
	m_ArticleSearchYN = FALSE;
	m_SearchType = -1;
	m_CMAlias = _T("");
	m_DMAlias = _T("");
	m_ColumnName = _T("");
	m_SearchPlace = _T("");
	m_KeyName = _T("");
}

CDuplicateSearchField::~CDuplicateSearchField()
{

}

VOID	CDuplicateSearchField::SetUseYN(BOOL useYN)
{
	m_UseYN = useYN;
}

VOID	CDuplicateSearchField::SetArticleSearchYN(BOOL articleSearchYN)
{
	m_ArticleSearchYN = articleSearchYN;
}

VOID	CDuplicateSearchField::SetSearchType(INT searchType)
{
	m_SearchType = searchType;
}

VOID	CDuplicateSearchField::SetCMAlias(CString cmalias)
{
	m_CMAlias = cmalias;
}

VOID	CDuplicateSearchField::SetDMAlias(CString dmalias)
{
	m_DMAlias = dmalias;
}

VOID	CDuplicateSearchField::SetColumnName(CString columnname)
{
	m_ColumnName = columnname;
}

VOID	CDuplicateSearchField::SetSearchPlace(CString searchPlace)
{
	m_SearchPlace = searchPlace;
}

VOID	CDuplicateSearchField::SetKeyName(CString keyname)
{
	m_KeyName = keyname;
}

BOOL	CDuplicateSearchField::GetUseYN()
{
	return m_UseYN;
}

BOOL	CDuplicateSearchField::GetArticleSearchYN()
{
	return m_ArticleSearchYN;
}

INT		CDuplicateSearchField::GetSearchType()
{
	return m_SearchType;
}

CString CDuplicateSearchField::GetCMAlias()
{
	return m_CMAlias;
}

CString	CDuplicateSearchField::GetDMAlias()
{
	return m_DMAlias;
}

CString	CDuplicateSearchField::GetColumnName()
{
	return m_ColumnName;
}

CString	CDuplicateSearchField::GetSearchPlace()
{
	return m_SearchPlace;
}

CString	CDuplicateSearchField::GetKeyName()
{
	return m_KeyName;
}

CString CDuplicateSearchField::GetFieldDataString()
{
	CString tmpstr;
	tmpstr.Format(
		_T("%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d")
		,m_KeyName
		,m_SearchPlace
		,m_ColumnName
		,m_CMAlias
		,m_DMAlias
		,m_SearchType
		,m_ArticleSearchYN
		,m_UseYN);
	return tmpstr;
}

INT	CDuplicateSearchField::SetFieldDataString(CString strData)
{
	TCHAR * tmpstr = strData.GetBuffer(0);
	TCHAR seps[]   = _T("\t");
	TCHAR * token = NULL;
	CString strDatas[8];
	token = _tcstok( tmpstr, seps );
	strDatas[0] = token;
	INT i = 0;
	while( token != NULL && i < 7 )
	{
		 token = _tcstok( NULL, seps );
		 i += 1;
		 strDatas[i] = token;
	}
	
	// KOL.UDF.022 시큐어코딩 보완
	m_KeyName = strDatas[0];
	m_SearchPlace = strDatas[1];
	m_ColumnName = strDatas[2];
	m_CMAlias = strDatas[3];
	m_DMAlias = strDatas[4];
	m_SearchType = _ttoi(strDatas[5].GetBuffer(0));
	m_ArticleSearchYN = _ttoi(strDatas[6].GetBuffer(0));
	m_UseYN = _ttoi(strDatas[7].GetBuffer(0));
	/*
	m_KeyName = strDatas[1];
	m_SearchPlace = strDatas[2];
	m_ColumnName = strDatas[3];
	m_CMAlias = strDatas[4];
	m_DMAlias = strDatas[5];
	m_SearchType = _ttoi(strDatas[6].GetBuffer(0));
	m_ArticleSearchYN = _ttoi(strDatas[7].GetBuffer(0));
	m_UseYN = _ttoi(strDatas[8].GetBuffer(0));*/

	return 0;
}

INT CDuplicateSearchField::SetDefaultValue(
										   CString keyName, 
										   CString searchPlace, 
										   CString columnName, 
										   CString DMAlias, 
										   CString CMAlias, 
										   INT searchType, 
										   BOOL articleSearchYN, 
										   BOOL useYN
										   )
{
	m_KeyName = keyName;
	m_SearchPlace = searchPlace;
	m_ColumnName = columnName;
	m_DMAlias = DMAlias;
	m_CMAlias = CMAlias;
	m_SearchType = searchType;
	m_ArticleSearchYN = articleSearchYN;
	m_UseYN = useYN;		
	return 0;
}

INT CDuplicateSearchField::SetAliasAndValue(CString alias, CString value)
{
	if(alias.GetLength() == 0) return -1;
	if(alias == _T("SEARCH_PLACE"))
		this->SetSearchPlace(value);
	else if(alias == _T("COLUMN_NAME"))
		this->SetColumnName(value);
	else if(alias == _T("DM_ALIAS"))
		this->SetDMAlias(value);
	else if(alias == _T("CM_ALIAS"))
		this->SetCMAlias(value);
	else if(alias == _T("SEARCH_TYPE"))
		this->SetSearchType(_ttoi(value.GetBuffer(0)));
	else if(alias == _T("ARTICLE_SEARCH_YN"))
		this->SetArticleSearchYN(_ttoi(value.GetBuffer(0)));
	else if(alias == _T("USE_YN"))
		this->SetUseYN(_ttoi(value.GetBuffer(0)));
	return 0;
}
