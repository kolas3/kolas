// KL_OTHER_RETURN_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_MAIN.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#include "BL_LOC_LOAN_PROC.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_MAIN dialog

CKL_OTHER_RETURN_MAIN::CKL_OTHER_RETURN_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMode = 0;
	mCtrl_pDlg_Request = NULL;
	mCtrl_pDlg_Offer = NULL;
	m_bShow = FALSE;

	m_pKLRegnoEdit = NULL;
}

CKL_OTHER_RETURN_MAIN::~CKL_OTHER_RETURN_MAIN()
{
	if(NULL != mCtrl_pDlg_Offer)
	{
		delete mCtrl_pDlg_Offer;
		mCtrl_pDlg_Offer = NULL;
	}
	if(NULL != mCtrl_pDlg_Request)
	{
		delete mCtrl_pDlg_Request;
		mCtrl_pDlg_Request = NULL;
	}
	if (m_pKLRegnoEdit != NULL)
	{
		delete m_pKLRegnoEdit;
		m_pKLRegnoEdit = NULL;
	}
}

BOOL CKL_OTHER_RETURN_MAIN::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}
void CKL_OTHER_RETURN_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_MAIN)
	DDX_Control(pDX, IDC_TAB_MAIN, mCtrl_TabMain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_MAIN, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_MAIN)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON1, OnSend)
	ON_BN_CLICKED(IDC_BUTTON2, OnReturn)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_btnKL_ReturnCancel, OnbtnKLReturnCancel)
	ON_BN_CLICKED(IDC_btnKL_GipyoPrint, OnbtnKLGipyoPrint)
	ON_BN_CLICKED(IDC_btnKL_PostInfo, OnbtnKLPostInfo)
	ON_BN_CLICKED(IDC_btnKL_print_waybill, OnbtnKLprintwaybill)
	ON_BN_CLICKED(IDC_btnKL_edit_memo, OnbtnKLeditmemo)
	ON_BN_CLICKED(IDC_btnKL_loan_user_view, OnbtnKLloanuserview)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KL_RETURN_CANCEL_CTL, OnReturnCancelCtl)	
	ON_MESSAGE(WM_KL_ACTIVE_BUTTON, OnActivateButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_MAIN message handlers

BOOL CKL_OTHER_RETURN_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());
	// TODO: Add extra initialization here	
	mCtrl_TabMain.InsertItem(0,	_T("발송자료관리"));
	mCtrl_TabMain.InsertItem(1,	_T("복귀자료관리"));

 	mCtrl_pDlg_Offer = new CKL_OTHER_RETURN_OFFER(this);
 	mCtrl_pDlg_Request = new CKL_OTHER_RETURN_REQUEST(this);

	mCtrl_pDlg_Offer->m_pInfo = m_pInfo;
	mCtrl_pDlg_Request->m_pInfo = m_pInfo;


	mCtrl_pDlg_Offer->m_pLoanManageValue = m_pLoanManageValue;
	mCtrl_pDlg_Offer->m_pLoanShareManager = m_pLoanShareManager;
	
	mCtrl_pDlg_Request->m_pLoanManageValue = m_pLoanManageValue;

	mCtrl_pDlg_Offer->Create(this);
	mCtrl_pDlg_Request->Create(this);

	SetWindowPos(NULL, 0, 0, 1210, 700, SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	SetMode(0);

	((CButton*)GetDlgItem(IDC_btnKL_PostInfo))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_btnKL_GipyoPrint))->ShowWindow(SW_HIDE);

	{
		m_pKLRegnoEdit = new CESL_ControlEdit(NULL);
		ASSERT(m_pKLRegnoEdit);

		if (m_pKLRegnoEdit->SubclassDlgItem(IDC_edt_KL_RETURN_REG_NO, this))
		{
			m_pKLRegnoEdit->SetKLRegnoMode(TRUE);	

			m_pKLRegnoEdit->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_OTHER_RETURN_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();	
}

void CKL_OTHER_RETURN_MAIN::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nMode = mCtrl_TabMain.GetCurSel();
	SetMode(nMode);

	*pResult = 0;
}

BOOL CKL_OTHER_RETURN_MAIN::SetMode(int nMode)
{
	mCtrl_TabMain.SetCurSel(nMode);
	ShowDlg(nMode);
	ChangeToolBarButton(nMode);
	m_nMode = nMode;

	if(m_nMode == 1)
	{
		GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
	}
	else
	{
		CString strState = mCtrl_pDlg_Request->GetComboBoxState();
		if(strState.Compare(_T("1"))==0)
		{
			GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
		}
	}

	return 0;
}

INT CKL_OTHER_RETURN_MAIN::ShowDlg(INT nIdx)
{
	CString strState;
	BOOL bShow = FALSE;
	if(0 == nIdx)
	{		
		mCtrl_pDlg_Request->ShowWindow(SW_SHOW);
		mCtrl_pDlg_Offer->ShowWindow(SW_HIDE);
		strState = mCtrl_pDlg_Request->GetComboBoxState();
		if(_T("0") == strState || _T("1") == strState) bShow = TRUE;
	}
	else if(1 == nIdx)
	{
		mCtrl_pDlg_Request->ShowWindow(SW_HIDE);
		mCtrl_pDlg_Offer->ShowWindow(SW_SHOW);
		strState = mCtrl_pDlg_Offer->GetComboBoxState();
		if(_T("2") == strState) bShow = TRUE;
	}
	else
		return -1;

	GetDlgItem(IDC_btnKL_ReturnCancel)->EnableWindow(bShow);

	return 0;
}

INT CKL_OTHER_RETURN_MAIN::ChangeToolBarButton(INT nIdx)
{

	if(0 == nIdx)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKL_GipyoPrint)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKL_GipyoPrint)->EnableWindow(FALSE);
	}

	return 0;
}

BOOL CKL_OTHER_RETURN_MAIN::ResizeControl()
{
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	if(NULL == mCtrl_TabMain.GetSafeHwnd())
	{
		return FALSE;
	}
	CRect rTab;
	mCtrl_TabMain.GetWindowRect(rTab);
	ScreenToClient(rTab);
	
	rTab.right = rDlg.Width();
	mCtrl_TabMain.MoveWindow(rTab);

	CRect rSubDlg = rDlg;
	rSubDlg.top = rTab.bottom+5;
	rSubDlg.right -= 2;

	if(NULL == mCtrl_pDlg_Offer || NULL == mCtrl_pDlg_Offer->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Offer->MoveWindow(rSubDlg);

	if(NULL == mCtrl_pDlg_Request || NULL == mCtrl_pDlg_Request->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Request->MoveWindow(rSubDlg);
	
	return TRUE;
}

INT CKL_OTHER_RETURN_MAIN::ExcutePath(CString strSM)
{
	INT ids = 0;
	if(1 == GetMode())
	{
		if(!mCtrl_pDlg_Offer->IsWindowEnabled())
		{
			return 1000;
		}

		if(_T("복귀완료") == strSM)
		{
			ids = mCtrl_pDlg_Offer->Return_Complete();
			if(0 == ids)
			{

			}
		}
		else if(_T("새로고침") == strSM)
		{
			mCtrl_pDlg_Offer->Refresh();
		}
		else if(_T("타관반납취소") == strSM)
		{
			mCtrl_pDlg_Offer->ReturnCancel();
		}
	}
	else if(0 == GetMode())
	{
		if(!mCtrl_pDlg_Request->IsWindowEnabled())
		{
			return 1000;
		}

		if(_T("복귀") == strSM)
		{
			ids = mCtrl_pDlg_Request->Return();
			if(0 == ids)
			{
			}
		}
		else if(_T("새로고침") == strSM)
		{
			mCtrl_pDlg_Request->Refresh();
		}
		else if(_T("타관반납취소") == strSM)
		{
			mCtrl_pDlg_Request->ReturnCancel();
		}
		else if(_T("기표지출력") == strSM)
		{
			mCtrl_pDlg_Request->PrintGipyo();
		}
		else if(_T("배송정보조회") == strSM)
		{
			mCtrl_pDlg_Request->ExeDeliveryInfo();
		}
	}

	return ids;
}

int CKL_OTHER_RETURN_MAIN::GetMode()
{
	return m_nMode;
}

void CKL_OTHER_RETURN_MAIN::OnSend() 
{
	// TODO: Add your control notification handler code here
	if(0 == GetMode())
	{
		ExcutePath(_T("복귀"));
	}
}

void CKL_OTHER_RETURN_MAIN::OnReturn() 
{
	// TODO: Add your control notification handler code here
	if(1 == GetMode())
	{
		ExcutePath(_T("복귀완료"));
	}
}

void CKL_OTHER_RETURN_MAIN::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	ExcutePath(_T("새로고침"));
}

void CKL_OTHER_RETURN_MAIN::OnButton3() 
{
	// TODO: Add your control notification handler code here
	CString sRegNo;
	GetDlgItemText(IDC_edt_KL_RETURN_REG_NO, sRegNo);
	
	sRegNo.TrimLeft();
	sRegNo.TrimRight();
	if(sRegNo.IsEmpty())
	{
		return;
	}

	CLocCommonAPI::MakeRegNoSpaceToZero(sRegNo);
	INT ids = 0;

	if(1 == GetMode())
	{
		ids = mCtrl_pDlg_Offer->SelectGrid(sRegNo);
	}
	else if(0 == GetMode())
	{
		ids = mCtrl_pDlg_Request->SelectGrid(sRegNo);
	}

	if(0 < ids)
	{
		CString strMessage;
		strMessage.Format(_T("등록번호가 [%s]인 자료가 존재하지 않습니다."), sRegNo);
		AfxMessageBox(strMessage);
	}
	SetDlgItemText(IDC_edt_KL_RETURN_REG_NO, _T(""));
}

BOOL CKL_OTHER_RETURN_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			CWnd* pWnd1 = GetFocus();
			CWnd* pWnd2 = GetDlgItem(IDC_edt_KL_RETURN_REG_NO);
			if(pWnd1->GetSafeHwnd() == pWnd2->GetSafeHwnd())
			{
				OnButton3();
				pWnd2->SetFocus();

				return TRUE;
			}
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLReturnCancel() 
{
	ExcutePath(_T("타관반납취소"));
}

LRESULT CKL_OTHER_RETURN_MAIN::OnReturnCancelCtl(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)lParam;
	GetDlgItem(IDC_btnKL_ReturnCancel)->EnableWindow(bShow);

	return 0;
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLGipyoPrint() 
{
	ExcutePath(_T("기표지출력"));	
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLPostInfo() 
{
	ExcutePath(_T("배송정보조회"));	
}

LRESULT CKL_OTHER_RETURN_MAIN::OnActivateButton(WPARAM wParam, LPARAM lParam)
{
	int nState = (int)lParam;

	GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
	
	if(nState == 1)
	{
		GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(TRUE);
	}

	return 0;
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLprintwaybill() 
{
	// TODO: Add your control notification handler code here

	if(mCtrl_pDlg_Request)
		mCtrl_pDlg_Request->PrintWaybill();
	
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLeditmemo() 
{
	// TODO: Add your control notification handler code here
	if(m_nMode == 0 )
	{
		mCtrl_pDlg_Request->ModifyMemo();	
	}
	else
	{
		mCtrl_pDlg_Offer->ModifyMemo();	
	}	
}

void CKL_OTHER_RETURN_MAIN::OnbtnKLloanuserview() 
{
	// TODO: Add your control notification handler code here

	CString m_strUserAccessAuth = m_pLoanManageValue->m_sLoanUserAccessMode;
	if( m_strUserAccessAuth.IsEmpty() || m_strUserAccessAuth == _T("1") )
	{
		AfxMessageBox(_T("이용자 정보 접근권한이 없습니다."));
		return;
	}
	
	if(m_nMode == 0 )
	{
		mCtrl_pDlg_Request->ViewLoanUser();	
	}
	else
	{
		mCtrl_pDlg_Offer->ViewLoanUser();	
	}	
	
}