// SE_CAT_REG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE SE_CAT_REGDLL = { NULL, NULL };
static INT OpenMode;

#include "SE_CAT_SEARCH_MGR.h"
#include "SE_CAT_SEARCH_MGR_GROUP.h"
#include "SE_CAT_REG_MAIN.h"
#include "SE_CAT_PREGROUP_MAIN.h"
#include "SE_CAT_GROUP_MAIN.h"
#include "SE_CAT_CODE_CHANGE.h"
#include "SE_CAT_REG_0400.h"
#include "..\..\..\단행\정리\BO_CAT_REG\BO_CAT_REG_0500.h"
#include "SE_CAT_GROUP_INSERT.h"
#include "SE_preGROUP_MNG.h"
#include "SE_CAT_SEARCH_MGR_PRE.h"
#include "SE_CAT_REG_VIEW.h"
#include "BO_CAT_REG_0800.h"
#include "MultiSortDlg.h"

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_CAT_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_CAT_REGDLL, hInstance))
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

		new CDynLinkLibrary(SE_CAT_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_CAT_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_CAT_REGDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{
	g_strMenuAlias = strMenuAlias;

	//등록자료관리 
	if ( strMenuAlias == _T("2410") )
	{
		CSE_CAT_REG_MAIN*	m_pWindow = new CSE_CAT_REG_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  0;
		return m_pWindow;
	}

	//가원부관리
	/*
	if ( strMenuAlias == _T("2420") )
	{
		CSE_CAT_PREGROUP_MAIN*	m_pWindow = new CSE_CAT_PREGROUP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  1;
		return m_pWindow;
	}
	*/

	//원부대장관리
	if ( strMenuAlias == _T("2420") )
	{
		CSE_CAT_GROUP_MAIN*	m_pWindow = new CSE_CAT_GROUP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  2;
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
	CSE_CAT_REG_MAIN* m_pWindow = (CSE_CAT_REG_MAIN*)pWindow;

	if (strSM == _T("검색"))
	{
		CSE_CAT_SEARCH_MGR *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR(m_pWindow);
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
	if (strSM == _T("연속_가원부자료검색"))
	{
		CSE_CAT_SEARCH_MGR_PRE *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR_PRE(m_pWindow);

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
	else if (strSM == _T("연속_원부대장관리_검색"))
	{
		CSE_CAT_SEARCH_MGR_GROUP *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR_GROUP(m_pWindow);

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
	else if (strSM == _T("번호부여"))
	{
		m_pWindow->CreateRegNO();
	}
	else if (strSM == _T("등록구분변경"))
	{
		CSE_CAT_CODE_CHANGE Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if (strSM == _T("번호취소"))
	{
		// 040309_HSS _tmain 함수 이용
		m_pWindow->DeleteRegNo();
		/*
		CSE_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CSE_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
		pDlg->CancelRegNo(); 
		*/
	}
	// 040323_HSS 책삭제 추가
	else if(strSM == _T("자료삭제"))
	{
		m_pWindow->DeleteBook();
	}
	else if (strSM == _T("가원부작성"))
	{
		
		CSE_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 0;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("연속_가원부자료추가"))
	{
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;		
		CSE_CAT_GROUP_INSERT Dlg(preWindow);
		Dlg.OpenMode = 1;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("정리대상처리"))
	{
		
		m_pWindow->DoCatalog();
			
	}
	else if (strSM == _T("연속_가원부일괄등록"))
	{
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;
		preWindow->GiveAllRegNo();
	}
	else if (strSM == _T("연속_가원부자료복귀"))
	{
		
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;		
		preWindow->BackToRegData();
		
	}
	else if (strSM == _T("연속_가원부관리"))
	{
		
		CSE_preGROUP_MNG Dlg(m_pWindow);
		Dlg.DoModal();
		
	}
	else if (strSM == _T("연속_원부자료추가"))
	{
		
		CSE_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 2;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("연속_자료복귀"))
	{
		CSE_CAT_GROUP_MAIN*	preWindow = (CSE_CAT_GROUP_MAIN*)pWindow;		
		
		CSE_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CSE_CAT_REG_0400(preWindow);
		pDlg->OpenMode =2;
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
	}
	else if (strSM == _T("연속_원부관리"))
	{
		CSE_CAT_GROUP_MAIN*	preWindow = (CSE_CAT_GROUP_MAIN*)pWindow;
		
		CBO_CAT_REG_0500 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0500(preWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)preWindow;
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
	else if (strSM == _T("자료수정"))
	{
		
		CSE_CAT_REG_VIEW Dlg(m_pWindow);
		Dlg.DoModal();
		
	}
	else if (strSM == _T("연속_원부출력"))
	{
		CBO_CAT_REG_0800 Dlg(m_pWindow);
		Dlg.DoModal();

	}
	else if (strSM == _T("연속실시간처리"))
	{
		
		m_pWindow->DoRealTimeShelf();
			
	}
	else if (strSM == _T("연속제본처리"))
	{
		
		m_pWindow->DoBinding();
			
	}
	else if (strSM == _T("등록출력"))
	{
		CESL_DataMgr *pDM = m_pWindow->FindDM(_T("DM_연속_정리_등록_메인"));
		if (pDM == NULL) return NULL;

		CSIReportManager SIReportManager(m_pWindow);
		SIReportManager.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		INT ids = SIReportManager.GetSIReportFileFromDB(_T("등록_검색결과_연속"));
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return NULL;
		}

		SIReportManager.SetDataMgr(0, pDM, 0);
		
		ids = SIReportManager.Print();
	}
	else if (strSM == _T("정렬"))
	{

		m_pWindow->DoSort();

		/*
		CString strDMAlias;
		CString strCMAlias;
		if (g_strMenuAlias == _T("연속_등록_등록자료관리"))
		{
			strDMAlias = _T("DM_연속_정리_등록_메인");
			strCMAlias = _T("CM_연속_정리_등록_메인");
		}
		else
		{
			strDMAlias = _T("DM_연속_정리_가원부_메인");
			strCMAlias = _T("CM_연속_정리_가원부_메인");
		}

		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;

		CMultiSortDlg dlg(pWindow);
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

	return NULL;
}

