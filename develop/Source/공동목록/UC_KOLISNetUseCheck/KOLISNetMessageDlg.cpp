// KOLISNetMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLISNetMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLISNetMessageDlg dialog


CKOLISNetMessageDlg::CKOLISNetMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKOLISNetMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKOLISNetMessageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CKOLISNetMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKOLISNetMessageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKOLISNetMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CKOLISNetMessageDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLISNetMessageDlg message handlers

BOOL CKOLISNetMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	( (CEdit*) GetDlgItem (IDC_redtMESSAGE) )->SetWindowText(m_strMsg);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CKOLISNetMessageDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnCancel();
	return;
	
	CDialog::OnLButtonDown(nFlags, point);
}

VOID CKOLISNetMessageDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnCancel();
	return;
	
	CDialog::OnRButtonDown(nFlags, point);
}


VOID CKOLISNetMessageDlg::SetMsg( CString sMsg )
{
	m_strMsg = sMsg;	
}
