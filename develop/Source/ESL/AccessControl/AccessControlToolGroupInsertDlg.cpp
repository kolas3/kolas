// AccessControlToolGroupInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlToolGroupInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolGroupInsertDlg dialog


CAccessControlToolGroupInsertDlg::CAccessControlToolGroupInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlToolGroupInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlToolGroupInsertDlg)
	m_GroupAlias = _T("");
	m_GroupDesc = _T("");
	//}}AFX_DATA_INIT
	m_eGroupAlign = 'T';
	// KOL.UDF.022
	MODE = 1;
}


VOID CAccessControlToolGroupInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlToolGroupInsertDlg)
	DDX_Control(pDX, IDC_GroupAlign, m_GroupAlign);
	DDX_Text(pDX, IDC_GroupAlias, m_GroupAlias);
	DDX_Text(pDX, IDC_GroupDesc, m_GroupDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlToolGroupInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlToolGroupInsertDlg)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolGroupInsertDlg message handlers

BOOL CAccessControlToolGroupInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	NewButtonList.Add(IDC_bOK, this, IDB_OK_U, IDB_OK_D, IDB_OK_F);
	NewButtonList.Add(IDC_bCANCEL, this, IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);

	// TODO: Add extra initialization here
	m_GroupAlign.ResetContent();
	m_GroupAlign.AddString(_T("위쪽(Top)"));
	m_GroupAlign.AddString(_T("아랫쪽(Bottom)"));
	m_GroupAlign.AddString(_T("왼쪽(Left)"));
	m_GroupAlign.AddString(_T("오른쪽(Right)"));

	if (m_eGroupAlign == 'T') m_GroupAlign.SetCurSel(0);
	if (m_eGroupAlign == 'B') m_GroupAlign.SetCurSel(1);
	if (m_eGroupAlign == 'L') m_GroupAlign.SetCurSel(2);
	if (m_eGroupAlign == 'R') m_GroupAlign.SetCurSel(3);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlToolGroupInsertDlg::OnbCANCEL() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

VOID CAccessControlToolGroupInsertDlg::OnbOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if (m_GroupAlias.GetLength() == 0) {
		AfxMessageBox(_T("Group Alias를 입력하십시오."));
		return;
	}
	if (m_GroupAlign.GetCurSel() == 0) m_eGroupAlign = 'T';
	if (m_GroupAlign.GetCurSel() == 1) m_eGroupAlign = 'B';
	if (m_GroupAlign.GetCurSel() == 2) m_eGroupAlign = 'L';
	if (m_GroupAlign.GetCurSel() == 3) m_eGroupAlign = 'R';

	CDialog::OnOK();

}
