// DlgPageSetup.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgPageSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPageSetup dialog


CDlgPageSetup::CDlgPageSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPageSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPageSetup)
	m_nOrientation = -1;
	//}}AFX_DATA_INIT

	m_strPaper = _T("A4");
}


VOID CDlgPageSetup::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPageSetup)
	DDX_Control(pDX, IDC_BMP_PORTRAIT, m_bmpPortrait);
	DDX_Control(pDX, IDC_BMP_LANDSCAPE, m_bmpLandscape);
	DDX_Radio(pDX, IDC_rORIENTATION, m_nOrientation);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgPageSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgPageSetup)
	ON_BN_CLICKED(IDC_rORIENTATION, OnrORIENTATION)
	ON_BN_CLICKED(IDC_rORIENTATION2, OnrORIENTATION2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPageSetup message handlers

BOOL CDlgPageSetup::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	UpdateData(FALSE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_cPAGESIZE);
	pComboBox->AddString(_T("A4"));
	pComboBox->AddString(_T("B4"));
	pComboBox->SelectString(0, m_strPaper);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgPageSetup::OnrORIENTATION() 
{
EFS_BEGIN

	m_bmpLandscape.ShowWindow(SW_SHOW);
	m_bmpPortrait.ShowWindow(SW_HIDE);
	

EFS_END
}

VOID CDlgPageSetup::OnrORIENTATION2() 
{
EFS_BEGIN

	m_bmpLandscape.ShowWindow(SW_HIDE);
	m_bmpPortrait.ShowWindow(SW_SHOW);

EFS_END
}

VOID CDlgPageSetup::OnOK() 
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_cPAGESIZE);
	if (pComboBox) {
		pComboBox->GetWindowText(m_strPaper);
	}
	CDialog::OnOK();
}
