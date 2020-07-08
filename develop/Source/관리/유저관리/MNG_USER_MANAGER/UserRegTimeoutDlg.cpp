// UserRegTimeoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserRegTimeoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserRegTimeoutDlg dialog


CUserRegTimeoutDlg::CUserRegTimeoutDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserRegTimeoutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserRegTimeoutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bReadOnly = FALSE;
	m_nTimeout = 0;	
}


void CUserRegTimeoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserRegTimeoutDlg)
	DDX_Control(pDX, IDC_COMBO_TIMEOUT, m_cmbTimeout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserRegTimeoutDlg, CDialog)
	//{{AFX_MSG_MAP(CUserRegTimeoutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserRegTimeoutDlg message handlers

BOOL CUserRegTimeoutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	// TODO: Add extra initialization here
	m_cmbTimeout.AddString(_T("10"));
	m_cmbTimeout.AddString(_T("20"));
	m_cmbTimeout.AddString(_T("30"));
	m_cmbTimeout.AddString(_T("60"));
	m_cmbTimeout.SetCurSel(0);

	if( m_nTimeout == 10 )
	{
		m_cmbTimeout.SetCurSel(0);
	}
	else if( m_nTimeout == 20 )
	{
		m_cmbTimeout.SetCurSel(1);
	}
	else if( m_nTimeout == 30 )
	{
		m_cmbTimeout.SetCurSel(2);
	}
	else if( m_nTimeout == 60 )
	{
		m_cmbTimeout.SetCurSel(3);
	}
	else if(m_nTimeout > 0)
	{
		CString strTmp = _T("");
		strTmp.Format(_T("%d"), m_nTimeout);
		m_cmbTimeout.SetWindowText(strTmp);
	}
	else
	{
		m_cmbTimeout.SetCurSel(0);
	}
	if(m_bReadOnly)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TIMEOUT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserRegTimeoutDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmp = _T("");

	m_cmbTimeout.GetWindowText(strTmp);

	m_nTimeout = _ttoi(strTmp);

	if(m_nTimeout < 0)
		m_nTimeout = 0;
	
	CDialog::OnOK();
}

void CUserRegTimeoutDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
