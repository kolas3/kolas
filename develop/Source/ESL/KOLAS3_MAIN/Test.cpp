// Test.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Test dialog


Test::Test(CWnd* pParent /*=NULL*/)
	: CDialog(Test::IDD, pParent)
{
	//{{AFX_DATA_INIT(Test)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Test)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Test, CDialog)
	//{{AFX_MSG_MAP(Test)
	ON_BN_CLICKED(IDC_BUTTON_MAIN1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Test message handlers

void Test::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL Test::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL Test::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	( (CButton*) GetDlgItem (IDC_BUTTON_MAIN1) )->EnableWindow(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
