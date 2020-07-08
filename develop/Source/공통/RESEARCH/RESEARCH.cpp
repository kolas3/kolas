// RESEARCH.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "RESEARCH_DLG_MAIN.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE RESEARCHDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("RESEARCH.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(RESEARCHDLL, hInstance))
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

		new CDynLinkLibrary(RESEARCHDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("RESEARCH.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(RESEARCHDLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{
	CRESEARCH_DLG_MAIN* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CRESEARCH_DLG_MAIN(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}


__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;

	CRESEARCH_DLG_MAIN* m_pWindow = (CRESEARCH_DLG_MAIN*)pWindow;
/*	
	if (strSM == _T("배가관리_검색"))
	{
		CBO_LOC_1110 *pDlg = NULL;
		pDlg = new CBO_LOC_1110(m_pWindow,gstrMenuAlias);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
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
*/
	if( strSM == _T("검색"))
	{
		m_pWindow->Search () ;
	}
	else if (strSM == _T("설문작성"))
	{
		m_pWindow->ShowResearchMakeDlg () ;
	}
	else if (strSM == _T("수정"))
	{
		m_pWindow->ModifyResearch();
	}
	else if (strSM == _T("삭제")) 
	{
		m_pWindow->Delete () ;
	}
	else if (strSM ==_T("설문발송"))
	{

	}
	else if (strSM ==_T("결과보기"))
	{
		m_pWindow->ShowResearchResult() ;
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
	{
		pWindow->DestroyWindow();
		delete pWindow;
	}
}
