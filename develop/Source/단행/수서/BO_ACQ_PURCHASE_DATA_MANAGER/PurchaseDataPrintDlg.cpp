// PurchaseDataPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseDataPrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseDataPrintDlg dialog

CPurchaseDataPrintDlg::CPurchaseDataPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPurchaseDataPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchaseDataPrintDlg)
	m_PurchasingPrintType = 0;
	//}}AFX_DATA_INIT
}
CPurchaseDataPrintDlg::~CPurchaseDataPrintDlg()
{
}

VOID CPurchaseDataPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseDataPrintDlg)
	DDX_Radio(pDX, IDC_radPURCHASING_CHECKING_LIST, m_PurchasingPrintType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchaseDataPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CPurchaseDataPrintDlg)
	ON_BN_CLICKED(IDC_radPURCHASING_CHECKING_LIST, OnradPURCHASINGCHECKINGLIST)
	ON_BN_CLICKED(IDC_radPURCHASING_CHECKED_LIST, OnradPURCHASINGCHECKEDLIST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseDataPrintDlg message handlers

BOOL CPurchaseDataPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchaseDataPrintDlg::OnradPURCHASINGCHECKINGLIST() 
{
 	// TODO: Add your control notification handler code here
	m_PurchasingPrintType = 0;
	UpdateData(FALSE);	
}

VOID CPurchaseDataPrintDlg::OnradPURCHASINGCHECKEDLIST() 
{
	// TODO: Add your control notification handler code here
	m_PurchasingPrintType = 1;
	UpdateData(FALSE);	
}

VOID CPurchaseDataPrintDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_PurchasingPrintType < 0)
	{
		ESLAfxMessageBox(_T("출력항목을 선정해 주십시요."));
		return;
	}
	CDialog::OnOK();
}

HBRUSH CPurchaseDataPrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}