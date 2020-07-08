#include "stdafx.h"
#include <afxdllx.h>
#include "BO_LOC_31001.h"
#include "BO_LOC_3110.h"
#include "BO_LOC_3190.h" 
#include "BO_LOC_3192.h"
#include "BO_LOC_3180.h"
#include "BO_LOC_3100_PrintReceipt.h"
#include "BO_LOC_LOAN_HIS.h"
#include "TL_BO_LOC_LOAN_HIS.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170_ReserveInfo.h"
#include "..\BO_LOC_3300\BO_LOC_33001.h"
#include "..\\BL_LOC_LOAN\\BL_LOC_LOAN_PROC.h"
#include "..\\BO_LOC_3300\\LoanUserManage.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "FileLoanDlg.h"
#include "../CO_USER_CARD/CardPrintDlg.h"

#include "..\\KL_OTHER_LOAN_MNG\\KL_OTHER_LOAN_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE BO_LOC_3100DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3100.DLL Initializing!\n");
		
		
		if (!AfxInitExtensionModule(BO_LOC_3100DLL, hInstance))
			return 0;

		new CDynLinkLibrary(BO_LOC_3100DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3100.DLL Terminating!\n");
		
		AfxTermExtensionModule(BO_LOC_3100DLL);
	}
	return 1;   
}

CBO_LOC_3190* g_pColorDlg = NULL;

CBO_LOC_3110* g_pSearchBookDlg = NULL;
CBO_LOC_LOAN_HIS* g_pLoanHisDlg = NULL;
TL_BO_LOC_LOAN_HIS* g_pTLLoanHisDlg = NULL;
CESL_Mgr* g_p3320 = NULL;
CBO_LOC_3100* g_p3100 = NULL;
CLoanUserManage* g_pLoanUserManage = NULL;
UINT DoModalDlgThread(LPVOID lParam);
UINT UserInsertDlgThread(LPVOID lParam);

CKL_OTHER_LOAN_MAIN* g_pKlOtherLoanMain = NULL;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){	
	CBO_LOC_3100* pWindow = NULL;
	pWindow = new CBO_LOC_3100(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	g_p3100 = pWindow;
	g_p3320 = NULL;
	g_pTLLoanHisDlg = NULL;
	g_pLoanHisDlg = NULL;
	g_pSearchBookDlg = NULL;
	g_pLoanUserManage = NULL;
	g_pColorDlg = NULL;
	g_pKlOtherLoanMain = NULL;
	
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly , CWnd *pWindow){
	if (!pWindow) return NULL;
	
	CBO_LOC_3100 *pTmpWindow = (CBO_LOC_3100*)pWindow;
	
	if( 10000 == pTmpWindow->m_pInfo->MODE || 30000 == pTmpWindow->m_pInfo->MODE )
	{
		if(
			_T("통함대출자정보보기") == strSM || 
			_T("통합센터대출이력조회") == strSM ||
			_T("타관반납관리") == strSM || 
			_T("타관반납자료") == strSM)
		{
			return NULL;
		}
	}
	
	if (strSM == _T("BO_LOC_3110"))
	{		
		if(NULL == g_pSearchBookDlg)
		{
			g_pSearchBookDlg = new CBO_LOC_3110(pTmpWindow,pTmpWindow->m_pLoanMangeValue,0,pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pPreRegNoMgr, pTmpWindow->m_pLoan->m_strUserNo);
			g_pSearchBookDlg->pMain = pTmpWindow->pMain;
			g_pSearchBookDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
			g_pSearchBookDlg->m_pInfo = pTmpWindow->m_pInfo;
			if(g_pSearchBookDlg->Create(pTmpWindow))
			{			
				g_pSearchBookDlg->CenterWindow();
				g_pSearchBookDlg->ShowWindow(SW_SHOW);
				g_pSearchBookDlg->UpdateWindow();
				return g_pSearchBookDlg;
			}
			else
			{
				delete g_pSearchBookDlg;
				g_pSearchBookDlg = NULL;
				return NULL;
			}
		}
		else
		{
			if(TRUE == g_pSearchBookDlg->m_bShow)
			{
				g_pSearchBookDlg->ShowWindow(SW_HIDE);
			}
			else
			{
				g_pSearchBookDlg->ShowWindow(SW_RESTORE);
				g_pSearchBookDlg->SetActiveWindow();
				g_pSearchBookDlg->SetFocus();
			}
		}
	}
	else if (strSM == _T("BO_LOC_3130"))	{
		pTmpWindow->m_pLoan->SPFExcutePath( _T("반납") );
		pTmpWindow->ClearNumberCtrl();
		
		if( pTmpWindow->m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
		{
			pTmpWindow->ShowFeeReceipt();
		}
		
	}	
	else if (strSM == _T("BO_LOC_3170"))	
	{
		INT nRowIndex = 0;
		INT ids = 0;
		BOOL bEdit = FALSE;
		
		CString sValue = _T("");
		pTmpWindow->m_pLoan->m_pCheckLoan->GetReserveMode(sValue);
		if( sValue.Compare(_T("0"))==0 )
		{
			CString sMsg;
			sMsg.Format( _T("예약 기능을 사용하실 수 없습니다. \r\n관리에서 설정하십시오") );
			pTmpWindow->m_pLoan->SelfCloseMessageBox( sMsg );
			return NULL;
		}
		CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
		TCHAR *sFieldAlias[5] = 
		{
			_T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY")
		};
		CString sGetData[5];

		ids = pTmpWindow->m_pLoan->GetCurrentGridPos( nRowIndex );
		if( ids < 0 ) return NULL;

		for( INT i = 0 ; i < 5 ; i++ )
		{
			ids = pTmpWindow->GetDataMgrData( sDMAlias , sFieldAlias[i] , sGetData[i] , nRowIndex );
			if( ids < 0 ) return NULL;
		}
		
		ids	=	pTmpWindow->m_pLoan->SPFExcutePath( _T("예약상태") );
		if( 0	!=	ids  )	return	NULL;		
	}
	else if ( strSM == _T("긴급대출") )
	{
		CBO_LOC_3192 *pDlg = NULL;
		pDlg = new CBO_LOC_3192(pTmpWindow,pTmpWindow->m_pLoanShareManager,pTmpWindow->m_pLoanMangeValue );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if( strSM == _T("예약") )
	{
		CString sValue;
		pTmpWindow->m_pLoan->m_pCheckLoan->GetReserveMode(sValue);
		if( sValue.Compare(_T("0"))==0 )
		{
			CString sMsg;
			sMsg.Format(_T("예약 기능을 사용하실 수 없습니다. \r\n관리에서 설정하십시오"));
			pTmpWindow->m_pLoan->m_IsReqPrior = FALSE;
			pTmpWindow->m_pLoan->SelfCloseMessageBox( sMsg );
		}

		pTmpWindow->m_pLoan->SPFExcutePath( _T("예약") );
		pTmpWindow->ClearNumberCtrl();
	}
	else if( _T("예약자료지정") == strSM )
		pTmpWindow->m_pLoan->SPFExcutePath( strSM );
	else if( strSM == _T("예약대출") )
	{
		CString sValue;
		pTmpWindow->m_pLoan->m_pCheckLoan->GetReserveMode(sValue);
		if( sValue.Compare(_T("0"))==0 )
		{
			CString sMsg;
			sMsg.Format(_T("예약 기능을 사용하실 수 없습니다. \r\n관리에서 설정하십시오"));
			pTmpWindow->m_pLoan->SelfCloseMessageBox( sMsg );
		}

		pTmpWindow->m_pLoan->SPFExcutePath( _T("예약대출") );
	}
	else if( strSM == _T("예약취소") )
	{
		CString sValue;
		pTmpWindow->m_pLoan->m_pCheckLoan->GetReserveMode(sValue);
		if( sValue.Compare(_T("0"))==0 )
		{
			CString sMsg;
			sMsg.Format(_T("예약 기능을 사용하실 수 없습니다. \r\n관리에서 설정하십시오"));
			pTmpWindow->m_pLoan->SelfCloseMessageBox( sMsg );
		}

		pTmpWindow->m_pLoan->SPFExcutePath( _T("예약취소") );
	}
		else if( strSM == _T("취소") )
	{
		pTmpWindow->m_pLoan->SPFExcutePath( _T("취소") );
	}
	else if( strSM == _T("반납연기") )
	{
		INT ids = pTmpWindow->m_pLoan->m_pCheckLoan->GetIsDelayFunc();
		if( ids == 1 )
		{
			pTmpWindow->m_pLoan->SelfCloseMessageBox( _T("반납 연기 기능을 사용할 수 없습니다.\r\n관리에서 설정하십시오.") );

			return NULL;
		}

		pTmpWindow->m_pLoan->SPFExcutePath( _T("반납연기") );

		((CEdit*) pTmpWindow->GetDlgItem(IDC_edtNUMBER) )->SetFocus();
	}
	else if( strSM == _T("색설정") )
	{
		g_pColorDlg = new CBO_LOC_3190(pTmpWindow,pTmpWindow->m_pLoanShareManager,pTmpWindow->m_pLoanMangeValue);
		g_pColorDlg->pMain = pTmpWindow->pMain;
		g_pColorDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		g_pColorDlg->m_pInfo = pTmpWindow->m_pInfo;
		g_pColorDlg->m_pLoan = pTmpWindow->m_pLoan;		
		if(g_pColorDlg->Create(pTmpWindow))
		{			
			g_pColorDlg->CenterWindow();
			g_pColorDlg->ShowWindow(SW_SHOW);
			g_pColorDlg->UpdateWindow();
			return g_pColorDlg;
		}
		else
		{
			delete g_pColorDlg;
			g_pColorDlg = NULL;
			return NULL;
		}
	}
	else if( strSM == _T("대출자") )
	{		
		g_pLoanUserManage = new CLoanUserManage(pTmpWindow,pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue );
  		g_pLoanUserManage->pMain = pTmpWindow->pMain;
  		g_pLoanUserManage->pParentMgr = (CESL_Mgr*)pTmpWindow;
  		if (bReadOnly) g_pLoanUserManage->SM_STYLE = 'R';
  		g_pLoanUserManage->m_pInfo = pTmpWindow->m_pInfo;
  		g_pLoanUserManage->Create(pTmpWindow);

		CString strUserNo;
		g_pLoanUserManage->UserInsert(strUserNo);			
		if(FALSE == strUserNo.IsEmpty())
		{
			pTmpWindow->SendMessage(WM_USER+1028, NULL, (LPARAM)&strUserNo);
		}

		delete g_pLoanUserManage;
		g_pLoanUserManage = NULL;

 		return NULL;
	}
	else if( _T("통함대출자정보보기") == strSM )
	{
		pTmpWindow->m_pUnionUserInfoViewer->ShowWindow( true );
	}
	else if( _T("오프라인대출반납") == strSM )
	{
		CBO_LOC_3180 Dlg( pTmpWindow , pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue);
		Dlg.DoModal();
	}
	else if( _T("개인별대출이력보기") == strSM )
	{
		CString sUserKey;
		sUserKey = pTmpWindow->GetCurrentUserPK();

		if( sUserKey.IsEmpty() )
		{
			pTmpWindow->m_pLoan->SelfCloseMessageBox(_T("이용자를 선택하십시오."));
			return 0;
		}

		pTmpWindow->m_pLoan->Send3000ApiSocket(_T("대출이력조회"), sUserKey);

		if( (pTmpWindow->m_pInfo->MODE == 10000) || (pTmpWindow->m_pInfo->MODE == 30000) )
		{
			TL_BO_LOC_LOAN_HIS LoanHis(pTmpWindow);
			LoanHis.SetUserKey(sUserKey);				
			LoanHis.DoModal();
			return NULL;
		}
		else
		{
			CBO_LOC_LOAN_HIS LoanHis(pTmpWindow, pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue);
			LoanHis.SetUserKey(sUserKey);				
			LoanHis.DoModal();
			return NULL;
		}
	}
	else if ( _T("영수증발급") == strSM )
	{
		pTmpWindow->m_pLoan->SPFExcutePath( _T("영수증출력") );
		pTmpWindow->ClearNumberCtrl();
	}
	else if ( _T("화면출력") == strSM )
	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("이용자 정보를 출력할 권한이 없습니다.\r\n해당 권한은 관리자를 통해서만 부여받을 수 있습니다."), MB_ICONWARNING);
			return NULL;
		}
		pTmpWindow->PrintScreen();
	}
	else if ( _T("SMS발송") == strSM )
	{
		pTmpWindow->SendOneSMS();
	}
	else if ( _T("바코드업무설정") == strSM )
	{
		pTmpWindow->ShowBarcodeFlowSetting();
	}	
	else if(_T("책두레_대출") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("책두레대출화면"));
	}
	else if(_T("책두레_반납") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("책두레반납화면"));
	}
	else if(_T("책두레_전환") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("책두레_전환"));
	}
	else if(_T("부록대출") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("부록대출"));
	}
	else if(_T("통합센터대출이력조회") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("통합센터대출이력조회"));
	}
	else if(_T("타관반납관리") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("타관반납관리"));
	}
	else if(_T("타관대출") == strSM)
	{
		pTmpWindow->m_pLoan->SPFExcutePath(_T("타관대출"));

	}
	else if(_T("타관대출관리") == strSM)
	{
		if(NULL == g_pKlOtherLoanMain)
		{
			g_pKlOtherLoanMain = new CKL_OTHER_LOAN_MAIN(pTmpWindow);
			g_pKlOtherLoanMain->pMain = pTmpWindow->pMain;
			g_pKlOtherLoanMain->pParentMgr = (CESL_Mgr*)pTmpWindow;
			g_pKlOtherLoanMain->m_pInfo = pTmpWindow->m_pInfo;
			g_pKlOtherLoanMain->m_pLoan = pTmpWindow->m_pLoan;
			if(g_pKlOtherLoanMain->Create(pTmpWindow))
			{			
				g_pKlOtherLoanMain->CenterWindow();
				g_pKlOtherLoanMain->ShowWindow(SW_SHOW);
				g_pKlOtherLoanMain->UpdateWindow();
				return g_pKlOtherLoanMain;
			}
			else
			{
				delete g_pKlOtherLoanMain;
				g_pKlOtherLoanMain = NULL;
				return NULL;
			}
		}
		else
		{
			if(TRUE == g_pKlOtherLoanMain->m_bShow)
			{
				g_pKlOtherLoanMain->ShowWindow(SW_HIDE);
			}
			else
			{
				g_pKlOtherLoanMain->ShowWindow(SW_RESTORE);
				g_pKlOtherLoanMain->SetActiveWindow();
				g_pKlOtherLoanMain->SetFocus();
			}
		}
	}
	
	else if ( _T( "타관반납자료" ) == strSM )
	{
		CUnionReturnDlg Dlg(pTmpWindow);
		Dlg.DoModal();
		if(Dlg.m_nReturnCode == 10000)
		{
			pTmpWindow->m_pLoan->SelfCloseMessageBox(_T("반납처리 되었습니다."), NULL, 2000);
		}
		else if(Dlg.m_nReturnCode == 20000)
		{
			pTmpWindow->m_pLoan->SelfCloseMessageBox(_T("반납취소처리 되었습니다."), NULL, 2000);
		}
		pTmpWindow->GetDlgItem(IDC_edtNUMBER)->SetFocus();	
	}
	else if(_T("파일일괄대출") == strSM)
	{
		INT ids = pTmpWindow->m_pLoan->IsSearchedUser();
		if(0 != ids)
		{
			return NULL;
		}	
		
		CFileLoanDlg Dlg(pTmpWindow);
		Dlg.m_pLoan = pTmpWindow->m_pLoan;
		Dlg.m_pLoanMangeValue = pTmpWindow->m_pLoanMangeValue;
		Dlg.m_pLoanShareManager = pTmpWindow->m_pLoanShareManager;
		Dlg.DoModal();
	}
	
	else if (_T("대출증출력관리") == strSM)
	{
		CString strUserKey;
		strUserKey = pTmpWindow->GetCurrentUserPK();

		if( strUserKey.IsEmpty() )
		{
			pTmpWindow->m_pLoan->SelfCloseMessageBox(_T("이용자를 선택하십시오."));
			return 0;
		}

		CCardPrintDlg Dlg(pTmpWindow);
		Dlg.SetIsUnityLoanService(pTmpWindow->m_pLoanMangeValue->m_sIsUnityLoanService);
		Dlg.SetUserKey(strUserKey);
		Dlg.DoModal();
	}
	else if(_T("통합회원_전환") == strSM)
	{
		pTmpWindow->StartUserSwitch();
	}
	
	else if(_T("통합회원_동기화") == strSM)
	{
		pTmpWindow->StartUserSync();
	}
	
	else if(_T("통합회원_대출현황보기") == strSM)
	{
		pTmpWindow->KL_LoanView();
	}
	
	else if(_T("통합회원_반입") == strSM)
	{
		pTmpWindow->StartUserImport();
	}
	
	else if(_T("책이음패널티") == strSM)
	{
		pTmpWindow->SetLillPanalty();
	}
	else if(_T("대출정보 수정") == strSM)
	{
		pTmpWindow->UpdateLoanInfo();
	}
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	CBO_LOC_3100 *pTmpWindow = (CBO_LOC_3100*)pWindow;

	if( pTmpWindow )
		delete pTmpWindow;

	if(NULL != g_pLoanHisDlg)
	{
		if(::IsWindow(g_pLoanHisDlg->m_hWnd))
		{
			g_pLoanHisDlg->EndDialog(IDCANCEL);
		}
		delete g_pLoanHisDlg;
		g_pLoanHisDlg = NULL;
	}

	if(NULL != g_pTLLoanHisDlg)
	{
		if(::IsWindow(g_pTLLoanHisDlg->m_hWnd))
		{
			g_pTLLoanHisDlg->EndDialog(IDCANCEL);
		}
		delete g_pTLLoanHisDlg;
		g_pTLLoanHisDlg = NULL;
	}

	if(NULL != g_pLoanUserManage)
	{
		delete g_pLoanUserManage;
		g_pLoanUserManage = NULL;
	}

}

UINT DoModalDlgThread(LPVOID lParam)
{
	CDialog* pDlg = (CDialog*)lParam;
	return pDlg->DoModal();
}

UINT UserInsertDlgThread(LPVOID lParam)
{
	CBO_LOC_3320* pDlg = (CBO_LOC_3320*)lParam;
	pDlg->DoModal();

	CBO_LOC_3100 *pTmpWindow = (CBO_LOC_3100*)pDlg->FindSM(_T("BO_LOC_3100"));

	if(!pTmpWindow)
	{
		return 1000;
	}

	CString strUserNo = pDlg->m_strLastWorkUserNo;
	if(FALSE == strUserNo.IsEmpty())
	{
		pTmpWindow->SendMessage(WM_USER+1028, NULL, (LPARAM)&strUserNo);
	}

	return 0;
}

