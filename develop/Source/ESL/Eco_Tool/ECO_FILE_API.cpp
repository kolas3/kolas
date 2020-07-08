#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "ECO_FILE_API.h"

//
#define _UNIFILE_START_CHAR 0xFEFF

//
//#define FDK_EOF (0x1A)
#define FDK_EOF WEOF

/*
#undef EOF
#define EOF (int)(0x1A)
#undef WEOF
#define WEOF (wint_t)(0x1A)
*/

// CR-LF 처리 때문에 유니코드버전에서는 항상Binary Mode로 처리하게 해놨음.

FILE * FdkFopen ( const _TCHAR * filename, const _TCHAR * mode )
{
	FILE* fp;

#ifdef _UNICODE
	fp = _wfopen( filename, mode );
	if ( fp == NULL ) return NULL;

	INT prev_mode = _setmode( _fileno( fp ), _O_BINARY );
	if ( prev_mode == -1 ) {
		FdkFclose(fp);
		return NULL;
	}

	if ( ftell(fp) == 0 ) {
		_TINT ids = FdkFgetc( fp );
		if ( ids == _UNIFILE_START_CHAR ) {
			//fseek( fp, 2, SEEK_CUR );
		} else {
			fseek( fp, 0, SEEK_SET );
			ids = FdkFputc( _UNIFILE_START_CHAR, fp );
			if ( ids == FDK_EOF ) {
				FdkFclose( fp );
				return NULL;
			}
		}
	}

#else
	fp = _tfopen( filename, mode );

#endif
	return fp;
}

_TINT FdkFputs ( const _TCHAR * string, FILE * stream )
{
	_TINT ids;
#ifdef _UNICODE

	do {
		ids = FdkFputc( *string, stream );
		if ( ids==WEOF ) return ids;
		string++;
	} while( *string != _T('\0') );
	return 0;

	//ids = fwrite( string, sizeof(_TCHAR), wcslen(string), stream );
	//return ( ids - wcslen(string) );
#else
	ids = _fputts( string, stream );
	return ids;
#endif
}

_TINT FdkFputc ( _TINT c, FILE * stream )
{
	_TINT ids;
#ifdef _UNICODE
	//BOOL bTranslate = FALSE;
	//INT prev_mode, result;
	if ( c==_T('\n') ) {
		//bTranslate = TRUE;
		//prev_mode = _setmode( _fileno( stream ), _O_BINARY );
		//if ( prev_mode == -1 ) return WEOF;
		ids = FdkFputc( _T('\r'), stream );
		if (ids==FDK_EOF) {
			//result = _setmode( _fileno( stream ), prev_mode );
			//if ( result == -1 ) return WEOF;
			return ids;
		}
	}

	INT a1, a2, o1, o2;
	a1 = c >> 8;
	a2 = c - (a1<<8);
	o1 = _fputtc( a2, stream );
	if ( o1==_TEOF ) return FDK_EOF;
	o2 = _fputtc( a1, stream );
	if ( o2==_TEOF ) return FDK_EOF;
	ids = (o2<<8) + o1;

	/*if (bTranslate) {
		result = _setmode( _fileno( stream ), prev_mode );
		if ( result == -1 ) return WEOF;
	}*/
#else
	ids = _fputtc( c, stream );
#endif
	return ids;
}

_TCHAR * FdkFgets ( _TCHAR * string, INT count, FILE * str )
{
#ifdef _UNICODE
	FILE *stream;
	_TCHAR *pointer = string;
	_TCHAR *retval = string;
	_TINT ch;

	_ASSERTE(string != NULL);
	_ASSERTE(str != NULL);

	if (count <= 0) return(NULL);

	stream = str;

	while (--count) {
		//if ((ch = _getwc_lk(stream)) == WEOF)
		if ( (ch = FdkFgetc(stream)) == FDK_EOF ) {
			if (pointer == string) {
				retval=NULL;
				goto done;
			}

			break;
		}

		if ((*pointer++ = (_TCHAR)ch) == _T('\n')) break;
	}

	*pointer = _T('\0');

done:
	return(retval);
#else
	return _fgetts( string, n, stream );
#endif
}

_TINT FdkFgetc ( FILE * stream )
{
	_TINT ids;
#ifdef _UNICODE
	INT a1, a2;
	a1 = _fgettc( stream );
	if ( a1==_TEOF ) return FDK_EOF;
	a2 = _fgettc( stream );
	if ( a1==_TEOF ) return FDK_EOF;
	ids = (a2<<8) + a1;
	if (ids==_T('\r')) ids = FdkFgetc(stream);
#else
	ids = _fgettc( stream );
#endif
	return ids;
}

INT FdkFseek ( FILE * stream, long offset, INT origin )
{
#ifdef _UNICODE
	if ( origin == SEEK_SET ) offset += 2;
#endif
	return fseek(stream, offset, origin);
}

size_t FdkFread ( VOID * buffer, size_t size, size_t count, FILE * stream )
{
	return fread( buffer, size, count, stream );
}

INT FdkFclose ( FILE * stream )
{
	return fclose( stream );
}

INT FdkFprintf ( FILE * stream, const _TCHAR * format, ...)
{
	INT ids;
	_TCHAR buffer[4096];
	va_list arglist;

	va_start(arglist, format);

	_ASSERTE(stream != NULL);
	_ASSERTE(format != NULL);

	ids = vswprintf( buffer, format, (va_list)arglist );
	if (ids<0) return ids;

	va_end(arglist);

#ifdef _UNICODE
	FdkFputs(buffer, stream);
#else
	ids = _ftprintf(stream,format,arglist);
#endif
	return ids;
}

size_t FdkFwrite ( const VOID * buffer, size_t size, size_t count, FILE * stream )
{
	return fwrite( buffer, size, count, stream );
}

INT FdkFeof ( FILE * stream )
{
	return feof(stream);
}

INT FdkFscanf ( FILE * stream, const _TCHAR * format, ...)
{
	return 1;
/*
	INT retval;
	va_list arglist;

	va_start(arglist, format);

	_ASSERTE(stream != NULL);
	_ASSERTE(format != NULL);

#ifdef _UNICODE
	retval = fwscanf(stream,format,arglist);
#else
	retval = _ftscanf(stream,format,arglist);
#endif
	return(retval);
*/
}

INT FdkSscanf ( const _TCHAR * buffer, const _TCHAR * format, ...)
{
	INT ids;
	va_list arglist;

	va_start(arglist, format);

	_ASSERTE(format != NULL);

#ifdef _UNICODE
	ids = swscanf(buffer,format,arglist);
#else
	ids = _ftscanf(buffer,format,arglist);
#endif
	return (ids);
}

INT FdkRename ( const _TCHAR * oldname, const _TCHAR * newname )
{
	INT ids;
#ifdef _UNICODE
	ids = _wrename( oldname, newname );
#else
	ids = _trename( oldname, newname );
#endif
	return ids;
}

_TCHAR * _FdkGetcwd ( _TCHAR * buffer, INT maxlen )
{
	_TCHAR* ids;
#ifdef _UNICODE
	ids = _wgetcwd( buffer, maxlen );
#else
	ids = _tgetcwd( buffer, maxlen );
#endif
	return ids;
}

INT _FdkChdir ( const _TCHAR * dirname )
{
	INT ids;
#ifdef _UNICODE
	ids = _wchdir( dirname );
#else
	ids = _tchdir( dirname );
#endif
	return ids;
}

INT _FdkMkdir ( const _TCHAR * dirname )
{
	INT ids;
#ifdef _UNICODE
	ids = _wmkdir( dirname );
#else
	ids = _tmkdir( dirname );
#endif
	return ids;
}

