// BO_ACQ_PurchaseDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PurchaseDataMgr.h"
#include "BO_ACQ_OrderDlg.h"
#include "BO_ACQ_ReviewDlg.h"
#include "PurchaseDataPrintDlg.h"
#include "../../../공통/BO_ACQ_COMMON/BO_ACQ_ImportMgr.h"
#include "../../../공통/BO_ACQ_COMMON/BO_ACQ_DataSortDlg.h"
#include "../../../공통/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../공통/K2UP_Common/BasicPrintMgr.h"
#include "../../../공통/Duplicate_Data_Search/DUP_KEY_SETTING.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr dialog


CBO_ACQ_PurchaseDataMgr::CBO_ACQ_PurchaseDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PurchaseDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_strDivNo = _T("");
	m_strAcqYear = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_bPurchaseCheckYN = FALSE;
}

CBO_ACQ_PurchaseDataMgr::~CBO_ACQ_PurchaseDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}	
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
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
}

VOID CBO_ACQ_PurchaseDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr message handlers

VOID CBO_ACQ_PurchaseDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_PurchaseDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입대상자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_구입자료관리"));
	m_pPrintDM = FindDM(_T("DM_구입자료관리_종"));
	// 2004.11.16 우선정리자료 사용여부에 따라 CM을 다른 걸로 정의하여 사용한다.
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);
/*	if( strPriorityCatalogYN == _T("Y"))
		m_pCM = FindCM(_T("CM_구입자료관리_5163"));
	else 
		m_pCM = FindCM(_T("CM_구입자료관리"));
*/
	m_pCM = FindCM(_T("CM_구입자료관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	GetDlgItem(IDC_stcPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_PURCHASE, PURCHASE, BO_PURCHASE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_PURCHASE, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	SetPurchaseDataInfo();
	InitPopupMenuItem();
/*
	//심의처리 사용여부를 검사한다. 'Y':사용함, 'O':선택사항, 'N':사용안함
	CString strValue;
	GetManageValue(_T("단행"), _T("수서"), _T("심의처리수행여부"))
*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_PurchaseDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_DATA);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(150, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(205, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 40, cx-10, cy-45);		
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
	/*
	CPurchaseDataPrintDlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return ;
	CString strSIReportFileName;
	INT nPrintType = dlg.m_PurchasingPrintType;
	if(nPrintType == 0)
	{
		nPrintType = BO_ACQ_PURCHASE_CHECKING_DATA;
		strSIReportFileName = _T("K2UP_심의대상자료목록");
	}
	else if(nPrintType == 1)
	{
		nPrintType = BO_ACQ_PURCHASE_CHECKED_DATA;
		strSIReportFileName = _T("K2UP_심의결과자료목록");
	}

	CESL_DataMgr * pDM = FindDM(_T("DM_구입자료관리_출력"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("출력용DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	INT ids = api.SPFExecutePath(_T("출력용DM생성"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return ;
	}
	
	CBasicPrintMgr mgr(this);
	mgr.SetPrintDMAlias(_T("DM_구입자료관리_출력"));
	mgr.SetPrintType(nPrintType);
	mgr.SetSIReportFilename(strSIReportFileName);
	mgr.Print();	
	*/
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPurchaseDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_PURCHASE);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
			return ;
		}
	}
	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	dlg.SetAccuKeyAlias(_T("BP_수서정보KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(SPECIES);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetPurchaseDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pPrintDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_PurchaseDataMgr::DeletePurchaseData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("삭제타입"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("BP_발주책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환할인종가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	ids = api.SPFExecutePath(_T("삭제"));
	if(ids == 0)
	{
		SetPurchaseDataInfo();
		DisplayGrid();	
	}
/*
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	api.SetTempData(_T("수서정보키알리아스"), _T("BP_수서정보KEY"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	ids = api.SPFExecutePath(_T("종삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제할 수 없습니다."));
		return;
	}
	if(ids == 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제하였습니다."));
		DisplayGrid();
		SetPurchaseDataInfo();
	}
*/
}

VOID CBO_ACQ_PurchaseDataMgr::ReviewPurchaseData()
{
	CString strGroupNo, strSortDesc;
	CBO_ACQ_ReviewDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupNo);
	m_pCM->GetControlMgrData(_T("그룹번호"), strGroupNo);
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempData(_T("그룹번호"), strGroupNo.GetBuffer(0));
	api.SetTempData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear().GetBuffer(0));
	api.SetTempData(_T("업무구분"), m_pSearchDlg->GetGroupCollection()->GetWorkCode().GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("심의처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 심의처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 심의처리 하였습니다."));
	if(dlg.m_bReviewResultPrint)
	{	
		INT ids = MakePrintDM();
		if(ids < 0) return ;
		CESL_DataMgr * pDM = FindDM(_T("DM_구입자료관리_출력"));
		if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
		m_pPrintDlg->SetPrintDM(pDM);
		m_pPrintDlg->SetSIReportFileName(_T("K2UP_심의결과자료목록"));
		m_pPrintDlg->Print();
		return ;
	}
}

VOID CBO_ACQ_PurchaseDataMgr::OrderPurcahseData()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return;
	}
	//그룹컬렉션 정보를 복사한다.
	CGroupCollection GroupCollection(this);
	m_pSearchDlg->GetGroupCollection()->CopyData(&GroupCollection);
	
	CheckGiveOrderSerialNo(&GroupCollection);

	if(GroupCollection.GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("주문하기 위한 차수정보가 없습니다."));
		return;
	}	

	CBO_ACQ_OrderDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDCANCEL == dlg.DoModal()) return ;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("그룹정보"), (DWORD*)&GroupCollection);
	CString strData = GroupCollection.GetAcqYear();
	api.SetTempData(_T("수입년도"), strData.GetBuffer(0));
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("주문처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 주문처리 할 수 없습니다."));
		return;
	}
	//JOB.2019.0073 : 충돌 License
	//ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);

	ESLAfxMessageBox(_T("선정한 자료를 주문처리 하였습니다."));
	m_pSearchDlg->GetGroupCollection()->DeleteThisGroupInfo(&GroupCollection);
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)NULL);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)&GroupCollection);
	ids = api.SPFExecutePath(_T("그룹자료삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("주문자료를 삭제할 수 없습니다."));
		return ;
	}

	SetPurchaseDataInfo();
	DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	if(IDOK == dlg.DoModal())
		DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangePurchaseInfoDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CPurchaseInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
		DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("구입대상"));
	mgr.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_PURCHASE);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(TRUE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::CopyPurchaseDataSearch()	
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("이미 중복조사가 수행중입니다."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("중복조사"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"), m_pSearchDlg->GetGroupCollection());
}

VOID CBO_ACQ_PurchaseDataMgr::DupPurchaseDataSearch()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("이미 복본조사가 수행중입니다."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("복본조사"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

BOOL CBO_ACQ_PurchaseDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

UINT CBO_ACQ_PurchaseDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입대상자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetPurchaseDataInfo();
		DisplayGrid();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("책To수서정보"));
	SetPurchaseDataInfo();
	SortByGroupInfo();
	DeleteDataProcessingDlg();
	DisplayGrid();
	
	return 0;
}


UINT CBO_ACQ_PurchaseDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("권DM"), NULL);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("삭제데이터"), (DWORD*)m_pDuplicateSearchMgr->GetAcqKeyArray());
	api.SetTempData(_T("비교Alias"), _T("IBS_종KEY"));
	INT ids = api.SPFExecutePath(_T("DM자료삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("삭제된 자료를 DM에서 제거할 수 없습니다."));
		return 0;
	}
	DisplayGrid();
	SetPurchaseDataInfo();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
/*	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRow+1);
	}
*/	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;
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
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_입력구분설명"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_입력구분"), nRow);
		if(strTmpData == _T("0")) str = _T("화면입력");
		else if(strTmpData == _T("1")) str = _T("재활용반입");
		else if(strTmpData == _T("2")) str = _T("희망자료반입");
		else if(strTmpData == _T("3")) str = _T("파일반입");
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_분류표구분"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		m_pPrintDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_구입처부호설명"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_할인종가격"))
	{
		CString strTmpData;
		INT nBookCnt = 0;
		DOUBLE dPrice, dDisc_Rate, dDisc_Price;
		strTmpData = m_pPrintDM->GetCellData(_T("BP_할인율"), nRow);
		dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
		strTmpData = m_pPrintDM->GetCellData(_T("UDF_종가격"), nRow);
		dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
		strTmpData.Format(_T("%.0f"), dDisc_Price);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_할인종가격"), strTmpData, nRow);
		m_pPrintDM->SetCellData(_T("UDF_변환할인종가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_종가격"))
	{
		str.TrimLeft();		str.TrimRight();
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환종가격"), str, nRow);
	}
	
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_PurchaseDataMgr::SetPurchaseDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d종 %d책의 구입 대상 자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_PurchaseDataMgr::ShowDataPurchaseImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strDivNo);
	CBO_ACQ_ImportMgr importDlg(this);
	importDlg.SetAcqType(PURCHASE);
	CGroupCollection * pGroups = m_pSearchDlg->GetGroupCollection();
	if(pGroups->GetGroupCount() == 0)
	{
		if(IDYES == ESLAfxMessageBox(_T("선정된 차수정보가 없습니다. 차수를 선정하시겠습니까?"), MB_YESNO))
		{
			CDivisionNo_List_Dlg dlg(this);
			dlg.SetWorkStatus(_T("구입대상"));
			dlg.SetAcquisitYear(_T(""));
			dlg.SetGroupCollection(pGroups);
			dlg.DoModal();
		}
		else return 0;
	}
	
	if(pGroups->IsEmpty()) return 0;
	importDlg.SetGroups(m_pSearchDlg->GetGroupCollection());
	importDlg.SetParentDM(m_pPrintDM);
	importDlg.SetParentBookDM(m_pDM);
	importDlg.SetParentGrid(m_pGrid);
	importDlg.DoModal();

	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nCurCnt > nRowCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt+1);
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowOrderSerialNoDlg()
{
	CString strGroupNo, strSortDesc;
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return;
	}
	CBO_ACQ_DataSortDlg dlg(this);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetType(1);
	if(IDCANCEL == dlg.DoModal()) return;
	DisplayGrid();
}

VOID CBO_ACQ_PurchaseDataMgr::SetCheckOrderInvalidData(CArray <INT, INT> * pArrayInvalidData)
{
	INT cnt = pArrayInvalidData->GetSize();
	if(cnt == 0) return;
	for(INT i=0 ; i<cnt ; i++)
	{
		SetGridRowBackColor(m_pGrid, pArrayInvalidData->GetAt(i));
	}
}

INT CBO_ACQ_PurchaseDataMgr::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
		pGrid->SetTextMatrix(nRow+1, 1, _T("V"));
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

UINT CBO_ACQ_PurchaseDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_PurchaseDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids = -1;
	CESL_DataMgr * pDM = NULL;
	if(lParam == BO_ACQ_PURCHASE_DATA)
	{
		m_pPrintDlg->SetPrintDM(m_pPrintDM);
		m_pPrintDlg->Print();
		return 0;
	}
	else if(lParam == BO_ACQ_PURCHASE_CHECKING_DATA || lParam == SERIES_PAPER)
	{
		ids = MakePrintDM();
		pDM = FindDM(_T("DM_구입자료관리_출력"));
	}
	else if(lParam == BO_ACQ_PURCHASE_CHECKED_DATA || lParam == SERIES_PAPER)
	{
		ids = MakePrintDM();
		pDM = FindDM(_T("DM_구입자료관리_출력"));
	}
	else if(lParam == BO_ACQ_ORDER_DATA_DIVNO)
	{
		ids = MakeOrderPrintDM(_T("BP_발주일련번호"));
		pDM = FindDM(_T("DM_주문자료관리_출력"));
	}
	else if(lParam == BO_ACQ_ORDER_DATA_PROVIDER)
	{
		ids = MakeOrderPrintDM(_T("BP_구입처부호"));
		pDM = FindDM(_T("DM_주문자료관리_출력"));
	}
	else if(lParam == BO_ACQ_ORDER_DATA_SUBJECTCODE)
	{
		ids = MakeOrderPrintDM(_T("BP_주제구분"));
		pDM = FindDM(_T("DM_주문자료관리_출력"));
	}

	if(ids < 0) return 0;
	
	m_pPrintDlg->SetPrintDM(pDM);
	m_pPrintDlg->Print();
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::CheckGiveOrderSerialNo(CGroupCollection *pGroupCollection)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("그룹정보"), (DWORD*)pGroupCollection);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("주문일련번호확인"));
	return ids;
}

VOID CBO_ACQ_PurchaseDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -601 /* DblClick */, OnDblClickgridPURCHASEDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_PurchaseDataMgr::OnDblClickgridPURCHASEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	ShowPurchaseDataDlg(GUPDATE);
}

VOID CBO_ACQ_PurchaseDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowOrderCancelMailMgr(INT nOpType, INT nOpenType, CGroupCollection * pGroupCollection)
{
	//JOB.2019.0073 : 충돌 License
// 	COrderMailMgr mailMgr(this);
// 	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();
// 
// 	mailMgr.SetParentGroupCollection(pGroupCollection);
// 	mailMgr.SetOperationType(nOpType);
// 	mailMgr.SetOpenType(nOpenType);
// 	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
// 	mailMgr.DoModal();
}

INT CBO_ACQ_PurchaseDataMgr::MakePrintDM()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CESL_DataMgr * pDM = FindDM(_T("DM_구입자료관리_출력"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("출력용DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	INT ids = api.SPFExecutePath(_T("출력용DM생성"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::MakeOrderPrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CESL_DataMgr * pDM = FindDM(_T("DM_주문자료관리_출력"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("출력용DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempData(_T("SORT_FIELD"), strSortField.GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("주문목록출력용DM생성"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_PurchaseDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CBO_ACQ_PurchaseDataMgr::SortByGroupInfo()
{
	if(m_pPrintDM == NULL) return 0;
	if(m_pPrintDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_작업번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::GiveOrderSerialNo()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	//
	CBO_ACQ_DataSortDlg dlg(this);
	CSortCondition * pCondition = dlg.GetSortCondition(_T(""));
	
	//발주일련번호 부여 루틴
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("정렬조건"), (DWORD*)pCondition);
	api.SetTempPtr(_T("SpeciesDM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GROUPCOLLECTION"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("발주일련번호부여"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("주문일련번호를 부여할 수 없습니다."));
		return;
	}
	
	ESLAfxMessageBox(_T("주문일련번호를 부여 하였습니다."));
	DisplayGrid();
}
