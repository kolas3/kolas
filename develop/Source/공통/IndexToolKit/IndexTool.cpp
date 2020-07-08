// IndexTool.cpp: implementation of the CIndexTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexTool.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexTool::CIndexTool()
{
}

CIndexTool::~CIndexTool()
{

}


//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////
VOID CIndexTool::Init(CMarcMgr *pMarcMgr, CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarcMgr == NULL) return;
	if (pMarc == NULL) return;

	CString strMarcGroup;

	strMarcGroup = pMarcMgr->GetMarcGroup();
	SetMarcGroup(strMarcGroup);

	pMarcMgr->SortMarc(pMarc);
	
	m_MonoIndex.SetMarcMgr(pMarcMgr);
	m_MonoIndex.SetMarc(pMarc);
	
	m_NonBookIndex.SetMarcMgr(pMarcMgr);
	m_NonBookIndex.SetMarc(pMarc);

	m_SerialIndex.SetMarcMgr(pMarcMgr);
	m_SerialIndex.SetMarc(pMarc);

	EFS_END
}

INT CIndexTool::GetIndex(CString strIndexAlias, CString &strIndexItem)
{	
	EFS_BEGIN

	INT ids = -1;

	strIndexItem.Empty();

	///////////////////////////////////////////////////////////////////
	
	if (GetMarcGroup() == _T("단행") || GetMarcGroup()==_T("통합마크")) {
		ids = m_MonoIndex.GetIndex(strIndexAlias, strIndexItem);
	}
	else if (GetMarcGroup().Find(_T("비도서")) >= 0)
		ids = m_NonBookIndex.GetIndex(strIndexAlias, strIndexItem);
	else if (GetMarcGroup() == _T("연속"))
		ids = m_SerialIndex.GetIndex(strIndexAlias, strIndexItem);
	else 
		return -1;
			
	///////////////////////////////////////////////////////////////////

	if (ids < 0)
		return ids;

	CIndex::TrimAll(strIndexItem);

	if (ids == INDEX_SEARCH)
	{
		m_Index.RemoveSpecialChar(strIndexItem);
		m_Index.ConvertHanjaToHangul(strIndexItem);
	}
	else if (ids == INDEX_OUTPUT)
	{
	}	
	
	if ( strIndexAlias.Find(_T("사항"), 0) < 0 ) 
	{
		m_Index.DupIndexRemove(strIndexItem);
	}

	return 0;

	EFS_END
	return -1;

}

