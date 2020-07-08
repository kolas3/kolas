// DivisionNo_Update_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Update_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Update_Dlg dialog


CDivisionNo_Update_Dlg::CDivisionNo_Update_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Update_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Update_Dlg)
	m_bPurchaseCheckYN = FALSE;
	m_bHoldDupYN = FALSE;
	m_bWorkDupYN = FALSE;
	m_bWorkNoUseYN = FALSE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_nOpType = OPERATION_TYPE_INVALID;
	m_bGridSelectYN = FALSE;

	m_pMainCM = NULL;
}

CDivisionNo_Update_Dlg::~CDivisionNo_Update_Dlg()
{
}

VOID CDivisionNo_Update_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Update_Dlg)
	DDX_Check(pDX, IDC_chkDIVNO_PURCHECK, m_bPurchaseCheckYN);
	DDX_Check(pDX, IDC_chkDIVNO_HOLDDUP, m_bHoldDupYN);
	DDX_Check(pDX, IDC_chkDIVNO_WORKDUP, m_bWorkDupYN);
	DDX_Check(pDX, IDC_chkDIVNO_WORK_NO_USE_YN, m_bWorkNoUseYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Update_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Update_Dlg)
	ON_BN_CLICKED(IDC_chkDIVNO_WORKDUP, OnchkDIVNOWORKDUP)
	ON_BN_CLICKED(IDC_chkDIVNO_HOLDDUP, OnchkDIVNOHOLDDUP)
	ON_BN_CLICKED(IDC_chkDIVNO_PURCHECK, OnchkDIVNOPURCHECK)
	ON_BN_CLICKED(IDC_btnDIVNO_PREV, OnbtnDIVNOPREV)
	ON_BN_CLICKED(IDC_btnDIVNO_NEXT, OnbtnDIVNONEXT)
	ON_BN_CLICKED(IDC_chkDIVNO_WORK_NO_USE_YN, OnchkDIVNOWORKNOUSEYN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Update_Dlg message handlers

BOOL CDivisionNo_Update_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입차수_수정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_구입차수_수정"));
	m_pDM = FindDM(_T("DM_구입차수관리"));
	
	if(m_pMainGrid == NULL) 
	{
		ESLAfxMessageBox(_T("부모 위도우의 그리드 정보가 설정되지 않았습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	Display(0);

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDIVNO_WORK_STATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDivisionNo_Update_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bWorkDupYN) m_pDM->SetCellData(_T("중복조사여부"), _T("Y"), m_nIdx);
	else m_pDM->SetCellData(_T("중복조사여부"), _T("N"), m_nIdx);
	if(m_bHoldDupYN) m_pDM->SetCellData(_T("복본조사여부"), _T("Y"), m_nIdx);
	else m_pDM->SetCellData(_T("복본조사여부"), _T("N"), m_nIdx);
	if(m_bPurchaseCheckYN) m_pDM->SetCellData(_T("심의완료여부"), _T("Y"), m_nIdx);
	else m_pDM->SetCellData(_T("심의완료여부"), _T("N"), m_nIdx);
	if(m_bWorkNoUseYN) m_pDM->SetCellData(_T("작업번호사용여부"), _T("Y"), m_nIdx);
	else m_pDM->SetCellData(_T("작업번호사용여부"), _T("N"), m_nIdx);
	
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("작업상태"), strTmpData);
	if(strTmpData == _T("구입대상")) strTmpData = _T("0");
	else if(strTmpData == _T("주문")) strTmpData = _T("1");
	else if(strTmpData == _T("검수완료")) strTmpData = _T("2");
	m_pCM->SetControlMgrData(_T("작업상태"), strTmpData);
	
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempData(_T("Index"), m_nIdx);
	INT ids = mgr.SPFExecutePath(_T("차수수정"));
	if(ids < 0)
	{
		AfxMessageBox(_T("수정중 에러가 발생하였습니다!"));
		return;
	}
	m_pMainGrid->DisplayLine(m_nIdx);
	if(m_nIdx < m_nLastIdx) Display(1);
	else Display(0);
	AfxMessageBox(_T("저장하였습니다."));
}

VOID CDivisionNo_Update_Dlg::InitFirstLastIndex()
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

VOID CDivisionNo_Update_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDIVNO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDIVNO_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_NEXT)->EnableWindow(FALSE);
}

VOID CDivisionNo_Update_Dlg::Display(INT idx)
{
	CString errstr;
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
	}

	AllControlDisplay(_T("CM_구입차수_수정"), m_nIdx);
	CString strTmpData;
	m_pDM->GetCellData(_T("중복조사여부"), m_nIdx, strTmpData);
	if(strTmpData == _T("Y")) m_bWorkDupYN = TRUE;
	else m_bWorkDupYN = FALSE;

	m_pDM->GetCellData(_T("복본조사여부"), m_nIdx, strTmpData);
	if(strTmpData == _T("Y")) m_bHoldDupYN = TRUE;
	else m_bHoldDupYN = FALSE;

	m_pDM->GetCellData(_T("심의완료여부"), m_nIdx, strTmpData);
	if(strTmpData == _T("Y")) m_bPurchaseCheckYN = TRUE;
	else m_bPurchaseCheckYN = FALSE;

	m_pDM->GetCellData(_T("작업번호사용여부"), m_nIdx, strTmpData);
	if(strTmpData == _T("Y")) m_bWorkNoUseYN = TRUE;
	else m_bWorkNoUseYN = FALSE;
	
	m_pDM->GetCellData(_T("작업상태"), m_nIdx, strTmpData);
	if(strTmpData == _T("0")) strTmpData = _T("구입대상");
	else if(strTmpData == _T("1")) strTmpData = _T("주문");
	else if(strTmpData == _T("2")) strTmpData = _T("검수완료");
	m_pCM->SetControlMgrData(_T("작업상태"), strTmpData);

	UpdateData(FALSE);

	m_pMainGrid->SetReverse(m_nIdx);
}
VOID CDivisionNo_Update_Dlg::OnchkDIVNOWORKDUP() 
{
	// TODO: Add your control notification handler code here
	m_bWorkDupYN = !m_bWorkDupYN;
}

VOID CDivisionNo_Update_Dlg::OnchkDIVNOHOLDDUP() 
{
	// TODO: Add your control notification handler code here
	m_bHoldDupYN = !m_bHoldDupYN;
}

VOID CDivisionNo_Update_Dlg::OnchkDIVNOPURCHECK() 
{
	// TODO: Add your control notification handler code here
	m_bPurchaseCheckYN = !m_bPurchaseCheckYN;
}

VOID CDivisionNo_Update_Dlg::OnbtnDIVNOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CDivisionNo_Update_Dlg::OnbtnDIVNONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CDivisionNo_Update_Dlg::OnchkDIVNOWORKNOUSEYN() 
{
	// TODO: Add your control notification handler code here
	m_bWorkNoUseYN = !m_bWorkNoUseYN;
}

INT CDivisionNo_Update_Dlg::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpType == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpType == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstIdx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CDivisionNo_Update_Dlg::GetPrevSelectIndex()
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

INT CDivisionNo_Update_Dlg::GetNextSelectIndex()
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

BOOL CDivisionNo_Update_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		else if(GetDlgItem(IDC_btnDIVNO_NEXT)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_NEXT, BN_CLICKED);
		else if(GetDlgItem(IDC_btnDIVNO_PREV)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnDIVNO_PREV, BN_CLICKED);
		else
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDivisionNo_Update_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
