// CHECKIN.cpp : implementation file
//

#include "stdafx.h"
#include "CHECKIN.h"
#include "CheckinAcqDelay.h"
#include "..\SE_API\CheckinReceipt.h"

#include "..\SE_CHECKIN_VOL\CheckinVolProgressive.h"
#include "..\SE_SPECIES\SeSpeciesForm.h"
#include "..\SE_ACCOUNT_STATEMENT\SeAccountStatement.h"

#include "..\SE_VOL\SeVolDlg.h"
#include "CheckinSisacSpeciesList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCHECKIN dialog


CCHECKIN::CCHECKIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
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

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_SeApi = new CSeApi(this);
	m_SeExpMgr = new CSeExpectMgr(this);

	AfxInitRichEdit();


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

	// 17/09/06 김혜영 : 연속>구입>체크인관리에서 메모리누수 현상 발생
//*/ ADD ---------------------------------------------------------------------------
	if(m_SeApi)
	{
		delete m_SeApi;
		m_SeApi = NULL;
	}
	
	if(m_SeExpMgr)
	{
		delete m_SeExpMgr;
		m_SeExpMgr = NULL;
	}
//*/ END ---------------------------------------------------------------------------
	
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
	
	pGrid->MoveWindow(-2, 35 , cx+2, cy-35);	

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
	
	//라이트 버젼여부를 가져온다
	CSeApi api(this);
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
	
	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
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

VOID CCHECKIN::LaunchCheckinVol(BOOL bShowMode)
{
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-009 JY 연속간행물 기능 안정화
	if (m_pDM->GetRowCount() <= 0) return;
	
	//수입년도와 접수번호를 확인한다 
	CString strACQ_YEAR;
	CString strRECEIPT_NO;
	m_pCM->GetControlMgrData(_T("접수번호"),strRECEIPT_NO);   
	m_pCM->GetControlMgrData(_T("수입년도"),strACQ_YEAR); 

	m_pGrid->SelectMakeList();

	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-009 JY 연속간행물 기능 안정화
	//==========================================================================
	//2010.01.11 ADD BY PJW : 선정값에 현재 index데이터 추가한다.
	//선정하지 않고 해당열을 선택만 했을경우 
	//if( m_pGrid->SelectGetCount() > 0 ) 
	//==========================================================================
	if( m_pGrid->SelectGetCount() == 0 ) 
	{
		m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
		m_pGrid->SelectMakeList();
	}
	
	CString msg;
	
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-009 JY 연속간행물 기능 안정화
	/*//===========================================================================
	if( m_pDM->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("선정된 정보가 없습니다!"));
		return;
	}
	*///===========================================================================
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

	if(bShowMode) 
	{
		//권별 체크인 화면을 띄운다
		m_CheckinVolLauncher.SetParentInfo(m_pDM, m_pGrid, m_nACQ_TYPE,strACQ_YEAR,strRECEIPT_NO,m_bIsLightVersion);
		m_CheckinVolLauncher.LaunchCheckinVol(GetParentFrame(),this);
	}
	else
	{
		INT nCnt = 	m_pGrid->SelectGetCount();
		msg.Format(_T("%d 건의 자료를 일괄입수처리 하시겠습니까?"),nCnt);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;

		ShowAcqDelayProgressDlg();

		//일괄 지연처리는 Thread를 돌린다 
		m_pThreadAcqExe = AfxBeginThread(ThreadFunc_AcqExe, this);
	}
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
	//=====================================================
	//2009.02.17 UPDATE BY PJW : 엑셀파일이 있으면 용지를 셋팅하지 않는다.
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	CString strChkFile = _T("..\\CFG\\RptStyle\\연속체크인목록.xls");
	CFileFind fFind;
	if( !fFind.FindFile(strChkFile) )
	{		
		pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	}
	//=====================================================
	
	
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

INT CCHECKIN::CopyExpToWork()
{
	INT nRow_Exp = m_pDM_EXP->GetRowCount();
	INT nRow_Work;
	CString strVoltitle;
	CSeApi api(this);
	CString strBeforeRow;

	for(INT i=0;i<nRow_Exp;i++)
	{
		m_pDM_VOL_WORK->InsertRow(-1);
		nRow_Work = m_pDM_VOL_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_EXP,i,m_pDM_VOL_WORK,nRow_Work);
		

		//예측권호를 권호명에 보여준다
		strVoltitle = _T("");
		strVoltitle = m_pDM_EXP->GetCellData(_T("SA_입수예측권호명"),i);
		m_pDM_VOL_WORK->SetCellData(_T("SV_권호명"),strVoltitle,nRow_Work);
		m_pDM_VOL_WORK->SetCellData(_T("UDF_입수상태"),_T("예정"),nRow_Work);
		m_pDM_VOL_WORK->SetCellData(_T("UDF_상태"),_T("E"),nRow_Work);
		m_pDM_VOL_WORK->SetCellData(_T("SV_권호구분"),_T("1"),nRow_Work);
		m_pDM_VOL_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
	
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_VOL_WORK->SetCellData(_T("UDF_행"),strBeforeRow,nRow_Work);

		//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
		//=====================================================
		//2009.02.18 UPDATE BY PJW : ROW INDEX를 그리드의 선택된것으로 설정한다.
// 		api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM,nSpeciesIdx,m_pDM_VOL_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		INT nGridRowIdx;
		INT nSpeciesIdx;
		INT nSelectGridCnt;
		CString strSpeciesDmKey;
		CString strChkDmKey;
		nSpeciesIdx = 0;
 		m_pGrid->SelectMakeList();
		nSelectGridCnt = m_pGrid->SelectGetCount();
 		nGridRowIdx = m_pGrid->SelectGetHeadPosition();
		
		strChkDmKey = m_pDM_ACQ->GetCellData(_T("SP_종KEY"),0);
		for( INT j = 0 ; j < nSelectGridCnt ; j++ )
		{
			strSpeciesDmKey = m_pDM->GetCellData(_T("ISS_종KEY"),nGridRowIdx);
			
			if( strChkDmKey == strSpeciesDmKey )
			{
				nSpeciesIdx = nGridRowIdx;
				break;
			}
			nGridRowIdx = m_pGrid->SelectGetNext();
		}

		api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM,nSpeciesIdx,m_pDM_VOL_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		//=====================================================
		
		

	}
	
	return 0;
}

INT	CCHECKIN::GetAutoExpCount()
{
	int iret; 

	CString strAutoUseYN = _T("N");
	CString strAutoCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				if(_T("AUTO_USE") == tmp)
				{
					strAutoUseYN = strLine.Mid(nPos+1);
					strAutoUseYN.TrimLeft();
					strAutoUseYN.TrimRight();
				}
				else if(_T("AUTO_COUNT") == tmp)
				{
					strAutoCount = strLine.Mid(nPos+1);
					strAutoCount.TrimLeft();
					strAutoCount.TrimRight();
				}

			}
		}

		file.Close();
	}

	iret=atoi((char*)strAutoCount.GetBuffer(0));
	
	return iret;
}

INT CCHECKIN::GetAcqYearAndReceiptNo(CString &strRetAcqYear,CString &strRetReceiptNo)
{
	CSeApi api(this);
	CString strTemp;
	
	m_pCM->GetControlMgrData(_T("수입년도"),strTemp);  
	if(strTemp.IsEmpty())
	{
		strRetAcqYear = api.GetCurrentYear();
	}
	else
	{
		strRetAcqYear = strTemp;
	}

	strTemp.Empty();
	m_pCM->GetControlMgrData(_T("접수번호"),strTemp);  
	if(strTemp != "" )
	{
		strRetReceiptNo = strTemp;
	}
	else
	{
		CString strRECEIPT_NO;
		strRECEIPT_NO = api.GetLastReceiptNo(m_pDM_VOL_WORK,m_nACQ_TYPE,strRetAcqYear);

		CString strDate;
		strDate = api.GetTodayDate();

		CString strWorkLog;
		strWorkLog = GetWorkLogMsg(WORK_LOG_CHECKIN);

		m_pDM_WORK->StartFrame();
		m_pDM_WORK->InitDBFieldData();
		// 2005-04-11 By SM5ong
		// 마지막 차수번호+1로 Update
		// BEGIN:
		CString strKindCode, strQuery;
  		if(1 == m_nACQ_TYPE)
			strKindCode = _T("CK1_NO"); //구입 접수번호 
		else if(2 == m_nACQ_TYPE)
			strKindCode = _T("CK2_NO"); //기증 접수번호 
  
		if( _T("1") == strRECEIPT_NO )	//검색결과가 없으면 INSERT 
		{
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
							_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s',UDF_MANAGE_CODE);"), 
							DEFAULT_SEQ_NAME, strKindCode, strRetAcqYear, strDate, m_pInfo->USER_ID, strWorkLog);
			m_pDM_WORK->AddFrame(strQuery); 
		}
		else	//있으면 UPDATE
		{
			//++2008.10.17 UPDATE BY PWR {{++
			// 통합시스템 적용
			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%s, LAST_WORK='%s' ")
							_T("WHERE MAKE_YEAR='%s' AND KIND_CODE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
							, strRECEIPT_NO, strWorkLog, strRetAcqYear, strKindCode );
//			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),
//							strRECEIPT_NO, strWorkLog, strRetAcqYear, strKindCode);
			//--2008.10.17 UPDATE BY PWR --}}
			m_pDM_WORK->AddFrame(strQuery);
		}
		// END:
		strQuery.Format(_T("INSERT INTO SE_RECEIPT_TBL ")
						_T("(REC_KEY, ACQ_CODE, ACQ_YEAR, RECEIPT_NO, LAST_SERIAL_NO, ")
						_T("RECEIPT_NO_CREATE_DATE, RECEIPT_NO_CREATE_WORKER, FIRST_WORK, LAST_WORK,MANAGE_CODE) ")
						_T("VALUES (%s.NEXTVAL,'%d','%s','%s',0,'%s','%s','%s','%s',UDF_MANAGE_CODE);")
						,DEFAULT_SEQ_NAME,m_nACQ_TYPE,strRetAcqYear,
						strRECEIPT_NO, strDate, m_pInfo->USER_ID, strWorkLog, strWorkLog);
		m_pDM_WORK->AddFrame(strQuery);
						
		INT ids = m_pDM_WORK->SendFrame();
		m_pDM_WORK->EndFrame();
		if(ids<0) return -1;

		strRetReceiptNo = strRECEIPT_NO;		
	}

	return 0;
}

INT CCHECKIN::AcqExeProcess()
{
	return 0;
}

INT CCHECKIN::CheckinForUser(INT nRowIdx)
{
	CString strACQ_EXP_BOOK_CNT;
	strACQ_EXP_BOOK_CNT = m_pDM_VOL_WORK->GetCellData(_T("SA_입수예정책수"),nRowIdx);
	if(!strACQ_EXP_BOOK_CNT.IsEmpty()) m_pDM_VOL_WORK->SetCellData(_T("SA_입수책수"),strACQ_EXP_BOOK_CNT,nRowIdx);
	m_pDM_VOL_WORK->SetCellData(_T("SA_결호책수"),_T("0"),nRowIdx);
	m_pDM_VOL_WORK->SetCellData(_T("UDF_입수상태"),_T("입수(예정)"),nRowIdx);
	m_pDM_VOL_WORK->SetCellData(_T("UDF_상태"),_T("C"),nRowIdx);
	//m_pGrid->DisplayLine(nRowIdx);
	
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

UINT ThreadFunc_AcqExe(LPVOID pParam)
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

	pDlg->m_pProgressDlg->CenterWindow();
	pDlg->m_pProgressDlg->ShowWindow(SW_SHOW);
	CString strTITLE;
	CString strProgress;

	pDlg->m_pDM_EXP  = pDlg->FindDM(_T("DM_체크인_예측"));
	pDlg->m_pDM_ACQ  = pDlg->FindDM(_T("DM_체크인_구입정보"));	
	pDlg->m_pDM_VOL_WORK  = pDlg->FindDM(_T("DM_체크인_작업"));	

	CString strAcqYear,strRECEIPT_NO;
	pDlg->GetAcqYearAndReceiptNo(strAcqYear,strRECEIPT_NO);

	int itemp=pDlg->m_pDM->GetRowCount();
	int nTotSpeciesCnt=0,nExeSpeciesCnt=0;
	nProcCnt=0;
	for(int i=0;i<pDlg->m_pDM->GetRowCount();i++)
	{
		CString tSelected=pDlg->m_pGrid->GetTextMatrix(i+1,1);
		if ( tSelected != "V" )
			continue;
		nTotSpeciesCnt++;

		strTITLE = _T("");
		strTITLE = pDlg->m_pDM->GetCellData(_T("ISS_표제사항"),i);  
		strProgress.Format(_T("[ %s ] \n 종을 입수 처리하는 중입니다"),strTITLE);
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcACQ_DELAY_TOP)->SetWindowText(strProgress);

		CArray<CString,CString> arrayVolExpKeyList;
		CArray<CString,CString> arrayOtherVolExpKeyList;

		pDlg->m_pDM_EXP->FreeData();
		pDlg->m_pDM_ACQ->FreeData();
		pDlg->m_pDM_VOL_WORK->FreeData(); 

		CString strSPECIES_KEY;
		strSPECIES_KEY=pDlg->m_pDM->GetCellData(_T("ISS_종KEY"),i);  
		CString strWhere;
		strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
		pDlg->m_pDM_ACQ->RefreshDataManager(strWhere);

		int nExpCnt=pDlg->GetAutoExpCount();

		CString strPUB_FREQ=pDlg->m_pDM->GetCellData(_T("ISS_간행빈도"),i);

		pDlg->m_SeExpMgr->InitMgr(pDlg->m_pDM_EXP, 
					pDlg->m_pDM_ACQ,
					strPUB_FREQ,
					pDlg->m_nACQ_TYPE,
					arrayVolExpKeyList,
					arrayOtherVolExpKeyList,
					nExpCnt,
					NULL);

		nExpCnt = pDlg->m_pDM_EXP->GetRowCount();
		//=====================================================
		//2009.02.09 ADD BY PJW : 현재 날짜보다 뒤의 입수예정일을 가진 데이터는 DM에서 제외한다.
		INT nChkDm = 0;
		while(nChkDm < nExpCnt)
		{
			CTime t = CTime::GetCurrentTime();
			CString strDmTime;
			CString strCurTime;
			strCurTime.Format(_T("%04d%02d%02d"),t.GetYear(), t.GetMonth(), t.GetDay());
			pDlg->m_pDM_EXP->GetCellData(_T("SA_입수예정일"),nChkDm, strDmTime);			
			strDmTime.Replace(_T("/"),_T(""));
			if( _ttoi(strCurTime) < _ttoi(strDmTime) )
			{
				pDlg->m_pDM_EXP->DeleteRow(nChkDm);
				nExpCnt--;
			}
			else
			{
				nChkDm++;
			}
		}
		//=====================================================
		if ( pDlg->m_pDM_EXP->GetRowCount() == 0 )
		{
			pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(++nProcCnt);
			continue;
		}

		pDlg->CopyExpToWork();
		CESL_DataMgr* pDM_VOL		  = pDlg->FindDM(_T("DM_체크인_권"));
		CESL_DataMgr* pDM_BOOK		  = pDlg->FindDM(_T("DM_체크인_책"));
		CESL_DataMgr* pDM_BOOK_INSERT = pDlg->FindDM(_T("DM_체크인_책_입력"));
		CESL_DataMgr* pDM_APPENDIX	  = pDlg->FindDM(_T("DM_체크인_부록"));
		CESL_DataMgr* pDM_ACQ_VOL	  = pDlg->FindDM(_T("DM_체크인_수입구분별권"));
		CESL_DataMgr* pDM_DEPART = pDlg->FindDM(_T("DM_체크인_부서지정자료"));

		int t1 = pDM_APPENDIX->GetRowCount();
		pDM_APPENDIX->FreeData();
		t1 = pDM_APPENDIX->GetRowCount();

		pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetRange(0,nExpCnt);
		for(int j=nExpCnt-1;j>=0;j--)	
		{
			pDlg->CheckinForUser(j);
			
			pDlg->m_SeApi->CHECK_IN(pDlg->m_pDM,
					 i,
					 pDlg->m_pDM_VOL_WORK,
					 j,
					 pDlg->m_pDM_ACQ,
					 pDlg->m_nACQ_TYPE,
					 pDM_VOL,
					 pDM_ACQ_VOL,
					 pDM_BOOK,
					 arrayVolExpKeyList,
					 arrayOtherVolExpKeyList,
					 pDM_BOOK_INSERT,
					 pDM_APPENDIX,
					 strAcqYear,
					 strRECEIPT_NO,
					 FALSE,
					 pDM_DEPART);	
			pDlg->m_pProgressDlg->m_ctrlProgressBottom.SetPos(nExpCnt-j);
		}
		nExeSpeciesCnt++;

		//위쪽 프로그래시브 바
		pDlg->m_pProgressDlg->m_ctrlProgressTop.SetPos(++nProcCnt);
	}

	//체크인 화면을 eable 시킨다.
	pDlg->m_pProgressDlg->ShowWindow(SW_HIDE);
	pDlg->EnableWindow(TRUE);
	pDlg->m_bIsThreadRunning = FALSE;
	SetEvent(pDlg->m_hThread);
		
	CString msg;
	msg.Format(_T("총 %d종 중에서 %d종 입수처리 작업을 완료했습니다!"),nTotSpeciesCnt,nExeSpeciesCnt);
	AfxMessageBox(msg);

	return 0;
}

INT CCHECKIN::ScanSisacBarcode(TCHAR *szBarcode)
{
	//
	return 0;
}

void CCHECKIN::GetSisacBarcodeInfo2(TCHAR *fi_szBarcode, SISAC_BARCODE_INFO_TYPE *fi_sBarcodeInfo)
{
	CString	strTemp;

	// ---------------------------------------------------------------------
	// 0.SISAC CODE
	// ---------------------------------------------------------------------
	strTemp.Format(_T("%s"), fi_szBarcode);
	m_SisacCode = strTemp;

	// ---------------------------------------------------------------------
	// 1.ISSN
	// ---------------------------------------------------------------------
	strTemp.Format(_T("%s"), fi_sBarcodeInfo->szISSN);
	m_ISSN = strTemp;

	// ---------------------------------------------------------------------
	// 2.Publish
	// ---------------------------------------------------------------------
	// 2-1.Date Code
	strTemp.Format(_T("%s"), fi_sBarcodeInfo->sPub_Info.szPub_code);
	m_DateCode = strTemp;

	// 2-2.Date Exist
	strTemp.Format(_T("%c"), fi_sBarcodeInfo->sPub_Info.cExist_yn);
	m_DateExist = strTemp;

	// 2-3.Date Range
	strTemp.Format(_T("%c"), fi_sBarcodeInfo->sPub_Info.cRange_yn);
	m_DateRange = strTemp;

	// 2-4.Date Type
	strTemp.Format( _T("%c"), fi_sBarcodeInfo->sPub_Info.cPub_type );
	m_DateType = strTemp;

	// 6.Date
	if ( fi_sBarcodeInfo->sPub_Info.cPub_type == _T('Y') ) 
	{
		strTemp.Format(_T("%s"), fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		m_StartDate = strTemp;
		
		if ( fi_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') )
		{
			strTemp.Format(_T("%s"), fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			m_EndDate = strTemp;
		}
	}
	else if ( fi_sBarcodeInfo->sPub_Info.cPub_type == _T('M') )
	{
		strTemp.Format(_T("%s%s"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear, 
									fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth );
		m_StartDate = strTemp;

		if ( fi_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') )
		{
			strTemp.Format(_T("%s%s"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear, 
									fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth );
			m_EndDate = strTemp;
		}
	}
	else if ( fi_sBarcodeInfo->sPub_Info.cPub_type == _T('D') )
	{
		strTemp.Format(_T("%s%s%s"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear, 
										fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, 
										fi_sBarcodeInfo->sPub_Info.sStart_Pub.szDay);
		m_StartDate = strTemp;

		if ( fi_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') )
		{
			strTemp.Format(_T("%s%s%s"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear, 
										fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, 
										fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay);
			m_EndDate = strTemp;
		}
	}
	else if ( fi_sBarcodeInfo->sPub_Info.cPub_type == _T('S') )
	{
		if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("21"), 2 ) == 0 )
			strTemp.Format(_T("%s 봄"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("22"), 2 ) == 0 )
			strTemp.Format(_T("%s 여름"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("23"), 2 ) == 0 )
			strTemp.Format(_T("%s 가을"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("24"), 2 ) == 0 )
			strTemp.Format(_T("%s 겨울"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
											
		m_StartDate = strTemp;

		if ( fi_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') )
		{
			if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("21"), 2 ) == 0 )
				strTemp.Format(_T("%s 봄"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("22"), 2 ) == 0 )
				strTemp.Format(_T("%s 여름"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("23"), 2 ) == 0 )
				strTemp.Format(_T("%s 가을"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("24"), 2 ) == 0 )
				strTemp.Format(_T("%s 겨울"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );

			m_EndDate = strTemp;
		}
	}
	else if ( fi_sBarcodeInfo->sPub_Info.cPub_type == _T('Q') )
	{
		if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("31"), 2 ) == 0 )
			strTemp.Format(_T("%s 1/4분기"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("32"), 2 ) == 0 )
			strTemp.Format(_T("%s 2/4분기"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("33"), 2 ) == 0 )
			strTemp.Format(_T("%s 3/4분기"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
		else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("34"), 2 ) == 0 )
			strTemp.Format(_T("%s 4/4분기"),  fi_sBarcodeInfo->sPub_Info.sStart_Pub.szYear );
											
		m_StartDate = strTemp;

		if ( fi_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') ) 
		{
			if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("31"), 2 ) == 0 )
				strTemp.Format(_T("%s 1/4분기"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("32"), 2 ) == 0 )
				strTemp.Format(_T("%s 2/4분기"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("33"), 2 ) == 0 )
				strTemp.Format(_T("%s 3/4분기"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );
			else if ( _tcsncmp( fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("34"), 2 ) == 0 )
				strTemp.Format(_T("%s 4/4분기"),  fi_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear );

			m_EndDate = strTemp;
		}
	}

	// ---------------------------------------------------------------------
	// 3.Enum
	// ---------------------------------------------------------------------
	// 3-1.Enum Code
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.szEnum_code );
	m_EnumCode = strTemp;

	// 3-2.Enum Exist
	strTemp.Format(_T("%c"), fi_sBarcodeInfo->sEnum_Info.cExist_yn);
	m_EnumExist = strTemp;

	// 3-3.Enum Merge
	strTemp.Format( _T("%c"), fi_sBarcodeInfo->sEnum_Info.cMerged_yn );
	m_EnumMerge = strTemp;	
	
	// 3-4.Enum Format
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.szEnum_format );
	m_EnumFormat = strTemp;

	// 3-5.Enum
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sStart_Enum.szVol );
	m_StartVol = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sStart_Enum.szNum );
	m_StartNum = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sStart_Enum.szPart );
	m_StartPart = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sStart_Enum.szSec );
	m_StartSec = strTemp;
	
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sEnd_Enum.szVol );
	m_EndVol = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sEnd_Enum.szNum );
	m_EndNum = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sEnd_Enum.szPart );
	m_EndPart = strTemp;
	strTemp.Format( _T("%s"), fi_sBarcodeInfo->sEnum_Info.sEnd_Enum.szSec );
	m_EndSec = strTemp;

	UpdateData(FALSE);
}

CString CCHECKIN::GetPubDate(CString strData)
{
	CString strYear,strMon,strDay,strRet;

	if ( strData.GetLength() == 8 )
	{
		strYear = strData.Left(4);
		strMon = strData.Mid(4,2);
		strDay = strData.Mid(6,2);
	}
	else if( strData.GetLength() == 6 )
	{
		strYear = strData.Left(4);
		strMon = strData.Mid(4,2);
		strDay = _T("01");
	}
	else
	{
		strYear = strData.Left(4);
		strMon = _T("01");
		strDay = _T("01");
	}

	strRet = strYear+_T("/")+strMon+_T("/")+strDay;

    return strRet;
}

INT CCHECKIN::SisacBarCode()
{
	int ids;

	// sisac 바코드로 종정보를 조회한다
	CCheckinSisacSearch dlg;
	if(dlg.DoModal() == IDCANCEL) 
		return -1;

	CSisac				sisacTest;
	SISAC_BARCODE_INFO_TYPE	sBarcodeInfo;

	ids = sisacTest.GetSisacBarcodeInfo (dlg.m_strSisacData.GetBuffer(0), & sBarcodeInfo );
	if ( ids )
	{
		AfxMessageBox(_T("바코드 입력값에 오류가 있습니다"));
		return -1;
	}
	GetSisacBarcodeInfo2( dlg.m_strSisacData.GetBuffer(0), &sBarcodeInfo);
	
	// 일단 ISSN으로 조회한다.
	CString strQry,strTemp,strISSN,strPubDate,strVol1,strVol2,strAcqDate;
	
	if ( m_ISSN.GetLength() == 8 )
	{
		strISSN = m_ISSN.Left(4)+_T("-")+m_ISSN.Right(4);
	}
	else
	{
		AfxMessageBox(_T("ISSN값에 오류가 있습니다"));
		return -1;
	}

	CString strPubYear,strPubMon,strPubDay;
	strPubDate = GetPubDate(m_StartDate);
	strVol1 = m_StartVol;
	strVol2 = m_StartNum;

	m_pDM->FreeData();
	strQry+=_T(" I.REC_KEY = S.REC_KEY ");
	strQry+=_T(" AND I.REC_KEY = P.SPECIES_KEY ");
	strTemp.Format(_T(" AND I.ST_ISSN='%s' "),strISSN);

	strQry+=strTemp;
	m_pDM->RefreshDataManager(strQry);

	if( m_pDM->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("서지정보가 존재하지 않습니다."));
		return -1;
	}
	else if( m_pDM->GetRowCount() > 1 )
	{
		CCheckinSisacSpeciesList dlg;
		dlg.m_strISSN = strISSN;
		if(dlg.DoModal() == IDOK)
		{
			strTemp.Format(_T(" AND I.REC_KEY='%s' "),dlg.m_strSpeciesKey);
			strQry+=strTemp;
			m_pDM->RefreshDataManager(strQry);
		}
		else
			return -1;
	}		

	CString strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
	CString strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);						

	if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),0);
	}
	else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),0);
	}
	else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
	{
		m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),0);
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행상태코드"),_T("UDF_간행상태"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));

	m_pDM_EXP  = FindDM(_T("DM_체크인_예측"));
	m_pDM_ACQ  = FindDM(_T("DM_체크인_구입정보"));	
	m_pDM_VOL_WORK  = FindDM(_T("DM_체크인_작업"));	

	CString strAcqYear,strRECEIPT_NO;
	GetAcqYearAndReceiptNo(strAcqYear,strRECEIPT_NO);

	CArray<CString,CString> arrayVolExpKeyList;
	CArray<CString,CString> arrayOtherVolExpKeyList;

	m_pDM_EXP->FreeData();
	m_pDM_ACQ->FreeData();
	m_pDM_VOL_WORK->FreeData(); 

	CString strSPECIES_KEY;
	strSPECIES_KEY=m_pDM->GetCellData(_T("ISS_종KEY"),0);  
	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSPECIES_KEY);
	m_pDM_ACQ->RefreshDataManager(strWhere);

	int nExpCnt=1;
	CString strPUB_FREQ=m_pDM->GetCellData(_T("ISS_간행빈도"),0);
	m_SeExpMgr->InitMgr(m_pDM_EXP, 
					m_pDM_ACQ,
					strPUB_FREQ,
					m_nACQ_TYPE,
					arrayVolExpKeyList,
					arrayOtherVolExpKeyList,
					nExpCnt,
					NULL);

	m_pDM_VOL_WORK->InsertRow(-1);
	CopyDMToDMByAlias(m_pDM_EXP,0,m_pDM_VOL_WORK,0);
	
	//예측권호를 권호명에 보여준다
	CString strVoltitle = _T("");
	strVoltitle = m_pDM_EXP->GetCellData(_T("SA_입수예측권호명"),0);
	m_pDM_VOL_WORK->SetCellData(_T("SV_권호명"),strVoltitle,0);
	m_pDM_VOL_WORK->SetCellData(_T("UDF_입수상태"),_T("예정"),0);
	m_pDM_VOL_WORK->SetCellData(_T("UDF_상태"),_T("E"),0);
	m_pDM_VOL_WORK->SetCellData(_T("SV_권호구분"),_T("1"),0);
	m_pDM_VOL_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),0);		
	m_pDM_VOL_WORK->SetCellData(_T("UDF_행"),_T("0"),0);

	//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
	m_SeApi->MAKE_EXP_WORK_by_IDX_ACQ(m_pDM,0,m_pDM_VOL_WORK,0,m_pDM_ACQ,1);

	CESL_DataMgr* pDM_VOL		  = FindDM(_T("DM_체크인_권"));
	CESL_DataMgr* pDM_BOOK		  = FindDM(_T("DM_체크인_책"));
	CESL_DataMgr* pDM_BOOK_INSERT = FindDM(_T("DM_체크인_책_입력"));
	CESL_DataMgr* pDM_APPENDIX	  = FindDM(_T("DM_체크인_부록"));
	CESL_DataMgr* pDM_ACQ_VOL	  = FindDM(_T("DM_체크인_수입구분별권"));
	CESL_DataMgr* pDM_DEPART = FindDM(_T("DM_체크인_부서지정자료"));

	CheckinForUser(0);
	m_pDM_VOL_WORK->SetCellData(_T("SV_권호명"),strVol1,0);
	m_pDM_VOL_WORK->SetCellData(_T("SV_부차적권호명"),strVol2,0);
	m_pDM_VOL_WORK->SetCellData(_T("SV_발행일"),strPubDate,0);

	CSeVolDlg dlg2(this);
	dlg2.m_pParentGrid = m_pGrid;
	dlg2.m_nOpenMode = 1;
	dlg2.m_nACQ_TYPE = 1;

	if(dlg2.DoModal() == IDOK) 
	{
		ids = m_SeApi->CHECK_IN(m_pDM,
					 0,
					 m_pDM_VOL_WORK,
					 0,
					 m_pDM_ACQ,
					 m_nACQ_TYPE,
					 pDM_VOL,
					 pDM_ACQ_VOL,
					 pDM_BOOK,
					 arrayVolExpKeyList,
					 arrayOtherVolExpKeyList,
					 pDM_BOOK_INSERT,
					 pDM_APPENDIX,
					 strAcqYear,
					 strRECEIPT_NO,
					 FALSE,			
					 pDM_DEPART);
	
		if ( ids >= 0 )
		{
			AfxMessageBox(_T("입수처리가 완료되었습니다"));
			m_pDM->RefreshDataManager(strQry);

			CString strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
			CString strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);						
			if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
			{
				m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),0);
			}
			else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
			{
				m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),0);
			}
			else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
			{
				m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),0);
			}
			
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행상태코드"),_T("UDF_간행상태"));
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
			m_pGrid->Display();
		}
	}

	return 0;
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




