// MNG_KORMARC_EDU.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "WorkDataSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static AFX_EXTENSION_MODULE MNG_KORMARC_EDUDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_KORMARC_EDU.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_KORMARC_EDUDLL, hInstance))
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

		new CDynLinkLibrary(MNG_KORMARC_EDUDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_KORMARC_EDU.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_KORMARC_EDUDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CWorkDataSearchDlg* pWindow = new CWorkDataSearchDlg(NULL);

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) void ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) void DeleteForm(CWnd *pWindow)
{
	if( pWindow )
		delete pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{
	if (pWindow == NULL) return NULL;

	CWorkDataSearchDlg *pDlg = static_cast<CWorkDataSearchDlg*>(pWindow);

	if (strSM == _T("뒤로"))
	{
		pDlg->GoBack();
	}
	else if (strSM == _T("앞으로"))
	{
		pDlg->GoForward();
	}
	else if (strSM == _T("중지"))
	{
		pDlg->Stop();
	}
	else if (strSM == _T("새로고침"))
	{
		pDlg->Refresh();
	}
	else if (strSM == _T("홈"))
	{
		pDlg->GoHome();
	}
	

	return NULL;
}