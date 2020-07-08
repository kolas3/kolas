// MngLocMgr07Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr07Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr07Dlg dialog


CMngLocMgr07Dlg::CMngLocMgr07Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr07Dlg)
	m_bLoanStopUserMessageFlag = FALSE;
	//}}AFX_DATA_INIT	

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;

}
CMngLocMgr07Dlg::~CMngLocMgr07Dlg()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
}


VOID CMngLocMgr07Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr07Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr07Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr07Dlg)
	ON_WM_CTLCOLOR() 
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr07Dlg message handlers

BOOL CMngLocMgr07Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMngLocMgr07Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 2006-05-24 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// 대출공유 관리 기능 수정
	//if ( InitESL_Mgr(_T("SM_MNG_LOC_MGR_07_DLG")) < 0 )
	//	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::OnInitDialog()") );
	// ----------------------------------------------------------------------

	if( InitESL_Mgr(_T("SM_대출환경설정_통합대출관리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_대출환경설정_통합대출관리"));
	m_pDM = FindDM(_T("DM_대출환경설정_통합대출관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   설정값을 보여준다.
// [ Mini Spec ]
// 1. 대출정지회원 메세지 출력 타입
// 2. 대출책수 제한 사용여부 
/////////////////////////////////////////////////////////////////////////////
INT CMngLocMgr07Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	const int nCnt = 4;
	// 구분
	CString sSection[nCnt] = 
			{	_T("통합대출관리"), _T("통합대출관리"), _T("통합대출관리"), _T("통합대출관리") };
	// 기능
	CString sAbility[nCnt] = 
			{	_T("이용자검색시 대출정지회원 안내"), 
				_T("이용자검색시 제적회원 안내"), 
				_T("센터반입시 ID, 비밀번호 반입여부"),
				_T("통합대출책수 사용여부") };
	// 설명
	CString sExplain[nCnt] = 
			{	_T("이용자 검색시 타도서관의 대출정지 회원일 경우 메시지 출력"), 
				_T("이용자 검색시 타도서관의 제적회원일 경우 메시지 출력"), 
				_T("센터반입시 중복되었을경우 ID와 비밀번호는 제외가 되며 메시지를 통해서 확인이 가능합니다"),
				_T("대출시 타도서관을 포함한 전체 대출책수가 통합대출책수를 초과햇을경우 대출제한") };
	// 설정값
	CString sManage[nCnt] = 
			{	_T("대출정지회원메시지출력"),
				_T("제적회원메시지출력"),
				_T("ID센터반입설정"),
				_T("통합대출책수제한") };

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("구분"), sSection[i], i ); 
		m_pDM->SetCellData( _T("기능"), sAbility[i], i );
		m_pDM->SetCellData( _T("설명"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("기타"), _T("공통"), _T("대출자공유관리"), sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("관리설정값을 가져오는데 실패하였습니다."));
			return -1;
		}

		// 이전 데이터 형식이 적용될수 있게
		if( sManage[i] == _T("대출정지회원메시지출력") || sManage[i] == _T("제적회원메시지출력") )
		{
			if( sGetData == _T("N") )		sGetData = _T("0");
			else if ( sGetData == _T("Y") )	sGetData = _T("1");
		}
		m_pDM->SetCellData( _T("설정값"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	// ======================================================================
	// Variable Declare
	// ======================================================================
	INT ids = 0;
	CString strData = _T("");	

	// ======================================================================
	// 1. 대출정지회원 메세지 출력 타입
	// ======================================================================
	// 2006-05-24 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++
	// 관리값 형태 수정에 따른 컨트롤 변경
	
	//ids = this->GetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("대출정지회원메시지출력"),strData);
	//if(_T("Y") == strData) m_bLoanStopUserMessageFlag = TRUE;
	//else  m_bLoanStopUserMessageFlag = FALSE;
	//strData.Empty();
	//if(ids<0) return -1;
	//
	//if ( m_bLoanStopUserMessageFlag )
	//{
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(1);
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(0);
	//}
	//else
	//{
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(0);
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(1);
	//}
	
	ids = this->GetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("대출정지회원메시지출력"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSetting_MESSAGE_TYPE);
	if ( !pCombo ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	// 이전 데이터 형식이 적용될수 있게
	// N -> 0 , Y -> 1
	if( strData == _T("N") )		strData = _T("0");
	else if ( strData == _T("Y") )	strData = _T("1");

	pCombo->SetCurSel( _ttoi( strData ) );
	// ----------------------------------------------------------------------

	// ======================================================================
	// 2. 대출책수 제한 사용여부 
	// ======================================================================
	ids = this->GetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("통합대출책수제한"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	// 없다...
	if ( ids == 100 )	strData = _T("N");

	if(_T("Y") == strData) m_bLoanStopUserMessageFlag = TRUE;
	else  m_bLoanStopUserMessageFlag = FALSE;
	strData.Empty();
	if(ids<0) return -1;
	
	if ( m_bLoanStopUserMessageFlag )
	{
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(1);
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(0);
	}
	else
	{
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(0);
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(1);
	}

	// 2006.12.07 ADD BY PDJ
	// ======================================================================
	// 3. 센터반입시 ID,PASSWORD 반입 여부	
	// ======================================================================
	ids = this->GetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("ID센터반입설정"), strData);	   
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	BOOL bCheck;	
	bCheck = ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->GetCheck();
	if ( strData == _T("Y") ) ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->SetCheck(TRUE);
	else ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_N))->SetCheck(TRUE);
	*/
	return 0;
EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   설정값을 저장한다.
// [ Mini Spec ]
// 1. 대출정지회원 메세지 출력 타입 저장
// 2. 대출책수 제한 사용여부 저장
/////////////////////////////////////////////////////////////////////////////
INT CMngLocMgr07Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 4;

	// 설정값
	CString sManage[nCnt] = 
			{	_T("대출정지회원메시지출력"),
				_T("제적회원메시지출력"),
				_T("ID센터반입설정"),
				_T("통합대출책수제한") };

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("설정값"), i, sCode );
		ids = this->SetManageValue( _T("기타"), _T("공통"), _T("대출자공유관리"), sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("관리설정값을 저장하는데 실패하였습니다."));
			return -1;
		}
	}
	/*
	// ======================================================================
	// Variable Declare
	// ======================================================================
	INT ids = 0;
	CString strData = _T("");	

	// ======================================================================
	// 1. 대출정지회원 메세지 출력 타입 저장
	// ======================================================================
	// 2006-05-24 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++
	// 관리값 형태 수정에 따른 컨트롤 변경
	
	//m_bLoanStopUserMessageFlag = ((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->GetCheck();
	//
	//if ( m_bLoanStopUserMessageFlag ) strData=_T("Y");
	//else strData=_T("N");
	//ids = this->SetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("대출정지회원메시지출력"),strData);
	//if(ids<0) return -2;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSetting_MESSAGE_TYPE);
	if ( !pCombo ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	strData.Format( _T("%d") , pCombo->GetCurSel() );

	ids = this->SetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("대출정지회원메시지출력"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	// ----------------------------------------------------------------------

	// ======================================================================
	// 2. 대출책수 제한 사용여부 
	// ======================================================================
	m_bLoanStopUserMessageFlag = ((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->GetCheck();

	if ( m_bLoanStopUserMessageFlag ) strData=_T("Y");
	else strData=_T("N");
	ids = this->SetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("통합대출책수제한"),strData);
	if ( ids < 0 && ids != -1) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	// 없다...
	if ( ids == -1 )
	{
		CString strQuery;
		strQuery.Format ( _T(" INSERT INTO ESL_MANAGE_TBL( REC_KEY,GROUP_1, GROUP_2,GROUP_3, GROUP_4, GROUP_5, CLASS_ALIAS,CLASS_DESC,") 
			              _T(" VALUE_NAME, VALUE_1, VALUE_2, VALUE_3, FIRST_WORK, LAST_WORK, SYS_SETUP_YN) ")
                          _T(" VALUES( ESL_SEQ.NEXTVAL	, 'L', '기타', '공통', '', '', '대출자공유관리'	, 'Y:통합대출책수제한 사용 N:사용안함', ") 
						  _T(" '통합대출책수제한', '%s', '', '', '', '', 'Y' );"),strData) ;
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		ids = dm.ExecuteQuery(strQuery, TRUE);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	}

	// ======================================================================
	// 2. 대출책수 제한 사용여부 
	// ======================================================================
	BOOL bCheck;
 	bCheck = ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->GetCheck();
	if ( bCheck ) strData = _T("Y");
	else strData = _T("N");
	
	ids = this->SetManageValue(_T("기타"),_T("공통"),_T("대출자공유관리"),_T("ID센터반입설정"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	*/
	return 0;

EFS_END
	return -1;	
}

HBRUSH CMngLocMgr07Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


BEGIN_EVENTSINK_MAP(CMngLocMgr07Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr07Dlg)
	ON_EVENT(CMngLocMgr07Dlg, IDC_gridMNG_LOC_MGR_07, -600 /* Click */, OnClickgridMNGLOCMGR07, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr07Dlg::OnClickgridMNGLOCMGR07() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();
}

void CMngLocMgr07Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_07);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);		
}

INT CMngLocMgr07Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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

// ** 콤보박스 생성 및 값 변경
VOID CMngLocMgr07Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData(_T("설정값"), nRow - 1);
	CString strElement = m_pDM->GetCellData(_T("기능"), nRow - 1);

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();
	
	// 1:대출정지회원메시지출력, 2:제적회원메시지출력
	if( nRow == 1 || nRow == 2 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용안함"));
		ArrCmbData.Add(_T("경고메시지"));
		ArrCmbData.Add(_T("대출제한"));
	}
	// 3:ID센터반입설정, 4:통합대출책수제한
	else if( nRow == 3 || nRow == 4 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
		ArrCmbData.Add(_T("사용안함"));
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("적용함"));
		ArrCmbData.Add(_T("적용안함"));
	}
	
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(7);
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