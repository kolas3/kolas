#include "stdafx.h"
#include "FeeLoanStop.h"

#include "BO_LOC_RECEIPT_PRINT_DLG.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\..\..\..\관리\도서관관리\FileManager\LibFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFeeLoanStop::CFeeLoanStop(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager, CLonaManageValue* pManageValue /*NULL*/)
	: CESL_Mgr(CFeeLoanStop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeLoanStop)
	//}}AFX_DATA_INIT
	m_pParent = pParent;

	m_bCheck = FALSE;
	m_bPrint = FALSE;

	m_pLoanShareManager = pLoanShareManager;

	m_pDM = NULL;
	m_sIsAddHolyDayReservation = _T("");
	m_sBasicArrear = _T("");	
	m_sRecKey = _T("");	
	m_sUserNo = _T("");
	m_sName = _T("");
	m_sLoanStopDate = _T("");
	m_sMoneyPaid = _T("");
	m_strLibCode = _T("");

	m_pManageValue = pManageValue;

	GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지휴관일포함유무"), m_sIsAddHolyDayReservation );
	GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("연체료"), m_sBasicArrear );

	m_tCurrent = COleDateTime::GetCurrentTime();
	m_tCurrent = COleDateTime( m_tCurrent.GetYear(), m_tCurrent.GetMonth(), m_tCurrent.GetDay(), 0, 0, 0 );

}

CFeeLoanStop::~CFeeLoanStop()
{
	if(m_pParent) m_pParent = NULL;

	m_bCheck = FALSE;
	m_bPrint = FALSE;

	if(m_pLoanShareManager) m_pLoanShareManager = NULL;

	if(m_pManageValue) m_pManageValue = NULL;

	m_sIsAddHolyDayReservation.Empty();
	m_sBasicArrear.Empty();

}

VOID CFeeLoanStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeLoanStop)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeLoanStop, CDialog)
	//{{AFX_MSG_MAP(CFeeLoanStop)
	ON_BN_CLICKED(IDC_btnLOANSTOPCLOSE2, OnbtnLOANSTOPCLOSE)
	ON_BN_CLICKED(IDC_btnPREVIEW2, OnbtnPREVIEW)
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT2, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_BTN_PRINT_CANCEL2, OnbtnPRINTCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeLoanStop message handlers

BOOL CFeeLoanStop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("FEE_LOANSTOPCANCEL_RECEIPT") ) < 0){
		AfxMessageBox( _T("InitESL_Mgr Error!::FEE_LOANSTOPCANCEL_RECEIPT") );
		return FALSE;
	}
	
	InitValue();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	return TRUE;
}

BOOL CFeeLoanStop::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}		
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeLoanStop::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CFeeLoanStop::SetUserKey( CESL_DataMgr *pDM, CString sUserKey, CString strUserNo, CString strName, CString strLoanStopDate)
{
	m_sRecKey = sUserKey;
	m_sUserNo = strUserNo;
	m_sName = strName;
	m_sLoanStopDate = strLoanStopDate;
	m_sMoneyPaid = _T("0");
	m_pDM = pDM;

	return 0;
}

VOID CFeeLoanStop::OnbtnLOANSTOPCLOSE() 
{
	OnCancel();	
}

VOID CFeeLoanStop::OnbtnPREVIEW() 
{
	INT ids = OnLoanStopReceipt(TRUE);
	if( ids != 100 ) OnCancel();
}

VOID CFeeLoanStop::OnbtnPRINTRECEIPT() 
{
	INT ids = OnLoanStopReceipt(FALSE);
	if( ids != 100 ) OnCancel();
}

INT CFeeLoanStop::InitValue()
{
	GetDlgItem(IDC_edtLOANSTOPDATE)->SetWindowText(m_sLoanStopDate);
	COleDateTime tLoanStopDate( _ttoi(m_sLoanStopDate.Mid(0,4)), _ttoi(m_sLoanStopDate.Mid(5,2)), _ttoi(m_sLoanStopDate.Mid(8,2)), 0, 0, 0 );
	
	CString strTmp;
	strTmp.Format(_T("%04d/%02d/%02d"), tLoanStopDate.GetYear(), tLoanStopDate.GetMonth(), tLoanStopDate.GetDay() );
	
	m_tCurrent = COleDateTime( m_tCurrent.GetYear(), m_tCurrent.GetMonth(), m_tCurrent.GetDay(), 0, 0, 0 );

	CString sStopDay;
	COleDateTimeSpan tStopDay = tLoanStopDate - m_tCurrent;	
	INT nStopDay_Include_O = tStopDay.GetDays()+1;
	INT nStopDay_Include_X = tStopDay.GetDays();
	sStopDay.Format( _T("%d"), nStopDay_Include_O );
	GetDlgItem(IDC_edtSTOP_DAY)->SetWindowText(sStopDay);

	CString sStopCancelPrice;

	if( m_sIsAddHolyDayReservation == _T("Y") )
	{
		CString sHolyDay;
		INT nValue = nStopDay_Include_X;
		INT ids = GetAddTotalHolyDay( nValue );
		if( ids < 0 ) OnCancel();
		INT nHolyDay = nValue - nStopDay_Include_X;
		sHolyDay.Format( _T("%d"), nHolyDay );
		GetDlgItem(IDC_edtHOLY_DAY)->SetWindowText(sHolyDay);

		sStopCancelPrice.Format( _T("%d"), ( (nStopDay_Include_O - nHolyDay) * _ttoi(m_sBasicArrear) ) );
		SetMoneyFormat(sStopCancelPrice);

		sStopDay.Format( _T("%d"), ( nStopDay_Include_O - nHolyDay ) );
		GetDlgItem(IDC_edtDISSOLVE_DAY)->SetWindowText(sStopDay);
	}
	else
	{

		GetDlgItem(IDC_edtHOLY_DAY)->SetWindowText( _T("적용안함") );
		
		sStopCancelPrice.Format( _T("%d"), ( nStopDay_Include_O * _ttoi(m_sBasicArrear) ) );
		SetMoneyFormat(sStopCancelPrice);
		
		GetDlgItem(IDC_edtDISSOLVE_DAY)->SetWindowText(sStopDay);
	}

	m_sMoneyPaid = sStopCancelPrice;
	GetDlgItem(IDC_edtFEE_PAYMENT)->SetWindowText( m_sMoneyPaid );

	if( m_sMoneyPaid == _T("0") )
	{
		GetDlgItem(IDC_btnPREVIEW2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PRINT_CANCEL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnPRINT_RECEIPT2)->EnableWindow(FALSE);
	}

	return 0;
}

INT CFeeLoanStop::OnLoanStopReceipt(BOOL bShow)
{
	CString sQuery, sRecKey, sInsertRecKey, sNumber, sUserNo, sName, sDate, sSetDate;
	CString sStopDay, sHolyDay, sDissolveDay, sReceiptRecKey;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_LOANSTOPCANCEL_RECEIPT") );

	sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	INT ids = pDM->GetOneValueQuery( sQuery, sNumber );
	if ( ids < 0 ) 
	{
		if( -4007 == ids )
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			
			ids = pDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("영수증_일련번호") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			ids = pDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
			if( 0 > ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("setRctTempVariable") );

			pDM->SendFrame();
			pDM->EndFrame();
		}
		else
		{
			return -1;
		}
	}
		
	sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE = '%s';")
					, _ttoi(sNumber)+1, m_pInfo->MANAGE_CODE );		
	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);

	m_tCurrent = COleDateTime::GetCurrentTime();

	sNumber.Format( _T("%d"), _ttoi(sNumber)+1 );
	sRecKey = m_sRecKey;
	sUserNo = m_sUserNo;
	sName = m_sName;
	sDate = m_tCurrent.Format( _T("%Y/%m/%d") );
	GetDlgItem(IDC_edtSTOP_DAY)->GetWindowText(sStopDay);
	GetDlgItem(IDC_edtHOLY_DAY)->GetWindowText(sHolyDay);	
	GetDlgItem(IDC_edtDISSOLVE_DAY)->GetWindowText(sDissolveDay);
	GetDlgItem(IDC_edtFEE_PAYMENT)->GetWindowText(m_sMoneyPaid);
	sSetDate = m_tCurrent.Format( _T("%Y/%m/%d %H:%M") );

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("REC_KEY")		, sRecKey			, 0);
	pDM->SetCellData( _T("일련번호")	, sNumber			, 0);
	pDM->SetCellData( _T("회원번호")	, sUserNo			, 0);
	pDM->SetCellData( _T("성명")		, sName				, 0);
	pDM->SetCellData( _T("납부일")		, sDate				, 0);
	pDM->SetCellData( _T("대출정지일")	, m_sLoanStopDate	, 0);
	pDM->SetCellData( _T("정지일수")	, sStopDay			, 0);
	pDM->SetCellData( _T("휴관일수")	, sHolyDay			, 0);
	pDM->SetCellData( _T("해제일수")	, sDissolveDay		, 0);
	pDM->SetCellData( _T("납부금액")	, m_sMoneyPaid		, 0);
	pDM->SetCellData( _T("처리일시")	, sSetDate			, 0);
	pDM->SetCellData( _T("기본연체료")	, m_sBasicArrear	, 0);
	
	
	CBO_LOC_RECEIPT_PRINT_DLG* ReceiptDlg;
	ReceiptDlg = NULL;
	
	ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pManageValue );
	ReceiptDlg->pMain = this->pMain;
	ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
	ReceiptDlg->m_pInfo = this->m_pInfo;

	INT nPrintStatus = ReceiptDlg->PrintLoanStopCancelInfo(bShow, TRUE);

	if ( nPrintStatus )
	{
		if( nPrintStatus == 100)
		{
			if( &ReceiptDlg != NULL )
				delete ReceiptDlg;
			ReceiptDlg = NULL;

			return 100;
		}
		else
		{
			if ( AfxMessageBox( _T("출력실패! 재시도 하시게습니까?"), MB_YESNO ) == IDYES )
			{
				OnLoanStopReceipt(FALSE);

				if( &ReceiptDlg != NULL )
					delete ReceiptDlg;
				ReceiptDlg = NULL;

				return 100;
			}
			else
			{
				if( &ReceiptDlg != NULL )
					delete ReceiptDlg;
				ReceiptDlg = NULL;
				return 0;
			}
		}
	}
	sQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
	pDM->GetOneValueQuery(sQuery, sReceiptRecKey);
	m_sMoneyPaid.Replace( _T(","), _T("") );
	CString sUserID = m_pInfo->USER_ID;
	CString sFirstWork = GetWorkLogMsg( _T("대출정지일해제(출력)"), __WFILE__, __LINE__ );

	sQuery.Format(	_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL")
					_T("(REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, MONEY_PAID, STATUS, ")
					_T("CREATE_DATE, RECEIPT_KEY, RECEIPT_PRINT_YN, USER_ID, LOAN_STOP_DATE, MANAGE_CODE, FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL, '2', %s, SYSDATE, '%s', 'Y', SYSDATE, ")
					_T("%s, 'Y', '%s', '%s', '%s','%s');")		
					, sRecKey, m_sMoneyPaid, sReceiptRecKey, sUserID, m_sLoanStopDate, m_pInfo->MANAGE_CODE,sFirstWork );

	pDM->AddFrame( sQuery );

	sQuery.Format(	_T("INSERT INTO CO_FEE_RECEIPT_TBL")
						_T(		  "(REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE)")
						_T(	"VALUES(%s, '1', SYSDATE, '%s', '대출정지일해제수수료 납부_'||SYSDATE, '%s');")
					, sReceiptRecKey, ReceiptDlg->m_strRCT, m_pInfo->MANAGE_CODE);
	pDM->AddFrame( sQuery );

	CString sGetData;
	m_pDM->GetCellData( _T("회원상태설명"), 0, sGetData);

	if( sGetData.Compare( _T("대출정지") )==0 )
	{
		m_pDM->SetCellData( _T("회원상태설명"), _T("정상"), 0);
		m_pDM->SetCellData( _T("대출정지일"), _T(""), 0);
		sQuery.Format(	_T("UPDATE CO_LOAN_USER_TBL ")
						_T(			"SET USER_CLASS='0', LOAN_STOP_DATE='' ")
						_T(			"WHERE REC_KEY=%s;"), sRecKey);
		pDM->AddFrame( sQuery );
	}
	else
	{
		m_pDM->SetCellData( _T("대출정지일"), _T(""), 0);
		sQuery.Format(	_T("UPDATE CO_LOAN_USER_TBL ")
						_T(			"SET LOAN_STOP_DATE='' ")
						_T(			"WHERE REC_KEY=%s;"), sRecKey);
		pDM->AddFrame( sQuery );
	}

	m_bPrint = TRUE;
	pDM->SendFrame(TRUE);	
	pDM->EndFrame();

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("USER_CLASS,LOAN_STOP_DATE");		
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	pDM->FreeData();
	
	if( &ReceiptDlg != NULL )
		delete ReceiptDlg;
	ReceiptDlg = NULL;
	
	return 0;
}

VOID CFeeLoanStop::OnbtnPRINTCANCEL() 
{
	if ( AfxMessageBox( _T("수수료를 받으셨습니까?\n(영수증 출력을 생략하고 대출정지일이 해제 됩니다.)\n")
						_T("취소된 자료는 수수료관리에서 재출력할 수 있습니다."), MB_YESNO) == IDYES )
	{
		CString sQuery, sRecKey, sInsertRecKey, sNumber, sUserNo, sName, sDate;
		CString sStopDay, sHolyDay, sDissolveDay, sReceiptRecKey;

		CESL_DataMgr *pDM = FindDM( _T("DM_FEE_LOANSTOPCANCEL_RECEIPT") );

		sQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO'AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
		INT ids = pDM->GetOneValueQuery( sQuery, sNumber );
		if ( ids < 0 ) 
		{
			if( -4007 == ids )
			{
				pDM->StartFrame();
				pDM->InitDBFieldData();
				
				ids = pDM->AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("RCT_NO") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("DIVISION_VALUE"),	_T("STRING"),	_T("영수증_일련번호") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("LAST_NUMBER"),	_T("NUMERIC"),	_T("0") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("CREATE_DATE"),	_T("NUMERIC"),		_T("SYSDATE") );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("CREATE_WORKER"),	_T("STRING"),	m_pInfo->USER_ID );
				if( 0 > ids )	return;

				ids = pDM->AddDBFieldData( _T("MANAGE_CODE"),	_T("STRING"),	m_pInfo->MANAGE_CODE  );
				if( 0 > ids )	return;

				ids = pDM->MakeInsertFrame( _T("CO_LAST_NUMBER_TBL") );
				if( 0 > ids )	return;

				pDM->SendFrame();
				pDM->EndFrame();
			}
			else
			{
				return;
			}
		}	
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE KIND_CODE='RCT_NO'AND MANAGE_CODE = '%s';")
						, _ttoi(sNumber)+1, m_pInfo->MANAGE_CODE );		
		pDM->StartFrame();
		pDM->InitDBFieldData();	
		pDM->AddFrame(sQuery);

		sNumber.Format( _T("%d"), _ttoi(sNumber)+1 );
		sRecKey = m_sRecKey;
		sUserNo = m_sUserNo;
		sName = m_sName;
		sDate = m_tCurrent.Format( _T("%Y/%m/%d") );
		GetDlgItem(IDC_edtSTOP_DAY)->GetWindowText(sStopDay);
		GetDlgItem(IDC_edtHOLY_DAY)->GetWindowText(sHolyDay);	
		GetDlgItem(IDC_edtDISSOLVE_DAY)->GetWindowText(sDissolveDay);
		GetDlgItem(IDC_edtFEE_PAYMENT)->GetWindowText(m_sMoneyPaid);

		pDM->InsertRow(-1);
		pDM->SetCellData( _T("REC_KEY")		, sRecKey			, 0);
		pDM->SetCellData( _T("일련번호")	, sNumber			, 0);
		pDM->SetCellData( _T("회원번호")	, sUserNo			, 0);
		pDM->SetCellData( _T("성명")		, sName				, 0);
		pDM->SetCellData( _T("납부일")		, sDate				, 0);
		pDM->SetCellData( _T("대출정지일")	, m_sLoanStopDate	, 0);
		pDM->SetCellData( _T("정지일수")	, sStopDay			, 0);
		pDM->SetCellData( _T("휴관일수")	, sHolyDay			, 0);
		pDM->SetCellData( _T("해제일수")	, sDissolveDay		, 0);
		pDM->SetCellData( _T("납부금액")	, m_sMoneyPaid		, 0);
		
		CBO_LOC_RECEIPT_PRINT_DLG* ReceiptDlg;
		ReceiptDlg = NULL;
		
		ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pManageValue );
		ReceiptDlg->pMain = this->pMain;
		ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		ReceiptDlg->m_pInfo = this->m_pInfo;

		INT nPrintStatus = ReceiptDlg->PrintLoanStopCancelInfo(FALSE, FALSE);

		if ( nPrintStatus )
		{
			AfxMessageBox( _T("XML생성에 실패 하였습니다.") );

			if( &ReceiptDlg != NULL )
				delete ReceiptDlg;
			ReceiptDlg = NULL;

			return;
		}

		sQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
		pDM->GetOneValueQuery(sQuery, sReceiptRecKey);

		m_sMoneyPaid.Replace( _T(","), _T("") );

		CString sUserID = m_pInfo->USER_ID;
		CString sFirstWork = GetWorkLogMsg( _T("대출정지일해제(출력취소)"), __WFILE__, __LINE__ );

		sQuery.Format(	_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL")
						_T(	"(REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, MONEY_PAID, STATUS, ")
						_T(	"CREATE_DATE, RECEIPT_KEY, RECEIPT_PRINT_YN, USER_ID, LOAN_STOP_DATE, MANAGE_CODE, FIRST_WORK) ")				
						_T("VALUES(ESL_SEQ.NEXTVAL, '2', %s, SYSDATE, '%s', 'Y', SYSDATE, ")
						_T(	"%s, 'N', '%s', '%s', '%s', '%s');")				
						, sRecKey, m_sMoneyPaid, sReceiptRecKey, sUserID, m_sLoanStopDate, m_pInfo->MANAGE_CODE, sFirstWork );
		pDM->AddFrame( sQuery );
	
		sQuery.Format(	_T("INSERT INTO CO_FEE_RECEIPT_TBL")
						_T("(REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE)")
						_T("VALUES(%s, '0', SYSDATE, '%s', '대출정지일해제수수료 납부_'||SYSDATE, '%s');")
						, sReceiptRecKey, ReceiptDlg->m_strRCT, m_pInfo->MANAGE_CODE);
		pDM->AddFrame( sQuery );

		CString sGetData;
		m_pDM->GetCellData( _T("회원상태설명"), 0, sGetData);

		if( sGetData.Compare( _T("대출정지") )==0 )
		{
			m_pDM->SetCellData( _T("회원상태설명"), _T("정상"), 0);
			m_pDM->SetCellData( _T("대출정지일"), _T(""), 0);
			sQuery.Format(	_T("UPDATE CO_LOAN_USER_TBL ")
							_T(			"SET USER_CLASS='0', LOAN_STOP_DATE='' ")
							_T(			"WHERE REC_KEY=%s;"), sRecKey);
			pDM->AddFrame( sQuery );
		}
		else
		{
			m_pDM->SetCellData( _T("대출정지일"), _T(""), 0);
			sQuery.Format(	_T("UPDATE CO_LOAN_USER_TBL ")
							_T(			"SET LOAN_STOP_DATE='' ")
							_T(			"WHERE REC_KEY=%s;"), sRecKey);
			pDM->AddFrame( sQuery );
		}

		m_bPrint = TRUE;
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();

		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;

		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("USER_CLASS,LOAN_STOP_DATE");
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		pDM->FreeData();

		if( &ReceiptDlg != NULL )
			delete ReceiptDlg;
		ReceiptDlg = NULL;

		OnCancel();
		return;
	}
}

VOID CFeeLoanStop::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace( _T(","), _T("") );

	CString strBuf = sMoney;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat( NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1 );

	sMoney = pszFormattedNumber;

	if ( pszFormattedNumber )
		delete [] pszFormattedNumber;
}

INT CFeeLoanStop::GetAddTotalHolyDay(INT &nExpDay)
{
EFS_BEGIN
	
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	COleDateTime tStartDate(tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), 0, 0, 0);
	return GetAddTotalHolyDay( nExpDay , tStartDate);	

EFS_END
	return -1;
}
INT CFeeLoanStop::GetAddTotalHolyDay(INT &nExpDay, COleDateTime tStartDate)
{
EFS_BEGIN
	
	COleDateTime tTargetDate;
	
	INT nCnt = nExpDay;
	for( INT i = 0; i <= nCnt; i++ )
	{
		tTargetDate = tStartDate + COleDateTimeSpan( i, 0, 0, 0 );

		if( GetHolyDay( tTargetDate ) )
		{
			nExpDay++;
		}
	}

	return 0;

EFS_END
	return -1;
}
INT CFeeLoanStop::GetHolyDay( COleDateTime tTargetDate )
{
	EFS_BEGIN

	INT nHolyDayCnt = 0;
	CStringList sYearList;
	CStringList sMonthList;
	CStringList sDayList;

	CLibFileManager HolyDayList;
	HolyDayList.GetLibHoliDayInfo( sYearList , sMonthList , sDayList );

	COleDateTime tHolyDate;
	POSITION PosYear , PosMonth , PosDay;
	CStringArray asDay;
	CString sYear , sMon , sDays;
	for( INT i = 0 ; i < sYearList.GetCount() ; i++ )
	{
		PosYear = sYearList.FindIndex(i);
		PosMonth = sMonthList.FindIndex(i);
		PosDay = sDayList.FindIndex(i);

		sYear = sYearList.GetAt(PosYear);
		sMon = sMonthList.GetAt(PosMonth);
		sDays = sDayList.GetAt(PosDay);

		CLocCommonAPI::MakeArrayList( sDays, _T(","), asDay );

		for( INT j = 0 ; j < asDay.GetSize() ; j++ )
		{
			tHolyDate = COleDateTime( _ttoi(sYear) , _ttoi(sMon) , _ttoi(asDay.GetAt(j)) , 0 , 0 , 0 );
			
			if( tHolyDate == COleDateTime( tTargetDate.GetYear() , tTargetDate.GetMonth() , tTargetDate.GetDay() , 0 , 0, 0 ) )
			{
				nHolyDayCnt++;
			}
		}
	}
	return nHolyDayCnt;

	EFS_END
	return -1;
}

HBRUSH CFeeLoanStop::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}