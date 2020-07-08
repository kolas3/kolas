// CHECK_RETURN_MAIL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "RETURN_MAIL_MAIN.h"


static AFX_EXTENSION_MODULE CHECK_RETURN_MAILDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CHECK_RETURN_MAIL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(CHECK_RETURN_MAILDLL, hInstance))
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

		new CDynLinkLibrary(CHECK_RETURN_MAILDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CHECK_RETURN_MAIL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(CHECK_RETURN_MAILDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{
	CRETURN_MAIL_MAIN* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CRETURN_MAIL_MAIN(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}


__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;

	CRETURN_MAIL_MAIN* m_pWindow = (CRETURN_MAIL_MAIN*)pWindow;
/*	
	if (strSM == _T("배가관리_검색"))
	{
		CBO_LOC_1110 *pDlg = NULL;
		pDlg = new CBO_LOC_1110(m_pWindow,gstrMenuAlias);
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
*/
	if( strSM == _T("검색"))
	{
		m_pWindow->DisplayRetrunMail () ;
//DEL 		AfxMessageBox(_T("검색되었습니다."));
	}
	else if (strSM == _T("내용확인"))
	{
		m_pWindow->ShowDetailReturnMail () ;
	}
	else if (strSM == _T("리턴코드정보"))
	{
		m_pWindow->ShowReturnCodeMsg();
	}
	else if (strSM == _T("로그저장"))
	{
		//m_pWindow->ShowDetailReturnMail () ;
		m_pWindow->SaveLogToTable () ;
	}
	else if (strSM == _T("로그보기"))
	{
		//m_pWindow->ShowReturnCodeMsg();
		m_pWindow->ShowLogTable () ;
	}
	else if (strSM == _T("이메일수정"))
	{
		m_pWindow->EMailEdit() ;
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
	if( pWindow )
	{
		pWindow->DestroyWindow();
		delete pWindow;
	}
}
