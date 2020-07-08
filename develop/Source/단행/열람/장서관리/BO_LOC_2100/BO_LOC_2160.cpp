// BO_LOC_2160.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2160 dialog


CBO_LOC_2160::CBO_LOC_2160(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2160)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_mode = 0;
	// KOL.UDF.022
	m_pSpfWork = NULL;
	m_nLoadSPF = NULL;
}

CBO_LOC_2160::~CBO_LOC_2160()
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
}	

VOID CBO_LOC_2160::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2160)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2160, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2160)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCHANGECHECKNO, OnbtnCHANGECHECKNO)
	ON_MESSAGE( SET_PROGRESS_RANGE, SetProgressRange )
	ON_MESSAGE( SET_PROGRESS_POS, SetProgressPos )
	ON_MESSAGE( SHOW_CURRENT_JOB, ShowCurrentJob )
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2160 message handlers

BOOL CBO_LOC_2160::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_2160")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBLBookCheckProc(this);

	m_pSpfWork->m_nLoadSPF = m_nLoadSPF;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2160::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


INT CBO_LOC_2160::InitGrid()
{
	EFS_BEGIN

	INT ids;
	
	ids = m_pSpfWork->SPFExcutePath( OPEN_BOOK_CHECK_NO_LIST );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGrid()") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2160::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	INT ids = InitGrid();	
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnShowWindow") );


	EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_LOC_2160, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_2160)
	ON_EVENT(CBO_LOC_2160, IDC_gridCHECKNOLIST, -600 /* Click */, OnClickgridCHECKNOLIST, VTS_NONE)
	ON_EVENT(CBO_LOC_2160, IDC_gridCHECKNOLIST, -604 /* KeyUp */, OnKeyUpgridCHECKNOLIST, VTS_PI2 VTS_I2)
	ON_EVENT(CBO_LOC_2160, IDC_gridCHECKNOLIST, -601 /* DblClick */, OnDblClickgridCHECKNOLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_2160::OnClickgridCHECKNOLIST() 
{
	EFS_BEGIN

	SetSelect();		

	EFS_END
}

BOOL CBO_LOC_2160::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	HWND MsgH = pMsg->hwnd;

	if(pMsg->message == WM_KEYUP && ( (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN) )  )
	{
		if(	GetDlgItem(IDC_gridCHECKNOLIST)->m_hWnd==MsgH )
		{
			INT ids = SetSelect();
			if( ids < 0  )
				return FALSE;
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}


INT CBO_LOC_2160::SetSelect( INT n )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_2160");
	CString sGridAlias = _T("gridCHECKNOLIST");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CLoanBookListDlg::SetSelect( INT n )") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CLoanBookListDlg::SetSelect( INT n )") );

	if( pGrid->GetMouseRow() < 1 ) return 0;
	
	if( n == 0 ) 	ids = pGrid->SetReverse(pGrid->GetIdx());
	else ids = pGrid->SetReverse(pGrid->GetIdx());

	if( ids < 0 )ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CLoanBookListDlg::SetSelect( INT n )") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2160::OnKeyUpgridCHECKNOLIST(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	if( *KeyCode == 13 )
	{
		INT ids = m_pSpfWork->SPFExcutePath( CHANGE_CHECK_NO );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnKeyUpgridCHECKNOLIST") );
		if( ids > 0 ) return ;
		OnCancel();
	}

	EFS_END
}


VOID CBO_LOC_2160::OnDblClickgridCHECKNOLIST() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_2160");
	CString sGridAlias = _T("gridCHECKNOLIST");
	
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2(  -1 , _T("OnDblClickgridCHECKNOLIST") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2(  -2 , _T("OnDblClickgridCHECKNOLIST") );
	
	if( pGrid->GetMouseRow() < 1 ) return ;
	

	INT ids = m_pSpfWork->SPFExcutePath( CHANGE_CHECK_NO );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnDblClickgridCHECKNOLIST") );
	if( ids > 0 ) return ;

	OnCancel();		

	EFS_END
}

BOOL CBO_LOC_2160::Create(CWnd* pParentWnd) 
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2160::OnbtnCHANGECHECKNO() 
{
	EFS_BEGIN

	INT ids = m_pSpfWork->SPFExcutePath( CHANGE_CHECK_NO );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgridCHECKNOLIST") );
	if( ids > 0 ) return ;

	OnCancel();

	EFS_END
}

INT CBO_LOC_2160::SetProgressRange( WPARAM wParam, LPARAM lParam )
{
	m_progress.SetRange( 0, ( INT ) wParam );
	m_progress.ShowPercent( true );
	m_progress.SetStartColor( 8355711 );
	m_progress.SetEndColor( 2631720 );
	m_progress.SetTextColor( 255 );
	m_progress.SetBkColor( 16777215 );
	
	return 0;
}

INT CBO_LOC_2160::SetProgressPos(WPARAM wParam, LPARAM lParam )
{
	m_progress.SetPos( ( INT )wParam );
	
	return 0;
}

VOID CBO_LOC_2160::ShowCurrentJob( WPARAM wParam, LPARAM lParam )
{
	CStatic *pStatic = NULL;
	if( 2 == ( INT )wParam )
		pStatic = ( CStatic * )GetDlgItem( IDC_sCURRENT_STATUS_MAIN );
	else
		pStatic = ( CStatic * )GetDlgItem( IDC_sCURRENT_STATUS );
	
	CString *jobName = ( CString * )lParam;
	pStatic->SetWindowText( jobName->GetBuffer( 0 ) );
}

HBRUSH CBO_LOC_2160::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
