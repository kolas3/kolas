// SeMemoEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SeMemoEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMemoEdit dialog


CSeMemoEdit::CSeMemoEdit(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMemoEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMemoEdit)
	m_strMEMO = _T("");
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}

CSeMemoEdit::~CSeMemoEdit()
{
}

VOID CSeMemoEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMemoEdit)
	DDX_Text(pDX, IDC_richeditMEMO, m_strMEMO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMemoEdit, CDialog)
	//{{AFX_MSG_MAP(CSeMemoEdit)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMemoEdit message handlers

BOOL CSeMemoEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeMemoEdit::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);

	CDialog::OnOK();
}

HBRUSH CSeMemoEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
