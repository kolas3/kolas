// BO_LOC_3870.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_LOC_38701.h"
#include "BO_LOC_3871.h"
#include "BO_LOC_3872.h"
#include "BO_LOC_3873.h"
#include "BO_LOC_3874.h"
#include "BO_LOC_3875.h"

static AFX_EXTENSION_MODULE BO_LOC_3870DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3870.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_3870DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_3870DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3870.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_3870DLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_3870* pWindow = NULL;
	pWindow = new CBO_LOC_3870(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow){
	if (!pWindow) return NULL;

	CBO_LOC_3870* m_pWindow = (CBO_LOC_3870*)pWindow;

	if( strSM == _T("검색") ) 
	{
		CBO_LOC_3872 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if( strSM == _T("입력") ) 
	{
		CBO_LOC_3871 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if( strSM == _T("수정") ) 
	{
		CBO_LOC_3873 Dlg(m_pWindow);
		Dlg.SetDlgType( __ECO_MODIFY_TOOLBAR_MODE , _T("CM_BO_LOC_3870") , _T("비회원메인그리드") , _T("DM_BO_LOC_3870") );
		Dlg.DoModal();
	}
	else if( strSM == _T("삭제") ) 
	{
		AfxGetApp()->DoWaitCursor(1);

		m_pWindow->DeleteProc();

		AfxGetApp()->DoWaitCursor(-1);
	}
	else if( strSM == _T("파일반입") ) 
	{
		CBO_LOC_3874 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if( strSM == _T("그룹지정") ) 
	{
		CBO_LOC_3875 Dlg(m_pWindow);
		Dlg.DoModal();
	}

	return NULL;
}



__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	
	delete pWindow;
}
