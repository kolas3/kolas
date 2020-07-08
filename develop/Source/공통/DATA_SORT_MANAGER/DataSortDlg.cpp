// DataSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataSortDlg.h"
#include "DataSortElementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSortDlg dialog


CDataSortDlg::CDataSortDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDataSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDM = NULL;
	m_pComboMgr  = NULL;
	m_pCM = NULL;
	m_pItemSet = NULL;
	m_nOpenMode = 0;
}

CDataSortDlg::~CDataSortDlg()
{
	if(m_pComboMgr) 
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CDataSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSortDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSortDlg)
	ON_BN_CLICKED(IDC_btnSET_ORDER_ELEMENT, OnbtnSETORDERELEMENT)
	ON_BN_CLICKED(IDC_btnSAVE_DEFAULT_VALUE, OnbtnSAVEDEFAULTVALUE)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_1, OnSelchangecmbORDERSORT1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_2, OnSelchangecmbORDERSORT2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_3, OnSelchangecmbORDERSORT3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_4, OnSelchangecmbORDERSORT4)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_1, OnSelchangecmbORDERSORTCOND1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_2, OnSelchangecmbORDERSORTCOND2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_3, OnSelchangecmbORDERSORTCOND3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_COND_4, OnSelchangecmbORDERSORTCOND4)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSortDlg message handlers

BOOL CDataSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_주문일련번호")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_주문일련번호"));
	
	if(m_pItemSet == NULL)
	{
		ESLAfxMessageBox(_T("초기값 지정이 올바르지 않습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!Init())
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataSortDlg::Init()
{
	m_pComboMgr = new CMulitComboBoxManager(this);
	
	if(m_nOpenMode == 0) return FALSE;
	if(m_nOpenMode < 0) GetDlgItem(IDOK)->ShowWindow(FALSE);
	else if(m_nOpenMode > 0) GetDlgItem(IDOK)->ShowWindow(TRUE);
	
	if(!m_pItemSet->IsInit()) return FALSE;
	
	if(m_pItemSet->GetDataCount() == 0)
	{
		ESLAfxMessageBox(_T("설정된 정렬항목정보가 없습니다. 먼저 정렬항목을 설정해 주십시요."));
		GetDlgItem(IDC_btnSAVE_DEFAULT_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return TRUE;
	}
	INT ids = SetComboData();
	if(ids < 0) return FALSE;
	
	return TRUE;
}

INT CDataSortDlg::SetComboData()
{
	CString strInitValue;
	CStringArray tmpArr;	tmpArr.RemoveAll();
	m_pItemSet->SortDM(_T("우선순위"), FALSE);
	INT ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 1, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("제1조건"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 2, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("제2조건"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 3, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("제3조건"));
	ids = m_pItemSet->GetSortItemOfPriority(&tmpArr, 4, strInitValue);
	if(ids < 0) return ids;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strInitValue, &tmpArr, _T("제4조건"));
	
	CString strSortCond;
	tmpArr.RemoveAll();	
	tmpArr.Add(_T("내림차순"));		tmpArr.Add(_T("오름차순"));
	if(m_pItemSet->GetSortDirection(1) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("제1정렬조건"));
	if(m_pItemSet->GetSortDirection(2) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("제2정렬조건"));
	if(m_pItemSet->GetSortDirection(3) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("제3정렬조건"));
	if(m_pItemSet->GetSortDirection(4) == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &tmpArr, _T("제4정렬조건"));

	return 0;
}

VOID CDataSortDlg::OnbtnSETORDERELEMENT() 
{
	// TODO: Add your control notification handler code here
	CDataSortElementDlg dlg(this);
	dlg.SetSortItemSet(m_pItemSet);
	if(IDOK == dlg.DoModal())
	{
		GetDlgItem(IDC_btnSAVE_DEFAULT_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		INT ids = SetComboData();
		if(ids < 0) return ;
	}
}

VOID CDataSortDlg::OnbtnSAVEDEFAULTVALUE() 
{
	// TODO: Add your control notification handler code here
	INT ids = m_pItemSet->WriteSortItem();
	if(ids < 0)
		ESLAfxMessageBox(_T("지정하신 정렬항목 기본값을 저장할 수 없습니다."));
	else
		ESLAfxMessageBox(_T("지정하신 정렬항목 기본값을 저장하였습니다."));
}

VOID CDataSortDlg::OnOK() 
{
	INT ids = m_pItemSet->DataSort(m_pMainDM);
	if(ids < 0)
		ESLAfxMessageBox(_T("자료를 정렬할 수 없습니다."));
	else
		ESLAfxMessageBox(_T("자료를 정렬하였습니다."));
	
	CDialog::OnOK();
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT1() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("제1조건"), 1);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT2() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("제2조건"), 2);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT3() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("제3조건"), 3);	
}

VOID CDataSortDlg::OnSelchangecmbORDERSORT4() 
{
	// TODO: Add your control notification handler code here
	RestructSortItem(_T("제4조건"), 4);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND1() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("제1정렬조건"), 1);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND2() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("제2정렬조건"), 2);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND3() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("제3정렬조건"), 3);
}

VOID CDataSortDlg::OnSelchangecmbORDERSORTCOND4() 
{
	// TODO: Add your control notification handler code here
	SetSortDirection(_T("제4정렬조건"), 4);
}

INT CDataSortDlg::RestructSortItem(CString strAlias, INT nPriority)
{
	CString strTmpData;
	m_pCM->GetControlMgrData(strAlias, strTmpData);
	INT ids = m_pItemSet->SetSortPriority(strTmpData, nPriority);
	if(ids < 0)
	{
		ESLAfxMessageBox(strAlias + _T("을 설정할 수 없습니다."));	
		return -1;
	}
	ids = SetComboData();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("정렬항목을 출력할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CDataSortDlg::SetSortDirection(CString strAlias, INT nPriority)
{
	CString strTmpData;
	INT nSortDirection;
	m_pCM->GetControlMgrData(strAlias, strTmpData);
	if(strTmpData == _T("내림차순")) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pItemSet->SetSortDirection(nSortDirection, nPriority);
	if(ids < 0)
	{
		ESLAfxMessageBox(strAlias + _T("을 설정할 수 없습니다."));	
		return -1;
	}	
	return 0;
}

HBRUSH CDataSortDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
