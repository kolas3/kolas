// SMS_04.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "SMS_4100.h"
#include "SMS_4110.h"

#include "../CSMSContentsEditor/SMSContentsEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SMS_04DLL = { NULL, NULL };


extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SMS_04.DLL Initializing!\n");
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SMS_04DLL, hInstance))
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

		new CDynLinkLibrary(SMS_04DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SMS_04.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SMS_04DLL);
	}
	return 1;   // ok
}

CString gstrMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{
	CSMS_4100* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CSMS_4100(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	gstrMenuAlias = strMenuAlias;

	m_pWindow->SetFocus();	

	return m_pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;

	CSMS_4100* m_pWindow = (CSMS_4100*)pWindow;

	if (strSM == _T("발송"))
	{
		m_pWindow->ViewSMSContentsEditor();
	}
	else if( strSM == _T("문자편집") )
	{
 		return NULL;
	}
	else if( strSM == _T("검색") )
	{
		CSMS_4110 *pDlg = NULL;
		pDlg = new CSMS_4110(m_pWindow,gstrMenuAlias);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;

		if( TRUE ==  bReadOnly) 
		{
			pDlg->SM_STYLE = 'R';
		}

		pDlg->m_pInfo = m_pWindow->m_pInfo;
		pDlg->m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;

		if( TRUE == pDlg->Create(m_pWindow))
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
	if( pWindow )
		pWindow->MoveWindow( rect );
}
__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	CSMS_4100* m_pWindow = (CSMS_4100*)pWindow;
	delete m_pWindow;
	m_pWindow = NULL;
}











































