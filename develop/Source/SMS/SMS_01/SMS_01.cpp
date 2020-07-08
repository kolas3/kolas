// SMS_01.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SMS_DLG_01.h"
#include "SMS_DLG_01_SEARCH.h"

static AFX_EXTENSION_MODULE SMS_01DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SMS_01.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SMS_01DLL, hInstance))
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

		new CDynLinkLibrary(SMS_01DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SMS_01.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SMS_01DLL);
	}
	return 1;   // ok
}


__declspec( dllexport ) CWnd* CreateForm( CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ) 
{
	CSMS_DLG_01 *pWindow = new CSMS_DLG_01( );
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create( pParent );
	pWindow->ShowWindow( SW_SHOW );
	pWindow->UpdateWindow();
	return pWindow;
}


__declspec( dllexport ) CESL_Mgr* CreateModelessDlg( CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow ) 
{	
	if( !pWindow )
		return NULL;		

	CSMS_DLG_01 * m_pWindow = ( CSMS_DLG_01 * )pWindow;

	if( _T("검색") == strSM )
	{	
		CSMS_DLG_01_SEARCH *pDlg = NULL;
		pDlg = new CSMS_DLG_01_SEARCH( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		pDlg->SetNightLoanReservationUseYn( m_pWindow->m_strNightLoanReservationUseYn );
		pDlg->m_sIsUnityLoanService = m_pWindow->m_sIsUnityLoanService;
		
		if( pDlg->Create( m_pWindow ) )
		{
			pDlg->ShowWindow( SW_SHOW );
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
	else if( _T("문자편집") == strSM )
	{
		m_pWindow->ViewSMSContentsEditor();
	}
	else if( _T("발송") == strSM )
	{
		m_pWindow->ViewSMSContentsEditor();
	}
	else if( _T("SMS") == strSM )
	{
		m_pWindow->SendOneSMSMsg();
	}

	return NULL;
}

__declspec( dllexport ) VOID ResizeForm( CRect rect, CWnd *pWindow ) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec( dllexport ) VOID DeleteForm( CWnd *pWindow )
{
	if( pWindow )
		delete pWindow;
}