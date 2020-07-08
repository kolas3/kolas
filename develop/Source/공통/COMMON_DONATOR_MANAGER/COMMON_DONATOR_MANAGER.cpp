// COMMON_DONATOR_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DonatorMgrMainDlg.h"
#include "DonatorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE COMMON_DONATOR_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("COMMON_DONATOR_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(COMMON_DONATOR_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(COMMON_DONATOR_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("COMMON_DONATOR_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(COMMON_DONATOR_MANAGERDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CDonatorMgrMainDlg * m_pWindow = new CDonatorMgrMainDlg(NULL);
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
	CDonatorMgrMainDlg * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CDonatorMgrMainDlg*)pWindow;

	if(strButtonAlias == _T("자료검색"))
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
		m_pWindow->ShowDonatorMgrDlg(SINSERT);
		return NULL;
	}
	else if(strButtonAlias == _T("수정"))
	{
		m_pWindow->ShowDonatorMgrDlg(SUPDATE);
		return NULL;
	}
	else if(strButtonAlias == _T("삭제"))
	{
		m_pWindow->DeleteDonatorMgrDlg();
		return NULL;
	}	
	else if(strButtonAlias == _T("상세보기"))
	{
		m_pWindow->ShowDonateDataDlg();
		return NULL;
	}
	//=======================================================
	//20080617 ADD BY PJW : 감사장 인수증출력에 있는 툴바 기능을 기증자관로 이동
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
	//=======================================================
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
