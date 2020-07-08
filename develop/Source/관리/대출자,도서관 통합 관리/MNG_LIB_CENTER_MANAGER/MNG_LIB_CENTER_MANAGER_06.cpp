// MNG_LIB_CENTER_MANAGER_06.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_06.h"
#include "MNG_LIB_CENTER_MANAGER_01.h"
#include "MNG_LIB_CENTER_MANAGER_07.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_06 dialog


CMNG_LIB_CENTER_MANAGER_06::CMNG_LIB_CENTER_MANAGER_06(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_LIB_CENTER_MANAGER_06::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_LIB_CENTER_MANAGER_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_06, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_06)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_BN_CLICKED(IDC_bLibINPUT, OnbLibINPUT)
	ON_MESSAGE(UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_BN_CLICKED(IDC_bLibDELETE, OnbLibDELETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_06 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_CENTER_MANAGER_06") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_CENTER_MANAGER_06") );
		return false;
	}

	m_pGridEdt = NULL;
	m_curLibRow = -1;
	SetCtrlProperty();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	m_checkedDMRow = NULL;
	m_checkedRowCnt = 0;
	m_index = 0;
	
	if( !SetCheckedRowInfo() )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		PostMessage( WM_CLOSE );
	}

	ViewModifyData();
}

// DM_MNG_LIB_CENTER_MANAGER_06
// CM_MNG_LIB_CENTER_MANAGER_06

VOID CMNG_LIB_CENTER_MANAGER_06::SetCtrlProperty()
{
	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("학생") );
	pCombo->InsertString( 2, _T("직장인") );
	pCombo->SetCurSel( 0 );

	MakeMailList();
}

VOID CMNG_LIB_CENTER_MANAGER_06::MakeMailList()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01_mailList") );
	pDM->RefreshDataManager( _T("") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	INT cnt = pDM->GetRowCount();
	CString desc;

	pCombo->InsertString( 0, _T("") );
	for( INT i = 0; i < cnt; i++ )
	{
		pDM->GetCellData( _T("DESCRIPTION"), i, desc );
		pCombo->InsertString( i + 1, desc );
	}
	pCombo->SetCurSel( 0 );
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	FreeGridEdt();	
	
	CDialog::DestroyWindow();
}

// 저장
VOID CMNG_LIB_CENTER_MANAGER_06::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT cCnt = 20;
	CString cData[ cCnt ];
	GetCenterData( cData );

	if( ErrorCheckCivilNum( cData[ 1 ] ) )
	{
		AfxMessageBox( _T("잘못된 주민등록번호 입니다.") );
		return;
	}

	if( UpdateCenterData( cData ) )
	{
		AfxMessageBox( _T("Error : INT CMNG_LIB_CENTER_MANAGER_06::UpdateCneterData( CString data[] )") );
		return;
	}

	if( UpdateLibData() )
	{
		AfxMessageBox( _T("Error : INT CMNG_LIB_CENTER_MANAGER_06::UpdateLibData()") );
		return;
	}

	AfxMessageBox( _T("수정되었습니다.") );
	
	// CDialog::OnOK();
}

// 닫기
VOID CMNG_LIB_CENTER_MANAGER_06::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	FreeCheckedInfo();
	
	CDialog::OnCancel();
}

// 이전
VOID CMNG_LIB_CENTER_MANAGER_06::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}

	m_index--;
	ViewModifyData();
}

// 다음
VOID CMNG_LIB_CENTER_MANAGER_06::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if( m_index + 1 == m_checkedRowCnt )
	{
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
		return;
	}

	m_index++;
	ViewModifyData();
}

INT CMNG_LIB_CENTER_MANAGER_06::SetCheckedRowInfo()
{
	CMNG_LIB_CENTER_MANAGER_01 *pParent = ( CMNG_LIB_CENTER_MANAGER_01 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedRowInfo( m_checkedRowCnt );
	
	return m_checkedRowCnt;
}

VOID CMNG_LIB_CENTER_MANAGER_06::FreeCheckedInfo()
{
	if( m_checkedRowCnt )
		free( m_checkedDMRow );
}

VOID CMNG_LIB_CENTER_MANAGER_06::ViewModifyData()
{
	const INT mDataCnt = 19;
	CString mData[ mDataCnt ];
	GetMData( mData );
	ShowMasterData( mData );

	GetCurrentPK();	
	
	ShowLibData();
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnSelchangeCombo3() 
{
	// TODO: Add your control notification handler code here

	SetCtrlPropertyByPType();
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetCtrlPropertyByPType( INT type )
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	if( -1 == type )
		type = pCombo->GetCurSel();

	CString caption[ 4 ] = { _T("학교 / 직장 명"), _T("학번 / 내선번호"), _T("반 / 직급"), _T("학년 / 부서") };
	switch( type )
	{
		case 1 :
			caption[ 0 ] = _T("학 교");
			caption[ 1 ] = _T("학 번");
			caption[ 2 ] = _T("반");
			caption[ 3 ] = _T("학 년");
			break;
		case 2 :
			caption[ 0 ] = _T("회사명");
			caption[ 1 ] = _T("내선번호");
			caption[ 2 ] = _T("직 급");
			caption[ 3 ] = _T("부 서");
			break;
	}
	
	CWnd *pWnd;
	UINT wndID[ 4 ] = { IDC_STATIC_1, IDC_STATIC_2, IDC_STATIC_3, IDC_STATIC_4 };
	for( INT i = 0; i < 4; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->SetWindowText( caption[ i ] );
	}

	CEdit *pEdt;
	const INT edtCnt = 7;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT15, IDC_EDIT13, IDC_EDIT14 };
	bool flag;
	if( type )
		flag = true;
	else
		flag = false;

	for( i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->EnableWindow( flag );
	}
}

VOID CMNG_LIB_CENTER_MANAGER_06::GetMData( CString data[] )
{
	const INT aliasCnt = 17;
	const INT ctrlCnt  = 19;
	
	CString alias[ aliasCnt ] = {
		_T("NAME"),				//	0
		_T("H_ZIPCODE"),	//	1
		_T("H_ADDR1"),		//	2
		_T("H_PHONE"),		//	3
		_T("HANDPHONE"),		//	4
		_T("P_NAME"),			//	5
		_T("P_LEVEL"),			//	6
		_T("P_GROUP"),			//	7
		_T("P_NUM"),			//	8
		_T("W_ADDR1"),		//	9
		_T("W_ZIPCODE"),	// 10
		_T("W_PHONE"),		// 11
		_T("BIRTHDAY"),			// 12
		_T("P_TYPE"),			// 13
		_T("DATE_TYPE"),		// 14
		_T("E_MAIL"),			// 15
		_T("CIVIL_NO")  };		// 16

	CString buf[ aliasCnt ];
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );
	pDM->GetCellData( alias, aliasCnt, buf, m_checkedDMRow[ m_index ] );

	for( INT i = 0; i < 12; i++ )
		data[ i ] = buf[ i ];

	INT locate = buf[ 15 ].Find( '@' );
	data[ 14 ] = buf[ 15 ].Left( locate );
	data[ 18 ] = buf[ 15 ].Right( buf[ 15 ].GetLength() - locate );

	data[ 12 ] = buf[ 16 ].Left( 6 );
	data[ 13 ] = buf[ 16 ].Right( 7 );

	data[ 16 ] = buf[ 13 ];
	data[ 17 ] = buf[ 14 ];
	data[ 15 ] = buf[ 12 ];
}

VOID CMNG_LIB_CENTER_MANAGER_06::ShowMasterData( CString data[] )
{
	const INT edtCnt = 15;
	unsigned edtID[ edtCnt ] = { IDC_EDIT1, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8, IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT15, IDC_EDIT13, IDC_EDIT14, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };
	
	CEdit *pBtn;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pBtn = ( CEdit * )GetDlgItem( edtID[ i ] );
		pBtn->SetWindowText( data[ i ] );
	}

	SetBirthDay( data[ 15 ] );
	SetPersonType( data[ 16 ] );
	SetDateType( data[ 17 ] );
	SetMailList( data[ 18 ] );
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetBirthDay( CString date )
{
	INT y, m, d;
	COleDateTime day;
	_stscanf( date.GetBuffer( 0 ), _T("%d/%d/%d\0"), &y, &m, &d );
	day.SetDate( y, m, d );

	CDateTimeCtrl * dateCtrl = ( CDateTimeCtrl * )GetDlgItem( IDC_DATETIMEPICKER1 );
	dateCtrl->SetTime( day );

	UpdateData( true );
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetPersonType( CString type )
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	if( _T("S") == type )
		pCombo->SetCurSel( 1 );
	else if( _T("C") == type )
		pCombo->SetCurSel( 2 );
	else
		pCombo->SetCurSel( 0 );

	SetCtrlPropertyByPType( pCombo->GetCurSel() );
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetDateType( CString type )
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	if( _T("+") == type )
		pCombo->SetCurSel( 0 );
	else
		pCombo->SetCurSel( 1 );
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetMailList( CString mail )
{	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01_mailList") );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	CString desc;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("DESCRIPTION"), i, desc );

		if( desc == mail )
		{
			pCombo->SetCurSel( i + 1 );
			return;
		}
	}

	pCombo->SetCurSel( 0 );
	pCombo->SetWindowText( mail );
}	

VOID CMNG_LIB_CENTER_MANAGER_06::ShowLibData()
{
	CString where;
	where.Format( _T("U.USER_KEY = '%s'"), m_pk );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );
	pDM->RefreshDataManager( where );

	ShowGrid();
}

CString CMNG_LIB_CENTER_MANAGER_06::GetCurrentPK()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );
	pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], m_pk );

	return m_pk;
}

VOID CMNG_LIB_CENTER_MANAGER_06::ShowGrid()
{
	m_curLibRow = -1;
	ControlDisplay( _T("CM_MNG_LIB_CENTER_MANAGER_06"), _T("그리드") );
}

BEGIN_EVENTSINK_MAP(CMNG_LIB_CENTER_MANAGER_06, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_LIB_CENTER_MANAGER_06)
	ON_EVENT(CMNG_LIB_CENTER_MANAGER_06, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_LIB_CENTER_MANAGER_06::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetGridReverse( INT row )
{
	CESL_ControlMgr *pCM = FindCM( _T("CM_MNG_LIB_CENTER_MANAGER_06") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	if( -1 == row )
		row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	m_curLibRow = row - 1;
	pGrid->SetReverse( m_curLibRow );
	pGrid->SetCol( col );

	if( col < 2 )
		return;

	GetGridCellNewValue( pGrid->GetText(), row, col );
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}

	FreeGridEdt();
}

VOID CMNG_LIB_CENTER_MANAGER_06::FreeGridEdt()
{
	if( m_pGridEdt )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}
}

VOID CMNG_LIB_CENTER_MANAGER_06::GetGridCellNewValue( CString value, INT row, INT col )
{
	FreeGridEdt();
	m_pGridEdt = new CGridEdt;
	
	m_pGridEdt->row = row;
	m_pGridEdt->col = col;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_mngGRID_1 );

	m_pGridEdt->parent = ( CWnd * )this;
	
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( value );
    m_pGridEdt->SetSel( 0, -1 );
}

VOID CMNG_LIB_CENTER_MANAGER_06::SetGridText(INT row, INT col, CString text)
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();

	pGrid->SetRow( row );
	pGrid->SetCol( col );
	pGrid->SetText( text );

	pGrid->SetRow( curRow );
	pGrid->SetCol( curCol );

	UpdateDMValue( row - 1, col - 1, text );
}

VOID CMNG_LIB_CENTER_MANAGER_06::UpdateDMValue( INT row, INT col, CString text )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );
	
	const INT aliasCnt = 4;
	CString alias[ aliasCnt ] = { _T("USER_NO"), _T("USER_ID"), _T("PASSWORD"), _T("FID_KEY") };
	pDM->SetCellData( alias[ col - 1 ], text, row );
}

VOID CMNG_LIB_CENTER_MANAGER_06::GetCenterData(CString data[])
{
	CWnd *pWnd;

	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT1 );
	pWnd->GetWindowText( data[ 0 ] );

	CString tmp1, tmp2;
	
	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT2 );
	pWnd->GetWindowText( tmp1 );
	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT3 );
	pWnd->GetWindowText( tmp2 );
	data[ 1 ].Format( _T("%s-%s"), tmp1, tmp2 );

	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT4 );
	pWnd->GetWindowText( tmp1 );
	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO2 );
	pWnd->GetWindowText( tmp2 );
	data[ 2 ].Format( _T("%s%s"), tmp1, tmp2 );

	const INT cnt = 11;
	UINT wndID[ cnt ] = {
		IDC_EDIT5,  IDC_EDIT6,  IDC_EDIT7,  IDC_EDIT8,  IDC_EDIT9, 
		IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT15, IDC_EDIT13, 
		IDC_EDIT14 };
	
	for( INT i = 0; i < cnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->GetWindowText( data[ i + 3 ] );
	}

	data[ 14 ] = GetDate();
	
	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO1 );
	pWnd->GetWindowText( data[ 15 ] );

	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO3 );
	pWnd->GetWindowText( tmp1 );
	if( _T("학생") == tmp1 )
		data[ 16 ].Format( _T("S") );
	else if( _T("직장인") == tmp1 )
		data[ 16 ].Format( _T("C") );
	else
		data[ 16 ].Empty();
}

CString CMNG_LIB_CENTER_MANAGER_06::GetDate(  )
{
	CString date;
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( IDC_DATETIMEPICKER1 );
	CTime time;
	UpdateData( false );
	
	pDate->GetTime( time );	
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	return date;
}

INT CMNG_LIB_CENTER_MANAGER_06::UpdateCenterData( CString data[] )
{
	const INT fieldCnt = 20;
	CString fieldName[ fieldCnt ] = {
		_T("NAME"),				//	0
		_T("CIVIL_NO"),			//	1
		_T("E_MAIL"),			//	2
		_T("H_ZIPCODE"),	//	3
		_T("H_ADDR1"),		//	4
		_T("H_PHONE"),		//	5
		_T("HANDPHONE"),		//	6
		_T("P_NAME"),			//	7
		_T("P_NUM"),			//	8
		_T("P_GROUP"),			//	9
		_T("P_LEVEL"),			// 10
		_T("W_PHONE"),		// 11
		_T("W_ZIPCODE"),	// 12
		_T("W_ADDR1"),		// 13
		_T("BIRTHDAY"),			// 14
		_T("DATE_TYPE"),		// 15
		_T("P_TYPE"),			// 16
		_T("REC_KEY"),			// 17
		_T("UPDATE_DATE"),		// 18
		_T("LAST_WORK") };		// 19
	
	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING") };		

	data[ 17 ] = m_pk;
	data[ 18 ] = GetCurrentDate();
	data[ 19 ] = GetWorkLogMsg( _T("도서관 통합 관리"), __WFILE__, __LINE );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	pDM->MakeUpdateFrame( _T("CO_MASTER_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 17 ] );

	AddDataToMasterDM( pDM, fieldName, data, fieldCnt );

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

INT CMNG_LIB_CENTER_MANAGER_06::ErrorCheckCivilNum(CString num)
{
	if( 14 != num.GetLength() )
		return -1;

	INT locate = num.Find( '-' );
	if( 6 != locate )
		return -2;

	CString frontNum = num.Left( 6 );
	
	INT n;
	n = _ttoi( frontNum.Mid( 2, 2 ) );
	if( n > 12 || n < 1 )
		return -3;

	n = _ttoi( frontNum.Mid( 4, 2 ) );
	if( n > 31 || n < 1 )
		return -4;
	
	return 0;
}

INT CMNG_LIB_CENTER_MANAGER_06::AddDataToMasterDM(CESL_DataMgr *pDM, CString fieldName[], CString data[], const INT cnt)
{
	const INT aliasCnt = 22;
	CString alias[ aliasCnt ];
	CString value[ aliasCnt ];

	for( INT i = 0; i < cnt; i++ )
	{
		alias[ i ] = fieldName[ i ];
		value[ i ] = data[ i ];
	}
	
	alias[ 20 ] = _T("생일");
	alias[ 21 ] = _T("집주소");

	value[ 20 ].Format( _T("%s(%s)"), data[ 14 ], data[ 15 ] );
	value[ 21 ].Format( _T("%s %s"), data[ 3 ], data[ 4 ] );
	
	pDM->SetCellData( alias, aliasCnt, value, m_checkedDMRow[ m_index ] );
	
	return 0;
}

CString CMNG_LIB_CENTER_MANAGER_06::GetCurrentDate()
{
	CString date;

	CTime time = CTime::GetCurrentTime();
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	return date;
}

INT CMNG_LIB_CENTER_MANAGER_06::UpdateLibData()
{
	DeleteOldData();
	
	const INT fieldCnt = 11;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),		//	0
		_T("USER_KEY"),		//	1
		_T("LIB_KEY"),		//	2
		_T("MAKE_DATE"),	//	3
		_T("UPDATE_DATE"),	//	4
		_T("USER_NO"),		//	5
		_T("USER_ID"),		//	6
		_T("PASSWORD"),		//	7
		_T("FID_KEY"),	//	8
		_T("FIRST_WORK"),	//	9
		_T("LAST_WORK") };	// 10

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") , _T("STRING"), _T("STRING") };
	
	CString data[ fieldCnt ];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( fieldName, fieldCnt, data, i );

		data[  1 ].Format( _T("%s"), m_pk );
		data[  4 ].Format( _T("%s"), GetCurrentDate() );
		data[ 10 ].Format( _T("%s"), GetWorkLogMsg( _T("도서관센터관리"), __WFILE__, __LINE ) );

		for( INT j = 0; j < fieldCnt; j++ )
			pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		pDM->MakeInsertFrame( _T("CO_LIBRARY_USER_TBL") );
	}

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;	
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnbLibINPUT() 
{
	// TODO: Add your control notification handler code here
	
	CMNG_LIB_CENTER_MANAGER_07 dlg( this );
	dlg.DoModal();
}

VOID CMNG_LIB_CENTER_MANAGER_06::OnbLibDELETE() 
{
	// TODO: Add your control notification handler code here

	if( -1 == m_curLibRow )
	{
		AfxMessageBox( _T("선택된 도서관 정보가 없습니다.") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );
	pDM->DeleteRow( m_curLibRow );

	FreeGridEdt();
	ShowGrid();
}

VOID CMNG_LIB_CENTER_MANAGER_06::DeleteOldData()
{
	CString query;
	query.Format( _T("DELETE FROM CO_LIBRARY_USER_TBL WHERE USER_KEY = '%s';"), m_pk );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );
	pDM->ExecuteQuery( query, 1 );
}

BOOL CMNG_LIB_CENTER_MANAGER_06::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( WM_KEYDOWN == pMsg->message && VK_DELETE == pMsg->wParam )
	{
		OnbLibDELETE();
		return true;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}
