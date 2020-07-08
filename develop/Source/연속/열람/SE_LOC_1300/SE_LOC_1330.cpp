// SE_LOC_1330.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1330.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1330 dialog


CSE_LOC_1330::CSE_LOC_1330(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1330)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1330::~CSE_LOC_1330()
{
}

VOID CSE_LOC_1330::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1330)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1330, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1330)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_BN_CLICKED(IDC_radALL1, OnradALL1)
	ON_BN_CLICKED(IDC_radALL2, OnradALL2)
	ON_CBN_SELCHANGE(IDC_cmbCHANGE2, OnSelchangecmbCHANGE2)
	ON_CBN_SELCHANGE(IDC_cmbCHANGE1, OnSelchangecmbCHANGE1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1330 message handlers

BOOL CSE_LOC_1330::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1330::OnClose() 
{	
	EFS_BEGIN

	CDialog::OnClose();

	EFS_END
}

BOOL CSE_LOC_1330::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1330")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pReasonList = new CShelfChangeReasonList(this);
	
	m_ViewerControl.InitViewer( IDC_gridBOOK_INFO , this );
	m_ViewerControl.SetViewerWidth( 25 , 70 );

	SetShelfChangeReasonControl();

	InitMovePos();

	ViewModeChange(1);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1330::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

INT CSE_LOC_1330::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	CString sParentCMAlias = _T("CM_SE_LOC_1300");

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return -1;

	ids = AllControlDisplay( _T("CM_SE_LOC_1330") , m_nCurrentIndex );
	if( ids < 0 ) return -1;

	// 책 정보 표시
	ids = DisplayBookStatus();
	if( ids < 0 ) return -1;

	// 배가변경 코드도 변경
	ids = ChangeShelfReasonControl();
	if( ids < 0 ) return -1;

	// 배가 지시자 설정
	InitShelfWorker();


	GetDlgItem(IDC_btnPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnNEXT)->EnableWindow( FALSE );
	
	pGrid->SetReverse( m_nCurrentIndex );

	SetChangeButtonEnable();

	return 0;


	EFS_END
	return -1;

}


VOID CSE_LOC_1330::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	bFirstView = TRUE;

	InitSetStartPos();
	INT ids = ViewCurrentWindow();
	if( ids < 0 ) return;
	InitControl();
	InitSelectCheck();

	bFirstView = FALSE;

	EFS_END
}

VOID CSE_LOC_1330::OnbtnPREV() 
{
	EFS_BEGIN

	INT ids = MoveIndex( 0 );
	if( ids < 0 ) return;



	EFS_END
}

VOID CSE_LOC_1330::OnbtnNEXT() 
{
	EFS_BEGIN

	INT ids = MoveIndex( 1 );
	if( ids < 0 ) return;

	EFS_END
}

INT CSE_LOC_1330::MoveIndex( INT nMode )
{
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1330::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

VOID CSE_LOC_1330::InitControl()
{
	EFS_BEGIN

	((CButton*)GetDlgItem( IDC_radALL1 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radALL2 ))->SetCheck(1);

	EFS_END
}

VOID CSE_LOC_1330::OnbtnCHANGE() 
{
	EFS_BEGIN

	INT ids;
	INT nCheckFlag;
	nCheckFlag = ((CButton*)GetDlgItem( IDC_radALL1 ))->GetCheck();

	CBL_SE_LOC_13 SpfWork( this );

	if( nCheckFlag == 1 )
	{
		CString sParentCMAlias = _T("CM_SE_LOC_1300");

		CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
		if( pCM == NULL ) return ;

		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
		if( pGrid == NULL ) return ;

		if( pGrid->SelectGetCount() <= 0 ) 
		{
			AfxMessageBox( _T("먼저 변경할 책을 선정해주십시오!") );
			return;
		}

		ids = SpfWork.SPFExcutePath( _T("배가자료관리_일괄변경") );		
	}
	else
	{
		ids = SpfWork.SPFExcutePath( _T("배가자료관리_낱권변경") );
		if( ids == 0 ) OnbtnNEXT();
	}
	if( ids < 0 ) return;


	EFS_END
}

VOID CSE_LOC_1330::OnradALL1() 
{
	EFS_BEGIN


	INT ids;


	CString sMainDMAlias = _T("DM_SE_LOC_1300");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1300"),_T("MainGrid"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("BS_종KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();

	sBookCnt.Format(_T("%d") , pMainGrid->SelectGetCount() );

	CString sCMAlias = _T("CM_SE_LOC_1330");
	ids = SetControlData( sCMAlias , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sCMAlias , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -5 , _T("DisplayDefaultValue") ); 


	ViewModeChange(0);

	EFS_END
}

VOID CSE_LOC_1330::OnradALL2() 
{
	EFS_BEGIN

	ViewModeChange(1);
	return;

	EFS_END
}

VOID CSE_LOC_1330::InitSelectCheck()
{
	EFS_BEGIN

	INT ids;
	CString sParentCMAlias = _T("CM_SE_LOC_1300");


	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return ;

	for( INT i = 0 ; i < pGrid->GetCount() ; i++)
	{
		SetDataMgrData( _T("DM_SE_LOC_1300") , _T("선정") , _T("") , i ); 
	}
	
	
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) return;

	INT nIndex;
	pGrid->SelectGetHeadPosition();
	for( i = 0 ; i < pGrid->SelectGetCount() ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		SetDataMgrData( _T("DM_SE_LOC_1300") , _T("선정") , _T("V") , nIndex ); 
		pGrid->SelectGetNext();
	}


	EFS_END
}

INT CSE_LOC_1330::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	// 이전 다음이
	// 'V'체크가 된 것이 있다면 'V'체크 된것만 보여주며
	// 'V'체크가 된 것이 없다면 모두 보여준다.
	
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = m_nCurrentIndex;

	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	return 0;

	EFS_END
	return -1;

}

CESL_Grid* CSE_LOC_1330::GetMainGridPtr()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_SE_LOC_1300");

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

// 배가 변경 사유 콘트롤 보여주는 함수
VOID CSE_LOC_1330::SetShelfChangeReasonControl()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1330");
	CString sControlAlias = _T("변경사유코드_입력");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CSE_LOC_1330::SetShelfChangeResonControl()") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CSE_LOC_1330::SetShelfChangeResonControl()") );

	CString *sGetData;
	sGetData = NULL;

	INT nRowCount;

	INT nReasonCnt = GetShelfChangeReasonCount();

	sGetData = new CString[nReasonCnt];

	ids = GetShelfChangeReasonList( sGetData , nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("CSE_LOC_1330::SetShelfChangeResonControl()") );

	for( INT i = 0 ; i < nRowCount ; i++ )
	{	
		pComboBox->AddString( sGetData[i] );
	}
	
	pComboBox->SetCurSel(0);

	if( sGetData != NULL ) delete []sGetData;


	EFS_END
}


INT CSE_LOC_1330::GetShelfChangeReasonList( CString sGetData[] , INT &nRowCount )
{
	EFS_BEGIN

	m_pReasonList->GetList( sGetData , nRowCount );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1330::GetShelfChangeReasonCount()
{
	EFS_BEGIN

	return m_pReasonList->GetListCount();

	EFS_END
	return -1;

}

INT CSE_LOC_1330::GetReasonCode( CString &sCode )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1330");
	CString sControlAlias = _T("변경사유코드_입력");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1330::GetReasonCode( CString &sCode )") );

	ids = pCM->GetControlMgrData( sControlAlias , sCode , -1 , 0 );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1330::GetReasonCode( CString &sCode )") );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1330::SetReasonDesc( CString sReasonData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1330");
	CString sControlAlias = _T("변경사유내용_입력");

	ids = SetControlData( sCMAlias , sControlAlias , sReasonData );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1330::SetReasonDesc( CString sReasonData )") );
	
	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1330::ChangeShelfReasonControl()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1300");
	CString sFieldAlias = _T("BB_배가변경사유");
	CString sCMAlias = _T("CM_SE_LOC_1330");
	CString sControlAlias = _T("변경사유코드_입력");

	CString sCode;
	ids = GetDataMgrData( sDMAlias , sFieldAlias , sCode , m_nCurrentIndex );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1330::ChangeShelfReasonControl()") );

	CComboBox *pComboBox = (CComboBox*)(GetDlgItem( IDC_cmbCHANGE2 ));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1330::ChangeShelfReasonControl()") );

	INT nComboCnt = pComboBox->GetCount();

	CString  sGetCode;
	for( INT i = 0 ; i < nComboCnt ; i++ )
	{
		pComboBox->GetLBText( i , sGetCode );
		if( sCode.Compare( sGetCode ) == 0 )
		{
			pComboBox->SetCurSel( i );
			OnSelchangecmbCHANGE2();
			return 0;
		}

	}

	pComboBox->SetCurSel(0);

	OnSelchangecmbCHANGE2();

	return 0;

	EFS_END
	return -1;

}

// 0 : 일괄 변경시 , 1 : 낱권 변경시 
// DEFAULT = 0 이라고 보고 변수명 적용
INT CSE_LOC_1330::ViewModeChange( INT nMode )
{
	EFS_BEGIN

	const INT nVisibleCnt = 5;
	const INT nUnVisibleCnt = 27;
	const INT nMoveCnt = 16;

	INT nVisibleIDC[nVisibleCnt] = 
	{
		IDC_staCHANGEGROUP2 , IDC_staWILLCHANGESHELFDATE2 , IDC_datCHANGE4 , IDC_datCHANGE5 , IDC_staWILLCHANGESHELFDATE3
	};
	INT nUnVisibleIDC[nUnVisibleCnt] = 
	{ 
		IDC_staBOOKSTATE , IDC_staTITLE , IDC_staAUTHOR , IDC_staVOL , IDC_staVOLTITLE  , 
		IDC_staPUBLISHYEAR , IDC_staSHELFCODE , IDC_staSHELFCODE , IDC_staREGNO , IDC_staSHELFLOC , 
		IDC_staCHANGEREASON , IDC_staSHELFDATE , IDC_staSHELFCHANGEDATE , IDC_edtCHANGE1 , IDC_edtCHANGE2 , 
		IDC_edtCHANGE3 , IDC_edtCHANGE4 , IDC_edtCHANGE5 , IDC_edtCHANGE6 , IDC_edtCHANGE7 , 
		IDC_cmbSHELF , IDC_edtCHANGE9 , IDC_datCHANGE1 , IDC_datCHANGE2 , IDC_btnPREV , 
		IDC_btnNEXT , IDC_gridBOOK_INFO
	};
	INT nMoveIDC[nMoveCnt] = 
	{
		IDC_staCHANGEGROUP , IDC_cmbCHANGE1 , IDC_datCHANGE3 , IDC_edtCHANGE10 , IDC_cmbCHANGE2 , 
		IDC_edtCHANGE11 , IDC_edtCHANGE11 , IDC_staWILLCHANGESHELFDATE , IDC_staWILLCHANGEREASONCODE , IDC_staWORKER , 
		IDC_staBUTTONLINE , IDC_btnPREV , IDC_btnNEXT , IDC_btnCHANGE , IDC_btnCLOSE , 
		IDC_staSHELFCHANGEPOS
	};

	CRect WndRect;
	GetWindowRect( &WndRect );


	// 일괄 변경시
	INT nHeight;
	if( nMode == 0 ) 
	{
		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			GetDlgItem( nUnVisibleIDC[i] )->ShowWindow( SW_HIDE );
		for( i = 0 ; i < nVisibleCnt ; i++ )
			GetDlgItem( nVisibleIDC[i] )->ShowWindow( SW_SHOW );

		nHeight = 290;
		MoveWindow( WndRect.left , WndRect.top ,  WndRect.Width() , nHeight );

		CRect ControlRect;
		for( i = 0 ; i < nMoveCnt ; i++ )
		{
			GetDlgItem( nMoveIDC[i] )->GetWindowRect( &ControlRect );
//			ClientToScreen( ControlRect );
			ScreenToClient( ControlRect );
			GetDlgItem( nMoveIDC[i] )->MoveWindow( ControlRect.left , m_nMovePos[i]-180 , ControlRect.Width() , ControlRect.Height() );
		}

	}
	// 낱권 변경시
	else
	{
		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			GetDlgItem( nUnVisibleIDC[i] )->ShowWindow( SW_SHOW );
		for( i = 0 ; i < nVisibleCnt ; i++ )
			GetDlgItem( nVisibleIDC[i] )->ShowWindow( SW_HIDE );

		nHeight = 470;
		MoveWindow( WndRect.left , WndRect.top ,  WndRect.Width() , nHeight );

		CRect ControlRect;
		for( i = 0 ; i < nMoveCnt ; i++ )
		{
			GetDlgItem( nMoveIDC[i] )->GetWindowRect( &ControlRect );
//			ClientToScreen( ControlRect );
			ScreenToClient( ControlRect );
			GetDlgItem( nMoveIDC[i] )->MoveWindow( ControlRect.left , m_nMovePos[i] , ControlRect.Width() , ControlRect.Height() );
		}
	}


	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1330::InitMovePos()
{
	EFS_BEGIN

	const INT nMoveCnt = 16;

	INT nMoveIDC[nMoveCnt] = 
	{
		IDC_staCHANGEGROUP , IDC_cmbCHANGE1 , IDC_datCHANGE3 , IDC_edtCHANGE10 , IDC_cmbCHANGE2 , 
		IDC_edtCHANGE11 , IDC_edtCHANGE11 , IDC_staWILLCHANGESHELFDATE , IDC_staWILLCHANGEREASONCODE , IDC_staWORKER , 
		IDC_staBUTTONLINE , IDC_btnPREV , IDC_btnNEXT , IDC_btnCHANGE , IDC_btnCLOSE , 
		IDC_staSHELFCHANGEPOS
	};

	CRect ControlRect;
	for( INT i = 0 ; i < nMoveCnt ; i++ )
	{
		GetDlgItem( nMoveIDC[i] )->GetWindowRect( &ControlRect );
		ScreenToClient( ControlRect );
		m_nMovePos[i] = ControlRect.top;
	}
	


	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1330::InitShelfWorker()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1330");

	// 배가 지시자 적용
	CString sUserName;
/*
	ids = GetControlData( sCMAlias , _T("변경지시자_입력") , sUserName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1330::SetShelfChangeResonControl()") );

	if( !sUserName.IsEmpty() ) return 0;
*/	
	sUserName = GetUserID();

	ids = SetControlData( sCMAlias , _T("변경지시자_입력") , sUserName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1330::SetShelfChangeResonControl()") );

	return 0;

	EFS_END
	return -1;

}

VOID CSE_LOC_1330::OnSelchangecmbCHANGE2() 
{
	EFS_BEGIN

	CString sCode;
	GetReasonCode( sCode );

	if( sCode.Compare( _T("ETC") ) == 0 ) 
	{
		GetDlgItem( IDC_edtCHANGE11 )->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem( IDC_edtCHANGE11 ))->SetReadOnly(FALSE);
	}
	else if( sCode.Compare(_T("적용안함"))==0 )
	{
		GetDlgItem( IDC_edtCHANGE11 )->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem( IDC_edtCHANGE11 )->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem( IDC_edtCHANGE11 ))->SetReadOnly(TRUE);
	}

	CString sReasonData;
	m_pReasonList->GetDesc( sCode , sReasonData );


	SetReasonDesc( sReasonData );

	EFS_END
}



VOID CSE_LOC_1330::SetChangeButtonEnable()
{
	EFS_BEGIN

	CString sData;
	INT ids;

	ids = GetControlData( _T("CM_SE_LOC_1330") , _T("변경위치_입력") , sData , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SetChangeButtonEnable") );
	sData.TrimLeft(); sData.TrimRight();

	if( sData.IsEmpty() )
	{
		GetDlgItem(IDC_btnCHANGE)->EnableWindow(FALSE);	
	}
	else
	{
		GetDlgItem(IDC_btnCHANGE)->EnableWindow(TRUE);	
	}

	return ;

	EFS_END
}

VOID CSE_LOC_1330::OnSelchangecmbCHANGE1() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}


INT CSE_LOC_1330::DisplayBookStatus()
{
	EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1300");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayBookStatus") );
	
	const INT nFieldCnt = 10;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("본표제") , _T("저작자") , _T("BB_편/권차") , _T("BB_편제") , _T("BB_등록번호") , 
		_T("청구기호") , _T("BB_자료실명") , _T("BB_최초배가일자") , _T("BB_배가변경일자") , _T("BB_배가변경사유내용")
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("서  명") , _T("저작자") , _T("편/권차") , _T("권호명") , _T("등록번호") , 
		_T("청구기호") , _T("배가위치") , _T("배가일자") , _T("배가변경일자") , _T("배가변경사유")
	};
	
	
	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData ) \r\n sDMFieldName[i] : %s \r\n m_nCurrentIndex : %d ") , 
				sDMFieldName[i] , m_nCurrentIndex );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayBookStatus") );
		
		m_ViewerControl.SetControlData( sTitleName[i] , sData );
		
	}
	
	m_ViewerControl.Display();
	
	return 0;

	EFS_END

	return -1;
}
HBRUSH CSE_LOC_1330::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
