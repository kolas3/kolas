// SIReportSample.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgSIReportSample.h"


static AFX_EXTENSION_MODULE SIReportSampleDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SIREPORTSAMPLE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SIReportSampleDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  _tremove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(SIReportSampleDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SIREPORTSAMPLE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SIReportSampleDLL);
	}
	return 1;   // ok
}


CDlgSIReportSample* m_pWindow=NULL;

__declspec(dllexport) VOID CreateForm(CWnd *pParent, CESL_Information* pInfo) 
{
	m_pWindow = NULL;
	m_pWindow = new CDlgSIReportSample(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, BOOL bReadOnly)
{
	if (!m_pWindow) return NULL;

	if (strSM == _T("SM_SIREPORT_SAMPLE_PRINT"))
	{
		m_pWindow->OnbPRINT();
		return NULL;
	}
	else if (strSM == _T("SM_SIREPORT_SAMPLE_SEARCH"))
	{
		m_pWindow->OnbSEARCH();
		return NULL;
	}
	
	return NULL;
}

__declspec(dllexport) VOID ResizeForm(CRect rect) 
{
	if (m_pWindow) 
	{
		m_pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm() 
{
	if (m_pWindow) 
	{
		delete m_pWindow;
	}
}
