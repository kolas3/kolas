// BO_LOC_2180.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2180.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2180 dialog


CBO_LOC_2180::CBO_LOC_2180(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2180)
	m_pSpfWork = NULL;
	//}}AFX_DATA_INIT
}

CBO_LOC_2180::~CBO_LOC_2180()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2180::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2180)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2180, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2180)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2180 message handlers


BOOL CBO_LOC_2180::Create(CWnd* pParentWnd) 
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_2180::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_2180")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBLBookCheckProc(this);
	
	SetInitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2180::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	DestroyWindow();

	EFS_END
}

VOID CBO_LOC_2180::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_2180::OnbtnCHANGE() 
{
	EFS_BEGIN

	INT ids = m_pSpfWork->SPFExcutePath( CHANGE_BOOK_CHECK_YN );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCHANGE") );

	EFS_END
}


VOID CBO_LOC_2180::SetInitControl()
{
	EFS_BEGIN

	((CButton*)(GetDlgItem(IDC_radSELECT1)))->SetCheck(1);

	EFS_END
}

HBRUSH CBO_LOC_2180::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
