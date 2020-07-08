// DataProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingDlg dialog


CDataProcessingDlg::CDataProcessingDlg(CWnd * pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDataProcessingDlg)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
	m_bCancel = FALSE;
	m_bCreate = FALSE;
	m_pParentWnd = pParent;
}
CDataProcessingDlg::~CDataProcessingDlg()
{
	
}

VOID CDataProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataProcessingDlg)
	DDX_Control(pDX, IDC_prgDATA_PROCESSING, m_prgDataProcessing);
	DDX_Text(pDX, IDC_stcDATA_PROCESSING, m_strMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataProcessingDlg, CDialog)
	//{{AFX_MSG_MAP(CDataProcessingDlg)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingDlg message handlers

BOOL CDataProcessingDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CDataProcessingDlg::SetMessage(CString newMsg)
{
	m_strMessage = newMsg;
	CButton * pBtn = (CButton*)GetDlgItem(IDC_stcDATA_PROCESSING);
	pBtn->SetWindowText(m_strMessage);
	//dateData(FALSE);
}

CButton * CDataProcessingDlg::GetDataProcessingMsgCtrl()
{
	return (CButton*)GetDlgItem(IDC_stcDATA_PROCESSING);
}

VOID CDataProcessingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	SendMsgToParent();
	CDialog::OnCancel();
}

VOID CDataProcessingDlg::SendMsgToParent()
{
	m_pParentWnd->SendMessage(JOB_CANCEL_MSG, 0, 0);
}

VOID CDataProcessingDlg::InitPos()
{
	m_prgDataProcessing.SetPos(0);
}

VOID CDataProcessingDlg::ShowHideCancelButton(BOOL bShowHide/*=TRUE*/)
{
	if(bShowHide) GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	else GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
}

BOOL CDataProcessingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDataProcessingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
