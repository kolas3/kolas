// BO_CAT_PROGRESS_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROGRESS_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROGRESS_DLG dialog


CBO_CAT_PROGRESS_DLG::CBO_CAT_PROGRESS_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROGRESS_DLG)
	//}}AFX_DATA_INIT
	m_ProgressCtrl = NULL ;
}

CBO_CAT_PROGRESS_DLG::~CBO_CAT_PROGRESS_DLG()
{
}

VOID CBO_CAT_PROGRESS_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROGRESS_DLG)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROGRESS_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROGRESS_DLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROGRESS_DLG message handlers
BOOL CBO_CAT_PROGRESS_DLG::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}



VOID CBO_CAT_PROGRESS_DLG::OnCancel() 
{
	// TODO: Add your control notification handler code here
    OnCancel () ;
}

BOOL CBO_CAT_PROGRESS_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    m_ProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_bar);
    
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROGRESS_DLG ::SetTitle (CString m_StrTitle)
{
	GetDlgItem(IDC_TextMsg)->SetWindowText(m_StrTitle) ;
}

HBRUSH CBO_CAT_PROGRESS_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
