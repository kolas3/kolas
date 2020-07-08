// StdioFileEx.cpp: implementation of the CStdioFileEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStdioFileEx::CStdioFileEx(LPCTSTR lpszFileName, UINT nOpenFlags)
: CStdioFile(lpszFileName, nOpenFlags)
{
	m_sCommentDelimeter = _T("..");
}

CStdioFileEx::CStdioFileEx()
{
	m_sCommentDelimeter = _T("..");
}

CStdioFileEx::~CStdioFileEx()
{

}

BOOL CStdioFileEx::ReadString(CString &rString)
{
	BOOLEAN bIsReturn = TRUE;
	CString sRecord;
	bIsReturn = CStdioFile::ReadString(sRecord);

	// =================================================================================================
	// 주석 제거
	// =================================================================================================
	INT iCurIdx = sRecord.Find(m_sCommentDelimeter);
	if(iCurIdx >= 0) {
		sRecord = sRecord.Left(iCurIdx);
	}

	sRecord.TrimLeft();
	sRecord.TrimRight();

	rString = sRecord;

	return bIsReturn;
}

VOID CStdioFileEx::SetCommentDelimeter(CString sDelimeter)
{
	m_sCommentDelimeter = sDelimeter;
}
