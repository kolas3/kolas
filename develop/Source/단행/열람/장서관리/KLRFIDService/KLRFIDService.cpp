// KLRFIDService.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KLRFIDService.h"
#include "KLRFIDServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKLRFIDServiceApp

BEGIN_MESSAGE_MAP(CKLRFIDServiceApp, CWinApp)
	//{{AFX_MSG_MAP(CKLRFIDServiceApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKLRFIDServiceApp construction

CKLRFIDServiceApp::CKLRFIDServiceApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKLRFIDServiceApp object

CKLRFIDServiceApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKLRFIDServiceApp initialization

BOOL CKLRFIDServiceApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CKLRFIDServiceDlg dlg;
	m_pMainWnd = &dlg;

	EnumWindows(ProgramEnumProc, (LPARAM)dlg.m_hWnd);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CALLBACK ProgramEnumProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szBuf[MAX_PATH];
	::GetWindowText(hWnd, szBuf, MAX_PATH);
	
	CString strBuf;
	strBuf.Format(_T("%s"), szBuf);

	if(		
		_T("KLRFIDService") == strBuf
		)
	{
		int nCnt = 0;
		HWND _hWnd = (HWND)lParam;

		if(hWnd != _hWnd)
		{
			::PostMessage(hWnd, WM_QUIT, NULL, NULL);
		}
	}

	return TRUE;
}