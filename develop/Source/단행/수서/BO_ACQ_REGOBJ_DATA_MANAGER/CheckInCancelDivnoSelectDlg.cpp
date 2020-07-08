// CheckInCancelDivnoSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckInCancelDivnoSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDivnoSelectDlg dialog


CCheckInCancelDivnoSelectDlg::CCheckInCancelDivnoSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckInCancelDivnoSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckInCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCheckInCancelDivnoSelectDlg::~CCheckInCancelDivnoSelectDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}

	m_Dlg_brush.DeleteObject();
}


VOID CCheckInCancelDivnoSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckInCancelDivnoSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckInCancelDivnoSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckInCancelDivnoSelectDlg)
	ON_CBN_SELCHANGE(IDC_cmbORDER_CANCEL_WORK_CODE, OnSelchangecmbORDERCANCELWORKCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDivnoSelectDlg message handlers

BOOL CCheckInCancelDivnoSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_취소차수선택")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_취소차수선택"));
	m_pDM = FindDM(_T("DM_구입등록대상자료관리"));

	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pOrderCancelGroupCollection = new CGroupCollection(this);

	if(!Init())
	{
		ESLAfxMessageBox(_T("주문취소 선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCheckInCancelDivnoSelectDlg::Init()
{
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	if(nGroupCnt == 0) return FALSE;
	CStringArray arrGroupInfo;
	CString strTmpData, strWorkCode;
	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pCM->SetControlMgrData(_T("업무구분"), strWorkCode);
	m_pGroupCollection->GetGroupInfo(&arrGroupInfo, strTmpData);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &arrGroupInfo, _T("차수정보"));
	return TRUE;
}

VOID CCheckInCancelDivnoSelectDlg::OnSelchangecmbORDERCANCELWORKCODE() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("차수정보"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();	
}

VOID CCheckInCancelDivnoSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pGroup == NULL) 
	{
		ESLAfxMessageBox(_T("선정 차수정보가 올바르지 않습니다."));
		return ;
	}
	CString strMsg;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Group"), (DWORD*)m_pGroup);
	INT ids = api.SPFExecutePath(_T("차수자료검수취소"));
	if(ids < 0)
	{
		strMsg.Format(_T("%s년도 %s 차수자료를 검수취소 할 수 없습니다."), m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
		return ;
	}
	else if(ids > 0)
	{
		strMsg.Format(_T("%d건의 책정보가 등록으로 인계되었습니다. \n%s-%s-%s차수는 검수취소 될수 없습니다."),
			ids, m_pGroup->GetAcqYear(), m_pGroup->GetWorkCode(), m_pGroup->GetGroupInfo());
		ESLAfxMessageBox(strMsg);
	}
	else
	{
		CGroupInfo Group;
		m_pGroup->CopyData(&Group);
		m_pOrderCancelGroupCollection->AddGroupInfo(&Group);
		m_pGroupCollection->DeleteThisGroupInfo(m_pGroup);
	}
	if(!Init())
	{
		EndDialog(IDOK);
	}	
}

HBRUSH CCheckInCancelDivnoSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0,0,0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239,239,239));		
		
		return m_Dlg_brush;		
	}

	return hbr;
}