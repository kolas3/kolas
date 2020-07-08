// CO_CENTER_USER_DETAILVIEWER2.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_DETAILVIEWER2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DETAILVIEWER2 dialog


CCO_CENTER_USER_DETAILVIEWER2::CCO_CENTER_USER_DETAILVIEWER2(CESL_Mgr* pParent , CString sParentCMAlias , CString sParentGridAlias )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_DETAILVIEWER2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sParentCMAlias = sParentCMAlias;
	m_sParentGridAlias = sParentGridAlias;

	m_pRS_SQL = NULL;
}

CCO_CENTER_USER_DETAILVIEWER2::~CCO_CENTER_USER_DETAILVIEWER2()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	delete m_pRS_SQL;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_DETAILVIEWER2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_DETAILVIEWER2, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_DETAILVIEWER2)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnDPREV, OnbtnDPREV)
	ON_BN_CLICKED(IDC_btnDNEXT, OnbtnDNEXT)
	ON_BN_CLICKED(IDC_btnDCLOSE, OnbtnDCLOSE)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTITLE, OnSelchangetabTITLE)
	ON_BN_CLICKED(IDC_radVIEWER_MODE, OnradVIEWERMODE)
	ON_BN_CLICKED(IDC_radVIEWER_MODE2, OnradVIEWERMODE2)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DETAILVIEWER2 message handlers

BOOL CCO_CENTER_USER_DETAILVIEWER2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("CO_CENTER_USER_DETIALVIEWER2"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("Error Init EslMgr[]"));
		return FALSE;
	}
	
	m_pRS_SQL = new CRS_SQL(this);

	// 2009.10.08 ADD BY KSJ : 대출화면주민번호표시유무
	ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strSecurityUserNoYN );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ChangeSecuriyUserNo") );

	m_strSecurityUserNoYN.TrimLeft();	m_strSecurityUserNoYN.TrimRight();
	m_strSecurityUserNoYN.MakeUpper();
	if( _T("Y") != m_strSecurityUserNoYN )
	{
		m_strSecurityUserNoYN = _T("N");
	}

	InitControl();

	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCO_CENTER_USER_DETAILVIEWER2::InitControl()
{
	((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->SetCheck(1);

	m_MasterUserInfoGrid.InitViewer( IDC_gridDCENTERUSER, this );
	m_MasterUserInfoGrid.SetViewerWidth( 35 , 60 );	


	GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_HIDE);

	InitTabControl();

	return 0;

}

INT CCO_CENTER_USER_DETAILVIEWER2::InitTabControl()
{
	INT ids;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	pTab->DeleteAllItems();
	pTab->InsertItem(  0 , _T("통합정보")  );	
	
	if( ((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->GetCheck() == 1 )
	{
		CESL_DataMgr* pLocalUserDM = FindDM(_T("DM_CO_CENTER_DETAIL_VIEWER2"));
		if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitTabControl") );

		CString sLibName;

		for( INT i = 0 ; i < pLocalUserDM->GetRowCount() ; i++ )
		{
			ids = pLocalUserDM->GetCellData( _T("LIB_CODE") , i , sLibName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitTabControl") );

			pTab->InsertItem( i+1 , sLibName );
		}
	}
	else
	{
		pTab->InsertItem( 1 , _T("소속 도서관별 대출자 정보") );	
	}

	pTab->SetCurSel(0);

	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
}



/// 부모 그리드 포인터
CESL_Grid* CCO_CENTER_USER_DETAILVIEWER2::GetMainGridPtr()
{
	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(m_sParentGridAlias));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}


/// 이전 다음 이동
INT CCO_CENTER_USER_DETAILVIEWER2::MoveIndex( INT nMode )
{
		
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
	
	SetPreNextButton();
	
	return 0;
	
}

/// 현재 인덱스와 처음 인덱스 , 마지막 인덱스 설정
INT CCO_CENTER_USER_DETAILVIEWER2::SetCurrentIndex()
{
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;
	
	// 이전 다음이
	// 'V'체크가 된 것이 있다면 'V'체크 된것만 보여주며
	// 'V'체크가 된 것이 없다면 모두 보여준다.
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->GetIdx();
	
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

	SetPreNextButton();
	
	return 0;
}


/// 이전,다음 버튼 활성화 비활성화
VOID CCO_CENTER_USER_DETAILVIEWER2::SetPreNextButton()
{
	
	GetDlgItem(IDC_btnDPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDNEXT)->EnableWindow(TRUE);
	
	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnDPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnDNEXT)->EnableWindow(FALSE);
	}
}


/// 정보를 화면에 보여줌
INT CCO_CENTER_USER_DETAILVIEWER2::DisplayWindow()
{
	INT ids;

	ids = SearchUserInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayWindow") );

	InitTabControl();

	ids = ShowUserInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayWindow") );

	return 0;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SetScreen()
{

	AfxGetApp()->DoWaitCursor(1);
	
	SetCurrentIndex();
	DisplayWindow();	

	AfxGetApp()->DoWaitCursor(-1);
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDPREV() 
{
	AfxGetApp()->DoWaitCursor(1);

	MoveIndex(0);	
	DisplayWindow();

	AfxGetApp()->DoWaitCursor(-1);
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDNEXT() 
{
	AfxGetApp()->DoWaitCursor(1);

	MoveIndex(1);	
	DisplayWindow();	

	AfxGetApp()->DoWaitCursor(-1);
	
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDCLOSE() 
{
	OnCancel();	
}


INT CCO_CENTER_USER_DETAILVIEWER2::SearchUserInfo()
{
	INT ids;

	CString sWhere;
	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchUserInfo") );
	if( ids > 0 ) return 1;

	CESL_DataMgr* pLibUserInfo = FindDM(_T("DM_CO_CENTER_DETAIL_VIEWER2"));
	if( pLibUserInfo == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUserInfo") );

//	m_pRS_SQL->SetFetchCount( 0, 100 );
//	ids = m_pRS_SQL->LOAN_Select( pLibUserInfo , sWhere , _T("") , 1 , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUserInfo") );	

	return 0;
}

INT CCO_CENTER_USER_DETAILVIEWER2::ShowUserInfo()
{
	INT ids;
	
	/// 정보 보여주기
	ids = ChangeView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowUserInfo") );


	/// 3. 아래 그리드에 리버스하기
	CESL_Grid* pParentGrid = GetMainGridPtr();
	if( pParentGrid == NULL ) return 0;

	pParentGrid->SetReverse(m_nCurrentIndex);

	return 0;
}


INT CCO_CENTER_USER_DETAILVIEWER2::GetWhere( CString &sWhere )
{
	CESL_DataMgr* pMasterUserDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pMasterUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhere") );

	CString sMasterUserKey;
	INT ids = pMasterUserDM->GetCellData( _T("레코드KEY") , m_nCurrentIndex , sMasterUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhere") );

	sWhere.Format(_T("MASTER_USER_KEY = %s") , sMasterUserKey );

	return 0;
}


INT CCO_CENTER_USER_DETAILVIEWER2::ShowMasterUserInfo()
{
	INT ids;

	const INT nFieldCnt = 34;
	CString sDMFieldAlias[nFieldCnt];

	sDMFieldAlias[  0 ] = _T("생성도서관이름");
	sDMFieldAlias[  1 ] = _T("생성일");
	sDMFieldAlias[  2 ] = _T("최종수정도서관이름");
	sDMFieldAlias[  3 ] = _T("최종수정일");
	sDMFieldAlias[  4 ] = _T("대출자번호");
	sDMFieldAlias[  5 ] = _T("대출자ID");
	sDMFieldAlias[  6 ] = _T("비밀번호");
	sDMFieldAlias[  7 ] = _T("이름");
	sDMFieldAlias[  8 ] = _T("이름색인");
	sDMFieldAlias[  9 ] = _T("생년월일");
	sDMFieldAlias[ 10 ] = _T("양음력");
	sDMFieldAlias[ 11 ] = _T("주민등록번호");
	sDMFieldAlias[ 12 ] = _T("주소_집");
	sDMFieldAlias[ 13 ] = _T("주소_근무지");
	sDMFieldAlias[ 14 ] = _T("우편번호_집");
	sDMFieldAlias[ 15 ] = _T("우편번호_근무지");
	sDMFieldAlias[ 16 ] = _T("전화번호_집");
	sDMFieldAlias[ 17 ] = _T("전화번호_근무지");
	sDMFieldAlias[ 18 ] = _T("내선번호");
	sDMFieldAlias[ 19 ] = _T("근무지부서명");
	sDMFieldAlias[ 20 ] = _T("근무지직급명");
	sDMFieldAlias[ 21 ] = _T("핸드폰");
	sDMFieldAlias[ 22 ] = _T("E-MAIL");
	sDMFieldAlias[ 23 ] = _T("등록일");
	sDMFieldAlias[ 24 ] = _T("제적일");
	sDMFieldAlias[ 25 ] = _T("학교");
	sDMFieldAlias[ 26 ] = _T("학년");
	sDMFieldAlias[ 27 ] = _T("반");
	sDMFieldAlias[ 28 ] = _T("교번");
	sDMFieldAlias[ 29 ] = _T("우편발송지역");
	sDMFieldAlias[ 30 ] = _T("비고");
	sDMFieldAlias[ 31 ] = _T("자료실코드");
	sDMFieldAlias[ 32 ] = _T("이용자사진이름");
	sDMFieldAlias[ 33 ] = _T("회원증분실여부");

	m_MasterUserInfoGrid.LIST.RemoveAll();
	m_MasterUserInfoGrid.Clear();
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );

	CString tmpTitle;
	CString sData;
	for( INT i = 0; i < nFieldCnt; i++ )
	{
		ids = pDM->GetCellData( sDMFieldAlias[i], m_nCurrentIndex, sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );
		
		switch( i )
		{
			case  6 :
			case  8 :
			case 18 :
			case 31 :
			case 32 :
				continue;
				break;
			case  4 :
				// 2009.10.08 ADD BY KSJ : 대출자번호가 주민번호일 경우 처리
				ChangeSecuriyUserNo( sData );
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], sData );
				break;
			case  5 :
				m_MasterUserInfoGrid.SetControlData( _T("사용자ID"), sData );
				break;
			case 10 :
				m_MasterUserInfoGrid.SetControlData( _T("양력(+), 음력(-)"), sData );
				break;
			case 11 :
				CLocCommonAPI::ChangeSecuriyCivilNo( this , sData , m_sIsUnityLoanService );
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ],  sData );
				break;
			case 12 :
			case 13 :
			case 14 :
			case 15 :
			case 16 :
			case 17 :
				tmpTitle = sDMFieldAlias[ i ];
				tmpTitle.Replace( _T("_"), _T(" ") );
				m_MasterUserInfoGrid.SetControlData( tmpTitle, sData );
				break;
			case 29 :
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], _T("H") == sData ? _T("자택") : _T("근무지") );
				break;
			default :
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], sData );
				break;
		}		
	}
	m_MasterUserInfoGrid.Display();
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnSelchangetabTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ChangeView();
	
	*pResult = 0;
}


/// 모드에 따라 다르다.
INT CCO_CENTER_USER_DETAILVIEWER2::ChangeView()
{

	INT ids , nTabIndex;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	nTabIndex = pTab->GetCurSel();

	GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_HIDE);
	/// 도서관별 정보보기
	if( ((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->GetCheck() == 1 )
	{
		if( nTabIndex > 0 )
		{
			ids = ShowLibUserInfo(nTabIndex-1);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ChangeView") );		
		}
		else
		{
			ShowMasterUserInfo();
		}
	}
	/// 한번에 모두 보기
	else
	{
		if( nTabIndex > 0 )
		{
			GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_SHOW);

			// 2009.10.08 UPDATE BY KSJ : 주민번호 숨기도록 수정
			CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_DETAIL_VIEWER2") );
			if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ChangeView") );	
			
			ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1  , _T("CIVIL_NO") , m_sIsUnityLoanService );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );
			
			CString strUserNo;
			INT i , nRowCnt = pDM->GetRowCount();
			for( i=0 ; i<nRowCnt ; i++ )
			{
				ids = pDM->GetCellData( _T("USER_NO") , i , strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );

				ChangeSecuriyUserNo(strUserNo);

				ids = pDM->SetCellData( _T("USER_NO") , strUserNo , i );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );
			}
		}
		else
		{
			ShowMasterUserInfo();
		}

		ids = AllControlDisplay(_T("CM_CO_CENTER_DETAIL_VIEWER2"));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeView") );
	}
	
	return 0;
}

/// 
INT CCO_CENTER_USER_DETAILVIEWER2::ShowLibUserInfo(INT nIndex)
{
	INT ids;
	
	const INT nFieldCnt = 44;
	CString sDMFieldAlias[nFieldCnt][2] = 
	{
		{ _T("LIB_CODE                 "),  _T("소속 도서관명                                    ")},		//0
		{ _T("USER_NO                  "),  _T("대출자번호                                       ")},		//1
		{ _T("USER_ID                  "),  _T("사용자ID                                         ")},		//2
		{ _T("PASSWORD                 "),  _T("비밀번호                                         ")},		//3
		{ _T("NAME                     "),  _T("이름                                             ")},		//4
		{ _T("BIRTHDAY                 "),  _T("생년월일                                         ")},		//5
		{ _T("BIRTHDAY_TYPE            "),  _T("생일 구분                              ")},					//6
		{ _T("CIVIL_NO                 "),  _T("주민등록번호                                     ")},
		{ _T("H_ADDR1             "),  _T("주소(집)                                         ")},
		{ _T("W_ADDR1             "),  _T("주소(근무지)                                     ")},
		{ _T("H_ZIPCODE            "),  _T("우편번호(집)                                     ")},		//10
		{ _T("W_ZIPCODE            "),  _T("우편번호(근무지)                                 ")},
		{ _T("H_PHONE               "),  _T("전화번호(집)                                     ")},
		{ _T("W_PHONE               "),  _T("전화번호(근무지)                                 ")},
		{ _T("EXCHANGE_NO              "),  _T("내선번호                                         ")},
		{ _T("DEPARTMENT_NAME          "),  _T("근무지 부서명                                    ")},		//15
		{ _T("CLASS_NAME               "),  _T("근무지 직급명                                    ")},
		{ _T("HANDPHONE                "),  _T("핸드폰                                           ")},
		{ _T("IDX_HANDPHONE            "),  _T("핸드폰 색인                                      ")},
		{ _T("E_MAIL                   "),  _T("E-MAIL                                           ")},
		{ _T("REG_DATE                 "),  _T("등록일                                           ")},		//20
		{ _T("REMOVE_DATE              "),  _T("제적일                                           ")},
		{ _T("USER_CLASS           "),  _T("회원 상태  ")},
		{ _T("INFERIOR_REASON          "),  _T("대출정지 사유                                     ")},
		{ _T("MEMBER_CLASS             "),  _T("회원구분                     ")},
		{ _T("SCHOOL                   "),  _T("학교                                             ")},		//25
		{ _T("GRADE                    "),  _T("학년                                             ")},
		{ _T("CLASS                    "),  _T("반                                               ")},
		{ _T("CLASS_NO                 "),  _T("교번                                             ")},
		{ _T("USER_POSITION_CODE  "),  _T("대출소속정보코드                                 ")},
		{ _T("USER_CLASS_CODE     "),  _T("대출직급정보코드                                 ")},		//30
		{ _T("MAIL_SEND_AREA           "),  _T("우편발송지역                     ")},
		{ _T("LOAN_COUNT               "),  _T("현재 총대출책수                                  ")},
		{ _T("RESERVATION_COUNT        "),  _T("현재 총예약책수                                  ")},
		{ _T("DELAY_COUNT              "),  _T("연체횟수                                         ")},
		{ _T("LOAN_STOP_DATE           "),  _T("대출정지일                                       ")},		//35
		{ _T("LAST_LOAN_DATE           "),  _T("최종대출일                                       ")},
		{ _T("LAST_MODIFY_DATE         "),  _T("최종정보수정일                                   ")},
		{ _T("REMARK                   "),  _T("비고                                             ")},
		{ _T("REISSUE_CNT              "),  _T("재발급횟수                                       ")},
		{ _T("LOAN_STOP_SET_DATE       "),  _T("대출정지 부여일                                  ")},		//40
		{ _T("SHELF_LOC_CODE           "),  _T("자료실코드                                       ")},
		{ _T("LOST_USER_CARD           "),  _T("회원증분실여부 ('Y','N')                         ")},
		{ _T("DELAYDAY_COUNT           "),  _T("누적연체일수                                     ")}
	};
	
	
	
	m_MasterUserInfoGrid.LIST.RemoveAll();
	m_MasterUserInfoGrid.Clear();
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_DETAIL_VIEWER2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );	
	
	CString tmpTitle;
	CString sData;
	for( INT i = 0; i < nFieldCnt; i++ )
	{
		sDMFieldAlias[i][0].TrimLeft(); sDMFieldAlias[i][0].TrimRight(); 
		sDMFieldAlias[i][1].TrimLeft(); sDMFieldAlias[i][1].TrimRight(); 
		ids = pDM->GetCellData( sDMFieldAlias[i][0], nIndex, sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );
		
		switch( i )
		{
		case 3 :
		case 18 :
			break;
		case 6 :
			m_MasterUserInfoGrid.SetControlData( _T("양력(+), 음력(-)"), sData );
			break;
		case 7 :
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sData , m_sIsUnityLoanService );
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1],  sData );
			break;
		case 22 :
			if( sData.Compare(_T("0"))==0 ) sData = _T("정상회원");
			else if( sData.Compare(_T("1"))==0 ) sData = _T("대출정지회원");
			else if( sData.Compare(_T("2"))==0 ) sData = _T("제적회원");
			// 2009.05.18 ADD BY CJY : 탈퇴회원 추가
			else if( sData.Compare(_T("3"))==0 ) sData = _T("탈퇴회원");
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		case 24 :
			if( sData.Compare(_T("0"))==0 ) sData = _T("정회원");
			else sData = _T("비회원");
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		case 31 :
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], _T("H") == sData ? _T("자택") : _T("근무지") );
			break;
		default :
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		}		
	}
	m_MasterUserInfoGrid.Display();
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnradVIEWERMODE() 
{
	InitTabControl();
	ChangeView();
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnradVIEWERMODE2() 
{
	InitTabControl();
	ChangeView();
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbPRINT() 
{
	EFS_BEGIN

	INT ids;
	CString sPrintAlias = _T("공유대출자정보");
	CString	sPrintDM = _T("DM_CO_CENTER_DETAIL_VIEWER2");
	CESL_DataMgr* pPrintDM = FindDM( sPrintDM );		

	// 프린트 객체 초기화
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	// KOL.UDF.022
	//if (!pSIReportManager) {
	if (&pSIReportManager == NULL) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return ;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	
	ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();

	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	for( INT i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		ids = SetUserDetailInfoCondition( pSIReportManager , pPrintDM , i );
		if( ids < 0 ) return;
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// 워크폼 Manual 반복 모드
		if( ids < 0 ) return;
	}
	
	
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return;

	EFS_END

	return;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SetUserDetailInfoCondition(  CSIReportManager* pSIReportManager , CESL_DataMgr* pPrintDM , INT nIndex )
{
	EFS_BEGIN
	
	const INT nCnt = 26;

	CString sInputTmpAlias[nCnt] =
	{
		_T("이용자번호"),	_T("가족ID"),		_T("이름"),			_T("주민등록번호"),		_T("생일") , 
		_T("휴대폰"),		_T("E_MAIL"),		_T("소속"),			_T("직급"),				_T("자료실") , 
		_T("자택_우편번호"),_T("자택_주소"),	_T("자택_전화"),	_T("근무지_우편번호"),	_T("근무지_전화") , 
		_T("근무지_주소"),	_T("학교명"),		_T("학년"),			_T("반"),				_T("비고"), 
	   _T("현재총대출책수"),_T("예약책수"),		_T("연체횟수"),		_T("대출정지일"),	_T("최종대출일"),
		_T("누적연체일")
	};

	const INT nGetFieldCnt = 26;
	CString sValue[nGetFieldCnt];
	CString sGetDMFieldAlias[nGetFieldCnt] =
	{
		_T("USER_NO"),		_T("FID_KEY"),			_T("NAME"),			_T("CIVIL_NO"),			_T("BIRTHDAY") , 
		_T("HANDPHONE"),	_T("E_MAIL"),	_T("USER_POSITION_CODE"),	_T("USER_CLASS_CODE"),	_T("SHELF_LOC_CODE") ,
		_T("H_ZIPCODE"),	_T("H_ADDR1"),			_T("H_PHONE"),		_T("W_ZIPCODE"),		_T("W_PHONE") ,
		_T("W_ADDR1"),		_T("SCHOOL"),			_T("GRADE"),		_T("CLASS"),			_T("REMARK"), 
		_T("LOAN_COUNT"),	_T("RESERVATION_COUNT"),_T("DELAY_COUNT"),	_T("LOAN_STOP_DATE"),	_T("LAST_LOAN_DATE"), 
		_T("DELAYDAY_COUNT") 
	};

	CString	sPrintDM = _T("DM_CO_CENTER_DETAIL_VIEWER2");
	CESL_DataMgr* pPrintDM = FindDM( sPrintDM );		

	pPrintDM->GetCellData( sGetDMFieldAlias , nGetFieldCnt , sValue , nIndex );	

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sValue[i].TrimLeft();

		// 2009.05.18 UPDATE BY CJY : 주민등록번호 뒷자리 숨김
		if( _T("CIVIL_NO") == sGetDMFieldAlias[i] )
		{
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sValue[i] , m_sIsUnityLoanService );
		}

		// 2009.10.09 ADD BY KSJ : 대출자번호가 주민등록번호일 때 처리
		if( _T("USER_NO") == sGetDMFieldAlias[i] )
		{
			ChangeSecuriyUserNo( sValue[i] );
		}

		sValue[i] = _T(" ") + sValue[i];
		pSIReportManager->SetTempVariable( sInputTmpAlias[i] , sValue[i] );
	}
	

	return 0;


	EFS_END

	return -1;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

HBRUSH CCO_CENTER_USER_DETAILVIEWER2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*
@Create
 - 2009.10.08 BY KSJ
@Description
 - 대출자번호가 주민번호일 경우, 옵션값에 따라 보안처리한다.
@Parameter
 - CString &strUserNo : 대출자번호
@Return Value
 - 0 : 성공
@Mini Spec
 1. 
*/
INT CCO_CENTER_USER_DETAILVIEWER2::ChangeSecuriyUserNo(CString &strUserNo)
{
EFS_BEGIN

	if( _T("Y") != m_strSecurityUserNoYN )
	{
		// 2010.09.03 ADD BY KSJ : 통합도서서비스 사용에 따른 처리
//DEL 		CString strBuf = strUserNo;
//DEL 		if( 13 <= strUserNo.GetLength() )
//DEL 		{
//DEL 			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 		}
//DEL 
//DEL 		INT ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ChangeSecuriyUserNo") );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			// 2009.12.14 ADD BY CJY : 주민번호 표시방법 변경
//DEL 			strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeSecuriyUserNo") );
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
	}

	return 0;

EFS_END
	return -1;
}
