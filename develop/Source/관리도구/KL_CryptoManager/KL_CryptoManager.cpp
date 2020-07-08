// KL_CryptoManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoManagerApp

BEGIN_MESSAGE_MAP(CKL_CryptoManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CKL_CryptoManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoManagerApp construction

CKL_CryptoManagerApp::CKL_CryptoManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKL_CryptoManagerApp object

CKL_CryptoManagerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoManagerApp initialization

BOOL CKL_CryptoManagerApp::InitInstance()
{
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

	CKL_CryptoManagerDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strMode = m_lpCmdLine;
	dlg.m_strMode.TrimLeft();
	dlg.m_strMode.TrimRight();
	dlg.m_strMode.MakeUpper();

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
