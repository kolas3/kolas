// CheckinAcqDelayProgressive.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinAcqDelayProgressive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelayProgressive dialog


CCheckinAcqDelayProgressive::CCheckinAcqDelayProgressive(CESL_Mgr* pParent /*=NULL*/)	: CESL_Mgr(CCheckinAcqDelayProgressive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinAcqDelayProgressive)
	m_strBottom = _T("");
	m_strTop = _T("");
	//}}AFX_DATA_INIT

	m_bCreate = FALSE;
}

CCheckinAcqDelayProgressive::~CCheckinAcqDelayProgressive()
{
}

VOID CCheckinAcqDelayProgressive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinAcqDelayProgressive)
	DDX_Control(pDX, IDC_prgACQ_DELAY_BOTTOM, m_ctrlProgressBottom);
	DDX_Control(pDX, IDC_prgACQ_DELAY_TOP, m_ctrlProgressTop);
	DDX_Text(pDX, IDC_stcACQ_DELAY_BOTTOM, m_strBottom);
	DDX_Text(pDX, IDC_stcACQ_DELAY_TOP, m_strTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinAcqDelayProgressive, CDialog)
	//{{AFX_MSG_MAP(CCheckinAcqDelayProgressive)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelayProgressive message handlers

BOOL CCheckinAcqDelayProgressive::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;

	return CDialog::Create(IDD, pParentWnd);
}


HBRUSH CCheckinAcqDelayProgressive::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinAcqDelayProgressive::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
