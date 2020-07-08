// QueryEditor_DLG.cpp : implementation file
//

#include "stdafx.h"
//#include "kolas2up_mobilecommon.h"
#include "QueryEditor_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueryEditor_DLG dialog


CQueryEditor_DLG::CQueryEditor_DLG(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueryEditor_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nStatus = -1;
	m_strErrQuery = "";
}

CQueryEditor_DLG::~CQueryEditor_DLG()
{
}

void CQueryEditor_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueryEditor_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQueryEditor_DLG, CDialog)
	//{{AFX_MSG_MAP(CQueryEditor_DLG)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MODIFYEXE, OnBtnModifyexe)
	ON_BN_CLICKED(IDC_BTN_IGNOREEXE, OnBtnIgnoreexe)
	ON_BN_CLICKED(IDC_BTN_SYNCSTOP, OnBtnSyncstop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueryEditor_DLG message handlers

HBRUSH CQueryEditor_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CQueryEditor_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDT_QUERY)->SetWindowText( m_strErrQuery );
	EnableThemeDialogTexture(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQueryEditor_DLG::OnBtnModifyexe() 
{
	// TODO: Add your control notification handler code here
	// 수정후 실행
	m_nStatus = 1;

	GetDlgItem(IDC_EDT_QUERY)->GetWindowText( m_strErrQuery );
	CDialog::OnOK();
}

void CQueryEditor_DLG::OnBtnIgnoreexe() 
{
	// TODO: Add your control notification handler code here
	// 무시후 진행
	m_nStatus = 2;
	CDialog::OnOK();
}

void CQueryEditor_DLG::OnBtnSyncstop() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnClose();
}
