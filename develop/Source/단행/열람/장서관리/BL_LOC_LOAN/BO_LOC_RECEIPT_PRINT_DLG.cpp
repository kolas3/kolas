// BO_LOC_RECEIPT_PRINT_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_RECEIPT_PRINT_DLG.h"
#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "lprn_api.h"
#include "RECEIPT_PRINT_SET_DLG.h"
#include "RECEIPT_SERARCH_DLG.h"
#include "BO_LOC_3000Api.h"
// 2006.05.26 ADD BY PDW(Jr)
// XML 형식으로 변형 작업
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_RECEIPT_PRINT_DLG dialog


CBO_LOC_RECEIPT_PRINT_DLG::CBO_LOC_RECEIPT_PRINT_DLG(INT nDialogType, CESL_Mgr* pParent, CLonaManageValue *pLMV/*=NULL*/)
	: CESL_Mgr(CBO_LOC_RECEIPT_PRINT_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_RECEIPT_PRINT_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_nDialogType = nDialogType;	
	m_pLMV = pLMV;
	m_PreviewDlg = NULL;
	m_strLibCode = _T("");
	if(NULL != m_pLMV)
	{
		// 2008.04.14 UPDATE BY PWR : 공지사항 추가 m_strNotify
		m_pLMV->GetReceiptManageValue(m_strBarcode, m_strBarcodeAll, m_strItem, m_strMode, m_strDevice, m_strPreview, m_strNotify);	
		m_pLMV->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strCivilNoViewYN);
		m_pLMV->GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	}
	else
	{
		GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strCivilNoViewYN);
		GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	}
	if(_T("Y") != m_strCivilNoViewYN) m_strCivilNoViewYN = _T("N");
	if(_T("Y") != m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	// 출력, 출력안함 구분값 추가
	m_strPrintYN = _T("Y");
}

CBO_LOC_RECEIPT_PRINT_DLG::~CBO_LOC_RECEIPT_PRINT_DLG()
{
	if ( m_PreviewDlg != NULL ) delete m_PreviewDlg;


}


VOID CBO_LOC_RECEIPT_PRINT_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_RECEIPT_PRINT_DLG)
	DDX_Control(pDX, IDC_TAB_LOAN1, m_tab);
	DDX_Control(pDX, IDC_TAB_LOAN2, m_tab2);
	//}}AFX_DATA_MAP
}


BOOL CBO_LOC_RECEIPT_PRINT_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	m_bPrintYN = TRUE;
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BEGIN_MESSAGE_MAP(CBO_LOC_RECEIPT_PRINT_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_RECEIPT_PRINT_DLG)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radLOAN, OnRadSelChange)
	ON_BN_CLICKED(IDC_radRETURN, OnRadSelChange)
	ON_BN_CLICKED(IDC_btnPREVIEW, OnbtnPREVIEW)
	ON_BN_CLICKED(IDC_btnSETUP, OnbtnSETUP)	
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnDATE_SEARCH, OnbtnDATE_SEARCH)
	ON_BN_CLICKED(IDC_btnPRINT_CANCEL, OnbtnPRINTCANCEL)			
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAN1, OnSelchangeTab1 )
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAN2, OnSelchangeTab2 )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_RECEIPT_PRINT_DLG message handlers

BOOL CBO_LOC_RECEIPT_PRINT_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 2007.05.08 ADD BY PDW
	// 대출자료 예약정보 출력
	if( 3 == m_nDialogType )
	{
		return TRUE;
	}
	// 다이얼로그를 표시하지 않는 모드
	if(-1 == m_nDialogType)
	{
		return TRUE;
	}
	
	INT ids = 0;
	CString sErrMsg = _T("");

	if(InitESL_Mgr(_T("BO_LOC_3100_RECEIPT")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowText( _T("영수증 출력") );

	(( CButton*) GetDlgItem (IDC_radLOAN))->SetCheck(1);

	//++2008.04.14 ADD BY PWR {{++
	// 날짜컨트롤 일주일검색으로 셋팅한다.
	SetDateControl();
	//--2008.04.14 ADD BY PWR --}}
		
	ids = initRctUseType( );
	if ( ids ) return FALSE;

	OnRadSelChange();

	// 17/04/20 박대우 : 중랑구립정보도서관 긴급조치 - 중복제거
/*//REM--------------------------------------------------------------------------
	ids = initData( );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initData( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
		return FALSE;
	}			
*///END--------------------------------------------------------------------------
	
	//++2006.05.26 ADD BY PDW {{++
	// XML 작업 
	ids = InitXML_Style();
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]InitXML_Style( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
		return FALSE;
	}
	//--2006.05.26 ADD BY PDW {{--

	//JOB.2020.0030 ADD BY KYJ
	CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pRctGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		pRctTotalGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pRctGrid->m_bPrivacyPrintLogRecord = TRUE;
		pRctGrid->m_strPrintInfo = _T("출력된영수증리스트");
		pRctTotalGrid->m_bPrivacyPrintLogRecord = TRUE;
		pRctTotalGrid->m_strPrintInfo = _T("미출력영수증리스트");
	}
	
	if( m_pLMV->m_sLoanUserAccessMode.IsEmpty() || m_pLMV->m_sLoanUserAccessMode == _T("1") )
	{
		pRctGrid->SetColumnEncryption(_T("회원명"),TRUE);
		pRctTotalGrid->SetColumnEncryption(_T("이름"),TRUE);
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

// **메인에서 선택된 사용자에 대해서 대출확인증 출력으로 초기화를 한다.
INT CBO_LOC_RECEIPT_PRINT_DLG::initData(BOOL bAllDisplay)
{
	CString sValue = _T("");
	CString sErrMsg = _T("");
	CString sWhere = _T("");
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sReceiptLogDMAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString strLoanCondition;
	CString strLoanExtraCondition;
	CString strReturnCondition;
	CString strReturnExtraCondition;			
	CString strReturnSelect;
	
	INT		nIdx;
	INT		nIdx2;	
	INT		ids = 0 ;

	if ( GetSafeHwnd() == NULL )
	{
		return 0;
	}
	
	// 17/04/20 박대우 : 중랑구립정보도서관 긴급조치 - 중복제거
//*/ ADD ---------------------------------------------------------------------------
	if (!IsWindowVisible())
	{
		return 0;
	}
//*/ END ---------------------------------------------------------------------------
	
	nIdx2 = GetTabCtlIdx(1);
	CESL_DataMgr* pTotalDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	
	if ( !pTotalDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::initData()") );

	//++2005.12.28 UPDATE BY PDW {{++
    // 분관에 대한 검색 쿼리 수정
	// 로그인 사용자의 관리구분과 대출이력의 관리 구분이 같은것만을 보여준다.
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::initData()") );

	// 추가될 EXTRA 쿼리
	CString sExtraManageCodeQuery = _T(" AND CL.MANAGE_CODE = '") + sManageCode + _T("'");
	//++2008.04.14 UPDATE BY PWR {{++
	// 날짜 컨트롤 추가하여 날짜컨트롤의 값에 따라 검색 되도록 WHERE추가
	// "전체미출력정보"는 이함수( initData() )에서
	// "영수증출력정보", "출력된영수증이력"의 대출/반납텝은 getReceiptWhere()에서
	// "출력된영수증이력"의 RECEIPT_LOG테이블 텝 getReceiptLogWhere()에서 날짜 조건 추가
	//
	// 컨트롤의 날짜 값을 가져온다.
	GetDateControl();
	// WHERE 구성
	CString sLoanDateWhere = _T("");
	CString sReturnDateWhere = _T("");
	// 1. To, From 둘다 입력된 상태 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To만 입력된 상태
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From만 입력된 상태
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	// 수정 (추가 EXTRA 쿼리)
	strLoanCondition		= _T("BB.REC_KEY = CL.BOOK_KEY AND CLU.REC_KEY = CL.USER_KEY AND CL.RECEIPT_PRINT_LOAN_YN='N' AND LOAN_DATE IS NOT NULL") + sExtraManageCodeQuery + sLoanDateWhere;
	strLoanExtraCondition	= _T("GROUP BY CLU.NAME,CLU.USER_NO, CLU.REC_KEY,TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD')");
	strReturnCondition		= _T("BB.REC_KEY = CL.BOOK_KEY AND CLU.REC_KEY = CL.USER_KEY AND CL.RECEIPT_PRINT_RETURN_YN='N' AND RETURN_DATE IS NOT NULL") + sExtraManageCodeQuery + sReturnDateWhere;
	strReturnExtraCondition = _T("GROUP BY CLU.NAME, CLU.USER_NO, CLU.REC_KEY, TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'), TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD')");			
	strReturnSelect			= _T("UNION ALL SELECT '반납',CLU.NAME, TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD'),CLU.REC_KEY,CLU.USER_NO FROM BO_BOOK_TBL BB,LH_STORAGE_V01 CL,CO_LOAN_USER_TBL CLU ");
	//--2008.04.14 UPDATE BY PWR --}}
	//--2005.12.28 UPDATE BY PDW --}}

	if ( nIdx2 == 2 && pTotalDM->GetRowCount() == 0 )
	{		
		// 대출 / 반납 출력 옵션에 따른 select		
		if ( m_tab.GetItemCount() == 2 )
		{
			// 전체라면
			pTotalDM->CONDITION = strLoanCondition;
			pTotalDM->EXTRA_CONDITION = strLoanExtraCondition + strReturnSelect + _T(" WHERE ") + strReturnCondition + _T(" ") + strReturnExtraCondition;
			pTotalDM->SetDataMgrRef(0,_T("대출상태"), _T("DECODE('반납','반납','대출')"), _T("str"),_T(""),_T(""));
			
			ids = pTotalDM->RefreshDataManager(_T(""));
		}
		else
		{
			CString strFieldName, strFieldType;					
			if ( GetTabCtlIdx(0) == 0 )
			{				
				// 대출만이면	
				// condition default는 대출이므로 생략.
				pTotalDM->CONDITION = strLoanCondition;
				pTotalDM->EXTRA_CONDITION = strLoanExtraCondition;
				pTotalDM->SetDataMgrRef(0,_T("대출상태"), _T("'대출'"), _T("str"),_T(""),_T(""));				
			}
			else
			{
				// 반납만이면
				pTotalDM->CONDITION = strReturnCondition;
				pTotalDM->EXTRA_CONDITION = strReturnExtraCondition;
			  	pTotalDM->SetDataMgrRef(0,_T("대출상태"), _T("'반납'"), _T("str"),_T(""),_T(""));
			}
			ids = pTotalDM->RefreshDataManager(_T(""));			
			if ( ids < 0 ) return ids;
		}
	}
	else if ( nIdx2 != 2 )
	{
		ids = pTotalDM->FreeData();
		if ( ids < 0 ) return ids;
	}	

	nIdx = GetTabCtlIdx(0);
	
	ids = getReceiptWhere( sWhere, nIdx );
	if ( ids > 0 )
	{
		// 화면 초기화
		ClearData();	
	}
	if ( ids != 0 ) return (ids);	

	if ( !sWhere.IsEmpty() )
	{
		ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
		if ( ids < 0 ) return -2;	
	}

	// 이력일 경우 영수증 번호 중심으로 보여준다.
	// BOOL bCheck;
	// bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();	
	if ( nIdx2 == 0 )
	{
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;
		pRctLogDM->FreeData();
	}
	else if ( nIdx2 == 1 )
	{	
		ids = getReceiptLogWhere( sWhere, nIdx );		
		if( ids ) return ids;
		ids = RefreshDataMgr( sReceiptLogDMAlias, sWhere );
		if( ids ) return ids;

		// 2011.08.31 ADD : 3000Api
		CString strEnc;
		CStringArray asDes1;

		// 2008.04.14 ADD BY PWR : 대출자번호 주민번호일때 *표시
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;
		for( INT i = 0; i < pRctLogDM->GetRowCount(); i++ )
		{
			if(_T("Y") == m_strCivilNoViewYN && FALSE == m_pInfo->m_bLibSecuUse) break;
			if(_T("Y") != m_strCivilNoViewYN)
			{
				CString sData;
				pRctLogDM->GetCellData( _T("회원번호"), i, sData );
				ChangeSecuriyCivilNo( sData );
				pRctLogDM->SetCellData( _T("회원번호"), sData, i );
			}
			// 2011.08.31 ADD : 3000Api
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				pRctLogDM->GetCellData(_T("주민번호"), i, strEnc);
				asDes1.Add(strEnc);
			}
		}

		// 2011.08.31 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					for(i = 0; i < pRctLogDM->GetRowCount(); i++)
					{
						pRctLogDM->SetCellData(_T("주민번호"), asDes1.GetAt(i), i);
					}
					pApi->CloseSocket();	
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
		}
	}
	
	ids = initControl(bAllDisplay);
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("[%d]initControl( ) Fail"), ids );
		AfxMessageBox( sErrMsg );
	}

	if ( 0 !=ids ) OnCancel( );

	
	INT		nRowCount;
	INT		i;

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -100;

	CESL_ControlMgr *pCM = FindCM( sReceiptCMAlias );
	if( pCM == NULL ) return -110;
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( _T("gridReceipt") );	
	if( pGrid == NULL ) return -120;
	
	//if ( nIdx == 0 && nIdx2 != 2)
	if ( nIdx2 == 0 )
	{
		nRowCount = pRctDM->GetRowCount( );
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pGrid->SetTextMatrix( i+1, 1, _T("V") );

		}
	}
	else if ( nIdx2 == 1 )
	{
		ClickLog(0);
	}
	
	return 0;
}

// **화면에 대출자 정보와 영수증 출력 자료를 보여주도록 초기화한다.
INT CBO_LOC_RECEIPT_PRINT_DLG::initControl(BOOL bAllDisplay)
{
	INT ids = 0;

	CString strLoanType;	
	CString sUserName = _T("");
	CString sUserNo = _T("");
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	INT		nUserIndex;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자이름"), sUserName, 0 );
	if ( ids < 0 )
		return -1;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), sUserNo, 0 );
	if ( ids < 0 ) return -2;
	//++2008.04.14 ADD BY PWR {{++
	// 대출자번호 주민번호일때 *표시 
	ChangeSecuriyCivilNo( sUserNo );
	//--2008.04.14 ADD BY PWR --}}
	
	GetDlgItem( IDC_edtUSER_NO )->SetWindowText( sUserNo );
	GetDlgItem( IDC_edtUSER_NAME )->SetWindowText( sUserName );		

	if ( bAllDisplay == TRUE )
	{
		ids = AllControlDisplay( sCMAlias );
		if ( ids < 0 ) return -3;
	}
	else
	{
		CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		
		nUserIndex = pGrid->GetRowSel()-1;

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출상태"), strLoanType,  nUserIndex);
		if ( ids < 0 ) { return -1; }		

		CESL_ControlMgr *pCM = FindCM( sCMAlias );
		if( pCM == NULL ) return -110;
		ids = pCM->ControlDisplay(_T("gridReceipt"), 0 );
		if ( ids < 0 ) return ids;				
	}

	if ( m_tab2.GetCurSel() == 2 )
	{
		if ( strLoanType == _T("대출") ) 
		{ 
			m_tab.SetCurSel(0);
		}
		else
		{	
			m_tab.SetCurSel(1);
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::initRctUseType()
{
	CString sRctUseType = _T("");

	sRctUseType = m_pLMV->m_sUseReceiptType;

	if ( _T("YY") != sRctUseType )
	{
		if ( 'Y' == sRctUseType.GetAt(0) )	m_tab.InsertItem( 0, _T(" 대출 ") );
		else m_tab.InsertItem( 0, _T(" 반납 ") );
	}
	else
	{
		m_tab.InsertItem( 0, _T(" 대출 ") );
		m_tab.InsertItem( 1, _T(" 반납 ") );
	}

	// 대출/반납이면
	if ( m_nDialogType == 0 )
	{
		m_tab2.InsertItem( 0, _T(" 영수증출력정보 ") );
		m_tab2.InsertItem( 1, _T(" 출력된영수증이력 ") );
		m_tab2.InsertItem( 3, _T(" 전체미출력정보 ") );
	}
	// 대출자료관리이면 
	// 이력만 존재한다.
	else
	{		
		m_tab2.InsertItem( 0, _T(" 출력된영수증이력 ") );	
	}	

	return 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

// **크기 조정함수
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect gRect;
	CRect gRect2;
	CRect gRect3;
	CRect gRect4;
	INT	  nIdx;
	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT);
	CMSHFlexGrid* pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT2);
	
	if(pGrid->GetSafeHwnd() == NULL){
		return;
	}

	pGrid->GetWindowRect( &gRect );
	m_tab.GetWindowRect( &gRect2 );

	pGrid2->GetWindowRect( &gRect3 );
	m_tab2.GetWindowRect( &gRect4 );

	ScreenToClient( gRect );
	ScreenToClient( gRect2 );

	ScreenToClient( gRect3 );
	ScreenToClient( gRect4 );

	nIdx = GetTabCtlIdx(1);

	if ( nIdx == 0 )
	{
		pGrid->MoveWindow(gRect.left, gRect.top, (cx-12), (cy - gRect.top)-11 );
		m_tab.MoveWindow(gRect2.left, gRect2.top, (cx-9), (cy - gRect2.top)-4 );		
		m_tab2.MoveWindow(gRect4.left, gRect4.top, (cx-3), (cy - gRect4.top) );
	}
	else
	{
		pGrid->MoveWindow(gRect.left, gRect.top, (cx-6), (cy - gRect.top)-5 );
		m_tab.MoveWindow(gRect2.left, gRect2.top, (cx-3), (cy - gRect2.top)-2 );
		
		pGrid2->MoveWindow(gRect3.left, gRect3.top, (cx-6), gRect3.bottom-gRect3.top );
		if ( (cx) >= 415 )
		{		
			m_tab2.MoveWindow(gRect4.left, gRect4.top, (cx-3), gRect3.bottom-gRect3.top+25 );
		}
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnRadSelChange() 
{	
	INT	nIdx;			

	nIdx = GetTabCtlIdx(1);

	if ( nIdx == 0 )
	{
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(FALSE);
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(TRUE);
		m_tab.EnableWindow(TRUE);
	}
	else if ( nIdx == 1 )
	{		
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(TRUE);
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(FALSE);
		m_tab.EnableWindow(TRUE);
	}
	else
	{
		((CButton*) GetDlgItem(IDC_btnPRINT_CANCEL))->EnableWindow(TRUE);
		((CButton*) GetDlgItem(IDC_btnSEARCH))->EnableWindow(FALSE);
		m_tab.EnableWindow(FALSE);
		//((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		//((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}
	/*	
	BOOL bCheck;
	bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();
	*/
	//if ( bCheck == TRUE )

	CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECEIPT);

	CRect rect,rect1,rect2;	
	GetWindowRect(rect);
	m_tab.GetWindowRect(rect1);
	pGrid->GetWindowRect(rect2);	
	ScreenToClient(&rect1);
	ScreenToClient(&rect2);

	DWORD CheckLogGrid;
	DWORD chkTotalGrid;
	CheckLogGrid = ( pRctGrid->GetStyle() & WS_VISIBLE );
	chkTotalGrid = ( pRctTotalGrid->GetStyle() & WS_VISIBLE );

	if ( nIdx == 0 )
	{		
		pRctGrid->ShowWindow(SW_HIDE);
		pRctTotalGrid->ShowWindow(SW_HIDE);	
		m_tab.ShowWindow(SW_SHOW);
		
		//( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_SHOW);
		//( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_SHOW);
		//( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_SHOW);
		//( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_SHOW);
		//( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_SHOW);
		
	}
	else if ( nIdx == 1 )
	{
		m_tab.ShowWindow(SW_SHOW);
		pRctGrid->ShowWindow(SW_SHOW);
		pRctTotalGrid->ShowWindow(SW_HIDE);
		m_tab.ShowWindow(SW_SHOW);			

		( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
	}
	else 
	{
		pRctTotalGrid->ShowWindow(SW_SHOW);		
		pRctGrid->ShowWindow(SW_HIDE);	
		m_tab.ShowWindow(SW_SHOW);

		( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
		( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
	}
	
	
	if ( nIdx == 0  )
	{
		// 대출반납 확인창이 
		if ( CheckLogGrid || chkTotalGrid )
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()-123, TRUE);
			m_tab.MoveWindow(rect1.left+3, rect1.top-145, rect1.Width()-6, rect1.Height()+18, TRUE);
			pGrid->MoveWindow(rect2.left+3, rect2.top-145, rect2.Width()-6, rect2.Height()+18, TRUE);
			( (CStatic*) GetDlgItem (IDC_STATIC1))->ShowWindow(SW_HIDE);
			( (CStatic*) GetDlgItem (IDC_STATIC2))->ShowWindow(SW_HIDE);
			( (CStatic*) GetDlgItem (IDC_STATIC3))->ShowWindow(SW_HIDE);
			( (CEdit*) GetDlgItem (IDC_edtUSER_NO))->ShowWindow(SW_HIDE);
			( (CEdit*) GetDlgItem (IDC_edtUSER_NAME))->ShowWindow(SW_HIDE);
		}
	}
	else if ( nIdx == 1 )
	{		
		if ( !CheckLogGrid  && !chkTotalGrid)
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+123, TRUE);
			m_tab.MoveWindow(rect1.left-3, rect1.top+145, rect1.Width()+6, rect1.Height()-18, TRUE);
			pGrid->MoveWindow(rect2.left-3, rect2.top+145, rect2.Width()+6, rect2.Height()-18, TRUE);
		}
	}		
	else
	{
		if ( !CheckLogGrid  && !chkTotalGrid)
		{
			MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+123, TRUE);
			m_tab.MoveWindow(rect1.left-3, rect1.top+145, rect1.Width()+6, rect1.Height()-18, TRUE);
			pGrid->MoveWindow(rect2.left-3, rect2.top+145, rect2.Width()+6, rect2.Height()-18, TRUE);
		}
	}
	
	if ( nIdx == 1 || nIdx == 2 )
	{
		pGrid->SetColWidth(1,0,0);		
	}
	else
	{
		pGrid->SetColWidth(1,0,500);
	}


	initData( );
}

// **영수증을 출력한다.
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPRINTRECEIPT() 
{
	INT ids = 0;
	
	//++2008.04.14 ADD BY PWR {{++
	// 출력, 출력안함 구분값 추가
	m_strPrintYN = _T("Y");
	//--2008.04.14 ADD BY PWR --}}

	if ( m_tab2.GetCurSel() == 2 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// 선정된 데이타 없으면 출력 못하도록 수정
		INT		nRowCount;
		CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("GridTotalReceipt") );
		if ( NULL == pRctGrid ) return;
		pRctGrid->SelectMakeList();	
		nRowCount = pRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("선정된 자료가 없습니다.") );
			return;
		}
		//--2008.05.13 ADD BY PWR --}}

		ids = doPrintReceiptSelectAll();
	}
	else
	{
		ids = doPrintReceipt( );
		if ( ids < 0 ) OnCancel( );
		else if ( 0 != ids ) return;
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::BarcodePrintCommand(INT nType)
{
	// 미리보기는 건별출력시에만 보여주도록 함.
	// 연속 출력시 프린터가 점유하여 화면 갱신이 바로바로 안됨	
	if ( nType == 0 )
	{	
		//tempPreviewFlag = m_strPreview;
		//m_strPreview = _T("N");
		ReceiptAutoPrint(TRUE);		
		//m_strPreview = tempPreviewFlag;
	}
	else if ( nType == 1 )
	{
		ReceiptAutoPrint(TRUE);
	}
	else 
	{
		m_tab2.SetCurSel(2);
		OnRadSelChange();
		doPrintReceiptSelectAll(TRUE);
	}

	// 미리보기 없게 출력 nType = 1;
	//OnbtnPRINTRECEIPT();
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnCLOSE() 
{
	this->DestroyWindow( );
}

// **DB로부터 가져올 데이터의 조건을 설정한다.
INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptWhere(CString &sWhere, INT f_nIndex, INT f_nRowIndex)
{
	INT ids = 0;
	INT nIdx2 = 0;

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTmpWhere = _T("");
	CString sUserKey = _T("");
	INT		nUserIndex;		

	nIdx2 = GetTabCtlIdx(1);

	CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		

	// 전체 보기는 조건이 없음.
	if ( nIdx2 == 2 )
	{		
		if (f_nRowIndex == -1 ) nUserIndex = pGrid->GetRowSel()-1;
		else nUserIndex = f_nRowIndex;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출자KEY"), sUserKey,  nUserIndex);
		if ( ids < 0 )
		{
			return -1;
		}		
	}
	else
	{
		ids = GetDataMgrData( sMemberDMAlias, _T("대출자KEY"), sUserKey, 0 );
		if ( ids < 0 )
		{
			return -1;
		}
	}
	
	if ( !sUserKey.IsEmpty( ) )
		sWhere += _T(" CL.USER_KEY = ") + sUserKey;
	else
	{		
		return 1;
	}

	//bCheck = (( CButton*) GetDlgItem (IDC_radLOAN))->GetCheck();

	//++2008.04.14 UPDATE BY PWR {{++
	// "영수증출력정보", "출력된영수증이력"의 대출/반납탭 날짜 조건 입력
	// 컨트롤의 날짜 값을 가져온다.
	GetDateControl();
	// WHERE 구성
	CString sLoanDateWhere = _T("");
	CString sReturnDateWhere = _T("");
	// 1. To, From 둘다 입력된 상태 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To만 입력된 상태
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From만 입력된 상태
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
		sReturnDateWhere.Format(_T(" AND CL.RETURN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	if ( nIdx2 == 0  )
	{ // 현황
		
		if ( f_nIndex == 0 )
		{
			// 대출
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_LOAN_YN != 'Y' or CL.RECEIPT_PRINT_LOAN_YN IS NULL )");
			sTmpWhere += sLoanDateWhere;
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{
			// 반납
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_RETURN_YN != 'Y' or CL.RECEIPT_PRINT_RETURN_YN IS NULL )");
			sTmpWhere += sReturnDateWhere;
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}
	}
	else if ( nIdx2 == 1 )// 이력
	{
		if ( f_nIndex == 0 )
		{
			// 대출
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN='Y'");
			sTmpWhere += sLoanDateWhere;
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{
			// 반납
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN='Y'");
			sTmpWhere += sReturnDateWhere;
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}
	}
	//--2008.04.14 UPDATE BY PWR --}}
	else
	{ 	
		CString strLoanDate;
		CString strReturnDate;
		CString strLoanType;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출일"), strLoanDate,  nUserIndex);
		if ( ids < 0 ) { return -1; }
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("반납일"), strReturnDate,  nUserIndex);
		if ( ids < 0 ) { return -1; }
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출상태"), strLoanType,  nUserIndex);
		if ( ids < 0 ) { return -1; }

		if ( strLoanType == _T("대출") )
		{			
			// 대출
			sTmpWhere += _T(" CL.STATUS NOT IN ( '3', '4', '5' )");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_LOAN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_LOAN_YN != 'Y' or CL.RECEIPT_PRINT_LOAN_YN IS NULL )");
			sTmpWhere += _T(" AND TO_CHAR(LOAN_DATE, 'YYYY/MM/DD')='")+strLoanDate+_T("'");
			sTmpWhere += _T(" ORDER BY CL.LOAN_DATE DESC");
		}
		else
		{		
			// 반납
			// 18.09.27 JMJ KOL.RED.2018.008
			sTmpWhere += _T(" CL.STATUS IN ('1','R')");
			//2020.04.14 JOB.2020.0033 ADD BY KYJ
			//sTmpWhere += _T(" AND CL.RECEIPT_PRINT_RETURN_YN != 'Y'");
			sTmpWhere += _T(" AND ( CL.RECEIPT_PRINT_RETURN_YN != 'Y' or CL.RECEIPT_PRINT_RETURN_YN IS NULL )");
			sTmpWhere += _T(" AND TO_CHAR(RETURN_DATE, 'YYYY/MM/DD')='")+strReturnDate+_T("'");
			sTmpWhere += _T(" ORDER BY CL.RETURN_DATE DESC");
		}		
	}

	//++2005.12.28 ADD BY PDW {{++
	// 관리구분에 대한 조건 추가	
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::getReceiptWhere()") );

// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = '") + sManageCode + _T("'");
	CString sExtraWhere;
	sExtraWhere.Format(_T(" CL.MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = UDF_MANAGE_CODE");
	
	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sExtraWhere;
	//--2005.12.28 ADD BY PDW --}}

	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sTmpWhere;

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptLogWhere(CString &sWhere, INT f_nIndex)
{
	INT ids = 0;
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTmpWhere = _T("");
	CString sUserKey = _T("");
	CString	strReceiptKey=_T("");

	sWhere=_T("");

	ids = GetDataMgrData( sMemberDMAlias, _T("대출자KEY"), sUserKey, 0 );
	if ( ids < 0 ) return -1;

	

	if ( f_nIndex == 0 )
	{
		sWhere = _T(" CL.RECEIPT_LOAN_KEY = CLR.REC_KEY AND");		
	}
	else
	{
		sWhere = _T(" CL.RECEIPT_RETURN_KEY = CLR.REC_KEY AND");		
	}
	
	//++2005.12.28 ADD BY PDW {{++
	// 관리구분에 대한 조건 추가	
	CString sManageCode;
	ids = GetLoginerManageCode( sManageCode );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::getReceiptLogWhere()") );

// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = '") + sManageCode + _T("' AND ");
	CString sExtraWhere;
	sExtraWhere.Format(_T(" CL.MANAGE_CODE = '%s' AND "), m_pInfo->MANAGE_CODE);
// 	CString sExtraWhere = _T(" CL.MANAGE_CODE = UDF_MANAGE_CODE AND ");

	sWhere += sExtraWhere;
	//--2005.12.28 ADD BY PDW --}}

	if ( !sUserKey.IsEmpty( ) )
		sWhere += _T(" CL.USER_KEY = ") + sUserKey;
	else
	{		
		return 1;
	}

	//++2008.04.14 UPDATE BY PWR {{++
	// "출력된영수증이력"의 RECEIPT_LOG테이블 탭에서 날짜 조건 입력
	// 컨트롤의 날짜 값을 가져온다.
	GetDateControl();
	// WHERE 구성
	CString sPrintDateWhere = _T("");
	// 1. To, From 둘다 입력된 상태 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateTo, m_sDateFrom );
	}
	// 2. To만 입력된 상태
	else if( !m_sDateTo.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								, m_sDateTo );
		
	}
	// 3. From만 입력된 상태
	else if( !m_sDateFrom.IsEmpty() )
	{
		sPrintDateWhere.Format( _T(" AND CLR.PRINT_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								, m_sDateFrom );
	}

	sWhere += sPrintDateWhere;
	//--2008.04.14 UPDATE BY PWR --}}

	/*	
	if ( f_nIndex == 0 )
	{
		// 대출		
		sTmpWhere += _T(" CL.RECEIPT_PRINT_LOAN_YN='Y'");		
	}
	else
	{
		// 반납		
		sTmpWhere += _T(" CL.RECEIPT_PRINT_RETURN_YN='Y'");	
	}
	*/

	/*
	if ( !sWhere.IsEmpty( ) )
		sWhere += _T(" AND");

	sWhere += sTmpWhere;
	*/

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::doPrintReceipt()
{
	if ( !m_bPrintYN )
	{
		return 0;		
	}

	CString	sReceiptStr;
	INT	ids;	
	INT	nIdx;	
	BOOL bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	//++2008.04.14 ADD BY PWR {{++
	// TotalGrid추가 
	CESL_Grid* pTotalGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalGrid ) return -3;
	//--2008.04.14 ADD BY PWR --}}

	nIdx = GetTabCtlIdx(0);
	if ( nIdx == 0 ) bIsLoan = TRUE;
	else bIsLoan = FALSE;		
	
	/**********************************************/
	// 1. 출력용 데이터 만들기
	/**********************************************/
	CString	sPrintDate;
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sPrintDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 		tTime.GetMonth( ),
// 		tTime.GetDay( ),
// 		tTime.GetHour( ),
// 		tTime.GetMinute( ) );

	ids = MakePrintData(sReceiptStr, sPrintDate);	
	if ( ids ) return 0;	
	/**********************************************/
	// Preview Option
	/**********************************************/
	if ( m_strPreview == _T("Y") )
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(sReceiptStr);
		if ( dlgPreview.DoModal() != IDOK ) return 0;
		

		/*
		if ( m_PreviewDlg == NULL )
		{
			m_PreviewDlg = new CBO_LOC_RECEIPT_PREVIEW_DLG(this);		
			m_PreviewDlg->Create(this);		
		}	
		m_PreviewDlg->m_strDevice = m_strDevice;
		m_PreviewDlg->SetPrintData(sReceiptStr);
		m_PreviewDlg->ShowWindow(TRUE);					
		*/
	}

	/**********************************************/
	// 5. Print receipt
	/**********************************************/
	// 2005.05.30 ADD BY PDJ
	// 내부적으로만 출력할지 실제 영수증으로 출력할지 결정
	
	//++2008.04.14 ADD BY PWR {{++
	// 출력안함은 출력하지 않는다.
	if( m_strPrintYN == _T("Y") )
	{
		ids = printReceipt( sReceiptStr );
		if ( ids < 0 ) return -5;
		else if ( ids > 0 ) return ids; 			
	}
	//--2008.04.14 ADD BY PWR --}}
	
	/**********************************************/
	// 6. DB에 기록
	// 마지막번호 하나 증가 -> 출력 유무 변경 -> 영수증 이력관리 테이블에 반영
	/**********************************************/
	ids = setDataToDB( pRctDM, pRctGrid, sReceiptStr, bIsLoan, sPrintDate );
	if ( ids < 0 ) return -6;

	//++2008.04.14 UPDATE BY PWR {{++
	// 전체미출력정보는 TotalGrid의 선정값을 보고 지워야 하기때문에 
	if( GetTabCtlIdx(1) == 2 )
	{
		ids = setDMAndGrid( pRctDM, pTotalGrid );	
	}
	else
	{
		ids = setDMAndGrid( pRctDM, pRctGrid );
	}
	/*
	ids = setDMAndGrid( pRctDM, pRctGrid );
	*/
	//--2008.04.14 UPDATE BY PWR --}}
	if ( ids < 0 ) return -7;

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::doPrintReceiptSelectAll(BOOL bAllFlag)
{
	CString	sReceiptStr;
	CString strLoanType;
	CString sWhere;
	INT	nLoanType;
	INT	ids;	
	INT	i;
	INT nRowCount;
	INT nRowIndex;
	BOOL bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;

	CESL_DataMgr* pTotalRctDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	if ( NULL == pTotalRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalRctGrid ) return -2;

	if ( bAllFlag == TRUE )
	{
		nRowCount = pTotalRctDM->GetRowCount();
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pTotalRctGrid->SetTextMatrix( i+1, 1, _T("V") );
		}
	}
	pTotalRctGrid->SelectMakeList();
	nRowCount = pTotalRctGrid->SelectGetCount();	
	pTotalRctGrid->SelectGetTailPosition();	
	for ( i=0 ; i<nRowCount ; i++ )
	{		
		sWhere=_T("");
		nRowIndex = pTotalRctGrid->SelectGetIdx ();		

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출상태"), strLoanType,  nRowIndex);
		if ( ids ) return ids;

		if ( strLoanType == _T("대출") ) 
		{
			bIsLoan = TRUE;
			nLoanType = 0;
			m_tab.SetCurSel(0);
		}
		else 
		{
			m_tab.SetCurSel(1);
			bIsLoan = FALSE;	
			nLoanType = 1;
		}
		
		ids = getReceiptWhere( sWhere, nLoanType,nRowIndex );
		if ( ids > 0 )
		{
			// 화면 초기화
			ClearData();	
		}
		if ( ids != 0 ) return (ids);	
		
		if ( !sWhere.IsEmpty() )
		{
			ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
			if ( ids < 0 ) return -2;	
		}
		
		/**********************************************/
		// 1. 출력용 데이터 만들기
		/**********************************************/
		CString	sPrintDate;
		// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
		CString strGetData;
		CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
		sPrintDate = strGetData.Left(16);
// 		CTime tTime = CLocCommonAPI::GetDBTime( this );
// 		sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 			tTime.GetMonth( ),
// 			tTime.GetDay( ),
// 			tTime.GetHour( ),
// 			tTime.GetMinute( ) );

		ids = MakePrintData(sReceiptStr, sPrintDate);	
		if ( ids ) return 0;

		if ( m_strPreview == _T("Y") )
		{
			CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
			dlgPreview.m_strDevice = m_strDevice;
			dlgPreview.SetPrintData(sReceiptStr);
			if ( dlgPreview.DoModal() != IDOK ) return 0;
		}
		/**********************************************/
		// 5. Print receipt
		/**********************************************/
		//++2008.04.14 ADD BY PWR {{++
		// 출력안함은 출력하지 않는다.
		if( m_strPrintYN == _T("Y") )
		{
			ids = printReceipt( sReceiptStr );
			if ( ids < 0 ) return -5;
			else if ( ids > 0 ) return ids; 		
		}
		//--2008.04.14 ADD BY PWR --}}

		/**********************************************/
		// 6. DB에 기록
		// 마지막번호 하나 증가 -> 출력 유무 변경 -> 영수증 이력관리 테이블에 반영
		/**********************************************/
		ids = setDataToDB( pRctDM, pRctGrid, sReceiptStr, bIsLoan, sPrintDate );
		if ( ids < 0 ) return -6;	
		
		/// DM의 데이터를 지우고
		ids = pTotalRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;

		/// 마지막 데이터에서 또 이전데이터를 호출하면 에러가 발생한다.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pTotalRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	if ( bAllFlag == TRUE )
	{
		pTotalRctDM->FreeData();
	}

	initData();
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::getReceiptString(CString &sRctStr)
{
	//++2006.05.26 UPDATE BY PDW {{++
	// XML에서 가져오기로 변경
	InitXML_Style();
	MakeStringStyle( sRctStr );
	/*
	const CString FILE_PATH = _T("..\\cfg\\LoanReturn\\ReceiptFormat.txt");

	INT ids = 0;
	CString sLine = _T("");
	CStdioFile fFile;

	sRctStr = _T("");

	if ( !fFile.Open( FILE_PATH, CFile::modeRead ) )
	{
		AfxMessageBox( _T("영수증 형식 파일이 존재하지 않습니다.\n(주)ECO에 문의해 주세요\n(Tel)02-3443-8369-332") );
		return 1;
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
	*/	
	//--2006.05.26 UPDATE BY PDW --}}
	return 0;
}

// **DM_BO_LOC_3100_RECEIPT 으로 셋팅(_T("%대_반일%"), _T("%서지정보%"),_T("%합계%"), _T("%추가내용%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sRctStr, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	INT nTotalCnt = 0;
	INT nNonReturnBookCnt;
	INT nTab2Idx;

	CString sLoanDate = _T("");        // 대출일
	CString sReturnDate = _T("");      // 반납일
	CString sReturnPlanDate = _T("");  // 반납예정일
	CString	sLoanStopDate = _T("");    // 대출정지일
	CString sTmpData = _T("");    
	CString sBookInfo = _T("");        // 책정보
	CString sTotalCnt = _T("");        // 대출책수
	CString	strCheckDate = _T("");    
	CString	strCheckDate2 = _T("");	

	CString sTmpBoInfo = _T("");

	nTab2Idx = m_tab2.GetCurSel();

	if ( nTab2Idx == 2 )
	{
		// 전체면 선정 구분없이 모두 출력
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{
		//++2008.05.23 ADD BY PWR {{++
		// 검색 영수증도 없는데 선정된 데이터가 없다는 메시지나와서 추가
		if( pRctDM->GetRowCount() < 1 ) return 1;
		//--2008.05.23 ADD BY PWR --}}

		ids = pRctGrid->SelectMakeList( );
		if ( ids < 0 ) return -1;	
		nSelectCnt = pRctGrid->SelectGetCount( );
		if ( nSelectCnt < 0 ) return -2;
		else if ( 0 == nSelectCnt ) 
		{
			AfxMessageBox( _T("선정된 데이터가 없습니다.") );
			return 1;
		}
	}

	if ( nTab2Idx != 2 )
	{
		nIdx = pRctGrid->SelectGetHeadPosition( );
		if ( nIdx < 0 ) return -3;
	}

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 전체 출력일 경우 index 무시
		if ( nTab2Idx == 2 ) nIdx = i;

		if ( bIsLoan )
		{
			ids = pRctDM->GetCellData( _T("대출일"), nIdx, sTmpData );
			if ( ids < 0 ) return -4;			
			if ( !sLoanDate.IsEmpty( ) )
			{	
				strCheckDate = sLoanDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( 0 != strCheckDate.Compare( strCheckDate2 ) )
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
				strCheckDate = sReturnDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( 0 != strCheckDate.Compare( strCheckDate2 ) )
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

		// 간단서지정보(서명/저작자)
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
			ids = pRctDM->GetCellData( _T("반납예정일"), nIdx, sReturnPlanDate );
			if ( ids < 0 ) return -10;			
		}
		else
		{
			ids = pRctDM->GetCellData( _T("반납예정일"), nIdx, sTmpData );
			if ( ids < 0 ) return -10;						

			// 반납예정일과 반납일을 비교한다
			//++2008.05.21 UPDATE BY PWR {{--
			if ( !sReturnDate.IsEmpty( ) )
			{
				strCheckDate = sReturnDate.Left(10);
				strCheckDate2 = sTmpData.Left(10);
				if ( strCheckDate.Compare( strCheckDate2 ) <= 0 )
					sTmpBoInfo = _T( "정상" );
				else
					sTmpBoInfo = _T( "연체" );
			}
			else
			{
				sTmpBoInfo = _T( " " );
			}
			//--2008.05.21 UPDATE BY PWR --}}

			// 미리보기일 경우 대출정지일을 아직 알 수 없음.
			if ( nTab2Idx != 2 )
			{
				ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출정지일"), sLoanStopDate, 0 );
			}
		}
		
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP>");
		
		// 부록일 경우 부록명 처리
		CString	strAppendixYN;
		CString	strAppendixTitle;

		ids = pRctDM->GetCellData( _T("부록여부"), nIdx, strAppendixYN );
		if ( strAppendixYN==_T("부록") )
		{
			ids = pRctDM->GetCellData( _T("부록명"), nIdx, sTmpBoInfo );
			if ( ids ) return 0;			
			// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
			strAppendixTitle.Format(_T("     위 자료의 부록임[부록명:%s]<!FP>"), sTmpBoInfo );
*/// AFTER -------------------------------------------------------------------------
			sTmpBoInfo = getLimitWord( sTmpBoInfo.GetBuffer(0), 28, FALSE );
			strAppendixTitle.Format(_T("     위 자료의 부록임<!FP>     [부록명:%s]<!FP>"), sTmpBoInfo );
//*/ END ---------------------------------------------------------------------------
			sBookInfo += strAppendixTitle;
		}
		if ( i+1 != nSelectCnt && nTab2Idx != 2)
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -11;
		}
	}
	if ( bIsLoan )
	{
		//++2006.05.26 UPDATE BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		sRctStr.Replace( _T("%대_반일%"), sLoanDate );	
		ChangePrintDomData( _T("%대_반일%"), sLoanDate );	
		
		sReturnPlanDate =  _T("<!FN>대출기한 : ") +sReturnPlanDate+_T(" 일까지<!LN><!FP><!FP>");
		
		sRctStr.Replace( _T("%반납예정일%"), sReturnPlanDate );
		ChangePrintDomData( _T("%반납예정일%"), sReturnPlanDate );	
		
		sRctStr.Replace( _T("%추가내용%"), _T("") );
		ChangePrintDomData( _T("%추가내용%"), _T("") );	
		/*
		sRctStr.Replace( _T("%대_반일%"), sLoanDate );			
		sReturnPlanDate =  _T("<!FN>대출기한 : ") +sReturnPlanDate+_T(" 일까지<!LN><!FP><!FP>");
		sRctStr.Replace( _T("%반납예정일%"), sReturnPlanDate );
		sRctStr.Replace( _T("%추가내용%"), _T("") );
		*/
		//--2006.05.26 UPDATE BY PDW --}}
	}
	else
	{
		CString	strUserKey;
		CString	strWhere;
		CString	strLoanCount;

		sRctStr.Replace( _T("%대_반일%"), sReturnDate );
		sRctStr.Replace( _T("%반납예정일%"), _T("<!FP>") );	
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%대_반일%"), sReturnDate );	
		ChangePrintDomData( _T("%반납예정일%"), _T("<!FP>") );	
		//--2006.05.26 ADD BY PDW --}}
		// 대출정지일이 있을 경우 표시하기
		if ( !sLoanStopDate.IsEmpty() )
		{
			sLoanStopDate = _T("★ 반납도서가 연체되어 ")+sLoanStopDate+_T(" 일까지 <!FP>                    대출정지 기간입니다.<!FP>");		
		}	
		
		//m_pLMV->GetManageValue(_T("기타"), _T("공통"), _T("영수증출력바코드"), _T("미반납도서상세출력"), strYN );	
		// 미반납 도서 출력		
		
		// 미반납 도서 간략 출력
		if ( nTab2Idx == 2 )
		{		
			CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");					
			INT nRowIndex;

			CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
			if ( NULL == pTotalRctGrid ) return -2;
			
			nRowIndex = pTotalRctGrid->SelectGetIdx();
			if ( nRowIndex == - 1 )
			{
				nRowIndex = pTotalRctGrid->GetRowSel()-1;
			}
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출자KEY"), strUserKey,  nRowIndex);
			if ( ids < 0 ) return ids;
		}
		else
		{
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), strUserKey, 0 );
		}

		CESL_DataMgr *pNonReturnBookDM = FindDM( _T("DM_BO_LOC_3100_RECEIPT_NON") );

		strWhere.Format(_T("USER_KEY = %s"), strUserKey);
		pNonReturnBookDM->RefreshDataManager(strWhere);

		/*
		strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE ")
			_T(" USER_KEY = %s AND STATUS IN ('0', '2')"), strUserKey );		
		pRctDM->GetOneValueQuery( strWhere, strLoanCount );
		*/
		nNonReturnBookCnt = pNonReturnBookDM->GetRowCount();
		if ( nNonReturnBookCnt > 0 )
		{
			strLoanCount.Format(_T("%d"), nNonReturnBookCnt);
			strLoanCount = _T("★ 미반납도서가 ") + strLoanCount + _T(" 권 있습니다.<!FP>");
			sLoanStopDate = sLoanStopDate+strLoanCount;
		}
		
		if ( m_strItem == _T("Y") )
		{
			// 미반납도서 상세 출력
			CString	strNonReturnBookInfo;
			DetailNonReturnBookInfo(&strNonReturnBookInfo, pNonReturnBookDM);
			sLoanStopDate += strNonReturnBookInfo;			
		}
	}		

	if ( sLoanStopDate.IsEmpty() )
	{
		sRctStr.Replace( _T("%추가내용%"), _T("") );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%추가내용%"), _T("") );	
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		sLoanStopDate += _T("<!FP>");
		sRctStr.Replace( _T("%추가내용%"), sLoanStopDate );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%추가내용%"), sLoanStopDate );	
		//--2006.05.26 ADD BY PDW --}}
	}				
	

	sRctStr.Replace( _T("%서지정보%"), sBookInfo );
	//++2006.05.26 ADD BY PDW {{++
	// DB저장용 XML의 데이터 갱신
	ChangePrintDomData( _T("%서지정보%"), sBookInfo );	
	//--2006.05.26 ADD BY PDW --}}

	sTotalCnt.Format( _T("%d"), nTotalCnt );
	sRctStr.Replace( _T("%합계%"), sTotalCnt );
	//++2006.05.26 ADD BY PDW {{++
	// DB저장용 XML의 데이터 갱신
	ChangePrintDomData( _T("%합계%"), sTotalCnt );	
	//--2006.05.26 ADD BY PDW --}}

	return 0;
}

// **한글, 한자, 영문을 체크하여 Byte수에 맞게 문자열을 자른다.
CString CBO_LOC_RECEIPT_PRINT_DLG::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	/// 한글 계산을 위하여 ansi로 변환한 후에
	/// 길이 계산을 하자
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( szToken, &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}
	if(szTmp == NULL)
	{
		CString sTemp = _T("");
		for(INT i = 0; i < nCutCnt; i++)
		{
			sTemp += _T(" ");
		}
		return sTemp;
	}

	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("…");
	CHAR szPeriods[5];
	sprintf( szPeriods, "%s", "…" );

	CHAR chComp = '\0' ;
	CHAR chExtra = '\0' ;

	CHAR szTmpWord[3] ; 
	CHAR szReturnWord[100] = "" ; 	

	nTokenLen = strlen( szTmp );

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
				sprintf ( szTmpWord, "%c", chComp );
				strcat ( szReturnWord, szTmpWord );
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szTmp[i];
			chExtra = szTmp[i + 1];
		}

		/// 한글과 한자일 때...
		if (chComp & 0x80)
		{
			if ( (i + 2) > nCutCnt )
			{				
				sprintf( szPeriods, "%s", "... " );
				break;
			}

			sprintf ( szTmpWord, "%c%c", chComp, chExtra ) ;			
			strcat ( szReturnWord, szTmpWord ) ;
			i++;
		}
		// 한글, 한자 제외일때
		else
		{
			sprintf ( szTmpWord, "%c", chComp ) ;
			strcat ( szReturnWord, szTmpWord ) ;
		}
	}

	if ( bIsCut )
	{
		sprintf( szTmpWord, "%s", szPeriods );
		strcat( szReturnWord, szTmpWord );
	}
	
	if( 0 < len )
	{
		delete [] szTmp;
	}

	/// 계산이 끝난 글자를
	/// 유니코드로 변환해야 한다
	TCHAR* pszDest = NULL;
	cv.MultiByteToUnicode( szReturnWord, &pszDest, &len );	
	sReturnWord.Format( _T("%s"), pszDest );

	if( 0 < len )
	{
		delete [] pszDest;
	}

	return sReturnWord;
}

// **GETONEVALUEQUERY로 셋팅, 공지사항->파일(ReceiptPrint.cfg)
// ( "%도서관명%",  "%일련번호%", "%공지사항%" )
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CESL_DataMgr *pRctDM, CString &sRctStr)
{
	INT ids = 0;

	CString sQuery		= _T("");
	CString sTmpData	= _T("");
	CString sRctNo		= _T("");
	CString sLibName	= _T("");

	if ( m_tab2.GetCurSel() != 1 )
	{
		// 1. 일련번호 설정
		//++2009.01.02 ADD BY KSJ {{++
		//Where 조건에 MAKE_YEAR 추가
		//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
		//CString		strCurrentYear;
		//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
		//--2009.01.02 ADD BY KSJ --}}
		//sQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND MAKE_YEAR='%s'") , strCurrentYear) ;
		sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
// 		sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
		ids = pRctDM->GetOneValueQuery( sQuery, sTmpData );
		if ( ids < 0 ) 
		{
			//++2009.01.02 UPDATE BY KSJ {{++
			//레코드가 없을 경우, 레코드 생성 후 처리한다
			if( -4007 == ids )
			{
				pRctDM->StartFrame();
				pRctDM->InitDBFieldData();
				
				ids = pRctDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
				
				//ids = pRctDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
				//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("영수증_일련번호") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				ids = pRctDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
				if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

				pRctDM->SendFrame();
				pRctDM->EndFrame();
			}
			else
			{
				return -1;
			}
			//--2009.01.02 UPDATE BY KSJ --}}
		}
		sRctNo.Format( _T("%010d"), (_ttoi( sTmpData )+1) );
		sRctStr.Replace( _T("%일련번호%"), sRctNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%일련번호%"), sRctNo );	
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		CString sReceiptLogDMAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
		INT nRowIndex;

		CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
		if ( NULL == pRctLogGrid ) return -110;		
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sReceiptLogDMAlias );
		if( pRctLogDM == NULL ) return -110;

		nRowIndex = pRctLogGrid->GetRowSel()-1;
		if ( nRowIndex >= 0 )
		{
			pRctLogDM->GetCellData(_T("RCT_SEQ_NO"), nRowIndex, sTmpData );			
			sRctNo.Format( _T("%010d"), (_ttoi( sTmpData )) );
			sRctStr.Replace( _T("%일련번호%"), sRctNo );
			//++2006.05.26 ADD BY PDW {{++
			// DB저장용 XML의 데이터 갱신
			ChangePrintDomData( _T("%일련번호%"), sRctNo );	
			//--2006.05.26 ADD BY PDW --}}
		}
		
	}
	
	// 2. 도서관 이름 설정
	if( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		pRctDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	sRctStr.Replace( _T("%도서관명%"), m_strLibCode );
	ChangePrintDomData( _T("%도서관명%"), m_strLibCode );	

	//++2008.04.14 ADD BY PWR {{++
	// 공지사항 추가
	CStdioFile	file;
	CString		strNotify;
	CString		strTemp;
	INT			idx;

	strTemp = _T("..\\cfg\\ReceiptPrint.cfg");	
	if(!file.Open (strTemp,CFile::modeRead) ) 
	{
		strNotify = _T("");
	}
	
	CString strLine;
	CString sTemp;
	while( file.ReadString( strLine ) )
	{
		strLine.TrimLeft();
		if ( strLine.Left(1) == _T("#") || strLine.IsEmpty() ) continue;		
		if ( strLine.Left(1) == _T("[") || strLine.IsEmpty() ) continue;		

		idx = strLine.Find( _T("=") );
		if ( 0 <= idx )
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if ( sTemp == _T("PRINT_NOTIFY") ) 
			{
				strNotify = strLine.Mid( idx + 1 );
				strNotify.TrimLeft();
				strNotify.TrimRight();
			}
		}
	}
	file.Close() ;

	if( !strNotify.IsEmpty() )
	{
		strNotify += _T("<!FP><!FP>");
	}
	sRctStr.Replace( _T("%공지사항%"), strNotify );
	ChangePrintDomData( _T("%공지사항%"), strNotify );
	//--2008.04.14 ADD BY PWR --}}

	return 0;
}

// **바로 셋팅 가능한 부분(_T("%대_반제목%"),_T("%대_반일제목%"), _T("%출력일시%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CString &sRctStr, BOOL bIsLoan, CString f_strPrintDate)
{
	INT ids = 0;
	CString sTitle = _T("");
	CString sDateTitle = _T("");
	CString sPrintDate = _T("");

	//CTime tTime = CLocCommonAPI::GetDBTime( this );

	// 1 대출/반납에 따라 달라지는 Alias 설정
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
	//++2006.05.26 ADD BY PDW {{++
	// DB저장용 XML의 데이터 갱신
	ChangePrintDomData( _T("%대_반제목%"), sTitle );	
	ChangePrintDomData( _T("%대_반일제목%"), sDateTitle );
	//--2006.05.26 ADD BY PDW --}}
	
	
	/*
	sPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
													tTime.GetMonth( ),
													tTime.GetDay( ),
													tTime.GetHour( ),
													tTime.GetMinute( ) );
													*/


	sRctStr.Replace( _T("%출력일시%"), f_strPrintDate );
	//++2006.05.26 ADD BY PDW {{++
	// DB저장용 XML의 데이터 갱신
	ChangePrintDomData( _T("%출력일시%"), f_strPrintDate );	
	//--2006.05.26 ADD BY PDW --}}

	return 0;
}

// **DM_BO_LOC_3100_MEMBER_INFO 으로 셋팅(_T("%대출자번호%"), _T("%대출자이름%"), _T("%부서명%"))
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTempVariable(CString &sRctStr)
{	
	CString sUserNo = _T("");
	CString sUserName = _T("");
	CString sDepartmentName = _T("");	
	INT		ids;
	INT		nRowIndex;

	if (m_tab2.GetCurSel() == 2 )
	{
		CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));		
		nRowIndex = pGrid->SelectGetIdx();
		if ( nRowIndex == -1 )
		{
			nRowIndex = pGrid->GetRowSel()-1;
		}
		
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출자번호"), sUserNo, nRowIndex );
		if ( ids < 0 ) return ids;
		//++2008.03.14 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( sUserNo );
		//for ( i=6 ; i<sUserNo.GetLength() ; i++ )
		//{
		//	sUserNo.SetAt(i, '*');		
		//}
		//--2008.03.14 ADD BY PWR --}}
		sRctStr.Replace( _T("%대출자번호%"), sUserNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData(_T("%대출자번호%"), sUserNo );
		//--2006.05.26 ADD BY PDW --}}

		// 4 대출자이름 설정
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("이름"), sUserName, nRowIndex );
		if ( ids < 0 ) return -3;
		sRctStr.Replace( _T("%대출자이름%"), sUserName );		
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData(_T("%대출자이름%"), sUserName );
		//--2006.05.26 ADD BY PDW --}}
	}
	else
	{
		// 3 대출자번호 설정
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), sUserNo, 0 );
		if ( ids < 0 ) return -2;
		//++2008.03.14 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( sUserNo );
		// 2005.05.11 ADD BY PDJ
		// 회원번호 앞에 7자리만 표시하고 뒤는 *로
		//for ( i=6 ; i<sUserNo.GetLength() ; i++ )
		//{
		//	sUserNo.SetAt(i, '*');		
		//}
		//--2008.03.14 ADD BY PWR --}}
		sRctStr.Replace( _T("%대출자번호%"), sUserNo );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%대출자번호%"), sUserNo );
		//--2006.05.26 ADD BY PDW --}}
		// 4 대출자이름 설정
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자이름"), sUserName, 0 );
		if ( ids < 0 ) return -3;
		sRctStr.Replace( _T("%대출자이름%"), sUserName );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%대출자이름%"), sUserName );
		//--2006.05.26 ADD BY PDW --}}
		// 5 대출자부서명 설정
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자소속"), sDepartmentName, 0 );
		if ( ids < 0 ) return -4;
		sRctStr.Replace( _T("%부서명%"), sDepartmentName );
		//++2006.05.26 ADD BY PDW {{++
		// DB저장용 XML의 데이터 갱신
		ChangePrintDomData( _T("%부서명%"), sDepartmentName );
		//--2006.05.26 ADD BY PDW --}}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::printReceipt(CString sRctStr, BOOL bStatusCheck)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	//UCHAR szStr[2048];
	// UCHAR szStr[2048]; 2048이상이 들어오면 에러라 다음과 같이 처리.
	UCHAR *szStr = new UCHAR[100000];
	if ( szStr == NULL ) return -100;

	//if ( m_strMode == '2' ) return 0;
	
	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
	DWORD dwWritten;
	HANDLE hPrinter;

	BOOL bIsWTPPrinter = TRUE;	
	TCHAR* szDest = NULL;			
	CHAR* szTmp2 = NULL;
	INT len2 = 0;
		
	CString strTemp;
	CString strPrinter;

	GetReceiptPrinterName(strPrinter);
	if( !strPrinter.IsEmpty() )
	{
		bIsWTPPrinter = FALSE;
		ids = ::OpenPrinter( strPrinter.GetBuffer(0), &hPrinter, NULL );
		if( 0 >= ids)
		{
			strTemp.Format(_T("[%s] 출력기를 찾을 수 없습니다."),strPrinter);
			CLocCommonAPI::MakeRFIDLog( _T("[ERROR]OpenPrinter()"), strTemp, __WFILE__, __LINE__, ids );
			AfxMessageBox(_T("발견된 출력기가 없습니다."));
			delete [] szStr;
			return 200;
		}			
	}
	else
	{
//*/ END ---------------------------------------------------------------------------	
		ids = LP_StartUp();	
		if( 0 != ids )
		{
			
			{
				LP_GetErrorMsg( ErrMsg );
				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
				AfxMessageBox(_T("발견된 출력기가 없습니다."));
				delete [] szStr;
				return 200;
			}
		}
	}

	if(TRUE == bStatusCheck)
	{
		if( TRUE == bIsWTPPrinter )
		{
 			nStatus = LP_GetStatus( );
 			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("상태정보 가져오기"), __WFILE__, __LINE__, nStatus );
 			if ( 0 > nStatus )
 			{
 				LP_CleanUp();
 				LP_GetErrorMsg( ErrMsg );
 				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_GetStatus()"), ErrMsg, __WFILE__, __LINE__, ids );
 				AfxMessageBox(_T("출력기의 전원이 꺼져있거나 연결이 되어 있지 않습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
 				delete [] szStr;
 				return 100;
 			}
 			else if ( 0 != nStatus )
 			{
 				LP_CleanUp();
 				ids = LP_StartUp();
 				if( 0 != ids )
 				{
 					LP_GetErrorMsg( ErrMsg );
 					CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
 					AfxMessageBox(_T("발견된 출력기가 없습니다."));
 					delete [] szStr;
 					return 200;
 				}
 			}	
		}
		// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
		else
		{
			DWORD dwStatus = 0;
			BOOL bError = IsPrinterError(hPrinter, dwStatus);
			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("상태정보 가져오기"), __WFILE__, __LINE__,  dwStatus);
			if( FALSE != bError )	//Error
			{
				CLocCommonAPI::MakeRFIDLog( _T("[ERROR]GetPrinter()"), _T("출력기를 사용할 수 없습니다."), __WFILE__, __LINE__, bError );
				if(PRINTER_STATUS_NOT_AVAILABLE == dwStatus)
				{
					AfxMessageBox(_T("출력기의 전원이 꺼져있거나 연결이 되어 있지 않습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
				}
				else
				{
 					AfxMessageBox(_T("출력기 상태에 문제가 있어서 출력할 수 없습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
				}
				::ClosePrinter(hPrinter);
 				delete [] szStr;
 				return 100;
			}
		}
//*/ END ---------------------------------------------------------------------------
	}

	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
	if( TRUE != bIsWTPPrinter )
	{
		//출력타이틀 찾기 - [헤더내용]
		INT nStart = sRctStr.Find('[', 0) + 1;
		INT nEnd = sRctStr.Find(']', nStart);
		strTemp = sRctStr.Mid(nStart, nEnd-nStart);

		DOC_INFO_1 di = {0x00};
		di.pDocName = strTemp.GetBuffer(0);
		di.pOutputFile = NULL;
		di.pDatatype = _T("RAW");

		::StartDocPrinter(hPrinter, 1, (LPBYTE)&di);
		::StartPagePrinter(hPrinter);
	}
//*/ END ---------------------------------------------------------------------------

	CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );

	/// TCHAR 형식을 UCHAR 형식으로 변환해준다.
	//++2008.05.21 UPDATE BY PWR {{++
	// 출력할 데이터가 6000정도가 넘어가면 영수증출력이 이루어지지 않는다.
	// 5000정도를 잘라서 루프를 돌면서 이어출력하도록 수정.	
	INT nOffset = 0;
	INT nAddIndex = 0;
	INT nCutStep = 0;
	INT nLength = 0;
	INT	i = 0;
	INT nIndex = 0;
		
//	nLength = sRctStr.GetLength( );

	//++2007.11.1 UNICODE WORK BY PDW {{++
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( sRctStr.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}

	szStr[len]= '\0';
	nLength = len;
//	ids = LP_PrintData( szStr );
	//--2007.11.1 UNICODE WORK BY PDW --}}

	while( i < nLength )
	{
		nAddIndex = 0;
		nCutStep = 0;
		nIndex = 0;		
		// 잘라진 부분부터 다시 시작
		for( i = nOffset ; i < nLength ; i++ )
		{
			szStr[nIndex] = (UCHAR)szTmp[i];
			// 데이터가 5000이 되면
			if ( (i > 0) && (i % 2500 == 0) )
			{				
				// 책의 반납형태까지 출력하고 잘른다.
				// 자르는부분 = 잘른부분(5000)부터 반납형태까지 찾은부분에서 자르기전부분
				/*
				nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i) + 15 )-i;				
				if ( sRctStr.Mid(i+nAddIndex, 1 ) == _T(" ") )
				{
					// 자르는 부분이 등록번호에서 끝나게 되면 한줄더 내려간다.
					nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i+nAddIndex) + 15 )-i;					
				}
				*/			
				
				CHAR* szFindDest = strstr(szTmp+i, "<!LN><!FP><!FN>  ") + 15;				
				CHAR* szFindBuf = szTmp + i; // 테스트용

				nAddIndex = szFindDest - (szTmp+i) + 1;
				
				if( 0 < nAddIndex &&  ' ' == szTmp[i+nAddIndex] )
				{
					szFindDest = strstr(szTmp+i+nAddIndex, "<!LN><!FP><!FN>  ") + 15;
					nAddIndex = szFindDest - (szTmp+i) + 1;
				}
	
			}
			
			if ( nAddIndex > 0 ) nCutStep ++;
			nOffset++;

			if ( nAddIndex > 0 && nCutStep == nAddIndex ) 
			{
				break;
			}
			nIndex++;
		}
		
		szStr[nIndex]= '\0';
		
		// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
		ids = LP_PrintData( szStr );	
*/// AFTER -------------------------------------------------------------------------
		if( TRUE == bIsWTPPrinter )
		{
			ids = LP_PrintData( szStr );		
		}
		else
		{				
			cv.MultiByteToUnicode( (char*)szStr, &szDest, &len2 );	
			strTemp.Format( _T("%s"), szDest );
			if( 0 < len2 )
			{
				delete [] szDest;
				szDest = NULL;
			}
			
			RemoveTag(strTemp);

			cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
			::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
			
			if( 0 < len2 )
			{
				delete [] szTmp2;
				szTmp2 = NULL;
			}
		}
//*/ END ---------------------------------------------------------------------------
	}

	if( 0 < len )
	{
		delete [] szTmp;
	}
		
	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
	if(FALSE == bStatusCheck)
	{
		LP_PrintCut();
	}

	LP_CleanUp();
*/// AFTER -------------------------------------------------------------------------
	if( 0 < nLength )	//메모리 누수로 추가
	{
		delete [] szStr;
	}

	if( TRUE == bIsWTPPrinter )
	{
		if(FALSE == bStatusCheck)
		{
			LP_PrintCut();
		}

		LP_CleanUp();
	}
	else
	{
		//페이지 끝 처리
		strTemp.Format(_T("\r\n\r\n\r\n\r\n\x1b\x69")); // \r\n(위치보정), \x1b\x69(print_cut)
		cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
		::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
		if( 0 < len2 )
		{
			delete [] szTmp2;
		}
		
		::EndPagePrinter(hPrinter);
		::EndDocPrinter(hPrinter);
		::ClosePrinter(hPrinter);
	}
//*/ END ---------------------------------------------------------------------------

	return 0;
}

// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
//설정파일의 프린터명 얻어오기
VOID CBO_LOC_RECEIPT_PRINT_DLG::GetReceiptPrinterName(CString &strValue)
{
	// 인증코드 
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;
	
	strTemp = _T("..\\cfg\\ReceiptPrintName.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) 
	{	
		return;
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	ids = file.ReadString (strLine);	
	strValue = strLine;
	file.Close();
}

//프린터 태그 제거
VOID CBO_LOC_RECEIPT_PRINT_DLG::RemoveTag(CString &strValue)
{
	strValue.Replace(_T("<!FH>"),_T("\x1b\x21\x51\x1b\x45\r"));		//header/footer 시작- \x1b\x21\x51(enlarge), \x1b\x45\r(bold)
	strValue.Replace(_T("<!FF>"),_T("\x1b\x21\x38"));				//\x1b\x21x38(bold_width_height) 
	strValue.Replace(_T("<!LF>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FC>"),_T("\x1b\x61\x31"));				//\x1b\x61\x31(align_center)
	strValue.Replace(_T("<!LC>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!FN>"),_T("\x1b\x61\x30\x1b\x21\x02"));	//body 시작 - \x1b\x61\x30(align_left), \x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!LN>"),_T(""));
	strValue.Replace(_T("<!FR>"),_T("\x1b\x61\x32"));				//\x1b\x61\x32(align_right)	
	strValue.Replace(_T("<!LR>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!LH>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FP>"),_T("\r\n"));
}

BOOL CBO_LOC_RECEIPT_PRINT_DLG::GetJobs(HANDLE hPrinter,        /* Handle to the printer. */ 
                JOB_INFO_2 **ppJobInfo, /* Pointer to be filled.  */ 
                INT *pcJobs,            /* Count of jobs filled.  */ 
                DWORD *pStatus)         /* Print Queue status.    */ 

{

	DWORD               cByteNeeded,
						nReturned,
						cByteUsed;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */ 
   if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
           return FALSE;
   }

   pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
   if (!(pPrinterInfo))
       /* Failure to allocate memory. */ 
       return FALSE;

   /* Get the printer information. */ 
   if (!GetPrinter(hPrinter,
           2,
           (unsigned char*)pPrinterInfo,
           cByteNeeded,
           &cByteUsed))
   {
       /* Failure to access the printer. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   /* Get job storage space. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           NULL,
           0,
           (LPDWORD)&cByteNeeded,
           (LPDWORD)&nReturned))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
       {
           free(pPrinterInfo);
           pPrinterInfo = NULL;
           return FALSE;
       }
   }

   pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
   if (!pJobStorage)
   {
       /* Failure to allocate Job storage space. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   ZeroMemory(pJobStorage, cByteNeeded);

   /* Get the list of jobs. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           (LPBYTE)pJobStorage,
           cByteNeeded,
           (LPDWORD)&cByteUsed,
           (LPDWORD)&nReturned))
   {
       free(pPrinterInfo);
       free(pJobStorage);
       pJobStorage = NULL;
       pPrinterInfo = NULL;
       return FALSE;
   }

   /*
    *  Return the information.
    */ 
   *pcJobs = nReturned;
   *pStatus = pPrinterInfo->Status;
   *ppJobInfo = pJobStorage;
   free(pPrinterInfo);

   return TRUE;

}

BOOL CBO_LOC_RECEIPT_PRINT_DLG::IsPrinterError(HANDLE hPrinter, DWORD &dwStatus)
{
   JOB_INFO_2  *pJobs;
   INT         cJobs,
               i;
   DWORD       dwPrinterStatus;

   /*
    *  Get the state information for the Printer Queue and
    *  the jobs in the Printer Queue.
    */ 
   if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
		return FALSE;

   dwStatus = dwPrinterStatus;

   /*
    *  If the Printer reports an error, believe it.
    */ 
   if (dwPrinterStatus &
       (PRINTER_STATUS_ERROR |
       PRINTER_STATUS_PAPER_JAM |
       PRINTER_STATUS_PAPER_OUT |
       PRINTER_STATUS_PAPER_PROBLEM |
       PRINTER_STATUS_OUTPUT_BIN_FULL |
       PRINTER_STATUS_NOT_AVAILABLE |
       PRINTER_STATUS_NO_TONER |
       PRINTER_STATUS_OUT_OF_MEMORY |
       PRINTER_STATUS_OFFLINE |
       PRINTER_STATUS_DOOR_OPEN))
   {
       free( pJobs );
       return TRUE;
   }

   /*
    *  Find the Job in the Queue that is printing.
    */ 
   for (i=0; i < cJobs; i++)
   {
       if (pJobs[i].Status & JOB_STATUS_PRINTING)
       {
           /*
            *  If the job is in an error state,
            *  report an error for the printer.
            *  Code could be inserted here to
            *  attempt an interpretation of the
            *  pStatus member as well.
            */ 
           if (pJobs[i].Status &
               (JOB_STATUS_ERROR |
               JOB_STATUS_OFFLINE |
               JOB_STATUS_PAPEROUT |
               JOB_STATUS_BLOCKED_DEVQ))
           {
               free( pJobs );
               return TRUE;
           }
       }
   }

   /*
    *  No error condition.
    */ 
   free( pJobs );
   return FALSE;

}
//*/ END ---------------------------------------------------------------------------

// 출력하고 난 후에 DB에 반영한다.
INT CBO_LOC_RECEIPT_PRINT_DLG::setDataToDB(CESL_DataMgr *pRctDM, CESL_Grid* pRctGrid, CString sReceiptStr, BOOL bIsLoan, CString f_strPrintDate)
{
	INT ids = 0;
	CString sLastNumber = _T("");
	CString sLoanKeyList = _T("");

	/************************************************************/
	/*FRAME의 시작												*/
	/************************************************************/
	ids = pRctDM->StartFrame( );
	if ( ids < 0 )	return -1;

	// 1. 마지막번호 하나 증가
	ids = setLastNumber( pRctDM, sLastNumber );
	if ( ids < 0 ) return -2;	
	
	// 2. 출력 유무 변경
	ids = setReceiptPrintYN( pRctDM, pRctGrid, sLoanKeyList, bIsLoan );
	if ( ids < 0 ) return -3;
	
	// 3. 영수증 이력관리 테이블에 반영
	ids = setReceiptTable( pRctDM, pRctGrid, sReceiptStr, sLastNumber, bIsLoan, sLoanKeyList, f_strPrintDate );
	if ( ids < 0 ) return -4;	
	
	ids = pRctDM->SendFrame ( ) ;
	if ( ids < 0 )	return -5;

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;
	/************************************************************/
	/*FRAME의 끝												*/
	/************************************************************/

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setLastNumber(CESL_DataMgr *pRctDM, CString &sLastNo)
{
	INT ids = 0;
	CString sWhere = _T("");
	CString sRecKey = _T("");

	pRctDM->InitDBFieldData( );

	//++2009.01.02 ADD BY KSJ {{++
	//Where 조건에 MAKE_YEAR 추가
	//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
	//CString		strCurrentYear;
	//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
	//--2009.01.02 ADD BY KSJ --}}

	//sWhere.Format( _T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE") , strCurrentYear );
// 	sWhere = _T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere.Format(_T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);

	ids = pRctDM->GetOneValueQuery( sWhere, sRecKey );
	if ( 0 > ids ) 
	{
		//++2009.01.02 UPDATE BY KSJ {{++
		//레코드가 없을 경우, 레코드 생성 후 처리한다
		if( -4007 == ids )
		{
			pRctDM->StartFrame();
			pRctDM->InitDBFieldData();
			
			ids = pRctDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
			
			//ids = pRctDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
			//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("영수증_일련번호") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pRctDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			pRctDM->SendFrame();
			pRctDM->EndFrame();
		}
		else
		{
			return -1;
		}
		//--2009.01.02 UPDATE BY KSJ --}}
	}
	//if ( ids < 0 )	return -1;

	//sWhere.Format( _T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE") , strCurrentYear );
// 	sWhere = _T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere.Format(_T("SELECT (LAST_NUMBER+1) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	ids = pRctDM->GetOneValueQuery( sWhere, sLastNo );
	if ( ids < 0 )	return -2;

	ids = pRctDM->AddDBFieldData ( _T("LAST_NUMBER"), _T("NUMERIC"), sLastNo );
	if ( ids < 0 )	return -3;

	ids = pRctDM->MakeUpdateFrame( _T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -4;	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setReceiptPrintYN(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString &sLoanKeyList, BOOL bIsLoan)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	INT nTab2Idx;
	CString sResultLoanYN = _T("");
	CString sResultReturnYN = _T("");
	CString sRecKey = _T("");

	sLoanKeyList.Empty( );
	pRctDM->InitDBFieldData( );

	nTab2Idx = m_tab2.GetCurSel();
	if ( nTab2Idx == 2 )
	{
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{	
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
	}
	

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		/*
		ids = pRctDM->GetCellData( _T("대출출력유무"), nIdx, sResultLoanYN );
		if ( ids < 0 ) return -4;
		ids = pRctDM->GetCellData( _T("반납출력유무"), nIdx, sResultReturnYN );
		if ( ids < 0 ) return -4;
		*/
		if ( nTab2Idx == 2 ) nIdx = i;

		ids = pRctDM->GetCellData( _T("REC_KEY"), nIdx, sRecKey );
		if ( ids < 0 ) return -5;

		if ( sLoanKeyList.IsEmpty( ) )	sLoanKeyList = sRecKey;
		else							sLoanKeyList += _T(", ") + sRecKey;

		if ( bIsLoan )	
		{
			sResultLoanYN = _T("Y");
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), sResultLoanYN );
		if ( ids < 0 )	return -6;
		}
		else			
		{
			sResultReturnYN = _T("Y");			
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), sResultReturnYN );
			if ( ids < 0 )	return -6;
		}		
		
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 )	return -7;

		if ( i+1 != nSelectCnt && nTab2Idx != 2 )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -8;
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setReceiptTable(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid, CString sReceiptStr, CString sLastNumber, BOOL bIsLoan, CString sLoanKeyList, CString f_strPrintDate)
{
	INT ids = 0;
	CString sRecKey = _T("");	
	CString	strPrintDate;
	
	//++2006.05.26 ADD BY PDW {{++
	// 영수증이력을 XML로 변경
	ids = ChangeReceiptStrTOXML( sReceiptStr );
	if ( ids < 0 ) return -6;
	//--2006.05.26 ADD BY PDW --}}

	pRctDM->InitDBFieldData( );

	ids = pRctDM->MakeRecKey( sRecKey );
	if ( ids < 0 ) return -1;

	ids = pRctDM->AddDBFieldData( _T("LOAN_KEY_LIST"), _T("STRING"), sLoanKeyList );
	if ( ids < 0 ) return -2;

	ids = pRctDM->AddDBFieldData( _T("RECEIPT_STRING"), _T("STRING"), sReceiptStr );
	if ( ids < 0 ) return -3;

	ids = pRctDM->AddDBFieldData( _T("RCT_SEQ_NO"), _T("NUMERIC"), sLastNumber );
	if ( ids < 0 ) return -4;

	//++2008.03.14 ADD BY PWR {{++
	// 출력안함일때에도 PRINT_COUNT=0으로 해서 저장하도록 한다.
	if( m_strPrintYN == _T("Y") )
	{
		ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("1") );
		if ( ids < 0 ) return -5;
	}
	else
	{
		ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("0") );
		if ( ids < 0 ) return -5;
	}
	//ids = pRctDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("1") );
	//if ( ids < 0 ) return -5;
	//--2008.03.14 ADD BY PWR --}}

	/*
	sInLoanDate.Format( _T("to_date('%s %d:%d:%d','YYYY/MM/DD HH24:MI:SS')") , sLoanDate , tLoanDate.GetHour() , tLoanDate.GetMinute() , tLoanDate.GetSecond() );
	ids = pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sLoanDate , TRUE );
	*/	
	strPrintDate.Format(_T("to_date('%s:00','YYYY/MM/DD HH24:MI:SS')"), f_strPrintDate);
	ids = pRctDM->AddDBFieldData( _T("PRINT_DATE"), _T("NUMERIC"), strPrintDate );
	if ( ids < 0 ) return -5;

	// 2005.06.08 ADD BY PDJ
	// 'x' 버튼으로 종료시 소멸되어 로그남길떄 다이함.
	/*
#ifdef __K2UP__
	
	  // 체크해도 다이해서 주석처리함.
	if ( m_pParentMgr->m_pInfo->USER_ID != _T("") )
	{
		ids = pRctDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("영수증출력"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -6;
	}
#endif
	*/
	ids = pRctDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( ids < 0 ) return -7;

	//++2008.10.27 ADD BY PWR {{++
	// 통합시스템 적용
// 	ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	ids = pRctDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE );
	if ( ids < 0 ) return -7;
	//--2008.10.27 ADD BY PWR --}}

	ids = pRctDM->MakeInsertFrame( _T("CO_LOAN_RECEIPT_TBL") );
	if ( ids < 0 ) return -8;

	// 2005.03.15 ADD BY PDJ
	// 4. 이력정보에 영수증 이력키 업데이트		
	INT nIdx = 0;
	INT nSelectCnt = 0;	
	INT nTab2Idx;
	CString sLoanRecKey = _T("");

	sLoanKeyList.Empty( );
	pRctDM->InitDBFieldData( );

	nTab2Idx = m_tab2.GetCurSel();
	if ( nTab2Idx == 2 )
	{
		// 전체면 선정 구분없이 모두 출력
		nSelectCnt = pRctDM->GetRowCount();
		if ( nSelectCnt == 0 ) return 1;
	}
	else
	{
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
	}

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if ( nTab2Idx == 2 ) nIdx = i;

		ids = pRctDM->GetCellData( _T("REC_KEY"), nIdx, sLoanRecKey );
		if ( ids < 0 ) return -5;

		// 대출일 경우 대출영수증 키로
		if ( bIsLoan  )
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_LOAN_KEY"), _T("NUMERIC"), sRecKey );
			if ( ids < 0 )	return -6;
		}
		else
		{
			//반납일 경우 반납영수증 키로
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_RETURN_KEY"), _T("NUMERIC"), sRecKey );
			if ( ids < 0 )	return -6;
		}
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sLoanRecKey );
		if ( ids < 0 )	return -7;

		if ( i+1 != nSelectCnt && nTab2Idx != 2 )
		{
			nIdx = pRctGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -8;
		}
	}

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::setDMAndGrid(CESL_DataMgr *pRctDM, CESL_Grid *pRctGrid)
{
	INT ids = 0;
	INT nSelectCnt = 0;
	INT nRowIndex = 0 ;
	INT nTotalCnt = 0 ;	

	// 이력을 출력하는 경우 출력데이터를 삭제하지 않는다.
	if ( GetTabCtlIdx(1) == 1 ) return 0;
	

	ids = pRctGrid->SelectMakeList( );
	if ( ids < 0 ) return -1;
	
	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt < 0 ) return -2;

	nSelectCnt = pRctGrid->SelectGetCount( );
	if ( nSelectCnt <= 0 )
	{
		AfxMessageBox( _T("선정된 데이터가 없습니다.") );
		return 0;
	}

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -1;

	/// 반드시 뒤에서부터 해야 이전의 Index를 변경시키지 않고 작업을 수행할 수 있다. 
	ids = pRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	for ( INT i = 0 ; i < nSelectCnt ; i++ ) 
	{
		nRowIndex = pRctGrid->SelectGetIdx( );
		if ( nRowIndex < 0 ) return -3;

		/// DM의 데이터를 지우고
		ids = pRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
	
		/// Grid의 데이터를 지운다.
		/// Grid상에 하나만 남아 있는 열을 RemoveItem 함수로는 지울 수 없기 때문에 
		/// Grid상의 전체 데이터와 선택된 데이터 수가 같으면 한 번에 모두 지우는 
		/// RemoveAll2 함수를 사용했다.
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

		/// 마지막 데이터에서 또 이전데이터를 호출하면 에러가 발생한다.
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

// Grid의 데이터를 지워도 번호가 다시 재배열되지 않기 때문에 직접 번호를 입력하여 
// 변경된 화면에 맞춰줘야 한다.
VOID CBO_LOC_RECEIPT_PRINT_DLG::renameNoInGrid(CESL_Grid *pRctGrid, INT nTotalCnt)
{
	CString sNo = _T("") ;

	for ( INT i = 0 ; i < nTotalCnt ; i++ ) 
	{
		sNo.Format ( _T("%d"), ( i + 1 ) ) ;

		pRctGrid->SetTextMatrix ( (i + 1 ), 0, sNo ) ;
	}
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPREVIEW() 
{
	CString	strPrintData;
	INT		ids;	

	if ( m_tab2.GetCurSel() == 2 )
	{
		CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
		CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
		if ( NULL == pTotalRctGrid ) return;
		pTotalRctGrid->SelectMakeList();

		//++2008.05.13 ADD BY PWR {{++
		// 선정된 데이터가 없으면 미리보기 안하도록 수정
		INT	nRowCount;
		nRowCount = pTotalRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("선정된 자료가 없습니다.") );
			return;
		}
		//--2008.05.13 ADD BY PWR --}}
	}
	
	ids = MakePrintData(strPrintData,_T(""));
	if ( ids > 0 ) return;		
	
	CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
	dlgPreview.m_strDevice = m_strDevice;
	dlgPreview.SetPrintData(strPrintData);	
	
	if ( dlgPreview.DoModal() == IDOK )
	{
		//++2008.05.13 ADD BY PWR {{++
		// 출력, 출력안함 구분값 추가
		m_strPrintYN = _T("Y");
	
		if ( m_tab2.GetCurSel() == 2 )
		{
			doPrintReceiptSelectAll();
		}
		else
		{
			doPrintReceipt();
		}
		//--2008.05.13 ADD BY PWR --}}
	}	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{	 
	CheckTab();
	*pResult = 0;
}
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnRadSelChange();

	*pResult = 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::CheckTab(INT nType)
{		
	INT		nIdx;	

	if ( GetSafeHwnd() == NULL )
	{
		return; 
	}

	if (nType != 0 )
	{
		m_tab2.SetCurSel(0);
		OnRadSelChange();
	}
	
	nIdx = GetTabCtlIdx(0);

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sWhere = _T("");

	if (nType == 1 )
	{
		m_tab.SetCurSel(0);		
		((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}
	if (nType == 2 )
	{
		// 대출/반납중 한가지만 사용할때는 무조건 0이다.
		if ( m_tab.GetItemCount() == 1 ) 
		{
			m_tab.SetCurSel(0);
		}
		else 
		{
			m_tab.SetCurSel(1);
		}
		
		//((CButton*) GetDlgItem(IDC_radLOAN))->SetCheck(1);
		//((CButton*) GetDlgItem(IDC_radRETURN))->SetCheck(0);
	}	



	initData();

	/*
	//CESL_Grid* pRctGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT2));
	CESL_Grid* pRctTotalGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT3));
	
	//DWORD CheckLogGrid;
	DWORD chkTotalGrid;
	//CheckLogGrid = ( pRctGrid->GetStyle() & WS_VISIBLE );
	chkTotalGrid = ( pRctTotalGrid->GetStyle() & WS_VISIBLE );

	//if ( CheckLogGrid ) pRctGrid->ShowWindow(SW_HIDE);
	if ( chkTotalGrid ) pRctTotalGrid->ShowWindow(SW_HIDE);	
	*/

	/*
	ids = getReceiptWhere( sWhere, nIdx );
	if ( ids != 0 ) return;
	
	ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
	if ( ids < 0 ) return;

	ids = AllControlDisplay( sReceiptCMAlias );
	if ( ids < 0 ) OnCancel( );
	*/	

}

VOID CBO_LOC_RECEIPT_PRINT_DLG::ClearData()
{
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");		
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return;

	pRctDM->FreeData();
	GetDlgItem( IDC_edtUSER_NO )->SetWindowText( _T("") );
	GetDlgItem( IDC_edtUSER_NAME )->SetWindowText( _T("") );	
	
	AllControlDisplay( sReceiptCMAlias );
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakePrintData(CString &f_pstrData, CString f_strPrintDate)
{
	BOOL bIsLoan = TRUE;
	INT	nIdx, nIdx2;

	INT ids = 0;
	INT nSelectCnt = 0;
	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString sReceiptStr = _T("");

	nIdx2 = GetTabCtlIdx(1);

	// _T("") 이면 미리보기인 경우
	if ( f_strPrintDate==_T("") && ( nIdx2 == 0 || nIdx2 == 2 ) )
	{
		// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
		CString strGetData;
		CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
		f_strPrintDate = strGetData.Left(16);
// 		CTime tTime = CLocCommonAPI::GetDBTime( this );
// 		f_strPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	}
	
	// 이력이면 미리보기던 머던 무조건 DB값으로 보여줌
	if ( nIdx2 == 1 )
	{
		INT	nRowIdx ;
		CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );
		if( pRctLogDM == NULL ) return -100;
		CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
		if ( NULL == pRctLogGrid ) return -110;

		nRowIdx = pRctLogGrid->GetIdx();

		pRctLogDM->GetCellData( _T("PRINT_DATE"), nRowIdx, f_strPrintDate );

	}

	nIdx = GetTabCtlIdx(0);
	if ( nIdx == 0 ) bIsLoan = TRUE;
	else bIsLoan = FALSE;

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	// 파일로부터 일정한 형식의 영수증 포맷을 문자열로 읽어들인다.
	ids = getReceiptString( sReceiptStr );
	if ( ids != 0 ) return ids;	

	/**********************************************/
	// 1. DM_BO_LOC_3100_RECEIPT 으로 셋팅
	//    (_T("%대_반일%"), _T("%서지정보%"), _T("%합계%"), _T("%추가내용%"))
	/**********************************************/
	ids = setRctTempVariable( pRctDM, pRctGrid, sReceiptStr, bIsLoan );
	if ( ids != 0 ) return ids;
	
	/**********************************************/
	// 2. GETONEVALUEQUERY로 셋팅
	//    (_T("%도서관명%"),  _T("%일련번호%"))
	/**********************************************/
	ids = setRctTempVariable( pRctDM, sReceiptStr );
	if ( ids != 0 ) return ids;

	/**********************************************/
	// 3. 바로 셋팅 가능한 부분
	//    (_T("%대_반제목%"),_T("%대_반일제목%"), _T("%출력일시%"))
	/**********************************************/
	// 2005.03.16
	// ADD BY PDJ 출력물과 로그에 찍기위한 날짜를 분단위 까지 맞추기 위해
	// param으로 넘긴다.
	ids = setRctTempVariable( sReceiptStr, bIsLoan, f_strPrintDate );
	if ( ids != 0 ) return ids;
	
	/**********************************************/
	// 4. DM_BO_LOC_3100_MEMBER_INFO 으로 셋팅
	//    (_T("%대출자번호%"), _T("%대출자이름%"), _T("%부서명%"))
	/**********************************************/
	ids = setRctTempVariable( sReceiptStr );
	if ( ids != 0 ) return ids;

	// 2005.03.03 ADD BY PDJ
	// 영수증 출력기가 한자지원이 안되는 관계로 한글로 변환하여 출력한다.	
	CMakeSearchData MakeSearchData(m_pParentMgr);
	//INT iCh = MakeSearchData.ChangeHanjaToHangul( sReceiptStr );	//한자 한글 변환

	f_pstrData = sReceiptStr;

	return 0;

}


BEGIN_EVENTSINK_MAP(CBO_LOC_RECEIPT_PRINT_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CPDJ_TestDlg)
	ON_EVENT(CBO_LOC_RECEIPT_PRINT_DLG, IDC_gridRECEIPT2, -600 /* Click */, OnClickgridRECEIPT2, VTS_NONE)
	ON_EVENT(CBO_LOC_RECEIPT_PRINT_DLG, IDC_gridRECEIPT3, -600 /* Click */, OnClickgridRECEIPT3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnClickgridRECEIPT2() 
{	
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias2 = _T("GridLog");
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridLog") );
	if ( NULL == pRctLogGrid ) return ;
	INT	nIdx;

	nIdx = pRctLogGrid->GetIdx();
	ClickLog(nIdx);
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnClickgridRECEIPT3() 
{	
	INT nCol;	

	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias3 = _T("GridTotalReceipt");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	CESL_Grid* pRctTotalGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias3 );
	
	nCol = pRctTotalGrid->GetCol();

	if ( nCol == 0 ) return;
	initData(FALSE);
	/*
	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sGridAlias3 = _T("GridTotalReceipt");
	CESL_Grid* pRctTotalGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias3 );
	if ( NULL == pRctTotalGrid ) return ;
	INT	nIdx;

	nIdx = pRctTotalGrid->GetIdx();
	ClickLog(nIdx);
	*/
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetTabCtlIdx(INT nType)
{
	TCITEM	tcm;
	CString	strTab;
	TCHAR	szBuff[64];
	INT		nIdx;

	tcm.mask = TCIF_TEXT;
	tcm.pszText = szBuff;
	tcm.cchTextMax = sizeof(szBuff);

	if ( nType == 0 )
	{
		nIdx = m_tab.GetCurSel();
		m_tab.GetItem(nIdx, &tcm);
		strTab = tcm.pszText;		

		if ( strTab == _T(" 대출 ") )
		{
			return 0;
		}
		else if ( strTab == _T(" 반납 ") )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		nIdx = m_tab2.GetCurSel();
		m_tab2.GetItem(nIdx, &tcm);
		strTab = tcm.pszText;
		if ( strTab == _T(" 영수증출력정보 ") )
		{
			return 0;
		}
		else if ( strTab == _T(" 출력된영수증이력 ") )
		{
			return 1;
		}
		else if ( strTab == _T(" 전체미출력정보 ") )
		{
			return 2;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnSETUP()
{		
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );

	CRECEIPT_PRINT_SET_DLG dlg(this, m_pLMV );
	dlg.m_pRctLogDM = pRctLogDM;
	dlg.DoModal();	
	//++2008.04.14 ADD BY PWR {{++
	// 공지사항 추가 m_strNotify
	m_pLMV->GetReceiptManageValue(m_strBarcode, m_strBarcodeAll, m_strItem, m_strMode, m_strDevice, m_strPreview, m_strNotify);
	//--2008.04.14 ADD BY PWR --}}

	if ( dlg.m_strPrintTypeSet != m_pLMV->m_sUseReceiptType )
	{
		m_pLMV->m_sUseReceiptType = dlg.m_strPrintTypeSet;
		TabCheck();
	}
	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnSEARCH()
{
	CString	strSerialNo;

	CRECEIPT_SERARCH_DLG	dlg;
	if ( dlg.DoModal() == IDOK )
	{
		dlg.GetSearchSerialNo(&strSerialNo);
		SearchSerialNo(strSerialNo);
	}	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnDATE_SEARCH()
{
	initData(TRUE);
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnPRINTCANCEL()
{
	INT nIdxTab2;	
	nIdxTab2 = m_tab2.GetCurSel();

	//++2008.04.14 ADD BY PWR {{++
	// 출력, 출력안함 구분값 추가
	m_strPrintYN = _T("N");
	//--2008.04.14 ADD BY PWR --}}

	if ( nIdxTab2 == 0 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// 확인 메시지 처리
		INT		nRowCount;
		CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridReceipt") );
		if ( NULL == pRctGrid ) return;
		pRctGrid->SelectMakeList();	
		nRowCount = pRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("선정된 자료가 없습니다.") );
			return;
		}
		else
		{
			if( AfxMessageBox(	_T("영수증출력을 안하시겠습니까?\n")
								_T("(영수증 출력을 생략합니다.)\n")
								_T("출력 생략한 자료는 출력된영수증이력 텝에서 재출력 하실 수 있습니다.")
								, MB_YESNO ) == IDNO )
			{ return;	}
		}
		//--2008.05.13 ADD BY PWR --}}

		//++2008.04.14 UPDATE BY PWR {{++
		// 출력안함일때에도 PRINT_COUNT=0으로 해서 저장하도록 한다.
		//CancelReceipt();
		doPrintReceipt();
		//--2008.04.14 UPDATE BY PWR --}}		
	}
	else if ( nIdxTab2 == 2 )
	{
		//++2008.05.13 ADD BY PWR {{++
		// 확인 메시지 처리
		INT		nRowCount;
		CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("GridTotalReceipt") );
		if ( NULL == pTotalRctGrid ) return;
		pTotalRctGrid->SelectMakeList();
		nRowCount = pTotalRctGrid->SelectGetCount();
		if ( nRowCount <= 0 )
		{
			AfxMessageBox( _T("선정된 자료가 없습니다.") );
			return;
		}
		else
		{
			if( AfxMessageBox(	_T("영수증출력을 안하시겠습니까?\n")
								_T("(영수증 출력을 생략합니다.)\n")
								_T("출력 생략한 자료는 출력된영수증이력 텝에서 재출력 하실 수 있습니다.")
								, MB_YESNO ) == IDNO )
			{ return;	}
		}
		//--2008.05.13 ADD BY PWR --}}

		//++2008.04.14 ADD BY PWR {{++
		// 출력안함일때에도 PRINT_COUNT=0으로 해서 저장하도록 한다.
		//CancelReceiptAll();
		doPrintReceiptSelectAll();
		//--2008.04.14 ADD BY PWR --}}
	}
}

INT CBO_LOC_RECEIPT_PRINT_DLG::SearchSerialNo(CString f_strSerailNo)
{
	CString sDMLogAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString	strSerialNo;
	INT	ids;
	INT	i;
	INT	nRowCount;

	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sDMLogAlias );
	if( pRctLogDM == NULL ) return -100;

	//++2008.05.13 UPDATE BY PWR {{++
	// 일련번호 검색시 해당 일련번호로 선택되도록 수정.
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( _T("CM_BO_LOC_3100_RECEIPT"), _T("gridLog") );
	if ( NULL == pRctLogGrid ) return -200;

	nRowCount = pRctLogDM->GetRowCount();
	if ( nRowCount < 0 ) return 0;
	
	for ( i = 0 ; i < nRowCount ; i++ )
	{
		ids = pRctLogDM->GetCellData( _T("RCT_SEQ_NO"), i, strSerialNo );
		if ( ids ) return ids;
		
		if ( strSerialNo == f_strSerailNo )
		{
			ids = pRctLogGrid->SetReverse(i);
			if ( ids ) return ids;

			ClickLog(i);
		}
	}	
	//--2008.05.13 UPDATE BY PWR --}}
	
	return 0;	
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::ClickLog(INT f_nIdx)
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	
	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sDMLogAlias = _T("DM_BO_LOAN_RECEIPT_LOG");
	CString sGridAlias = _T("GridReceipt");
	CString sGridAlias2 = _T("GridLog");
	CString	strReceiptKey;
	CString	strRecKey;
	CString strWhere;
	INT	nIdx2;
	INT	nRowCount;
	INT	i;
	INT	ids;


	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return;
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( sDMLogAlias );
	if( pRctLogDM == NULL ) return;
	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	if( pRctLogDM == NULL ) return;

	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return ;
	CESL_Grid* pRctLogGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("gridLog") );
	if ( NULL == pRctLogGrid ) return ;
	
	nIdx2 = GetTabCtlIdx(0);
	pRctLogDM->GetCellData(_T("REC_KEY"), f_nIdx, strRecKey );	

	if ( pRctLogDM->GetRowCount() <= 0 ) return;

	if ( nIdx2 == 0 )
	{
		strWhere.Format(_T("RECEIPT_LOAN_KEY = %s"), strRecKey);
	}
	else
	{
		strWhere.Format(_T("RECEIPT_RETURN_KEY = %s"), strRecKey);
	}

	pRctDM->RefreshDataManager(strWhere);
	
	ids = pCM->ControlDisplay(_T("gridReceipt"), 0 );
	if ( ids < 0 ) return;

	nRowCount = pRctDM->GetRowCount();
	if ( nRowCount > 0 )
	{
		for ( i=0 ; i<nRowCount ; i++ )
		{
			pRctGrid->SetAt( i , 0 , _T("V") );				
		}
	}

	/*
	nRowCount = pRctDM->GetRowCount();
	if ( nRowCount > 0 )
	{
		BOOL bFirstRowSel;
		bFirstRowSel = FALSE;
		for ( i=0 ; i<nRowCount ; i++ )
		{
			if ( nIdx2 == 0 )
			{
				ids = pRctDM->GetCellData(_T("RECEIPT_LOAN_KEY"), i, strReceiptKey );
				if ( ids ) return;
			}
			else
			{
				ids = pRctDM->GetCellData(_T("RECEIPT_RETURN_KEY"), i, strReceiptKey );
				if ( ids ) return;
			}

			if ( strReceiptKey == strRecKey )
			{
				pRctGrid->SetAt( i , 0 , _T("V") );
				if ( bFirstRowSel == FALSE )
				{
					// 첫번째 자료를 맨 위로 올림					
				}
			}
			else
			{
				pRctGrid->SetAt( i , 0 , _T("") );
			}
		}
	}
	
	*/
	//pGrid->m_nReverse = pGrid->GetIdx();
	//m_pLoan->ViewGrid();

	EFS_END	
}

INT CBO_LOC_RECEIPT_PRINT_DLG::DetailNonReturnBookInfo(CString *strNonReturn, CESL_DataMgr *pDM)
{	
	CString	sTmpData;
	CString sTmpBoInfo;
	CString	sSpace;
	CString strWhere;
	INT nRowCount, i, ids;
	
	if( pDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		
	sSpace = _T("                              ");	
	
	nRowCount = pDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		sTmpData.Format(_T("%4d "), i+1 );
		sTmpBoInfo += sTmpData;
		
		ids = pDM->GetCellData( _T("본표제"), i, sTmpData );
		if ( ids < 0 ) return -8;
		// 2010.05.13 UPDATE BY PWR : 미반납목록 등록번호가 두줄로 나오는현상 수정
		sTmpData = getLimitWord(sTmpData.GetBuffer(0), 24, TRUE);
//		sTmpData = getLimitWord( sTmpData.GetBuffer(0), 24, FALSE );
// 		if ( sTmpData.GetLength() < 24 )
// 		{
// 			sTmpData += sSpace.Left(24 - sTmpData.GetLength() );
// 		}
		sTmpBoInfo += sTmpData;

		ids = pDM->GetCellData( _T("등록번호"), i, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpData = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sTmpBoInfo += _T(" ") + sTmpData + _T("<!FP>");
	}
	
	*strNonReturn = sTmpBoInfo;

	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::GetLoanReceiptPrintCnt()
{
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );		
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString	strWhere;
	CString	sUserKey;
	CString	strCnt;
	INT nRowCount, ids;

	ids = GetDataMgrData( sMemberDMAlias, _T("대출자KEY"), sUserKey, 0 );
	
	if( pLoanDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		

	if(!sUserKey.IsEmpty())
	{
		// 2009.03.26 UPDATE BY PWR : VIEW 분리
		strWhere.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
						//2020.04.14 JOB.2020.0033 ADD BY KYJ
						//_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
						_T("WHERE USER_KEY=%s AND (RECEIPT_PRINT_LOAN_YN!='Y' or RECEIPT_PRINT_LOAN_YN IS NULL ) AND LOAN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s')")
						_T("+")
						_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
						//2020.04.14 JOB.2020.0033 ADD BY KYJ
						//_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
						_T("WHERE USER_KEY=%s AND (RECEIPT_PRINT_LOAN_YN!='Y' or RECEIPT_PRINT_LOAN_YN IS NULL ) AND LOAN_DATE IS NOT NULL ")						
						_T("AND MANAGE_CODE='%s') FROM DUAL")
						, sUserKey, m_pInfo->MANAGE_CODE, sUserKey, m_pInfo->MANAGE_CODE );
	// 	// 2008.10.17 UPDATE BY PWR : 통합시스템 적용
	// 	strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 ")
	// 					_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
	// 					_T("AND MANAGE_CODE = '%s'"), sUserKey, m_pInfo->MANAGE_CODE );
		// strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE USER_KEY = %s AND RECEIPT_PRINT_LOAN_YN !='Y' AND LOAN_DATE IS NOT NULL "), sUserKey );
		pLoanDM->GetOneValueQuery(strWhere, strCnt );
		nRowCount = _ttoi(strCnt);
		if ( nRowCount > 0 )
		{
			return nRowCount;
		}
	}

	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::GetReturnReceiptPrintCnt()
{
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );		
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString	strWhere;
	CString	sUserKey;
	CString	strCnt;
	INT nRowCount, ids;

	ids = GetDataMgrData( sMemberDMAlias, _T("대출자KEY"), sUserKey, 0 );
	
	if( pLoanDM == NULL )
	{
		AfxMessageBox(_T("FindDM() Error!"));
		return -100;
	}		

	if(!sUserKey.IsEmpty())
	{
		// 2009.03.26 UPDATE BY PWR : VIEW 분리
		strWhere.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
						_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_RETURN_YN!='Y' AND RETURN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s')")
						_T("+")
						_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
						_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_RETURN_YN!='Y' AND RETURN_DATE IS NOT NULL ")
						_T("AND MANAGE_CODE='%s') FROM DUAL")
						, sUserKey, m_pInfo->MANAGE_CODE, sUserKey, m_pInfo->MANAGE_CODE );
	// 	// 2008.10.17 UPDATE BY PWR : 통합시스템 적용
	// 	strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 ")
	// 					_T("WHERE USER_KEY=%s AND RECEIPT_PRINT_LOAN_YN!='Y' AND LOAN_DATE IS NOT NULL ")
	// 					_T("AND MANAGE_CODE = '%s'"), sUserKey, m_pInfo->MANAGE_CODE );
		// strWhere.Format(_T("SELECT COUNT(1) FROM LH_STORAGE_V01 WHERE USER_KEY = %s AND RECEIPT_PRINT_RETURN_YN !='Y' AND RETURN_DATE IS NOT NULL"), sUserKey );
		pLoanDM->GetOneValueQuery(strWhere, strCnt );
		nRowCount = _ttoi(strCnt);
		if ( nRowCount > 0 )
		{
			return nRowCount;
		}		
	}
	return 0;
}

INT	CBO_LOC_RECEIPT_PRINT_DLG::ReceiptAutoPrint(BOOL bPrintYN)
{	
	INT	nRowCnt;

	//++2008.04.14 ADD BY PWR {{++
	// 이력탭의 LOG초기화해야 한다.
	CESL_DataMgr* pRctLogDM = ( CESL_DataMgr* ) FindDM( _T("DM_BO_LOAN_RECEIPT_LOG") );
	if( pRctLogDM == NULL ) return -110;
	pRctLogDM->FreeData();
	// 탭을 영수증출력정보 탭으로 변경하고 진행해야 대상 회원의 미출력영수증을 출력할 수 있다.
	m_tab2.SetCurSel(0);
	//++2008.04.14 ADD BY PWR --}}
	
	// 출력옵션에 따라 출력할 수 있도록 변경	
	CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gridRECEIPT));		
	if( pGrid == NULL ) return 100;		
	
	m_bPrintYN = bPrintYN;
	nRowCnt = GetLoanReceiptPrintCnt();
	if ( nRowCnt > 0 )
	{
		m_tab.SetCurSel(0);	
		initData();		
		doPrintReceipt();
	}

	nRowCnt = GetReturnReceiptPrintCnt();
	if ( nRowCnt > 0 )
	{
		m_tab.SetCurSel(1);
		initData();			
		doPrintReceipt();
	}

	// 정상으로 돌려놓기
	m_bPrintYN = TRUE;	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::CancelReceipt()
{
	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");
	CString strWhere;
	CString strRecKey;
	INT		nRowCount, nRowIndex;
	INT		nTotalCnt;
	INT		i, ids;
	
	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;
	
	pRctGrid->SelectMakeList();	
	nRowCount = pRctGrid->SelectGetCount();
	if ( nRowCount <= 0 )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	else
	{
		//++2008.04.14 ADD BY PWR {{++
		// 출력취소 -> 출력안함으로 변경 
		//if ( AfxMessageBox("선정된 자료를 출력취소 하시겠습니까?\n취소된 자료는 영수증으로 다시 출력할 수 없습니다.", MB_YESNO) == IDNO )
		if( AfxMessageBox(	_T("영수증출력을 안하시겠습니까?\n")
							_T("(영수증 출력을 생략합니다.)\n")
							_T("출력 생략한 자료는 출력된영수증이력 텝에서 재출력 하실 수 있습니다.")
							, MB_YESNO ) == IDNO )
		{ return 0;	}
		//--2008.04.14 ADD BY PWR --}}
	}

	ids = pRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	nTotalCnt = pRctDM->GetRowCount( );
	if ( nTotalCnt < 0 ) return -1;

	ids = pRctDM->StartFrame( );
	if ( ids < 0 )	return -1;

	pRctDM->InitDBFieldData( );
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nRowIndex = pRctGrid->SelectGetIdx ();		

		/// DM의 데이터를 지우고
		pRctDM->GetCellData( _T("REC_KEY"), nRowIndex, strRecKey );

		if ( m_tab.GetCurSel() == 0 )
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("C") );
			if ( ids < 0 )	return ids;
		}
		else
		{
			ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("C") );
			if ( ids < 0 )	return ids;
		}		
		
		ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		if ( ids < 0 )	return -7;

		ids = pRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
	
		/// Grid의 데이터를 지운다.
		/// Grid상에 하나만 남아 있는 열을 RemoveItem 함수로는 지울 수 없기 때문에 
		/// Grid상의 전체 데이터와 선택된 데이터 수가 같으면 한 번에 모두 지우는 
		/// RemoveAll2 함수를 사용했다.
		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nRowCount ) 
			{
				pRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pRctGrid->RemoveItem( nRowIndex + 1 );
		}
		
		/// 마지막 데이터에서 또 이전데이터를 호출하면 에러가 발생한다.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}

	ids = pRctDM->SendFrame ( ) ;
	if ( ids < 0 )	return -5;

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;

	return 0;

}

INT CBO_LOC_RECEIPT_PRINT_DLG::CancelReceiptAll()
{
	CString	sReceiptStr;
	CString strLoanType;
	CString sWhere;
	CString strRecKey;
	INT		nLoanType;
	INT		nTotalCnt;
	INT		ids;	
	INT		i, y;
	INT		nRowCount;
	INT		nRowCount2;
	INT		nRowIndex;	
	BOOL	bIsLoan;

	CString sReceiptDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CString sReceiptCMAlias = _T("CM_BO_LOC_3100_RECEIPT");	

	CESL_DataMgr* pRctDM = ( CESL_DataMgr* ) FindDM( sReceiptDMAlias );
	if ( NULL == pRctDM ) return -1;

	CESL_DataMgr* pTotalRctDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	if ( NULL == pTotalRctDM ) return -1;
	
	CESL_Grid* pRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("gridReceipt") );
	if ( NULL == pRctGrid ) return -2;

	CESL_Grid* pTotalRctGrid = ( CESL_Grid* ) FindControl( sReceiptCMAlias, _T("GridTotalReceipt") );
	if ( NULL == pTotalRctGrid ) return -2;

	pTotalRctGrid->SelectMakeList();
	nRowCount = pTotalRctGrid->SelectGetCount();
	if ( nRowCount <= 0 )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	else
	{
		//++2008.04.14 ADD BY PWR {{++
		// 출력취소 -> 출력안함으로 변경 
		//if ( AfxMessageBox("선정된 자료를 출력취소 하시겠습니까?\n취소된 자료는 영수증으로 다시 출력할 수 없습니다.", MB_YESNO) == IDNO )
		if( AfxMessageBox(	_T("영수증출력을 안하시겠습니까?\n")
							_T("(영수증 출력을 생략합니다.)\n")
							_T("출력 생략한 자료는 출력된영수증이력 텝에서 재출력 하실 수 있습니다.")
							, MB_YESNO ) == IDNO )
		{ return 0;	}
		//--2008.04.14 ADD BY PWR --}}
	}

	ids = pTotalRctGrid->SelectGetTailPosition( );
	if ( ids < 0 ) return -2;

	nTotalCnt = pTotalRctDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{		
		sWhere=_T("");
		nRowIndex = pTotalRctGrid->SelectGetIdx ();
		
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_TOTAL_RECEIPT"), _T("대출상태"), strLoanType,  nRowIndex);
		if ( ids ) return ids;
		
		if ( strLoanType == _T("대출") ) 
		{
			bIsLoan = TRUE;	
			nLoanType = 0;
		}
		else 
		{		
			bIsLoan = FALSE;
			nLoanType = 1;
		}
		
		ids = getReceiptWhere( sWhere, nLoanType,nRowIndex );
		if ( ids > 0 )
		{
			// 화면 초기화
			ClearData();	
		}
		if ( ids != 0 ) return (ids);	
		
		if ( !sWhere.IsEmpty() )
		{
			ids = RefreshDataMgr( sReceiptDMAlias, sWhere );
			if ( ids < 0 ) return -2;	
		}	
		
		nRowCount2 = pRctDM->GetRowCount();	
		ids = pRctDM->StartFrame( );
		if ( ids < 0 )	return -1;
		
		pRctDM->InitDBFieldData( );
		for ( y=0 ; y<nRowCount2 ; y++ )
		{
			/// DM의 데이터를 지우고
			pRctDM->GetCellData( _T("REC_KEY"), y, strRecKey );
			
			if ( bIsLoan )
			{
				ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("C") );
				if ( ids < 0 )	return ids;
			}
			else
			{
				ids = pRctDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("C") );
				if ( ids < 0 )	return ids;
			}		
			
			ids = pRctDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
			if ( ids < 0 )	return -7;
		}

		ids = pRctDM->SendFrame ( ) ;
		if ( ids < 0 )	return -5;

		if ( 0 != nTotalCnt ) 
		{
			if ( nTotalCnt == nRowCount ) 
			{
				pTotalRctGrid->RemoveAll2( );
				nTotalCnt = 0;
			}
			else
				pTotalRctGrid->RemoveItem( nRowIndex + 1 );
		}

		ids = pTotalRctDM->DeleteRow( nRowIndex, -1 ); 
		if ( ids < 0 ) return -4;
		
		/// 마지막 데이터에서 또 이전데이터를 호출하면 에러가 발생한다.
		if ( ( i + 1 ) == nRowCount )	break;
		ids = pTotalRctGrid->SelectGetPrev( );
		if ( ids < 0 ) return -5;
	}	

	ids = pRctDM->EndFrame ( ) ;
	if ( ids < 0 )	return -6;

	pRctDM->FreeData();
	CESL_ControlMgr *pCM = FindCM( sReceiptCMAlias );
	if( pCM == NULL ) return -110;
	pCM->ControlDisplay(_T("gridReceipt"), 0 );

	return 0;
}



INT CBO_LOC_RECEIPT_PRINT_DLG::TabCheck()
{
	CString sRctUseType = _T("");	

	m_tab.DeleteAllItems();
		

	sRctUseType = m_pLMV->m_sUseReceiptType;
	if ( _T("YY") != sRctUseType )
	{
		if ( 'Y' == sRctUseType.GetAt(0) )	m_tab.InsertItem( 0, _T(" 대출 ") );
		else m_tab.InsertItem( 0, _T(" 반납 ") );
	}
	else
	{
		m_tab.InsertItem( 0, _T(" 대출 ") );
		m_tab.InsertItem( 1, _T(" 반납 ") );
	}

	CString sTotalReceiptDMAlias = _T("DM_BO_LOC_3100_TOTAL_RECEIPT");
	CESL_DataMgr* pTotalDM = ( CESL_DataMgr* ) FindDM( sTotalReceiptDMAlias );
	pTotalDM->FreeData();

	initData( );

	return 0;
}

VOID CBO_LOC_RECEIPT_PRINT_DLG::RECEIPT_CUT()
{
	LP_PrintCut( );
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-28 ~ 2005-12-28
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   함수설명   ] 
      로그인 사용자의 관리구분을 구한다.

   [  PARAMETERS  ]
      sGetManageCode : [OUT] 관리구분

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패
--------------------------------------------------------------------------------------------*/
INT CBO_LOC_RECEIPT_PRINT_DLG::GetLoginerManageCode(CString &sGetManageCode)
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_RECEIPT");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
    if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginManageCode()") );

	CString sOneValueQuery;
//	sOneValueQuery.Format( _T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK );
//	ids = pDM->GetOneValueQuery( sOneValueQuery, sGetManageCode );
// 	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginManageCode()") );

	sGetManageCode = m_pInfo->MANAGE_CODE;
	sGetManageCode.TrimLeft();sGetManageCode.TrimRight();

	// 관리구분이 없다면 도서관 관리구분정보를 입력한다.
	// 입력되는 도서관 관리구분코드는 디폴트로 선정된 값이다.
	if ( sGetManageCode.IsEmpty() )
	{
		CString sCodeName = _T("관리구분");
				
		ids = m_pInfo->pCodeMgr->GetDefaultCode( sCodeName,  sGetManageCode);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_RECEIPT_PRINT_DLG::GetLoginerManageCode()") );		
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   스타일정의 XML 파일을 로드한다.
/////////////////////////////////////////////////////////////////////////////
INT CBO_LOC_RECEIPT_PRINT_DLG::InitXML_Style(CString strFileName/*=_T("")*/)
{
	CoInitialize(NULL);
	IXMLDOMDocumentPtr pDom(_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	pDom->async = FALSE;
	if( strFileName.IsEmpty() )
	{
		strFileName = _T("..\\Cfg\\LoanReturn\\ReceiptStyle.xml");
	}

	CComVariant varFileName = (LPCTSTR)strFileName;
	if (!pDom->load(varFileName))
	{
		AfxMessageBox( _T("NOT FOUND XML Style File!!") );
		return -1;
	}
	
	/// 멤버 변수 선언시 초기화 아 스킬이 딸린다. ㅡ.ㅜ
	m_pStyleDom = pDom;
	
	// 문자열 형태의 형식을 만들자.
//	if ( MakeStringStyle() < 0 )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::InitXML_Style()") );			

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringStyle(CString& strValue)
{
	INT ids;	
	// 노드의 리스트
	IXMLDOMNodeListPtr StyleList = m_pStyleDom->childNodes;

	for ( LONG i = 0;i < StyleList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = StyleList->item[i];
		ids = MakeStringNode( pObjectNode, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringStyle()") );
		
		pObjectNode.Release();
	}

	StyleList.Release();

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode(IXMLDOMNodePtr pDom, CString &strValue)
{
	INT ids;	
	CComBSTR nameString;

	pDom->get_baseName(&nameString);
	
	if ( CString( nameString ) == _T("ITEM") )
	{
		ids = MakeStringItem( pDom, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode()") );
	}	
	else
	{
		/// 노드가 있다면 재귀호출이다!
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		for (LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = MakeStringNode( pObjectNode, strValue );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::MakeStringNode()") );

			pObjectNode.Release();
		}

		pNodeList.Release();
	}	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::MakeStringItem(IXMLDOMNodePtr pItemDom, CString &strValue)
{
	CString strForward;
	CString strData;
	CString strBackward;
	CString strItem;
	// 실제 ITEM 스트링을 만들자.
	IXMLDOMNodeListPtr pItemList = pItemDom->childNodes;

	for ( LONG i = 0;i < pItemList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = pItemList->item[i];
		
		CComBSTR text;
		CComBSTR nameString;

		pObjectNode->get_baseName(&nameString);
		pObjectNode->get_text(&text);

		CString strNameString(nameString);
		CString strText(text);	
		
		// Forward, Backward는  [] 를 <>로 Replace;
		strText.TrimLeft( );
		strText.TrimRight( );

		if ( strNameString == _T("FORWARD") )
		{
			strText.Replace( _T("["), _T("<") );
			strText.Replace( _T("]"), _T(">") );
			strForward = strText;
		}
		else if ( strNameString == _T("VALUE") )	strData = strText;
		else if ( strNameString == _T("BACKWARD") )
		{
			strText.Replace( _T("["), _T("<") );
			strText.Replace( _T("]"), _T(">") );
			strBackward = strText;
		}

		nameString.Empty();
		text.Empty();		

		pObjectNode.Release();
	}

	pItemList.Release();
	
	strItem = strForward + strData + strBackward;
	strValue += strItem;
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData(CString strTagetData, CString strValue)
{
	INT ids;	
	// 노드의 리스트
	IXMLDOMNodeListPtr PrintList = m_pStyleDom->childNodes;

	for ( LONG i = 0;i < PrintList->length;i++ )
	{
		IXMLDOMNodePtr pObjectNode = PrintList->item[i];
		ids = ChangePrintDomData( pObjectNode, strTagetData, strValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData()") );
	
		pObjectNode.Release();
	}

	PrintList.Release();

	return 0;	
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData(IXMLDOMNodePtr pDom, CString strTagetData, CString strValue)
{
	INT ids;	
	CComBSTR nameString;	
	pDom->get_baseName(&nameString);	

	if ( CString( nameString ) == _T("ITEM") )
	{
		// 데이터만 바꾸면 된다.
		CComBSTR text;

		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		
		for ( LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];

			pObjectNode->get_baseName(&nameString);			
			pObjectNode->get_text(&text);
				
			// 값을 변경하여 put 하자
			if ( CString( nameString ) == _T("VALUE") )
			{
				CString VALUE( text );
				VALUE.Replace( strTagetData, strValue );

				// 이번 노드에 집어 넣으면 된다
				pObjectNode->put_text( VALUE.AllocSysString() );			
			}
	
				pObjectNode.Release();
			}

		pNodeList.Release();		
	}	
	else
	{
		/// 노드가 있다면 재귀호출이다!
		IXMLDOMNodeListPtr pNodeList = pDom->childNodes;
		for (LONG i = 0; i < pNodeList->length; i++)
		{
			IXMLDOMNodePtr pObjectNode = pNodeList->item[i];
			ids = ChangePrintDomData( pObjectNode, strTagetData, strValue );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_RECEIPT_PRINT_DLG::ChangePrintDomData()") );

			pObjectNode.Release();
		}

		pNodeList.Release();
	}
	

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ChangeReceiptStrTOXML(CString &strXMLReceipt)
{
	/*
	// 메모리상의 XML을 파일로 저장하고 
	m_pStyleDom->save((LPCTSTR)CString(_T("..\\cfg\\LoanReturn\\Temp.xml")));
	// 저장된 파일을 열어서 문자열로 넘긴다.
	CStdioFile file;
	if ( !file.Open(_T("..\\cfg\\LoanReturn\\Temp.xml"), CFile::modeRead | CFile::typeBinary ) )
	{
		AfxMessageBox ( _T("NOT FOUND STYLE FILE!!") );
		return -1;
	}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
		
	strXMLReceipt.Empty();
	CString strLine;

	while ( file.ReadString( strLine ) )
	{
		strXMLReceipt += strLine;	
	}
	*/

	strXMLReceipt.Empty();

	BSTR bText = m_pStyleDom->Getxml();
	if( bText == NULL )
	{
		strXMLReceipt = _T("");
	}
	else
	{
		strXMLReceipt = CString(bText);
	}
   
	return 0;
}

// 2007.05.08 add by pdw 
// 반납처리시 예약정보 출력
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintReservationInfo(CString strDMAlias, INT nRowIndex, BOOL bPreview)
{
	INT		ids;

	//++2008.12.10 ADD BY KSJ {{++
	INT		nPrintIndex		=	0;	//영수증 출력 할 자료의 Index
	INT		nSelectIndex	=	0;	//예약자Grid에서 선택한 Index
	//--2008.12.10 ADD BY KSJ --}}

	// 1. init xml
	CString strFile;
	strFile = _T("..\\Cfg\\LoanReturn\\ReservationStyle.xml");
	ids = InitXML_Style(strFile);
	if( 0>ids ) return ids;

	// 2. make print string
	// 2.1 book info
	CESL_DataMgr* pLoanInfo;
	CString strCallNo, strTitle, strRegNo;
	CString strUserNo, strUserName, strHP, strPhoneHome;		
	CString strReservationDate, strReservationExpireDate;
	
	if ( strDMAlias != _T("") ) // 우선대출예약(DM_BO_LOC_1170)
	{
		pLoanInfo = FindDM( strDMAlias );
		if( !pLoanInfo ) return -101;

		pLoanInfo->GetCellData( _T("청구기호"), nRowIndex, strCallNo );
		pLoanInfo->GetCellData( _T("서명"), nRowIndex, strTitle );
		pLoanInfo->GetCellData( _T("등록번호"), nRowIndex, strRegNo );
		// 2.2 user info		
		pLoanInfo->GetCellData( _T("대출자번호"), nRowIndex, strUserNo );
		//++2008.06.02 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( strUserNo );
		//--2008.06.02 ADD BY PWR --}}
		pLoanInfo->GetCellData( _T("예약자명"), nRowIndex, strUserName );

		//JOB.2020.0010 EDIT BY KYJ 예약자명 다 표시하도록 주석처리
// 		//++2009.01.19 ADD BY PWR {{++
// 		// 대출자번호를 체크해서 주민번호일경우 대출자번호와 이름 끝자리를 암호화한다.
// 		if(!strUserName.IsEmpty())
//  		{
// 			strUserName.Format(_T("%s*"),strUserName.Left(strUserName.GetLength()-1));
// 		}
// 		//++2009.01.19 ADD BY PWR --}}

		pLoanInfo->GetCellData( _T("핸드폰"), nRowIndex, strHP );
		pLoanInfo->GetCellData( _T("집전화번호"), nRowIndex, strPhoneHome );
		// 2.3 loan info		
		pLoanInfo->GetCellData( _T("예약일"), nRowIndex, strReservationDate );
		pLoanInfo->GetCellData( _T("예약만기일"), nRowIndex, strReservationExpireDate );
	}
	else
	{
		//++2008.12.10 UPDATE BY KSJ {{++
		// 한줄서기 영수증출력 기능 추가
		if( !(m_pLMV->m_bVolReserve) )
		{
			CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_BOOK_INFO"));
			if( !pBookInfo )	return	-101;

			pBookInfo->GetCellData( _T("본표제"),	nPrintIndex, strTitle );
			pBookInfo->GetCellData( _T("청구기호"),	nPrintIndex, strCallNo );
			pBookInfo->GetCellData( _T("등록번호"),	nPrintIndex, strRegNo );

			pLoanInfo = FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
			if( !pLoanInfo )	return	-102;
		}
		else	//한줄서기 사용 할 경우
		{
			////////////////////////////////////////////////////////////////////////
			//1.예약자 정보 DM
			pLoanInfo = FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") );
			if( !pLoanInfo )	return	-102;
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//2.예약자료 정보 DM
			CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO"));
			if( !pBookInfo )	return	-101;
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//3.예약자 지정 그리드에서 선택된 예약자 찾기     
			CESL_Grid*	pVRReserveGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
			if( pVRReserveGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintReservationInfo") );

			nSelectIndex	=	pVRReserveGrid->GetIdx();
			////////////////////////////////////////////////////////////////////////
			
			////////////////////////////////////////////////////////////////////////
			//4.예약지정 된 자료의 등록번호 가져오기
			CString	strMarkRegNo;
			pLoanInfo->GetCellData( _T("등록번호")	, nSelectIndex , strMarkRegNo );
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//5.예약지정이 된 자료 찾기
			CESL_Grid*	pVRBookInfoGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRBookInfoGrid") );
			if( pVRReserveGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintReservationInfo") );

			INT		nVRBookInfoGridCnt	=	pVRBookInfoGrid->GetCount();	//예약자료의 갯수
			CString	strTempReg;		//REG_NO 임시 저장 변수

			for( INT i=0 ; i<nVRBookInfoGridCnt; i++ )
			{
				pBookInfo->GetCellData( _T("등록번호")	, i , strTempReg );
				if(	strMarkRegNo	==	strTempReg)
				{
					nPrintIndex	=	i;
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			//6.예약지정이 된 자료 정보 가져오기
			pBookInfo->GetCellData( _T("서명")		, nPrintIndex , strTitle );
			pBookInfo->GetCellData( _T("청구기호")	, nPrintIndex , strCallNo);
			pBookInfo->GetCellData( _T("등록번호")	, nPrintIndex , strRegNo );
			////////////////////////////////////////////////////////////////////////
		}
		/*
		CESL_DataMgr* pBookInfo = FindDM( _T("DM_BO_LOC_3170_BOOK_INFO"));
		if( !pBookInfo ) return -101;

		pLoanInfo = FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
		if( !pLoanInfo ) return -102;

		pBookInfo->GetCellData( _T("청구기호"), 0, strCallNo );
		pBookInfo->GetCellData( _T("본표제"), 0, strTitle );
		pBookInfo->GetCellData( _T("등록번호"), 0, strRegNo );
		*/
		//--2008.12.10 UPDATE BY KSJ --}}
		// 길이제한
		// 서명 
		strTitle = getLimitWord( strTitle.GetBuffer(0), 24, TRUE );
		// 청구기호
		strCallNo = getLimitWord( strCallNo.GetBuffer(0), 24, TRUE );
		
		// 2.2 user info		
		pLoanInfo->GetCellData( _T("대출자번호"),	nSelectIndex, strUserNo );
		//++2008.06.02 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( strUserNo );
		//--2008.06.02 ADD BY PWR --}}
		pLoanInfo->GetCellData( _T("대출자"),		nSelectIndex, strUserName );
		//JOB.2020.0010 EDIT BY KYJ 예약자명 다 표시하도록 주석처리
// 		//++2009.01.19 ADD BY PWR {{++
// 		// 대출자번호를 체크해서 주민번호일경우 대출자번호와 이름 끝자리를 암호화한다.
// 		if(!strUserName.IsEmpty())
//  		{
// 			strUserName.Format(_T("%s*"),strUserName.Left(strUserName.GetLength()-1));
// 		}
// 		//++2009.01.19 ADD BY PWR --}}

		pLoanInfo->GetCellData( _T("핸드폰"),		nSelectIndex, strHP );
		pLoanInfo->GetCellData( _T("전화번호"),		nSelectIndex, strPhoneHome );
		// 2.3 loan info
		pLoanInfo->GetCellData( _T("예약일"),		nSelectIndex, strReservationDate );
		pLoanInfo->GetCellData( _T("예약만기일"),	nSelectIndex, strReservationExpireDate );
	}

	// 2.4 print date
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString f_strPrintDate;
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	f_strPrintDate = strGetData.Left(16);
// 	CString f_strPrintDate;
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	f_strPrintDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	// 인쇄용 스트링 생성
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;

	// 2009.01.22 ADD BY PWR : SMS발송일 셋팅
	// SMS사용여부에 따른 처리(사용안함이면 'SMS발송일'을 보여주지않는다.)
	// 우선대출예약에서도 보여주지않는다.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	if( nIsSMS == 0 || strDMAlias == _T("DM_BO_LOC_1170") ) 
	{ // SMS사용안함
		strRCT.Replace(_T("● SMS 발송일 : %SMS발송일%<!LN><!FP>"), _T(""));
	}
	else
	{ // SMS사용함
		CString sDate, sQuery, sLoanKey;
		pLoanInfo->GetCellData(_T("대출/반납정보KEY"), nSelectIndex, sLoanKey);
		// 17/11/01 김혜영 : LMS추가로 인한 처리 - 예약자료명세표 출력시 LMS발송날짜 표시 요청
/*//BEFORE-----------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_TBL WHERE LOAN_KEY=%s"), sLoanKey);	
*///AFTER------------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_VIEW WHERE LOAN_KEY=%s"), sLoanKey);	
/*//END-------------------------------------------------------------------------------*/		
		pLoanInfo->GetOneValueQuery(sQuery, sDate);
		if(sDate.IsEmpty()) 
		{ // SMS발송이력에 LS_WORK_T01의 해당 예약이력의 키로 저장된 이력이 없다면 발송이력이 없다는것 
			strRCT.Replace(_T("● SMS 발송일 : %SMS발송일%<!LN><!FP>"), _T(""));
		}
		else
		{ // 이력이 확인되었다 발송일을 보여준다.
			strRCT.Replace( _T("%SMS발송일%"), sDate );
			ChangePrintDomData( _T("%SMS발송일%"), sDate );
		}
	}

	//JOB.2020.0030 ADD BY KYJ : 개인정보 보호 강화 
	if( m_pLMV->m_sLoanUserAccessMode.IsEmpty() || m_pLMV->m_sLoanUserAccessMode == _T("1") ) strUserName = _T("***");
	
	// 데이터 입력
	strRCT.Replace( _T("%출력일시%"), f_strPrintDate );
	ChangePrintDomData( _T("%출력일시%"), f_strPrintDate );	
	strRCT.Replace( _T("%예약자번호%"), strUserNo );
	ChangePrintDomData( _T("%예약자번호%"), strUserNo );	
	strRCT.Replace( _T("%예약자이름%"), strUserName );
	ChangePrintDomData( _T("%예약자이름%"), strUserName );
	strRCT.Replace( _T("%집전화%"), strPhoneHome );
	ChangePrintDomData( _T("%집전화%"), strPhoneHome );
	strRCT.Replace( _T("%핸드폰%"), strHP );
	ChangePrintDomData( _T("%핸드폰%"), strHP );

	strRCT.Replace( _T("%청구기호%"), strCallNo );
	ChangePrintDomData( _T("%청구기호%"), strCallNo );
	strRCT.Replace( _T("%서명%"), strTitle );
	ChangePrintDomData( _T("%서명%"), strTitle );
	strRCT.Replace( _T("%등록번호%"), strRegNo );
	ChangePrintDomData( _T("%등록번호%"), strRegNo );

	strRCT.Replace( _T("%예약일%"), strReservationDate );
	ChangePrintDomData( _T("%예약일%"), strReservationDate );
	strRCT.Replace( _T("%예약만기일%"), strReservationExpireDate );
	ChangePrintDomData( _T("%예약만기일%"), strReservationExpireDate );

	// 2. 도서관 이름 설정
	if( m_strLibCode.IsEmpty() )
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pLoanInfo->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	strRCT.Replace( _T("%도서관명%"), m_strLibCode );
	ChangePrintDomData( _T("%도서관명%"), m_strLibCode );	

	if ( strDMAlias != _T("") )
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if ( bPreview )
		{
			if ( dlgPreview.DoModal() == IDOK )
			{
				ids = printReceipt( strRCT );
				if ( ids < 0 ) return -5;
				
				// 컷팅~
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}	
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids < 0 ) return -5;
			
			// 컷팅~
			LP_StartUp();
			LP_PrintCut( );	
			LP_CleanUp();		
		}
		
	}
	else
	{
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		if ( dlgPreview.DoModal() == IDOK )
		{
			ids = printReceipt( strRCT );
			if ( ids < 0 ) return -5;
			
			// 컷팅~
			LP_StartUp();
			LP_PrintCut( );	
			LP_CleanUp();
		}

	}
	
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetLPStartUp()
{
	INT ids;
	while(TRUE)
	{
		// StartUp
		ids = LP_StartUp();
		if(0 == ids)
		{
			break;
		}
		else
		{
			ids = MessageBox(_T("발견된 출력기가 없습니다.\r\n출력을 재시도 하시겠습니까?"), _T("영수증출력"), MB_YESNO | MB_ICONQUESTION);
			if(ids != IDYES)
			{
				return -1;
			}
		}
	}
	while(TRUE)
	{
		// GetStatus
		ids = LP_GetStatus();
		if(0 == ids)
		{
			break;
		}
		else
		{
			ids = MessageBox(_T("출력기의 전원이 꺼져있거나 연결이 되어 있지 않습니다.\r\n출력을 재시도 하시겠습니까?"), _T("영수증출력"), MB_YESNO | MB_ICONQUESTION);
			if(ids != IDYES)
			{
				return -2;
			}
		}
	}
	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::GetReservationProcPrint(CESL_DataMgr *pDM, INT nIndex, CString &strPrintData)
{
	INT ids;
 	ids = InitXML_Style(_T("..\\Cfg\\LoanReturn\\ReservationStyle2.xml"));
 	if(0 > ids) 
 	{
 		return ids;
 	}
 
 	CString strCallNo, strTitle, strRegNo, strUserNo, strUserName;		
 	CString strReservationDate, strReservationExpireDate;
 	pDM->GetCellData(_T("서명"), nIndex, strTitle);
 	strTitle = getLimitWord(strTitle.GetBuffer(0), 24, TRUE);
 	pDM->GetCellData(_T("청구기호"), nIndex, strCallNo);
 	strCallNo = getLimitWord(strCallNo.GetBuffer(0), 24, TRUE);
 	pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
 	pDM->GetCellData(_T("대출자번호표시"), nIndex, strUserNo);
 	pDM->GetCellData(_T("이름"), nIndex, strUserName);
 	pDM->GetCellData(_T("예약일"), nIndex, strReservationDate);
 	pDM->GetCellData(_T("예약만기일"), nIndex, strReservationExpireDate);
 	
 	CString strRCT;
 	ids = MakeStringStyle(strRCT);
 	if(0 > ids)
 	{
 		return ids;
 	}
 	strRCT.Replace(_T("%예약자번호%"), strUserNo);
 	ChangePrintDomData(_T("%예약자번호%"), strUserNo);	
 	strRCT.Replace(_T("%예약자이름%"), strUserName);
 	ChangePrintDomData(_T("%예약자이름%"), strUserName);
 	strRCT.Replace(_T("%청구기호%"), strCallNo);
 	ChangePrintDomData(_T("%청구기호%"), strCallNo);
 	strRCT.Replace(_T("%서명%"), strTitle);
 	ChangePrintDomData(_T("%서명%"), strTitle);
 	strRCT.Replace(_T("%등록번호%"), strRegNo);
 	ChangePrintDomData(_T("%등록번호%"), strRegNo);
 	strRCT.Replace(_T("%예약일%"), strReservationDate);
 	ChangePrintDomData(_T("%예약일%"), strReservationDate);
	// SMS발송일 표시
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	if(nIsSMS == 0) 
	{ // SMS사용안함
		strRCT.Replace(_T("● SMS 발송일 : %SMS발송일%<!LN><!FP>"), _T(""));
	}
	else
	{ // SMS사용함
		CString sDate, sQuery, sLoanKey;
		pDM->GetCellData(_T("LOAN_KEY"), nIndex, sLoanKey);

		// 17/11/01 김혜영 : LMS추가로 인한 처리 - 예약자료명세표 출력시 LMS발송날짜 표시 요청
/*//BEFORE-----------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_TBL WHERE LOAN_KEY=%s"), sLoanKey);	
*///AFTER------------------------------------------------------------------------------
		sQuery.Format(_T("SELECT TO_CHAR(MAX(SEND_TIME),'YYYY/MM/DD HH24:MI') FROM MN_SMS_LOG_VIEW WHERE LOAN_KEY=%s"), sLoanKey);
/*//END-------------------------------------------------------------------------------*/
		pDM->GetOneValueQuery(sQuery, sDate);
	
		if(sDate.IsEmpty()) 
		{ // SMS발송이력에 LS_WORK_T01의 해당 예약이력의 키로 저장된 이력이 없다면 발송이력이 없다는것 
			strRCT.Replace(_T("● SMS 발송일 : %SMS발송일%<!LN><!FP>"), _T(""));
		}
		else
		{ // 이력이 확인되었다 발송일을 보여준다.
			strRCT.Replace( _T("%SMS발송일%"), sDate );
			ChangePrintDomData( _T("%SMS발송일%"), sDate );
		}
	}
// 	strRCT.Replace(_T("● SMS 발송일 : %SMS발송일%<!LN><!FP>"), _T(""));
 	strRCT.Replace(_T("%예약만기일%"), strReservationExpireDate);
 	ChangePrintDomData(_T("%예약만기일%"), strReservationExpireDate);
 	if(m_strLibCode.IsEmpty())
 	{
 		CString	strQuery;
 		strQuery.Format(_T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"));
 		pDM->GetOneValueQuery(strQuery, m_strLibCode);
 	}
 	strRCT.Replace(_T("%도서관명%"), m_strLibCode);
 	ChangePrintDomData(_T("%도서관명%"), m_strLibCode);	
 
 	strPrintData = strRCT;
 	
 	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ReservationObjectPrint(CESL_Grid* pGrid, CESL_DataMgr *pDM)
{
	if(NULL == pDM)
	{
		return -1;
	}
	if(NULL == pGrid)
	{
		return -2;
	}
	pGrid->SelectGetHeadPosition();

	INT nIndex;
	CString strReceiptString;
	strReceiptString = _T("<!FC><!FF>[예약대상자료]<!LF><!LC><!FP><!FP>");
	for(INT i = 0; i < pGrid->SelectGetCount(); i++)
	{
		nIndex = pGrid->SelectGetIdx();

		CString strTitle;
		pDM->GetCellData(_T("서명"), nIndex, strTitle);
		strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
		CString strCallNo;
		pDM->GetCellData(_T("청구기호"), nIndex, strCallNo);
		CString strRegNo;
		pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
		CString strReservationDate;
		pDM->GetCellData(_T("예약일"), nIndex, strReservationDate);

		CString strData;
		strData.Format(
			_T("<!FC>------------------------------------------<!LC><!FP>")
			_T("<!FN>%d. 서명 : %s<!LN><!FP>")
			_T("<!FN>   등록번호 : %s<!LN><!FP>")
			_T("<!FN>   청구기호 : %s<!LN><!FP>")
			_T("<!FN>   예약일 : %s<!LN><!FP>")
			, i+1, strTitle, strRegNo, strCallNo, strReservationDate);

		strReceiptString += strData;

		pGrid->SelectGetNext();
	}

	printReceipt(strReceiptString);

	return 0;
}

INT CBO_LOC_RECEIPT_PRINT_DLG::ReservationShelfPrint(CESL_Grid* pGrid, CESL_DataMgr *pDM)
{
	if(NULL == pDM)
	{
		return -1;
	}
	if(NULL == pGrid)
	{
		return -2;
	}
	pGrid->SelectGetHeadPosition();

	INT nIndex;
	CString strReceiptString;
	strReceiptString = _T("<!FC><!FF>[예약취소자료]<!LF><!LC><!FP><!FP>");
	for(INT i = 0; i < pGrid->SelectGetCount(); i++)
	{
		nIndex = pGrid->SelectGetIdx();

		CString strTitle;
		pDM->GetCellData(_T("서명"), nIndex, strTitle);
		strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
		CString strCancelClass;
		pDM->GetCellData(_T("취소종류"), nIndex, strCancelClass);
		CString strRegNo;
		pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
		CString strReservationExpireDate;
		pDM->GetCellData(_T("예약만기일"), nIndex, strReservationExpireDate);
		CString strCallNo;
		pDM->GetCellData(_T("청구기호"), nIndex, strCallNo);
		CString strReservationObject;
		pDM->GetCellData(_T("예약대상"), nIndex, strReservationObject);
		if(!strReservationObject.IsEmpty())
		{
			strReservationObject = _T("[예약대상]");
		}

		CString strData;
		strData.Format(
			_T("<!FC>------------------------------------------<!LC><!FP>")
			_T("<!FN>%d. 서명 : %s<!LN><!FP>")
			_T("<!FN>   청구기호 : %s<!LN><!FP>")
			_T("<!FN>   등록번호 : %s   [%s]<!LN><!FP>")
			_T("<!FN>   예약만기일 : %s   %s<!LN><!FP>")
			, i+1, strTitle, strCallNo, strRegNo, strCancelClass, strReservationExpireDate, strReservationObject);

		strReceiptString += strData;

		pGrid->SelectGetNext();
	}

	printReceipt(strReceiptString);

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<수수료관리시스템>>
// **연체료 영수증출력
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintFeeInfo(BOOL bPreview, BOOL bPrint, INT nIndex)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeStyle.xml") );

    // 1. 데이터 구축===================================================================================
	CESL_DataMgr *pUserDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_USER") );
	CESL_DataMgr *pLoanDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_LOAN") );

	CString sQuery, sNumber, sUserNo, sName, sDate, sSetDate, sLoanPossibleDate, sPaymentDate, sTempInfo;
	CString sTotalBook, sTotalMoney, sLib, sTotalDelayDay, sTotalApplyDelayday, sLoanStopDate, sBasicArrear;
	CString sBlank = _T("                   ");
	
	//++2009.01.02 ADD BY KSJ {{++
	//Where 조건에 MAKE_YEAR 추가
	//CTime		tCurrentCTime = CLocCommonAPI::GetDBTime(m_pParentMgr);
	//CString		strCurrentYear;
	//strCurrentYear.Format( _T("%d") , tCurrentCTime.GetYear());
	//--2009.01.02 ADD BY KSJ --}}

	//일련번호SELECT
	//sQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE AND MAKE_YEAR='%s'") , strCurrentYear );
// 	sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE");
	sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	ids = pUserDM->GetOneValueQuery( sQuery, sNumber );
	if ( ids < 0 ) 
	{
		//++2009.01.02 UPDATE BY KSJ {{++
		//레코드가 없을 경우, 레코드 생성 후 처리한다
		if( -4007 == ids )
		{
			pUserDM->StartFrame();
			pUserDM->InitDBFieldData();
			
			ids = pUserDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );
			
			//ids = pUserDM->AddDBFieldData( _T("MAKE_YEAR"),		_T("STRING"),	strCurrentYear );
			//if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("영수증_일련번호") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pUserDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			pUserDM->SendFrame();
			pUserDM->EndFrame();
		}
		else
		{
			return -1;
		}
			//--2009.01.02 UPDATE BY KSJ --}}
	}
	if ( ids < 0 ) return -1;

	//대출자번호
	pUserDM->GetCellData( _T("대출자번호"), 0, sUserNo);
	//++2009.01.19 ADD BY PWR {{++
	// 대출자번호 주민번호일때 *표시 
	ChangeSecuriyCivilNo( sUserNo );
	//--2009.01.19 ADD BY PWR --}}

	//대출자이름
	pUserDM->GetCellData( _T("성명"), nIndex, sName);
	//++2008.12.02 ADD BY PJW {{++
	// 대출자번호를 체크해서 주민번호일경우 대출자번호와 이름 끝자리를 암호화한다.
	if(!sName.IsEmpty())
 	{
		sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
	}
	//++2008.12.02 ADD BY PJW --}}
	
	
	pUserDM->GetCellData( _T("납부일"), nIndex, sPaymentDate);
	//처리일시
	sSetDate = sPaymentDate;
	//납부일
	sPaymentDate = sPaymentDate.Left(10);
	//영수증출력일자(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );
	
	//서지정보
	setRctTemp(sTempInfo);
	//총권
	sTotalBook.Format( _T("%d"), pLoanDM->GetRowCount() );
	//총연체일
	//sTotalDelayDay
	pUserDM->GetCellData( _T("총연체일"), nIndex, sTotalDelayDay );
	sTotalDelayDay.Format( _T("%d"), _ttoi(sTotalDelayDay) );
	//적용연체일
	//sTotalApplyDelayday
	pUserDM->GetCellData( _T("적용연체일"), nIndex, sTotalApplyDelayday );
	sTotalApplyDelayday.Format( _T("%d"), _ttoi(sTotalApplyDelayday) );
	//총계
	pUserDM->GetCellData( _T("누적납부금"), nIndex, sTotalMoney );
	SetMoneyFormat(sTotalMoney);
	//기본연체료
	pLoanDM->GetCellData( _T("기본연체료"), 0, sBasicArrear );
	//대출정지일
	//sLoanStopDate
	pUserDM->GetCellData( _T("대출정지일"), nIndex, sLoanStopDate );
	//대출가능일자
	if(sLoanStopDate==_T(""))
	{
		sLoanPossibleDate = sPaymentDate;
	}
	else
	{
		COleDateTimeSpan tOneDay( 1, 0 , 0 , 0 );	// 하루
		COleDateTime tLoanPossibleDate( _ttoi(sLoanStopDate.Mid(0,4)) , _ttoi(sLoanStopDate.Mid(5,2)) , _ttoi(sLoanStopDate.Mid(8,2)) , 0 , 0 , 0 );
		tLoanPossibleDate = tLoanPossibleDate + tOneDay;
		sLoanPossibleDate = tLoanPossibleDate.Format( _T("%Y/%m/%d") );
	}
	//도서관
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pUserDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}

    // 2. 문자열 치환 ================================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//일련번호
	sNumber.Format( _T("%010d"), ( _ttoi(sNumber) + 1 ) );
	strRCT.Replace( _T("%일련번호%"), sNumber );
	ChangePrintDomData( _T("%일련번호%"), sNumber );	
	//대출자번호
	strRCT.Replace( _T("%대출자번호%"), sUserNo );
	ChangePrintDomData( _T("%대출자번호%"), sUserNo );
	//대출자이름
	strRCT.Replace( _T("%대출자이름%"), sName );
	ChangePrintDomData( _T("%대출자이름%"), sName);
	//납부일
	strRCT.Replace( _T("%납부일%"), sPaymentDate );
	ChangePrintDomData( _T("%납부일%"), sPaymentDate);
	//출력일시
	strRCT.Replace( _T("%출력일시%"), sDate );
	ChangePrintDomData( _T("%출력일시%"), sDate);	
	//서지정보
	strRCT.Replace( _T("%서지정보%"), sTempInfo );
	ChangePrintDomData( _T("%서지정보%"), sTempInfo );	
	//총권
	strRCT.Replace( _T("%총권%"), sTotalBook );
	ChangePrintDomData( _T("%총권%"), sTotalBook );
	//총연체일
	strRCT.Replace( _T("%총연체일%"), sTotalDelayDay );
	ChangePrintDomData( _T("%총연체일%"), sTotalDelayDay );
	//적용연체일
	strRCT.Replace( _T("%적용연체일%"), sTotalApplyDelayday );
	ChangePrintDomData( _T("%적용연체일%"), sTotalApplyDelayday );
	//총계
	strRCT.Replace( _T("%총계%"), sTotalMoney );
	ChangePrintDomData( _T("%총계%"), sTotalMoney );
	//대출정지일
	strRCT.Replace( _T("%대출정지일%"), sLoanStopDate );
	ChangePrintDomData( _T("%대출정지일%"), sLoanStopDate );
	//추가내용
	strRCT.Replace( _T("%추가내용%"), _T("") );
	ChangePrintDomData( _T("%추가내용%"), _T("") );
	//도서관명
	strRCT.Replace( _T("%도서관명%"), m_strLibCode );
	ChangePrintDomData( _T("%도서관명%"), m_strLibCode );
	//공간
	strRCT.Replace( _T("%공간%"), sBlank );
	ChangePrintDomData( _T("%공간%"), sBlank );
	//처리일시
	strRCT.Replace( _T("%처리일시%"), sSetDate );
	ChangePrintDomData( _T("%처리일시%"), sSetDate );
	//기본연체료
	strRCT.Replace( _T("%기본연체료%"), sBasicArrear);
	ChangePrintDomData( _T("%기본연체료%"), sBasicArrear );
	//대출가능일
	strRCT.Replace( _T("%대출가능일%"), sLoanPossibleDate );
	ChangePrintDomData( _T("%대출가능일%"), sLoanPossibleDate );

	if( bPrint==TRUE )
	{
		// 3. 출력 =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(m_pParentMgr);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if( bPreview )
		{
			if( ( dlgPreview.DoModal() ) == IDOK )
			{
				//미리보기 화면에서 출력
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//미리보기 화면에서 닫기 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}

	//일련번호UPDATE
// 	sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE;")
// 					, _ttoi( sNumber ) );
	sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s';")
					, _ttoi( sNumber ), m_pInfo->MANAGE_CODE );
	pUserDM->StartFrame();
	pUserDM->InitDBFieldData();	
	pUserDM->AddFrame(sQuery);
	pUserDM->SendFrame(TRUE);	
	ids = pUserDM->EndFrame();
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<수수료관리시스템>>
// **회원증재발급 영수증출력
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintFeeReCardInfo(BOOL bPreview, BOOL bPrint)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeReCardStyle.xml") );

	// 1. 데이터 구축 =============================================================================
	CESL_DataMgr* pDM = FindDM( _T("DM_FEE_RECARD_RECEIPT") );
	CString sNumber, sUserNo, sName, sClass, sCard, sPayment, sRecKey, sDate;
	if ( pDM->GetRowCount() > 0 )
	{
		pDM->GetCellData( _T("일련번호")	, 0, sNumber);
		pDM->GetCellData( _T("회원번호")	, 0, sUserNo);
		pDM->GetCellData( _T("성명")		, 0, sName);
		pDM->GetCellData( _T("회원등급")	, 0, sClass);
		pDM->GetCellData( _T("카드종류")	, 0, sCard);
		pDM->GetCellData( _T("납부금액")	, 0, sPayment);
		pDM->GetCellData( _T("REC_KEY")		, 0, sRecKey);
		
		//++2009.01.19 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( sUserNo );
		//--2009.01.19 ADD BY PWR --}}

		//++2008.12.02 ADD BY PJW {{++
		// 대출자번호를 체크해서 이름 끝자리를 암호화한다.
		if(!sName.IsEmpty())
 		{
			sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
		}
		//++2008.12.02 ADD BY PJW --}}
	}
	//도서관
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");	
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);	
		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	//영수증출력일자(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );

	// 2. 문자열 치환 =============================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//일련번호
	sNumber.Format( _T("%010d"),  _ttoi(sNumber) );
	strRCT.Replace( _T("%일련번호%"), sNumber );
	ChangePrintDomData( _T("%일련번호%"), sNumber );	
	//대출자번호
	strRCT.Replace( _T("%대출자번호%"), sUserNo );
	ChangePrintDomData( _T("%대출자번호%"), sUserNo );
	//대출자이름
	strRCT.Replace( _T("%대출자이름%"), sName );
	ChangePrintDomData( _T("%대출자이름%"), sName);
	//회원등급
	strRCT.Replace( _T("%회원등급%"), sClass );
	ChangePrintDomData( _T("%회원등급%"), sClass);
	//카드종류
	strRCT.Replace( _T("%카드종류%"), sCard );
	ChangePrintDomData( _T("%카드종류%"), sCard);	
	//납부금액
	strRCT.Replace( _T("%납부금액%"), sPayment );
	ChangePrintDomData( _T("%납부금액%"), sPayment );
	//출력일시
	strRCT.Replace( _T("%출력일시%"), sDate );
	ChangePrintDomData( _T("%출력일시%"), sDate );
	//도서관명
	strRCT.Replace( _T("%도서관명%"), m_strLibCode );
	ChangePrintDomData( _T("%도서관명%"), m_strLibCode );

	if( bPrint==TRUE )
	{
		// 3. 출력 =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlg(this);
		dlg.m_strDevice = m_strDevice;
		dlg.SetPrintData(strRCT);
		
		if( bPreview ) 
		{
			if( ( dlg.DoModal() ) == IDOK )
			{
				//미리보기 화면에서 출력
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//미리보기 화면에서 닫기 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}
	
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.13 ADD BY PWR
// <<수수료관리시스템>>
// **대출정지일해제 영수증출력
INT CBO_LOC_RECEIPT_PRINT_DLG::PrintLoanStopCancelInfo(BOOL bPreview, BOOL bPrint)
{
	INT ids;

	ids = InitXML_Style( _T("..\\Cfg\\LoanReturn\\FeeLoanStopCancelStyle.xml") );

	// 1. 데이터 구축 =============================================================================
	CESL_DataMgr* pDM = FindDM( _T("DM_FEE_LOANSTOPCANCEL_RECEIPT") );
	CString sNumber, sUserNo, sName, sPayment, sRecKey, sDate, sPaymentDate, sSetDate, sBasicArrear;
	CString sLoanStopDate, sStopDay, sHolyDay, sDissolveDay, sLoanStopPeriod;
	CString sSpace = _T("       ");
	CString sBlank = _T("                   ");

	if ( pDM->GetRowCount() > 0 )
	{
		pDM->GetCellData( _T("REC_KEY")		, 0, sRecKey		);
		pDM->GetCellData( _T("일련번호")	, 0, sNumber		);
		pDM->GetCellData( _T("회원번호")	, 0, sUserNo		);
		pDM->GetCellData( _T("성명")		, 0, sName			);
		pDM->GetCellData( _T("납부일")		, 0, sPaymentDate	);
		pDM->GetCellData( _T("대출정지일")	, 0, sLoanStopDate	);
		pDM->GetCellData( _T("정지일수")	, 0, sStopDay		);
		sStopDay.Format( _T("%d"), _ttoi( sStopDay ) );
		pDM->GetCellData( _T("휴관일수")	, 0, sHolyDay		);
		if( sHolyDay.Compare( _T("적용안함") ) != 0 ) 
			sHolyDay.Format( _T("%d"), _ttoi(sHolyDay) );
		pDM->GetCellData( _T("해제일수")	, 0, sDissolveDay	);
		sDissolveDay.Format( _T("%d"), _ttoi( sDissolveDay ) );
		pDM->GetCellData( _T("납부금액")	, 0, sPayment		);
		//납부금액 폭 맞추기
		SetMoneyFormat(sPayment);
		//++2009.01.19 ADD BY PWR {{++
		// 대출자번호 주민번호일때 *표시 
		ChangeSecuriyCivilNo( sUserNo );
		//--2009.01.19 ADD BY PWR --}}
		//++2008.12.02 ADD BY PJW {{++
		// 대출자번호를 체크해서 이름 끝자리를 암호화한다.
		if(!sName.IsEmpty())
 		{
			sName.Format(_T("%s*"),sName.Left(sName.GetLength()-1));
		}		
		//++2008.12.02 ADD BY PJW --}}
		//++2009.01.19 ADD BY PWR {{++
		// 대출정지기간 (납부일~대출정지일)
		sLoanStopPeriod.Format(_T("%s ∼ %s"), sPaymentDate, sLoanStopDate);
		//--2009.01.19 ADD BY PWR --}}
		pDM->GetCellData( _T("처리일시")	, 0, sSetDate		);
		pDM->GetCellData( _T("기본연체료")	, 0, sBasicArrear	);
	}
	//도서관
	if(m_strLibCode.IsEmpty())
	{
		CString	strWhere;
// 		strWhere = _T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		
		strWhere.Format(_T("SELECT MIN(LIB_NAME) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);		
		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}
	//영수증출력일자(SYSTEM)
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);
	sDate = strGetData.Left(16);
// 	CTime tTime = CLocCommonAPI::GetDBTime( this );
// 	sDate.Format( _T("%04d/%02d/%02d %02d:%02d"),	tTime.GetYear( ),
// 													tTime.GetMonth( ),
// 													tTime.GetDay( ),
// 													tTime.GetHour( ),
// 													tTime.GetMinute( ) );

	// 2. 문자열 치환 =============================================================================
	CString strRCT;
	ids = MakeStringStyle( strRCT );
	if( 0>ids ) return ids;
	
	//일련번호
	sNumber.Format( _T("%010d"), _ttoi(sNumber) );
	strRCT.Replace( _T("%일련번호%"), sNumber );
	ChangePrintDomData( _T("%일련번호%"), sNumber );	
	//대출자번호
	strRCT.Replace( _T("%대출자번호%"), sUserNo );
	ChangePrintDomData( _T("%대출자번호%"), sUserNo );
	//대출자이름
	strRCT.Replace( _T("%대출자이름%"), sName );
	ChangePrintDomData( _T("%대출자이름%"), sName );
	//납부일
	strRCT.Replace( _T("%납부일%"), sPaymentDate );
	ChangePrintDomData( _T("%납부일%"), sPaymentDate );	
	//대출정지일
	strRCT.Replace( _T("%대출정지일%"), sLoanStopDate );
	ChangePrintDomData( _T("%대출정지일%"), sLoanStopDate );
	//정지일수
	strRCT.Replace( _T("%정지일수%"), sStopDay );
	ChangePrintDomData( _T("%정지일수%"), sStopDay );
	//휴관일수
	strRCT.Replace( _T("%휴관일수%"), sHolyDay );
	ChangePrintDomData( _T("%휴관일수%"), sHolyDay );
	//해제일수
	strRCT.Replace( _T("%해제일수%"), sDissolveDay );
	ChangePrintDomData( _T("%해제일수%"), sDissolveDay );
	//납부금액
	strRCT.Replace( _T("%납부금액%"), sPayment );
	ChangePrintDomData( _T("%납부금액%"), sPayment );
	//출력일시
	strRCT.Replace( _T("%출력일시%"), sDate );
	ChangePrintDomData( _T("%출력일시%"), sDate );
	//도서관명
	strRCT.Replace( _T("%도서관명%"), m_strLibCode );
	ChangePrintDomData( _T("%도서관명%"), m_strLibCode );
	//공간
	strRCT.Replace( _T("%공간%"), sBlank );
	ChangePrintDomData( _T("%공간%"), sBlank );
	//++2009.01.19 ADD BY PWR {{++
	//대출정지기간
	strRCT.Replace( _T("%대출정지기간%"), sLoanStopPeriod );
	ChangePrintDomData( _T("%대출정지기간%"), sLoanStopPeriod );
	//--2009.01.19 ADD BY PWR --}}	
	strRCT.Replace( _T("%처리일시%"), sSetDate );
	ChangePrintDomData( _T("%처리일시%"), sSetDate );
	strRCT.Replace( _T("%기본연체료%"), sBasicArrear );
	ChangePrintDomData( _T("%기본연체료%"), sBasicArrear );

	if( bPrint==TRUE )
	{
		// 3. 출력 =====================================================================================
		CBO_LOC_RECEIPT_PREVIEW_DLG dlgPreview(this);
		dlgPreview.m_strDevice = m_strDevice;
		dlgPreview.SetPrintData(strRCT);
		
		if( bPreview )
		{
			if( ( dlgPreview.DoModal() ) == IDOK )
			{
				//미리보기 화면에서 출력
				ids = printReceipt( strRCT );
				if ( ids ) return -5;
				
				LP_StartUp();
				LP_PrintCut( );	
				LP_CleanUp();
			}
			else
			{
				//미리보기 화면에서 닫기 
				return 100;
			}
		}
		else
		{
			ids = printReceipt( strRCT );
			if ( ids ) return -5;
			
			LP_StartUp();
			LP_PrintCut();	
			LP_CleanUp();		
		}
	}
	m_strRCT = strRCT;

	return 0;
}

// 2002.02.01 ADD BY PWR
// <<수수료관리시스템>>
// **연체료 서지정보
INT CBO_LOC_RECEIPT_PRINT_DLG::setRctTemp(CString &sBookInfo)
{
	INT ids = 0;
	INT nTotalCnt = 0;

	CString sTmpData = _T("");    
	CString sTotalCnt = _T("");        // 대출책수
	CString sTmpBoInfo = _T("");

	CESL_DataMgr *pLoanDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_LOAN") );

	for ( int i = 0 ; i < pLoanDM->GetRowCount(); i++ )
	{
		sTmpBoInfo.Format( _T("%4d"), ++nTotalCnt );
		sBookInfo += _T("<!FN>") + sTmpBoInfo;

		ids = pLoanDM->GetCellData( _T("청구기호"), i, sTmpData );
		if ( ids < 0 ) return -6;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 24, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo;

		ids = pLoanDM->GetCellData( _T("등록번호"), i, sTmpData );
		if ( ids < 0 ) return -7;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, TRUE );
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP><!FN>");

		ids = pLoanDM->GetCellData( _T("서명"), i, sTmpData );
		if ( ids < 0 ) return -8;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 18, FALSE );

		ids = pLoanDM->GetCellData( _T("저작자"), i, sTmpData );
		if ( ids < 0 ) return -9;
		sTmpData = sTmpBoInfo + _T("/") + sTmpData;
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 28, TRUE );
		sBookInfo += " " + sTmpBoInfo;
		
		ids = pLoanDM->GetCellData( _T("납부금액"), i, sTmpData );
		if ( ids < 0 ) return -10;
		SetMoneyFormat(sTmpData);
		sTmpData += _T("원");
		sTmpBoInfo = getLimitWord( sTmpData.GetBuffer(0), 12, FALSE );
		
		sBookInfo += _T(" ") + sTmpBoInfo + _T("<!LN><!FP>");
	}

	return 0;
}

// 2008.04.14 ADD BY PWR 
// **천원단위 표현
VOID CBO_LOC_RECEIPT_PRINT_DLG::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace( _T(","), _T("") );

	CString strBuf = sMoney;
	strBuf.Replace( _T(","), _T("") );

	int nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.04.14 ADD BY PWR
// ** 대출자번호 주민번호일때 *표시
VOID CBO_LOC_RECEIPT_PRINT_DLG::ChangeSecuriyCivilNo(CString &sUserNo)
{
	if(_T("Y") != m_strCivilNoViewYN)
	{
		CLocCommonAPI::ChangeSecuriyUserNo(this, sUserNo, m_sIsUnityLoanService);
	}
}

// 2008.04.14 ADD BY PWR
// 2008.05.13 UPDATE BY PWR ( 체크지원 )
// ** 날짜컨트롤에서 날짜의 값을 가져온다.
VOID CBO_LOC_RECEIPT_PRINT_DLG::GetDateControl()
{
	SYSTEMTIME	tTime;
	DWORD		dwResult;

	// TO
	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		m_sDateTo = _T("");
	}
	else
	{
		m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
	{
		m_sDateFrom = _T("");
	}
	else
	{
		m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
}

// 2008.04.14 ADD BY PWR
// ** 날짜컨트롤을 일주일간으로 셋팅한다.
VOID CBO_LOC_RECEIPT_PRINT_DLG::SetDateControl()
{
	CTime tFrom = CTime::GetCurrentTime();
	CTime tTo = tFrom - CTimeSpan(7,0,0,0);
	
	// TO
	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	pDateTo->SetTime( &tTo );

	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	pDateFrom->SetTime( &tFrom );
}

// 2008.04.14 ADD BY PWR
// ** 기간검색을 한다.
VOID CBO_LOC_RECEIPT_PRINT_DLG::OnbtnDATESEARCH()
{
	INT nIndex = GetTabCtlIdx(1);
	//전체
	if( nIndex == 2 )
	{
		TabCheck();
	}
	else
	{	
		OnRadSelChange();
	}
}


HBRUSH CBO_LOC_RECEIPT_PRINT_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}


//=====================================================
//2008.12.02 UPDATE BY PJW : 주민번호인지 아닌지를 체크한다.
BOOL CBO_LOC_RECEIPT_PRINT_DLG::IsRRN(CString sRRN)
{

    if( sRRN.IsEmpty() == TRUE )	return TRUE;

    sRRN.Replace(_T(" "), _T("")); //공백 제거
//	sRRN.Replace("-", ""); //'-'문자 제거

    if( sRRN.GetLength() != 14 )	return FALSE; //길이 검사

    if( sRRN[6] != '-' )	return FALSE; //'-' 위치 검사

    int Gender;

    Gender = sRRN[7]-'0'; //남성(1,3) 여성(2,4)
    if( Gender < 1 || Gender > 4 )	return FALSE;

    int Val = 0;
    int nWeight[] = {

        2, 3, 4, 5, 6, 7, 

        0, //'-' 값 무효화 가중치

        8, 9, 2, 3, 4, 5

    };

    for (int i=0; i<14-1; i++) 

        Val += (sRRN[i] - '0') * nWeight[i];

    Val = 11 - (Val % 11);

    Val = Val % 10;

    if (Val == (sRRN[13]-'0')) return TRUE;

    return FALSE;
}
//=====================================================

INT CBO_LOC_RECEIPT_PRINT_DLG::Print(CString& strTitle, CMSHFlexGrid* pGrid, INT aPrintColNoList[], INT nPrintColNoCnt)
{
	if(NULL == pGrid)
	{
		return -1000;
	}
	CString strReceiptString;
	strReceiptString.Format(_T("<!FC><!FF>[%s]<!LF><!LC><!FP><!FP>"), strTitle);
	
	CString *astrHeader = NULL, strData;
	astrHeader = new CString[nPrintColNoCnt];

	INT nCheckCnt = 0;
	INT nColCnt = pGrid->GetCols(0);
	const INT nMaxRow = pGrid->GetRows();
	for(INT nRow = 1; nRow < nMaxRow; nRow++)
	{
		if(_T("V") == pGrid->GetTextMatrix(nRow, 1))
		{
			nCheckCnt++;
		}
	}

	for(nRow = 0; nRow < nMaxRow; nRow++)
	{
		for(INT nCol = 0; nCol < nPrintColNoCnt; nCol++)
		{
			if(0 == nRow)
			{
				astrHeader[nCol] = pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]);
			}
			else
			{
				if(0 < nCheckCnt)
				{
					if(pGrid->GetTextMatrix(nRow, 1).IsEmpty())
					{
						break;
					}
				}

				if(0 == nCol)
				{
					strData.Format(
						_T("<!FC>------------------------------------------<!LC><!FP>")
						_T("<!FN>%s. %s : %s<!LN><!FP>"),
						pGrid->GetTextMatrix(nRow, 0),
						astrHeader[nCol],
						getLimitWord(pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]).GetBuffer(0), 24, TRUE));
				}
				else
				{
					strData.Format(
						_T("<!FN>   %s : %s<!LN><!FP>"),
						astrHeader[nCol],
						pGrid->GetTextMatrix(nRow, aPrintColNoList[nCol]));

				}
				strReceiptString += strData;
			}
		}
		if(0 == nRow)
		{
			continue;
		}
	}
	delete []astrHeader;

	printReceipt(strReceiptString);

	return 0;

}