// MNG_SET_DUPLICATEKEY_MGR.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MNG_SET_DUPLICATEKEY_MGR_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MNG_SET_DUPLICATEKEY_MGRDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_SET_DUPLICATEKEY_MGR.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_SET_DUPLICATEKEY_MGRDLL, hInstance))
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

		new CDynLinkLibrary(MNG_SET_DUPLICATEKEY_MGRDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_SET_DUPLICATEKEY_MGR.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_SET_DUPLICATEKEY_MGRDLL);
	}
	return 1;   // ok
}

__declspec( dllexport ) CWnd* CreateForm( CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ) 
{
	CMNG_SET_DUPLICATEKEY_MGR_01 *pWindow = new CMNG_SET_DUPLICATEKEY_MGR_01( NULL );
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create( pParent );
	pWindow->ShowWindow( SW_SHOW );
	pWindow->UpdateWindow();
	return pWindow;	
}

__declspec( dllexport ) CESL_Mgr* CreateModelessDlg( CString strSM, CString strButtonAlias, BOOL 

bReadOnly, CWnd *pWindow ) 
{	
	if( !pWindow )
		return NULL;		
	
	CMNG_SET_DUPLICATEKEY_MGR_01 * m_pWindow = ( CMNG_SET_DUPLICATEKEY_MGR_01 * )pWindow;
	
	if( _T("변경적용") == strSM )
		m_pWindow->UpdateManageValue();

	return NULL;
}

__declspec( dllexport ) VOID ResizeForm( CRect rect, CWnd *pWindow ) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec( dllexport ) VOID DeleteForm( CWnd *pWindow )
{
	if( pWindow )
		delete pWindow;
}