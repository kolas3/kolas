// AcqGroupMgr.cpp: implementation of the CAcqGroupMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupMgr.h"
#include "AcqGroupFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupMgr::CAcqGroupMgr()
{
	m_pAcqGroupResouceMgr = NULL;
	m_pFrameWnd = NULL;
	m_pAcqGroupDlgMgr = NULL;
	m_nGroupOpenType = GROUP_OPEN_TYPE_INVALID;
}

CAcqGroupMgr::~CAcqGroupMgr()
{
	if(m_pAcqGroupDlgMgr)
	{
		delete m_pAcqGroupDlgMgr;
		m_pAcqGroupDlgMgr = NULL;
	}
	if(m_pAcqGroupResouceMgr)
	{
		delete m_pAcqGroupResouceMgr;
		m_pAcqGroupResouceMgr = NULL;
	}
}

INT CAcqGroupMgr::CreateResource(CFrameWnd *pFrameWnd, ACQ_GROUP_TYPE nAcqGroupType/*=ACQ_GROUP_INVALID*/)
{
	
	m_pFrameWnd = pFrameWnd;
	
	if (m_pAcqGroupDlgMgr == NULL) return -1;
	m_pAcqGroupDlgMgr->SetMainWnd(pFrameWnd);
	
	if (m_pAcqGroupResouceMgr) delete m_pAcqGroupResouceMgr;
	
	m_pAcqGroupResouceMgr = new CAcqGroupResourceMgr(m_pAcqGroupDlgMgr->GetAcqGroupDlg());
	m_pAcqGroupResouceMgr->SetGroupOpenType(m_nGroupOpenType);
	if(nAcqGroupType == ACQ_GROUP_INVALID)
	{
		if(m_nGroupOpenType == ACQOBJ_GROUP_OPEN) nAcqGroupType = ACQ_OBJ_GROUP;
		else if(m_nGroupOpenType == ACQ_GROUP_OPEN) nAcqGroupType = ACQ_PURCHASE_GROUP;
		else if(m_nGroupOpenType == PURCHASE_GROUP_OPEN) nAcqGroupType = ACQ_PURCHASE_GROUP;
		else if(m_nGroupOpenType == DONATE_GROUP_OPEN) nAcqGroupType = ACQ_DONATE_GROUP;
	}

	m_pAcqGroupResouceMgr->CreateResource(pFrameWnd, nAcqGroupType);
	
	if(m_pAcqGroupDlgMgr->GetAcqGroupDlg())
	{
		m_pAcqGroupDlgMgr->GetAcqGroupDlg()->SetResourceMgr(m_pAcqGroupResouceMgr);
	}	
	
	return 0;
}

BOOL CAcqGroupMgr::OnCmdMsg(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	return m_pAcqGroupResouceMgr->OnCmdMsg(nID);
}

BOOL CAcqGroupMgr::OnCommand(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	CString strCommandAlias;
	
	BOOL bReturn = m_pAcqGroupResouceMgr->OnCommand(nID, strCommandAlias);
	
	if (strCommandAlias.IsEmpty()) return bReturn;
	
	INT ids = -1;
	
	CAcqGroupFormView *pView = (CAcqGroupFormView *) m_pFrameWnd->GetActiveView();
	if (strCommandAlias == _T("ACQ_OBJ_GROUP"))
	{
		m_pAcqGroupDlgMgr->CreateAcqGroupDlg(ACQ_OBJ_GROUP);
		CreateResource(m_pFrameWnd, ACQ_OBJ_GROUP);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("ACQ_PURCHASE_GROUP"))
	{
		m_pAcqGroupDlgMgr->CreateAcqGroupDlg(ACQ_PURCHASE_GROUP);
		CreateResource(m_pFrameWnd, ACQ_PURCHASE_GROUP);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("ACQ_DONATE_GROUP"))
	{
		m_pAcqGroupDlgMgr->CreateAcqGroupDlg(ACQ_DONATE_GROUP);
		CreateResource(m_pFrameWnd, ACQ_DONATE_GROUP);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("ACQ_OTHER_GROUP"))
	{
		m_pAcqGroupDlgMgr->CreateAcqGroupDlg(ACQ_OTHER_GROUP);
		CreateResource(m_pFrameWnd, ACQ_OTHER_GROUP);
		pView->CreateChildWindow();
		
		ids = 0;
	}
		
	if (strCommandAlias == _T("검색"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_SEARCH();
	else if (strCommandAlias == _T("입력"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_INSERT();
	else if (strCommandAlias == _T("수정"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_UPDATE();
	else if (strCommandAlias == _T("삭제"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_DELETE();
	else if (strCommandAlias == _T("차수정보보기"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_VIEW_GROUPDATA();
	else if (strCommandAlias == _T("접수번호정보보기"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_VIEW_GROUPDATA();
	else if (strCommandAlias == _T("수서그룹정보보기"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_VIEW_GROUPDATA();
	else if (strCommandAlias == _T("그룹통합"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_MERGE();
	else if (strCommandAlias == _T("차수통합"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_MERGE();
	else if (strCommandAlias == _T("접수번호통합"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_MERGE();
	else if (strCommandAlias == _T("수서그룹통합"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_MERGE();
	else if (strCommandAlias == _T("그룹분리"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_DIVIDE();
	else if (strCommandAlias == _T("차수분리"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_DIVIDE();
	else if (strCommandAlias == _T("접수번호분리"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_DIVIDE();
	else if (strCommandAlias == _T("수서그룹분리"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_DIVIDE();
	else if (strCommandAlias == _T("그룹복사"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_COPY();
	else if (strCommandAlias == _T("차수복사"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_COPY();
	else if (strCommandAlias == _T("접수번호복사"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_COPY();
	else if (strCommandAlias == _T("수서그룹복사"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_COPY();
	else if (strCommandAlias == _T("차수상태변경"))
		ids = m_pAcqGroupDlgMgr->GetAcqGroupDlg()->CMD_GROUP_CHANGE_STATUS();

	else if (strCommandAlias == _T("닫기"))
		if(m_pFrameWnd) 
			::PostMessage( m_pFrameWnd->GetSafeHwnd(), WM_CLOSE, 0,0);	
		
	if (ids < 0)
		return FALSE;
	else
		return TRUE;
}

INT CAcqGroupMgr::InitManager(CESL_Mgr *pParentESLMgr, ACQ_GROUP_OPEN_TYPE nGroupOpenType)
{
	if (pParentESLMgr == NULL) 
	{
		AfxMessageBox(_T("부모 ESL Mgr이 NULL 입니다!"));
		return -1;
	}
	if (pParentESLMgr->m_pInfo == NULL) return -1;
	if (pParentESLMgr->m_pInfo->pMarcMgr == NULL) return -1;
	
	m_nGroupOpenType = nGroupOpenType;
	
	if (CreateAcqGroupDlg(pParentESLMgr) < 0) return -1;
	
	return 0;
}

INT CAcqGroupMgr::CreateAcqGroupDlg(CESL_Mgr *pParentESLMgr)
{
	if (pParentESLMgr == NULL) return -1;
	
	if (m_pAcqGroupDlgMgr)
	{
		delete m_pAcqGroupDlgMgr;
		m_pAcqGroupDlgMgr = NULL;
	}
	
	m_pAcqGroupDlgMgr = new CAcqGroupDlgMgr(pParentESLMgr);
	ACQ_GROUP_TYPE nAcqGroupType;
	if(m_nGroupOpenType == ACQOBJ_GROUP_OPEN) nAcqGroupType = ACQ_OBJ_GROUP;
	else if(m_nGroupOpenType == ACQ_GROUP_OPEN) nAcqGroupType = ACQ_PURCHASE_GROUP;
	else if(m_nGroupOpenType == PURCHASE_GROUP_OPEN) nAcqGroupType = ACQ_PURCHASE_GROUP;
	else if(m_nGroupOpenType == DONATE_GROUP_OPEN) nAcqGroupType = ACQ_DONATE_GROUP;
	m_pAcqGroupDlgMgr->CreateAcqGroupDlg(nAcqGroupType);
	return 0;
}
