// WorkDupSearchSelect.cpp : implementation file
//

#include "stdafx.h"
#include "WorkDupSearchSelect.h"
#include "../../../공통/K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkDupSearchSelect dialog


CWorkDupSearchSelect::CWorkDupSearchSelect(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CWorkDupSearchSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorkDupSearchSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGroupCollection = NULL;
	m_pCM = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pParentDM = NULL;
	m_pParentCM = NULL;
}


VOID CWorkDupSearchSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkDupSearchSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkDupSearchSelect, CDialog)
	//{{AFX_MSG_MAP(CWorkDupSearchSelect)
	ON_CBN_SELCHANGE(IDC_cmbHOLD_DIVINFO, OnSelchangecmbHOLDDIVINFO)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkDupSearchSelect message handlers

VOID CWorkDupSearchSelect::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearchByDivno(_T("차수중복조사"), m_pParentDM->DB_MGR_ALIAS, m_pParentCM->CM_ALIAS, _T("MainGrid"), m_pGroupCollection->GetCurSelGroupInfo(), m_pGroupCollection);
}

VOID CWorkDupSearchSelect::OnSelchangecmbHOLDDIVINFO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("그룹정보"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	SetGroupDataCount();	
}

BOOL CWorkDupSearchSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_중복본조사_차수선택")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_중복본조사_차수선택"));

	SetGroupInfo();
	SetGroupDataCount();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CWorkDupSearchSelect::SetGroupDataCount()
{
	INT cnt = m_pGroupCollection->GetGroupDataCount(TRUE, m_pParentDM);
	CString strTmpData ;
	strTmpData.Format(_T("%d"), cnt);
	m_pCM->SetControlMgrData(_T("종수"), strTmpData);
}

VOID CWorkDupSearchSelect::SetGroupInfo()
{
	CStringArray tmpArray;
	m_pGroupCollection->GetDetailGroupInfo(&tmpArray);
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
}

UINT CWorkDupSearchSelect::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}