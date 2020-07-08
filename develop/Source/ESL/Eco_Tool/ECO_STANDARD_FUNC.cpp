
#include "stdafx.h"
#include "ECO_STANDARD_FUNC.h"

VOID* ECO_malloc (
INT AllocSize
)
{

_TCHAR *pTemp;

if ( AllocSize <= 0 ) return NULL;

pTemp = (_TCHAR *)malloc( AllocSize );
if ( pTemp == NULL ) return NULL;

wmemset( pTemp, 0x00, AllocSize );

return pTemp;

}

VOID * ECO_CopyMalloc (
INT AllocSize ,
VOID *OrgData
)
{

_TCHAR *pTemp;

if ( AllocSize <= 0 ) return NULL;
if ( OrgData == NULL ) return NULL;

pTemp = (_TCHAR *)malloc( AllocSize );
if ( pTemp == NULL ) return NULL;

wmemcpy( pTemp, (_TCHAR *)OrgData, AllocSize );

return pTemp;

}

VOID * ECO_realloc (
VOID *pData ,
INT AllocSize
)
{

_TCHAR *pTemp;

if ( AllocSize <= 0 ) {
ECO_free( pData );
return NULL;
}

pTemp = (_TCHAR *)realloc( pData, AllocSize );
if ( pTemp == NULL ) return NULL;

return pTemp;

}

VOID ECO_free (
VOID *pData
)
{

if ( pData == NULL ) return;
free( pData );
pData = NULL;

return;

}

INT ECO_strcmpi(
_TCHAR *String1 ,
_TCHAR *String2
)
{

if ( String1 == NULL && String2 == NULL ) return 0;
if ( String1 == NULL ) {
if ( _tcsclen( String2 ) == 0 ) return 0;
return 1;
}
if ( String2 == NULL ) {
if ( _tcsclen( String1 ) == 0 ) return 0;
return -1;
}

return _tcsicmp( String1, String2 );

}

VOID ECO_strcpy (
_TCHAR *String1 ,
_TCHAR *String2
)
{

if ( String1 == NULL ) return;
if ( String2 == NULL ) {
String1[0] = '\0';
return;
}

_tcscpy( String1, String2 );

return;

}
