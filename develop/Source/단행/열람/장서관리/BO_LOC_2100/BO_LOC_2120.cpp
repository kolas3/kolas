// BO_LOC_2120.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2120.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2120 dialog


CBO_LOC_2120::CBO_LOC_2120(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2120)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSpfWork = NULL;
	m_pSpfWork = new CBLBookCheckProc( this );
}

CBO_LOC_2120::~CBO_LOC_2120()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2120::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2120)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2120, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2120)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radFIND1, OnradFIND1)
	ON_BN_CLICKED(IDC_radFIND2, OnradFIND2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2120 message handlers

BOOL CBO_LOC_2120::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2120::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	if( m_pSpfWork != NULL ) 
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
	DestroyWindow();

	EFS_END
}

BOOL CBO_LOC_2120::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_2120")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	INT ids = SetInitControl();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog()") );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2120::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pSpfWork->SPFExcutePath(BOOK_SEARCH);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFIND()") );

	EFS_END
}

VOID CBO_LOC_2120::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

INT CBO_LOC_2120::SetInitControl()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 2;
	INT nCheckGroup[2] =
	{
		IDC_radFIND1 , IDC_chkFIND1
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		((CButton*)GetDlgItem(nCheckGroup[i]))->SetCheck(1);
	}

	ids = SetUnvisibleGroup();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetInitControl()") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_2120::SetUnvisibleGroup()
{
	EFS_BEGIN

	INT nMode = 0;

	if( ((CButton*)GetDlgItem(IDC_radFIND1))->GetCheck() != 1 )
		nMode = 1;

	const INT nCnt = 7;
	INT nIDCGroup[nCnt] =
	{
		IDC_cmbFIND2 , IDC_edtFIND1 , IDC_edtFIND2 , IDC_cmbFIND3 , IDC_cmbFIND4 , 
		IDC_edtFIND3 , IDC_edtFIND4 
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem( nIDCGroup[i] )->EnableWindow(TRUE);
	}

	INT nFirst=0;
	INT nLast=0;
	if( nMode == 0 )
	{
		nFirst = 3;
		nLast = nCnt;
	}
	else if( nMode == 1)
	{
		nFirst = 0;
		nLast = 3;
	}

	for( INT j = nFirst ; j < nLast ; j++ )
	{
		GetDlgItem( nIDCGroup[j] )->EnableWindow(FALSE);
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2120::OnradFIND1() 
{
	EFS_BEGIN

	INT ids;
	ids = SetUnvisibleGroup();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnradFIND1()") );

	EFS_END
}

VOID CBO_LOC_2120::OnradFIND2() 
{
	EFS_BEGIN

	INT ids;
	ids = SetUnvisibleGroup();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnradFIND2()") );

	EFS_END
}

HBRUSH CBO_LOC_2120::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
