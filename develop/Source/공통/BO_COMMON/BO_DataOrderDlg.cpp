// BO_DataOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_DataOrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_DataOrderDlg dialog


CBO_DataOrderDlg::CBO_DataOrderDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_DataOrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_DataOrderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_DataOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_DataOrderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_DataOrderDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_DataOrderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_DataOrderDlg message handlers

BOOL CBO_DataOrderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_주문일련번호")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_주문일련번호"));	
	if(!Init())
	{
		ESLAfxMessageBox(_T("화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
