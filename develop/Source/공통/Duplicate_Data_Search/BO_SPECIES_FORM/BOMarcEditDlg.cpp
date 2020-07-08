// BOMarcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOMarcEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOMarcEditDlg dialog

CBOMarcEditDlg::CBOMarcEditDlg(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOMarcEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();

	SetMarc(pMarc);

	m_bShowErrorDlg = FALSE;
	m_bReadOnly = FALSE;
}

BOOL CBOMarcEditDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOMarcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOMarcEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOMarcEditDlg, CDialog)
	//{{AFX_MSG_MAP(CBOMarcEditDlg)
	ON_BN_CLICKED(IDC_bAPPLY_MARC, OnbAPPLYMARC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOMarcEditDlg Operations

VOID CBOMarcEditDlg::Display()
{
	// Marc Editor Display
	m_marcEditor.Display();

	if (m_bReadOnly)
	{
		CMarcEditCtrl *pEditCtrl = m_marcEditor.GetEditCtrl();
		if (pEditCtrl)
			pEditCtrl->SetReadOnly();
		GetDlgItem(IDC_bAPPLY_MARC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

		CString strText;
		GetWindowText(strText);
		strText += _T("(읽기전용)");
		SetWindowText(strText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBOMarcEditDlg message handlers

BOOL CBOMarcEditDlg::OnInitDialog() 
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOMarcEditDlg::OnbAPPLYMARC() 
{
	if (m_bShowErrorDlg)
		m_marcEditor.ApplyEdit(0);
	else 
		m_marcEditor.ApplyEdit(1);	
}
VOID CBOMarcEditDlg::OnOK() 
{
	OnbAPPLYMARC();
	CDialog::OnOK();
}
