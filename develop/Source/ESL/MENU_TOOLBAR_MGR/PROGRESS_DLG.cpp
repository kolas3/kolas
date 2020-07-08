// PROGRESS_DLG.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "PROGRESS_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPROGRESS_DLG dialog


CPROGRESS_DLG::CPROGRESS_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CPROGRESS_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPROGRESS_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strResult = _T("") ;
//	AfxInitRichEdit () ;
}


VOID CPROGRESS_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPROGRESS_DLG)
	DDX_Control(pDX, IDC_PROGRESS_UPLOAD, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPROGRESS_DLG, CDialog)
	//{{AFX_MSG_MAP(CPROGRESS_DLG)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPROGRESS_DLG message handlers

VOID CPROGRESS_DLG::SetMessage(CString strMsg)
{
	
	CButton *pMsg = (CButton*)GetDlgItem(IDC_MSG);
	pMsg->SetWindowText (strMsg) ;

}

VOID CPROGRESS_DLG::ShowResult(CString strResult)
{
/*
	if(m_strResult != _T(""))
	{
		m_strResult += _T("\n") + strResult ;
	}
    else
	{
		m_strResult = strResult ;
	}

	CButton *pMsg = (CButton *)GetDlgItem (IDC_EDIT_MSG) ;
	pMsg->SetWindowText (m_strResult ) ;

*/
}

BOOL CPROGRESS_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    //SetTimer(10,10,NULL) ;

	//CProgressCtrl * pCtrl = (CProgressCtrl *)GetDlgItem (IDC_PROGRESS_UPLOAD);
	//pCtrl->ShowWindow (SW_SHOW) ;



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPROGRESS_DLG::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
/*	
	if(nIDEvent == 10)
	{
		KillTimer(10);
		//GetDlgItem(IDC_PROGRESS_UPLOAD)->RedrawWindow();
		//GetDlgItem(IDC_RICHEDIT_MSG)->RedrawWindow();
		SetTimer(10,10,NULL) ;
	}
*/

	CDialog::OnTimer(nIDEvent);
}
