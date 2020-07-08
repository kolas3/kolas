// SMSMessageConvert.cpp : implementation file
//

#include "stdafx.h"
#include "SMSMessageConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSMessageConvert

CSMSMessageConvert::CSMSMessageConvert()
{
}

CSMSMessageConvert::~CSMSMessageConvert()
{
}


BEGIN_MESSAGE_MAP(CSMSMessageConvert, CWnd)
	//{{AFX_MSG_MAP(CSMSMessageConvert)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSMSMessageConvert message handlers



INT	CSMSMessageConvert::UnicodeToMultiByte(_TCHAR *fi_pSourceData, CHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;

	
	nWCharSource   = wcslen(fi_pSourceData);

	if ( nWCharSource <= 0 ) {
		*fo_nByteDes = 0;
		return -100;
	}
	
	/* Query the number of bytes required to store the Dest string */
	nBytesNeeded = WideCharToMultiByte( CP_ACP 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	
	/* Allocate the required amount of space */
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		DWORD dw = GetLastError();
		return -100 ;
	}
		
	/* We need more 1 byte for '\0' */
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
		
	/* Do the conversion */
	nBytesNeeded = WideCharToMultiByte     (CP_ACP		, 
											0					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											NULL			    , 
										    NULL );
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}

	*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';	

	*fo_pDestinationData = pszDestinationData;
	*fo_nByteDes         = nBytesNeeded;



	return 0;
}

INT	CSMSMessageConvert::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	/* Query the number of WChar required to store the Dest string */
	nWCharNeeded = MultiByteToWideChar( CP_ACP , 
										(UINT)0  , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		return -100 ;
	}
	

	/* Allocate the required amount of space */	
	/* We need more 2 bytes for '\0' */
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
		
	/* Do the conversion */
	nWCharNeeded = MultiByteToWideChar(	CP_ACP				, 
										0						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
	*fo_nByteDes = 2*nWCharNeeded ;	

	*fo_pDestinationData = pszDestinationData;


	return 0;
}