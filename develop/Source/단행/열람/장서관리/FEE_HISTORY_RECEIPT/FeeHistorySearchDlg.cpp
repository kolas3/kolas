// FeeHistorySearchDlg.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeHistorySearchDlg.h"
#include "FeeHistoryReceiptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeHistorySearchDlg dialog


CFeeHistorySearchDlg::CFeeHistorySearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeHistorySearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeHistorySearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CFeeHistorySearchDlg::~CFeeHistorySearchDlg()
{
}

void CFeeHistorySearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeHistorySearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeHistorySearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeHistorySearchDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_chkFEE_ALL, OnchkFEE_ALL)
	ON_BN_CLICKED(IDC_chkLOAN, OnchkLOAN)
	ON_BN_CLICKED(IDC_chkRETURN, OnchkRETURN)
	ON_BN_CLICKED(IDC_chkRECEIPT_ALL, OnchkRECEIPT_ALL)
	ON_BN_CLICKED(IDC_chkRECEIPT_Y, OnchkRECEIPT_Y)
	ON_BN_CLICKED(IDC_chkRECEIPT_N, OnchkRECEIPT_N)
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_chkFEE_ARREAR, OnchkFEE_ARREAR)
	ON_BN_CLICKED(IDC_chkFEE_LOANSTOPCANCEL, OnchkFEE_LOANSTOPCANCEL)
	ON_BN_CLICKED(IDC_chkFEE_RECARD, OnchkFEE_RECARD)
	ON_BN_CLICKED(IDC_chkPAYMENT, OnchkPAYMENT)
	ON_BN_CLICKED(IDC_chkPAYMENT_Y, OnchkPAYMENTY)
	ON_BN_CLICKED(IDC_chkPAYMENT_N, OnchkPAYMENTN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeHistorySearchDlg message handlers

BOOL CFeeHistorySearchDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeHistorySearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeHistorySearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_HISTORY_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_HISTORY_SEARCH") );
		return false;
	}

	// 컨트롤 초기값 ( 연체료, 납부일자, 출력유무Y/N )
	EnableControl( 1, FALSE );
	EnableControl( 2, FALSE );
	// 납부 유무 = Y
	((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

void CFeeHistorySearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

// 2008.03.03 ADD BY PWR
// ** 수수료종류 : 전체
void CFeeHistorySearchDlg::OnchkFEE_ALL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_ALL))->GetCheck();
	if( bCheck )
	{//채크함
		// 1. 수수료종류 전부체크
		EnableControl( 0, TRUE );
		// 2. 납부일자만 활성화
		EnableControl( 1, FALSE );
	}
	else
	{//채크안함
		// 1. 연체료만 체크
		((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->SetCheck(FALSE);
		// 2. 기간설정 전부 활성화
		EnableControl( 1, TRUE );
	}
}

// 2008.03.03 ADD BY PWR
// ** 수수료종류 : 연체료
void CFeeHistorySearchDlg::OnchkFEE_ARREAR() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->GetCheck();
	if( bCheck )
	{//채크함
		// 1. 회원증재발급과 대출정지일해제가 비활성화인지 검사
		BOOL bCheckReCard			= ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
		BOOL bCheckLoanStopCancel	= ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();
		// 2. 둘다 비활성화일때 기간모두 활성화
		if( !bCheckReCard && !bCheckLoanStopCancel )
		{
			EnableControl( 1, TRUE );
		}
		// 3. 납부유무 활성화
		((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
		// 4. 자료실 활성화
		((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
	}
	else
	{//채크안함
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** 수수료종류 : 회원증재발급
void CFeeHistorySearchDlg::OnchkFEE_RECARD() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
	if( bCheck )
	{//채크함
		// 기간설정 납부일자만 활성화
		EnableControl( 1, FALSE );
	}
	else
	{//채크안함
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** 수수료종류 : 대출정지일해제
void CFeeHistorySearchDlg::OnchkFEE_LOANSTOPCANCEL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();
	if( bCheck )
	{//채크함
		// 기간설정 납부일자만 활성화
		EnableControl( 1, FALSE );
	}
	else
	{//채크안함
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** 기간설정 : 납부일자
void CFeeHistorySearchDlg::OnchkPAYMENT()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT))->GetCheck();
	if( bCheck )
	{//채크함
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** 기간설정 : 대출일자
void CFeeHistorySearchDlg::OnchkLOAN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkLOAN))->GetCheck();
	if( bCheck )
	{//채크함
		((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(TRUE);
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** 기간설정 : 반납일자
void CFeeHistorySearchDlg::OnchkRETURN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRETURN))->GetCheck();
	if( bCheck )
	{//채크함
		((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(TRUE);
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** 영수증출력여부 : 전부
void CFeeHistorySearchDlg::OnchkRECEIPT_ALL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->GetCheck();
	if( bCheck )
	{//채크함
		EnableControl( 2, TRUE );
	}
	else
	{//채크안함
		EnableControl( 2, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** 영수증출력여부 : Y
void CFeeHistorySearchDlg::OnchkRECEIPT_Y() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->GetCheck();
	if( bCheck )
	{//채크함
		
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** 영수증출력여부 : N
void CFeeHistorySearchDlg::OnchkRECEIPT_N() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->GetCheck();
	if( bCheck )
	{//채크함

	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
	}
}

// 2008.04.29 ADD BY PWR
// ** 수수료 납부유무 : Y
void CFeeHistorySearchDlg::OnchkPAYMENTY() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->GetCheck();
	if( bCheck )
	{//채크함
		
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	}
}

// 2008.04.29 ADD BY PWR
// ** 수수료 납부유무 : N
void CFeeHistorySearchDlg::OnchkPAYMENTN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->GetCheck();
	if( bCheck )
	{//채크함
		
	}
	else
	{//채크안함
		((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	}
}

// 2008.03.04 ADD BY PWR
// ** 컨트롤의 활성화 및 비활성화
// nType ( 0:수수료종류, 1:기간설정, 2:영수증출력유무 )
VOID CFeeHistorySearchDlg::EnableControl( INT nType, BOOL bEnable )
{
	BOOL bCheckArrear		  = ((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->GetCheck();
	BOOL bCheckReCard		  = ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
	BOOL bCheckLoanStopCancel = ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();

	////////////////////////////////////////////////////////////////////////////
	// 수수료종류 컨트롤
	if( nType == 0 )
	{
		if( bEnable )
		{
			// 수수료종류 전부체크
			((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->SetCheck(TRUE);
		}
		else
		{	
			((CButton*)GetDlgItem(IDC_chkFEE_ALL))->SetCheck(FALSE);

			// 해제할때 수수료종류가 전부 체크가 안되었다면
			if( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel )
			{
				// 전체검사
				// 납부기간만 활성화
				EnableControl( 1, FALSE );
			}
			// 회원증재발급이나 대출정지일해제가 하나라도 활성화가 있으면
			else if( bCheckReCard || bCheckLoanStopCancel )
			{
				// 납부기간만 활성화
				EnableControl( 1, FALSE );
			}
			// 연체료만 활성화
			else if( bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel )
			{
				// 기간전부 활성화
				EnableControl( 1, TRUE );
			}
			// 연체료 활성화
			if( bCheckArrear || ( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel ) )
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(FALSE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(FALSE);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	// 기간설정 컨트롤
	else if( nType == 1 )
	{
		// 기간설정을 전부 활성화한다. + 납부유무도 활성화
		if( bEnable )
		{
			// 1. 납부일자 활성화
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
			// 2. 대출일자 활성화
			((CButton*)GetDlgItem(IDC_chkLOAN))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkLOAN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
			// 3. 반납일자 활성화
			((CButton*)GetDlgItem(IDC_chkRETURN))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkRETURN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
			// 4. 납부유무 활성화
			((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
		}
		// 기간설정에서 납부일자만 활성화하고 나머지는 비활성화한다. + 납부유무도 비활성화
		else
		{
			// 1. 납부일자 활성화
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
			// 2. 대출일자 비활성화
			((CButton*)GetDlgItem(IDC_chkLOAN))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_chkLOAN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
			// 3. 반납일자 비활성화
			((CButton*)GetDlgItem(IDC_chkRETURN))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_chkRETURN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
			// 4. 납부유무 비활성화
			if( bCheckArrear || ( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel ) )
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(FALSE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(FALSE);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	//영수증출력유무 컨트롤
	else if( nType == 2 )
	{
		if( bEnable )
		{
			((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->SetCheck(FALSE);
		}
	}
	else
	{
		return;
	}
}

// 2008.03.03 ADD BY PWR
// ** 검색버튼 ( 쿼리 생성해서 검색함수로 보낸다. )
void CFeeHistorySearchDlg::OnbtnSEARCH() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	CFeeHistoryReceiptDlg *pParent = ( CFeeHistoryReceiptDlg* )pParentMgr;
	CString sQuery = MakeQuery( pParent );
	if( !sQuery.IsEmpty() )
	{
		pParent->SearchData( sQuery );
	}
	
	AfxGetApp()->DoWaitCursor(-1);
}

// 2008.03.03 ADD BY PWR
// ** 닫기버튼
void CFeeHistorySearchDlg::OnbtnCLOSE() 
{
	this->ShowWindow(FALSE);
}

// 2008.03.04 ADD BY PWR
// ** Query 생성
CString CFeeHistorySearchDlg::MakeQuery( VOID *pParent )
{
	CString sQuery, sQuery1, sQuery1_1, sQuery2, sQuery3, sQuery4;
	CString sFeeWhere1, sFeeWhere2, sFeeWhere3, sFeeWhere4;
	CString sDateWhere, sUserNoWhere, sUserNameWhere, sReceiptWhere, sPaymentWhere, sSearchWhere;
	CString strManageCodeWhere, strShelfLocCodeWhere;

	// 0:연체료만선택되어있을때(CASE1)
	// 1:연체료와같이(UNION)
	// 2:회원증재발급이나 대출정지일해제(CASE2)
	INT nType = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. 체크박스의 선택유무를 가져온다.
	INT	 nCheckCnt = 13;
	BOOL bCheck[13];
	INT	 nCheckName[13] = { IDC_chkFEE_ALL				, IDC_chkFEE_ARREAR	, IDC_chkFEE_RECARD	, 
							IDC_chkFEE_LOANSTOPCANCEL	, IDC_chkPAYMENT	, IDC_chkLOAN		, 
							IDC_chkRETURN				, IDC_chkRECEIPT_ALL, IDC_chkRECEIPT_Y	, 
							IDC_chkRECEIPT_N			, IDC_chkPAYMENT_ALL, IDC_chkPAYMENT_Y	,
							IDC_chkPAYMENT_N	};

	for(INT i = 0; i < nCheckCnt; i++)
	{
		bCheck[i] = ((CButton*)GetDlgItem(nCheckName[i]))->GetCheck();
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2. 대출자번호와 대출자명을 가져온다.
	const int nCnt = 4;
	CString sControlAlias[nCnt] = 
				{ _T("대출자번호1") , _T("대출자번호2") , _T("대출자명1") , _T("대출자명2") };
	
	CString sGetData[nCnt];
	for( i = 0 ; i < nCnt; i++ )
	{
		GetControlData( _T("CM_FEE_HISTORY_SEARCH") , sControlAlias[i] , sGetData[i] , -1 , 0 );
		sGetData[i].TrimRight();
	}

	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. 수수료종류에 따른 조건생성
	// 3.1 연체료
	sQuery1 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("TO_CHAR( CL.LOAN_DATE, 'YYYY/MM/DD' ), TO_CHAR( CL.RETURN_PLAN_DATE, 'YYYY/MM/DD' ), ")
				_T("TO_CHAR( CL.RETURN_DATE, 'YYYY/MM/DD' ), CL.REG_NO, CL.TITLE, ") // 2008.03.19 ADD BY PWR ( 서명, 등록번호 )
				_T("CL.SHELF_LOC_CODE, CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CL.REC_KEY=CFPI.LOAN_INFO_KEY AND CL.USER_KEY=CLU.REC_KEY AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	sQuery1_1 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("TO_CHAR( CL.LOAN_DATE, 'YYYY/MM/DD' ), TO_CHAR( CL.RETURN_PLAN_DATE, 'YYYY/MM/DD' ), ")
				_T("TO_CHAR( CL.RETURN_DATE, 'YYYY/MM/DD' ), CL.REG_NO, CL.TITLE, ") // 2008.03.19 ADD BY PWR ( 서명, 등록번호 )
				_T("CL.SHELF_LOC_CODE, CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CL.REC_KEY=CFPI.LOAN_INFO_KEY AND CL.USER_KEY=CLU.REC_KEY AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	// 3.2 회원증재발급, 대출정지일해제
	sQuery2 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("NULL, NULL, NULL, NULL, NULL, NULL, ")
				_T("CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	// 2009.03.06 ADD BY PWR : 소급입력 추가
	// 연체료
	sQuery3 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN,CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE,")
				_T("NULL,NULL,NULL,NULL,NULL,NULL,CFPI.RECEIPT_KEY,CFPI.CARD_CLASS,CFPI.USER_ID,")
				_T("CFPI.REC_KEY,CFPI.LOAN_INFO_KEY,CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS IN('Y')");
	
	// 회원증재발급, 대출정지일해제
	sQuery4 = sQuery3;

	MakeFeeWhere( bCheck, sFeeWhere1, sFeeWhere2, sFeeWhere3, sFeeWhere4, nType );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 4. 기간설정에 따른 조건생성
	sDateWhere = _T("");
	MakeDateWhere( bCheck, sDateWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 5. 대출자번호
	sUserNoWhere = _T("");
	MakeUserNoWhere( sGetData, sUserNoWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 6. 대출자명
	sUserNameWhere = _T("");
	MakeUserNameWhere( sGetData, sUserNameWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 7. 영수증출력유무
	sReceiptWhere = _T("");
	MakeReceiptWhere( bCheck, sReceiptWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 8. 수수료납부유무
	sPaymentWhere = _T("");
	MakePaymentWhere( bCheck, sPaymentWhere );

	// 2009.03.09 ADD BY PWR : 관리구분 조건 추가
	strManageCodeWhere = _T("");
	MakeManageCodeWhere( strManageCodeWhere );

	// 2009.03.09 ADD BY PWR : 자료실 조건 추가
	strShelfLocCodeWhere = _T("");
	MakeShelfLocCodeWhere( strShelfLocCodeWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 10. 생성된 조건 합치기
	// 10.1 연체료만선택되어있을때(CASE1)
	if( nType == 0 )
	{
		// 2009.10.07 UPDATE BY KSJ : 대출일자 / 반납일자 조건이 있다면 소급은 제외해서 검색
		//DEL if(strShelfLocCodeWhere.IsEmpty())
		if( TRUE == strShelfLocCodeWhere.IsEmpty() && TRUE != bCheck[5] && TRUE != bCheck[6] )
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery3 + sFeeWhere3 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
		else // 자료실 조건이 있다면 소급은 제외해서 검색
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
	}
	// 10.2 연체료와같이(UNION)
	else if( nType == 1 )
	{
		if(strShelfLocCodeWhere.IsEmpty())
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery3 + sFeeWhere3 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere +
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
		else // 자료실 조건이 있다면 소급은 제외해서 검색
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere +
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
	}
	// 10.3 회원증재발급이나 대출정지일해제(CASE2)
	else if( nType == 2 )
	{ 
		sQuery =	sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
					+ sPaymentWhere + strManageCodeWhere + 
					_T(" UNION ALL ") +
					sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
					+ sPaymentWhere + strManageCodeWhere + 
					_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
	}

	return sQuery;
}

// **기간설정 일자 구하기 (CM에서 해당 값을 반환한다)
CString CFeeHistorySearchDlg::TakeControlData( CString sAlias )
{
EFS_BEGIN

	CString sDate;
	CESL_ControlMgr* pCM = FindCM( _T("CM_FEE_HISTORY_SEARCH") );
	if( !pCM ) return _T("");
	CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement( sAlias );
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )this->GetDlgItem( pCE->CTRL_ID );

	SYSTEMTIME tTime;
	DWORD dwResult = pDate->GetTime( &tTime );
	if (dwResult != GDT_VALID ) return _T("");
	sDate.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	return sDate;

EFS_END
return _T("");
}

// 2008.03.04 ADD BY PWR
// ** 수수료종류에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakeFeeWhere( BOOL bCheck[], CString &sFeeWhere1, CString &sFeeWhere2, CString &sFeeWhere3, CString &sFeeWhere4, INT &nType )
{
	// 000 모두 선택안함
	if( !bCheck[1] && !bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 1;//(UNION)
	}
	// 001 대출정지일해제만 선택
	else if( !bCheck[1] && !bCheck[2] && bCheck[3] )
	{
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(2)");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0");			// 2008.06.02 ADD BY PWR ( 0원제외 )
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('2')");
		nType = 2;//(CASE2)
	}
	// 010 회원증재발급만 선택
	else if( !bCheck[1] && bCheck[2] && !bCheck[3] )
	{
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('1')");
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1')");
		nType = 2;//(CASE2)
	}
	// 011 회원증재발급과 대출정지일해제 선택
	else if( !bCheck[1] && bCheck[2] && bCheck[3] )
	{
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 2;//(CASE2)
	}
	// 100 연체료만 선택
	else if( bCheck[1] && !bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		nType = 0;//(CASE1)
	}
	// 101 연체료와 대출정지일해제 선택
	else if( bCheck[1] && !bCheck[2] && bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(2)");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0");			// 2008.06.02 ADD BY PWR ( 0원제외 )
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('2')");
		nType = 1;//(UNION)
	}
	// 110 연체료와 회원증재발급 선택
	else if( bCheck[1] && bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('1')");
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1')");
		nType = 1;//(UNION)
	}
	// 111 모두선택
	else if( bCheck[1] && bCheck[2] && bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( 대출정지일0원제외 )
		// 2009.03.09 ADD BY PWR : 소급입력
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 1;//(UNION)
	} 
}

// 2008.03.04 ADD BY PWR
// ** 기간설정에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakeDateWhere( BOOL bCheck[], CString &sDateWhere )
{
	CString sPaymentDate1	= TakeControlData( _T("납부일1") );
	CString sPaymentDate2	= TakeControlData( _T("납부일2") );
	CString sLoanDate1		= TakeControlData( _T("대출일1") );
	CString sLoanDate2		= TakeControlData( _T("대출일2") );
	CString sReturnDate1	= TakeControlData( _T("반납일1") );
	CString sReturnDate2	= TakeControlData( _T("반납일2") );

	// 000 모두 선택 안함 ( 조건없음 )
	if( !bCheck[4] && !bCheck[5] && !bCheck[6] )
	{
		sDateWhere = _T("");
	}
	// 001 반납일만 선택 ( 반납일 검색 )
	else if( !bCheck[4] && !bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}
	}
	// 010 대출일만 선택 ( 대출일 검색 )
	else if( !bCheck[4] && bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}			
	}
	// 011 대출일과 반납일 선택 ( 대출일,반납일 검색 )
	else if( !bCheck[4] && bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
		// sDateWhere.Format(	
		// 	_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
		// 	, sLoanDate1, sLoanDate2, sReturnDate1, sReturnDate2 );
		CString strWhereLoanDate , strWhereReturnDate;
		strWhereLoanDate = _T("");		strWhereReturnDate = _T("");
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}
		
		sDateWhere = strWhereLoanDate + strWhereReturnDate;
	}
	// 100 납부일만 선택 ( 납부일 검색 )
	else if( bCheck[4] && !bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
	}
	// 101 납부일과 반납일선택 ( 납부일,반납일 검색 )
	else if( bCheck[4] && !bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sReturnDate1, sReturnDate2 );
		CString strWherePaymentDate , strWhereReturnDate;
		strWherePaymentDate = _T("");		strWhereReturnDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
		
		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereReturnDate;
	}
	// 110 납부일과 대출일선택 ( 납부일,대출일 검색 )
	else if( bCheck[4] && bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sLoanDate1, sLoanDate2 );
		CString strWherePaymentDate , strWhereLoanDate;
		strWherePaymentDate = _T("");		strWhereLoanDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
		
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereLoanDate;
	}
	// 111 모두 선택 ( 납부일, 대출일, 반납일 검색 )
	else if( bCheck[4] && bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query 오류 수정
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			//===============================================================================
//DEL 			// 2009.10.16 UPDATE BY KSJ : Where 조건에 AS 들어갈 수 없음
//DEL 			//DEL _T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AS ORDER_BY_DATE")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
//DEL 			//===============================================================================
//DEL 			_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sLoanDate1, sLoanDate2, sReturnDate1, sReturnDate2 );
		CString strWherePaymentDate , strWhereLoanDate , strWhereReturnDate;
		strWherePaymentDate = _T(""); strWhereLoanDate = _T(""); strWhereReturnDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}

		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereLoanDate + strWhereReturnDate;
	}
}

// 2008.03.04 ADD BY PWR
// ** 대출자번호에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakeUserNoWhere( CString sGetData[], CString &sUserNoWhere )
{
	if( !sGetData[0].IsEmpty() )
	{
		sUserNoWhere = _T(" AND");
		if( !sGetData[1].IsEmpty() )
			sUserNoWhere += _T(" CLU.USER_NO BETWEEN '") + sGetData[0] + _T("' AND '") + sGetData[1]+ _T("'");
		else
			sUserNoWhere += _T(" CLU.USER_NO = '") + sGetData[0] + _T("'");
	}
}

// 2008.03.04 ADD BY PWR
// ** 대출자명에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakeUserNameWhere( CString sGetData[], CString &sUserNameWhere )
{
	if( !sGetData[2].IsEmpty() )
	{
		sUserNameWhere = _T(" AND");
		if( !sGetData[3].IsEmpty() )
			sUserNameWhere += _T(" CLU.IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[3]+ _T("'");
		else
			sUserNameWhere += _T(" CLU.IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[2]+ _T("힝'");
	}
}

// 2008.03.04 ADD BY PWR
// ** 영수증출력유무에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakeReceiptWhere( BOOL bCheck[], CString &sReceiptWhere )
{
	// Y, N 둘다 선택안함 ( 조건없음 )
	if( !bCheck[8] && !bCheck[9] )
	{
		sReceiptWhere = _T("");
	}
	// N만 선택 ( N만 검색 )
	else if( !bCheck[8] && bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('N')");
	}
	// Y만 선택 (Y만 검색 )
	else if( bCheck[8] && !bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('Y')");
	}
	// Y, N 둘다 선택 ( Y,N 검색 )
	else if( bCheck[8] && bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('Y', 'N')");
	}
}

// 2008.04.29 ADD BY PWR
// ** 수수료 납부유무에 따른 WHERE생성
VOID CFeeHistorySearchDlg::MakePaymentWhere( BOOL bCheck[], CString &sPaymentWhere )
{
	// Y, N 둘다 선택안함 ( 조건없음 )
	if( !bCheck[11] && !bCheck[12] )
	{
		sPaymentWhere = _T("");
	}
	// N만 선택 ( N만 검색 )
	else if( !bCheck[11] && bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('N')");
	}
	// Y만 선택 (Y만 검색 )
	else if( bCheck[11] && !bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('Y')");
	}
	// Y, N 둘다 선택 ( Y,N 검색 )
	else if( bCheck[11] && bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('Y', 'N')");
	}
}

VOID CFeeHistorySearchDlg::MakeManageCodeWhere( CString &strManageCodeWhere )
{
	CString strManageCode;
	GetControlData( _T("CM_FEE_HISTORY_SEARCH"), _T("관리구분"), strManageCode );
	strManageCode.TrimRight();
	if( strManageCode.IsEmpty() || (strManageCode.Compare(_T("0"))==0) || (strManageCode.Compare(_T("적용안함"))==0) ) 
	{
		return;
	}	
	strManageCodeWhere = _T(" AND CFPI.MANAGE_CODE='") + strManageCode + _T("' ");
}

VOID CFeeHistorySearchDlg::MakeShelfLocCodeWhere( CString &strShelfLocCodeWhere )
{	
	CString strShelfLocCode;
	GetControlData( _T("CM_FEE_HISTORY_SEARCH"), _T("자료실"), strShelfLocCode );
	strShelfLocCode.TrimRight();
	if( strShelfLocCode.IsEmpty() || (strShelfLocCode.Compare(_T("0"))==0) || (strShelfLocCode.Compare(_T("적용안함"))==0) ) 
	{
		return;
	}
	
	strShelfLocCodeWhere = _T(" AND CL.SHELF_LOC_CODE='") + strShelfLocCode + _T("' ");
}

HBRUSH CFeeHistorySearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
