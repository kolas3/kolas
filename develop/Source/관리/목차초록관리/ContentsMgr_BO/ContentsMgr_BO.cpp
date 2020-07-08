// ContentsMgr_BO.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CONTENTMGR_01.h"
#include "CONTENTMGR_02.h"
#include "CONTENTMGR_03.h"
#include "DATA_SEARCH.h"
#include "CODE_SEARCH.h"
#include "CLASS_SEARCH.h"
#include "CONTENTMGR_SPECIES.h"
#include "CONTENTMGR_SEARCH.h"
#include "CONTENTMGR_INDEX_SEARCH.h"

static AFX_EXTENSION_MODULE ContentsMgr_BODLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CONTENTSMGR_BO.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ContentsMgr_BODLL, hInstance))
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

		new CDynLinkLibrary(ContentsMgr_BODLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CONTENTSMGR_BO.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ContentsMgr_BODLL);
	}
	return 1;   // ok
}



__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 

{
	CCONTENTMGR_01* m_pWindow = new CCONTENTMGR_01(NULL);   
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow ;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	CCONTENTMGR_01* m_pWindow = (CCONTENTMGR_01*)pWindow;

	if (strSM == _T("목차편집"))
	{  

        m_pWindow->OnInsert_TOC_ABS() ;
	       
	}
    
	else if (strSM == _T("수 정"))
	{ 
		m_pWindow->OnEdit_TOC_ABS () ; 
	/*
    	CCONTENTMGR_SPECIES dlg(m_pWindow); // 0  : 목차 초록 신규입력
    	dlg.DoModal();
    */   
	}
	else if(strSM==_T("목차보기"))
	{	
		m_pWindow->OnViewMoc();
	}
	else if(strSM==_T("초록보기"))
	{	
		m_pWindow->OnViewCho();
	}
	else if(strSM==_T("삭 제"))
	{	
		
	}

	else if(strSM ==_T("출 력"))
	{
 
		m_pWindow->Print_List () ;
		
	}


   	else if(strSM==_T("목차검색"))
	{	
	//	CCONTENTMGR_INDEX_SEARCH
		CCONTENTMGR_INDEX_SEARCH *pDlg = NULL;
		pDlg = new CCONTENTMGR_INDEX_SEARCH(m_pWindow);
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
    else if(strSM==_T("검 색"))
	{	
		CCONTENTMGR_SEARCH *pDlg = NULL;
		pDlg = new CCONTENTMGR_SEARCH(m_pWindow);
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


 
__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if (pWindow) 
	{
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)	
//__declspec(dllexport) VOID DeleteForm() 
{
	if (pWindow) 
	{
		delete pWindow;
	}
}
