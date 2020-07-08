// MARC_ANALYZER.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_ANALYZER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMARC_ANALYZER dialog


CMARC_ANALYZER::CMARC_ANALYZER(CWnd* pParent /*=NULL*/)
	: CDialog(CMARC_ANALYZER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_ANALYZER)
	//}}AFX_DATA_INIT
	m_strMarc = _T("");
	// KOL.UDF.022
	m_pMarcEditView = NULL;
}

CMARC_ANALYZER::~CMARC_ANALYZER()
{
	if(m_pMarcEditView)
		delete m_pMarcEditView;
}

VOID CMARC_ANALYZER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_ANALYZER)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_ANALYZER, CDialog)
	//{{AFX_MSG_MAP(CMARC_ANALYZER)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_ANALYZER message handlers

VOID CMARC_ANALYZER::SetMarc(CString strMarc)
{
	m_strMarc = strMarc;
}

BOOL CMARC_ANALYZER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;
	m_pMarcEditView = new CMarcEditCtrl();

	(( CStatic*) GetDlgItem (IDC_sMarcViewClient))->GetWindowRect(rect);
	
	ScreenToClient(&rect);
	m_pMarcEditView->Create(_T("RichEdit20W"), _T(""), WS_CHILD | ES_MULTILINE, rect, this, 10202 );
	m_pMarcEditView->ShowWindow(TRUE);	
	m_pMarcEditView->m_bIsAnalyzer = TRUE;
	m_pMarcEditView->SetWindowText(m_strMarc);
	m_pMarcEditView->SetFont(GetFont());

	m_pMarcEditView->FormatAll();
	
	return TRUE;
}

VOID CMARC_ANALYZER::OnApply() 
{
	m_pMarcEditView->GetWindowText(m_strMarc);	
	
	CDialog::OnOK();
}

INT CMARC_ANALYZER::DoModal() 
{
	return CDialog::DoModal();
}
