// MngLocMgr02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr02Dlg.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr02Dlg dialog


CMngLocMgr02Dlg::CMngLocMgr02Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr02Dlg)
	m_nRETURN_DELAY_USE = -1;
	m_nRETURN_DELAY_AUTO = -1;
	m_nRESERVE_AUTO_CANCEL = -1;
	m_nRESERVE_EXPIRE_AUTO = -1;
	m_nSEND_CANCEL_MAIL = -1;
	m_nAutoReserveComplete = -1;
	m_nAddHolydayReservation = -1;
	m_nRETURN_SYSDATE_DELAY_USE = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	// 2005.05.26
	m_strUserID=pParent->GetUserID();
	m_strLibCode=_T("");

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl1 = NULL;
	m_pEditCtrl2 = NULL;
}
CMngLocMgr02Dlg::~CMngLocMgr02Dlg()
{	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl1)
	{
		delete m_pEditCtrl1;
		m_pEditCtrl1 = NULL;
	}
	if(m_pEditCtrl2)
	{
		delete m_pEditCtrl2;
		m_pEditCtrl2 = NULL;
	}

	
}

VOID CMngLocMgr02Dlg::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr02Dlg)
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_USE_Y2, m_nRETURN_DELAY_USE);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y2, m_nRETURN_DELAY_AUTO);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y, m_nRESERVE_AUTO_CANCEL);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_EXPIRE_ATUO_Y, m_nRESERVE_EXPIRE_AUTO);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y3, m_nSEND_CANCEL_MAIL);
	DDX_Radio(pDX, IDC_radSETTING_RESERVE_ATUO_CANCEL_Y4, m_nAutoReserveComplete);
	DDX_Radio(pDX, IDC_radSETTING_ADD_HOLYDAY_RESERVATION1, m_nAddHolydayReservation);
	DDX_Radio(pDX, IDC_radSETTING_SYSDATE_RESERVE_USE_Y, m_nRETURN_SYSDATE_DELAY_USE);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CMngLocMgr02Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr02Dlg)
	ON_CBN_SELCHANGE(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2, OnSelchangecmbSETTINGDELAYORRESERVERETURN2)
	ON_BN_CLICKED(IDC_radSETTING_RESERVE_USE_N2, OnradSETTINGRESERVEUSEN2)
	ON_BN_CLICKED(IDC_radSETTING_RESERVE_USE_Y2, OnradSETTINGRESERVEUSEY2)
	ON_WM_CTLCOLOR()  
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtDELAYRETURN_CNT, OnKillfocusedtDELAYRETURNCNT)
	ON_EN_KILLFOCUS(IDC_edtLIMIT, OnKillfocusedtLIMIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr02Dlg message handlers

BOOL CMngLocMgr02Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_대출환경설정_반납연기예약환경")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_대출환경설정_반납연기예약환경"));
	m_pDM = FindDM(_T("DM_대출환경설정_반납연기예약환경"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 반납연기횟수
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtDELAYRETURN_CNT);
	m_pEditCtrl1->ShowWindow(SW_HIDE);
	// 예약허용인원
	m_pEditCtrl2 = new CEdit();
	m_pEditCtrl2->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtLIMIT);
	m_pEditCtrl2->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMngLocMgr02Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMngLocMgr02Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 14;
	// 구분
	CString sSection[nCnt] = 
			{	_T("반납연기관리"), _T("반납연기관리"), _T("반납연기관리"), 
				_T("반납연기관리"), _T("반납연기관리"), _T("반납연기관리"),	
				_T("예약관리"), _T("예약관리"), _T("예약관리"), 
				_T("예약관리"),	_T("예약관리"), _T("예약관리"), 
				_T("예약관리"), _T("예약관리") };
	// 기능
	CString sAbility[nCnt] = 
			{	_T("반납연기사용여부"),		_T("당일대출자료 반납연기허용"), 
				_T("반납연기일 자동부여"),	_T("반납연기시 예약우선순위 결정"), 
				_T("반납연기 가능횟수"),	_T("반납연기일 적용 기준일"), 
				_T("예약자동취소"),			_T("예약만기일 자동부여"), 
				_T("예약만기일부여시 SMS발송여부"),
				_T("예약기능 사용여부"),	_T("예약만기일 휴관일 포함유무"), 
				_T("예약허용인원"),			_T("예약취소 안내메일 발송여부"), 
				_T("한줄서기예약기능")};
	// 설명
	CString sExplain[nCnt] = 
			{	_T("대출/반납 메뉴에서 반납연기 기능의 사용여부 결정"), 
				_T("당일 대출한 자료의 반납연기 허용여부"), 
				_T("반납연기시 직급별이나 등록구분별로 설정되어 있는 기본 반납연기일을 적용"),
				_T("반납연기 대상자료가 이미 예약된 자료이고 예약우선이라면 반납연기 기능 미적용"),
				_T("반납연기 대상 자료의 반납연기 가능횟수"), 
				_T("반납연기일이 현재일에 합산되거나 반납예정일에 합산되어 반납예정일을 결정"),
				_T("대출/반납 메뉴 사용시 예약만기일이 지난 예약자료에 대해 자동으로 예약취소 여부결정"), 
				_T("예약만기일 부여시 예약만기일을 자동으로 적용할지 사용자가 값을 입력할지 결정"),
				_T("예약만기일이 부여되면 SMS를 자동으로 발송할지 결정"),
				_T("대출중인 자료에만 예약이 가능하게 하거나 비치, 대출중인 자료에 예약이 가능하게한다"),
				_T("예약만기일에 휴관일을 포함할지 결정 (예약만기일+휴관일=새로운예약만기일)"), 
				_T("한 자료에 대해 예약가능한 이용자 수"), 
				_T("대출/반납 화면에서 예약된 자료를 취소할 경우 예약취소 안내메일발송 여부결정"), 
				_T("예약된 자료를 반납시 자동으로 예약자료지정, 예약만기일 부여, 메일발송여부 결정")	};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("반납연기기능사용여부"),	_T("당일반납연기허용유무"), 
				_T("반납연기자동수동부여"),	_T("반납연기예약우선순위"), 
				_T("반납연기가능횟수"),		_T("반납연기부여기준"), 
				_T("예약자동취소여부"), 	_T("예약만기일자동부여여부"), 
				_T("예약만기일부여시SMS발송여부"),
				_T("예약기능사용여부"), 	_T("예약만기휴관일포함유무"), 
				_T("예약허용인원"), 		_T("예약취소시메일발송여부"), 
				_T("자동예약자료지정여부") };

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		// 한줄서기예약기능은 CO_UTIL_LOCK_TBL에 관리값에 따라 보여준다.
		if( sAbility[i] == _T("한줄서기예약기능") )
		{
			CString sData;
			CString sQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'");
			ids = m_pDM->GetOneValueQuery( sQuery, sData );
			if ( (ids<0) && (-4007!=ids) ) 
			{
				ESLAfxMessageBox(_T("관리설정값을 가져오는데 실패하였습니다."));
				return -1;
			}
			if( sData != _T("Y") ) break;
		}
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("구분"), sSection[i], i ); 
		m_pDM->SetCellData( _T("기능"), sAbility[i], i );
		m_pDM->SetCellData( _T("설명"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("열람"), _T("공통"), sSection[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("관리설정값을 가져오는데 실패하였습니다."));
			return -1;
		}
		
		// DM저장
		m_pDM->SetCellData( _T("설정값"), sGetData, i );
	}
	m_pCM->AllControlDisplay();

	// 그리드 셋팅 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SettingGrid02(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	/*
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// 예약만기일에 휴관일 포함유무
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약만기휴관일포함유무"),strData);
	if(_T("Y") == strData) m_nAddHolydayReservation = 0;
	else  m_nAddHolydayReservation = 1;
	strData.Empty();
	if(ids<0) return -7;

	//세번째 
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기기능사용여부"),strData);
	if(_T("Y") == strData) m_nRETURN_DELAY_USE = 0;
	else  m_nRETURN_DELAY_USE = 1;
	strData.Empty();
	if(ids<0) return -7;

	//++2008.06.06 ADD BY PWR {{++
	// 당일반납연기허용유무 관리값 추가
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("당일반납연기허용유무"),strData);
	if(_T("Y") == strData) m_nRETURN_SYSDATE_DELAY_USE = 0;
	else  m_nRETURN_SYSDATE_DELAY_USE = 1;
	strData.Empty();
	if(ids<0) return -7;
	//--2008.06.06 ADD BY PWR --}}

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기자동수동부여"),strData);
	if(_T("Y") == strData) m_nRETURN_DELAY_AUTO = 0;
	else  m_nRETURN_DELAY_AUTO = 1;
	strData.Empty();
	if(ids<0) return -8;

	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기예약우선순위"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기가능횟수"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -10;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbRETURN_PLAN_DAY_DELAY_OPTION);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기부여기준"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	//네번째
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약자동취소여부"),strData);
	if(_T("Y") == strData) m_nRESERVE_AUTO_CANCEL = 0;
	else  m_nRESERVE_AUTO_CANCEL = 1;
	strData.Empty();
	if(ids<0) return -11;
		
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약만기일자동부여여부"),strData);
	if(_T("Y") == strData) m_nRESERVE_EXPIRE_AUTO = 0;
	else  m_nRESERVE_EXPIRE_AUTO = 1;
	strData.Empty();
	if(ids<0) return -8;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약기능사용여부"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else if(_T("1") == strData) pCombo->SetCurSel(1);
	else pCombo->SetCurSel(2);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -12;

	pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT2);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약허용인원"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	if(ids<0) return -13;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약취소시메일발송여부"),strData);
	if(_T("Y") == strData) m_nSEND_CANCEL_MAIL = 0;
	else  m_nSEND_CANCEL_MAIL = 1;
	strData.Empty();
	if(ids<0) return -14;

	//(A_2004-1115_HAN)한줄서기 예약 사용여부에 따라 설정 여부 등록
	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );
	CString sQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'");
	ids = pDM->GetOneValueQuery( sQuery, strData );
	if ( (ids<0) && (-4007!=ids) ) return -15;
	else if ( _T("Y")==strData )
	{
		ids = this->GetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("자동예약자료지정여부"),strData);
		if ( _T("Y")==strData ) m_nAutoReserveComplete = 0;
		else if ( _T("N")==strData ) m_nAutoReserveComplete = 1;
	}
	else
	{
		GetDlgItem(IDC_lblY6)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_Y4)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblN6)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_N4)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblRESERVE_DESC3)->ShowWindow(FALSE);

		CRect rect1, rect2 , rect3, rect4, rect5;

		CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_RE_BORDER);
		CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG4);
		CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG5);
		CStatic* pStatic4 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG6);
		CStatic* pStatic5 = (CStatic*)GetDlgItem(IDC_lblRESERVE_FUNC6);

		pStatic1->GetWindowRect( &rect1 );
		pStatic2->GetWindowRect( &rect2 );
		pStatic3->GetWindowRect( &rect3 );
		pStatic4->GetWindowRect( &rect4 );
		pStatic5->GetWindowRect( &rect5 );

		ScreenToClient( rect1 );
		ScreenToClient( rect2 );
		ScreenToClient( rect3 );
		ScreenToClient( rect4 );
		ScreenToClient( rect5 );

		INT nBorderHeight = rect1.Height( ) - (rect1.bottom - rect5.top);

		//어쩔 수 없이 그대로 맞춰줬다.
		pStatic1->MoveWindow( 10, 0, rect1.Width( ), nBorderHeight );
		pStatic2->MoveWindow( 95, 8, rect2.Width( ), nBorderHeight-8 );
		pStatic3->MoveWindow( 95+245, 8, rect3.Width( ), nBorderHeight-8 );
		pStatic4->MoveWindow( 340+165, 8, rect4.Width( ), nBorderHeight-8 );
		GetDlgItem(IDC_lblRESERVE_FUNC6)->ShowWindow(FALSE);
	}

	
	UpdateData(FALSE);

	OnSelchangecmbSETTINGDELAYORRESERVERETURN2();

	SettingDelayFunc();
	*/

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr02Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 14;

	// 구분
	CString sSection[nCnt] = 
			{	_T("반납연기관리"), _T("반납연기관리"), _T("반납연기관리"), 
				_T("반납연기관리"), _T("반납연기관리"), _T("반납연기관리"),	
				_T("예약관리"), _T("예약관리"), _T("예약관리"), 
				_T("예약관리"),	_T("예약관리"), _T("예약관리"), 
				_T("예약관리"), _T("예약관리") };
	// 설정값
	CString sManage[nCnt] = 
			{	_T("반납연기기능사용여부"),	_T("당일반납연기허용유무"), 
				_T("반납연기자동수동부여"),	_T("반납연기예약우선순위"), 
				_T("반납연기가능횟수"),		_T("반납연기부여기준"), 
				_T("예약자동취소여부"), 	_T("예약만기일자동부여여부"), 
				_T("예약만기일부여시SMS발송여부"),
				_T("예약기능사용여부"), 	_T("예약만기휴관일포함유무"), 
				_T("예약허용인원"), 		_T("예약취소시메일발송여부"), 
				_T("자동예약자료지정여부") };

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("설정값"), i, sCode );
		
		ids = this->SetManageValue( _T("열람"), _T("공통"), sSection[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("관리설정값을 저장하는데 실패하였습니다."));
			return -1;
		}
	}

	// 공통 반납연기 정보 변경시 센터 도서관에 알리기 
	const INT nDelayManageCnt = 5;
	CString sValueNames[nDelayManageCnt] =
	{
		_T("반납연기기능사용여부"), _T("반납연기자동수동부여"), _T("반납연기예약우선순위"), 
		_T("반납연기가능횟수"),		_T("반납연기부여기준") 
	};

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere;
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// 						_T("WHERE ID = '%s')"), m_strUserID );
// 		pSrcDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pInfo->LIB_CODE;
	}
	
	INT nIndex;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("64");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ESL_MANAGE_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	
	CString sRecKey;
	for( i = 0 ; i < nDelayManageCnt ; i++ )
	{
		ids = CLocCommonAPI::FindManageDMIndex( this, pSrcDM, _T("열람"), _T("공통"), _T("반납연기관리"), sValueNames[i], nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
		if( ids > 0 ) return 0;

		pSrcDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	return 0;
	/*
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids = 0;
	
	const INT nDelayManageCnt = 5;
	CString sValueNames[nDelayManageCnt] =
	{
		_T("반납연기기능사용여부") , _T("반납연기자동수동부여") , _T("반납연기예약우선순위") , _T("반납연기가능횟수") , _T("반납연기부여기준") 
	};
	CString sDelayManageValue[nDelayManageCnt];
	
	//세번째 
	if(0 == m_nRETURN_DELAY_USE) strData = _T("Y");
	else  strData = _T("N");
	sDelayManageValue[0] = strData;
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기기능사용여부"),strData);
	strData.Empty();
	if(ids<0) return -7;

	if(0 == m_nRETURN_DELAY_AUTO) strData = _T("Y");
	else  strData = _T("N");
	sDelayManageValue[1] = strData;
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기자동수동부여"),strData);
	strData.Empty();
	if(ids<0) return -8;
		
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	sDelayManageValue[2] = strData;
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기예약우선순위"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT);
	pEdit->GetWindowText(strData);
	sDelayManageValue[3] = strData;	
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기가능횟수"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -10;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbRETURN_PLAN_DAY_DELAY_OPTION);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	sDelayManageValue[4] = strData;	
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("반납연기부여기준"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -9;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// 공통 반납연기 정보 변경시 센터 도서관에 알리기 
	// 2003년 11월 8일 작업
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	// 2005.05.26 ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
						_T("WHERE ID = '%s')"), m_strUserID );
		pSrcDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	
	INT nIndex;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("64");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ESL_MANAGE_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	
	CString sRecKey;
	for( INT i = 0 ; i < nDelayManageCnt ; i++ )
	{
		ids = CLocCommonAPI::FindManageDMIndex( this , pSrcDM , _T("열람") , _T("공통") , _T("반납연기관리") , sValueNames[i] , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
		if( ids > 0 ) return 0;

		pSrcDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------

	//네번째
	if(0 == m_nRESERVE_AUTO_CANCEL) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약자동취소여부"),strData);
	strData.Empty();
	if(ids<0) return -11;
	
	if(0 == m_nRESERVE_EXPIRE_AUTO) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약만기일자동부여여부"),strData);
	strData.Empty();
	if(ids<0) return -12;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약기능사용여부"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -13;
	
	pEdit = (CEdit*)GetDlgItem(IDC_edtDELAYRETURNCNT2);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약허용인원"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -14;

	if(0 == m_nSEND_CANCEL_MAIL) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약취소시메일발송여부"),strData);
	strData.Empty();
	if(ids<0) return -15;

	// 2007.07.13 add by pdw
	// 예약만기일에 휴관일 포함 유무
	if(0 == m_nAddHolydayReservation) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("예약만기휴관일포함유무"),strData);
	strData.Empty();
	if(ids<0) return -15;

	//++2008.06.06 ADD BY PWR {{++
	// 당일반납연기허용유무 관리값 추가
	if(0 == m_nRETURN_SYSDATE_DELAY_USE) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납연기관리"),_T("당일반납연기허용유무"),strData);
	strData.Empty();
	if(ids<0) return -15;
	//--2008.06.06 ADD BY PWR --}}

	//CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	//if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );
	//CString sQuery = _T("SELECT FROM WHERE");
	//ids = pDM->GetOneValueQuery( sQuery, strData );
	//if ( ids<0 ) return -15;
	strData = _T("Y");
	if ( _T("Y")==strData )
	{
		if(0 == m_nAutoReserveComplete ) strData = _T("Y");
		else  strData = _T("N");
		ids = this->SetManageValue(_T("열람"),_T("공통"),_T("예약관리"),_T("자동예약자료지정여부"),strData);
		strData.Empty();
		if(ids<0) return -16;
	}
	*/

	return 0;

EFS_END
return -1;
}

void CMngLocMgr02Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_02);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

BEGIN_EVENTSINK_MAP(CMngLocMgr02Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr02Dlg)
	ON_EVENT(CMngLocMgr02Dlg, IDC_gridMNG_LOC_MGR_02, -600 /* Click */, OnClickgridMNGLOCMGR02, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr02Dlg::OnClickgridMNGLOCMGR02() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();	
}

// ** 콤보박스 생성 및 값 변경
VOID CMngLocMgr02Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("설정값"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("기능"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	/// 반납연기 사용여부에 따른 표시
	// 1:반납연기사용여부가 사용안함(N)이면 
	// 3:당일대출자료반납연기허용,4:반납연기시 예약우선순위결정,5:반납연기가능횟수는 비활성화 되어야 한다.
	// 2009.05.20 UPDATE BY PDJ : 2:당일대출자료 반납연기허용여부, 6:반납연기일적용기준일 추가함
	//if( nRow == 3 || nRow == 4 || nRow == 5 )	
	if( nRow == 2 || nRow == 3 || nRow == 4 || nRow == 5 || nRow == 6)
	{
		CString sDelay = m_pDM->GetCellData( _T("설정값"), 0 );
		if( sDelay != _T("Y") ) return;
	}
	/// 예약기능 사용여부에 따른 표시
	// 9:예약기능사용여부가 사용안함(0)이면
	// 7:예약자동취소는 사용안함(N),비활성화로 12:예약허용인원은 비활성화한다
	if( nRow == 7 || nRow == 12 )
	{
		CString sDelay = m_pDM->GetCellData( _T("설정값"), 8 );
		if( sDelay == _T("0") ) return;
	}

	// 1:반납연기사용여부, 2:당일대출자료 반납연기허용, 3:반납연기일 자동부여, 7:예약자동취소, 
	// 8:예약만기일 자동부여, 9:예약만기일부여시SMS발송여부, 11:예약만기일 휴관일 포함유무, 
	// 13:예약취소 안내메일 발송여부, 14:한줄서기예약기능
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 7 || 
		nRow == 8 || nRow == 9 || nRow == 11 || nRow == 13 || nRow == 14)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
		ArrCmbData.Add(_T("사용안함"));
	}
	// 4:반납연기시 예약우선순위 결정
	else if( nRow == 4 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("반납연기우선"));
		ArrCmbData.Add(_T("예약우선"));
	}
	// 6:반납연기일 적용 기준일
	else if( nRow == 6 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("반납예정일"));
		ArrCmbData.Add(_T("현재일"));
	}
	// 10:예약기능 사용여부
	else if( nRow == 10 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("대출중인 자료만 예약가능"));
		ArrCmbData.Add(_T("모든자료 예약가능"));
	}
	// 5:반납연기 가능횟수
	else if( nRow == 5 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	// 12:예약허용인원
	else if( nRow == 12 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl2);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("적용함"));
		ArrCmbData.Add(_T("적용안함"));
	}
	
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(2);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM, m_pGrid);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);
	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMngLocMgr02Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// 텍스트 설정
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("회"), _T("") );
	sValue.Replace( _T("명"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr02Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr02Dlg::ApplyDataChange(LPCTSTR lpszData)
{
EFS_BEGIN

	if (lpszData == NULL) return -1;
	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();

	// 그리드에 변경을 적용시킨다.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(nRow, nCol, lpszData);

	// 데이터 메니저에 변경을 적용시킨다.
	m_pDM->SetCellData( _T("설정값"), lpszData, nRow - 1 );
	m_pGrid->Display();

	// 그리드 셋팅 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SettingGrid02(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	return 0;

EFS_END
return -1;
}

void CMngLocMgr02Dlg::OnKillfocusedtDELAYRETURNCNT() 
{
	SetFocus();
	if (m_pEditCtrl1 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl1->GetSafeHwnd())) return ;

	m_pEditCtrl1->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl1->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr02Dlg::OnKillfocusedtLIMIT() 
{
	SetFocus();
	if (m_pEditCtrl2 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl2->GetSafeHwnd())) return ;

	m_pEditCtrl2->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl2->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

INT CMngLocMgr02Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("설정값"))
	{
		CString sAbility;
		m_pDM->GetCellData( _T("기능"), nRow, sAbility );

		// 해당기능의 설정값의 코드를 설명으로 변경한다
		CChangeValue *pMng = new CChangeValue();
		str = pMng->CodeToDesc( sAbility, str );

		delete pMng;
		pMng = NULL;
	}
	
	return 0;
}

// 예약 기능이 사용할 경우
VOID CMngLocMgr02Dlg::OnSelchangecmbSETTINGDELAYORRESERVERETURN2() 
{
EFS_BEGIN

	INT nCur;
	
	nCur = ((CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN2))->GetCurSel();

	if( nCur == 0 )
	{
		((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_ATUO_CANCEL_N))->SetCheck(1);
		GetDlgItem(IDC_edtDELAYRETURNCNT2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_edtDELAYRETURNCNT2)->EnableWindow(TRUE);
	}

EFS_END
}

VOID CMngLocMgr02Dlg::OnradSETTINGRESERVEUSEN2() 
{
EFS_BEGIN

	SettingDelayFunc();

EFS_END
}

VOID CMngLocMgr02Dlg::OnradSETTINGRESERVEUSEY2() 
{
EFS_BEGIN

	SettingDelayFunc();

EFS_END
}


VOID CMngLocMgr02Dlg::SettingDelayFunc()
{
EFS_BEGIN


	INT nCheck = ((CButton*)GetDlgItem(IDC_radSETTING_RESERVE_USE_Y2))->GetCheck();

	const INT nCnt = 6;
	INT nIDC[nCnt] =
	{
		IDC_radSETTING_RESERVE_ATUO_CANCEL_Y2 , IDC_radSETTING_RESERVE_ATUO_CANCEL_N2 ,
		IDC_cmbSETTING_DELAY_OR_RESERVE_RETURN , IDC_edtDELAYRETURNCNT,
		IDC_radSETTING_SYSDATE_RESERVE_USE_Y, IDC_radSETTING_SYSDATE_RESERVE_USE_N
	};

	if( nCheck == 1 )
	{
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			GetDlgItem( nIDC[i] )->EnableWindow( TRUE );
		}	
	}
	else
	{
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			GetDlgItem( nIDC[i] )->EnableWindow( FALSE );
			((CComboBox*)GetDlgItem( nIDC[2] ))->SetCurSel(1);
		}	
	}

EFS_END
}




/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr02Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	ids = CLocCommonAPI::GetManageValue( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr02Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	INT nIndex;

	ids = CLocCommonAPI::FindManageDMIndex( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
	if( ids > 0 )
	{
		/// 존재하지 않는다면 새로 만든다.
		pDM->StartFrame();
		
		CString sRecKey;
		
		ids = pDM->MakeRecKey( sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SetManageValue") );
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("GROUP_1") , _T("STRING") , _T("L") , TRUE );
		pDM->AddDBFieldData( _T("GROUP_2") , _T("STRING") , sGroup2 , TRUE );
		pDM->AddDBFieldData( _T("GROUP_3") , _T("STRING") , sGroup3 , TRUE );
		pDM->AddDBFieldData( _T("CLASS_ALIAS") , _T("STRING") , sAlias, TRUE );
		pDM->AddDBFieldData( _T("VALUE_NAME") , _T("STRING") , sValueName , TRUE );
		pDM->AddDBFieldData( _T("VALUE_1") , _T("STRING") , sValue , TRUE );
		pDM->AddDBFieldData( _T("SYS_SETUP_YN") , _T("STRING") , _T("Y") , TRUE );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") , TRUE ); // 2008.10.22 ADD BY PWR (통합시스템 적용)
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("반납연기&예약환경"), __WFILE__, __LINE__ ), TRUE );
		
		pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
		
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetManageValue") );
		
		pDM->InsertRow(-1);
		INT nInsertRow = pDM->GetRowCount() -1 ;
		ids = pDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_2") , sGroup2 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_3") , sGroup3 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("CLASS_ALIAS") , sAlias , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_NAME") , sValueName , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_1") , sValue , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("SetManageValue") );
		
		return 0;
	}
	
	ids = pDM->SetCellData( _T("VALUE_1") , sValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetManageValue") );
	
	
	

	ids = CESL_Mgr::SetManageValue( sGroup2 , sGroup3 , sAlias , sValueName , sValue );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

HBRUSH CMngLocMgr02Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
