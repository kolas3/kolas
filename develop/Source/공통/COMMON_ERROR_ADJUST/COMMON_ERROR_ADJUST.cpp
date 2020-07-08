// COMMON_ERROR_ADJUST.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_DonateErrorAdjust.h"
#include "BO_PurchaseErrorAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE COMMON_ERROR_ADJUSTDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("COMMON_ERROR_ADJUST.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(COMMON_ERROR_ADJUSTDLL, hInstance))
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

		new CDynLinkLibrary(COMMON_ERROR_ADJUSTDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("COMMON_ERROR_ADJUST.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(COMMON_ERROR_ADJUSTDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	if(strMenuAlias == _T("구입정보오류수정"))
	{
		CBO_PurchaseErrorAdjust * m_pWindow = new CBO_PurchaseErrorAdjust(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();
		return m_pWindow;
	}
	else if(strMenuAlias == _T("기증정보오류수정"))
	{
		CBO_DonateErrorAdjust * m_pWindow = new CBO_DonateErrorAdjust(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();
		return m_pWindow;
	}
	return NULL;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if(strButtonAlias == _T("구입정보오류수정"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->AdjustPurchaseDataError();
		return NULL;
	}
	else if(strButtonAlias == _T("구입책색인동기화"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->SynchronizedBookToIndex();
		return NULL;		
	}
	else if(strButtonAlias == _T("기증정보오류수정"))
	{
		((CBO_DonateErrorAdjust*)pWindow)->AdjustDonateDataError();
		return NULL;
	}
	else if(strButtonAlias == _T("기증책색인동기화"))
	{
		((CBO_DonateErrorAdjust*)pWindow)->SynchronizedBookToIndex();
		return NULL;		
	}
	else if(strButtonAlias == _T("기증검색"))
	{
		((CBO_DonateErrorAdjust*)pWindow)->MakeDonateErrorData();
		return NULL;
	}
	else if(strButtonAlias == _T("구입검색"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->MakePurchaseErrorData(0);
		return NULL;		
	}
	else if(strButtonAlias == _T("구입색인재구축"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->RestructIndexField();
		return NULL;		
	}
	else if(strButtonAlias == _T("색인재구축검색"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->ShowIndexRestructSearchDlg();
		return NULL;		
	}
	else if(strButtonAlias == _T("쓰레기자료검색"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->SearchWasteData();
		return NULL;		
	}
	else if(strButtonAlias == _T("쓰레기자료삭제"))
	{
		((CBO_PurchaseErrorAdjust*)pWindow)->DeleteWasteData();
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
