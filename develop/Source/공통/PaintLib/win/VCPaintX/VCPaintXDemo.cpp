// VCPaintXDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VCPaintXDemo.h"
#include "VCPaintXDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCPaintXDemoApp

BEGIN_MESSAGE_MAP(CVCPaintXDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CVCPaintXDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCPaintXDemoApp construction

CVCPaintXDemoApp::CVCPaintXDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVCPaintXDemoApp object

CVCPaintXDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVCPaintXDemoApp initialization

BOOL CVCPaintXDemoApp::InitInstance()
{
	AfxEnableControlContainer();
  CoInitialize(NULL); // Initialize COM 

	CVCPaintXDemoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	// Since the dialog has been closed, return false so that we exit the
	//  application, rather than start the application's message pump.
	return false;
}
