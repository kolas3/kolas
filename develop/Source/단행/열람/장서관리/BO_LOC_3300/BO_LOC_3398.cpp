// BO_LOC_3398.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3398.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3398 dialog


CBO_LOC_3398::CBO_LOC_3398(CESL_Mgr* pParent , CString sLibKey , CString sLibCode , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3398)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_sLibKey = sLibKey;
	m_sLibCode = sLibCode;
	m_pSpfWork = NULL;
}

CBO_LOC_3398::~CBO_LOC_3398()
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;
}

VOID CBO_LOC_3398::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3398)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3398, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3398)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radGRIDSELECT, OnradGRIDSELECT)
	ON_BN_CLICKED(IDC_radCOND, OnradCOND)
	ON_BN_CLICKED(IDC_btnUPLOAD, OnbtnUPLOAD)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3398 message handlers

BOOL CBO_LOC_3398::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_3398")) < 0)
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pSpfWork = NULL;
	m_pSpfWork = new BL_LOC_USER_PROC( this , m_pLoanShareManager );

	InitControl();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END

	return FALSE;
}


BOOL CBO_LOC_3398::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		// TODO: Add your specialized code here and/or call the base class
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}

VOID CBO_LOC_3398::OnbtnCLOSE() 
{
	EFS_BEGIN 

	OnCancel();	

	EFS_END
}



INT CBO_LOC_3398::InitControl()
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radGRIDSELECT))->SetCheck(1);

	SetCondControl();

	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3398::SetCondControl()
{
	EFS_BEGIN

	bool bEnable;

	if( ((CButton*)GetDlgItem(IDC_radGRIDSELECT))->GetCheck() == 1 )
		bEnable = FALSE;
	else
		bEnable = TRUE;

	const INT nCnt = 3;
	INT nIDC[nCnt] = 
	{
		IDC_chkUSER1 , IDC_chkUSER4 , IDC_chkUSER5	
	};

	for( INT i = 0 ; i < nCnt ; i++ )
		GetDlgItem(nIDC[i])->EnableWindow(bEnable);

	return 0;

	EFS_END

	return -1;
}


VOID CBO_LOC_3398::OnradGRIDSELECT() 
{
	EFS_BEGIN

	SetCondControl();

	EFS_END
}

VOID CBO_LOC_3398::OnradCOND() 
{
	EFS_BEGIN
		
	SetCondControl();
	
	EFS_END
}

VOID CBO_LOC_3398::OnbtnUPLOAD() 
{
	EFS_BEGIN

	CenterUpload();	

	EFS_END
}

INT CBO_LOC_3398::CenterUpload()
{
	EFS_BEGIN

	m_pSpfWork->SetTempData( _T("도서관KEY") , CComVariant(m_sLibKey) );
	m_pSpfWork->SetTempData( _T("도서관부호") , CComVariant(m_sLibCode) );
	
	if( ((CButton*)GetDlgItem(IDC_radGRIDSELECT))->GetCheck() == 1 )
		m_pSpfWork->SetTempData( _T("센터업로드모드") , _T("0") );
	else
		m_pSpfWork->SetTempData( _T("센터업로드모드") , _T("1") );

	INT ids = m_pSpfWork->SPFExcutePath( _T("센터업로드") );

	return 0;

	EFS_END

	return -1;
}


HBRUSH CBO_LOC_3398::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
