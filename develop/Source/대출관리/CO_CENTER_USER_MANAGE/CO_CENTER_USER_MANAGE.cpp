// CO_CENTER_USER_MANAGE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "CO_CENTER_USER_MANAGER_02.h"
#include "CO_CENTER_USER_MANAGE_04.h"
#include "CO_CENTER_USER_MANAGE_05.h"
#include "../../관리/대출자,도서관 통합 관리/MNG_LIB_CENTER_MANAGER/MNG_LIB_CENTER_MANAGER_02.h"
#include "CO_CENTER_USER_DETAILVIEWER2.h"
#include "CO_CENTER_USER_CHANGE_HISTORY.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE CO_CENTER_USER_MANAGEDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CO_CENTER_USER_MANAGE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(CO_CENTER_USER_MANAGEDLL, hInstance))
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

		new CDynLinkLibrary(CO_CENTER_USER_MANAGEDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CO_CENTER_USER_MANAGE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(CO_CENTER_USER_MANAGEDLL);
	}
	return 1;   // ok
}


CString gstrMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE , CString strMenuAlias) 
{
	CCO_CENTER_USER_MANAGE_04* pWindow = new CCO_CENTER_USER_MANAGE_04(NULL);

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	gstrMenuAlias = strMenuAlias;
	return pWindow;
	
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if( !pWindow )
		return NULL;		
	
	CCO_CENTER_USER_MANAGE_04* m_pWindow = (CCO_CENTER_USER_MANAGE_04*)pWindow;
	
	if( _T("검색") == strSM )
	{	
		CCO_CENTER_USER_MANAGER_02* pDlg = new CCO_CENTER_USER_MANAGER_02(m_pWindow);
		// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
		pDlg->m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		pDlg->Create(m_pWindow);
		pDlg->ShowWindow(SW_SHOW);
		pDlg->UpdateWindow();
		pDlg->CenterWindow();

		return pDlg;
	}	
	else if( _T("상세보기") == strSM )
	{	
		if( m_pWindow->SearchedUser() < 1 )
		{
			AfxMessageBox(_T("검색하십시오"));
			return NULL;
		}
		
		CCO_CENTER_USER_DETAILVIEWER2 Dlg(m_pWindow,_T("CM_CO_CENTER_USER_MANAGER_01"),_T("센터이용자정보그리드"));
		// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
		Dlg.m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		Dlg.DoModal();
	}		
	else if( _T("변경이력보기") == strSM )
	{	
		if( m_pWindow->SearchedUser() < 1 )
		{
			AfxMessageBox(_T("검색하십시오"));
			return NULL;
		}
		
		CCO_CENTER_USER_CHANGE_HISTORY Dlg(m_pWindow,_T("CM_CO_CENTER_USER_MANAGER_01"),_T("센터이용자정보그리드"));
		// 2010.09.13 ADD BY KSJ : 통합도서서비스 사용여부
		Dlg.m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		Dlg.DoModal();
	}		
	
	return NULL;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if( pWindow )
		delete pWindow;
}
