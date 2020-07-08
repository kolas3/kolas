// BO_ACQ_PurchaseDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PurchaseDataMgr.h"
#include "BO_ACQ_OrderDlg.h"
#include "BO_ACQ_ReviewDlg.h"
#include "PurchaseDataPrintDlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../공통/BO_ACQ_COMMON/BO_ACQ_ImportMgr.h"
#include "../../../공통/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../공통/Duplicate_Data_Search/DUP_KEY_SETTING.h"
#include <direct.h>
// 2005-01-31
#include "DataExportDlg.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"
#include "../../../공통/CMarcChanger/MARC_CHANGER_01.h"
//==============================================
//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
#include "SBL.H"
//==============================================

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
	m_pPrintObjDM = NULL;
	m_pSubTotalPrintDM = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_strDivNo = _T("");
	m_strAcqYear = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_bPurchaseCheckYN = FALSE;
	m_pSortItemSet = NULL;
	m_nRow = -1;
	m_strPosNumber.Empty(); //2005-01-13
	m_strIs5163 = _T("");
	m_strIs64	= _T("");
	m_strIs90	= _T("");
	m_strOrderPrice_Unit = _T("");
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
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
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
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr message handlers
VOID CBO_ACQ_PurchaseDataMgr::OnClose() 
{
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_PurchaseDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_구입대상자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_구입자료관리"));
	m_pPrintDM = FindDM(_T("DM_구입자료관리_종"));
	m_pPrintObjDM = FindDM(_T("DM_구입자료관리_출력용"));
	m_pSubTotalPrintDM = FindDM(_T("DM_주문자료관리_출력"));
	// 2004.11.16 우선정리자료 사용여부에 따라 CM을 다른 걸로 정의하여 사용한다.
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);
	// 2005-01-13
	// 가격 자릿수 설정값을 가져온다.
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
	if( GetManageValue(_T("수서"), _T("단행"), _T("구입가격단위설정"), _T(""), m_strOrderPrice_Unit) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}
	
	// 2005-01-28
	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), m_strIs64);
	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T(""), m_strIs90);

	
	m_pCM = FindCM(_T("CM_구입자료관리"));
	if ( m_pCM == NULL ) {
		AfxMessageBox( _T("CM_구입자료관리 error") );
		return -2;
	}
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
	//m_pPrintDlg->SetIs90Flag(m_strIs90);
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	SetPurchaseDataInfo();
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_PurchaseDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_DATA);
	//if(pBtn->GetSafeHwnd() == NULL) return;
	//pBtn->MoveWindow(5, 5, cx-10, 30);

	CButton *pBtn;

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 35, cx+2, cy-30);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) 
	{
		m_pPrintDlg->Create(this);
	}
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPurchaseDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	BOOL strNewSaveFlag;
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
	dlg.SetParentType(SPECIES);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	//========================================================
	//20080613 ADD BY PJW : 새로저장일경우 새로저 Flag를 받아온다
	strNewSaveFlag = FALSE;
	strNewSaveFlag = dlg.m_bNewSaveFlag;
	//========================================================
	

	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();

	//JOB.2020.0026 ADD BY KYJ : 입력 > 닫기 > 입력 > 저장 할 경우 그리드 그리드 빈칸 생성되는 오류 수정
	if( nCurCnt > 0 && SINSERT == nOpType )
	{
	//=====================================================
	//2008.12.15 ADD BY PJW : UDF_종등록구분을 입력일경우도 셋팅한다.
	CString strSpeciesRegCode;
	m_pPrintDM->GetCellData(_T("IBS_색인등록구분"),nCurCnt-1,strSpeciesRegCode);
	strSpeciesRegCode.TrimLeft();	strSpeciesRegCode.TrimRight();
	strSpeciesRegCode.Replace(_T(" "),_T("/"));
	m_pPrintDM->SetCellData(_T("UDF_종등록구분"),strSpeciesRegCode,nCurCnt-1);
	//=====================================================
	}
	//========================================================
	//20080613 UPDATE BY PJW : 새로저장일경우 주문번호를 마지막 주문번호로 가지도록 
	//if(nOpType == SINSERT)
	strNewSaveFlag = dlg.m_bNewSaveFlag;
	if(SINSERT == nOpType || strNewSaveFlag == strNewSaveFlag)
	//========================================================
	{
		if(nCurCnt > nRowCnt)
		{
			// 구입-입력된 책 선정처리 _T("Y") 적용
			// 2005-09-28 강봉수
			//
				for(INT i = nRowCnt ; i<nCurCnt ; i++)
				{
					m_pPrintDM->SetCellData(_T("BP_선정비선정"), _T("Y"), i );
				}
				AllControlDisplay(_T("CM_구입자료관리"));
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
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
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
}

VOID CBO_ACQ_PurchaseDataMgr::ReviewPurchaseData()
{
	CString strGroupNo, strSortDesc;
	
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return;
	}
	
	//===================================
	//20080603 ADD BY PJW : 심의처리할자료의 책키를 넘겨준다.
	m_pSearchDlg->GetGroupCollection()->m_strSelectBookKey = GetSelectBookKey();
	//===================================

	//차수콜렉션이 전체자료일 경우 특정차수를 선택할 것을 종용
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return;
	}
	
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
		ShowDataProcessingDlg();
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CMakePrintDMMgr PrintMgr(this);
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
		PrintMgr.SetTempData(_T("책DM알리아스"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2형식"), FALSE);
		PrintMgr.SetTempData(_T("출력자료정렬여부"), FALSE);
		PrintMgr.SetTempData(_T("출력데이터타입"), 0);
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("그룹컬렉션"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pSubTotalPrintDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T(""));
		
		ids = PrintMgr.SPFExecutePath(_T("소계자료출력"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
			return ;
		}
		if(ids == 1000) return ;
		
		m_pPrintDlg->CreatePrintMgr();
		m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
		m_pPrintDlg->SetSIReportFileName(_T("K2UP_심의결과자료목록"));
		m_pPrintDlg->PrintPaper();
	}
}

VOID CBO_ACQ_PurchaseDataMgr::OrderPurcahseData()
{
	CBO_ACQ_Business_Api api(this);
	//BOOL bGridSelectCol;
	//==========================
	//2008.07.01 ADD BY PJW : 주문번호를 부여한다.
	//==========================
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return;
	}
	//차수콜렉션이 전체자료일 경우 특정차수를 선택할 것을 종용
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return;
	}

	CString strSpeciesKey;
	CString strBookSpeciesKey;
	INT nIdx, y;
	for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
	{	
		m_pDM->SetCellData(_T("선정"), _T(""), y );	
	}			

	for (nIdx = 0 ; nIdx < m_pPrintDM->GetRowCount() ; nIdx++ )
	{
		if ( m_pGrid->GetTextMatrix(nIdx+1,1) == _T("V") )
		{
			if ( m_pPrintDM->GetCellData ( _T("BP_선정비선정"), nIdx ) != _T("Y") )
			{
				/*
				if ( IDYES == ESLAfxMessageBox(_T("한 차수 내에 주문번호가 부여되지 않은 자료가 있습니다. \r\n주문번호를 부여하시거나 차수분리를 통해 한 차수내에 주문번호가 부여된 자료만 있도록 해야 합니다. \r\n지금 차수관리를 통해 차수 분리를 하시겠습니까? "), MB_YESNO) )
				{
					ShowDivNoMgrDlg();
					return;	
				}
				*/
				ESLAfxMessageBox(_T("선정된 자료중 주문번호가 부여되지 않은 자료가 있습니다. \r\n주문번호를 부여하여 주십시오."));				
				return;				
			}
			m_pPrintDM->SetCellData( _T("선정"), _T("V"), nIdx  );
			// 2008.03.12
			// 종정보의 선정 상태를 책정보에 반영한다.
			// DM_구입정보관리_종 -> DM_구입정보관리
			strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_종KEY"), nIdx );
			for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
			{
				strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_종KEY"), y );
				if ( strSpeciesKey == strBookSpeciesKey )
				{
					m_pDM->SetCellData(_T("선정"), _T("V"), y );
				}
			}			
		}
		else
		{
			m_pPrintDM->SetCellData( _T("선정"), _T(""), nIdx  );
		}
	}	

	//그룹컬렉션 정보를 복사한다.
	CGroupCollection GroupCollection(this);
	m_pSearchDlg->GetGroupCollection()->CopyData(&GroupCollection);
	
	CheckGiveOrderSerialNo(&GroupCollection);
	api.CheckValidationOrderProcess(m_pDM, &GroupCollection);
	if(GroupCollection.GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("주문하기 위한 차수정보가 없습니다."));
		return;
	}	
	
	//===================================
	//20080603 ADD BY PJW : 심의처리할자료의 책키를 넘겨준다.
	CString strSelectBookKey;
	strSelectBookKey = GetSelectBookKey();
	if( strSelectBookKey.IsEmpty() )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	m_pSearchDlg->GetGroupCollection()->m_strSelectBookKey = strSelectBookKey;
	//===================================
	CBO_ACQ_OrderDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDCANCEL == dlg.DoModal()) return ;
	
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
	//JOB.2019.0073 : 충돌 License 제거
	// 20070814 ADD PJW : 주문메일발송 사용 함일 때만 적용되도록 한다.
// 	CString strMailSendYN;
// 	m_pDM ->GetOneValueQuery(_T(" SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='주문메일자동발송' AND GROUP_3='단행' AND MANAGE_CODE = UDF_MANAGE_CODE "), strMailSendYN);
// 	
// 	if ( strMailSendYN == _T("Y") )	ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);
	//ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);

	ESLAfxMessageBox(_T("선정한 자료를 주문처리 하였습니다."));
	//================================================
	//20080617 REM BY PJW : 그리드에 남은 데이처를 주문처리할수 있도록 하기위해서 그룹지우는 것을 제거
	//m_pSearchDlg->GetGroupCollection()->DeleteThisGroupInfo(&GroupCollection);
	//================================================
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)NULL);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)&GroupCollection);
	//==================================================
	//20080604 UPDATE BY PJW : 그리드의 내용이 모두 사라지는 것을 방지하지 위해 그룹삭제를 하지 않는다.
	//ids = api.SPFExecutePath(_T("그룹자료삭제"));
	//if(ids < 0)
	//{
	//	ESLAfxMessageBox(_T("주문자료를 삭제할 수 없습니다."));
	//	return ;
	//}
	CString strSelect;
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	for ( INT i=0 ; i < nRowCnt ; i++ )
	{	
		m_pDM->GetCellData( _T("선정"), i, strSelect );
		if( _T("V") == strSelect )
		{
			m_pDM->DeleteRow(i);
			--nRowCnt;
			i--;
		}
	}
	nRowCnt = m_pPrintDM->GetRowCount();
	for ( i=0 ; i < nRowCnt ; i++ )
	{	
		m_pPrintDM->GetCellData( _T("선정"), i, strSelect );
		if( _T("V") == strSelect )
		{
			m_pPrintDM->DeleteRow(i);
			--nRowCnt;
			i--;
		}
	}
	//==================================================

	SetPurchaseDataInfo();
	
	//bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	//m_pGrid->m_bProtectSelectColumn = TRUE ;
	DisplayGrid();
	//m_pGrid->m_bProtectSelectColumn = bGridSelectCol ;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	if( IDOK == dlg.DoModal() )
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
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
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetSpeciesType(1);

	if(IDOK == dlg.DoModal())
	{
//		m_pGrid->SetProtectSelectColumn(TRUE);
		//===================================================
		//2009.03.27 UPDATE BY LKS : 출력물을 위하여 재검색 해준다.
		OnQueryCreate(0,0);
		//===================================================
		DisplayGrid();
//		m_pGrid->SetProtectSelectColumn(FALSE);
		AllControlDisplay(_T("CM_구입자료관리"));
	}
	SetPurchaseDataInfo();
	/******************************************
	 ******************************************/
	//20080319 ADD BY PJW : 그리드의 선정을 유지하기 위해서 재선정 해준다.
	INT nSelect;
	CString strTemp;
	INT nCnt;
	nCnt = m_pGrid->SelectGetCount();
	nSelect = m_pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		m_pGrid->SetTextMatrix( nSelect+1, 1,_T("V"));
		nSelect = m_pGrid->SelectGetNext();
	}

	/******************************************
	 ******************************************/
//DEL	CString strWhere;
//DEL	strWhere.Format(_T("sdfds"));
//DEL	m_pDM_BOOK->RefreshDataManager(strWhere);


}

VOID CBO_ACQ_PurchaseDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("구입대상"));
	mgr.DoModal();
	
	//===================================================
	//2009.06.04 ADD BY LKS : 검색된 차수 새로 검색
	
	CString strAcqNo;
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtPURCHASE_DIVNO);
		pEdit->GetWindowText(strAcqNo);
	if(!strAcqNo.IsEmpty())
		OnQueryCreate(0,0);
	//===================================================
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
/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ] 
// 복본조사 툴바버튼 수행
//
// [ 미니스펙 ]
// 1. 출력용 DM 및 그리드의 선정을 체크하여 가장 위에 있는 내용을 선택하여
//	  복본조사를 준비한다.
// 2. 복본조사 수행을 위한 복본조사 클래스를 생성한다.
// 3. 복본조사가 수행중인데 또 수행을 하려 한다면 메시지로 알려준다.
// 4. 복본조사를 수행한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PurchaseDataMgr::DupPurchaseDataSearch()
{
	INT ids = 0;


	//=======================================================================
	// 1. 출력용 DM 및 그리드의 선정을 체크하여 가장 위에 있는 내용을 선택하여
	//	  복본조사를 준비한다.
	//=======================================================================
	ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}


	//=======================================================================
	// 2. 복본조사 수행을 위한 복본조사 클래스를 생성한다.
	//=======================================================================
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	//=======================================================================
	// 3. 복본조사가 수행중인데 또 수행을 하려 한다면 메시지로 알려준다.
	//=======================================================================
	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("이미 복본조사가 수행중입니다."));
		return ;
	}		
	
	//=======================================================================
	// 4. 복본조사를 수행한다.
	//	- m_pPrintDM : DM_구입자료관리_종
	//=======================================================================
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("복본조사"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

BOOL CBO_ACQ_PurchaseDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 검색쿼리문으로 DM을 구축한다.
// [ 미니스펙 ]
// 1. 프로그레스창을 생성한다.
// 2. 검색용 쿼리문을 이용하여 책DM을 구축한다.
// 3. 구축된 책DM을 이용하여 종DM을 구축한다.
/////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_PurchaseDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	
	//=======================================================================
	// 변수정의
	//=======================================================================	
	CString strQuery = _T("");
	
	//=======================================================================
	// 1. 프로그레스창을 생성한다.
	//=======================================================================
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입대상자료를 구축중입니다."));

	//=======================================================================
	// 2. 검색용 쿼리문을 이용하여 책DM을 구축한다.
	//=======================================================================
	m_pSearchDlg->GetWhereStmt(strQuery);
	TRACE(_T("strQuery"));
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());

	//=======================================================================
	// 3. 구축된 책DM을 이용하여 종DM을 구축한다.
	//=======================================================================
	m_pPrintDM->FreeData();
	if( 0 != m_pDM->GetRowCount() )
	{
		CBO_ACQ_Business_Api mgr(this);
		mgr.InitTempData();
		mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	
		INT ids = mgr.SPFExecutePath(_T("책To수서정보"));
		DeleteDataProcessingDlg();
			if( ids < 0 )	return ids;
	
		m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
		SetPurchaseDataInfo();
		//SortByGroupInfo();

		DisplayGrid();
	}
	else
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetPurchaseDataInfo();
		DisplayGrid();
	}
	
	// 2006.04.18 KBS - 국방대요구사항 : 검색후 검색창 숨기기
	if( m_strIs90 == _T("Y") )	m_pSearchDlg->HideSearchDlg();
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
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


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids, nGroupNo, nWorkNo, nSerialNo, nBookCnt;
	DOUBLE dPrice, dDisc_Rate, dExchangeRate, dDisc_Price;
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	CString strData1, strData2, strPrice, strDiscRate, strExchangeRate;
	CMarc marc;
	
	TCHAR* pEndPtr = NULL;

	if( m_nRow != nRow )
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if( ids < 0 )	return -1;

		strTmpData.Empty();	
		ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if( ids >= 0 )	m_pPrintDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);

		strTmpData.Empty();	
		ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if( ids >= 0 )	m_pPrintDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);

		strTmpData.Empty();	
		// IBS 본표제를 왜가져오는가?
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_본표제"), nRow);

		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);

		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
		
		// 2005-01-27 5163추가내용(육사에도 반영)
		if( _T("Y") == m_strIs5163 || _T("Y") == m_strIs64 )
		{
			strTmpData =m_pPrintDM->GetCellData(_T("BS_사용자정의코드1"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("도서분류코드1"), strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_사용자정의코드1"), strTmpData, nRow);

			// 2005-01-28
			// UDF_서명저작자사항
			strData1 = m_pPrintDM->GetCellData(_T("UDF_표제사항"), nRow);
			strData2 = m_pPrintDM->GetCellData(_T("IBS_저작자"), nRow);
			strTmpData.Empty();
			if( strData2.IsEmpty() == TRUE )	strTmpData = strData1;
			else  strTmpData = strData1 + _T(" / ") + strData2;
			m_pPrintDM->SetCellData(_T("UDF_서명저작자사항"), strTmpData, nRow);

			// UDF_발행자사항
			strData1.Empty();		strData2.Empty();
			strData1 = m_pPrintDM->GetCellData(_T("IBS_발행자"), nRow);
			strData2 = m_pPrintDM->GetCellData(_T("IBS_발행년"), nRow);
			if( strData2.IsEmpty() == TRUE )	strTmpData = strData1;
			else  strTmpData = strData1 + _T(", ") + strData2;
			m_pPrintDM->SetCellData(_T("UDF_발행자사항"), strTmpData, nRow);

			// UDF_할인단가
			strPrice = m_pPrintDM->GetCellData(_T("BP_가격"), nRow);
			//dPrice = (DOUBLE)atof(strPrice.GetBuffer(0));
			dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

			strDiscRate = m_pPrintDM->GetCellData(_T("BP_할인율"), nRow);
			//dDisc_Rate = (DOUBLE)atof(strDiscRate.GetBuffer(0));
			dDisc_Rate = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);

			strExchangeRate = m_pPrintDM->GetCellData(_T("BP_환율"), nRow);
			//dExchangeRate = (DOUBLE)atof(strExchangeRate.GetBuffer(0));
			dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

			// 할인단가 계산
			dDisc_Price = (dPrice - ((dPrice*dDisc_Rate)/100)) * dExchangeRate;
			
			// 계산한결과는 소수점 버린다.
			//20070512 pjw수정 원화일경우 소수점제외
			if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
			{
				strTmpData.Format(_T("%d"), (INT)dDisc_Price);
			}
			else
			{
				strTmpData.Format(_T("%.0f"), dDisc_Price);
			}

			//strTmpData.Format(_T("%.0f"), dDisc_Price);

			// 결과값에 1000원단위로 콤마를 삽입하여 UDF_할인단가에 저장한다.
			m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_할인단가"), strTmpData, nRow);
		}

		m_nRow = nRow;

		if( m_nRow == m_pPrintDM->GetRowCount()-1 )		m_nRow = -1;
	}



	//=======================================================================
	// 1. UDF_차수정보 및 UDF_출력차수정보를 저장한다.
	//=======================================================================
	if( strDMFieldAlias == _T("UDF_차수정보") )
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_수입년도"), nRow);
		strWorkCode = m_pPrintDM->GetCellData(_T("IBS_업무구분"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pPrintDM->GetCellData(_T("BP_작업번호"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pPrintDM->GetCellData(_T("BP_일련번호"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));

		// 차수정보를 생성한다.
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);

		// 출력차수정보를 생성한다.
		strGroupNo.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(_T("UDF_출력차수정보"), strGroupNo, nRow);
	}
	//=======================================================================
	// 2. UDF_입력구분설명을 저장한다.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_입력구분설명") )
	{
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BP_입력구분"), nRow);

		if		( strTmpData == _T("0") )	str = _T("화면입력");
		else if	( strTmpData == _T("1") )	str = _T("재활용반입");
		else if ( strTmpData == _T("2") )	str = _T("희망자료반입");
		else if	( strTmpData == _T("3") )	str = _T("파일반입");
		else								str = _T("적용안함");

		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// 3. UDF_주제구분설명을 저장한다.
	//=======================================================================
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{ 
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_분류표구분"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		else if ( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
		}
		else /*if( strTmpData.IsEmpty() == TRUE )*/ 
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	//=======================================================================
	// 4. UDF_구입처부호설명을 저장한다.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_구입처부호설명") )
	{
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// UDF_단가를 저장한다.
	//=======================================================================
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		str.Replace(_T(","), _T(""));
		str.TrimLeft();		str.TrimRight();
		//dPrice = (DOUBLE)atof(str);
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			str.Format(_T("%d"), (INT)dPrice);
		}
		else	str = TruncPrice(m_pPrintDM, dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// UDF_변환종가격을 저장한다.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_종가격") )
	{
		//20070829 ADD PJW : 구입정보 변경에 따른 종가격을 구입정보 테이블에서 바로 받아온다.
		//BP_총권수 
		CString strSpeciesPrice;
		CString strBookCnt;
		CString strVolCnt;
		DOUBLE dSpeciesPrice;
		INT iBookCnt = 1;
		INT iVolCnt;

		strSpeciesPrice = m_pPrintDM->GetCellData(_T("BP_가격"), nRow);
		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		strVolCnt = m_pPrintDM->GetCellData(_T("BP_총권수"), nRow);
		iVolCnt = _ttoi( strVolCnt );
		if( iVolCnt == 1 )
		{
			strBookCnt = m_pPrintDM->GetCellData(_T("UDF_책수"), nRow);
			iBookCnt = _ttoi(strBookCnt);
			dSpeciesPrice*=iBookCnt;
			strSpeciesPrice.Format(_T("%.2f"), dSpeciesPrice);
			str = strSpeciesPrice;
			//=====================================================
			//2008.12.12 REM BY PJW : 책이 한권일때 이외에도 UDF_종가격 들어가야 한다.
// 			m_pPrintDM->SetCellData(_T("UDF_종가격"), strSpeciesPrice, nRow);
			//=====================================================
		}
	
		//

		str.TrimLeft();		str.TrimRight();
		// 2005-01-26
		//dPrice = (DOUBLE)atof(str.GetBuffer(0));
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		//20070512 pjw수정 원화일경우 소수점제외
		
		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			str.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, dPrice);
		}
		
		//str = TruncPrice(m_pPrintDM, dPrice);
		
		//=====================================================
		//2008.12.12 ADD BY PJW : 마지막에 종가격에 값이 잘못들어가 있어서 수정함 
// 		m_pPrintDM->SetCellData(_T("UDF_종가격"), strSpeciesPrice, nRow);
		m_pPrintDM->SetCellData(_T("UDF_종가격"), str, nRow);
		//=====================================================

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환종가격"), str, nRow);
	}
	//=======================================================================
	// 5. UDF_할인종가격과 UDF_변환할인종가격을 저장한다.
	//=======================================================================
	else if(strDMFieldAlias == _T("UDF_할인종가격"))
	{
		//20070829 ADD PJW : 구입정보 변경에 따른 발주가격을 구입정보 테이블에서 바로 받아온다.
		/*
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strBookCnt;
		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookCnt;
		
		strSpeciesPrice = m_pPrintDM->GetCellData(_T("BP_가격"), nRow);
		strPriceDiscountRate = m_pPrintDM->GetCellData(_T("BP_할인율"), nRow);
		strExchangeRate = m_pPrintDM->GetCellData(_T("BP_환율"), nRow);
		strBookCnt = m_pPrintDM->GetCellData(_T("UDF_책수"), nRow);
		dSpeciesPrice = atof(strSpeciesPrice);
		dExchangeRate = atof(strExchangeRate);
		dPriceDiscountRate = atof(strPriceDiscountRate);
		iBookCnt = _ttoi(strBookCnt);
		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 ) * iBookCnt;
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pPrintDM->SetCellData(_T("BP_발주가격"), strTmpData, nRow);
		*/

		
		//20070831 ADD PJW : 구입정보 변경에 따른 구입가격을 셋팅한다..
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strBookCnt;
		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookCnt;
		strSpeciesPrice = m_pPrintDM->GetCellData(_T("UDF_종가격"), nRow);
		strPriceDiscountRate = m_pPrintDM->GetCellData(_T("BP_할인율"), nRow);
		strExchangeRate = m_pPrintDM->GetCellData(_T("BP_환율"), nRow);
		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pPrintDM->SetCellData(_T("BP_발주가격"), strTmpData, nRow);

		//20070829 DELETE PJW : 새로운코드에 맞춰서 기존 코드 제외
		//strTmpData = m_pPrintDM->GetCellData(_T("BP_발주가격"), nRow);
		
		//
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);


		//20070512 pjw수정 원화일경우 소수점제외
		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.f"), dPrice);
			}
		}
		
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.f"), dPrice);
		}
		*/
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_할인종가격"), strTmpData, nRow);
		m_pPrintDM->SetCellData(_T("UDF_변환할인종가격"), str, nRow);
	}
	//=======================================================================
	// 보조등록구분 설명값, 학처명, 부서명을 설정한다.
	// 2005.11.02 강봉수 추가
	//=======================================================================
	//  육사용+++
	else if( strDMFieldAlias == _T("UDF_보조등록구분설명") )
	{
		strTmpData.Empty();		str.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BS_보조등록구분"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("보조등록구분"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if( strDMFieldAlias == _T("UDF_학처명") )
	{
		if( m_pPrintDM->GetCellData(_T("BP_신청구분"), nRow) == _T("DEP") )
		{
			strTmpData.Empty();		str.Empty();
			strTmpData = m_pPrintDM->GetCellData(_T("BP_학처명"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학부"), strTmpData, str);
			m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		}
		else	m_pPrintDM->SetCellData(strDMFieldAlias, _T(""), nRow);
	}
	// 육사용---
	// 국방대용+++
	else if( strDMFieldAlias == _T("UDF_부서명") )
	{
		if( m_pPrintDM->GetCellData(_T("BP_신청구분"), nRow) == _T("CLA") )
		{
			strTmpData.Empty();		str.Empty();
			strTmpData = m_pPrintDM->GetCellData(_T("BP_부서코드"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("직원부서"), strTmpData, str);
			m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		}
		else	m_pPrintDM->SetCellData(strDMFieldAlias, _T(""), nRow);
	}
	// 국방대용---
	//=======================================================================
	//2008.12.04 ADD BY LKS : 등록구분 구분자 " " 에서 "/"로 교체 (ex. EM JU  -> EM/JU)
	else if( strDMFieldAlias == _T("UDF_종등록구분") )
	{
		m_pPrintDM->GetCellData(_T("IBS_색인등록구분"),nRow,str);
		str.TrimLeft();str.TrimRight();
		str.Replace(_T(" "),_T("/"));
		m_pPrintDM->SetCellData(_T("UDF_종등록구분"),str,nRow);
	}
	//=====================================================
	
			
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if( nCurSel < 0 )							bSetTopRow = FALSE;
	if( m_pPrintDM->GetRowCount() == 0 )		bSetTopRow = FALSE;
	if( nCurSel > m_pPrintDM->GetRowCount() )	bSetTopRow = FALSE;

	m_pGrid->Display();
	if( bSetTopRow == TRUE )		m_pGrid->SetTopRow(nCurSel);
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


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::ShowDataPurchaseImport()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nRowCnt, nCurCnt, ids;
	// 자료반입 클래스
	CBO_ACQ_ImportMgr importDlg(this);



	nRowCnt = m_pPrintDM->GetRowCount();

	importDlg.SetAcqType(PURCHASE);

	//=======================================================================
	// 1. 차수번호를 멤버변수로 설정한다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strDivNo);

	//=======================================================================
	// 2. 그룹정보를 검색 클래스에서 가져와 반입할 차수정보를 확인한다.
	// 3. 차수정보가 없다면 차수번호리스트 창을 연다.
	// 4. 라이트 버젼이라면 차수정보 적용없이 적용안함으로 그냥 패스한다.
	//=======================================================================
	CGroupCollection* pGroups = m_pSearchDlg->GetGroupCollection();		
	if( pGroups->GetGroupCount() == 0 )
	{
		// 3.
		if( IDYES == ESLAfxMessageBox(_T("선정된 차수정보가 없습니다. 차수를 선정하시겠습니까?"), MB_YESNO) )
		{
			CDivisionNo_List_Dlg dlg(this);
			dlg.SetWorkStatus(_T("구입대상"));
			dlg.SetAcquisitYear(pGroups->GetAcqYear());
			dlg.SetGroupCollection(pGroups);
			dlg.DoModal();
		}
		// 4.
		else if (m_pInfo->GetKIIUPClientVersion() == 'L' )
		{
			pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("적용안함"), pGroups->GetWorkCode());
		}
		else return 0;
	}
	// 4.
	else if( m_pInfo->GetKIIUPClientVersion() == 'L' )
	{
		pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("적용안함"), pGroups->GetWorkCode());
	}
	

	//=======================================================================
	// 5. 그룹이 없다면 리턴 한다.
	//=======================================================================
	if( pGroups->IsEmpty() == TRUE )	return 0;


	//=======================================================================
	// 6. 반입클래스에 그룹정보/DM/Grid를 설정한 후에 화면을 띄운다.
	//=======================================================================
	importDlg.SetGroups(m_pSearchDlg->GetGroupCollection());
	importDlg.SetParentDM(m_pPrintDM);
	importDlg.SetParentBookDM(m_pDM);
	importDlg.SetParentGrid(m_pGrid);
	importDlg.DoModal();


	//=======================================================================
	// 7. 라이트 버젼이라면 현재 적용된 그룹을 지운다.
	//=======================================================================
	if( m_pInfo->GetKIIUPClientVersion() == 'L' )
	{
		CGroupInfo Group;
		Group.SetAcqYear(pGroups->GetAcqYear());
		Group.SetGroupInfo(_T("적용안함"));
		Group.SetWorkCode(pGroups->GetWorkCode());
		ids = pGroups->DeleteThisGroupInfo(&Group);
		if( ids < 0 )	return -1;
	}
	
	//=======================================================================
	// 8. 반입된 자료에 대한 종수 및 책수를 메인 화면에 적용 한다.
	//=======================================================================
	SetPurchaseDataInfo();

	//=======================================================================
	// 9. 반입된 자료가 있다면 선정비선정을 _T("Y")로 셋팅하고 그리드를 다시 그린다.
	//=======================================================================
	nCurCnt = m_pPrintDM->GetRowCount();	
	if( nCurCnt > nRowCnt )
	{
		// 반입된 책 선정처리 _T("Y") 적용
		// 2005-11-14 강봉수 추가
		//			
		for(INT i=nRowCnt ; i<nCurCnt ; i++)
		{
				m_pPrintDM->SetCellData(_T("BP_선정비선정"), _T("Y"), i );
		}
		AllControlDisplay(_T("CM_구입자료관리"));			
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt+1);
	}
	return 0;
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

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM에 값이 있는지 체크하고, 그리드에 선정된 자료를 체크하여
//	가장 위에있는 Row를 선택한다.
//
// [ 미니스펙 ]
// 1. 출력용 DM에 값이 있는지 확인한다.
// 2. 그리드에 선정 체크를 설정한다.
// 3. 그리드에 선정 체크된 Row중 가장 위에 있는 것을 선택한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::CheckSelectDataValid()
{
	INT ids = 0;

	//=======================================================================
	// 1. 출력용 DM에 값이 있는지 확인한다.
	//=======================================================================
	if(m_pPrintDM->GetRowCount() == 0) return -1;


	//=======================================================================
	// 2. 그리드에 선정 체크를 설정한다.
	//=======================================================================
	ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;


	//=======================================================================
	// 3. 그리드에 선정 체크된 Row중 가장 위에 있는 것을 선택한다.
	//=======================================================================
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

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 출력물에 대한 DM을 구성하도록 해당함수를 호출한다.
// - PrintPurchaseData() : 일반리스트 출력
// - PrintPurchaseSubTotalBookData() : 책소계출력
// - PrintPurchaseSubTotalData() : 소계출력
//
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_PurchaseDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	// 2005-01-29 5163용 추가.
	switch( lParam )
	{
	// 구입대상자료목록, 구입대상자료목록(신청부서포함)
	case  BO_ACQ_PURCHASE_DATA:
	case BO_ACQ_PURCHASE_DATA_5163:		return PrintPurchaseData(lParam);
		break;
	// 구입대상자료목록(등록구분별)
	case BO_ACQ_PURCHASE_DATA_REGCODE:	return PrintPurchaseSubTotalBookData(lParam);
		break;	
	default:							return PrintPurchaseSubTotalData(lParam, wParam);
		break;
	}
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalData(INT nPrintType, INT nExtraInfo)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids = -1;
	BOOL bExtraInfo = FALSE;
	CString strDivAlias = _T("BP_수서정보KEY");
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);
	
	
	//=======================================================================
	// 1. 인자를 비교하여 KOALS2형식의 출력물인지 판단한다.
	//=======================================================================
	if(nExtraInfo == 1000) bExtraInfo = TRUE;
	
	//=======================================================================
	// 2. _T("그리드선정자료") 출력이라면 그리드에 선정체크된 자료가 있는지 먼저 확인한다.
	//=======================================================================
	if( m_pPrintDlg->GetPrintDataType() == 2 )
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			return -1;
		}
	}

	//=======================================================================
	// 3. Progress Ctrl 창을 생성한다.
	//=======================================================================
	ShowDataProcessingDlg();
	
	//=======================================================================
	// 4. CMakePrintDMMgr 클래스에 값들을 설정한다.
	//=======================================================================
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("프린트타입"), nPrintType);
	PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
	PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_가격"));
/*	]
	if(nPrintType == BO_ACQ_ORDER_DATA_MEDIACODE)   //2005-01-31
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("BP_발주가격"));
	else
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
*/
	PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
	PrintMgr.SetTempData(_T("책DM알리아스"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	PrintMgr.SetTempData(_T("KOLAS2형식"), bExtraInfo);
	PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("그룹컬렉션"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pSubTotalPrintDM);
	PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
	
	//=======================================================================
	// 5. 프린트 타입에 따라서 정렬필드를 추가 설정한다.
	//=======================================================================
	switch( nPrintType )
	{
	case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
	case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
	case BO_ACQ_ORDER_DATA_DIVNO:
		{
			PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_그룹번호"));
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER :
	case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER :
	case BO_ACQ_ORDER_DATA_PROVIDER :
		{
		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_구입처부호"));
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE :
	case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE :
	case BO_ACQ_ORDER_DATA_SUBJECTCODE :
		{
		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_주제구분"));
		}
		break;

	// 2005-01-31 5163용 매체구분별 주문목록 추가.
	case BO_ACQ_ORDER_DATA_MEDIACODE :
		{
		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BB_매체구분"));
		}
		break;

	// 2005-10-19 육사용 학처별 추가
	case BO_ACQ_PURCHASE_DATA_DEPT:
		{
			PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_학처명"));
		}
		break;

	// 2005-10-19 육사용 도서분류코드1별 추가
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:	
		{
			PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BS_사용자정의코드1"));
		}
		break;

	default:
	{
		ESLAfxMessageBox(_T("출력물 형식을 지정할 수 없습니다."));
		DeleteDataProcessingDlg();
		return -1;
	}
		break;
	}
	

	//=======================================================================
	// 6. CMakePrintDMMgr의 _T("소계자료출력")을 실행한다.
	//=======================================================================
	ids = PrintMgr.SPFExecutePath(_T("소계자료출력"));


	//=======================================================================
	// 7. 생성된 Progress Ctrl을 지운다.
	//=======================================================================
	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	else if( ids == 1000 )		return 0;
	
	
	//=======================================================================
	// 8. DM_주문자료관리_출력을 출력용 DM으로 설정하고 SIReport로 출력한다.
	//=======================================================================
	m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
	m_pPrintDlg->Print();

	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalBookData(INT nPrintType)
{
	INT ids = -1;
	CESL_DataMgr PrintBookDM;
	PrintBookDM.InitDMPKFromFile(m_pDM->DB_MGR_ALIAS);
	PrintBookDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	PrintBookDM.SetMarcMgr(m_pInfo->pMarcMgr);

	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CString strDivAlias = _T("BP_수서정보KEY");
	CMakePrintDMMgr PrintMgr(this);
	if(m_pPrintDlg->GetPrintDataType() == 2)
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			return -1;
		}
	}
	ShowDataProcessingDlg();

	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("프린트타입"), nPrintType);
	PrintMgr.SetTempData(_T("구분알리아스"), strDivAlias.GetBuffer(0));
	PrintMgr.SetTempData(_T("가격알리아스"), _T("BB_가격"));
	PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
	
	// nPrintType에 따른 자료구분별 출력 추가 (64)
	// 2005-09-05 강봉수
	//
	switch(nPrintType)
	{
	case BO_ACQ_PURCHASE_DATA_REGCODE:	PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BB_등록구분"));
		break;

	default:
		{
			ESLAfxMessageBox(_T(",정렬 조건을 알 수 없습니다.\n등록구분으로 정렬 합니다."));
			PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BB_등록구분"));
		}
		break;
	}
	//
	// 이상 끝.

	PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("그룹컬렉션"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)&PrintBookDM);
	PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("책소계자료출력"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	if(ids == 1000)
		return 0;
	
	m_pPrintDlg->SetPrintDM(&PrintBookDM);
	m_pPrintDlg->Print();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 구입대상자료목록, 구입대상자료목록(신청부서포함)
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseData(INT nPrintType)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids = -1;
	CString strDivAlias;
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);

	//=======================================================================
	// 1. 구분알리아스를 수서정보키(ACQ_KEY)로 설정한다.
	//=======================================================================
	strDivAlias = _T("BP_수서정보KEY");

	//=======================================================================
	// 2. 그리드 선정자료 출력이라면 선정된 자료만 DM을 구축한다.
	//=======================================================================
	if( m_pPrintDlg->GetPrintDataType() == 2 )
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			return -1;
		}
	}

	//=======================================================================
	// 3. Progress Ctrl 생성
	//=======================================================================
	ShowDataProcessingDlg();
	
	//=======================================================================
	// 4. 프린트DM생성 클래스에 필요한값들을 설정한다. - 일반리스트출력
	//=======================================================================
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("프린트타입"), nPrintType);
	PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
	PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_종가격"));
	PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인종가격"));
	PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("원본출력DM"), (DWORD*)m_pPrintDM);
	PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pPrintObjDM);
	PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("일반리스트출력"));

	//=======================================================================
	// 5. Progress Ctrl 소멸
	//=======================================================================
	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	else if(ids == 1000)		return 0;
	
	//=======================================================================
	// 6. 생성된 DM을 SIReport로 출력한다.
	//=======================================================================
	m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
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
	if(m_pInfo->GetKIIUPClientVersion() != 'L')
	{
		RemoveListArray.Add(_T("차수변경"));
	}
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	for(INT i=0 ; i<RemoveListArray.GetSize() ; i++)
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&RemoveListArray[i], TRUE);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -601 /* DblClick */, OnDblClickgridPURCHASEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -607 /* MouseUp */, OnMouseUpgridPURCHASEDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
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
//JOB.2019.0073 : 충돌 License 제거
// 	COrderMailMgr mailMgr(this);
// 	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();
// 	
// 	mailMgr.SetSelectSpeciesKey(m_strSelectSpeciesKey);
// 	mailMgr.SetParentGroupCollection(pGroupCollection);
// 	mailMgr.SetOperationType(nOpType);
// 	mailMgr.SetOpenType(nOpenType);
// 	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
// 	mailMgr.DoModal();
}

INT CBO_ACQ_PurchaseDataMgr::MakePrintDM()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return -1;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return -1;
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
		return -1;
	}
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::MakeOrderPrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return -1;
	}
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
	if(m_pDataProcessingDlg == NULL) 
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
	}

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

VOID CBO_ACQ_PurchaseDataMgr::GiveOrderSerialNo()
{
	//BOOL bGridSelectCol ;

	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}

	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return;
	}
	
	//선정/비선정 체크
	INT nIdx;
	INT ids = -1;
	INT nSelCount = 0;
	ids = m_pGrid->SelectMakeList();
	if ( ids < 0 ) return;

	nIdx = m_pGrid->SelectGetHeadPosition();
	if ( nIdx < 0 ) 
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	while ( nIdx > -1 )
	{
		if ( m_pPrintDM->GetCellData( _T("BP_선정비선정"), nIdx ) != _T("Y") ) 
		{
			nSelCount++;
		}
		//===========================================================
		//20080625 ADD BY PJW : 그리드선정을 DM에 반영한다

		CString strChk;
		CString strSpeciesKey;
		strChk = _T("V");
		m_pPrintDM->SetCellData( _T("선정"), strChk, nIdx );
		m_pPrintDM->GetCellData( _T("IBS_종KEY"), nIdx, strSpeciesKey);
		SelectGridToDmChk(m_pDM,strSpeciesKey,strChk);

		//===========================================================

		nIdx = m_pGrid->SelectGetNext();
	}

	if ( nSelCount > 0 )
	{
		CString strMessage;
		strMessage.Format( _T("선택된 자료중 선정처리되지 않은 자료가 %d건 있습니다. \r\n주문처리를 하기전에 선정처리를 수행하여 주십시요"), nSelCount );
		AfxMessageBox ( strMessage );
		return;
	}

	//발주일련번호 부여 루틴
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("SpeciesDM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	ids = api.SPFExecutePath(_T("발주일련번호부여"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("주문일련번호를 부여할 수 없습니다."));
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("차수정보가 부여되지 않은 [%d]건의 자료에 대하여는 주문일련번호를 부여하지 않았습니다.\n")
			_T("해당 자료는 차수정보부여후 주문일련번호를 부여할 수 있습니다."), ids);
		ESLAfxMessageBox(strMsg);
	}
	//=============================
	//20080.07.01 REM BY PJW : 주문일련번호 부여 메세지를 보여주지 않는다.
//	else
//		ESLAfxMessageBox(_T("주문일련번호를 부여 하였습니다."));
	//=============================

	//bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	DisplayGrid();
	m_pGrid->m_bProtectSelectColumn = FALSE;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_PurchaseDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_PurchaseDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("IBS_업무구분"));
	pTmpData->Add(_T("BP_그룹번호"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_작업번호"));
	pTmpData->Add(_T("BP_일련번호"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	pMap->SetAt(_T("UDF_구입처부호설명"), _T("BP_구입처부호"));
	pMap->SetAt(_T("UDF_종가격"), _T("UDF_변환종가격"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_PurchaseDataMgr::ChangeGroupInfo()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetWorkStatus(_T("구입대상"));
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * pTargetGroup = dlg.GetSelGroup();

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)pTargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	ids = api.SPFExecutePath(_T("그룹정보변경"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("차수정보를 변경할 수 없습니다."));
		return ;
	}
	ESLAfxMessageBox(_T("선정하신 자료에 대하여 차수정보를 변경하였습니다."));
	
	if(pTargetGroup && !m_pSearchDlg->GetGroupCollection()->IsHaveGroupInfo(pTargetGroup))
		m_pSearchDlg->GetGroupCollection()->AddGroupInfo(pTargetGroup);
	SetPurchaseDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}

CString CBO_ACQ_PurchaseDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 가격정책에 따라 절사단위설정값을 읽어와서 지정된 자릿수만큼 표기하도록 한다.
	INT nPos = -1;
	CString strTmpData, strTmp, strValue;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if		( m_strPosNumber == _T("1") )  	strTmpData = strTmp.Left(nPos+2);
	else if	( m_strPosNumber == _T("2") )	strTmpData = strTmp.Left(nPos+3);
	else									strTmpData = strTmp.Left(nPos);		// m_strPosNumber == _T("0") || m_strPosNumber.Empty()

	return strTmpData;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowFileExport()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return ;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return ;
    sFileName = dlg.GetPathName();	

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일을 선택하세요."));
		return ;
	}

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	CString sMarc;
	CMarc marc;
	INT idx;
	idx = m_pGrid->SelectGetHeadPosition();
	while ( idx >= 0 ) {
		sMarc = m_pPrintDM->GetCellData( _T("BS_MARC"), idx );

		m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
		m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

		sMarc += _T("\r\n");
		_ftprintf( fp, sMarc );

		idx = m_pGrid->SelectGetNext();
	}

	fclose( fp );

	INT nTotCount = m_pGrid->SelectGetCount();
	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), nTotCount );
	AfxMessageBox( sMessage );

}

VOID CBO_ACQ_PurchaseDataMgr::ShowFileImport()
{
	INT nNextRowCnt;

	INT nRowCnt = m_pPrintDM->GetRowCount();
	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("수서구입"), _T("BOA111N"));
	dlg.SetOpenMode(4000);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	//========================================================
	//2008.07.15 ADD BY PJW : 반입시 추가된 차수를  GroupInfo에 추가한다
	//2008.07.25 UPDATE BY PJW : 기존에 차수에 추가 반입될경우는 ACQNO를 추가하지 않는다.
	nNextRowCnt = m_pPrintDM->GetRowCount();
	if( nRowCnt < nNextRowCnt )
	{
		INT nIdx;
		
		CString strImportAcqNo;
		CString strImportAcqYear;	

		CString strWhere;
		CString strSpeciesKey;
		CString strSpeciesKeys;
		CString strAcqNo;
		INT nCount;
		INT nPos;
		
		strImportAcqNo = dlg.m_strExtGruopInfo;
		strImportAcqYear = dlg.m_strExtAcqYear;

		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtPURCHASE_DIVNO);
		pEdit->GetWindowText(strAcqNo);
		nPos = strAcqNo.Find(strImportAcqNo);
		if( nPos < 0 )
		{
			m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strImportAcqYear, strImportAcqNo, _T("1"));
		}
	}
	//========================================================

	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		//========================================================
		//2008.07.15 ADD BY PJW : 반입받으면 주문번호부여 선정되도록 수정
		for(INT i = nRowCnt ; i<nCurCnt ; i++)
		{
		m_pPrintDM->SetCellData(_T("BP_선정비선정"), _T("Y"), i );
		}
		AllControlDisplay(_T("CM_구입자료관리"));
		//========================================================
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt + 1);
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ShowACQFileExport()
{
	INT ids;
	CString strAcqYear, strGroupInfo, strWorkCode, strQuery, strResult;
	CDataExportDlg Dlg(this);
	ids = Dlg.DoModal();
	if( ids == IDCANCEL ) return ;

	strAcqYear = Dlg.GetAcqYear();
	strGroupInfo = Dlg.GetSeqNo();
	strWorkCode = Dlg.GetWorkCode();

	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strWorkCode.IsEmpty())
	{
		AfxMessageBox(_T("반출 조건을 모두 입력하셔야 합니다."));
		return ;
	}
	
	strQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
								strAcqYear, strGroupInfo, strWorkCode);
	strResult.Empty();
	ids = m_pDM->GetOneValueQuery(strQuery, strResult);

	if( ids <0 || strResult.IsEmpty()) 
	{
		AfxMessageBox(_T("입력조건에 맞는 차수가 존재하지 않습니다."));
		return ;
	}

	CBO_ACQ_FileExportMgr ExportMgr;
	
	ExportMgr.SetAcqYear(strAcqYear);
	ExportMgr.SetGroupInfo(strGroupInfo);
	ExportMgr.SetWorkCode(strWorkCode);
	ExportMgr.SetSpeciesType(BO_ORDER);		//종타입
	
	// 파일을 반출한다.
	ids = ExportMgr.FileExportProc();

	if(ids == 0)
		AfxMessageBox(_T("파일 반출을 완료하였습니다."));

}

VOID CBO_ACQ_PurchaseDataMgr::ShowACQFileImport()
{
	CBO_ACQ_FileImportMgr  ImportMgr(this);

	ImportMgr.SetSpeciesType(BO_ORDER);		//종타입
	// 파일을 반입한다.
	INT ids = ImportMgr.FileImportProc();
	if(ids < 0) return ;

	if(ids ==0)
		AfxMessageBox(_T("파일 반입을 완료하였습니다."));	
	else return ;
}


VOID CBO_ACQ_PurchaseDataMgr::ProcSelect()
{
	//선정처리
	INT nIdx;
	INT ids = -1;
	BOOL bGridSelectCol ;
	if ( AfxMessageBox (_T("선택된 자료에 대해 선정처리를 하시겠습니까?") , MB_YESNO ) ==  IDYES )
	{
		ids = m_pGrid->SelectMakeList();
		if ( ids < 0 ) return;

		nIdx = m_pGrid->SelectGetHeadPosition();
		while ( nIdx > -1 )
		{
			CString strPReckey;
			CString strQuery;
			strPReckey = m_pPrintDM->GetCellData ( _T("BP_수서정보KEY") , nIdx );
			if ( ids < 0 ) return;

			ids = m_pPrintDM->SetCellData ( _T("BP_선정비선정"), _T("Y"), nIdx ) ;
			if ( ids < 0 ) return;

			strQuery.Format ( _T("UPDATE BO_PURCHASEINFO_TBL SET SEL_YN = 'Y' WHERE REC_KEY = '%s'; ") , strPReckey );
			ids = m_pPrintDM->ExecuteQuery( strQuery, 1 );
			if ( ids < 0 ) return;
		
			nIdx = m_pGrid->SelectGetNext();
		}
	}
	else
		AfxMessageBox ( _T("취소되었습니다."));

	bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	AllControlDisplay(_T("CM_구입자료관리"));
	m_pGrid->m_bProtectSelectColumn = bGridSelectCol;
}

VOID CBO_ACQ_PurchaseDataMgr::ProcUnSelect()
{
	//비선정처리
	INT nIdx;
	INT ids = -1;
	BOOL bGridSelectCol;
	if ( AfxMessageBox (_T("선택된 자료에 대해 비선정처리를 하시겠습니까?") , MB_YESNO ) ==  IDYES )
	{
		ids = m_pGrid->SelectMakeList();
		if ( ids < 0 ) return;

		nIdx = m_pGrid->SelectGetHeadPosition();
		while ( nIdx > -1 )
		{
			CString strPReckey;
			CString strQuery;
			strPReckey = m_pPrintDM->GetCellData ( _T("BP_수서정보KEY") , nIdx );
			if ( ids < 0 ) return;

			ids = m_pPrintDM->SetCellData ( _T("BP_선정비선정"), _T("N"), nIdx ) ;
			if ( ids < 0 ) return;

			strQuery.Format ( _T("UPDATE BO_PURCHASEINFO_TBL SET SEL_YN = 'N' WHERE REC_KEY = '%s'; ") , strPReckey );
			ids = m_pPrintDM->ExecuteQuery( strQuery, 1 );
			if ( ids < 0 ) return;
		
			nIdx = m_pGrid->SelectGetNext();
		}
	}
	else
		AfxMessageBox ( _T("취소되었습니다."));
	
	bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	AllControlDisplay(_T("CM_구입자료관리"));
	m_pGrid->m_bProtectSelectColumn = bGridSelectCol;
}


VOID CBO_ACQ_PurchaseDataMgr::ChangeMarc()
{
	//===========================================
	//2008.07.16 ADD BY PJW : 선정한자료가 없을경우 마크교체 다이얼로그를 띄우지 않는다.
	INT nSelectCnt;
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 1 > nSelectCnt ) 
	{
		AfxMessageBox(_T("마크교체 할 자료를 선정해 주세요."));
		return;
	}
	
	//===========================================
	CMARC_CHANGER_01 dlg(this);
	//===============================================
	//2008.07.02 UPDATE BY PJW : 대표책의 데이터도 변경한다.
	//dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_구입자료관리"), _T("MainGrid"), _T("DM_구입자료관리_종"), _T("BS_MARC"));
	dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_구입자료관리"), _T("MainGrid"), _T("DM_구입자료관리_종"), _T("BS_MARC"), _T("DM_구입자료관리"));
	//===============================================
	
	//===============================================
	//2008.08.22 UPDATE BY PJW : 선정값을유지하고 그리드를 디스플레이한다.
// 	if ( dlg.DoModal() == IDOK )
//	{
//		AllControlDisplay(_T("CM_구입자료관리"));
//		m_pDM->RefreshPositionInfo();		
//	}
	m_pGrid->SetProtectSelectColumn(TRUE);
	if ( dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_구입자료관리"));
		m_pDM->RefreshPositionInfo();		
	}
	m_pGrid->Display();
	m_pGrid->SetProtectSelectColumn(FALSE);
	//===============================================
}

HBRUSH CBO_ACQ_PurchaseDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
//===================================
//2008.07.02 ADD BY PJW : 선택된자료의 종키를 얻는다.
//CString CBO_ACQ_PurchaseDataMgr::GetSelectSpeciesKey()
//{
//	
//	INT nSelect;
//	CString strSpeciesDmKey;
//	CString strTemp;
//	CString strSelectSpeciesKey;
//	INT nSelectCnt;
//	INT nDmCnt;
//	m_pGrid->SelectMakeList();
//	nSelectCnt = m_pGrid->SelectGetCount();
//	nSelect = m_pGrid->SelectGetHeadPosition();
//	strSelectSpeciesKey = _T("");
//	for( INT i = 0 ; i < nSelectCnt ; i++ )
//	{
//		m_pPrintDM->GetCellData(_T("IBS_종KEY"),nSelect,strSpeciesDmKey);
//		
//		if( strSelectSpeciesKey.IsEmpty() )
//		{
//			strSelectSpeciesKey.Format(_T("%s"),strSpeciesDmKey);
//		}
//		else
//		{
//			strTemp = strSelectSpeciesKey;
//			strSelectSpeciesKey.Format(_T("%s,%s"), strTemp, strSpeciesDmKey);
//		}
//		nSelect = m_pGrid->SelectGetNext();
//	}
//	return strSelectBookKey; 
//}
//===================================
//===================================
//20080603 ADD BY PJW : 선택된자료의 책키,종키를 얻는다.
CString CBO_ACQ_PurchaseDataMgr::GetSelectBookKey()
{
	
	INT nSelect;
	CString strSpeciesDmKey;
	CString strBookDmKey;
	CString strTemp;
	CString strSelectBookKey;
	CString strSelectSpeciesKey;
	INT nSelectCnt;
	INT nDmCnt;
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	nSelect = m_pGrid->SelectGetHeadPosition();
	nDmCnt = m_pDM->GetRowCount();
	strSelectBookKey = _T("");
	strSelectSpeciesKey = _T("");
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		

		m_pPrintDM->GetCellData(_T("IBS_종KEY"),nSelect,strSpeciesDmKey);
		if( strSelectSpeciesKey.IsEmpty() )
		{
			strSelectSpeciesKey.Format(_T("%s"),strSpeciesDmKey);
		}
		else
		{
			strTemp = strSelectSpeciesKey;
			strSelectSpeciesKey.Format(_T("%s,%s"), strTemp, strSpeciesDmKey);
		}

		for( INT j = 0 ; j < nDmCnt ; j++ )
		{
			m_pDM->GetCellData(_T("IBS_종KEY"),j,strBookDmKey);
			if( strSpeciesDmKey == strBookDmKey )
			{
				m_pDM->GetCellData(_T("BB_책KEY"),j,strBookDmKey);
				if( strSelectBookKey.IsEmpty() )
				{
					strSelectBookKey.Format(_T("%s"),strBookDmKey);
				}
				else
				{
					strTemp = strSelectBookKey;
					strSelectBookKey.Format(_T("%s,%s"), strTemp, strBookDmKey);
				}
			}
		}
		
		nSelect = m_pGrid->SelectGetNext();
	}
	m_strSelectSpeciesKey = strSelectSpeciesKey;
	return strSelectBookKey; 
}

//========================================================================
//20080605 ADD BY PJW : 마우스 UP시 선정값으로 SelectGridToDmChk()호출
void CBO_ACQ_PurchaseDataMgr::OnMouseUpgridPURCHASEDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
//	CString strBookKey;
//	CMSHFlexGrid *pGrid;
	

//  pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridPURCHASE_DATA );
//	nSelectRow	= pGrid->GetRow();
//	nSelectCol	= pGrid->GetCol();
//	strChk		= pGrid->GetText();
	//m_pGrid
	//pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridPURCHASE_DATA );
	nSelectRow	= m_pGrid->GetRow();
	nSelectCol	= m_pGrid->GetCol();
	strChk		= m_pGrid->GetText();
	
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;
	if ( 1 == Button && 1 == nSelectCol ) 
	{
		m_pPrintDM->SetCellData( _T("선정"), strChk, nSelectRow );
		m_pPrintDM->GetCellData( _T("IBS_종KEY"), nSelectRow, strSpeciesKey);
		SelectGridToDmChk(m_pDM,strSpeciesKey,strChk);
	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : 그리드에 따라서 DM의 선정 값을 변경한다.
INT CBO_ACQ_PurchaseDataMgr::SelectGridToDmChk(	CESL_DataMgr* pDM		, 
												 CString strSpeciesKey	, 
												 CString strChk	
												 )
{
	CString strTempKey;
	INT i;

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("IBS_종KEY"), i, strTempKey);
		if( strSpeciesKey == strTempKey )
		{
			pDM->SetCellData( _T("선정"), strChk, i );
		}
	}
	return 0;
}
//========================================================================


VOID CBO_ACQ_PurchaseDataMgr::KSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{	
	CString strQuery;
	
	
	BEGIN_SBL();
	strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN('0', '1') AND B.WORKING_STATUS = 'BOA111N' AND B.ACQ_CODE = '%s' AND P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO IN (%s)")
		, strAcqCode, strAcqYear, strWorkCode, strSeqNo );
	m_pDM->RefreshDataManager(strQuery);
	END_SBL(m_pDM->GetRowCount());

	m_pPrintDM->FreeData();
	ShowDataProcessingDlg();
	if( 0 != m_pDM->GetRowCount() )
	{
		CBO_ACQ_Business_Api mgr(this);
		mgr.InitTempData();
		mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	
		INT ids = mgr.SPFExecutePath(_T("책To수서정보"));
		DeleteDataProcessingDlg();
		if( ids < 0 )	return;
	
		m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
		SetPurchaseDataInfo();
		//SortByGroupInfo();	
		DisplayGrid();
	}
	DeleteDataProcessingDlg();

	m_pCM->AllControlDisplay(-1);
	m_pCM->SetControlMgrData(_T("그룹번호"), strSeqNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);

	m_pSearchDlg->GetGroupCollection()->RemoveAll();
	m_pSearchDlg->GetGroupCollection()->SetAcqCode(strAcqCode);	
	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strAcqYear, strSeqNo, strWorkCode);	
	m_pSearchDlg->GetGroupCollection()->SetHaveGroupInfo(TRUE);
	
	//m_pSearchDlg->GetGroupCollection()->m_bAllGroupInfo = FALSE;

	//m_pPrintDM
	
}