// SIReportPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportPrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportPrintDlg dialog


CSIReportPrintDlg::CSIReportPrintDlg(CWnd* pParent /*=NULL*/, CSIReportManager *pManager/*=NULL*/)
	: CDialog(CSIReportPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIReportPrintDlg)
	m_nEndPage = 0;
	m_nStartPage = 0;
	m_nPaper = 0;
	//}}AFX_DATA_INIT
	m_pManager = pManager;
	m_nTotalPages = 0;
	IsPrint = FALSE;
}


VOID CSIReportPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportPrintDlg)
	DDX_Text(pDX, IDC_EDIT_END_PAGE, m_nEndPage);
	DDX_Text(pDX, IDC_EDIT_START_PAGE, m_nStartPage);
	DDX_Text(pDX, IDC_nPAPER, m_nPaper);
	DDV_MinMaxUInt(pDX, m_nPaper, 1, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSIReportPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CSIReportPrintDlg)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportPrintDlg message handlers

BOOL CSIReportPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 비트맵 버튼의 초기화
	NewButtonList.Add(IDC_btnPRINT, this, IDB_PRINT_U, IDB_PRINT_D, IDB_PRINT_F);
	NewButtonList.Add(IDC_btnCLOSE, this, IDB_CLOSE_U, IDB_CLOSE_D, IDB_CLOSE_F);
	
	CButton *b1 = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	CButton *b2 = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
	if (b1 && b2) {
		b1->SetCheck(true);
		b2->SetCheck(false);
		GetDlgItem(IDC_EDIT_START_PAGE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_END_PAGE)->EnableWindow(false);
	}
	m_nPaper = 1;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSIReportPrintDlg::OnRadioRange() 
{
	SetButtonStatus();
}

VOID CSIReportPrintDlg::OnRadioAll() 
{
	SetButtonStatus();
}

VOID CSIReportPrintDlg::SetButtonStatus(VOID)
{
	CButton *b1 = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	CButton *b2 = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
	if (!b1 || !b2) return;
	if (b1->GetCheck()) {
		GetDlgItem(IDC_EDIT_START_PAGE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_END_PAGE)->EnableWindow(false);
	}
	if (b2->GetCheck()) {
		GetDlgItem(IDC_EDIT_START_PAGE)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_END_PAGE)->EnableWindow(true);
	}
	return;
}

VOID CSIReportPrintDlg::OnbtnPRINT() 
{
	CWnd *pMsg = GetDlgItem(IDC_STATIC2);
	CProgressCtrl *pBar = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS1);

	CButton *b1 = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	CButton *b2 = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
	UpdateData(true);
	if (b1->GetCheck()) {
		m_pManager->PrintDocument(m_nPaper, 1, m_nTotalPages, pMsg, pBar);
	} else {
		m_pManager->PrintDocument(m_nPaper, m_nStartPage, m_nEndPage, pMsg, pBar);
	}
	IsPrint = TRUE;
	CDialog::OnOK();
}

VOID CSIReportPrintDlg::OnbtnCLOSE() 
{
	CDialog::OnOK();
}

VOID CSIReportPrintDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	UpdateData(true);
	if (m_nPaper == 1 && -1*pNMUpDown->iDelta == -1) return;
	m_nPaper += (-1*pNMUpDown->iDelta);
	UpdateData(false);
	*pResult = 0;
}
