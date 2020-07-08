// BO_LOC_3400.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_34001.h"
#include "BO_LOC_3410.h"
#include "BO_LOC_3420.h"
#include "BO_LOC_3430.h"
#include "BO_LOC_3440.h"
#include "..\BL_LOC_USER\BO_LOC_3320.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_3400DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3400.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_3400DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_3400DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3400.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_3400DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_3400* m_pWindow = NULL;
	m_pWindow = new CBO_LOC_3400(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();

	return m_pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow){
	if (!pWindow) return NULL;
	
	CBO_LOC_3400 *m_pWindow = (CBO_LOC_3400*)pWindow;
	
	if (strSM == _T("BO_LOC_3410"))
	{
		CBO_LOC_3410 *pDlg = NULL;
		pDlg = new CBO_LOC_3410(m_pWindow,m_pWindow->m_pLoanMangeValue);
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
	}else if (strSM == _T("BO_LOC_3420"))	{		
		CBO_LOC_3420 Dlg(m_pWindow,m_pWindow->m_pLoanMangeValue,0,m_pWindow->m_pLoanShareManager );
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	}else if (strSM == _T("BO_LOC_3430"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		CBO_LOC_3420 Dlg(m_pWindow,m_pWindow->m_pLoanMangeValue,1,m_pWindow->m_pLoanShareManager );
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	}else if (strSM == _T("접수처리"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		CBO_LOC_3430 Dlg( m_pWindow, m_pWindow->m_pLoanMangeValue );
		Dlg.DoModal();		
	}else if (strSM == _T("배달처리"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		CBO_LOC_3440 Dlg(m_pWindow,m_pWindow->m_pLoanShareManager,m_pWindow->m_pLoanMangeValue);
		Dlg.DoModal();		
	}else if (strSM == _T("회원정보"))	{
		CESL_DataMgr *pDM = m_pWindow->FindDM( _T("DM_BO_LOC_3400") );
		if( pDM->GetRowCount() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 검색해주십시오") );
			return NULL;
		}

		CESL_Grid* pGrid = m_pWindow->m_pSpfWork->GetMainGridPtr();

		INT nCurrentIndex = pGrid->GetIdx();

		CString sRecKey;
		pDM->GetCellData( _T("이용자KEY") , nCurrentIndex , sRecKey );

		CBO_LOC_3320 ViewMember( m_pWindow, m_pWindow->m_pLoanShareManager, m_pWindow->m_pLoanMangeValue );
		ViewMember.SetMode(7,TRUE);

		ViewMember.SetRecKey( sRecKey );
		ViewMember.m_nAction = ACTION_READ_F_DELIVERY;
		ViewMember.DoModal();
	}else if (strSM == _T("삭제"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		m_pWindow->m_pSpfWork->SPFExcutePath( DELETE_DELIVERY );
	}else if (strSM == _T("출력"))	{

		if (!m_pWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("이용자 정보를 출력할 권한이 없습니다.\r\n해당 권한은 관리자를 통해서만 부여받을 수 있습니다."), MB_ICONWARNING);
			return NULL;
		}
		m_pWindow->PrintList();
	}
	else if (strSM == _T("메일발송"))
	{
		m_pWindow->OpenMail () ;
	}
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	CBO_LOC_3400 *m_pWindow = (CBO_LOC_3400*)pWindow;
	if(pWindow){
		if( m_pWindow->m_pLoanMangeValue != NULL ) 
			delete m_pWindow->m_pLoanMangeValue;
		if( m_pWindow->m_pSpfWork != NULL ) 
			delete m_pWindow->m_pSpfWork;
		delete pWindow;
	}
}
