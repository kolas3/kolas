// ContentsMgr_SE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SE_CONTENTS_01.h"
#include "SE_CONTENTS_EDIT.h"
#include "SE_CONTENTS_SEARCH.h"
#include "CONTENTMGR_SE_INDEX_SEARCH.h"
#include "..\..\목차초록관리\ContentsMgr_Editor\CONTENTMGR_SE_KISA_EDIT.h"


static AFX_EXTENSION_MODULE ContentsMgr_SEDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CONTENTSMGR_SE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ContentsMgr_SEDLL, hInstance))
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

		new CDynLinkLibrary(ContentsMgr_SEDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CONTENTSMGR_SE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ContentsMgr_SEDLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 

{
	CSE_CONTENTS_01* m_pWindow = new CSE_CONTENTS_01(NULL);   
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow ;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	CSE_CONTENTS_01* m_pWindow = (CSE_CONTENTS_01*)pWindow;

	if (strSM == _T("검 색"))
	{  
      
       
		CSE_CONTENTS_SEARCH *pDlg = NULL;
		pDlg = new CSE_CONTENTS_SEARCH(m_pWindow);
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
	else if (strSM == _T("기사검색"))
	{  
      
       
		CCONTENTMGR_SE_INDEX_SEARCH *pDlg = NULL;
		pDlg = new CCONTENTMGR_SE_INDEX_SEARCH(m_pWindow);
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
    else if (strSM == _T("목차편집"))
	{ 
		CSE_CONTENTS_EDIT  dlg(m_pWindow );
		dlg.DoModal () ;
	}
    else if (strSM == _T("기사목차편집"))
	{   
		if(m_pWindow->m_TabCtrl.GetCurSel () == 1)
		{
		   CCONTENTMGR_SE_KISA_EDIT  dlg(m_pWindow );
		   dlg.GetDialogType (TRUE); // 메뉴버튼에서 생성
		   dlg.Set_Mgr (_T("CM_CONTENTMGR_SE_MAIN"),_T("DM_CONTENTMGR_SE_INDEX_TBL"),_T("MainGrid_Kisa"));
		   dlg.DoModal () ;
		}
	}


	else if (strSM == _T("목차보기"))
	{ 
		m_pWindow->OnViewMoc () ;

	}
	else if(strSM==_T("기사보기"))
	{	
	    m_pWindow->OnViewKisa () ;
	}
	else if(strSM==_T("출 력"))
	{	
		m_pWindow->Print_List() ;
	}
	else if(strSM==_T("기사마크반입"))
	{	
		m_pWindow->KisaMarcImport() ;
	}
	else if(strSM==_T("기사마크반출"))
	{	
		m_pWindow->KisaMarcExport() ;
	}
	else if(strSM==_T("기사마크일괄생성"))
	{	
		m_pWindow->MakeKisaMarc() ;
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
//__declspec(dllexport) VOID DeleteForm() 
{
	if (pWindow) 
	{
		delete pWindow;
	}
}
