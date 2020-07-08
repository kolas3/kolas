// SE_MISSING_DATA.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "MISSING_DATA_BASE.h"
#include "MISSING_DATA_SEARCH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE SE_MISSING_DATADLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_MISSING_DATA.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_MISSING_DATADLL, hInstance))
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

		new CDynLinkLibrary(SE_MISSING_DATADLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_MISSING_DATA.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_MISSING_DATADLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{
	CMISSING_DATA_BASE* m_pWindow = new CMISSING_DATA_BASE(NULL,strMenuAlias);   
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow ;
}

//CMISSING_DATA_SEARCH* m_pDlg = NULL;

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	CMISSING_DATA_BASE* m_pWindow = (CMISSING_DATA_BASE*)pWindow;

	if (strButtonAlias == _T("찾기")) {  
		
		m_pWindow->DoSearch();
		
		/*
		m_pDlg = NULL;
		m_pDlg = new CMISSING_DATA_SEARCH(m_pWindow);
		m_pDlg->pMain = m_pWindow->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		m_pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (bReadOnly) m_pDlg->SM_STYLE = 'R';
		m_pWindow->m_pDlg = m_pDlg;
		
		if (m_pDlg->Create(m_pWindow))			
		{
			m_pDlg->ShowWindow(SW_SHOW);
			m_pDlg->UpdateWindow();
			m_pDlg->CenterWindow();
			return m_pDlg;
		}
		else
		{
			delete m_pDlg;
			return NULL;
		}
		*/
	}
	else if(strButtonAlias == _T("독촉")) {  
        m_pWindow->DoProc();
	}
// 	//JOB.2019.0073 : 충돌 License 제거 
// 	else if(strButtonAlias==_T("메일발송")) {	
//         m_pWindow->DoSendMail();
// 	}
	else if(strButtonAlias==_T("출력")) {	 
        m_pWindow->Print_List();
	}
	else if(strButtonAlias==_T("가격합산")) {	 
        m_pWindow->SumPrice();
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
