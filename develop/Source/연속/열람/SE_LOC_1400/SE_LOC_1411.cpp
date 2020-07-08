// SE_LOC_1411.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1411.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1411 dialog


CSE_LOC_1411::CSE_LOC_1411(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1411)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1411::~CSE_LOC_1411()
{
}

BOOL CSE_LOC_1411::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_LOC_1411::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1411)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1411, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1411)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1411 message handlers

BOOL CSE_LOC_1411::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1411")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	DefaultSearch();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1411::OnbtnFIND() 
{
	SearchSeqInfo();
}


// 현재 년도의 차수들을 검색한다.
INT CSE_LOC_1411::DefaultSearch()
{
	INT ids;

	// 1. 현재 년도 입력하기
	CString sYear;
	sYear.Format(_T("%d") , CLocCommonAPI::GetDBTime(this).GetYear());

	GetDlgItem(IDC_edtYEAR)->SetWindowText(sYear);

	// 2. 자료 가져오기
	ids = SearchSeqInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DefaultSearch") );

	return 0;
}

VOID CSE_LOC_1411::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CSE_LOC_1411::SearchSeqInfo()
{
	INT ids;

	CString sWhere;
	CString sYear;
	GetDlgItem(IDC_edtYEAR)->GetWindowText(sYear);

	CESL_DataMgr *pDM = FindDM( _T("DM_SE_LOC_1411") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchSeqInfo") );

	sWhere.Format( _T(" SEQ_YEAR = '%s' ") , sYear );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchSeqInfo") ); 

	CString sCMAlias = _T("CM_SE_LOC_1411");
	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchSeqInfo") ); 

	return 0;
}
HBRUSH CSE_LOC_1411::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
