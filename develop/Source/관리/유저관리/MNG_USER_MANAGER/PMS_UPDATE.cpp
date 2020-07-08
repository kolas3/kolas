// PMS_UPDATE.cpp : implementation file
//

#include "stdafx.h"
#include "PMS_UPDATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPMS_UPDATE dialog


CPMS_UPDATE::CPMS_UPDATE(CWnd* pParent /*=NULL*/)
	: CDialog(CPMS_UPDATE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMS_UPDATE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPMS_UPDATE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMS_UPDATE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPMS_UPDATE, CDialog)
	//{{AFX_MSG_MAP(CPMS_UPDATE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPMS_UPDATE message handlers

void CPMS_UPDATE::OnOK() 
{
	CString strName;
	CString strDesc;

	( (CEdit*) GetDlgItem (IDC_edtNAME ) )->GetWindowText(strName);
	( (CEdit*) GetDlgItem (IDC_edtDESC ) )->GetWindowText(strDesc);

	m_strPMSName = strName;
	m_strPMSDesc = strDesc;	
	
	CDialog::OnOK();
}

void CPMS_UPDATE::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


VOID CPMS_UPDATE::SetData(CString strName, CString strDesc)
{
	m_strPMSName = strName;
	m_strPMSDesc = strDesc;	
}
VOID CPMS_UPDATE::GetData(CString &strName, CString &strDesc)
{
	strName = m_strPMSName;
	strDesc = m_strPMSDesc;	
	
}

BOOL CPMS_UPDATE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	( (CEdit*) GetDlgItem (IDC_edtNAME ) )->SetWindowText(m_strPMSName);
	( (CEdit*) GetDlgItem (IDC_edtDESC ) )->SetWindowText(m_strPMSDesc);
	SetWindowText(m_strAlias);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
