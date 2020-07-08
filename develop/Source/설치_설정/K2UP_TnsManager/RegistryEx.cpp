/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Shane Martin
// All rights reserved
//
// Distribute freely, except: don't _tremove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or _tremove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    shane.kim@kaiserslautern.netsurf.de
/////////////////////////////////////////////////////////////////////////////

// last revised: 24 Apr 98
// Registry.cpp : implementation file
//
// Description:
// CRegistryEx is a wrapper for the Windows Registry API.  It allows
//  easy modification of the Registry with easy to remember terms like
//  Read, Write, Open, and Close.

// *********************************************
// Seain B. Conover 1 Jan 1999
// added ::getprofilestring and ::findkey



#include "stdafx.h"
#include "RegistryEx.h"

CRegistryEx::CRegistryEx(HKEY hKeyRoot)
{
	m_hKey = hKeyRoot;
}

CRegistryEx::~CRegistryEx()
{
	Close();
}


BOOL CRegistryEx::VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ASSERT (hKeyRoot);
	ASSERT (pszPath);

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	return FALSE;
}

BOOL CRegistryEx::VerifyKey (LPCTSTR pszPath)
{
	ASSERT (m_hKey);

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::VerifyValue (LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	LONG lReturn = RegQueryValueEx(m_hKey, pszValue, NULL,
		NULL, NULL, NULL);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::CreateKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	DWORD dw;

	LONG ReturnValue = RegCreateKeyEx (hKeyRoot, pszPath, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&m_hKey, &dw);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Open (HKEY hKeyRoot, LPCTSTR pszPath)
{
	m_sPath = pszPath;

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

VOID CRegistryEx::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, INT iVal)
{
	DWORD dwValue;

	ASSERT(m_hKey);
	ASSERT(pszKey);
	
	dwValue = (DWORD)iVal;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = sizeof(DWORD);
	m_Info.dwType = REG_DWORD;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, _tcsclen(pszData), FALSE));

	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, _tcsclen(pszData) + 1);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = _tcsclen(pszData) + 1;
	m_Info.dwType = REG_SZ;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CStringList& scStringList)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scStringList.IsSerializable());
	scStringList.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CByteArray& bcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(bcArray.IsSerializable());
	bcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CWordArray& wcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(wcArray.IsSerializable());
	wcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scArray.IsSerializable());
	scArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write(LPCTSTR pszKey, LPCRECT rcRect)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 30;
	CDWordArray dwcArray;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, rcRect->top);
	dwcArray.SetAt(1, rcRect->bottom);
	dwcArray.SetAt(2, rcRect->left);
	dwcArray.SetAt(3, rcRect->right);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_RECT;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 20;
	CDWordArray dwcArray;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, lpPoint->x);
	dwcArray.SetAt(1, lpPoint->y);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_POINT;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, INT& iVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPCTSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (INT)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, LPCTSTR lpszBuffer, INT nBufferSize )
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = nBufferSize;
	TCHAR  szString[255];

	LONG lReturn = RegQueryValueEx (m_hKey, (LPCTSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;
	
	if(lReturn == ERROR_SUCCESS)
	{
		wmemcpy( lpszBuffer, szString, (INT) dwSize );
		nBufferSize = (INT) dwSize;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPCTSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = 200;
	TCHAR  szString[255];

	LONG lReturn = RegQueryValueEx (m_hKey, (LPCTSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CStringList& scStringList)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scStringList.IsSerializable());
		scStringList.RemoveAll();
		scStringList.Serialize(ar);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CByteArray& bcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	INT OldSize = bcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(bcArray.IsSerializable());
		bcArray.RemoveAll();
		bcArray.SetSize(10);
		bcArray.Serialize(ar);
		bcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	INT OldSize = dwcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(10);
		dwcArray.Serialize(ar);
		dwcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CWordArray& wcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	INT OldSize = wcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(wcArray.IsSerializable());
		wcArray.RemoveAll();
		wcArray.SetSize(10);
		wcArray.Serialize(ar);
		wcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 4096;
	INT OldSize = scArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scArray.IsSerializable());
		scArray.RemoveAll();
		scArray.SetSize(10);
		scArray.Serialize(ar);
		scArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, LPRECT& rcRect)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 30;
	CDWordArray dwcArray;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(5);
		dwcArray.Serialize(ar);
		ar.Close();
		file.Close();
		rcRect->top = dwcArray.GetAt(0);
		rcRect->bottom = dwcArray.GetAt(1);
		rcRect->left = dwcArray.GetAt(2);
		rcRect->right = dwcArray.GetAt(3);
	}

	m_Info.lMessage = lReturn;
	m_Info.dwType = REG_RECT;
	m_Info.dwSize = sizeof(RECT);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const INT iMaxChars = 20;
	CDWordArray dwcArray;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(5);
		dwcArray.Serialize(ar);
		ar.Close();
		file.Close();
		lpPoint->x = dwcArray.GetAt(0);
		lpPoint->y = dwcArray.GetAt(1);
	}

	m_Info.lMessage = lReturn;
	m_Info.dwType = REG_POINT;
	m_Info.dwSize = sizeof(POINT);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::DeleteValue (LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	LONG lReturn = RegDeleteValue(m_hKey, pszValue);


	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ASSERT(pszPath);
	ASSERT(hKeyRoot);

	LONG lReturn = RegDeleteKey(hKeyRoot, pszPath);

	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

// for debugging
VOID ShowSysMsg(LONG res, TCHAR* str, TCHAR* file, DWORD line)
{
	TCHAR msg[256];
	LPVOID lpMsgBuf;
	FormatMessage(		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM |	 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,	 
						res,	
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Defaultlanguage    
						(LPTSTR) &lpMsgBuf,    
						0,    
						NULL );
	_stprintf(msg, _T("%u %s\n%s %u"), res, str, file, line);
	MessageBox( NULL, msg, (TCHAR*)lpMsgBuf, MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf ); 
}

CString CRegistryEx::FindKey (LPCTSTR pszSearchRoot, LPCTSTR pszKey, LPCTSTR pszValue)
{
	CString strResult;

	ASSERT(m_hKey);
	ASSERT(pszSearchRoot);
	ASSERT(pszKey);
	ASSERT(pszValue);

	// read key from registry based on search root; determine value
	CRegistryEx registry;
	registry.Open( HKEY_LOCAL_MACHINE, pszSearchRoot );
	CString strValueFound;
	registry.Read( pszKey, strValueFound );
	registry.Close();

	CString strValueWanted = pszValue;
	
	strValueFound.MakeUpper();
	strValueWanted.MakeUpper();

	if ( strValueFound == strValueWanted )
	{
		strResult = pszSearchRoot;
	}
	else
	{
		CStringArray aKeys;
		if ( ListKey( pszSearchRoot, aKeys ) )
		{
			INT nCount;
			const INT nSize = aKeys.GetSize();
			for ( nCount = 0; nCount < nSize && strResult.IsEmpty(); nCount++ )
			{
				const CString strKey = CString( pszSearchRoot ) + _T("\\") + aKeys[ nCount ];
				CStringArray aSubKeys;
				if ( ListKey( strKey, aSubKeys ) )
				{
					strResult = FindKey( strKey, pszKey, pszValue );
				}
			}
		}
	}

	return strResult;
}

BOOL CRegistryEx::ListKey (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	DWORD	dwSubKeys = 0;
	DWORD	idw;
	LONG	lReturn;
	HKEY	hQKey  = NULL;
	CString szSubKeyName;
	TCHAR	szStr[200];
	DWORD	dwSubKeyNameLen;
	FILETIME	LastFileTime;
	BOOL	Ok;

	lReturn = RegOpenKeyEx (m_hKey, pszKey, 0L,
		KEY_READ, &hQKey);

	Ok = (lReturn == ERROR_SUCCESS);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if (Ok) {

		lReturn = RegQueryInfoKey( hQKey, NULL, NULL, NULL
			, &dwSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		
		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
	}
	if (Ok) {
		scArray.RemoveAll();
	}
	for ( idw=0; ((idw<dwSubKeys) && Ok); idw++) {
		dwSubKeyNameLen = 200;
		lReturn = RegEnumKeyEx( 
				hQKey
			,	idw
			,	szStr
			,	&dwSubKeyNameLen
			,	NULL
			,	NULL
			,	NULL
			,	&LastFileTime
			);

		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
		if (Ok)  {
			// Add name to array
			szSubKeyName = szStr;
			scArray.Add( szSubKeyName );
		}
		else {
			ShowSysMsg(lReturn, _T("RegEnumKeyEx"), __WFILE__, __LINE__);
		}

	}
	if (hQKey != NULL) {
		RegCloseKey (hQKey);
		hQKey = NULL;
	}
	if(Ok)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::ListValues (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	DWORD	dwValues = 0;
	DWORD	idw;
	LONG	lReturn;
	HKEY	hQKey  = NULL;
	CString szValueName;
	TCHAR	szStr[200];
	DWORD	dwValueNameLen;
	DWORD	dwType;
	BOOL	Ok;

	lReturn = RegOpenKeyEx (m_hKey, pszKey, 0L,
		KEY_READ, &hQKey);

	Ok = (lReturn == ERROR_SUCCESS);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if (Ok) {

		lReturn = RegQueryInfoKey( hQKey, NULL, NULL, NULL
			, NULL, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL );
		
		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
	}
	if (Ok) {
		scArray.RemoveAll();
	}
	for ( idw=0; ((idw<dwValues) && Ok); idw++) {
		dwValueNameLen = 200;
		lReturn = RegEnumValue( 
				hQKey				// handle to key to query
			,	idw					// index of value to query
			,	szStr				// address of buffer for value string
			,	&dwValueNameLen		// address for size of value buffer
			,	NULL				// reserved
			,	&dwType				// address of buffer for type code
			,	NULL				// address of buffer for value data
			,	NULL				// address for size of data buffer
		);

		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
		if (Ok)  {
			// Add name to array
			szValueName = szStr;
			scArray.Add( szValueName );
		}
		else {
			ShowSysMsg(lReturn, _T("RegEnumValue"), __WFILE__, __LINE__);
		}

	}
	if (hQKey != NULL) {
		RegCloseKey (hQKey);
		hQKey = NULL;
	}
	if(Ok)
		return TRUE;

	return FALSE;
}
