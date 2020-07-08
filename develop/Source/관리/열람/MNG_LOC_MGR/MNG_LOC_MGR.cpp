// MNG_LOC_MGR.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "MngLocMgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE MNG_LOC_MGRDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MNG_LOC_MGR.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(MNG_LOC_MGRDLL, hInstance))
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

		new CDynLinkLibrary(MNG_LOC_MGRDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MNG_LOC_MGR.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MNG_LOC_MGRDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CMngLocMgrDlg* pWindow = NULL;
	pWindow = new CMngLocMgrDlg(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly , CWnd *pWindow){
	if (!pWindow) return NULL;
	
	CMngLocMgrDlg *pTmpWindow = (CMngLocMgrDlg*)pWindow;
	
	if( strSM.Compare(_T("수정"))==0 )
	{
		AfxGetApp()->DoWaitCursor(1);
		pTmpWindow->SaveClientDlg();
		AfxGetApp()->DoWaitCursor(-1);
	}
	if( strSM.Compare(_T("이전내용"))==0 )
	{
		pTmpWindow->ViewClientDlg(-1);
	}
	// 자료 찾기
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){

	CMngLocMgrDlg *pTmpWindow = (CMngLocMgrDlg*)pWindow;
	if(pWindow){
		if( pTmpWindow->m_pMngLocMgr01 != NULL ) delete pTmpWindow->m_pMngLocMgr01;	
		if( pTmpWindow->m_pMngLocMgr02 != NULL ) delete pTmpWindow->m_pMngLocMgr02;
		if( pTmpWindow->m_pMngLocMgr03 != NULL ) delete pTmpWindow->m_pMngLocMgr03;
		if( pTmpWindow->m_pMngLocMgr04 != NULL ) delete pTmpWindow->m_pMngLocMgr04;
		if( pTmpWindow->m_pMngLocMgr05 != NULL ) delete pTmpWindow->m_pMngLocMgr05;
		if( pTmpWindow->m_pMngLocMgr06 != NULL ) delete pTmpWindow->m_pMngLocMgr06;
		if( pTmpWindow->m_pMngLocMgr07 != NULL ) delete pTmpWindow->m_pMngLocMgr07;
		if( pTmpWindow->m_pMngLocMgr08 != NULL ) delete pTmpWindow->m_pMngLocMgr08;
		// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정 메뉴 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
		if( pTmpWindow->m_pMngLocMgr09 != NULL ) delete pTmpWindow->m_pMngLocMgr09;
		if( pTmpWindow->m_pMngLocMgr10 != NULL ) delete pTmpWindow->m_pMngLocMgr10;
//*/ END ---------------------------------------------------------------------------
		delete pWindow;
	}
}
