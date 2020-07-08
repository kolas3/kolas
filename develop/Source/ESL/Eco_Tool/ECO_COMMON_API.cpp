
#include "stdafx.h"
#include "ECO_COMMON_API.h"
#include "ECO_FileIO.h"

//======================================================================
//  : 수행 속도 체크.
//  : 시작 지점에서 한 번 호출하고 끝나는 지점에서 호출하면 수행 시간이 나온다.
//
// [RETURN_VALUE]
//
// [HISTORY]
//  1999/06/ 채정호 추가
//
//======================================================================
/*
VOID clock_check( )
{

	FILE *fp;

static clock_t start = 0;
clock_t end;
double duration;

if ( start == 0 ) {
start = clock();
} else {
end = clock();
duration = (double)(end - start) / CLOCKS_PER_SEC;
if ( NULL != ( fp = _tfopen ( _T("..\\tunning\\filelog\\workersearch.txt") , _T("a") ) ) ) {
_ftprintf( fp , _T("Process Time : %f\n"), duration );
		 fclose ( fp );
	 }
start = 0;
}

}
*/
VOID clock_check( _TCHAR *msg )
{

	FILE *fp;

	static clock_t start = 0;
	clock_t end;
	double duration;

	if ( start == 0 || msg == NULL ) 
	{
		start = clock();
	} 
	else 
	{
		end = clock();
		duration = (double)(end - start) / CLOCKS_PER_SEC;
		if ( NULL != ( fp = _tfopen ( _T("..\\tunning\\filelog\\time_check.txt") , _T("ab") ) ) ) 
		{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf( fp , _T("%s : %f\r\n"), msg, duration );
			fclose ( fp );
		}
/*
		_TCHAR	tmp[10000];
		_stprintf( tmp, _T("%s : %f\n"), msg, duration );
		AfxMessageBox( tmp );
*/
	start = 0;
}

}

////////////////////////////////////////////////////////////////////////
// Data를 Debug.txt파일로 출력한다.
////////////////////////////////////////////////////////////////////////
VOID debug( _TCHAR *str )
{

	FILE *fp;
	_TCHAR szFileName[524];

	long ltime;
	tm *t;

	time(&ltime);
	t = localtime ( &ltime );

	_stprintf ( szFileName , _T("..\\LOG\\(%02d%02d)debug.txt") , t->tm_mon+1, t->tm_mday );

	fp = _tfopen ( szFileName , _T("a+b") );
	if ( fp == NULL )
	{
		_tprintf( _T("\n _tfopen error \n ") );
		return ;
	}

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
	
	_ftprintf ( fp, _T("\r\n %s \r\n"), str );

	fclose(fp);

}

VOID FilterSpecialChar(_TCHAR *data, _TCHAR *keyword)
{
	_TUCHAR ch , ch2;

for (; ch = *data; data++)
{
		if( ch & 0x80 )
		{
			ch2 = *(data+1);
			if ( ch >= 0xa1 && ch <= 0xa3 && ch2 >= 0xa1)
			{
				if ( ch != 0xa3 || !( ch2 >= 0xb0 && ch2 <= 0xb9 ) ) // JEGEO DAESANG KODEUGABS
					;
				else	// 2Byte SUSJA => 1Byte SUSJARO
					*keyword++ = '0' + ( ch2 - 0xb0 );
				++data;
			}
else
			{
		 *keyword++ = ch;
			 *keyword++ = *(++data);
			}
		}else if ((ch >= 'A' && ch <= 'Z') || // DAEMUNJA YEONGMUN
			 (ch >= 'a' && ch <= 'z') || // SOMUNJA YEONGMUN
			 (ch >= '0' && ch <= '9') ) // SUSJA
		{
		*keyword++ = ch;
		}

}
*keyword = '\0';
}

////////////////////////////////////////////////////////////////////////
//
// DataLeftRightTrim
//
// [PURPOSE]
// $1. 데이타의 좌우의 특정값(Space)를 잘라낸다.
//
// [MINI_SPEC]
// $1. Left Space를 제거한다.
// $2. Right Space를 제거한다.
//
////////////////////////////////////////////////////////////////////////
VOID DataLeftRightTrim( _TCHAR * Data, _TCHAR Var )
{
	CECO_FileIO FileIO;
	_TCHAR StrOut[526];

	// $1. Left Space를 제거한다.
	FileIO.str_ltrim( Data, StrOut, Var );

	// $2. Right Space를 제거한다.
	FileIO.str_rtrim( StrOut, Data, Var );

	return;
}

