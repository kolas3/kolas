// MISSING_DATA_BASE.cpp : implementation file
//

#include "stdafx.h"
#include "MISSING_DATA_BASE.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"
#include "MISSING_DATA_SUMPRICE.h"

#include "..\SE_API\SeMemoDlg.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SeExpectMgr.h"

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

	m_pSpeciesDM = NULL;

	m_pProgressDlg= NULL;

	m_SeApi = new CSeApi(this);
	m_SeExpMgr = new CSeExpectMgr(this);

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

	//progress bar delete
	if(m_pProgressDlg) {
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
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
	
	if(InitESL_Mgr(_T("SM_SE_MISSING_BASE"))<0)
	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	m_pSpeciesDM = FindDM(_T("DM_체크인_색인"));
	

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


	ShowAcqDelayProgressDlg();
	m_pThreadBatchAcqDelay = AfxBeginThread(ThreadFunc_BatchAcqDelay, this);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT ThreadFunc_BatchAcqDelay(LPVOID pParam)
{
	CMISSING_DATA_BASE *pDlg = (CMISSING_DATA_BASE*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	//체크인 화면을 disable 시킨다.
	pDlg->EnableWindow(FALSE);

	CString strQry;

	pDlg->m_pSpeciesDM->FreeData();
	strQry+=_T(" I.REC_KEY = S.REC_KEY ");
	strQry+=_T(" AND I.REC_KEY = P.SPECIES_KEY ");
	strQry+=_T(" and i.PUB_STATUS='c' ");
	strQry+=_T(" and (select to_char(max(exp_start_date),'YYYY/MM/DD') from se_month_exp_tbl smp where smp.rec_key=p.acq_exp_key) <= nvl(to_char(p.end_date,'YYYY/MM/DD'),'2099/01/01') ");
	strQry+=_T(" and (select to_char(max(exp_start_date),'YYYY/MM/DD') from se_month_exp_tbl smp where smp.rec_key=p.acq_exp_key) < to_char(sysdate,'YYYY/MM/DD') ");
	pDlg->m_pSpeciesDM->RefreshDataManager(strQry);

	INT nProcCnt;
	nProcCnt = pDlg->m_pSpeciesDM->GetRowCount();
	pDlg->m_pProgressDlg->BringWindowToTop();
	pDlg->m_pProgressDlg->m_ctrlProgressTop.SetRange(0,nProcCnt);
	pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(1);

	pDlg->m_pProgressDlg->CenterWindow();
	pDlg->m_pProgressDlg->ShowWindow(SW_SHOW);
	CString strTITLE;
	CString strProgress;

	CESL_DataMgr* pDM_WORK = pDlg->FindDM(_T("DM_체크인_입수지연_작업"));
	CESL_DataMgr* pDM_EXP = pDlg->FindDM(_T("DM_체크인_입수지연_예측"));
	CESL_DataMgr* pDM_ACQ = pDlg->FindDM(_T("DM_체크인_구입정보"));

	CArray<CString,CString> arrayVolExpKeyList;
	CArray<CString,CString> arrayOtherVolExpKeyList;

	if(!pDM_WORK || !pDM_EXP || !pDM_ACQ)
	{
		AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
		return -1;
	}

	nProcCnt=0;
	for(int i=0;i<pDlg->m_pSpeciesDM->GetRowCount();i++)
	{
		strTITLE = _T("");
		strTITLE = pDlg->m_pSpeciesDM->GetCellData(_T("ISS_표제사항"),i);  
		strProgress.Format(_T("[ %s ] \n 종을 입수 처리하는 중입니다"),strTITLE);
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcACQ_DELAY_TOP)->SetWindowText(strProgress);

		pDM_WORK->FreeData();
		pDM_EXP->FreeData();
		pDM_ACQ->FreeData();
	
		CString strSPECIES_KEY,strPUB_FREQ;
		strSPECIES_KEY = pDlg->m_pSpeciesDM->GetCellData(_T("ISS_종KEY"),i);
		strPUB_FREQ = pDlg->m_pSpeciesDM->GetCellData(_T("ISS_간행빈도"),i);
		
		if(strSPECIES_KEY.IsEmpty())
		{
			AfxMessageBox(_T("해당 종KEY를 찾을 수 없습니다!"));
			return -2;
		}
		
		CString strWhere;
		strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
		pDM_ACQ->RefreshDataManager(strWhere);
		if(0 == pDM_ACQ->GetRowCount()) {
			AfxMessageBox(_T("수입구분정보를 가져올 수 없습니다"));
			return -2;
		}
		
		CString strACQ_KEY = _T("");
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),i);
		
		//예측정보를 구성한다 
		arrayVolExpKeyList.RemoveAll();
		arrayOtherVolExpKeyList.RemoveAll();

		pDlg->m_SeExpMgr->InitMgr(pDM_EXP, 
					pDM_ACQ, 
					strPUB_FREQ, 
					1, 
        			arrayVolExpKeyList,
					arrayOtherVolExpKeyList,
					0,
					NULL);

		//작업DM을 구성한다.
		INT nRow_Exp = pDM_EXP->GetRowCount();
		INT nRow_Work;
		CString strVoltitle;
		CString strACQ_EXP_BOOK_CNT;

		for(int j=0;j<nRow_Exp;j++)
		{
			pDM_WORK->InsertRow(-1);
			nRow_Work = pDM_WORK->GetRowCount()-1;
			pDlg->CopyDMToDMByAlias(pDM_EXP,j,pDM_WORK,nRow_Work);

			//예측권호를 권호명에 보여준다
			strVoltitle = _T("");
			strVoltitle = pDM_EXP->GetCellData(_T("SA_입수예측권호명"),j);
			pDM_WORK->SetCellData(_T("SV_권호명"),strVoltitle,nRow_Work);

			//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
			pDlg->m_SeApi->MAKE_EXP_WORK_by_IDX_ACQ(pDlg->m_pSpeciesDM,i,pDM_WORK,nRow_Work,pDM_ACQ,1);
					
			//결호처리를 한다 
			strACQ_EXP_BOOK_CNT = _T("");
			strACQ_EXP_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수예정책수"),j);
			pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),j);
			pDM_WORK->SetCellData(_T("SA_결호책수"),strACQ_EXP_BOOK_CNT,j);
			pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호(예정)"),j);
			pDM_WORK->SetCellData(_T("UDF_상태"),_T("M"),j);
		}

		CESL_DataMgr* pDM_VOL = pDlg->FindDM(_T("DM_체크인_권"));
		CESL_DataMgr* pDM_ACQ_VOL = pDlg->FindDM(_T("DM_체크인_수입구분별권"));
		CESL_DataMgr* pDM_BOOK = pDlg->FindDM(_T("DM_체크인_책"));
		CESL_DataMgr* pDM_BOOK_INSERT = pDlg->FindDM(_T("DM_체크인_책_입력"));
		CESL_DataMgr* pDM_APPENDIX = pDlg->FindDM(_T("DM_체크인_부록"));

		//수입 구분 정보
		pDM_ACQ = pDlg->FindDM(_T("DM_체크인_구입정보"));

		if(!pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK || !pDM_BOOK_INSERT || !pDM_APPENDIX || !pDM_WORK)
		{
			AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
			return -1;
		}

		INT nRow = pDM_WORK->GetRowCount();
		INT ids = 0;
		CString msg;
		
		pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetRange(0,nRow);
		for(j=0;j<nRow;j++)
		{
			ids = pDlg->m_SeApi->ACQ_DELAY(pDlg->m_pSpeciesDM,
								i,
								pDM_WORK,
								nRow-j-1,
								pDM_ACQ,
								1,
								pDM_VOL,
								pDM_ACQ_VOL,
								pDM_BOOK,
								arrayVolExpKeyList,
								arrayOtherVolExpKeyList);							
			if(ids<0)
			{	
				msg.Format(_T("[ %d ] 번째 작업에 문제가 생겨 작업을 중단 합니다!"),i+1);
				AfxMessageBox(msg);
				return -2;
			}
			pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetPos(j+1);
		}

		//위쪽 프로그래시브 바
		pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(++nProcCnt);
	}

	//체크인 화면을 eable 시킨다.
	pDlg->m_pProgressDlg->ShowWindow(SW_HIDE);
	pDlg->EnableWindow(TRUE);
	pDlg->m_bIsThreadRunning = FALSE;
	SetEvent(pDlg->m_hThread);	

	if ( pDlg->m_pSpeciesDM->GetRowCount() > 0 )
	{
		CString strTemp;
		strTemp.Format(_T("총 %d개 종을 일괄입수지연처리 했습니다"),pDlg->m_pSpeciesDM->GetRowCount());
		AfxMessageBox(strTemp);
	}

	return 0;
}

VOID CMISSING_DATA_BASE::ShowAcqDelayProgressDlg()
{
	
	if(m_pProgressDlg == NULL)
	{
		m_pProgressDlg = new CCheckinAcqDelayProgressive(this);
	}
	
	if(!m_pProgressDlg->GetCreate())
	{
		if(!m_pProgressDlg->Create(this))
		{
			delete m_pProgressDlg;
			m_pProgressDlg = NULL;
			return;
		}
	}	
}

INT CMISSING_DATA_BASE::BatchAcqDelayProcess()
{
	CString strQry;

	m_pSpeciesDM->FreeData();
	strQry+=_T(" I.REC_KEY = S.REC_KEY ");
	strQry+=_T(" AND I.REC_KEY = P.SPECIES_KEY ");
	strQry+=_T(" and i.PUB_STATUS='c' ");
	strQry+=_T(" and (select to_char(max(exp_start_date),'YYYY/MM/DD') from se_month_exp_tbl smp where smp.rec_key=p.acq_exp_key) <= nvl(to_char(p.end_date,'YYYY/MM/DD'),'2099/01/01') ");
	strQry+=_T(" and (select to_char(max(exp_start_date),'YYYY/MM/DD') from se_month_exp_tbl smp where smp.rec_key=p.acq_exp_key) < to_char(sysdate,'YYYY/MM/DD') ");
	m_pSpeciesDM->RefreshDataManager(strQry);

	CESL_DataMgr* pDM_WORK = FindDM(_T("DM_체크인_입수지연_작업"));
	CESL_DataMgr* pDM_EXP = FindDM(_T("DM_체크인_입수지연_예측"));
	CESL_DataMgr* pDM_ACQ = FindDM(_T("DM_체크인_구입정보"));

	CArray<CString,CString> arrayVolExpKeyList;
	CArray<CString,CString> arrayOtherVolExpKeyList;

	if(!pDM_WORK || !pDM_EXP || !pDM_ACQ)
	{
		AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
		return -1;
	}

	for(int i=0;i<m_pSpeciesDM->GetRowCount();i++)
	{
		pDM_WORK->FreeData();
		pDM_EXP->FreeData();
		pDM_ACQ->FreeData();
	
		CString strSPECIES_KEY,strPUB_FREQ;
		strSPECIES_KEY = m_pSpeciesDM->GetCellData(_T("ISS_종KEY"),i);
		strPUB_FREQ = m_pSpeciesDM->GetCellData(_T("ISS_간행빈도"),i);
		
		if(strSPECIES_KEY.IsEmpty())
		{
			AfxMessageBox(_T("해당 종KEY를 찾을 수 없습니다!"));
			return -2;
		}
		
		CString strWhere;
		strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
		pDM_ACQ->RefreshDataManager(strWhere);
		if(0 == pDM_ACQ->GetRowCount()) {
			AfxMessageBox(_T("수입구분정보를 가져올 수 없습니다"));
			return -2;
		}
		
		CString strACQ_KEY = _T("");
		strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),i);
		
		//예측정보를 구성한다 
		arrayVolExpKeyList.RemoveAll();
		arrayOtherVolExpKeyList.RemoveAll();

		CSeExpectMgr mgr(this);
		mgr.InitMgr(pDM_EXP, 
					pDM_ACQ, 
					strPUB_FREQ, 
					1, 
        			arrayVolExpKeyList,
					arrayOtherVolExpKeyList,
					0,
					NULL);

		//작업DM을 구성한다.
		CSeApi api(this);
		INT nRow_Exp = pDM_EXP->GetRowCount();
		INT nRow_Work;
		CString strVoltitle;
		CString strACQ_EXP_BOOK_CNT;

		for(int j=0;j<nRow_Exp;j++)
		{
			pDM_WORK->InsertRow(-1);
			nRow_Work = pDM_WORK->GetRowCount()-1;
			CopyDMToDMByAlias(pDM_EXP,j,pDM_WORK,nRow_Work);

			//예측권호를 권호명에 보여준다
			strVoltitle = _T("");
			strVoltitle = pDM_EXP->GetCellData(_T("SA_입수예측권호명"),j);
			pDM_WORK->SetCellData(_T("SV_권호명"),strVoltitle,nRow_Work);

			//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
			api.MAKE_EXP_WORK_by_IDX_ACQ(m_pSpeciesDM,i,pDM_WORK,nRow_Work,pDM_ACQ,1);
					
			//결호처리를 한다 
			strACQ_EXP_BOOK_CNT = _T("");
			strACQ_EXP_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수예정책수"),j);
			pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),j);
			pDM_WORK->SetCellData(_T("SA_결호책수"),strACQ_EXP_BOOK_CNT,j);
			pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호(예정)"),j);
			pDM_WORK->SetCellData(_T("UDF_상태"),_T("M"),j);
		}

		CESL_DataMgr* pDM_VOL = FindDM(_T("DM_체크인_권"));
		CESL_DataMgr* pDM_ACQ_VOL = FindDM(_T("DM_체크인_수입구분별권"));
		CESL_DataMgr* pDM_BOOK = FindDM(_T("DM_체크인_책"));
		CESL_DataMgr* pDM_BOOK_INSERT = FindDM(_T("DM_체크인_책_입력"));
		CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_체크인_부록"));

		//수입 구분 정보
		pDM_ACQ = FindDM(_T("DM_체크인_구입정보"));

		if(!pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK || !pDM_BOOK_INSERT || !pDM_APPENDIX || !pDM_WORK)
		{
			AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
			return -1;
		}

		INT nRow = pDM_WORK->GetRowCount();
		INT ids = 0;
		CString msg;
		
		for(j=0;j<nRow;j++)
		{
			ids = api.ACQ_DELAY(m_pSpeciesDM,
								i,
								pDM_WORK,
								nRow-j-1,
								pDM_ACQ,
								1,
								pDM_VOL,
								pDM_ACQ_VOL,
								pDM_BOOK,
								arrayVolExpKeyList,
								arrayOtherVolExpKeyList);							
			if(ids<0)
			{	
				msg.Format(_T("[ %d ] 번째 작업에 문제가 생겨 작업을 중단 합니다!"),i+1);
				AfxMessageBox(msg);
				return -2;
			}
		}
	}

	CString strTemp;
	strTemp.Format(_T("총 %d개 종을 입수지연처리 했습니다"),m_pSpeciesDM->GetRowCount());
	AfxMessageBox(strTemp);

	return 0;
}

INT CMISSING_DATA_BASE::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CMISSING_DATA_BASE::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_LIST_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CMISSING_DATA_BASE::InitializeTabCtrl()
{
	INT i, nItemCnt;
	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB);

	nItemCnt = pTabCtrl->GetItemCount();

	for( i = nItemCnt-1 ; i >= 0 ; i-- )
		pTabCtrl->DeleteItem(i);

	TC_ITEM TabCtrlItem;
	CRect rect;
	
	TCHAR *title[] = { _T("결호자료") , _T("입수지연자료") , _T("책수미달자료") };

	TabCtrlItem.mask = TCIF_TEXT;

	for( i = 0 ; i < 3 ; i++ ) {

		TabCtrlItem.pszText = title[i];
		pTabCtrl->InsertItem(i, &TabCtrlItem);
	}
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

		ids = AllControlDisplay( _T("CM_SE_MISSING_01"), -1);
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

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();

	// ===========================================================================================================
	// - m_sSearchCode = '1' 인경우, 
	//
	//  1. 최근에 독촉된 자료만 골라낸다.
	//  2. 독촉대상인지 판단 : NVL(C.CLAIM_DATE,A.ACQ_EXP_DATE) + M.CLAIM_INTERVAL < 오늘 날짜  + M.CLAIM_YN = 'Y'
	// ===========================================================================================================
	if( m_sSearchCode == '1' ) {
		
		// 1. 최근에 독촉된 자료만 골라낸다.
		for( i = nRowCnt-1 ; i >= 0 ; i-- ) {
		
			ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("ACQ_TYPE_VOL_KEY"), sCurKey, i );
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

	nRowCnt = pDataMgr->GetRowCount();
	
	// 구입처 설명 변환
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDataMgr,_T("구입처관리"),_T("UDF_구입처"));

	ids = AllControlDisplay( _T("CM_SE_MISSING_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d 건의 자료가 검색되었습니다.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

BOOL CMISSING_DATA_BASE::CheckClaimYn( INT f_nIdx ,COleDateTime tToday)
{
	// =============================================================================================
	// 독촉대상 : C.CLAIM_DATE(NULL 일때는 ACQ_EXP_DATE) + M.CLAIM_INTERVAL < 오늘 날짜  return FALSE;
	//            = 0 < 오늘 날짜 - M.CLAIM_INTERVAL - C.CLAIM_DATE

	// 독촉자료 : return TRUE
	// =============================================================================================
	INT ids, nClaimInterval;
    INT yyyy, mm, dd;
	CString sTodayDate, sAcqExpDate, sClaimDate, sClaimInterval;

	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("CLAIM_DATE"), sClaimDate, f_nIdx );
	if( ids < 0 ) return -1;

	if( sClaimDate == _T("") ) {
		ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("ACQ_EXP_DATE"), sAcqExpDate, f_nIdx );
		if( ids < 0 ) return -1;

		sClaimDate = sAcqExpDate;
	}

	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("CLAIM_INTERVAL"), sClaimInterval, f_nIdx );
	if( ids < 0 ) return -2;

	//if( sClaimDate == _T("") ) return FALSE;

	nClaimInterval = _ttoi( (TCHAR*)LPCTSTR(sClaimInterval) );
	
	// =================================================================================================
	// 오늘날짜를 구한다.
	// =================================================================================================
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
	if( pDataMgr == NULL ) return -1;
	

	//modified 2003.12.13
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
		
		AfxMessageBox( _T("선택된 자료는 이미 독촉된 자료 입니다.") );
		return 0;
	}

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_01") );
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

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
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
		AfxMessageBox( _T("독촉처리를 실패하였습니다.") );
		return -9;
	}

	//JOB.2019.0073 : 충돌 Licen
	// 메일전송
// 	CString sVal;
// 	GetManageValue( _T("수서"), _T("연속"), _T("독촉메일자동발송여부"), _T(""), sVal );
// 	
// 	if( sVal == _T("Y") ) DoSendMail();

	CString sMsg;
	sMsg.Format( _T("%d개의 자료를 독촉처리 하였습니다."), nRowCnt );
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

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
	if( pDataMgr == NULL ) return -1;

	// rec_key 생성
	ids = pDataMgr->MakeRecKey( sRecKey );
	if(ids < 0) return -2;

	pDataMgr->SetCellData(_T("ACQ_TYPE_VOL_KEY"),sRecKey,f_nIdx);

	// ACQ_TYPE
	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("ACQ_CODE"), sAcqType, f_nIdx );
	if(ids < 0) return -3;

	// ACQ_TYPE_VOL_KEY
	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("REC_KEY"), sAcqTypeVolKey, f_nIdx );
	if(ids < 0) return -4;

	// ACQ_TYPE_VOL_KEY
	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("REC_KEY"), sAcqTypeVolKey, f_nIdx );
	if(ids < 0) return -5;

	// MISS_BOOK_CNT
	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("MISS_BOOK_CNT"), sMissBookCnt, f_nIdx );
	if(ids < 0) return -6;
	
	// CLAIM_SEQ
	CString sSql, sData;
	sSql.Format( _T("SELECT NVL(MAX(CLAIM_SEQ),0) FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = %s"), 
				 sAcqTypeVolKey );
	ids = pDataMgr->GetOneValueQuery( sSql, sData );
	if(ids < 0) return -7;
	
	sClaimSeq.Format( _T("%d") , _ttoi( (TCHAR*)LPCTSTR(sData) ) + 1 );
	pDataMgr->SetCellData(_T("ACQ_TYPE_VOL_KEY"),sClaimSeq,f_nIdx);

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
	
//2006-02-09 : HJY 메일 발송일은 메일 발송 여부 확인 후 저장한다.	
/*	CString sCurProvider = _T("");

	ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("PROVIDER_CODE"), sCurProvider, f_nIdx );
	if( ids < 0 ) return -1;

  if(!sCurProvider.IsEmpty())
	{
		ids = pDataMgr->AddDBFieldData( _T("MAIL_SEND_DATE")	, _T("STRING")	, sMailSendDate);
		if(ids < 0) return -8;
	}
*/
	
	ids = pDataMgr->MakeInsertFrame( _T("SE_CLAIM_HISTORY_TBL") );
	if( ids < 0 ) return -9;

	return 0;
}

INT CMISSING_DATA_BASE::DoSendMail()
{
	//JOB.2019.0073 : 충돌 Licen
// 	INT i, ids, nResult;
// 	INT nIdx, nRowCnt, nProviderCnt;
// 	
// 	CString sMsg;
// 	CString sPrevProvider, sCurProvider;
// 	CArray <CString, CString> sPoroviderList;
// 
// 	sPoroviderList.RemoveAll();
// 	nProviderCnt = 0;
// 	sPrevProvider = sCurProvider = _T("");
// 
// 	// ============================================================================================================
// 	// 선택된 DM에서 구입처의 갯수를 구하고, 구입처별로 메일발송
// 	// ============================================================================================================
// 	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_01") );
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
// 	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
// 	if( pDataMgr == NULL ) return -4;
// 	
// 	INT nSize = 0;
// 	CString strTmp;
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
// 		ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("PROVIDER_CODE"), sCurProvider, nIdx );
// 		if( ids < 0 ) return -1;
// 
// 		if( sCurProvider.IsEmpty() ) {
// 			nNonInfoList.Add(i+1);
// 		}
// 		
// 		if(!sCurProvider.IsEmpty())
// 		{
// 			if(IsNewStringInArray(sPoroviderList,sCurProvider))
// 				sPoroviderList.Add( sCurProvider );
// 		}
// 			
// 		sPrevProvider = sCurProvider;
// 	}
// 	if( 0 < nNonInfoList.GetSize() )
// 	{
// 		sMsg.Format(_T("구입처 정보가 없는 %d개 항목은 메일발송이 취소됩니다."), nNonInfoList.GetSize() );
// 		AfxMessageBox(sMsg);
// 	}
// 
// 	if( sPoroviderList.GetSize() <= 0 )
// 		return 0;
// 
// 	if(!m_pMailMgr)
// 	{
// 		m_pMailMgr = new CSendMailMgr(this);
// 	}
// 
// 	SendMailRecord *pMailRecord;
// 	pMailRecord = new SendMailRecord[sPoroviderList.GetSize()];
// 
// 	for( i = 0 ; i < sPoroviderList.GetSize() ; i++ ) {
// 
// 		// Make MailRecord
// 		ids = MakeMailRecord( pMailRecord[nProviderCnt], &sPoroviderList, i );
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
// 
// 		// Add MailRecord
// 		m_pMailMgr->AddMailRecord( &pMailRecord[nProviderCnt] );
// 	}
// 
// 	// send mail
// 	INT recordCnt = 0;
// 	recordCnt = m_pMailMgr->GetCountMailRecord();
// 	
// 	if(recordCnt <= 0)
// 	{
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
// 
// 	if(recordCnt>0) m_pMailMgr->DoModal(MAIL_SERVER_CFGFILE);
// 	
// 	CArray <INT,INT> nSuccessList;
// 	nSuccessList.RemoveAll();
// 
// 	// MAIL_SEND_DATE
// 	CString sTodayDate;
// 	CTime time = pDataMgr->GetDBTime();
// 	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
// 	
// 	pDataMgr->StartFrame();
// 	pDataMgr->InitDBFieldData();
// 	
// 	for( i = 0 ; i < recordCnt ; i++ ) {
// 		
// 		nResult = m_pMailMgr->GetMailRecordSendState(i);
// 		if(nResult == 0)	// 메일발송 성공->해당 구입처인 자료들에 메일 발송일 저장
// 		{
// 			nRowCnt = pGrid->SelectGetCount();
// 				
// 			for( INT j = 0 ; j < nRowCnt ; j++ ) 
// 			{				
// 				if( j == 0 ) 
// 				{
// 					nIdx = pGrid->SelectGetHeadPosition();
// 					if( nIdx == -1 ) break;
// 				}
// 				else 
// 				{
// 					nIdx = pGrid->SelectGetNext();
// 					if( nIdx == -1 ) break;
// 				}
// 				
// 				ids = pDataMgr->GetCellData(_T("PROVIDER_CODE"), nIdx, sCurProvider);
// 				if( ids < 0 ) return -1;
// 								
// 				if( sPoroviderList.GetAt(i) == sCurProvider )
// 				{
// 					CString sType_vol_key, sSeq_no, sQuery, sRec_key;
// 					ids = pDataMgr->GetCellData(_T("ACQ_TYPE_VOL_KEY"), nIdx, sType_vol_key);
// 					if( ids < 0 ) return -1;
// 					ids = pDataMgr->GetCellData(_T("CLAIM_SEQ"), nIdx, sSeq_no);
// 					if( ids < 0 ) return -1;
// 
// 					sQuery.Format(_T("SELECT REC_KEY FROM SE_CLAIM_HISTORY_TBL WHERE ACQ_TYPE_VOL_KEY = '%s' AND CLAIM_SEQ = '%s'"),sType_vol_key, sSeq_no );
// 					pDataMgr->GetOneValueQuery(sQuery, sRec_key);
// 															
// 					pDataMgr->AddDBFieldData(_T("MAIL_SEND_DATE"), _T("STRING"), sTodayDate, -1);
// 					pDataMgr->MakeUpdateFrame(_T("SE_CLAIM_HISTORY_TBL"),_T("REC_KEY"),_T("NUMERIC"), sRec_key);
// 				}
// 								
// 			}
// 
// 		}
// 	}
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

INT CMISSING_DATA_BASE::MakeMailRecord( SendMailRecord	&f_MailRecord, CArray <CString, CString> * f_sPoroviderList, INT f_nIdx )
{
	INT ids;
	CString sSender, sBody, sHeader, sFooter;
	CString sProvider_Name, sProvider_Emailaddr, sProvider_Tel, sProvider_Addr;
	CString sSql, sProvidercode;

	sProvidercode = f_sPoroviderList->GetAt(f_nIdx);

	//
	CESL_DataMgr tmpDM;
	CESL_DataMgr *pDataMgr = FindDM( _T("DM_SE_MISSING_01") );
	
	tmpDM.SetCONNECTION_INFO(pDataMgr->CONNECTION_INFO);
	
	sSql.Format(_T("SELECT DESCRIPTION, EMAIL, TEL, ADDRESS FROM CD_CODE_TBL WHERE CLASS=2 AND TRIM(CODE) = '%s'"), sProvidercode );
	
	ids = tmpDM.RestructDataManager(sSql);
	if(ids < 0) return -1000;

	tmpDM.GetCellData(0, 0, sProvider_Name);
	tmpDM.GetCellData(0, 1, sProvider_Emailaddr);
	tmpDM.GetCellData(0, 2, sProvider_Tel);
	tmpDM.GetCellData(0, 3, sProvider_Addr);


	f_MailRecord.m_nFileAttachCnt	= 0;
	f_MailRecord.m_pFileAttach		= _T("");
	f_MailRecord.m_sEncoding		= _T("text/html");
	f_MailRecord.m_sRecipientAddress= sProvider_Emailaddr;//_T(sProvider_Emailaddr);

	f_MailRecord.m_sRecipientAddress= sProvider_Emailaddr;
	f_MailRecord.m_sRecipientName	= sProvider_Name; // _T("");
	f_MailRecord.m_sCCName			= _T("");// sProvider_Name;//_T(sProvider_Name);
	f_MailRecord.m_sCCAddress		= _T("");// sProvider_Addr;//_T(sProvider_Addr);
	f_MailRecord.m_sSubject			= _T("독촉메일");

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
			//DEL _T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '독촉메일'")
			_T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '독촉메일' AND MG.MANAGE_CODE=UDF_MANAGE_CODE")
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
	sHeader	= _T("독촉메일");
	sFooter = _T("다음의 자료들을 독촉합니다.");

	// body
	ids = MakeMailBody(
						pDataMgr, 
						sProvidercode, 
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
										  CString		Providercode, 
										  CString		&body, 
										  CString		ProviderName, 
										  CString		Provider_Tel, 
										  CString		Provider_Addr, 
										  CString		sender, 
										  CString		header, 
										  CString		footer		)
{
	INT ids, nRowCnt, nIdx;
	CString	sCurProvidercode;

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_01") );
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

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
	if( pDataMgr == NULL ) return -6;

	INT nProviderCnt = 0;
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

		ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("PROVIDER_CODE"), sCurProvidercode, nIdx );
		if( ids ) return -1;
		if( Providercode == sCurProvidercode ) nProviderCnt++;
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
	tmpstr.Format(_T("		<div align=\"center\">%d 권</div>\r\n"), nProviderCnt);
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
		ids = GetDataMgrData( _T("DM_SE_MISSING_01"), _T("PROVIDER_CODE"), sCurProvidercode, nIdx );
		if( ids ) return -1;
		if( Providercode != sCurProvidercode ) continue;
		
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_27"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_15"));
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
  ReportAlias = _T("K2UP_구입독촉");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_MISSING_01") );
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return ;
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		
		/*
		 if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

		return ;
	}
	
	
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(ReportAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			
		/*
		 if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

		return ;
	}

    // 제목 입력 ///////////////////////////////////////////////////////////////
	pSIReportManager->SetDataMgr(0, pDM);

	CString strtitle;

	if(_T("1") == m_sSearchCode) strtitle = _T("독촉대상자료목록");
	else strtitle = _T("독촉자료목록");

	pSIReportManager->SetTempVariable(_T("제목"),strtitle);



	//PRINT//////////////////////////////////////////////////////////////////////////
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

VOID CMISSING_DATA_BASE::SumPrice()
{

	if( m_sSearchCode == _T("2") ) {
		
		AfxMessageBox( _T("선택된 자료는 이미 독촉된 자료 입니다.") );
		return ;
	}

	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_MISSING_01") );
	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	INT ids = pGrid->SelectMakeList();
	if( ids ) {
		AfxMessageBox( _T("SelectMakeList Error!") );
		return ;
	}

	INT nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) {
		AfxMessageBox( _T("자료를 선택하십시오.") );
		return ;
	}
	if( nRowCnt <  0 ) {
		AfxMessageBox( _T("그리드의 선택정보를 가져오는데 실패하였습니다.") );
		return ;
	}

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_MISSING_01"));
	if( pDataMgr == NULL ) return ;


	CString strSumPrice;
	INT nSumPrice = 0;
	INT nChildIdx = 0;

	INT nIdx = -1;
	nIdx = pGrid->SelectGetHeadPosition();
	while ( nIdx > 0 ) 
	{
		CString strPrice;
		strPrice = pDataMgr->GetCellData( _T("구입가격"), nIdx );
		nSumPrice = nSumPrice + _ttoi(strPrice);

		nIdx = pGrid->SelectGetNext();
	}	
	strSumPrice.Format(_T("%d"), nSumPrice);

	CMISSING_DATA_SUMPRICE dlg;
	dlg.SetParentInfo(pDataMgr,  pGrid);
	dlg.SetPrice( strSumPrice );
	dlg.DoModal();


}

HBRUSH CMISSING_DATA_BASE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
