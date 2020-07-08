// KOLAS2UP_SETUP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "SetupMainSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLAS2UP_SETUPApp

BEGIN_MESSAGE_MAP(CKOLAS2UP_SETUPApp, CWinApp)
	//{{AFX_MSG_MAP(CKOLAS2UP_SETUPApp)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLAS2UP_SETUPApp construction

CKOLAS2UP_SETUPApp::CKOLAS2UP_SETUPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKOLAS2UP_SETUPApp object

CKOLAS2UP_SETUPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKOLAS2UP_SETUPApp initialization

BOOL CKOLAS2UP_SETUPApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should _tremove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	/*CKOLAS2UP_SETUPDlg dlg;
	m_pMainWnd = &dlg;
	INT nResponse = dlg.DoModal();
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
	*/
	// KOLAS II Light.msi파일이 있으면 설치전 설정모드이다.
	FILE *fp = _tfopen(_T("KOLAS III.msi"), _T("rb"));
	if (fp == NULL)
	{
		CSetupMainSheet dlg(IDS_SETUP);
		m_pMainWnd = &dlg;
		dlg.m_psh.dwFlags &= ~PSH_HASHELP;
		dlg.DoModal();
	}
	else
	{
		CSetupMainSheet dlg(IDS_PRE_SETUP);
		m_pMainWnd = &dlg;
		dlg.m_psh.dwFlags &= ~PSH_HASHELP;
		dlg.DoModal();

		fclose(fp);
	}


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
