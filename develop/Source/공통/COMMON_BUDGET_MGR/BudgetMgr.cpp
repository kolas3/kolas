// BudgetMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetMgr.h"
#include "BudgetListDlg.h"
#include "BudgetApi.h"
#include "../../ESL\SIReportMgr\SIReportManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr dialog


CBudgetMgr::CBudgetMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM =			NULL;
	m_pGrid =		NULL;
	m_pCM =			NULL;
	m_pDlg =		NULL;
	m_nCurrentIdx	= -1;
	m_strSIAlias	= _T("");


}

CBudgetMgr::~CBudgetMgr()
{	
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
	
}

VOID CBudgetMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BOOL CBudgetMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetMgr, CDialog)
	//{{AFX_MSG_MAP(CBudgetMgr)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr message handlers

BOOL CBudgetMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("예산관리") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_예산관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_예산관리"));
	
	if(!m_pDM || !m_pGrid || !m_pCM) {
		AfxMessageBox( _T("DM,Grid Error : DM_예산관리,그리드") );
		return FALSE;
	}
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CBudgetSearch(this);
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

VOID CBudgetMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBUDGET_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	
}


VOID CBudgetMgr::InsertProcedure()
{
	CBudgetListDlg dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.DoModal();

	m_pCM->AllControlDisplay();
	
}

VOID CBudgetMgr::ModifyProcedure()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;		
	}
	else
	{

		CBudgetListDlg dlg(this);
		dlg.m_nOpenMode = -1;


		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;

		dlg.DoModal();
	}
	m_pCM->AllControlDisplay();
}


VOID CBudgetMgr::DeleteProcedure()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선정된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 예산을 삭제 하시겠습니까?"),nCount);


	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CBudgetApi api(this);
	CString strBUDGET_COMPILATION_KEY;


	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strBUDGET_COMPILATION_KEY = m_pDM->GetCellData(_T("REC_KEY"),idx);
		

		ids = api.DeleteBudgetList(m_pDM,
								   strBUDGET_COMPILATION_KEY);
							 
	
		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("예산편성명"),idx);

			msg.Format(_T("[ %s ] 예산을 삭제하던 중 문제가 발생해 작업을 중단합니다!"),strTITLE);
			AfxMessageBox(msg);
			break;
		}
			
		arrayDeletedRow.Add(idx);

		idx = m_pGrid->SelectGetNext();
	}
	
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] 건의 예산을 삭제하였습니다!"),nSize);
	AfxMessageBox(msg);
	return; 	

}

INT CBudgetMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CBudgetApi api(this);
	CString strTmpData1, strTmpData2;
	
	if(strDMFieldAlias == _T("UDF_예산편성액"))
	{	
		
		strTmpData1 = m_pDM->GetCellData(_T("예산편성액"),nRow);
		if(strTmpData1.IsEmpty() == FALSE)
			m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("UDF_예산편성액"), strTmpData2, nRow);
	}

	return 0;
}

VOID CBudgetMgr::SearchProcedure()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CBudgetMgr::OnSendQuery(WPARAM w,LPARAM l)
{

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성구분"),_T("UDF_예산편성구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성상태"),_T("UDF_예산편성상태"));


	m_pGrid->Display();
	m_nCurrentIdx = -1;
	
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

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
	m_pCM->AllControlDisplay();


}

BEGIN_EVENTSINK_MAP(CBudgetMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetMgr)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -600 /* Click */, OnClickgridBUDGETMGR, VTS_NONE)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -601 /* DblClick */, OnDblClickgridBUDGETMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetMgr::OnClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

VOID CBudgetMgr::OnDblClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifyProcedure();
}

VOID CBudgetMgr::PrintProcedure()
{
EFS_BEGIN

	// 기초 DM - DM_예산집행계획
	// 출력 DM - DM_예산집행계획출력
	// DM 초기화
	//
	INT nRow, nCol, i,j;
	long double dSum=0;
	CString szQuery, szTemp;
	SYSTEMTIME time;
	GetSystemTime(&time);
	CESL_DataMgr *pSrcDM, *pTgtDM;
	pSrcDM = pTgtDM = NULL;

	pSrcDM = FindDM(_T("DM_예산집행계획"));
	pTgtDM = FindDM(_T("DM_예산집행계획출력"));

	if(pSrcDM==NULL || pTgtDM==NULL)
	{
		ESLAfxMessageBox(_T("DM Load Failed"));
		return;
	}

	szQuery.Format(_T("CS.BUDGET_COMPILATION_KEY = CC.REC_KEY")
					_T(" AND CC.BUDGET_YEAR = '%d'")
					_T(" GROUP BY BUDGET_CLASS, CODE")
					_T(" ORDER BY BUDGET_CLASS"), time.wYear);

	pSrcDM->RefreshDataManager(szQuery);
	pTgtDM->FreeData();
	
	// SIReport 형식에 맞는 빈 틀을 생성한다.
	// 학부코드 8개, 소계 1개 - 총 9개의 필드가 필요
	// 
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		pTgtDM->InsertRow(-1);
	}

	// 기초 DM의 값을 출력 DM에 인덱스를 맞춰 삽입한다.
	//
	for(i=0;i<pSrcDM->GetRowCount(); i++)
	{
		nRow = Match_Row_Idx(pSrcDM->GetCellData(i,0));
		nCol = Match_Col_Idx(pSrcDM->GetCellData(i,1));
		pTgtDM->SetCellData(nRow, nCol, pSrcDM->GetCellData(i,2));
	}

	// 합계를 구한다.
	//
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		dSum = 0;
		for(j=1; j<BUDGETPLAN_COLCNT; j++)
		{
			dSum+=_ttoi64(pTgtDM->GetCellData(i,j));			
		}
		szTemp.Format(_T("%.0f"), dSum);
		pTgtDM->SetCellData(i,0, szTemp);
	}
	for(i=0; i<BUDGETPLAN_COLCNT; i++)
	{
		dSum=0;
		for(j=0; j<BUDGETPLAN_ROWCNT-1; j++)
		{
			dSum+=_ttoi64(pTgtDM->GetCellData(j,i));
		}
		szTemp.Format(_T("%.0f"), dSum);
		pTgtDM->SetCellData(j, i, szTemp);
	}

	// 자릿수를 찍어준다.
	//
	for(i=0; i<BUDGETPLAN_ROWCNT; i++)
	{
		for(j=0;j<BUDGETPLAN_COLCNT; j++)
		{
			pTgtDM->SetCellData(i,j, AddComma(pTgtDM->GetCellData(i,j)));
		}
	}

	// 출력 DM을 SIReport로 출력한다.
	//
	m_strSIAlias = _T("K2UP_예산집행계획");
	if(!PrintToSIReport())
	{
		ESLAfxMessageBox(_T("출력을 완료 하지 못하였습니다."));
	}

EFS_END
}

INT CBudgetMgr::Match_Row_Idx(CString szIdx)
{
	if(szIdx == _T("01"))			// 인문과학처
	{
		return 0;
	}
	else if(szIdx == _T("02"))		// 사회과학처
	{
		return 1;
	}
	else if(szIdx == _T("03"))		// 이학처
	{
		return 2;
	}
	else if(szIdx == _T("04"))		// 공학처
	{
		return 3;
	}
	else if(szIdx == _T("05"))		// 체육학과
	{
		return 4;
	}
	else if(szIdx == _T("06"))		// 군사학처
	{
		return 5;
	}
	else if(szIdx == _T("07"))		// 도서관
	{
		return 6;
	}
	else if(szIdx == _T("09"))		// 군사도서
	{
		return 7;
	}


	return -1;
}

INT CBudgetMgr::Match_Col_Idx(CString szIdx)
{
	INT nIdx = _ttoi(szIdx);
	if( 0 < nIdx && 5 >  nIdx )
	{
		return nIdx;
	}
	return -1;
}

BOOL CBudgetMgr::PrintToSIReport()
{
	INT ids;
	CESL_DataMgr *pDM_Dst;

	//통계 수행 - SiReport 로 출력
	//SI리포트 CLASS 생성
	//
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	
	if (!pSIReportManager) 
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return FALSE;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();

	//DB에서 SI리포트 파일 생성
	//
	ids = pSIReportManager->GetSIReportFileFromDB(m_strSIAlias);
	if (ids < 0) 
	{
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return FALSE;
	}

	// 변수에 값 저장
	// DM 연결
	//
	if(m_strSIAlias = _T("K2UP_예산집행계획"))
	{
		pDM_Dst = FindDM ( _T("DM_예산집행계획출력") );
	}
	
	if ( pDM_Dst == NULL ) 		return FALSE;
	
	pSIReportManager->SetDataMgr(0, pDM_Dst );
	pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
	
	//Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	
	return TRUE;
}

INT CBudgetMgr::SIReportEventHandler(CSIReportEventInfo* EventInfo)
{
	CSIReportManager *pSIReportManager = ( CSIReportManager * )EventInfo->m_pSIReportManager;
	if( !pSIReportManager )
		return -1;
	else
		return 0;
}

VOID CBudgetMgr::Print_SetLastPageLine()
{
EFS_BEGIN

	INT ids;

	// XML 과 WorkForm객체를 생성한다.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) 
	{
		AfxMessageBox(_T("XML 처리기 생성에 실패하였습니다."));
		return;
	}
	if (pPageWorkForm == NULL) 
	{
		AfxMessageBox(_T("중간 XML 처리기 생성에 실패하였습니다."));
		return;
	}

	// 현재 마지막 페이지 번호를 가져온다.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) 
	{
		AfxMessageBox(_T("페이지 정보를 찾을 수 없습니다."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// 현재 마지막 페이지 파일을 불러온다.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) 
	{
		AfxMessageBox(_T("페이지를 로드 하지 못하였습니다."));
		return;
	}
	// 불러온 WorkForm중에서 First Sheet를 가져온다.(중간 페이지 파일에는 Sheet가 한개뿐이다.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) 
	{
		AfxMessageBox(_T("페이지를 찾을 수 없습니다."));
		return;
	}
	// 마지막 수평줄 처리가 이미 되어 있는지 본다. 이미 처리되어 있다면 두번 할 필요 없다.
	CSIReportObject * pObject = pSheet->FindObject(_T("추가된마지막수평줄"));
	if (pObject) return;
	// 외부테이블 과 반복 테이블을 가져온다.(테이블 이름은 상황에 따라 변경할 것)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_27"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_271"));
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
	if (InLeft.bottom < OutLeft.bottom) 
	{
		// 마지막 수평줄의 Rect를 계산한다.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// 마지막 수평줄을 중간 페이지 파일에 추가한다.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) 
		{
			pLine->SetName(_T("추가된마지막수평줄"));
			pLine->SetRect(NewRect);
		}

		// 변경된 중간페이지 파일을 저장한다.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// 생성된 Object를 해제
	if (pXMLIOMgr) 
	{
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) 
	{
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;

EFS_END

}

CString CBudgetMgr::AddComma(CString szVal)
{	
	INT nLen = szVal.GetLength();
	if(nLen <= 3) return szVal;
	INT nCnt = 0;
	CString szResult;
	CString szTemp = ReverseString(szVal);

	for(INT i=0; i<nLen; i++)
	{		
		if( nCnt ==3 )
		{
			nCnt = 0;
			szResult+=',';
		}
		szResult += szTemp.GetAt(i);
		nCnt++;
	}

	return (ReverseString(szResult));
}

CString CBudgetMgr::ReverseString(CString str)
{
	CString szResult = str;
	INT nLen = str.GetLength();
	for(INT i=0; i<nLen; i++)
	{
		szResult.SetAt(i, str.GetAt(nLen-i-1));
	}

	return szResult;
}

HBRUSH CBudgetMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
