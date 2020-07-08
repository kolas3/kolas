// SE_TRANSFER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "TRANSFER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_TRANSFERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_TRANSFER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_TRANSFERDLL, hInstance))
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

		new CDynLinkLibrary(SE_TRANSFERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_TRANSFER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_TRANSFERDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE,CString strMenuAlias) 
{
	CTRANSFER* m_pWindow = new CTRANSFER(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	if(_T("구입자료인계") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 1; //구입
	else if(_T("기증자료인계") == strMenuAlias) m_pWindow->m_nACQ_TYPE = 2; //기증
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}


__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CTRANSFER * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CTRANSFER*)pWindow;


	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->DoSearch();
	}
	
	if(strButtonAlias == _T("상세보기"))
	{
		m_pWindow->ShowSpecificData();
	}
	
	if(strButtonAlias == _T("실시간인계"))
	{
		m_pWindow->TransferToRealTime();
	}

	if(strButtonAlias == _T("실시간배가"))
	{
		m_pWindow->TransferToShelf();
	}

	if(strButtonAlias == _T("등록대상처리"))
	{
		m_pWindow->TransferToReg();
	}

	if(strButtonAlias == _T("제본대상처리"))
	{
		m_pWindow->TransferToBinding();
	}
	
	if(strButtonAlias == _T("인계자료목록"))
	{
		m_pWindow->PrintTransferedList();
	}
		
	if(strButtonAlias == _T("가등록번호부여"))
	{
		m_pWindow->RecordTmpRegNo();
	}
		
	if(strButtonAlias == _T("가등록번호부여취소"))
	{
		m_pWindow->CancelTmpRegNo();
	}

	if(strButtonAlias == _T("레이블출력"))
	{
		m_pWindow->PrintLabel();
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

