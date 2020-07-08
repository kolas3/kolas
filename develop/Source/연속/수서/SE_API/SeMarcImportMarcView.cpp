// SeMarcImportMarcView.cpp : implementation file
//

#include "stdafx.h"
#include "SeMarcImportMarcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImportMarcView dialog


CSeMarcImportMarcView::CSeMarcImportMarcView(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMarcImportMarcView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMarcImportMarcView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_pMarc = NULL;
}
CSeMarcImportMarcView::~CSeMarcImportMarcView()
{
}

VOID CSeMarcImportMarcView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMarcImportMarcView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMarcImportMarcView, CDialog)
	//{{AFX_MSG_MAP(CSeMarcImportMarcView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImportMarcView message handlers

BOOL CSeMarcImportMarcView::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(!m_pMarc) {
		AfxMessageBox(_T("마크 구조체를 찾을 수 없습니다!"));
		return FALSE;
	}
	
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->CheckSimpleSyntax(m_pMarc);
	m_marcEditor.SubclassDlgItem( IDC_richeditMARC_IMPORT_MARC_VIEWER, -1, this );
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );
	m_marcEditor.Init( m_pInfo->pMarcMgr, m_pMarc );
	m_marcEditor.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSeMarcImportMarcView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
