// SE_LOC_STATISTICS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "LOC_STATISTICS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_LOC_STATISTICSDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_LOC_STATISTICS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_LOC_STATISTICSDLL, hInstance))
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

		new CDynLinkLibrary(SE_LOC_STATISTICSDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_LOC_STATISTICS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_LOC_STATISTICSDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CLOC_STATISTICS* m_pWindow = new CLOC_STATISTICS(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	
	//=====================================================
	//2009.01.29 UPDATE BY PJW : MenuAlias가 id값으로 넘어올경우도 처리해준다.
// 	if(_T("통계_작업중자료_연속") == strMenuAlias) m_pWindow->m_nTYPE = 1; 
// 	else if(_T("통계_소장중자료_연속") == strMenuAlias) m_pWindow->m_nTYPE = 2; 
// 	else if(_T("통계_실시간열람자료_연속") == strMenuAlias) m_pWindow->m_nTYPE = 3;
	if(_T("통계_작업중자료_연속") == strMenuAlias || _T("2640") == strMenuAlias) m_pWindow->m_nTYPE = 1; 
	else if(_T("통계_소장중자료_연속") == strMenuAlias || _T("2641") == strMenuAlias) m_pWindow->m_nTYPE = 2; 
	else if(_T("통계_실시간열람자료_연속") == strMenuAlias || _T("2642") == strMenuAlias) m_pWindow->m_nTYPE = 3; 	
	//=====================================================

	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}



__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CLOC_STATISTICS * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CLOC_STATISTICS*)pWindow;

	if (strButtonAlias == _T("통계"))
	{
		m_pWindow->ShowTmpDlg();
	}
	if (strButtonAlias == _T("출력"))
	{
		m_pWindow->PrintData();
	}

	return NULL;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if (pWindow) 
	{
		pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm(CWnd *pWindow) 
{
	if (pWindow) 
	{
		delete pWindow;
	}

}

