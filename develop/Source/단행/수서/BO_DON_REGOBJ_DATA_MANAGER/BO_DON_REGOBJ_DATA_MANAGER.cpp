// BO_DON_REGOBJ_DATA_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_DON_RegObjDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_DON_REGOBJ_DATA_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_DON_REGOBJ_DATA_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_DON_REGOBJ_DATA_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(BO_DON_REGOBJ_DATA_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_DON_REGOBJ_DATA_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_DON_REGOBJ_DATA_MANAGERDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CBO_DON_RegObjDataManager  * m_pWindow = new CBO_DON_RegObjDataManager(NULL);
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
	CBO_DON_RegObjDataManager * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CBO_DON_RegObjDataManager*)pWindow;

	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->ShowSearchDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("출력"))
	{
		m_pWindow->ShowPrintDlg();
		return NULL;		
	}
	else if(strButtonAlias == _T("입력"))
	{
		m_pWindow->ShowRegDataDlg(SINSERT);
		return NULL;
	}
	else if(strButtonAlias == _T("수정"))
	{
		m_pWindow->ShowRegDataDlg(SUPDATE);
		return NULL;
	}
	else if(strButtonAlias == _T("삭제"))
	{
		m_pWindow->DeleteRegObjData();
		return NULL;
	}
	else if(strButtonAlias == _T("코드변경"))
	{
		m_pWindow->ShowChangeCodeDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("등록인계"))
	{
		m_pWindow->RegTransfer(strButtonAlias);
		return NULL;
	}
	else if(strButtonAlias == _T("복본대상"))
	{
		m_pWindow->RegTransfer(strButtonAlias);
		return NULL;
	}
	else if(strButtonAlias == _T("분류대상"))
	{
		m_pWindow->RegTransfer(strButtonAlias);
		return NULL;
	}
	else if(strButtonAlias == _T("정리대상"))
	{
		m_pWindow->RegTransfer(strButtonAlias);
		return NULL;
	}
	else if(strButtonAlias == _T("차수관리"))
	{
		m_pWindow->ShowDivNoMgrDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("정렬"))
	{
		m_pWindow->RegObjDataOrder();
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
