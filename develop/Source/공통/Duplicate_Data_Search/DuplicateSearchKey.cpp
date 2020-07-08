// DuplicateSearchKey.cpp: implementation of the CDuplicateSearchKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuplicateSearchKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplicateSearchKey::CDuplicateSearchKey()
{

}

CDuplicateSearchKey::~CDuplicateSearchKey()
{
	this->DeleteAllList();
}

INT CDuplicateSearchKey::SetSearchKeys(CString filename)
{
	if(filename.GetLength() == 0)
		filename = _T(".\\duplicate_search_key.txt");
	
	INT ids, i ;
	ids = i = 0;
	CString tmpstr, tmpvalue, tmpalias;

	CStdioFile file;
	CDuplicateSearchField * psfield = NULL;

	if(!m_SearchKeys.IsEmpty())
		this->DeleteAllList();

	if ( file.Open(filename, CFile::modeRead | CFile::typeBinary) == FALSE ) 
	{
		ids = SetDefaultSearchKeys();
		if(ids < 0) return -1000;
		return 0;
	}
	else
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while(file.ReadString(tmpstr))
		{
			tmpstr.TrimLeft();	tmpstr.TrimRight();
			ids = CheckString(tmpstr);
			if(ids == -1) continue;
			else if(ids == 0) 
			{
				if(psfield) 
					m_SearchKeys.AddTail((CObject*)psfield);
				psfield = new CDuplicateSearchField();
				tmpvalue = GetKeyName(tmpstr);
				if(tmpvalue.GetLength() == 0)
				{
					delete psfield;
					psfield = NULL;
					continue;
				}
				psfield->SetKeyName(tmpvalue);
			}
			else
			{
				if(!psfield) continue;
				ids = GetAliasAndValue(tmpalias, tmpvalue, tmpstr);
				if(tmpalias.GetLength() == 0 || tmpvalue.GetLength() == 0) continue;
				psfield->SetAliasAndValue(tmpalias, tmpvalue);				
			}
		}
		if(psfield)
			m_SearchKeys.AddTail((CObject*)psfield);
	}

	if(m_SearchKeys.GetCount() == 0)
		this->SetDefaultSearchKeys();
	
	file.Close();
	return 0;
}

INT CDuplicateSearchKey::CheckString(CString str)
{
	//공백문자면 return
	if(str.GetLength() == 0) return -1;
	TCHAR firstchar = str.GetAt(0);

	if(firstchar == '[') // '['으로 시작하면 0리턴 : 새로운 CDuplicateSearchField를 생성한다.
		return 0;
	else if(firstchar == '.') // '.'으로 시작하면 해당 라인은 주석
		return -1;
	return 1; // 이외의 라인은 CDuplicateSearchField의 각 속성값들이다.
}

CString CDuplicateSearchKey::GetKeyName(CString str)
{
	if(str.GetLength() == 0) return _T("");
	INT firstidx, lastidx;
	firstidx = lastidx = 0;
	firstidx = str.Find('[');
	lastidx = str.Find(']');
	CString value = str.Mid(firstidx+1, lastidx - firstidx - 1);
	value.TrimLeft();	value.TrimRight();
	return value;
}

INT CDuplicateSearchKey::GetAliasAndValue(CString &alias, CString &tmpvalue, CString str)
{
	if(str.GetLength() == 0) return -1;
	INT sepidx = str.Find('=');
	INT maxidx = str.GetLength() - 1;
	alias = str.Mid(0, sepidx);
	alias.TrimLeft();	alias.TrimRight();
	tmpvalue = str.Mid(sepidx+1, maxidx);
	tmpvalue.TrimLeft();	tmpvalue.TrimRight();
	return 0;
}

INT CDuplicateSearchKey::GetSearchKeys(CString filename)
{
	if(filename.GetLength() == 0)
		filename = _T(".\\duplicate_search_key.txt");
	
	INT ids, i ;
	ids = i = 0;
	CString tmpstr, tmpstr1;

	CStdioFile file;
	CDuplicateSearchField * psfield = NULL;
	if ( file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == FALSE ) 
	{
		filename += _T(" 파일을 생성할수 없습니다.");
		AfxMessageBox(filename, MB_ICONSTOP);
		return -1;
	}
	else
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		POSITION pos = m_SearchKeys.GetHeadPosition();
		while(pos != NULL)
		{
			psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
			//tmpstr = psfield->GetFieldDataString();
			tmpstr.Format(_T("[%s]"), psfield->GetKeyName());
			tmpstr += _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("SEARCH_PLACE=");
			tmpstr += psfield->GetSearchPlace() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("COLUMN_NAME=");
			tmpstr += psfield->GetColumnName() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("DM_ALIAS=");
			tmpstr += psfield->GetDMAlias() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("CM_ALIAS=");
			tmpstr += psfield->GetCMAlias() + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("SEARCH_TYPE=");
			tmpstr1.Format(_T("%d"),psfield->GetSearchType());
			tmpstr += tmpstr1 + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("ARTICLE_SEARCH_YN=");
			tmpstr1.Format(_T("%d"), psfield->GetArticleSearchYN());
			tmpstr += tmpstr1 + _T("\r\n");
			file.WriteString(tmpstr);
			tmpstr = _T("USE_YN=");
			tmpstr1.Format(_T("%d"), psfield->GetUseYN());
			tmpstr += tmpstr1 + _T("\r\n") + _T("\r\n");
			file.WriteString(tmpstr);			
		}

		/*
		// 제적자료 포함여부 옵션
		if ( m_bIncludeDiscardedBook ) tmpstr.Format(_T("[제적자료포함]\nUSE_YN=0\n"));
		else tmpstr.Format(_T("[제적자료포함]\nUSE_YN=1\n"));	
		file.WriteString(tmpstr);			
		*/
	}

	file.Close();
	
	return 0;
}

INT CDuplicateSearchKey::SetDefaultSearchKeys()
{
	CDuplicateSearchField * psfield = NULL;
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("본표제"), _T("IDX_BO_TBL"), _T("IDX_TITLE"), _T("IBS_본표제"), _T("IBS_본표제"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	psfield = new CDuplicateSearchField();
//	psfield->SetDefaultValue(_T("총서명"), _T("IDX_BO_TBL"), _T("IDX_TITLE"), _T("UDF_총서명"), _T("UDF_총서명"), 0, FALSE, TRUE);
	psfield->SetDefaultValue(_T("총서명"), _T("IDX_BO_TBL"), _T("IDX_ALL_ITEM"), _T("UDF_총서명"), _T("UDF_총서명"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	//===================================================
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("저작자"), _T("IDX_BO_TBL"), _T("IDX_AUTHOR"), _T("IBS_저작자"), _T("IBS_저작자"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("발행자"), _T("IDX_BO_TBL"), _T("IDX_PUBLISHER"), _T("IBS_발행자"), _T("IBS_발행자"), 0, FALSE, TRUE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("발행년"), _T("IDX_BO_TBL"), _T("PUB_YEAR"), _T("IBS_발행년"), _T("IBS_발행년"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("낱권ISBN"), _T("IDX_BO_TBL"), _T("ST_CODE"), _T("BB_낱권_ISBN"), _T("BB_낱권_ISBN"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("매체구분"), _T("IDX_BO_TBL"), _T("MEDIA_CODE"), _T("BB_매체구분"), _T("BB_매체구분"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("편권차"), _T("IDX_BO_TBL"), _T("IDX_VOL"), _T("BB_편권차"), _T("BB_편권차"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("권서명"), _T("IDX_BO_TBL"), _T("IDX_VOL_TITLE"), _T("BB_권서명"), _T("BB_권서명"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("별치기호"), _T("IDX_BO_TBL"), _T("IDX_SE_SHELF_CODE"), _T("BB_별치기호"), _T("BB_별치기호"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	psfield = new CDuplicateSearchField();
	psfield->SetDefaultValue(_T("등록구분"), _T("IDX_BO_TBL"), _T("REG_CODE"), _T("BB_등록구분"), _T("BB_등록구분"), -1, FALSE, FALSE);
	m_SearchKeys.AddTail((CObject*)psfield);
	return 0;
}

INT CDuplicateSearchKey::GetFieldKeySettingInfo(CString fieldName, BOOL *pUseYN, INT *pSearchType, BOOL *pArticleSearchYN)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		*pUseYN = psfield->GetUseYN();
		*pSearchType = psfield->GetSearchType();
		if(pArticleSearchYN)
			*pArticleSearchYN = psfield->GetArticleSearchYN();
	}
	else
	{
		*pUseYN = FALSE;
		*pSearchType = -1;
		if(pArticleSearchYN)
			*pArticleSearchYN = FALSE;
	}
	return 0;
}

INT CDuplicateSearchKey::SetFieldKeySettingInfo(CString fieldName, BOOL useYN, INT searchType, BOOL articleSearchYN)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		psfield->SetUseYN(useYN);
		psfield->SetSearchType(searchType);
		psfield->SetArticleSearchYN(articleSearchYN);
	}
	else 
	{
		psfield->SetUseYN(FALSE);
		psfield->SetSearchType(-1);
		psfield->SetArticleSearchYN(FALSE);
	}
	return 0;
}

INT CDuplicateSearchKey::DeleteAllList()
{
	CDuplicateSearchField* pa;
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return 0;

	for(INT i=0 ; i<count ; i++)
	{
		pa = (CDuplicateSearchField*)m_SearchKeys.RemoveHead(); 
		if(pa) delete pa;
	}
	m_SearchKeys.RemoveAll();
	return(count);
}

INT CDuplicateSearchKey::GetSearchFieldKeySettingInfo(CString fieldName, CString &searchPlace, CString &searchField)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		searchPlace = psfield->GetSearchPlace();
		searchField = psfield->GetColumnName();
	}
	else
		return -1000;
	return 0;
}

INT CDuplicateSearchKey::GetDMCMAlias(CString fieldName, CString &DMAlias, CString &CMAlias)
{
	INT count = m_SearchKeys.GetCount();
	if(count == 0) return -1;
	CDuplicateSearchField * psfield = NULL;
	POSITION pos = m_SearchKeys.GetHeadPosition();
	for(INT i=0 ; i<count ; i++)
	{	
		psfield = (CDuplicateSearchField *)m_SearchKeys.GetNext(pos);
		if(psfield == NULL) continue;
		if(psfield->GetKeyName() == fieldName)
			break;		
	}
	if(i < count)
	{
		DMAlias = psfield->GetDMAlias();
		CMAlias = psfield->GetCMAlias();
	}
	else
		return -1000;
	return 0;
}
