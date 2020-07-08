// BO_CAT_MNG_UnlessWord.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_UNLESS_WORD_0000.h"
#include "BO_CAT_UNLESS_WORD_0100.h"
#include "BO_CAT_UNLESS_WORD_0200.h"
#include "BO_CAT_UNLESS_WORD_0300.h"

static AFX_EXTENSION_MODULE BO_CAT_MNG_UnlessWordDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_MNG_UNLESSWORD.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_MNG_UnlessWordDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_MNG_UnlessWordDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_MNG_UNLESSWORD.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_MNG_UnlessWordDLL);
	}
	return 1;   // ok
}

CBO_CAT_UNLESS_WORD_0000* m_pWindow=NULL;

__declspec(dllexport) VOID CreateForm(CWnd *pParent, CESL_Information* pInfo) 
{
	m_pWindow = NULL;
	m_pWindow = new CBO_CAT_UNLESS_WORD_0000(NULL);
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
}

__declspec(dllexport) VOID ResizeForm(CRect rect) 
{
	if (m_pWindow) 
	{
		m_pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm() 
{
	if (m_pWindow) 
	{
		delete m_pWindow;
	}
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly)
//__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, BOOL bReadOnly)
{
	if (!m_pWindow) return NULL;
	
	if (strSM == _T("검색"))
	{
		CBO_CAT_UNLESS_WORD_0100 *pDlg = NULL;
		pDlg = new CBO_CAT_UNLESS_WORD_0100(m_pWindow);
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
	else if (strSM == _T("입력"))
	{
		CBO_CAT_UNLESS_WORD_0200 *pDlg = NULL;
		pDlg = new CBO_CAT_UNLESS_WORD_0200(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		if( pDlg->DoModal() == IDOK ) {
			m_pWindow->CreateFile();
			//CreateFile();
		}
		delete pDlg;
		return NULL;
		/*
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
		*/
	}
	else if (strSM == _T("수정"))
	{
		CBO_CAT_UNLESS_WORD_0300 *pDlg = NULL;
		pDlg = new CBO_CAT_UNLESS_WORD_0300(m_pWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;

		if( pDlg->DoModal() == IDOK ) {
			m_pWindow->CreateFile();
			//CreateFile();
		}
		delete pDlg;
		return NULL;
		/*
		if (pDlg->Create(m_pWindow) && pDlg->m_bInitDlg)		
		//if (pDlg->Create(m_pWindow))
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
	else if (strSM == _T("삭제"))
	{
		
		//CBO_CAT_UNLESS_WORD_0000 *pDlg = NULL;
		//pDlg->Delete();		
		m_pWindow->Delete();
		/*
		SelectMakeList(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		INT idx = SelectGetTailPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			return;
		}

		if (MessageBox(_T("선정한 원부를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
			pDM->StartFrame();

			CString sAccRecKey, sAccRecKeyTmp;
			do
			{
				CString strCondition;
				pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

				sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

				pDM->MakeDeleteFrame(_T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
				pDM->DeleteRow(idx);

				idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));			
			}		
			while (idx > 0);

			sAccRecKey = sAccRecKeyTmp.Mid(1);
			CString sQuery;
			sQuery.Format( _T("DELETE FROM BO_BOOK_TBL WHERE ACCESSION_NO IN (%s);") , sAccRecKey );
			INT iCh = pDM->AddFrame(sQuery);

			INT ids = pDM->SendFrame();
			if (ids < 0) {
				AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
				return;
			}
			pDM->EndFrame();
		}
		AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
		*/
	}

	return NULL;
}