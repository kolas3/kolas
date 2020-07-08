// FeeEditLoanStop.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditLoanStop.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\..\..\..\관리\도서관관리\FileManager\LibFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeEditLoanStop dialog


CFeeEditLoanStop::CFeeEditLoanStop(CESL_Mgr* pParent)
	: CESL_Mgr(CFeeEditLoanStop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeEditLoanStop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSuccess = FALSE;
	// KOL.UDF.022
	m_nIndex = 0;
	m_nBasicArrear = 0;
	m_sIsHolyDay = _T("");
	m_strEdtFee = _T("");
}

CFeeEditLoanStop::~CFeeEditLoanStop()
{
}

void CFeeEditLoanStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeEditLoanStop)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDT_LOANSTOP_PAYMENT, m_edtFee);
	DDX_Text(pDX, IDC_EDT_LOANSTOP_PAYMENT, m_strEdtFee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeEditLoanStop, CDialog)
	//{{AFX_MSG_MAP(CFeeEditLoanStop)
	ON_EN_CHANGE(IDC_EDT_LOANSTOP_PAYMENT, OnChangeEdtLoanstopPayment)
	ON_BN_CLICKED(IDC_BTN_LOANSTOP_EDIT, OnBtnLoanstopEdit)
	ON_BN_CLICKED(IDC_BTN_LOANSTOP_CANCEL, OnBtnLoanstopCancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeEditLoanStop message handlers

BOOL CFeeEditLoanStop::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeEditLoanStop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/*
	// 휴관일 대출정지일 포함유무
	GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지휴관일포함유무"), m_sIsHolyDay );

	// 기본연체료
	CString sValue;
	GetManageValue( _T("열람"), _T("공통"), _T("수수료관리"), _T("연체료"), sValue );
	m_nBasicArrear = _ttoi( sValue );

	// 현재일
	m_tCurrentTime = COleDateTime::GetCurrentTime();
	m_tCurrentTime = COleDateTime( m_tCurrentTime.GetYear(), m_tCurrentTime.GetMonth(), m_tCurrentTime.GetDay(), 0, 0, 0 );
	*/
	InitControl();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeeEditLoanStop::OnChangeEdtLoanstopPayment() 
{
	UpdateData(TRUE);

	m_strEdtFee.Replace( _T(","), _T("") );

	CString strBuf = m_strEdtFee;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	m_strEdtFee = pszFormattedNumber;

	if (pszFormattedNumber)
	delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(nLen, -1);
}

// **컨트롤의 초기화
VOID CFeeEditLoanStop::InitControl()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditArrear::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	CString sPayment;
	pDM->GetCellData( _T("납부금액"), m_nIndex, sPayment );
	sPayment.Replace( _T(","), _T("") );
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_PAYMENT))->SetWindowText( sPayment );
	((CEdit*)GetDlgItem( IDC_EDT_LOANSTOP_PAYMENT ))->SetFocus();
	//납부금액에 블럭잡기
	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(0, nLen);

	/*
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditLoanStop::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	CString sPayment, sUserNO, sUserName, sPaymentDate, sUserKey;
	CString sQuery, sBeforeLoanStopDate, sAfterLoanStopDate, sStopDay, sTotalStopDay; 
	// 납부금액
	pDM->GetCellData( _T("납부금액"), m_nIndex, sPayment );
	sPayment.Replace( _T(","), _T("") );
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_PAYMENT))->SetWindowText( sPayment );
	((CEdit*)GetDlgItem( IDC_EDT_LOANSTOP_PAYMENT ))->SetFocus();
	// 납부금액에 블럭잡기
	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(0, nLen);

	// 대출자번호
	pDM->GetCellData( _T("대출자번호"), m_nIndex, sUserNO );
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_USER_NO))->SetWindowText( sUserNO );
	// 대출자명
	pDM->GetCellData( _T("대출자명"), m_nIndex, sUserName );
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_USER_NAME))->SetWindowText( sUserName );
	// 납부일자
	pDM->GetCellData( _T("납부일자"), m_nIndex, sPaymentDate );
	sPaymentDate = sPaymentDate.Left(10);
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_PAYMENT_DATE))->SetWindowText( sPaymentDate );
	m_tPaymentDate = COleDateTime( _ttoi(sPaymentDate.Mid(0,4)), _ttoi(sPaymentDate.Mid(5,2)), _ttoi(sPaymentDate.Mid(8,2)), 0, 0, 0 );

	pDM->GetCellData( _T("LOAN_INFO_KEY"), m_nIndex, sUserKey );
	// 현재대출정지일
	sQuery.Format(	_T("SELECT LOAN_STOP_DATE FROM CO_LOAN_USER_TBL ")
					_T("WHERE REC_KEY=%s"), sUserKey );
	pDM->GetOneValueQuery( sQuery, sBeforeLoanStopDate );
	if( sBeforeLoanStopDate.IsEmpty() ) 
	{
		GetDlgItem(IDC_EDT_LOANSTOP_BEFORE_LOANSTOPDATE)->SetWindowText( _T("없음") );
	}
	else
	{
		GetDlgItem(IDC_EDT_LOANSTOP_BEFORE_LOANSTOPDATE)->SetWindowText( sBeforeLoanStopDate );
	}

	// 정지일
	INT nStopDay = _ttoi(sPayment) / m_nBasicArrear;
	sStopDay.Format( _T("%d"), nStopDay );
	GetDlgItem(IDC_EDT_LOANSTOP_STOP_DAY)->SetWindowText( sStopDay );

	// 휴관일
	// 대출정지일에 휴관일 포함여부에 따른 계산
	CString sHolyDay;
	if( m_sIsHolyDay == _T("Y") )
	{// 휴관일 포함
		nStopDay--;
		INT nValue = nStopDay;
		INT ids = GetAddTotalHolyDay( nValue, m_tPaymentDate );
		if( ids < 0 ) OnCancel();
		INT nHolyDay = nValue - nStopDay;
		sHolyDay.Format( _T("%d"), nHolyDay );
	}
	else
	{// 휴관일포함안함
		sHolyDay = _T("적용안함");
	}
	GetDlgItem(IDC_EDT_LOANSTOP_HOLYDAY)->SetWindowText( sHolyDay );

	// 총정지일
	sTotalStopDay.Format( _T("%d"), _ttoi( sStopDay ) + _ttoi( sHolyDay ) );
	GetDlgItem(IDC_EDT_LOANSTOP_TOTAL_STOP_DAY)->SetWindowText( sTotalStopDay );

	// 수정후 대출정지일
	COleDateTimeSpan tStopDay( _ttoi( sTotalStopDay )-1, 0, 0, 0 );
	COleDateTime tLoanStopDate;
	tLoanStopDate = m_tPaymentDate + tStopDay;
	sAfterLoanStopDate = GetLoanStopDate( m_tCurrentTime, sBeforeLoanStopDate, tLoanStopDate );
	GetDlgItem(IDC_EDT_LOANSTOP_AFTER_LOANSTOPDATE)->SetWindowText( sAfterLoanStopDate );
	*/
}

// ** 오늘 + 예상 대출기한 의 날이 휴일을 경우 휴일이 아닌날까지 더해준다.
INT CFeeEditLoanStop::GetAddTotalHolyDay( INT &nExpDay )
{
EFS_BEGIN
	
	COleDateTime tStartDate = COleDateTime::GetCurrentTime();
	tStartDate = COleDateTime( tStartDate.GetYear(), tStartDate.GetMonth(), tStartDate.GetDay(), 0, 0, 0 );
	return GetAddTotalHolyDay( nExpDay, tStartDate );

EFS_END
return -1;
}

// ** 타겟날짜 + 예상 대출기한 의 날이 휴일을 경우 휴일이 아닌날까지 더해준다.
INT CFeeEditLoanStop::GetAddTotalHolyDay( INT &nExpDay, COleDateTime tStartDate )
{
EFS_BEGIN
	
	COleDateTime tTargetDate;
	
	// * 기존 휴관일 구하는 방법은 기준이 되는 날짜부터(tStartDate)부터 정지일(nExpDay)까지 하루씩 계산하여
	//	휴관일이 나오면 정지일을+1 하여 나중에 나오는 값이 대출정지일수가 된다.
	// * 대출정지일해제는 이미 대출정지일이 정해져있고 기준이 되는 날짜(tStartDate)와 정지일까지의 
	//	휴관일이 이미 계산되어 있다. 그러므로 하루씩 계산하며 휴관일이 나와서+1을 해주지 않는다.

	for( INT i = 0; i <= nExpDay; i++ )
	{
		tTargetDate = tStartDate + COleDateTimeSpan( i, 0, 0, 0 );
		
		//CString sTmp;
		//sTmp.Format("%04d/%02d/%02d", tTargetDate.GetYear(), tTargetDate.GetMonth(), tTargetDate.GetDay() );

		if( GetHolyDay( tTargetDate ) )
		{
			nExpDay++;
		}
	}

	return 0;

EFS_END
	return -1;
}

// ** 타겟날짜까지의 휴관일을 구한다.
INT CFeeEditLoanStop::GetHolyDay( COleDateTime tTargetDate )
{
EFS_BEGIN

	INT nHolyDayCnt = 0;
	CStringList sYearList;
	CStringList sMonthList;
	CStringList sDayList;

	CLibFileManager HolyDayList;
	HolyDayList.GetLibHoliDayInfo( sYearList, sMonthList, sDayList );

	COleDateTime tHolyDate;
	POSITION PosYear, PosMonth, PosDay;
	CStringArray asDay;
	CString sYear, sMon, sDays;
	for( INT i = 0; i < sYearList.GetCount(); i++ )
	{
		PosYear = sYearList.FindIndex(i);
		PosMonth = sMonthList.FindIndex(i);
		PosDay = sDayList.FindIndex(i);

		sYear = sYearList.GetAt(PosYear);
		sMon = sMonthList.GetAt(PosMonth);
		sDays = sDayList.GetAt(PosDay);

		CLocCommonAPI::MakeArrayList( sDays, _T(","), asDay );

		for( INT j = 0; j < asDay.GetSize(); j++ )
		{
			tHolyDate = COleDateTime( _ttoi(sYear), _ttoi(sMon), _ttoi(asDay.GetAt(j)), 0, 0, 0 );
			
			if( tHolyDate == COleDateTime( tTargetDate.GetYear(), tTargetDate.GetMonth(), tTargetDate.GetDay(), 0, 0, 0 ) )
			{
				nHolyDayCnt++;
			}
		}
	}
	return nHolyDayCnt;

EFS_END
return -1;
}

// ** 취소하여 적용해야할 대출정지일을 리턴한다.
// 계산된 대출정지일, 현재일, 기존대출정지일을 이용하여 부여할 대출정지일을 구한다.
CString CFeeEditLoanStop::GetLoanStopDate( COleDateTime tCurrentTime, CString sBeforeLoanStopDate, 
											COleDateTime tLoanStopDate )
{
	CString sLoanStopDate;

	// 기존대출정지일이 존재하는가?
	if( !sBeforeLoanStopDate.IsEmpty() )
	{
		COleDateTime tBeforeLoanStopDate(	_ttoi( sBeforeLoanStopDate.Mid(0,4) ), 
											_ttoi( sBeforeLoanStopDate.Mid(5,2) ), 
											_ttoi( sBeforeLoanStopDate.Mid(8,2) ), 0, 0, 0 );
		// 현재일 <= 기존대출정지일
		if( tCurrentTime <= tBeforeLoanStopDate )
		{
			// 기존대출정지일 <= 계산된 정지일
			if( tBeforeLoanStopDate <= tLoanStopDate )
			{
				// 적용할 대출정지일 = 계산된 정지일
				sLoanStopDate = tLoanStopDate.Format( _T("%Y/%m/%d") );
			}
			else
			{
				// 적용할 대출정지일 = 기존대출정지일
				sLoanStopDate = tBeforeLoanStopDate.Format( _T("%Y/%m/%d") );
			}
		}
		else
		{
			// 적용할 대출정지일 = 없음
			sLoanStopDate = _T("");
		}
	}
	else
	{//기존대출정지일이 존재하지 않는다.
		// 현재일 <= 계산된 정지일
		if( tCurrentTime <= tLoanStopDate )
		{
			// 적용할 대출정지일 = 계산된 정지일
			sLoanStopDate = tLoanStopDate.Format( _T("%Y/%m/%d") );
		}
		else
		{
			// 적용할 대출정지일 = 없음
			sLoanStopDate = _T("");
		}
	}

	return sLoanStopDate;
}

void CFeeEditLoanStop::OnBtnLoanstopEdit() 
{
	INT ids;
	m_bSuccess = FALSE;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditReCard::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	// 자료수집
	CString sPaymentKey, sPayment, sMoneyPaid, sEditor, sEditContent;
	pDM->GetCellData( _T("PAYMENT_KEY"), m_nIndex, sPaymentKey );
	pDM->GetCellData( _T("납부금액"), m_nIndex, sPayment );
	((CEdit*)GetDlgItem(IDC_EDT_LOANSTOP_PAYMENT))->GetWindowText( sMoneyPaid );
	sEditor.Format( _T("%s::%s"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP );
	sEditContent.Format( _T("납부금액:%s원->%s원")
						, sPayment, sMoneyPaid );

	sPayment.Replace( _T(","), _T("") );
	sMoneyPaid.Replace( _T(","), _T("") );

	CString sLastWork = GetWorkLogMsg( _T("대출정지일해제수수료수정"), __WFILE__, __LINE__ );

	// 쿼리생성
	CString sQuery;
	sQuery.Format(	_T("UPDATE CO_FEE_PAYMENT_INFO_TBL ")
					_T("SET MONEY_PAID=%s, EDIT_DATE=SYSDATE, ")
					_T("EDITOR='%s', EDIT_CONTENT='%s', LAST_WORK='%s' ")
					_T("WHERE REC_KEY=%s;")
					, sMoneyPaid, sEditor, sEditContent, sLastWork, sPaymentKey );

	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);
	ids = pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("SendFrame Error::대출정지일해제수정");
		if( ids == -4004 )
			sErrorMsg = _T("SendFrame Error::대출정지일해제수정\r\n-접속되지않음");
		else if( ids == -4005 )
			sErrorMsg = _T("SendFrame Error::대출정지일해제수정\r\n-쿼리실행시 에러발생");
		else if( ids == -4999 )
			sErrorMsg = _T("SendFrame Error::대출정지일해제수정\r\n-아직 Oracle 를 지원하지 않음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );

		// UPDATE 실패
		m_bSuccess = FALSE;
		return;
	}

	// UPDATE 성공
	m_bSuccess = TRUE;
	OnCancel();
}

void CFeeEditLoanStop::OnBtnLoanstopCancel() 
{
	OnCancel();	
}

BOOL CFeeEditLoanStop::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			HWND MsgH = pMsg->hwnd;
			if( GetDlgItem(IDC_EDT_LOANSTOP_PAYMENT)->m_hWnd == MsgH )
			{
				GetDlgItem(IDC_BTN_LOANSTOP_EDIT)->SetFocus();
			}
			else if( GetDlgItem(IDC_BTN_LOANSTOP_EDIT)->m_hWnd == MsgH )
			{
				OnBtnLoanstopEdit();
			}
			else if( GetDlgItem(IDC_BTN_LOANSTOP_CANCEL)->m_hWnd == MsgH )
			{
				OnCancel();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CFeeEditLoanStop::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
