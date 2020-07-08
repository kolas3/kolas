// SoosikEditor.cpp: implementation of the CSoosikEDitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoosikEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "TocEditorDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoosikEDitor::CSoosikEDitor()
{
	m_hParentWnd= NULL ;
}

CSoosikEDitor::~CSoosikEDitor()
{
	RemoveSharedMemoryPool() ;
}

INT CSoosikEDitor::ExecuteSoosikEditor(HWND hParentWnd)
{
	m_hParentWnd= hParentWnd ;

	HWND hWnd= NULL ;
	hWnd= ::FindWindow(NULL, TITLE_ECOMATH) ;
	if ( hWnd!=NULL ) {
		CString sMsg ;
		sMsg= TITLE_ECOMATH ;
		sMsg+= _T(" 가 실행중입니다!") ;
		AfxMessageBox(sMsg, MB_ICONERROR) ;
		return -1 ;
	}

	if ( CreateSharedMemoryPool() ) {
		AfxMessageBox(_T("수식 편집기를 띄울 수 없습니다."), MB_ICONERROR) ;
		return -2 ;
	}

	return 0 ;
}

INT CSoosikEDitor::CreateSharedMemoryPool()
{

	if(ExecuteEcoMath() == true) {
		if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
			AfxMessageBox(_T("Wait Fail"));
			return -1;
		}

#include "TocEDitorDef.h"

		if(SendMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_CONNECT, 0) == -1) {
			AfxMessageBox(_T("Connect Error"));
			return -2;
		}

		if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
			AfxMessageBox(_T("Wait Fail"));
			return -3;
		}

		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_INIT, 0) == -1) {
			AfxMessageBox(_T("Connect Error"));
			return -4;
		}

		m_cSharedMemory.Open(_T("TestPool"), 20);

		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_INITOK, 0) == -1) {
			AfxMessageBox(_T("INIT Fail"));
			return -5;
		}
//		CString sTmp ;
//		sTmp.Format( _T("%d") , m_hParentWnd ) ;
//		AfxMessageBox(sTmp) ;
		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_SET_HWND, (LPARAM) m_hParentWnd) == -1) {
			AfxMessageBox(_T("Set hWnd Fail"));
			return -6;
		}
		return 0 ;
	}
	else {
		return -6 ;
	}

}

bool CSoosikEDitor::ExecuteEcoMath()
{

    STARTUPINFO si;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &m_pi, sizeof(m_pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,		// No module name (use command line). 
        PATH_ECOMATH,				// Command line. 
        NULL,						// Process handle not inheritable. 
        NULL,						// Thread handle not inheritable. 
        FALSE,						// Set handle inheritance to FALSE. 
        0,							// No creation flags. 
        NULL,						// Use parent's environment block. 
        NULL,						// Use parent's starting directory. 
        &si,						// Pointer to STARTUPINFO structure.
        &m_pi )						// Pointer to PROCESS_INFORMATION structure.
    ) 
    {
		CString sError ;
		sError= _T("CreateProcess failed.") ;
		sError+= _T("\r\n\r\n다음 파일이 존재하는지 확인하여 주십시오.\r\n") ;
		sError+= PATH_ECOMATH ;
        AfxMessageBox( sError );
    }

	return true;
}

VOID CSoosikEDitor::RemoveSharedMemoryPool()
{
	m_cSharedMemory.Close();

}

CString CSoosikEDitor::ReadFromSoosikEditor()
{
	INT retval= Read() ;
	if ( retval ) {
		if ( retval!=-1 )
			AfxMessageBox( _T("수식편집기로부터 수식태그를 읽어오지 못했습니다!"), MB_ICONERROR) ;
		return _T("");
	}

	return m_sRead ;
}

INT CSoosikEDitor::Read()
{
	TCHAR	sBuf[21];
	INT len;
	INT len2read = 0;

	if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
//		AfxMessageBox(_T("Wait Fail"));
		AfxMessageBox(_T("먼저 수식편집기를 실행 후 명령을 실행해 주십시오."));
		return -1;
	}

	len = SendMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_GETLEN, 0);

	if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_GETMATH, 0) == -1) {
		AfxMessageBox(_T("ECOMATH_GETMATH Error"));
		return -2;
	}

	m_sRead.Empty();

	len2read = 20;

	for(INT loop = 0; loop < (len / 20 + 1); loop++) {
		if(loop == (len / 20)) {
			len2read = len%20;
		}
		wmemset(sBuf, 0, sizeof(sBuf));
		switch (m_cSharedMemory.Read(sBuf, len2read, 10000))
		{
		case CSharedMemory::MEM_SUCCESS:
			m_sRead += sBuf;
			break;
		case CSharedMemory::MEM_ERROR_CLOSED:
			AfxMessageBox(_T("Not opened"));
			return -1 ;
			break;
		case CSharedMemory::MEM_ERROR_TIMEOUT:
			AfxMessageBox(_T("Timeout"));
			return -2 ;
			break;
		case CSharedMemory::MEM_ERROR_OTHERPARTY:
			AfxMessageBox(_T("Other party disconnected"));
			m_cSharedMemory.Close();
			return -1 ;
			break;
		case CSharedMemory::MEM_ERROR_UNKNOWN:
			AfxMessageBox(_T("Unknown"));
			return -1 ;
			break;
		}
	}

	return 0 ;
//	UpdateData(false);
}

typedef struct _FINDWINDOWHANDLESTRUCT {
LPPROCESS_INFORMATION ProcessInfo;
HWND hWndFound; 
} FINDWINDOWHANDLESTRUCT;

INT CSoosikEDitor::SendMessageToProcess(LPPROCESS_INFORMATION lpProcessInformation, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT fwhs;
	fwhs.ProcessInfo = lpProcessInformation;
	fwhs.hWndFound  = NULL;

	EnumWindows ( EnumWindowCallBack, (LPARAM)&fwhs ) ;

	return ::SendMessage ( fwhs.hWndFound, Msg, wParam, lParam );
}

INT CSoosikEDitor::PostMessageToProcess(LPPROCESS_INFORMATION lpProcessInformation, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT fwhs;
	fwhs.ProcessInfo = lpProcessInformation;
	fwhs.hWndFound  = NULL;

	EnumWindows ( EnumWindowCallBack, (LPARAM)&fwhs ) ;

	return ::PostMessage ( fwhs.hWndFound, Msg, wParam, lParam );
}

BOOL CALLBACK CSoosikEDitor::EnumWindowCallBack(HWND hwnd, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT * pfwhs = (FINDWINDOWHANDLESTRUCT * )lParam; 
	DWORD ProcessId; 
	CString Title; 
	GetWindowThreadProcessId ( hwnd, &ProcessId ); 

	// note: In order to make sure we have the MainFrame, verify that the title 
	// has Zero-Length. Under Windows 98, sometimes the Client Window ( which doesn't 
	// have a title ) is enumerated before the MainFrame 

	CWnd::FromHandle( hwnd )->GetWindowText(Title);
	if ( ProcessId  == pfwhs->ProcessInfo->dwProcessId && Title.GetLength() != 0) 
	{ 
		pfwhs->hWndFound = hwnd; 
		return false; 
	} 
	else 
	{ 
		// Keep enumerating 
		return true; 
	} 
}
