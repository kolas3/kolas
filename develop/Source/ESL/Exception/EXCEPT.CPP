/////////////////////////////////////////////////////////////////////////////
// execption code definition.
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*======================================================================
D E F I N E D
======================================================================*/
#define EXCEPTION_MESSAGE 1
#define EXCEPTION_LOGGING 2

#define EXCEPTION_MODE ( EXCEPTION_MESSAGE | EXCEPTION_LOGGING )
//#define EXCEPTION_MODE       ( EXCEPTION_MESSAGE )
//#define EXCEPTION_MODE       ( EXCEPTION_LOGGING )

#define EXCEPTOION_MAX 16

/*======================================================================
E X C E P T I O N I N F O
======================================================================*/
struct EXCEPTION_INFO {
	UINT nCode;
	_TCHAR szMessage[300];
};

static EXCEPTION_INFO ExceptionInfo[EXCEPTOION_MAX] = {
{ EXCEPTION_ACCESS_VIOLATION, _T("EXCEPTION_ACCESS_VIOLATION : The thread tried to read from or write to a virtual address for which it does not have the appropriate access.") },
{ EXCEPTION_BREAKPOINT, _T("EXCEPTION_BREAKPOINT : A breakpoint was encountered.") },
{ EXCEPTION_DATATYPE_MISALIGNMENT, _T("EXCEPTION_DATATYPE_MISALIGNMENT : The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.") },
{ EXCEPTION_SINGLE_STEP, _T("EXCEPTION_SINGLE_STEP : A trace trap or other single-instruction mechanism signaled that one instruction has been executed.") },
{ EXCEPTION_ARRAY_BOUNDS_EXCEEDED, _T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED : The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.") },
	{ EXCEPTION_FLT_DENORMAL_OPERAND, _T("EXCEPTION_FLT_DENORMAL_OPERAND : One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.") },
	{ EXCEPTION_FLT_DIVIDE_BY_ZERO, _T("EXCEPTION_FLT_DIVIDE_BY_ZERO : The thread tried to divide a floating-point value by a floating-point divisor of zero.") },
	{ EXCEPTION_FLT_INEXACT_RESULT, _T("EXCEPTION_FLT_INEXACT_RESULT : The result of a floating-point operation cannot be represented exactly as a decimal fraction.") },
	{ EXCEPTION_FLT_INVALID_OPERATION, _T("EXCEPTION_FLT_INVALID_OPERATION : This exception represents any floating-point exception not included in this list.") },
	{ EXCEPTION_FLT_OVERFLOW, _T("EXCEPTION_FLT_OVERFLOW : The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.") },
	{ EXCEPTION_FLT_STACK_CHECK, _T("EXCEPTION_FLT_STACK_CHECK : The stack overflowed or underflowed as the result of a floating-point operation.") },
	{ EXCEPTION_FLT_UNDERFLOW, _T("EXCEPTION_FLT_UNDERFLOW : The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.") },
	{ EXCEPTION_INT_DIVIDE_BY_ZERO, _T("EXCEPTION_INT_DIVIDE_BY_ZERO : The thread tried to divide an integer value by an integer divisor of zero.") },
	{ EXCEPTION_INT_OVERFLOW, _T("EXCEPTION_INT_OVERFLOW : The result of an integer operation caused a carry out of the most significant bit of the result.") },
	{ EXCEPTION_PRIV_INSTRUCTION, _T("EXCEPTION_PRIV_INSTRUCTION : The thread tried to execute an instruction whose operation is not allowed in the current machine mode.") },
	{ STATUS_NONCONTINUABLE_EXCEPTION, _T("STATUS_NONCONTINUABLE_EXCEPTION : The thread tried to continue execution after a noncontinuable exception occurred.") }
};

/*======================================================================
L O G G I N G F I L E
======================================================================*/
VOID LogException( _TCHAR * Message )
{
	FILE * fp;
	long ltime;
	tm * t;

	time(&ltime);
	t = localtime ( &ltime );

	if ( NULL != ( fp = _tfopen ( _T("EXCEPT.TXT"), _T("ab") ) ) ) 
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
		
		_ftprintf( fp, _T("[EXCEPTION] %04d/%02d/%02d %02d:%02d:%02d %s\r\n"), t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, Message );
		fclose( fp );
	}
}

/*======================================================================
S E A R C H E X C E P T I O N
======================================================================*/
__declspec(dllexport) VOID SearchException( UINT nCode, _TCHAR * szFileName, INT Line )
{
	INT idx;
	_TCHAR szMessage[500];

	for ( idx = 0; idx < EXCEPTOION_MAX; idx ++ ) 
	{
		if ( ExceptionInfo[idx].nCode == nCode ) 
		{
			_stprintf( szMessage, _T("[FILE_NAME: %s] [LINE: %d] >> %s"), szFileName, Line, ExceptionInfo[idx].szMessage );
			if ( 0x0002 & EXCEPTION_MODE ) 
			{
				LogException( szMessage );
			}
			if ( 0x0001 & EXCEPTION_MODE ) 
			{
				::MessageBox( NULL, szMessage, _T("EXCEPTION BALSAENG"), MB_OK );
			}
			break;
		}
	}
}
