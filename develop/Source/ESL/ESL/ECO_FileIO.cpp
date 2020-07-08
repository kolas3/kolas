// CECO_FileIO.cpp: implementation of the CECO_FileIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "except.h"
#include "ECO_FileIO.h"
#include "ECO_FILE_API.h"
#include "ECO_STANDARD_FUNC.h"
#include "efs.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECO_FileIO::CECO_FileIO()
{

}

CECO_FileIO::~CECO_FileIO()
{

}

INT CECO_FileIO::get_line(
		 FILE *fp ,
		 _TCHAR *s_line ,
		 INT *s_cnt
				 )
{
EFS_BEGIN
    SHORT col;
    _TINT c;
    for ( col = 0; col < 100000 && (c=FdkFgetc(fp)) != _TEOF && c != _T('\n'); col++) s_line[col] = c;

    *s_cnt = col;
    if ( c == _T('\n') ) s_line[col++] = c;
    s_line[col]= _T('\0');

    if (c == _TEOF && (*s_cnt) == 0) {return -1;}

EFS_END
    return( 0 );

}


INT CECO_FileIO::next_line (
								FILE *fp ,
								_TCHAR *ctmp ,
								INT *line_length
							)
{
EFS_BEGIN
    ctmp[0] = _T(' ');
    do {
    if (get_line(fp, ctmp, (INT *)line_length) == -1) {return -1;}
    if (ctmp[0] == _T('.') || *(INT*)line_length == 0) continue;
    else break;
    } while (1);

EFS_END
    return 0;
}

INT CECO_FileIO::chrsrch( INT option   ,
						  _TCHAR *ctmp ,
						  INT range    ,  
		                  _TCHAR ch	   )
{
EFS_BEGIN
	INT i;

    switch (option) {
    case 1:
        for(i = 1; i <= range; i++) if (ch == ctmp[i-1]) {return i;}
    case 2:
        for(i = 1; i <= range; i++) if (ch != ctmp[i-1]) {return i;}
    }

EFS_END
    return 0;
}


INT CECO_FileIO::str$trimall( _TCHAR *strln    ,
							  _TCHAR *strline1 ,
							  _TCHAR var       )
{
EFS_BEGIN
	INT i = 0, j = 0;

    for( i=0; strln[i] != _T('\0') && strln[i] != _T('\n'); i++){
        if(strln[i] == var);
        else {
        strline1[j] = strln[i]; j++;
        }
    }
    strline1[j] =_T('\0');

EFS_END
    return 0;
}


VOID CECO_FileIO::str_trimall( _TCHAR *strln    ,
		                       _TCHAR *strline1 ,
		                       _TCHAR var       )
{
EFS_BEGIN
    INT i = 0, j = 0;

    for ( i = 0; strln[i] != _T('\0') && strln[i] != _T('\n'); i++ ){
        if(strln[i] == var);
        else { strline1[j] = strln[i]; j++; }
    }

    strline1[j] =_T('\0');
EFS_END
}

VOID CECO_FileIO::strim ( _TCHAR *a ,
						  _TCHAR *b )
{
EFS_BEGIN
    INT i = 0, j = 0;

    str_trimall( a, b, _T('\t') );
    if ( a[0] == 32 ) {
        for ( i = 0; a[i+1] != _T('\0'); i++ ) {
            if ( a[i] == 32 && a[i+1] == 32 );
            else { b[j] = a[i+1]; j++; }
        }
        a[j] = _T('\0');
    }

    if ( a[0] != 32 ) {
        for ( i = 0; a[i] != _T('\0'); i++ ) {
            if( a[i] == 32 && a[i+1] ==32 );
            else { b[j] = a[i]; j++; }
        }
        b[j] = _T('\0');
    }
EFS_END
}

VOID CECO_FileIO::str_ltrim	( _TCHAR *strln    ,
		                      _TCHAR *strline1 ,
		                      _TCHAR var       )
{
EFS_BEGIN
    INT i = 0;

    for ( i = 0; strln[i] != _T('\0') && strln[i] != _T('\n'); i++ ){
        if(strln[i] != var) {
            _tcscpy( strline1, &strln[i] );
            {return;}
        }
    }
    strline1[0] = _T('\0');
EFS_END
}

VOID CECO_FileIO::str_rtrim	( _TCHAR *strln    ,
		                      _TCHAR *strline1 ,
		                      _TCHAR  var	   )
{
EFS_BEGIN
	INT i = 0;

    _tcscpy( strline1, strln );
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
        if(strln[i] != var) {
            strline1[i+1] = _T('\0');
            {return;}
        }
    }
    strline1[0] = _T('\0');
EFS_END
}

INT CECO_FileIO::GetTagData	( _TCHAR *Source ,
							  _TCHAR *alias  ,
							  _TCHAR *s_data )
{
EFS_BEGIN
    TCHAR *TokenStr;
    TCHAR ctmp[4096];
    
    _tcscpy( ctmp, Source );
    TokenStr = _tcstok( ctmp, _T("=") );
    if ( NULL == TokenStr ) {return -1;}

    _tcscpy( alias, TokenStr );
    str_trimall( alias, alias, _T(' ') );

    TokenStr = _tcstok( NULL, _T("^") );
    if ( NULL == TokenStr ) {
        s_data[0] = _T('\0');
    } else {
        _tcscpy( s_data, TokenStr );
    }
    str_rtrim( s_data, s_data, _T(' ') );

EFS_END
    return 0;
}


INT CECO_FileIO::str$token( _TCHAR *srcString  ,
							_TCHAR *delimiter  ,
							INT *tokenCnt      ,
							_TCHAR ***TokenData )
{
EFS_BEGIN
	_TCHAR **OldDataPtr,tmpSrcString[4096];
    INT      i,length, OldPtr;
    
    *tokenCnt = OldPtr = 0;
    _tcscpy( tmpSrcString, srcString );

    OldDataPtr = NULL;
    length = _tcsclen( tmpSrcString );
    for ( i = 0; i < length; i++ ) {
		if ( tmpSrcString[i] == delimiter[0] ) {
            (*tokenCnt)++;
            OldDataPtr = (_TCHAR **)realloc( OldDataPtr, sizeof(_TCHAR*) * (*tokenCnt) );
            if ( OldDataPtr == NULL ) {
                {return -2;}
            }
            OldDataPtr[(*tokenCnt)-1] = (_TCHAR *)malloc( i - OldPtr + 1);

            if ( OldDataPtr[(*tokenCnt)-1] == NULL ) {
                for ( i=0; i < *tokenCnt-1; i++ ) free( OldDataPtr[i] );
                free( OldDataPtr );
                {return -3;}
            }

            tmpSrcString[i] = '\0';
            _tcscpy ( OldDataPtr[*tokenCnt-1], &tmpSrcString[OldPtr] );
            OldPtr = i+1;
        }else
		if(tmpSrcString[i] == delimiter[1]){
			(*tokenCnt)++;
            OldDataPtr = (_TCHAR **)realloc( OldDataPtr, sizeof(_TCHAR*) * (*tokenCnt) );
            if ( OldDataPtr == NULL ) {
                {return -2;}
            }
            OldDataPtr[(*tokenCnt)-1] = (_TCHAR *)malloc( i - OldPtr + 1);
            if ( OldDataPtr[(*tokenCnt)-1] == NULL ) {
                for ( i=0; i < *tokenCnt-1; i++ ) free( OldDataPtr[i] );
                free( OldDataPtr );
                {return -3;}
            }

            tmpSrcString[i] = '\0';
            _tcscpy ( OldDataPtr[*tokenCnt-1], &tmpSrcString[OldPtr] );
            OldPtr = i+1;
		}
   }

   (*tokenCnt)++;

   OldDataPtr = (_TCHAR **)realloc( OldDataPtr, sizeof(_TCHAR*) * (*tokenCnt) );
   if ( OldDataPtr == NULL ) {
        {return -4;}
   }

   OldDataPtr[(*tokenCnt)-1] = (_TCHAR *)malloc( length - OldPtr + 1);
   if ( OldDataPtr[(*tokenCnt)-1] == NULL ) {
       for ( i=0; i < *tokenCnt-1; i++ ) free( OldDataPtr[i] );
       free( OldDataPtr );
       {return -5;}
   }

   tmpSrcString[i] = '\0';
   _tcscpy ( OldDataPtr[*tokenCnt-1], &tmpSrcString[OldPtr] );

   *TokenData = OldDataPtr;

EFS_END
   return 0;
}
