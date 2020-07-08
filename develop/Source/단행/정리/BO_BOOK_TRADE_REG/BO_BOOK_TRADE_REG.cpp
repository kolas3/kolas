// BO_BOOK_TRADE_REG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_REG_EPC_MAIN.h"

static AFX_EXTENSION_MODULE BO_BOOK_TRADE_REGDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_BOOK_TRADE_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_BOOK_TRADE_REGDLL, hInstance))
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

		new CDynLinkLibrary(BO_BOOK_TRADE_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_BOOK_TRADE_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_BOOK_TRADE_REGDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias )
{
	CBO_REG_EPC_MAIN* pWindow = NULL;

	pWindow = new CBO_REG_EPC_MAIN(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();

	return pWindow;
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	
	CBO_REG_EPC_MAIN* pTmpWindow = (CBO_REG_EPC_MAIN*)pWindow;

	if(NULL != pTmpWindow)
	{
		// 쓰레드가 동작중에 있다면
		// 쓰레드가 종료되기까지 메시지를 처리하며 대기
		while(TRUE == pTmpWindow->m_bIsRunningSearchThread)
		{ 
			pTmpWindow->StopEPCThread();
			pTmpWindow->m_bIsFreeDLL = TRUE;

			MSG msg;
			DWORD dwStart;
			dwStart = GetTickCount();
						
			while (GetTickCount() - dwStart < 100)
			{
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		delete pTmpWindow;
	}
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow)
{
	if(pWindow)
	{
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;	
	
	CBO_REG_EPC_MAIN *pTmpWindow = (CBO_REG_EPC_MAIN*)pWindow;
	
	if(TRUE == pTmpWindow->m_bIsRunningSearchThread) return NULL;

	// 자료 찾기
	if( _T("검색") == strSM )
	{
		pTmpWindow->ShowSearchDlg();
	}
	else if( _T("일괄부여") ==strSM )
	{
		pTmpWindow->RunEPCThread();
//DEL 	pTmpWindow->CreateEPC();
	}
	else if( _T("취소") ==strSM )
	{
		pTmpWindow->CancelEPC();
	}

	return NULL;
}	