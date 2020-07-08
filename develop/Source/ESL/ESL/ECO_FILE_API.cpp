#include "stdafx.h"
#include "ECO_FILE_API.h"

#define _UNIFILE_START_CHAR 0xFEFF

#define _SHORT_ISBN_LNG    10
#define _LONG_ISBN_LNG     13

#define FDK_EOF WEOF


FILE * FdkFopen ( const _TCHAR * filename, const _TCHAR * mode )
{
	FILE* fp;

	CString	sBuffer;

#ifdef _UNICODE
	fp = _wfopen( filename, mode );
	if ( fp == NULL ) {return NULL;}

	int prev_mode = _setmode( _fileno( fp ), _O_BINARY );
	if ( prev_mode == -1 ) {
		FdkFclose(fp);
		{return NULL;}
	}

	sBuffer = mode;
	if ( ftell(fp) == 0 && sBuffer.Find(_T("a")) == -1 ) 
	{
		_TINT ids = FdkFgetc( fp );
		if ( ids == _UNIFILE_START_CHAR ) 
		{
		} 
		else 
		{
			fseek( fp, 0, SEEK_SET );
			ids = FdkFputc( _UNIFILE_START_CHAR, fp );
			if ( ids == FDK_EOF ) 
			{
				FdkFclose( fp );
				{return NULL;}
			}
		}
	}
	else if ( sBuffer.Find(_T("a")) != -1 )
	{
		FdkFclose( fp );
		if ( ( fp = FdkFopen(filename, _T("r"))) == NULL )
			fp = FdkFopen(filename, _T("w"));
		FdkFclose( fp );

		fp = _wfopen( filename, mode );
		if ( fp == NULL ) {return NULL;}
		
		int prev_mode = _setmode( _fileno( fp ), _O_BINARY );
		if ( prev_mode == -1 ) 
		{
			FdkFclose(fp);
			{return NULL;}
		}
	}

#else
	fp = fopen( filename, mode );

#endif

	return fp;
}

_TINT FdkFputs ( const _TCHAR * string, FILE * stream )
{

	_TINT ids;
#ifdef _UNICODE

	do {
		ids = FdkFputc( *string, stream );
		if ( ids==WEOF ) {return ids;}
		string++;
	} while( *string != _T('\0') );

	return 0;
#else
	ids = fputs( string, stream );

	return ids;
#endif
}

_TINT FdkFputc ( _TINT c, FILE * stream )
{
	_TINT ids;

#ifdef _UNICODE
	if ( c==_T('\n') ) {
		ids = FdkFputc( _T('\r'), stream );
		if (ids==FDK_EOF) {
			return ids;
		}
	}

	int a1, a2, o1, o2;
	a1 = c >> 8;
	a2 = c - (a1<<8);
	o1 = fputc( a2, stream );
	if ( o1==EOF ) {return FDK_EOF;}
	o2 = fputc( a1, stream );
	if ( o2==EOF ) {return FDK_EOF;}
	ids = (o2<<8) + o1;
#else
	ids = fputc( c, stream );
#endif

	return ids;
}

_TCHAR * FdkFgets ( _TCHAR * string, int count, FILE * str )
{
	_TCHAR *retval = string;
#ifdef _UNICODE
	FILE *stream;
	_TCHAR *pointer = string;
	_TINT ch;

	_ASSERTE(string != NULL);
	_ASSERTE(str != NULL);

	if (count <= 0) { return (NULL); }

	stream = str;

	while (--count) {
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
	return fgets( string, n, stream );
#endif
}

_TINT FdkFgetc ( FILE * stream )
{
	_TINT ids;

#ifdef _UNICODE
	int a1, a2;
	a1 = fgetc( stream );
	if ( a1==EOF ) {return FDK_EOF;}
	a2 = fgetc( stream );
	if ( a1==EOF ) {return FDK_EOF;}
	ids = (a2<<8) + a1;
	if (ids==_T('\r')) ids = FdkFgetc(stream);
#else
	ids = fgetc( stream );
#endif

	return ids;
}

int FdkFseek ( FILE * stream, long offset, int origin )
{

#ifdef _UNICODE
	if ( origin == SEEK_SET ) offset += 2;
#endif

	return fseek(stream, offset, origin);
}

size_t FdkFread ( void * buffer, size_t size, size_t count, FILE * stream )
{
	return fread( buffer, size, count, stream );
}

int FdkFclose ( FILE * stream )
{
	return fclose( stream );
}

int FdkFprintf ( FILE * stream, const _TCHAR * format, ...)
{
	int ids;

	_TCHAR*g_szBuffer = NULL;
	va_list arglist;

	g_szBuffer = (TCHAR*)malloc( sizeof( TCHAR ) * 200000 );
	va_start(arglist, format);

	_ASSERTE(stream != NULL);
	_ASSERTE(format != NULL);

	ids = vswprintf( g_szBuffer, format, (va_list)arglist );
	if (ids<0) {return ids;}

	va_end(arglist);

#ifdef _UNICODE
	FdkFputs(g_szBuffer, stream);
#else
	ids = fprintf(stream,format,arglist);
#endif

	free( g_szBuffer );

	return ids;
}

size_t FdkFwrite ( const void * buffer, size_t size, size_t count, FILE * stream )
{
	return fwrite( buffer, size, count, stream );
}

int FdkFeof ( FILE * stream )
{
	return feof(stream);
}

long   FdkFtell    (FILE *fp)
{
	return ftell(fp);
}

INT UniCodeToAnsiString(TCHAR * inUniCodeString, CHAR ** outAnsiString, INT * nUseByte)
{
	INT		nNeedByte, nLen;

	if ( NULL == inUniCodeString ) {
		*outAnsiString = new CHAR[5];
		*outAnsiString[0] = '\0';
		*nUseByte = 0;
		return 0;
	}

	nLen = _tcslen(inUniCodeString);
	if ( nLen <= 0 ) {
		*outAnsiString = NULL;
		*nUseByte = 0;
		return -10;
	}

	nNeedByte = WideCharToMultiByte( CP_ACP			 	, 
								0						,
								inUniCodeString			, 
								-1						,
								NULL					, 
								0						,
								NULL					, 
								NULL      );
	if ( 0 == nNeedByte ) {
		*outAnsiString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*outAnsiString = new CHAR[nNeedByte];
	if ( NULL == *outAnsiString ) {
		*nUseByte = 0;
		return -100;
	}

	nNeedByte = WideCharToMultiByte( CP_ACP			 	, 
								0						,
								inUniCodeString			, 
								-1						,
								(*outAnsiString)		, 
								nNeedByte				,
								NULL					, 
								NULL      );
	if ( 0 == nNeedByte ) {
		delete [] *outAnsiString;
		*outAnsiString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}
	*nUseByte = nNeedByte;
	
	return 0;
}

INT UniCodeToUtf8String(TCHAR * inUniCodeString, CHAR ** outAnsiString, INT * nUseByte)
{
	INT		nNeedByte, nLen;

	if ( NULL == inUniCodeString ) {
		*outAnsiString = new CHAR[5];
		*outAnsiString[0] = '\0';
		*nUseByte = 0;
		return 0;
	}

	nLen = _tcslen(inUniCodeString);
	if ( nLen <= 0 ) {
		*outAnsiString = NULL;
		*nUseByte = 0;
		return -10;
	}

	nNeedByte = WideCharToMultiByte( CP_UTF8		 	, 
								0						,
								inUniCodeString			, 
								-1						,
								NULL					, 
								0						,
								NULL					, 
								NULL      );
	if ( 0 == nNeedByte ) {
		*outAnsiString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*outAnsiString = new CHAR[nNeedByte];
	if ( NULL == *outAnsiString ) {
		*nUseByte = 0;
		return -100;
	}

	nNeedByte = WideCharToMultiByte( CP_UTF8		 	, 
								0						,
								inUniCodeString			, 
								-1						,
								(*outAnsiString)		, 
								nNeedByte				,
								NULL					, 
								NULL      );
	if ( 0 == nNeedByte ) {
		delete [] *outAnsiString;
		*outAnsiString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}
	*nUseByte = nNeedByte;
	
	return 0;
}

INT AnsiToUnicode(CHAR * inAnsiString, TCHAR ** outUniString, INT * nUseByte)
{
	INT		nNeedByte, nLen;

	if ( NULL == inAnsiString ) {
		*outUniString = new TCHAR[5];
		*outUniString[0] = '\0';
		*nUseByte = 0;
		return 0;
	}
	
	nLen = strlen(inAnsiString);
	if(nLen <= 0 ) {
		*outUniString = NULL;
		*nUseByte = 0;
		return -10;
	}

	nNeedByte = MultiByteToWideChar(
									CP_ACP				, 
									0					,
									inAnsiString		, 
									-1					,
									NULL				, 
									NULL );
	if ( 0 == nNeedByte ) {
		*outUniString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*outUniString = new TCHAR[nNeedByte];
	nNeedByte = MultiByteToWideChar(
									CP_ACP				, 
									0					,
									inAnsiString		, 
									-1					,
									*outUniString		, 
									nNeedByte );
	if ( 0 == nNeedByte ) {
		delete [] *outUniString;
		*outUniString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*nUseByte = nNeedByte*2;
	return 0;
}

INT Utf8ToUnicode(CHAR * inUtf8String, TCHAR ** outUniString, INT * nUseByte)
{
	INT		nNeedByte, nLen;

	if ( NULL == inUtf8String ) {
		*outUniString = new TCHAR[5];
		*outUniString[0] = '\0';
		*nUseByte = 0;
		return 0;
	}
	
	nLen = strlen(inUtf8String);
	if(nLen <= 0 ) {
		*outUniString = NULL;
		*nUseByte = 0;
		return -10;
	}

	nNeedByte = MultiByteToWideChar(
									CP_UTF8				, 
									0					,
									inUtf8String		, 
									-1					,
									NULL				, 
									NULL );
	if ( 0 == nNeedByte ) {
		*outUniString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*outUniString = new TCHAR[nNeedByte];
	nNeedByte = MultiByteToWideChar(
									CP_UTF8				, 
									0					,
									inUtf8String		, 
									-1					,
									*outUniString		, 
									nNeedByte );
	if ( 0 == nNeedByte ) {
		delete [] *outUniString;
		*outUniString = NULL;
		*nUseByte = 0;
		return GetLastError();
	}

	*nUseByte = nNeedByte*2;
	return 0;
}

INT Utf8ToAnsiString(CHAR * inUtf8String, CHAR ** outAnsiString, INT * nUseByte)
{
	INT		ids, nByte;
	TCHAR * pTemp = NULL;

	if ( NULL == inUtf8String ) {
		*outAnsiString = new CHAR[5];
		*outAnsiString[0] = '\0';
		*nUseByte = 0;
		return 0;
	}

	ids = Utf8ToUnicode(inUtf8String, &pTemp, &nByte);
	if ( ids ) {
		*outAnsiString = NULL;
		*nUseByte = NULL;
		return ids;
	}

	ids = UniCodeToAnsiString(pTemp, outAnsiString, nUseByte);
	delete [] pTemp;
	if ( ids ) {
		*outAnsiString = NULL;
		*nUseByte = 0;
		return ids;
	}

	return 0;
}

INT AnsiStringToUtf8(CHAR * inAnsiString, CHAR ** outUtf8String, INT * nUseByte)
{
	INT		ids, nByte;
	TCHAR * pTemp = NULL;

	if ( NULL == inAnsiString ) {
		*outUtf8String = new CHAR[5];
		*outUtf8String[0] = '\0';
		*nUseByte = 0;
		return 0;
	}

	ids = AnsiToUnicode(inAnsiString, &pTemp, &nByte);
	if ( ids ) {
		*outUtf8String = NULL;
		*nUseByte = 0;
		return ids;
	}

	ids = UniCodeToUtf8String(pTemp, outUtf8String, nUseByte);
	delete [] pTemp;
	if ( ids ) {
		*outUtf8String = NULL;
		*nUseByte = 0;
		return ids;
	}

	return 0;
}

void FDK_DELETE(void ** pointer)
{
	if ( NULL != *pointer ) {
		delete [] *pointer;
		*pointer = NULL;
	}
}
