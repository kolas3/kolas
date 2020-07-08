// BO_CAT_REG_SEARCH.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_CAT_REG_SEARCH1.h"
#include "BO_CAT_DROP_INPUT.h"
#include "BO_CAT_REG_SEARCH_0100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include _T("..\BO_CAT_REG\BO_CAT_REG_0320.h")

static AFX_EXTENSION_MODULE BO_CAT_REG_SEARCHDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_REG_SEARCH.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_REG_SEARCHDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_REG_SEARCHDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_REG_SEARCH.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_REG_SEARCHDLL);
	}
	return 1;   // ok
}

//CBO_CAT_REG_0320* m_pWindow=NULL;
CBO_CAT_REG_SEARCH_0100 *pDlg;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ) 
//__declspec(dllexport) VOID CreateForm(CWnd *pParent, CESL_Information* pInfo) 
{
	//CBO_CAT_REG_0320* m_pWindow = new CBO_CAT_REG_0320(NULL);
	CBO_CAT_REG_SEARCH* m_pWindow = new CBO_CAT_REG_SEARCH(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	//m_pWindow = NULL;
	//m_pWindow = new CBO_CAT_REG_0320(CHILD, NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();

	pDlg = NULL;

	return m_pWindow;
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

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
//__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly)
{
	//if (!m_pWindow) return NULL;
	if (!pWindow) return NULL;
	//CBO_CAT_REG_0320* m_pWindow = (CBO_CAT_REG_0320*)pWindow;
	CBO_CAT_REG_SEARCH* m_pWindow = (CBO_CAT_REG_SEARCH*)pWindow;
	
	//CBO_CAT_REG_SEARCH_0100 *pDlg = NULL;
	//CBO_CAT_REG_SEARCH_0100 *pDlg;
	
	if (strSM == _T("검색"))
	{
		//CBO_CAT_REG_SEARCH_0100 *pDlg = NULL;
		if ( pDlg == NULL )
		{
			pDlg = new CBO_CAT_REG_SEARCH_0100(m_pWindow);
		}
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
	else if (strSM == _T("파일출력"))
	{/*
		CBO_CAT_REG_SEARCH_0100 *pDlg = NULL;
		*/		
		if ( pDlg == NULL )
		{
			pDlg = new CBO_CAT_REG_SEARCH_0100(m_pWindow);
		}
		
		//CString sRegSw = pDlg->m_sRegSw;
		CString sRegSw = m_pWindow->m_sRegSh;
		pDlg->Onb0320TOFILE(sRegSw);
		/*
		CBO_CAT_PROC_0200 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0200(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		pDlg->DoModal();
		delete pDlg;
		return NULL;
		/*
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
		*/
	}
	else if (strSM == _T("누락등록번호입력"))
	{/*
		CBO_CAT_DROP_INPUT *pDlg = NULL;
		pDlg = new CBO_CAT_DROP_INPUT(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		pDlg->DoModal();
		delete pDlg;
		return NULL;
		*/
		
		CBO_CAT_DROP_INPUT dlg(m_pWindow);		
		dlg.DoModal();

	}

	return NULL;
}