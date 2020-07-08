// Trim.cpp: implementation of the CTrim class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Trim.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrim::CTrim()
{

}

CTrim::~CTrim()
{

}

VOID CTrim::trimAll(TCHAR *szInput, TCHAR *szOutput, TCHAR cTrimChar)
{
	INT i = 0, j = 0;
	
    for ( i = 0 ; szInput[i] != '\0' && szInput[i] != '\n' ; i++ ) 
	{
        if ( szInput[i] == cTrimChar )
		{
		}
        else 
		{ 
			szOutput[j] = szInput[i]; 
			j++; 
		}
    }
    szOutput[j] ='\0';
}

VOID CTrim::lTrim(TCHAR *szInput, TCHAR *szOutput, TCHAR cTrimChar)
{
	INT i = 0;
	
    for ( i=0 ; szInput[i]!='\0' && szInput[i]!='\n' ; i++ )
	{
        if (szInput[i] != cTrimChar) 
		{
            _tcscpy(szOutput, &szInput[i]);
            return;
        }
    }
	
    szOutput[0] = '\0';
}

VOID CTrim::rTrim(TCHAR *szInput, TCHAR *szOutput, TCHAR cTrimChar)
{
	INT i = 0;
	
    _tcscpy( szOutput, szInput );
    for ( i = _tcsclen(szInput)-1  ;  i >= 0  ;  i-- )
	{
        if(szInput[i] != cTrimChar) 
		{
            szOutput[i+1] = '\0';
            return;
        }
    }
    szOutput[0] = '\0';
}

