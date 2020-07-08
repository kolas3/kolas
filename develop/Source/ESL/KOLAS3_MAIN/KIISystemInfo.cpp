// KIISystemInfo.cpp: implementation of the CKIISystemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "KIISystemInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKIISystemInfo* CKIISystemInfo::m_pInstance = NULL;

CString CKIISystemInfo::GetRegistryInfo(TCHAR* path, TCHAR* stringName)
{
    LONG    ids;
    HKEY    hKey;
    DWORD   dwReserved, dwType, n;
	
    TCHAR    szValue[80];
	
    dwReserved = 0;
    ids = RegOpenKey(HKEY_LOCAL_MACHINE, path, &hKey);
    if ( ids != ERROR_SUCCESS ) return _T("");
	
    n = 80;
    ids = RegQueryValueEx (hKey, stringName, NULL, &dwType, (UCHAR*)&szValue[0], &n);
    if ( ids != ERROR_SUCCESS ) return _T("");
	
    return CString(szValue);
}

INT CKIISystemInfo::SetRegistryInfo(TCHAR* path, TCHAR* stringName, TCHAR* value)
{
    LONG    ids;
    HKEY    hKey;
    DWORD   dwReserved, n;
	
    dwReserved = 0;
    ids = RegOpenKey( HKEY_LOCAL_MACHINE, path, &hKey );
    if ( ids != ERROR_SUCCESS ) return -1;
	
    n = 80;
    ids = RegSetValueEx (hKey, stringName, NULL, REG_SZ, (UCHAR*)value, n);
    if ( ids != ERROR_SUCCESS ) return -2;
	
    return 0;
}
