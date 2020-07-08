// HtmlEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HtmlEdit.h"
#include "HtmlEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditApp

BEGIN_MESSAGE_MAP(CHtmlEditApp, CWinApp)
	//{{AFX_MSG_MAP(CHtmlEditApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditApp construction

CHtmlEditApp::CHtmlEditApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHtmlEditApp object

CHtmlEditApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditApp initialization

BOOL CHtmlEditApp::InitInstance()
{
	#if _MSC_VER > 1200 // Visual Studio 7.0 or higher
		#error This project is a special version for Visual Studio 6.0 ONLY! Please download the Visual Studio 7 version from CodeProject.com!!
	#endif

	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CHtmlEditDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

