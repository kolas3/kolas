// MNG_KEYWORD_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_KEYWORD_SEARCH_DLG.h"
#include "MNG_KEYWORD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_SEARCH_DLG dialog


CMNG_KEYWORD_SEARCH_DLG::CMNG_KEYWORD_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KEYWORD_SEARCH_DLG::~CMNG_KEYWORD_SEARCH_DLG()
{
}

VOID CMNG_KEYWORD_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_SEARCH_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CMNG_KEYWORD_SEARCH_DLG::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}



BEGIN_MESSAGE_MAP(CMNG_KEYWORD_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_SEARCH_DLG)
	ON_BN_CLICKED(IDbSearch, OnbSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_SEARCH_DLG message handlers

VOID CMNG_KEYWORD_SEARCH_DLG::OnbSearch() 
{
EFS_BEGIN
	CString keyword;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eSearch );
	pEdt->GetWindowText( keyword );

	keyword.TrimLeft(); 
	keyword.TrimRight();

	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("검색어를 입력하여 주십시오") );
		return;
	}
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_주제어검색요어_주제어"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return ;
	}
	
	CMNG_KEYWORD *pmng = new CMNG_KEYWORD();
	INT ids = pmng->SearchKeyword(pDM, keyword);
	if( ids<0 )
		return ;

	AllControlDisplay(_T("CM_관리_주제어검색요어_메인"));

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		return;
	}
	


EFS_END
}

BOOL CMNG_KEYWORD_SEARCH_DLG::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN ) {
		
		if ( pMsg->wParam == VK_RETURN ) {
			OnbSearch();
			return TRUE;
		} 
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CMNG_KEYWORD_SEARCH_DLG::OnCancel() 
{
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eSearch );
	pEdt->SetWindowText(_T(""));
	
	CDialog::OnCancel();
}

HBRUSH CMNG_KEYWORD_SEARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

BOOL CMNG_KEYWORD_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
