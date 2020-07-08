// MNG_KEYWORD_MAIN_POPUP.cpp : implementation file
//

#include "stdafx.h"
//#include _T("mng_keyword_manager.h")
#include "MNG_KEYWORD_MAIN_POPUP.h"
#include "MNG_KEYWORD_DETAIL_DLG.h"
#include "MNG_KEYWORD.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAIN_POPUP dialog


CMNG_KEYWORD_MAIN_POPUP::CMNG_KEYWORD_MAIN_POPUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_MAIN_POPUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KEYWORD_MAIN_POPUP::~CMNG_KEYWORD_MAIN_POPUP()
{
}

VOID CMNG_KEYWORD_MAIN_POPUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_MAIN_POPUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KEYWORD_MAIN_POPUP, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_MAIN_POPUP)
	ON_BN_CLICKED(IDbSearch, OnbSearch)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_bDetail_popup, OnbDetailpopup)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAIN_POPUP message handlers


BOOL CMNG_KEYWORD_MAIN_POPUP::OnInitDialog() 
{
EFS_BEGIN
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_주제어검색요어_메인_POPUP")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
EFS_END
	return FALSE;
}

VOID CMNG_KEYWORD_MAIN_POPUP::OnbSearch() 
{
EFS_BEGIN

	CString keyword=_T("");
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_주제어검색요어_메인_POPUP"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return ;
	}
	
	pCM->GetControlMgrData(_T("검색어"), keyword);
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("검색어를 입력하여 주십시오") );
		return;
	}
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_주제어검색요어_메인_POPUP"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return ;
	}
	
	CMNG_KEYWORD *pmng = new CMNG_KEYWORD();
	INT ids = pmng->SearchKeyword(pDM, keyword);
	if( ids<0 )
		return ;

	pCM->AllControlDisplay();

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		return;
	}

EFS_END	
}

VOID CMNG_KEYWORD_MAIN_POPUP::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CMNG_KEYWORD_MAIN_POPUP::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if ( pMsg->message == WM_KEYDOWN ) {
	
		if ( pMsg->wParam == VK_RETURN ) {
			OnbSearch();
			return TRUE;
		} 
	}

	return CDialog::PreTranslateMessage(pMsg);
EFS_END
	return FALSE;
}

BEGIN_EVENTSINK_MAP(CMNG_KEYWORD_MAIN_POPUP, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_KEYWORD_MAIN_POPUP)
	ON_EVENT(CMNG_KEYWORD_MAIN_POPUP, IDC_gMAIN_popup, -600 /* Click */, OnClickgMAINpopup, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_KEYWORD_MAIN_POPUP::OnClickgMAINpopup() 
{
	INT iClickGridRow = SetCheckGrid();	
	
}

INT CMNG_KEYWORD_MAIN_POPUP::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_popup );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	if( 1 != col ) return 0;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		
		if( row == i )
		{
			pGrid->SetText( _T("V") );
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}
	
	pGrid->SetRow( row );	
	return row;
}

VOID CMNG_KEYWORD_MAIN_POPUP::OnbDetailpopup() 
{
EFS_BEGIN
	
	CMNG_KEYWORD_DETAIL_DLG *pDlg=NULL;
	pDlg = new CMNG_KEYWORD_DETAIL_DLG( this );

	pDlg->SetMainType(_T("POPUP"));
	pDlg->SetCMName_Main(_T("CM_관리_주제어검색요어_메인_POPUP"));
	pDlg->SetDMName_Main(_T("DM_관리_주제어검색요어_메인_POPUP"));
	
	pDlg->SetMarcMgr( m_pMarcMgr, m_pMarc );
	
	if ( IDOK == pDlg->DoModal() )	CDialog::OnOK();

EFS_END
}

/*CMNG_KEYWORD_MAIN_POPUP::~CMNG_KEYWORD_MAIN_POPUP()
{
	if ( m_pMarcMgr ) m_pMarcMgr=NULL;
	if ( m_pMarc ) m_pMarc=NULL;

}*/

HBRUSH CMNG_KEYWORD_MAIN_POPUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
