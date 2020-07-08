// OrderDataCancelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderDataCancelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderDataCancelDlg dialog


COrderDataCancelDlg::COrderDataCancelDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COrderDataCancelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderDataCancelDlg)
	m_nOrderCancelType = -1;
	//}}AFX_DATA_INIT
}
COrderDataCancelDlg::~COrderDataCancelDlg()
{
}

VOID COrderDataCancelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderDataCancelDlg)
	DDX_Radio(pDX, IDC_radDIVISIONNO_ORDER_CANCEL, m_nOrderCancelType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderDataCancelDlg, CDialog)
	//{{AFX_MSG_MAP(COrderDataCancelDlg)
	ON_BN_CLICKED(IDC_radDIVISIONNO_ORDER_CANCEL, OnradDIVISIONNOORDERCANCEL)
	ON_BN_CLICKED(IDC_radSELDATA_ORDER_CANCEL, OnradSELDATAORDERCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderDataCancelDlg message handlers

BOOL COrderDataCancelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nOrderCancelType = 0;
	UpdateData(FALSE);

	OnradSELDATAORDERCANCEL();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COrderDataCancelDlg::OnradDIVISIONNOORDERCANCEL() 
{
	// TODO: Add your control notification handler code here
	m_nOrderCancelType = 0;
	UpdateData(FALSE);
}

VOID COrderDataCancelDlg::OnradSELDATAORDERCANCEL() 
{
	// TODO: Add your control notification handler code here
	m_nOrderCancelType = 1;
	UpdateData(FALSE);
}

VOID COrderDataCancelDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

HBRUSH COrderDataCancelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}