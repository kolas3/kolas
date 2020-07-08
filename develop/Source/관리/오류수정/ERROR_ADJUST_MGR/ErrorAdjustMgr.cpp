// ErrorAdjustMgr.cpp: implementation of the CErrorAdjustMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustMgr.h"
#include "ErrorAdjustFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustMgr::CErrorAdjustMgr()
{
	m_pErrorAdjustResouceMgr = NULL;
	m_pFrameWnd = NULL;
	m_pErrorAdjustDlgMgr = NULL;
}

CErrorAdjustMgr::~CErrorAdjustMgr()
{
	if(m_pErrorAdjustDlgMgr)
	{
		delete m_pErrorAdjustDlgMgr;
		m_pErrorAdjustDlgMgr = NULL;
	}
	if(m_pErrorAdjustResouceMgr)
	{
		delete m_pErrorAdjustResouceMgr;
		m_pErrorAdjustResouceMgr = NULL;
	}
}

INT CErrorAdjustMgr::CreateResource(CFrameWnd *pFrameWnd, ERROR_ADJUST_TYPE nErrorAdjustType/*=PURCHASEINFO_ERROR_ADJUST*/)
{
	
	m_pFrameWnd = pFrameWnd;
	
	if (m_pErrorAdjustDlgMgr == NULL) return -1;
	m_pErrorAdjustDlgMgr->SetMainWnd(pFrameWnd);
		
	// 리소스 매니져 생성
	if (m_pErrorAdjustResouceMgr) delete m_pErrorAdjustResouceMgr;
	
	m_pErrorAdjustResouceMgr = new CErrorAdjustResourceMgr(m_pErrorAdjustDlgMgr->GetErrorAdjustDlg());
	m_pErrorAdjustResouceMgr->CreateResource(pFrameWnd, nErrorAdjustType);
	
	
	// 리소스 매니져의 포인터를 ESL에게 넘겨준다. 
	if(m_pErrorAdjustDlgMgr->GetErrorAdjustDlg())
	{
		m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->SetResourceMgr(m_pErrorAdjustResouceMgr);
	}	
	
	return 0;
}

BOOL CErrorAdjustMgr::OnCmdMsg(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	return m_pErrorAdjustResouceMgr->OnCmdMsg(nID);
}

BOOL CErrorAdjustMgr::OnCommand(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	CString strCommandAlias;
	
	BOOL bReturn = m_pErrorAdjustResouceMgr->OnCommand(nID, strCommandAlias);
	
	if (strCommandAlias.IsEmpty()) return bReturn;
	
	INT ids = -1;
	
	//종정보보기
	CErrorAdjustFormView *pView = (CErrorAdjustFormView *) m_pFrameWnd->GetActiveView();
	if (strCommandAlias == _T("PURCHASEINFO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(PURCHASEINFO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, PURCHASEINFO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("VOLINDEX_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(VOLINDEX_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, VOLINDEX_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("INDEX_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(INDEX_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, INDEX_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("WASTEDATA_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(WASTEDATA_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, WASTEDATA_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("RECYCLE_DATA_RESTORE"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(RECYCLE_DATA_RESTORE);
		CreateResource(m_pFrameWnd, RECYCLE_DATA_RESTORE);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("REQUEST_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(REQUEST_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, REQUEST_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	
	else if (strCommandAlias == _T("SPECIESINFO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(SPECIESINFO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, SPECIESINFO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("MANAGENO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(MANAGENO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, MANAGENO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("MARCDATA_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(MARCDATA_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, MARCDATA_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
		
	if (strCommandAlias == _T("검색"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_SEARCH();
	else if (strCommandAlias == _T("구입정보생성"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("색인재구축"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("책색인동기화"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("책부록플래그설정"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST1();
	else if (strCommandAlias == _T("쓰레기자료삭제"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("재활용자료복원"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("비치희망색인재구축"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("종정보오류수정"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("관리번호오류수정"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("마크수정"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("새로고침"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_REFRESH();
	else if (strCommandAlias == _T("상세보기"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_VIEWERRORINFO();
	else if (strCommandAlias == _T("닫기"))
		if(m_pFrameWnd) 
			::PostMessage( m_pFrameWnd->GetSafeHwnd(), WM_CLOSE, 0,0);	
		
	if (ids < 0)
		return FALSE;
	else
		return TRUE;
}

INT CErrorAdjustMgr::InitManager(CESL_Mgr *pParentESLMgr)
{
	// Check 
	if (pParentESLMgr == NULL) 
	{
		AfxMessageBox(_T("부모 ESL Mgr이 NULL 입니다!"));
		return -1;
	}
	if (pParentESLMgr->m_pInfo == NULL) return -1;
	if (pParentESLMgr->m_pInfo->pMarcMgr == NULL) return -1;
	
	INT ids = -1;
	
	// ErrorAdjustDlg 생성 (new 만 한다)
	if (CreateErrorAdjustDlg(pParentESLMgr) < 0) return -1;
	
	return 0;
}

INT CErrorAdjustMgr::CreateErrorAdjustDlg(CESL_Mgr *pParentESLMgr)
{
	if (pParentESLMgr == NULL) return -1;
	
	// ErrorAdjustDlg
	if (m_pErrorAdjustDlgMgr)
	{
		delete m_pErrorAdjustDlgMgr;
		m_pErrorAdjustDlgMgr = NULL;
	}
	
	m_pErrorAdjustDlgMgr = new CErrorAdjustDlgMgr(pParentESLMgr);
	m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg();
	return 0;
}
