// SMS_05.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SMS_5100.h"
#include "SMS_5110.h"
#include "SMS_5120.h"
#include "SMS_5130.h"


static AFX_EXTENSION_MODULE SMS_05DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SMS_05.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SMS_05DLL, hInstance))
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

		new CDynLinkLibrary(SMS_05DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SMS_05.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SMS_05DLL);
	}
	return 1;   // ok
}



CString gstrMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{

	CSMS_5100* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CSMS_5100(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	gstrMenuAlias = strMenuAlias;
	return m_pWindow;

}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	
	CSMS_5100* m_pWindow = (CSMS_5100*)pWindow;
	
	if (strSM == _T("검색"))
	{
		CSMS_5110 *pDlg = NULL;
		pDlg = new CSMS_5110(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		pDlg->m_bUseILL = m_pWindow->m_bUseILL;
		
		
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
	else if (strSM == _T("발송내역상세보기"))
	{
		CSMS_5120 Dlg(m_pWindow);
		Dlg.m_bUseILL = m_pWindow->m_bUseILL;
		
		Dlg.m_strSearchWhere	=	m_pWindow->GetSearchWhere();
		Dlg.DoModal();

		m_pWindow->ViewChangeGrid();
	}		
	else if (strSM == _T("출력"))
	{
		m_pWindow->PrintList();
	}		
	else if (strSM == _T("전송이력조회"))
	{
		CSMS_5130 Dlg(m_pWindow);
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
	CSMS_5100* m_pWindow = (CSMS_5100*)pWindow;
	
	if( pWindow )
	{
		delete m_pWindow;
	}
}
