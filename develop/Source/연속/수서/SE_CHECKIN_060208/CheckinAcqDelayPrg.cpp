// CheckinAcqDelayPrg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinAcqDelayPrg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelayPrg dialog


CCheckinAcqDelayPrg::CCheckinAcqDelayPrg(CESL_Mgr* pParent /*=NULL*/)	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinAcqDelayPrg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCheckinAcqDelayPrg::~CCheckinAcqDelayPrg()
{
}

VOID CCheckinAcqDelayPrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinAcqDelayPrg)
	DDX_Control(pDX, IDC_prgDOWN, m_ctrlPrgDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinAcqDelayPrg, CDialog)
	//{{AFX_MSG_MAP(CCheckinAcqDelayPrg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelayPrg message handlers
BOOL CCheckinAcqDelayPrg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CCheckinAcqDelayPrg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinAcqDelayPrg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
