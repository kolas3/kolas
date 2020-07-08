// CheckInCancelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckInCancelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDlg dialog


CCheckInCancelDlg::CCheckInCancelDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckInCancelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckInCancelDlg)
	m_nCheckInCancelType = -1;
	//}}AFX_DATA_INIT
}
CCheckInCancelDlg::~CCheckInCancelDlg()
{
}

VOID CCheckInCancelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckInCancelDlg)
	DDX_Radio(pDX, IDC_radDIVISIONNO_CHECKIN_CANCEL, m_nCheckInCancelType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckInCancelDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckInCancelDlg)
	ON_BN_CLICKED(IDC_radSELDATA_CHECKIN_CANCEL, OnradSELDATACHECKINCANCEL)
	ON_BN_CLICKED(IDC_radDIVISIONNO_CHECKIN_CANCEL, OnradDIVISIONNOCHECKINCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckInCancelDlg message handlers

VOID CCheckInCancelDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CCheckInCancelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nCheckInCancelType = 0;
	UpdateData(FALSE);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCheckInCancelDlg::OnradSELDATACHECKINCANCEL() 
{
	// TODO: Add your control notification handler code here
	m_nCheckInCancelType = 1;
	UpdateData(FALSE);	
}

VOID CCheckInCancelDlg::OnradDIVISIONNOCHECKINCANCEL() 
{
	// TODO: Add your control notification handler code here
	m_nCheckInCancelType = 0;
	UpdateData(FALSE);	
}

HBRUSH CCheckInCancelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}