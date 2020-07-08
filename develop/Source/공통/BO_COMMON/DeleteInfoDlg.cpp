// DeleteInfoDlg.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_common.h")
#include "DeleteInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeleteInfoDlg dialog


CDeleteInfoDlg::CDeleteInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDeleteInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
}


VOID CDeleteInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDeleteInfoDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteInfoDlg message handlers

VOID CDeleteInfoDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CDeleteInfoDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CDeleteInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_DELETE_INFO")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_DELETE_INFO"));
	m_pDM = FindDM(_T("DM_DELETE_INFO"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
