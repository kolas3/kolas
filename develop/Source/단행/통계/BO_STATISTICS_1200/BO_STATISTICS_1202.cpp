// BO_STATISTICS_1202.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATISTICS_1202.h"
#include "ChkListbox.h"

#include "BO_STATISTICS_1201.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATISTICS_1202 dialog


CBO_STATISTICS_1202::CBO_STATISTICS_1202( CString strMenuAlias,CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CBO_STATISTICS_1202)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pChkList = NULL;
	m_pParent = pParent;
}

CBO_STATISTICS_1202::~CBO_STATISTICS_1202()
{	
	if( m_pChkList )
		delete m_pChkList;
}
  
VOID CBO_STATISTICS_1202::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATISTICS_1202)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_STATISTICS_1202, CDialog)
	//{{AFX_MSG_MAP(CBO_STATISTICS_1202)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_rSTATION, OnrSTATION)
	ON_BN_CLICKED(IDC_rSHELFCODE, OnrSHELFCODE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATISTICS_1202 message handlers

BOOL CBO_STATISTICS_1202::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("BO_STATISTICS_1202") ) < 0 )
		return false;
	
	MakeDefaultInfo();
	InitSetCtrls();
	SetDefaultDate();
	SetDefaultOpt();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_STATISTICS_1202::InitSetCtrls()
{
	MakeCheckListCtrl();
	
}

VOID CBO_STATISTICS_1202::MakeCheckListCtrl()
{
	const INT l = 20;
	const INT t = 218;
	const INT r = 413;
	const INT b = 346;

	m_pChkList = new CCheckList();
	if( !m_pChkList->Create( CRect( l, t, r, b ), ( CWnd * )this, CHECK_LIST_BOX, IDB_bCHECK, IDB_bUNCHECK, IDB_bMIDDLECHECK ) )
		return;
	m_pChkList->ShowWindow( true );
}

VOID CBO_STATISTICS_1202::MakeDefaultInfo()
{
	CESL_DataMgr* pDM;
	INT i = 0;
	
	CString tmp;
	pDM = FindDM( _T("DM_BO_STATISTICS_1201_STATION") );
	pDM->RefreshDataManager( _T("") );
	for( i = 0; i < pDM->GetRowCount(); i++ )
		m_station.AddTail( pDM->GetCellData( _T("NAME"), i ) );
		
	pDM = FindDM( _T("DM_BO_STATISTICS_1201_SHELFCODE") );
	pDM->RefreshDataManager( _T("") );
	for( i = 0; i < pDM->GetRowCount(); i++ )
		m_shelfCode.AddTail( pDM->GetCellData( _T("DESCRIPTION"), i ) );	
}

VOID CBO_STATISTICS_1202::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CBO_STATISTICS_1202::OnOK() 
{
	// TODO: Add extra validation here
	
	struct SEARCH_CONDITION searchCond;
	GetSearchCondition( searchCond );

	CBO_STATISTICS_1201 *pParent = ( CBO_STATISTICS_1201 * )m_pParent;
	
	CString manageCode, dateType, unit;
	switch( searchCond.ready )
	{
		case -1 :
			MessageBox( _T("대출일 또는 반납일을 선택하여 주십시요"), _T("K2Up") );
			return;		
		case -2 :
			MessageBox( _T("값 구분을 선택하여 주십시요"), _T("K2Up") );
			return;		
		case -3 :
			MessageBox( _T("행 항목을 선택하여 주십시요"), _T("K2Up") );
			return;
		case -4 :
			MessageBox( _T("행 구성 항목을 선택하여 주십시요"), _T("K2Up") );
			return;
		case -5 :
			MessageBox( _T("열 항목을 선택하여 주시시요"), _T("K2Up") );
			return;
		default :
			if( GetStatisticsQuery( searchCond ) )
				return;
			else
			{
				GetControlData( _T("CM_BO_STATISTICS_1202"), _T("관리구분"), manageCode, -1, 1 );
				if( 1 == searchCond.dateType )
					dateType.Format( _T(" 대출일 : %s ~ %s "), searchCond.fromDate, searchCond.toDate );
				else
					dateType.Format( _T(" 반납일 : %s ~ %s "), searchCond.fromDate, searchCond.toDate );
				if( 1 == searchCond.valueType )
					unit.Format( _T(" 단위 : 대출자수 ") );
				else
					unit.Format( _T(" 단위 : 대출권수 ") );				
				pParent->m_searchCondition.Format( _T("%s %s  관리구분 : %s"), dateType, unit, manageCode );

				pParent->ShowGrid( m_sQuery, searchCond );
			}
			break;		
	}
}

VOID CBO_STATISTICS_1202::OnrSTATION() 
{
	// TODO: Add your control notification handler code here
	
	ShowRowList();
}

VOID CBO_STATISTICS_1202::OnrSHELFCODE() 
{
	// TODO: Add your control notification handler code here
	
	ShowRowList();
}

VOID CBO_STATISTICS_1202::ShowRowList()
{
	CStringList list;
	switch( GetRowType() )
	{
		case 1 :
			list.AddTail( &m_station );
			break;
		case 2 :
			list.AddTail( &m_shelfCode );
			break;
		default :
			return;
	}
	
	if( m_pChkList->GetCount() )
		m_pChkList->SetTopIndex( 0 );

	m_pChkList->ResetContent();
	POSITION pos = list.GetHeadPosition();
	while( pos )
		m_pChkList->AddString( list.GetNext( pos ) );
	m_pChkList->RedrawWindow();

	ReverseCheckList();
}

VOID CBO_STATISTICS_1202::GetCheckedItemRow( UINT &type, CArray < INT, INT >& row )
{
	type = GetRowType();

	row.RemoveAll();
	LISTITEM *pItem;
	for( INT i = 0; i < m_pChkList->GetCount(); i++ )
	{
		pItem = m_pChkList->GetItem( i );
		
		if( pItem->nCheckedState )
			row.Add( i );
	}
}

INT CBO_STATISTICS_1202::GetRowType()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_rSTATION );
	if( pBtn->GetCheck() )
		return 1;	// 스테이션

	pBtn = ( CButton * )GetDlgItem( IDC_rSHELFCODE );
	if( pBtn->GetCheck() )
		return 2;	// 자료실

	return 0;
}

INT CBO_STATISTICS_1202::GetColType()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_rMONTH );
	if( pBtn->GetCheck() )
		return 1;	// 월별

	pBtn = ( CButton * )GetDlgItem( IDC_rDAY );
	if( pBtn->GetCheck() )
		return 2;	// 일별

	return 0;
}

INT CBO_STATISTICS_1202::GetDateType()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_rLOAN_DATE );
	if( pBtn->GetCheck() )
		return 1;	// 대출일

	pBtn = ( CButton * )GetDlgItem( IDC_rRETURN_DATE );
	if( pBtn->GetCheck() )
		return 2;	// 반납일

	return 0;
}

INT CBO_STATISTICS_1202::GetValueType()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_rLOAN_USER );
	if( pBtn->GetCheck() )
		return 1;	// 대출자수 

	pBtn = ( CButton * )GetDlgItem( IDC_rLOAN_COUNT );
	if( pBtn->GetCheck() )
		return 2;	// 대출권수

	return 0;
}

VOID CBO_STATISTICS_1202::SetDefaultDate()
{
	CString date = GetDate( IDC_dTO );
	INT y, m, d;
	_stscanf( date, _T("%d/%d/%d"), &y, &m, &d );

	date.Format( _T("%d/%d/01"), y, m );
	SetDate( IDC_dFROM, date );
}

CString CBO_STATISTICS_1202::GetDate( const INT id )
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	
	COleDateTime date;
	pDate->GetTime( date );

	CString d;
	d.Format( _T("%04d/%02d/%02d"), date.GetYear(), date.GetMonth(), date.GetDay() );
	return d;
}

VOID CBO_STATISTICS_1202::SetDate( const INT id, CString date )
{
	INT y, m, d;
	_stscanf( date, _T("%d/%d/%d"), &y, &m, &d );
	CTime time( y, m, d, 0, 0, 0 );

	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	pDate->SetTime( &time );
}

VOID CBO_STATISTICS_1202::GetSearchCondition( struct SEARCH_CONDITION &searchCond )
{
	searchCond.ready = 0;
	
	searchCond.colType  = GetColType();
	if( !searchCond.colType )
		searchCond.ready = -5;
	
	GetCheckedItemRow( searchCond.rowType, searchCond.checkedRow );
	if( !searchCond.checkedRow.GetSize() )
		searchCond.ready = -4;
	if( !searchCond.rowType )
		searchCond.ready = -3;

	searchCond.valueType = GetValueType();
	if( !searchCond.valueType )
		searchCond.ready = -2;
	
	searchCond.dateType = GetDateType();
	if( !searchCond.dateType )
		searchCond.ready = -1;
	
	searchCond.fromDate = GetDate( IDC_dFROM );
	searchCond.toDate   = GetDate( IDC_dTO );
	searchCond.manageType = TakeControlData( _T("관리구분") );
}

CString CBO_STATISTICS_1202::TakeControlData( CString alias, INT option )
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_BO_STATISTICS_1202"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_BO_STATISTICS_1202"), _T("K2Up") );
		return _T("");
	}
	
	// 코드값이 _T("0") 이거나 설명값이 _T("적용안함")일때 _T("")를 반환한다. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CMNG_USER_MANAGER_01"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CBO_STATISTICS_1202::GetStatisticsQuery( struct SEARCH_CONDITION &searchCond )
{
	//BO_STATISTICS_1201_STATISTICS
	
	CString dateField = GetDateField( searchCond.dateType, searchCond.colType );
	CString group;
	CString locateField = GetLocateField( searchCond.dateType, searchCond.rowType, group );
	
	CString select;
	select.Format( _T(" %s, %s, %s %s "), dateField, locateField, MakeSelect( searchCond.valueType ), GetTypeCode( searchCond ) );
	CString from = _T(" LH_STORAGE_V01 ");
	CString where = MakeWhere( searchCond );
	CString groupBy;
	groupBy.Format( _T(" %s, %s "), dateField, group );

	m_sQuery.Format( _T("SELECT %s FROM %s WHERE %s GROUP BY %s ORDER BY %s"), select, from, where, groupBy, groupBy );
	m_colType = GetColType();	

	return 0;
}

CString CBO_STATISTICS_1202::MakeWhere( struct SEARCH_CONDITION &searchCond )
{
	CString where, tmp;
	//=========================================================
	//2008.10.17 UPDATE BY PJW : 메니지 코드를 DEFINE한다.
//	if( !searchCond.manageType.IsEmpty() )
//	{
//		tmp.Format( _T(" MANAGE_CODE = '%s' AND "), searchCond.manageType );
//		where += tmp;
//	}
	tmp.Format( _T(" MANAGE_CODE = UDF_MANAGE_CODE AND "));
	where += tmp;
	//=========================================================

	switch( searchCond.rowType )
	{
		case 1 :
			switch( searchCond.dateType )
			{
				case 1 :
					tmp.Format( _T(" SUBSTR( LOAN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND LOAN_TYPE_CODE = '3' AND "), searchCond.fromDate, searchCond.toDate );
					break;
				case 2 :
					tmp.Format( _T(" SUBSTR( RETURN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND RETURN_TYPE_CODE = '3' AND "), searchCond.fromDate, searchCond.toDate );
					break;
				default :
					break;
			}
			break;
		case 2 :
			switch( searchCond.dateType )
			{
				case 1 :
					tmp.Format( _T(" SUBSTR( LOAN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND "), searchCond.fromDate, searchCond.toDate );
					break;
				case 2 :
					tmp.Format( _T(" SUBSTR( RETURN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND "), searchCond.fromDate, searchCond.toDate );
					break;
				default :
					break;
			}
			break;
		default :
			break;
	}
	where += tmp;
	

	/*
	switch( searchCond.dateType )
	{
		case 1 :	// 대출일일 경우
			tmp.Format( _T(" SUBSTR( LOAN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND LOAN_TYPE_CODE = '3' AND "), searchCond.fromDate, searchCond.toDate );
			where += tmp;
			break;
		case 2 :	// 반납일일 경우
			tmp.Format( _T(" SUBSTR( RETURN_DATE, 0, 10 ) BETWEEN '%s' AND '%s' AND RETURN_TYPE_CODE = '3' AND "), searchCond.fromDate, searchCond.toDate );
			where += tmp;
			break;
		default :
			break;
	}
	*/
	
	tmp.Format( _T(" %s "), GetColList( searchCond.dateType, searchCond.rowType, searchCond.checkedRow ) );
	where += tmp;
	
	return where;
}

// 검색 WHERE 절에서 행설정( 스테이션, 자료실 )에 대한 조건 만들기
CString CBO_STATISTICS_1202::GetColList( INT dateType, INT type, CArray < INT, INT > &checkedRow )
{
	CESL_DataMgr* pDM = NULL;
	CString alias, field;
	switch( type )
	{
		case 1 :
			pDM = FindDM( _T("DM_BO_STATISTICS_1201_STATION") );
			alias = _T("PC_NAME");

			switch( dateType )
			{
				case 1 :
					field = _T("SELF_LOAN_STATION");
					break;
				case 2 :
					field = _T("SELF_RETURN_STATION");
					break;
				default :
					break;
			}

			break;
		case 2 :
			pDM = FindDM( _T("DM_BO_STATISTICS_1201_SHELFCODE") );
			alias = _T("CODE");
			field = _T("SHELF_LOC_CODE");
			break;
		default :
			return _T("");
	}

	CString where, list, tmp;
	for( INT i = 0; i < checkedRow.GetSize(); i++ )
	{
		tmp.Format( _T(" '%s',"), pDM->GetCellData( alias, checkedRow.GetAt( i ) ) );

		if( tmp.GetLength() > 4 )
			list += tmp;
	}
	
	if( !list.IsEmpty() )
		where.Format( _T(" %s IN ( %s ) "), field, list.Left( list.GetLength() - 1 ) );	
	
	return where;
}

CString CBO_STATISTICS_1202::MakeSelect( INT valueType )
{
	CString select, tmp;
	
	switch( valueType )
	{
		case 1 :
			tmp.Format( _T(" COUNT( DISTINCT USER_KEY ) ") );
			select += tmp;
			break;
		case 2 :
			tmp.Format( _T(" COUNT( * ) ") );
			select += tmp;
			break;
		default :
			break;
	}
	
	
	
	return select;
}


CString CBO_STATISTICS_1202::GetDateField( INT dateType, INT colType )
{
	CString field;
	switch( dateType )
	{
		case 1 :
			switch( colType )
			{
				case 1 :
					field.Format( _T(" SUBSTR( LOAN_DATE, 0, 7 ) ") );
					break;
				case 2 :
					field.Format( _T(" SUBSTR( LOAN_DATE, 0, 10 ) ") );
					break;
				default :
					break;			
			}
			break;
		case 2 :
			switch( colType )
			{
				case 1 :
					field.Format( _T(" SUBSTR( RETURN_DATE, 0, 7 ) ") );
					break;
				case 2 :
					field.Format( _T(" SUBSTR( RETURN_DATE, 0, 10 ) ") );
					break;
				default :
					break;			
			}
			break;
		default :
			break;
	}

	return field;
}

CString CBO_STATISTICS_1202::GetLocateField( INT dateType, INT rowType, CString &group )
{
	CString field;
	switch( rowType )
	{
		case 1 :
			switch( dateType )
			{
				case 1 :
					field.Format( _T(" ( SELECT NAME FROM SMART_ID_STATION_TBL WHERE PC_NAME = SELF_LOAN_STATION ) ") );
					group.Format( _T(" SELF_LOAN_STATION ") );
					break;
				case 2 :
					field.Format( _T(" ( SELECT NAME FROM SMART_ID_STATION_TBL WHERE PC_NAME = SELF_RETURN_STATION ) ") );
					group.Format( _T(" SELF_RETURN_STATION ") );
					break;
				default :
					break;
			}
			break;
		case 2 :
			switch( dateType )
			{
				case 1 :
					field.Format( _T(" ( SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = SHELF_LOC_CODE ) ") );
					group.Format( _T(" SHELF_LOC_CODE, LOAN_TYPE_CODE ") );
					break;
				case 2 :
					field.Format( _T(" ( SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = SHELF_LOC_CODE ) ") );
					group.Format( _T(" SHELF_LOC_CODE, RETURN_TYPE_CODE ") );
					break;
				default :
					break;
			}
			break;
		default :
			break;
	}

	return field;
}

VOID CBO_STATISTICS_1202::ReverseCheckList()
{
	LISTITEM *pItem;
	for( INT i = 0; i < m_pChkList->GetCount(); i++ )
	{
		pItem = m_pChkList->GetItem( i );
		
		if( pItem->nCheckedState )
			pItem->nCheckedState = 0;
		else
			pItem->nCheckedState = 1;

		m_pChkList->SetCheck( i, pItem->nCheckedState );
	}
}

VOID CBO_STATISTICS_1202::SetDefaultOpt()
{
	CButton *pBtn = NULL;

	pBtn = ( CButton * )GetDlgItem( IDC_rLOAN_DATE );
	pBtn->SetCheck( true );

	pBtn = ( CButton * )GetDlgItem( IDC_rLOAN_USER );
	pBtn->SetCheck( true );
	
	pBtn = ( CButton * )GetDlgItem( IDC_rSTATION );
	pBtn->SetCheck( true );
	ShowRowList();
	
	pBtn = ( CButton * )GetDlgItem( IDC_rMONTH );
	pBtn->SetCheck( true );
}

BOOL CBO_STATISTICS_1202::Create( CWnd *pParentWnd )
{
	return CDialog::Create(IDD, pParentWnd);
}

CString CBO_STATISTICS_1202::GetTypeCode( struct SEARCH_CONDITION &searchCond )
{
	if( 1 == searchCond.rowType )
		return _T("");

	CString typeCode;
	switch( searchCond.dateType )
	{
		case 1 :
			typeCode.Format( _T(", LOAN_TYPE_CODE ") );
			break;
		case 2 :
			typeCode.Format( _T(", RETURN_TYPE_CODE ") );
			break;
		default :
			break;
	}
	
	return typeCode;
}


HBRUSH CBO_STATISTICS_1202::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

