// BO_LOC_1300.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_13001.H"
#include "BO_LOC_1310.H"
#include "BO_LOC_1320.H"
#include "BO_LOC_1330.H"
#include "BO_LOC_1340.H"
#include "BO_LOC_1350.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_1300DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_1300.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_1300DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_1300DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_1300.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_1300DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_1300* pWindow = NULL;
	pWindow = new CBO_LOC_1300(NULL);
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

	CBO_LOC_1300 *pTmpWindow = (CBO_LOC_1300*)pWindow;

	if (strSM == _T("BO_LOC_1310")){
		CBO_LOC_1310 *pDlg = NULL;
		pDlg = new CBO_LOC_1310(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_1320"))	{
		if( pTmpWindow->GetParentDMCount() < 1 )
			return NULL;

		pTmpWindow->DetailView();
	}
	else if (strSM == _T("BO_LOC_1330"))	{
		if( pTmpWindow->GetParentDMCount() < 1 )
			return NULL;

		CBO_LOC_1330 pDlg(pTmpWindow);
		pDlg.DoModal();
	}

	if (strSM == _T("최초배가일변경")){
		CBO_LOC_1350 *pDlg = NULL;
		pDlg = new CBO_LOC_1350(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_1340")){
		if( pTmpWindow->GetParentDMCount() < 1 )
			return NULL;

		CBO_LOC_1340 pDlg(pTmpWindow);
		pDlg.DoModal();
	}
	else if (strSM == _T("출력"))
	{
		pTmpWindow->PrintList();
		return NULL;
	}
	else if ( strSM == _T("변경목록출력"))
	{
		pTmpWindow->PrintList2();
		return NULL;
	}
	else if ( strSM == _T("삭제"))
	{
		pTmpWindow->Delete();
		return NULL;
	}

	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){

	CBO_LOC_1300 *pTmpWindow = (CBO_LOC_1300*)pWindow;
	delete pTmpWindow ;
	
}
