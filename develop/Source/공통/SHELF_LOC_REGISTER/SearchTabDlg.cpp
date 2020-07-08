// SearchTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "shelf_loc_register.h"
#include "SearchTabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchTabDlg dialog


CSearchTabDlg::CSearchTabDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_initShow = 0;
}

CSearchTabDlg::~CSearchTabDlg()
{
		
}

void CSearchTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchTabDlg)
	DDX_Control(pDX, IDC_TAB2, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchTabDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchTabDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, OnSelchangeTab2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchTabDlg message handlers

BOOL CSearchTabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_tab.InsertItem(SHELF_LOC_REG, _T(" 단행 ") );
	m_tab.InsertItem(SHELF_LOC_SEARCH, _T(" 연속 ") );

	CreateDlgs();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSearchTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CSearchTabDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_tab.SetCurSel( m_initShow );
	ShowDlg(m_initShow);	
}

void CSearchTabDlg::DlgQuit()
{
	this->OnOK();
}

void CSearchTabDlg::OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowDlg( m_tab.GetCurSel() );
	*pResult = 0;
}

void CSearchTabDlg::ShowDlg( int idx )
{
	const int x = 14;
	const int y = 31;
	const int length = 823;
	const int height = 390;
	CRect rect( x, y, x + length, y + height );
	
	switch( idx )
 	{
 		case 0 :
 				
 			m_SearchResultDlg->MoveWindow( rect );
 			m_SearchResultDlg->ShowWindow( true );
			m_SearchResultDlg->SetFocus();
 
			m_SearchResult_SEDlg->ShowWindow( false );
 
 			break;
 
 		case 1 :
 
   			m_SearchResult_SEDlg->MoveWindow( rect );
   			m_SearchResult_SEDlg->ShowWindow( true );
			m_SearchResult_SEDlg->SetFocus();
  
  			m_SearchResultDlg->ShowWindow( false );
 
 			break;
	}
}

VOID CSearchTabDlg::CreateDlgs()
{
//	return;

	m_SearchResultDlg = new CSearchResult( this );
	m_SearchResultDlg->Create( IDD_SEARCH_RESULT_BO, this );

	m_SearchResult_SEDlg = new CSearchResult_SE(this);
	m_SearchResult_SEDlg->Create( IDD_SEARCH_RESULT_SE,this );
}
