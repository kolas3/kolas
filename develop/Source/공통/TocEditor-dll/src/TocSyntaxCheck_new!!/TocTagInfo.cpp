// TocTagInfo.cpp: implementation of the CTocTagInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocTagInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
INT CTocTagInfo::TAG_TYPE_SINGLE	= 0;			// = 0 Single Tag
INT CTocTagInfo::TAG_TYPE_MULTI		= 1;			// = 1 Multi Tag
INT CTocTagInfo::TAG_TYPE_PAGEINFO	= 2;			// = 2 Page Info

INT CTocTagInfo::LEVEL0	= 0;					// = 0 Toc File
INT CTocTagInfo::LEVEL1	= 1;					// = 1 Level 1 
INT CTocTagInfo::LEVEL2	= 2;					// = 2 Level 2
INT CTocTagInfo::LEVEL3	= 3;					// = 3 Level 3
INT CTocTagInfo::LEVEL4	= 4;					// = 4 Level 4
INT CTocTagInfo::LEVEL5	= 5;					// = 5 Level 5 Page Info

CTocTagInfo::CTocTagInfo( CErrorQueue* pErrorQueue,
						  CString sSourceRecord	,
						  INT iTagType			,
						  INT iLevel			,
						  BOOLEAN bIsRepeatTag	)
{
	// =====================================================================================
	// Member의 초기화 ++
	// =====================================================================================
	m_pErrorQueue   = pErrorQueue;
	m_sSourceRecord	= sSourceRecord;
	m_iStartLine	= -1;
	m_iEndLine		= -1;
	m_sHeadTag		= _T("");
	m_sTailTag		= _T("");
	m_iTagType		= iTagType;
	m_iLevel		= iLevel;
	m_bIsRepeatTag	= bIsRepeatTag;
	m_iStartPage	= -1;							// 시작 페이지수
	m_iEndPage		= -1;							// 끝 페이지수
	m_iLength		= -1;							// Length
	// =====================================================================================
	// Member의 초기화 --
	// =====================================================================================
}

CTocTagInfo::~CTocTagInfo()
{

}

BOOLEAN CTocTagInfo::IsRepeatTag()
{
	return m_bIsRepeatTag;
}

CString CTocTagInfo::GetHeadTag()
{
	return m_sHeadTag;
}

INT CTocTagInfo::GetLevel()
{
	return m_iLevel;
}

INT CTocTagInfo::GetLine()
{
	return m_iStartLine;
}

VOID CTocTagInfo::SetHeadTag(INT iLine, CString sTag)
{
	m_iStartLine = iLine;
	m_sHeadTag = sTag;
}

VOID CTocTagInfo::SetTailTag(INT iLine, CString sTag)
{
	m_iEndLine = iLine;
	m_sTailTag = sTag;
}

INT CTocTagInfo::CheckTocSyntax()
{
	return 0;
}

BOOLEAN CTocTagInfo::IsCurrentPairTag(CString sTag)
{
	if(m_iTagType != TAG_TYPE_MULTI) {
		return FALSE;
	}
	
	if(sTag.Compare(_T("/") + m_sHeadTag) == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

INT CTocTagInfo::GetType()
{
	return m_iTagType;
}

BOOLEAN CTocTagInfo::IsControlId()
{
	if(m_sHeadTag.Compare(_T("id")) == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

CString CTocTagInfo::GetValue()
{
	if(m_iTagType == TAG_TYPE_PAGEINFO) {
		return m_sSourceRecord;
	}

	INT iCurIdx = m_sSourceRecord.Find(_T(">"));
	if(iCurIdx >= 0) {
		CString sReturn;
		sReturn = m_sSourceRecord.Right(m_sSourceRecord.GetLength() - iCurIdx -1);
		sReturn.TrimLeft();
		sReturn.TrimRight();

		return sReturn;
	} else {
		return m_sSourceRecord;
	}
}

BOOL CTocTagInfo::IsPageCheck()
{
	CString tag= m_sHeadTag;
	tag.MakeUpper();
	//if (tag==_T("TABLE") || tag==_T("FIGURE") )
	if (tag==_T("TABLE"))
	{
		return FALSE;
	}
	return TRUE;

}
