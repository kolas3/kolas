// BO_LOC_3210.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3210.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include  "BO_LOC_32001.h"

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3210 dialog


CBO_LOC_3210::CBO_LOC_3210(CESL_Mgr* pParent , CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3210)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoan = pLona;
	m_pLoanShareManager = pLoanShareManager;

	m_pSpfWork = NULL;
	m_pSpfWork = new CBL_LOC_LOAN_MNG_PROC(this, pLona , m_pLoanShareManager);

	GetManageValue( _T("기타"), _T("공통"), _T("국방대"), _T(""), m_sDefenceFlag );	
	GetManageValue( _T("열람"), _T("공통"), _T("대출자료관리"), _T("야간대출예약기능유무"), m_sNightReservationFlag );	
	GetManageValue( _T("기타"), _T("공통"), _T("타관반납설정"), _T(""), m_sOtherLibCodeReturn );	
	GetManageValue( _T("열람"), _T("공통"), _T("대출자료관리"), _T("무인대출예약기능유무"), m_sUnmannedReservationFlag );
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();

	if( _tcscmp( m_sDefenceFlag, _T("Y") ) != 0 && 
		_tcscmp( m_sNightReservationFlag, _T("Y") ) != 0 &&
		_tcscmp( m_sOtherLibCodeReturn, _T("Y") ) != 0 &&
		_tcscmp( m_sUnmannedReservationFlag, _T("Y") ) != 0 )
	{
		m_sDefenceFlag				= _T("N");
		m_sNightReservationFlag		= _T("N");
		m_sOtherLibCodeReturn		= _T("N");
		m_sUnmannedReservationFlag	= _T("N");
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
	}
	else if( _tcscmp( m_sNightReservationFlag, _T("Y") ) == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_LEE );
		m_nIDHelp = IDD_LEE;
	}
	else if( _tcscmp( m_sUnmannedReservationFlag, _T("Y") ) == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_UNMANNED );
		m_nIDHelp = IDD_UNMANNED;
	}
	else if( _tcscmp( m_sDefenceFlag, _T("Y") ) == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_ND );
		m_nIDHelp = IDD_ND;
	}
	else if( _tcscmp( m_sOtherLibCodeReturn, _T("Y") ) == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_LIB );
		m_nIDHelp = IDD_LIB;
	}
}

CBO_LOC_3210::~CBO_LOC_3210()
{
	if( m_pSpfWork != NULL ) 
		delete m_pSpfWork;
}

VOID CBO_LOC_3210::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3210)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3210, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3210)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDetail, OnbtnDetail)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnFind, OnbtnFind)
	ON_BN_CLICKED(IDC_chkFIND1, OnchkFIND1)
	ON_BN_CLICKED(IDC_chkFIND2, OnchkFIND2)
	ON_BN_CLICKED(IDC_chkFIND3, OnchkFIND3)
	ON_BN_CLICKED(IDC_chkFIND4, OnchkFIND4)
	ON_BN_CLICKED(IDC_chkFIND5, OnchkFIND5)
	ON_BN_CLICKED(IDC_chkFIND6, OnchkFIND6)
	ON_BN_CLICKED(IDC_chkFIND7, OnchkFIND7)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_chkFIND8, OnchkFIND8)
	ON_BN_CLICKED(IDC_chkFIND9, OnchkFIND9)
	ON_BN_CLICKED(IDC_chkFIND10, OnchkFIND10)
	ON_BN_CLICKED(IDC_chkFIND11, OnchkFIND11)
	ON_BN_CLICKED(IDC_chkFIND14, OnchkFIND14)
	ON_BN_CLICKED(IDC_chkFIND13, OnchkFIND13)
	ON_BN_CLICKED(IDC_chkFIND16, OnchkFIND16)
	ON_BN_CLICKED(IDC_chkFIND17, OnchkFIND17)
	ON_BN_CLICKED(IDC_chkFIND18, OnchkFIND18)
	ON_BN_CLICKED(IDC_chkFIND19, OnchkFIND19)
	ON_BN_CLICKED(IDC_BTN_REG_NO_MODE_CHANGE, OnBtnRegNoModeChange)
	ON_EN_SETFOCUS(IDC_edtFIND5, OnFocusedtFIND5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3210 message handlers

BOOL CBO_LOC_3210::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class

 	if ( _tcscmp( m_sDefenceFlag , _T("N") ) == 0 && 
 		 _tcscmp( m_sNightReservationFlag , _T("N") ) == 0 && 
 		 _tcscmp( m_sUnmannedReservationFlag , _T("N") ) == 0 )
 		return CDialog::Create(IDD, pParentWnd);
	else if ( _tcscmp( m_sNightReservationFlag , _T("Y") ) == 0 )
 		return CDialog::Create(IDD_LEE, pParentWnd);
 	else if ( _tcscmp( m_sUnmannedReservationFlag , _T("Y") ) == 0 )
 		return CDialog::Create(IDD_UNMANNED, pParentWnd);
 	else if ( _tcscmp( m_sDefenceFlag , _T("Y") ) == 0 )
 		return CDialog::Create(IDD_ND, pParentWnd);
 	else if ( _tcscmp( m_sOtherLibCodeReturn , _T("Y") ) == 0 )
 		return CDialog::Create(IDD_LIB, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3210::OnClose() 
{	
EFS_BEGIN

	OnCancel();

EFS_END
}

BOOL CBO_LOC_3210::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3210")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowSize( CString(_T("간략검색")) );

	CString strValue;
	m_pLoan->GetManageValue(_T("기타"),_T("공통"),_T("육사"),_T("육사"),strValue);
	strValue.TrimLeft();strValue.TrimRight();
	strValue.MakeUpper();
	if ( strValue == _T("Y") )
		((CComboBox*)GetDlgItem(IDC_cmbFIND1))->AddString( _T("장기") );

	((CComboBox*)GetDlgItem(IDC_cmbFIND1))->SetCurSel(0);

	if ( m_sDefenceFlag != _T("Y") )
		((CComboBox*)GetDlgItem(IDC_cmbACQ_CODE))->SetCurSel(0);

 	m_pLoan->GetManageValue(_T("열람"),_T("공통"),_T("대출자료관리"),_T("야간대출예약기능유무"),strValue);
	strValue.TrimLeft();strValue.TrimRight();
	strValue.MakeUpper();
	if ( strValue == _T("Y") )
		GetDlgItem(IDC_chkFIND11)->ShowWindow(SW_SHOW);

	m_pLoan->GetManageValue(_T("열람"), _T("공통"), _T("무인반납중자료상태사용여부"), _T(""), strValue);
	if (strValue.CompareNoCase(_T("Y")) == 0)
	{ 
		GetDlgItem(IDC_chkFIND8)->ShowWindow(SW_SHOW);
	}

	ChangeScreenFromOtherManageReturn();

	CreateMyTooltip();

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3210"), _T("등록번호1"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_3210"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3210"), _T("등록번호2"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_3210"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3210::SetWindowSize( CString &sCaption )
{
	EFS_BEGIN

	CRect ChangeWndPos;		/// 변경될 콘트롤 사이즈를 얻어오기 위한 인스턴스
	CRect WndPos;				  /// 현재 윈도우의 위치를 얻어오기 위한 인스턴스	
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnDetail )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEWDETAIL;
	}
	else
	{
		GetDlgItem( IDC_btnDetail )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEWSIMPLE;
	}

	GetWindowRect( &WndPos );
	
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	CenterWindow();

	CRect cr1, cr2, cr3, cr4;

	CButton* pCheck1 = ( CButton* ) GetDlgItem( IDC_chkFIND9 );
	CButton* pCheck2 = ( CButton* ) GetDlgItem( IDC_chkFIND7 );
	CButton* pCheck3 = ( CButton* ) GetDlgItem( IDC_chkFIND8 );
	CButton* pCheck4 = ( CButton* ) GetDlgItem( IDC_chkFIND10 );
	
	pCheck1->GetWindowRect( &cr1 );
	pCheck2->GetWindowRect( &cr2 );
	pCheck3->GetWindowRect( &cr3 );
	pCheck4->GetWindowRect( &cr4 );

	ScreenToClient( cr1 );
	ScreenToClient( cr2 );
	ScreenToClient( cr3 );
	ScreenToClient( cr4 );	

	CString strIsReturn;
	m_pLoan->GetManageValue( _T("기타"), _T("공통"), _T("관리구분별반납설정"), _T(""), strIsReturn );

	CRect cr5;
	CButton* pCheck5 = ( CButton* ) GetDlgItem( IDC_chkFIND12 ); // 타관반납중		
	pCheck5->GetWindowRect( &cr5 );
	ScreenToClient( cr5 );
	
	if ( m_pLoan->m_bVolReserve )
	{	
		if(strIsReturn.Compare(_T("Y")) != 0 )
		{
			CRect cr6;
			CButton* pCheck6 = ( CButton* ) GetDlgItem( IDC_chkFIND5 );
			pCheck6->GetWindowRect( &cr6 );
			ScreenToClient( cr6 );

			if ( cr5.left == cr6.left ) return;
			pCheck5->MoveWindow( cr6.left, cr5.top, cr5.Width( ), cr5.Height( ) );
		}
		else
		{
			const INT nCtrlCnt = 5;

			UINT UpLineCtrlID[ nCtrlCnt ] = {
				IDC_chkFIND2, IDC_chkFIND3, IDC_chkFIND4, IDC_chkFIND5, IDC_chkFIND8,
			};

			UINT DownLineCtrlID[ nCtrlCnt ] = {
				IDC_chkFIND6, IDC_chkFIND9, IDC_chkFIND7, IDC_chkFIND10, IDC_chkFIND12
			};

			CButton* pUpButton = NULL;
			CButton* pDownButton = NULL;

			CRect rcUp, rcDown;

			for( INT i=0;i<nCtrlCnt;i++ )
			{
				pUpButton   = ( CButton* )GetDlgItem( UpLineCtrlID[i] );
				pDownButton = ( CButton* )GetDlgItem( DownLineCtrlID[i] );
				
				pUpButton->GetWindowRect( &rcUp );
				pDownButton->GetWindowRect( &rcDown );

				ScreenToClient( rcUp );
				ScreenToClient( rcDown );

				pDownButton->MoveWindow( rcUp.left, rcDown.top, rcDown.Width( ), rcDown.Height( ) );
			}
		}
	}
	else
	{
		if ( cr1.left != cr2.left ) 
		{
			pCheck5->MoveWindow( cr4.left, cr5.top, cr5.Width( ), cr5.Height( ) );
			pCheck4->MoveWindow( cr2.left, cr4.top, cr4.Width( ), cr4.Height( ) );
			pCheck2->MoveWindow( cr1.left, cr2.top, cr2.Width( ), cr2.Height( ) );
			pCheck1->ShowWindow( FALSE );			
		}

		GetDlgItem( IDC_chkFIND6 )->SetWindowText( _T("예약") );		
		
		if( strIsReturn.Compare(_T("Y")) != 0 )
		{
			CRect cr6;
			CButton* pCheck6 = ( CButton* ) GetDlgItem( IDC_chkFIND4 );
			pCheck6->GetWindowRect( &cr6 );
			ScreenToClient( cr6 );
		
			if ( cr5.left == cr6.left ) return;
			pCheck5->MoveWindow( cr6.left, cr5.top, cr5.Width( ), cr5.Height( ) );	
		}
	}	

	EFS_END
}


CString CBO_LOC_3210::GetCurrentWndMode()
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDetail )->GetWindowText( sCaption );

	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		return szCaption[1];
	}
	else
	{
		return szCaption[0];
	}

	EFS_END
	return _T("");

}

VOID CBO_LOC_3210::OnbtnDetail() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDetail )->GetWindowText( sCaption );
	SetWindowSize( sCaption );	

	EFS_END
}

VOID CBO_LOC_3210::OnbtnClose() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_3210::OnbtnFind() 
{
	EFS_BEGIN

	INT ids = 0;

	ids = CheckIsInputDateControl_And_SetDefaultDate();
	if(0 > ids) ERROR_MSG_RETURN_VOID2( -3 , _T("CBO_LOC_3210::OnbtnFind()") );
	if(IDCANCEL == ids) return;

	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_검색") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3210::OnbtnFind()") );

	ids = TabView();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("CBO_LOC_3210::OnbtnFind()") )

	EFS_END
}


VOID CBO_LOC_3210::OnchkFIND1() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND1))->GetCheck();
	if( nCheck == 1 )
	{
		AllControlCheck(1);

		NightControlCheck(0);
	}
	else
	{
		AllControlCheck(0);
	}

	EFS_END
}

INT CBO_LOC_3210::AllControlCheck(INT nCheck)
{
EFS_BEGIN

	const INT CHECK_CNT = 16;

	INT nCHECKIDC[CHECK_CNT] = 
	{
		IDC_chkFIND1, IDC_chkFIND2, IDC_chkFIND3, IDC_chkFIND4, IDC_chkFIND5,
		IDC_chkFIND6, IDC_chkFIND7, IDC_chkFIND8, IDC_chkFIND9, IDC_chkFIND10,
		IDC_chkFIND11, IDC_chkFIND16, IDC_chkFIND17, IDC_chkFIND18, IDC_chkFIND19,
		IDC_chkFIND12		
	};

	CButton* pBtn = NULL;
	for( INT i=0;i<CHECK_CNT;i++ )
	{
		pBtn = (CButton*)GetDlgItem(nCHECKIDC[i]);
		if( pBtn )
		{
			if( pBtn->IsWindowVisible() )
			pBtn->SetCheck( nCheck );
		}
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3210::OnchkFIND2() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND2))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnchkFIND3() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND3))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnchkFIND4() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND4))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnchkFIND5() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND5))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnchkFIND6() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND6))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnchkFIND7() 
{
	EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND7))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);

	EFS_END
}

VOID CBO_LOC_3210::OnbtnCLEAR() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3210");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2(-1,_T("OnbtnCLEAR"));

	pCM->AllControlClear();

	AllControlCheck(0);			

	EFS_END
}

BOOL CBO_LOC_3210::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	m_ToolTip.RelayEvent(pMsg);

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnFind();
		return TRUE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}


INT CBO_LOC_3210::TabView()
{
	EFS_BEGIN

	INT ids;
	DWORD START1 = GetTickCount();

	CString sTitle, sTitle2;
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3200"));

	int nMOCnt = 0;
	int nSECnt = 0;
	int nNBCnt = 0;

	CStringArray saUserList;
	CString sUserKey;
	BOOL bIsInsert;
	for(int i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		bIsInsert = TRUE;

		ids = pDM->GetCellData( _T("대출자KEY") , i , sUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("TabView") );

		CString strPublishFormCode;
		ids = pDM->GetCellData( _T("단행/연속구분") , i , strPublishFormCode);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("TabView") );

		if(_T("MO") == strPublishFormCode) nMOCnt++;
		else if(_T("SE") == strPublishFormCode) nSECnt++;
		else if(_T("NB") == strPublishFormCode) nNBCnt++;

		for( INT j = 0 ; j < saUserList.GetSize() ; j++ )
		{
			if( !bIsInsert ) break;
			if( sUserKey.Compare(saUserList.GetAt(j)) ==0 )
			{
				bIsInsert = FALSE;
			}
		}

		if( !bIsInsert ) continue;

		saUserList.Add(sUserKey);
	}

	CString sTmpTitle[3];
	if(0 < nMOCnt) sTmpTitle[0].Format(_T("[단행 : %d건]"), nMOCnt);
	if(0 < nSECnt) sTmpTitle[1].Format(_T("[연속 : %d건]"), nSECnt);
	if(0 < nNBCnt) sTmpTitle[2].Format(_T("[긴급 : %d건]"), nNBCnt);

	for(int nCnt = 0; nCnt < 3; nCnt++)
	{
		if(FALSE == sTmpTitle[nCnt].IsEmpty()) 
		{
			if(FALSE == sTitle.IsEmpty()) sTitle += _T(" , ");
			sTitle += sTmpTitle[nCnt];
		}
	}


	if( saUserList.GetSize() > 0 )
	{
		if( !sTitle.IsEmpty() )
			sTitle += _T(" , ");
		sTitle2.Format( _T("[%s : %d명]") ,_T("이용자수") , saUserList.GetSize() );
		sTitle += sTitle2;
	}

	if( sTitle.IsEmpty() )
		sTitle = _T("자료가 존재하지 않습니다.");

	

	sTitle = _T(" ") + sTitle +_T("                             ");

	CWnd* pTitleBar = (((CWnd*)(pParentMgr->GetDlgItem(IDC_staMESSAGE))));
	
	pTitleBar->SetWindowText( sTitle );

	DWORD FINISH1 = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - 대출 자료 관리 자료 찾기(탭 디스플레이) 걸린 시간 :  [%d.%d sec]\n"), (FINISH1-START1)/1000, (FINISH1-START1)%1000);
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3210::OnchkFIND8() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND8))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	
	EFS_END	
}


INT CBO_LOC_3210::SetSelfReturnStatusUseYN(CString sSelfReturnUseYN)
{
EFS_BEGIN

	m_sSelfReturnUseYN = sSelfReturnUseYN;

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_3210::ChangeScreenFromOtherManageReturn()
{
EFS_BEGIN
	
	INT		ids;
	CString strIsReturn;

	if( _T("Y") == m_pLoan->m_strEachManageUseYN )
	{
		ids = m_pLoan->GetManageValue( _T("기타"), _T("공통"), _T("관리구분별반납설정"), _T(""), strIsReturn );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

		if( _T("Y") == strIsReturn )
		{
			CWnd* pWnd = NULL;

			pWnd = GetDlgItem(IDC_chkFIND10);

			if( pWnd )
			{
				pWnd->ShowWindow( SW_SHOW );
			}
		}
	}

	if( _T("Y") == m_sOtherLibCodeReturn )
	{
		CWnd* pWnd = NULL;

		pWnd = GetDlgItem(IDC_chkFIND12);

		if( pWnd )
		{
			pWnd->ShowWindow( SW_SHOW );
		}
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3210::OnchkFIND9() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND9))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	
	EFS_END	
}

VOID CBO_LOC_3210::OnchkFIND10() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND10))->GetCheck();
	if( nCheck == 0 )
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	
	EFS_END	
}

VOID CBO_LOC_3210::OnchkFIND11() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND11))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkFIND13))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND14))->SetCheck(0);
		
	}
	
	EFS_END	
}

VOID CBO_LOC_3210::OnchkFIND13() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND13))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND11))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND14))->SetCheck(0);
		
	}
	
	EFS_END	
	
}

VOID CBO_LOC_3210::OnchkFIND14() 
{
	EFS_BEGIN
		
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND14))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND11))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND13))->SetCheck(0);	
	}
	
	EFS_END		
}

void CBO_LOC_3210::OnBtnRegNoModeChange() 
{
	CString strText;
	GetDlgItemText(IDC_BTN_REG_NO_MODE_CHANGE, strText);

	if( _T("구분") == strText )
	{
		strText = _T("범위");
		SetDlgItemText(IDC_BTN_REG_NO_MODE_CHANGE, strText);
	
		GetDlgItem(IDC_STA_REG_NO_RANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtFIND1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtFIND2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDT_REG_NO_ADDITION)->ShowWindow(SW_SHOW);	
		
		((CButton*)GetDlgItem(IDC_CHK_REG_NO_MODE))->SetCheck(1);
	}
	else if( _T("범위") == strText )
	{
		strText = _T("구분");
		SetDlgItemText(IDC_BTN_REG_NO_MODE_CHANGE, strText);

		GetDlgItem(IDC_STA_REG_NO_RANGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtFIND1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtFIND2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDT_REG_NO_ADDITION)->ShowWindow(SW_HIDE);

		((CButton*)GetDlgItem(IDC_CHK_REG_NO_MODE))->SetCheck(0);
	}
}

HBRUSH CBO_LOC_3210::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_3210::NightControlCheck(INT nCheck)
{
EFS_BEGIN

	const INT CHECK_CNT = 2;

	INT nCHECKIDC[CHECK_CNT] = 
	{
		IDC_chkFIND13, IDC_chkFIND14
	};

	CButton* pBtn = NULL;
	for( INT i = 0; i < CHECK_CNT; i++ )
	{
		pBtn = (CButton*)GetDlgItem(nCHECKIDC[i]);
		if( pBtn )
		{
			if( pBtn->IsWindowVisible() )
			pBtn->SetCheck( nCheck );
		}
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3210::OnchkFIND16() 
{
EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND16))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND17))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND18))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND19))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkFIND17))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_chkFIND18))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_chkFIND19))->SetCheck(1);
	}
	
EFS_END		
}

VOID CBO_LOC_3210::OnchkFIND17() 
{
EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND17))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND16))->SetCheck(0);
	}
	else
	{
		
	}

EFS_END
}

VOID CBO_LOC_3210::OnchkFIND18() 
{
EFS_BEGIN

	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND18))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND16))->SetCheck(0);
	}
	else
	{
		
	}

EFS_END
}

VOID CBO_LOC_3210::OnchkFIND19() 
{
EFS_BEGIN
	
	INT nCheck = ((CButton*)GetDlgItem(IDC_chkFIND19))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkFIND1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_chkFIND16))->SetCheck(0);
	}
	else
	{

	}

EFS_END	
}

VOID CBO_LOC_3210::CreateMyTooltip()
{
EFS_BEGIN

	CString sHelp = _T("※ 찾고자 하시는 주소의 동이나\r\n 군이나 도이름을 입력하십시오.\r\n")
					_T("예) 역삼동, 화양동, 서울, 연기군");

	m_ToolTip.Create(this);

	m_ToolTip.SetMaxTipWidth(400);
	m_ToolTip.AddTool( GetDlgItem(IDC_edtSADDRESS) , sHelp );
	m_ToolTip.Activate(TRUE);

	m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(0,0));
	m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(20000,0));

EFS_END
} 

VOID CBO_LOC_3210::OnFocusedtFIND5()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND5)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND5)->m_hWnd,hIME); 
    }
}

int CBO_LOC_3210::CheckIsInputDateControl_And_SetDefaultDate()
{
	CString strFuncName = _T("CheckIsInputDateControl_And_SetDefaultDate");
	int ids = 0;
	int nCnt = 0;

	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_LOC_3210") );
	if( NULL == pCM ) ERROR_MSG_RETURN_ERROR_CODE2( -11, strFuncName );

	CStringArray astrControlAlias;
	astrControlAlias.Add(_T("대출자료상태_대출|대출일1|대출일2|대출일"));
	astrControlAlias.Add(_T("대출자료상태_반납|반납일1|반납일2|반납일"));
	astrControlAlias.Add(_T("대출자료상태_연체|반납예정일1|반납예정일2|반납예정일"));
	astrControlAlias.Add(_T("대출자료상태_반납연기|반납예정일1|반납예정일2|반납예정일"));
	astrControlAlias.Add(_T("대출자료상태_분관반납중|반납일1|반납일2|반납일"));
	astrControlAlias.Add(_T("대출자료상태_예약|예약일1|예약일2|예약일"));
	astrControlAlias.Add(_T("대출자료상태_예약완료|예약일1|예약일2|예약일"));
	astrControlAlias.Add(_T("대출자료상태_예약취소|예약취소일1|예약취소일2|예약취소일"));
	astrControlAlias.Add(_T("대출자료상태_무인반납중|대출일1|대출일2|대출일"));



 	if ( _tcscmp( m_sDefenceFlag , _T("N") ) == 0 && 
 		 _tcscmp( m_sNightReservationFlag , _T("N") ) == 0 && 
 		 _tcscmp( m_sUnmannedReservationFlag , _T("N") ) == 0 )
	{
	}
	else if ( _tcscmp( m_sNightReservationFlag , _T("Y") ) == 0 )
	{
		astrControlAlias.Add(_T("야간대출상태_신청|예약일1|예약일2|예약일"));
		astrControlAlias.Add(_T("야간대출상태_접수|예약일1|예약일2|예약일"));
		astrControlAlias.Add(_T("대출자료상태_타관반납중|반납일1|반납일2|반납일"));
	}
 	else if ( _tcscmp( m_sUnmannedReservationFlag , _T("Y") ) == 0 ) 	
	{
		astrControlAlias.Add(_T("무인대출상태_신청|예약일1|예약일2|예약일"));
		astrControlAlias.Add(_T("무인대출상태_대기|예약일1|예약일2|예약일"));
		astrControlAlias.Add(_T("무인대출상태_지연|예약일1|예약일2|예약일"));
		astrControlAlias.Add(_T("대출자료상태_타관반납중|반납일1|반납일2|반납일"));
	}
 	else if ( _tcscmp( m_sDefenceFlag , _T("Y") ) == 0 )
	{
	}
 	else if ( _tcscmp( m_sOtherLibCodeReturn , _T("Y") ) == 0 )
	{
	}

	const INT CONTROL_ALIAS_CNT = astrControlAlias.GetSize();
	
	CStringArray astrIsChecked;
	for(nCnt = 0; nCnt < CONTROL_ALIAS_CNT; nCnt++)
	{
		astrIsChecked.Add(_T("FALSE"));
	}
	
	int nCheckCnt = 0;
	int nInputCnt = 0;	
	
	for(nCnt = 0; nCnt < CONTROL_ALIAS_CNT; nCnt++)
	{
		CString strBtnAlias;
		AfxExtractSubString(strBtnAlias, astrControlAlias.GetAt(nCnt), 0, '|');

		CESL_Control_Element* pCE = pCM->FindControlElement( strBtnAlias );
		if(NULL == pCE) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

 		CButton* pBtn = (CButton*)GetDlgItem(pCE->CTRL_ID);
 		if(NULL == pBtn) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

		CString strCheck = (TRUE == pBtn->GetCheck()) ? _T("TRUE") : _T("FALSE");
		astrIsChecked.SetAt(nCnt, strCheck);

 		if(_T("TRUE") == strCheck) nCheckCnt++;
	
		CString strInputCheckAlias1;
		AfxExtractSubString(strInputCheckAlias1, astrControlAlias.GetAt(nCnt), 1, '|');

		CString strData;
		ids = pCM->GetControlMgrData(strInputCheckAlias1, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		if(FALSE == strData.IsEmpty()) nInputCnt++;
	}

	if(0 < nInputCnt) return 1;
	
	if(0 == nCheckCnt)
	{
		for(nCnt = 0; nCnt < CONTROL_ALIAS_CNT; nCnt++)
		{
			CString strBtnAlias;
			AfxExtractSubString(strBtnAlias, astrControlAlias.GetAt(nCnt), 0, '|');
			if(_T("대출자료상태_대출") == strBtnAlias)
			{
				astrIsChecked.SetAt(nCnt, _T("TRUE"));
			}
		}
	}

	for(nCnt = 0; nCnt < CONTROL_ALIAS_CNT; nCnt++)
	{
		if(_T("TRUE") == astrIsChecked.GetAt(nCnt))
		{
			CString strShowDateMessage;
			AfxExtractSubString(strShowDateMessage, astrControlAlias.GetAt(nCnt), 3, '|');
			
			CString strMessage;
			strMessage.Format(
				_T("기본 검색 조건이 입력되지 않아 검색 시간이 오래 걸릴 수 있습니다.\r\n")
				_T("%s을(를) 최근 한 달로 설정하시겠습니까?\r\n"), strShowDateMessage);	

			int nResult = AfxMessageBox(strMessage, MB_YESNOCANCEL);	
			if(IDCANCEL == nResult) { return IDCANCEL; }
			else if(IDYES == nResult)
			{
				COleDateTime Time = COleDateTime::GetCurrentTime();
				
				CString strInputCheckAlias2;
				AfxExtractSubString(strInputCheckAlias2, astrControlAlias.GetAt(nCnt), 2, '|');

				ids = pCM->SetControlMgrData( strInputCheckAlias2, Time.Format( _T("%Y/%m/%d") ) );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -14, strFuncName );
				
				COleDateTimeSpan MonthDay;
				MonthDay.SetDateTimeSpan(30, 0, 0, 0);
				Time.operator -= (MonthDay);
				
				CString strInputCheckAlias1;
				AfxExtractSubString(strInputCheckAlias1, astrControlAlias.GetAt(nCnt), 1, '|');

				ids = pCM->SetControlMgrData( strInputCheckAlias1, Time.Format( _T("%Y/%m/%d") ) );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -14, strFuncName );
			}
			break;
		}
	}

	return 0;
} 