// MNG_MNGVALUE_OPERATOR_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGVALUE_OPERATOR_01.h"
#include "GridEdt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_01 dialog


CMNG_MNGVALUE_OPERATOR_01::CMNG_MNGVALUE_OPERATOR_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGVALUE_OPERATOR_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_MNGVALUE_OPERATOR_01::~CMNG_MNGVALUE_OPERATOR_01()
{
	
}
BOOL CMNG_MNGVALUE_OPERATOR_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_MNGVALUE_OPERATOR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGVALUE_OPERATOR_01)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGVALUE_OPERATOR_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGVALUE_OPERATOR_01)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_CLOSE()
	ON_MESSAGE(UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_01 message handlers

BOOL CMNG_MNGVALUE_OPERATOR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MNGVALUE_OPERATOR_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_01") );
		return false;
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MNGVALUE_OPERATOR_01"), _T("그리드") );
	if( pGrid )
	{
		//RemoveListArray.Add( _T("") );

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}

	SetTabCtrl();
	m_type = 0;
	m_curAliasCnt = 0;
	m_pGridEdt = NULL;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MNGVALUE_OPERATOR_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	// INT CESL_Mgr::GetManageValue( CString group_2, CString group_3, CString alias, CString valueName, CString &value )

	ShowGrid();
}

// CM_MNG_MNGVALUE_OPERATOR_01
// DM_MNG_MNGVALUE_OPERATOR_01

VOID CMNG_MNGVALUE_OPERATOR_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( 15, 40, cx - 25, cy - 55 );
	pGrid->MoveWindow(1, 60, cx-1, cy-75);

	//m_tab.MoveWindow( 2, 3, cx - 2, cy - 3 );
	m_tab.MoveWindow(0, 0, cx+4, cy);
}

// 변경 사항 적용
VOID CMNG_MNGVALUE_OPERATOR_01::UpdateModifiedData()
{
	const INT fieldCnt = 14;
	
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("GROUP_1"), 
		_T("GROUP_2"), 
		_T("GROUP_3"), 
		_T("GROUP_4"), 
		_T("GROUP_5"), 
		_T("CLASS_ALIAS"), 
		_T("CLASS_DESC"), 
		_T("VALUE_NAME"), 
		_T("VALUE_2"), 
		_T("VALUE_3"), 
		_T("FIRST_WORK"), 
		_T("VALUE_1"), 
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CString data[ fieldCnt ];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_01") );
	INT rows = pDM->GetRowCount();

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	data[ 13 ] = GetWorkLogMsg( _T(""), __WFILE__, __LINE__ );
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	pGrid->SetCol( 5 );

	for( INT i = 0; i < rows; i++ )
	{
		pDM->GetCellData( fieldName, fieldCnt - 2, data, i );
		pGrid->SetRow( i + 1 );
		data[ 12 ] = pGrid->GetText();

		for( INT j = 0; j < fieldCnt; j++ )
			pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		pDM->MakeUpdateFrame( _T("ESL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();	
	
	pGrid->SetRow( row );
	pGrid->SetCol( col );
	
	AfxMessageBox( _T("변경사항이 적용되었습니다.") );
}

VOID CMNG_MNGVALUE_OPERATOR_01::SetTabCtrl()
{
	m_tab.InsertItem( 0, _T(" 수 서 ") );
	m_tab.InsertItem( 1, _T(" 열 람 ") );
	m_tab.InsertItem( 2, _T(" 정 리 ") );
	m_tab.InsertItem( 3, _T(" 기 타 ") );	
}

VOID CMNG_MNGVALUE_OPERATOR_01::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	m_type = m_tab.GetCurSel();
	ShowGrid();
}

VOID CMNG_MNGVALUE_OPERATOR_01::ShowGrid( VOID )
{
	/*
	0 : 수서
	1 : 열람
	2 : 정리
	3 : 기타
	*/

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_01") );
	pDM->RefreshDataManager( MakeSelectQuery() );

	ControlDisplay( _T("CM_MNG_MNGVALUE_OPERATOR_01"), _T("그리드") );
}

CString CMNG_MNGVALUE_OPERATOR_01::MakeSelectQuery( VOID )
{
	CString where, group;

	switch( m_type )
	{
		case 0 :
			group = _T("수서");
			break;
		case 1 :
			group = _T("열람");
			break;
		case 2 :
			group = _T("정리");
			break;
		case 3 :
			group = _T("기타");
			break;
	}

	where.Format( _T(" GROUP_2 = '%s' "), group );
	return where;
}

BEGIN_EVENTSINK_MAP(CMNG_MNGVALUE_OPERATOR_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_MNGVALUE_OPERATOR_01)
	ON_EVENT(CMNG_MNGVALUE_OPERATOR_01, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_MNGVALUE_OPERATOR_01::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	SetGridProperty();
}

VOID CMNG_MNGVALUE_OPERATOR_01::SetGridProperty( VOID )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		
	INT gridRow = pGrid->GetRow();
	INT gridCol = pGrid->GetCol();
	INT dmRow = gridRow - 1;
	FreeGridEdt();

	if( !gridRow )
		return;
	
	if( 5 == gridCol )
		ConvertValue( dmRow );

	SetSameAliasColor( dmRow );

	pGrid->SetRow( gridRow );
	pGrid->SetCol( gridCol );
}

VOID CMNG_MNGVALUE_OPERATOR_01::SetSameAliasColor( INT dmRow )
{
	CString alias, tmp;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_01") );
	pDM->GetCellData( _T("CLASS_ALIAS"), dmRow, alias );
	
	CArray< INT, INT > rows;

	INT idx = dmRow - 1;
	pDM->GetCellData( _T("CLASS_ALIAS"), idx, tmp );
	while( tmp == alias && idx > -1 )
	{
		rows.Add( idx );

		pDM->GetCellData( _T("CLASS_ALIAS"), --idx, tmp );
	}

	idx = dmRow;
	INT dmRowCnt = pDM->GetRowCount();
	pDM->GetCellData( _T("CLASS_ALIAS"), idx, tmp );
	while( tmp == alias && idx < dmRowCnt )
	{
		rows.Add( idx );

		pDM->GetCellData( _T("CLASS_ALIAS"), ++idx, tmp );
	}

	INT cnt = m_curAliasCnt = rows.GetSize();
	INT *row = ( INT * )calloc( cnt, sizeof( INT ) );

	for( INT i = 0; i < cnt; i++ )
		row[ i ] = rows.GetAt( i );
	SetGridBackColor( row, cnt );

	free( row );
}

VOID CMNG_MNGVALUE_OPERATOR_01::SetGridBackColor( INT *row, INT cnt )
{
	COLORREF blue, white, black, backColor;
	blue  = RGB( 228, 243, 251 );
	white = RGB( 255, 255, 255 );
	black = RGB( 0,   0,   0   );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT rows = pGrid->GetRows();

	INT i, j;
	for( i = 1; i < rows; i++ )
	{
		backColor = white;
		pGrid->SetRow( i );
		
		for( j = 0; j < cnt; j++ )
			if( i - 1 == row[ j ] )
			{
				backColor = blue;
				break;
			}

		for( j = 1; j < 6; j++ )
		{
			pGrid->SetCol( j );
			pGrid->SetCellBackColor( backColor );
		}
	}
}

INT CMNG_MNGVALUE_OPERATOR_01::ConvertValue( INT dmRow )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	CString value, newValue;
	value = pGrid->GetText();

	if( _T("N") == value )
		pGrid->SetText( _T("Y") );
	else if( _T("Y") == value )
		pGrid->SetText( _T("N") );
	else
		GetGridCellNewValue( value, dmRow + 1 );
	
	return 0;
}

VOID CMNG_MNGVALUE_OPERATOR_01::GetGridCellNewValue( CString value, INT row )
{
	FreeGridEdt();
	m_pGridEdt = new CGridEdt;
	
	m_pGridEdt->row = row;
	m_pGridEdt->col = 5;
	
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

VOID CMNG_MNGVALUE_OPERATOR_01::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	FreeGridEdt();
	CDialog::OnClose();
}

VOID CMNG_MNGVALUE_OPERATOR_01::FreeGridEdt()
{
	if( m_pGridEdt )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}
}

VOID CMNG_MNGVALUE_OPERATOR_01::SetGridText( INT row, INT col, CString text )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();

	pGrid->SetRow( row );
	pGrid->SetCol( col );
	pGrid->SetText( text );

	pGrid->SetRow( curRow );
	pGrid->SetCol( curCol );
}

VOID CMNG_MNGVALUE_OPERATOR_01::OnUpdateGridCelText( WPARAM w, LPARAM l )
{
	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}

	FreeGridEdt();
}
HBRUSH CMNG_MNGVALUE_OPERATOR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


