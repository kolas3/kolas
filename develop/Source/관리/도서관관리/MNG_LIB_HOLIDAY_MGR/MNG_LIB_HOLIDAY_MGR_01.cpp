// MNG_LIB_HOLIDAY_MGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_HOLIDAY_MGR_01.h"
#include "..\FileManager\LibFileManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_LIB_HOLIDAY_MGR_02.h"
#include "MNG_LIB_HOLIDAY_MGR_03.h"
#include "MNG_LIB_HOLIDAY_MGR_05.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_01 dialog


CMNG_LIB_HOLIDAY_MGR_01::CMNG_LIB_HOLIDAY_MGR_01(CESL_Mgr* pParent  )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOLIDAY_MGR_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pGridEdt = NULL;
	m_cUseMANAGE_CODE = 'Y';
}



BOOL CMNG_LIB_HOLIDAY_MGR_01::Create(CWnd* pParentWnd)
{	
	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_LIB_HOLIDAY_MGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOLIDAY_MGR_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOLIDAY_MGR_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOLIDAY_MGR_01)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE( UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_MESSAGE(WM_USER+1000, OnbFIND)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_01 message handlers

BOOL CMNG_LIB_HOLIDAY_MGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HWND hEdit = ::GetDlgItem(m_hWnd, IDC_eYEAR);
	::SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)4, 0);

	m_pLoanShareManager = new CLoanShareManager(this);
	
	m_strMANAGE_CODE = _T("");
	OnInitEslMgr();
	InitSetGrid();
	InitMANAGE_CODE();

	SetTimer (0, 100, NULL);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_HOLIDAY_MGR_01::OnSize( UINT nType, INT cx, INT cy ) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CStatic *pStatic;

	pStatic = ( CStatic * )GetDlgItem( IDC_sGROUP_1 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 7, 3, cx- 15, 35 );

	pStatic = ( CStatic * )GetDlgItem( IDC_sGROUP_2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 7, 35, cx - 15, cy - 45 );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;
	else
		pGrid->MoveWindow(0, 35, cx, cy);
		//pGrid->MoveWindow( 14, 50, cx - 28, cy - 65 );	
}


VOID CMNG_LIB_HOLIDAY_MGR_01::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	FindHolidayData();
}

VOID CMNG_LIB_HOLIDAY_MGR_01::InitMANAGE_CODE(VOID)
{
	m_cUseMANAGE_CODE = 'Y';
	m_strUSER_MANAGE_CODE = m_pInfo->MANAGE_CODE;

	return;
}

VOID CMNG_LIB_HOLIDAY_MGR_01::FindHolidayData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_01") );
	if (pDM == NULL) return;

	CString where = MakeSelectQuery();
	if( !where.GetLength() )
		return;

	InitHoliday();
	pDM->RefreshDataManager( where );
	if (pDM->GetRowCount() == 0) {
		CleanGrid();
		return;
	}
	APPLY_Holiday_FromDM(pDM);
	ShowGrid();
}

CString CMNG_LIB_HOLIDAY_MGR_01::MakeSelectQuery()
{
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eYEAR );
	pEdt->GetWindowText( m_year );

	m_year.TrimLeft();
	m_year.TrimRight();
	if( !m_year.GetLength() )
		return _T("");

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_01") );
	if (pDM == NULL) return -1;


	CString where;
	where.Format( _T("YEAR=%s AND MANAGE_CODE=UDF_MANAGE_CODE"), m_year );

	return where;
}

VOID CMNG_LIB_HOLIDAY_MGR_01::ShowGrid()
{
	CString strDays, strTmp;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	if (pGrid == NULL) return;
	INT i, j;
	for (i = 0; i < 12; i++) {
		strDays = _T("");
		for (j = 0; j < 31; j++) {
			strTmp = _T("");
			if (HOLIDAY[i][j] == 'Y') {
				if (strDays.GetLength() == 0) strTmp.Format(_T("%d"), j+1);
					else strTmp.Format(_T(", %d"), j+1);
				strDays += strTmp;
			}
		}

		pGrid->SetTextMatrix(i+1, 2, strDays);
	}
}

VOID CMNG_LIB_HOLIDAY_MGR_01::OnShowWindow( BOOL bShow, UINT nStatus ) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CMNG_LIB_HOLIDAY_MGR_01::InitSetGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	pGrid->SetCols( 0, 3 );
	pGrid->SetTextMatrix( 0, 1, _T("월") );
	pGrid->SetTextMatrix( 0, 2, _T("날짜") );
	pGrid->SetColWidth( 1, 0, 800 );
	pGrid->SetColWidth( 2, 0, 4000 );
	pGrid->SetColAlignment( 1, 1 );
	pGrid->SetColAlignment( 2, 1 );
	
	pGrid->SetRow( 0 );
	pGrid->SetCol( 1 );
	pGrid->SetCellAlignment( 4 );
	pGrid->SetCol( 2 );
	pGrid->SetCellAlignment( 4 );
	
	pGrid->SetRows( 13 );
	pGrid->SetColWidth( 0, 0, 0 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	CString mon;
	pGrid->SetCol( 1 );
	for( INT i = 0; i < 12; i++ )
	{
		mon.Format( _T("%2d 월"), i + 1 );
		pGrid->SetTextMatrix( i + 1, 1, mon );
		
		pGrid->SetRow( i + 1 );
		pGrid->SetCellBackColor( RGB( 228, 243, 251 ) );
	}
	
	pGrid->SetRow( row );
	pGrid->SetCol( col );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::ClearGridDaysInfo()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	for( INT i = 0; i < 12; i++ )
		pGrid->SetTextMatrix( i + 1, 2, _T("") );
}

BEGIN_EVENTSINK_MAP(CMNG_LIB_HOLIDAY_MGR_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_LIB_HOLIDAY_MGR_01)
	ON_EVENT(CMNG_LIB_HOLIDAY_MGR_01, IDC_mngGRID_1, -601 /* DblClick */, OnDblClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_LIB_HOLIDAY_MGR_01::OnDblClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	if (m_pInfo->MODE == 0)		OnModifyHoliDayInfo();
}

VOID CMNG_LIB_HOLIDAY_MGR_01::CreateGridEdt()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT col = pGrid->GetCol();
	INT row = pGrid->GetRow();
	if( 2 != col || !m_year.GetLength() )
		return;

	CreateGridCell( row, col, pGrid->GetTextMatrix( row, col ) );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}

	FreeGridEdt();
}

VOID CMNG_LIB_HOLIDAY_MGR_01::CreateGridCell(INT row, INT col, CString text)
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

	m_pGridEdt->SetWindowText( text );
    m_pGridEdt->SetSel( 0, -1 );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::FreeGridEdt()
{
	if( m_pGridEdt )
		delete m_pGridEdt;
	m_pGridEdt = NULL;
}

VOID CMNG_LIB_HOLIDAY_MGR_01::SetGridText( INT row, INT col, CString text )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	pGrid->SetTextMatrix( row, col, text );
	AddDataToDM( row, text );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::SaveHoliDayInfo()
{
	if (m_year.GetLength() == 0) {
		AfxMessageBox(_T("먼저 검색을 하여 주십시오."));
		return;
	}
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_01") );
	if (pDM == NULL) return;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	if (pGrid == NULL) return ;

	CString query;
	query.Format( _T("DELETE FROM MN_LIB_HOLIDAY_TBL WHERE YEAR=%s AND MANAGE_CODE=UDF_MANAGE_CODE;"), m_year );

	pDM->ExecuteQuery( query, 1 );	

	CString sTempRecKey;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("63");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LIB_HOLIDAY_TBL");
	

	for( INT c = 0 ; c < pDM->GetRowCount() ; c++ )
	{
		pDM->GetCellData( _T("REC_KEY") , c , sTempRecKey );

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sTempRecKey;

		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	CStringArray asRecKey;
	
	CString strDays, strRecKey, strMonth;
	INT ids;
	pDM->StartFrame();
	INT nQueryCount = 0;
	for (INT i = 1; i <= 12; i++) {
		strMonth.Format(_T("%d"), i);
		strDays = pGrid->GetTextMatrix(i, 2);
		strDays.TrimLeft();			strDays.TrimRight();
		if (strDays.GetLength() > 0) {
			pDM->MakeRecKey(strRecKey);
			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("REC_KEY")	, _T("NUMERIC")	, strRecKey);
			pDM->AddDBFieldData(_T("YEAR")		, _T("NUMERIC")	, m_year);
			pDM->AddDBFieldData(_T("MONTH")		, _T("NUMERIC")	, strMonth);
			pDM->AddDBFieldData(_T("DAYS")		, _T("STRING")	, strDays);
			pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING")	, m_strMANAGE_CODE);
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("휴관일관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeInsertFrame(_T("MN_LIB_HOLIDAY_TBL"));

			nQueryCount++;

			asRecKey.Add(strRecKey);			
		}


	}

	

	ids = 0;
	if (nQueryCount > 0) {
		ids = pDM->SendFrame();
	}
	if (ids == 0) AfxMessageBox(_T("변경사항이 적용되었습니다."));
		else AfxMessageBox(_T("DB 작업중 에러가 발생하였습니다."));

	for( c = 0 ; c < asRecKey.GetSize() ; c++ )
	{
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = asRecKey.GetAt(c);
		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );		
	}		
}

VOID CMNG_LIB_HOLIDAY_MGR_01::GetDMAlias( CString alias[] )
{
	alias[ 0 ] = _T("REC_KEY");
	alias[ 1 ] = _T("YEAR");
	alias[ 2 ] = _T("MONTH");
	alias[ 3 ] = _T("DAYS");
}

VOID CMNG_LIB_HOLIDAY_MGR_01::AddDataToDM( INT mon, CString days )
{
	const INT cnt = 4;
	CString alias[ cnt ];
	GetDMAlias( alias );
	CString m, data[ cnt ];

	bool add = true;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_01") );
	
	pDM->MakeRecKey( data[ 0 ] );
	data[ 1 ].Format( _T("%s"), m_year );
	data[ 2 ].Format( _T("%d"), mon );
	data[ 3 ].Format( _T("%s"), days );
	
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( alias[ 2 ], i, m );

		if( _ttoi( m ) == mon )
		{
			add = false;
			pDM->SetCellData( alias, cnt, data, i );			
		}
	}

	if( add )
		pDM->SetCellData( alias, cnt, data, -1 );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::SaveDataToFile()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_01_ForFile") );
	CLibFileManager fileMgr;
	fileMgr.SaveDataToFile(pDM, GetUserID());
}

CMNG_LIB_HOLIDAY_MGR_01::~CMNG_LIB_HOLIDAY_MGR_01()
{
	delete m_pLoanShareManager;
	FreeGridEdt();
}

VOID CMNG_LIB_HOLIDAY_MGR_01::CleanGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	for( INT i = 1; i < 13; i++ )
		pGrid->SetTextMatrix( i, 2, _T("") );
}

VOID CMNG_LIB_HOLIDAY_MGR_01::OnInitEslMgr()
{
	if( InitESL_Mgr( _T("MNG_LIB_HOLIDAY_MGR_01") ) < 0 )
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_HOLIDAY_MGR_01") );
}

INT CMNG_LIB_HOLIDAY_MGR_01::OnHolidayMng(VOID)
{
	CMNG_LIB_HOLIDAY_MGR_02 dlg(this);
	dlg.DoModal();

	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::OnHolidayApply(VOID)
{
	if (m_year.GetLength() == 0) {
		AfxMessageBox(_T("먼저 검색을 하여 주십시오."));
		return -1;
	}
	if (AfxMessageBox(_T("등록된 공휴일을 모두 적용하시겠습니까?"), MB_YESNO) != IDYES) return 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_공휴일") );
	if (pDM == NULL) return -1;
	pDM->RefreshDataManager(_T(""));
	INT nRows = pDM->GetRowCount();
	CString strMonth, strDay;
	for (INT i = 0; i < nRows; i++) {
		pDM->GetCellData(_T("MONTH"), i, strMonth);
		strMonth.TrimLeft();		strMonth.TrimRight();
		pDM->GetCellData(_T("DAY"), i, strDay);
		strDay.TrimLeft();			strDay.TrimRight();

		SetHoliday(_ttoi(strMonth), _ttoi(strDay));			
	}
	ShowGrid();
 
	AfxMessageBox(_T("적용되었습니다."));
	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::OnBatchApply(VOID)
{
	if (m_year.GetLength() == 0) {
		AfxMessageBox(_T("먼저 검색을 하여 주십시오."));
		return -1;
	}
	CMNG_LIB_HOLIDAY_MGR_03 dlg(this);
	if (dlg.DoModal() != IDOK) return 0;

	if (dlg.MODE == _T("MONTH")) {
		APPLY_Batch_PerMonth(dlg.m_nStartMonth, dlg.m_nPerMonth, dlg.m_nDay);
	}
	if (dlg.MODE == _T("WEEK")) {
		APPLY_Batch_PerWeek(dlg.m_nStartWeek, dlg.m_nPerWeek, dlg.m_nWeek);
	}

	ShowGrid();
	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::InitHoliday(VOID)
{
	INT i, j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 31; j++) {
			HOLIDAY[i][j] = 'N';
		}
	}
	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::APPLY_Holiday_FromDM(CESL_DataMgr *pDM)
{
	if (pDM == NULL) return -1;

	INT i;
	INT nRow = pDM->GetRowCount();
	CString strMonth, strDays;

	for(i = 0; i < nRow; i++ )
	{
		strMonth = _T("");
		strDays = _T("");
		pDM->GetCellData(_T("MONTH"), i, strMonth);
		pDM->GetCellData(_T("DAYS"), i, strDays);

		strMonth.TrimLeft();			strMonth.TrimRight();
		strDays.TrimLeft();				strDays.TrimLeft();
		if (strMonth.GetLength() == 0) continue;
		if (strDays.GetLength() == 0) continue;

		APPLY_Holiday_ByMonth(_ttoi(strMonth), strDays);
	}
	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::APPLY_Holiday_ByMonth(INT nMonth, CString strDays)
{
	if (nMonth < 1 || nMonth > 12) return -1;

	CString strDay, szTmp;
	INT nDay;
	INT i;
	for (i = 0; i < 31; i++) HOLIDAY[nMonth-1][i] = 'N';
	for (i = 0; true; i++) {
		strDay = _T("");
		strDay = strDays.SpanExcluding(_T(","));
		strDay.TrimLeft();		strDay.TrimRight();
		if (strDay.GetLength() > 0) {
			nDay = _ttoi(strDay);
			if (nDay >= 1 && nDay <= 31) {
				if (IsValidDay(_ttoi(m_year), nMonth, nDay) == TRUE) {
					HOLIDAY[nMonth-1][nDay-1] = 'Y';
				}
			} 
		}
		if (strDay == strDays) break;
		szTmp = strDays.Mid(strDay.GetLength()+1);
		if (szTmp.GetLength() == 0) break;
		strDays = szTmp;
	}

	return 0;
}

BOOL CMNG_LIB_HOLIDAY_MGR_01::IsValidDay(INT nYear, INT nMonth, INT nDay)
{
    INT max_day[]= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if ( (nMonth == 2) && (nYear % 4 == 0) && ( (nYear % 100 != 0) || (nYear % 400 == 0) ) ) {
		max_day[1]= 29;
	}

	if ( nDay < 1 || nDay > max_day[nMonth-1]) {
		return FALSE;
	}
	return TRUE;
}

VOID CMNG_LIB_HOLIDAY_MGR_01::OnModifyHoliDayInfo(VOID)
{
	if (m_year.GetLength() == 0) {
		AfxMessageBox(_T("먼저 검색을 하여 주십시오."));
		return;
	}
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	if (pGrid == NULL) return ;
	INT nMonth = pGrid->GetRow();

	CString strDays;
	strDays = pGrid->GetTextMatrix(nMonth, 2);
	strDays.TrimLeft();			strDays.TrimRight();

	CMNG_LIB_HOLIDAY_MGR_05 dlg(this);
	dlg.m_strDays = strDays;
	dlg.m_nYear = _ttoi(m_year);
	dlg.m_strParentYear = m_year;
	dlg.m_nMonth = nMonth;
	if (dlg.DoModal() != IDOK) return;

	strDays = dlg.m_strDays;
	strDays.TrimLeft();			strDays.TrimRight();

	for ( int i=0 ; i<12 ; i++ )
	{
		APPLY_Holiday_ByMonth(i+1, dlg.m_strDaysList.GetAt(i));
	}	

	ShowGrid();
}

INT CMNG_LIB_HOLIDAY_MGR_01::APPLY_Batch_PerWeek(INT nStartWeek, INT nPerWeek, INT nWeek)
{
	INT nYear = _ttoi(m_year);
	CTime tStartofYearDay(nYear, 1, 1, 12, 0, 0);
	INT nStartofYearDay = tStartofYearDay.GetTime();
	INT nStartDayofWeek = tStartofYearDay.GetDayOfWeek();
	CTime tToday = tStartofYearDay;
	INT nToday;
	INT nTodayWeek;
	INT nTodayDayofWeek;

	while(TRUE) {
		nToday = tToday.GetTime();
		nTodayDayofWeek = tToday.GetDayOfWeek();

		nTodayWeek = (nToday - nStartofYearDay) / (7 * 24 * 60 * 60);
		nTodayWeek += 1;

		if (nStartDayofWeek > nTodayDayofWeek) nTodayWeek += 1;

		INT nYear = tToday.GetYear();
		INT nMonth = tToday.GetMonth();
		INT nDay = tToday.GetDay();

		if (nWeek == nTodayDayofWeek && nTodayWeek >= nStartWeek) {
			if ((nTodayWeek-nStartWeek) % nPerWeek == 0) {
				SetHoliday(tToday.GetMonth(), tToday.GetDay());	
			}
		}

		if (tToday.GetMonth() == 12 && tToday.GetDay() ==31) break;
		nToday = nToday + (24 * 60 * 60);
		tToday = CTime((time_t)nToday);
	}

	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::APPLY_Batch_PerMonth(INT nStartMonth, INT nPerMonth, INT nDay)
{
	INT i;
	for (i = nStartMonth; i <= 12; i += nPerMonth) {
		SetHoliday(i, nDay);
	}
	return 0;
}

INT CMNG_LIB_HOLIDAY_MGR_01::SetHoliday(INT nMonth, INT nDay)
{
	if (nMonth < 1 || nMonth > 12) return -1;
	if (nDay < 1 || nDay > 31) return -2;

	INT nYear = _ttoi(m_year);
	if (IsValidDay(nYear, nMonth, nDay) == FALSE) return -3;

	HOLIDAY[nMonth-1][nDay-1] = 'Y';
	return 0;
}
VOID CMNG_LIB_HOLIDAY_MGR_01::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		CWnd *pWnd  = GetDlgItem( IDC_eYEAR );		
		if (pWnd) {
			CString strYear;
			CTime t = CTime::GetCurrentTime();
			strYear.Format(_T("%04d"), t.GetYear());

			pWnd->SetWindowText(strYear);
			FindHolidayData();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CMNG_LIB_HOLIDAY_MGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
