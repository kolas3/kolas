// BOMarcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BOMarcEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOMarcEdit dialog

CBOMarcEdit::CBOMarcEdit(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOMarcEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	SetMarc(pMarc);

	m_bShowErrorDlg = FALSE;

	AfxInitRichEdit();
}

BOOL CBOMarcEdit::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOMarcEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOMarcEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOMarcEdit, CDialog)
	//{{AFX_MSG_MAP(CBOMarcEdit)
	ON_BN_CLICKED(IDC_bAPPLY_MARC, OnbAPPLYMARC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOMarcEdit message handlers

BOOL CBOMarcEdit::OnInitDialog() 
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
	m_marcEditor.Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOMarcEdit::OnbAPPLYMARC() 
{
	if (m_bShowErrorDlg)
		m_marcEditor.ApplyEdit(0);
	else 
		m_marcEditor.ApplyEdit(1);	
}
