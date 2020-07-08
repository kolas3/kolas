// SE_BIBINFO.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BIBINFO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_BIBINFODLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_BIBINFO.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_BIBINFODLL, hInstance))
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

		new CDynLinkLibrary(SE_BIBINFODLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_BIBINFO.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_BIBINFODLL);
	}
	return 1;   // ok
}



__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE,CString strMenuAlias) 
{
	CBIBINFO* m_pWindow = new CBIBINFO(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	//=================================================
	//20080606 UPDATE BY PJW : strMenuAlias가 숫자로 바뀜
	//if(_T("구입서지정보관리") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 1; //구입
	//else if(_T("기증서지정보관리") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 2; //기증
	if(_T("2140") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 1; //구입
	else if(_T("2230") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 2; //기증
	//=================================================
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}


__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CBIBINFO * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CBIBINFO*)pWindow;
	
	
	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->DoSearch();
	}
	
	if(strButtonAlias == _T("입력"))
	{
		m_pWindow->InsertSpecies();
	}

	if(strButtonAlias == _T("수정"))
	{
		m_pWindow->ModifySpecies();
	}
	
	if(strButtonAlias == _T("삭제"))
	{
		m_pWindow->DeleteSpecies();
	}
	
	if(strButtonAlias == _T("교열지출력"))
	{
		m_pWindow->PrintListData();
	}

	if(strButtonAlias == _T("출력"))
	{
		m_pWindow->PrintGridData();
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

