// Light_CENTERUPLOAD.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "center_upload_01.h"
#include "CENTER_UPLOAD_02.h"
#include "CENTER_UPLOAD_03.h"
#include "CENTER_UPLOAD_04.h"
#include "CENTER_UPLOAD_Search.h"
#include "UploadMarcExportDlg.h"
#include "StatusChange.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0300.h"
#include "CENTER_UPLOAD_PRINT.h"
#include "..\..\공동목록\check_dup_api\LOCAL_UPDATE.h"

static AFX_EXTENSION_MODULE Light_CENTERUPLOADDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("LIGHT_CENTERUPLOAD.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(Light_CENTERUPLOADDLL, hInstance))
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

		new CDynLinkLibrary(Light_CENTERUPLOADDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LIGHT_CENTERUPLOAD.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(Light_CENTERUPLOADDLL);
	}
	return 1;   // ok
}



//CCENTER_UPLOAD_01* m_pWindow=NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 

{
//	m_pWindow = NULL;
//	m_pWindow = new CCENTER_UPLOAD_01(NULL);
	CCENTER_UPLOAD_01* m_pWindow = new CCENTER_UPLOAD_01(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	CCENTER_UPLOAD_01* m_pWindow = (CCENTER_UPLOAD_01*)pWindow;
	//if (!m_pWindow) return NULL;

	if (strSM == _T("찾기"))
	{  
		//m_pWindow->OnSearch () ;
        		
		CCENTER_UPLOAD_Search *pDlg = NULL;
		pDlg = new CCENTER_UPLOAD_Search(m_pWindow);
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

	else if(strSM==_T("일괄전송"))
	{	
		CCENTER_UPLOAD_02 pDlg(m_pWindow) ;
		pDlg.DoModal () ;
	}
   

	else if(strSM==_T("건별전송"))
	{	 
       CCENTER_UPLOAD_03 pDlg(m_pWindow) ;
	   pDlg.DoModal () ;
	}

    
     
	else if(strSM==_T("자료수정"))
	{	
       m_pWindow->ShowModifyDlg () ;
	}

    
	else if(strSM==_T("교열지"))
	{

	  CCENTER_UPLOAD_PRINT pDlg(m_pWindow) ;
	  pDlg.DoModal () ;

	}

	else if(strSM==_T("마크반출"))
	{
	  CUploadMarcExportDlg pDlg(m_pWindow) ;
	  pDlg.DoModal () ;
	}

	else if(strSM==_T("코드변경"))
	{
	  CStatusChange pDlg(m_pWindow) ;
	  pDlg.DoModal () ;
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
