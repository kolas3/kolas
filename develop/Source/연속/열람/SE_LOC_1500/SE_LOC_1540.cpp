// SE_LOC_1540.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1540.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1540 dialog


CSE_LOC_1540::CSE_LOC_1540(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1540)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1540::~CSE_LOC_1540()
{
}


VOID CSE_LOC_1540::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1540)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1540, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1540)
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
// CSE_LOC_1540 message handlers

BOOL CSE_LOC_1540::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1540::OnClose() 
{	
	EFS_BEGIN

	CDialog::OnClose();

	EFS_END
}

BOOL CSE_LOC_1540::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1540")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_15(this);
	
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

VOID CSE_LOC_1540::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

INT CSE_LOC_1540::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = m_pSpfWork->GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	ids = AllControlDisplay( _T("CM_SE_LOC_1540") , m_nCurrentIndex );
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


VOID CSE_LOC_1540::OnShowWindow(BOOL bShow, UINT nStatus) 
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

VOID CSE_LOC_1540::OnbtnPREV() 
{
	EFS_BEGIN

	INT ids = MoveIndex( 0 );
	if( ids < 0 ) return;



	EFS_END
}

VOID CSE_LOC_1540::OnbtnNEXT() 
{
	EFS_BEGIN

	INT ids = MoveIndex( 1 );
	if( ids < 0 ) return;

	EFS_END
}

INT CSE_LOC_1540::MoveIndex( INT nMode )
{
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
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

VOID CSE_LOC_1540::InitControl()
{
	EFS_BEGIN

	((CButton*)GetDlgItem( IDC_radALL1 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radALL2 ))->SetCheck(1);

	EFS_END
}

VOID CSE_LOC_1540::OnbtnCHANGE() 
{
	INT ids;
	INT nCheckFlag;
	nCheckFlag = ((CButton*)GetDlgItem( IDC_radALL1 ))->GetCheck();

	if( nCheckFlag == 1 )
		ids = m_pSpfWork->SPFExcutePath( ALL_BOOK_SHELF_CHANGE );
	else
	{
		m_pSpfWork->SetTempData( _T("선택인덱스") , CComVariant(m_nCurrentIndex) );
		ids = m_pSpfWork->SPFExcutePath( ONE_BOOK_SHELF_CHANGE );
		if( ids > 0 ) return ;
		OnbtnNEXT();
	}

	

}

VOID CSE_LOC_1540::OnradALL1() 
{
	EFS_BEGIN

	// 종수 , 책수를 계산하여 넣어준다.

	INT ids;


	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("종정보KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();

	sBookCnt.Format(_T("%d") , pMainGrid->SelectGetCount() );

	CString sCMAlias = _T("CM_SE_LOC_1540");
	ids = SetControlData( sCMAlias , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sCMAlias , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -5 , _T("DisplayDefaultValue") ); 


	ViewModeChange(0);

	EFS_END
}

VOID CSE_LOC_1540::OnradALL2() 
{
	EFS_BEGIN

	ViewModeChange(1);
	return;

	EFS_END
}

VOID CSE_LOC_1540::InitSelectCheck()
{
	EFS_BEGIN

	INT ids;


	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
	if( pGrid == NULL ) return ;
	
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) return;


	EFS_END
}

INT CSE_LOC_1540::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
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

// 배가 변경 사유 콘트롤 보여주는 함수
VOID CSE_LOC_1540::SetShelfChangeReasonControl()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1540");
	CString sControlAlias = _T("변경사유코드_입력");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CSE_LOC_1540::SetShelfChangeResonControl()") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CSE_LOC_1540::SetShelfChangeResonControl()") );

	CString *sGetData;
	sGetData = NULL;

	INT nRowCount;

	INT nReasonCnt = GetShelfChangeReasonCount();

	sGetData = new CString[nReasonCnt];

	ids = GetShelfChangeReasonList( sGetData , nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("CSE_LOC_1540::SetShelfChangeResonControl()") );

	for( INT i = 0 ; i < nRowCount ; i++ )
	{	
		pComboBox->AddString( sGetData[i] );
	}
	
	pComboBox->SetCurSel(0);

	if( sGetData != NULL ) delete []sGetData;


	EFS_END
}


INT CSE_LOC_1540::GetShelfChangeReasonList( CString sGetData[] , INT &nRowCount )
{
	EFS_BEGIN

	m_pReasonList->GetList( sGetData , nRowCount );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1540::GetShelfChangeReasonCount()
{
	EFS_BEGIN

	return m_pReasonList->GetListCount();

	EFS_END
	return -1;

}

INT CSE_LOC_1540::GetReasonCode( CString &sCode )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1540");
	CString sControlAlias = _T("변경사유코드_입력");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::GetReasonCode( CString &sCode )") );

	ids = pCM->GetControlMgrData( sControlAlias , sCode , -1 , 0 );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1540::GetReasonCode( CString &sCode )") );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1540::SetReasonDesc( CString sReasonData )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1540");
	CString sControlAlias = _T("변경사유내용_입력");

	ids = SetControlData( sCMAlias , sControlAlias , sReasonData );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::SetReasonDesc( CString sReasonData )") );
	
	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1540::ChangeShelfReasonControl()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SE_LOC_1500");
	CString sFieldAlias = _T("배가변경사유코드");
	CString sCMAlias = _T("CM_SE_LOC_1540");
	CString sControlAlias = _T("변경사유코드_입력");

	CString sCode;
	ids = GetDataMgrData( sDMAlias , sFieldAlias , sCode , m_nCurrentIndex );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::ChangeShelfReasonControl()") );

	CComboBox *pComboBox = (CComboBox*)(GetDlgItem( IDC_cmbCHANGE2 ));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1540::ChangeShelfReasonControl()") );

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
INT CSE_LOC_1540::ViewModeChange( INT nMode )
{
	EFS_BEGIN

	const INT nVisibleCnt = 5;
	const INT nUnVisibleCnt = 20;
	const INT nMoveCnt = 16;

	INT nVisibleIDC[nVisibleCnt] = 
	{
		IDC_staCHANGEGROUP3 , IDC_staWILLCHANGESHELFDATE2 , IDC_datCHANGE4 , IDC_staWILLCHANGESHELFDATE3 , IDC_datCHANGE5 
	};
	INT nUnVisibleIDC[nUnVisibleCnt] = 
	{ 
		IDC_staTITLE , IDC_staAUTHOR , IDC_staVOLTITLE  , IDC_staSHELFLOC , IDC_staCHANGEREASON , 
		IDC_staSHELFDATE , IDC_staSHELFCHANGEDATE , IDC_edtCHANGE1 , IDC_edtCHANGE2 ,IDC_edtCHANGE4 , 
		IDC_cmbSHELF , IDC_edtCHANGE9 , IDC_datCHANGE1 , IDC_datCHANGE2 , IDC_btnPREV , 
		IDC_btnNEXT , IDC_staCHANGEGROUP2 , IDC_cmbSHELF2 , IDC_staTITLE2 , IDC_gridBOOK_INFO
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

		nHeight = 280;
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

		nHeight = 460;
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

INT CSE_LOC_1540::InitMovePos()
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

INT CSE_LOC_1540::InitShelfWorker()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_SE_LOC_1540");

	// 배가 지시자 적용
	CString sUserName;
/*
	ids = GetControlData( sCMAlias , _T("변경지시자_입력") , sUserName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::SetShelfChangeResonControl()") );

	if( !sUserName.IsEmpty() ) return 0;
*/	
	sUserName = GetUserID();

	ids = SetControlData( sCMAlias , _T("변경지시자_입력") , sUserName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1540::SetShelfChangeResonControl()") );

	return 0;

	EFS_END
	return -1;

}

VOID CSE_LOC_1540::OnSelchangecmbCHANGE2() 
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



VOID CSE_LOC_1540::SetChangeButtonEnable()
{
	EFS_BEGIN

	CString sData;
	INT ids;

	ids = GetControlData( _T("CM_SE_LOC_1540") , _T("변경위치_입력") , sData , -1 , 0 );
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

VOID CSE_LOC_1540::OnSelchangecmbCHANGE1() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}


INT CSE_LOC_1540::DisplayBookStatus()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1500");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayBookStatus") );
	
	const INT nFieldCnt = 8;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("수입구분") , _T("서명") , _T("발행자") , _T("권호명") , _T("자료실") , 
		_T("최초배가일자") , _T("배가변경일자") , _T("배가변경사유설명")
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("수입구분") , _T("서명") , _T("발행자") , _T("권호명") , _T("자료실") , 
		_T("최초배가일자") , _T("배가변경일자") , _T("배가변경사유")
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
HBRUSH CSE_LOC_1540::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
