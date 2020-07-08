// BO_CAT_SPECIAL_SORT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MngSpecialSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_CAT_SPECIAL_SORTDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_SPECIAL_SORT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_SPECIAL_SORTDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_SPECIAL_SORTDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_SPECIAL_SORT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_SPECIAL_SORTDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE) 
{
	CMngSpecialSort* pWindow = new CMngSpecialSort(NULL);
	
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
	if( !pWindow )
		return NULL;		
	
	CMngSpecialSort* m_pWindow = (CMngSpecialSort*)pWindow;
	
	if( _T("변경적용") == strSM )
	{
		m_pWindow->SaveToFile();
	}

//	{	
//		CMNG_CAT_MANAGER_02 *pDlg = NULL;
//		pDlg = new CMNG_CAT_MANAGER_02( m_pWindow );
//		pDlg->pMain = m_pWindow->pMain;
//		
//		if( bReadOnly )
//			pDlg ->SM_STYLE = 'R';
//		
//		pDlg->m_pInfo = m_pWindow->m_pInfo;
//		
//		if( pDlg->Create( m_pWindow ) )
//		{
//			pDlg->ShowWindow( SW_SHOW );
//			pDlg->UpdateWindow();
//			pDlg->CenterWindow();
//			return pDlg;
//		}
//		else
//		{
//			delete pDlg;
//			return NULL;
//		}
//	}


	return NULL;
}