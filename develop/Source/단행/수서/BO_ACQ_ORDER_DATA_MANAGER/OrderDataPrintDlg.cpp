// OrderDataPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderDataPrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderDataPrintDlg dialog


COrderDataPrintDlg::COrderDataPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COrderDataPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderDataPrintDlg)
	m_nPrintType = 0;
	//}}AFX_DATA_INIT
}
COrderDataPrintDlg::~COrderDataPrintDlg()
{
}

VOID COrderDataPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderDataPrintDlg)
	DDX_Radio(pDX, IDC_radORDERLIST_ALL, m_nPrintType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderDataPrintDlg, CDialog)
	//{{AFX_MSG_MAP(COrderDataPrintDlg)
	ON_BN_CLICKED(IDC_radORDERLIST_ALL, OnradORDERLISTALL)
	ON_BN_CLICKED(IDC_radORDERLIST_PROVIDER, OnradORDERLISTPROVIDER)
	ON_BN_CLICKED(IDC_radORDERLIST_SUBJECTCODE, OnradORDERLISTSUBJECTCODE)
	ON_BN_CLICKED(IDC_radCHECKING_LIST, OnradCHECKINGLIST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderDataPrintDlg message handlers

BOOL COrderDataPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COrderDataPrintDlg::OnradORDERLISTALL() 
{
 	// TODO: Add your control notification handler code here
	m_nPrintType = 0;
	UpdateData(FALSE);
}

VOID COrderDataPrintDlg::OnradORDERLISTPROVIDER() 
{
	// TODO: Add your control notification handler code here
	m_nPrintType = 1;
	UpdateData(FALSE);
}

VOID COrderDataPrintDlg::OnradORDERLISTSUBJECTCODE() 
{
	// TODO: Add your control notification handler code here
	m_nPrintType = 3;
	UpdateData(FALSE);
}

VOID COrderDataPrintDlg::OnradCHECKINGLIST() 
{
	// TODO: Add your control notification handler code here
	m_nPrintType = 2;
	UpdateData(FALSE);
}

VOID COrderDataPrintDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nPrintType < 0)
	{
		ESLAfxMessageBox(_T("출력항목을 선정해 주십시요."));
		return;
	}	
	CDialog::OnOK();
}

HBRUSH COrderDataPrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}