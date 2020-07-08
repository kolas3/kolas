// ChorokTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditor_Test.h"
#include "ChorokTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChorokTestDlg dialog


CChorokTestDlg::CChorokTestDlg(CString sChorokText, CWnd* pParent /*=NULL*/)
	: CDialog(CChorokTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChorokTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sChorokText= sChorokText ;
	m_pTocEditorDlgWnd= pParent ;
}


VOID CChorokTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChorokTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChorokTestDlg, CDialog)
	//{{AFX_MSG_MAP(CChorokTestDlg)
	ON_BN_CLICKED(IDC_BUTTON_VIEWER, OnButtonViewer)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChorokTestDlg message handlers

BOOL CChorokTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect(100,100,400,500);
	m_ChorokDlg.MyCreate(this, m_pTocEditorDlgWnd, rect);
	if(!m_ChorokDlg.SetChorok(m_sChorokText))
		AfxMessageBox(_T("초록문법 Error"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CChorokTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sChorokText= m_ChorokDlg.GetChorokText();

	CDialog::OnOK();
}

CString CChorokTestDlg::GetChorokText()
{
	return m_sChorokText ;
}

VOID CChorokTestDlg::OnButtonViewer() 
{
	// TODO: Add your control notification handler code here
	m_sChorokText = m_ChorokDlg.GetChorokText();
	m_ChorokDlg.ShowViewer(TRUE, TRUE);
		
}

VOID CChorokTestDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	CString cstmp;
	// TODO: Add your message handler code here
	if(m_ChorokDlg.CheckChorokSyntax() != 0) {
		cstmp.Format(_T("%d"), m_ChorokDlg.CheckChorokSyntax());
		//AfxMessageBox(_T("초록문법 Error"));
		AfxMessageBox(cstmp);
	}
}
