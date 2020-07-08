// TRANSFER.cpp : implementation file
//

#include "stdafx.h"
#include "TRANSFER.h"
#include "..\SE_API\SeApi.h"
#include "TransferDetail.h"
#include "TransferMgr.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "TransferRegNo.h"
#include "TransferLabelPrint.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTRANSFER dialog


CTRANSFER::CTRANSFER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTRANSFER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nACQ_TYPE = 1; //default
	m_pDlg = NULL;
	m_pDM = NULL;
	m_pDM_PRINT = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_nSearchType = 0; //default
	m_bIsLightVersion = FALSE;
}

CTRANSFER::~CTRANSFER()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}


VOID CTRANSFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTRANSFER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTRANSFER, CDialog)
	//{{AFX_MSG_MAP(CTRANSFER)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTRANSFER message handlers
BOOL CTRANSFER::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CTRANSFER::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridTRANSFER);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	

}

VOID CTRANSFER::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}

VOID CTRANSFER::OnSendQuery(WPARAM w,LPARAM l)
{
	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);
	INT nRowCnt = m_pDM->GetRowCount();
	
	m_nSearchType = m_pDlg->m_nSearchType;

	//자료실, 자료상태, 간행빈도
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));


	CString strWorkingStatus;
	CString strDesc;
	CString strRFIDSerial;
	CSeApi api(this);

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkingStatus = _T("");
		strDesc = _T("");
		strRFIDSerial = _T("");

		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),i);
		
		
		if(_T("SEL111O") == strWorkingStatus)
		{
			if(0 == m_nSearchType)
			{
				strDesc = _T("실시간인계자료(예정)"); 
			}
			else if(1 == m_nSearchType)
			{
				strDesc = _T("실시간인계자료");
			}		

		}
		else
		{
			strDesc = api.GetWorkingStatusDesc(strWorkingStatus);
		}
		
		m_pDM->SetCellData(_T("UDF_자료상태"),strDesc,i);

		//RFID여부 
		strRFIDSerial = m_pDM->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("Y"),i);
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

BOOL CTRANSFER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_자료인계") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_자료인계") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_연속_수서_자료인계"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_수서_자료인계"));
	m_pDM_PRINT = FindDM(_T("DM_연속_수서_자료인계_출력"));
	
	if(!m_pDM || !m_pDM_PRINT || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,Grid Error : 연속_수서_자료인계") );
		return false;
	}
	
	//라이트 버젼 여부 
	CSeApi api(this);
	m_bIsLightVersion = api.IsLightVersion(m_pDM);

	//라이트 버젼 setting 
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CTransferSearch(this);
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


VOID CTRANSFER::SetLightVersion()
{
	//라이트 버젼에서는 제본 인계 버튼과 등록 인계 버튼을 안보이게 한다
	CString strRemoveButton;
	strRemoveButton = _T("등록대상처리");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);
	
	strRemoveButton = _T("제본대상처리");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);

	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	RemoveListArray.Add(_T("등록대상처리"));
	RemoveListArray.Add(_T("제본대상처리"));
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	

}

VOID CTRANSFER::TransferToRealTime()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("이미 인계된 자료입니다!"));
		return;
	}

	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("실시간인계"));
}

VOID CTRANSFER::TransferToReg()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("이미 인계된 자료입니다!"));
		return;
	}

	
	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("등록대상처리"));
}

VOID CTRANSFER::TransferToBinding()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("이미 인계된 자료입니다!"));
		return;
	}
	
	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("제본대상처리"));
	
}

VOID CTRANSFER::TransferToShelf()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("이미 인계된 자료입니다!"));
		return;
	}

	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("실시간배가"));
}

VOID CTRANSFER::ShowSpecificData()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	CTransferDetail dlg(this);
	dlg.m_pDM = m_pDM;
	dlg.m_nIdx = m_nCurrentIdx;

	dlg.DoModal();

}

VOID CTRANSFER::PrintTransferedList()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}
	
	
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString class_name = _T("자료실구분");
	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];

	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	INT nCodeCnt = m_pInfo->pCodeMgr->GetCodeCount(class_name);
	
	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속인계자료목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_PRINT, 0);


	CString strSHELF_LOC_CODE;
	INT nPrintIdx = -1;
	INT nRowCount;

	pSIReportManager->DeleteTempPageFiles();

	
	for(INT i=0;i<nCodeCnt;i++)
	{
		m_pDM_PRINT->FreeData();
		
		nIdx = m_pGrid->SelectGetHeadPosition();
		
		while(nIdx>=0)
		{
			strSHELF_LOC_CODE = _T("");
			strSHELF_LOC_CODE = m_pDM->GetCellData(_T("SB_배가위치_코드"),nIdx);
			
			if(strCode[i] == strSHELF_LOC_CODE)
			{
				m_pDM_PRINT->InsertRow(-1);
				nPrintIdx = m_pDM_PRINT->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM,nIdx,m_pDM_PRINT,nPrintIdx);
			}

			nIdx = m_pGrid->SelectGetNext();
		}
		
 		nRowCount = m_pDM_PRINT->GetRowCount();
		
		if(nRowCount > 0)
		{
			pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
	
		}
	}

	ids = pSIReportManager->Print2();
	

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

}

BEGIN_EVENTSINK_MAP(CTRANSFER, CDialog)
    //{{AFX_EVENTSINK_MAP(CTRANSFER)
	ON_EVENT(CTRANSFER, IDC_gridTRANSFER, -600 /* Click */, OnClickgridTRANSFER, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CTRANSFER::OnClickgridTRANSFER() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

INT CTRANSFER::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

// 가등록번호 부여
/// 선정된 자료에 대해 일괄 처리 
/// 1. 가등록번호를 기록할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요

VOID CTRANSFER::RecordTmpRegNo()
{
	
	INT ids;

	CString sSpeciesCnt,sBookCnt;
	ids = GetDMEqualRemoveCnt( this  , m_pDM , _T("SB_종KEY") , m_pGrid , sSpeciesCnt );
	if( ids < 0 ) return;

	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return;
	}

	sBookCnt.Format(_T("%d"),nSelectCnt);


	CTransferRegNo dlg(this);
	dlg.m_strBookCnt = sBookCnt;
	dlg.m_strSpeciesCnt = sSpeciesCnt;
	if(dlg.DoModal() != IDOK) return;


	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CTransferProgress* pProgressBar = NULL;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);


	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = m_pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		sWorkingStatus = m_pDM->GetCellData( _T("SB_자료상태") , nIndex);

		if(_T("SEL111O") != sWorkingStatus)
		{
			m_pGrid->SelectGetNext();
			continue;
		}

		// 배가일 기록 DB작업
		ids = RecordRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("DB작업 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("데이터 작업 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	
		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) {
			AfxMessageBox(_T("Grid 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	
		nSuccessCnt++;
		m_pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료에 등록번호를 부여하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록번호를 부여 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 등록번호를 부여 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );
}


/// 가등록번호 취소
/// 선정된 자료에 대해 일괄 처리 
/// 1. 가등록번호 취소할수 있는 자료인지 검사
/// 2. 책상태 UPDATE 
/// 3. DM 변경
/// 4. 그리드 변경
/// 일괄 작업이기 때문에 프로그래스바 필요
VOID CTRANSFER::CancelTmpRegNo()
{
	
	INT ids;

	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return;
	}

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태

	// 프로그래스바 생성
	CTransferProgress* pProgressBar = NULL;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = m_pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		sWorkingStatus = m_pDM->GetCellData( _T("SB_자료상태") , nIndex);

		if(_T("SEL111O") != sWorkingStatus)
		{
			m_pGrid->SelectGetNext();
			continue;
		}


		// 가등록번호 취소 DB작업
		ids = CancelRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("DB작업 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("데이터 작업 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) {
			AfxMessageBox(_T("Grid 과정중 오류가 있어서 작업을 중단합니다"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		nSuccessCnt++;
		m_pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );


	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료에 등록번호를 삭제하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 등록번호를 삭제 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택 자료 : '%d'\r\n\r\n성공 자료 : '%d'\r\n")
						   _T("실패 자료 : '%d'(자료상태가 등록번호를 삭제 할 수 없는 상태이기 때문에 작업에 실패하였습니다.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );
	

}

VOID CTRANSFER::PrintLabel()
{
	CTransferLabelPrint Dlg(this);
	Dlg.SetParentInfo(m_pDM,m_pGrid);
	Dlg.DoModal();

}

// 프로그래스바 생성
CTransferProgress* CTRANSFER::CreateProgressBarDlg( INT nUpper )
{
	CTransferProgress* pProgressBar = new CTransferProgress(this);
	pProgressBar->Create( this );
	pProgressBar->BringWindowToTop();
	pProgressBar->CenterWindow();
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// 프로그래스바 종료
INT CTRANSFER::DeleteProgressBarDlg( CTransferProgress *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}

/// 가등록번호 부여 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CTRANSFER::RecordRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	
	INT ids;

	CString sRegNo;

	// 1. Column정보 pTmpDM에 복사
	ids = MakeCopyDM( this , m_pDM , pTmpDM );
	if( ids < 0 ) return -2; 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = AddDM( m_pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) return -3; 

	m_pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("SB_책KEY") , 0 , sBookRecKey );
	if( ids < 0 ) return -4; 

	ids = MakeTmpRegNo(sRegNo);
	if( ids < 0 ) return -5; 

	// 프레임 생성
	// 1. 책정보 UPDATE
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , sRegNo );

	m_pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	m_pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("SB_등록번호") , sRegNo , 0 );
	if( ids < 0 ) return -6; 

	// 3. 프레임 DB에 Send~
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -7;
	

	return 0;
}

/// 가등록번호 쉬소 DB 작업
/// 1. 책정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CTRANSFER::CancelRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	// 1. Column정보 pTmpDM에 복사
	ids = MakeCopyDM( this , m_pDM , pTmpDM );
	if( ids < 0 ) return -1; 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = AddDM( m_pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) return -2;

	m_pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("SB_책KEY") , 0 , sBookRecKey );
	if( ids < 0 ) return -3;


	// 프레임 생성
	// 1. 책정보 UPDATE
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T("") );

	m_pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	m_pDM->InitDBFieldData();

	// 1.1. TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("SB_등록번호") , _T("") , 0 );
	if( ids < 0 ) return -4;

	// 3. 프레임 DB에 Send~
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -5;

	return 0;

}


/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CTRANSFER::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	ids = AddDM( pTmpDM , m_pDM , 0 , nDMIndex , this );
	if( ids < 0 ) return -1;


	ids = m_pDM->SetCellData( _T("SB_등록번호"), pTmpDM->GetCellData( _T("SB_등록번호"), 0 ), nDMIndex );
	if( ids < 0 ) return -2;	
	
	return 0;
}

/// 1. INDEX로부터 그리드의 내용 변경
INT CTRANSFER::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	ids = InsertGridRow( this , m_pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) return -1; 

	return 0;
}

/// 가등록번호 생성 & 참조 DB작업
INT CTRANSFER::MakeTmpRegNo( CString &sRegNo )
{
	INT ids;

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	CSeApi api(this);
	sYear = api.GetCurrentYear();


	// 마지막 등록번호 얻어옴
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
				 _T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), sYear );
	// sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s'"),sYear);
	//--2008.10.17 UPDATE BY PWR --}}
	m_pDM_PRINT->GetOneValueQuery( sSql , sTailRegNo );

	// 마지막 등록번호가 존재하지 않을 경우 생성
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = m_pDM_PRINT->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) return -2;

		sTailRegNo = _T("1");
	}

	sRegNo.Format( _T("%s%08s"),sYear,sTailRegNo);

	// 마지막 등록번호 ++
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
					, _ttoi(sTailRegNo)+1, sYear );
//	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
//					_T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
//					, _ttoi(sTailRegNo)+1 , sYear 
//					);
	//--2008.10.17 UPDATE BY PWR --}}

	ids = m_pDM_PRINT->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) return -2; 

	return 0;
}

/// DM을 ADD해주는 함수
INT CTRANSFER::AddDM(	CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM , INT nSrcIndex , INT nDstIndex , CESL_Mgr *pEslMgr )
{
	
	INT ids;

	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	if( pDstDM == NULL ) return -1;

	if( nDstIndex == -1 )
	{
		pDstDM->InsertRow(-1);
		nDstIndex = pDstDM->GetRowCount()-1;
	}

	CString sResult;
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sColAlias[i] , nSrcIndex , sResult );
		if( ids < 0 && (ids != -4012) )
		{
			CString sEMsg;
			sEMsg.Format( _T("sColAlias[i] : %s ") , sColAlias[i] );
			AfxMessageBox( sColAlias[i]);
		}
		if( ids < 0 && (ids != -4012) ) return -2;
		if( ids == -4012 ) sResult.Empty();
		
		ids = pDstDM->SetCellData( sColAlias[i] , sResult , nDstIndex );
		if( ids < 0 ) return -3;
	}

	delete []sColAlias;

	return 0;
}

// 그리드에 DM의 index의 열을 인서트한다.
INT CTRANSFER::InsertGridRow(  CESL_Mgr *pEslMgr , CESL_Grid *pGrid , INT nDMIndex , INT nGridIndex , INT nRowHeight )
{

	INT ids;

	INT nColCount = pGrid->GetColCount();

	if( nGridIndex == -1 )
		nGridIndex = pGrid->GetCount();

	CString sData;
	CESL_DataMgr *pDM;
	for( INT k = 0 ; k < nColCount ; k++ )
	{
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(k);
		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);

		if( pColumnInfo->strColumnDMAlias.IsEmpty() ) continue;
		if( pColumnInfo->strColumnDMFieldAlias.IsEmpty()) continue;

		pDM = pEslMgr->FindDM( pColumnInfo->strColumnDMAlias );
		if( pDM == NULL ) return -1;

		ids = pEslMgr->GetDataMgrData( pColumnInfo->strColumnDMAlias , pColumnInfo->strColumnDMFieldAlias , sData , nDMIndex );
		if( ids < 0 ) return -2;

		pGrid->SetAt( nGridIndex , k , sData );

	}

	if( nRowHeight != -1 ) 
		pGrid->SetRowHeight( nGridIndex+1 , nRowHeight );

	CString sIndex;
	sIndex.Format(_T("%d"),nGridIndex+1);
	pGrid->SetTextMatrix( nGridIndex+1 , 0 , sIndex );

	pGrid->m_nRealRowCount = pDM->GetRowCount();
	
	return 0;

}

INT CTRANSFER::MakeCopyDM( CESL_Mgr *pEslMgr , CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	INT ids;

	pDstDM->SetCONNECTION_INFO(GetDBConnection(pEslMgr->m_pInfo->MODE));
	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;

	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}		
	
	return 0;
}

CString CTRANSFER::GetDBConnection(INT nMode)
{

	CString sPath;
	sPath = _T("");
	//++2008.09.05 DEL BY CJY {{++
//DEL 	if( nMode == 10000 || (nMode==30000) )
//DEL 	{
//DEL 		sPath = _T("..\\cfg\\Mobile\\MobileDB.cfg");
//DEL 	}
	//--2008.09.05 DEL BY CJY --}}
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	return ReadCfg.m_sConnection;

}

/// pGrid의 자료중 체크된 자료중 pDM의 sEqualFieldAlias의 갯수중 sCnt에 저장 
INT CTRANSFER::GetDMEqualRemoveCnt( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sEqualFieldAlias , CESL_Grid* pGrid , CString &sCnt )
{

	INT ids;

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	CStringArray sUniqueValue;

	CString sValue;
	CString sData;
	bool IsEqual;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		IsEqual = FALSE;
		ids = pDM->GetCellData( sEqualFieldAlias , nIndex , sData );
		if( ids < 0 ) return -1;

		for( INT j = 0 ; j < sUniqueValue.GetSize() ; j++ )
		{
			sValue = sUniqueValue.GetAt(j);
			if( sValue.Compare(sData) == 0 )
				IsEqual = TRUE;

		}

		if( IsEqual )
		{
			pGrid->SelectGetNext();
			continue;
		}

		sUniqueValue.Add( sData );

		pGrid->SelectGetNext();
	}

	sCnt.Format( _T("%d") , sUniqueValue.GetSize() );

	return 0;
	
}

HBRUSH CTRANSFER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

 