// Light_uc_notify_local.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Light_UC_NOTIFY_01.h"
#include "Light_UC_NOTIFY_02.h"
#include "Light_UC_NOTIFY_03.h"
#include "Light_UC_NOTIFY_DETAIL.h"



static AFX_EXTENSION_MODULE Light_uc_notify_localDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("LIGHT_UC_NOTIFY_LOCAL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(Light_uc_notify_localDLL, hInstance))
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

		new CDynLinkLibrary(Light_uc_notify_localDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LIGHT_UC_NOTIFY_LOCAL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(Light_uc_notify_localDLL);
	}
	return 1;   // ok
}




//CUC_DOWN_TAG_01* m_pWindow=NULL;
 __declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ) 

{
//	m_pWindow = NULL;
//	m_pWindow = new CUC_DOWN_TAG_01(NULL);
	CLight_UC_NOTIFY_01* m_pWindow = new CLight_UC_NOTIFY_01(NULL);   
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow ;
}



__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
//__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly)
{
	if (!pWindow) return NULL;
	CLight_UC_NOTIFY_01* m_pWindow = (CLight_UC_NOTIFY_01*)pWindow;

	/*
	if (strSM == _T("보내기"))
	{       
       CLight_UC_NOTIFY_02 dlg(m_pWindow) ;
	   dlg.DoModal () ;
	}   
	else 
	*/	
	if (strSM == _T("받 기"))
	{  
		m_pWindow->SubRECEIPTNO();
	}	
	else if(strSM==_T("삭 제"))
	{	
		m_pWindow->SubDelete();
	}
    else if(strSM==_T("상세보기"))
	{	
	   CLight_UC_NOTIFY_DETAIL dlg(m_pWindow);
	   dlg.DoModal ();

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
//__declspec(dllexport) VOID DeleteForm() 
{
	if (pWindow) 
	{
		delete pWindow;
	}
}