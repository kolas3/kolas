// OrderCancelDivnoSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderCancelDivnoSelectDlg.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderCancelDivnoSelectDlg dialog


COrderCancelDivnoSelectDlg::COrderCancelDivnoSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COrderCancelDivnoSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGroupCollection = NULL;
	m_pOrderCancelGroupCollection = NULL;
	m_pGroup = NULL;
	m_pComboMgr = NULL;
}

COrderCancelDivnoSelectDlg::~COrderCancelDivnoSelectDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}

}

VOID COrderCancelDivnoSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderCancelDivnoSelectDlg, CDialog)
	//{{AFX_MSG_MAP(COrderCancelDivnoSelectDlg)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_INFO, OnSelchangecmbDIVNOINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderCancelDivnoSelectDlg message handlers

BOOL COrderCancelDivnoSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_취소차수선택")) < 0 ) 
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_취소차수선택"));
	m_pDM = FindDM(_T("DM_주문자료관리"));
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pOrderCancelGroupCollection = new CGroupCollection(this);

	if( m_pCM == NULL || m_pDM == NULL || m_pComboMgr == NULL || m_pOrderCancelGroupCollection == NULL || 
		Init() == FALSE )
	{
		ESLAfxMessageBox(_T("주문취소 선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID COrderCancelDivnoSelectDlg::OnOK() 
{
	if( m_pGroup == NULL )
	{
		ESLAfxMessageBox(_T("선정 차수정보가 올바르지 않습니다."));
		return ;
	}

	CString strMsg;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Group"), (DWORD*)m_pGroup);
	INT ids = api.SPFExecutePath(_T("차수자료주문취소"));
	if( ids < 0 )
	{
		strMsg.Format(_T("%s년도 %s 차수자료를 주문취소 할 수 없습니다."), m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
		return ;
	}
	else
	{
		CGroupInfo Group;
		m_pGroup->CopyData(&Group);
		m_pOrderCancelGroupCollection->AddGroupInfo(&Group);
		strMsg.Format(_T("차수번호 [%s]번을 주문취소하였습니다."), m_pGroup->GetGroupInfo());
		m_pGroupCollection->DeleteThisGroupInfo(m_pGroup);
		ESLAfxMessageBox(strMsg);
	}

	if( Init() == FALSE )
	{
		EndDialog(IDOK);
	}
}

BOOL COrderCancelDivnoSelectDlg::Init()
{
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	if( nGroupCnt == 0 )	return FALSE;

	CStringArray arrGroupInfo;
	CString strTmpData, strWorkCode;

	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pCM->SetControlMgrData(_T("업무구분"), strWorkCode);
	m_pGroupCollection->GetGroupInfo(&arrGroupInfo, strTmpData);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &arrGroupInfo, _T("차수정보"));

	return TRUE;
}

VOID COrderCancelDivnoSelectDlg::OnSelchangecmbDIVNOINFO() 
{
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("차수정보"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

HBRUSH COrderCancelDivnoSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}