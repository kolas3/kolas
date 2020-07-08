// MNG_KDCDDC_CLASS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MNG_KDCDDC_MAIN.h"
#include "MNG_KEYWORD_MAIN.h"
#include "MNG_KDCDDC_HELP_MAIN.h"
#include "MNG_INDEX_MAIN.h"
#include "MNG_KDCDDC_SEARCH.h"
#include "MNG_NEW_KDCDDC.h"
#include "MNG_NEW_HELP.h"
#include "MNG_NEW_INDEX.h"
#include "MNG_NEW_KEYWORD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MNG_KDCDDC_CLASSDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_KDCDDC_CLASS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_KDCDDC_CLASSDLL, hInstance))
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

		new CDynLinkLibrary(MNG_KDCDDC_CLASSDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_KDCDDC_CLASS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_KDCDDC_CLASSDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{

	//등록자료관리 
	if ( strMenuAlias == _T("분류기호관리_주기표") )
	{
		CMNG_KDCDDC_MAIN*	m_pWindow = new CMNG_KDCDDC_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();


		return m_pWindow;

	}

	if ( strMenuAlias == _T("분류기호관리_조기표") )
	{
		CMNG_KDCDDC_HELP_MAIN*	m_pWindow = new CMNG_KDCDDC_HELP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();


		return m_pWindow;

	}

	if ( strMenuAlias == _T("분류기호관리_연동색인어") )
	{
		CMNG_INDEX_MAIN*	m_pWindow = new CMNG_INDEX_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();


		return m_pWindow;

	}

	if ( strMenuAlias == _T("분류기호관리_주제명") )
	{
		CMNG_KEYWORD_MAIN*	m_pWindow = new CMNG_KEYWORD_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();


		return m_pWindow;

	}

	return NULL;

}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if (pWindow) 
	{
		pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if (pWindow) 
	{
		delete pWindow;

	}
}
__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow )
{
	if (!pWindow) return NULL;
	CMNG_KDCDDC_MAIN* m_pWindow = (CMNG_KDCDDC_MAIN*)pWindow;


	if (strSM == _T("주기표_검색"))
	{
		CMNG_KDCDDC_SEARCH *pDlg = NULL;
		pDlg = new CMNG_KDCDDC_SEARCH(m_pWindow);
		pDlg->OpenMode = 0;

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
	if (strSM == _T("주기표_입력"))
	{
		CMNG_NEW_KDCDDC Dlg(m_pWindow);
		Dlg.OpenMode = 0;

		Dlg.DoModal();

		/*CMNG_NEW_KDCDDC *pDlg = NULL;
		pDlg = new CMNG_NEW_KDCDDC(m_pWindow);
		pDlg->OpenMode = 0;
		

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
		}*/
	}
	if (strSM == _T("주기표_수정"))
	{
		CMNG_NEW_KDCDDC Dlg(m_pWindow);
		Dlg.OpenMode = 1;

		Dlg.DoModal();

	}
	if (strSM == _T("주기표_삭제"))
	{
		CMNG_NEW_KDCDDC Dlg(m_pWindow);
		Dlg.DELETE_CODE();
	}
	if (strSM == _T("조기표_검색"))
	{
		CMNG_KDCDDC_SEARCH *pDlg = NULL;
		pDlg = new CMNG_KDCDDC_SEARCH(m_pWindow);
		pDlg->OpenMode = 1;

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
	if (strSM == _T("조기표_입력"))
	{
		CMNG_NEW_HELP Dlg(m_pWindow);
		Dlg.OpenMode = 0;

		Dlg.DoModal();

		/*CMNG_NEW_HELP *pDlg = NULL;
		pDlg = new CMNG_NEW_HELP(m_pWindow);
		pDlg->OpenMode = 0;
		

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
		}*/
	}
	if (strSM == _T("조기표_수정"))
	{
		CMNG_NEW_HELP Dlg(m_pWindow);
		Dlg.OpenMode = 1;

		Dlg.DoModal();

	}
	if (strSM == _T("조기표_삭제"))
	{
		CMNG_NEW_HELP Dlg(m_pWindow);
		Dlg.DELETE_CODE();
	}
	if (strSM == _T("연동색인어_검색"))
	{
		CMNG_KDCDDC_SEARCH *pDlg = NULL;
		pDlg = new CMNG_KDCDDC_SEARCH(m_pWindow);
		pDlg->OpenMode = 2;

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
	if (strSM == _T("연동색인어_입력"))
	{
		CMNG_NEW_INDEX Dlg(m_pWindow);
		Dlg.OpenMode = 0;

		Dlg.DoModal();

		/*CMNG_NEW_INDEX *pDlg = NULL;
		pDlg = new CMNG_NEW_INDEX(m_pWindow);
		pDlg->OpenMode = 0;
		

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
		}*/
	}
	if (strSM == _T("연동색인어_수정"))
	{
		CMNG_NEW_INDEX Dlg(m_pWindow);
		Dlg.OpenMode = 1;

		Dlg.DoModal();

	}
	if (strSM == _T("연동색인어_삭제"))
	{
		CMNG_NEW_INDEX Dlg(m_pWindow);
		Dlg.DELETE_CODE();
	}
	if (strSM == _T("주제명_검색"))
	{
		CMNG_KDCDDC_SEARCH *pDlg = NULL;
		pDlg = new CMNG_KDCDDC_SEARCH(m_pWindow);
		pDlg->OpenMode = 3;

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
	if (strSM == _T("주제명_입력"))
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.07 김소정 수정

		CMNG_NEW_KEYWORD Dlg(m_pWindow);
		Dlg.OpenMode = 0;

		Dlg.DoModal();

		/*CMNG_NEW_KEYWORD *pDlg = NULL;
		pDlg = new CMNG_NEW_KEYWORD(m_pWindow);
		pDlg->OpenMode = 0;
		

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
		}*/

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
	if (strSM == _T("주제명_수정"))
	{
		CMNG_NEW_KEYWORD Dlg(m_pWindow);
		Dlg.OpenMode = 1;
		Dlg.DoModal();

	}
	if (strSM == _T("주제명_삭제"))
	{
		CMNG_NEW_KEYWORD Dlg(m_pWindow);
		Dlg.DELETE_CODE();
	}

	return NULL;
}

