#include "stdafx.h"
#include "RegKey.h"

CRegKeyMgr::CRegKeyMgr()
{
	m_hKey = NULL;
}

CRegKeyMgr::~CRegKeyMgr()
{
	Close();
}

LONG CRegKeyMgr::Open(HKEY hKeyRoot, LPCTSTR pszPath)
{
	DWORD dw;

	m_sPath = pszPath;

	// The RegCreateKeyEx function creates the specified key.
	// If the key already exists in the registry, the function opens it.
	return RegCreateKeyEx(hKeyRoot, pszPath, 0L, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dw);
}

VOID CRegKeyMgr::Close()
{
	if (m_hKey)
	{
		// The RegCloseKey function releases a handle to the specified key. 
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

LONG CRegKeyMgr::Write(LPCTSTR pszKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	// The RegSetValueEx function sets the data and type of a specified value under a registry key. 
	return RegSetValueEx(m_hKey, pszKey, 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
}

LONG CRegKeyMgr::Write(LPCTSTR pszKey, LPCTSTR pszData)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, _tcsclen(pszData), FALSE));

	return RegSetValueEx(m_hKey, pszKey, 0L, REG_SZ, (CONST BYTE*) pszData, _tcsclen(pszData) + 1);
}

LONG CRegKeyMgr::Write(LPCTSTR pszKey, const BYTE* pData, DWORD dwLength)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pData && dwLength > 0);
	ASSERT(AfxIsValidAddress(pData, dwLength, FALSE));

	return RegSetValueEx(m_hKey, pszKey, 0L, REG_BINARY, pData, dwLength);
}

LONG CRegKeyMgr::Read(LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;

	// The RegQueryValueEx function retrieves the type and data
	// for a specified value name associated with an open registry key. 
	LONG lRet = RegQueryValueEx(m_hKey, (LPCTSTR) pszKey, NULL, &dwType, (BYTE*) &dwDest, &dwSize);
	if (lRet == ERROR_SUCCESS)
		dwVal = dwDest;

	return lRet;
}

LONG CRegKeyMgr::Read(LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize= 200;
	TCHAR  string[200];

	LONG lReturn = RegQueryValueEx(m_hKey, (LPCTSTR) pszKey, NULL, &dwType, (BYTE*) string, &dwSize);
	if (lReturn == ERROR_SUCCESS)
		sVal = string;

	return lReturn;
}

LONG CRegKeyMgr::Read(LPCTSTR pszKey, BYTE* pData, DWORD& dwLen)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;

	return RegQueryValueEx(m_hKey, (LPCTSTR) pszKey, NULL, &dwType, pData, &dwLen);
}

LONG CRegKeyMgr::GetEnumKeys(HKEY hKeyRoot, LPCTSTR pszPath, CStringArray& strArray)
{
	INT  nIndex;
	long lRetCode;
	TCHAR szValue[MAX_PATH];

	if ((lRetCode = Open(hKeyRoot, pszPath)) != ERROR_SUCCESS)
		return lRetCode;

	strArray.RemoveAll();

	for (nIndex = 0, lRetCode = ERROR_SUCCESS; lRetCode == ERROR_SUCCESS; nIndex++)
	{
		// The RegEnumKey function enumerates subkeys of the specified open registry key.
		// The function retrieves the name of one subkey each time it is called.
		// This function is provided for compatibility with Windows version 3.1. 
		// Win32-based applications should use the RegEnumKeyEx function. 
		lRetCode = RegEnumKey(m_hKey, nIndex, szValue, MAX_PATH);

		if (lRetCode == ERROR_SUCCESS)
			strArray.Add(szValue);
	}

	Close();

	return ERROR_SUCCESS;
}

LONG CRegKeyMgr::GetEnumValues(HKEY hKeyRoot, LPCTSTR pszPath, CStringArray& strArray)
{
	INT   nIndex;
	long  lRetCode;
	TCHAR  szValue[MAX_PATH];
	DWORD dwValue;

	if ((lRetCode = Open(hKeyRoot, pszPath)) != ERROR_SUCCESS)
		return lRetCode;

	strArray.RemoveAll();

	for (nIndex = 0, lRetCode =ERROR_SUCCESS; lRetCode == ERROR_SUCCESS; nIndex++)
	{
		szValue[0] = '\0';
		dwValue    = MAX_PATH;
		lRetCode   = RegEnumValue(m_hKey, nIndex, szValue, &dwValue, NULL, NULL, NULL, NULL);
		if (lRetCode == ERROR_SUCCESS)
			strArray.Add(szValue);
	}

	Close();
	
	return ERROR_SUCCESS;
}
