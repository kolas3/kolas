// BO_ACQ_REG.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_REG_MAIN.h"
#include "BO_CAT_SEARCH.h"
#include "BO_CAT_REG_0410.h"
#include "BO_CAT_REG_0420.h"

#include "../../정리/BO_CAT_REG/BO_CAT_REG_0400.h"
#include "../../정리/BO_CAT_REG/BO_CAT_SEARCH_MGR_PRE.h"
#include "../../정리/BO_CAT_REG/BO_REG_CODE.h"
#include "../../정리/BO_CAT_REG/BO_CAT_GROUP_INSERT.h"
#include "../../정리/BO_CAT_REG/BO_CAT_REG_0500.h"
#include "../../정리/BO_CAT_REG/BO_CAT_PROC_1100.h"
#include "../../정리/BO_CAT_REG/MultiSortDlg.h"
#include "../../정리/BO_CAT_REG/BO_CAT_REG_RFID.h"

#include "..\..\..\SmartID\BarCodeToRFID2\SmartIDScannerDlg.h"
#include "..\..\..\SmartID\BarCodeToRFID2\BarcodeToRFIDDlg.h"

static AFX_EXTENSION_MODULE BO_ACQ_REGDLL = { NULL, NULL };

static INT OpenMode;

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_ACQ_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_ACQ_REGDLL, hInstance))
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

		new CDynLinkLibrary(BO_ACQ_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_ACQ_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_ACQ_REGDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{

	g_strMenuAlias = strMenuAlias;

	//등록자료관리 
	if ( strMenuAlias == _T("단행_등록_등록자료관리") )
	{
		CBO_CAT_REG_MAIN*	m_pWindow = new CBO_CAT_REG_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  0;

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
	CBO_CAT_REG_MAIN* m_pWindow = (CBO_CAT_REG_MAIN*)pWindow;

	if (strSM == _T("검색"))
	{
		CBO_CAT_SEARCH *pDlg = NULL;
		pDlg = new CBO_CAT_SEARCH(m_pWindow);
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
	
	else if (strSM == _T("등록자료입력"))
	{
		m_pWindow->InputBook();
	}
	else if (strSM == _T("등록자료수정"))
	{
		m_pWindow->ModifyBook();
	}
	else if (strSM == _T("등록자료삭제"))
	{
		m_pWindow->DeleteBook();
	}
	else if (strSM == _T("번호부여"))
	{
		m_pWindow->CreateRegNO();
	}
	
	else if (strSM == _T("번호취소"))
	{
//		INT Type = 0;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
		pDlg->OperationType = 0; 
		pDlg->DoModal();
		return NULL;
/*
		pDlg->CancelRegNo(); 
		CBO_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow) && pDlg->m_bInitDlg)
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
*/
	}

// '등록인계취소' 버튼 추가 [2003/10/17/박현석/]
	else if (strSM == _T("등록인계취소"))
	{
//		INT Type = 1;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
		pDlg->OperationType = 1; 
		pDlg->DoModal();
//		pDlg->CancelRegNo(); 
	}

// '검수취소' 버튼 추가 [2003/09/30/박현석] 
	else if (strSM == _T("검수취소"))
	{
//		INT Type = 1;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
//		pDlg->CancelType =1;
		pDlg->OperationType = 2; 
		pDlg->DoModal();
/*		
		pDlg->CancelRegNo();
		CBO_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
		pDlg->CancelType =1; 
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow) && pDlg->m_bInitDlg)
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
*/
	}

	else if (strSM == _T("등록구분변경"))
	{
/*		
		CBO_REG_CODE Dlg(m_pWindow);
		Dlg.DoModal();
*/	
//정종석추가===================================================================		
		CString strDMAlias;
		CString strCMAlias;
		CString strGridAlias;
		if (g_strMenuAlias == _T("단행_등록_등록자료관리"))
		{
			strDMAlias = _T("DM_단행_정리_등록_메인");
			strCMAlias = _T("CM_단행_정리_등록_메인");
			strGridAlias = _T("MAIN_GRID");
		}
		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;
		CESL_ControlMgr * pCM = static_cast<CESL_Mgr*>(pWindow)->FindCM(strCMAlias);
		if (pCM == NULL) return NULL;
		CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(strGridAlias);
		m_pWindow->CodeChange(pCM, pDM, pGrid);
//정종석추가===================================================================
	}

	else if (strSM == _T("가원부작성"))
	{
		CBO_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 0;
		Dlg.DoModal();
	}

	else if (strSM == _T("복본대상처리"))
	{
		m_pWindow->DoFindCopy();
	}
	else if (strSM == _T("분류대상처리"))
	{
		m_pWindow->DoClassify();
	}
	else if (strSM == _T("정리대상처리"))
	{
		m_pWindow->DoCatalog();
	}
	else if (strSM == _T("자료반입"))
	{
		m_pWindow->FileImport();
	}
	
	else if (strSM == _T("자료반출"))
	{
		CBO_CAT_PROC_1100 *pDlg = new CBO_CAT_PROC_1100(m_pWindow);
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}

	else if (strSM == _T("등록출력"))
	{
		CESL_DataMgr *pDM = m_pWindow->FindDM(_T("DM_단행_정리_등록_메인"));
		if (pDM == NULL) return NULL;

		CSIReportManager SIReportManager(m_pWindow);
		SIReportManager.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		INT ids = SIReportManager.GetSIReportFileFromDB(_T("등록_검색결과_단행"));
		if (ids < 0) 
		{
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return NULL;
		}

		SIReportManager.SetDataMgr(0, pDM, 0);
		ids = SIReportManager.Print();
	}

	else if (strSM == _T("정렬"))
	{
//정종석추가===================================================================
		CString strDMAlias;
		CString strCMAlias;
		if (g_strMenuAlias == _T("단행_등록_등록자료관리"))
		{
			strDMAlias = _T("DM_단행_정리_등록_메인");
			strCMAlias = _T("CM_단행_정리_등록_메인");
		}
		else
		{
			strDMAlias = _T("DM_단행_정리_가원부_메인");
			strCMAlias = _T("CM_단행_정리_가원부_메인");
		}
		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;
		CESL_ControlMgr * pCM = static_cast<CESL_Mgr*>(pWindow)->FindCM(strCMAlias);
		if (pCM == NULL) return NULL;
		m_pWindow->RegDataOrder(pCM, pDM);
//정종석추가===================================================================
		
/*
		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;

		CMultiSortDlg dlg(pWindow);
		dlg.AddSortInfo(_T("BS_주제구분"), _T("주제구분"));
		dlg.AddSortInfo(_T("IBS_본표제"), _T("본표제"));
		dlg.AddSortInfo(_T("IBS_저작자"), _T("저자"));
		dlg.AddSortInfo(_T("IBS_발행자"), _T("발행자"));
		dlg.AddSortInfo(_T("BB_관리번호"), _T("관리번호"));
		if (dlg.DoModal() == IDOK)
		{
			pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

			static_cast<CESL_Mgr*>(pWindow)->AllControlDisplay(strCMAlias);
		}
*/		
	}
	
	else if (strSM == _T("RFID"))
	{
		m_pWindow->GetRegnoTitle();
		//CBO_CAT_REG_RFID RfidRegno;
		//RfidRegno.GetRegnoTitle();
		//CSmartIDScannerDlg dlg;
		//INT ids = dlg.DoModal();
/*
		CBarcodeToRFIDDlg dlg;
		dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
		CRFIDBookInfo *pBook;

		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030180");
			pBook->strTitle = _T("서울1987");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030181");
			pBook->strTitle = _T("모나리자");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030182");
			pBook->strTitle = _T("꿈");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030183");
			pBook->strTitle = _T("이젠 그랬으면 좋겠네");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030184");
			pBook->strTitle = _T("사랑하는 그대에게");
		}

		dlg.DoModal();
		*/
	}
	return NULL;
}

