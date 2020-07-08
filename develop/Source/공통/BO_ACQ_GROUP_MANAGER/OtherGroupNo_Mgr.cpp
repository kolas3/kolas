// OtherGroupNo_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Mgr dialog


COtherGroupNo_Mgr::COtherGroupNo_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_Mgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COtherGroupNo_Mgr::~COtherGroupNo_Mgr()
{
}
VOID COtherGroupNo_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_Mgr, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_Mgr)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Mgr message handlers

BOOL COtherGroupNo_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH COtherGroupNo_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
