// ArrearMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearMainDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearMainDlg dialog


CArrearMainDlg::CArrearMainDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPaymentDlg = NULL;
	m_pUnpaidDlg = NULL;
	m_pAllDlg = NULL;
	m_pStatisticsDlg = NULL;
}
CArrearMainDlg::~CArrearMainDlg()
{
	if(NULL != m_pPaymentDlg)
	{
		delete m_pPaymentDlg;
		m_pPaymentDlg = NULL;
	}
	if(NULL != m_pUnpaidDlg)
	{
		delete m_pUnpaidDlg;
		m_pUnpaidDlg = NULL;
	}
	if(NULL != m_pAllDlg)
	{
		delete m_pAllDlg;
		m_pAllDlg = NULL;
	}
	if(NULL != m_pStatisticsDlg)
	{
		delete m_pStatisticsDlg;
		m_pStatisticsDlg = NULL;
	}
}

void CArrearMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearMainDlg)
	DDX_Control(pDX, IDC_tabAM_MAIN, m_tabMain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearMainDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearMainDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnAM_PRINT, OnbtnAMPRINT)
	ON_BN_CLICKED(IDC_btnAM_CLOSE, OnbtnAMCLOSE)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabAM_MAIN, OnSelchangetabAMMAIN)
	ON_WM_MOVE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_btnAM_PAYMENT, OnbtnAMPAYMENT)
	ON_BN_CLICKED(IDC_btnAM_UNPAID, OnbtnAMUNPAID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearMainDlg message handlers

BOOL CArrearMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	m_pPaymentDlg = new CArrearPaymentDlg(this);	
	m_pPaymentDlg->Create(IDD_ARREAR_PAYMENT, this);

	m_pUnpaidDlg = new CArrearUnpaidDlg(this);
	m_pUnpaidDlg->Create(IDD_ARREAR_UNPAID, this);

	m_pAllDlg = new CArrearAllDlg(this);
	m_pAllDlg->Create(IDD_ARREAR_ALL, this);

	m_pStatisticsDlg = new CArrearStatisticsDlg(this);
	m_pStatisticsDlg->Create(IDD_ARREAR_STATISTICS, this);

	m_tabMain.InsertItem(0, _T("전체") );
	m_tabMain.InsertItem(1, _T("납입") );
	m_tabMain.InsertItem(2, _T("미납") );
	m_tabMain.InsertItem(3, _T("통계") );

	m_nSetIndex = 2;
	m_tabMain.SetCurSel(m_nSetIndex);
	ShowTabWindow(m_nSetIndex);
	
	CenterWindow();
	InitControlPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CArrearMainDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnAMCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnAM_PRINT)->m_hWnd == pMsg->hwnd )
			{
				OnbtnAMPRINT();
			}
			else if(GetDlgItem(IDC_btnAM_CLOSE)->m_hWnd == pMsg->hwnd )
			{
				OnbtnAMCLOSE();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CArrearMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	InitControlPos();
}

VOID CArrearMainDlg::InitControlPos()
{
	if( GetSafeHwnd() )
	{
		if(m_tabMain.GetSafeHwnd() == NULL)	return;
		CWnd* pPrint = GetDlgItem(IDC_btnAM_PRINT);
		CWnd* pClose = GetDlgItem(IDC_btnAM_CLOSE);
		CWnd* pPayment = GetDlgItem(IDC_btnAM_PAYMENT);
		CWnd* pUnPaid = GetDlgItem(IDC_btnAM_UNPAID);
		CWnd* pLine = GetDlgItem(IDC_stcAM_LINE);
		
		CRect rDlg, rTab, rPrint, rClose, rPayment, rUnPaid, rLine;
		GetWindowRect(rDlg);
		m_tabMain.GetWindowRect(rTab);
		pPrint->GetWindowRect(rPrint);
		pClose->GetWindowRect(rClose);
		pPayment->GetWindowRect(rPayment);
		pUnPaid->GetWindowRect(rUnPaid);
		pLine->GetWindowRect(rLine);
		ScreenToClient(rTab);
 		ScreenToClient(rPrint);
 		ScreenToClient(rClose);
		ScreenToClient(rDlg);
		ScreenToClient(rPayment);
		ScreenToClient(rUnPaid);
		ScreenToClient(rLine);

 		m_tabMain.MoveWindow(5, rTab.top, rDlg.Width()-18, rDlg.Height()-80);
 		m_pPaymentDlg->MoveWindow(rDlg.left+14, rDlg.top+60, rDlg.Width()-30, rDlg.Height()-110);
 		m_pUnpaidDlg->MoveWindow(rDlg.left+14, rDlg.top+60, rDlg.Width()-30, rDlg.Height()-110);
 		m_pAllDlg->MoveWindow(rDlg.left+14, rDlg.top+60, rDlg.Width()-30, rDlg.Height()-110);
		m_pStatisticsDlg->MoveWindow(rDlg.left+14, rDlg.top+60, rDlg.Width()-30, rDlg.Height()-110);

		GetWindowRect(rDlg);
		pClose->MoveWindow(rDlg.Width()-95, rDlg.Height()-67, rClose.Width(), rClose.Height());
		pPrint->MoveWindow(rDlg.Width()-180, rDlg.Height()-67, rPrint.Width(), rPrint.Height());
		pLine->MoveWindow(rDlg.Width()-195, rDlg.Height()-67, rLine.Width(), rLine.Height());
		pPayment->MoveWindow(rDlg.Width()-285, rDlg.Height()-67, rPayment.Width(), rPayment.Height());
		pUnPaid->MoveWindow(rDlg.Width()-285, rDlg.Height()-67, rUnPaid.Width(), rUnPaid.Height());
	}
}

void CArrearMainDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CArrearMainDlg::OnbtnAMPRINT() 
{
	if(0 == m_nSetIndex)
	{
		m_pAllDlg->ExcelReport();
	}
	else if(1 == m_nSetIndex)
	{
		m_pPaymentDlg->ExcelReport();
	}
	else if(2 == m_nSetIndex)
	{
		m_pUnpaidDlg->ExcelReport();
	}
	else if(3 == m_nSetIndex)
	{
		m_pStatisticsDlg->ExcelReport();
	}
	else
	{
	}
}

void CArrearMainDlg::OnbtnAMCLOSE() 
{
	OnCancel();
}

VOID CArrearMainDlg::ShowTabWindow(INT nIndex)
{
	m_nSetIndex = nIndex;
	if(0 == nIndex)
	{
		SetWindowText(_T("전체 연체료 현황"));
		m_pAllDlg->ShowWindow(SW_SHOW);
		m_pPaymentDlg->ShowWindow(SW_HIDE);
		m_pUnpaidDlg->ShowWindow(SW_HIDE);
		m_pStatisticsDlg->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_stcAM_LINE))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->EnableWindow(FALSE);
	}
	else if(1 == nIndex)
	{
		SetWindowText(_T("납입 연체료 현황"));
		m_pPaymentDlg->ShowWindow(SW_SHOW);
		m_pUnpaidDlg->ShowWindow(SW_HIDE);
		m_pAllDlg->ShowWindow(SW_HIDE);
		m_pStatisticsDlg->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_stcAM_LINE))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->EnableWindow(FALSE);
		if(_T("S") == m_pInfo->USER_PRIVILEGE)
		{
			((CButton*)GetDlgItem(IDC_stcAM_LINE))->ShowWindow(TRUE);
			((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->ShowWindow(TRUE);
			((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->EnableWindow(TRUE);
		}
	}
	else if(2 == nIndex)
	{
		SetWindowText(_T("미납 연체료 현황"));
		m_pUnpaidDlg->ShowWindow(SW_SHOW);
		m_pPaymentDlg->ShowWindow(SW_HIDE);
		m_pAllDlg->ShowWindow(SW_HIDE);
		m_pStatisticsDlg->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_stcAM_LINE))->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->EnableWindow(FALSE);
	}
	else if(3 == nIndex)
	{
		SetWindowText(_T("연체료 통계"));
		m_pStatisticsDlg->ShowWindow(SW_SHOW);
		m_pUnpaidDlg->ShowWindow(SW_HIDE);
		m_pPaymentDlg->ShowWindow(SW_HIDE);
		m_pAllDlg->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_stcAM_LINE))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_PAYMENT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnAM_UNPAID))->EnableWindow(FALSE);
	}
	else
	{
	}
}

void CArrearMainDlg::OnSelchangetabAMMAIN(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowTabWindow(m_tabMain.GetCurSel());	
	*pResult = 0;
}

void CArrearMainDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	InitControlPos();
}

void CArrearMainDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x	= 660;
 	lpMMI->ptMinTrackSize.y = 400;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CArrearMainDlg::OnbtnAMPAYMENT() 
{
	INT ids;
	if(2 == m_nSetIndex)
	{
		ids = m_pUnpaidDlg->SetPayments();
		if(0 == ids) 
		{
			m_pUnpaidDlg->SearchStart();
// 			m_pAllDlg->SearchStart();
// 			m_pPaymentDlg->SearchStart();
		}
	}
	else
	{
		return;
	}
}

void CArrearMainDlg::OnbtnAMUNPAID() 
{
	INT ids;
	if(1 == m_nSetIndex)
	{
		ids = m_pPaymentDlg->SetUnPaid();
		if(0 == ids)
		{
			m_pPaymentDlg->SearchStart();
// 			m_pUnpaidDlg->SearchStart();
// 			m_pAllDlg->SearchStart();
		}
	}
	else
	{
		return;
	}
}
