// SePurchaseProgress.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseProgress dialog


CSePurchaseProgress::CSePurchaseProgress(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSePurchaseProgress::~CSePurchaseProgress()
{
}

VOID CSePurchaseProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseProgress)
	DDX_Control(pDX, IDC_prgPURCHASE, m_ctrlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseProgress, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseProgress)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseProgress message handlers
BOOL CSePurchaseProgress::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CSePurchaseProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSePurchaseProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
