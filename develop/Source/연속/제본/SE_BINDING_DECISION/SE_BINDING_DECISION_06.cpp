// SE_BINDING_DECISION_06.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_06.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_06 dialog

CSE_BINDING_DECISION_06::CSE_BINDING_DECISION_06(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
}

CSE_BINDING_DECISION_06::~CSE_BINDING_DECISION_06()
{
}


VOID CSE_BINDING_DECISION_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_06, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_06)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_06 message handlers

BOOL CSE_BINDING_DECISION_06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_06")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	INT ids = InitDisplay();
	if( ids == -100 ) { 
		AfxMessageBox( _T("발주자료가 존재하지 않습니다.") );
		CDialog::OnOK();
	}
    else if( ids <     0 ) {
		AfxMessageBox (_T("InitDisplay() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_DECISION_06::SetSpeciesKey( TCHAR* f_SpeciesKey , CString f_sTitle )
{
	m_pSpeciesKey = f_SpeciesKey;
	m_sTitle = f_sTitle;
}

INT CSE_BINDING_DECISION_06::InitDisplay()
{
	INT ids, nRowCnt;
	CString strQuery;

	GetDlgItem( IDC_eTITLE ) -> SetWindowText( m_sTitle );

	strQuery.Format( _T(" SPECIES_KEY = %s AND BINDED_BOOK_YN = 'Y' ")
					 _T(" AND WORKING_STATUS = 'SEB113N' ")
					,m_pSpeciesKey );
	
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_06"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( strQuery );
	if( ids ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -2;
	}

	nRowCnt = pDataMgr->GetRowCount();
	if( nRowCnt <  0 ) return -4;
	if( nRowCnt == 0 ) return -100;

	ids = AllControlDisplay( _T("CM_SE_BINDING_DECISION_06"), -1 );
	if( ids ) return -5;

	return 0;
}

HBRUSH CSE_BINDING_DECISION_06::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
