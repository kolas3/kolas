// MergeGroup_Select_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MergeGroup_Select_Dlg.h"
#include "../K2UP_Common/MulitComboBoxManager.h"
#include "DivisionNo_Insert_Dlg.h"
#include "Group_Insert_Dlg.h"
#include "RequisitionNo_Insert_Dlg.h"
#include "OtherGroupNo_Insert_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMergeGroup_Select_Dlg dialog


CMergeGroup_Select_Dlg::CMergeGroup_Select_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMergeGroup_Select_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMergeGroup_Select_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGroupCollection = NULL;
	m_nGroupType = GROUP_NO;
	m_pCM = NULL;
	m_pGroup = NULL;

	m_pDM = NULL;
}

CMergeGroup_Select_Dlg::~CMergeGroup_Select_Dlg()
{
}

VOID CMergeGroup_Select_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMergeGroup_Select_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMergeGroup_Select_Dlg, CDialog)
	//{{AFX_MSG_MAP(CMergeGroup_Select_Dlg)
	ON_BN_CLICKED(IDC_btnMSEL_GROUP_CREATE, OnbtnMSELGROUPCREATE)
	ON_CBN_SELCHANGE(IDC_cmbMSEL_GROUP_INFO, OnSelchangecmbMSELGROUPINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMergeGroup_Select_Dlg message handlers

BOOL CMergeGroup_Select_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_통합그룹선택")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_통합그룹선택"));

	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMergeGroup_Select_Dlg::OnbtnMSELGROUPCREATE() 
{
	// TODO: Add your control notification handler code here
	if(m_nGroupType == DIVNO)
	{
		InsertDivNoInfo();
	}
	else if(m_nGroupType == RECEIPT_NO)
	{
		InsertReceiptNoInfo();
	}
	else if(m_nGroupType == GROUP_NO)
	{
		InsertGroupInfo();
	}
	else if(m_nGroupType == OTHER_GROUP_NO)
		InsertOtherGroupInfo();
}

VOID CMergeGroup_Select_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pGroup == NULL) 
	{
		ESLAfxMessageBox(_T("선정한 그룹정보가 올바르지 않습니다."));
		return;
	}
//	ESLAfxMessageBox(m_pGroup->GetGroupInfo());
	CDialog::OnOK();
}

BOOL CMergeGroup_Select_Dlg::Init()
{
	if(m_nGroupType == DIVNO)
	{
		SetWindowText(_T("통합차수선택"));
		GetDlgItem(IDC_stcMSEL_GROUP_INFO)->SetWindowText(_T("차수정보"));
	}
	else if(m_nGroupType == RECEIPT_NO)
	{
		SetWindowText(_T("통합접수번호선택"));
		GetDlgItem(IDC_stcMSEL_GROUP_INFO)->SetWindowText(_T("접수번호정보"));
	}
	else if(m_nGroupType == GROUP_NO)
	{
		SetWindowText(_T("통합그룹선택"));
		GetDlgItem(IDC_stcMSEL_GROUP_INFO)->SetWindowText(_T("그룹정보"));
	}
	else if(m_nGroupType == OTHER_GROUP_NO)
	{
		SetWindowText(_T("통합그룹선택"));
		GetDlgItem(IDC_stcMSEL_GROUP_INFO)->SetWindowText(_T("그룹번호정보"));
	}

	INT nGroupCount = m_pGroupCollection->GetGroupCount();
	if(nGroupCount < 2) return FALSE;
	if(m_pDM == NULL) return FALSE;
	
	INT ids = SetGroupSelectData();
	if(ids < 0) return FALSE;

	return TRUE;
}

INT CMergeGroup_Select_Dlg::InsertDivNoInfo()
{
	CString strGroupNo, strWorkNo, strLastNo, strWorkCode;
	CESL_DataMgr * pWorkNoDM = FindDM(_T("DM_작업번호관리"));
	CDivisionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetDivNoDMPtr(m_pDM);
	dlg.SetWorkNoDMPtr(pWorkNoDM);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetWorkStatus(m_strWorkStatus);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	if(IDOK == dlg.DoModal()) 
	{
		dlg.GetInsertInfo(strGroupNo, strWorkNo, strLastNo, strWorkCode);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo, strWorkCode);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		SetGroupSelectData();
		return 0;
	}
	return -1;
}

INT CMergeGroup_Select_Dlg::InsertReceiptNoInfo()
{
	CRequisitionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo;
		dlg.GetInsertInfo(strAddGroupNo);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		SetGroupSelectData();
	}
	return 0;
}

INT CMergeGroup_Select_Dlg::InsertGroupInfo()
{
	CGroup_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo, strAddGroupName, strAddLastSerialNo;
		dlg.GetInsertInfo(strAddGroupNo, strAddGroupName, strAddLastSerialNo);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupName, strAddGroupNo);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		SetGroupSelectData();
	}
	return 0;
}

INT CMergeGroup_Select_Dlg::InsertOtherGroupInfo()
{
	COtherGroupNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetAcqCode(m_pGroupCollection->GetAcqCode());
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo, strAddAcqCode;
		dlg.GetInsertInfo(strAddGroupNo, strAddAcqCode);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo, strAddAcqCode);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		SetGroupSelectData();
	}
	return 0;
}

INT CMergeGroup_Select_Dlg::SetGroupSelectData()
{
	CMulitComboBoxManager mgr(this);
	CStringArray tmpArray;
	CString strTmpData;
	m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("그룹선택"));
	return 0;
}

VOID CMergeGroup_Select_Dlg::OnSelchangecmbMSELGROUPINFO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("그룹선택"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

CGroupInfo * CMergeGroup_Select_Dlg::GetSelGroup()
{
	return m_pGroup;
}

HBRUSH CMergeGroup_Select_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
