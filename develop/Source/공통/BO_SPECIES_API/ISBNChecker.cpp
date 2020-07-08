// ISBNChecker.cpp: implementation of the CISBNChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ISBNChecker.h"

#include "Keyword.h"
			   
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CISBNChecker::CISBNChecker()
{

}

CISBNChecker::~CISBNChecker()
{

}

INT CISBNChecker::ConvertISBN (TCHAR* szUnFormattedISBN, 
							   TCHAR* szCorrectFormattedISBN, 
							   INT *nCorrect, 
							   BOOL  b89/*=TRUE*/) 
{
    INT     ISBNData;
	TCHAR    *szExtraStrPointer;
    TCHAR    szTemp[128];
	TCHAR    szTempISBN[128];
    INT     i, j, nCheckSum, nCheckRest, nFirstDash, nSecondDash, nThirdDash;
	
	*nCorrect = 0;

    szExtraStrPointer = _tcschr( szUnFormattedISBN, '(' );

	_tcscpy(szTemp, szUnFormattedISBN); 
	_tcscpy(szTempISBN, szUnFormattedISBN);

	CKeyword::getKeyword(szTempISBN , szTempISBN, FALSE);

	switch( _tcsclen( szTempISBN ) )
	{
	case 10:
		{
			if( b89 == TRUE )
			{
				if( _tcsncmp( szTempISBN, _T("89"), 2 ) != 0 )
				{
					_tcscpy( szCorrectFormattedISBN, szUnFormattedISBN );
					return 0;
				}
			}
			
			nCheckSum = 0;
			szTemp[1] = 0;
    
			for( i = 0, j = 10 ; i < 9 ; i++, j-- ) 
			{
				szTemp[0] = szTempISBN[i];
				nCheckSum += ( _ttoi(szTemp) * j );
			}

			nCheckRest = nCheckSum % 11;

			switch (nCheckRest) 
			{
				case 0:
					{
						if( szTempISBN[9] != '0' )
						{
							szTempISBN[9] = '0';
							*nCorrect = -2;
						}
					}
					break;
				case 1:
					{
						if( szTempISBN[9] != 'x' && szTempISBN[9] != 'X' )
						{
							szTempISBN[9] = 'X';
							*nCorrect = -2;
						}
					}
					break;
				default:
					{
						_stprintf( szTemp, _T("%d"), 11 - nCheckRest );
						if( szTemp[0] != szTempISBN[9] )
						{
							szTempISBN[9] = szTemp[0];
							*nCorrect = -2;
						}
					}
					break;
			}

			nFirstDash = 2;
			nThirdDash = 11;

			wmemcpy( szTemp , &szTempISBN[2] , 2 );
			szTemp[2] = 0;
			ISBNData = _ttoi( szTemp );

			if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5;
			if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6;
			if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7;
			if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8;
			if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9;

			wmemcpy( szCorrectFormattedISBN                  , szTempISBN                 , 2 );
			wmemcpy( szCorrectFormattedISBN +(nFirstDash +1) , szTempISBN +2              , nSecondDash-nFirstDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-1) , nThirdDash-nSecondDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN +9              , 1 );

			szCorrectFormattedISBN[nFirstDash]	= '-';
			szCorrectFormattedISBN[nSecondDash] = '-';
			szCorrectFormattedISBN[nThirdDash]	= '-';
			szCorrectFormattedISBN[13]			= 0	 ;

			if ( szExtraStrPointer != NULL ) 
				_tcscpy( &szCorrectFormattedISBN[13], szExtraStrPointer );

		}
		break;

	case 13:
		{
			if( b89 == TRUE )
			{
				if( _tcsncmp( szTempISBN+3, _T("89"), 2 ) != 0 )
				{
					_tcscpy( szCorrectFormattedISBN, szUnFormattedISBN );
					return 0;
				}
			}

			nCheckSum = 0;
			szTemp[1] = 0;

			for( i=0; i<12; i++ )
			{
				szTemp[0] = szTempISBN[i];
				if( i%2 )	nCheckSum += _ttoi(szTemp) * 3;
				else		nCheckSum += _ttoi(szTemp) * 1;
			}

			nCheckRest = nCheckSum%10;
			if( nCheckRest != 0 )
			{
				_stprintf( szTemp, _T("%d"), (10-nCheckRest) );
			}
			else
			{
				_stprintf( szTemp, _T("%d"), nCheckRest );
			}

			if( szTemp[0] != szTempISBN[12] )
			{
				szTempISBN[12] = szTemp[0];
				*nCorrect = -2;
			}

			INT nAddDash = 3;
			nFirstDash = 2 + nAddDash + 1;
			nThirdDash = 11 + nAddDash + 1;

			wmemcpy( szTemp , &szTempISBN[2+nAddDash] , 2 );
			szTemp[2] = 0;
			ISBNData = _ttoi( szTemp );

			if ( 0  <= ISBNData && ISBNData <= 24 ) nSecondDash = 5 + nAddDash +1;
			if ( 25 <= ISBNData && ISBNData <= 54 ) nSecondDash = 6 + nAddDash +1;
			if ( 55 <= ISBNData && ISBNData <= 84 ) nSecondDash = 7 + nAddDash +1;
			if ( 85 <= ISBNData && ISBNData <= 94 ) nSecondDash = 8 + nAddDash +1;
			if ( 95 <= ISBNData && ISBNData <= 99 ) nSecondDash = 9 + nAddDash +1;

			wmemcpy( szCorrectFormattedISBN                  , szTempISBN                , nAddDash );
			wmemcpy( szCorrectFormattedISBN +(nAddDash +1)   , szTempISBN+nAddDash       , 2 );
			wmemcpy( szCorrectFormattedISBN +(nFirstDash+1)  , szTempISBN+nAddDash+2     , nSecondDash-nFirstDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nSecondDash+1) , szTempISBN+(nSecondDash-2), nThirdDash-nSecondDash-1 );
			wmemcpy( szCorrectFormattedISBN +(nThirdDash +1) , szTempISBN+nAddDash+9     , 1 );

			szCorrectFormattedISBN[nAddDash]	= '-';
			szCorrectFormattedISBN[nFirstDash]	= '-';
			szCorrectFormattedISBN[nSecondDash] = '-';
			szCorrectFormattedISBN[nThirdDash]	= '-';
			szCorrectFormattedISBN[17]			= 0	 ;

			if( szExtraStrPointer != NULL ) 
			{
				_tcscpy( szCorrectFormattedISBN+17, szExtraStrPointer );
			}

		}
		break;

	default:
		{
			{
				_tcscpy( szCorrectFormattedISBN, szUnFormattedISBN );
				*nCorrect = -1;
				return -1;
			}
		}
		break;
	}
  
    return 0;
}

INT CISBNChecker::ReturnISBNCheckDigit( TCHAR *szInputISBN, TCHAR *szOutputISBN, TCHAR *szReturnValue, BOOL b89/*=TRUE*/)
{
	INT i, j, nCheckSum, nCheckRest, nISBNData, nDash;
	TCHAR szTemp[3], szISBN[256];

	nCheckSum = 0;
	ZeroMemory(szISBN, sizeof(TCHAR)*256);
	ZeroMemory(szTemp, sizeof(TCHAR)*3);
	szTemp[1] = 0;
	
	_tcscpy( szISBN, szInputISBN );
	
	CKeyword::getKeyword( szISBN, szISBN, TRUE );

	switch( _tcsclen(szISBN) )
	{
	case 9:
		{
			if( b89 == TRUE )
			{
				if( _tcsncmp( szISBN, _T("89"), 2 ) != 0 )
				{
					szReturnValue[0] = 0;
					return 100;
				}
			}

			for( i=0, j=10; i<9; i++, j-- )
			{
				szTemp[0] = szISBN[i];
				nCheckSum += ( _ttoi( szTemp ) * j );
			}

			nCheckRest = nCheckSum % 11;

			switch( nCheckRest )
			{
				case 0 :
					_stprintf( szReturnValue, _T("%d"), nCheckRest );
					break;
				case 1 :
					_stprintf( szReturnValue, _T("X") );
					break;
				default :
					_stprintf( szReturnValue, _T("%d"), 11 - nCheckRest );
					break;
			}

			wmemcpy( szTemp, &szISBN[2], 2 );
			szTemp[2] = 0;
			nISBNData = _ttoi( szTemp );
			
			if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
			if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
			if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
			if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
			if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

			wmemcpy( szOutputISBN, szISBN + 2, nDash );
			wmemcpy( szOutputISBN + nDash + 1, szISBN + nDash + 2 , 9 - nDash );

			szOutputISBN[nDash] = '-';
			szOutputISBN[8] = 0;
		}
		break;
	case 12:
		{
			if( b89 == TRUE )
			{
				if( _tcsncmp( szISBN+3, _T("89"), 2 ) != 0 )
				{
					szReturnValue[0] = 0;
					return 100;
				}
			}

			nCheckSum = 0;
			for( i=0; i<12; i++ )
			{
				szTemp[0] = szISBN[i];
				if( i%2 )	nCheckSum += _ttoi(szTemp) * 3;
				else		nCheckSum += _ttoi(szTemp) * 1;
			}

			nCheckRest = nCheckSum%10;
			if( nCheckRest != 0 )
			{
				_stprintf( szReturnValue, _T("%d"), (10-nCheckRest) );				
			}
			else
			{
				_stprintf( szReturnValue, _T("%d"), nCheckRest );
			}

			_tcsnccpy( szTemp, &szISBN[5], 2 );
			szTemp[2] = 0;
			nISBNData = _ttoi( szTemp );
			
			if ( 0  <= nISBNData && nISBNData <= 24 ) nDash = 2;
			if ( 25 <= nISBNData && nISBNData <= 54 ) nDash = 3;
			if ( 55 <= nISBNData && nISBNData <= 84 ) nDash = 4;
			if ( 85 <= nISBNData && nISBNData <= 94 ) nDash = 5;
			if ( 95 <= nISBNData && nISBNData <= 99 ) nDash = 6;

			wmemcpy( szOutputISBN, szISBN + 5, nDash );
			wmemcpy( szOutputISBN + nDash + 1, szISBN + 5 + nDash , 12 - nDash );

			szOutputISBN[nDash] = '-';
			szOutputISBN[8] = 0;
		}
		break;

	default:
		return -1;
	}

	return 0;
}

VOID CISBNChecker::RemoveDashFromISBN(TCHAR* szSource, TCHAR* szDest )
{
    BOOL b = FALSE;
	
	for( ; *szSource; szSource++ )
	{
        if( *szSource == '(' )		b = TRUE;
		
        if( b ||
			( *szSource >= '0' && *szSource <= '9' ) ||
			( *szSource >= 'A' && *szSource <= 'Z' ) ||
			( *szSource >= 'a' && *szSource <= 'z' )   ) 
		{
            *szDest++ = *szSource;
        }
	}
    *szDest = 0;
}

