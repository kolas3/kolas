// BO_LOC_3100_PrintReceipt.cpp : implementation file
//

#include "stdafx.h"
#include "bo_loc_31001.h"
#include "lprn_api.h"
#include "BO_LOC_3100_PrintReceipt.h"
#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3100_PrintReceipt dialog


CBO_LOC_3100_PrintReceipt::~CBO_LOC_3100_PrintReceipt()
{
	
}

CBO_LOC_3100_PrintReceipt::CBO_LOC_3100_PrintReceipt(CLonaManageValue* pLMV, CESL_Mgr* pParent/*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3100_PrintReceipt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParentMgr = pParent;

	m_pLMV = pLMV;
}


VOID CBO_LOC_3100_PrintReceipt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3100_PrintReceipt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CBO_LOC_3100_PrintReceipt::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BEGIN_MESSAGE_MAP(CBO_LOC_3100_PrintReceipt, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3100_PrintReceipt)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btn3100_CLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radLOAN, OnRadSelChange)
	ON_BN_CLICKED(IDC_radRETURN, OnRadSelChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3100_PrintReceipt message handlers

BOOL CBO_LOC_3100_PrintReceipt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = 0;
	CString sErrMsg = _T("");

	if(InitESL_Mgr(_T("BO_LOC_3100_RECEIPT")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	ids = initRctUseType( );

	ids = initData( );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initData( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
	}
	if ( 0!=ids ) OnCancel( );
	
	ids = initControl( );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initControl( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
	}
	if ( 0!=ids ) OnCancel( );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_LOC_3100_PrintReceipt::initData()
{
	INT ids = 0 ;
	CString sValue = _T("");

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sWhere = _T("");

	ids = getReceiptWhere( sWhere );
	if ( ids != 0 ) return (ids);
	
	ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
	if ( ids < 0 ) return -2;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::initControl()
{
	INT ids = 0;

	CString sUserName = _T("");
	CString sUserNo = _T("");
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자이름"), sUserName, 0 );
	if ( ids < 0 )
		return -1;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), sUserNo, 0 );
	if ( ids < 0 )
		return -2;

	SetWindowText( _T("영수증 출력") );
	GetDlgItem( IDC_edtUSER_NO )->SetWindowText( sUserNo );
	GetDlgItem( IDC_edtUSER_NAME )->SetWindowText( sUserName );

	ids = AllControlDisplay( sCMAlias );
	if ( ids < 0 ) return -3;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::initRctUseType()
{
	CString sRctUseType = _T("");

	sRctUseType = m_pLMV->m_sUseReceiptType;

	if ( _T("YY") != sRctUseType )
	{
		if ( 'Y' == sRctUseType.GetAt(0) )
			((CButton*)GetDlgItem( IDC_radLOAN ))->SetCheck( 1 );
		else
			((CButton*)GetDlgItem( IDC_radRETURN ))->SetCheck( 1 );
		
		GetDlgItem( IDC_STATIC1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_radLOAN )->EnableWindow( FALSE );
		GetDlgItem( IDC_radRETURN )->EnableWindow( FALSE );
	}
	else
		((CButton*)GetDlgItem( IDC_radLOAN ))->SetCheck( 1 );

	return 0;
}

VOID CBO_LOC_3100_PrintReceipt::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

VOID CBO_LOC_3100_PrintReceipt::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect gRect;
	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT);
	
	if(pGrid->GetSafeHwnd() == NULL){
		return;
	}

	pGrid->GetWindowRect( &gRect );

	ScreenToClient( gRect );

	pGrid->MoveWindow(gRect.left, gRect.top, (cx-20), (cy - gRect.top)-10 );
}

VOID CBO_LOC_3100_PrintReceipt::OnRadSelChange() 
{
	INT ids = 0;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sWhere = _T("");

	ids = getReceiptWhere( sWhere );
	if ( ids != 0 ) return;
	
	ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
	if ( ids < 0 ) return;

	ids = AllControlDisplay( sReceiptCMAlias );
	if ( ids < 0 ) OnCancel( );
}

VOID CBO_LOC_3100_PrintReceipt::OnbtnPRINTRECEIPT() 
{
	INT ids = 0;

	ids = doPrintReceipt( );
	if ( ids < 0 ) OnCancel( );
	else if ( 0 != ids ) return;
}

VOID CBO_LOC_3100_PrintReceipt::OnbtnCLOSE() 
{
	OnCancel( );
}

INT CBO_LOC_3100_PrintReceipt::getReceiptWhere(CString &sWhere)
{
	INT ids = 0;

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTmpWhere = _T("");
	CString sUserKey = _T("");

	ids = GetDataMgrData( sMemberDMAlias, _T("대출자KEY"), sUserKey, 0 );
	if ( ids < 0 )
		return -1;
	
	if ( !sUserKey.IsEmpty( ) )
		sWhere += _T(" CL.USER_KEY = ") + sUserKey;
	else
	{
		AfxMessageBox( _T("대출자를 선택하고 진행하십시요!") );
		return 1;
	}

	if ( 1 == ((CButton*)GetDlgItem( IDC_radLOAN ))->GetCheck( ) )
	{
		sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
		sTmpWhere += _T(" AND CL.RECEIPT_PRINT_YN IN ( 'NN', 'NY' )");
		sTmpWhere += _T(" ORDER BY CL.LOAN_DATE");
	}
	else
	{
		sTmpWhere += _T(" CL.STATUS = '1'");
		sTmpWhere += _T(" AND CL.RECEIPT_PRINT_YN IN ( 'NN', 'YN' )");
		sTmpWhere += _T(" ORDER BY CL.RETURN_DATE");
	}

	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sTmpWhere;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::doPrintReceipt()
{
	BOOL bIsLoan = TRUE;

	INT ids = 0;
	INT nSelectCnt = 0;
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sReceiptStr = _T("");

	bIsLoan = ((CButton*)GetDlgItem( IDC_radLOAN ))->GetCheck( );

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	ids = getReceiptString( sReceiptStr );
	if ( ids != 0 ) return ids;

	ids = setRctTempVariable( pRctDM, pRctGrid, sReceiptStr, bIsLoan );
	if ( ids != 0 ) return ids;
	
	ids = setRctTempVariable( pRctDM, sReceiptStr );
	if ( ids != 0 ) return ids;

	ids = setRctTempVariable( sReceiptStr, bIsLoan );
	if ( ids != 0 ) return ids;
	
	ids = setRctTempVariable( sReceiptStr );
	if ( ids != 0 ) return ids;
	
	CMakeSearchData MakeSearchData(m_pParentMgr);
	INT iCh = MakeSearchData.ChangeHanjaToHangul( sReceiptStr );
		
	ids = printReceipt( sReceiptStr );
	if ( ids < 0 ) return -5;
	else if ( ids > 0 ) return ids; 

	ids = setDataToDB( pRctDM, pRctGrid, sReceiptStr, bIsLoan );
	if ( ids < 0 ) return -6;

	ids = setDMAndGrid( pRctDM, pRctGrid );
	if ( ids < 0 ) return -7;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::getReceiptString(CString &sRctStr)
{
	const CString FILE_PATH = _T("..\\cfg\\LoanReturn\\ReceiptFormat.txt");

	INT ids = 0;
	CString sLine = _T("");
	CStdioFile fFile;

	sRctStr = _T("");

	if ( !fFile.Open( FILE_PATH, CFile::modeRead | CFile::typeBinary ) )
	{
		AfxMessageBox( _T("영수증 형식 파일이 존재하지 않습니다.\n(주)ECO에 문의해 주세요\n(Tel)02-3443-8369-332") );
		return 1;
	}

		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	while ( TRUE )
	{
		if ( !fFile.ReadString( sLine ) )
			break;

		sLine.TrimLeft( );
		sLine.TrimRight( );

		// Header를 건너뛴다.
		if ( (sLine.Left( 1 ) == _T(".")) || (sLine.IsEmpty( )) ) continue ;

		sRctStr += sLine;
	}
	
	if ( sRctStr.IsEmpty( ) )
	{
		AfxMessageBox( _T("형식이 정의되어 있지 않습니다.\n(주)ECO에 문의해 주세요\n(Tel)02-3443-8369-332") );
		return 2;
	}

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setRctTempVariable(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sRctStr, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	INT nTotalCnt = 0;

	CString sLoanDate = _T("");
	CString sReturnDate = _T("");
	CString sReturnPlanDate = _T("");
	CString sTmpData = _T("");
	CString sBookInfo = _T("");
	CString sTotalCnt = _T("");

	CString sTmpBoInfo = _T("");

	ids = pRctGrid->SelectMakeList( );
	if ( ids < 0 ) return -1;
	
	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt < 0 ) return -2;
	else if ( 0 == nSelectCnt ) 
	{
		AfxMessageBox( _T("선정된 데이터가 없습니다.") );
		return 1;
	}

	nIdx = pRctGrid->SelectGetHeadPosition( );
	if ( nIdx < 0 ) return -3;

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if ( bIsLoan )
		{
			ids = pRctDM->GetCellData( _T("대출일"), nIdx, sTmpData );
			if ( ids < 0 ) return -4;
			if ( !sLoanDate.IsEmpty( ) )
			{
				if ( 0 != sLoanDate.Compare( sTmpData ) )
				{
					AfxMessageBox( _T("대출일이 같은 자료에 대해서만 출력이 가능합니다.") );
					return 2;
				}
			}
			else
				sLoanDate = sTmpData;
		}
		else
		{
			ids = pRctDM->GetCellData( _T("반납일"), nIdx, sTmpData );
			if ( ids < 0 ) return -5;
			if ( !sReturnDate.IsEmpty( ) )
			{
				if ( 0 != sReturnDate.Compare( sTmpData ) )
				{
					AfxMessageBox( _T("반납일이 같은 자료에 대해서만 출력이 가능합니다.") );
					return 2;
				}
			}
			else
				sReturnDate = sTmpData;
		}

		sTmpBoInfo.Format( _T("%4d"), ++nTotalCnt );
		sBookInfo += _T("<!FN>") + sTmpBoInfo;

		ids = pRctDM->GetCellData( _T("청구기호"), nIdx, sTmpData );
		if ( ids < 0 ) return -6;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 24, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		ids = pRctDM->GetCellData( _T("등록번호"), nIdx, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP><!FN>    ");

		ids = pRctDM->GetCellData( _T("서명"), nIdx, sTmpData );
		if ( ids < 0 ) return -8;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 18, FALSE );

		ids = pRctDM->GetCellData( _T("저작자"), nIdx, sTmpData );
		if ( ids < 0 ) return -9;
		sTmpData = sTmpBoInfo + _T("/") + sTmpData;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 28, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		if ( bIsLoan )
		{
			ids = pRctDM->GetCellData( _T("대출형태"), nIdx, sTmpData );
			if ( ids < 0 ) return -10;
			sTmpBoInfo.Format( _T("%s"), sTmpData.Left( 8 ) );
		}
		else
		{
			ids = pRctDM->GetCellData( _T("반납예정일"), nIdx, sTmpData );
			if ( ids < 0 ) return -10;

			if ( sReturnDate.Compare( sTmpData ) < 0 )
				sTmpBoInfo = _T("정상");
			else
				sTmpBoInfo = _T("연체");
		}
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP>");

		if ( i+1 != nSelectCnt )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -11;
		}
	}
	if ( bIsLoan )
		sRctStr.Replace( _T("%대_반일%"), sLoanDate );
	else
		sRctStr.Replace( _T("%대_반일%"), sReturnDate );

	sRctStr.Replace( _T("%서지정보%"), sBookInfo );

	sTotalCnt.Format( _T("%d"), nTotalCnt );
	sRctStr.Replace( _T("%합계%"), sTotalCnt );

	return 0;
}

CString CBO_LOC_3100_PrintReceipt::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("...");

	TCHAR chComp = '\0' ;
	TCHAR chExtra = '\0' ;

	TCHAR szTmpWord[3] ; 
	TCHAR szReturnWord[100] = _T("") ; 

	nTokenLen = _tcsclen ( szToken ) ;

	if ( nTokenLen > nCutCnt )
	{
		bIsCut = TRUE;
		nCutCnt -= 3;
	}

	for ( INT i = 0 ; i < nCutCnt ; i++ )
	{
		if ( i > (nTokenLen-1) )
		{
			if ( bIsAddSpace )
			{
				chComp = ' ';
				_stprintf ( szTmpWord, _T("%c"), chComp );
				_tcscat ( szReturnWord, szTmpWord );
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szToken[i];
			chExtra = szToken[i + 1];
		}

		if (chComp & 0x80)
		{
			if ( (i + 2) > nCutCnt )
			{
				sPeriods = _T("... ");
				break;
			}

			_stprintf ( szTmpWord, _T("%c%c"), chComp, chExtra ) ;			
			_tcscat ( szReturnWord, szTmpWord ) ;
			i++;
		}
		else
		{
			_stprintf ( szTmpWord, _T("%c"), chComp ) ;
			_tcscat ( szReturnWord, szTmpWord ) ;
		}
	}

	if ( bIsCut )
	{
		_stprintf( szTmpWord, _T("%s"), sPeriods );
		_tcscat( szReturnWord, szTmpWord );
	}

	sReturnWord.Format( _T("%s"), szReturnWord );

	return sReturnWord;
}

INT CBO_LOC_3100_PrintReceipt::setRctTempVariable(CESL_DataMgr *pRctDM, CString &sRctStr)
{
	INT ids = 0;

	CString sQuery = _T("");
	CString sTmpData = _T("");
	CString sRctNo = _T("");
	CString sLibName = _T("");

	sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
	ids = pRctDM->GetOneValueQuery( sQuery, sTmpData );
	if ( ids < 0 ) return -1;
	sRctNo.Format( _T("%010d"), (_ttoi( sTmpData )+1) );
	sRctStr.Replace( _T("%일련번호%"), sRctNo );

	sQuery = _T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL AND MANAGE_CODE = UDF_MANAGE_CODE");
	ids = pRctDM->GetOneValueQuery( sQuery, sLibName );
	if ( ids < 0 ) return -2;
	sRctStr.Replace( _T("%도서관명%"), sLibName );

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setRctTempVariable(CString &sRctStr, BOOL bIsLoan)
{
	INT ids = 0;
	CString sTitle = _T("");
	CString sDateTitle = _T("");
	CString sPrintDate = _T("");

	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	sPrintDate = strGetData.Left(16);

	if ( bIsLoan )
	{
		sTitle = _T("대출");
		sDateTitle = _T("대 출 일");
	}
	else
	{
		sTitle = _T("반납");
		sDateTitle = _T("반 납 일");
	}
	sRctStr.Replace( _T("%대_반제목%"), sTitle );
	sRctStr.Replace( _T("%대_반일제목%"), sDateTitle );

	sRctStr.Replace( _T("%출력일시%"), sPrintDate );

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setRctTempVariable(CString &sRctStr)
{
	INT ids = 0;
	CString sUserNo = _T("");
	CString sUserName = _T("");
	CString sDepartmentName = _T("");

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), sUserNo, 0 );
	if ( ids < 0 ) return -2;
	sRctStr.Replace( _T("%대출자번호%"), sUserNo );

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자이름"), sUserName, 0 );
	if ( ids < 0 ) return -3;
	sRctStr.Replace( _T("%대출자이름%"), sUserName );

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자소속"), sDepartmentName, 0 );
	if ( ids < 0 ) return -4;
	sRctStr.Replace( _T("%부서명%"), sDepartmentName );

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::printReceipt(CString sRctStr)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	UCHAR szStr[2048];
	
	ids = LP_StartUp();
	if( 0 != ids )
	{
		LP_GetErrorMsg( ErrMsg );
		CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
		AfxMessageBox(_T("발견된 출력기가 없습니다."));
		return 200;
	}
	else
		CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );


	for( INT i = 0 ; i < sRctStr.GetLength( ) ; i++ )
		szStr[i] = ( UCHAR ) sRctStr.GetAt( i );

	szStr[i]= '\0';

	ids = LP_PrintData( szStr );	

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setDataToDB(CESL_DataMgr *pRctDM, CESL_Grid* pRctGrid, CString sReceiptStr, BOOL bIsLoan)
{
	INT ids = 0;
	CString sLastNumber = _T("");
	CString sLoanKeyList = _T("");

	ids = pRctDM->StartFrame( );
	if ( ids < 0 )	return -1;

	ids = setLastNumber( pRctDM, sLastNumber );
	if ( ids < 0 ) return -2;	
	
	ids = setReceiptPrintYN( pRctDM, pRctGrid, sLoanKeyList, bIsLoan );
	if ( ids < 0 ) return -3;
	
	ids = setReceiptTable( pRctDM, sReceiptStr, sLastNumber, sLoanKeyList );
	if ( ids < 0 ) return -4;
	
	ids = pRctDM->SendFrame ( ) ;
	if ( ids < 0 )	return -5;

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setLastNumber(CESL_DataMgr *pRctDM, CString &sLastNo)
{
	INT ids = 0;
	CString sWhere = _T("");
	CString sRecKey = _T("");

	pRctDM->InitDBFieldData( );

	sWhere.Format( _T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '2004' AND MANAGE_CODE = UDF_MANAGE_CODE") );
	ids = pRctDM->GetOneValueQuery( sWhere, sRecKey );
	if ( ids < 0 )	return -1;

	sWhere.Format( _T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '2004' AND MANAGE_CODE = UDF_MANAGE_CODE") );
	ids = pRctDM->GetOneValueQuery( sWhere, sLastNo );
	if ( ids < 0 )	return -2;

	ids = pRctDM->AddDBFieldData ( _T("LAST_NUMBER"), _T("NUMERIC"), sLastNo );
	if ( ids < 0 )	return -3;

	ids = pRctDM->MakeUpdateFrame( _T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -4;	

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setReceiptPrintYN(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sLoanKeyList, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	CString sResultYN = _T("");
	CString sRecKey = _T("");

	sLoanKeyList.Empty( );
	pRctDM->InitDBFieldData( );

	ids = pRctGrid->SelectMakeList( );
	if ( ids < 0 ) return -1;
	
	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt < 0 ) return -2;
	else if ( 0 == nSelectCnt ) 
	{
		AfxMessageBox( _T("선정된 데이터가 없습니다.") );
		return 1;
	}

	nIdx = pRctGrid->SelectGetHeadPosition( );
	if ( nIdx < 0 ) return -3;

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		ids = pRctDM->GetCellData( _T("출력유무"), nIdx, sResultYN );
		if ( ids < 0 ) return -4;
		ids = pRctDM->GetCellData( _T("REC_KEY"), nIdx, sRecKey );
		if ( ids < 0 ) return -5;

		if ( sLoanKeyList.IsEmpty( ) )	sLoanKeyList = sRecKey;
		else							sLoanKeyList += _T(", ") + sRecKey;

		if ( bIsLoan )	sResultYN.SetAt( 0, 'Y' );
		else			sResultYN.SetAt( 1, 'Y' );

		ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_YN"), _T("STRING"), sResultYN );
		if ( ids < 0 )	return -6;
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 )	return -7;

		if ( i+1 != nSelectCnt )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -8;
		}
	}

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setReceiptTable(CESL_DataMgr *pRctDM, CString sReceiptStr, CString sLastNumber, CString sLoanKeyList)
{
	INT ids = 0;
	CString sRecKey = _T("");

	pRctDM->InitDBFieldData( );

	ids = pRctDM->MakeRecKey( sRecKey );
	if ( ids < 0 ) return -1;

	ids = pRctDM->AddDBFieldData( _T("LOAN_KEY_LIST"), _T("STRING"), sLoanKeyList );
	if ( ids < 0 ) return -2;

	ids = pRctDM->AddDBFieldData( _T("RECEIPT_STRING"), _T("STRING"), sReceiptStr );
	if ( ids < 0 ) return -3;

	ids = pRctDM->AddDBFieldData( _T("RCT_SEQ_NO"), _T("NUMERIC"), sLastNumber );
	if ( ids < 0 ) return -4;

	ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("1") );
	if ( ids < 0 ) return -5;
#ifdef __K2UP__
	ids = pRctDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("영수증출력"), __WFILE__, __LINE__ ) );
	if ( ids < 0 ) return -6;
#endif
	ids = pRctDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -7;

	ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	if ( ids < 0 ) return -7;

	ids = pRctDM->MakeInsertFrame( _T("CO_LOAN_RECEIPT_TBL") );
	if ( ids < 0 ) return -8;

	return 0;
}

INT CBO_LOC_3100_PrintReceipt::setDMAndGrid(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid)
{
	INT ids = 0;
	INT nSelectCnt = 0;
	INT nRowIndex = 0 ;
	INT nTotalCnt = 0 ;

	ids = pRctGrid->SelectMakeList( );
	if ( ids < 0 ) return -1;
	
	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt < 0 ) return -2;

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -1;

	ids = pRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	for ( INT i = 0 ; i < nSelectCnt ; i++ ) 
	{
		nRowIndex = pRctGrid->SelectGetIdx( );
		if ( nRowIndex < 0 ) return -3;

		ids = pRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;

		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nSelectCnt ) 
			{
				pRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pRctGrid->RemoveItem( nRowIndex + 1 );
		}

		if ( ( i + 1 ) == nSelectCnt )	break;

		ids = pRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -6;
	else if ( 0 != nTotalCnt )
		renameNoInGrid( pRctGrid, nTotalCnt );

	return 0;
}

VOID CBO_LOC_3100_PrintReceipt::renameNoInGrid(CESL_Grid *pRctGrid, INT nTotalCnt)
{
	CString sNo = _T("") ;

	for ( INT i = 0 ; i < nTotalCnt ; i++ ) 
	{
		sNo.Format ( _T("%d"), ( i + 1 ) ) ;

		pRctGrid->SetTextMatrix ( (i + 1 ), 0, sNo ) ;
	}
}
HBRUSH CBO_LOC_3100_PrintReceipt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}