/**************************************************************************/
// CharConvert.cpp -- convert to/from unicode using
//                    MulitByteToWideChar & WideCharToMulitByte
//
//         Park Daewoo
//         ECO Chinese Develope Team
//         Copyright (c) 2002.04 ECO Corporation
//
/**************************************************************************/
#include    "stdafx.h"
#include    "charconvert.h"

CHAR		szBOM[] = "\377\376";  // 0xFF, 0xFE  // leave off TEXT() macro.
_TCHAR		NlsRegEntryString[]=TEXT("SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage");

// Static Variables
CHAR	CharConvert::m_DefaultChar[] =  "?";
DWORD	CharConvert::m_CFlags  = 0;
DWORD	CharConvert::m_MBFlags = MB_PRECOMPOSED;	
BOOL	CharConvert::m_UsedDefaultChar = FALSE;
BOOL	CharConvert::m_InitFlag = FALSE;
INT		CharConvert::m_DestinationCodePage = 0;
INT		CharConvert::m_SourceCodePage = 0;
unsigned char CharConvert::m_UTF8BOM[] = { 0xEF, 0xBB, 0xBF };


/**************************************************************************/
//    Contructor
/**************************************************************************/
CharConvert::CharConvert(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	m_pszCodePage  = NULL;
	m_iCodePageCnt = 0;

	if ( !m_InitFlag )
	{
		GetCurrCodePage(&m_SourceCodePage);
		m_DestinationCodePage = m_SourceCodePage;
		m_InitFlag = TRUE;
	}

	m_UTF8Header = FALSE;
}

/**************************************************************************/
//    Destructor
/**************************************************************************/
CharConvert::~CharConvert()
{
	if ( m_pszCodePage != NULL && m_iCodePageCnt > 0 )
		FreeCodePages(m_pszCodePage, m_iCodePageCnt);
}

/**************************************************************************/

//    Purpose : Check the First byte is 0xFF and the second byte is 0xFE
//    Return  : TRUE  - the First byte is 0xFF and the second byte is 0xFE
//              FALSE - the First byte is not 0xFF and the second byte is not 0xFE
//    USES    : Little Endian For Intel Machines

/**************************************************************************/
BOOL CharConvert::IsBOM(PBYTE pb)
{
ECO_EXCEPTION_BEGIN
	if ((*pb == 0xFF) & (*(pb+1) == 0xFE))  // BOM
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
ECO_EXCEPTION_END
	return FALSE;
}

/**************************************************************************/

//    Purpose : Check the First byte is 0xFE and the second byte is 0xFF
//    Return  : TRUE  - the First byte is 0xFE and the second byte is 0xFF
//              FALSE - the First byte is not 0xFE and the second byte is not 0xFF
//    USES    : Big Endian For Other Machines

/**************************************************************************/
BOOL CharConvert::IsRBOM(PBYTE pb)
{
ECO_EXCEPTION_BEGIN
	if ((*pb == 0xFE) & (*(pb+1) == 0xFF))  // RBOM
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
ECO_EXCEPTION_END
	return FALSE;
}

/**************************************************************************/

//    Purpose : Get Code Pages
//    IN      : None
//    OUT     : None
//    Return  : 0         - Success
//              otherwise - Fail


/**************************************************************************/
INT CharConvert::GetAllCodePage() 
{
ECO_EXCEPTION_BEGIN
	_TCHAR	szKeyname[MAX_PATH], szValue[MAX_PATH];
	DWORD	cBytesName, cBytesValue, iSubKey;
	INT		cp;

	HKEY	hKey;
	LONG	rValue;


	rValue = RegOpenKeyEx (HKEY_LOCAL_MACHINE, NlsRegEntryString, 0, KEY_QUERY_VALUE, &hKey);
    if (rValue == ERROR_ACCESS_DENIED)
	{
		RegCloseKey (hKey);
		return GENERAL_ERROR;
	}

    if (rValue != ERROR_SUCCESS) 
	{
		RegCloseKey (hKey);
		return GENERAL_ERROR;
	}

	iSubKey = 0;
	m_iCodePageCnt = NULL;
	m_pszCodePage  = NULL;
	cBytesName     = cBytesValue = MAX_PATH;	

	while (!RegEnumValue (hKey, iSubKey, szKeyname, &cBytesName, NULL, NULL, (LPBYTE)szValue, &cBytesValue)) 
	{		
		if (GetRegKey (szValue, &cp) == 1) 
		{	
			wsprintf (szValue, TEXT("%d"), cp);
			
			m_pszCodePage = (_TCHAR**) realloc ( m_pszCodePage, sizeof(_TCHAR*)*(m_iCodePageCnt + 1) );
			m_pszCodePage[m_iCodePageCnt] =  (_TCHAR*) malloc ( wcslen(szValue)*sizeof(_TCHAR) + sizeof(_TCHAR) );
			_tcscpy(m_pszCodePage[m_iCodePageCnt++], szValue);
		}
		
		iSubKey++;
		cBytesName = cBytesValue = MAX_PATH;  // undoc.ed feature, must be set each time.
	}

	RegCloseKey (hKey);
ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Convert a string into a number (like sscanf).
//  However, this function works independent of UNICODE turned on.
//	NOT a general function... looking for "c_%d.nls"
//  IN     : pSource - Registry Data
//  OUT    : pValue  - c_1000.nls  ---> 1000 
//  RETURN : if pSource is null, then return 0 OR
//           if pSource's Format is "c_%d.nls", then return 1 
//           otherwise return 0

***************************************************************************/
INT	CharConvert::GetRegKey (_TCHAR* pSource, INT* pValue)
{
	CHAR			ansibuffer[MAX_PATH];
ECO_EXCEPTION_BEGIN
	INT				iStrLen;
	
	iStrLen = lstrlen (pSource);
	if (iStrLen == 0) 
	{
		return 0;
	}
	
#ifdef UNICODE
	WideCharToMultiByte (	CP_ACP          , 
							0               ,  
							pSource         , 
							-1              , 
							ansibuffer      , 
							MAX_PATH        , 
							NULL            , 
							NULL );
#else
	lstrcpy (ansibuffer, pSource);
#endif
	
	CharLowerBuffA (ansibuffer,lstrlenA (ansibuffer));
	
ECO_EXCEPTION_END
	return (sscanf (ansibuffer, "c_%d.nls", pValue)); // leave off TEXT()
}

/***************************************************************************

//	Purpose : Get Current Local Code Page
//  IN      : None
//  OUT	    : Current Code Page 
//  RETURN  : 0

***************************************************************************/
INT CharConvert::GetCurrCodePage(INT *fo_CodePage)
{
ECO_EXCEPTION_BEGIN
	INT		i_ACP, i_OEM;

	i_ACP = GetACP();
	i_OEM = GetOEMCP();

	if ( i_ACP == i_OEM )
		*fo_CodePage = i_ACP;
	else
		*fo_CodePage = i_OEM;

ECO_EXCEPTION_END
	return 0;
}

/***************************************************************************

//	free Two Dimension malloc Buffer
//  IN		:  fi_Buffer - Two Demension malloc Buffer
//             fi_Cnt    - Buffer Count
//  OUT		:  None  

***************************************************************************/
VOID CharConvert::FreeCodePages(_TCHAR **fi_Buffer, INT fi_Cnt)
{
ECO_EXCEPTION_BEGIN
	for ( INT i = 0 ; i < fi_Cnt ; i++ )
	{
		free(fi_Buffer[i]);
		fi_Buffer[i] = NULL;
	}

	fi_Buffer      = NULL;
	fi_Cnt         = 0;
ECO_EXCEPTION_END
}


/***************************************************************************

//	Convert Multibyte String To Unicode string
//  IN		:  fi_pSourceData      - Multibyte String
//  OUT		:  fo_pDestinationData - Unicode String
//             fo_nByteDes         - Bytes of Unicdoe String
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{
ECO_EXCEPTION_BEGIN
	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	/* Query the number of WChar required to store the Dest string */
	nWCharNeeded = MultiByteToWideChar( m_SourceCodePage , 
										(UINT)m_MBFlags  , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		return GENERAL_ERROR ;
	}
	

	/* Allocate the required amount of space */	
	/* We need more 2 bytes for '\0' */
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));
		
	/* Do the conversion */
	nWCharNeeded = MultiByteToWideChar(	m_SourceCodePage				, 
										m_MBFlags						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		free(pszDestinationData);
		return GENERAL_ERROR ;
	}
	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
	*fo_nByteDes = 2*nWCharNeeded ;

	*fo_pDestinationData = pszDestinationData;

ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Convert Unicode String to Multibyte String for each code pages
//  IN		:  fi_pSourceData      - Unicode String
//			(DELETE)fi_nByteSource      - Bytes of Unicode String 
//  OUT		:  fo_pDestinationData - Multibyte String
//             fo_nByteDes         - Bytes of Multibyte String
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::UnicodeToMultiByte(_TCHAR *fi_pSourceData, CHAR **fo_pDestinationData, INT *fo_nByteDes )
{
ECO_EXCEPTION_BEGIN
	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;

	
	nWCharSource   = wcslen(fi_pSourceData);
	
	/* Query the number of bytes required to store the Dest string */
	nBytesNeeded = WideCharToMultiByte( m_DestinationCodePage 	, 
										m_CFlags				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										m_DefaultChar			, 
										&m_UsedDefaultChar );
	
	/* Allocate the required amount of space */
	if (nBytesNeeded == 0) 
	{
		return GENERAL_ERROR ;
	}
		
	/* We need more 1 byte for '\0' */
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
		
	/* Do the conversion */
	nBytesNeeded = WideCharToMultiByte     (m_DestinationCodePage		, 
											m_CFlags					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											m_DefaultChar				, 
											&m_UsedDefaultChar );		
	if (nBytesNeeded == 0) 
	{
		return GENERAL_ERROR ;
	}

	*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';

	*fo_pDestinationData = pszDestinationData;
	*fo_nByteDes         = nBytesNeeded;


ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Check Unicode File
//  IN		:  fi_pFileName - Unicode File
//  OUT		:  fo_nByte	    - Bytes of Unicode File
//  RETURN  :  TRUE   -  Unicode File
//             FALSE  -  Not Unicode File

***************************************************************************/
INT		CharConvert::IsUnicodeFile(_TCHAR *fi_pFileName, INT *fo_nByte )
{
ECO_EXCEPTION_BEGIN
	BY_HANDLE_FILE_INFORMATION	bhfi;
	HANDLE						hFile;
	_TCHAR						szBom[SIZEOFBOM];
	DWORD						nBytesRead;
	INT							nBytesSource;


	// FILE READ
	hFile =	CreateFile(fi_pFileName			,
                       GENERIC_READ			,
                       0					,
                       NULL					,
                       OPEN_EXISTING		,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}	
	
	GetFileInformationByHandle (hFile, &bhfi);
	if (bhfi.nFileSizeHigh != 0) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}
	
	// Get Bytes of File 
	nBytesSource = bhfi.nFileSizeLow;

	/* first read two bytes and look for BOM */
	if (!ReadFile (hFile, szBom, SIZEOFBOM, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	CloseHandle (hFile);

	if (IsUnicode ((unsigned char*)szBom)) 
	{
		*fo_nByte = nBytesSource - SIZEOFBOM;	
		return ISUNICODE;
	}
	else
	{
		*fo_nByte = nBytesSource;
		return NOTUNICODE;
	}
	
ECO_EXCEPTION_END
	return GENERAL_ERROR;
}


/***************************************************************************

//	Read All of Input File   For Unicode File
//  IN		:  fi_pFileName		- File Name
//  OUT		:  fo_pFileString	- All Strings of File
			   fo_nReadBytes    - Bytes of File	
//  RETURN  :  0 - Success
//             1 - Fail

***************************************************************************/
INT	CharConvert::ReadAllOfFile(_TCHAR *fi_pFileName, CString *fo_pFileString, INT *fo_nReadBytes )
{
ECO_EXCEPTION_BEGIN
	HANDLE		hFile;
	DWORD		nBytesRead;
	_TCHAR		*spFileString;
	_TCHAR		szBom[SIZEOFBOM];
	INT			nByte;	
	

	IsUnicodeFile(fi_pFileName, &nByte );
	spFileString = (_TCHAR*)malloc(sizeof(_TCHAR)*nByte + sizeof(_TCHAR));

	// FILE OPEN
	hFile =	CreateFile(fi_pFileName			,
                       GENERIC_READ			,
                       0					,
                       NULL					,
                       OPEN_EXISTING		,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}	
	
	/* first read two bytes and look for BOM */
	if (!ReadFile (hFile, szBom, SIZEOFBOM, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	/* read all of file */
	if (!ReadFile (hFile, spFileString, nByte, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	spFileString[nBytesRead/2] = L'\0';
	*fo_pFileString = spFileString;
	*fo_nReadBytes  = nBytesRead;

	
	CloseHandle (hFile);

ECO_EXCEPTION_END
	return	NOT_ERROR;
}

/***************************************************************************

//	Read All of Input File		For MultiByteCode File
//  IN		:  fi_pFileName		- File Name
//  OUT		:  fo_pFileString	- All Strings of File
			   fo_nReadBytes    - Bytes of File	
//  RETURN  :  0 - Success
//             1 - Fail

***************************************************************************/
INT	CharConvert::ReadAllOfFile(_TCHAR *fi_pFileName, CHAR **fo_pFileString, INT *fo_nReadBytes )
{
ECO_EXCEPTION_BEGIN
	HANDLE		hFile;
	DWORD		nBytesRead;
	CHAR		*spzString;
	_TCHAR		szUTF8Bom[SIZEOFUTF8BOM];
	INT			nByte;	
	BOOL		bUTF8 = FALSE;


	//IsUnicodeFile(fi_pFileName, &nByte );
	if ( IsUTF8File(fi_pFileName, &nByte ) == ISUTF8 )
		bUTF8 = TRUE;

	spzString = (CHAR*)malloc(sizeof(CHAR)*nByte + sizeof(CHAR));

	// FILE OPEN
	hFile =	CreateFile(fi_pFileName			,
                       GENERIC_READ			,
                       0					,
                       NULL					,
                       OPEN_EXISTING		,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}	

	if ( m_UTF8Header & bUTF8 )
	{
		/* first read three bytes and look for UTF8BOM */
		if (!ReadFile (hFile, szUTF8Bom, SIZEOFUTF8BOM, &nBytesRead, NULL)) 
		{
			CloseHandle (hFile);
			return FILE_ERROR;
		}
	}

	/* read all of file */
	if (!ReadFile (hFile, spzString, nByte, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	spzString[nBytesRead] = '\0';
	*fo_pFileString = spzString;
	*fo_nReadBytes  = nBytesRead;

	CloseHandle (hFile);

ECO_EXCEPTION_END
	return	NOT_ERROR;
}

/***************************************************************************

//	Write To File For MultiByteCode 
//  IN		:  fi_pFileName		- File Name
//			   fi_pFileString	- Strings for write
			   fi_nReadBytes    - Bytes of String
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::WriteToFile(_TCHAR *fi_pFileName, CHAR* fi_pFileString, INT fi_nReadBytes )
{
ECO_EXCEPTION_BEGIN
	HANDLE		hFile;
	DWORD		nBytesRead;


	// FILE OPEN
	hFile = CreateFile(fi_pFileName			,
                      GENERIC_WRITE			,
                      0						,
                      NULL					,
                      CREATE_ALWAYS			,
                      FILE_ATTRIBUTE_NORMAL	,
                      NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}

	if ( m_DestinationCodePage == ISUTF8 )
	{
		if (!WriteFile (hFile, m_UTF8BOM, 3, &nBytesRead, NULL)) 
		{
			CloseHandle (hFile);
			return FILE_ERROR;
		}
	}

	if (!WriteFile (hFile, fi_pFileString, fi_nReadBytes, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	CloseHandle (hFile);

ECO_EXCEPTION_END
	return NOT_ERROR;
}


/***************************************************************************

//	Write To File For Unicode
//  IN		:  fi_pFileName		- File Name
//			   fi_pFileString	- Strings for write
			   fi_nReadBytes    - Bytes of String
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::WriteToFile(_TCHAR *fi_pFileName, _TCHAR* fi_pFileString, INT fi_nReadBytes )
{
ECO_EXCEPTION_BEGIN
	HANDLE		hFile;
	DWORD		nBytesRead;


	// FILE OPEN
	hFile = CreateFile(fi_pFileName			,
                      GENERIC_WRITE			,
                      0						,
                      NULL					,
                      CREATE_ALWAYS			,
                      FILE_ATTRIBUTE_NORMAL	,
                      NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}

	if (!WriteFile (hFile, szBOM, 2, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	if (!WriteFile (hFile, fi_pFileString, fi_nReadBytes, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	CloseHandle (hFile);

ECO_EXCEPTION_END
	return NOT_ERROR;
}


/***************************************************************************

//	Change Destination Code Page
//  IN		:  fi_CodePage      - CodePage
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::SetDestinationCodepage(INT	fi_CodePage)
{
ECO_EXCEPTION_BEGIN
	m_DestinationCodePage = fi_CodePage;

ECO_EXCEPTION_END
	return 0;
}


/***************************************************************************

//	Change Conversion Option
//  IN		:  fi_CodePage      - CodePage
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::SetConversionOption(DWORD fi_MBFlag, DWORD fi_CFlag, CHAR *fi_DefaultChar, BOOL fi_UseDefault)
{
ECO_EXCEPTION_BEGIN
	m_UsedDefaultChar = fi_UseDefault;
	strcpy(m_DefaultChar, fi_DefaultChar);
	m_MBFlags = fi_MBFlag;
	m_CFlags  = fi_CFlag;

ECO_EXCEPTION_END
	return 0;
}


/***************************************************************************

//	Change CodePage To CodePage Description
//  IN		:  fi_CodePage      - CodePage
//  OUT     :  fo_CodePageDesc  - CodePage Description
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::ChangeCodepageToDesc(INT fi_CodePage, _TCHAR **fo_CodePageDesc)
{
ECO_EXCEPTION_BEGIN
	IMultiLanguagePtr	pMultiLang;
	MIMECPINFO			pCodePageInfo;
	HRESULT				hResult;
	CString				sBuffer;
	_TCHAR				*pszCodePageDesc;



	if( pMultiLang.CreateInstance(__uuidof(CMultiLanguage) ) != S_OK )
	{
		return GENERAL_ERROR;
	}
				
	//	pMultiLang->Release();
	hResult =  pMultiLang->GetCodePageInfo(
											fi_CodePage    ,
											&pCodePageInfo
											);
	if ( FAILED(hResult) )
	{
		return	LOOP_CONTINUE;
	}

	pszCodePageDesc = (_TCHAR*)malloc(sizeof(_TCHAR)*wcslen(pCodePageInfo.wszDescription) + sizeof(_TCHAR));
	_tcscpy(pszCodePageDesc, pCodePageInfo.wszDescription);
	*fo_CodePageDesc = pszCodePageDesc;


ECO_EXCEPTION_END
	return NOT_ERROR;
}


/***************************************************************************

//	Correct Unicode Marc File        
//  IN		:  fi_AllStreamMarc      - Stream Marc of File
//  OUT     :  fo_AllStreamMarc      - Corrected Stream Marc of File
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::CorrectMarcFile(_TCHAR *fi_AllStreamMarc, _TCHAR **fo_AllStreamMarc)
{
ECO_EXCEPTION_BEGIN
	CString		szStreamMarc, szTemp;
	CString		szTempMarc;
	_TCHAR		*pszAllStreamMarc;
	_TCHAR		szStream[50000];
	_TCHAR		seps[8];
	INT			ids, nPos, nStart;


	szStreamMarc = _T("");
	_stprintf(seps, _T("%c"), 29);

	pszAllStreamMarc = (_TCHAR*)malloc(sizeof(_TCHAR)*wcslen(fi_AllStreamMarc)+sizeof(_TCHAR));
	if ( pszAllStreamMarc == NULL )
	{
		return MALLOC_ERROR;
	}

	_tcscpy(pszAllStreamMarc, fi_AllStreamMarc);
	szTemp = fi_AllStreamMarc;

	nPos = szTemp.Find(seps);
	nStart = 0;
	
	while( nPos != -1 )
	{
		_tcsncpy(szStream, &pszAllStreamMarc[nStart], nPos-nStart);
		szStream[nPos-nStart] = _T('\0');
		szTempMarc = szStream;
		szTempMarc.Remove(_T('\r'));
		szTempMarc.Remove(_T('\n'));
		_tcscpy(szStream, szTempMarc);

		ids = ERROR_MarcModify( szStream, szStream );
		if ( ids ) 
		{
			return GENERAL_ERROR;
		}

		szStreamMarc += szStream;
		szStreamMarc += _T("\r\n");
		nStart = nPos+1;
		nPos = szTemp.Find(seps, nStart);
	}

	free(pszAllStreamMarc);
	pszAllStreamMarc = NULL;
	pszAllStreamMarc = (_TCHAR*)malloc(sizeof(_TCHAR)*szStreamMarc.GetLength()+sizeof(_TCHAR));
	if ( pszAllStreamMarc == NULL )
	{
		return MALLOC_ERROR;
	}

	_tcscpy(pszAllStreamMarc, szStreamMarc.GetBuffer(0));
	*fo_AllStreamMarc = pszAllStreamMarc;


ECO_EXCEPTION_END
	return NOT_ERROR;
}


/***************************************************************************

//	Change Unicode Marc File To Multibyte Marc File
//  IN		:  fi_AllStreamMarc      - Stream Marc of File
//  OUT     :  fo_AllStreamMarc      - Corrected Stream Marc of File
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::CorrectMarcFile(_TCHAR *fi_AllStreamMarc, CHAR **fo_AllStreamMarc)
{
ECO_EXCEPTION_BEGIN
	CString		szTemp;
	CString		szTempMarc;
	CHAR		*szChangedMarc;
	CHAR		*szResultMarc;
	_TCHAR		szStream[50000];
	_TCHAR		seps[8];
	INT			ids, nPos, nStart, nLen;


	nLen   = 
	nStart = 0;
	_stprintf(seps, _T("%c"), 29);
	szResultMarc = NULL;

	szResultMarc = (CHAR*)malloc(sizeof(CHAR)*wcslen(fi_AllStreamMarc)*2);
	szResultMarc[0] = '\0';
	szTemp = fi_AllStreamMarc;
	nPos = szTemp.Find(seps);
	

	while( nPos != -1 )
	{
		_tcsncpy(szStream, &fi_AllStreamMarc[nStart], nPos-nStart);
		szStream[nPos-nStart] = _T('\0');
		szTempMarc = szStream;
		szTempMarc.Remove(_T('\r'));
		szTempMarc.Remove(_T('\n'));
		_tcscpy(szStream, szTempMarc);

		ids = ChangeUnicodeMarcToMulticodeMarc( szStream, &szChangedMarc );
		if ( ids ) 
		{
			return GENERAL_ERROR;
		}
		
		sprintf( &szResultMarc[nLen], "%s\r\n", szChangedMarc);
		szResultMarc[strlen(szResultMarc)] = '\0';
		nLen = strlen(szResultMarc);

		nStart = nPos+1;
		nPos = szTemp.Find(seps, nStart);
	}
	
	*fo_AllStreamMarc = szResultMarc;

ECO_EXCEPTION_END
	return NOT_ERROR;
}


INT	CharConvert::ChangeUnicodeMarcToMulticodeMarc( _TCHAR *fi_szStream, CHAR** fo_ChangedMarc )
{
ECO_EXCEPTION_BEGIN
	tMARC_STRU  s_MarcInfo;
	CHAR	*	szStreamMarc;
	TCHAR		UNI_szStreamMarc[50000];
	INT			ids , nSize;

	
	ids = ReadMarcStruFromStreamMarc(
												fi_szStream , 
												&s_MarcInfo 
													);
	if ( ids )
	{
		return GENERAL_ERROR;
	}

	/*
	ids = WriteMarcStruToStreamMarc_ANSI_TYPE(s_MarcInfo            ,
							                   szStreamMarc          ,
                                               m_DestinationCodePage ,
											   m_CFlags              ,
                                               m_DefaultChar         ,
											   m_UsedDefaultChar 
											 );
	*/


	ids = WriteMarcStruToStreamMarc( s_MarcInfo            ,
							                   UNI_szStreamMarc          
                                             );

	ids = UnicodeToMultiByte(UNI_szStreamMarc , &szStreamMarc , &nSize );
	if ( ids ) 
	{
		return ids;
	}

	if ( ids )
	{
		return GENERAL_ERROR;
	}

	(*fo_ChangedMarc) = szStreamMarc;
	FreeMarcStru( s_MarcInfo );

ECO_EXCEPTION_END
	return NOT_ERROR;
}


/***************************************************************************

//	Change Unicode String To UTF8 String
//  IN		:  fi_UString      - Unicode String
//  OUT     :  fo_UTF8String   - UTF8 String
//			   fo_nBytes	   - UTF8 String's Length
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::UnicodeToUTF8 (_TCHAR* fi_UString, CHAR** fo_UTF8String, INT* fo_nBytes )
{
ECO_EXCEPTION_BEGIN
	INT		nLength;

	nLength = WideStrToUTF8Str(fi_UString, fo_UTF8String);
	fo_UTF8String[nLength] = '\0';
	*fo_nBytes = nLength;

ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Change MultiByte String To UTF8 String
//  IN		:  fi_MString      - MultiByte String
//  OUT     :  fo_UTF8String   - UTF8 String
//			   fo_nBytes	   - UTF8 String's Length
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::MultiByteToUTF8 (CHAR* fi_MString, CHAR** fo_UTF8String, INT* fo_nBytes )
{
ECO_EXCEPTION_BEGIN
	_TCHAR		*szUString;
	INT			nLength;
	INT			ids;

	// Convert to Unicode
	ids = MultiByteToUnicode(fi_MString, &szUString, &nLength);
	if ( ids )
	{
		AfxMessageBox(_T("MultiByteToUnicode Error"));
		return ids;
	}

	nLength = WideStrToUTF8Str(szUString, fo_UTF8String);
	(*fo_UTF8String)[nLength] = '\0';
	*fo_nBytes = nLength;

	free(szUString);

ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Change MultiByte String To UTF8 String
//  IN		:  fi_UTF8String     - UTF8 String
//  OUT     :  fo_UCString       - Unicode String
//			   fo_nBytes	     - Unicode String's Length
//  RETURN  :  0    - Success
//             else - Fail

***************************************************************************/
INT	CharConvert::UTF8ToUnicode(CHAR* fi_UTF8String,   _TCHAR** fo_UCString, INT* fo_nBytes )
{
ECO_EXCEPTION_BEGIN

	INT		nCodePage, ids;
	DWORD   szMBFlag;

	
	// codepage Backup
	nCodePage = m_SourceCodePage;
	szMBFlag = m_MBFlags;

	m_SourceCodePage = CP_UTF8;
	m_MBFlags = 0;

	ids = MultiByteToUnicode(fi_UTF8String, fo_UCString, fo_nBytes);
	if ( ids )
		return ids;

	// codepage recovery
	m_SourceCodePage = nCodePage;
	m_MBFlags = szMBFlag;

ECO_EXCEPTION_END
	return NOT_ERROR;
}

/***************************************************************************

//	Check UTF8 File
//  IN		:  fi_pFileName - UTF8 File
//  OUT		:  fo_nByte	    - Bytes of UTF8 File
//  RETURN  :  ISUTF8   -  UTF8 File
//             NOTUTF8  -  Not UTF8 File
//			   ELSE		-  ERROR

***************************************************************************/
INT		CharConvert::IsUTF8File(_TCHAR *fi_pFileName, INT *fo_nByte )
{
ECO_EXCEPTION_BEGIN
	BY_HANDLE_FILE_INFORMATION	bhfi;
	HANDLE						hFile;
	_TCHAR						szUTF8Bom[SIZEOFUTF8BOM];
	_TCHAR						szUTFString[10000];
	DWORD						nBytesRead;
	INT							nBytesSource;


	m_UTF8Header = FALSE;


	// FILE READ
	hFile =	CreateFile(fi_pFileName			,
                       GENERIC_READ			,
                       0					,
                       NULL					,
                       OPEN_EXISTING		,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return FILE_ERROR;
	}	
	
	GetFileInformationByHandle (hFile, &bhfi);
	if (bhfi.nFileSizeHigh != 0) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}
	
	// Get Bytes of File 
	nBytesSource = bhfi.nFileSizeLow;

	/* first read three bytes and look for BOM */
	if (!ReadFile (hFile, szUTF8Bom, SIZEOFUTF8BOM, &nBytesRead, NULL)) 
	{
		CloseHandle (hFile);
		return FILE_ERROR;
	}

	if (IsUTF8 ((unsigned char*)szUTF8Bom)) 
	{
		*fo_nByte = nBytesSource - SIZEOFUTF8BOM;	
		m_UTF8Header = TRUE;
		CloseHandle (hFile);

		return ISUTF8;
	}
	else
	{
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		if (!ReadFile (hFile, szUTFString, 10000, &nBytesRead, NULL)) 
		{
			CloseHandle (hFile);
			return FILE_ERROR;
		}
		
		CloseHandle (hFile);
		*fo_nByte = nBytesSource;

		if ( IsUTF8String((unsigned char*)szUTFString) )
			return ISUTF8;
		else
			return NOTUTF8;
	}
	
ECO_EXCEPTION_END
	return GENERAL_ERROR;
}

/*******************************************************************************************************/

//    Purpose : Check the First byte is 0xEF and the second byte is 0xBB and the third byte is 0xBF
//    Return  : TRUE  - UTF8 File
//              FALSE - not UTF8 File
//    USES    : Check UTF8 File

/*******************************************************************************************************/
BOOL CharConvert::IsUTF8(PBYTE pb)
{
ECO_EXCEPTION_BEGIN
	if ((*pb == 0xEF) & (*(pb+1) == 0xBB) & (*(pb+2) == 0xBF))  // UTF8 BOM
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
ECO_EXCEPTION_END
	return FALSE;
}

/*******************************************************************************************************/

//    Purpose : Check the string is UTF8, First Byte 0xC0 ~ 0xFD and Second Byte 0x80 ~ 0xBF
//    Return  : TRUE  - UTF8 File
//              FALSE - not UTF8 File
//    USES    : Check UTF8 File

/*******************************************************************************************************/
BOOL CharConvert::IsUTF8String(PBYTE pb)
{
ECO_EXCEPTION_BEGIN
	while ( pb )	
	{
		if ( *pb <= 0x80 )
			pb++;
		else if ( (*pb >= 0xC0 && *pb <= 0xFD ) & (*(pb+1) >= 0x80 && *(pb+1) <= 0xBF) & (*(pb+2) >= 0x80 && *(pb+2) <= 0xBF) )
			return TRUE;
		else
			return FALSE;
	}
ECO_EXCEPTION_END
	return FALSE;
}

/*******************************************************************************************************/

//    Purpose : Change Static member m_SourceCodePage
//    IN	  : source Codepage
//    USES    : Change Static member m_SourceCodePage

/*******************************************************************************************************/
VOID CharConvert::ChangeSourceCodePage( INT fi_SourceCodePage)
{
	m_SourceCodePage = fi_SourceCodePage;
}

/*******************************************************************************************************/

//    Purpose : Change Static member m_DestinationCodePage
//    IN	  : destination Codepage
//    USES    : Change Static member m_DestinationCodePage

/*******************************************************************************************************/
VOID CharConvert::ChangeDescCodePage( INT fi_DescCodePage)
{
	m_DestinationCodePage = fi_DescCodePage;
}
