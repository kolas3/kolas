// PURCHASING_PRINT.cpp : implementation file
//

#include "stdafx.h"
#include "PURCHASING_PRINT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPURCHASING_PRINT dialog


CPURCHASING_PRINT::CPURCHASING_PRINT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPURCHASING_PRINT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPURCHASING_PRINT)
	m_PurchasingPrintType = 0;
	//}}AFX_DATA_INIT
}


VOID CPURCHASING_PRINT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPURCHASING_PRINT)
	DDX_Radio(pDX, IDC_radPURCHASING_CHECKING_LIST, m_PurchasingPrintType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPURCHASING_PRINT, CDialog)
	//{{AFX_MSG_MAP(CPURCHASING_PRINT)
	ON_BN_CLICKED(IDC_radPURCHASING_CHECKING_LIST, OnradPURCHASINGCHECKINGLIST)
	ON_BN_CLICKED(IDC_radPURCHASING_CHECKED_LIST, OnradPURCHASINGCHECKEDLIST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPURCHASING_PRINT message handlers

BOOL CPURCHASING_PRINT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CPURCHASING_PRINT::GetPurchasingPrintType()
{
	return m_PurchasingPrintType;
}

VOID CPURCHASING_PRINT::OnradPURCHASINGCHECKINGLIST() 
{
	// TODO: Add your control notification handler code here
	m_PurchasingPrintType = 0;
	UpdateData(FALSE);
}

VOID CPURCHASING_PRINT::OnradPURCHASINGCHECKEDLIST() 
{
	// TODO: Add your control notification handler code here
	m_PurchasingPrintType = 1;
	UpdateData(FALSE);	
}

VOID CPURCHASING_PRINT::OnOK() 
{
	// TODO: Add extra validation here
	if(m_PurchasingPrintType < 0)
	{
		ESLAfxMessageBox(_T("출력항목을 선정해 주십시요."));
		return;
	}
	CDialog::OnOK();
}
