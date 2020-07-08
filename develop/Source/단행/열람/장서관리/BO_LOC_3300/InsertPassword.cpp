// InsertPassword.cpp : implementation file
//

#include "stdafx.h"
#include "InsertPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertPassword dialog


CInsertPassword::CInsertPassword(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CInsertPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertPassword)
	m_sPassWord = _T("");
	//}}AFX_DATA_INIT

	m_nCloseMode = 0;
	m_nViewMode = 0;
}

CInsertPassword::~CInsertPassword()
{

}

VOID CInsertPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertPassword)
	DDX_Text(pDX, IDC_edtRPASS, m_sPassWord);
	DDV_MaxChars(pDX, m_sPassWord, 20);	//++2008.12.05 UPDATE BY KSJ - 비밀번호 최대 20자리로
	//DDV_MaxChars(pDX, m_sPassWord, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertPassword, CDialog)
	//{{AFX_MSG_MAP(CInsertPassword)
	ON_BN_CLICKED(IDC_btnRINSERT, OnbtnRINSERT)
	ON_BN_CLICKED(IDC_btnRCLOSE, OnbtnRCLOSE)
	ON_BN_CLICKED(IDC_btnRINSERT2, OnbtnRINSERT2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertPassword message handlers

BOOL CInsertPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	CWnd* pWnd	  = NULL;
	CButton* pBtn = NULL;
	if( 0 == m_nViewMode )
	{		
		pWnd = GetDlgItem( IDC_btnRINSERT );
		pWnd->EnableWindow();
		pBtn = (CButton*)GetDlgItem( IDC_radNEW );
		pBtn->SetCheck(1);
	}
	else if( 1 == m_nViewMode )
	{
		pWnd = GetDlgItem( IDC_btnRINSERT2 );
		pWnd->EnableWindow();
		pBtn = (CButton*)GetDlgItem( IDC_radAGAIN );
		pBtn->SetCheck(1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CInsertPassword::OnbtnRINSERT() 
{
	UpdateData(TRUE);

	INT		nLength		=	m_sPassWord.GetLength();
	if( 4 > nLength || 20 < nLength )
	{
		AfxMessageBox(_T("암호는 4자리 이상, 20자리 이하로 입력하여 주십시요."), MB_ICONEXCLAMATION);
		return ;
	}

	m_nCloseMode = 1;
	OnCancel();
}

VOID CInsertPassword::OnbtnRCLOSE() 
{
	OnCancel();
}

CString CInsertPassword::GetPass()
{
	return m_sPassWord;
}

INT CInsertPassword::GetCloseMode()
{
	return m_nCloseMode;
}

VOID CInsertPassword::OnbtnRINSERT2() 
{
	OnbtnRINSERT();
}

HBRUSH CInsertPassword::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CInsertPassword::PreTranslateMessage(MSG* pMsg) 
{
	
	if( WM_KEYDOWN == pMsg->message && 
	   	VK_RETURN  == pMsg->wParam  )
	{
		if( GetDlgItem(IDC_edtRPASS)->GetSafeHwnd() == pMsg->hwnd )
		{
			OnbtnRINSERT();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
