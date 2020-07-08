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

/** [static] 문자열에서 지정 문자를 제거한다.
	@param szInput		[in]  입력문자열
	@param szOutput		[out] 지정 문자가 제거된 출력문자열
	@param cTrimChar	[in]  입력문자열에서 제거할 문자
	@author 박길주
	@since 2002-11-22
*/
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

/** [static] 문자열 왼쪽편에서 지정 문자를 제거한다.
	@param szInput		[in]  입력문자열
	@param szOutput		[out] 지정 문자가 제거된 출력문자열
	@param cTrimChar	[in]  입력문자열에서 제거할 문자
	@author 박길주
	@since 2002-11-22
*/
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

/** [static] 문자열 오른쪽편에서 지정 문자를 제거한다.
	@param szInput		[in]  입력문자열
	@param szOutput		[out] 지정 문자가 제거된 출력문자열
	@param cTrimChar	[in]  입력문자열에서 제거할 문자
	@author 박길주
	@since 2002-11-22
*/
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

