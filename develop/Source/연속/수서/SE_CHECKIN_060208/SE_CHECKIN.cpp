// SE_CHECKIN.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "CHECKIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_CHECKINDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_CHECKIN.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_CHECKINDLL, hInstance))
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

		new CDynLinkLibrary(SE_CHECKINDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_CHECKIN.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_CHECKINDLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CCHECKIN* m_pWindow = new CCHECKIN(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;

	if(_T("구입체크인") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 1; //구입 
	else if(_T("기증체크인") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 2; //기증 

	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}



__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CCHECKIN * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CCHECKIN*)pWindow;


	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->DoSearch();	
	}
	
	if(strButtonAlias == _T("입수지연"))
	{
		m_pWindow->AcqDelay();
	}
	
	if(strButtonAlias == _T("일괄입수지연"))
	{
		m_pWindow->AcqDelay(FALSE);
	}

	if(strButtonAlias == _T("권별체크인"))
	{
		m_pWindow->LaunchCheckinVol();
	}
	
	if(strButtonAlias == _T("접수번호"))
	{
		m_pWindow->SetReceiptNo();
	}

	if(strButtonAlias == _T("메모"))
	{
		m_pWindow->MemoEdit();
	}
	
	if(strButtonAlias == _T("출력"))
	{
		m_pWindow->PrintGridData();
	}

	if(strButtonAlias == _T("서지입력"))
	{
		m_pWindow->InsertSpecies();
	}

	if(strButtonAlias == _T("서지수정"))
	{
		m_pWindow->ModifySpecies();
	}

	if(strButtonAlias == _T("정산관리"))
	{
		m_pWindow->AccountStatement();
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

