// Keyword.cpp: implementation of the CKeyword class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Keyword.h"

#include "trim.h"
#include "SpecialCharacter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#define SPECIAL_CHAR_COUNT 26
tSPECIAL_CHAR_STRU s_DelCharInfo[SPECIAL_CHAR_COUNT] = {
	{ _T("한글")       , 0xB0 , 0xA1 , 0xC8 , 0xFE },
	{ _T("한자")       , 0xCA , 0xA1 , 0xFD , 0xFE },
	{ _T("일어")       , 0xAA , 0xA1 , 0xAA , 0xF3 },
	{ _T("일어")       , 0xAB , 0xA1 , 0xAB , 0xF6 },
	{ _T("그리스어")   , 0xA5 , 0xC1 , 0xA5 , 0xD8 },
	{ _T("그리스어")   , 0xA5 , 0xE1 , 0xA5 , 0xF8 },
	{ _T("라틴어")     , 0xA8 , 0xA1 , 0xA8 , 0xA4 },
	{ _T("라틴어")     , 0xA8 , 0xA6 , 0xA8 , 0xA6 },
	{ _T("라틴어")     , 0xA8 , 0xA8 , 0xA8 , 0xAF },
	{ _T("라틴어")     , 0xA9 , 0xA1 , 0xA9 , 0xAF }, 
	{ _T("러시아어")   , 0xAC , 0xA1 , 0xAC , 0xC1 },
	{ _T("러시아어")   , 0xAC , 0xD1 , 0xAC , 0xF1 },
	{ _T("한글자모")   , 0xA4 , 0xA1 , 0xA4 , 0xD3 },
	{ _T("한글고어")   , 0xA4 , 0xD5 , 0xA4 , 0xFE },
	{ _T("숫자")       , 0xA3 , 0xB0 , 0xA3 , 0xB9 },
	{ _T("영문대문자") , 0xA3 , 0xC1 , 0xA3 , 0xDA },
	{ _T("영문소문자") , 0xA3 , 0xE1 , 0xA3 , 0xFA },
	{ _T("그림문자")   , 0xA1 , 0xA1 , 0xA1 , 0xFE },
	{ _T("그림문자")   , 0xA2 , 0xA1 , 0xA2 , 0xE5 },
	{ _T("그림문자")   , 0xA3 , 0xA1 , 0xA3 , 0xFE },
	{ _T("그림문자")   , 0xA5 , 0xA1 , 0xA5 , 0xAA },
	{ _T("그림문자")   , 0xA5 , 0xB0 , 0xA5 , 0xB9 },
	{ _T("그림문자")   , 0xA7 , 0xA1 , 0xA7 , 0xFE },
	{ _T("그림문자")   , 0xA8 , 0xB1 , 0xA8 , 0xFE },
	{ _T("그림문자")   , 0xA9 , 0xB0 , 0xA9 , 0xFE },
	{ _T("한글확장")   , 0x81 , 0x20 , 0xC5 , 0xFF }
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyword::CKeyword()
{

}

CKeyword::~CKeyword()
{

}

VOID CKeyword::getKeyword(TCHAR *szInput, TCHAR *szKeyword , BOOL bKeywordFlag )
{
    TCHAR     s_tmpbuf[1024];
    _TUCHAR ch;
    TCHAR*	 ptr;
	TCHAR*	 s_pStrptr;
    INT      s_Strlen;
	
    _tcscpy( s_tmpbuf , szInput );
    if ( bKeywordFlag == FALSE ) 
	{
        ptr = _tcschr( szInput , '.' );
        if ( ptr != NULL )
		{
            s_pStrptr = _tcschr( szInput , '.' );
            if ( s_pStrptr != NULL )
			{
                s_Strlen = _tcsclen(szInput) - _tcsclen(s_pStrptr);
                _tcsnccpy( s_tmpbuf , szInput , s_Strlen );
                s_tmpbuf[s_Strlen] = '\0';
            }
			CTrim::trimAll(&ptr[1], &ptr[1] , '.');
            _stprintf( &s_tmpbuf[_tcsclen(s_tmpbuf)] , _T(".%s"), &ptr[1] );
        }
        _tcscpy( szInput , s_tmpbuf );
		
        CTrim::trimAll( szInput, szKeyword, ' ' );
        CTrim::trimAll( szKeyword, szKeyword, '-' );

        return;
    }
	
    ptr = szKeyword ;
	
    for ( ; ch = *szInput ; szInput++)
    {
		if ( (_TUCHAR)ch & 0x80 )
		{
			*szKeyword++ = ch;
			*szKeyword++ = *(++szInput);
		}
		else if ( (ch >= 'A' && ch <= 'Z') ||
				  (ch >= 'a' && ch <= 'z') ||
				  (ch >= '0' && ch <= '9')   )
		{
			if ( ch >= 'a' && ch <= 'z' ) ch -= 32;
			*szKeyword++ = ch;
		}
    }

    *szKeyword = '\0';
    CKeyword::deleteSpecialChar(ptr, ptr, bKeywordFlag);
}

VOID CKeyword::deleteSpecialChar(TCHAR *szInput , TCHAR *szOutput , BOOL bFlag)
{
	INT           i,j,idx; 
    TCHAR          s_tmpbeffer[10000];
    _TUCHAR ch1,ch2;
	
    idx = 0;
    if ( szInput == NULL ) 
	{
		szOutput = NULL  ;
		return;
	}
	
    _tcscpy( s_tmpbeffer , szInput );
	szOutput[0] = '\0';
	
	idx = 0;
	
    for ( i = 0; i < (INT)_tcsclen(s_tmpbeffer); i++ )
	{
        if ( (_TUCHAR)s_tmpbeffer[i] >= 0x80 ) 
		{
            if( i == (INT)_tcsclen(s_tmpbeffer) - 1 )
			{
                szOutput[idx] = '\0';
                break;
            }
			
            ch1 = (_TUCHAR)s_tmpbeffer[i];
            ch2 = (_TUCHAR)s_tmpbeffer[i+1];
                    
			for ( j = 0; j < SPECIAL_CHAR_COUNT ;j++) 
			{
                if ( _tcscmp( s_DelCharInfo[j].Comment , _T("그림문자"))  == 0 && bFlag == TRUE )
				{ 
                    continue;
                }
				
                if ( ch1 >= s_DelCharInfo[j].s_FirstChar && ch2 >= s_DelCharInfo[j].s_SecondChar &&
                     ch1 <= s_DelCharInfo[j].e_FirstChar && ch2 <= s_DelCharInfo[j].e_SecondChar )
				{
					
                    if ( _tcscmp( s_DelCharInfo[j].Comment , _T("숫자"))  == 0 )
					{
                        szOutput[idx++] = ch2 - 0x80;
                    }
					else if ( _tcscmp( s_DelCharInfo[j].Comment , _T("영문대문자"))  == 0 )
					{
						szOutput[idx++] = ch2 - 0x80;
					}
					else if( _tcscmp( s_DelCharInfo[j].Comment , _T("영문소문자"))  == 0 )
					{
						szOutput[idx++] = ch2 - 0xA0;
					}
					else if( _tcscmp( s_DelCharInfo[j].Comment , _T("한글확장") ) == 0 )
					{
						if ( ch2 <= 0x7F )
						{
							if ( ( ch2 >= 0x41 && ch2 <= 0x5A ) || ( ch2 >= 0x61 && ch2 <= 0x7A ) )
							{
								szOutput[idx++] = ch1;
								szOutput[idx++] = ch2;
							}
						}
						else
						{
							if ( ( ch1 <= 0xA0 ) && ( ch2 >= 0x81 && ch2 <= 0xFE ) ) 
							{
								szOutput[idx++] = ch1;
								szOutput[idx++] = ch2;
							}
							else if ( ( ch1 >= 0xA1 ) && ( ch2 >= 0x81 && ch2 <= 0xA0 ) )
							{
								szOutput[idx++] = ch1;
								szOutput[idx++] = ch2;
							}
						}
					}
					else
					{
						szOutput[idx++] = ch1;
						szOutput[idx++] = ch2;
					}

					break ;
                }
            }

            i++;

		}

		else
		{
            szOutput[idx++] = s_tmpbeffer[i];
		}
    }

	szOutput[idx] = '\0';
}