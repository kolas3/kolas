#include "stdAfx.h"
#include "win32api.h"

INT SemaphoreOpen(
HANDLE *hSemaphore,
_TCHAR *Name
)
{

*hSemaphore = CreateSemaphore( NULL, 1, 1, Name );
if (*hSemaphore == NULL) {
if (GetLastError() == ERROR_ALREADY_EXISTS) {
*hSemaphore = OpenSemaphore( SEMAPHORE_ALL_ACCESS, 1, Name );
if (*hSemaphore == NULL) { _tprintf( _T("\nOpen Semaphore. ids: %d"), GetLastError() ); return -1; }
} else {
_tprintf( _T("\nSemaphore Create Error. ids: %d"), GetLastError() ); return -2;
}
}

return 0;
}


INT EventOpen(
HANDLE *hEvent,
_TCHAR *Name
)
{

*hEvent = CreateEvent( NULL, FALSE, FALSE, Name );
if (*hEvent == NULL) {
if (GetLastError() == ERROR_ALREADY_EXISTS) {
*hEvent = OpenEvent( EVENT_ALL_ACCESS, TRUE, Name );
if (*hEvent == NULL) { _tprintf( _T("\nOpen Error. ids: %d"), GetLastError() ); return -1; }
} else {
_tprintf( _T("\nEvent Create Error. ids: %d"), GetLastError() ); return -2;
}
}

return 0;
}

INT WaitObject (
INT ObjectCnt ,
HANDLE *ObjectHandle,
LONG TimeOut
)
{
DWORD dwWaitResult;


if ( ObjectCnt <= 0 ) return -100;

if ( ObjectCnt == 1 ) {
dwWaitResult = WaitForSingleObject ( *ObjectHandle, TimeOut );
} else {
dwWaitResult = WaitForMultipleObjects( ObjectCnt, ObjectHandle, FALSE, TimeOut );
}

if ( WAIT_TIMEOUT == dwWaitResult ) return -1;

return (dwWaitResult - WAIT_OBJECT_0);
}
