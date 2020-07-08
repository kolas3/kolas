// MNG_AC_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "ESL_Mgr.h"

#include "ACMARCGetterDlg.h"
#include "MNG_AC_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MNG_AC_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_AC_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_AC_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(MNG_AC_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_AC_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_AC_MANAGERDLL);
	}
	return 1;   // ok
}

//CMNG_AC_MANAGER_01* m_pWindow = NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE) 
{
	CMNG_AC_MANAGER_01* pWindow = new CMNG_AC_MANAGER_01(NULL);

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if( !pWindow )
		return NULL;	
	AfxInitRichEdit();
	CMNG_AC_MANAGER_01* m_pWindow = (CMNG_AC_MANAGER_01*)pWindow;
	//CACMARCGetterDlg *m_pWindow = (CACMARCGetterDlg*)pWindow;

	if( _T("검색") == strButtonAlias )
	{	
		m_pWindow->ACSearch();
	}	
	else if( _T("입력") == strButtonAlias )
	{	
		m_pWindow->ACInput();		
	}
	else if( _T("수정") == strButtonAlias )
	{	
		m_pWindow->ACUpdate();
	}
	else if( _T("삭제") == strButtonAlias )
	{
		m_pWindow->ACDelete();
	}
	else if( _T("전거마크반입") == strButtonAlias )
	{
		m_pWindow->ACMarcImport();
	}
	else if( _T("전거마크반출") == strButtonAlias )
	{
		m_pWindow->ACMarcExport();
	}
	return NULL;
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
