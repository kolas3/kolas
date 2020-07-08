// BO_LOC_3860.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_LOC_38601.h"
#include "BO_LOC_3861.h"
#include "BO_LOC_3862.h"
#include "BO_LOC_3863.h"

static AFX_EXTENSION_MODULE BO_LOC_3860DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3860.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_3860DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_3860DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3860.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_3860DLL);
	}
	return 1;   // ok
}



__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_3860* pWindow = NULL;
	pWindow = new CBO_LOC_3860(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow){
	if (!pWindow) return NULL;

	CBO_LOC_3860* m_pWindow = (CBO_LOC_3860*)pWindow;

	if( strSM == _T("이용자검색") ) 
	{
		CBO_LOC_3861 *pDlg = NULL;
		pDlg = new CBO_LOC_3861(m_pWindow,0);
		// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용여부
		pDlg->m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
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
	else if( strSM == _T("검색된이용자일괄변경") ) 
	{
		CBO_LOC_3862 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if( strSM == _T("SQL일괄변경") ) 
	{
		CBO_LOC_3863 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if( strSM == _T("조건일괄변경") ) 
	{
		CBO_LOC_3861 *pDlg = NULL;
		pDlg = new CBO_LOC_3861(m_pWindow,1);
		// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용여부
		pDlg->m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
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


	return NULL;
}



__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	
	delete pWindow;
}
