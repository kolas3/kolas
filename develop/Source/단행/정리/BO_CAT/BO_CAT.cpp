// BO_CAT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MainDefaultDlg.h"

#include "BO_CAT_SearchDlg.h"

#include "..\..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0400_08.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0500_09.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0801.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_1300.h"

#include "..\BO_CAT_MARC_EXPORT\MarcExportDlg.h"

#include "..\BO_CAT_MARC_IMPORT\MarcImportDlg.h"

#include "BO_ClassCodeVolumeDlg.h"

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
		TRACE0("BO_CAT.DLL Initializing!\n");
		
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
		TRACE0("BO_CAT.DLL Terminating!\n");
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
	
	if (strSM.Find(_T("검색")) > 0)
	{
		CBO_CAT_SearchDlg *pDlg = new CBO_CAT_SearchDlg(pParentDlg, g_strMenuAlias);
		if (pDlg->Create(pParentDlg))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
	}
	else if (strSM.Find(_T("교열지작성")) > 0)
	{
		pParentDlg->PRINT(_T("교열지"));
	}

	else if (strSM.Find(_T("공용사항관리")) > 0)
	{
		CMarcDefaultValueConfigDlg dlg(pParentDlg);

		CString strFormCode;
		INT nIds;
		nIds = pParentDlg->m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"),strFormCode);
		strFormCode.TrimLeft();	strFormCode.TrimRight();
		if( strFormCode.IsEmpty() || nIds < 0)
		{
			dlg.SetMarcGroupFormCode(_T("BK"), _T("통합마크"));
		}
		else
		{
			dlg.SetMarcGroupFormCode(strFormCode, _T("통합마크"));			
		}	
		dlg.DoModal();
	}

	else if (strSM == _T("복본조사_복본조사"))
	{
		pParentDlg->DUP_Search();
	}
	else if (strSM == _T("복본조사_복본해제"))
	{
		ids = pParentDlg->DUP_Free();
		if (ids == 1) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("복본조사_키설정"))
	{
		pParentDlg->DUP_KeySetting();
	}
	else if (strSM == _T("분류_수정"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("목록완성_웹반입"))
	{
		pParentDlg->WebImport();
	}
	else if (strSM == _T("목록완성_학위논문반입"))
	{
		pParentDlg->DMDPImport();
	}
	else if (strSM == _T("목록완성_입력"))
	{
		pParentDlg->MarcAdjust_Insert();
	}

	else if (strSM == _T("목록완성_수정"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("목록완성_삭제"))
	{
		pParentDlg->MarcAdjust_Delete();
	}
	else if (strSM == _T("목록완성_종통합"))
	{
		CBO_CAT_PROC_0400_08  pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
		return NULL;
	}
	else if (strSM == _T("목록완성_종분리"))
	{		
		CBO_CAT_PROC_0500_09 pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
		return NULL;
		
	}
	else if (strSM == _T("목록완성_데이터일괄변경"))
	{
		CBO_CAT_PROC_0801 *pDlg = new CBO_CAT_PROC_0801(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg->DoModal();
		pParentDlg->RecoverySelect();
	}
	else if (strSM == _T("목록완성_자료일괄확인"))
	{
		CBO_CAT_PROC_1300 pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
	}
	else if (strSM == _T("목록완성_마크반입"))
	{
		CMarcImportDlg dlg(pParentDlg);
		dlg.SetOpenMode(4000);
		dlg.DoModal();
	}
	else if (strSM == _T("목록완성_마크반출"))
	{
		pParentDlg->MarcExport();
		pParentDlg->RecoverySelect();
	}
	else if (strSM == _T("분류_분류판사항"))
	{
		CBO_ClassCodeVolumeDlg dlg(pParentDlg);
		dlg.DoModal();
		return NULL;
	}
	else if (strSM == _T("출력자료보관"))
	{
		pParentDlg->PRINT(_T("출력자료보관"));
	}
	else if (strSM == _T("보관된자료출력"))
	{
		pParentDlg->PRINT(_T("보관된자료출력"));
	}
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
	else if (strSM == _T("열람인계처리_출력"))
	{
		pParentDlg->PRINT(_T("열람인계목록"));
	}
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
	else if (strSM == _T("목록완성_등록자료복귀"))
	{
        ids = pParentDlg->CheckReturnRegist() ;
		if ( ids == 0 )
		{
			ids = pParentDlg->UpdateWorkingStatus(_T(""), _T("BOR112N"));
			if (ids >= 0) 
			{
				pParentDlg->Delete049Tag () ;	
					AfxMessageBox(_T("등록자료로 복귀 처리하였습니다."));
				pParentDlg->SelectAgain();
			}
		}
	}
	else if (strSM == _T("목록완성_정리완료_열람인계대상"))
	{
		ids = pParentDlg->Complete_Cataloging( CATALOG_COMPLETE );
		if (ids >= 0) 
		{
			AfxMessageBox(_T("정리 완료 처리하였습니다."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("목록완성_정리완료_열람인계자료"))
	{
		ids = pParentDlg->Complete_Cataloging(LOC_TRANSFER);
		if (ids >= 0) 
		{
			AfxMessageBox(_T("열람인계 처리하였습니다."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("목록완성_정리완료_배가자료"))
	{
		ids = pParentDlg->Complete_Cataloging( SHELF_DATE_WRITE, FALSE);
		if (ids >= 0) 
		{
			pParentDlg->Delete_DupFlag_Species() ;

			AfxMessageBox(_T("배가기록 처리하였습니다."));
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
		ids = pParentDlg->Complete_Cataloging( SHELF_DATE_WRITE, FALSE);
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("배가기록 처리하였습니다."));
		}
	}
	else if (strSM == _T("배가관리_배가일자기록"))
	{
		ids = pParentDlg->Loc_Record();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("목록완성_코드변경"))
	{
		ids = pParentDlg->ModifyCode() ;
	}
	else if (strSM == _T("목록완성_등록번호교체"))
	{
		ids = pParentDlg->RegNoChang() ;
	}
	else if (strSM == _T("목록완성_원문입력"))
	{
		ids = pParentDlg->InputDcm();
	}
	else if (strSM == _T("목록완성_분류판사항"))
	{
		CBO_ClassCodeVolumeDlg dlg(pParentDlg);
		dlg.DoModal();
		return NULL;
	}
	else if (strSM == _T("기관부호출력"))
	{
		pParentDlg->PRINT(_T("기관부호출력"));
	}

	return NULL;
}



