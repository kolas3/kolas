// SE_CONTENTS_01.cpp : implementation file
//

#include "stdafx.h"

#include "SE_CONTENTS_01.h"
#include "SE_CREATE_ARTICLE1.h"
#include "SE_CONTENTS_EDIT.h"
#include "MakeKisaMarc.h"

#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_01 dialog


CSE_CONTENTS_01::CSE_CONTENTS_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CONTENTS_01::~CSE_CONTENTS_01()
{
	
}   


BOOL CSE_CONTENTS_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
VOID CSE_CONTENTS_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CONTENTS_01)
	DDX_Control(pDX, IDC_TAB_CONTENT_SE, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CONTENTS_01, CDialog)
	//{{AFX_MSG_MAP(CSE_CONTENTS_01)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTENT_SE, OnSelchangeTabContentSe)
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_01 message handlers

BOOL CSE_CONTENTS_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AfxInitRichEdit();
		
    if(InitESL_Mgr(_T("SM_CONTENTMGR_SE_MAIN"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
    
	m_CurrentDM = _T("DM_CONTENTMGR_SE_MAIN");
	m_CurrentCM = _T("CM_CONTENTMGR_SE_MAIN") ;
    m_CurrentGrid = _T("MainGrid") ;

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_CONTENT_SE);

	pTabCtlr->InsertItem (0,_T("권정보"));
    pTabCtlr->InsertItem (1,_T("기사정보"));

	//pTabCtlr->SetCurSel (1) ;
    //pTabCtlr->SetCurSel (0) ;

    m_TabCtrl.SetCurSel (0);
    

	CString strButtonAlias = _T("기사목차편집");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	strButtonAlias = _T("기사보기");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);
	strButtonAlias = _T("목차편집");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);



	m_TocEditdlg.MyCreate(this,CRect(0,10,11,11),_YON_);

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_CurrentCM, m_CurrentGrid);
	pGrid->m_bProtectSelectColumn = TRUE;
	
	CMakeKisaMarc *pDlg = NULL;
	pDlg = new CMakeKisaMarc(this);
	pDlg->pMain = this->pMain;
	pDlg->pParentMgr = (CESL_Mgr*)this;
	pDlg->m_pInfo = this->m_pInfo;
	if ( pDlg->Create(this) )
	{
		pDlg->CenterWindow();
		pDlg->UpdateWindow();
		pDlg->ShowWindow(SW_SHOW);

		pDlg->ExeSearch();
		pDlg->MakeKisaMarc();
		//pDlg->Close();
		pDlg->DestroyWindow();
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CONTENTS_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_CONTENT_SE);
   	if(pTabCtlr->GetSafeHwnd () == NULL) return ;
    pTabCtlr->MoveWindow (0, 0, cx+4, cy);


	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT_SE);
	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 20, cx, cy);
    
	CMSHFlexGrid *pGrid_Kisa = (CMSHFlexGrid*)GetDlgItem(IDC_Grid_CONTENT_Kisa);
	if (pGrid_Kisa->GetSafeHwnd() == NULL) return;
    pGrid_Kisa->MoveWindow(0, 20, cx, cy);


}

INT CSE_CONTENTS_01::OnViewMoc()
{
    View(0) ;
	return 0 ;						
}

VOID CSE_CONTENTS_01::View(INT nType)
{
	// Initial
    BOOL bPrev,bNext;
	CESL_DataMgr* pDataMgr;
	
	pDataMgr = FindDM(_T("DM_CONTENTMGR_SE_TBL")) ;
    
	CString strShow,strRecKey,strYN;
    
    pDataMgr->FreeData () ;

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

	CString sVol_Key ;
    CString sOption ;
    INT ids ;

	if(m_TabCtrl.GetCurSel () == 0)
	{
		ids = GetDataMgrData (m_CurrentDM ,_T("권호키"),sVol_Key,RowIndex) ;
		sOption.Format (_T("VOL_KEY = %s"),sVol_Key) ;
    }
	else
	{
	    ids = GetDataMgrData (m_CurrentDM ,_T("VOL_KEY"),sVol_Key,RowIndex) ;
		sOption.Format (_T("VOL_KEY = %s"),sVol_Key) ;

	}
    
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
		/*
		pDataMgr->GetCellData (_T("초록"),0,strShow);
		m_ChorokDlg.SetChorok(strShow);
		m_ChorokDlg.ShowViewer(bPrev,bNext);
		*/
	}
	return;
    AfxMessageBox(_T("보기에 실패하였습니다."));
	
}

INT CSE_CONTENTS_01::OnViewKisa()
{
	CSE_CREATE_ARTICLE dlg(1,this) ;
	//dlg.GetMokcha (sMokcha) ;
	//dlg.GetKeyValue (m_Contents_key,m_sVol_Key) ;
	dlg.DoModal () ;    
	return 0 ;
}


VOID CSE_CONTENTS_01::OnSelchangeTabContentSe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	  CESL_ControlMgr *pCM = FindCM(m_CurrentCM );
	 if(pCM == NULL) {
		 AfxMessageBox(_T("FindCM() Error~!"));
		 return  ;
	 }

	 CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));
	 if(pGrid == NULL) 
	 {
		 AfxMessageBox(_T("FindControl (MainGrid)"));
		 return ;
	 }
     CESL_Grid *pGrid_Kisa = (CESL_Grid*)pCM->FindControl (_T("MainGrid_Kisa"));
	 if(pGrid_Kisa == NULL) 
	 {
		 AfxMessageBox(_T("FindControl (MainGrid_Kisa)"));
		 return ;
	 }
     
	 CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_CONTENT_SE);
	 INT nTabCur ;
     nTabCur = pTabCtlr->GetCurSel () ;

	 if(nTabCur == 0 )
	 {
		 pGrid->ShowWindow (SW_SHOW) ;
         pGrid_Kisa->ShowWindow (SW_HIDE);
		// pCM->ControlDisplay(_T("MainGrid"));
		 m_CurrentGrid = _T("MainGrid") ;
		 m_CurrentDM = _T("DM_CONTENTMGR_SE_MAIN");

		 CString strButtonAlias = _T("기사목차편집");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		 strButtonAlias = _T("기사보기");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);
		 strButtonAlias = _T("목차편집");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);
     }
	 else
	 {
	     pGrid->ShowWindow (SW_HIDE) ;
         pGrid_Kisa->ShowWindow (SW_SHOW);
      //   pCM->ControlDisplay(_T("MainGrid_Kisa"));
		 m_CurrentGrid = _T("MainGrid_Kisa") ;
		 m_CurrentDM = _T("DM_CONTENTMGR_SE_INDEX_TBL");
	     CString strButtonAlias = _T("기사목차편집");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);

		 strButtonAlias = _T("기사보기");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		 strButtonAlias = _T("목차편집");
         pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	 }

	*pResult = 0;
}

bool CSE_CONTENTS_01::Print()
{
		
	return TRUE;
}
INT CSE_CONTENTS_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}

VOID CSE_CONTENTS_01::Print_List() 
{
	INT ids;
	CString ReportAlias ;
	
	
	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_CONTENT_SE);
	INT nTabCur ;
	nTabCur = pTabCtlr->GetCurSel () ;
	
	if(nTabCur == 0) 
	{
		ReportAlias = _T("목차초록_연속");
		m_Table_01 = _T("테이블_3");
		m_Table_02 = _T("테이블_16") ;
	}
	else
	{
		ReportAlias = _T("목차초록_연속_기사목록");
		m_Table_01 = _T("테이블_14");
		m_Table_02 = _T("테이블_19") ;
	}
	
	
	
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
	CString strChkFile = _T("..\\CFG\\RptStyle\\목차초록_연속.xls");
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

VOID CSE_CONTENTS_01::Print_SetLastPageLine(VOID)
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(m_Table_01);
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(m_Table_02);
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

BEGIN_EVENTSINK_MAP(CSE_CONTENTS_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CONTENTS_01)
	ON_EVENT(CSE_CONTENTS_01, IDC_Grid_CONTENT_SE, -601 /* DblClick */, OnDblClickGridCONTENTSE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CONTENTS_01::OnDblClickGridCONTENTSE() 
{
	CESL_ControlMgr * pCM = FindCM(_T("CM_CONTENTMGR_SE_MAIN"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));
	if(pGrid == NULL) 
	{
		 AfxMessageBox(_T("FindControl (MainGrid)"));
		return ;
	}
	
    CESL_DataMgr *pDM = FindDM(m_CurrentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	if(pDM->GetRowCount () == 0 )
	{
		AfxMessageBox(_T("선택 항목이 없습니다."));
		return ;
	}

	INT row = pGrid->GetMouseRow();
	if( row > 1)
	{
		CSE_CONTENTS_EDIT  dlg(this );
		dlg.SetDbClickMode (true);
		dlg.DoModal () ;
	}
}

VOID CSE_CONTENTS_01::KisaMarcImport()
{
	// 기사마크 반입
	// 열린 StreamMarc를 가지고 기사 생성
}

VOID CSE_CONTENTS_01::KisaMarcExport()
{
	//기사마크 반출, 
	//선정된 권호키를 가지고 그 권호키에 물려있는 기사들의 마크를 모두 가져와서 반출..

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_CurrentCM, m_CurrentGrid);
	
	pGrid->SelectMakeList();
	if (pGrid->SelectGetCount() > 0 && AfxMessageBox(_T("선정된 자료가 있습니다. \r\n선정된 자료를 반출하시겠습니까? "), IDOK, IDCANCEL ) == IDOK ) 
	{	
		//선정된 데이터가 있을경우 선정된 데이터로 반출 가능하도록 수정
		MakeFileSelected( _T("CM_CONTENTMGR_SE_MAIN"), m_CurrentGrid );
	}
	else 
	{
		AfxMessageBox( _T("선정된 권호가 없습니다.") );
		return;
	}

}

INT CSE_CONTENTS_01::MakeFileSelected(CString sCMAlias, CString sGridAlias)
{
	EFS_BEGIN

	INT idx;
	SelectMakeList(sCMAlias, sGridAlias);
	idx = SelectGetHeadPosition(sCMAlias, sGridAlias);

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return -1;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();	

	CESL_DataMgr * pDmRegMain;
	pDmRegMain = FindDM( _T("DM_단행_정리_검색결과") );
	if ( pDmRegMain == NULL ) {
		AfxMessageBox( _T("DM_단행_정리_등록_메인 확인") );
		return -1;
	}

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return -1;
	}

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일을 선택하세요."));
		return -1;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}

	CString sBookKeyList;
	CStringArray aBookKeyList;
	while ( idx >= 0 ) {
		CString sBookKey = pDmRegMain->GetCellData( _T("IBS_종KEY") , idx );
		sBookKeyList = sBookKeyList + _T(", ") + sBookKey;

		if ( ((idx+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}

		idx = SelectGetNext(sCMAlias, sGridAlias);
	}
	aBookKeyList.Add( sBookKeyList );

	INT iRowCount;
	INT iTotCount = 0;
	for ( INT j = 0 ; j < aBookKeyList.GetSize() ; j++ ) {
		sBookKeyList = aBookKeyList.GetAt(j).Mid(2);

		CString strWhere;
		strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

		iRowCount = pDM->GetRowCount();
		iTotCount = iTotCount + iRowCount;

		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );
			CMarc marc;
			m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
			m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

			sMarc += _T("\r\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), iTotCount );
	AfxMessageBox( sMessage );

	return 0;
	EFS_END
	return -1;
}

VOID CSE_CONTENTS_01::MakeKisaMarc()
{
	//기사마크 일괄적용..
	CMakeKisaMarc dlg(this);
	dlg.DoModal () ;
}


HBRUSH CSE_CONTENTS_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
