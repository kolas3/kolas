// BO_LOC_2100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_21001.h"
#include "BO_LOC_2110.h"
#include "BO_LOC_2120.h"
#include "BO_LOC_2130.h"
#include "BO_LOC_2140.h"
#include "BO_LOC_2150.h"
#include "BO_LOC_2160.h"
#include "BO_LOC_2170.h"
#include "BO_LOC_2180.h"
#include "BO_LOC_2190.h"
#include "..\BL_LOC_2100\BLBookCheckProc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_2100DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_2100.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_2100DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_2100DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_2100.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_2100DLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias ){
	CBO_LOC_2100* pWindow = NULL;
	pWindow = new CBO_LOC_2100(strMenuAlias,NULL);
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
	
	CBO_LOC_2100 *pTmpWindow = (CBO_LOC_2100*)pWindow;

	if (strSM == _T("BO_LOC_2110"))
	{
		CBO_LOC_2110 *pDlg = NULL;
		pDlg = new CBO_LOC_2110(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_2120"))	{		
		INT ids = MessageBox( pTmpWindow->m_hWnd , _T("비소장자료(입력되지 않은 등록번호)를 검색합니다.") , _T("비소장검색") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES )
		{
			CBLBookCheckProc SpfWork( pTmpWindow );
			SpfWork.SPFExcutePath(SEARCH_BOOK_CHECK_N);

			return NULL;
		}
	}
	else if (strSM == _T("BO_LOC_2130"))	{
		CBO_LOC_2130 *pDlg = NULL;
		pDlg = new CBO_LOC_2130(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_2140"))	{
		INT ids = pTmpWindow->IsShelfLocNoSelect();
		if( ids == 1 )
		{
			ids = MessageBox( pTmpWindow->m_hWnd ,  _T("자료실이 선택이 되어 있지 않습니다. \r\n선택하시고 진행하시겠습니까?\r\n\r\n(예:자료실선택후진행 , 아니오:무시하고진행)") 
				, _T("장서 점검") 
				, MB_YESNO | MB_ICONQUESTION );			
			if( ids == IDYES ) return NULL;
		}

		CBO_LOC_2140 Dlg;
		Dlg.pMain = pTmpWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)pTmpWindow;
		Dlg.m_pInfo = pTmpWindow->m_pInfo;
		Dlg.DoModal();

		pTmpWindow->ResultGridDisplay();
	}
	else if (strSM == _T("초기화"))	{
		CBO_LOC_2150 *pDlg = NULL;
		pDlg = new CBO_LOC_2150(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if ( strSM == _T("점검완료") )
	{		
		INT ids = MessageBox( pTmpWindow->m_hWnd , _T("점검번호를 새로 생성하겠습니까?\r\n\r\n(예:새로운 점검번호 부여,아니오:예전에 사용되었던 점검번호 선택 후 진행,취소:취소)") , _T("결과저장") , MB_YESNOCANCEL | MB_ICONQUESTION );		
		if( ids == IDCANCEL ) return NULL;
		if( ids == IDNO )
		{
			CBO_LOC_2160 NumberDlg(pTmpWindow);
			NumberDlg.m_nLoadSPF = 1;
			NumberDlg.DoModal();
		}
		else if( ids == IDYES )
		{
			pTmpWindow->m_pSpfWork->SPFExcutePath(INIT_BOOK_CHECK_NUMBER);
		}

		CBO_LOC_2170 Dlg(pTmpWindow);
		Dlg.pMain = pTmpWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = pTmpWindow->m_pInfo;

		Dlg.DoModal();
	}
	else if (strSM == _T("값변경"))	{
		CBO_LOC_2180 *pDlg = NULL;
		pDlg = new CBO_LOC_2180(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("목록출력"))	{
		pTmpWindow->PrintList();
	}
	else if (strSM == _T("통계"))	{
		CBO_LOC_2190 *pDlg = NULL;
		pDlg = new CBO_LOC_2190(pTmpWindow,pTmpWindow->m_sStrMenuName);
		pDlg->m_pAxisVal_Statics = &pTmpWindow->m_cAxisVal_Statics;
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("정렬"))	
	{
		return	pTmpWindow->ShowSortDlg();	
	}

	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	if(pWindow){
		delete pWindow;
	}
}