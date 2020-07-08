// CHECKIN.cpp : implementation file
//

#include "stdafx.h"
#include "CHECKIN.h"
#include "CheckinAcqDelay.h"
#include "..\SE_API\CheckinReceipt.h"
#include "..\SE_API\SeMemoDlg.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SeExpectMgr.h"
#include "..\SE_CHECKIN_VOL\CheckinVolProgressive.h"
#include "..\SE_SPECIES\SeSpeciesForm.h"
#include "..\SE_ACCOUNT_STATEMENT\SeAccountStatement.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCHECKIN dialog


CCHECKIN::CCHECKIN(CESL_Mgr* pParent /*=NULL*/)	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCHECKIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pDM_WORK = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDlg = NULL;
	m_nACQ_TYPE = 1; //default
	m_nCurrentIdx = -1;
	

	//Thread
	m_pThreadAcqDelay = NULL;
	m_bIsThreadRunning = FALSE;
	m_pProgressDlg = NULL;
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));

	//RFID
	m_bRFID = FALSE;


	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;
}

CCHECKIN::~CCHECKIN()
{
	// Thread Check
	if (m_bIsThreadRunning)
	{
		m_bIsThreadRunning = FALSE;

		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
			break;
		}
	}
	
	//progress bar delete
	if(m_pProgressDlg) {
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
	
	//search dialog delete
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}

}

VOID CCHECKIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCHECKIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCHECKIN, CDialog)
	//{{AFX_MSG_MAP(CCHECKIN)
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_MESSAGE(WM_USER+1980, OnRFID)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCHECKIN message handlers
BOOL CCHECKIN::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CCHECKIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCHEKIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	pGrid->MoveWindow(-2, 35 , cx+2, cy);	

}

BOOL CCHECKIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_체크인"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_체크인"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_색인"));
	m_pDM_WORK = FindDM(_T("DM_체크인_입수지연_작업"));

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : DM_체크인_색인, CM_연속_체크인") );
		return false;
	}

	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) strACQ_TYPE = _T("구입");
	else strACQ_TYPE = _T("기증");

	CString strCurYear;
	strCurYear = GetCurrentYear();
	m_pCM->SetControlMgrData(_T("수입구분"),strACQ_TYPE);   
	m_pCM->SetControlMgrData(_T("수입년도"),strCurYear);  
	
	
	//RFID 관리값 파악
	CSeApi api(this);
	m_bRFID = api.IsRFIDUse(m_pDM);

	//라이트 버젼여부를 가져온다
	m_bIsLightVersion = api.IsLightVersion(m_pDM);

	//부서지정 사용 여부를 가져온다
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM);

	//표지관리 사용 여부를 가져온다

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CCheckinSearch(this);
		m_pDlg->m_nACQ_TYPE = m_nACQ_TYPE;
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


VOID CCHECKIN::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}


VOID CCHECKIN::OnSendQuery(WPARAM w,LPARAM l)
{
	if(2 == m_nACQ_TYPE) m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D");

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행상태코드"),_T("UDF_간행상태"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("관리구분"),_T("UDF_관리구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료구분"),_T("UDF_자료구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용제한구분"),_T("UDF_이용제한구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용대상구분"),_T("UDF_이용대상구분"));

	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

	CString strPURCHASE_YN;
	CString strDONATE_YN;
	

	for(INT i=0;i<nRowCnt;i++)
	{
		strPURCHASE_YN = _T("");
		strDONATE_YN = _T("");
		strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),i);
		strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),i);
				
		if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),i);
		}
		else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),i);
		}
		else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),i);
		}		
		
	}

	m_pGrid->Display();
	m_nCurrentIdx = -1;

	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}

}

VOID CCHECKIN::InsertSpecies()
{
	
	CSeSpeciesForm dlg(this);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
	dlg.DoModal();

}

VOID CCHECKIN::ModifySpecies()
{
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;		
	}
	else
	{
		CSeSpeciesForm dlg(this);

		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_nACQ_TYPE = m_nACQ_TYPE;
		dlg.m_nOpenMode = -1;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;
		dlg.m_bIsLightVersion = m_bIsLightVersion;
		dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
		dlg.DoModal();
	}

}


VOID CCHECKIN::LaunchCheckinVol()
{
	//수입년도와 접수번호를 확인한다 
	CString strACQ_YEAR;
	CString strRECEIPT_NO;
	m_pCM->GetControlMgrData(_T("접수번호"),strRECEIPT_NO);   
	m_pCM->GetControlMgrData(_T("수입년도"),strACQ_YEAR);  
	
	if( m_pDM->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("선정된 정보가 없습니다!"));
		return;
	}
	
	if(!strRECEIPT_NO.IsEmpty() && strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("접수번호의 수입년도 항목이 없습니다!"));
		return;
	}

	
	if(!strACQ_YEAR.IsEmpty() && !strRECEIPT_NO.IsEmpty())
	{
		//접수번호의 유효성을 한번 더 검사한다.
		CString strREC_KEY;
		CString strQuery;

		strQuery.Format(_T("SELECT REC_KEY FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),m_nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO);
		m_pDM->GetOneValueQuery(strQuery,strREC_KEY);
		
		if(strREC_KEY.IsEmpty())
		{
			AfxMessageBox(_T("존재하지 않는 접수번호입니다!"));
			return;
		}

	}

	//권별 체크인 화면을 띄운다
	m_CheckinVolLauncher.SetParentInfo(m_pDM, m_pGrid, m_nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO,m_bIsLightVersion);
	m_CheckinVolLauncher.LaunchCheckinVol(GetParentFrame(),this);
}

VOID CCHECKIN::MemoEdit()
{
	if(m_nCurrentIdx<0 || m_pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;		
	}
	else
	{
		SeMemoDlg dlg(this);

		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_pDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;
		dlg.DoModal();
	}

}

VOID CCHECKIN::PrintGridData()
{
	INT nRowCnt = m_pDM->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속체크인목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM, 0);

	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}



}

VOID CCHECKIN::AcqDelay(BOOL bShowMode)
{
	//입수지연 처리 함수
	//bShowMode TRUE:개별 화면을 보여줌  FALSE:개별화면을 보여주지 않음
	
	m_pGrid->SelectMakeList();
	
	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}
	
	CString msg;

	if(bShowMode) 
	{
		
		CCheckinVolProgressive* progressdlg = new CCheckinVolProgressive;
		progressdlg->Create(this);

		INT nProcCnt = m_pGrid->SelectGetCount();
		INT ids = 0;
		CString strProgress;
		CString strTITLE;
		INT nCheckCnt = 0;

		CProgressCtrl* pProgress = progressdlg->GetProgressCtrl();

		INT nAcqDelayCnt = 0;
		INT nTotalVolCnt = 0;
		INT nDoModalOption = 0;

		while(idx>=0)
		{
			
			nCheckCnt++;

			//예측 구성 
			progressdlg->SetProGressRange(100);
			strTITLE = m_pDM->GetCellData(_T("ISS_표제사항"),idx);
			progressdlg->SetProGressPos(0);
			progressdlg->BringWindowToTop();
			progressdlg->CenterWindow();
			progressdlg->ShowWindow(SW_SHOW);
			
			AcqDelayComposition(idx,pProgress);
			
			progressdlg->ShowWindow(SW_HIDE);
			
			strTITLE = _T("");
			strTITLE = m_pDM->GetCellData(_T("ISS_표제사항"),idx);
			

			if(nProcCnt == nCheckCnt || 1 == nProcCnt)
			{
				nDoModalOption = DoModalAcqDelaydlg(strTITLE,FALSE);
			}
			else
			{
				nDoModalOption = DoModalAcqDelaydlg(strTITLE);
			}
			

			if(nDoModalOption < 0)
			{
				if(-1 == nDoModalOption)
				{
					break;
				}
				else if(-1001 == nDoModalOption)
				{
					idx = m_pGrid->SelectGetNext();
					continue;
				}
			}
			
			//지연 처리 작업
			INT nRow_work = m_pDM_WORK->GetRowCount();
			progressdlg->SetProGressRange(nRow_work+1);
	
			progressdlg->SetMessage(strProgress);
			progressdlg->ShowWindow(SW_SHOW);
			
			ids = AcqDelayProcess(idx,nTotalVolCnt,pProgress);
			

			if(ids<0) {

				if(progressdlg) {
					delete progressdlg;
					progressdlg = NULL;
				}
				
				return;
			}
			else
			{
				nAcqDelayCnt++;
			}
		
			idx = m_pGrid->SelectGetNext();


		}
		
		if(progressdlg) {
			delete progressdlg;
			progressdlg = NULL;
		}
		
		if(nAcqDelayCnt>0)
		{
			
			if(nTotalVolCnt>0)
			{			
				msg.Format(_T("%d 종 %d 권의 입수지연처리 작업을 완료했습니다!"),nAcqDelayCnt,nTotalVolCnt);
			}
			else
			{
				msg.Format(_T("입수지연처리 대상 권호가 없습니다!"));
			}
			
			AfxMessageBox(msg);
		}
		

	}
	else
	{
		
		INT nCnt = 	m_pGrid->SelectGetCount();
		msg.Format(_T("%d 건의 자료를 일괄입수지연처리 하시겠습니까?"),nCnt);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;

		ShowAcqDelayProgressDlg();

		//일괄 지연처리는 Thread를 돌린다 
		m_pThreadAcqDelay = AfxBeginThread(ThreadFunc_AcqDelay, this);

	}


}


INT CCHECKIN::DoModalAcqDelaydlg(CString strTitle,BOOL bShowNextBtn)
{
	CCheckinAcqDelay dlg(this);
	dlg.m_strTITLE = strTitle;
	dlg.m_bIsMultiProc = bShowNextBtn;

	INT nOption = dlg.DoModal();

	if(nOption == IDOK) return 0;
	else if(nOption == IDCANCEL) return -1;
	else return -1001; //다음 버튼

	return 0;
}

INT CCHECKIN::AcqDelayComposition(INT nIdx,CProgressCtrl* pProgress)
{
	
	CESL_DataMgr* pDM_WORK = FindDM(_T("DM_체크인_입수지연_작업"));
	CESL_DataMgr* pDM_EXP = FindDM(_T("DM_체크인_입수지연_예측"));
	CESL_DataMgr* pDM_ACQ = NULL;

	//수입 구분 정보
	if(1 == m_nACQ_TYPE) 
		pDM_ACQ = FindDM(_T("DM_체크인_구입정보"));
	else
		pDM_ACQ = FindDM(_T("DM_체크인_기증정보"));

	if(!pDM_WORK || !pDM_EXP || !pDM_ACQ)
	{
		AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
		return -1;
	}

	CString strSPECIES_KEY,strPUB_FREQ;
	strSPECIES_KEY = m_pDM->GetCellData(_T("ISS_종KEY"),nIdx);
	strPUB_FREQ = m_pDM->GetCellData(_T("ISS_간행빈도"),nIdx);
	

	if(strSPECIES_KEY.IsEmpty())
	{
		AfxMessageBox(_T("해당 종KEY를 찾을 수 없습니다!"));
		return -2;
	}
	
	pDM_WORK->FreeData(); 
	pDM_EXP->FreeData(); 
	pDM_ACQ->FreeData();

	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
	pDM_ACQ->RefreshDataManager(strWhere);
	if(0 == pDM_ACQ->GetRowCount()) {
		AfxMessageBox(_T("수입구분정보를 가져올 수 없습니다"));
		return -2;
	}
	
	CString strACQ_KEY = _T("");
	if(1 == m_nACQ_TYPE) strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),0);
	else strACQ_KEY = pDM_ACQ->GetCellData(_T("SD_기증관리KEY"),0);
	

	//예측정보를 구성한다 
	m_arrayVolExpKeyList.RemoveAll();
	m_arrayOtherVolExpKeyList.RemoveAll();
	
	
	CSeExpectMgr mgr(this);
	mgr.InitMgr(pDM_EXP, 
	            pDM_ACQ, 
				strPUB_FREQ, 
				m_nACQ_TYPE, 
        		m_arrayVolExpKeyList,
				m_arrayOtherVolExpKeyList,
				0,
				pProgress);


	//작업DM을 구성한다.
	CSeApi api(this);
	INT nRow_Exp = pDM_EXP->GetRowCount();
	INT nRow_Work;
	CString strVoltitle;
	CString strACQ_EXP_BOOK_CNT;

	for(INT i=0;i<nRow_Exp;i++)
	{
		pDM_WORK->InsertRow(-1);
		nRow_Work = pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(pDM_EXP,i,pDM_WORK,nRow_Work);


		//예측권호를 권호명에 보여준다
		strVoltitle = _T("");
		strVoltitle = pDM_EXP->GetCellData(_T("SA_입수예측권호명"),i);
		pDM_WORK->SetCellData(_T("SV_권호명"),strVoltitle,nRow_Work);


		//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
		api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM,nIdx,pDM_WORK,nRow_Work,pDM_ACQ,m_nACQ_TYPE);

				
		//결호처리를 한다 
		strACQ_EXP_BOOK_CNT = _T("");
		strACQ_EXP_BOOK_CNT = pDM_WORK->GetCellData(_T("SA_입수예정책수"),i);
		pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),i);
		pDM_WORK->SetCellData(_T("SA_결호책수"),strACQ_EXP_BOOK_CNT,i);
		pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호(예정)"),i);
		pDM_WORK->SetCellData(_T("UDF_상태"),_T("M"),i);

	}

	if(pProgress) pProgress->SetPos(100);


	return 0;
}


INT CCHECKIN::AcqDelayProcess(INT nIdx,INT& nTotalProc,CProgressCtrl* pProgress)
{
	
	CSeApi api(this);
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_체크인_권"));
	CESL_DataMgr* pDM_ACQ_VOL = FindDM(_T("DM_체크인_수입구분별권"));
	CESL_DataMgr* pDM_BOOK = FindDM(_T("DM_체크인_책"));
	CESL_DataMgr* pDM_BOOK_INSERT = FindDM(_T("DM_체크인_책_입력"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_체크인_부록"));
	CESL_DataMgr* pDM_WORK = FindDM(_T("DM_체크인_입수지연_작업"));
	CESL_DataMgr* pDM_ACQ = NULL;


	//수입 구분 정보
	if(1 == m_nACQ_TYPE) 
		pDM_ACQ = FindDM(_T("DM_체크인_구입정보"));
	else
		pDM_ACQ = FindDM(_T("DM_체크인_기증정보"));
		

	if(!pDM_WORK || !pDM_ACQ || !pDM_VOL || !pDM_ACQ_VOL || !pDM_BOOK || !pDM_BOOK_INSERT || !pDM_APPENDIX || !pDM_WORK)
	{
		AfxMessageBox(_T("입수지연관련 DM을 찾을 수 없습니다!"));
		return -1;
	}

	INT nRow = pDM_WORK->GetRowCount();
	INT ids = 0;
	CString msg;
	
	for(INT i=0;i<nRow;i++)
	{
		ids = api.ACQ_DELAY(m_pDM,
						    nIdx,
						    pDM_WORK,
						    nRow-i-1,
						    pDM_ACQ,
						    m_nACQ_TYPE,
						    pDM_VOL,
						    pDM_ACQ_VOL,
						    pDM_BOOK,
						    m_arrayVolExpKeyList,
						    m_arrayOtherVolExpKeyList);
					
		
		if(ids<0)
		{	
			msg.Format(_T("[ %d ] 번째 작업에 문제가 생겨 작업을 중단 합니다!"),i+1);
			AfxMessageBox(msg);
			return -2;
		}
		
		nTotalProc++;

		if(pProgress) pProgress->SetPos(i+1);
	}


	return ids;
}

CString CCHECKIN::GetCurrentYear()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;

}

VOID CCHECKIN::SetReceiptNo()
{

	CString strACQ_YEAR;
	m_pCM->GetControlMgrData(_T("수입년도"),strACQ_YEAR);  

	CCheckinReceipt dlg(this);
	dlg.m_strACQ_YEAR = strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	UpdateData(FALSE);

	if(dlg.DoModal() == IDOK)
	{
		m_pCM->SetControlMgrData(_T("수입년도"),dlg.m_strACQ_YEAR);   
		m_pCM->SetControlMgrData(_T("접수번호"),dlg.m_strRECEIPT_NO);	 
	}
	
}

INT CCHECKIN::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

BEGIN_EVENTSINK_MAP(CCHECKIN, CDialog)
    //{{AFX_EVENTSINK_MAP(CCHECKIN)
	ON_EVENT(CCHECKIN, IDC_gridCHEKIN, -600 /* Click */, OnClickgridCHEKIN, VTS_NONE)
	ON_EVENT(CCHECKIN, IDC_gridCHEKIN, -601 /* DblClick */, OnDblClickgridCHEKIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCHECKIN::OnClickgridCHEKIN() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;

	}
	
}

UINT ThreadFunc_AcqDelay(LPVOID pParam)
{
	CCHECKIN *pDlg = (CCHECKIN*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	//체크인 화면을 disable 시킨다.
	pDlg->EnableWindow(FALSE);
	
	INT idx;
	INT nProcCnt;
	idx = pDlg->m_pGrid->SelectGetHeadPosition();
	nProcCnt = pDlg->m_pGrid->SelectGetCount();
	pDlg->m_pProgressDlg->BringWindowToTop();
	pDlg->m_pProgressDlg->m_ctrlProgressTop.SetRange(0,nProcCnt);
	pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(1);
	pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetPos(0);
	pDlg->m_pProgressDlg->CenterWindow();
	pDlg->m_pProgressDlg->ShowWindow(SW_SHOW);
	CString strTITLE;
	CString strProgress;

	nProcCnt = 0;
	INT ids = 0;
	INT nWorkDMRowCnt = 0;
	INT nAcqDelayCnt = 0;
	CString msg;
	INT nTotalVolCnt = 0;

	while(idx>=0)
	{
		
		//예측정보 
		pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetRange(0,100);

		strTITLE = _T("");
		strTITLE = pDlg->m_pDM->GetCellData(_T("ISS_본표제"),idx);  
		strProgress.Format(_T("[ %s ] \n 종을 지연 처리하는 중입니다"),strTITLE);
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcACQ_DELAY_TOP)->SetWindowText(strProgress);
		strProgress.Format(_T("예측정보를 구성 하는 중입니다"));
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcACQ_DELAY_BOTTOM)->SetWindowText(strProgress);
		
		pDlg->AcqDelayComposition(idx,&pDlg->m_pProgressDlg->m_ctrlProgressBottom);
		nWorkDMRowCnt = pDlg->m_pDM_WORK->GetRowCount();
		


		//지연처리  
		pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetRange(0,nWorkDMRowCnt);
		strProgress.Format(_T("권호별 지연처리를 하는 중입니다"));
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcACQ_DELAY_BOTTOM)->SetWindowText(strProgress);
		

		pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetPos(0);
		ids = pDlg->AcqDelayProcess(idx,nTotalVolCnt,&pDlg->m_pProgressDlg->m_ctrlProgressBottom);
		if(ids<0) {
			pDlg->m_pProgressDlg->ShowWindow(SW_HIDE);
			pDlg->EnableWindow(TRUE);
			pDlg->m_bIsThreadRunning = FALSE;
			return -1;
		}
		else
		{
			nAcqDelayCnt++;
		}

		//위쪽 프로그래시브 바
		pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(nProcCnt+2);
		nProcCnt++;
		idx = pDlg->m_pGrid->SelectGetNext();
	}
	

	//체크인 화면을 eable 시킨다.
	pDlg->m_pProgressDlg->ShowWindow(SW_HIDE);
	pDlg->EnableWindow(TRUE);
	pDlg->m_bIsThreadRunning = FALSE;
	SetEvent(pDlg->m_hThread);
	
	
	if(nAcqDelayCnt>0)
	{
			
		if(nTotalVolCnt>0)
		{			
			msg.Format(_T("%d 종 %d 권의 입수지연처리 작업을 완료했습니다!"),nAcqDelayCnt,nTotalVolCnt);
		}
		else
		{
			msg.Format(_T("입수지연처리 대상 권호가 없습니다!"));
		}
			
		AfxMessageBox(msg);
	}

	return 0;
}



VOID CCHECKIN::OnDblClickgridCHEKIN() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
		LaunchCheckinVol();
	//ModifySpecies();
	
}

VOID CCHECKIN::OnRFID(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == m_bRFID) return;
	if(2 == m_nACQ_TYPE) m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D");

	CString strSerial;
	strSerial.Format(_T("%s"),lParam);
	
	//해당 책의 종정보를 검색해 보여준다
	CString strQuery;
	CString strTmpQuery;

	//수입 구분 조건 
	if(1 == m_nACQ_TYPE) {
	
		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = P.SPECIES_KEY");
	
	} else if(2 == m_nACQ_TYPE) {

		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = D.SPECIES_KEY");
	
	}

	strTmpQuery.Format(_T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE WORKNO = '%s')"),strSerial); 

	strQuery += strTmpQuery;

	m_pDM->RefreshDataManager(strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	INT nRowCnt = m_pDM->GetRowCount();
	m_pGrid->Display();


	if(nRowCnt>0) 
	{
		m_nCurrentIdx = 0;
		m_pGrid->SetAt(0,0,_T("V"));
	}
	else
	{
		m_nCurrentIdx = -1;
	}


	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}
	else
	{
		CString str;
		str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}

}

VOID CCHECKIN::RFID_test()
{
	//해당 책의 종정보를 검색해 보여준다
	if(2 == m_nACQ_TYPE) m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D");

	CString strQuery;
	CString strTmpQuery;

	//수입 구분 조건 
	if(1 == m_nACQ_TYPE) {
	
		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = P.SPECIES_KEY");
	
	} else if(2 == m_nACQ_TYPE) {

		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = D.SPECIES_KEY");
	
	}

	m_pDM->RefreshDataManager(strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt) return;

	m_pGrid->Display();
	m_nCurrentIdx = 0;
	m_pGrid->SetAt(0,0,_T("V"));
}

VOID CCHECKIN::ShowAcqDelayProgressDlg()
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



VOID CCHECKIN::AccountStatement()
{
	CSeAccountStatement dlg(this);
	dlg.DoModal();
}


HBRUSH CCHECKIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}