// BO_STATICS_1100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_STATICS_11001.h"


#include "TmpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_STATICS_1100DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_STATICS_1100.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_STATICS_1100DLL, hInstance))
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

		new CDynLinkLibrary(BO_STATICS_1100DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_STATICS_1100.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_STATICS_1100DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_STATICS_11001* pWindow = NULL;
	pWindow = new CBO_STATICS_11001(strMenuAlias,NULL);
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

	CBO_STATICS_11001 *pTmpWindow = (CBO_STATICS_11001*)pWindow;

	if (strSM == _T("조건생성"))
	{
		pTmpWindow->ViewCondWindow();
	}
	else if (strSM == _T("행렬결정"))
	{
		pTmpWindow->ViewXYWindow();
	}
	else if (strSM == _T("통계"))
	{
		pTmpWindow->ShowTempDlg(bReadOnly);
	}
	else if (strSM == _T("출력"))
	{
		pTmpWindow->PrintStat();
	}

	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	CBO_STATICS_11001 *pTmpWindow = (CBO_STATICS_11001*)pWindow;
	if(pWindow){

		if( pTmpWindow->m_pSearchCreateDlg1 != NULL )
		{
			if( pTmpWindow->m_pSearchCreateDlg1->GetSafeHwnd() != NULL )
				pTmpWindow->m_pSearchCreateDlg1->DestroyWindow();
			delete pTmpWindow->m_pSearchCreateDlg1;
			pTmpWindow->m_pSearchCreateDlg1 = NULL;
		}
						
		if( pTmpWindow->m_pSearchCreateDlg2 != NULL )
		{
			if( pTmpWindow->m_pSearchCreateDlg2->GetSafeHwnd() != NULL )
				pTmpWindow->m_pSearchCreateDlg2->DestroyWindow();
			delete pTmpWindow->m_pSearchCreateDlg2;
			pTmpWindow->m_pSearchCreateDlg2 = NULL;
		}

		if( pTmpWindow->m_pSearchCreateDlg3 != NULL )
		{
			if( pTmpWindow->m_pSearchCreateDlg3->GetSafeHwnd() != NULL )
				pTmpWindow->m_pSearchCreateDlg3->DestroyWindow();
			delete pTmpWindow->m_pSearchCreateDlg3;
			pTmpWindow->m_pSearchCreateDlg3 = NULL;
		}

		if( pTmpWindow->m_pSearchCreateDlg4 != NULL )
		{
			if( pTmpWindow->m_pSearchCreateDlg4->GetSafeHwnd() != NULL )
				pTmpWindow->m_pSearchCreateDlg4->DestroyWindow();
			delete pTmpWindow->m_pSearchCreateDlg4;
			pTmpWindow->m_pSearchCreateDlg4 = NULL;
		}

		if( pTmpWindow->m_pXYCreateDlg != NULL )
		{
			if( pTmpWindow->m_pXYCreateDlg->GetSafeHwnd() != NULL )
				pTmpWindow->m_pXYCreateDlg->DestroyWindow();
			delete pTmpWindow->m_pXYCreateDlg;
			pTmpWindow->m_pXYCreateDlg = NULL;
		}
		
		if( pTmpWindow->m_pSInfoMgr	!= NULL ) 
			delete pTmpWindow->m_pSInfoMgr;
		pTmpWindow->m_pSInfoMgr = NULL;

		delete pWindow;
	}
}
