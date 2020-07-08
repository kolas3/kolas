// UNION_RETURN_STATICS_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_STATICS_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_MAIN dialog


CUNION_RETURN_STATICS_MAIN::CUNION_RETURN_STATICS_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUNION_RETURN_STATICS_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_pResultGrid = NULL;
}

CUNION_RETURN_STATICS_MAIN::~CUNION_RETURN_STATICS_MAIN()
{
	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}
}

void CUNION_RETURN_STATICS_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_STATICS_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_STATICS_MAIN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_STATICS_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_MAIN message handlers

BOOL CUNION_RETURN_STATICS_MAIN::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CUNION_RETURN_STATICS_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{			
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUNION_RETURN_STATICS_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("UnionReturnStatics")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return false;
	}
	
	if( 0 > InitLibraryinfo() )
	{
		AfxMessageBox(_T("InitLibraryinfo ERROR"));
		return false;
	}

	if( 0 > InitLibraryList() )
	{
		AfxMessageBox(_T("InitLibraryList ERROR"));
		return false;
	}
	
	if( 0 > InitStaticsGrid() )
	{
		AfxMessageBox(_T("InitStaticsGrid ERROR"));
		return false;
	}

	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}

	m_pResultGrid = new CESL_Grid(NULL);
	m_pResultGrid->SubclassDlgItem(IDC_GRID_UNION_RETURN_MAIN, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUNION_RETURN_STATICS_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(-2, -2, cx+4, cy+4);
	
}

///////////////////////////////////////////////////////////////////////
// 도서관 코드 입력
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InitLibraryinfo()
{
	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, this );
	if( 0 > ids ) return ids;

	m_strLibraryCode = strLibCode[0];

	return 0;
}

///////////////////////////////////////////////////////////////////////
// 도서관 정보를 설정
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InitLibraryList()
{
	INT		ids;
	// 통합대출공유서버에서 도서관정보 가져오기	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
	ids = RemoteDBMgr.LOAN_Select(pdmLibrary, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) return ids;
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
// 통계 결과 그리드를 초기화 한다.
// * InitLibraryList()가 선행되어야 한다.
// 1. 그리드를 초기화 한다.
// 2. 도서관정보를 로우 정보에 적용한다.
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::InitStaticsGrid()
{
	// 1. 그리드를 초기화 한다.
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	pGrid->SetRows(2);
	pGrid->SetCols(0, 2);

	pGrid->SetTextMatrix(0, 1, _T(""));
	pGrid->SetTextMatrix(1, 0, _T(""));
	pGrid->SetTextMatrix(1, 1, _T(""));

	// 2. 도서관정보를 로우 정보에 적용한다.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	if( 0 < nRowCount )
	{		
		pGrid->SetRows(nRowCount+2);
		CString strLibraryName;

		for(INT i = 0; i < nRowCount; i++ )
		{
			pdmLibrary->GetCellData(_T("도서관명"), i, strLibraryName);			

			pGrid->SetTextMatrix(i+1, 0, strLibraryName);
		}

		pGrid->SetTextMatrix(nRowCount+1, 0, _T("합계"));
	}

	SetGridProperty();
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
// 통계의 컬럼을 초기화 한다.
// 1. 날짜를 계산하여 컬럼 갯수를 구한다.
// 2. 그리드 반영
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::InitColumn(CString strStartDate, CString strEndDate)
{
	// 1. 날짜를 계산하여 컬럼 갯수를 구한다.
/*	CTime tStartDate( atoi(strStartDate.Mid(0,4)), atoi(strStartDate.Mid(5,2)), atoi(strStartDate.Mid(8,2)), 0, 0, 0);
	CTime tEndDate( atoi(strEndDate.Mid(0,4)), atoi(strEndDate.Mid(5,2)), atoi(strEndDate.Mid(8,2)), 0, 0, 0);
	

	while( tStartDate < tEndDate )
	{
		m_arColumnDate.Add(tStartDate.Format("%Y/%m/%d"));

		tStartDate += CTimeSpan(1,0,0,0);
	}
*/
	/*
	// 2. 그리드 반영
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	INT		nColCount = m_arColumnDate.GetSize();
	
	pGrid->SetCols(0, nColCount+1);

	for( INT i = 0; i < nColCount; i++ )
	{
		pGrid->SetTextMatrix(0, i + 1, m_arColumnDate.GetAt(i));
	}

	SetGridProperty();
	*/

	return 0;
}

///////////////////////////////////////////////////////////////////////
// 그리드의 셀에 속성을 준다.
// * 정렬 속성
// * 
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::SetGridProperty()
{

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	if( pGrid->GetRows() > 1 )
		pGrid->SetFixedRows( 1 );
	if( pGrid->GetCols( 0 ) > 1 )
		pGrid->SetFixedCols( 1 );

	int nColCnt = pGrid->GetCols(0);
	int nRowCnt = pGrid->GetRows();
	
	for( int i = 0 ; i < nColCnt ; i++ )
	{
		pGrid->SetColAlignment(i,4);
		
		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);		
		pGrid->SetColWidth(i,0,1500);
	}
	
	for( int j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);	
		pGrid->SetRowHeight( j , 400 );		
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// DM에 데이터 입력
// 1. 입력할 로우 인덱스를 찾는다.
// 2. DM에 입력 
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InsertData(CESL_DataMgr* pDM, CString strLibCode, CString strDate, CString strCount)
{
	INT		nRowIdx;	

	// 1. 입력할 로우 인덱스를 찾는다.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	CString strLibraryCode;

	for(INT i = 0; i < nRowCount; i++ )
	{
		pdmLibrary->GetCellData(_T("도서관코드"), i, strLibraryCode);

		strLibraryCode.TrimLeft();
		strLibraryCode.TrimRight();

		if( strLibraryCode == strLibCode )
		{
			nRowIdx = i;
			break;
		}
	}

	// 2. DM에 입력
	pDM->SetCellData( strDate, strCount, nRowIdx );

	return 0;
}

///////////////////////////////////////////////////////////////////////
// 타관반납 통계
// 1. 조건 입력화면 팝업
// 2. 쿼리 생성
// 3. 통계
// 4. 결과를 DM으로 옮긴다.
// 5. 합계 계산
// 6. 결과 출력
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::UnionReturnStatics()
{
	INT		ids;

	// 1. 조건 입력화면 팝업
	CUNION_RETURN_STATICS_RUN	dlg;
	if( IDOK != dlg.DoModal() )
	{
		return 0;
	}

	InitStaticsGrid();

	// 2. 쿼리 생성
	CString strQuery;
	ids = MakeStaticsQuery( dlg.GetStaticsMode(), 
		                    dlg.GetReturnDateStart(), 
							dlg.GetReturnDateEnd(),
							strQuery );
	if( 0 > ids ) return ids;

	// 3. 통계
	DB_DATA_ARRAY_TYPE* RESULT = NULL;

	INT		nRow = 0;
	INT     nCol = 0;
	
	ids = RS_SelectProc( strQuery.GetBuffer( 0 ), &nRow, &nCol, &RESULT, LOAN, false );
	if( 0 > ids )
	{
		AfxMessageBox(_T("RS_SelectProc ERROR"));
		return ids;
	}

	if( 0 == nRow )
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
		delete RESULT;		
		return 0;
	}	

	// 4. 결과를 DM으로 옮긴다.
	//    최종 그리드 출력 형태를 구하기 위해 내부적으로
	//    DM에서 먼저 처리한 이후에 출력한다.
	CESL_DataMgr dm;
	MakeResultDM(&dm, dlg.GetReturnDateStart(), dlg.GetReturnDateEnd());
	
	DB_DATA_ARRAY_TYPE_SEARCH *DATA = ( DB_DATA_ARRAY_TYPE_SEARCH * )RESULT;

	for( INT i = 0; i < nRow; i++ )
	{
		InsertData(&dm, CString(DATA[ 0 ].db_data[ i ].stringData),
						CString(DATA[ 1 ].db_data[ i ].stringData),
						CString(DATA[ 2 ].db_data[ i ].stringData) );
	}

	// 5. 합계 계산
	INT		nRowCount = dm.GetRowCount();
	INT		nColCount = dm.GetColCount();
	//    가로열 합계
	for( i = 0;i < nRowCount;i++ )
	{
		CString strData;
		INT		nTotal = 0;

		for( INT j = 0; j < nColCount; j++ )
		{
			dm.GetCellData(i, j, strData);
			strData.TrimLeft();
			strData.TrimRight();

			if( strData.IsEmpty() )
			{
				strData = _T("0");
				dm.SetCellData(i, j, strData);
			}

			nTotal += _ttoi(strData);
		}

		strData.Format(_T("%d"), nTotal);
		dm.SetCellData(i, nColCount-1, strData);
	}
	
	//    세로열 합계
	for( i = 0;i < nColCount;i++ )
	{
		CString strData;
		INT		nTotal = 0;

		for( INT j = 0; j < nRowCount; j++ )
		{
			dm.GetCellData(j, i, strData);
			strData.TrimLeft();
			strData.TrimRight();

			if( strData.IsEmpty() )
			{
				strData = _T("0");
				dm.SetCellData(i, j, strData);
			}

			nTotal += _ttoi(strData);
		}

		strData.Format(_T("%d"), nTotal);
		dm.SetCellData(nRowCount-1, i, strData);
	}
	
	// 6. 결과 출력	
	//    세로열 합계가 0 이라면 입력하지 않는다.
	//    입력될 날짜를 구한다.
	CStringArray arRealColumnDate;
	CString      strTotal;

	for( i = 0;i < nColCount;i++ )
	{
		dm.GetCellData(nRowCount-1, i, strTotal);

		if( 0 < _ttoi(strTotal) )
		{
			CString strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType;
			dm.GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
			arRealColumnDate.Add( strFieldAlias );
		}
	}

	//    컬럼을 설정한다
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	nColCount = arRealColumnDate.GetSize();
	
	pGrid->SetCols(0, nColCount+1);

	for( i = 0; i < nColCount; i++ )
	{
		pGrid->SetTextMatrix(0, i + 1, arRealColumnDate.GetAt(i));
	}

	SetGridProperty();

	//    데이터를 입력한다.
	CString strResult;

	for( i = 0;i < nRowCount;i++ )
	{
		for( INT j = 0; j < nColCount; j++ )
		{
			dm.GetCellData(arRealColumnDate.GetAt(j), i, strResult);
			pGrid->SetTextMatrix(i + 1, j + 1, strResult);
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// 결과 DM을 생성한다.
// 1. 컬럼의 갯수를 구한다.
// 2. 컬럼 ALIAS를 입력한다.
// 3. 로우 수는 고정된다.
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::MakeResultDM(CESL_DataMgr* pDM, 
											 CString       strReturnDateStart, 
											 CString       strReturnDateEnd)
{	
	pDM->FreeData();
	pDM->FreeDMRef();

	// 1. 컬럼의 갯수를 구한다.
	CTime tStartDate( _ttoi(strReturnDateStart.Mid(0,4)), _ttoi(strReturnDateStart.Mid(5,2)), _ttoi(strReturnDateStart.Mid(8,2)), 0, 0, 0);
	CTime tEndDate( _ttoi(strReturnDateEnd.Mid(0,4)), _ttoi(strReturnDateEnd.Mid(5,2)), _ttoi(strReturnDateEnd.Mid(8,2)), 0, 0, 0);
	CStringArray arColumnDate;

	while( tStartDate <= tEndDate )
	{
		arColumnDate.Add(tStartDate.Format("%Y/%m/%d"));

		tStartDate += CTimeSpan(1,0,0,0);
	}

	// 2. 컬럼 ALIAS를 입력한다.
	INT		nColumnDateCount = arColumnDate.GetSize();
	pDM->InitDataMgrRef(nColumnDateCount+1);
	pDM->m_nCols = nColumnDateCount+1;

	for( INT i = 0; i<nColumnDateCount; i++ )
	{
		pDM->SetDataMgrRef(i, arColumnDate.GetAt(i), "", "", "", "");
	}
	
	// 합계열 추가
	pDM->SetDataMgrRef(nColumnDateCount, "합계", "", "", "", "");

	// 3. 로우 수는 고정된다.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	for( i = 0; i < nRowCount; i++ )
	{
		pDM->InsertRow(-1);
	}

	// 합계로우 추가
	pDM->InsertRow(-1);

	return 0;
}

///////////////////////////////////////////////////////////////////////
// 쿼리 생성
// 1. 통계 대상 필드, 조건절 필드를 정한다.
// 2. 날짜조건 생성
// 3. 도서관 부호 조건 생성
// 4. 쿼리 생성
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::MakeStaticsQuery(INT     nStaticsMode, 
												 CString strReturnDateStart, 
												 CString strReturnDateEnd,
												 CString &strMakeQuery)
{
	// 1. 통계 대상 필드, 조건절 필드를 정한다.
	CString strStaticsField;
	CString strWhereField;

	if( 0 == nStaticsMode )
	{
		/// 타관반납통계
		strStaticsField = _T("REQUEST_LIB_CODE");
		strWhereField   = _T("UPDATE_LIB_CODE");
	}
	else if( 1 == nStaticsMode )
	{
		/// 타관반납대행통계
		strStaticsField = _T("UPDATE_LIB_CODE");
		strWhereField   = _T("REQUEST_LIB_CODE");
	}

	// 2. 날짜조건 생성
	CString strStartDate;
	CString strEndDate;

	strStartDate.Format(_T("TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')"), strReturnDateStart);
	strEndDate.Format(_T("TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), strReturnDateEnd);

	// 3. 도서관 부호 조건 생성
	// 4. 쿼리 생성
	/*
	SELECT REQUEST_LIB_CODE, REQUEST_DATE, COUNT(1) 
 FROM CT_TEMP_RETURN_INFO_TBL
WHERE 1=1 
GROUP BY REQUEST_LIB_CODE, REQUEST_DATE;

SELECT UPDATE_LIB_CODE, REQUEST_DATE, COUNT(1) 
 FROM CT_TEMP_RETURN_INFO_TBL
WHERE 1=1 
GROUP BY UPDATE_LIB_CODE, REQUEST_DATE;

  nStaticsMode = 0; 1
	*/
	// 2009.02.23 UPDATE BY KSJ : 조건 추가- 타관 반납 요청(STATUS=0)과 완료(STATUS=1)만 통계에 들어간다
	strMakeQuery.Format(_T("SELECT %s, TO_CHAR(REQUEST_DATE,'YYYY/MM/DD'), COUNT(1) ")
						_T("FROM CT_TEMP_RETURN_INFO_TBL ")
						_T("WHERE %s='%s' AND STATUS IN ('0','1') AND REQUEST_DATE BETWEEN %s AND %s ")
						_T("GROUP BY %s, TO_CHAR(REQUEST_DATE,'YYYY/MM/DD')"), 
						strStaticsField, strWhereField, m_strLibraryCode, 
						strStartDate, strEndDate, strStaticsField);
	
	return 0;
}

