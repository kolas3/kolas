// Parse.cpp: implementation of the CParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParse::CParse(CString str,BOOLEAN bIsSubString /*= FALSE*/, INT iIdx)
{
	m_sSource = str;
	m_sSource.TrimLeft();
	m_sSource.TrimRight();
	m_sIdx = iIdx;
	m_sDelimeter = _T(" ");
	m_bIsSubString = bIsSubString;
	m_iDelimeterLength = 1;
}

CParse::~CParse()
{
	
}

CString CParse::NextToken()
{
	INT iIdx;
	
	if(m_bIsSubString)
		iIdx = m_sSource.Find(m_sDelimeter);		// zero-base index
	else
		iIdx = m_sSource.FindOneOf(m_sDelimeter);		// zero-base index

	CString retToken;
	if(iIdx == -1)
	{
		retToken=m_sSource;
		m_sSource.Empty();
		return retToken;
	}
	retToken = m_sSource.Left(iIdx);
	INT iNextLength = m_sSource.GetLength();
	if(iNextLength != 0) {
		if(m_bIsSubString) 
			m_sSource = m_sSource.Right(iNextLength - iIdx - m_iDelimeterLength);
		else
			m_sSource = m_sSource.Right(iNextLength - iIdx - 1);
	}
	
	if(!m_bIsSubString) {
		m_sSource.TrimLeft();
		m_sSource.TrimRight();

		retToken.TrimLeft();
		retToken.TrimRight();
	}

	m_sIdx--;
	return retToken;
	
}

BOOL CParse::HasMoreToken()
{
	if(m_sSource.IsEmpty())
		return FALSE;
	else
	{
		if(IsOnlyDelimeter(m_sSource) && !m_bIsSubString) {
			return FALSE;
		} else {
			if(m_sIdx == 0)
				return FALSE;
			else
				return TRUE;
		}
	}
}

VOID CParse::SetDelimeter(CString sDelimeter)
{
	m_sDelimeter = sDelimeter;
	m_iDelimeterLength = m_sDelimeter.GetLength();
}

BOOLEAN CParse::IsOnlyDelimeter(CString sSource)
{
	INT iLength = sSource.GetLength();
	INT iIdx;
	for(INT i = 0 ; i < iLength ; i ++)
	{
		iIdx = m_sDelimeter.Find(sSource.GetAt(i));
		if(iIdx == -1)
			return FALSE;
	}
	return TRUE;
}
