// MNG_USER_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_USER_MANAGER_01.h"
#include "MNG_USER_MANAGER_02.h"
#include "MNG_USER_MANAGER_03.h"
#include "MNG_USER_MANAGER_05.h"

#include "MNG_PRIVACY_PRINT_LOG_DLG.h"
#include "MNG_PRIVACY_LOG_SEARCH_DLG.h"

static AFX_EXTENSION_MODULE MNG_USER_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_USER_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_USER_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(MNG_USER_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_USER_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_USER_MANAGERDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMNG_USER_MANAGER_01* pWindow = new CMNG_USER_MANAGER_01(NULL);

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	CMNG_USER_MANAGER_01* m_pWindow = (CMNG_USER_MANAGER_01*)pWindow;

	delete m_pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{
	if( !pWindow )
		return NULL;		
	
	CMNG_USER_MANAGER_01* m_pWindow = (CMNG_USER_MANAGER_01*)pWindow;
	
	if( _T("MNG_USER_MANAGER_02") == strSM )
	{	
		if( m_pWindow->GetUserPermition() == 'U' )
		{
			m_pWindow->SearchCurrentUser();
			return NULL;
		}

		CMNG_USER_MANAGER_02 *pDlg = NULL;
		pDlg = new CMNG_USER_MANAGER_02( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
		if( pDlg->Create( m_pWindow ) )
		{
			pDlg->ShowWindow( SW_SHOW );
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}	
	else if( _T("등록") == strSM )
	{	
		CMNG_USER_MANAGER_03 Dlg( m_pWindow );
		Dlg.pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			Dlg.SM_STYLE = 'R';
		
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		
		Dlg.m_mode = 1;

		Dlg.DoModal();
	}
	else if( _T("수정") == strSM )
	{	
		CMNG_USER_MANAGER_03 Dlg( m_pWindow );
		Dlg.pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			Dlg.SM_STYLE = 'R';
		
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		
		Dlg.m_mode = 2;
		Dlg.DoModal();
		
	}
	else if( _T("상세보기") == strSM )
	{	
		CMNG_USER_MANAGER_03 Dlg( m_pWindow );
		Dlg.pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			Dlg.SM_STYLE = 'R';
		
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		
		Dlg.m_mode = 3;

		Dlg.DoModal();
		
	}
	else if( _T("MNG_USER_MANAGER_delete") == strSM )
	{
		m_pWindow->DeleteData();
		return NULL;
	}
	else if( _T("API인증키") == strSM )
	{
		CMNG_USER_MANAGER_05 dlg(m_pWindow);
		dlg.DoModal();
		return NULL;
	}
	else if( _T("개인정보 처리로그") == strSM )
	{
		CMNG_PRIVACY_LOG_SEARCH_DLG dlg(m_pWindow);
		dlg.DoModal();
	}
	
	return NULL;
}

