// BO_ACQ_PURCHASE_DATA_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_ACQ_PurchaseDataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_ACQ_PURCHASE_DATA_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_ACQ_PURCHASE_DATA_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_ACQ_PURCHASE_DATA_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(BO_ACQ_PURCHASE_DATA_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_ACQ_PURCHASE_DATA_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_ACQ_PURCHASE_DATA_MANAGERDLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CBO_ACQ_PurchaseDataMgr * m_pWindow = new CBO_ACQ_PurchaseDataMgr(NULL);
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
	CBO_ACQ_PurchaseDataMgr * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CBO_ACQ_PurchaseDataMgr*)pWindow;

	if(strButtonAlias == _T("검색"))
	{
		m_pWindow->ShowSearchDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("출력"))
	{
		m_pWindow->ShowPrintDlg();
		return NULL;		
	}
	else if(strButtonAlias == _T("입력"))
	{
		m_pWindow->ShowPurchaseDataDlg(SINSERT);
		return NULL;
	}
	else if(strButtonAlias == _T("수정"))
	{
		m_pWindow->ShowPurchaseDataDlg(SUPDATE);
		return NULL;
	}
	else if(strButtonAlias == _T("삭제"))
	{
		m_pWindow->DeletePurchaseData();
		return NULL;
	}
	else if(strButtonAlias == _T("심의처리"))
	{
		m_pWindow->ReviewPurchaseData();
		return NULL;
	}
	else if(strButtonAlias == _T("주문처리"))
	{
		m_pWindow->OrderPurcahseData();
		return NULL;
	}
	else if(strButtonAlias == _T("코드변경"))
	{
		m_pWindow->ShowChangeCodeDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("구입정보"))
	{
		m_pWindow->ShowChangePurchaseInfoDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("차수관리"))
	{
		m_pWindow->ShowDivNoMgrDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("가격합산"))
	{
		m_pWindow->ShowPriceSumDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("중복조사"))
	{
		m_pWindow->CopyPurchaseDataSearch();
		return NULL;
	}
	else if(strButtonAlias == _T("복본조사"))
	{
		m_pWindow->DupPurchaseDataSearch();
		return NULL;
	}
	else if(strButtonAlias == _T("주문번호"))
	{
		m_pWindow->GiveOrderSerialNo();
		return NULL;
	}
	else if(strButtonAlias == _T("정렬"))
	{
		m_pWindow->ShowSortDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("자료반입"))
	{
		m_pWindow->ShowDataPurchaseImport();
		return NULL;
	}
	else if(strButtonAlias == _T("키설정"))
	{
		m_pWindow->DupKeySetting();
		return NULL;
	}
// 	//JOB.2019.0073 : 충돌 License 제거
// 	else if(strButtonAlias == _T("주문취소메일"))
// 	{
// 		m_pWindow->ShowOrderCancelMailMgr(-1000, -1000, NULL);
// 		return NULL;
// 	}
	else if(strButtonAlias == _T("차수변경"))
	{
		m_pWindow->ChangeGroupInfo();
	}
	// 2005-01-31 수서반출/반입 추가
	else if(strButtonAlias == _T("수서반출"))
	{
		m_pWindow->ShowACQFileExport();
	}
	else if(strButtonAlias == _T("수서반입"))
	{
		m_pWindow->ShowACQFileImport();
	}
	// 2005-02-12 자료반출/파일반입 추가
	else if(strButtonAlias == _T("자료반출"))
	{
		m_pWindow->ShowFileExport();
	}
	else if(strButtonAlias == _T("파일반입"))
	{
		m_pWindow->ShowFileImport();
	}	
	else if(strButtonAlias == _T("선정처리"))
	{
		m_pWindow->ProcSelect();
	}
	else if(strButtonAlias == _T("비선정처리"))
	{
		m_pWindow->ProcUnSelect();
	}
	else if(strButtonAlias == _T("마크교체"))
	{
		m_pWindow->ChangeMarc();
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

__declspec(dllexport) void KalendarSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo, CWnd *pWindow)
{
	CBO_ACQ_PurchaseDataMgr * m_pWindow = NULL;
	m_pWindow = (CBO_ACQ_PurchaseDataMgr*)pWindow;

	m_pWindow->KSearch(WorkType, strAcqCode, strAcqYear, strWorkCode, strSeqNo);
}