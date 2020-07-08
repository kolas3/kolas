// FeeStatisticsDlg.cpp : implementation file
// Make by pwr

#include "stdafx.h"
#include "FeeStatisticsDlg.h"

#include "..\..\..\ESL\ESL\ESLDlg_ExcelExportWait.h"
#include "XLEzAutomation.h"

// SBL
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeStatisticsDlg dialog


CFeeStatisticsDlg::CFeeStatisticsDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeStatisticsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFeeStatisticsDlg::~CFeeStatisticsDlg()
{
}

void CFeeStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeStatisticsDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeStatisticsDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeStatisticsDlg message handlers

BOOL CFeeStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_STATISTICS") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_STATISTICS") );
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;
}

/*
BOOL CFeeStatisticsDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_F12 )
		{
			// F12 : 엑셀화면으로 그리드의 내용을 표시한다.
			CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
			pGrid->OnExcelExport();
			return TRUE;
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
	
}
*/

BOOL CFeeStatisticsDlg::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

void CFeeStatisticsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	const int term = 0;	
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );

}

// ** 검색 (sWhere로 검색 조건이 들어온다. 납부일 ?부터 ?까지)
VOID CFeeStatisticsDlg::SearchData( CString sWhere )
{
EFS_BEGIN

////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. DM 생성
////////////////////////////////////////////////////////////////////////////////////////////////////
	INT ids;
/// 1.1 쿼리 생성 
/// 1.1.1 ( 검색구분과 수수료종류에 따라 납부건수와 납부금액을 검색한다. ) 
	CString sQuery1 = MakeQuery( sWhere );
/// 1.1.2 ( ROW를 결정하기위해 직급만을 따로 검색한다. )
	CString sQuery2 = MakeRowQuery( sWhere );
/// 1.1.3 ( COL을 결정하기위해 날짜만을 따로 검색한다. )
	CString sQuery3 = MakeColQuery( sWhere );

	BEGIN_SBL();

/// 1.2 생성한 쿼리로 DM생성 
/// 1.2.1 ( 검색결과DM : 납부건수, 납부금액 )
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( !pBodyDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pBodyDM->RestructDataManager( sQuery1 );
	if( !( pBodyDM->GetRowCount() > 0 )) 
	{
		AfxMessageBox( _T("검색결과가 없습니다.") );
		return;
	}
/// 1.2.2 ( ROW DM : 직급 )
	CESL_DataMgr *pRowDM	= FindDM( _T("DM_STATISTICS_ROW") );
	if( !pRowDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pRowDM->RestructDataManager( sQuery2 );

/// 1.2.3 ( COL DM : 날짜 )
	CESL_DataMgr *pColDM	= FindDM( _T("DM_STATISTICS_COL") );
	if( !pColDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pColDM->RestructDataManager( sQuery3 );

	END_SBL(pRowDM->GetRowCount());

/// 1.3 직급을 직급명으로 변경한다.
/// 1.3.1 ( CodeDM에서 직급코드와 직급명을 가져온다. )
	INT nCodeCount;
	CString strCode[256], strDesc[256];
	GetCode( strCode, strDesc, nCodeCount );
/// 1.3.2 ( 검색결과DM 직급코드 직급명으로 변환 )
	ids = ConvertCodeToDescInDM( pBodyDM, strCode, strDesc, nCodeCount );
/// 1.3.3 ( ROW DM 직급코드 직급명으로 변환 )
	ids = ConvertCodeToDescInDM( pRowDM, strCode, strDesc, nCodeCount );

////////////////////////////////////////////////////////////////////////////////////////////////////
// 2. 그리드 초기화 ( Row와 Col 크기설정 )
////////////////////////////////////////////////////////////////////////////////////////////////////
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );

	InitGrid( pRowDM, pColDM, pGrid );

////////////////////////////////////////////////////////////////////////////////////////////////////
// 3. 그리드 값 입력
////////////////////////////////////////////////////////////////////////////////////////////////////
	INT nRow, nCol;
	nRow = pRowDM->GetRowCount();
	nCol = pColDM->GetRowCount();

	CString sClass;	//직급
	CString sMoney;	//납부금액
	CString sCnt;	//납부건수
	CString sDate;	//날짜
	for( INT i = 0; i < pBodyDM->GetRowCount(); i++ )
	{
		pBodyDM->GetCellData( _T("직급")	, i, sClass );
		pBodyDM->GetCellData( _T("납부금액"), i, sMoney );
		pBodyDM->GetCellData( _T("납부건수"), i, sCnt );
		pBodyDM->GetCellData( _T("날짜")	, i, sDate );

		InserToGrid( pGrid, sClass, sMoney, sCnt, sDate );
	}
///합계보여주기
	ViewSum( pGrid );
///그리드 셀크기 및 정렬
	SetFixProperty();

EFS_END
}

// 2008.02.29 add by pwr
// ** DM의 직급코드를 직급명으로 변경한다.
// -1 : Target DM NULL
// -2 : Target DM RowCount==0
INT CFeeStatisticsDlg::ConvertCodeToDescInDM ( CESL_DataMgr *pTargetDM, CString strCode[], CString strDesc[], INT nCodeCount )
{
	INT i, j, ids;

	if (!pTargetDM) return -1;
	
	INT nRowCount = pTargetDM->GetRowCount();
	if( nRowCount == 0 ) return -2;
	
	CString strTmpCode;
	for (i = 0; i < nRowCount; i++) 
	{
		ids = pTargetDM->GetCellData( _T("직급"), i, strTmpCode );
		for (j = 0; j < nCodeCount; j++) 
		{
			if (strTmpCode == strCode[j]) 
			{
				pTargetDM->SetCellData( _T("직급"), strDesc[j], i);
				break;
			}
		}
	}
	return 0;
}

// 2008.02.29 add by pwr
// ** 직급코드, 직급명을 가져온다.
// -1 : 직급코드DM Reference 존재 하지 않는다.
// -2 : CodeDM Restruct실패 
// -3 : 직급코드와 명이 존재 하지 않는다. 
INT CFeeStatisticsDlg::GetCode( CString strCode[], CString strDesc[], INT &nCodeCount )
{
	INT ids;
	CString sQuery = _T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=31");
	CESL_DataMgr *pCodeDM	= FindDM( _T("DM_STATISTICS_CLASS") );
	if( !pCodeDM ) return -1;
	ids = pCodeDM->RestructDataManager( sQuery );
	if( ids < 0 ) return -2;

	nCodeCount = pCodeDM->GetRowCount();
	if( nCodeCount == 0 ) return -3;

	for(INT i = 0; i< nCodeCount; i++)
	{
		pCodeDM->GetCellData(i, 0, strCode[i] ); //직급코드
		pCodeDM->GetCellData(i, 1, strDesc[i] ); //직급명
	}
	return 0;
}

// 2008.02.28 add by pwr
// **그리드를 초기화한다
// (검색조건으로 직급과 날짜를 구해서 Row와 Col을 결정한다.)
INT CFeeStatisticsDlg::InitGrid(CESL_DataMgr *pRowDM, CESL_DataMgr *pColDM, CESL_Grid *pGrid)
{
	INT nRow, nCol;
	nRow = pRowDM->GetRowCount();
	nCol = pColDM->GetRowCount();

/// 그리드 초기화
	pGrid->Clear();

/// DM은 0부터 시작이라 1를 더한다.
	pGrid->SetRows( nRow + 1 );
	pGrid->SetCols( 0, nCol + 1 );

	CString sClass;
	CString sDate;
/// X축 ROW : 직급
	for(INT i = 0; i < nRow; i++)
	{
		pRowDM->GetCellData( _T("직급"), i, sClass ); 
		pGrid->SetTextMatrix( i+1, 0, sClass );
	}
/// Y축 COL : 날짜 
	for(i = 0; i < nCol; i++)
	{
		pColDM->GetCellData( _T("날짜"), i, sDate ); 
		pGrid->SetTextMatrix( 0, i+1, sDate );
	}

	return 0;
}

// 2008.02.28 add by pwr
// ** 그리드에 DM의 검색된 데이터를 INSERT한다.
VOID CFeeStatisticsDlg::InserToGrid( CESL_Grid *pGrid, CString sClass, CString sMoney, CString sCnt, CString sDate )
{
	INT nX, nY;
	CString sData;
	CString sSumCnt;
	CString sSumMoney;

/// 해당 직급에 날짜에 해당하는 그리드의 x,y를 구한다.
	GetLocate( nX, nY, sClass, sDate );
	
/// 해당 값을 가져와서 넣을 값과 합친다. ->   (건)\n납부금액
	sData = pGrid->GetTextMatrix( nX, nY );
	if( !sData.IsEmpty() )
	{
		// 기존 그리드의 값과 합친다.
		INT nCnt, nMoney;
		GetSeperationValue( nCnt, nMoney, sData );
		nCnt += _ttoi( sCnt );
		nMoney += _ttoi( sMoney );
		sSumCnt.Format( _T("(%d)\n"), nCnt );
		sSumMoney.Format( _T("%d"), nMoney );
		SetMoneyFormat( sSumMoney );
		sData = sSumCnt + sSumMoney;
	}
	else
	{
		// 납부건수와 납부금액을 합친다.
		SetMoneyFormat( sMoney );
		sData = _T("(") + sCnt + _T(")") + _T("\n") + sMoney;
	}

/// 해당 좌표에 값 입력
	pGrid->SetTextMatrix( nX, nY, sData );
}

// 2008.02.28 add by pwr
// ** 천원단위 표현
VOID CFeeStatisticsDlg::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace(_T(","), _T(""));

	CString strBuf = sMoney;
	strBuf.Replace(_T(","), _T(""));

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.02.28 add by pwr
// ** 데이터가 들어갈 그리드의 위치를 찾는다.
VOID CFeeStatisticsDlg::GetLocate( INT &nX, INT &nY, CString sClass, CString sDate )
{
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	INT i = 0;
	nX = nY = 0;

/// X축 ROW
	INT rows = pGrid->GetRows();
	for( i = 1; i < rows; i++ )
	{
		if( sClass == pGrid->GetTextMatrix( i, 0 ) )
		{
			nX = i;
		}
	}
/// Y축 COL
	INT cols = pGrid->GetCols( 0 );
	for( i = 1; i < cols; i++ )
	{
		if( sDate == pGrid->GetTextMatrix( 0, i ) )
		{
			nY = i;
		}
	}
}

// 2008.02.28 add by pwr
// ** 그리드 셀크기 및 정렬 
VOID CFeeStatisticsDlg::SetFixProperty()
{
EFS_BEGIN

	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetRows() > 1 )
		pGrid->SetFixedRows( 1 );
	if( pGrid->GetCols( 0 ) > 1 )
		pGrid->SetFixedCols( 1 );

/// 그리드의 폭과 정렬시키기
	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();
	
/// COL
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
		{
			pGrid->SetColAlignment(i,1);
		}
		else
		{
			pGrid->SetColAlignment(i,4);
		}

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);
		pGrid->SetColWidth(i,0,1250);
	}
	
/// ROW
	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
		pGrid->SetCellAlignment(1);
		pGrid->SetRowHeight( j , 500 );	
	}

	/*
	INT ids = ControlDisplay( _T("CM_FEE_STATISTICS"), _T("MainGrid") );	
	if( ids < 0 )
	{
		CString sErrorMsg = _T("ControlDisplay Error::CM_FEE_STATISTICS-MainGrid");
		if( ids == -1 )
			sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
		else if( ids == -1006 )
			sErrorMsg = _T("ControlDisplay Error::'CM_FEE_STATISTICS' CM을 찾을 수 없음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
	}
	*/

EFS_END
}

// 2008.02.28 add by pwr
// ** 합계 보여주기
VOID CFeeStatisticsDlg::ViewSum( CESL_Grid *pGrid )
{
	INT nRows = pGrid->GetRows();
	INT nCols = pGrid->GetCols( 0 );
	INT nSumCnt;
	INT nSumMoney;
	CString percent;
	
	pGrid->SetCols( 0, nCols + 1 );
	pGrid->SetRows( nRows + 1 );
	pGrid->SetTextMatrix( 0, nCols, _T("합계") );
	pGrid->SetTextMatrix( nRows, 0, _T("합계") );
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 가로 합계
	CString sData;
	for( INT i = 1; i < nRows; i++ )
	{
		nSumCnt = 0;
		nSumMoney = 0;
		for( INT j = 1; j < nCols; j++ )
		{
			// 값을 납부건수와 납부금액을 분리해서 계산
			sData = pGrid->GetTextMatrix( i, j );
			INT nCnt, nMoney; 
			nCnt = nMoney = 0;
			if( !sData.IsEmpty() )
				GetSeperationValue( nCnt, nMoney, sData );
			nSumCnt += nCnt;
			nSumMoney += nMoney;
		}	
		// 분리한 값을 합치기
		CString sMoney;
		sMoney.Format( _T("%d"), nSumMoney );
		SetMoneyFormat( sMoney );
		sData.Format( _T("(%d)\n") , nSumCnt );
		sData += sMoney;
		pGrid->SetTextMatrix( i, nCols, sData );
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 세로 합계 ( 가로합계 포함 )
	for( i = 1; i < nCols+1; i++)
	{
		nSumCnt = 0;
		nSumMoney = 0;
		for( INT j = 1; j < nRows; j++)
		{
			// 값을 납부건수와 납부금액을 분리해서 계산
			sData = pGrid->GetTextMatrix( j, i );
			INT nCnt, nMoney;
			nCnt = nMoney = 0;
			if( !sData.IsEmpty() )
				GetSeperationValue( nCnt, nMoney, sData );
			nSumCnt += nCnt;
			nSumMoney += nMoney;
		}
		// 분리한 값을 합치기
		CString sMoney;
		sMoney.Format( _T("%d"), nSumMoney );
		SetMoneyFormat( sMoney );
		sData.Format( _T("(%d)\n") , nSumCnt );
		sData += sMoney;
		pGrid->SetTextMatrix( nRows, i, sData );
	}
}

// 2008.02.28 add by pwr
// ** 납부건수와 납부금액으로 합쳐진 데이타를 분리하여 각각을 반환
VOID CFeeStatisticsDlg::GetSeperationValue( INT &nCnt, INT &nMoney, CString sData )
{
	CString sClass;
	CString sMoney;
	INT nLen = sData.GetLength() - 1;

/// 납부건수
	INT nBase = 0;
	nBase = sData.Find( _T(")") );
	sClass = sData.Left( nBase );
	sClass.Replace( _T("("), _T("") );
	sClass.Replace( _T(")"), _T("") );
	nCnt = _ttoi( sClass );

/// 납부금액
	sMoney = sData.Right( nLen - nBase );
	sMoney.Replace( _T("\n"), _T("") );
	sMoney.Replace( _T(","), _T("") );
	nMoney = _ttoi( sMoney );
}

// 2008.02.28 add by pwr
// **검색 구분별로 조건을 리턴한다.
CString CFeeStatisticsDlg::GetSearchType()
{
	CString sSearchType;
	if( m_cSearchType == 'Y' )
	{//년별
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY' ) ");
	}
	else if( m_cSearchType == 'M' )
	{//월별
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY/MM' ) ");
	}
	else if( m_cSearchType == 'D' )
	{//일별
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY/MM/DD' ) ");
	}
	return sSearchType;
}

// 2008.02.28 add by pwr
// ** 쿼리 생성1 
// (검색구분과 수수료종류에 따라 납부건수와 납부금액을 검색한다.)
CString CFeeStatisticsDlg::MakeQuery( CString sWhere )
{
	CString sQuery;

/// 검색구분별로 조건을 받는다.
	CString sSearchType = GetSearchType();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 전체 (연체료, 회원증재발급, 대출정지일해제)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //자료실 조건없다
		{
			sQuery.Format(	
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('1','2') ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else // 자료실 조건이 있다면 소급입력(연체료)은 제외해서 검색
		{
			sQuery.Format(	
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") //관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("%s ") // 자료실
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("%s ") // 자료실
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('1','2') ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 연체료
	else if( m_cFeeType == 'A' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) // 자료실 조건없다
		{
			sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else // 자료실 조건이 있다면 소급입력(연체료)은 제외해서 검색
		{
			sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 자료실
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 자료실
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 회원증재발급
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.CLASS_CODE IN('1') ")
		_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("%s ") // 관리구분
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		// 2009.03.09 ADD BY PWR : 소급입력 추가
		_T("UNION ALL ")
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.RETRO_YN='Y' ")
		_T("AND CFPI.STATUS='Y' ")
		_T("AND CFPI.CLASS_CODE IN('1') ")
		_T("%s ") // 관리구분
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 대출정지일 해제
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.CLASS_CODE IN('2') ")
		_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
		_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0원제외 )
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("%s ") // 관리구분
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		// 2009.03.09 ADD BY PWR : 소급입력 추가
		_T("UNION ALL ")
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.RETRO_YN='Y' ")
		_T("AND CFPI.STATUS='Y' ")
		_T("AND CFPI.CLASS_CODE IN('2') ")
		_T("AND CFPI.MONEY_PAID!=0 ")
		_T("%s ") // 관리구분
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
		
	return sQuery;
}

// 2008.02.28 add by pwr
// ** 쿼리 생성2
// (ROW를 결정하기위해 직급만을 따로 검색한다.)
CString CFeeStatisticsDlg::MakeRowQuery( CString sWhere )
{
	CString sQuery;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 전체 (연체료, 회원증재발급, 대출정지일해제)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //자료실 조건없다
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : 소급입력 추가
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere );
		}
		else // 자료실 조건이 있다면 소급입력(연체료)은 제외해서 검색
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : 소급입력 추가
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
				, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 연체료
	else if( m_cFeeType == 'A' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //자료실 조건없다
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : 소급입력 추가
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
			, sWhere, m_strManageCodeWhere
			, sWhere, m_strManageCodeWhere
			, sWhere, m_strManageCodeWhere );
		}
		else // 자료실 조건이 있다면 소급입력(연체료)은 제외해서 검색
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY CLU.USER_CLASS_CODE ")
			, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
			, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 회원증재발급
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE ")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ")
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE ")
		, sWhere, m_strManageCodeWhere
		, sWhere, m_strManageCodeWhere );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 대출정지일 해제
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE ")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ")
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // 관리구분
			_T("GROUP BY CLU.USER_CLASS_CODE ")
		, sWhere, m_strManageCodeWhere
		, sWhere, m_strManageCodeWhere );
	}

	return sQuery;
}

// 2008.02.28 add by pwr
// ** 쿼리 생성3
// (COL을 결정하기위해 날짜만을 따로 검색한다.)
CString CFeeStatisticsDlg::MakeColQuery( CString sWhere )
{
	CString sQuery;

/// 검색구분별로 조건을 받는다.
	CString sSearchType = GetSearchType();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 전체 (연체료, 회원증재발급, 대출정지일해제)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //자료실 조건없다
		{
			sQuery.Format(
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				// 2009.03.09 ADD BY PWR : 소급입력 추가
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else
		{
			sQuery.Format(
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // 자료실
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
				// 2009.03.09 ADD BY PWR : 소급입력 추가
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("%s ") // 관리구분
				_T("GROUP BY %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 연체료
	else if( m_cFeeType == 'A' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( 연체료는0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 회원증재발급
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 대출정지일 해제
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( 납부완료자료만 )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0원제외 )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : 소급입력 추가
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // 관리구분
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}

	return sQuery;
}

// 2008.02.29 add by pwr
// ** 엑셀 파일로 반출
// 다음 3가지 방식으로 주석 처리로 바로 사용가능하다.
// 방식 1 : Excel Automation (속도가 빠르다, 엑셀이 설치되어있어야한다.)
// 방식 2 : CESL_Grid의 F12이며 이것도 Automation사용한것이다. ( 폭과 너비가 기본이라 잘려보인다.)
// 방식 3 : ExcelManager (속도가 느리다. OCX가 설치되어있으면 된다.)
INT CFeeStatisticsDlg::MakeGridToExcelFile()
{
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 방식 1 : EXCEL AUTOMATION
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
/// 통계낸 자료가 있는지를 확인한다.
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( pBodyDM->GetRowCount() <= 0 ) 
	{
		return -1;
	}
	
	TCHAR TmpDir[1024] ;
	ZeroMemory(TmpDir, 1024);
	GetCurrentDirectory(1024,TmpDir);

/// 파일 다이얼로그
	BOOL bMode = FALSE;
	CFileDialog dlg( bMode, _T("xls"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("파일형식(*.xls)|*.xls;|All Files(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) 
	{
		// 경로와 이름 저장
		CString sFileName = dlg.GetPathName();
		
		INT nWidth, nHeight, i, j, k, nRow, nCol;
		
		TCHAR szLineData[10000];
		CRect rect, DlgRect;
		CString strPathName, strData, strTmpData ;
		CFile file;

		// 진행상황 표시 ( 프로그래스바 )		
		GetClientRect(&rect);		
		nWidth	= DlgRect.Width();
		nHeight = DlgRect.Height();
		
		DlgRect = rect;
		DlgRect.top		= (nHeight - 50) / 2;
		DlgRect.bottom	= DlgRect.top + 50;
		DlgRect.left	= (nWidth - 175) / 2;
		DlgRect.right	= DlgRect.left + 175;
		
		CESLDlg_ExcelExportWait WaitDlg(this);
		WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, this);
		WaitDlg.MoveWindow( DlgRect );
		WaitDlg.CenterWindow(NULL);
		//WaitDlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		WaitDlg.ShowWindow(TRUE);	
		WaitDlg.m_Progress1.SetPos(0);
		WaitDlg.m_Progress1.SetRange(0,100);
		CString strTotalData;
		CString strTmp2;
		FLOAT	nProgressStep=0;
		FLOAT	nProgress =0;
		CStringArray strTotalExportLine;
		
		nRow = pGrid->GetRows();
		nCol = pGrid->GetCols(0);	
		nProgressStep=(float)100/nRow;
		BOOL bRemainder;	
		
		strTotalExportLine.RemoveAll();
		bRemainder = FALSE;
		for( i = 0; i < nRow; i++ )
		{
			strData.Empty();	strTmpData.Empty();	
			memset(szLineData, 0, 10000);
			for( j = 0; j < nCol; j++ )
			{			
				strData = pGrid->GetTextMatrix(i, j);
				//날짜를 표시 하기 위해서 년도를 두자리로 표현하고 앞에 '를 붙였다.
				//#######으로 표시되는것을 막기위한 조치
				if( !(strData.Find( _T("/") ) < 0) )
				{
					strData = _T("'") + strData.Right(8);
				}
				strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");						
			}		
			nProgress += nProgressStep;
			WaitDlg.m_Progress1.SetPos((INT)nProgress);
			
			strTmpData = strTmpData + _T("\n");
			
			_tprintf(szLineData, _T("%s"), strTmpData );
			
			INT nLen = _tcsclen(szLineData);
			for ( k=0 ; k < nLen ; k++)
			{
				if ( szLineData[k] == 13 ) 
				{
					szLineData[k] = 32;						
				}
			}		
			strTotalData += szLineData;
			if ( i%1000 == 0 ) 
			{
				strTotalExportLine.Add(strTotalData);
				strTotalData = _T("");
				bRemainder = FALSE;
			}
			else if ( i%1000 == 1)
			{
				bRemainder = TRUE;
			}		
		}
		
		if ( bRemainder )
		{
			strTotalExportLine.Add(strTotalData);
			strTotalData = _T("");
		}
		
		for ( i = 0 ; i < strTotalExportLine.GetSize() ; i++ )
		{
			strTotalData += strTotalExportLine.GetAt(i);
		}
		
		WaitDlg.m_Progress1.SetPos(100);
		
		//TRUE : 엑셀띄우기, FALSE : 엑셀화면 안보여준다.
		CXLEzAutomation XL(FALSE);
		XL.ExportCString( strTotalData );
		//XL.CreateXYChart(2);	
		//XL.Create3YChart(0,13);
		XL.SaveFileAs(sFileName);
		//XL.OpenExcelFile(sFileName);
		//Close Excel and release OLE. 
		XL.ReleaseExcel();
		WaitDlg.ShowWindow(FALSE);

		SetCurrentDirectory(TmpDir);

		CString sMsg;
		sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , sFileName );
		AfxMessageBox( sMsg );
	}
	else
	{
		return -2;
	}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 방식 2 : ESL_Grid의 F12와 동일 Automation사용 화면에 바로 띄운다.
////////////////////////////////////////////////////////////////////////////////////////////////////////
	pGrid->OnExcelExport();

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 방식 3 : ExcelManager
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*	
	INT i, j;
/// 통계낸 자료가 있는지를 확인한다.
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( pBodyDM->GetRowCount() <= 0 ) 
	{
		return -1;
	}
	
	TCHAR TmpDir[1024] ;
	ZeroMemory(TmpDir, 1024);
	GetCurrentDirectory(1024,TmpDir);

/// 파일 다이얼로그
	BOOL bMode = FALSE;
	CFileDialog dlg( bMode, _T("xls"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("파일형식(*.xls)|*.xls;|All Files(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) 
	{
		// 경로와 이름 저장
		CString sFileName = dlg.GetPathName();

	//////////////////////////////////////////////////////////////////////////////
	// 4. 엑셀 초기화
	//////////////////////////////////////////////////////////////////////////////
	CRect rc;
	GetClientRect(&rc);	
	
	C_ExcelManControl ExcelCtrl;
	ExcelCtrl.Create( _T("Excel Control"), BS_DEFPUSHBUTTON, rc, this, 8805);
	if (ExcelCtrl.m_hWnd == NULL)
	{
		MessageBox( _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."), _T("Warnning!"), MB_ICONEXCLAMATION );
		return -2;
	}

    CExcelManager ExcelManager(&ExcelCtrl);
    if(ExcelManager.OpenManager() == FALSE) 
	{
        MessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!"), _T("Warnning!"), MB_ICONEXCLAMATION );
		return -3;
	}

	// 진행상황 표시 ( 프로그래스바 )
	CRect rect, DlgRect;
	INT nWidth, nHeight;

	GetClientRect(&rect);		
	nWidth	= DlgRect.Width();
	nHeight = DlgRect.Height();
	
	DlgRect = rect;
	DlgRect.top		= (nHeight - 50) / 2;
	DlgRect.bottom	= DlgRect.top + 50;
	DlgRect.left	= (nWidth - 175) / 2;
	DlgRect.right	= DlgRect.left + 175;
	
	CESLDlg_ExcelExportWait WaitDlg(this);
	WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, this);
	WaitDlg.MoveWindow( DlgRect );
	WaitDlg.CenterWindow(NULL);
	//WaitDlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	WaitDlg.ShowWindow(TRUE);	
	WaitDlg.m_Progress1.SetPos(0);
	WaitDlg.m_Progress1.SetRange(0,100);
	FLOAT	nProgressStep=0;
	FLOAT	nProgress =0;
	

	// 4.1) 폰트를 설정한다.
	ExcelManager.Font(1, 1, 40, 1, TRUE, FALSE, RGB(0, 0, 0), 11);

	// 4.2) 수평 정렬을 맞춘다.
	//      Horizontal (1 - 표준),      (2 - 왼쪽),      (3 - 가운데),               (4 - 오른쪽), 
	//                 (5 - 채우기),    (6 - 양쪽 맞춤), (7 - 선택 영역의 가운데로), (8 - 배분)
    ExcelManager.Horizontal_Alignment(1, 1, 40, 1, 3);

	// 직급 ( ROW )
	CESL_DataMgr *pRowDM	= FindDM( _T("DM_STATISTICS_ROW") );
	INT nRowCnt = pRowDM->GetRowCount();
	nProgressStep=(float)100/nRowCnt;
	for( i = 0; i < nRowCnt+1; i++ )
	{
		CString sData;
		pRowDM->GetCellData( _T("직급"), i, sData );
		if( i == nRowCnt )
		{
			sData = _T("합계");
		}
		ExcelManager.SetTextMatrix( i+2, 1, sData );
	}
	// 날짜 ( COL )
	CESL_DataMgr *pColDM	= FindDM( _T("DM_STATISTICS_COL") );
	INT nColCnt = pColDM->GetRowCount();
	for( i = 0; i < nColCnt+1; i++ )
	{
		CString sData;
		pColDM->GetCellData( _T("날짜"), i, sData );
		if( !(sData.Find( _T("/") ) < 0) )
		{
			sData = _T("'") + sData.Right(8);
		}
		if( i == nColCnt )
		{
			sData = _T("합계");
		}
		ExcelManager.SetTextMatrix( 1, i+2, sData );
	}

	INT nRow, nCol;
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	nRow = pGrid->GetRows();
	nCol = pGrid->GetCols(0);
	for( i = 1; i < nRow; i++ )
	{
		for( j = 1; j < nCol; j++)
		{
			CString sData;
			sData = pGrid->GetTextMatrix( i, j );
			ExcelManager.SetTextMatrix( i+1, j+1, sData );
		}
		nProgress += nProgressStep;
		WaitDlg.m_Progress1.SetPos((INT)nProgress);
	}
	ExcelManager.SaveAs( sFileName );
	WaitDlg.m_Progress1.SetPos(100);
	// 5.3) 사용한 엑셀 객체를 반환한다.
    ExcelManager.CloseManager();

	//////////////////////////////////////////////////////////////////////////////
	// 6. 마무리
	//////////////////////////////////////////////////////////////////////////////
	SetCurrentDirectory(TmpDir);

	CString sMsg;
	sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , sFileName );
	AfxMessageBox( sMsg );
	}
	else
	{
		return -4;
	}
*/
	return 0;
}

HBRUSH CFeeStatisticsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}