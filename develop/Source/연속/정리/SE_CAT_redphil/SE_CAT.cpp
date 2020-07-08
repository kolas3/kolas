// BO_CAT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MainDefaultDlg.h"

// 검색
#include "SE_CAT_SearchDlg.h"

// 기본값 설정
#include "..\..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

// 데이터 일괄변경
#include "..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0801.h"

// 데이터 일괄확인
#include "..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_1300.h"

// 마크 반출
//#include _T("..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_1100.h")
#include "..\SE_CAT_MARC_EXPORT\MarcExportDlg_SE.h"

// 마크 반입
#include "..\SE_CAT_MARC_IMPORT\MarcImportDlg_SE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_CATDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_CAT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CATDLL, hInstance))
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

		new CDynLinkLibrary(BO_CATDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_CAT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CATDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMainDefaultDlg* pWindow = new CMainDefaultDlg(NULL);

	g_strMenuAlias = strMenuAlias;

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->m_strMenuAlias = strMenuAlias;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if( pWindow )
	{
		pWindow->DestroyWindow();
		delete pWindow;
	}
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if (!pWindow) return NULL;

	INT ids = -1;

	CMainDefaultDlg *pParentDlg = static_cast<CMainDefaultDlg*>(pWindow);

	// 검색
	if (strSM.Find(_T("검색")) > 0)
	{
		CSE_CAT_SearchDlg *pDlg = new CSE_CAT_SearchDlg(pParentDlg, g_strMenuAlias);
		if (pDlg->Create(pParentDlg))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
	}

	// 교열지 작성
	else if (strSM.Find(_T("교열지작성")) > 0)
	{
		pParentDlg->PRINT(_T("교열지"));
	}

	// 공용사항관리
	else if (strSM.Find(_T("공용사항관리")) > 0)
	{
		CMarcDefaultValueConfigDlg dlg(pParentDlg);
		dlg.SetMarcGroupFormCode(_T("TX"), _T("연속"));
		dlg.DoModal();
	}

	// 복본조사
	else if (strSM == _T("복본조사_복본조사"))
	{
		pParentDlg->DUP_Search();
	}
	else if (strSM == _T("복본조사_복본해제"))
	{
		pParentDlg->DUP_Free();
	}
	else if (strSM == _T("복본조사_키설정"))
	{
		pParentDlg->DUP_KeySetting();
	}

	// 목록완성
	else if (strSM == _T("목록완성_입력"))
	{
		pParentDlg->MarcAdjust_Insert();
	}
	else if (strSM == _T("목록완성_수정"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("목록완성_데이터일괄변경"))
	{
		CBO_CAT_PROC_0801 *pDlg = new CBO_CAT_PROC_0801(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		if (pDlg->Create(pParentDlg) && pDlg->m_bInitDlg)
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
	else if (strSM == _T("목록완성_자료일괄확인"))
	{
		CBO_CAT_PROC_1300 *pDlg = new CBO_CAT_PROC_1300(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		if (pDlg->Create(pParentDlg) && pDlg->m_bInitDlg)
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
	else if (strSM == _T("목록완성_마크반입"))
	{
		CMarcImportDlg_SE dlg(pParentDlg);
		dlg.SetOpenMode(4000);
		dlg.DoModal();
	}
	else if (strSM == _T("목록완성_마크반출"))
	{
		//CBO_CAT_PROC_1100 *pDlg = new CBO_CAT_PROC_1100(pParentDlg);
		CMarcExportDlg_SE *pDlg = new CMarcExportDlg_SE(pParentDlg);
		if (pDlg->Create(pParentDlg))
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


	// 목록편성
	else if (strSM == _T("목록편성_장비출력"))
	{
		pParentDlg->PRINT(_T("장비"));
	}
	else if (strSM == _T("목록편성_카드목록"))
	{
		pParentDlg->PRINT(_T("카드목록"));
	}
	else if (strSM == _T("목록편성_원부류별통계"))
	{
		pParentDlg->PRINT(_T("원부류"));
	}

	// 열람인계
	else if (strSM == _T("열람인계처리_출력"))
	{
		pParentDlg->PRINT(_T("열람인계목록"));
	}

	// 상태 이동
	else if (strSM == _T("복본조사_복본조사완료_정리대상"))
	{
		ids = pParentDlg->Complete_Dup();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("복본조사_복본조사완료_분류대상"))
	{
		ids = pParentDlg->Complete_Dup2();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("분류_분류완료"))
	{
		ids = pParentDlg->Complete_Classification();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("목록완성_정리완료_열람인계대상"))
	{
		ids = pParentDlg->Complete_Cataloging();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("정리 완료 처리하였습니다."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("목록완성_정리완료_열람인계자료"))
	{
		ids = pParentDlg->Complete_Cataloging();
		if (ids < 0) return NULL;
		ids = pParentDlg->Complete_Loc();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("정리 완료 처리하였습니다."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("목록완성_정리완료_배가자료"))
	{
		ids = pParentDlg->Complete_Cataloging(FALSE);
		if (ids < 0) return NULL;
		ids = pParentDlg->Complete_Loc2();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("정리 완료 처리하였습니다."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("열람인계처리_열람인계자료"))
	{
		ids = pParentDlg->Complete_Loc();
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("열람인계 처리하였습니다."));
		}
	}
	else if (strSM == _T("열람인계처리_배가자료"))
	{
		ids = pParentDlg->Complete_Loc2();
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("열람인계 처리하였습니다."));
		}
	}

	// 배가일자 기록
	else if (strSM == _T("배가관리_배가일자기록"))
	{
		pParentDlg->Loc_Record();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	
	// 040319_HSS 종삭제 추가
	if(strButtonAlias == _T("삭제"))
	{
		pParentDlg->DeleteSpecies();
	}

	return NULL;
}