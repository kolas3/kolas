//	-------------------------------------------------------------------
//	Filename		:	registry.cpp
//	Description		:	registry manipulation class
//	By				:	John Cruise
//	Date			:	1996 Mar 25
//	Last Modified	:	1997 Jul 14
//	Notes			:	tabstop = 4
//	-------------------------------------------------------------------
//	Modifications	:	1996 Mar 25		John Cruise
//					:		none as of now
//					:
//					:	1997 Jul 14		John Cruise
//					:		modified to support exception handling
//	-------------------------------------------------------------------

#pragma message( _T("If the target mode is not a windows application...\n") )
#pragma message( _T("remember to comment out the stdafx.h") )

#include "stdafx.h"
#include "registry.h"

CRegistry::CRegistry()
{
	m_bIsOpen	= FALSE;
	// KOL.UDF.022 시큐어코딩 보완
	m_nLastError = 0;
	m_hKey = NULL;
}

CRegistry::CRegistry( HKEY hKey, LPCTSTR cSubKey, DWORD nOptions/*=0*/, REGSAM samDesired/*=KEY_ALL_ACCESS*/ )
{
	if(( m_nLastError=RegOpenKeyEx( hKey, cSubKey, nOptions, samDesired, &m_hKey ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));

	m_bIsOpen		= TRUE;
}

CRegistry::~CRegistry()
{
	//	if the registry is still open ... then close it
	try {
	if( m_bIsOpen )
		Close();
	}

	catch( CRegistryException *regError ) {
		regError->ReportError();
		regError->Delete();
	}
}

VOID CRegistry::Create( DWORD &nDisposition, HKEY hKey, LPCTSTR cName, DWORD nOptions/*=REG_OPTION_NON_VOLATILE*/, REGSAM samDesired/*=KEY_ALL_ACCESS*/, LPSECURITY_ATTRIBUTES lpSecurityAttributes/*=NULL*/ )
{
	//	if open, close it
	try {
		if( m_bIsOpen )
			Close();
	}

	catch( CRegistryException *regError ) {
		throw( regError );
	}

	//	create the key
	if(( m_nLastError=RegCreateKeyEx( hKey, cName, NULL, _T(""), nOptions, samDesired, lpSecurityAttributes, &m_hKey, &nDisposition ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));

	m_bIsOpen    = TRUE;
}

VOID CRegistry::Open( HKEY hKey, LPCTSTR cSubKey, DWORD nOptions/*=0*/, REGSAM samDesired/*=KEY_ALL_ACCESS*/ )
{
	//	if has previously open something, close it
	try {
		if( m_bIsOpen )
			Close();
	}

	catch( CRegistryException *regError ) {
		throw( regError );
	}

	//	open the key
	if(( m_nLastError=RegOpenKeyEx( hKey, cSubKey, nOptions, samDesired, &m_hKey ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));

	m_bIsOpen		= TRUE;
}

VOID CRegistry::Close()
{
	if( m_bIsOpen ) {
		RegFlushKey( m_hKey );
		m_nLastError = RegCloseKey( m_hKey );
		m_bIsOpen = FALSE;
	}

	else
		m_nLastError = ERROR_SUCCESS;
}

VOID CRegistry::Remove( LPCTSTR cSubKey )
{
	if(( m_nLastError=RegDeleteKey( m_hKey, cSubKey ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::RemoveValue(LPCTSTR cValue)
{
	if(( m_nLastError=RegDeleteValue( m_hKey, cValue ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::Write( LPCTSTR cValueName, CONST BYTE *pData, DWORD nSize, DWORD nType/*=REG_SZ*/ )
{
	if(( m_nLastError=RegSetValueEx( m_hKey, cValueName, NULL, nType, pData, nSize ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::Read( LPBYTE pDest, DWORD *nSize, DWORD *nType, LPCTSTR cValueName )
{
	if(( m_nLastError=RegQueryValueEx( m_hKey, cValueName, NULL, nType, pDest, nSize ))!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

LONG CRegistry::GetLastError()
{
	return( m_nLastError );
}

BOOL CRegistry::EnumValue( DWORD nIndex, LPCTSTR szValueName, LPDWORD nValueNameLen, LPDWORD nType/*=NULL*/, LPBYTE lpValue/*=NULL*/, LPDWORD nValueSize/*=NULL*/ )
{
	//	enumerate the values
	m_nLastError	= RegEnumValue( m_hKey, nIndex, szValueName, nValueNameLen, NULL, nType, lpValue, nValueSize );

	//	check the return value
	if( m_nLastError==ERROR_NO_MORE_ITEMS )
		return( FALSE );

	else if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));

	return( TRUE );
}

BOOL CRegistry::EnumKey( DWORD nIndex, LPCTSTR szKeyName, LPDWORD nKeyNameLen, PFILETIME stTimeWrite, LPCTSTR szClass/*=NULL*/, LPDWORD nClassLen/*=NULL*/ )
{
	//	enumerate the keys
	m_nLastError	= RegEnumKeyEx( m_hKey, nIndex, szKeyName, nKeyNameLen, NULL, szClass, nClassLen, stTimeWrite );

	//	check the return value
	if( m_nLastError==ERROR_NO_MORE_ITEMS )
		return( FALSE );

	else if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));

	return( TRUE );
}

VOID CRegistry::SaveKey( LPCTSTR szFilename, LPSECURITY_ATTRIBUTES lpSecurity/*=NULL*/ )
{
	//	save the key
	m_nLastError	= RegSaveKey( m_hKey, szFilename, lpSecurity );

	//	check the return value
	if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::LoadKey( LPCTSTR szSubKey, LPCTSTR szFilename )
{
	//	load the key
	m_nLastError	= RegLoadKey( m_hKey, szSubKey, szFilename );

	//	check the return value
	if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::UnloadKey( LPCTSTR szSubKey )
{
	//	unload the previously loaded key
	m_nLastError	= RegUnLoadKey( m_hKey, szSubKey );

	//	check the return value
	if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

VOID CRegistry::RestoreKey( LPCTSTR szFilename, DWORD nFlags/*=0*/ )
{
	//	restore the key from the file
	m_nLastError	= RegRestoreKey( m_hKey, szFilename, nFlags );

	//	check the return value
	if( m_nLastError!=ERROR_SUCCESS )
		throw( new CRegistryException( m_nLastError ));
}

//////////////////////////////////////////////////////////////////////
// CRegistryException Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryException::CRegistryException( UINT nError )
{
	m_nError	= nError;
}

CRegistryException::~CRegistryException()
{

}

INT CRegistryException::ReportError(UINT nType, UINT nMessageID)
{
	//	format the message
	LPVOID lpMsgBuf=NULL;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, \
		NULL, \
		m_nError, \
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
		( LPTSTR )&lpMsgBuf, \
		0, \
		NULL );

	CString	szError;
	szError.Format( _T("Registry Error\n\nError Code: %d\nDescription: %s"), m_nError, lpMsgBuf );

	// Free the buffer.
	LocalFree( lpMsgBuf );

	//	display the error message
	INT	nRet=AfxMessageBox( szError, nType, 0 );

	//	return the value that the afxMessageBox() has returned
	return( nRet );
}
