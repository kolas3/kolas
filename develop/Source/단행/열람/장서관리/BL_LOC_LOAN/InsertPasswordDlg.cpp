// InsertPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InsertPasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertPasswordDlg dialog


CInsertPasswordDlg::CInsertPasswordDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CInsertPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertPasswordDlg)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT

	m_nCloseMode = 0;
	m_nViewMode = 0;
}
CInsertPasswordDlg::~CInsertPasswordDlg()
{

}

void CInsertPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertPasswordDlg)
	DDX_Text(pDX, IDC_edtPassword, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 20);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertPasswordDlg)
	ON_BN_CLICKED(IDC_btnNewIssue, OnbtnNewIssue)
	ON_BN_CLICKED(IDC_btnReIssue, OnbtnReIssue)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertPasswordDlg message handlers
BOOL CInsertPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	CWnd* pWnd	  = NULL;
	CButton* pBtn = NULL;
	if( 0 == m_nViewMode )
	{		
		pWnd = GetDlgItem( IDC_btnNewIssue );
		pWnd->EnableWindow();
		pBtn = (CButton*)GetDlgItem( IDC_radNewIssue );
		pBtn->SetCheck(1);
	}
	else if( 1 == m_nViewMode )
	{
		pWnd = GetDlgItem( IDC_btnReIssue );
		pWnd->EnableWindow();
		pBtn = (CButton*)GetDlgItem( IDC_radReIssue );
		pBtn->SetCheck(1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CInsertPasswordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CInsertPasswordDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam  )
	{
		if( GetDlgItem(IDC_edtPassword)->GetSafeHwnd() == pMsg->hwnd )
		{
			OnbtnNewIssue();
			return TRUE;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CInsertPasswordDlg::OnbtnNewIssue() 
{
	UpdateData(TRUE);

	INT nLength = m_strPassword.GetLength();
	if( 4 > nLength || 20 < nLength )
	{
		AfxMessageBox( _T("암호는 4자리 이상, 20자리 이하로 입력하여 주십시요.") , MB_ICONEXCLAMATION );
		return ;
	}

	m_nCloseMode = 1;
	OnCancel();
}

void CInsertPasswordDlg::OnbtnReIssue() 
{
	OnbtnNewIssue();
}

void CInsertPasswordDlg::OnbtnCANCEL() 
{
	OnCancel();
}

CString CInsertPasswordDlg::GetPassword()
{
	return m_strPassword;
}

INT CInsertPasswordDlg::GetCloseMode()
{
	return m_nCloseMode;
}