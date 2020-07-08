// Parse.cpp: implementation of the CParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\공동목록\Parse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParse::CParse()
{
    m_QueryInfoCnt = 0;
    m_pQueryInfo   = NULL;
    m_Offset       = 0;
    m_BraceCnt     = 0;
    m_FieldCnt     = 0;
    m_EqualCnt     = 0;
    m_ValueCnt     = 0;
}

CParse::~CParse()
{
    Clear( );
}

VOID CParse::ParseQuery( TCHAR *lpQuery )
{

    INT   GueryLen = _tcsclen(lpQuery);
    INT   offset = 0;
    INT   len;
    TCHAR  szTmp[1024];

    Clear();
    while ( 1 ) {
        if ( GueryLen <= m_Offset ) break;

        // Is Brace ?
        if ( IsBrace (lpQuery[m_Offset]) ) {
            szTmp[0] = lpQuery[m_Offset];
            szTmp[1] = '\0';
            Push ( _BR_ , szTmp );
            m_Offset++;
        } else
        // Is Space ?
        if ( IsSpace (lpQuery[m_Offset]) ) {
            m_Offset++;
        } else
        // Is Operator ?
        if ( IsOperator (lpQuery[m_Offset]) ) {
            szTmp[0] = lpQuery[m_Offset];
            szTmp[1] = '\0';
            Push ( _OP_ , szTmp );
            m_Offset++;
        } else
        // Is Field ?
        if ( IsField (&lpQuery[m_Offset], &len ) ) {
            wmemcpy ( szTmp , &lpQuery[m_Offset] , len );
            szTmp[len] = '\0';
            Push ( _FN_ , szTmp );
            m_Offset += len;
            offset = 0;
            while ( 1 ) {
                if ( GueryLen <= m_Offset ) break;
                // Is Space ?
                if ( IsSpace (lpQuery[m_Offset]) ) {
                    m_Offset++;
                }
                // Is Brace ?
                if ( IsBrace (lpQuery[m_Offset]) ) {
                    break;
                }
                // Is Operator ?
                if ( IsOperator (lpQuery[m_Offset]) ) {
                    break;
                }
                // IsEqual '=' , '<=' , '>=' , '<', '>' ?
                if ( IsEqual( &lpQuery[m_Offset],&len ) ) {
                    wmemcpy ( szTmp , &lpQuery[m_Offset] , len );
                    szTmp[len] = '\0';
                    Push ( _EQ_ , szTmp );
                    m_Offset += len;
                    while ( 1 ) {
                        if ( GueryLen <= m_Offset ) {
                            szTmp[offset] = '\0';
                            Push ( _VA_ , szTmp );
                            break;
                        }
                        // Is Brace ?
                        if ( IsBrace (lpQuery[m_Offset]) ) {
                            szTmp[offset] = '\0';
                            Push ( _VA_ , szTmp );
                            break;
                        }
                        // Is Operator ?
                        if ( IsOperator (lpQuery[m_Offset]) ) {
                            szTmp[offset] = '\0';
                            Push ( _VA_ , szTmp );
                            break;
                        }
                        szTmp[offset] = lpQuery[m_Offset];
                        offset++;
                        m_Offset++;
                    }
                }
            }
        } else {
            m_Offset++;
        }
//        m_Offset++;
    }
}

INT CParse::Push( INT  Operator , TCHAR  *lpValue )
{
    m_pQueryInfo = (tQUERY_PARSE_INFO *)realloc( m_pQueryInfo , sizeof ( tQUERY_PARSE_INFO ) * ( m_QueryInfoCnt +1 ) );
    if ( NULL == m_pQueryInfo ) {
        return -1;
    }

    m_pQueryInfo[m_QueryInfoCnt].Opertor = Operator;
    _tcscpy ( m_pQueryInfo[m_QueryInfoCnt].szValue , lpValue );
    m_QueryInfoCnt++;

    // Brace check.
    if ( Operator == _BR_ ) {
        if ( lpValue[0] == '(' ) {
            m_BraceCnt++;
        } else
        if ( lpValue[0] == ')' ) {
            m_BraceCnt--;
        }
    }
    if ( Operator == _FN_ ) {
        m_FieldCnt++;
    }
    if ( Operator == _VA_ ) {
        m_ValueCnt++;
    }
    if ( Operator == _EQ_ ) {
        m_EqualCnt++;
    }
    return 0;
}

INT CParse::SyntaxCheckQuery( )
{
    if ( m_BraceCnt != 0 ) {
        return -100;
    }
    if ( m_EqualCnt == m_FieldCnt && m_EqualCnt == m_ValueCnt ) {
    } else {
        return -200;
    }
    if ( m_EqualCnt == 0 && m_FieldCnt == 0 && m_ValueCnt == 0 ) return -400;
    if ( m_EqualCnt == 0 || m_FieldCnt == 0 || m_ValueCnt == 0 ) return -300;
    return 0;
}

INT CParse::Pop ( TCHAR *lpField , TCHAR *lpEqual , TCHAR  *lpValue )
{


    return 0;
}

VOID CParse::Clear( )
{
    if ( m_pQueryInfo !=  NULL ) {
        free ( m_pQueryInfo );
        m_pQueryInfo = NULL;
    }
    m_Offset       = 0;
    m_QueryInfoCnt = 0;
    m_BraceCnt     = 0;
    m_FieldCnt     = 0;
    m_EqualCnt     = 0;
    m_ValueCnt     = 0;
}

BOOL CParse::IsSpace( TCHAR ch )
{
    if ( ch == ' ' ) {
        return TRUE;
    } else {
        return FALSE;
    }
}
BOOL CParse::IsBrace( TCHAR ch )
{
    if ( ch == '(' || ch == ')' ) {
        return TRUE;
    } else {
        return FALSE;
    }
}
BOOL CParse::IsEqual( TCHAR *lpStream , INT *lpLength )
{
    if (   memicmp( lpStream , _T("<=")  , 2 ) == 0 ||  memicmp( lpStream , _T(">=") , 2 ) == 0 ) {
        *lpLength = 2;
        return TRUE;
    } else 
    if (   memicmp( lpStream , _T("=")  , 1 ) == 0 ||  memicmp( lpStream , _T("<") , 1 ) == 0 || memicmp( lpStream , _T(">") , 1 ) == 0  ) {
        *lpLength = 1;
        return TRUE;
    } else {
        *lpLength = 0;
        return FALSE;
    }
}
BOOL CParse::IsOperator( TCHAR ch )
{
    if ( ch == '*' || ch == '+' ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL CParse::IsField( TCHAR *lpStream , INT *lpLength )
{
    if ( memicmp( lpStream , _T("TI") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("TK") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("AU") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("SU") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("PN") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("IB") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("IS") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("STRN") , 4 )  == 0 ) {
        *lpLength = 4;
    } else 
    if ( memicmp( lpStream , _T("RN") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("CD") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("ISRC") , 4 )  == 0 ) {
        *lpLength = 4;
    } else 
    if ( memicmp( lpStream , _T("UPC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("ISMN") , 4 )  == 0 ) {
        *lpLength = 4;
    } else 
    if ( memicmp( lpStream , _T("EAN") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("SICI") , 4 )  == 0 ) {
        *lpLength = 4;
    } else 
    if ( memicmp( lpStream , _T("GD") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("KDCP") , 4 )  == 0 ) {
        *lpLength = 4;
    } else 
    if ( memicmp( lpStream , _T("KDC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("DDC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("CEC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("CWC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("COC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("UDC") , 3 )  == 0 ) {
        *lpLength = 3;
    } else 
    if ( memicmp( lpStream , _T("LCC") , 3 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("UC") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("GP") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("DPLOLI") , 6 )  == 0 ) {
        *lpLength = 6;
    } else 
    if ( memicmp( lpStream , _T("TH") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("PY") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("LG") , 2 )  == 0 ) {
        *lpLength = 2;
    } else 
    if ( memicmp( lpStream , _T("FR") , 2 )  == 0 ) {
        *lpLength = 2;
    } else {
        *lpLength = 0;
        return FALSE;
    }
    return TRUE;
}
