// MNG_KOMARC_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MNG_KOMARC_MANAGER_01.h"
#include "MNG_KOMARC_MANAGER_02.h"
#include "MNG_KOMARC_MANAGER_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MNG_KOMARC_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_KOMARC_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_KOMARC_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(MNG_KOMARC_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_KOMARC_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_KOMARC_MANAGERDLL);
	}
	return 1;   // ok
}

//CMNG_KOMARC_MANAGER_01* m_pWindow = NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE) 
{
	CMNG_KOMARC_MANAGER_01* pWindow = new CMNG_KOMARC_MANAGER_01(NULL);
	
	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

/*
__declspec( dllexport ) VOID CreateForm( CWnd *pParent, CESL_Information* pInfo ) 
{
	m_pWindow = NULL;
	m_pWindow = new CMNG_KOMARC_MANAGER_01( NULL );
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create( pParent );
	m_pWindow->ShowWindow( SW_SHOW );
	m_pWindow->UpdateWindow();
}
*/

//__declspec( dllexport ) CESL_Mgr* CreateModelessDlg( CString strSM, CString strButtonAlias, BOOL bReadOnly )
__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if( !pWindow )
		return NULL;		
	
	CMNG_KOMARC_MANAGER_01* m_pWindow = (CMNG_KOMARC_MANAGER_01*)pWindow;
	
	if( _T("MNG_KOMARC_MANAGER_02") == strSM )
	{	
		CMNG_KOMARC_MANAGER_02 *pDlg = NULL;
		pDlg = new CMNG_KOMARC_MANAGER_02( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
		if( pDlg->Create( m_pWindow ) )
		{
			pDlg->ShowWindow( SW_SHOW );
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}	
	else if( _T("MNG_KOMARC_MANAGER_03") == strSM )
	{	
		CMNG_KOMARC_MANAGER_03 *pDlg = NULL;
		pDlg = new CMNG_KOMARC_MANAGER_03( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
		if( _T("입력") == strButtonAlias )
			pDlg->m_mode = 1;
		else if( _T("수정") == strButtonAlias )
			pDlg->m_mode = 2;
		
		if( pDlg->Create( m_pWindow ) )
		{
			pDlg->ShowWindow( SW_SHOW );
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if( _T("MNG_KOMARC_MANAGER_delete") == strSM )
	{
		m_pWindow->DeleteData();
		return NULL;
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