// BO_CAT_NUMBER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_ACQUISIT_NO.h"
#include "BO_CAT_NUMBER_0000.h"
#include "BO_CAT_NUMBER_0100.h"
#include "BO_CAT_NUMBER_0200.h"
#include "BO_CAT_NUMBER_0300.h"

static AFX_EXTENSION_MODULE BO_CAT_NUMBERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_NUMBER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_NUMBERDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_NUMBERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_NUMBER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_NUMBERDLL);
	}
	return 1;   // ok
}


//CBO_CAT_ACQUISIT_NO* m_pWindow=NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias ) 
//__declspec(dllexport) VOID CreateForm(CWnd *pParent, CESL_Information* pInfo) 
{
	//CBO_CAT_ACQUISIT_NO* m_pWindow = new CBO_CAT_ACQUISIT_NO(NULL);
	CBO_CAT_NUMBER_0000* m_pWindow = new CBO_CAT_NUMBER_0000(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;		
	//m_pWindow = new CBO_CAT_ACQUISIT_NO(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}

//__declspec(dllexport) VOID ResizeForm(CRect rect) 
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
	if (!pWindow) return NULL;
	CBO_CAT_ACQUISIT_NO* m_pWindow = (CBO_CAT_ACQUISIT_NO*)pWindow;

	if (strSM == _T("검색"))
	{
		CBO_CAT_NUMBER_0100 *pDlg = NULL;
		pDlg = new CBO_CAT_NUMBER_0100(m_pWindow);
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
	else if (strSM == _T("입력"))
	{
		CBO_CAT_NUMBER_0200 *pDlg = NULL;
		pDlg = new CBO_CAT_NUMBER_0200(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
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
		pDlg->DoModal();
		delete pDlg;
		return NULL;
	}
	else if (strSM == _T("수정"))
	{
		CBO_CAT_NUMBER_0300 *pDlg = NULL;
		pDlg = new CBO_CAT_NUMBER_0300(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
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
		pDlg->DoModal();
		delete pDlg;
		return NULL;
	}
	else if (strSM == _T("삭제"))
	{
		CBO_CAT_NUMBER_0000 *pDlg = NULL;
		pDlg = new CBO_CAT_NUMBER_0000(m_pWindow);
		//CString m_sWork = m_pWindow->m_sWork;
		CString sWork = ((CBO_CAT_NUMBER_0000*)m_pWindow)->m_sWork;
		if ( sWork == _T("마지막수입순번호") ) {
			sWork = _T("수입순번호관리");
		}
		//pDlg->OnbDELETE();
		//m_pWindow->OnbDELETE(m_sWork);
		pDlg->OnbDELETE(sWork);
	}

	return NULL;
}