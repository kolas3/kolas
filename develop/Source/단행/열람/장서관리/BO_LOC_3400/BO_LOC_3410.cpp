// BO_LOC_3410.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3410 dialog


CBO_LOC_3410::CBO_LOC_3410(CESL_Mgr* pParent /*=NULL*/ , CLonaManageValue *pLoanMangeValue)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3410)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	
	m_pLoanMangeValue = pLoanMangeValue;

	m_pSpfWork = NULL;
}

CBO_LOC_3410::~CBO_LOC_3410()
{

}

VOID CBO_LOC_3410::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3410)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3410, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3410)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDFIND, OnbtnDFIND)
	ON_BN_CLICKED(IDC_btn3400_DCLOSE, OnbtnDCLOSE)
	ON_CBN_SELCHANGE(IDC_cmbDSTATUS, OnSelchangecmbDSTATUS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3410 message handlers

BOOL CBO_LOC_3410::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3410::OnClose() 
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
	
	CDialog::OnClose();
}

BOOL CBO_LOC_3410::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3410")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	AddColorControl(IDC_datDFIND1, _T("CM_BO_LOC_3410"), _T("일자1"), 'E' );

	m_pSpfWork = new CBL_BO_LOC_34(this,m_pLoanMangeValue);

	CTime tFrom = CTime::GetCurrentTime();
	CTime tTo = tFrom - CTimeSpan(7,0,0,0);

	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDFIND1 );
	pDateTo->SetTime( &tTo );

	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDFIND2 );
	pDateFrom->SetTime( &tFrom );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3410::OnbtnDFIND() 
{
	if( !m_pSpfWork )
		m_pSpfWork = new CBL_BO_LOC_34(this);
	
	m_pSpfWork->SPFExcutePath( SEARCH );
}

VOID CBO_LOC_3410::OnbtnDCLOSE() 
{
	OnCancel();	
}

VOID CBO_LOC_3410::OnSelchangecmbDSTATUS() 
{
	ChangeDateName();
}

INT CBO_LOC_3410::ChangeDateName()
{
	INT ids;
	CString sStatus;

	CString sCMAlias = _T("CM_BO_LOC_3410");
	ids = GetControlData( sCMAlias , _T("상태") , sStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeDateName") );

	CString sControlName = _T("일    자");
	if( sStatus.Compare( _T("신청자료") ) == 0 )
		sControlName = _T("신청일자");
	else if( sStatus.Compare( _T("접수완료") ) == 0 )
		sControlName = _T("접수일자");
	else if( sStatus.Compare( _T("배달완료") ) == 0 )
		sControlName = _T("배달일자");
	
	GetDlgItem(IDC_staDDATENAME)->SetWindowText( sControlName );

	return 0;
}

HBRUSH CBO_LOC_3410::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
