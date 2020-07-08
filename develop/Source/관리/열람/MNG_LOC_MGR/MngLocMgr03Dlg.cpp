// MngLocMgr03Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr03Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr03Dlg dialog


CMngLocMgr03Dlg::CMngLocMgr03Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr03Dlg)
	m_nDELAY_MONEY_USE = -1;
	m_nDELAY_LOAN_STOP = -1;
	m_nAPPENDIX_DELAY_LOAN_STOP = -1;
	m_AddHolydayLoanStop = -1;
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl1 = NULL;
	m_pEditCtrl2 = NULL;
	// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정 메뉴 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	m_pEditCtrl3 = NULL;
//*/ END ---------------------------------------------------------------------------
}

CMngLocMgr03Dlg::~CMngLocMgr03Dlg()
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
	// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정 메뉴 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	if(m_pEditCtrl3)
	{
		delete m_pEditCtrl3;
		m_pEditCtrl3 = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}


VOID CMngLocMgr03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr03Dlg)
	DDX_Radio(pDX, IDC_radSETTING_DELAY_MONEY_USE_Y, m_nDELAY_MONEY_USE);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_USE_Y, m_nDELAY_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_LOAN_STOP_USE_Y2, m_nAPPENDIX_DELAY_LOAN_STOP);
	DDX_Radio(pDX, IDC_radSETTING_ADD_HOLYDAY_LOANSTOP1, m_AddHolydayLoanStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr03Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr03Dlg)
	ON_WM_CTLCOLOR()  
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_eMNG01_DELAYDAY_WEIGHT, OnKillfocuseMNG01DELAYDAYWEIGHT)
	ON_EN_KILLFOCUS(IDC_eMNG01_BASIC_ARREAR, OnKillfocuseMNG01BASICARREAR)
	ON_EN_KILLFOCUS(IDC_eMNG01_MAX_LOAN_STOP_DAY, OnKillfocuseMNG01MAXLOANSTOPDAY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr03Dlg message handlers

BOOL CMngLocMgr03Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMngLocMgr03Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 11;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 12;
/*//END-------------------------------------------------------------------------------*/
	
	// 구분
	CString sSection[nCnt] = 
			{	_T("연체관리"), _T("부록관리"), _T("연체관리"), _T("연체관리"), _T("연체관리"), 
				_T("연체관리"),	_T("연체관리"), _T("연체관리"), _T("연체관리"), _T("연체관리"),
				_T("연체관리") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체관리")
//*/ END ---------------------------------------------------------------------------
			};
	// 기능
	CString sAbility[nCnt] = 
			{	_T("대출정지일부여"),		 _T("부록연체시 대출정지일부여"),	_T("연체료부여"),	
				_T("대출정지일 휴관일포함"), _T("대출정지일 부여기준"),			_T("연체일 가중치"), 
				_T("연체일가감일1"),		 _T("연체일가감일2"),				_T("기본연체료"),	
				_T("누적 연체횟수 부여방법"),_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체기간 휴관일포함")
//*/ END ---------------------------------------------------------------------------
			};
	// 설명
	CString sExplain[nCnt] = 
			{	_T("이용자가 연체된 자료반납시 대출정지일을 부여한다"), 
				_T("부록자료가 연체되었을 경우 대출정지일을 부여한다"), 
				_T("이용자가 연체된 자료반납시 연체료를 부여한다"),
				_T("대출정지일에 휴관일을 포함한 기간을 적용한다"),
				_T("반납책중 연체일이 가장큰 책으로 부여할지(큰대출정지일로부여) 반납자료의 연체일의합(기존대출정지일에 합산)으로 할지결정"), 
				_T("연체 하루당 부여되는 가중치"),
				_T("대출정지부여일수 = (연체일 + 연체일가감일1) x 연체일가중치 + 연체일가감일2"), 
				_T("대출정지부여일수 = (연체일 + 연체일가감일1) x 연체일가중치 + 연체일가감일2"),
				_T("연체 하루당 지불해야 하는 금액"),
				_T("누적연체횟수를 대출정지일을 부여한 횟수로 할지 연체된 자료의 누적연체횟수로 할지 결정"),
				_T("대출정지일 부여시 부여할 수 있는 최대일수 (0일:제한안함)") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			   , _T("연체일에 휴관일을 포함한 기간을 적용한다") 
//*/ END ---------------------------------------------------------------------------
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("대출정지일부여여부"),		_T("부록연체적용여부"),	_T("연체료부여여부"),	
				_T("대출정지휴관일포함유무"),	_T("연체일모드"),		_T("연체일가중치"), 
				_T("대출정지일부여일수"),		_T("연체일부여일수"),	_T("연체료"),	
				_T("누적연체횟수부여방법"),		_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			   , _T("연체기간휴관일포함유무")
//*/ END ---------------------------------------------------------------------------
			};

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
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
		
		if( sAbility[i] == _T("연체일가감일1") || sAbility[i] == _T("연체일가감일2") )
			if( sGetData == _T("-0") ) sGetData = _T("0");
		// DM저장
		m_pDM->SetCellData( _T("설정값"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// 대출정지일에 휴관일 포함유무
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("대출정지휴관일포함유무"),strData);
	if(_T("Y") == strData) m_AddHolydayLoanStop = 0;
	else  m_AddHolydayLoanStop = 1;
	strData.Empty();
	if(ids<0) return -1;

	//첫 번째 
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("대출정지일부여여부"),strData);
	if(_T("Y") == strData) m_nDELAY_LOAN_STOP = 0;
	else  m_nDELAY_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록연체적용여부"),strData);
	if(_T("Y") == strData) m_nAPPENDIX_DELAY_LOAN_STOP = 0;
	else  m_nAPPENDIX_DELAY_LOAN_STOP = 1;
	strData.Empty();
	if(ids<0) return -1;

	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료부여여부"),strData);
	if(_T("Y") == strData) m_nDELAY_MONEY_USE = 0;
	else  m_nDELAY_MONEY_USE = 1;
	strData.Empty();
	if(ids<0) return -2;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_DAY_HOW);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체일모드"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -3;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_COUNT_HOW);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("누적연체횟수부여방법"),strData);
	if(_T("0") == strData) pCombo->SetCurSel(0);
	else pCombo->SetCurSel(1);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -4;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER2);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체일가중치"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -5;

	//pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER);
	//ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료가중치"),strData);
	//pEdit->SetWindowText(strData);
	//pEdit = NULL;
	//strData.Empty();
	//if(ids<0) return -6;

	pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY);
	ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료"),strData);
	pEdit->SetWindowText(strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -7;

	//const INT cnt = 2;
	//CString alias[ cnt ] = {
	//	_T("대출정지일부여일수"), _T("연체일부여일수") };
	//UINT comboID[ cnt ] = {
	//		IDC_COMBO1, IDC_COMBO3 };
	//UINT edtID[ cnt ] = {
	//		IDC_EDIT1, IDC_EDIT2 };
	//
	//if( GetManageValue( _T("열람"),_T("공통"),_T("연체관리"), alias[ opt ], value ) < 0 )
	//	return -1;
	
	if( this->GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지일부여일수"), strData ) < 0 )
		return -8;
	else
		SetDelayDateValue( 0, strData );

	if( this->GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("연체일부여일수"), strData ) < 0 )
		return -9;
	else
		SetDelayDateValue( 1, strData );
	
	//if( SetDelayDateValue( 0 ) )
	//	return -8;
	//
	//if( SetDelayDateValue( 1 ) )
	//	return -9;

	//if( GetManageValue( _T("열람"),_T("공통"),_T("연체관리"),_T("대출정지일부여일수"),strData ) < 0 )
	//	return -8;
	//SetDlgItemText( IDC_EDIT1, strData );

	UpdateData(FALSE);
	*/
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr03Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 11;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 12;
/*//END-------------------------------------------------------------------------------*/

	// 구분
	CString sSection[nCnt] = 
			{	_T("연체관리"), _T("부록관리"), _T("연체관리"), _T("연체관리"), _T("연체관리"), 
				_T("연체관리"),	_T("연체관리"), _T("연체관리"), _T("연체관리"), _T("연체관리"),
				_T("연체관리") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체관리")
//*/ END ---------------------------------------------------------------------------
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("대출정지일부여여부"),		_T("부록연체적용여부"),	_T("연체료부여여부"),	
				_T("대출정지휴관일포함유무"),	_T("연체일모드"),		_T("연체일가중치"), 
				_T("대출정지일부여일수"),		_T("연체일부여일수"),	_T("연체료"),	
				_T("누적연체횟수부여방법"),		_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체기간휴관일포함유무")
//*/ END ---------------------------------------------------------------------------
			};

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
	/*
	UpdateData(TRUE);
	
	CString strData = _T("");
	INT ids = 0;

	// 2007.07.13 add by pdw
	// 대출정지일에 휴관일 포함유무
	if(0 == m_AddHolydayLoanStop) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("대출정지휴관일포함유무"),strData);
	strData.Empty();
	if(ids<0) return -1;

	//첫 번째 
	if(0 == m_nDELAY_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("대출정지일부여여부"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nAPPENDIX_DELAY_LOAN_STOP) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("부록관리"),_T("부록연체적용여부"),strData);
	strData.Empty();
	if(ids<0) return -1;

	if(0 == m_nDELAY_MONEY_USE) strData = _T("Y");
	else  strData = _T("N");
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료부여여부"),strData);
	strData.Empty();
	if(ids<0) return -2;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_DAY_HOW);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체일모드"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -3;
	
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbSETTING_DELAY_COUNT_HOW);
	strData.Format(_T("%d"),pCombo->GetCurSel());
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("누적연체횟수부여방법"),strData);
	pCombo = NULL;
	strData.Empty();
	if(ids<0) return -4;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER2);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체일가중치"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -5;

	//pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY_ADD_NUMBER);
	//pEdit->GetWindowText(strData);
	//ids = this->GetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료가중치"),strData);
	//pEdit = NULL;
	//strData.Empty();
	//if(ids<0) return -6;

	pEdit = (CEdit*)GetDlgItem(IDC_edtSETTING_DELAY_MONEY);
	pEdit->GetWindowText(strData);
	ids = this->SetManageValue(_T("열람"),_T("공통"),_T("연체관리"),_T("연체료"),strData);
	pEdit = NULL;
	strData.Empty();
	if(ids<0) return -7;

	if( this->SetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지일부여일수"), GetDelayDateValue( 0 ) ) < 0 )
		return -8;

	if( this->SetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("연체일부여일수"), GetDelayDateValue( 1 ) ) < 0 )
		return -9;
	
	//if( UpdateDelayDateValue( 0 ) )
	//	return -8;
	//
	//if( UpdateDelayDateValue( 1 ) )
	//	return -9;

	//GetDlgItemText( IDC_EDIT1, strData );
	//if( SetManageValue( _T("열람"),_T("공통"),_T("연체관리"),_T("대출정지일부여일수"),strData ) < 0 )
	//	return -8;
	*/
	return 0;

EFS_END
return -1;
}

/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr03Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
INT CMngLocMgr03Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
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
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("연체자료"), __WFILE__, __LINE__ ), TRUE );
		
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
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

EFS_END
return -1;
}

BOOL CMngLocMgr03Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/*
	CComboBox *pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	*/
	if( InitESL_Mgr(_T("SM_대출환경설정_연체자료")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_대출환경설정_연체자료"));
	m_pDM = FindDM(_T("DM_대출환경설정_연체자료"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 반납연기횟수
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_DELAYDAY_WEIGHT);
	m_pEditCtrl1->ShowWindow(SW_HIDE);
	// 예약허용인원
	m_pEditCtrl2 = new CEdit();
	m_pEditCtrl2->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_BASIC_ARREAR);
	m_pEditCtrl2->ShowWindow(SW_HIDE);
	// 최대대출정지부여일수
	m_pEditCtrl3 = new CEdit();
	m_pEditCtrl3->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG01_MAX_LOAN_STOP_DAY);
	m_pEditCtrl3->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;
}

INT CMngLocMgr03Dlg::SetDelayDateValue( INT opt, CString value )
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("대출정지일부여일수"), _T("연체일부여일수") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	//if( GetManageValue( _T("열람"),_T("공통"),_T("연체관리"), alias[ opt ], value ) < 0 )
	//	return -1;

	INT nValue = _ttoi( value );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( nValue > 0 )
		pCombo->SetCurSel( 0 );
	else
		pCombo->SetCurSel( 1 );

	CString sValue;
	sValue.Format( _T("%d"), abs( nValue ) );
	SetDlgItemText( edtID[ opt ], sValue );

	return 0;
}

INT CMngLocMgr03Dlg::UpdateDelayDateValue( INT opt )
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("대출정지일부여일수"), _T("연체일부여일수") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	CString flag;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( pCombo->GetCurSel() )
		flag = _T("-");
	else
		flag = _T("+");

	CString sValue, value;
	GetDlgItemText( edtID[ opt ], sValue );

	value.Format( _T("%s%s"), flag, sValue );
	if( SetManageValue( _T("열람"),_T("공통"),_T("연체관리"), alias[ opt ], value ) < 0 )
		return -1;

	return 0;
}

CString CMngLocMgr03Dlg::GetDelayDateValue(INT opt)
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("대출정지일부여일수"), _T("연체일부여일수") };
	UINT comboID[ cnt ] = {
			IDC_COMBO1, IDC_COMBO3 };
	UINT edtID[ cnt ] = {
			IDC_EDIT1, IDC_EDIT2 };
	
	CString flag;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( comboID[ opt ] );
	if( pCombo->GetCurSel() )
		flag = _T("-");
	else
		flag = _T("+");

	CString sValue, value;
	GetDlgItemText( edtID[ opt ], sValue );

	value.Format( _T("%s%s"), flag, sValue );

	return value;
}

HBRUSH CMngLocMgr03Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BEGIN_EVENTSINK_MAP(CMngLocMgr03Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr03Dlg)
	ON_EVENT(CMngLocMgr03Dlg, IDC_gridMNG_LOC_MGR_03, -600 /* Click */, OnClickgridMNGLOCMGR03, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr03Dlg::OnClickgridMNGLOCMGR03() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();	
}

void CMngLocMgr03Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_03);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

// ** 콤보박스 생성 및 값 변경
VOID CMngLocMgr03Dlg::CreateComboBoxInGridSell()
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

	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	// 1:대출정지일부여, 2:부록연체시 대출정지일부여, 3:연체료부여, 4:대출정지일 휴관일포함
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 4 )
*///AFTER------------------------------------------------------------------------------
	// 1:대출정지일부여, 2:부록연체시 대출정지일부여, 3:연체료부여, 4:대출정지일 휴관일포함, 12:연체기간휴관일포함유무
	if( nRow == 1 || nRow == 2 || nRow == 3 || nRow == 4 || nRow == 12 )
/*//END-------------------------------------------------------------------------------*/
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
		ArrCmbData.Add(_T("사용안함"));
	}
	// 5:대출정지일 부여기준
	else if( nRow == 5 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("연체일중 가장 큰 연체일"));
		ArrCmbData.Add(_T("각 자료의 연체일 합"));
	}
	// 10:누적연체횟수부여방법
	else if( nRow == 10 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("대출정지부여일을 기준으로"));
		ArrCmbData.Add(_T("연체된 책수로 부여"));
	}
	// 11:최대대출정지부여일수
	else if( nRow == 11 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl3);
		return;
	}
	// 7:연체일가감일1, 8:연체일가감일2
	else if( nRow == 7 || nRow == 8 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("-3"));
		ArrCmbData.Add(_T("-2"));
		ArrCmbData.Add(_T("-1"));
		ArrCmbData.Add(_T("0"));
		ArrCmbData.Add(_T("1"));
		ArrCmbData.Add(_T("2"));
		ArrCmbData.Add(_T("3"));
	}
	// 6:연체일가중치
	else if( nRow == 6 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	// 9:기본연체료
	else if( nRow == 9 )
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
	m_pComboBox->InitMngLocMgr(3);
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

INT CMngLocMgr03Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
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
	sValue.Replace( _T("원"), _T("") );
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

INT CMngLocMgr03Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr03Dlg::ApplyDataChange(LPCTSTR lpszData)
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

	return 0;

EFS_END
return -1;
}

void CMngLocMgr03Dlg::OnKillfocuseMNG01DELAYDAYWEIGHT() 
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

void CMngLocMgr03Dlg::OnKillfocuseMNG01BASICARREAR() 
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

void CMngLocMgr03Dlg::OnKillfocuseMNG01MAXLOANSTOPDAY() 
{
	SetFocus();
	if (m_pEditCtrl3 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl3->GetSafeHwnd())) return ;

	m_pEditCtrl3->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl3->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

INT CMngLocMgr03Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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


