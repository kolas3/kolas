// FEE_STATISTICS.cpp : Defines the initialization routines for the DLL.
// Make by pwr

#include "stdafx.h"
#include <afxdllx.h>
#include "FeeStatisticsDlg.h"
#include "StatisticsTermDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE FEE_STATISTICSDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FEE_STATISTICS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(FEE_STATISTICSDLL, hInstance))
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

		new CDynLinkLibrary(FEE_STATISTICSDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FEE_STATISTICS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(FEE_STATISTICSDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias )
{
	CFeeStatisticsDlg* pWindow = NULL;
	pWindow = new CFeeStatisticsDlg(NULL);
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
	if (!pWindow) return NULL;

	CFeeStatisticsDlg *pTmpWindow = (CFeeStatisticsDlg*)pWindow;

	if( strSM == _T("통계") )
	{	
		CStatisticsTermDlg *pDlg = NULL;
		pDlg = new CStatisticsTermDlg ( pTmpWindow );
		pDlg->pMain = pTmpWindow->pMain;
		if( bReadOnly )	pDlg->SM_STYLE = 'R';
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
	}
	else if ( strSM == _T("파일반출") )
	{
		pTmpWindow->MakeGridToExcelFile();
	}

	return NULL;
}	

__declspec(dllexport) void ResizeForm(CRect rect, CWnd *pWindow)
{
	if(pWindow)
	{
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) void DeleteForm(CWnd *pWindow)
{
	CFeeStatisticsDlg *pTmpWindow = (CFeeStatisticsDlg*)pWindow;
	if(pTmpWindow)
	{
		delete pTmpWindow;
	}
}