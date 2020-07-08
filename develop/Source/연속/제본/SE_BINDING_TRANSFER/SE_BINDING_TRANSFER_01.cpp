// SE_BINDING_TRANSFER_01.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_TRANSFER_01.h"
#include "SE_BINDING_TRANSFER_02.h"

#include "../../../ESL/KOLAS3_MAIN/KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TRANSFER_01 dialog

CSE_BINDING_TRANSFER_01::CSE_BINDING_TRANSFER_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDlg = NULL;
}

CSE_BINDING_TRANSFER_01::~CSE_BINDING_TRANSFER_01()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CSE_BINDING_TRANSFER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_TRANSFER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_TRANSFER_01, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_TRANSFER_01)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TRANSFER_01 message handlers

BOOL CSE_BINDING_TRANSFER_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_BINDING_TRANSFER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_TRANSFER_01")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if(!pDM) return FALSE;
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSE_BINDING_TRANSFER_02(this);
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

VOID CSE_BINDING_TRANSFER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_BASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);
	
}

VOID CSE_BINDING_TRANSFER_01::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CSE_BINDING_TRANSFER_01::OnSendQuery( WPARAM w, LPARAM l )
{
	INT ids;

	m_Query	= m_pDlg->m_strQuery;

	if( !m_Query.Compare(_T("NO_DATA") ) ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("검색결과가 없습니다."));
		return ;
	}

	ids = BaseDisplay();
	if( ids <  0 ) AfxMessageBox( _T("Display Error!") );
	if( ids == 0 ) {

		ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
		if( ids ) return ;

		AfxMessageBox(_T("검색결과가 없습니다."));
		return ;
	}

	if( ids > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

// optionchange & sorting
INT CSE_BINDING_TRANSFER_01::BaseDisplay() 
{
	INT ids, nRowCnt, i;
	
	CString sSpeciesKey, sBindVolCnt;
	CString sQuery, sData, sKey1, sKey2;
	CString sSeqNo, sYear;


	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if( pDataMgr == NULL ) return -1;

	ids = pDataMgr->RefreshDataManager( m_Query );
	if( ids < 0 ) return -1;

	nRowCnt = pDataMgr->GetRowCount();
	
	CString strRFIDSerial;

	for( i = 0 ; i < nRowCnt ; i++ ) {
		
		// 발주차수
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("BINDING_YEAR"), sYear, i );
		if( ids < 0 ) return -1;
		
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("BINDING_SEQ_NO"), sSeqNo, i );
		if( ids < 0 ) return -1;

		sData = sYear + _T("-") + sSeqNo;
		ids = SetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("ORDER_SEQ_NO"), sData, i );
		if( ids < 0 ) return -1;


		//RFID유무
		strRFIDSerial = _T("");
		strRFIDSerial = pDataMgr->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			pDataMgr->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			pDataMgr->SetCellData(_T("UDF_RFID"),_T("Y"),i);
		}

	}
	

	ids = AllControlDisplay( _T("CM_SE_BINDING_TRANSFER_01"), -1);
	if( ids ) return -1;

	CString str;
	str.Format( _T("%d 건의 자료가 검색되었습니다.") ,nRowCnt );
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	return nRowCnt;
}

// SER111N(등록인계자료)
/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 제본등록>>등록인계자료에서 등록으로 인계하는 작업을 수행한다.
//	 작없상태값을 SEB114N -> SER111N 으로 변경된다.
//
// [ 미니스펙 ]
// 1. 사용할 CM 및 Grid를 설정한다.
// 2. Grid에 선정체크된 Row들을 List에 저장한다.
// 3. Grid에 체크된 Row의 갯수를 얻어온다.
// 4. 사용할 DM을 설정한다.
// 5. 선정한 Grid 자료들(Row)중 맨 처음 값의 인덱스를 가져온다.
// 6. Query문 발송 준비한다.
// 7. 선정된 자료들의 REC_KEY를 가져와 자료상태를 변경하는 쿼리문을 작성한다.
//		- SE_BOOK_TBL : SEB114N -> SER111N으로 변경
// 8. 작성한 쿼리문을 적용한다.
// 9. 세션을 닫는다.
// 10. 등록인계처리된 자료들을 Grid에서 삭제한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSE_BINDING_TRANSFER_01::ProcTrans() 
{
	//=======================================================================
	// 변수 정의
	//=======================================================================
	INT ids, nRowCnt, nIdx, i=0;
	CString sRecKey, sSql, sMsg;

	//=======================================================================
	// 1. 사용할 CM 및 Grid를 설정한다.
	//=======================================================================
	CESL_ControlMgr *pCM   = FindCM( _T("CM_SE_BINDING_TRANSFER_01") );	
	if( pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_SE_BINDING_TRANSFER_01]을 설정할 수 없습니다."));
		return -1;
	}

	CESL_Grid		*pGrid = (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;
	if( pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM_SE_BINDING_TRANSFER_01::MainGrid]을 설정할 수 없습니다."));
		return -1;
	}


	//=======================================================================
	// 2. Grid에 선정체크된 Row들을 List에 저장한다.
	//=======================================================================
	ids = pGrid->SelectMakeList() ;
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("선정 처리에 실패하였습니다.") );
		return -1;
	}

	//=======================================================================
	// 3. Grid에 체크된 Row의 갯수를 얻어온다.
	//=======================================================================
	nRowCnt = pGrid->SelectGetCount();
	if( nRowCnt == 0 ) 
	{
		ESLAfxMessageBox( _T("자료를 선택하십시오.") );
		return -2;
	}
	if( nRowCnt <  0 ) 
	{
		ESLAfxMessageBox( _T("그리드의 선택정보를 가져오는데 실패하였습니다.") );
		return -3;
	}


	//=======================================================================
	// 4. 사용할 DM을 설정한다.
	//=======================================================================
	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_TRANSFER_01"));
	if( pDataMgr == NULL ) 
	{
		ESLAfxMessageBox( _T("[DM_SE_BINDING_TRANSFER_01]을 설정할 수 없습니다.") );
		return -4;
	}


	//=======================================================================
	// 5. 선정한 Grid 자료들(Row)중 맨 처음 값의 인덱스를 가져온다.
	//=======================================================================
	nIdx = pGrid->SelectGetHeadPosition();
	if( nIdx == -1 ) 
	{
		ESLAfxMessageBox( _T("첫 선정값을 읽지 못했습니다.") );
		return -5;
		}

	//=======================================================================
	// 6. Query문 발송 준비한다.
	//=======================================================================
	ids = pDataMgr->StartFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("인계처리에 실패하였습니다!") );
		return -9;
		}
		


	//=======================================================================
	// 7. 선정된 자료들의 REC_KEY를 가져와 자료상태를 변경하는 쿼리문을 작성한다.
	//		- SE_BOOK_TBL : SEB114N -> SER111N으로 변경
	//=======================================================================
	for( i=0; i<nRowCnt; i++)
	{
		// Grid 선정된 자료의 인덱스를 정상적으로 읽어왔는지 비교한다.
		if( nIdx < 0 )
		{
			ESLAfxMessageBox( _T("인계처리에 실패하였습니다.") );
			pDataMgr->EndFrame();
			return -9;
		}
		// WORKING_STATUS, BINDING_STATUS
		ids = GetDataMgrData( _T("DM_SE_BINDING_TRANSFER_01"), _T("REC_KEY"), sRecKey, nIdx );
		if( ids < 0 ) return -6;

		// 제본책
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE REC_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);


		// 대상책
		sSql.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', REG_TRANSFER_DATE = '%s' WHERE BINDING_BOOK_KEY = %s;"), GetTodayDate(), sRecKey );
		pDataMgr->AddFrame(sSql);

		nIdx = pGrid->SelectGetNext();		
	}


	//=======================================================================
	// 8. 작성한 쿼리문을 적용한다.
	//=======================================================================
	ids = pDataMgr->SendFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("인계처리에 실패하였습니다!") );
		return -9;
	}


	//=======================================================================
	// 9. 세션을 닫는다.
	//=======================================================================
	pDataMgr->EndFrame();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("인계처리에 실패하였습니다!") );
		return -9;
	}


	//=======================================================================
	// 9. 등록인계처리에 대한 메시지를 보여준다.
	//=======================================================================
	sMsg.Format( _T("%d개의 자료를 인계처리 하였습니다."), nRowCnt );
	AfxMessageBox(sMsg);


	//=======================================================================
	// 10. 등록인계처리된 자료들을 Grid에서 삭제한다.
	//=======================================================================
	ids = BaseDisplay();
	if( ids < 0 ) 
	{
		ESLAfxMessageBox( _T("화면 갱신에 실패하였습니다.") );
		return -11;
	}

	return 0;
}


INT CSE_BINDING_TRANSFER_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_BINDING_TRANSFER_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_25"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_37"));
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

VOID CSE_BINDING_TRANSFER_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("K2UP_제본인계목록");

  CESL_DataMgr *pDM = FindDM( _T("DM_SE_BINDING_TRANSFER_01") );
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


	//PRINT//////////////////////////////////////////////////////////////////////////
   /* 
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
   */

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

CString CSE_BINDING_TRANSFER_01::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
HBRUSH CSE_BINDING_TRANSFER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
