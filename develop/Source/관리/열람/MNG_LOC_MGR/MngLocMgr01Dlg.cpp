// MngLocMgr01Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr01Dlg.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr01Dlg dialog


CMngLocMgr01Dlg::CMngLocMgr01Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr01Dlg)
	m_nRELOAN_ONE = -1;
	m_nRELOAN_FAMILY = -1;
	m_nLOAN_MANAGE = -1;
	m_nLOAN_TYPE0 = -1;
	m_nLOAN_TYPE1 = -1;
	m_nLOAN_TYPE2 = -1;
	m_nLOAN_TYPE3 = -1;
	m_nRETURN_TYPE0 = -1;
	m_nRETURN_TYPE1 = -1;
	m_nRETURN_TYPE2 = -1;
	m_nUSER_FAMILY = -1;
	m_nUSER_NUMBER = -1;
	m_nUSER_LOAN_STOP = -1;
	m_Appendix_Include_loan_book_Y = -1;
	m_Appendix_Loan_Y = -1;
	m_UserCard_Y = -1;
	m_nReLoanView = -1;
	m_sReLoanLimitDay = _T("0");
	m_cAutoNumType = -1;
	m_nRETURN_MANAG = -1;
	m_nLOAN_RECEIPT = -1;
	m_nRETURN_RECEIPT = -1;
	m_nCIVILNO_CHECK_Y = -1;
	m_Appendix_Return_Y = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl[0] = NULL;
	m_pEditCtrl[1] = NULL;

	m_bIsUnityLoanService = FALSE;
}
CMngLocMgr01Dlg::~CMngLocMgr01Dlg()
{	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl[0])
	{
		delete m_pEditCtrl[0];		
		m_pEditCtrl[0] = NULL;
	}
	if(m_pEditCtrl[1])
	{
		delete m_pEditCtrl[1];
		m_pEditCtrl[1] = NULL;
	}	
}


VOID CMngLocMgr01Dlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN	

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr01Dlg)
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_ONE_Y, m_nRELOAN_ONE);
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_FAMILY_Y, m_nRELOAN_FAMILY);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_MANAGE_Y, m_nLOAN_MANAGE);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_0_Y, m_nLOAN_TYPE0);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_1_Y, m_nLOAN_TYPE1);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_2_Y, m_nLOAN_TYPE2);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_TYPE_3_Y, m_nLOAN_TYPE3);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_0_Y, m_nRETURN_TYPE0);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_1_Y, m_nRETURN_TYPE1);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_TYPE_2_Y, m_nRETURN_TYPE2);
	DDX_Radio(pDX, IDC_radSETTING_FAMILY_USE_Y, m_nUSER_FAMILY);
	DDX_Radio(pDX, IDC_radSETTING_USER_NO_AUTO_USE_Y, m_nUSER_NUMBER);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_DATE_AUTO_USE_Y, m_nUSER_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_INLUDE_LOAN_BOOK_Y, m_Appendix_Include_loan_book_Y);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_LOAN_Y, m_Appendix_Loan_Y);
	DDX_Radio(pDX, IDC_radSETTING_USER_CARD_Y, m_UserCard_Y);
	DDX_Radio(pDX, IDC_radSETTING_RELOAN_VIEW_Y, m_nReLoanView);
	DDX_Text(pDX, IDC_edtRELOAN_LIMIT_DAY, m_sReLoanLimitDay);
	DDX_Control(pDX, IDC_edtRELOAN_LIMIT_DAY, m_edtReLoanLimitDay);
	DDX_CBIndex(pDX, IDC_COMBO2, m_cAutoNumType);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_MANAGE_Y, m_nRETURN_MANAG);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_RECEIPT_USE_Y, m_nLOAN_RECEIPT);
	DDX_Radio(pDX, IDC_radSETTING_RETURN_RECEIPT_USE_Y, m_nRETURN_RECEIPT);
	DDX_Radio(pDX, IDC_radSETTING_CIVILNO_AUTO_CHECK_USE_Y, m_nCIVILNO_CHECK_Y);
	DDX_Radio(pDX, IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y, m_Appendix_Return_Y);
	//}}AFX_DATA_MAP

	EFS_END		
}


BEGIN_MESSAGE_MAP(CMngLocMgr01Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr01Dlg)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_ONE_N, OnradSETTINGRELOANONEN)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_ONE_Y, OnradSETTINGRELOANONEY)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_FAMILY_Y, OnradSETTINGRELOANFAMILYY)
	ON_BN_CLICKED(IDC_radSETTING_RELOAN_FAMILY_N, OnradSETTINGRELOANFAMILYN)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y, OnradSETTINGAPPENDIXINLUDERETURNBOOKY)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N, OnradSETTINGAPPENDIXINLUDERETURNBOOKN)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_LOAN_Y, OnradSETTINGAPPENDIXLOANY)
	ON_BN_CLICKED(IDC_radSETTING_APPENDIX_LOAN_N, OnradSETTINGAPPENDIXLOANN)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtRELOAN_LIMITDAY, OnKillfocusedtRELOANLIMITDAY)
	ON_EN_KILLFOCUS(100, OnKillfocusedt2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr01Dlg message handlers

BOOL CMngLocMgr01Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/*
	pMngSpfWork = new CMngLocMgrAPI(this);
	
	// 김용배 추가
	const INT listCnt = 3;
	CString list[ listCnt ] = {
		_T("사용안함"), _T("마지막번호"), _T("주민등록번호") };
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	for( INT i = 0; i < listCnt; i++ )
		pCombo->InsertString( i, list[ i ] );
	pCombo->SetCurSel( 0 );
	*/
	if( InitESL_Mgr(_T("SM_대출환경설정_대출반납과이용자")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_대출환경설정_대출반납과이용자"));
	m_pDM = FindDM(_T("DM_대출환경설정_대출반납과이용자"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 재대출제한일
	m_pEditCtrl[0] = new CEdit();
	m_pEditCtrl[0]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_edtRELOAN_LIMITDAY);
	m_pEditCtrl[0]->ShowWindow(SW_HIDE);

	m_pEditCtrl[1] = new CEdit();
	m_pEditCtrl[1]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, 100);
	m_pEditCtrl[1]->ShowWindow(SW_HIDE);

	if(m_pInfo->MODE==10000)
	{
		CString strButtonAlias ;
		strButtonAlias = _T("변경내용저장");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("이전내용");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
	}
	
	CString strValue;

	// 18.09.27 JMJ KOL.RED.2018.008
	// 책두레운영모드 관리값 추가
	INT ids = pParentMgr->GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	}
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y") == strValue) 
	{
		ids = pParentMgr->GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레운영모드"), strValue);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("OnInitDialog"));
		}
		strValue.TrimLeft(); strValue.TrimRight();
		if(strValue.IsEmpty())
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");
	}
	
	// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템 사용여부
	ids = pParentMgr->GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), strValue);

	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y")==strValue)
	{
		m_bIsUnityLoanService = TRUE;
	}
	else
	{
		m_bIsUnityLoanService = FALSE;
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMngLocMgr01Dlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN	

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

INT CMngLocMgr01Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 23;
*///AFTER------------------------------------------------------------------------------
	//const int nCnt = 24;
/*//END-------------------------------------------------------------------------------*/
	//JOB.2019.0033 ADD BY KYJ
	const int nCnt = 25;
	// 구분
	CString sSection[nCnt] = 
			{	_T("대출형식"),		_T("대출형식"),		_T("대출형식"),		_T("대출형식"),		//4
				_T("반납형식"),		_T("반납형식"),		_T("반납형식"),		_T("재대출"),		//8	
				_T("재대출"),		_T("재대출"),		_T("재대출"),		 _T("대출관리"), //JOB.2019.0033 ADD BY KYJ	//12
				_T("부록자료"),		_T("부록자료"),		_T("부록자료"),		
				_T("이용자관리"),	_T("이용자관리"), 	_T("이용자관리"),	
				_T("이용자관리"),	_T("이용자관리"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("이용자관리"),	_T("영수증사용"), _T("등록구분"), _T("배달자료관리") 
*///AFTER------------------------------------------------------------------------------
				_T("이용자관리"),	_T("이용자관리"),
				_T("영수증사용"),	 _T("등록구분"),	_T("배달자료관리") 
/*//END-------------------------------------------------------------------------------*/
			};
				
	// 기능
	CString sAbility[nCnt] = 
			{	_T("일반대출사용"),			_T("관내대출사용"),		_T("특별대출사용"), //3
				_T("장기대출사용"),			_T("일반반납사용"),		_T("일괄반납사용"),	//6		
				_T("소급반납사용"),			_T("가족재대출허용"),	_T("개인재대출허용"), //9	
				_T("재대출이력보여주기"),	_T("재대출제한일"),		_T("타시스템카드번호사용여부"), //JOB.2019.0033 ADD BY KYJ //12
				_T("부록자료대출"), 
				_T("부록자료반납"), 		_T("대출책수에 포함"),	_T("가족회원기능"),			
				_T("대출정지일 자동일괄해제/선정"),		_T("대출자번호 자동부여"), 
				_T("대출자번호 사용자ID로 자동입력"),	_T("주민등록번호 뒷자리 비밀번호로 자동입력"), // 2009.04.2 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("주민등록번호 자동중복검사"),		_T("영수증출력사용"), _T("도서관별 등록구분 중복여부"),
*///AFTER------------------------------------------------------------------------------
				_T("주민등록번호 자동중복검사"),		 _T("본인인증을 통한 이용자조회 사용여부"),
				_T("영수증출력사용"),					_T("도서관별 등록구분 중복여부"),
/*//END-------------------------------------------------------------------------------*/				
				_T("기본 대출기한")
			};
	// 설명
	CString sExplain[nCnt] = 
			{	_T("보통 관외 대출"), 
				_T("대출기한에 1일 적용"), 
				_T("대출제한에 걸려있는 책이나 이용자와 상관없이 대출"),
				_T("이용자 직급에 상관없이 지정된 대출기한을 적용"),
				_T("일반적인 낱권반납"), 
				_T("여러책을 동시에 반납"),
				_T("반납일을 지정하여 반납"), 
				_T("대출하려는 자료가 이미 가족 구성원의 대출이력이 있어도 대출가능"),
				_T("대출하려는 자료가 해당 이용자의 대출이력이 있어도 대출가능"),
				_T("대출,예약 하려는 자료가 대출이력이 있다면 이력을 보여준다"), 
				_T("반납일로부터 적용일자 만큼 자료가 대출한 이력이 있다면 대출제한을 준다"), 
				//JOB.2019.0033 ADD BY KYJ
				_T("타시스템의 카드번호를 사용할지 결정한다"), //12
				_T("부록자료의 대출여부를 결정한다"),
				_T("반납시 대출된 부록자료가있는 자료는 필수적으로 부록자료를 반납하는지 결정"),
				_T("부록자료 대출책수에 포함한다"), 
				_T("이용자입력이나 대출/반납에서 가족회원기능을 사용한다"),
				_T("대출/반납메뉴 사용시 대출정지일이 지난 이용자의 불량 회원구분을 자동일괄해제한다"), 
				_T("대출자 입력시 대출자번호를 마지막번호 또는 주민번호로 자동입력한다"),
				_T("대출자번호를 사용자ID로 자동입력한다."),			// 2009.04.21 ADD BY KSJ
				_T("비밀번호를 주민등록번호 뒷자리로 자동입력한다."),	// 2009.04.21 ADD BY KSJ
				_T("주민등록번호 입력시 자동으로 중복검사를 한다"),
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
//*/ ADD ---------------------------------------------------------------------------
				_T("본인인증기능을 사용하여 이용자정보를 조회한다"),
//*/ END ---------------------------------------------------------------------------
				_T("설정에 따라 대출이나 반납일때 사용할 수 있고, 대출반납 모두에서 사용할 수 있다."),
				_T("도서관별로 이전 등록구분이 중복될 때, 다른 도서관의 이전 등록구분을 적용할 수 있도록 한다."),
				_T("배달처리시의 기본 대출기한을 설정한다.")};
	// Alias
	CString sAlias[nCnt] = 
			{	_T("대출형식사용여부"), _T("대출형식사용여부"), _T("대출형식사용여부"), 
				_T("대출형식사용여부"), _T("반납형식사용여부"), _T("반납형식사용여부"),	
				_T("반납형식사용여부"), _T("대출관리"),			_T("대출관리"), 
				_T("대출관리"),	
				//JOB.2019.0033 ADD BY KYJ				
				_T("대출관리"),  
				_T("대출관리"), 		_T("부록관리"),			_T("부록관리"), 
				_T("부록관리"),			_T("이용자관리"),		_T("이용자관리"), 
				_T("이용자관리"),		
				_T("이용자관리"),		_T("이용자관리"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("이용자관리"),		_T("영수증출력"), _T("등록구분"), _T("배달자료관리") 
*///AFTER------------------------------------------------------------------------------
				_T("이용자관리"),		_T("이용자관리"),
				_T("영수증출력"),		_T("등록구분"),			_T("배달자료관리") 
/*//END-------------------------------------------------------------------------------*/
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("일반"), _T("관내"), _T("특별"),	_T("장기"), _T("일반"), _T("일괄"),	_T("소급"),  //7
				_T("가족재대출여부"),		_T("개인재대출여부"),			_T("재대출이력보여주기여부"),  //10
				_T("재대출제한일"),		//11
				//JOB.2019.0033 ADD BY KYJ
				_T("타시스템카드번호사용여부"),	 //12
				_T("부록대출여부"),			_T("부록반납여부"), 			_T("부록대출권수포함여부"),			
				_T("가족회원기능사용여부"),	_T("대출정지일괄해제여부"),		_T("대출자번호자동부여여부"),
				_T("대출자번호사용자ID로자동입력"),	_T("주민등록번호뒷자리비밀번호로자동입력"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("주민등록번호자동중복검사여부"),	_T("영수증출력사용형태"), _T("도서관별 등록구분 중복여부"),
*///AFTER------------------------------------------------------------------------------
				_T("주민등록번호자동중복검사여부"), _T("본인인증조회사용여부"),
				_T("영수증출력사용형태"), _T("도서관별 등록구분 중복여부"),
/*//END-------------------------------------------------------------------------------*/
				_T("기본 대출기한") };

	// Display
	m_pDM->FreeData();
	INT nNum = 0;
	for( INT i = 0; i < nCnt; i++ )
	{
		CString sData;
		// 장기대출은 육사에서만 보이도록 한다
		if( sManage[i] == _T("장기") )
		{
			ids = this->GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		// 영수증출력 사용유무에 따라 그리드에 표시
		else if( sManage[i] == _T("영수증출력사용형태") )
		{
			ids = this->GetManageValue( _T("열람"), _T("공통"), _T("영수증출력"), _T("영수증출력사용여부"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("구분"), sSection[i], nNum ); 
		m_pDM->SetCellData( _T("기능"), sAbility[i], nNum );
		m_pDM->SetCellData( _T("설명"), sExplain[i], nNum );

		CString sGetData;
		ids = this->GetManageValue( _T("열람"), _T("공통"), sAlias[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("관리설정값을 가져오는데 실패하였습니다."));
			return -1;
		}

		// 2010.12.22 DELETE BY KSJ : 일반회원이라면 대출자번호 자동부여 관리값 사용 할 수 있도록 수정
		// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템 사용 할 경우 대출자번호 자동부여는 Y(자동부여)
//DEL 		if(m_bIsUnityLoanService)
//DEL 		{
//DEL 			if(16==i) sGetData = _T("Y");
//DEL 		}

		// DM저장
		m_pDM->SetCellData( _T("설정값"), sGetData, nNum );
		nNum++;
	}
	m_pCM->AllControlDisplay();

	// 그리드 셋팅 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid01(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	/*
	CString strData = _T("");
	INT ids = 0;

	//첫 번째 
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("일반"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE0 = 0;
	else  m_nLOAN_TYPE0 = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("관내"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE1 = 0;
	else  m_nLOAN_TYPE1 = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("특별"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE2 = 0;
	else  m_nLOAN_TYPE2 = 1;
	strData.Empty();
	if(ids<0) return -3;
	
	// 2005.09.27. ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++
	// 장기형식 추가 
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("장기"),strData);
	if(_T("Y") == strData) m_nLOAN_TYPE3 = 0;
	else  m_nLOAN_TYPE3 = 1;
	strData.Empty();
	if(ids<0) return -3;
	// _________________________________________________________________________

	//두번째
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("일반"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE0 = 0;
	else  m_nRETURN_TYPE0 = 1;
	strData.Empty();
	if(ids<0) return -4;
	
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("일괄"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE1 = 0;
	else  m_nRETURN_TYPE1 = 1;
	strData.Empty();
	if(ids<0) return -5;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("소급"),strData);
	if(_T("Y") == strData) m_nRETURN_TYPE2 = 0;
	else  m_nRETURN_TYPE2 = 1;
	strData.Empty();
	if(ids<0) return -6;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("가족재대출여부"),strData);
	if(_T("Y") == strData) m_nRELOAN_FAMILY = 0;
	else  m_nRELOAN_FAMILY = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("개인재대출여부"),strData);
	if(_T("Y") == strData) m_nRELOAN_ONE = 0;
	else  m_nRELOAN_ONE = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("재대출이력보여주기여부"),strData);
	if(_T("Y") == strData) m_nReLoanView = 0;
	else  m_nReLoanView = 1;
	strData.Empty();
	if(ids<0) return -2;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("다른관리구분값대출여부"),strData);
	if(_T("Y") == strData) m_nLOAN_MANAGE = 0;
	else  m_nLOAN_MANAGE = 1;
	strData.Empty();
	if(ids<0) return -5;

	/// 20040312 조병걸 수정 다른관리구분값반납여부 추가
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("다른관리구분값반납여부"),strData);
	if(_T("Y") == strData) m_nRETURN_MANAG = 0;
	else  if(_T("N") == strData ) m_nRETURN_MANAG = 1;
	else m_nRETURN_MANAG = m_nLOAN_MANAGE;
	strData.Empty();
	if(ids<0) return -5;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("재대출제한일"),m_sReLoanLimitDay);
	if(ids<0) return -5;

	//두번째
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("가족회원기능사용여부"),strData);
	if(_T("Y") == strData) m_nUSER_FAMILY = 0;
	else  m_nUSER_FAMILY = 1;
	strData.Empty();
	if(ids<0) return -8;
	
	// 2005 07.27 ADD BY PDJ
	// 이용자 입력시 주민등록번호 자동중복검사 옵션 추가
	ids = this->GetManageValue( _T("열람"),_T("공통"),_T("이용자관리"),_T("주민등록번호자동중복검사여부"),strData);
	if(_T("Y") == strData) m_nCIVILNO_CHECK_Y = 0;
	else  m_nCIVILNO_CHECK_Y = 1;
	strData.Empty();
	if(ids<0) return -11;
	ids = this->GetManageValue( _T("열람"),_T("공통"),_T("이용자관리"),_T("대출자번호자동부여여부"),strData);
	// 김용배 수정
	// N : 사용안함 , A : 주민번호로 생성, Y : 마지막번호로 생성
	strData.TrimLeft();
	if( strData.IsEmpty() )
		return -9;
	switch( strData.GetAt( 0 ) )
	{
		case 'Y' :				// 마지막번호로 사용
			m_cAutoNumType = 1;
			break;
		case 'A' :				// 주민번호로 사용
			m_cAutoNumType = 2;
			break;
		case 'N' :				// 사용 안함 
		default :
			m_cAutoNumType = 0;
			break;
	}
	
	
	//if(_T("Y") == strData) m_nUSER_NUMBER = 0;
	//else  m_nUSER_NUMBER = 1;
	//strData.Empty();
	//if(ids<0) return -9;
	

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("대출정지일괄해제여부"),strData);
	if(_T("Y") == strData) m_nUSER_LOAN_STOP = 0;
	else  m_nUSER_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -10;

	// 2007.04.06 add by pdw {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 부록관리
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록대출여부"),strData);
	if(_T("Y") == strData)
	{
		m_Appendix_Loan_Y = 0;
		OnradSETTINGAPPENDIXINLUDERETURNBOOKY();
	}
	else
	{
		m_Appendix_Loan_Y = 1;
		OnradSETTINGAPPENDIXINLUDERETURNBOOKN();
	}
	strData.Empty();
	if(ids<0) return -10;

	// 부록자료반납 기능 사용유무
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록반납여부"),strData);
	if(_T("Y") == strData) 	m_Appendix_Return_Y = 0;		
	else	m_Appendix_Return_Y = 1;
	strData.Empty();
	if(ids<0) return -10;
	// -------------------------------------------------------------------------------}}

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록대출권수포함여부"),strData);
	if(_T("Y") == strData) m_Appendix_Include_loan_book_Y = 0;
	else  m_Appendix_Include_loan_book_Y = 1;
	strData.Empty();
	if(ids<0) return -10;

	// 대출증 사용여부
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("대출증사용여부"),strData);
	if(_T("Y") == strData) m_UserCard_Y = 0;
	else  m_UserCard_Y = 1;
	strData.Empty();

	// 영수증 사용여부(Append_20041002_HAN...)
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("영수증출력"),_T("영수증출력사용여부"),strData);
	strData.TrimLeft( ); strData.TrimRight( );
	if ( strData.IsEmpty( ) ) strData = _T("N");
	if(_T("N") == strData)//사용하지 않으면 아예 보여주지 않는다.
	{
		GetDlgItem(IDC_lblTITLE8)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblFUNCTION18)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblFUNCTION19)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_LOAN_RECEIPT_USE_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_LOAN_RECEIPT_USE_N)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RETURN_RECEIPT_USE_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_radSETTING_RETURN_RECEIPT_USE_N)->ShowWindow(FALSE);
		GetDlgItem(IDC_lblDESC9)->ShowWindow(FALSE);

		CRect rect1, rect2 , rect3, rect4, rect5;

		CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_STATIC_BORDER);
		CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG1);
		CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG2);
		CStatic* pStatic4 = (CStatic*)GetDlgItem(IDC_imgDIVISION_LONG3);
		CStatic* pStatic5 = (CStatic*)GetDlgItem(IDC_imgDIVISION_WIDTH);

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

		INT nBorderHeight = rect1.Height( ) - (rect1.bottom - rect5.bottom);

		//어쩔 수 없이 그대로 맞춰줬다.
		pStatic1->MoveWindow( 10, 0, rect1.Width( ), nBorderHeight );
		pStatic2->MoveWindow( 115, 8, rect2.Width( ), nBorderHeight-8 );
		pStatic3->MoveWindow( 115+180, 8, rect3.Width( ), nBorderHeight-8 );
		pStatic4->MoveWindow( 330+90, 8, rect4.Width( ), nBorderHeight-8 );
		GetDlgItem(IDC_imgDIVISION_WIDTH)->ShowWindow(FALSE);
	}
	else
	{
		// 영수증 출력을 사용하면 사용형태값을 가져온다.
		strData.Empty();
		ids = this->GetManageValue(_T("열람"),_T("공통"),_T("영수증출력"),_T("영수증출력사용형태"),strData);
		if ( 'Y' == strData.GetAt(0) )	m_nLOAN_RECEIPT = 0;
		else	m_nLOAN_RECEIPT = 1;
		if ( 'Y' == strData.GetAt(1) )	m_nRETURN_RECEIPT = 0;
		else	m_nRETURN_RECEIPT = 1;
		strData.Empty();
		if(ids<0) return -10;
	}
	if(ids<0) return -10;
	
	// 2005.09.27 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// 대출형식에 육사 관련 장기대출 항목 추가 (육사 경우에만 보여준다)
	//										   (기존 형식위치를 옮긴다.)
	ids = this->GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , strData);
	if ( strData == _T("Y") )
	{
		// 장기대출 그룹 보여주기 
		GetDlgItem(IDC_COMMON_LOAN_TYPE_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOAN_TYPE_EXP_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radSETTING_LOAN_TYPE_3_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radSETTING_LOAN_TYPE_3_N)->ShowWindow(SW_SHOW);
		// 기존 대출형식 위치 옮기기
		CWnd* pWnd[10];
		pWnd[0] = GetDlgItem(IDC_COMMON_LOAN_TYPE_1);pWnd[1] = GetDlgItem(IDC_COMMON_LOAN_TYPE_2);
		pWnd[2] = GetDlgItem(IDC_COMMON_LOAN_TYPE_3);pWnd[3] = GetDlgItem(IDC_LOAN_TYPE_EXP_1);
		pWnd[4] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_0_Y);pWnd[5] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_0_N);
		pWnd[6] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_1_Y);pWnd[7] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_1_N);
		pWnd[8] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_2_Y);pWnd[9] = GetDlgItem(IDC_radSETTING_LOAN_TYPE_2_N);

		CRect rc;

		for (INT i = 0;i < 10;i++)
		{
			pWnd[i]->GetWindowRect(&rc);
			this->ScreenToClient(&rc);
			rc.top = rc.top -8;rc.bottom = rc.bottom - 8;
			pWnd[i]->MoveWindow(&rc);
		}		
	}
	if (ids<0) return -10;
	// ______________________________________________________________________
	UpdateData(FALSE);

	if( ((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_FAMILY_N))->GetCheck()==1)
		OnradSETTINGRELOANFAMILYN();
	*/

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr01Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 23;
*///AFTER------------------------------------------------------------------------------
	//const int nCnt = 24;
/*//END-------------------------------------------------------------------------------*/
	//JOB.2019.0033 ADD BY KYJ
	const int nCnt = 25;
	// Alias
	CString sAlias[nCnt] = 
			{	_T("대출형식사용여부"), _T("대출형식사용여부"), _T("대출형식사용여부"), 
				_T("대출형식사용여부"), _T("반납형식사용여부"), _T("반납형식사용여부"),	
				_T("반납형식사용여부"), _T("대출관리"),			_T("대출관리"), 
				_T("대출관리"),			_T("대출관리"), 		_T("대출관리"), // JOB.2019.0033 ADD BY KYJ
				_T("부록관리"),			
				_T("부록관리"), 		_T("부록관리"),			_T("이용자관리"),		
				_T("이용자관리"), 		_T("이용자관리"),		
				_T("이용자관리"), 		_T("이용자관리"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("이용자관리"),		_T("영수증출력"), _T("등록구분"), _T("배달자료관리") 
*///AFTER------------------------------------------------------------------------------
				_T("이용자관리"),      _T("이용자관리"),
				_T("영수증출력"),	   _T("등록구분"),		   _T("배달자료관리") 
/*//END-------------------------------------------------------------------------------*/
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("일반"), _T("관내"), _T("특별"),	_T("장기"), _T("일반"), _T("일괄"),	_T("소급"), 
				_T("가족재대출여부"),		_T("개인재대출여부"),			_T("재대출이력보여주기여부"), 
				_T("재대출제한일"),			_T("타시스템카드번호사용여부"),	 //JOB.2019.0033 ADD BY KYJ //12
				_T("부록대출여부"),			_T("부록반납여부"), 			_T("부록대출권수포함여부"),			
				_T("가족회원기능사용여부"),	_T("대출정지일괄해제여부"),		_T("대출자번호자동부여여부"),
				_T("대출자번호사용자ID로자동입력"),	_T("주민등록번호뒷자리비밀번호로자동입력"),	// 2009.04.21 ADD BY KSJ
				// 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
/*//BEFORE-----------------------------------------------------------------------------
				_T("주민등록번호자동중복검사여부"),	_T("영수증출력사용형태"), _T("도서관별 등록구분 중복여부"),
*///AFTER------------------------------------------------------------------------------
				_T("주민등록번호자동중복검사여부"),	 _T("본인인증조회사용여부"),
				_T("영수증출력사용형태"),			 _T("도서관별 등록구분 중복여부"),
/*//END-------------------------------------------------------------------------------*/
				_T("기본 대출기한")
			};

	CString sCode;
	CString sData;
	INT nNum = 0;
	for( INT i = 0; i < nCnt; i++ )
	{
		// 장기대출은 육사에서만 보이도록 한다
		if( sManage[i] == _T("장기") )
		{
			ids = this->GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}
		// 영수증출력 사용유무에 따라 그리드에 표시
		else if( sManage[i] == _T("영수증출력사용형태") )
		{
			ids = this->GetManageValue( _T("열람"), _T("공통"), _T("영수증출력"), _T("영수증출력사용여부"), sData );
			sData.TrimLeft( ); sData.TrimRight( );
			if ( sData != _T("Y") ) continue;
		}

		m_pDM->GetCellData( _T("설정값"), nNum, sCode );
		nNum++;

		ids = this->SetManageValue( _T("열람"), _T("공통"), sAlias[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("관리설정값을 저장하는데 실패하였습니다."));
			return -1;
		}
	}
	/*	
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids;

	//첫 번째 
	if(0 == m_nLOAN_TYPE0) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("일반"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nLOAN_TYPE1) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("관내"),strData);
	strData.Empty();
	if(ids<0) return -2;

	if(0 == m_nLOAN_TYPE2) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("특별"),strData);
	strData.Empty();
	if(ids<0) return -3;
	
	// 2005.09.27 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++
	// 장기 형태 추가 
	if(0 == m_nLOAN_TYPE3) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("대출형식사용여부"),_T("장기"),strData);
	strData.Empty();
	if(ids<0) return -7;
	// _________________________________________________________________________

	//두번째
	if(0 == m_nRETURN_TYPE0) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("일반"),strData);
	strData.Empty();
	if(ids<0) return -4;

	if(0 == m_nRETURN_TYPE1) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("일괄"),strData);
	strData.Empty();
	if(ids<0) return -5;

	if(0 == m_nRETURN_TYPE2) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("반납형식사용여부"),_T("소급"),strData);
	strData.Empty();
	if(ids<0) return -6;


	if(0 == m_nRELOAN_FAMILY) strData = _T("Y");
	else strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("가족재대출여부"),strData);
	if(ids<0) return -1;

	if(0 == m_nRELOAN_ONE) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("개인재대출여부"),strData);
	if(ids<0) return -2;

	if(0 == m_nReLoanView) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("재대출이력보여주기여부"),strData);
	if(ids<0) return -2;
	
	if(0 == m_nLOAN_MANAGE) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("다른관리구분값대출여부"),strData);
	if(ids<0) return -5;

	if(0 == m_nRETURN_MANAG) strData = _T("Y");
	else strData = _T("N");
	ids =this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("다른관리구분값반납여부"),strData);
	if(ids<0) return -5;

	strData = m_sReLoanLimitDay;
	//++2008.06.16 ADD BY PWR {{++
	// 재대출 제한일 0 ~ 100으로 제한 오류 수정
	if( _ttoi(m_sReLoanLimitDay) < 0 || _ttoi(m_sReLoanLimitDay) > 100 )
	{
		AfxMessageBox( _T("재대출 제한일을 0에서 100사이의 정수를 입력하십시오.") );
		// 재대출 제한일에 포커스
		m_edtReLoanLimitDay.SetFocus();
		// 재대출 제한일에 블럭잡기
		INT nLen = m_sReLoanLimitDay.GetLength();
		m_edtReLoanLimitDay.SetSel(0, nLen);		
		return 1000;
	}
	//--2008.06.16 ADD BY PWR --}}
	ids =this->SetManageValue(_T("열람"),_T("공통"),_T("대출관리"),_T("재대출제한일"),strData);
	if(ids<0) return -5;

	//두번째
	if(0 == m_nUSER_FAMILY) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("가족회원기능사용여부"),strData);
	strData.Empty();
	if(ids<0) return -8;

	// 2005 07.27 ADD BY PDJ
	// 이용자 입력시 주민등록번호 자동중복검사 옵션 추가
	if(0 == m_nCIVILNO_CHECK_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("주민등록번호자동중복검사여부"),strData);
	strData.Empty();
	if(ids<0) return -8;

	
	//if(0 == m_nUSER_NUMBER) strData = _T("Y");
	//else  strData = _T("N");
		
	// 김용배수정 
	// N : 사용안함 , A : 주민번호로 생성, Y : 마지막번호로 생성
	switch( m_cAutoNumType )
	{
	case 1 :
		strData = _T("Y");
		break;
	case 2 :
		strData = _T("A");
		break;
	case 0 :
	default :
		strData = _T("N");
		break;
	}
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("대출자번호자동부여여부"),strData);
	strData.Empty();
	if(ids<0) return -9;

	if(0 == m_nUSER_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("대출정지일괄해제여부"),strData);
	strData.Empty();
	if(ids<0) return -10;

	// 부록관리
	if(0 == m_Appendix_Loan_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록대출여부"),strData);
	if(ids<0) return -11;
	// 2007.04.06 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 부록자료반납
	if(0 == m_Appendix_Return_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록반납여부"),strData);
	if(ids<0) return -11;
	// ------------------------------------------------------------------------------}}

	if(0 == m_Appendix_Include_loan_book_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록대출권수포함여부"),strData);
	if(ids<0) return -12;

	// 대출증사용여부
	if(0 == m_UserCard_Y) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("이용자관리"),_T("대출증사용여부"),strData);
	if(ids<0) return -12;

	// 영수증 사용여부(Append_20041002_HAN...)
	if(0 == m_nLOAN_RECEIPT) strData = _T("Y");
	else  strData = _T("N");
	if(0 == m_nRETURN_RECEIPT) strData += _T("Y");
	else  strData += _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("영수증출력"),_T("영수증출력사용형태"),strData);
	if(ids<0) return -12;
	*/
	return 0;

EFS_END
return -1;
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANONEN() 
{
	EFS_BEGIN

	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANONEY() 
{
	EFS_BEGIN

	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANFAMILYY() 
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->EnableWindow(TRUE);
	
	EFS_END
}

VOID CMngLocMgr01Dlg::OnradSETTINGRELOANFAMILYN() 
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_FAMILY_N))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_Y))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_radSETTING_RELOAN_ONE_N))->SetCheck(1);

	EFS_END
}


/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr01Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
INT CMngLocMgr01Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") , TRUE );
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("대출/반납형식&이용자"), __WFILE__, __LINE__ ), TRUE );
		
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
	if( ids < 0 ) 
	{
		CString sEMsg;
		sEMsg.Format( _T("Error Func: pDM->SetCellData( VALUE , %s , %d );") , sValue , nIndex );
		AfxMessageBox( sEMsg );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetManageValue") );

	ids = CESL_Mgr::SetManageValue( sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

	EFS_END
	return -1;

}
                      
VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXINLUDERETURNBOOKY() 
{
	
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXINLUDERETURNBOOKN() 
{
	
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXLOANY() 
{
	// 부록대출을 사용한다면 
	// 부록반납을 able로 변경한다.
	const INT nCtrlCnt = 2;
	INT       nCtrlID[nCtrlCnt] = { 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y , 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N
	};

	CWnd* pWnd = NULL;
	for( INT i=0;i<nCtrlCnt;i++ )
	{
		pWnd = GetDlgItem(nCtrlID[i]);
		if( pWnd )
		{
			pWnd->EnableWindow();
		}
	}
}

VOID CMngLocMgr01Dlg::OnradSETTINGAPPENDIXLOANN() 
{
	// 부록대출을 사용안한다면
	// 부록반납을 enable로 변경하며 값을 N으로 바꾼다.
	const INT nCtrlCnt = 2;
	INT       nCtrlID[nCtrlCnt] = { 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y , 
		IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N
	};

	CWnd* pWnd = NULL;
	for( INT i=0;i<nCtrlCnt;i++ )
	{
		pWnd = GetDlgItem(nCtrlID[i]);
		if( pWnd )
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	((CButton*)GetDlgItem(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_Y))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radSETTING_APPENDIX_INLUDE_RETURN_BOOK_N))->SetCheck(1);
}

HBRUSH CMngLocMgr01Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr01Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_01);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

BEGIN_EVENTSINK_MAP(CMngLocMgr01Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr01Dlg)
	ON_EVENT(CMngLocMgr01Dlg, IDC_gridMNG_LOC_MGR_01, -600 /* Click */, OnClickgridMNGLOCMGR01, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr01Dlg::OnClickgridMNGLOCMGR01() 
{
	if( 0 == m_pGrid->GetMouseRow() ) 
	{
		return;
	}

	INT col = m_pGrid->GetCol();
	if( 4 == col ) 
	{
		CreateComboBoxInGridSell();
	}
}

// ** 콤보박스 생성 및 값 변경
VOID CMngLocMgr01Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if( 0 == nRowCnt ) 
	{
		return ;
	}

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("설정값"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("기능"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	/// 가족재대출, 부록자료대출 사용여부에 따른 표시
	if( strElement == _T("개인재대출허용") || strElement == _T("부록자료반납") )
	{
		CString sDelay = m_pDM->GetCellData( _T("설정값"), nRow-2 );
		if( _T("Y") != sDelay ) 
		{
			return;
		}
	}

	// 2010.12.22 DELETE BY KSJ : 일반회원이라면 대출자번호 자동부여 관리값 사용 할 수 있도록 수정
	// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템에선 대출자번호 자동부여 값을 바꿀 수 없다
//DEL 	if(m_bIsUnityLoanService)
//DEL 	{
//DEL 		if(16 == nRow) return;
//DEL 	}

	// 제대출 제한일
	//JOB.2019.0033 WORK BY KYJ 
	//if( 10 == nRow )
	if( _T("재대출제한일") == strElement )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl[0]);
		return;
	}
	// 대출자번호 자동부여
	//JOB.2019.0033 WORK BY KYJ 
	//else if( 16 == nRow )
	else if( _T("대출자번호 자동부여") == strElement )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("마지막번호"));
		ArrCmbData.Add(_T("주민등록번호"));
		ArrCmbData.Add(_T("사용자ID"));
	}
	// 2009.04.21 ADD BY KSJ : 대출자번호사용자ID로자동입력
	//JOB.2019.0033 WORK BY KYJ 
	//else if( 17 == nRow )
	else if( _T("대출자번호 사용자ID로 자동입력") == strElement )
	{
		// 2009.10.19 ADD BY KSJ : 대출자번호 자동부여 -> 사용자ID 일 경우
		CString strValue = m_pDM->GetCellData( _T("설정값"), nRow-2 );
		if( _T("U") == strValue ) 
		{
			return;
		}

		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("사용함"));
	}	
	// 2009.04.21 ADD BY KSJ : 주민등록번호뒷자리비밀번호로자동입력
	//JOB.2019.0033 EDIT BY KYJ
	//else if( 18 == nRow )
	else if( _T("주민등록번호 뒷자리 비밀번호로 자동입력") == strElement )
	{
		// 2009.10.19 ADD BY KSJ : 대출자번호 자동부여 -> 사용자ID / 대출자번호사용자ID로자동입력 옵션 둘다 사용 안 할 경우
		//JOB.2019.0033 WORK BY KYJ
		//-------------------------------------------------------------------------------
		//CString strValue1 = m_pDM->GetCellData( _T("설정값"), nRow-3 );
		//CString strValue2 = m_pDM->GetCellData( _T("설정값"), nRow-2 );
		//-------------------------------------------------------------------------------
		CString strTempVal = _T("") , strValue1 = _T("") , strValue2 = _T("") ;
		for(int k = 0 ; k < m_pDM->GetRowCount() ;  k++){
			strTempVal = m_pDM->GetCellData(_T("기능"), k );
			if (_T("대출자번호 자동부여") == strTempVal)
			{
				strValue1 = m_pDM->GetCellData( _T("설정값") , k );
			}
			else if ( _T("대출자번호 사용자ID로 자동입력")  == strTempVal )
			{
				strValue2 = m_pDM->GetCellData( _T("설정값") , k );
			}
		}
		//-------------------------------------------------------------------------------

		if( _T("U") != strValue1 && _T("Y") != strValue2 )
		{
			return;
		}

		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("사용함"));
	}
	// 영수증출력사용
	//else if( nRow == 18 )
// 	else if( 20 == nRow )
	else if(_T("영수증출력사용") == strElement)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("대출,반납 모두"));
		ArrCmbData.Add(_T("대출일때만"));
		ArrCmbData.Add(_T("반납일때만"));
		ArrCmbData.Add(_T("사용안함"));
	}
// 	else if( 21 == nRow )
	else if(_T("도서관별 등록구분 중복여부") == strElement)
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("사용함"));
	}
	// 일반대출,반납은 무조건 사용함
	else if( _T("일반대출사용") == strElement || _T("일반반납사용") == strElement )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
	}
	else if(_T("기본 대출기한") == strElement)
	{
		ShowEditBox(m_pGrid, m_pEditCtrl[1]);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
		ArrCmbData.Add(_T("사용안함"));
	}

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(1);
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

INT CMngLocMgr01Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if ( NULL == pGrid ) 
	{
		return -1;
	}

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if ( 1 > nRow || 1 > nCol ) 
	{
		return -1;
	}

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// 텍스트 설정
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("일"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr01Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr01Dlg::ApplyDataChange(LPCTSTR lpszData)
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

	// 그리드 적용
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid01(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	return 0;

EFS_END
return -1;
}

void CMngLocMgr01Dlg::OnKillfocusedtRELOANLIMITDAY() 
{
	SetFocus();
	if (NULL == m_pEditCtrl[0]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[0]->GetSafeHwnd())) return ;

	m_pEditCtrl[0]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[0]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr01Dlg::OnKillfocusedt2()
{
	SetFocus();
	if (NULL == m_pEditCtrl[1]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[1]->GetSafeHwnd())) return ;

	m_pEditCtrl[1]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[1]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}
INT CMngLocMgr01Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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
