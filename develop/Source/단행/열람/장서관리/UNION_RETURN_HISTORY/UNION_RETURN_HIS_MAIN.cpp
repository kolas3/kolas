// UNION_RETURN_HIS_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_HIS_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_MAIN dialog


CUNION_RETURN_HIS_MAIN::CUNION_RETURN_HIS_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pResultGrid		=	NULL;
	m_strSearchLibCode	=	_T("");
	m_nHistoryMode		=	0;	
}

CUNION_RETURN_HIS_MAIN::~CUNION_RETURN_HIS_MAIN()
{
	if( NULL != m_pResultGrid )
	{
		delete	m_pResultGrid;
		m_pResultGrid	=	NULL;
	}
}

void CUNION_RETURN_HIS_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_HIS_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CUNION_RETURN_HIS_MAIN::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_HIS_MAIN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_HIS_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_MAIN message handlers

BOOL CUNION_RETURN_HIS_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("UNION_RETURN_HISTORY_MAIN")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return FALSE;
	}

	INT ids;

	// 센터에서 도서관 List 가져오기
	ids = InitLibList();
	if( 0 > ids )
	{
		return FALSE;
	}
	
	// Grid Control 초기화
	ids = InitHistoryGrid();
	if( 0 > ids )
	{
		AfxMessageBox(_T("InitHistoryGrid ERROR"));
		return FALSE;
	}

	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}
	
	//m_pResultGrid = new CESL_Grid(NULL);
	//m_pResultGrid->SubclassDlgItem(IDC_gridUNION_RETURN_HIS_MAIN, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUNION_RETURN_HIS_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid* pGridRequest	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_REQUEST);
	if( NULL == pGridRequest->GetSafeHwnd() ) 
	{
		return;
	}

	CMSHFlexGrid* pGridUpdate	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_UPDATE);
	if( NULL == pGridUpdate->GetSafeHwnd() ) 
	{
		return;
	}

	m_nCX	=	cx + 4;
	m_nCY	=	cy + 4;

	pGridRequest->MoveWindow( -2 , -2 , m_nCX , m_nCY );
	pGridUpdate->MoveWindow( -3 , -3 , 0 , 0 );
}

BOOL CUNION_RETURN_HIS_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{			
			return TRUE;
		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - Grid Control 초기화
@Parameter
 - 
@Return Value
 - 0 : 성공
@Mini Spec
 1. Grid의 Row와 Column 초기 Set
 2. 이력 종류에 따라, Grid SHOW/HIDE
*/
INT CUNION_RETURN_HIS_MAIN::InitHistoryGrid()
{
EFS_BEGIN

	// 타관반납목록 Grid
	CMSHFlexGrid* pGridRequest	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_REQUEST);
	if( NULL == pGridRequest->GetSafeHwnd() ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitHistoryGrid") );
	}
	// 타관반납대행목록 Grid
	CMSHFlexGrid* pGridUpdate	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_UPDATE);
	if( NULL == pGridUpdate->GetSafeHwnd() ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitHistoryGrid") );
	}

	// 1. Grid의 Row와 Column 초기 Set
	pGridRequest = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납") );
	pGridRequest->SetRows(2);
	pGridRequest->SetCols(0,11);
	
	pGridUpdate = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납대행") );
	pGridUpdate->SetRows(2);
	pGridUpdate->SetCols(0,11);

	// 2. 이력 종류에 따라, Grid SHOW/HIDE
	if( 0 == m_nHistoryMode )
	{
		pGridRequest->MoveWindow( -2 , -2 , m_nCX , m_nCY );
		pGridUpdate->MoveWindow( -3 , -3 , 0 , 0 );
	}
	else if( 1 == m_nHistoryMode )
	{
		pGridUpdate->MoveWindow( -2 , -2 , m_nCX , m_nCY );
		pGridRequest->MoveWindow( -3 , -3 , 0 , 0 );
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_UNION_RETURN_HISTORY_MAIN"));
	pCM->AllControlDisplay();

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - 이력 조회
@Parameter
 - 
@Return Value
 - 0 : 성공
@Mini Spec
 1. 조건 입력화면 팝업
 2. 이력 모드 결정 - 0 : 타관 반납 목록 1 : 타관 반납 대행 목록
 3. 이력 조회 도서관 정보
 4. 이력 조회 조건 날짜
 5. Grid 초기화
 6. 쿼리 생성
 7. 이력 조회
 8. DM에 결과값 입력
 9. Grid에 결과 출력
*/
INT CUNION_RETURN_HIS_MAIN::UnionReturnHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. 조건 입력화면 팝업
	CUNION_RETURN_HIS_RUN	dlg(this);
	if( IDOK != dlg.DoModal() )
	{
		return 0;
	}

	// 2. 이력 모드 결정 - 0 : 타관 반납 목록 1 : 타관 반납 대행 목록
	m_nHistoryMode			=	dlg.GetHistoryMode();

	// 3. 이력 조회 도서관 정보
	m_strSearchLibCode		=	dlg.GetLibCode();
	m_strSearchLibName		=	dlg.GetLibName();

	// 4. 이력 조회 조건 날짜
	m_strReturnDateStart	=	dlg.GetReturnDateStart();
	m_strReturnDateEnd		=	dlg.GetReturnDateEnd();

	// 5. Grid 초기화
	InitHistoryGrid();

	// 6. 쿼리 생성
	CString strQuery;
	ids = MakeHistoryQuery( strQuery );
	if( 0 > ids ) 
	{
		return ids;
	}

	// 7. 이력 조회
	DB_DATA_ARRAY_TYPE* RESULT = NULL;
	
	INT		nRowCnt = 0;
	INT     nColCnt = 0;

	ids = RS_SelectProc( strQuery.GetBuffer( 0 ), &nRowCnt, &nColCnt, &RESULT, LOAN, false );
	if( 0 > ids )
	{
		AfxMessageBox( _T("RS_SelectProc ERROR") , MB_ICONINFORMATION );
		return ids;
	}

	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("검색결과가 없습니다.") , MB_ICONINFORMATION );
		delete RESULT;		
		return 0;
	}

	// 8. DM에 결과값 입력
	DB_DATA_ARRAY_TYPE_SEARCH *DATA = ( DB_DATA_ARRAY_TYPE_SEARCH * )RESULT;

	CESL_DataMgr* pHistoryDM = FindDM(_T("DM_UNION_RETURN_HISTORY"));
	if( NULL == pHistoryDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UnionReturnHistory") );
	}

	CESL_DataMgr* pLibListDM = FindDM(_T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibListDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UnionReturnHistory") );
	}
	INT		nLibListCnt		=	pLibListDM->GetRowCount();

	const INT nCnt = 9;
	CString sDMFieldAlias[nCnt] =
	{
		_T("대출자이름"),	_T("대출자번호"),	_T("등록번호"), 
		_T("서명"),			_T("대출일"),		_T("반납일"), 
		_T("반납예정일"),	_T("대출상태"),		_T("도서관")
	};

	pHistoryDM ->FreeData();

	INT i,j;
	CString strData;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		DWORD* row = NULL;
		pHistoryDM ->InsertRow(-1);
		row = pHistoryDM ->MakeRow(nColCnt);
		pHistoryDM ->InitRow(row);

		for( j=0 ; j<nColCnt ; j++ )
		{
			strData = CString(DATA[ j ].db_data[ i ].stringData);
			
			ids = pHistoryDM ->SetCellData( i , j , strData );
			if( 0 > ids )
			{
				return ids;
			}

			if( 7 == j )
			{
				if( _T("0") == strData )
				{
					strData	= _T("대출");
				}
				else if( _T("1") == strData )
				{
					strData	= _T("반납");
				}
				else if( _T("2") == strData )
				{		
					strData = _T("반납연기");
				}
				else if( _T("3") == strData )
				{
					strData = _T("예약");
				}
				else if( _T("4") == strData )
				{
					strData = _T("예약취소");
				}
				else if( _T("5") == strData )
				{
					strData	= _T("무인반납중");
				}
				else if( _T("6") == strData )
				{
					strData	= _T("분관반납중");
				}
				else if( _T("7") == strData )
				{
					strData	= _T("타관반납중");
				}
				else
				{
					strData	= _T("반납");
				}

				ids = pHistoryDM->SetCellData( _T("대출상태설명") , strData , i );
				if( 0 > ids )
				{
					return ids;
				}
			}

			if( 8 == j )
			{
				CString		strLibCode;
				INT			k;
				for( k=0 ; k<nLibListCnt ; k++ )
				{
					pLibListDM->GetCellData( _T("도서관코드") , k , strLibCode );
					if( 0 > ids )
					{
						return ids;
					}

					if( 0 == strData.Compare(strLibCode) )
					{	
						pLibListDM->GetCellData( _T("도서관명") , k , strData );
						if( 0 > ids )
						{
							return ids;
						}

						ids = pHistoryDM->SetCellData( _T("도서관설명") , strData , i );
						if( 0 > ids )
						{
							return ids;
						}
					}
				}
			}
		}
	}

	// 9. Grid에 결과 출력
	CESL_Grid* pGrid;
	
	if( 0 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납") );
		pGrid->Display();
	}
	else if( 1 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납대행") );
		pGrid->Display();
	}

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - 이력 조회 Query 만들기
@Parameter
 - strMakeHistoryQuery[OUT] : 검색 쿼리
@Return Value
 - 0 : 성공
@Mini Spec
 1. 자관 도서관부호 구하기
 2. 이력 대상 도서관, 조건절 도서관 구분하기
 3. 날짜조건 생성
 4. 이력 대상 도서관 설정
*/
INT CUNION_RETURN_HIS_MAIN::MakeHistoryQuery( CString &strMakeHistoryQuery )
{
	INT		ids;

	// 1.자관 도서관부호 구하기
	CString		strMyLibCode;
	ids = GetMyLibCode(strMyLibCode);
	if( 0 > ids )
	{
		return ids;
	}

	// 2. 이력 대상 도서관, 조건절 도서관 구분하기
	CString strHistoryField;
	CString strWhereField;

	if( 0 == m_nHistoryMode )
	{/// 타관 반납 이력
		strHistoryField =	_T("REQUEST_LIB_CODE");
		strWhereField   =	_T("UPDATE_LIB_CODE");
	}
	else if( 1 == m_nHistoryMode )
	{/// 타관 반납 대행 이력
		strHistoryField =	_T("UPDATE_LIB_CODE");
		strWhereField   =	_T("REQUEST_LIB_CODE");
	}
	
	// 3. 날짜조건 생성
	CString strStartDate;
	CString strEndDate;

	strStartDate.Format(_T("TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')"), m_strReturnDateStart);
	strEndDate.Format(_T("TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), m_strReturnDateEnd);

	strMakeHistoryQuery.Format(	_T("SELECT CTCLU.NAME, ")		//이용자 이름
								_T("CTCLU.USER_NO, ")			//이용자 번호
								_T("CTCL.REG_NO, ")				//자료 등록번호
								_T("CTCL.TITLE, ")				//자료명
								_T("TO_CHAR(CTCL.LOAN_DATE,'YYYY/MM/DD HH24:MI:SS'), ")		//대출일
								_T("TO_CHAR(CTCL.RETURN_DATE,'YYYY/MM/DD HH24:MI:SS'), ")	//반납일
								_T("CTCL.RETURN_PLAN_DATE, ")	//반납예정일
								_T("CTCL.STATUS, ")				//대출 상태
								_T("CTRI.%s ")					//m_nHistoryMode - 0 : 신청한 도서관 1 :처리한 도서관
								_T("FROM CT_TEMP_RETURN_INFO_TBL CTRI, CT_CO_LOAN_INFO_TBL CTCL, CT_MASTER_LOAN_USER_TBL CTCLU ")
								_T("WHERE CTRI.UPDATED_TABLE_REC_KEY=CTCL.CLIENT_REC_KEY ")
								_T("AND CTRI.UPDATE_LIB_CODE=CTCL.LIB_CODE ")
								_T("AND CTCL.MASTER_USER_KEY=CTCLU.REC_KEY ")
								_T("AND CTRI.STATUS IN ('0','1') ")
								_T("AND CTRI.%s='%s' "),
								strHistoryField, 
								strWhereField,
								strMyLibCode
								);

	// 4. 이력 대상 도서관 설정
	CString strTemp;
	
	m_strSearchLibCode.TrimLeft();		
	m_strSearchLibCode.TrimRight();		
	m_strSearchLibCode.Replace( _T(" ") , _T("") );

	if(  _T("") == m_strSearchLibCode )
	{
		// 도서관 선택 안 했을 경우, 전체를 조건으로 검색
		strTemp.Format( _T("AND CTRI.REQUEST_DATE BETWEEN %s AND %s ")	//검색 기간
						_T("ORDER BY CTRI.REQUEST_DATE ASC "),			//요청날짜순으로 정렬
						strStartDate,
						strEndDate );
		strMakeHistoryQuery += strTemp;

		m_strSearchLibName	=	_T("전 체");
	}
	else
	{
		strTemp.Format( _T("AND CTRI.%s='%s' ")//m_nHistoryMode - 0 : 처리한 도서관 , 1 : 신청한 도서관
						_T("AND CTRI.REQUEST_DATE BETWEEN %s AND %s ")	//검색 기간
						_T("ORDER BY CTRI.REQUEST_DATE ASC "),			//요청날짜순으로 정렬
						strHistoryField,
						m_strSearchLibCode,
						strStartDate, 
						strEndDate );
		strMakeHistoryQuery += strTemp;		
	}
	
	return 0;
}

/*
@Create
 - 2009.02.25 BY KSJ
@Description
 - 자관 도서관 코드 구하기
@Parameter
 - strMyLibCode : 도서관 코드 저장 할 변수
@Return Value
 - 0 : 성공
@Mini Spec
 1. MN_LIBINFO2_TBL에서 자관 도서관 코드를 가져온다.
*/
INT CUNION_RETURN_HIS_MAIN::GetMyLibCode(CString &strMyLibCode)
{
	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, this );
	if( 0 > ids ) 
	{
		return ids;
	}

	strMyLibCode = strLibCode[0];
	return 0;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - 도서관 List 가져오기
@Parameter
 - 
@Return Value
 - 0 : 성공
@Mini Spec
 1. 센터 DB에 접속하여 도서관 List를 가져온다
*/
INT CUNION_RETURN_HIS_MAIN::InitLibList()
{
EFS_BEGIN

	INT ids;
	CRS_SQL			RemoteDBMgr(this);
	CESL_DataMgr*	pLibraryDM = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibraryDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitLibList") );
	}

	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) 
	{
		return ids;
	}

	ids = RemoteDBMgr.LOAN_Select(pLibraryDM, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) 
	{
		return ids;
	}

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.27 BY KSJ
@Description
 - SIReport 출력
@Parameter
 - 
@Return Value
 - 0 : 성공
@Mini Spec
 1. 목록 DM 과 출력 DM 초기화
 2. 출력 필드 설정
 3. 출력 Row 설정->선정된 로우가 없다면 전체 출력
 4. pSrcDM -> pPrintDM으로 복사
 5. Data 출력
*/
INT CUNION_RETURN_HIS_MAIN::PrintList()
{
EFS_BEGIN

	INT ids;
	
	// =========================================================================================
	// 1. 목록 DM 과 출력 DM 초기화 
	// =========================================================================================
	CESL_DataMgr* pSrcDM = FindDM( _T("DM_UNION_RETURN_HISTORY") );	
	if( NULL == pSrcDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	}
	INT		nRowCnt = pSrcDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}
	
	CESL_DataMgr* pPrintDM = FindDM( _T("DM_UNION_RETURN_HISTORY_PRINT") );
	if( NULL == pPrintDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintList") );
	}
	pPrintDM->FreeData();

	// =========================================================================================
	// 2. 출력 필드 설정
	// =========================================================================================	
	const int nCopyCnt = 9;
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("대출자이름"), _T("대출자이름") },
		{ _T("대출자번호"), _T("대출자번호") },
		{ _T("등록번호"),	_T("등록번호") },
		{ _T("서명"),		_T("서명") },
		{ _T("대출일"),		_T("대출일") },
		{ _T("반납일"),		_T("반납일") },
		{ _T("반납예정일"), _T("반납예정일") },
		{ _T("대출상태설명"),_T("대출상태설명") },
		{ _T("도서관설명"),	_T("도서관설명") },
	};
	
	// =========================================================================================
	// 3. 출력모드 설정
	//    선정된 로우가 없다면 전체 출력
	// =========================================================================================
	CESL_Grid* pGrid;
	if( 0 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납") );
	}
	else if( 1 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_타관반납대행") );
	}
				
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( 0 < nSelectCnt )
	{
		nMode = 1;
	}
	else
	{
		nSelectCnt = pSrcDM->GetRowCount();
	}
	
	// =========================================================================================
	// 4. pSrcDM -> pPrintDM으로 복사
	// =========================================================================================
	CString strData;
	INT nIndex;
	INT i;
	for( i=0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
		{
			nIndex = i;
		}
		else
		{
			nIndex = pGrid->SelectGetIdx();		
		}

		pPrintDM->InsertRow(-1);
		
		strData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("순번") , strData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintList") );
		
		for( int j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , strData );
			if( 0 > ids ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , strData : %s ") , nIndex , sField[j][0] , strData );
				AfxMessageBox( sEMsg );
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			}
			
			ids = pPrintDM->SetCellData( sField[j][1] , strData , i );
			if( 0 > ids )
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , strData : %s ") , i , sField[j][0] , strData );
				AfxMessageBox( sEMsg );
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintList") );
			}
		}
		if( nMode == 1)
		{
			pGrid->SelectGetNext();
		}
	}
	
	// =========================================================================================	
	// 5. Data 출력
	// =========================================================================================	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( int( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if( NULL == pSIReportManager )
	{
		AfxMessageBox( _T("Report Manager를 생성하지 못하였습니다.") );
		return 1;
	}

	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	if( 0 == m_nHistoryMode )
	{
		ids = pSIReportManager->GetSIReportFileFromDB( _T("타관반납목록") );
		if( 0 > ids ) 
		{
			AfxMessageBox( _T("DB로 부터 SI Report 정보를 가져오지 못하였습니다.") );
			return 1;
		}
	}
	else if( 1 == m_nHistoryMode )
	{
		ids = pSIReportManager->GetSIReportFileFromDB( _T("타관반납대행목록") );
		if( 0 > ids ) 
		{
			AfxMessageBox( _T("DB로 부터 SI Report 정보를 가져오지 못하였습니다.") );
			return 1;
		}
	}

	// 문자 치환 : 검색 기간
	CString		strDate;
	strDate.Format( _T("%s ~ %s") , m_strReturnDateStart , m_strReturnDateEnd );
	ids = pSIReportManager->SetTempVariable( _T("기간") , strDate );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintList") );
	}
	
	// 문자 치환 : 검색 도서관명
	ids = pSIReportManager->SetTempVariable( _T("도서관명") , m_strSearchLibName );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintList") );
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if( NULL != pSIReportManager )
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}


/*
@Create
 - 2009.02.27 BY KSJ
@Description
 - SIReport Event Handle 얻기
@Parameter
 - EventInfo[OUT] : SIReport Event Handle
@Return Value
 - 0 : 성공
@Mini Spec
 1.
*/
INT CUNION_RETURN_HIS_MAIN::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if( NULL == pSIReportManager )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SIReportEventHandler") );
	}
	
	return 0;
	
EFS_END
	return -1;	
}
