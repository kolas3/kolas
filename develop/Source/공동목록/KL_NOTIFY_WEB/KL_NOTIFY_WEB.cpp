// KL_NOTIFY_WEB.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "KlNotifyWebDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE KL_NOTIFY_WEBDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("KL_NOTIFY_WEB.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(KL_NOTIFY_WEBDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(KL_NOTIFY_WEBDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("KL_NOTIFY_WEB.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(KL_NOTIFY_WEBDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE) 
{
	CKlNotifyWebDlg* pWindow = new CKlNotifyWebDlg(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if( pWindow )
		delete pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{
	if (pWindow == NULL) return NULL;
	
	CKlNotifyWebDlg* m_pWindow = (CKlNotifyWebDlg*)pWindow;
	if (_T("SM_뒤로") == strSM) {
		m_pWindow->GoBack();
	} else if (_T("SM_앞으로") == strSM) {
		m_pWindow->GoForward();
	} else if (_T("SM_중지") == strSM) {
		m_pWindow->Stop();
	} else if (_T("SM_새로고침") == strSM) {
		m_pWindow->Refresh();
	} else if (_T("SM_홈") == strSM) {
		m_pWindow->GoHome();
	} 

	return NULL;
}