// BO_LOC_3290.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3290.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3290 dialog


CBO_LOC_3290::CBO_LOC_3290(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOC_3290::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3290)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMode = -1;
}

CBO_LOC_3290::~CBO_LOC_3290()
{
}

VOID CBO_LOC_3290::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3290)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3290, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3290)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3290 message handlers

VOID CBO_LOC_3290::OnbtnClose() 
{
	OnCancel();
}

VOID CBO_LOC_3290::OnbtnPREV() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_RADIO1);
	if( !pBtn ) return;

	if( pBtn->GetCheck() )
	{
		m_nMode = 0;
	}

	pBtn = (CButton*)GetDlgItem(IDC_RADIO2);
	if( !pBtn ) return;

	if( pBtn->GetCheck() )
	{
		m_nMode = 1;
	}

	OnOK();
}

BOOL CBO_LOC_3290::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBO_LOC_3290::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
