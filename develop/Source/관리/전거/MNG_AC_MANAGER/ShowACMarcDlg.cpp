// ShowACMarcDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "ShowACMarcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowACMarcDlg dialog


CShowACMarcDlg::CShowACMarcDlg(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowACMarcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CShowACMarcDlg::~CShowACMarcDlg()
{
}

BOOL CShowACMarcDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CShowACMarcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowACMarcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowACMarcDlg, CDialog)
	//{{AFX_MSG_MAP(CShowACMarcDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowACMarcDlg message handlers

VOID CShowACMarcDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
 
	return;
	
//	m_marcEditor.SubclassDlgItem( IDC_RICHEDIT1, -1, this );
//	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );

	/*
	if( m_pInfo->pMarcMgr->Decoding( m_marcString, &m_marc ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return;
	}
	*/
	//m_marcEditor.Init( m_pInfo->pMarcMgr, &m_marc );
	//m_marcEditor.Display();
}

BOOL CShowACMarcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Check m_pMarc
	if (m_pMarc == NULL)
	{
		AfxMessageBox(_T("pMarc Is NULL"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	// Subclass RichEdit
	if (!m_marcEditor.SubclassDlgItem(IDC_RICHEDIT_MARC, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
	
	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CShowACMarcDlg::Display()
{
EFS_BEGIN
	m_marcEditor.Display();

	if (m_bReadOnly)
	{
		CMarcEditCtrl *pEditCtrl = m_marcEditor.GetEditCtrl();
		if (pEditCtrl)
			pEditCtrl->SetReadOnly();
		
		CString strText;
		GetWindowText(strText);
		strText += _T("(읽기전용)");
		SetWindowText(strText);
	}
EFS_END

}

HBRUSH CShowACMarcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
