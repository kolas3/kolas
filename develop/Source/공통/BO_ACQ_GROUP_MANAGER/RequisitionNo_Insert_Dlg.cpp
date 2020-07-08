// RequisitionNo_Insert_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RequisitionNo_Insert_Dlg.h"
#include "DropGroupNoApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Insert_Dlg dialog


CRequisitionNo_Insert_Dlg::CRequisitionNo_Insert_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequisitionNo_Insert_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequisitionNo_Insert_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pMainGrid = NULL;
	m_pDM = NULL;
	m_strAcqYear = _T("");
	m_bType = FALSE;
	m_bGridSelectYN = FALSE;
	m_nLastReceiptNo = -1;
	m_bDropNoApply = FALSE;
}

CRequisitionNo_Insert_Dlg::~CRequisitionNo_Insert_Dlg()
{
	if(m_bType && m_pDM)
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CRequisitionNo_Insert_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequisitionNo_Insert_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequisitionNo_Insert_Dlg, CDialog)
	//{{AFX_MSG_MAP(CRequisitionNo_Insert_Dlg)
	ON_EN_KILLFOCUS(IDC_edtREQNO_ACQYEAR, OnKillfocusedtREQNOACQYEAR)
	ON_BN_CLICKED(IDC_btnREQNO_PREV, OnbtnREQNOPREV)
	ON_BN_CLICKED(IDC_btnREQNO_NEXT, OnbtnREQNONEXT)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDROP_GROUPNO_APPLY, OnbtnDROPGROUPNOAPPLY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Insert_Dlg message handlers

BOOL CRequisitionNo_Insert_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_접수번호_입력")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_접수번호_입력"));
	m_pDM = FindDM(_T("DM_접수번호관리"));	
	if(m_pMainCM == NULL) m_pMainCM = FindCM(_T("CM_접수번호관리"));
	if(m_pMainCM != NULL)
		m_pMainGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	if(!m_pDM)
	{
		m_bType = TRUE;
		m_pDM = new CESL_DataMgr;
		m_pDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pDM->InitDMPKFromFile(_T("DM_접수번호관리"));
		m_pDM->SetMarcMgr(this->m_pInfo->pMarcMgr);		
	}
	
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	AddColorControl(IDC_edtREQNO_ACQYEAR, _T("CM_접수번호_입력"), _T("수입년도"), 'E');
	AddColorControl(IDC_edtREQNO_CREATOR, _T("CM_접수번호_입력"), _T("접수번호생성자"), 'E');
	AddColorControl(IDC_edtREQNO_CREATE_DATE, _T("CM_접수번호_입력"), _T("접수번호생성일"), 'D');
	
	ShowCtrlByOpenMode();

	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT) Display(1000);
	else Display(0);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequisitionNo_Insert_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnREQNO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnREQNO_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnREQNO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnREQNO_NEXT)->EnableWindow(FALSE);
}

VOID CRequisitionNo_Insert_Dlg::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_nFirstIdx = 0;
		m_nLastIdx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastIdx = m_pMainGrid->SelectGetTailPosition();
		m_nFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CRequisitionNo_Insert_Dlg::Display(INT idx)
{
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = GetNextSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		if(!m_strAcqYear.IsEmpty())
		{
			m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
			m_pCM->SetControlMgrData(_T("접수번호생성자"), m_pInfo->USER_NAME);
			m_pCM->SetControlMgrData(_T("마지막일련번호"), _T("0"));
			INT ids = SetLastRequisitionNo();
		}
		return;
	}

	m_pCM->AllControlDisplay(m_nIdx);
	m_pMainGrid->SetReverse(m_nIdx);
}

VOID CRequisitionNo_Insert_Dlg::ShowCtrlByOpenMode()
{
	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT)
	{
		this->SetWindowText(_T("접수번호생성"));
		GetDlgItem(IDC_btnREQNO_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_NEXT)->ShowWindow(SW_HIDE);
	}
	else 
	{
		GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_edtREQNO_ACQYEAR))->SetReadOnly(TRUE);
		this->SetWindowText(_T("접수번호수정"));
	}
}

VOID CRequisitionNo_Insert_Dlg::OnKillfocusedtREQNOACQYEAR() 
{
	// TODO: Add your control notification handler code here
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtREQNO_ACQYEAR);
	if(pAcqYear->GetStyle() & ES_READONLY) return;
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"),  strAcqYear);
	if(strAcqYear != m_strAcqYear)
	{
		m_strAcqYear = strAcqYear;
		SetLastRequisitionNo();
	}
}


INT CRequisitionNo_Insert_Dlg::SetLastRequisitionNo()
{
	CString strLastNo;
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("Type"), _T("DON_NO"));
	mgr.SetTempData(_T("AcqYear"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("Desc"), _T(""));	
	mgr.SetTempData(_T("Pub_Form_Code"), _T("MO"));
	INT ids = mgr.SPFExecutePath(_T("마지막번호추출"));
	if(ids < 0) return ids;
	
	mgr.GetTempData(_T("LastNumber"), strLastNo);
	m_pCM->SetControlMgrData(_T("접수번호"), strLastNo);
	m_nLastReceiptNo = _ttoi(strLastNo.GetBuffer(0));
	return 0;
}

VOID CRequisitionNo_Insert_Dlg::OnbtnREQNOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CRequisitionNo_Insert_Dlg::OnbtnREQNONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CRequisitionNo_Insert_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids;
	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT)
	{
		mgr.SetTempData(_T("DROP_NO_APPLY"), m_bDropNoApply);
		ids = mgr.SPFExecutePath(_T("접수번호생성"));
	}
	else
	{
		mgr.SetTempData(_T("Index"), m_nIdx);
		ids = mgr.SPFExecutePath(_T("접수번호수정"));
	}
	if(ids < 0)
	{
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("중복된 접수번호가 존재합니다. 마지막 접수번호 %d 입니다.\n접수번호를 재설정 합니다."), ids);
		ESLAfxMessageBox(strMsg);
		SetLastRequisitionNo();
		return ;
	}
	ESLAfxMessageBox(_T("저장하였습니다."));

	if(m_nOpenMode == SUPDATE)
	{
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_nIdx);
		if(m_nIdx < m_nLastIdx) Display(1);
	}
	else if(m_nOpenMode == DINSERT)
	{
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_pDM->GetRowCount() - 1);
		CDialog::OnOK();
	}
	else
	{
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_pDM->GetRowCount() - 1);
		if(m_bDropNoApply)
			ApplyLastNo();
		SetLastRequisitionNo();
	}
}

VOID CRequisitionNo_Insert_Dlg::GetInsertInfo(CString &strAddGroupNo)
{
	INT nCurRow = m_pDM->GetRowCount() - 1;
	m_pDM->GetCellData(_T("접수번호"), nCurRow, strAddGroupNo);
}

HBRUSH CRequisitionNo_Insert_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

INT CRequisitionNo_Insert_Dlg::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstIdx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CRequisitionNo_Insert_Dlg::GetPrevSelectIndex()
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nFirstIdx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CRequisitionNo_Insert_Dlg::GetNextSelectIndex()
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nLastIdx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}
VOID CRequisitionNo_Insert_Dlg::OnbtnDROPGROUPNOAPPLY() 
{
	// TODO: Add your control notification handler code here
	if(!m_bDropNoApply) 
		ApplyDropNo();
	else
		ApplyLastNo();
}

INT CRequisitionNo_Insert_Dlg::ApplyDropNo()
{
	CString strAcqYear, strTmpData;
	INT nTmpData;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	CDropGroupNoApplyDlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkCode(_T(""));
	dlg.SetLastGroupNo(m_nLastReceiptNo);
	dlg.SetGroupType(RECEIPT_NO);
	if(IDOK == dlg.DoModal())
	{
		m_bDropNoApply = TRUE;
		nTmpData = dlg.GetSelectGroupNo();
		if(nTmpData < 0) return -1;
		strTmpData.Format(_T("%d"), nTmpData);
		m_pCM->SetControlMgrData(_T("접수번호"), strTmpData);
		GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->SetWindowText(_T("마지막번호적용"));
	}	
	return 0;
}

INT CRequisitionNo_Insert_Dlg::ApplyLastNo()
{
	SetLastRequisitionNo();
	m_bDropNoApply = FALSE;
	GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->SetWindowText(_T("누락번호적용"));
	return 0;
}

BOOL CRequisitionNo_Insert_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		else if(GetDlgItem(IDC_btnDROP_GROUPNO_APPLY)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDROP_GROUPNO_APPLY, BN_CLICKED);
		else if(GetDlgItem(IDC_btnREQNO_PREV)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnREQNO_PREV, BN_CLICKED);
		else if(GetDlgItem(IDC_btnREQNO_NEXT)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnREQNO_NEXT, BN_CLICKED);
		else
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}
