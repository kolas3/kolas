// CheckinVolMgr.cpp: implementation of the CCheckinVolMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckinVolMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckinVolMgr::CCheckinVolMgr()
{
	m_pCheckinVolESLMgr = NULL;
	m_pCheckinVolResouceMgr = NULL;

	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nACQ_TYPE = 1; //default
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");
	m_bIsLightVersion = FALSE;

}

CCheckinVolMgr::~CCheckinVolMgr()
{
	// Resource Mgr
	if (m_pCheckinVolResouceMgr) 
	{
		delete m_pCheckinVolResouceMgr;
		m_pCheckinVolResouceMgr = NULL;
	}
	
	// ESL Mgr
	if (m_pCheckinVolESLMgr)
	{
		delete m_pCheckinVolESLMgr;
		m_pCheckinVolESLMgr = NULL;
	}
	
}

INT CCheckinVolMgr::SetParentInfo(CESL_DataMgr *pParentDM, 
								  CESL_Grid *pParentGrid,
								  INT nACQ_TYPE,
								  CString strACQ_YEAR,
								  CString strRECEIPT_NO,
								  BOOL bIsLightVersion)
{
	
	m_pParentDM = pParentDM;
	m_pParentGrid = pParentGrid;
	m_nACQ_TYPE = nACQ_TYPE;
	m_strACQ_YEAR = strACQ_YEAR;
	m_strRECEIPT_NO = strRECEIPT_NO;
	m_bIsLightVersion = bIsLightVersion;

	return 0;
}


INT CCheckinVolMgr::InitManager(CESL_Mgr *pParentESLMgr)
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

	// 권별 체크인 ESL Mgr 생성 (new 만 한다)
	if (CreateCheckinVolESLMgr(pParentESLMgr) < 0) return -1;


	// 부모 정보 체크 -- 기본적인 검증을 한다 
	if (!m_pParentDM || !m_pParentGrid)
		return -1;
	
	
	//부모 그리드에서 선정정보를 얻어온다
	if(!m_pParentGrid) {
		AfxMessageBox(_T("부모 그리드를 찾을 수 없음니다!"));
		return -1;
	}

	if(!m_pParentDM) {
		AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
		return -2;
	}

	
	INT nHead;
	m_pParentGrid->SelectMakeList();
	nHead = m_pParentGrid->SelectGetHeadPosition();
	if(nHead<0) {
		nHead = m_pParentGrid->GetIdx();
		//AfxMessageBox(_T("선정된 정보가 없습니다!"));
		//return -3;
	}
	
	//ESL dialog에게 부모 정보를 넘겨준다
	if(!m_pCheckinVolESLMgr) {
		AfxMessageBox(_T("ESL dialog new fail"));
		return -4;
	}

	m_pCheckinVolESLMgr->SetParentInfo(m_pParentDM, 
									   m_pParentGrid, 
									   m_nACQ_TYPE,
									   m_strACQ_YEAR,
									   m_strRECEIPT_NO,
									   m_bIsLightVersion);
		
	return 0;
}



INT CCheckinVolMgr::CreateCheckinVolESLMgr(CESL_Mgr *pParentESLMgr)
{
	if (pParentESLMgr == NULL) return -1;

	// CheckIn Vol ESL Mgr
	if (m_pCheckinVolESLMgr)
	{
		m_pCheckinVolESLMgr->DestroyWindow();
		delete m_pCheckinVolESLMgr;
	}
	
	m_pCheckinVolESLMgr = new CCheckinVolESL(pParentESLMgr);
	

	return 0;
}

INT CCheckinVolMgr::CreateResource(CFrameWnd *pFrameWnd)
{
	
	m_pFrameWnd = pFrameWnd;

	if (m_pCheckinVolESLMgr == NULL) return -1;
	m_pCheckinVolESLMgr->pMain = pFrameWnd;


	// 리소스 매니져 생성
	if (m_pCheckinVolResouceMgr) delete m_pCheckinVolResouceMgr;

	m_pCheckinVolResouceMgr = new CCheckinVolResouceMgr(m_pCheckinVolESLMgr);
	m_pCheckinVolResouceMgr->CreateResource(pFrameWnd);
	
	
	// 리소스 매니져의 포인터를 ESL에게 넘겨준다. 선택된 항목의 종정보를 보여준다 
	if(m_pCheckinVolESLMgr)
	{
		m_pCheckinVolESLMgr->m_pResourceMgr = m_pCheckinVolResouceMgr;

		if(m_pCheckinVolESLMgr->m_nCurrentParentIdx >= 0)
		{
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_nACQ_TYPE = m_nACQ_TYPE;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_strRECEIPT_NO = m_strRECEIPT_NO;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_strACQ_YEAR = m_strACQ_YEAR;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_pCheckinVolESLMgr->m_nCurrentParentIdx);
		}
		CString strQuery, strUseCoverImage;
		strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"));
		m_pParentDM->GetOneValueQuery(strQuery,strUseCoverImage);
		if( strUseCoverImage.IsEmpty() || strUseCoverImage == _T("N"))
			m_pCheckinVolESLMgr->m_pResourceMgr->HideToolBarButton(_T("표지정보"));
	}


	return 0;
}

BOOL CCheckinVolMgr::OnCmdMsg(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;

	return m_pCheckinVolResouceMgr->OnCmdMsg(nID);
}

BOOL CCheckinVolMgr::OnCommand(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;

	CString strCommandAlias;

	BOOL bReturn = m_pCheckinVolResouceMgr->OnCommand(nID, strCommandAlias);

	if (strCommandAlias.IsEmpty()) return bReturn;

	INT ids = -1;
	
	//종정보보기
	if (strCommandAlias == _T("VIEW_IDD_CHECKIN_VOL_SPECIES"))
	{
		if (m_pCheckinVolResouceMgr->m_wndSpeciesInfo.IsWindowVisible())
			m_pCheckinVolResouceMgr->m_wndSpeciesInfo.ShowWindow(SW_HIDE);
		else
			m_pCheckinVolResouceMgr->m_wndSpeciesInfo.ShowWindow(SW_SHOW);

		m_pFrameWnd->DockControlBar(&m_pCheckinVolResouceMgr->m_wndSpeciesInfo, AFX_IDW_DOCKBAR_BOTTOM);
		m_pFrameWnd->RecalcLayout();

		ids = 0;
	}
	

	if (strCommandAlias == _T("입수처리"))
		ids = m_pCheckinVolESLMgr->CMD_CHECKIN();
	else if (strCommandAlias == _T("결호처리"))
		ids = m_pCheckinVolESLMgr->CMD_MISS();
	else if (strCommandAlias == _T("권호입력"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_INSERT();
	else if (strCommandAlias == _T("권호수정"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_MODIFY();
	else if (strCommandAlias == _T("권호삭제"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_DELETE();
	else if (strCommandAlias == _T("합병호"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_MERGE();
	else if (strCommandAlias == _T("미발행"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_NON_PUBLISH();
	else if (strCommandAlias == _T("예측정보"))
		ids = m_pCheckinVolESLMgr->CMD_EXP_INFO();
	else if (strCommandAlias == _T("목차편집"))
		ids = m_pCheckinVolESLMgr->CMD_CONTENTS_INFO();
	else if (strCommandAlias == _T("새로고침"))
		ids = m_pCheckinVolESLMgr->CMD_REFRESH();
	else if (strCommandAlias == _T("추가예측"))
		ids = m_pCheckinVolESLMgr->CMD_MORE_EXP();
	else if (strCommandAlias == _T("설정"))
		ids = m_pCheckinVolESLMgr->CMD_MORE_EXP_SETTING();
	else if (strCommandAlias == _T("접수번호"))
		ids = m_pCheckinVolESLMgr->CMD_RECEIPT_NO();
	else if (strCommandAlias == _T("표지정보"))
		ids = m_pCheckinVolESLMgr->CMD_COVER_INFO();
	else if (strCommandAlias == _T("이전"))
		ids = m_pCheckinVolESLMgr->CMD_PREV();
	else if (strCommandAlias == _T("다음"))
		ids = m_pCheckinVolESLMgr->CMD_NEXT();
	else if (strCommandAlias == _T("저장"))
		ids = m_pCheckinVolESLMgr->CMD_SAVE();
	else if (strCommandAlias == _T("출력"))
		ids = m_pCheckinVolESLMgr->CMD_PRINT();
	else if (strCommandAlias == _T("입수현황"))
		ids = m_pCheckinVolESLMgr->CMD_PROCESSPRINT();
	else if (strCommandAlias == _T("닫기"))
		if(m_pFrameWnd) ::PostMessage( m_pFrameWnd->GetSafeHwnd(), WM_CLOSE, 0,0);	
			
	
	
	
	if (ids < 0)
		return FALSE;
	else
		return TRUE;
}