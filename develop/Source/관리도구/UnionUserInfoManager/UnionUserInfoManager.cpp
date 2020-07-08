// UnionUserInfoManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UnionUserInfoManager.h"
#include "UnionUserInfoManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoManagerApp

BEGIN_MESSAGE_MAP(CUnionUserInfoManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CUnionUserInfoManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoManagerApp construction

CUnionUserInfoManagerApp::CUnionUserInfoManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUnionUserInfoManagerApp object

CUnionUserInfoManagerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoManagerApp initialization

BOOL CUnionUserInfoManagerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	InitCommonControls(); 
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CUnionUserInfoManagerDlg dlg;
	m_pMainWnd = &dlg;
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


INT SetIMEStatus(CWnd* pWnd, CString strMode, UINT uControlID)
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	hIME = ::ImmGetContext(pWnd->GetDlgItem(uControlID)->m_hWnd);
	if(NULL != hIME)
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		if(_T("한글") == strMode)
		{
			dwConversion=IME_CMODE_HANGUL;
		}
		else if(_T("영문") == strMode)
		{
			dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
			dwConversion |= IME_CMODE_ALPHANUMERIC;
		}
		else return -1;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );		
		ImmReleaseContext(pWnd->GetDlgItem(uControlID)->m_hWnd,hIME); 
	}
	else return -2;

	return 0;
}
