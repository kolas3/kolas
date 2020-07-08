// MarcRefManager.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "MarcRefMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MarcRefManagerDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MARCREFMANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MarcRefManagerDLL, hInstance))
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

		new CDynLinkLibrary(MarcRefManagerDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MARCREFMANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MarcRefManagerDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMarcRefMain* m_pWindow = new CMarcRefMain(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}



__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CMarcRefMain * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CMarcRefMain*)pWindow;


	if(strButtonAlias == _T("그룹"))
	{
		m_pWindow->ShowGroupMgr();	
	}
	
	if(strButtonAlias == _T("태그입력"))
	{
		m_pWindow->InsertTag();	
	}

	if(strButtonAlias == _T("태그수정"))
	{
		m_pWindow->ModifyTag();	
	}

	if(strButtonAlias == _T("태그삭제"))
	{
		m_pWindow->DeleteTag();	
	}

	if(strButtonAlias == _T("지시기호입력"))
	{
		m_pWindow->InsertIndicator();	
	}

	if(strButtonAlias == _T("지시기호수정"))
	{
		m_pWindow->ModifyIndicator();
	}
	
	if(strButtonAlias == _T("지시기호삭제"))
	{
		m_pWindow->DeleteIndicator();	
	}

	if(strButtonAlias == _T("식별기호입력"))
	{
		m_pWindow->InsertSubfield();	
	}

	if(strButtonAlias == _T("식별기호수정"))
	{
		m_pWindow->ModifySubfield();
	}
	
	if(strButtonAlias == _T("식별기호삭제"))
	{
		m_pWindow->DeleteSubfield();	
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

