// BO_STATISTICS_1201.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_STATISTICS_1201.h"

/////////////////////////////////////////////////////////////////////////////
// CBO_STATISTICS_1201 dialog


CBO_STATISTICS_1201::CBO_STATISTICS_1201( CString strMenuAlias,CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CBO_STATISTICS_1201)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_STATISTICS_1201::~CBO_STATISTICS_1201()
{
	
}



VOID CBO_STATISTICS_1201::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATISTICS_1201)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BOOL CBO_STATISTICS_1201::Create( CWnd* pParentWnd ) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BEGIN_MESSAGE_MAP(CBO_STATISTICS_1201, CDialog)
	//{{AFX_MSG_MAP(CBO_STATISTICS_1201)
	ON_WM_SIZE()
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATISTICS_1201 message handlers

VOID CBO_STATISTICS_1201::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	

	ResizeGrid( cx, cy );
}

VOID CBO_STATISTICS_1201::ResizeGrid( INT cx, INT cy )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	if( !pGrid->GetSafeHwnd() )
		return;

	const INT sx = 5, sy = 5;
	INT width  = cx - ( sx * 2 );
	INT height = cy - ( sy * 2 );
	pGrid->MoveWindow( sx, sy, width, height, true );
}

VOID CBO_STATISTICS_1201::ViewStatisticsDlg()
{
	CBO_STATISTICS_1202 dlg( _T("통계"), ( CESL_Mgr * )this );
	if( IDOK == dlg.DoModal() )
	{
		//m_colType = dlg.m_colType;
		m_colType.Format(_T("%d"),dlg.m_colType);
		//ShowGrid( dlg.m_sQuery );	
	}
}

BOOL CBO_STATISTICS_1201::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("BO_STATISTICS_1201") ) < 0 )
		return false;

	CleanGrid();
	
	// 월별 통계 테스트
	//ShowGrid( _T("SELECT   SUBSTR (loan_date, 0, 7), (SELECT NAME FROM smart_id_station_tbl WHERE pc_name = self_loan_station), COUNT (DISTINCT user_key) FROM LH_STORAGE_V01 WHERE manage_code = 'MA' AND loan_type_code = '3' AND self_loan_station IN ('LOAN01', 'ECO_CHECKOUT') GROUP BY SUBSTR (loan_date, 0, 7), self_loan_station ORDER BY SUBSTR (loan_date, 0, 7), self_loan_station") );
	// 일별 통계 테스트
	//ShowGrid( _T("SELECT   SUBSTR (loan_date, 0, 10), (SELECT NAME FROM smart_id_station_tbl WHERE pc_name = self_loan_station), COUNT (DISTINCT user_key) FROM LH_STORAGE_V01 WHERE manage_code = 'MA' AND loan_type_code = '3' AND self_loan_station IN ('LOAN01', 'ECO_CHECKOUT') GROUP BY SUBSTR (loan_date, 0, 10), self_loan_station ORDER BY SUBSTR (loan_date, 0, 10), self_loan_station") );
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_STATISTICS_1201::ShowGrid( CString query, struct SEARCH_CONDITION &searchCond )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_STATISTICS_1201_STATISTICS") );
	pDM->RestructDataManager( query );

	CleanGrid();
	if( !pDM->GetRowCount() )
		MessageBox( _T("검색결과가 없습니다."), _T("KOLAS 2 Up") );
	else
	{
		SetCellGridData( pDM, searchCond );
		SetCellBold();

		switch( searchCond.rowType )
		{
			case 1 :	// 스테이션
				m_rowType.Format( _T("스테이션") );
				switch( searchCond.colType )
				{
					case 1 :	// 월
						m_statisticsType = 1;
						m_colType.Format( _T("월") );
						if( 1 == searchCond.dateType )
							m_rptTitle = _T("월별 자가대출통계");
						else
							m_rptTitle = _T("월별 자가반납통계");
						break;
					case 2 :	// 일
						m_statisticsType = 2;
						m_colType.Format( _T("일") );
						if( 1 == searchCond.dateType )
							m_rptTitle = _T("일별 자가대출통계");
						else
							m_rptTitle = _T("일별 자가반납통계");
						break;
					default :
						break;
				}
				break;
			case 2 :	// 자료실
				m_rowType.Format( _T("자료실") );
				switch( searchCond.colType )
				{
					case 1 :	// 월
						m_statisticsType = 3;
						m_colType.Format( _T("월") );
						if( 1 == searchCond.dateType )
							m_rptTitle = _T("월별 자가대출통계");
						else
							m_rptTitle = _T("월별 자가반납통계");
						break;
					case 2 :	// 일
						m_statisticsType = 4;
						m_colType.Format( _T("일") );
						if( 1 == searchCond.dateType )
							m_rptTitle = _T("일별 자가대출통계");
						else
							m_rptTitle = _T("일별 자가반납통계");
						break;
					default :
						break;
				}
				break;
			default :
				break;
		}		
	}
}

VOID CBO_STATISTICS_1201::SetCellGridData( CESL_DataMgr *pDM, struct SEARCH_CONDITION &searchCond )
{
	if( 1 == searchCond.rowType )
	{
		SetCellGridData( pDM );
		return;
	}

	CStringList rowSum, colSum;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	INT x = 1, y = 1, flag = 0;
	CString date, locate, cnt, type;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		date   = pDM->GetCellData( i, 0 );
		locate = pDM->GetCellData( i, 1 );
		cnt	   = pDM->GetCellData( i, 2 );
		type   = pDM->GetCellData( i, 3 );

		if( i )
			GetCellPlace( pGrid, date, locate, x, y );
		else
		{
			x = 1;
			y = 1;
			pGrid->SetTextMatrix( 0, 1, date );
			pGrid->SetTextMatrix( 1, 0, locate );
		}

		if( x + 1 > pGrid->GetRows() )
		{
			pGrid->SetRows( x + 1 );
			pGrid->SetRowHeight( x, 400 );
			pGrid->SetTextMatrix( x, 0, locate );
		}

		if( y + 1 > pGrid->GetCols( 0 ) )
		{
			pGrid->SetCols( 0, y + 1 );
			pGrid->SetTextMatrix( 0, y, date );
		}

		pGrid->SetTextMatrix( x, y, GetCellValue( pGrid, x, y, _ttoi( cnt ), _ttoi( type ) ) );
	}
	
	if( rowCnt )
		ViewSum( pGrid );
}

VOID CBO_STATISTICS_1201::SetCellGridData( CESL_DataMgr *pDM )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );

	CArray< INT, INT > rowSum;
	CArray< INT, INT > colSum;
	
	INT x = 1, y = 1, flag = 0;
	CString date, locate, cnt;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		date   = pDM->GetCellData( i, 0 );
		locate = pDM->GetCellData( i, 1 );
		cnt    = pDM->GetCellData( i, 2 );

		if( i )
			GetCellPlace( pGrid, date, locate, x, y );
		else
		{
			x = 1;
			y = 1;
			pGrid->SetTextMatrix( 0, 1, date );
			pGrid->SetTextMatrix( 1, 0, locate );
		}
		
		if( x + 1 > pGrid->GetRows() )
		{
			pGrid->SetRows( x + 1 );
			pGrid->SetRowHeight( x, 400 );
			pGrid->SetTextMatrix( x, 0, locate );
		}
		
		if( y + 1 > pGrid->GetCols( 0 ) )
		{
			pGrid->SetCols( 0, y + 1 );
			pGrid->SetTextMatrix( 0, y, date );
		}

		pGrid->SetTextMatrix( x, y, cnt );
		AddSum( x - 1, y - 1, _ttoi( cnt ), rowSum, colSum );
	}

	if( rowCnt )
		ShowSum( pGrid, rowSum, colSum );
}

VOID CBO_STATISTICS_1201::GetCellPlace( CMSHFlexGrid *pGrid, CString date, CString locate, INT &x, INT &y )
{
	INT rows = pGrid->GetRows();
	INT cols = pGrid->GetCols( 0 );
	INT i = 0;
	CString tmp;
	bool flag = false;
	
	for( i = 1; i < rows; i++ )
	{
		tmp = pGrid->GetTextMatrix( i, 0 );
		if( tmp == locate )
		{
			flag = true;
			x = i;
			break;
		}
	}
	if( !flag )
		x = rows;
	flag = false;

	for( i = y; i < cols; i++ )
	{
		tmp = pGrid->GetTextMatrix( 0, i );
		if( tmp == date )
		{
			flag = true;
			y = i;
			break;
		}
	}
	if( !flag )
		y = cols;	
}

VOID CBO_STATISTICS_1201::ShowSum( CMSHFlexGrid *pGrid, CArray< INT, INT > &rowSum, CArray< INT, INT > &colSum )
{
	INT rows = pGrid->GetRows();
	INT cols = pGrid->GetCols( 0 );
	
	pGrid->SetRows( rows + 1 );
	pGrid->SetRowHeight( rows, 400 );
	pGrid->SetCols( 0, cols + 1 );
	pGrid->SetTextMatrix( 0, cols, _T("계") );
	pGrid->SetTextMatrix( rows, 0, _T("계") );

	INT i = 0, totalSum = 0, tmp = 0;;
	CString sum;

	for( i = 0; i < rows -1 ; i++ )
	{
		tmp = colSum.GetAt( i );
		totalSum += tmp;

		sum.Format( _T("%d"), tmp );
		pGrid->SetTextMatrix( i + 1, cols, sum );
	}

	for( i = 0; i < cols -1 ; i++ )
	{
		sum.Format( _T("%d"), rowSum.GetAt( i ) );
		pGrid->SetTextMatrix( rows, i + 1, sum );
	}

	sum.Format( _T("%d"), totalSum );
	pGrid->SetTextMatrix( rows, cols, sum );
}

VOID CBO_STATISTICS_1201::PrintData()
{
	CESL_DataMgr* pDM;
	switch( m_statisticsType )
	{
		case 1 :
		case 3 :
			pDM = FindDM( _T("DM_BO_STATISTICS_1201_MONTH") );
			MakeMonthDM( pDM );
			break;
		case 2 :
		case 4 :
			pDM = FindDM( _T("DM_BO_STATISTICS_1201_DAY") );
			MakeDayDM( pDM );
			break;
		default :
			break;
	}
	
	if( pDM->GetRowCount() )
		PrintStatisticsData( pDM, m_statisticsType );
}

VOID CBO_STATISTICS_1201::MakeMonthDM( CESL_DataMgr *pDM )
{
	pDM->FreeData();
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );

	const INT colCnt = 15;
	CString alias[ colCnt ] = {
		_T("ROW_TITLE"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("ROW_SUM"), _T("USE_PERSENT") };
	CString data[ colCnt ];
	
	INT defaultYear, m, d;
	GetCellDate( defaultYear, m, d, 1 );
	if( -1 == defaultYear )
	{
		MessageBox( _T("출력할 목록이 존재하지 않습니다."), _T("KOLAS 2 Up") );
		return;
	}
	
	INT rows = pGrid->GetRows();
	INT cols = pGrid->GetCols( 0 );
	
	for( INT i = 1; i < rows; i++ )
	{
		pDM->InsertRow( -1 );
		
		for( INT j = 1; j < colCnt; j++ )
			data[ j ] = _T("-");

		GetGridMonthData( pGrid, data, defaultYear, i, cols );
		pDM->SetCellData( alias, colCnt, data, i - 1 );
	}
}

VOID CBO_STATISTICS_1201::MakeDayDM( CESL_DataMgr *pDM )
{
	pDM->FreeData();
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );

	const INT colCnt = 34;
	CString alias[ colCnt ] = {
		_T("ROW_TITLE"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("13"), _T("14"), _T("15"), _T("16"), _T("17"), _T("18"), _T("19"), _T("20"), _T("21"), _T("22"), _T("23"), _T("24"), _T("25"), _T("26"), _T("27"), _T("28"), _T("29"), _T("30"), _T("31"), _T("ROW_SUM"), _T("USE_PERSENT") };
	CString data[ colCnt ];

	INT defaultYear, defaultMonth, d;
	GetCellDate( defaultYear, defaultMonth, d, 1 );
	if( -1 == defaultMonth )
	{
		MessageBox( _T("출력할 목록이 존재하지 않습니다."), _T("KOLAS 2 Up") );
		return;
	}

	INT rows = pGrid->GetRows();
	INT cols = pGrid->GetCols( 0 );

	for( INT i = 1; i < rows; i++ )
	{
		pDM->InsertRow( -1 );
		
		for( INT j = 0; j < colCnt; j++ )
			data[ j ] = _T("-");
		
		GetGridDayData( pGrid, data, defaultYear, defaultMonth, i, cols );		
		pDM->SetCellData( alias, colCnt, data, i - 1 );
	}	
}

VOID CBO_STATISTICS_1201::GetGridDayData( CMSHFlexGrid *pGrid, CString data[], INT defaultYear, INT defaultMonth, INT row, INT cols )
{
	data[ 0 ] = pGrid->GetTextMatrix( row, 0 );
	INT y, m, d;
	CString cnt;
	for( INT i = 1; i < cols; i++ )
	{
		GetCellDate( y, m, d, i );
		if( y > defaultYear || m > defaultMonth )
			continue;

		if( d > 0 || -2 == y || -3 == y )
		{
			cnt = pGrid->GetTextMatrix( row, i );
			if( cnt.IsEmpty() )
				cnt = _T("0");
			
			if( -2 == y )
				data[ 32 ] = cnt;
			else if( -3 == y )
				data[ 33 ] = cnt;
			else
				data[ d ] = cnt;
		}
	}
	
}

VOID CBO_STATISTICS_1201::GetGridMonthData( CMSHFlexGrid *pGrid, CString data[], INT defaultYear, INT row, INT cols )
{
	data[ 0 ] = pGrid->GetTextMatrix( row, 0 );
	INT y, m, d;
	CString cnt;
	for( INT i = 1; i < cols; i++ )
	{
		GetCellDate( y, m, d, i );
		if( y > defaultYear )
			continue;

		if( m > 0 || -2 == y || -3 == y )
		{
			cnt = pGrid->GetTextMatrix( row, i );
			if( cnt.IsEmpty() )
				cnt = _T("0");

			if( -2 == y )
				data[ 13 ] = cnt;
			else if( -3 == y )
				data[ 14 ] = cnt;
			else
				data[ m ] = cnt;
		}
	}
}

VOID CBO_STATISTICS_1201::CleanGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	pGrid->SetRows( 2 );
	pGrid->SetCols( 0, 2 );
	pGrid->SetRowHeight( 0, 400 );
	pGrid->SetRowHeight( 1, 400 );
	pGrid->SetColWidth( 0, 0, 1200 );
	
	pGrid->SetTextMatrix( 0, 1, _T("") );
	pGrid->SetTextMatrix( 1, 0, _T("") );
	pGrid->SetTextMatrix( 1, 1, _T("") );
}

INT CBO_STATISTICS_1201::GetCellYear( INT col )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	CString year;

	year = pGrid->GetTextMatrix( 0, col );
	if( year.IsEmpty() )
		return -1;
	else
		return _ttoi( year.Left( 4 ) );
}

INT CBO_STATISTICS_1201::GetCellMonth(INT col)
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	CString mon;

	mon = pGrid->GetTextMatrix( 0, col );
	if( mon.IsEmpty() )
		return -1;
	else
		return _ttoi( mon.Mid( 5, 2 ) );
}

VOID CBO_STATISTICS_1201::GetCellDate( INT &y, INT &m, INT &d, INT col )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );

	y = m = d = -1;
	CString date = pGrid->GetTextMatrix( 0, col );

	if( _T("계") == date )
		y = -2;
	if( _T("이용률") == date )
		y = -3;

	if( 10 == date.GetLength() )
		_stscanf( date.GetBuffer( 0 ), _T("%d/%d/%d"), &y, &m, &d );
	else if( 7 == date.GetLength() )
		_stscanf( date.GetBuffer( 0 ), _T("%d/%d"), &y, &m, &d );
}

VOID CBO_STATISTICS_1201::PrintStatisticsData( CESL_DataMgr* pDM, INT sType )
{
	CString rptTitle[ 4 ] = { _T("자가대출반납통계-월"), _T("자가대출반납통계-일"), _T("자가대출반납통계-월-S"), _T("자가대출반납통계-일-S") };

	// 출력하기
	CSIReportManager *pSIReportManager = new CSIReportManager( this );
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if( !pSIReportManager )
	{
		AfxMessageBox( _T("Report Manager를 생성하지 못하였습니다.") );
		return;
	}
	
	pSIReportManager->SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	if( pSIReportManager->GetSIReportFileFromDB( rptTitle[ sType - 1 ] ) < 0 )
	{
		AfxMessageBox( _T("DB로 부터 SI Report 정보를 가져오지 못하였습니다.") );
		return;
	}

	pSIReportManager->SetDataMgr( 0, pDM , 0 );
	
	//pSIReportManager->SetTempVariable( alias , value );
	pSIReportManager->SetTempVariable( _T("행"),		m_rowType );
	pSIReportManager->SetTempVariable( _T("열"),		m_colType );
	pSIReportManager->SetTempVariable( _T("제목"),		m_rptTitle );
	pSIReportManager->SetTempVariable( _T("검색조건"),	m_searchCondition );

	pSIReportManager->Print();
	if( pSIReportManager )
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
}

/*
참고 소스
INT CBO_LOC_1300::PrintList()
  
	// 일반적인 반복물 출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("배가자료목록"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
*/

INT CBO_STATISTICS_1201::SIReportEventHandler( CSIReportEventInfo *EventInfo )
{
	CSIReportManager *pSIReportManager = ( CSIReportManager * )EventInfo->m_pSIReportManager;
	if( !pSIReportManager )
		return -1;
	else
		return 0;
}

CString CBO_STATISTICS_1201::GetCellValue( CMSHFlexGrid *pGrid, INT x, INT y, INT cnt, INT type )
{
	CString value;
	INT r = 0, t = 0;
	CString beforeValue = pGrid->GetTextMatrix( x, y );
	if( beforeValue.IsEmpty() )
	{
		if( 3 == type )
			r = cnt;
		t = cnt;

		value.Format( _T("%d/%d"), r, t );
	}
	else
	{
		_stscanf( beforeValue, _T("%d/%d"), &r, &t );
		
		if( 3 == type )
			r += cnt;
		t += cnt;

		value.Format( _T("%d/%d"), r, t );
	}
	
	return value;
}

VOID CBO_STATISTICS_1201::AddSum( INT x, INT y, INT cnt, CArray< INT, INT > &rowSum, CArray< INT, INT > &colSum )
{
	if( colSum.GetSize() - 1 < x )
		colSum.Add( cnt );
	else
		colSum.SetAt( x, colSum.GetAt( x ) + cnt );

	if( rowSum.GetSize() - 1 < y )
		rowSum.Add( cnt );
	else
		rowSum.SetAt( y, rowSum.GetAt( y ) + cnt );
}


VOID CBO_STATISTICS_1201::ViewSum( CMSHFlexGrid *pGrid )
{
	INT rows = pGrid->GetRows();
	INT cols = pGrid->GetCols( 0 );

	pGrid->SetRows( rows + 2 );
	pGrid->SetRowHeight( rows, 400 );
	pGrid->SetRowHeight( rows + 1, 400 );

	pGrid->SetCols( 0, cols + 2 );

	pGrid->SetTextMatrix( 0, cols, _T("계") );
	pGrid->SetTextMatrix( 0, cols + 1, _T("이용률") );
	pGrid->SetTextMatrix( rows, 0, _T("계") );
	pGrid->SetTextMatrix( rows + 1, 0, _T("이용률") );

	CString beforeValue;
	INT bR, bC;
	INT tR, tC;
	INT i, j;

	for( i = 1; i < rows; i++ )
	{
		bR = bC = tR = tC = 0;
		for( j = 1; j < cols; j++ )
		{
			beforeValue = pGrid->GetTextMatrix( i, j );
			
			if( beforeValue.IsEmpty() )
				pGrid->SetTextMatrix( i, j, _T("-") );
			else if( _T("-") != beforeValue )
			{
				_stscanf( beforeValue, _T("%d/%d"), &bR, &bC );

				tR += bR;
				tC += bC;
			}
		}

		beforeValue.Format( _T("%d/%d"), tR, tC );
		pGrid->SetTextMatrix( i, cols, beforeValue );

		beforeValue.Format( _T("%.1f%%"), ( float )tR / ( float )tC * 100.0 );
		pGrid->SetTextMatrix( i, cols + 1, beforeValue );
	}
	
	for( i = 1; i < cols + 1; i++ )
	{
		bR = bC = tR = tC = 0;
		for( j = 1; j < rows; j++ )
		{
			beforeValue = pGrid->GetTextMatrix( j, i );

			if( !beforeValue.IsEmpty() )
			{
				_stscanf( beforeValue, _T("%d/%d"), &bR, &bC );

				tR += bR;
				tC += bC;
			}
		}

		beforeValue.Format( _T("%d/%d"), tR, tC );
		pGrid->SetTextMatrix( rows, i, beforeValue );

		beforeValue.Format( _T("%.1f%%"), ( float )tR / ( float )tC * 100.0 );
		pGrid->SetTextMatrix( rows + 1, i, beforeValue );
	}
	pGrid->SetTextMatrix( rows, cols + 1, beforeValue );
	pGrid->SetTextMatrix( rows + 1, cols + 1, _T("-") );
}

/*
	rpt파일에서 날짜보여주기
  (%DATE_YEAR%년 %DATE_MONTH%월 %DATE_DAY%일)
	rpt파일에 값 집어넣기	
  %GETTEMPVARIABLE(행)%
  %GETTEMPVARIABLE(열)%
  %GETTEMPVARIABLE(검색조건)%
  %GETTEMPVARIABLE(제목)%	
*/

VOID CBO_STATISTICS_1201::SetCellBold()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		pGrid->SetCol( 0 );
		pGrid->SetCellFontBold( true );
	}
	for( i = 1; i < pGrid->GetCols( 0 ); i++ )
	{
		pGrid->SetRow( 0 );
		pGrid->SetCol( i );
		pGrid->SetCellFontBold( true );
	}
	pGrid->SetRow( 0 );
	pGrid->SetCol( 0 );
}
HBRUSH CBO_STATISTICS_1201::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

