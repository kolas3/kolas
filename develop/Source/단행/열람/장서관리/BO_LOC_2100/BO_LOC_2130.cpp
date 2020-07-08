// BO_LOC_2130.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2130.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2130 dialog


CBO_LOC_2130::CBO_LOC_2130(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2130)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSpfWork = NULL;
	m_pSpfWork = new CBLBookCheckProc( this );
}

CBO_LOC_2130::~CBO_LOC_2130()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2130::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2130)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2130, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2130)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2130 message handlers

BOOL CBO_LOC_2130::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2130::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) 
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
	DestroyWindow();

	EFS_END
}

BOOL CBO_LOC_2130::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_2130")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2130::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

// ¼±Á¤
VOID CBO_LOC_2130::OnbtnSELECT() 
{
	EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( OPTION_SELECT );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnSELECT()") );

	EFS_END
}


VOID CBO_LOC_2130::SetInitControl()
{
	EFS_BEGIN

	((CButton*)(GetDlgItem(IDC_radSELECT1)))->SetCheck(1);

	EFS_END
}

VOID CBO_LOC_2130::InitControl()
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSELECT1))->SetCheck(1);

	EFS_END
}

HBRUSH CBO_LOC_2130::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
