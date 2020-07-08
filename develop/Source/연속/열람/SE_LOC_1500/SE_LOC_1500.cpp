// SE_LOC_1500.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "SE_LOC_15001.h"
#include "SE_LOC_1510.h"
#include "SE_LOC_1520.h"
#include "SE_LOC_1530.h"
#include "SE_LOC_1540.h"
#include "SE_LOC_1550.h"
#include "SE_LOC_1550_5163.h"
#include "SE_LOC_1560.h"
#include "SE_LOC_1580.h"
#include "SE_LOC_1590.h"
#include "SE_LOC_1595.h"
#include "SE_LOC_1598.h"
//#include _T("SE_LOC_1599.h")
#include "SE_LOC_1599_EX.h"
#include "EquipPrintDlg_SE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_LOC_1500DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_LOC_1500.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_LOC_1500DLL, hInstance))
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

		new CDynLinkLibrary(SE_LOC_1500DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_LOC_1500.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_LOC_1500DLL);
	}
	return 1;   // ok
}



__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CSE_LOC_1500* m_pWindow = NULL;
	m_pWindow = NULL;
	m_pWindow = new CSE_LOC_1500(NULL);
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
	
	CSE_LOC_1500* m_pWindow = (CSE_LOC_1500*)pWindow;
	
	if (strSM == _T("검색"))
	{
		CSE_LOC_1510 *pDlg = NULL;
		pDlg = new CSE_LOC_1510(m_pWindow);
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
	// (A_2004-1019_HAN...)
	// 실시간열람 메뉴에서 삭제할 수 있는 기능 추가
	}else if (strSM == _T("삭제"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		m_pWindow->m_pSpfWork->SPFExcutePath( REMOVE_COMPLETE );

	}else if (strSM == _T("데이터복귀"))	{

		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1520 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();

	}else if (strSM == _T("배가일기록"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1530 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	}
	else if (strSM == _T("책배가변경"))	{
		
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1540 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	
	}else if (strSM == _T("가등록번호부여"))	{
		
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
	
		CString strIs5163=_T("");
		m_pWindow->GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , strIs5163 );

		if ( _T("Y") == strIs5163 )
		{	
			CSE_LOC_1550_5163 Dlg(m_pWindow);
			Dlg.pMain = m_pWindow->pMain;
			Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
			if (bReadOnly) Dlg.SM_STYLE = 'R';
			Dlg.m_pInfo = m_pWindow->m_pInfo;
			Dlg.DoModal();
		}
		else
		{
			CSE_LOC_1550 Dlg(m_pWindow);
			Dlg.pMain = m_pWindow->pMain;
			Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
			if (bReadOnly) Dlg.SM_STYLE = 'R';
			Dlg.m_pInfo = m_pWindow->m_pInfo;
			Dlg.DoModal();
		}	
	}else if (strSM == _T("함번호변경"))	{

		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1560 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();

	}else if (strSM == _T("배가취소"))	{
		
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		
		m_pWindow->m_pSpfWork->SPFExcutePath( CANCEL_SHELF );	
	
	}else if (strSM == _T("가등록번호부여취소"))	{
		
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		
		m_pWindow->m_pSpfWork->SPFExcutePath( CANCEL_TMP_REG_NO );	
	
	}else if (strSM == _T("폐기처리"))	{

		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1580 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();

	}else if (strSM == _T("종배가변경"))	{

		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}

		CSE_LOC_1590 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();

	}
	else if (strSM == _T("코드변경"))	{
		
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 )
		{
			AfxMessageBox( _T("자료를 먼저 선정해 주십시오.") );
			return NULL;
		}
		
		CSE_LOC_1595 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
		
	}	
	else if (strSM == _T("출력"))	{		
		CSE_LOC_1598 Dlg(m_pWindow);
		Dlg.DoModal();

		if( Dlg.m_nCloseMode == 1 )	//레이블출력
		{
/*
			CSE_LOC_1599 Dlg(m_pWindow);
			Dlg.pMain = m_pWindow->pMain;
			Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
			if (bReadOnly) Dlg.SM_STYLE = 'R';
			Dlg.m_pInfo = m_pWindow->m_pInfo;
			CESL_DataMgr* pDM = m_pWindow->FindDM(_T("DM_SE_LOC_1500"));
			Dlg.SetParentInfo(pDM);
			Dlg.DoModal();
*/
			CSE_LOC_1599_EX Dlg(m_pWindow);
			Dlg.SetParentInfo( m_pWindow->FindDM(_T("DM_SE_LOC_1500")) , ((CESL_Grid*)m_pWindow->FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"))) );
			/*
			CStringArray sArr;
			CEquipPrintDlg_SE Dlg(m_pWindow);
			Dlg.SetParentInfo( m_pWindow->FindDM(_T("DM_SE_LOC_1500")) , ((CESL_Grid*)m_pWindow->FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"))), _T("권호KEY"), &sArr );
			*/
			Dlg.DoModal();
		}
		else if( Dlg.m_nCloseMode == 0 )
		{
			m_pWindow->PrintList( Dlg.m_printTitle, _T("출력") );
		}

		
	}	
	else if (strSM == _T("보관자료출력"))
	{
		CSE_LOC_1598 Dlg(m_pWindow);
		Dlg.DoModal();

		if( Dlg.m_nCloseMode == 1 )	//레이블출력
		{
			CStringArray sArr;
			sArr.Copy(m_pWindow->m_arrStrVolKey);
			
			CEquipPrintDlg_SE Dlg(m_pWindow);
			Dlg.SetParentInfo( m_pWindow->FindDM(_T("DM_SE_LOC_1500")) , ((CESL_Grid*)m_pWindow->FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"))), _T("권호KEY"), 0);
			Dlg.SetArrDeleteBookKey( m_pWindow->m_arrStrDeleteBookKey );
			Dlg.DoModal();

			// 삭제키 가져와서 SET
			sArr.RemoveAll();
			Dlg.GetArrDeleteBookKey(sArr);

			m_pWindow->m_arrStrDeleteBookKey.RemoveAll();
			m_pWindow->m_arrStrDeleteBookKey.Copy(sArr);

		}
		else if( Dlg.m_nCloseMode == 0 )
		{
			m_pWindow->PrintList( Dlg.m_printTitle, _T("보관자료출력") );
		}

	}
	else if (strSM == _T("출력자료보관"))
	{
		m_pWindow->SavePrintList( _T("책정보KEY") );
	}
	
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	CSE_LOC_1500* m_pWindow = (CSE_LOC_1500*)pWindow;

	if(m_pWindow){
		if( m_pWindow->m_pSpfWork )
			delete m_pWindow->m_pSpfWork;
		delete m_pWindow;

	}
}