// LOC_STATISTICS.cpp : implementation file
//

#include "stdafx.h"
#include "LOC_STATISTICS.h"
// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
#include "LocStatisticsList.h"
/*//END------------------------------------------------------------------------*/
// KOL.RED.2018.211 ---------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLOC_STATISTICS dialog


CLOC_STATISTICS::CLOC_STATISTICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLOC_STATISTICS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTYPE = 1; //default

	m_pCM = NULL;
	m_pDM_QUERY = NULL;
	m_pDM_RESULT = NULL;
	m_pDM_SUM = NULL;
	m_pDM_PRINT = NULL;
	m_pDM_KDC = NULL;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = NULL;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pGrid = NULL;


	m_pApi = NULL;
	m_pAxisWnd = NULL;
	m_pTmpDialog = NULL;
	
	m_pCondWnd = NULL;
	m_pCondWnd1 = NULL;
	m_pCondWnd2 = NULL;
	m_pCondWnd3 = NULL;



	m_strClassName = _T("");
	m_strPrintClassName = _T("");
}

CLOC_STATISTICS::~CLOC_STATISTICS()
{
	if(m_pApi)
	{
		delete m_pApi;
		m_pApi = NULL;
	}

	if(m_pAxisWnd)
	{
		delete m_pAxisWnd;
		m_pAxisWnd = NULL;
	}


	if(m_pTmpDialog)
	{
		delete m_pTmpDialog;
		m_pTmpDialog = NULL;
	}
	
	if(m_pCondWnd1)
	{
		delete m_pCondWnd1;
		m_pCondWnd1 = NULL;
	}
	
	if(m_pCondWnd2)
	{
		delete m_pCondWnd2;
		m_pCondWnd2 = NULL;
	}
	
	if(m_pCondWnd3)
	{
		delete m_pCondWnd3;
		m_pCondWnd3 = NULL;
	}
}

VOID CLOC_STATISTICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOC_STATISTICS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLOC_STATISTICS, CDialog)
	//{{AFX_MSG_MAP(CLOC_STATISTICS)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_chkDUP_DEL, OnchkDUPDEL)
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUN_S, OnRunS)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOC_STATISTICS message handlers

BOOL CLOC_STATISTICS::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CLOC_STATISTICS::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_staTITLE);
	CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_staTITLE2);


	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	if(pStatic1->GetSafeHwnd() == NULL){
		return;
	}
	if(pStatic2->GetSafeHwnd() == NULL){
		return;
	}

	CRect Bar1,Bar2;

	pStatic1->GetWindowRect( &Bar1 );
	pStatic2->GetWindowRect( &Bar2 );

	ScreenToClient( Bar1 );
	ScreenToClient( Bar2 );
	
	//pGrid->MoveWindow(-2, 35, cx+2, cy);
	pGrid->MoveWindow(5, Bar1.Height()+Bar2.Height()+1+20, cx-10, cy-( Bar1.Height()+Bar2.Height()+1)-30 );
	pStatic1->MoveWindow(0, Bar1.top , cx, Bar1.Height());
	pStatic2->MoveWindow(0, Bar2.top , cx, Bar2.Height());
}

BOOL CLOC_STATISTICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("연속_열람_통계") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_열람_통계") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_열람_통계"));
	m_pDM_QUERY = FindDM(_T("DM_연속_열람_통계_쿼리"));
	m_pDM_RESULT = FindDM(_T("DM_연속_열람_통계_결과"));
	m_pDM_SUM = FindDM(_T("DM_연속_열람_통계_합계"));
	m_pDM_PRINT = FindDM(_T("DM_연속_열람_통계_출력"));
	m_pDM_KDC = FindDM(_T("DM_연속_열람_통계_분류기호"));
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = FindDM(_T("DM_연속_열람_통계_대상목록"));;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("통계결과그리드"));

	if(!m_pCM || !m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM || !m_pDM_PRINT || !m_pDM_KDC || !m_pGrid)
	{
		AfxMessageBox( _T("CM,DM Error : 연속_열람_통계") );
		return FALSE;
	}
	

	InitControl();

	CreateChildDlg();

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLOC_STATISTICS::InitControl()
{
	CComboBox* pCombo = NULL;
	pCombo = (CComboBox*)m_pCM->FindControl(_T("통계종류"));
	if(!pCombo) return;
	
	if(1 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("등록구분별통계"));
		pCombo->InsertString(1,_T("자료구분별통계"));
		pCombo->InsertString(2,_T("간행빈도별통계"));
		pCombo->InsertString(3,_T("별치기호별통계"));
	}
	else if(2 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("등록구분별통계"));
		pCombo->InsertString(1,_T("자료실구분별통계"));
		pCombo->InsertString(2,_T("수입구분별통계"));
		pCombo->InsertString(3,_T("배가상태별통계"));
		pCombo->InsertString(4,_T("간행빈도별통계"));
		pCombo->InsertString(5,_T("별치기호별통계"));
	}
	else if(3 == m_nTYPE)
	{
		pCombo->InsertString(0,_T("자료구분별통계"));
		pCombo->InsertString(1,_T("자료실구분별통계"));
		pCombo->InsertString(2,_T("배가상태별통계"));
		pCombo->InsertString(3,_T("간행빈도별통계"));
	}

	pCombo->SetCurSel(0);
	
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_chkSUMZERODEL);
	if(pButton) pButton->SetCheck(1);

}

VOID CLOC_STATISTICS::CreateChildDlg()
{
	
	//좌표 윈도우 
	m_pAxisWnd = new CLocStatisticsAxis(this);
	m_pAxisWnd->m_nTYPE = m_nTYPE;

	//통계 API
	m_pApi = new CLocStatisticsApi(this);
	m_pApi->m_pDM_QUERY = m_pDM_QUERY;
	m_pApi->m_pDM_RESULT = m_pDM_RESULT;
	m_pApi->m_pDM_SUM = m_pDM_SUM;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
	m_pApi->m_pDM_LIST = m_pDM_LIST;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_pApi->m_nTYPE = m_nTYPE;


	if(1 == m_nTYPE)
	{
		m_pCondWnd1 = new CLocStatisticsCond1(this);
		m_pCondWnd1->m_pApi = m_pApi;
	}
	else if(2 == m_nTYPE)
	{
		m_pCondWnd2 = new CLocStatisticsCond2(this);
		m_pCondWnd2->m_pApi = m_pApi;
	}
	else if(3 == m_nTYPE)
	{
		m_pCondWnd3 = new CLocStatisticsCond3(this);
		m_pCondWnd3->m_pApi = m_pApi;

	}
	

}

VOID CLOC_STATISTICS::ShowTmpDlg()
{

	if(1 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd1;
	}
	else if(2 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd2;
	}
	else if(3 == m_nTYPE)
	{
		m_pCondWnd = m_pCondWnd3;	

	}

	//행 조건 설정
	CString strtmp;
	CString class_name;

	CComboBox* pCombo = NULL;
	pCombo = (CComboBox*)m_pCM->FindControl(_T("통계종류"));
	if(!pCombo) return;

	pCombo->GetWindowText(strtmp);


	if(_T("간행빈도별통계") == strtmp) {
		class_name = _T("간행빈도코드");
		m_strPrintClassName = _T("간행빈도별");
	}
	else if(_T("자료구분별통계") == strtmp) {
		class_name = _T("자료구분");
		m_strPrintClassName = _T("자료구분별");
	}
	else if(_T("등록구분별통계") == strtmp) {
		class_name = _T("등록구분");
		m_strPrintClassName = _T("등록구분별");
	}
	else if(_T("자료실구분별통계") == strtmp) {
		class_name = _T("자료실구분");
		m_strPrintClassName = _T("자료실구분별");
	}
	else if(_T("배가상태별통계") == strtmp) {
		class_name = _T("배가상태");
		m_strPrintClassName = _T("배가상태별");
	}
	else if(_T("별치기호별통계") == strtmp) {
		class_name = _T("별치기호");
		m_strPrintClassName = _T("별치기호별");
	}
	else if(_T("수입구분별통계") == strtmp) {
		class_name = _T("입수구분");
		m_strPrintClassName = _T("수입구분별");
	}


	m_pAxisWnd->m_strClassName = class_name;
	m_strClassName = class_name; //현재 조건 저장

	if(m_pAxisWnd->m_bCreated)
	{
		m_pAxisWnd->SetClassCondition();
	}

	if(!m_pTmpDialog)
	{
		m_pTmpDialog = new CTmpDialog(this, m_pCondWnd ,m_pAxisWnd);
		m_pTmpDialog->m_nTYPE = m_nTYPE;

		if (m_pTmpDialog->Create(this))
		{
			m_pTmpDialog->ShowWindow(SW_SHOW);
			m_pTmpDialog->UpdateWindow();
			m_pTmpDialog->CenterWindow();
		
		}

	}
	else
	{
			m_pTmpDialog->ShowWindow(SW_SHOW);
			m_pTmpDialog->UpdateWindow();
			m_pTmpDialog->CenterWindow();
	}
}

LRESULT CLOC_STATISTICS::OnRunS(WPARAM wParam, LPARAM lParam) 
{
	INT ids;
	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = RunStatistics();
	AfxGetApp()->DoWaitCursor( -1 );
	
	return 0;
}

INT CLOC_STATISTICS::RunStatistics()
{
	
	m_pApi->RunStatisticsProc(m_strClassName);

	GridDisplay();

	return 0;
}

VOID CLOC_STATISTICS::GridDisplay()
{
	INT nRow = m_pDM_RESULT->GetRowCount();
	if(nRow<2) return;
		
	
	//중복 허용 및 0인 항목지우기 여부를 파악한다
	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;

	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;
	

	//먼저 결과 DM을 출력DM에 copy한다
	CopyResultDMToPrintDM(bDupDel,bZeroShow);

	m_pGrid->ShowWindow(FALSE);
	
	INT nPrintRow = m_pDM_PRINT->GetRowCount();
	INT nGridRow,nGridCol;
	nGridCol = 13;
	nGridRow = nPrintRow+1;
	
	m_pGrid->SetRows(nGridRow);
	m_pGrid->SetCols(0,nGridCol);
	m_pGrid->SetRowHeight(0,300);
	
	CString strData;
	CString strAlias;

	
	//column header(분류기호) 
	m_pGrid->SetRow(0);

	for(INT i=1;i<13;i++)
	{		
		m_pGrid->SetCol(i);

		strAlias.Format(_T("%d"),i);
		strData = m_pDM_KDC->GetCellData(strAlias,0);
		m_pGrid->SetColWidth(i, 0, 1000);
		m_pGrid->SetCellAlignment(4);
		m_pGrid->SetCellFontBold(TRUE);
		m_pGrid->SetText(strData);
	}	
		
	
	for(i=0;i<nPrintRow;i++)
	{
		m_pGrid->SetRow(i+1);
		m_pGrid->SetRowHeight(i+1,600);

		//row header(조건)
		m_pGrid->SetCol(0);
		m_pGrid->SetCellFontBold(TRUE);
		m_pGrid->SetCellAlignment(4);
		strData = m_pDM_PRINT->GetCellData(_T("행조건"),i);
		m_pGrid->SetText(strData);

		
		//데이터 
		for(INT j=1;j<12;j++)
		{		
			m_pGrid->SetCol(j);

			strAlias.Format(_T("%d"),j);
			strData = m_pDM_PRINT->GetCellData(strAlias,i);
			m_pGrid->SetCellAlignment(4);
			m_pGrid->SetText(strData);

		}	

		//행 합계 
		m_pGrid->SetCol(nGridCol-1);
		strData = m_pDM_PRINT->GetCellData(_T("합계"),i);
		m_pGrid->SetCellAlignment(4);
		m_pGrid->SetText(strData);
	}


	m_pGrid->ShowWindow(TRUE);

}

VOID CLOC_STATISTICS::OnchkDUPDEL() 
{
	// TODO: Add your control notification handler code here
	GridDisplay();
}

VOID CLOC_STATISTICS::OnchkSUMZERODEL() 
{
	// TODO: Add your control notification handler code here
	GridDisplay();
}


INT CLOC_STATISTICS::SetGridCellAlignment(CESL_Grid* pGrid)
{
	//가운데 정렬
	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();

	
	
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
			pGrid->SetColAlignment(i,1);
		else
			pGrid->SetColAlignment(i,4);

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);

	}

	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
	}

	return 0;
}

VOID CLOC_STATISTICS::MakeOutputFormat(CString& strData)
{
	if(strData.GetLength() == 0) return ;
	
	INT nSpecies,nVol,nBook;
	nSpecies = nVol =  nBook = 0;

	
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);
	
	//종 
	if(idx > 0) nSpecies = _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//권
	if(idx > 0)	nVol += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//책
	nBook = _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
	
	strData.Format(_T("%d\n[%d]\n(%d)"),nSpecies,nVol,nBook);
}

VOID CLOC_STATISTICS::MakeAliasArray(CArray<CString,CString>& arrayAlias)
{
	//결과 DM에서 열의 합계가 0이 아닌 alias를 array로 만든다
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	INT nSumRowIdx = nRowCnt - 2; //끝에서 두번째 -> 중복 합계 
	
	CString strAlias;
	CString strData;


	for(INT i=1;i<12;i++)
	{
		
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,nSumRowIdx);		
		
		if(IsValueExist(strData))
		{
			arrayAlias.Add(strAlias);
		}
	}

}

BOOL CLOC_STATISTICS::IsValueExist(CString strData)
{
	//데이터 값이 존재하는지 여부 
	if(strData.GetLength() == 0) return FALSE;
	
	INT nSpecies,nVol,nBook;
	nSpecies = nVol =  nBook = 0;

	
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);
	
	if(idx<0) return FALSE;

	//종 
	if(idx > 0) nSpecies = _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//권
	if(idx > 0)	nVol += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//책
	nBook = _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
	
	
	if(nSpecies != 0 || nVol != 0 || nBook != 0)
	{
		return TRUE;
	}
	
	
	return FALSE;
}

INT CLOC_STATISTICS::CopyResultDMToPrintDM(BOOL bDupDel,BOOL bZeroShow)
{
	
	m_pDM_PRINT->FreeData();
	m_pDM_KDC->FreeData();
	m_pDM_KDC->InsertRow(-1);
		

	CString strAlias,strData;

	//분류기호를 카피
	for(INT i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,0);
		m_pDM_KDC->SetCellData(strAlias,strData,0);
	}

	m_pDM_KDC->SetCellData(_T("12"),_T("합계"),0);


	//데이터는 출력 DM에 카피
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	CString strSum;
	INT nPrintRowIdx;

	for(i=1;i<nRowCnt-2;i++)
	{
		
		if(!bZeroShow) // 합계가 0인 행은 삭제한다 
		{
			strData = m_pDM_RESULT->GetCellData(_T("중복합계"),i);
			if(!IsValueExist(strData)) continue;
		}
		
		
		m_pDM_PRINT->InsertRow(-1);
		nPrintRowIdx = m_pDM_PRINT->GetRowCount()-1;
		
		//행조건 
		strData = m_pDM_RESULT->GetCellData(_T("행조건"),i);
		m_pDM_PRINT->SetCellData(_T("행조건"),strData,nPrintRowIdx);
	

		//데이터 
		for(INT j=1;j<12;j++)
		{
			
			strAlias.Format(_T("%d"),j);
			strData = m_pDM_RESULT->GetCellData(strAlias,i);
			MakeOutputFormat(strData);
			m_pDM_PRINT->SetCellData(strAlias,strData,nPrintRowIdx);
		}	
		

		//합계
		if(bDupDel)
		{
			strData = m_pDM_RESULT->GetCellData(_T("합계"),i);
		}
		else
		{
			strData = m_pDM_RESULT->GetCellData(_T("중복합계"),i);
		}
		
		MakeOutputFormat(strData);
		m_pDM_PRINT->SetCellData(_T("합계"),strData,nPrintRowIdx);
	}
	
	//열합계 
	INT nResultRowIdx;

	m_pDM_PRINT->InsertRow(-1);
	nPrintRowIdx = m_pDM_PRINT->GetRowCount()-1;

	if(bDupDel)
	{
		nResultRowIdx = nRowCnt - 1;
	}
	else
	{
		nResultRowIdx = nRowCnt - 2;
	}
	
	for(i=1;i<12;i++)
	{
			
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_RESULT->GetCellData(strAlias,nResultRowIdx);
		MakeOutputFormat(strData);
		m_pDM_PRINT->SetCellData(strAlias,strData,nPrintRowIdx);
	}	
	

	//총계 
	if(bDupDel)
	{
		strData = m_pDM_RESULT->GetCellData(_T("합계"),nResultRowIdx);
	}
	else
	{
		strData = m_pDM_RESULT->GetCellData(_T("중복합계"),nResultRowIdx);
	}
	
	MakeOutputFormat(strData);
	m_pDM_PRINT->SetCellData(_T("합계"),strData,nPrintRowIdx);
	m_pDM_PRINT->SetCellData(_T("행조건"),_T("합계"),nPrintRowIdx);

	return 0;
}

VOID CLOC_STATISTICS::PrintData()
{
	INT nRowCnt = m_pDM_PRINT->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM_PRINT->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속열람통계목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_PRINT, 0);
	
	
	CString strTitle;
	if(1 == m_nTYPE)
	{
		strTitle.Format(_T("연속작업종통계(%s)"),m_strPrintClassName);
	}
	else if(2 == m_nTYPE)
	{
		strTitle.Format(_T("연속소장종통계(%s)"),m_strPrintClassName);
	}	
	else if(3 == m_nTYPE)
	{
		strTitle.Format(_T("연속실시간열람통계(%s)"),m_strPrintClassName);
	}	
	
	pSIReportManager->SetTempVariable(_T("조건"),strTitle);
	

	CString strAlias,strData;
	for(INT i=1;i<13;i++)
	{
		strAlias.Format(_T("%d"),i);
		strData = m_pDM_KDC->GetCellData(strAlias,0);
		pSIReportManager->SetTempVariable(strAlias,strData);
	}
	
	pSIReportManager->SetTempVariable(_T("설명"),_T("*표시방법\n\n종수\n[권수]\n(책수)"));


	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}


}
HBRUSH CLOC_STATISTICS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/	

BEGIN_EVENTSINK_MAP(CLOC_STATISTICS, CESL_Mgr)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0400_08)
	ON_EVENT(CLOC_STATISTICS, IDC_gridMAIN, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLOC_STATISTICS::OnClickMshflexgrid() 
{
EFS_BEGIN
	//조회된 통계정보가 없으면 대상목록 리스트를 보일 필요가 없음
	if(m_pDM_RESULT->GetRowCount() < 1)
	{
		return ;
	}

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	CString sXValue, sYValue;

	// YColumn 코드값 확인
	sXValue = pGrid->GetTextMatrix( 0 , nCol );	
	sYValue = pGrid->GetTextMatrix( nRow , 0 );	

	INT ids = m_pApi->RunStatisticsListProc(m_strClassName, sXValue, sYValue);
	if(ids >= 0)
	{
		if(m_pApi->m_pDM_LIST != NULL && m_pApi->m_pDM_LIST->GetRowCount() > 0)
		{
			CLocStatisticsList dlg(this, m_pApi->m_pDM_LIST);
			dlg.DoModal();
		}
		else
		{
			MessageBox(_T("대상 목록 리스트가 존재 하지 않습니다."));
		}
	}
EFS_END
}
/*//END------------------------------------------------------------------------*/
// KOL.RED.2018.211 ---------------------------------------------------------------------------