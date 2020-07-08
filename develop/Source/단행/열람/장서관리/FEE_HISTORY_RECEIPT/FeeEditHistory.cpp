// FeeEditHistory.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeEditHistory dialog


CFeeEditHistory::CFeeEditHistory(CESL_Mgr* pParent)
	: CESL_Mgr(CFeeEditHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeEditHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIsChangeSecuriy = _T("");
}
CFeeEditHistory::~CFeeEditHistory()
{
}

VOID CFeeEditHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeEditHistory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeEditHistory, CDialog)
	//{{AFX_MSG_MAP(CFeeEditHistory)
	ON_BN_CLICKED(IDC_BTN_EDIT_DATE_SEARCH, OnBtnEditDateSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeEditHistory message handlers

BOOL CFeeEditHistory::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeEditHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_EDIT_HISTORY") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error!!::FEE_EDIT_HISTORY") );
		return FALSE;
	}

	COleDateTime tCurrent = COleDateTime::GetCurrentTime();
	COleDateTime tFrom( tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), 0, 0, 0 );
	COleDateTimeSpan tWeek( 7, 0, 0, 0 );
	COleDateTime tTo = tFrom - tWeek;
	// TO
	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_DAT_EDIT_DATE1 );
	pDateTo->SetTime( tTo );
	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_DAT_EDIT_DATE2 );
	pDateFrom->SetTime( tFrom );

	// 2010.05.20 ADD BY KSJ : 대출자번호가 주민번호일 경우 개인정보보호정책 옵션(*표시) 적용 여부
	INT ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strIsChangeSecuriy);
	if(0>ids) return FALSE;

	//JOB.2019.0017 ADD BY KYJ
	//개인정보 엑셀 반출,로그 설정값 적용
	//-------------------------------------------------------------------------
	CESL_ControlMgr* pCM =FindCM(_T("CM_FEE_EDIT_HISTORY"));
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("EditGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("수수료 입력 - 대출자 검색");
	}
	//-------------------------------------------------------------------------

	OnBtnEditDateSearch();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

INT CFeeEditHistory::MakeQuery( CString &sQuery )
{
	CString sQuery1, sQuery1_1, sQuery2, sQuery3, sEditDate;
	sEditDate = _T("");

	// 날짜를 저장한다.
	GetDateControl();
	// 1. To, From 둘다 입력된 상태 
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sEditDate.Format( _T("AND CFPI.EDIT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') ")
						  _T( 					    "AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
						  , m_sDateTo, m_sDateFrom );
	}
	// 2. To만 입력된 상태
	else if( !m_sDateTo.IsEmpty() )
	{
		sEditDate.Format( _T("AND CFPI.EDIT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') ")
						  _T( 					    "AND SYSDATE ")
						  , m_sDateTo);
	}
	// 3. From만 입력된 상태
	else if( !m_sDateFrom.IsEmpty() )
	{
		sEditDate.Format( _T("AND CFPI.EDIT_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
					  , m_sDateFrom );
	}

	sQuery1.Format( 
		_T("SELECT CFPI.EDITOR, TO_CHAR(CFPI.EDIT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS EDIT_DATE, ")
		_T("CFPI.EDIT_CONTENT, DECODE(CFPI.CLASS_CODE,'0','연체료','1','회원증재발급','2','대출정지일해제'), ")
		_T("CLU.USER_NO, CLU.IDX_USER_NAME, CFPI.MONEY_PAID, TO_CHAR(CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE, ")
		_T("CL.REG_NO, CL.TITLE, TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD') AS LOAN_DATE, ")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD') AS RETURN_PLAN_DATE, ")
		_T("TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD') AS RETURN_DATE ")
		_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE CL.USER_KEY=CLU.REC_KEY AND CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CFPI.CLASS_CODE IN('0') ")
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("AND CFPI.EDIT_DATE IS NOT NULL ") );
	sQuery1_1.Format( 
		_T("SELECT CFPI.EDITOR, TO_CHAR(CFPI.EDIT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS EDIT_DATE, ")
		_T("CFPI.EDIT_CONTENT, DECODE(CFPI.CLASS_CODE,'0','연체료','1','회원증재발급','2','대출정지일해제'), ")
		_T("CLU.USER_NO, CLU.IDX_USER_NAME, CFPI.MONEY_PAID, TO_CHAR(CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE, ")
		_T("CL.REG_NO, CL.TITLE, TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD') AS LOAN_DATE, ")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD') AS RETURN_PLAN_DATE, ")
		_T("TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD') AS RETURN_DATE ")
		_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE CL.USER_KEY=CLU.REC_KEY AND CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CFPI.CLASS_CODE IN('0') ")
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("AND CFPI.EDIT_DATE IS NOT NULL ") );
	sQuery2.Format(
		_T("SELECT CFPI.EDITOR, TO_CHAR(CFPI.EDIT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS EDIT_DATE, ")
		_T("CFPI.EDIT_CONTENT, DECODE(CFPI.CLASS_CODE,'0','연체료','1','회원증재발급','2','대출정지일해제'), ")
		_T("CLU.USER_NO, CLU.IDX_USER_NAME, CFPI.MONEY_PAID, ")
		_T("TO_CHAR(CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE, ")
		_T("NULL, NULL, NULL, NULL, NULL ")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY AND CFPI.CLASS_CODE IN('1','2') ") 
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("AND CFPI.EDIT_DATE IS NOT NULL ") );
	sQuery3.Format(
		_T("SELECT CFPI.EDITOR,TO_CHAR(CFPI.EDIT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS EDIT_DATE,")
		_T("CFPI.EDIT_CONTENT,DECODE(CFPI.CLASS_CODE,'0','연체료','1','회원증재발급','2','대출정지일해제'),")
		_T("CLU.USER_NO, CLU.IDX_USER_NAME,CFPI.MONEY_PAID,")
		_T("TO_CHAR(CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE,")
		_T("NULL,NULL,NULL,NULL,NULL ")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY AND CFPI.RETRO_YN='Y' ")
		_T("AND CFPI.EDIT_DATE IS NOT NULL ") );
		
	sQuery = sQuery1 + sEditDate + 
			_T("UNION ALL ") + 
			sQuery1_1 + sEditDate + 
			_T("UNION ALL ") +
			sQuery2 + sEditDate + 
			_T("UNION ALL ") + 
			sQuery3 + sEditDate +
			_T("ORDER BY EDIT_DATE DESC");

	return 0;
}

// ** 데이트컨트롤의 값을 가져온다.
VOID CFeeEditHistory::GetDateControl()
{
	SYSTEMTIME	tTime;
	DWORD		dwResult;

	// TO
	CDateTimeCtrl *pDateTo = (CDateTimeCtrl*)GetDlgItem( IDC_DAT_EDIT_DATE1 );
	dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
		m_sDateTo = _T("");
	else
		m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	// FROM
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_DAT_EDIT_DATE2 );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
		m_sDateFrom = _T("");
	else
		m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
}

VOID CFeeEditHistory::OnBtnEditDateSearch() 
{
	INT ids;
	CString sQuery;
	MakeQuery( sQuery );

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_EDIT_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditHistory::OnBtnEditDateSearch() \r\n")
						_T("-Reference Error::DM_FEE_EDIT_HISTORY"), MB_ICONSTOP );
		return;
	}
	ids = pDM->RestructDataManager( sQuery );
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("RestructDataManager Error::수정이력");
		if( ids == -4004 )
			sErrorMsg = _T("RestructDataManager Error::수정이력\r\n-접속되지않음");
		else if( ids == -4005 )
			sErrorMsg = _T("RestructDataManager Error::수정이력\r\n-쿼리실행시 에러발생");
		else if( ids == -4006 )
			sErrorMsg = _T("RestructDataManager Error::수정이력\r\n-쿼리가 너무많음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}

	// 2010.05.20 ADD BY KSJ : 대출자번호가 주민번호일 경우 개인정보보호정책 옵션에 따라 *로 보여준다
	if(_T("Y")!=m_strIsChangeSecuriy)
	{
		INT i, nRowCnt;
		nRowCnt = pDM->GetRowCount();
		for(i=0; i<nRowCnt; i++)
		{
			CString strUserNo;
			ids = pDM->GetCellData(_T("USER_NO"), i, strUserNo);
			if(0>ids) return;

			// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
//DEL 			CString strBuf = strUserNo;
//DEL 			if(13<=strUserNo.GetLength())
//DEL 			{
//DEL 				strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 			}
//DEL 
//DEL 			ids = CLocCommonAPI::CivilNoCheck(strBuf);
//DEL 			if(0>ids) return;
//DEL 			if(0==ids)
//DEL 			{
//DEL 				strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 				if(0>ids) return;
//DEL 
//DEL 				ids = pDM->SetCellData(_T("USER_NO"), strUserNo, i);
//DEL 				if(0>ids) return;
//DEL 			}
			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
			ids = pDM->SetCellData(_T("USER_NO"), strUserNo, i);
			if(0>ids) return;
		}
	}

	ids = ControlDisplay( _T("CM_FEE_EDIT_HISTORY"), _T("EditGrid") );	
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("ControlDisplay Error::CM_FEE_EDIT_HISTORY-EditGrid");
		if( ids == -1 )
			sErrorMsg = _T("ControlDisplay Error::'EditGrid' Grid Control을 찾을 수 없음");
		else if( ids == -1006 )
			sErrorMsg = _T("ControlDisplay Error::'CM_FEE_EDIT_HISTORY' CM을 찾을 수 없음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}
}

HBRUSH CFeeEditHistory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
