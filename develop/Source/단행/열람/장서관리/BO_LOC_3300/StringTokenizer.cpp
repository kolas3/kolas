// StringTokenizer.cpp: implementation of the CStringTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringTokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringTokenizer::CStringTokenizer()
{
	m_psToken = NULL;

	m_sString = _T("");	
	m_sDelimiter = _T("");
	m_nDelimiterCnt = 0;
}

CStringTokenizer::CStringTokenizer( CString sString , CString sDelimiter )
{
	m_psToken = NULL;
	m_sString = sString;
	m_sDelimiter = sDelimiter;

	MakeToken();
}

CStringTokenizer::~CStringTokenizer()
{
	if( m_psToken != NULL ) delete []m_psToken;
}

INT CStringTokenizer::ReMake( CString sString , CString sDelimiter )
{
	if( m_psToken != NULL ) delete []m_psToken;

	m_psToken = NULL;
	m_sString = sString;
	m_sDelimiter = sDelimiter;

	MakeToken();

	return 0;
}

INT CStringTokenizer::GetTokenCnt()
{
	return m_nDelimiterCnt;
}

INT CStringTokenizer::GetTokenCnt( CString sString )
{
	INT nLength = sString.GetLength();

	INT nDelimiterCnt = 0;
	for( INT i = 0 ; i < nLength ; i++ )
	{
		if( m_sDelimiter.Compare(CString(sString.GetAt(i))) == 0 ) 
			nDelimiterCnt++;
	}

	return nDelimiterCnt;
}

CString CStringTokenizer::GetAt( INT nIndex )
{
	return m_psToken[nIndex];
}

INT CStringTokenizer::MakeToken()
{
	m_nDelimiterCnt = GetTokenCnt( m_sString );
	
	m_psToken = new CString[m_nDelimiterCnt];

	CString sString = m_sString;

	INT nIndex = 0;
	INT nFindIndex;
	while( (nFindIndex = sString.Find( m_sDelimiter , 0 )) > -1 )
	{
		m_psToken[nIndex] = sString.Mid( 0 , nFindIndex );
		sString = sString.Mid( nFindIndex+1 );

		nIndex++;
	}

	return 0;
}

