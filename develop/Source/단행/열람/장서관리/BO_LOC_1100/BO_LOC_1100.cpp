// BO_LOC_1100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_11001.h"
#include "BO_LOC_1110.h"
#include "BO_LOC_1120.h"
#include "BO_LOC_1130.h"
#include "BO_LOC_1140.h"
#include "BO_LOC_1160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_1100DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_1100.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_1100DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_1100DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_1100.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_1100DLL);
	}
	return 1;   // ok
}

CString gstrMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{
	CBO_LOC_1100* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CBO_LOC_1100(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	gstrMenuAlias = strMenuAlias;
	return m_pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	
	CBO_LOC_1100* m_pWindow = (CBO_LOC_1100*)pWindow;
	
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
	else if (strSM == _T("배가관리_배가일자기록"))
	{
		CESL_Grid* pParentGrid = (CESL_Grid*)m_pWindow->FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
		if( pParentGrid == NULL ) return FALSE;

		INT ids = pParentGrid->SelectMakeList();
		if(ids < 0) return FALSE;
		ids = pParentGrid->SelectGetHeadPosition();
		if(ids < 0) 
		{
			AfxMessageBox ( _T("선정된 자료가 없습니다.") );
			return FALSE;
		}

		CBO_LOC_1140 Dlg(m_pWindow);
		Dlg.DoModal();

	}
	else if ( strSM == _T("인계목록") )
	{
		m_pWindow->PrintList();
	}
	else if ( strSM == _T("출력") )
	{
		m_pWindow->SimplePrintList();
	}
	else if ( strSM == _T("분류별통계") )
	{
		CBO_LOC_1160 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if ( strSM == _T("배가관리_부록정보") )
	{
		m_pWindow->AppView();
	}
	else if ( strSM == _T("배가관리_권별정보") )
	{
		m_pWindow->DetailView();
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
