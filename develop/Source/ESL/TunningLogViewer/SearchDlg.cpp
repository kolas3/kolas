// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TunningLogViewer.h"
#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog


CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSearchDirection = 0;
	m_bIsLowerUpper = FALSE;
}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
	DDX_Control(pDX, IDC_EDIT_SEARCH_TEXT, mCtrl_edtSearchText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg message handlers

void CSearchDlg::OnBtnFind() 
{
	// TODO: Add your control notification handler code here
	mCtrl_edtSearchText.GetWindowText(m_strSearchText);
	
	CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_UP);
	if(NULL != pBtn && NULL != pBtn->GetSafeHwnd())
	{
		if(TRUE == pBtn->GetCheck())
			m_nSearchDirection = SEARCH_TYPE_PREV_SEARCH;
		else 
			m_nSearchDirection = SEARCH_TYPE_NEXT_SEARCH;
	}

	pBtn = (CButton*)GetDlgItem(IDC_CHECK_LOWER);
	if(NULL != pBtn && NULL != pBtn->GetSafeHwnd())
	{
		m_bIsLowerUpper = pBtn->GetCheck();		
	}	

	EndDialog(IDOK);
}

BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnBtnFind();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSearchDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	CButton *pBtnUp = (CButton*)GetDlgItem(IDC_RADIO_UP);
	CButton *pBtnDown = (CButton*)GetDlgItem(IDC_RADIO_DOWN);	
	if(NULL != pBtnUp && NULL != pBtnUp->GetSafeHwnd())
	{
		if(SEARCH_TYPE_PREV_SEARCH == m_nSearchDirection)
		{
			pBtnUp->SetCheck(TRUE);
			pBtnDown->SetCheck(FALSE);
		}
		else
		{
			pBtnUp->SetCheck(FALSE);
			pBtnDown->SetCheck(TRUE);
		}

	}

	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK_LOWER);
	if(NULL != pBtn && NULL != pBtn->GetSafeHwnd())
	{
		pBtn->SetCheck(m_bIsLowerUpper);
	}	

	mCtrl_edtSearchText.SetWindowText(m_strSearchText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
