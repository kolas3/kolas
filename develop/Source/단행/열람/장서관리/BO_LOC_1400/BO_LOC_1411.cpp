// BO_LOC_1411.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1411.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1411 dialog


CBO_LOC_1411::CBO_LOC_1411(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1411)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1411::~CBO_LOC_1411()
{

}

BOOL CBO_LOC_1411::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


VOID CBO_LOC_1411::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1411)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1411, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1411)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1411 message handlers

BOOL CBO_LOC_1411::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1411")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	DefaultSearch();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1411::OnbtnFIND() 
{
	SearchSeqInfo();
}

INT CBO_LOC_1411::DefaultSearch()
{
	INT ids;

	CString sYear;

	CString strGetData;
	sYear.Format(_T("%d"), (CLocCommonAPI::GetCurrentDateTime(this, strGetData)).GetYear() );

	GetDlgItem(IDC_edtYEAR)->SetWindowText(sYear);

	ids = SearchSeqInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DefaultSearch") );

	return 0;
}

VOID CBO_LOC_1411::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CBO_LOC_1411::SearchSeqInfo()
{
	INT ids;

	CString sWhere;
	CString sYear;
	GetDlgItem(IDC_edtYEAR)->GetWindowText(sYear);

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_1411") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchSeqInfo") );

	sWhere.Format( _T(" SEQ_YEAR = '%s' ") , sYear );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchSeqInfo") ); 

	CString sCMAlias = _T("CM_BO_LOC_1411");
	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchSeqInfo") ); 

	return 0;
}

HBRUSH CBO_LOC_1411::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
