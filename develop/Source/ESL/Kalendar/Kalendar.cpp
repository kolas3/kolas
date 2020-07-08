// Kalendar.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Kalendar.h"
#include "KalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKalendarApp

BEGIN_MESSAGE_MAP(CKalendarApp, CWinApp)
	//{{AFX_MSG_MAP(CKalendarApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKalendarApp construction

CKalendarApp::CKalendarApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKalendarApp object

CKalendarApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKalendarApp initialization

BOOL CKalendarApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();	
 	AfxInitRichEdit() ;
	// Standard initialization
	// If you are not using		hese features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CString strDir = _T("");
	CCalFileMgr::Get_FILEDIR(strDir);

	CFileFind filefind;
 	if(TRUE == filefind.FindFile(strDir + _T("DO_NOT_EXCUTE")))
	{
		return FALSE;
	}

	CKalendarDlg dlg;
	m_pMainWnd = &dlg;
	dlg.SetAuth(m_lpCmdLine);
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

UINT ChkDay(UINT uYear, UINT uMonth, UINT uDay, UINT uHour, UINT uMinute)
{
	if(0==uYear||0==uMonth||0==uDay)
		return FALSE;
	if(2100<uYear||12<uMonth||31<uDay||23<uHour||59<uMinute)
		return FALSE;
	return TRUE;
}

UINT g_ViewMode = MEMOVIEW;
