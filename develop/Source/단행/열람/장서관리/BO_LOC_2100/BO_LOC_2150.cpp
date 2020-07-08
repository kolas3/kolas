// BO_LOC_2150.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2150.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2150 dialog


CBO_LOC_2150::CBO_LOC_2150(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2150)
	m_pSpfWork = NULL;
	//}}AFX_DATA_INIT
}

CBO_LOC_2150::~CBO_LOC_2150()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2150::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2150)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2150, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2150)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2150 message handlers

VOID CBO_LOC_2150::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	DestroyWindow();


	EFS_END
}

BOOL CBO_LOC_2150::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_2150")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	InitControl();

	m_pSpfWork = new CBLBookCheckProc(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_2150::Create(CWnd* pParentWnd) 
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2150::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	INT ids;
	ids = SettingShelfLoc();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnShowWindow(BOOL bShow, UINT nStatus) ") );


	EFS_END
}

INT CBO_LOC_2150::SettingShelfLoc()
{
	EFS_BEGIN

	INT ids;

	CString sParentCMAlias = _T("CM_BO_LOC_2100_ETC_INFO");
	CString sParentControlAlias = _T("자료실명");

	CString sCMAlias = _T("CM_BO_LOC_2150");
	CString sControlAlias = _T("자료실명");


	CString sShelfLocCode;

	ids = GetControlData( sParentCMAlias , sParentControlAlias , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingShelfLoc()") );

	ids = SetControlData( sCMAlias , sControlAlias , sShelfLocCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SettingShelfLoc()") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2150::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

VOID CBO_LOC_2150::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids = m_pSpfWork->SPFExcutePath( INITIAL_BOOK_CHECK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFIND()") );

	AfxMessageBox( _T("완료되었습니다.") );

	EFS_END
}

INT CBO_LOC_2150::InitControl()
{
	EFS_BEGIN

	const INT nCnt = 7;
	INT nIDC[nCnt] = 
	{
		IDC_chkSELECT1 , IDC_chkSELECT3 , IDC_chkSELECT5 , IDC_chkSELECT7 , IDC_chkSELECT2 ,
		IDC_chkSELECT4 , IDC_chkSELECT6
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		((CButton*)GetDlgItem( nIDC[i] ))->SetCheck(1);
	}

	
	((CButton*)GetDlgItem( IDC_chkSELECT3 ))->SetCheck(0);

	return 0;

	EFS_END
	return -1;

}

HBRUSH CBO_LOC_2150::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
