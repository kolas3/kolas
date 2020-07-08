// MNG_CODE_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_CODE_MANAGER_01.h"
#include "MNG_CODE_MANAGER_02.h"
#include "MNG_CODE_MANAGER_03.h"
#include "MNG_CODE_MANAGER_09.h"

static AFX_EXTENSION_MODULE MNG_CODE_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_CODE_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_CODE_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(MNG_CODE_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_CODE_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_CODE_MANAGERDLL);
	}
	return 1;   // ok
}

//CMNG_CODE_MANAGER_01* m_pWindow = NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMNG_CODE_MANAGER_01* pWindow = new CMNG_CODE_MANAGER_01(NULL);
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
	
	CMNG_CODE_MANAGER_01* m_pWindow = (CMNG_CODE_MANAGER_01*)pWindow;
	
	// 코드 검색
	if( _T("MNG_CODE_MANAGER_02") == strSM )
	{	
		CMNG_CODE_MANAGER_02 *pDlg = NULL;
		pDlg = new CMNG_CODE_MANAGER_02( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
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
	// 코드 클래스 관리
	else if( _T("MNG_CODE_MANAGER_09") == strSM )
	{	
		CMNG_CODE_MANAGER_09 *pDlg = NULL;
		pDlg = new CMNG_CODE_MANAGER_09( m_pWindow );
		pDlg->pMain = m_pWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		
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
	// 코드 삭제
	else if( _T("MNG_CODE_MANAGER_01_delete") == strSM )
	{
		m_pWindow->DeleteCode();
		m_pWindow->RefreshCodeData();
		return NULL;
		
	}
	// 코드 수정
	else if( _T("MNG_CODE_MANAGER_01_modify") == strSM )
	{
		m_pWindow->SaveModifyedUserCode();
		m_pWindow->RefreshCodeData();
		return NULL;
		
	}
	// 코드 패턴 추가
	else if( _T("MNG_CODE_MANAGER_04") == strSM )
	{
		m_pWindow->AddCodePattern();
		return NULL;
		
	}
	// 코드 패턴 삭제
	else if( _T("MNG_CODE_MANAGER_01_delPattern") == strSM )
	{
		m_pWindow->DeleteCodePattern();
		return NULL;
		
	}
	// 코드 패턴 수정
	else if( _T("MNG_CODE_MANAGER_01_patternModify") == strSM )
	{
		m_pWindow->SaveModifyedPattern();
		return NULL;
		
	}
	// 코드 입력 / 수정
	else if( _T("MNG_CODE_MANAGER_03") == strSM  )
	{
/*		
		if( !m_pWindow->GetDMRow() )
			return NULL;
*/		
		if( _T("코드입력") == strButtonAlias )
			m_pWindow->m_type = 1;
		else if( _T("코드수정") == strButtonAlias )
		{
			if( !m_pWindow->GetDMRow() )
				return NULL;
			m_pWindow->m_type = 2;
		}

		m_pWindow->AddOrModifyCode();
		//m_pWindow->RefreshCodeData();
		
		return NULL;
	}	

	return NULL;
}
