// BO_CAT_AUXILIARY_WORD.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_AUXILIARY_WORD1.h"
#include "BO_CAT_AUXILIARY_WORD_0000.h"
#include "BO_CAT_AUXILIARY_WORD_0100.h"
#include "BO_CAT_AUXILIARY_WORD_0200.h"
#include "BO_CAT_AUXILIARY_WORD_0300.h"

static AFX_EXTENSION_MODULE BO_CAT_AUXILIARY_WORDDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_AUXILIARY_WORD.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_AUXILIARY_WORDDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_AUXILIARY_WORDDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_AUXILIARY_WORD.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_AUXILIARY_WORDDLL);
	}
	return 1;   // ok
}

CBO_CAT_AUXILIARY_WORD* m_pWindow=NULL;

__declspec(dllexport) VOID CreateForm(CWnd *pParent, CESL_Information* pInfo) 
{
	m_pWindow = NULL;
	m_pWindow = new CBO_CAT_AUXILIARY_WORD(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
}

__declspec(dllexport) VOID ResizeForm(CRect rect) 
{
	if (m_pWindow) 
	{
		m_pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm() 
{
	if (m_pWindow) 
	{
		delete m_pWindow;
	}
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly)
{
	if (!m_pWindow) return NULL;

	if (strSM == _T("검색"))
	{
		CBO_CAT_AUXILIARY_WORD_0100 *pDlg = NULL;
		pDlg = new CBO_CAT_AUXILIARY_WORD_0100(m_pWindow);
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
		CBO_CAT_AUXILIARY_WORD_0200 *pDlg = NULL;
		pDlg = new CBO_CAT_AUXILIARY_WORD_0200(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		if( pDlg->DoModal() == IDOK ) {
			m_pWindow->CreateFile();
			//CreateFile();
		}
		delete pDlg;
		return NULL;		
	}
	else if (strSM == _T("수정"))
	{
		CBO_CAT_AUXILIARY_WORD_0300 *pDlg = NULL;
		pDlg = new CBO_CAT_AUXILIARY_WORD_0300(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		if( pDlg->DoModal() == IDOK ) {
			m_pWindow->CreateFile();
		}
		delete pDlg;
		return NULL;
	}
	else if (strSM == _T("삭제"))
	{	
		m_pWindow->Delete();
	}

	return NULL;
}