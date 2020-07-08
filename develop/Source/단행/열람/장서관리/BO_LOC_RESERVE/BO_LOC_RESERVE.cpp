// BO_LOC_RESERVE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BoLocReserveDlg.h"
#include "ReserveSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_RESERVEDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_RESERVE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_RESERVEDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(BO_LOC_RESERVEDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_RESERVE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_RESERVEDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBoLocReserveDlg* pWindow = NULL;
	pWindow = new CBoLocReserveDlg(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly , CWnd *pWindow){
	if(!pWindow) return NULL;
	CBoLocReserveDlg *pTmpWindow = (CBoLocReserveDlg*)pWindow;
	if(strSM == _T("검색"))
	{
		CReserveSearchDlg dlg(pTmpWindow);
		dlg.m_pLocProc = pTmpWindow->m_pLocProc;
		dlg.m_strCivilNoHideYN = pTmpWindow->m_strCivilNoHideYN;
		dlg.m_OneLineReserve = pTmpWindow->m_pLoanMangeValue->m_bVolReserve;
		dlg.DoModal();
		pTmpWindow->m_strWhere = dlg.m_strWhere;
	}
	else if (strSM == _T("예약상태"))	
	{
		CString sValue = _T("");
		pTmpWindow->m_pLocProc->m_pCheckLoan->GetReserveMode(sValue);
		if(_T("0") == sValue)
		{
			CString sMsg;
			sMsg.Format( _T("예약 기능을 사용후에 시도하십시오.") );
			pTmpWindow->m_pLocProc->SelfCloseMessageBox(sMsg, pTmpWindow, 2000);
			return NULL;
		}
		pTmpWindow->ReservationStatus();
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
