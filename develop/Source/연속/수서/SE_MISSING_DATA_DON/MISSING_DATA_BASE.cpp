// MISSING_DATA_BASE.cpp : implementation file
//

#include "stdafx.h"
#include "MISSING_DATA_BASE.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_BASE dialog


CMISSING_DATA_BASE::CMISSING_DATA_BASE( CESL_Mgr* pParent  /*=NULL*/, CString f_strMenuAlias )
	: CESL_Mgr(IDD, pParent)
{
	m_pDlg = NULL;
	m_pMailMgr = NULL;

	if     ( f_strMenuAlias == _T("독촉(구입)") )	m_sAcqCode = _T('1');
	else if( f_strMenuAlias == _T("독촉(기증)") )	m_sAcqCode = _T('2');
}

CMISSING_DATA_BASE::~CMISSING_DATA_BASE()
{
	if(m_pMailMgr)
	{
		delete m_pMailMgr;
		m_pMailMgr = NULL;
	}

	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}

}

BOOL CMISSING_DATA_BASE::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMISSING_DATA_BASE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMISSING_DATA_BASE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMISSING_DATA_BASE, CDialog)
	//{{AFX_MSG_MAP(CMISSING_DATA_BASE)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_BASE message handlers

BOOL CMISSING_DATA_BASE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_SE_MISSING_BASE_DON"))<0)
	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	//test
	if(!m_pDlg)
	{
		m_pDlg = new CMISSING_DATA_SEARCH(this);
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMISSING_DATA_BASE::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_LIST_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CMISSING_DATA_BASE::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}

VOID CMISSING_DATA_BASE::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	m_Query			= m_pDlg->m_strQuery;
	m_sSearchCode	= m_pDlg->m_sSearchCode;

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_MISSING_DON_01"), -1);
		if( ids ) return ;

		m_sSearchCode = _T("");

		AfxMessageBox(_T("검색결과가 없습니다."));
		return ;
	}

	m_pDlg->ShowWindow(SW_HIDE);
}

// optionchange & sorting
INT CMISSING_DATA_BASE::BaseDisplay() 
{
	INT i, ids, nRowCnt;
	CString sCurKey, sPrevKey;

	CString sSpeciesKey, sBindVolCnt;
	//CString sQuery, sData, sKey1, sKey2;

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	// ===========================================================================================================
	// - m_sSearchCode = '1' 인경우, 
	//
	//  1. 최근에 독촉된 자료만 골라낸다.
	//  2. 독촉대상인지 판단 : NVL(C.CLAIM_DATE,A.ACQ_EXP_DATE) + M.CLAIM_INTERVAL < 오늘 날짜  
	// ===========================================================================================================
	if( m_sSearchCode == '1' ) {
		
		// 1. 최근에 독촉된 자료만 골라낸다.
		for( i = nRowCnt-1 ; i >= 0 ; i-- ) {
		
			ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("ACQ_TYPE_VOL_KEY"), sCurKey, i );
			if( ids < 0 ) return -3;

			if( i == nRowCnt-1 ) {
				sPrevKey = sCurKey;
				continue;
			}

			// 한자료에 여러번 독촉이 되었다면... 가장 최근에 독촉된 자료만 독촉대상이 된다...
			if( sCurKey != _T("") && sPrevKey == sCurKey ) {
			
				ids = pDataMgr->DeleteRow(i+1);
				if( ids < 0 ) return -4;
			}

			sPrevKey = sCurKey;
		}

		// 2. 독촉대상인지 체크한다.
		nRowCnt = pDataMgr->GetRowCount();
		
		//오늘 날짜를 미리 구하는 FLOW로 변경
		INT yyyy, mm, dd;
		CTime time = CTime::GetCurrentTime();
		CString sTodayDate;
		sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

		_stscanf( (TCHAR*)LPCTSTR(sTodayDate), _T("%d/%d/%d"), &yyyy, &mm, &dd );
		COleDateTime tToday(yyyy, mm, dd, 0,0,0);

		for( i = nRowCnt-1 ; i >= 0 ; i-- ) {
			
			if( CheckClaimYn(i,tToday) ) {
				ids = pDataMgr->DeleteRow(i);
				if( ids < 0 ) return -4;
			}
		}
	}

	// 표제, 권호명으로 sorting
	if( nRowCnt > 0 ) {
		
		INT nCondColumns = 0;
		CString strAlias[32];
		INT nCond[32];

		strAlias[nCondColumns] = _T("TITLE_INFO"); nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC; nCondColumns++;
		strAlias[nCondColumns] = _T("VOL_TITLE"); nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC; nCondColumns++;

		pDataMgr->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	}

	ids = AllControlDisplay( _T("CM_SE_MISSING_DON_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d 건의 자료가 검색되었습니다.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

BOOL CMISSING_DATA_BASE::CheckClaimYn( INT f_nIdx, COleDateTime tToday)
{
	// =============================================================================================
	// 독촉대상 : C.CLAIM_DATE + M.CLAIM_INTERVAL < 오늘 날짜  return FALSE;
	//            = 0 < 오늘 날짜 - M.CLAIM_INTERVAL - C.CLAIM_DATE
	// 독촉자료 : return TRUE
	// =============================================================================================
	INT ids, nClaimInterval;
    INT yyyy, mm, dd;
	CString sTodayDate, sAcqExpDate, sClaimDate, sClaimInterval;

	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("CLAIM_DATE"), sClaimDate, f_nIdx );
	if( ids < 0 ) return -1;

	if( sClaimDate == _T("") ) {
		ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("ACQ_EXP_DATE"), sAcqExpDate, f_nIdx );
		if( ids < 0 ) return -1;

		sClaimDate = sAcqExpDate;
	}

	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("CLAIM_INTERVAL"), sClaimInterval, f_nIdx );
	if( ids < 0 ) return -2;

	//if( sClaimDate == _T("") ) return FALSE;

	nClaimInterval = _ttoi( (TCHAR*)LPCTSTR(sClaimInterval) );
	
	// =================================================================================================
	// 오늘날짜를 구한다.
	// =================================================================================================
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return -1;
	
	/*
	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

    _stscanf( (TCHAR*)LPCTSTR(sTodayDate), _T("%d/%d/%d"), &yyyy, &mm, &dd );
	COleDateTime tToday(yyyy, mm, dd, 0,0,0);
	*/

    _stscanf( (TCHAR*)LPCTSTR(sClaimDate), _T("%d/%d/%d"), &yyyy, &mm, &dd );
	COleDateTime tClaimDate(yyyy, mm, dd, 0,0,0);
	
	tToday -= COleDateTimeSpan ( nClaimInterval, 0, 0, 0 );
	
	if( tToday.m_dt - tClaimDate.m_dt >= 0 ) 
		return FALSE;
	else
		return TRUE;
}

INT CMISSING_DATA_BASE::DoProc() 
{
	INT i, ids, nRowCnt, nIdx;
	CString sRecKey, sSql;

	if( m_sSearchCode == _T("2") ) {
		
		AfxMessageBox( _T("선택된 자료는 이미 의뢰된 자료 입니다.") );
		return 0;
	}

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_DON_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList();
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("자료를 선택하십시오.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("그리드의 선택정보를 가져오는데 실패하였습니다.") );
		return -3;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return -4;

	ids = pDataMgr->StartFrame();
	if( ids < 0 ) return -5;

	for( i = nRowCnt-1 ; i >= 0 ; i-- ) {

		if( i == nRowCnt-1 ) {
			nIdx = pGrid->SelectGetTailPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetPrev();
			if( nIdx == -1 ) break;
		}

		ids = ProcClaim(nIdx);
		if( ids ) return -6;
	}

	ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) {
		AfxMessageBox( _T("의뢰처리를 실패하였습니다.") );
		return -9;
	}

	//JOB.2019.0073 : 충돌 License
	// 메일전송
// 	CString sVal;
// 	GetManageValue( _T("수서"), _T("연속"), _T("독촉메일자동발송여부"), _T(""), sVal );
// 	
// 	if( sVal == _T("Y") ) DoSendMail();

	CString sMsg;
	sMsg.Format( _T("%d개의 자료를 의뢰처리 하였습니다."), nRowCnt );
	AfxMessageBox(sMsg);

	ids = BaseDisplay();
	if( ids < 0 ) {

		AfxMessageBox( _T("Display Error!") );
		return -11;
	}

	return 0;
}

// SE_CLAIM_HISTORY_TBL 생성
INT CMISSING_DATA_BASE::ProcClaim(INT f_nIdx)
{
	INT ids;
	
	CString sRecKey, sAcqType, sAcqTypeVolKey, sMissBookCnt, sClaimSeq, sClaimDate, sMailSendDate;

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return -1;

	// rec_key 생성
	ids = pDataMgr->MakeRecKey( sRecKey );
	if(ids < 0) return -2;

	// ACQ_TYPE
	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("ACQ_CODE"), sAcqType, f_nIdx );
	if(ids < 0) return -3;

	// ACQ_TYPE_VOL_KEY
	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("REC_KEY"), sAcqTypeVolKey, f_nIdx );
	if(ids < 0) return -4;

	// ACQ_TYPE_VOL_KEY
	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("REC_KEY"), sAcqTypeVolKey, f_nIdx );
	if(ids < 0) return -5;

	// MISS_BOOK_CNT
	ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("MISS_BOOK_CNT"), sMissBookCnt, f_nIdx );
	if(ids < 0) return -6;
	
	// CLAIM_SEQ
	CString sSql, sData;
	sSql.Format( _T("SELECT NVL(MAX(CLAIM_SEQ),0) FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s"), 
				 sAcqTypeVolKey );
	ids = pDataMgr->GetOneValueQuery( sSql, sData );
	if(ids < 0) return -7;
	
	sClaimSeq.Format( _T("%d") , _ttoi( (TCHAR*)LPCTSTR(sData) ) + 1 );
	

	// CLAIM_DATE
	CString sTodayDate;
	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	sClaimDate = sTodayDate;

	// MAIL_SEND_DATE
	sMailSendDate = sTodayDate;


	// ...............................................................................................
	// Query DB Field를 초기화 한다.
	// ...............................................................................................
	pDataMgr->InitDBFieldData();

	// ...............................................................................................
	// Query 에 DB Field를 추가한다.
	// ...............................................................................................
	ids = pDataMgr->AddDBFieldData( _T("REC_KEY")			, _T("STRING")	, sRecKey);
	if(ids < 0) return -2;
	ids = pDataMgr->AddDBFieldData( _T("ACQ_CODE")			, _T("STRING")	, sAcqType);
	if(ids < 0) return -3;
	ids = pDataMgr->AddDBFieldData( _T("ACQ_TYPE_VOL_KEY")	, _T("STRING")	, sAcqTypeVolKey);
	if(ids < 0) return -4;
	ids = pDataMgr->AddDBFieldData( _T("MISS_BOOK_CNT")		, _T("STRING")	, sMissBookCnt);
	if(ids < 0) return -5;
	ids = pDataMgr->AddDBFieldData( _T("CLAIM_SEQ")			, _T("STRING")	, sClaimSeq);
	if(ids < 0) return -6;
	ids = pDataMgr->AddDBFieldData( _T("CLAIM_DATE")		, _T("STRING")	, sClaimDate);
	if(ids < 0) return -7;

	CString sCurDonator = _T("");
	sCurDonator = pDataMgr->GetCellData(_T("DONATOR_KEY"), f_nIdx);
	if(!sCurDonator.IsEmpty())
	{
		ids = pDataMgr->AddDBFieldData( _T("MAIL_SEND_DATE")	, _T("STRING")	, sMailSendDate);
		if(ids < 0) return -8;
	}
	
	ids = pDataMgr->MakeInsertFrame( _T("SE_CLAIM_HISTORY_TBL") );
	if( ids < 0 ) return -9;

	return 0;
}

INT CMISSING_DATA_BASE::DoSendMail()
{
	//JOB.2019.0073 : 충돌 Licen
// 	INT i, ids, nResult;
// 	INT nIdx, nRowCnt, nDonatorCnt;
// 	
// 	CString sMsg;
// 	CString sPrevDonator, sCurDonator;
// 	CArray <CString, CString> sDonatorList;
// 
// 	sDonatorList.RemoveAll();
// 	nDonatorCnt = 0;
// 	sPrevDonator = sCurDonator = _T("");
// 
// 	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_DON_01") );
// 	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;
// 
// 	ids = pGrid->SelectMakeList();
// 	if( ids ) {
// 		AfxMessageBox( _T("SelectMakeList Error!") );
// 		return -1;
// 	}
// 
// 	nRowCnt = pGrid->SelectGetCount();
// 
// 	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
// 	if( pDataMgr == NULL ) return -4;
// 	
// 	CArray <INT, INT> nNonInfoList;
// 
// 	for( i = 0 ; i < nRowCnt ; i++ ) {
// 
// 		if( i == 0 ) {
// 			nIdx = pGrid->SelectGetHeadPosition();
// 			if( nIdx == -1 ) break;
// 		}
// 		else {
// 			nIdx = pGrid->SelectGetNext();
// 			if( nIdx == -1 ) break;
// 		}
// 	
// 		sCurDonator = pDataMgr->GetCellData(_T("DONATOR_KEY"),nIdx);
// 
// 		if( sCurDonator.IsEmpty() ) {
// 			nNonInfoList.Add(i+1);
// 
// 		}
// 		
// 		if(!sCurDonator.IsEmpty())
// 		{
// 			if(IsNewStringInArray(sDonatorList,sCurDonator))
// 				sDonatorList.Add( sCurDonator );
// 		}
// 
// 	}
// 
// 	if( 0 < nNonInfoList.GetSize() )
// 	{
// 		sMsg.Format(_T("구입처 정보가 없는 %d개 항목은 메일발송이 취소됩니다."), nNonInfoList.GetSize() );
// 		AfxMessageBox(sMsg);
// 	}
// 
// 	if( sDonatorList.GetSize() <= 0 )
// 		return 0;
// 
// 	m_pMailMgr = new CSendMailMgr(this);
// 	
// 	SendMailRecord *pMailRecord;
// 	pMailRecord = new SendMailRecord[sDonatorList.GetSize()];
// 
// 	for( i = 0 ; i < sDonatorList.GetSize() ; i++ ) {
// 
// 		ids = MakeMailRecord( pMailRecord[nDonatorCnt], &sDonatorList, i );
// 		
// 		if(ids<0) 
// 		{
// 			if(m_pMailMgr)
// 			{
// 				delete m_pMailMgr;
// 				m_pMailMgr = NULL;
// 			}
// 			
// 			if(pMailRecord)
// 			{
// 				delete [] pMailRecord;
// 				pMailRecord = NULL;
// 			}
// 			
// 			return -1;
// 		}
// 
// 		// Add MailRecord
// 		m_pMailMgr->AddMailRecord( &pMailRecord[nDonatorCnt] );
// 	}
// 
// 	// send mail
// 	INT recordCnt = m_pMailMgr->GetCountMailRecord();
// 	if(recordCnt <= 0) {
// 
// 		if(m_pMailMgr)
// 		{
// 			delete m_pMailMgr;
// 			m_pMailMgr = NULL;
// 		}
// 			
// 		if(pMailRecord)
// 		{
// 			delete [] pMailRecord;
// 			pMailRecord = NULL;
// 		}
// 		
// 		return -1001;
// 	}
// 
// 	m_pMailMgr->DoModal(MAIL_SERVER_CFGFILE);
// 
// 	for( i = 0 ; i < recordCnt ; i++ ) {
// 		
// 		nResult = m_pMailMgr->GetMailRecordSendState(i);
// 		if(nResult == 0)	{
// 		}
// 	}
// 
// 	if(m_pMailMgr)
// 	{
// 		delete m_pMailMgr;
// 		m_pMailMgr = NULL;
// 	}
// 			
// 	if(pMailRecord)
// 	{
// 		delete [] pMailRecord;
// 		pMailRecord = NULL;
// 	}

	return 0;
}

INT CMISSING_DATA_BASE::MakeMailRecord( SendMailRecord	&f_MailRecord, CArray <CString, CString> * f_sDonatorList, INT f_nIdx )
{
	INT ids;
	CString sSender, sBody, sHeader, sFooter;
	CString sProvider_Name, sProvider_Emailaddr, sProvider_Tel, sProvider_Addr;
	CString sSql, sDonatorKey;

	sDonatorKey = f_sDonatorList->GetAt(f_nIdx);

	CESL_DataMgr tmpDM;
	CESL_DataMgr *pDataMgr = FindDM( _T("DM_SE_MISSING_DON_01") );
	
	tmpDM.SetCONNECTION_INFO(pDataMgr->CONNECTION_INFO);
	
	sSql.Format(_T("SELECT DONATOR, EMAIL, TEL, ADDRESS FROM CO_DONATOR_TBL WHERE REC_KEY = '%s'"), sDonatorKey );
	
	ids = tmpDM.RestructDataManager(sSql);
	if(ids < 0) return -1000;

	tmpDM.GetCellData(0, 0, sProvider_Name);
	tmpDM.GetCellData(0, 1, sProvider_Emailaddr);
	tmpDM.GetCellData(0, 2, sProvider_Tel);
	tmpDM.GetCellData(0, 3, sProvider_Addr);

	f_MailRecord.m_nFileAttachCnt	= 0;
	f_MailRecord.m_pFileAttach		= _T("");
	f_MailRecord.m_sEncoding		= _T("text/html");
	f_MailRecord.m_sRecipientAddress= sProvider_Emailaddr;  //_T(sProvider_Emailaddr);

	f_MailRecord.m_sRecipientName	= sProvider_Name;// _T("");
	f_MailRecord.m_sRecipientAddress= sProvider_Emailaddr;
	f_MailRecord.m_sCCName			= _T(""); //sProvider_Name;       //_T(sProvider_Name);
	f_MailRecord.m_sCCAddress		= sProvider_Addr;       //_T(sProvider_Addr);
	f_MailRecord.m_sSubject			= _T("의뢰메일");

	// 2009.03.03 ADD BY CJY : 데몬전송을 위해 메일키와 클래스를 받아온다.
	{
		CString strAlias;

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));
		CString strSQL;
		strSQL.Format(
			_T("SELECT MM.REC_KEY, MM.MAIL_CLASS ")
			_T("FROM MN_MAIL_TBL MM, MN_MAIL_GROUP_TBL MG ")
			// 2009.11.30 UPDATE BY KSJ : 검색 조건에 관리구분 추가
			//DEL _T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '의뢰메일'")
			_T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '의뢰메일' AND MG.MANAGE_CODE=UDF_MANAGE_CODE")
			);
		ids = TmpDM.RestructDataManager(strSQL);
		if(0 > ids)
		{
			return -1000;
		}

		f_MailRecord.m_sMailInfoKey = TmpDM.GetCellData(0, 0);
		f_MailRecord.m_sMailClass = TmpDM.GetCellData(0, 1);			
	}

	// header
	//도서관 이름 
	sSender = GetLIBInfo(pDataMgr);
	sHeader	= _T("의뢰메일");
	sFooter = _T("다음의 자료들을 의뢰합니다.");

	// body
	ids = MakeMailBody(
						pDataMgr, 
						sDonatorKey, 
						sBody, 
						sProvider_Name, 
						sProvider_Tel, 
						sProvider_Addr, 
						sSender, 
						sHeader, 
						sFooter);

	f_MailRecord.m_sBody = sBody;

	return 0;
}

INT CMISSING_DATA_BASE::MakeMailBody(
										  CESL_DataMgr *pDM, 
										  CString		DonatorKey, 
										  CString		&body, 
										  CString		ProviderName, 
										  CString		Provider_Tel, 
										  CString		Provider_Addr, 
										  CString		sender, 
										  CString		header, 
										  CString		footer		)
{
	INT ids, nRowCnt, nIdx;
	CString	sCurDonatorKey;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_DON_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	ids = pGrid->SelectMakeList() ;
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return -1;
	}

	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("자료를 선택하십시오.") );
		return -2;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("그리드의 선택정보를 가져오는데 실패하였습니다.") );
		return -3;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return -6;

	INT nDonatorCnt = 0;
	CString tmpstr;

	// ===========================================================================================================
	// 같은구입처 count
	// ===========================================================================================================
	for( INT i = 0 ; i < nRowCnt ; i++ ) {

		if( i == 0 ) {
			nIdx = pGrid->SelectGetHeadPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetNext();
			if( nIdx == -1 ) break;
		}

		ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("DONATOR_KEY"), sCurDonatorKey, nIdx );
	
		if( ids ) return -1;
		if( DonatorKey == sCurDonatorKey ) nDonatorCnt++;
	}

	body.GetBuffer(1000000);
	
	
	body +=	_T("<html>\r\n")
			_T("<head>\r\n")
			_T("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=EUC-KR\">\r\n");
	tmpstr.Format(_T("<title>%s</title>\r\n"), header);
	body +=	tmpstr;
	body +=	_T("<style TYPE=\"text/css\">\r\n")
			_T("<!--	td {font-size:9pt; font-face:굴림}\r\n")
			_T("	font {font-face:굴림}\r\n")
			_T("	.mid {font:12pt 굴림; color=\"#003399\"}-->\r\n")
			_T("</style>\r\n")
			_T("</head>\r\n")
			_T("<body leftmargin=\"0\" topmargin=\"0\" marginwidth=\"0\" marginheight=\"0\" text=\"#000000\" bgcolor=ffffff link=\"#006699\" alink=\"#996600\" vlink=\"#996600\">\r\n")
			_T("<br>\r\n")
			_T("<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\" align=\"center\" width=300 bordercolor=\"#666666\">\r\n")
			_T("<tr> \r\n");
	tmpstr.Format(_T("<td align=\"center\" bgcolor=\"#dddddd\"><font size=3>%s</font></td>\r\n"), header);
	body += tmpstr;
	body += _T("</tr>\r\n")
			_T("</table>\r\n")
			_T("<br><br>\r\n")
			_T("<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=750 align=\"center\">\r\n")
			_T("<tr>	\r\n")
			_T("<td align=left valign=bottom> <br>\r\n")
			_T("  <table width=\"365\" border=\"1\" cellspacing=\"0\" cellpadding=\"5\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td rowspan=\"3\" width=\"12\">공<br>\r\n")
			_T("		급<br>\r\n")
			_T("		처</td>\r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">상 호</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), ProviderName);
	body += tmpstr;
	body += _T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">주 소</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), Provider_Addr);
	body += tmpstr;
	body +=	_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">전화번호</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), Provider_Tel);
	body += tmpstr;
	body += _T("	</tr>\r\n")
			_T("  </table>\r\n")
  			_T("</td>\r\n");
	tmpstr.Format(_T("<td align=right valign=bottom>			발송일자 : &nbsp;%s<br><br>\r\n"), GetTodayDate());
	body += tmpstr;
	body += _T("<table width=\"200\" border=\"1\" cellspacing=\"0\" cellpadding=\"5\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td rowspan=\"3\" width=\"12\">주문자</td>\r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">성명</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">&nbsp;%s</div>\r\n"), sender);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\" height=\"26\"> \r\n")
			_T("		<div align=\"center\">주문일자</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\" height=\"26\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">&nbsp;%s</div>\r\n"), GetTodayDate());
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">독촉권수</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">%d 권</div>\r\n"), nDonatorCnt);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("  </table>\r\n")
			_T("</td>\r\n")
			_T("</tr></table><br><center>\r\n")
			_T("<font size=3><br>\r\n");
	tmpstr.Format(_T("%s</font>\r\n"), footer);
	body += tmpstr;
	body +=	_T("</center>\r\n")
			_T("<br>\r\n")
			_T("<table width=\"750\" border=\"1\" cellspacing=\"0\" cellpadding=\"1\" bordercolor=\"#666666\" align=\"center\">\r\n")
			_T("<tr> \r\n")
			_T("<td width=\"17\" bgcolor=\"#eeeeee\"> \r\n")
			_T("  <div align=\"center\">번호</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"224\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">도서명</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"224\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">출판사</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">권호명</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">부차적권호명</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">입수예측일</div>\r\n")
			_T("</td>\r\n")
			_T("</tr>\r\n");
	
	INT nSeq = 0;		// 번호
	CString tmpstr1;
	
	for( i = 0 ; i < nRowCnt ; i++ ) {

		if( i == 0 ) {
			nIdx = pGrid->SelectGetHeadPosition();
			if( nIdx == -1 ) break;
		}
		else {
			nIdx = pGrid->SelectGetNext();
			if( nIdx == -1 ) break;
		}

		// 같은구입처끼리 묶는다.
		// 2005-04-04 by SM5ong : 이상한 DM에서 이상한 필드값을 가져오고 있어서 수정하였음.. ㅡ.,ㅡa
		// ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("PROVIDER_CODE"), sCurDonatorKey, nIdx );
		ids = GetDataMgrData( _T("DM_SE_MISSING_DON_01"), _T("DONATOR_KEY"), sCurDonatorKey, nIdx );
		if( ids ) return -1;
		if( DonatorKey != sCurDonatorKey ) continue;
		
		body +=	_T("<tr>\r\n")
				_T("<td width=\"17\" height=\"25\"> \r\n");
		tmpstr.Format(_T("<div align=\"center\">%d</div>\r\n"), ++nSeq);
		body += tmpstr;
		body +=	_T("</td>\r\n");

		pDataMgr->GetCellData(_T("TITLE_INFO"), nIdx, tmpstr1);
		tmpstr.Format(_T("<td width=\"224\">&nbsp;%s</td>\r\n"), tmpstr1);
		body += tmpstr;
		
		body += _T("<td width=\"224\"> \r\n");
		pDataMgr->GetCellData(_T("PUBLISHER"), nIdx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");

		body += _T("<td width=\"80\"> \r\n");
		pDataMgr->GetCellData(_T("VOL_TITLE"), nIdx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");

		body += _T("<td width=\"80\"> \r\n");
		pDataMgr->GetCellData(_T("OTHER_VOL_TITLE"), nIdx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");

		body += _T("<td width=\"80\"> \r\n");
		pDataMgr->GetCellData(_T("ACQ_EXP_DATE"), nIdx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");
		
	}
	
	_T("</table>\r\n")
	_T("</body></html>\r\n");
	
	return 0;
}

INT CMISSING_DATA_BASE::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CMISSING_DATA_BASE::Print_SetLastPageLine(VOID)
{
	INT ids;

	// XML 과 WorkForm객체를 생성한다.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML 처리기 생성에 실패하였습니다."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("중간 XML 처리기 생성에 실패하였습니다."));
		return;
	}

	// 현재 마지막 페이지 번호를 가져온다.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("페이지 정보를 찾을 수 없습니다."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// 현재 마지막 페이지 파일을 불러온다.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("페이지를 로드 하지 못하였습니다."));
		return;
	}
	// 불러온 WorkForm중에서 First Sheet를 가져온다.(중간 페이지 파일에는 Sheet가 한개뿐이다.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("페이지를 찾을 수 없습니다."));
		return;
	}
	// 마지막 수평줄 처리가 이미 되어 있는지 본다. 이미 처리되어 있다면 두번 할 필요 없다.
	CSIReportObject * pObject = pSheet->FindObject(_T("추가된마지막수평줄"));
	if (pObject) return;
	// 외부테이블 과 반복 테이블을 가져온다.(테이블 이름은 상황에 따라 변경할 것)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_22"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_44"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// 반복테이블의 마지막 라인의 왼쪽 처음과 오른쪽 마지막셀의 Rect를 구하고
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// 외부테이블의 마지막 라인의 왼쪽 처음 셀의 Rect를 구한다.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// 외부테이블보다 반복테이블이 작으면 마지막 수평줄을 추가해준다.
	if (InLeft.bottom < OutLeft.bottom) {
		// 마지막 수평줄의 Rect를 계산한다.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// 마지막 수평줄을 중간 페이지 파일에 추가한다.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("추가된마지막수평줄"));
			pLine->SetRect(NewRect);
		}

		// 변경된 중간페이지 파일을 저장한다.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// 생성된 Object를 해제
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;
}

VOID CMISSING_DATA_BASE::Print_List() 
{
	INT ids;
	CString ReportAlias ;
	ReportAlias = _T("K2UP_기증독촉");

	CESL_DataMgr *pDM = FindDM( _T("DM_SE_MISSING_DON_01") );
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return ;
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return ;
	}
	
	
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(ReportAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}

    
	// 제목 입력 ///////////////////////////////////////////////////////////////
	pSIReportManager->SetDataMgr(0, pDM);
	CString strtitle;

	if(_T("1") == m_sSearchCode) strtitle = _T("의뢰대상자료목록");
	else strtitle = _T("의뢰자료목록");

	pSIReportManager->SetTempVariable(_T("제목"),strtitle);



	//print
	ids = pSIReportManager->MakeXMLPageFiles();
	// 마지막 수평선을 그림
    Print_SetLastPageLine();
	ids = pSIReportManager->Print2();
    if (pSIReportManager) {
	delete pSIReportManager;
	pSIReportManager = NULL;
    }

	return ;
}

BOOL CMISSING_DATA_BASE::IsNewStringInArray(CArray<CString,CString> &arrayList, CString strNewValue)
{
	INT cnt = arrayList.GetSize();
	if(cnt == 0) return TRUE;

	for(INT i=0 ; i<cnt ; i++)
		if(arrayList.GetAt(i) == strNewValue) return FALSE;
	
	return TRUE;
}

CString CMISSING_DATA_BASE::GetLIBInfo(CESL_DataMgr * pDM)
{
	CString strLibName;
	pDM->GetOneValueQuery(_T("SELECT MAX(VALUE) FROM MN_LIBINFO_TBL WHERE VALUE_NAME = '도서관명'"), strLibName);
	return strLibName;
}

CString CMISSING_DATA_BASE::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
HBRUSH CMISSING_DATA_BASE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
