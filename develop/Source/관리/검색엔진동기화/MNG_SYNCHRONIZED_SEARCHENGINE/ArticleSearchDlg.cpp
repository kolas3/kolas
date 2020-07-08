// ArticleSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArticleSearchDlg.h"
#include "SynchronizedSearchEngineMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArticleSearchDlg dialog


CArticleSearchDlg::CArticleSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArticleSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArticleSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}

CArticleSearchDlg::~CArticleSearchDlg()
{	
	
}


VOID CArticleSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArticleSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArticleSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CArticleSearchDlg)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArticleSearchDlg message handlers

VOID CArticleSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CSynchronizedSearchEngineMgr *pParent = ( CSynchronizedSearchEngineMgr * )m_pParent;
	pParent->m_articleSCond.Format( _T("%s"), GetSearchWhere() );
	::PostMessage( m_pParent->m_hWnd, SEARCH_ARTICLE, 0, 0 );

	//CDialog::OnOK();
}

CString CArticleSearchDlg::GetSearchWhere()
{
	CString where, tmp;
	CEdit *pEdt;

	const INT cnt = 4;
	UINT ctrlID[ cnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };
	CString field[ cnt ] = {
		_T("VOL_INFO"), _T("IDX_ARTICLE_AUTHOR"), _T("IDX_ARTICLE_KEYWORD"), _T("IDX_ARTICLE_TOC_KEYWORD") };

	CString data;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( ctrlID[ i ] );
		pEdt->GetWindowText( data );
		data.TrimLeft();
		data.TrimRight();

		if( !data.IsEmpty() )
		{
			if( i )
				tmp.Format( _T(" CATSEARCH( %s, '%s*', NULL ) > 0 AND"), field[ i ], data );
			else
				tmp.Format( _T(" %s LIKE '%%%s%%' AND"), field[ i ], data );
			where += tmp;
		}
	}
	
	where.TrimLeft();
	where.TrimRight();
	if( !where.IsEmpty() )
		where = where.Left( where.GetLength() - 3 );	
	
	return where;
}

BOOL CArticleSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CArticleSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}