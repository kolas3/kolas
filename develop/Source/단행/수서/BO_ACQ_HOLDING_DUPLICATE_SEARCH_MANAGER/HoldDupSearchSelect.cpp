// HoldDupSearchSelect.cpp : implementation file
//

#include "stdafx.h"
#include "HoldDupSearchSelect.h"
#include "../../../공통/K2UP_Common/MulitComboBoxManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoldDupSearchSelect dialog


CHoldDupSearchSelect::CHoldDupSearchSelect(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CHoldDupSearchSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoldDupSearchSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGroupCollection = NULL;
	m_pCM = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pParentDM = NULL;
	m_pParentCM = NULL;
}

CHoldDupSearchSelect::~CHoldDupSearchSelect()
{
}

VOID CHoldDupSearchSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldDupSearchSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoldDupSearchSelect, CDialog)
	//{{AFX_MSG_MAP(CHoldDupSearchSelect)
	ON_CBN_SELCHANGE(IDC_cmbHOLD_DIVINFO, OnSelchangecmbHOLDDIVINFO)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoldDupSearchSelect message handlers

BOOL CHoldDupSearchSelect::OnInitDialog() 
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

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CHoldDupSearchSelect::SetGroupDataCount()
{
	INT cnt = m_pGroupCollection->GetGroupDataCount(TRUE, m_pParentDM);
	CString strTmpData ;
	strTmpData.Format(_T("%d"), cnt);
	m_pCM->SetControlMgrData(_T("종수"), strTmpData);
}

VOID CHoldDupSearchSelect::SetGroupInfo()
{
	CStringArray tmpArray;
	m_pGroupCollection->GetDetailGroupInfo(&tmpArray);
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
}

VOID CHoldDupSearchSelect::OnSelchangecmbHOLDDIVINFO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("그룹정보"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	SetGroupDataCount();
}

VOID CHoldDupSearchSelect::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearchByDivno(_T("차수복본조사"), m_pParentDM->DB_MGR_ALIAS, m_pParentCM->CM_ALIAS, _T("MainGrid"), m_pGroupCollection->GetCurSelGroupInfo(), m_pGroupCollection);
}

UINT CHoldDupSearchSelect::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

HBRUSH CHoldDupSearchSelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
