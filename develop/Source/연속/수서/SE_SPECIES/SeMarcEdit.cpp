// SeMarcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SeMarcEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMarcEdit dialog


CSeMarcEdit::CSeMarcEdit(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMarcEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMarcEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_pMarc = NULL;
}

CSeMarcEdit::~CSeMarcEdit()
{
}


VOID CSeMarcEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMarcEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMarcEdit, CDialog)
	//{{AFX_MSG_MAP(CSeMarcEdit)
	ON_BN_CLICKED(IDC_btnMARC_EDIT_CLOSE, OnbtnMARCEDITCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMarcEdit message handlers

BOOL CSeMarcEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(!m_pMarc) {
		AfxMessageBox(_T("마크 구조체를 찾을 수 없습니다!"));
		return FALSE;
	}
	
	m_pInfo->pMarcMgr->CheckSimpleSyntax(m_pMarc);
	m_marcEditor.SubclassDlgItem( IDC_richeditSPE_MARC, -1, this );
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );
	m_marcEditor.Init( m_pInfo->pMarcMgr, m_pMarc );
	m_marcEditor.Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeMarcEdit::OnbtnMARCEDITCLOSE() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);	
}

VOID CSeMarcEdit::OnOK() 
{
	m_marcEditor.ApplyEdit();
	
	// TODO: Add extra validation here
	CDialog::OnOK();
}

HBRUSH CSeMarcEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
