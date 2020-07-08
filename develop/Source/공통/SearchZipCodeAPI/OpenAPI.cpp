// NaverOpenAPI.cpp: implementation of the COpenAPI class.
//
//////////////////////////////////////////////////////////////////////

// 2012.11.22 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차	

#include "stdafx.h"
#include "OpenAPI.h"
#include "afxinet.h"

#include "..\\..\\..\\..\\include\\ESL\\CharConvert.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenAPI::COpenAPI()
{
	m_nTotal = 0;
}

COpenAPI::~COpenAPI()
{

}

CString COpenAPI::GetErrorMessage()
{
	return m_strErrorMessage;
}

void COpenAPI::SetErrorMessage(const LPCTSTR lpszErrorMessage)
{
	m_strErrorMessage = lpszErrorMessage;
	TRACE(_T("%s\r\n"), lpszErrorMessage);
}



INT COpenAPI::RequestURL(CString strRequestURL, BOOL bUnicodeResult/*=TRUE*/)
{
	CharConvert Convert;
	INT ids;

	CInternetFile* p_InternetFile;
	CInternetSession session;
	CMemFile *memfile = new CMemFile();

	//UNICODE -> UTF-8
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_UTF8, 0, strRequestURL, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_UTF8, 0, strRequestURL, -1, szBuff, nSize, NULL, NULL);
	}	
	strRequestURL = (CString)szBuff;

	try
	{
		p_InternetFile = (CInternetFile*)session.OpenURL(strRequestURL);// 페이지접근
	}
	catch(CInternetException* e)
	{
		TCHAR szError[MAX_PATH];
		e->GetErrorMessage(szError, MAX_PATH);
		SetErrorMessage(szError);

		delete memfile;
		memfile = NULL;

		return e->m_dwError;
	}

	m_strSearchResult.Empty();

	CHAR* lpText = NULL;
	CHAR* lpRead = new CHAR[1024];
	INT   nReadLen = 0;
	do
	{
		// 16/02/12 김혜영 : 2015년 공공도서관 시스템 개선 - 긴데이터를 가져오지 못하는 문제
/*//REM--------------------------------------------------------------------------
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   원인불명으로 데이터를 끝까지 가져오지 못하는 현상 해결을 위해서		
		Sleep(100);
*///END--------------------------------------------------------------------------
		ZeroMemory(lpRead, 1024);
		nReadLen = p_InternetFile->Read(lpRead, 1024);  //<<-- 여기 FILE 클래스 삽입
		
		INT nTextLen    = (lpText) ? strlen(lpText) : 0;
		INT nNewTextLen = nTextLen + nReadLen;
		CHAR* lpNewText = new CHAR[nNewTextLen+1];

		if (lpText) memcpy(lpNewText, lpText, nTextLen);
		if (lpRead) memcpy(lpNewText+nTextLen, lpRead, nReadLen);
		lpNewText[nNewTextLen] = '\0';

		// SWAP MEMORY
		if (lpText) delete [] lpText;
		lpText = lpNewText;
	}
	// 16/02/12 김혜영 : 2015년 공공도서관 시스템 개선 - 긴데이터를 가져오지 못하는 문제
/*// BEFORE ----------------------------------------------------------------------------------
		while (nReadLen == 1024);
*/// AFTER -----------------------------------------------------------------------------------
		while (nReadLen != 0);
//*/ END -------------------------------------------------------------------------------------

	delete [] lpRead;

	// 여기서 문자셋 변환
	if (bUnicodeResult)
	{
		INT	nByte;
		TCHAR* szBuffer = NULL;
		ids = Convert.UTF8ToUnicode(lpText, &szBuffer, &nByte);

		m_strSearchResult.Format(_T("%s"), szBuffer);
		if (szBuffer) delete [] szBuffer;
	}
	else
	{
		m_strSearchResult.Format(_T("%s"), lpText);
	}

	delete [] lpText;


//	BYTE* pRead[1024];
//	ZeroMemory(pRead, 1024);
//	while ( 1024 == p_InternetFile->Read(pRead, 1024) )
//	{
//		strReadText += strReadLine;
//	}
/*
	LPCSTR lpszText = (LPCSTR)strReadText.GetBuffer(0);
	INT nLength     = strlen(lpszText);

	INT nWideLength = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, lpszText, sizeof(CHAR)*nLength, NULL, NULL);
	TCHAR* szBuffer = new TCHAR[nWideLength+1];

	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, lpszText, sizeof(CHAR)*nLength, szBuffer, nWideLength);

	m_strSearchResult.Format(_T("%s"), szBuffer);
	delete [] szBuffer;
	szBuffer = NULL;
*/



/*	CHAR buffer[10240];
	memset(buffer, 0x00, sizeof(buffer));
	if( 0 < p_InternetFile->Read(buffer, sizeof(buffer)) )
	{
		INT i = 0;
		INT nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, sizeof(buffer), NULL, NULL);
		TCHAR* szBuffer = new TCHAR[nLen+1];
		memset(szBuffer, 0x00, sizeof(szBuffer));
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, sizeof(buffer), szBuffer, nLen);
		
		INT		nByte;
		ids = Convert.UTF8ToUnicode(buffer, &szBuffer, &nByte);
		if (ids)
		{
			free(buffer);
		}

		m_strSearchResult.Format(_T("%s"), szBuffer);
		delete []szBuffer;
		szBuffer = NULL;
	}*/

	session.Close();
	p_InternetFile->Close();
	if(NULL != p_InternetFile)
	{
		delete p_InternetFile;
		p_InternetFile = NULL;
	}
 	memfile->Close();
 	if(NULL != memfile)
 	{
 		delete memfile;
 		memfile = NULL;
	}
	return 0;
}
