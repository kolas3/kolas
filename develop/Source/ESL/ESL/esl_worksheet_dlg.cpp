// ESL_WorkSheet_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_WorkSheet_dlg.h"

#include "ESL_DataMgr.h"

#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_WorkSheet_dlg dialog

BEGIN_EVENTSINK_MAP(CESL_WorkSheet_dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CESL_WorkSheet_dlg)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_2, -605 /* MouseDown */, OnMouseDownMshflexgrid2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_1, 72 /* LeaveCell */, OnLeaveCellMshflexgrid1, VTS_NONE)
	ON_EVENT(CESL_WorkSheet_dlg, IDC_MSHFLEXGRID_1, 71 /* EnterCell */, OnEnterCellMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

CESL_WorkSheet_dlg::CESL_WorkSheet_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CESL_WorkSheet_dlg::IDD, pParent)
{
	m_wnd = pParent;
	//{{AFX_DATA_INIT(CESL_WorkSheet_dlg)
	//}}AFX_DATA_INIT

	// 기본 크기
	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = 500;
	m_rect.bottom = 500;

	m_cell_ratio_grid_1_1 = 40; // 60% (GRID1의 ALias컬럼의 비율)
}

CESL_WorkSheet_dlg::~CESL_WorkSheet_dlg()
{
	if( m_ctrlInfo != NULL )
	{
		delete [] m_ctrlInfo;
		m_ctrlInfo = NULL;
	}

	if( NULL != m_multiComboBox )
	{
		delete m_multiComboBox;
		m_multiComboBox = NULL;
	}

}

VOID CESL_WorkSheet_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESL_WorkSheet_dlg)
	DDX_Control(pDX, IDOK_2, m_ok_2);
	DDX_Control(pDX, IDC_TAB_COM, m_tab);
	DDX_Control(pDX, IDC_MSHFLEXGRID_1, m_grid_1);
	DDX_Control(pDX, IDC_MSHFLEXGRID_2, m_grid_2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESL_WorkSheet_dlg, CDialog)
	//{{AFX_MSG_MAP(CESL_WorkSheet_dlg)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COM, OnSelchangeTab)
	ON_BN_CLICKED(IDOK_2, On2)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_MUUTICOMBO_BOX, OnSelchangeMuuticomboBox)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_WorkSheet_dlg message handlers

	/*
		< m_pInfo >
		
		// DB 접속 정보
		CString CONNECTION_INFO;

		// User 관련 정보
		CString USER_PK;
		CString USER_ID;
		CString USER_NAME;
		CString USER_PRIVILEGE;

		// Code Mgr 관련 정보
		CESL_CodeMgr *pCodeMgr;

		CESL_DataMgr *pESLMgr;
		CESL_DataMgr *pESLDataMgr;
		CESL_DataMgr *pESLControlMgr;
			SM : 
				pk
				esl_mgr_alias
				engineer
				mgr_pk
				mgr_type

			DM : 
				pk
				db_mgr_alias
				connection_info
				tbl_name
				condition
				extra_condition
				field_alias
				field_name
				field_type
				init_value
				data_type
				
			CM : 
				0	pk
				1	ctrl_mgr_alias
				2	ctrl_alias
				3	ctrl_type
				4	ctrl_id
				5	ctrl_style
				6	init_value
				7	list_data
				8	link_data
				9	validation
				10	requirecheck
	*/

BOOL CESL_WorkSheet_dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_editBox = NULL;
	m_dateBox = NULL;
	m_ctrlInfo = NULL;
	m_multiComboBox = NULL;
	m_change = true;

	CComboBox *comboBox = ( CComboBox * )GetDlgItem( IDC_MUUTICOMBO_BOX );
	comboBox->ShowWindow( false );

	CESL_Mgr *pMgr = (CESL_Mgr*)m_wnd;
	m_pCM = pMgr->FindCM(m_cmAlias);
	if (m_pCM == NULL) {
		;
	}

	SetTimer( 1905, 100, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 탭 개수와 이름 설정
VOID CESL_WorkSheet_dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	m_tab.InsertItem( 0, _T("입 력") );
	m_tab.InsertItem( 1, _T("설 정") );	

	ViewTabControl( 0 );

	UpdateData( true );
	CRect rect;

	m_grid_1.GetWindowRect( &rect );
	m_grid_1.SetRowHeight( 0, 300 );	
	m_grid_1.SetColWidth( 0, 0, 500 );
	m_grid_1.SetColWidth( 1, 0, ( INT )( rect.Width() * ( ( double )m_cell_ratio_grid_1_1 / 100.0 ) * 15 ) - 900 );
	m_grid_1.SetColWidth( 2, 0, ( INT )( rect.Width() * ( ( double )( 100 - m_cell_ratio_grid_1_1 ) / 100.0 ) ) * 15 );
	m_grid_1.SetColAlignmentBand( 1, 0, 4 );
	m_grid_1.SetColAlignmentBand( 0, 2, 4 );
	m_grid_1.SetRow( 1 );
	m_grid_1.SetCol( 2 );
	
	m_grid_2.GetWindowRect(&rect);
	m_grid_2.SetColWidth( 0, 0, 500 );
	m_grid_2.SetColWidth( 1, 0, 1000 );
	m_grid_2.SetColWidth( 2, 0, ( rect.Width() * 15 ) - 500 - 1000 - 400 );
	m_grid_2.SetColAlignmentBand( 0, 0, 4 );
	m_grid_2.SetColAlignmentBand( 0, 1, 4 );
	m_grid_2.SetRow( 1 );
	m_grid_2.SetCol( 1 );
}

// WorkSheet 입력 type을 얻어온다. 
INT CESL_WorkSheet_dlg::GetWorkSheetType()
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );
	
	if( r1->GetCheck() )
		return 1;

	if( r2->GetCheck() )
		return 2;

	if( r3->GetCheck() )
		return 3;

	if( r4->GetCheck() )
		return 4;

	return 0;	
}

VOID CESL_WorkSheet_dlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	
	ViewTabControl( m_tab.GetCurSel() );
}

// 첫번째 또는 두번째 탭을 보여준다. 
VOID CESL_WorkSheet_dlg::ViewTabControl( INT flag )
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );
	
	switch( flag )
	{
		case 0 : // 입력
			
			SetFirstTabControl();
			SetControls();

			m_grid_1.ShowWindow( true );

			m_grid_2.ShowWindow( false );
			m_ok_2.ShowWindow( false );
			r1->ShowWindow( false );
			r2->ShowWindow( false );
			r3->ShowWindow( false );
			r4->ShowWindow( false );

			break;

		case 1 : // 설정
			
			MoveGridCell( 1 );
			SetSecondTabControl();

			m_grid_2.ShowWindow( true );
			m_ok_2.ShowWindow( true );
			r1->ShowWindow( true );
			r2->ShowWindow( true );
			r3->ShowWindow( true );
			r4->ShowWindow( true );
			
			m_grid_1.ShowWindow( false );

			OnLeaveCellMshflexgrid1();
			
			break;
	}
}

// 입력화면일경우 이전입력값을 그리드에 셋팅한다.
VOID CESL_WorkSheet_dlg::SetControls()
{
	for( INT i = 0; i < m_controlCnt; i++ )
		if( _T("M") != m_ctrlInfo[ i ].ctrl_type )
			SetGrid1String( i + 1, 2, m_ctrlInfo[ i ].value );
		else
			SetGrid1String( i + 1, 2, m_ctrlInfo[ i ].desc  );
}

// 전체 control의 개수를 받아온다. 
INT CESL_WorkSheet_dlg::GetAllControlCount()
{
	return m_pCM->Control_List.GetCount();
}

// 전체 control의 alias를 받아온다. 
INT CESL_WorkSheet_dlg::GetAllControlAlias( CString *alias )
{
	INT index = 0;
	INT cecount = m_pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = m_pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE) {
			alias[index++] = pCE->CTRL_ALIAS;
		}
	}
	return 0;
}

// 두번째 컨트롤을 셋팅한다.
INT CESL_WorkSheet_dlg::SetSecondTabControl()
{
	SetRadioButton( m_workSheetType );

	INT rows; 
	CString *allControlAlias;
	allControlAlias = GetAllControlInfo( rows );	
	
	SetGrid_2( rows, allControlAlias );

	CheckViewControlList();
	
	delete [] allControlAlias;

	return 0;
}

// 두번째 탭이 보여질때, 이전에 설정된 보여줄 control들을 체크한다. 
INT CESL_WorkSheet_dlg::CheckViewControlList()
{
	INT count;
	CString *alias;
	alias = GetViewControInfo( count );

	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		SetGrid2String( i, 1, _T("V") );
	
	CString tmp;
	for( i = 1; i < m_grid_2.GetRows(); i++ )
		for( INT j = 0; j < count; j++ )
			if( alias[ j ] == GetGrid2String( i, 2 ) && alias[ j ])
				SetGrid2String( i, 1, _T("") );
			
	delete [] alias;
	
	return 0;
}

// 그리드에서 입력할 control항목 list정보를 받아온다. 
CString * CESL_WorkSheet_dlg::GetViewControInfo( INT &count )
{
	CString query;
	query.Format( _T("SELECT ALIAS FROM ESL_WORKSHEET_SUB_TBL WHERE CM_KEY = '%s'"), GetCMKey() );

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	
	dm.RestructDataManager( query );

	count = dm.GetRowCount();
	CString *alias = new CString[ count ];

	for( INT i = 0; i < count; i++ )
		dm.GetCellData( i, 0, alias[ i ] );
	
	return alias;
}

// 설정 그리드의 목록을 구성한다. 
INT CESL_WorkSheet_dlg::SetGrid_2(INT rows, CString *alias)
{
	m_grid_2.SetRows( rows + 1 );
	m_grid_2.SetCols( 0, 3 );

	SetGrid2String( 0, 1, _T("선택") );
	SetGrid2String( 0, 2, _T("Alias") );

	CString row;
	for( INT i = 0; i< rows; i++ )
	{
		row.Format( _T("%d"), i + 1 );
		SetGrid2String( i + 1, 0, row );
		SetGrid2String( i + 1, 2, alias[ i ] );
	}
	
	return 0;
}

// 설정 그리드가 클릭됐을때,
VOID CESL_WorkSheet_dlg::OnMouseDownMshflexgrid2( short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	
	if( 1 != m_grid_2.GetCol() || 0 == m_grid_2.GetRow() )
		return;
	
	CString check;
	check = m_grid_2.GetText();

	if( check == _T("V") )
		m_grid_2.SetText( _T("") );
	else
		m_grid_2.SetText( _T("V") );
}

// 설정 그리드의 ( x, y )에 문자열을 넣는다. 
VOID CESL_WorkSheet_dlg::SetGrid2String( INT row, INT col, CString string )
{
	m_grid_2.SetRow( row );
	m_grid_2.SetCol( col );
	m_grid_2.SetText( string );
}

// 입력 그리드의 ( x, y )에 문자열을 넣는다. 
VOID CESL_WorkSheet_dlg::SetGrid1String( INT row, INT col, CString string )
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );
	m_grid_1.SetText( string );	
}

// 설정 그리드의 ( x, y )에서 문자열을 받아온다.
CString CESL_WorkSheet_dlg::GetGrid2String(INT row, INT col)
{
	m_grid_2.SetRow( row );
	m_grid_2.SetCol( col );

	return m_grid_2.GetText();
}

// 입력 그리드의 ( x, y )에서 문자열을 받아온다.
CString CESL_WorkSheet_dlg::GetGrid1String(INT row, INT col)
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );

	return m_grid_2.GetText();
}

// 전체 control의 개수와 alias를 얻어온다.
CString * CESL_WorkSheet_dlg::GetAllControlInfo( INT &count )
{
	count = GetAllControlCount();
	CString *alias;
	alias = new CString[ count ];

	GetAllControlAlias( alias );

	return alias;
}

// Radio Button 체크하기.
VOID CESL_WorkSheet_dlg::SetRadioButton( INT flag )
{
	CButton *r1 = ( CButton * )GetDlgItem( IDC_RADIO1_COM );
	CButton *r2 = ( CButton * )GetDlgItem( IDC_RADIO2_COM );
	CButton *r3 = ( CButton * )GetDlgItem( IDC_RADIO3_COM );
	CButton *r4 = ( CButton * )GetDlgItem( IDC_RADIO4_COM );

	switch( flag )
	{
		case 1 :
			r1->SetCheck( true );
			break;
		case 2 :
			r2->SetCheck( true );
			break;
		case 3 :
			r3->SetCheck( true );
			break;
		case 4 :
			r4->SetCheck( true );
			break;
		default :
			break;
	}
}

// 설정 -> 확인
VOID CESL_WorkSheet_dlg::On2() 
{
	// TODO: Add your control notification handler code here
		
	INT uncheckedRowsCount = 0;
	CString *uncheckedRowAlias = GetUncheckedRowInfo( uncheckedRowsCount );

	DeleteBeforeConfigureData();
	SaveConfigureData( uncheckedRowAlias, uncheckedRowsCount );
	
	delete [] uncheckedRowAlias;

	MessageBox( _T("변경사항이 적용되었습니다.") , _T("Warnning!"), MB_ICONEXCLAMATION);
	
	m_change = true;
	if( m_ctrlInfo != NULL )
	{
		delete [] m_ctrlInfo;
		m_ctrlInfo = NULL;
	}

}

// 이전 설정 값들을 모두 지운다. 
INT CESL_WorkSheet_dlg::DeleteBeforeConfigureData()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString query;
	CString cmKey = GetCMKey();

	if( cmKey.GetLength() > 0 )
	{
		query.Format( _T("DELETE FROM ESL_WORKSHEET_TBL WHERE PK = %s;"), cmKey );
		dm.ExecuteQuery( query, 1);

		query.Format( _T("DELETE FROM ESL_WORKSHEET_SUB_TBL WHERE CM_KEY = %s;"), cmKey );
		dm.ExecuteQuery( query, 1);
	}
	
	return 0;
}

// ESL_WORKSHEET_TBL의 CM_KEY를 얻어온다. 
CString CESL_WorkSheet_dlg::GetCMKey()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString query, cmKey;

	query.Format( _T("SELECT PK FROM ESL_WORKSHEET_TBL WHERE CM_ALIAS = '%s'"), m_cmAlias );
	dm.GetOneValueQuery( query, cmKey );

	return cmKey;
}

// 설정된 내용을 DB에 저장한다. 
INT CESL_WorkSheet_dlg::SaveConfigureData( CString *alias, INT count )
{
	/*
	CREATE TABLE ESL_WORKSHEET_TBL
	(
		PK			INTEGER	NOT NULL,
		TYPE		INTEGER,
		K2U_USER	VARCHAR( 255 ),
		CM_ALIAS	VARCHAR( 255 )
	);
	*/

	CString pk = GetPK();
	
	const INT fieldCount = 4;
	
	CString fieldName[ fieldCount ] = {
		_T("PK"), _T("TYPE"), _T("K2U_USER"), _T("CM_ALIAS") };
	
	CString fieldType[ fieldCount ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING") };
	
	CString fieldData[ fieldCount ];
	INT workSheetType = GetWorkSheetType();
	fieldData[ 0 ].Format( _T("%s"), pk );
	fieldData[ 1 ].Format( _T("%d"), workSheetType );
	fieldData[ 2 ].Format( _T("%s"), m_pInfo->USER_ID );
	fieldData[ 3 ].Format( _T("%s"), m_cmAlias );

	CESL_Mgr *pEslMgr = ( CESL_Mgr * )m_wnd;
	pEslMgr->m_workSheetType = workSheetType;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	dm.StartFrame();
	dm.InitDBFieldData();

	for( INT i = 0; i < fieldCount; i++ )
		dm.AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );
	dm.MakeInsertFrame( _T("ESL_WORKSHEET_TBL") );

	if( dm.SendFrame() < 0 )
	{
		MessageBox( _T("Error : CESL_WorkSheet_dlg::SaveConfigureData( CString *alias, INT count )") , _T("Warnning!"), MB_ICONEXCLAMATION);
		return -1;
	}

	dm.EndFrame();

	if( SaveConfigureSubData( pk, alias, count ) )
	{
		MessageBox( _T("Error : CESL_WorkSheet_dlg::SaveConfigureSubData(CString pk, CString *alias, INT count)") , _T("Warnning!"), MB_ICONEXCLAMATION);
		return -2;
	}
	
	return 0;
}

INT CESL_WorkSheet_dlg::SaveConfigureSubData( CString pk, CString *alias, INT count )
{
	/*
	CREATE TABLE ESL_WORKSHEET_SUB_TBL
	(
		PK			INTEGER NOT NULL,
		CM_KEY		INTEGER,
		ALIAS		VARCHAR( 255 )
	);
	*/

	const INT fieldCount = 3;

	CString fieldName[ fieldCount ] = {
		_T("PK"), _T("CM_KEY"), _T("ALIAS") };

	CString fieldType[ fieldCount ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING") };

	CString fieldData[ fieldCount ];

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	for( INT i = 0; i < count; i++ )
	{
		dm.StartFrame();
		dm.InitDBFieldData();

		fieldData[ 0 ].Format( _T("%s"), GetPK() );
		fieldData[ 1 ].Format( _T("%s"), pk );
		fieldData[ 2 ].Format( _T("%s"), alias[ i ] );		
		
		for( INT j = 0; j < fieldCount; j++ )
			dm.AddDBFieldData( fieldName[ j ], fieldType[ j ], fieldData[ j ] );
		dm.MakeInsertFrame( _T("ESL_WORKSHEET_SUB_TBL") );

		if( dm.SendFrame() < 0 )
			return -1;
		dm.EndFrame();
	}

	return 0;
}

// Primary key를 얻어온다. 
CString CESL_WorkSheet_dlg::GetPK()
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString pk;
	dm.MakeRecKey( pk );

	return pk;
}

// 설정 그리드에서 체크되지 않은 row의 개수와 행 번호를 받아온다. 
CString * CESL_WorkSheet_dlg::GetUncheckedRowInfo( INT &rowCount )
{
	CString *alias;
	rowCount = GetUncheckedRowCount();
	alias = new CString[ rowCount ];
	
	INT index = 0;
	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		if( _T("V") != GetGrid2String( i, 1 ) )
			alias[ index++ ].Format( _T("%s"), GetGrid2String( i, 2 ) );
	
	return alias;
}

// 설정 그리드에서 체크되지 않은 행의 개수를 받아온다. 
INT CESL_WorkSheet_dlg::GetUncheckedRowCount()
{
	INT count = 0;
	
	for( INT i = 1; i < m_grid_2.GetRows(); i++ )
		if( _T("V") != GetGrid2String( i, 1 ) )
			count++;

	return count;
}

// 첫번째 컨트롤을 셋팅한다.
INT CESL_WorkSheet_dlg::SetFirstTabControl()
{
	if( false == m_change )
		return 0;

	INT count;
	CString *alias;
	alias = GetViewControlInfo( count );
	m_controlCnt = count;

	if( NULL != m_ctrlInfo )
	{
		delete m_ctrlInfo;
		m_ctrlInfo = NULL;
	}
	m_ctrlInfo = new ControlInfo[ count ];
			
	m_change = false;
	
	GetInputControlInfo( alias, count );
	GetControlValue();
	SetInputGridCaption( alias, count );
	
	delete [] alias;

	return 0;
}

// 그리드의 선택된 cell의 rect를 구한다. 
CRect CESL_WorkSheet_dlg::GetGridCellRect()
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );

	CRect rect, gridRect;
	RECT parentRect;
	wnd->GetWindowRect( &gridRect );
	wnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( wnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left +	( long )( ( float )m_grid_1.GetCellLeft()	/ 15.0f );
	rect.top	= gridRect.top +	( long )( ( float )m_grid_1.GetCellTop()	/ 15.0f );
	rect.right	= rect.left +		( long )( ( float )m_grid_1.GetCellWidth()	/ 15.0f );
	rect.bottom = rect.top +		( long )( ( float )m_grid_1.GetCellHeight() / 15.0f );

	return rect;
}

// row행에 EditBox를 생성한다.
INT CESL_WorkSheet_dlg::CreateEditBox( INT row )
{
	if( NULL != m_editBox )
	{
		delete m_editBox;
		m_editBox = NULL;
	}
	m_editBox = new CESL_GridSelEditor;
		
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );
	
	m_editBox->row = row + 2;
	m_editBox->col = 2;
	m_editBox->parent = GetDlgItem( IDC_MSHFLEXGRID_1 );
	m_editBox->Create( ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD, CRect( 0, 0, 10, 20 ), wnd->GetParent(), 645 );

	CRect rect = GetGridCellRect();
	m_editBox->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_editBox->ShowWindow( true );
	m_editBox->SetFocus();

	m_editBox->SetWindowText( m_ctrlInfo[ row - 1 ].value );

	return 0;
}

// row행에 DateBox를 생성한다.
INT CESL_WorkSheet_dlg::CreateDateBox( INT row )
{
	if( NULL != m_dateBox )
	{
		delete m_dateBox;
		m_dateBox = NULL;
	}
	m_dateBox = new CDateTimeCtrl;
	
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );

	m_dateBox->Create( DTS_APPCANPARSE | DTS_LONGDATEFORMAT | WS_CLIPSIBLINGS | WS_OVERLAPPED, CRect( 0, 0, 10, 20 ), wnd, 646 );
						
	CRect rect = GetGridCellRect();
	m_dateBox->MoveWindow( rect.left - 24, rect.top - 44, rect.Width() + 2, rect.Height() + 2 );

	m_dateBox->ShowWindow( true );
	m_dateBox->SetFocus();
	
	SetBeforeDate( row );

	return 0;
}

// row행에 MultiComboBox를 생성한다.
INT CESL_WorkSheet_dlg::CreateMultiComboBox( INT row )
{
	CESL_Control_Element *ctrlElement = GetCtrlElement( row );

	if( NULL != m_multiComboBox )
		m_multiComboBox->ELEMENT = ctrlElement;
	else
	{
		m_multiComboBox = new CESL_ControlMultiComboBox( ctrlElement );	
		m_multiComboBox->SubclassDlgItem( IDC_MUUTICOMBO_BOX, this );
	}
	
	// WJChoi - (Start) - 콤보박스에 데이타가 있으면 모두 지워준다. 
	// 일단. CJStruct List를 모두 날려준후
	INT nRow = m_multiComboBox->m_jcbList.GetCount();
	CJStruct *pRow;
	for( INT i = 0; i < nRow; i++ )
	{
		pRow = ( CJStruct * )m_multiComboBox->m_jcbList.RemoveHead();
		if( pRow )
			delete pRow;
	}
	m_multiComboBox->m_jcbList.RemoveAll();
	// JComboBox를 초기화 하고
	m_multiComboBox->PrevRow = -1;
	m_multiComboBox->PrevCol = -1;
	m_multiComboBox->m_nMaxRowCnt = 0;

	// CComboBox를 초기화 한다.
	if( m_multiComboBox->GetCount() > 0 )
		m_multiComboBox->ResetContent();
	// WJChoi - ( END ) - 콤보박스에 데이타가 있으면 모두 지워준다. 

	m_multiComboBox->InitMultiComboBoxRef();

	INT rowCount = m_multiComboBox->GetRowCount();

	CRect rect = GetGridCellRect();
	CComboBox *comboBox = ( CComboBox * )GetDlgItem( IDC_MUUTICOMBO_BOX );
	comboBox->MoveWindow( rect.left, rect.top, rect.Width() + 2, rect.Height() * rowCount );
	
	m_multiComboBox->ShowWindow( true );
	
	delete ctrlElement;
	
	return 0;
}

// 이전에 입력된 날짜값을 새로 생성되는 dateBox에 넣는다. 
VOID CESL_WorkSheet_dlg::SetBeforeDate( INT row )
{
	CString str = m_ctrlInfo[ row - 1 ].value;
	if( !str.GetLength() )
		return;

	COleDateTime day;
	INT y = 0, m = 0, d = 0;

	_stscanf( str.GetBuffer( 0 ), _T("%d/%d/%d\0"), &y, &m, &d );
	day.SetDate( y, m, d );
	m_dateBox->SetTime( day );
	UpdateData( true );
}

// MultiComboBox를 생성하기 위한 CESL_Control_Element 를 받아온다.
CESL_Control_Element * CESL_WorkSheet_dlg::GetCtrlElement( INT index )
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_MSHFLEXGRID_1 );
	
	CESL_Control_Element * ctrlElement;
	ctrlElement = new CESL_Control_Element( this );

	ctrlElement->WorkWnd		= m_wnd;
	ctrlElement->CM_ALIAS		= m_cmAlias;
	ctrlElement->CTRL_ID		= IDC_MUUTICOMBO_BOX;
	ctrlElement->CTRL_TYPE		= _T("M");
	ctrlElement->CTRL_STYLE		= m_ctrlInfo[ index - 1 ].ctrl_style;
	ctrlElement->INIT_VALUE		= m_ctrlInfo[ index - 1 ].init_value;
	ctrlElement->LIST_DATA		= m_ctrlInfo[ index - 1 ].list_data;
	ctrlElement->LINK_DATA		= m_ctrlInfo[ index - 1 ].link_data;
	ctrlElement->VALIDATION		= m_ctrlInfo[ index - 1 ].validation;
	ctrlElement->REQUIRECHECK	= m_ctrlInfo[ index - 1 ].requirecheck;

	return ctrlElement;
}

// 그리드에서 셀이 봐뀌었을때,
VOID CESL_WorkSheet_dlg::OnEnterCellMshflexgrid1() 
{
	// TODO: Add your control notification handler code here

	INT row = m_grid_1.GetRow();
	INT col = m_grid_1.GetCol();

	if( col != 2 )
		return;

	switch( GetControlType( row ) )
	{
		case 'E' :
			CreateEditBox( row );
			break;

		case 'D' :
			CreateDateBox( row );
			break;

		case 'M' :
			CreateMultiComboBox( row );
			break;
	}	
}

// 그리드 또는 control배열의 row번째 control type을 얻어온다. 
TCHAR CESL_WorkSheet_dlg::GetControlType( INT row, INT option )
{
	// option == 1 : 그리드 행	( default )
	// option == 0 : ControlInfo class의 index

	TCHAR type;
	wmemcpy( &type, &m_ctrlInfo[ row - 1 ].ctrl_type.GetBuffer( 0 )[ 0 ], 1 );

	return type;
}


// 입력 그리드의 행번호, 열제목, alias를 넣는다. 
INT CESL_WorkSheet_dlg::SetInputGridCaption( CString *alias, INT count )
{
	if( count < 1 )
		m_grid_1.SetRows( 2 );
	else
		m_grid_1.SetRows( count + 1 );
	m_grid_1.SetCols( 0, 3 );

	SetGrid1String( 0, 1, _T("Alias") );
	SetGrid1String( 0, 2, _T("Value") );

	CString tmp;
	for( INT i = 0; i < count; i++ )
	{
		tmp.Format( _T("%d"), i + 1 );
		SetGrid1String( i + 1, 0, tmp );
		SetGrid1String( i + 1, 1, alias[ i ] );

		m_grid_1.SetRowHeight( i + 1, 400 );
	}

	return 0;
}

// 입력받을 control의 정보를 얻어온다. 
INT CESL_WorkSheet_dlg::GetInputControlInfo( CString *alias, INT count )
{
	INT index = 0;
	INT cecount = m_pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = m_pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		
		for( INT j = 0; j < count; j++ )
			// KOL.UDF.022
			//if( pCE->CTRL_ALIAS == alias[ j ] && pCE )
			if( &pCE != NULL && pCE->CTRL_ALIAS == alias[ j ] )
			{
				m_ctrlInfo[ index ].ctrl_alias	= pCE->CTRL_ALIAS;
				m_ctrlInfo[ index ].ctrl_type	= pCE->CTRL_TYPE;
				m_ctrlInfo[ index ].ctrl_id		= pCE->CTRL_ID;
				m_ctrlInfo[ index ].ctrl_style	= pCE->CTRL_STYLE;
				m_ctrlInfo[ index ].init_value	= pCE->INIT_VALUE;
				m_ctrlInfo[ index ].list_data	= pCE->LIST_DATA;
				m_ctrlInfo[ index ].link_data	= pCE->LINK_DATA;
				m_ctrlInfo[ index ].validation	= pCE->VALIDATION;
				m_ctrlInfo[ index ].requirecheck= pCE->REQUIRECHECK;

				index++;
			}
		}	
	
	return 0;
}

// 수정시 이전 콘트롤에 있는 값들을 받아온다.
INT CESL_WorkSheet_dlg::GetControlValue()
{
	for( INT i = 0; i < m_controlCnt; i++ )
		if( 'M' != m_ctrlInfo[ i ].ctrl_type )
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value );
		else
		{
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value, -1, 0 );
			m_ctrlMgr->GetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].desc,  -1, 1 );
		}
	
	return 0;
}

// 입력화면에 보여줄 control목록 정보를 얻어온다. 
CString * CESL_WorkSheet_dlg::GetViewControlInfo( INT &count )
{
	INT allControlCount;
	CString *allControlAlias;
	allControlAlias = GetAllControlInfo( allControlCount );

	INT hiddenControlCount;
	CString *hiddenControlAlias;
	hiddenControlAlias = GetViewControInfo( hiddenControlCount );

	count = allControlCount - hiddenControlCount;
	CString *alias = new CString[ count ];
	
	INT index = 0;
	bool check = false;
	for( INT i = 0; i < allControlCount; i++ )
	{
		for( INT j = 0; j < hiddenControlCount; j++ )
			if( allControlAlias[ i ] == hiddenControlAlias[ j ] )
			{
				check = true;
				continue;
			}

		if( false == check )
			alias[ index++ ] = allControlAlias[ i ];
		check = false;
	}

	delete [] allControlAlias;
	delete [] hiddenControlAlias;

	return alias;
}

// 찾이 닫힐때 control에 대한 정보 class에 대한 메모리 해제
VOID CESL_WorkSheet_dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}


// 입력그리드에서 셀을 떠날때,
VOID CESL_WorkSheet_dlg::OnLeaveCellMshflexgrid1() 
{
	// TODO: Add your control notification handler code here
	
	CString str;
	COleDateTime day;

	INT row = m_grid_1.GetRow();
	INT col = m_grid_1.GetCol();

	if( 2 != col )
		return;

	switch( GetControlType( row ) )
	{
		case 'E' :
			
			if( NULL == m_editBox )
				return;

			m_editBox->GetWindowText( str );
			SetGrid1String( row, col, str );

			delete m_editBox;
			m_editBox = NULL;

			m_ctrlInfo[ row - 1 ].value = str;
			
			break;
	
		case 'D' :

			if( NULL == m_dateBox )
				return;

			m_dateBox->GetTime( day );
			
			m_ctrlInfo[ row - 1 ].value.Format( _T("%04d/%02d/%02d"), day.GetYear(), day.GetMonth(), day.GetDay() );
			SetGrid1String( row, col, m_ctrlInfo[ row - 1 ].value );
			
			delete m_dateBox;
			m_dateBox = NULL;

			break;
		
		case 'M' :

			m_multiComboBox->GetData( m_ctrlInfo[ row - 1 ].value, -1, 0 );
			m_multiComboBox->GetData( m_ctrlInfo[ row - 1 ].desc,  -1, 1 );

			SetGrid1String( row, col, m_ctrlInfo[ row - 1 ].desc );
			
			m_multiComboBox->ShowWindow( false );
			
			break;
		
		default :
			break;
	}
}

// 그리드 안에서의 key 처리.
BOOL CESL_WorkSheet_dlg::PreTranslateMessage( MSG* pMsg ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( m_tab.GetCurSel() )
		return CDialog::PreTranslateMessage( pMsg );
	
	if( WM_KEYDOWN == pMsg->message )
		switch( pMsg->wParam )
		{
			case VK_TAB :
				
				INT row, col;
				GetGridFocus( row, col );
				if( 2 != col )
					return CDialog::PreTranslateMessage( pMsg );

				MoveGridCell();
				return true;
	
				break;

			case VK_ESCAPE :
				
				OnLeaveCellMshflexgrid1();
				MoveGridCell( 1 );
				
				return true;

				break;
			
			default :
				break;
		}
	
	return CDialog::PreTranslateMessage( pMsg );
}

// 그리드의 현재 선택된 셀위치를 받아온다. 
VOID CESL_WorkSheet_dlg::GetGridFocus( INT &row, INT &col )
{
	row = m_grid_1.GetRow();
	col = m_grid_1.GetCol();
}

// row행, col열에 위치하게 한다. 
VOID CESL_WorkSheet_dlg::SetGridFocus( INT row, INT col )
{
	m_grid_1.SetRow( row );
	m_grid_1.SetCol( col );
}

// 다음 열로 이동한다. 
VOID CESL_WorkSheet_dlg::MoveGridCell( INT option )
{
	INT rows, row, col;
	rows = m_grid_1.GetRows();
	GetGridFocus( row, col );
	
	OnLeaveCellMshflexgrid1();

	switch( option )
	{
		case 0 :
			if( row + 1 == rows )
				SetGridFocus( 1, 2 );
			else
				SetGridFocus( row + 1, 2 );

			break;
		
		case 1 :
			SetGridFocus( 0, 0 );

			break;
	}

	OnEnterCellMshflexgrid1();	
}


// 멀티콤보의 셀이 변경됐을때,
VOID CESL_WorkSheet_dlg::OnSelchangeMuuticomboBox() 
{
	// TODO: Add your control notification handler code here
	m_grid_1.SetEnabled( true );	
	
	if( NULL != m_multiComboBox )
	{
		//delete m_multiComboBox;
		//m_multiComboBox = NULL;
	}	
}

// 현재 그리드에 입력된 내용을 저장한다. 
INT CESL_WorkSheet_dlg::SaveWorkSheetData()
{
	MoveGridCell( 1 );
	
	for( INT i = 0; i < m_controlCnt; i++ )
		m_ctrlMgr->SetControlMgrData( m_ctrlInfo[ i ].ctrl_alias, m_ctrlInfo[ i ].value, -1, 0 );

	return 0;
}

// worksheet의 데이타를 다시 읽어온다
INT CESL_WorkSheet_dlg::RefreshWorkSheetData( VOID )
{
	GetControlValue();
	SetControls();
	
	return 0;
}

VOID CESL_WorkSheet_dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1905 )
	{
		KillTimer( 1905 );
		m_grid_1.SetRow( 1 );
		m_grid_1.SetCol( 0 );
		m_grid_1.SetFocus();
		UpdateData( false );
	}

	CDialog::OnTimer(nIDEvent);
}

INT CESL_WorkSheet_dlg::SetRect(CRect rect)
{
	m_rect = rect;
	return 0;
}

CRect CESL_WorkSheet_dlg::GetRect(VOID)
{
	return m_rect;
}

INT CESL_WorkSheet_dlg::ReSizeWindow(VOID)
{
	CWnd *pWnd;
	
	pWnd = GetDlgItem( IDC_TAB_COM );
	if( pWnd )
		pWnd->MoveWindow( &m_rect, true );

	pWnd = GetDlgItem( IDC_STATIC1111 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 10, m_rect.top + 20, m_rect.Width() - 20, m_rect.Height() - 30, true );

	pWnd = GetDlgItem( IDC_MSHFLEXGRID_1 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 20, m_rect.top + 40, m_rect.Width() - 40, m_rect.Height() - 60, true );

	pWnd = GetDlgItem( IDC_MSHFLEXGRID_2 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.left + 20, m_rect.top + 130, m_rect.Width() - 40, m_rect.Height() - 190, true );
	
	pWnd = GetDlgItem( IDOK_2 );
	if( pWnd )
		pWnd->MoveWindow( m_rect.right - 20 - 50, m_rect.bottom - 50, 50, 30, true );

	return 0;
}
