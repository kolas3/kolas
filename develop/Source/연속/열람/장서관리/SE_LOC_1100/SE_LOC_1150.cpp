// SE_LOC_1150.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1150.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1150 dialog


CSE_LOC_1150::CSE_LOC_1150(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1150)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bGabPrintOpt = FALSE;
}

CSE_LOC_1150::~CSE_LOC_1150()
{
}

VOID CSE_LOC_1150::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1150)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1150, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1150)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_cGABPrintOpt, OncGABPrintOpt)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1150 message handlers

BOOL CSE_LOC_1150::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SE_LOC_1150")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1150::OnbtnPRINT() 
{
	GetControlData( _T("CM_SE_LOC_1150") , _T("인계일자") , m_sPassDate );
	OnCancel();
}

VOID CSE_LOC_1150::OnbtnClose() 
{
	OnCancel();	
}

VOID CSE_LOC_1150::OncGABPrintOpt() 
{
	CButton* pBtn = ( CButton* )GetDlgItem( IDC_cGABPrintOpt );
	if( pBtn->GetCheck() )
		m_bGabPrintOpt = TRUE;
	else
		m_bGabPrintOpt = FALSE;			
}

HBRUSH CSE_LOC_1150::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
