// ExeExtractor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ExeExtractor.h"
#include "ExeExtractorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExeExtractorApp

BEGIN_MESSAGE_MAP(CExeExtractorApp, CWinApp)
	//{{AFX_MSG_MAP(CExeExtractorApp)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExeExtractorApp construction

CExeExtractorApp::CExeExtractorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExeExtractorApp object

CExeExtractorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CExeExtractorApp initialization

BOOL CExeExtractorApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should _tremove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CExeExtractorDlg dlg;
	m_pMainWnd = &dlg;
	// No need to retrieve dialog return value (IDOK/IDCANCEL), just exit application
	// after dialog is closed
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
