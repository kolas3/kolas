// MarcInfoConversionMgr.cpp: implementation of the CMarcInfoConversionMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcInfoConversionMgr::CMarcInfoConversionMgr()
{
	m_pMarcMgr = NULL;
}

CMarcInfoConversionMgr::~CMarcInfoConversionMgr()
{

}

INT CMarcInfoConversionMgr::GetSeriesStatement(CMarc *pMarc, CString &strSeriesStatement, BOOL bPunctuationSymbol/*=FALSE*/)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;
	
	CString strTmpData;
	m_pMarcMgr->GetItem(pMarc, _T("490$a"), strTmpData);
	if(!strTmpData.IsEmpty())
	{
		//2018-11-13 KYJ
		//JOB.2018.0096 총사사항 표시할때 490태그도 총서번호 지원
		/*//=================================================================
		if(bPunctuationSymbol)
			strSeriesStatement = _T(" .- ( ") + strTmpData + _T(" )");
		else
			strSeriesStatement = strTmpData;
		*///=================================================================
		strSeriesStatement = strTmpData;

		m_pMarcMgr->GetItem(pMarc, _T("490$v"), strTmpData);
		if(!strTmpData.IsEmpty())
		{
			if(strSeriesStatement.IsEmpty())
				strSeriesStatement = strTmpData;
			else
				strSeriesStatement += _T(" ; ") + strTmpData;
		}

		if(!strSeriesStatement.IsEmpty()) 
		{
			if(bPunctuationSymbol)
				strSeriesStatement = _T(" .- (") + strSeriesStatement + _T(")");
		}
		//===================================================================
	}
	else 
	{
		m_pMarcMgr->GetItem(pMarc, _T("440$a"), strTmpData);
		if(!strTmpData.IsEmpty())
		{
			strSeriesStatement = strTmpData;
		}
		m_pMarcMgr->GetItem(pMarc, _T("440$v"), strTmpData);
		if(!strTmpData.IsEmpty())
		{
			if(strSeriesStatement.IsEmpty())
				strSeriesStatement = strTmpData;
			else
				strSeriesStatement += _T(" ; ") + strTmpData;
		}

		if(!strSeriesStatement.IsEmpty()) 
		{
			if(bPunctuationSymbol)
				strSeriesStatement = _T(" .- (") + strSeriesStatement + _T(")");
		}
	}
	return 0;
}

INT CMarcInfoConversionMgr::GetEditionStatement(CMarc *pMarc, CString &strEditionStatement, BOOL bPunctuationSymbol/*=FALSE*/)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;
	CString strTmpData;
	m_pMarcMgr->GetItem( pMarc, _T("250$a"), strTmpData );
	if(!strTmpData.IsEmpty())
	{
		if(bPunctuationSymbol)
			strEditionStatement = _T(" .- ") + strTmpData;
		else
			strEditionStatement = strTmpData;
	}
	return 0;
}

INT CMarcInfoConversionMgr::GetCurrencyCode(CMarc * pMarc, CString &strCurrencyCode)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;
	CString strSource;
	INT ids = m_pMarcMgr->GetItem(pMarc, _T("950$b"), strSource);

/*	2004.09.21
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		}
		else if(ch == '.')
			szBufDigit[nBufDigitSize++]  = ch;
		else 
		{
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}

	szBufNotDigit[nBufNotDigitSize] = '\0';
	strCurrencyCode.Format(_T("%s"), szBufNotDigit);
	strCurrencyCode.TrimLeft();			strCurrencyCode.TrimRight();
	strCurrencyCode = strCurrencyCode.Left(3);
*/

	TCHAR * strData = strSource.GetBuffer(0);
	INT cnt = strSource.GetLength();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strData[i] >= '0' && strData[i] <= '9')
			break;
	}
	
	strCurrencyCode = strSource.Mid(0, i);
	
	return 0;
}

INT CMarcInfoConversionMgr::GetPriceInfo(CMarc * pMarc, CString &strPriceInfo)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;
	CString strSource;
	INT ids = m_pMarcMgr->GetItem(pMarc, _T("950$b"), strSource);
	
/*	2004.09.21
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		}
		else if(ch == '.')
			szBufDigit[nBufDigitSize++]  = ch;
		else 
		{
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}
	szBufDigit[nBufDigitSize] = '\0';
	strPrice.Format(_T("%s"), szBufDigit);
	strPrice.TrimLeft();		strPrice.TrimRight();
	CString strTmpData = strPrice.Left(strPrice.Find(_T(".")));
	if(strTmpData.GetLength() > 8) strTmpData = strPrice.Left(8);
	strPriceInfo = strTmpData + strPrice.Mid(strPrice.Find(_T(".")));
*/
	
	TCHAR * strData = strSource.GetBuffer(0);
	INT cnt = strSource.GetLength();
	for(INT i=0 ; i<cnt ; i++)
	{
		// 2005-01-19 송승민
		// 숫자가 아닌 애들은 화폐구분으로 인식하여 잘라내고 숫자만 모은다.
		if( (strData[i] >= '0' && strData[i] <= '9') || (strData[i] == '.') )
			strPriceInfo += strData[i];
	}
	
	// strPrice = strSource.Mid(i, cnt);

	return 0;
}

INT CMarcInfoConversionMgr::GetTitleStatement(CMarc * pMarc, CString &strTitleInfo)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;

	CString strTmpData;
	INT ids = GetEditionStatement(pMarc, strTmpData, TRUE);
	if(ids < 0) return -1;
	strTitleInfo += strTmpData;
	strTmpData.Empty();
	ids = GetSeriesStatement(pMarc, strTmpData, TRUE);
	if(ids < 0) return -1;
	strTitleInfo += strTmpData;
	return 0;
}

INT CMarcInfoConversionMgr::GetPublishPlace(CMarc * pMarc, CString &strPubPlace)
{
	if(pMarc == NULL || m_pMarcMgr == NULL) return -1;
	
	CString strTmpData;
	m_pMarcMgr->GetItem(pMarc, _T("260$a"), strTmpData);
	strPubPlace = strTmpData;
	return 0;
}
