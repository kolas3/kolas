// DONATOR_DATA_PRINT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DonatorDataPrint.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE DONATOR_DATA_PRINTDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DONATOR_DATA_PRINT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(DONATOR_DATA_PRINTDLL, hInstance))
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

		new CDynLinkLibrary(DONATOR_DATA_PRINTDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DONATOR_DATA_PRINT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(DONATOR_DATA_PRINTDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CDonatorDataPrint * m_pWindow = new CDonatorDataPrint(NULL);
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
	CDonatorDataPrint * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CDonatorDataPrint*)pWindow;

	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->ShowSearchDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("수정"))
	{
		m_pWindow->ShowDonatorDataDlg(SUPDATE);
		return NULL;
	}
	else if(strButtonAlias == _T("인수증출력"))
	{
		m_pWindow->DonateConfirmDataPrint();
		return NULL;		
	}
	else if(strButtonAlias == _T("감사장출력"))
	{
		m_pWindow->DonatorDataPrint();
		return NULL;		
	}
	else if(strButtonAlias == _T("주소레이블출력"))
	{
		m_pWindow->AddressLabelPrint();
		return NULL;		
	}
	else if(strButtonAlias == _T("상세보기"))
	{
		m_pWindow->ShowDonateDataDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("정렬"))
	{
		m_pWindow->ShowSortDlg();
		return NULL;
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
