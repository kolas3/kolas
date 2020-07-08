// Code_Search_Dlg.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "Code_Search_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCode_Search_Dlg dialog




CCode_Search_Dlg::CCode_Search_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmd_Search_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCode_Search_Dlg::~CCode_Search_Dlg()
{
	
}


BOOL CCode_Search_Dlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}




VOID CCode_Search_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCode_Search_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCode_Search_Dlg, CDialog)
	//{{AFX_MSG_MAP(CCode_Search_Dlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCode_Search_Dlg message handlers

BOOL CCode_Search_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCode_Search_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
