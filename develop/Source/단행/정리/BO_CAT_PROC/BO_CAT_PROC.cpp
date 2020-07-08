// BO_CAT_PROC.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_PROC_0000.h"
#include "BO_CAT_PROC_0100.h"
#include "BO_CAT_PROC_0200.h"
#include "BO_CAT_PROC_0300.h"
#include "BO_CAT_PROC_0301.h"
#include "BO_CAT_PROC_0400.h"
#include "BO_CAT_PROC_0400_08.h"
#include "BO_CAT_PROC_0500_09.h"
#include "BO_CAT_PROC_0801.h"
#include "BO_CAT_PROC_1100.h"
#include "BO_CAT_PROC_1200.h"
#include "BO_CAT_PROC_1300.h"

#include "../../../공통/Duplicate_Data_Search/DuplicateSearch_Interface.h"
#include "../../../공통/Duplicate_Data_Search/DUP_KEY_SETTING.h"
#include "..\..\..\공통\FileImport\FileImportDlg.h"
#include "..\BO_CAT_DUP_FREE\BO_CAT_DUP_FREE2.h"

static AFX_EXTENSION_MODULE BO_CAT_PROCDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_PROC.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_PROCDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_PROCDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_PROC.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_PROCDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CBO_CAT_PROC_0000* m_pWindow = new CBO_CAT_PROC_0000(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
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

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	if (!pWindow) return NULL;
	CBO_CAT_PROC_0000* m_pWindow = (CBO_CAT_PROC_0000*)pWindow;
	
	if (strSM == _T("검색"))
	{
		CBO_CAT_PROC_0100 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0100(m_pWindow);
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
	else if (strSM == _T("출력"))
	{
		CBO_CAT_PROC_0200 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0200(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		pDlg->DoModal();
		delete pDlg;
		return NULL;
	}
	else if (strSM == _T("입력"))
	{
		CBO_CAT_PROC_0301 dlg(m_pWindow);		
		dlg.DoModal();		
	}
	
	else if (strSM == _T("수정"))
	{
		CBO_CAT_PROC_0300 dlg(m_pWindow);
		dlg.SetAlias( _T("CM_BO_CAT_PROC_0000"), _T("ProcGrid") );
		dlg.DoModal();
	}
	else if (strSM == _T("삭제"))
	{
		CBO_CAT_PROC_0400 dlg(m_pWindow);
		dlg.OnInitDialog();		
		dlg.OnbACCESSIONNOGIVE();
	}
	else if (strSM == _T("통합"))
	{
		CBO_CAT_PROC_0400_08 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0400_08(m_pWindow);
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
	else if (strSM == _T("분리"))
	{
		CBO_CAT_PROC_0500_09 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0500_09(m_pWindow);
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

	else if (strSM == _T("공용사항관리"))
	{

	}
	else if (strSM == _T("데이터일괄변경"))
	{
		CBO_CAT_PROC_0801 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_0801(m_pWindow);
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
	else if (strSM == _T("원부일괄확인"))
	{
		CBO_CAT_PROC_1300 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_1300(m_pWindow);
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
	
	else if (strSM == _T("복본조사"))
	{
		CString MainDMAlias = _T("DM_BO_CAT_PROC_0000");
		CString GridAlias = _T("ProcGrid");
	
		m_pWindow->SearchDuplicateData();

		return NULL;
	}
	

	else if (strSM == _T("키설정"))
	{
		CDUP_KEY_SETTING Dlg(m_pWindow, _T(".\\duplicate_search_key.txt"));
		Dlg.DoModal();
		return NULL;
	}

	else if (strSM == _T("반입"))
	{
		CFileImportDlg dlg(m_pWindow);
		dlg.SetBeginImportStatus(_T("정리"), _T("BOC111N"));
		dlg.SetOpenMode(4000);
		dlg.DoModal();
	}
	
	else if (strSM == _T("반출"))
	{
		CBO_CAT_PROC_1100 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_1100(m_pWindow);
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
	else if (strSM == _T("복본해제"))
	{
		CBO_CAT_DUP_FREE dlg(m_pWindow);
		dlg.DoModal();
	}
	else if (strSM == _T("코드변경"))
	{

	}

	else if (strSM == _T("정리완료"))
	{
		CBO_CAT_PROC_1200 *pDlg = NULL;
		pDlg = new CBO_CAT_PROC_1200(m_pWindow);
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

	return NULL;
}