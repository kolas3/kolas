	// MNG_CODE_SHELF.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_CODE_SHELF_01.h"
#include "MNG_CODE_SHELF_02.h"
#include "MNG_CODE_SHELF_03.h"
#include "MNG_CODE_SHELF_04.h"

static AFX_EXTENSION_MODULE MNG_CODE_SHELFDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_CODE_SHELF.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_CODE_SHELFDLL, hInstance))
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

		new CDynLinkLibrary(MNG_CODE_SHELFDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_CODE_SHELF.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_CODE_SHELFDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMNG_CODE_SHELF_01* pWindow = new CMNG_CODE_SHELF_01(NULL);

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if( pWindow )
		delete pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if( !pWindow )
		return NULL;		

	CMNG_CODE_SHELF_01* m_pWindow = (CMNG_CODE_SHELF_01*)pWindow;
	
	if( _T("MNG_CODE_SHELF_02") == strSM )
	{	
		CMNG_CODE_SHELF_02 *pDlg = NULL;
		pDlg = new CMNG_CODE_SHELF_02( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		// 17/03/09 김혜영 : 공통>관리>업무환경설정>배가기준관리의 배가실을 두번 실행했을때 프로그램 종료현상
/*//BEFORE-----------------------------------------------------------------------------
		pDlg->m_openMode = 1;
*///AFTER------------------------------------------------------------------------------
		pDlg->m_openMode = 2;
/*//END-------------------------------------------------------------------------------*/
		
		if( pDlg->Create( m_pWindow ) )
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow( SW_SHOW );
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if( _T("MNG_CODE_SHELF_03") == strSM )
	{	
		CMNG_CODE_SHELF_03 *pDlg = NULL;
		pDlg = new CMNG_CODE_SHELF_03( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
		if( pDlg->Create( m_pWindow ) )
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow( SW_SHOW );
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if( _T("MNG_CODE_SHELF_04") == strSM )
	{	
		CMNG_CODE_SHELF_04 *pDlg = NULL;
		pDlg = new CMNG_CODE_SHELF_04( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
		if( _T("입력") == strButtonAlias )
			pDlg->m_mode = 1;
		else if( _T("수정") == strButtonAlias )
			pDlg->m_mode = 2;

		if( pDlg->Create( m_pWindow ) )
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow( SW_SHOW );
			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if( _T("MNG_CODE_SHELF_delete") == strSM )
	{
		m_pWindow->DeleteData();
		return NULL;
	}

	return NULL;
}