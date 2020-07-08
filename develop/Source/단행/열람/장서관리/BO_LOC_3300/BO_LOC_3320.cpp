// BO_LOC_3320.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3320.h"

#include "..\\BL_LOC_USER\\BL_LOC_USER_PROC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3320 dialog


CBO_LOC_3320::CBO_LOC_3320(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3320)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_nMode = 0;
	m_nCurrentIndex = 0;
	m_nFirstIndex = 0;
	m_nLastIndex = 0;
}


VOID CBO_LOC_3320::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3320)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3320, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3320)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnDUPLICATE3, OnbtnDUPLICATE3)
	ON_BN_CLICKED(IDC_btnDUPLICATE1, OnbtnDUPLICATE1)
	ON_BN_CLICKED(IDC_btnDUPLICATE2, OnbtnDUPLICATE2)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3320 message handlers

BOOL CBO_LOC_3320::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3320::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CBO_LOC_3320::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3320")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3320::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	SetScreen();
}


VOID CBO_LOC_3320::OnbtnCLOSE() 
{
	DestroyWindow();	
}

VOID CBO_LOC_3320::SetMode( INT nMode )
{
	m_nMode = nMode;
}

VOID CBO_LOC_3320::SetScreen()
{

	if( m_nMode == 0 )
	{
		const INT nUnVisibleCnt = 17;
		INT nUnVisibleGroup[ nUnVisibleCnt ] =
		{
			IDC_STATIC_UNVISIBLE_1 , IDC_STATIC_UNVISIBLE_3 , IDC_cmbUSER7 , IDC_STATIC_UNVISIBLE_4 ,
			IDC_datUSER1 , IDC_STATIC_UNVISIBLE_5 , IDC_datUSER2 , IDC_STATIC__UNVISIBLE_6 ,
			IDC_datUSER3 , IDC_STATIC_UNVISIBLE_7 , IDC_datUSER4 , IDC_STATIC_UNVISIBLE_2 ,
			IDC_edtUSER22 , IDC_btnOLDDATA , IDC_btnPREV , IDC_btnNEXT , 
			IDC_btnPRINT
		};
		const INT nMoveCtrlCnt = 8;
		INT nMoveCtrlGroup[ nMoveCtrlCnt ] =
		{
			IDC_STATIC_BUTTON_SEP , IDC_btnCLEAR , IDC_btnOLDDATA , IDC_btnPREV ,
			IDC_btnNEXT , IDC_btnPRINT ,IDC_btnSAVE , IDC_btnCLOSE
		};
		INT nMovedCtrlPosGroup[ nMoveCtrlCnt ] = 
		{
			IDC_STATIC_MOVE_1 , IDC_btnCLEAR2 , IDC_btnOLDDATA2 , IDC_btnPREV2 ,
			IDC_btnNEXT2 , IDC_btnPRINT2 , IDC_btnSAVE2 , IDC_btnCLOSE2
		};

		CRect TempRect;

		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
		{
			GetDlgItem( nUnVisibleGroup[i] )->ShowWindow( FALSE );
		}
		for( i = 0 ; i < nMoveCtrlCnt ;i++ )
		{
			GetDlgItem( nMovedCtrlPosGroup[i] )->GetWindowRect( TempRect );
			ScreenToClient( &TempRect );
			GetDlgItem( nMoveCtrlGroup[i] )->MoveWindow( TempRect );
		}
		CRect WndPos;
		// 현재 윈도우의 위치를 얻어옴
		GetWindowRect( &WndPos );
		
		// 변경될 위치를 얻어옴
		GetDlgItem( IDC_INPUTMODE_SIZE )->GetWindowRect( &TempRect );
		
		ClientToScreen(&WndPos);
		ClientToScreen(&TempRect);

		// 변경될 위치로 변경함
		WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , TempRect.bottom );
		
		ScreenToClient(&WndPos);

		MoveWindow( WndPos );	

	}
	else if( m_nMode == 1)
	{
		const INT nUnVisibleCnt =1;
		INT nUnVisibleGroup[ nUnVisibleCnt ] =
		{
			IDC_btnPRINT 
		};
		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
		{
			GetDlgItem( nUnVisibleGroup[i] )->ShowWindow( FALSE );
		}
		
		SetCurrentIndex();
		DisplayWindow();
	}
	else if( m_nMode == 2 )
	{
		const INT nUnVisibleCnt = 11;
		INT nUnVisibleGroup[ nUnVisibleCnt ] =
		{
			IDC_btnDUPLICATE1 , IDC_btnDUPLICATE2 , IDC_edtUSER5 , IDC_edtUSER6 ,
			IDC_btnDUPLICATE3 , IDC_btnFIND1 , IDC_btnFIND2 , IDC_btnCLEAR , 
			IDC_btnOLDDATA , IDC_btnSAVE , IDC_btnCANCEL
		};
		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
		{
			GetDlgItem( nUnVisibleGroup[i] )->ShowWindow( FALSE );
		}
		SetCurrentIndex();
		DisplayWindow();
		
	}

}

VOID CBO_LOC_3320::OnbtnSAVE() 
{
	INT ids;
	
	BL_LOC_USER_PROC SpfWork( this );

	if( m_nMode == 0 )
	{
		ids = SpfWork.SPFExcutePath( _T("이용자관리_입력") );
	}
	else if( m_nMode == 1 )
	{
		ids = SpfWork.SPFExcutePath( _T("이용자관리_수정") );
	}	
}

VOID CBO_LOC_3320::OnbtnDUPLICATE3() 
{
	INT ids;

	BL_LOC_USER_PROC SpfWork( this );

	ids = SpfWork.SPFExcutePath( _T("가족ID찾기") );
	
}

VOID CBO_LOC_3320::OnbtnDUPLICATE1() 
{
	INT ids;
	bool IsDuplicate = false;
	ids = IsDuplicateUserNo( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 번호입니다. 다른 번호를 입력하십시오") );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 번호가 존재하지 않습니다. ") );
	}
}

INT CBO_LOC_3320::IsDuplicateUserNo( bool &IsDuplicate )
{
	INT ids;
	CString sSql;

	CString sGetData;
	ids = GetControlData( _T("CM_BO_LOC_3320") , _T("대출자번호") , sGetData );
	if( ids < 0 ) return -1;	

	if( sGetData.IsEmpty() )
	{
		AfxMessageBox( _T("대출자 번호를 입력하시고 중복 조회 기능을 사용하십시오") );
		IsDuplicate = FALSE;
		return 2;
	}
	
	sSql.Format( _T("SELECT count(*) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'") , sGetData );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3300") );
	if( pDM == NULL ) return -1;

	CString sResult;
//	ids = pDM->GetOneValueQuery( sSql , sResult );
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );
	if( ids < 0 ) return -1;

	if( sResult.Compare(_T("0")) != 0 ) IsDuplicate = true;

	return 0;	
}


VOID CBO_LOC_3320::OnbtnDUPLICATE2() 
{
	INT ids;
	bool IsDuplicate = false;
	ids = IsDuplicateUserID( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 ID입니다. 다른 번호를 입력하십시오") );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 ID가 존재하지 않습니다. ") );
	}	
}


INT CBO_LOC_3320::IsDuplicateUserID( bool &IsDuplicate )
{
	INT ids;
	CString sSql;

	CString sGetData;
	ids = GetControlData( _T("CM_BO_LOC_3320") , _T("사용자ID") , sGetData );
	if( ids < 0 ) return -1;	

	if( sGetData.IsEmpty() )
	{
		AfxMessageBox( _T("대출자 ID를 입력하시고 중복 조회 기능을 사용하십시오") );
		IsDuplicate = FALSE;
		return 2;
	}
	
	sSql.Format( _T("SELECT count(*) FROM CO_LOAN_USER_TBL WHERE USER_ID = '%s'") , sGetData );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3300") );
	if( pDM == NULL ) return -1;

	CString sResult;
//	ids = pDM->GetOneValueQuery( sSql , sResult );
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );
	if( ids < 0 ) return -1;

	if( sResult.Compare(_T("0")) != 0 ) IsDuplicate = true;

	return 0;	
}


INT CBO_LOC_3320::DisplayWindow()
{
	CString sParentCMAlias = _T("CM_BO_LOC_3300");
	CString sParentDMAlias = _T("DM_BO_LOC_3300");
	CString sCMAlias = _T("CM_BO_LOC_3320" );

	INT ids;

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return -1;
	
	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) return -1;

	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) return -1;

	CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
	if( pChildCM == NULL ) return -1;

	ids = pChildCM->AllControlDisplay( m_nCurrentIndex );
	if( ids < 0 ) return -1;
	SetPreNextButton();

	return 0;
}

INT CBO_LOC_3320::SetCurrentIndex()
{
	CString sParentCMAlias = _T("CM_BO_LOC_3300");

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return -1;

	m_nCurrentIndex = pGrid->GetIdx();
	m_nFirstIndex = 0;
	m_nLastIndex = pGrid->GetCount();
	
	return 0;
}

VOID CBO_LOC_3320::SetPreNextButton()
{
	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex-1 )
	{
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
	}
}

VOID CBO_LOC_3320::OnbtnPREV() 
{
	m_nCurrentIndex--;	
	DisplayWindow();
}


VOID CBO_LOC_3320::OnbtnNEXT() 
{
	m_nCurrentIndex++;	
	DisplayWindow();
	
}
