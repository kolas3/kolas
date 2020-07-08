// DivisionNo_Insert_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Insert_Dlg.h"
#include "DropGroupNoApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Insert_Dlg dialog


CDivisionNo_Insert_Dlg::CDivisionNo_Insert_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Insert_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Insert_Dlg)
	m_bWorkNo_UseYN = FALSE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM = NULL;
	m_pWorkNoDM = NULL;
	m_strWorkStatus = _T("");
	m_strAcqYear = _T("");
	m_bCreateDM = FALSE;
	m_bCreateWorkNoDM = FALSE;
	m_nOpenMode = SINSERT;
	m_nLastDivNo = -1;
	m_bDropNoApply = FALSE;
	m_pParentGrid = NULL;
}
CDivisionNo_Insert_Dlg::~CDivisionNo_Insert_Dlg()
{
	if(m_bCreateDM)
	{
		if(m_pDM)
		{
			//delete m_pDM;
			//m_pDM->FreeData();
			m_pDM = NULL;
		}
	}
	if(m_bCreateWorkNoDM)
	{
		if(m_pWorkNoDM)
		{
			delete m_pWorkNoDM;
			//m_pWorkNoDM.FreeData();
			m_pWorkNoDM = NULL;
		}
	}
}

VOID CDivisionNo_Insert_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Insert_Dlg)
	DDX_Check(pDX, IDC_chkDIVNO_UNUSE_WORK_NO, m_bWorkNo_UseYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Insert_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Insert_Dlg)
	ON_EN_KILLFOCUS(IDC_edtDIVNO_ACQYEAR, OnKillfocusedtDIVNOACQYEAR)
	ON_BN_CLICKED(IDC_chkDIVNO_UNUSE_WORK_NO, OnchkDIVNOUNUSEWORKNO)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_CODE, OnSelchangecmbDIVNOWORKCODE)
	ON_BN_CLICKED(IDC_btnDROP_GROUPNO_APPLY, OnbtnDROPGROUPNOAPPLY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Insert_Dlg message handlers

BOOL CDivisionNo_Insert_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입차수_입력")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_구입차수_입력"));
	m_bCreateDM = TRUE;

	if(!m_strAcqYear.IsEmpty())
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM->SetControlMgrData(_T("작업상태"), m_strWorkStatus);
	m_pCM->SetControlMgrData(_T("차수생성자"), m_pInfo->USER_NAME);

	AddColorControl(IDC_edtDIVNO_ACQYEAR, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtDIVNO_CREATE_DATE, _T("CM_구입차수_입력"), _T("차수생성일"), 'D');
	AddColorControl(IDC_edtDIVNO_CREATOR, _T(""), _T(""), 'E');
	
	if(!m_strWorkCode.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("업무구분"), m_strWorkCode);
		GetDlgItem(IDC_cmbDIVNO_WORK_CODE)->EnableWindow(FALSE);
	}

	INT ids = SetLastDivNo();
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("마지막 차수번호를 추출할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	m_pCM->SetControlMgrData(_T("마지막작업번호"), _T("1"));

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDIVNO_WORK_STATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	//OnchkDIVNOUNUSEWORKNO();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDivisionNo_Insert_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CGroupMgr mgr(this);
	if(!m_pDM)
	{
		CString strDMPK;
		m_pDM = new CESL_DataMgr;
		m_pDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pDM->InitDMPKFromFile(_T("DM_구입차수관리"));
		m_pDM->SetMarcMgr(this->m_pInfo->pMarcMgr);
		m_bCreateDM = TRUE;
	}
	if(!m_pWorkNoDM)
	{
		m_pWorkNoDM = new CESL_DataMgr;
		m_pWorkNoDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pWorkNoDM->InitDMPKFromFile(_T("DM_작업번호관리"));
		m_pWorkNoDM->SetMarcMgr(this->m_pInfo->pMarcMgr);
		m_bCreateWorkNoDM = TRUE;
	}
	if(!CheckWorkCode())
	{
		ESLAfxMessageBox(_T("업무구분을 설정해 주십시요."));
		return;
	}
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("WorkNoDM"), (DWORD*)m_pWorkNoDM);
	mgr.SetTempData(_T("WORK_NO_USE_YN"), m_bWorkNo_UseYN);
	mgr.SetTempData(_T("DROP_NO_APPLY"), m_bDropNoApply);
	INT ids = mgr.SPFExecutePath(_T("차수생성"));
	if(ids < 0)
	{
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("중복된 차수번호가 존재합니다. 마지막 차수번호는 %d 입니다.\n차수번호를 재설정 합니다."), ids);
		ESLAfxMessageBox(strMsg);
		SetLastDivNo();
		return ;
	}
	
	ESLAfxMessageBox(_T("저장하였습니다."));
	if(m_nOpenMode == DINSERT)
	{
		CDialog::OnOK();
	}
	else if(m_nOpenMode != DINSERT && m_pParentGrid != NULL)
	{
		ids = m_pDM->GetRowCount();
		m_pParentGrid->DisplayLine(ids-1);
	}
	if(m_bDropNoApply)
	{
		ids = ApplyLastNo();
		if( ids < 0 )return ;
	}
	else
	{
		ids = SetLastDivNo();
		if( ids < 0 ) return ;
	}
}

VOID CDivisionNo_Insert_Dlg::OnKillfocusedtDIVNOACQYEAR() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if(m_strAcqYear != strAcqYear)
	{
		m_strAcqYear = strAcqYear;
		SetLastDivNo();
	}
}

INT CDivisionNo_Insert_Dlg::SetLastDivNo()
{
	CString strLastNo, strWorkCode;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("Type"), _T("PUR_NO"));
	mgr.SetTempData(_T("AcqYear"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("Desc"), strWorkCode.GetBuffer(0));	
	mgr.SetTempData(_T("Pub_Form_Code"), _T("MO"));
	INT ids = mgr.SPFExecutePath(_T("마지막번호추출"));
	if(ids < 0) return ids;
	
	mgr.GetTempData(_T("LastNumber"), strLastNo);
	m_pCM->SetControlMgrData(_T("차수번호"), strLastNo);
	m_nLastDivNo = _ttoi(strLastNo.GetBuffer(0));
	return 0;
}

VOID CDivisionNo_Insert_Dlg::OnchkDIVNOUNUSEWORKNO() 
{
	// TODO: Add your control notification handler code here
	m_bWorkNo_UseYN = !m_bWorkNo_UseYN;
	UpdateData(FALSE);
}

VOID CDivisionNo_Insert_Dlg::GetInsertInfo(CString &strAddGroupNo, CString &strAddWorkNo, CString &strAddLastNo, CString &strAddWorkCode)
{
	INT nGroupCurRow = m_pDM->GetRowCount() - 1;
	INT nWorkCurRow = m_pWorkNoDM->GetRowCount() - 1;
	m_pDM->GetCellData(_T("차수번호"), nGroupCurRow, strAddGroupNo);	
	m_pWorkNoDM->GetCellData(_T("작업번호"), nWorkCurRow, strAddWorkNo);
	m_pWorkNoDM->GetCellData(_T("마지막일련번호"), nWorkCurRow, strAddLastNo);
	m_pDM->GetCellData(_T("업무구분"), nGroupCurRow, strAddWorkCode);
}

HBRUSH CDivisionNo_Insert_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CDivisionNo_Insert_Dlg::OnSelchangecmbDIVNOWORKCODE() 
{
	// TODO: Add your control notification handler code here
	SetLastDivNo();
}

BOOL CDivisionNo_Insert_Dlg::CheckWorkCode()
{
	CString strWorkCode ;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	if(strWorkCode.IsEmpty() && m_strWorkCode != strWorkCode)
		return FALSE;
	return TRUE;
}

VOID CDivisionNo_Insert_Dlg::OnbtnDROPGROUPNOAPPLY() 
{
	// TODO: Add your control notification handler code here
	if(!m_bDropNoApply) 
		ApplyDropNo();
	else
		ApplyLastNo();
}

INT CDivisionNo_Insert_Dlg::ApplyDropNo()
{
	CString strWorkCode, strAcqYear, strTmpData;
	INT nTmpData;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	CDropGroupNoApplyDlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetLastGroupNo(m_nLastDivNo);
	dlg.SetGroupType(DIVNO);
	if(IDOK == dlg.DoModal())
	{
		m_bDropNoApply = TRUE;
		nTmpData = dlg.GetSelectGroupNo();
		if(nTmpData < 0) return -1;
		strTmpData.Format(_T("%d"), nTmpData);
		m_pCM->SetControlMgrData(_T("차수번호"), strTmpData);
		GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->SetWindowText(_T("마지막번호적용"));
	}
	return 0;
}

INT CDivisionNo_Insert_Dlg::ApplyLastNo()
{
	SetLastDivNo();
	m_bDropNoApply = FALSE;
	GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->SetWindowText(_T("누락번호적용"));
	return 0;
}

BOOL CDivisionNo_Insert_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		else if(GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDROP_GROUPNO_APPLY, BN_CLICKED);
		else
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
