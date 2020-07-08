// CONTENTMGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_01.h"
#include "CONTENTMGR_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_01 dialog


CCONTENTMGR_01::CCONTENTMGR_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CCONTENTMGR_01::~CCONTENTMGR_01()
{

}

BOOL CCONTENTMGR_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_01, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_01)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_01 message handlers




VOID CCONTENTMGR_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT);
	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 0, cx, cy);
}

BOOL CCONTENTMGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_MAIN")) <0 )
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

	AfxInitRichEdit();


    m_TocEditdlg.MyCreate(this,CRect(0,10,11,11),_DAN_SINGLE);
  	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,CRect(0,10,11,11));
	

    m_CurrentCM   = _T("CM_CONTENTMGR_MAIN") ;
	m_CurrentDM   = _T("DM_BO_CONTENTMGR_SPECIES") ;
	m_CurrentGrid = _T("MainGrid_CONTENT");    

	AllControlDisplay (m_CurrentCM ,-1) ;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CCONTENTMGR_01::OnInsert_TOC_ABS ()
{
	CCONTENTMGR_02 dlg(0,this);
	dlg.DoModal() ;
    
	return 0 ;
}

INT CCONTENTMGR_01::OnEdit_TOC_ABS ()
{
	CCONTENTMGR_02 dlg(1,this);
	dlg.DoModal() ;
    
	return 0 ;
}


VOID CCONTENTMGR_01::View(INT nType)
{
	// Initial
    BOOL bPrev,bNext;
	CESL_DataMgr* pDataMgr;
	
	pDataMgr = FindDM(_T("DM_목차초록")) ;

	CString strShow,strRecKey,strYN;

	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid) ;
	if(pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return ;
	}
    INT RowIndex ; 
     
	pGrid->SelectMakeList ();
	

	if(pGrid->SelectGetCount () == 0)
	{
		AfxMessageBox(_T("항목을 선택해 주십시요~!")) ;
		return ;
	}


	RowIndex = pGrid->GetIdx () ;

	CString Species_Key ;
    CString sOption ;
    INT ids ;
	ids = GetDataMgrData (m_CurrentDM ,_T("IBS_종KEY"),Species_Key,RowIndex) ;
    sOption.Format (_T("toc.SPECIES_KEY = %s"),Species_Key) ;

    ids = pDataMgr->RefreshDataManager (sOption); 
    
	bPrev = FALSE ;
	bNext = FALSE ;

	if(nType==0){

        pDataMgr->GetCellData (_T("목차"),0,strShow);

		//Test 후 삭제 할 것 ////////////////////////////
		//strShow = _T("<id>KMO199700001\r\n<call>01$a021.7$b국26e$c4\r\n<count>\r\n<date>1997\r\n<pub>국립중앙도서관\r\n<sertitle>\r\n<title>도서관정보화\r\n<body>목차\r\n표제지 = 0\r\n발간사 = i\r\n목차 = ii\r\n제1장 서론 = 1\r\n 제1절 현황 = 2\r\n 제2절 문제점 = 5\r\n제2장 본론 = 20\r\n 제1절 해결방안 = 20\r\n 제2절 향후계획 = 30\r\n제3장 결론 = 55\r\n참고문헌 = 155\r\n</body>\r\n<table>표목차\r\n<표1>한국사서교사 배치현황 = 15\r\n<표2>일본의 학교도서관 사서교사 배치기준 = 22\r\n</table>\r\n<figure>그림목차\r\n[그림1]사서교사의 직능별 학력 = 8\r\n[모형1]학교 도서관 = 12\r\n</figure>\r\n<body>Contents\r\ntitle page = 0\r\nReward = i\r\ncontents = ii\r\nchapter 1 preface = 1\r\n section 1 introduction = 2\r\n section 2 problem = 5\r\n</body>") ;
		/////////////////////////////////////////////////
		
		m_TocEditdlg.SetTocFromString(strShow);
		m_TocEditdlg.ShowViewer(bPrev,bNext);

	}else{
		pDataMgr->GetCellData (_T("초록"),0,strShow);
		m_ChorokDlg.SetChorok(strShow);
		m_ChorokDlg.ShowViewer(bPrev,bNext);
	}
	return;
    AfxMessageBox(_T("보기에 실패하였습니다."));
	
}


INT CCONTENTMGR_01::OnViewMoc()
{
    View(0) ;
	return 0 ;						
}

INT CCONTENTMGR_01::OnViewCho()
{
    View(1);
	return 0 ;
}

INT CCONTENTMGR_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CCONTENTMGR_01::Print_List() 
{
  INT ids;
  CString ReportAlias ;
  ReportAlias = _T("목차초록_단행");

  CESL_DataMgr *pDM = FindDM(m_CurrentDM );
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
	//=====================================================
	//2009.05.21 UPDATE BY PJW : 엑셀파일이 없을경우 만 수행한다.
// 	Print_SetLastPageLine();
	CString strChkFile = _T("..\\CFG\\RptStyle\\목차초록_단행.xls");
	CFileFind fFind;
	if( !fFind.FindFile(strChkFile) )
	{		
		Print_SetLastPageLine();
	}
	//=====================================================
    
	ids = pSIReportManager->Print2();
    if (pSIReportManager) {
	delete pSIReportManager;
	pSIReportManager = NULL;
    }

	return ;
}

VOID CCONTENTMGR_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_48"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_29"));
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

BEGIN_EVENTSINK_MAP(CCONTENTMGR_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCONTENTMGR_01)
	ON_EVENT(CCONTENTMGR_01, IDC_Grid_CONTENT, -601 /* DblClick */, OnDblClickGridCONTENT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCONTENTMGR_01::OnDblClickGridCONTENT() 
{
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT);
	if( pGrid->GetMouseRow() > 0 )
	{
		CESL_DataMgr *pDM = FindDM(m_CurrentDM );
		if(pDM == NULL)
		{
			AfxMessageBox(_T("FindDM() Error~!"));
			return ;
		}

		if(pDM->GetRowCount () ==0)
		{
			AfxMessageBox(_T("선택 항목이 없습니다."));
			return ;
		}
		
		CCONTENTMGR_02 dlg(0,this);
		dlg.SetDbClickMode (true);
		dlg.DoModal() ;
	}
}


HBRUSH CCONTENTMGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


