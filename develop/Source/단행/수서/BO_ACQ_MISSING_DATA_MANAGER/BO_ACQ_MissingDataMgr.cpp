// BO_ACQ_MissingDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_MissingDataMgr.h"
#include "../BO_ACQ_STATISTICS/BO_Purchase_Statistics.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../공통/BO_ACQ_COMMON/PriceSumDlg.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MissingDataMgr dialog


CBO_ACQ_MissingDataMgr::CBO_ACQ_MissingDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_MissingDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_MissingDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRow		= -1;
	m_nEditCol	= -1;
	m_bCellEditing	= TRUE;	
	m_strDivNo				= _T("");	
	///수정전 납품책수 백업
	m_sDeliveredCountBackup = _T("");
	m_strOrderPrice_Unit	= _T("0");
	m_strPosNumber			= _T("");
	m_strIs5163				= _T("");
	m_strIs64				= _T("");

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM				 = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM		 = NULL;
	m_pSearchDlg = NULL;
	m_pResultSearchDlg = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
}
CBO_ACQ_MissingDataMgr::~CBO_ACQ_MissingDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pResultSearchDlg)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}	
	if(m_pPrintDlg)
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}

}

VOID CBO_ACQ_MissingDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_MissingDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_MissingDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_MissingDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnRESULT_SEARCH, OnbtnRESULTSEARCH)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MissingDataMgr message handlers

VOID CBO_ACQ_MissingDataMgr::OnClose() 
{
	DestroyWindow();
}

// 초기화
BOOL CBO_ACQ_MissingDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strVal;

	//=======================================================================
	// 1. SM설정
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_미납자료관리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	//=======================================================================
	// 2. DM설정
	//=======================================================================
	m_pDM			= FindDM(_T("DM_미납자료관리")			);
	m_pPrintDM		= FindDM(_T("DM_미납자료관리_권")		);
	m_pPrintObjDM = FindDM(_T("DM_미납자료관리_권_출력용"));
	if( m_pDM == NULL || m_pPrintDM == NULL || m_pPrintObjDM == NULL )
	{
		ESLAfxMessageBox(_T("[DM_미납자료관리]\n[DM_미납자료관리_권]\n[DM_미납자료관리_권_출력용]\n설정되지 않았습니다."));
		return FALSE;
	}

	//=======================================================================
	// 3. CM설정
	//=======================================================================
	// 2004.11.16 우선정리자료 사용여부에 따라 CM을 다른 걸로 정의하여 사용한다.
	// BEGIN:
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);

	m_pCM = FindCM(_T("CM_미납자료관리"));
	if( m_pCM == NULL ) 
	{
		ESLAfxMessageBox( _T("[CM_미납자료관리]가 설정되지 않았습니다.") );
		return FALSE;
	}
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// END:

	//=======================================================================
	// 4. 가격절사단위설정, 5163/64 여부를 ESL_Manage테이블에서 값을 가져온다.
	//=======================================================================
	// 2005-01-13
	// 가격 자릿수 설정값을 가져온다.
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
	if( GetManageValue(_T("수서"), _T("단행"), _T("구입가격단위설정"), _T(""), m_strOrderPrice_Unit) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}
	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), m_strIs64);

		
	//=======================================================================
	// 5. 모든리소스를 숨긴다.
	//=======================================================================
	GetDlgItem(IDC_stcMISSING_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtMISSING_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcMISSING_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtMISSING_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_btnRESULT_SEARCH)->ShowWindow(SW_HIDE);	
	
	//=======================================================================
	// 6. 검색창클래스를 멤버변수에 등록한다.
	//=======================================================================
	if( m_pSearchDlg != NULL )
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_MISSING, PURCHASE, BO_MISSING, m_pDM->CONNECTION_INFO);


	//=======================================================================
	// 7. 출력물 클래스를 멤버변수에 등록한다.
	//=======================================================================
	if( m_pPrintDlg != NULL )
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_MISSING, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	

	//=======================================================================
	// 8. 정렬항목을 설정한다.
	//=======================================================================
	InitSortItem();

	//=======================================================================
	// 9. 마크정보를 설정한다.
	//=======================================================================
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	//=======================================================================
	// 10. 미납자료의 권,책수를 알려준다.
	//=======================================================================
	SetMissingDataInfo();

	//=======================================================================
	// 13. 팝업메뉴를 설정한다.
	//=======================================================================
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

VOID CBO_ACQ_MissingDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE 재지정
//	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcMISSING_DATA);
//	if(pBtn->GetSafeHwnd() == NULL) return;
//	pBtn->MoveWindow(5, 5, cx-10, 30);
	//====================================================

	CRect rect;
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE 재지정
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcMISSING_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	//====================================================
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtMISSING_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcMISSING_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtMISSING_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_btnRESULT_SEARCH);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(400, 10, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMISSING_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);	
	pGrid->MoveWindow(-2, 35, cx+2, cy);
}

VOID CBO_ACQ_MissingDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("검수완료"));
	mgr.DoModal();
}

VOID CBO_ACQ_MissingDataMgr::OrderCancel()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::CheckInMissingData(CString strWorkingStatus)
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("권DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("그리드"), (DWORD*)m_pGrid);
	api.SetTempData(_T("그리드컬럼번호"), m_nEditCol);
	api.SetTempData(_T("상태변경여부"), TRUE);
	api.SetTempData(_T("자료상태"), strWorkingStatus.GetBuffer(0));
	ids = api.SPFExecutePath(_T("검수처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("작업하신 검수작업 내용을 저장할 수 없습니다."));
		return;
	}
	else if(ids > 0) return ;
	
	ESLAfxMessageBox(_T("작업하신 검수작업 내용을 저장하였습니다."));

	m_pPrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("권DM"), (DWORD*)m_pPrintDM);
	ids = mgr.SPFExecutePath(_T("책To권"));
	SetMissingDataInfo();
	DisplayGrid();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료삭제
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_MissingDataMgr::DeleteMissingData()
{
	//=======================================================================
	// 1. 자료 선정 여부를 체크한다.
	//=======================================================================
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	//=======================================================================
	// 2. API에 값을 설정하고 [삭제]작업을 수행한다.
	//=======================================================================
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_MISSING);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("삭제타입"), (INT)VOLUME_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환할인가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	ids = api.SPFExecutePath(_T("삭제"));

	//=======================================================================
	// 3. [삭제]작업이 정상 수행 되었다면 하단내용과 그리드를 다시 그린다.
	//=======================================================================
	if( ids == 0 )
	{
		SetMissingDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_MissingDataMgr::ShowMissingDataDlg(OPERATION_TYPE nOpType)
{
	// 2005-01-06 권정보 그리드의 경우 다권본 수정시 화면 깜빡임을 방지 하기 위해 일괄 display하므로 
	// 선정 컬럼 보호 기능을 따로 둔다 
	if(m_pGrid) m_pGrid->m_bProtectSelectColumn = TRUE;

	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_MISSING);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
			return ;
		}
	}

	//===================================================
	//2009.04.08 ADD BY PJW : 그리드 수정을 할경우 선정된자료가 있을경우 현재커서를 선정에 추가한다.
	m_pGrid->SelectMakeList();
//DEL	if(nOpType == GUPDATE)
//DEL	{
		if( m_pGrid->SelectGetCount() > 0 )
		{
			m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
			m_pGrid->SelectMakeList();
		}
//DEL	}	
	//===================================================

	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	dlg.SetAccuKeyAlias(_T("BP_수서정보KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(VOLUME);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
//	DisplayGrid();
	SetMissingDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurSel = m_pPrintDM->GetRowCount();
		if(nCurSel > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}

	// 2005-01-06 
	// 선정 컬럼 보호 기능을 해제.
	if(m_pGrid) m_pGrid->m_bProtectSelectColumn = FALSE;

}

VOID CBO_ACQ_MissingDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();	
}

VOID CBO_ACQ_MissingDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

BOOL CBO_ACQ_MissingDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_MissingDataMgr::OnbtnRESULTSEARCH() 
{
	// TODO: Add your control notification handler code here
	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}
	m_pResultSearchDlg = new CSearchMgr(this, BO_ACQ_RESEARCH, m_pPrintDM, m_pGrid);
	m_pResultSearchDlg->ShowSearchDlg();
}

UINT CBO_ACQ_MissingDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("미납자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetMissingDataInfo();
		DisplayGrid();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("권DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("책To권"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetMissingDataInfo();
	//SortByGroupInfo();
	DisplayGrid();
	
	return 0;
}

UINT CBO_ACQ_MissingDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids;
	if(lParam == BO_ACQ_MISSING_DATA)
	{
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_수서정보KEY");
		CString strExtraAlias = _T("BB_권일련번호");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue, strExtraAlias);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("권리스트출력"));
		DeleteDataProcessingDlg();
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("출력데이터에 오류가 있습니다."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		/********************************************
		 ********************************************/
		//20080425 ADD BY PJW : 바탕화면 출력시 그리드내용을 딸라 가도록 수정
		if( 1 == m_pPrintDlg->GetPrintDataType() && 0 == m_pPrintDlg->GetDataSortYN())
		{
			INT nCntPrintDM;
			INT nCntPrintObjDM;
			CString strPrintDMRecKey;
			CString strPrintObjDMRecKey;
			nCntPrintDM = m_pPrintDM->GetRowCount();
			nCntPrintObjDM = m_pPrintObjDM->GetRowCount();
			for(INT i = 0 ; i < nCntPrintDM ; i++)
			{
				m_pPrintDM->GetCellData(_T("BB_책KEY"),i,strPrintDMRecKey);
				for(INT j = 0 ; j < nCntPrintObjDM ; j++)
				{
					m_pPrintObjDM->GetCellData(_T("BB_책KEY"),j,strPrintObjDMRecKey);
					if( strPrintDMRecKey == strPrintObjDMRecKey )
					{
						m_pPrintObjDM->SORT_SwapRow(i,j);
					}

				}

			}
		}
		/********************************************
		 ********************************************/
		m_pPrintDlg->Print();
	}
	if(lParam == BO_ACQ_NONCHECKIN_STATISTICS)
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(-1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("검수완료"));
		dlg.SetSpeciesType(BO_MISSING);
		dlg.DoModal();		
	}
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::SetMissingDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d권 %d책의 미납 자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_MissingDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode, strOrderSerialNo;
	INT nGroupNo, nSerialNo, nWorkNo, nOrderSerialNo;
	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_차수정보"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_수입년도"), nRow);
		strWorkCode = m_pPrintDM->GetCellData(_T("IBS_업무구분"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pPrintDM->GetCellData(_T("BP_작업번호"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pPrintDM->GetCellData(_T("BP_일련번호"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		strOrderSerialNo = m_pPrintDM->GetCellData(_T("BP_발주일련번호"), nRow);
		nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		strTmpData.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
		m_pPrintDM->SetCellData(_T("UDF_출력차수정보"), strTmpData, nRow);
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);		
	}
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_분류표구분"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("2") ) 
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") ) 
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
		}
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_구입처부호설명"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	/*
	else if(strDMFieldAlias == _T("BP_할인율"))
	{
		strTmpData = m_pPrintDM->GetCellData(strDMFieldAlias, nRow);
		if(strTmpData.IsEmpty()) 
		{
			str = _T("0");
			m_pPrintDM->SetCellData(strDMFieldAlias, _T("0"), nRow);
		}
	}
	*/
	else if(strDMFieldAlias == _T("UDF_할인가"))
	{
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("UDF_발주가격_권"), nRow);
		strTmpData.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pPrintDM->SetCellData(_T("UDF_변환할인가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_가격"))
	{
		CString strTmpData;
		INT nBookCnt = 0;
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("BB_가격"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pPrintDM->GetCellData(_T("UDF_책수"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		// 2004.09.14
		strTmpData = TruncPrice(m_pPrintDM, dPrice*nBookCnt);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		// 2004.12.02
		CString strTmpData;
		DOUBLE dPrice;
		str.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(str.GetBuffer(0));
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		strTmpData = TruncPrice(m_pPrintDM, dPrice);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_MissingDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_MissingDataMgr)
	ON_EVENT(CBO_ACQ_MissingDataMgr, IDC_gridMISSING_DATA, -600 /* Click */, OnClickgridMISSINGDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_MissingDataMgr, IDC_gridMISSING_DATA, -601 /* DblClick */, OnDblClickgridMISSINGDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_MissingDataMgr::OnClickgridMISSINGDATA() 
{
	// TODO: Add your control notification handler code here
/*	INT nRow = -1;
	INT nCol = -1;
	nRow = m_pGrid->GetRow();
	nCol = m_pGrid->GetCol();
	
	if (10 == nCol) 
	{
		this->m_sDeliveredCountBackup = m_pGrid->GetText();
		m_pGrid->GridEdit(m_pGrid->GetRow()-1, m_pGrid->GetCol()-1);
	}
	else
	{
		this->m_bCellEditing = FALSE;
	}	
*/
}


BOOL CBO_ACQ_MissingDataMgr::PreTranslateMessage(MSG* pMsg) 
{
	if (VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_gridMISSING_DATA)->m_hWnd == pMsg->hwnd)
		{
			this->m_bCellEditing = !this->m_bCellEditing;
			if (!this->m_bCellEditing) 
			{
				OnClickgridMISSINGDATA();
			}
		}
	}
	
	if (ESL_GRID_MODIFY == pMsg->message)
	{
		// 납풉책수 컬럼이 수정되었다면.
		if (pMsg->lParam == m_nEditCol) 
		{	
			if (0>ReCalcBookCount(pMsg->wParam + 1))	///수정 행번호
			{
				ESLAfxMessageBox(_T("미납책수를 설정하는데 실패하였습니다."));
			}	
/*			if(0>ReCalcDeliveryPrice(pMsg->wParam + 1))	///수정 행번호
			{
				ESLAfxMessageBox(_T("납품가격을 설정하는데 실패하였습니다."));
			}
*/		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

BOOL CBO_ACQ_MissingDataMgr::CheckIsDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}


INT CBO_ACQ_MissingDataMgr::ReCalcBookCount(INT nRow/*def:-1*/)
{
	INT nCurRow = -1;
	if (-1==nRow)	nCurRow = m_pGrid->GetRow();
	else			nCurRow = nRow;
	///입력된 책 수 검증
	CString sTemp, strDeliveredCount;
	BOOL bInvalidCount = FALSE;
	INT nOrderCount;
	
	nOrderCount	= _ttoi((m_pGrid->GetTextMatrix(nCurRow, m_nEditCol)).GetBuffer(0));
	strDeliveredCount = m_pGrid->GetTextMatrix(nCurRow, m_nEditCol+1);
	
	if(!CheckIsDigit(strDeliveredCount))
	{
		sTemp.Format(_T("납품책수에 숫자 아닌값( %s )이 입력되었습니다."), strDeliveredCount);
		bInvalidCount = TRUE;
	}
	INT nDeliveredCount = _ttoi((strDeliveredCount).GetBuffer(0));
	INT nNondeliveredCount = nOrderCount-nDeliveredCount;

	///납품책수가 주문책수보다 크다면
	if (nOrderCount<nDeliveredCount) 
	{
		sTemp.Format(_T("납품책수(%d)가 주문책수(%d)보다 큽니다."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	///납품책수가 0보다 작다면
	else if (0>nDeliveredCount)
	{
		sTemp.Format(_T("납품책수(%d)가 0 보다 작습니다."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	
	///잘못된 책수 입력됨
	if (bInvalidCount) 
	{
		ESLAfxMessageBox(sTemp);
		///원래 납품책수/주문책수 값으로 복원
		m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+1, m_pPrintDM->GetCellData(_T("UDF_납품책수"), nCurRow-1));
		nDeliveredCount = _ttoi(m_pPrintDM->GetCellData(_T("UDF_납품책수"), nCurRow-1).GetBuffer(0));
		sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
		m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);
		return 1;
	}
	
	sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
	m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);

	return 0;
}

VOID CBO_ACQ_MissingDataMgr::OnDblClickgridMISSINGDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	if(m_pGrid->GetCol() == m_nEditCol+1) return ;
	
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	ShowMissingDataDlg(GUPDATE);
}

INT CBO_ACQ_MissingDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::SelDataDeliveryProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("GET_COLUMN"), _T("책수"));
	api.SetTempData(_T("SET_COLUMN"), _T("납품책수"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("미납책수"));
	ids = api.SPFExecutePath(_T("납품처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 납품처리할 수 없습니다."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::SelDataDeliveryCancelProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("GET_COLUMN"), _T("책수"));
	api.SetTempData(_T("SET_COLUMN"), _T("미납책수"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("납품책수"));
	ids = api.SPFExecutePath(_T("납품취소"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 납품취소할 수 없습니다."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();

	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	m_pGrid->m_bGridEdit = TRUE;
	m_pGrid->SELEDITOR_RemoveAllColumns();
	CBO_ACQ_Business_Api api(this);
	m_nEditCol = api.GetGridColumnIdx(m_pGrid, _T("납품책수"));
	m_pGrid->SELEDITOR_AddColumn(m_nEditCol);
}

VOID CBO_ACQ_MissingDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	dlg.SetSpeciesYN(FALSE);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_MissingDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_MissingDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

/*
INT CBO_ACQ_MissingDataMgr::SortByGroupInfo()
{
	if(m_pPrintDM == NULL) return 0;
	if(m_pPrintDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}
*/

CString CBO_ACQ_MissingDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery;
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	m_pDM->GetOneValueQuery(strQuery, strValue);
	return strValue;
}

VOID CBO_ACQ_MissingDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_MISSING);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentBookDM(m_pDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(FALSE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_MissingDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pPrintDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pPrintDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

UINT CBO_ACQ_MissingDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_MissingDataMgr::InitSortItem()
{
	CStringArray* pTmpData	 = NULL;	
	CMapStringToString* pMap = NULL;
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("IBS_업무구분"));
	pTmpData->Add(_T("BP_그룹번호"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_발주일련번호"));
	pTmpData->Add(_T("BB_권일련번호"));

	pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	pMap->SetAt(_T("UDF_구입처부호설명"), _T("BP_구입처부호"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

CString CBO_ACQ_MissingDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 가격정책에 따라 절사단위설정값을 읽어와서 지정된 자릿수만큼 표기하도록 한다.
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else  // m_strPosNumber == _T("0") || m_strPosNumber.Empty()
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

HBRUSH CBO_ACQ_MissingDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
