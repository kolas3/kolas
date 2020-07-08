// Group_Insert_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Group_Insert_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_Insert_Dlg dialog


CGroup_Insert_Dlg::CGroup_Insert_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_Insert_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_Insert_Dlg)
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
	m_strGroupNo = _T("");
	m_bType = FALSE;
	m_bGridSelectYN = FALSE;
}

CGroup_Insert_Dlg::~CGroup_Insert_Dlg()
{
	if(m_bType && m_pDM)
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CGroup_Insert_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_Insert_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_Insert_Dlg, CDialog)
	//{{AFX_MSG_MAP(CGroup_Insert_Dlg)
	ON_BN_CLICKED(IDC_btnGROUP_PREV, OnbtnGROUPPREV)
	ON_BN_CLICKED(IDC_btnGROUP_NEXT, OnbtnGROUPNEXT)
	ON_EN_KILLFOCUS(IDC_edtGROUP_CREATE_YEAR, OnKillfocusedtGROUPCREATEYEAR)
	ON_EN_KILLFOCUS(IDC_edtGROUP_SEQ, OnKillfocusedtGROUPSEQ)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_Insert_Dlg message handlers

BOOL CGroup_Insert_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_그룹관리_입력")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_그룹관리_입력"));
	m_pDM = FindDM(_T("DM_그룹관리"));
	if(m_pMainCM == NULL) m_pMainCM = FindCM(_T("CM_그룹관리"));
	if(m_pMainCM != NULL)
		m_pMainGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	
	if(!m_pDM)
	{
		m_bType = TRUE;
		CGroupMgr mgr(this);
		m_pDM = new CESL_DataMgr;
		m_pDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pDM->InitDMPKFromFile(_T("DM_그룹관리"));
		m_pDM->SetMarcMgr(this->m_pInfo->pMarcMgr);		
	}
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	AddColorControl(IDC_edtGROUP_CREATE_YEAR, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtGROUP_CREATOR, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtGROUP_SEQ, _T(""), _T(""), 'E');

	ShowCtrlByOpenMode();

	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT) Display(1000);
	else Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CGroup_Insert_Dlg::ShowCtrlByOpenMode()
{
	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT)
	{
		this->SetWindowText(_T("그룹생성"));
		GetDlgItem(IDC_btnGROUP_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_NEXT)->ShowWindow(SW_HIDE);
	}
	else 
	{
		CEdit * pAcqYear = (CEdit*)m_pCM->FindControl(_T("그룹생성년도"));
		pAcqYear->EnableWindow(FALSE);
		this->SetWindowText(_T("그룹수정"));
	}
}

VOID CGroup_Insert_Dlg::Display(INT idx)
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
			m_pCM->SetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
		m_pCM->SetControlMgrData(_T("그룹생성자"), m_pInfo->USER_NAME);
		return;
	}

	m_pCM->AllControlDisplay(m_nIdx);
	m_pMainGrid->SetReverse(m_nIdx);
}

VOID CGroup_Insert_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnGROUP_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnGROUP_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnGROUP_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnGROUP_NEXT)->EnableWindow(FALSE);
}

VOID CGroup_Insert_Dlg::InitFirstLastIndex()
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

VOID CGroup_Insert_Dlg::OnbtnGROUPPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CGroup_Insert_Dlg::OnbtnGROUPNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CGroup_Insert_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strGroupName;
	m_pCM->GetControlMgrData(_T("그룹이름"), strGroupName);
	if(strGroupName.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("그룹명을 입력해 주세요."));
		return ;
	}
	
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids;
	if(m_nOpenMode == SINSERT || m_nOpenMode == DINSERT)
	{
		SetLastGroupNo();
		mgr.SetTempData(_T("그룹번호"), m_strGroupNo.GetBuffer(0));
		ids = mgr.SPFExecutePath(_T("그룹추가"));
	}
	else
	{
		mgr.SetTempData(_T("Index"), m_nIdx);
		ids = mgr.SPFExecutePath(_T("그룹수정"));
	}
	if(ids < 0)
	{
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("중복된 그룹번호가 존재합니다. 마지막 그룹번호 %d 입니다.\n그룹번호를 재설정 합니다."), ids);
		ESLAfxMessageBox(strMsg);
		SetLastGroupNo();
		return ;
	}
	ESLAfxMessageBox(_T("저장하였습니다."));
	if(m_nOpenMode == DINSERT)
	{
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);
		CDialog::OnOK();
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_nIdx);
		Display(1);
	}
	else
		if(m_pMainGrid != NULL) m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);
}

VOID CGroup_Insert_Dlg::OnKillfocusedtGROUPCREATEYEAR() 
{
	// TODO: Add your control notification handler code here
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtGROUP_CREATE_YEAR);
	if(pAcqYear->GetStyle() & ES_READONLY) return;
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("그룹생성년도"),  strAcqYear);
	if(strAcqYear != m_strAcqYear)
	{
		m_strAcqYear = strAcqYear;
		SetLastGroupNo();
	}
}

INT CGroup_Insert_Dlg::SetLastGroupNo()
{
	CString strLastNo;
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("Type"), _T("GRP_NO"));
	mgr.SetTempData(_T("AcqYear"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("Desc"), _T(""));	
	mgr.SetTempData(_T("Pub_Form_Code"), _T("MO"));
	INT ids = mgr.SPFExecutePath(_T("마지막번호추출"));
	if(ids < 0) return ids;
	
	mgr.GetTempData(_T("LastNumber"), strLastNo);
	m_strGroupNo = strLastNo ;
	return 0;
}

VOID CGroup_Insert_Dlg::GetInsertInfo(CString &strAddGroupNo, CString &strAddGroupName, CString &strAddLastNo)
{
	INT nCurRow = m_pDM->GetRowCount() - 1;
	m_pDM->GetCellData(_T("그룹이름"), nCurRow, strAddGroupName);
	m_pDM->GetCellData(_T("그룹번호"), nCurRow, strAddGroupNo);
	m_pDM->GetCellData(_T("마지막일련번호"), nCurRow, strAddLastNo);
}

VOID CGroup_Insert_Dlg::OnKillfocusedtGROUPSEQ() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strGroupName;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹이름"), strGroupName);

	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("CREATE_YEAR"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("GROUP_NAME"), strGroupName.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("그룹중복검사"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("그룹명이 중복됩니다."));
		m_pCM->SetControlMgrData(_T("그룹이름"), _T(""));
		return ;
	}
}

HBRUSH CGroup_Insert_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

INT CGroup_Insert_Dlg::GetFirstSelectIndex()
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

INT CGroup_Insert_Dlg::GetPrevSelectIndex()
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

INT CGroup_Insert_Dlg::GetNextSelectIndex()
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

BOOL CGroup_Insert_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
		    PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		else if(GetDlgItem(IDC_btnGROUP_PREV)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_PREV, BN_CLICKED);
		else if(GetDlgItem(IDC_btnGROUP_NEXT)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_NEXT, BN_CLICKED);
		else
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
