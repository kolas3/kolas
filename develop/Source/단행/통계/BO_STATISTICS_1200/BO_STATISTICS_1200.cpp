// BO_STATISTICS_1200.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_STATISTICS_1201.h"
#include "BO_STATISTICS_1202.h"

static AFX_EXTENSION_MODULE BO_STATISTICS_1200DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_STATISTICS_1200.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_STATISTICS_1200DLL, hInstance))
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

		new CDynLinkLibrary(BO_STATISTICS_1200DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_STATISTICS_1200.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_STATISTICS_1200DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm( CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias )
{
	CBO_STATISTICS_1201* pWindow = NULL;
	pWindow = new CBO_STATISTICS_1201( strMenuAlias, NULL );
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create( pParent );
	pWindow->ShowWindow( SW_SHOW );
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg( CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow )
{
	if (!pWindow) return NULL;	//검색

	CBO_STATISTICS_1201 *pTmpWindow = ( CBO_STATISTICS_1201 * )pWindow;
	
	if( _T("통계") == strSM )
	{
		CBO_STATISTICS_1202 *pDlg = NULL;
		pDlg = new CBO_STATISTICS_1202( _T(""), pTmpWindow );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = ( CESL_Mgr * )pTmpWindow;
		if( bReadOnly )
			pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if( pDlg->Create( pTmpWindow ) )
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
		
		pTmpWindow->ViewStatisticsDlg();
	}

	if( _T("출력") == strSM )
		pTmpWindow->PrintData();
	
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm( CRect rect, CWnd *pWindow )
{
	if( pWindow )
		pWindow->MoveWindow(rect);
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	CBO_STATISTICS_1201 *pTmpWindow = ( CBO_STATISTICS_1201 * )pWindow;
	if( pTmpWindow )
		delete pTmpWindow;
}
