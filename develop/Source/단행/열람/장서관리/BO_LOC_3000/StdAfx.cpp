// stdafx.cpp : source file that includes just the standard includes
//	BO_LOC_3000.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

INT GetRegistryInfo(const CString &strPath, const CString &strName, CString &strGetValue, HKEY _hKey)
{
	LONG    ids;
	HKEY    hKey;
	DWORD   dwReserved, dwType, dwSize = 0;
	TCHAR*    pszValue = NULL;
	dwReserved = 0;
	ids = RegOpenKey(_hKey, strPath, &hKey);
 	if(ERROR_SUCCESS == ids)
 	{
 		ids = RegQueryValueEx (hKey, strName, NULL, &dwType, (UCHAR*)pszValue, &dwSize);
 	}
 	if(ERROR_SUCCESS == ids)
 	{
 		pszValue = new TCHAR[dwSize];
 		ids = RegQueryValueEx (hKey, strName, NULL, &dwType, (UCHAR*)pszValue, &dwSize);
 	}
 	RegCloseKey(hKey);
 	if(NULL != pszValue)
 	{
 		strGetValue.Format(_T("%s"), pszValue);
 		delete []pszValue;
 		pszValue = NULL;
 	} 
 	return ids;
}

INT SetRegistryInfo(const CString &strPath, const CString &strName, CString &strSetValue, HKEY _hKey)
{
	HKEY hKey = NULL; 
	LONG lResult = RegOpenKeyEx(_hKey, strPath, 0, KEY_ALL_ACCESS, &hKey); 
	if(ERROR_SUCCESS != lResult)
	{
		lResult = RegCreateKey(_hKey, strPath, &hKey);
		if(ERROR_SUCCESS != lResult) return -5;
	}
	DWORD dwCount = (sizeof(TCHAR)*(strSetValue.GetLength()+1));
	TCHAR* pszValue = new TCHAR [dwCount];
	ZeroMemory(pszValue, dwCount);
	_tcscpy(pszValue, strSetValue.GetBuffer(0));
	strSetValue.ReleaseBuffer();
	lResult = RegSetValueEx(hKey, strName, NULL, REG_SZ, (LPBYTE)pszValue, dwCount);
	delete []pszValue;
	pszValue = NULL;
	if(ERROR_SUCCESS != lResult) return -6;
	RegCloseKey(hKey);
	return 0;
}

VOID TunningLog(TCHAR *text)
{
	if(NULL == text) return;
	SYSTEMTIME t;
	GetLocalTime(&t);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d"), t.wYear, t.wMonth, t.wDay);
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d.%03d] %s;"), t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds, text);
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));
	BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));
	if(FALSE == bFind && FALSE == bNoLogFind)
	{
		filename = filename + _T(".tunning");
		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		CFile file;
		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(TRUE != bIsOpen) return;
		file.SeekToEnd();
		{
			const INT nLen = msg.GetLength();
			TCHAR *pChar = new TCHAR[nLen+1];
			memset(pChar, NULL, nLen);
			pChar[nLen] = '\0';
			_tcscpy(pChar, (TCHAR*)msg.GetBuffer(0));
			for(INT nCnt=0; nCnt < nLen; nCnt++)
			{
				pChar[nCnt] ^= 0xAA;
			}
			file.WriteHuge(pChar, sizeof(TCHAR)*nLen);
			delete []pChar;
		}
		{
			CString strLine = _T("\r\n");	
			const INT LEN = strLine.GetLength();
			TCHAR *szLine = new TCHAR[LEN+1];
			memset(szLine, NULL, LEN);
			szLine[LEN] = '\0';
			_tcscpy(szLine, (TCHAR*)strLine.GetBuffer(0));
			for(INT nCnt=0; nCnt < LEN; nCnt++)
			{
				szLine[nCnt] ^= 0xAA;
			}
			file.Write(szLine, sizeof(TCHAR)*LEN);
			delete []szLine;
		}
		file.Close();
	}
	else if(FALSE == bNoLogFind)
	{
		filename = filename + _T(".txt");
		FILE* fp;
		fp = _tfopen (filename, _T("a+b"));
		if (fp == NULL)
		{
			return ;
		}
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		_ftprintf (fp, _T("%s\r\n"), msg);
		fclose(fp);
	}
}