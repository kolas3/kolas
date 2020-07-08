// FEE_HISTORY_RECEIPT.cpp : Defines the initialization routines for the DLL.
// MAKE BY PWR

#include "stdafx.h"
#include <afxdllx.h>
#include "FeeHistoryReceiptDlg.h"
#include "FeeHistorySearchDlg.h"
#include "FeeHistoryReceiptPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE FEE_HISTORY_RECEIPTDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FEE_HISTORY_RECEIPT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(FEE_HISTORY_RECEIPTDLL, hInstance))
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

		new CDynLinkLibrary(FEE_HISTORY_RECEIPTDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FEE_HISTORY_RECEIPT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(FEE_HISTORY_RECEIPTDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias )
{
	CFeeHistoryReceiptDlg* pWindow = NULL;
	pWindow = new CFeeHistoryReceiptDlg(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;

	CFeeHistoryReceiptDlg *pTmpWindow = (CFeeHistoryReceiptDlg*)pWindow;

	//////////////////////////////////////////////////////////////////////////////////////////////
	if ( strSM == _T("검색") )
	{
		CFeeHistorySearchDlg *pDlg = new CFeeHistorySearchDlg ( pTmpWindow );
		pDlg->pMain = pTmpWindow->pMain;
		
		if( bReadOnly )
			pDlg ->SM_STYLE = 'R';
		
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		
		if( pDlg->Create( pTmpWindow ) )
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
	//////////////////////////////////////////////////////////////////////////////////////////////
	else if ( strSM == _T("목록출력") )
	{
		pTmpWindow->PrintList();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	else if ( strSM == _T("영수증재발급") )
	{
		CFeeHistoryReceiptPreview *pDlg = NULL;
		pDlg = new CFeeHistoryReceiptPreview( pTmpWindow );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;

		// 1. 출력할 내역이 있는지 확인한다.
		if( pDlg->CheckParentDMCount() < 1 )
		{
			//없으면 무시
			delete pDlg;
			return NULL;
		}
		// 2. 선정된 내역이 있는지 확인한다.
		if( pDlg->CheckSelectCount() < 1 )
		{
			//없으면 메시지
			AfxMessageBox( _T("선정된 내역이 없습니다.") );
			delete pDlg;
			return NULL;
		}
		// 3. 영수증재출력 화면을 보여준다.
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	else if( strSM == _T("수정") )
	{
		pTmpWindow->PaymentEdit();
	}
	else if( strSM == _T("수정이력") )
	{
		pTmpWindow->PaymentEditHistory();
	}
	else if( strSM == _T("입력") )
	{
		pTmpWindow->FeeInsertProc();
	}

	return NULL;
}	

__declspec(dllexport) void ResizeForm(CRect rect, CWnd *pWindow)
{
	if(pWindow)
	{
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) void DeleteForm(CWnd *pWindow)
{
	CFeeHistoryReceiptDlg *pTmpWindow = (CFeeHistoryReceiptDlg*)pWindow;
	if(pTmpWindow)
	{
		delete pTmpWindow;
	}
}