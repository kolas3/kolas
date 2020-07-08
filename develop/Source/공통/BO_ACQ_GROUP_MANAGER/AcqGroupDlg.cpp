// AcqGroupDlg.cpp: implementation of the CAcqGroupDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupDlg::CAcqGroupDlg(UINT nIDD, CESL_Mgr * pParentMgr) : CESL_Mgr(nIDD, pParentMgr)
{
	m_pResourceMgr = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pGroupMgr = NULL;
	m_pGroupCollection = NULL;
}

CAcqGroupDlg::~CAcqGroupDlg()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pGroupMgr)
	{
		delete m_pGroupMgr;
		m_pGroupMgr = NULL;
	}
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
}

INT CAcqGroupDlg::ShowSearchDlg(CESL_Mgr * pParentMgr)
{
	if(m_pSearchDlg == NULL) return -1;
	if(!m_pSearchDlg->GetCreate())
		m_pSearchDlg->Create(pParentMgr);
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	m_pSearchDlg->CenterWindow();
	return 0;
}

INT CAcqGroupDlg::CMD_VIEW_GROUPDATA()
{
	return 0;
}

INT CAcqGroupDlg::CMD_GROUP_CHANGE_STATUS()
{
	return 0;
}

INT CAcqGroupDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	return 0;
}

INT CAcqGroupDlg::CheckSelectDataValid()
{
	if(m_pGrid == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

INT CAcqGroupDlg::DeleteGroup(CESL_DataMgr *pDM_Delete, CString strPath)
{
	if(m_pGroupMgr == NULL) return -1;
	if(pDM_Delete == NULL) return -1;

	m_pGroupMgr->InitTempData(); m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("DELETE_DM"), (DWORD*)pDM_Delete);
	m_pGroupMgr->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	INT ids = m_pGroupMgr->SPFExecutePath(strPath);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정하신 그룹정보를 삭제할 수 없습니다."));
		return -1;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("삭제할 그룹정보가 없습니다."));
		return 0;
	}
	else if(ids == 0) m_pGrid->Display();	
	return 0;
}

CString CAcqGroupDlg::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CAcqGroupDlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}
