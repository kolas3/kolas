// BO_ACQ_RegObjDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RegObjDataMgr.h"
#include "CheckInCancelDlg.h"
#include "CheckInCancelDivnoSelectDlg.h"
#include "../BO_ACQ_STATISTICS/BO_Purchase_Statistics.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_Mgr.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../공통/BO_ACQ_COMMON/PriceSumDlg.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RegObjDataMgr dialog


CBO_ACQ_RegObjDataMgr::CBO_ACQ_RegObjDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_RegObjDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RegObjDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pPrintVolumeDM = NULL;
	m_pSearchDlg = NULL;
	m_strDivNo	= _T("");
	m_strIs5163 = _T("");
	m_strIs64	= _T("");
	m_strPosNumber = _T("");
	m_strOrderPrice_Unit = _T("0");
	m_nAcqType = ACQ_TYPE_INVALID;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet= NULL;
	m_nRow = -1;
}

CBO_ACQ_RegObjDataMgr::~CBO_ACQ_RegObjDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
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

	m_Dlg_brush.DeleteObject();
}


VOID CBO_ACQ_RegObjDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RegObjDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RegObjDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RegObjDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RegObjDataMgr message handlers

VOID CBO_ACQ_RegObjDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
}

BOOL CBO_ACQ_RegObjDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	if (InitESL_Mgr(_T("K2UPGRADE_구입등록대상자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_구입등록대상자료관리"));
	m_pPrintObjDM = FindDM(_T("DM_구입등록대상자료관리_출력용"));
	m_pPrintDM = FindDM(_T("DM_구입등록대상자료관리_권"));
	m_pPrintVolumeDM = FindDM(_T("DM_구입등록대상자료관리_권_출력용"));
	m_pCM = FindCM(_T("CM_구입등록대상자료관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
	if( GetManageValue(_T("수서"), _T("단행"), _T("구입가격단위설정"), _T(""), m_strOrderPrice_Unit) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}

	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), m_strIs64);
	
	GetDlgItem(IDC_stcREGOBJ_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtREGOBJ_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcREGOBJ_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtREGOBJ_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_REGOBJ, PURCHASE, BO_REGOBJ, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_REGOBJ, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	SetRegObjDataInfo();
	
	InitPopupMenuItem();

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_RegObjDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_REGOBJ_DATA);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcREGOBJ_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtREGOBJ_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcREGOBJ_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(150, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtREGOBJ_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(205, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridREGOBJ_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);	
	pGrid->MoveWindow(-2, 35, cx+2, cy);

}

UINT CBO_ACQ_RegObjDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입 등록대상처리자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetRegObjDataInfo();
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
	SetRegObjDataInfo();
	//SortByGroupInfo();
	DisplayGrid();
	
	return 0;
}

VOID CBO_ACQ_RegObjDataMgr::SetRegObjDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d권 %d책의 구입 등록대상 자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_RegObjDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode, strOrderSerialNo;
	INT nGroupNo, nSerialNo, nWorkNo, nOrderSerialNo;
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
	else if(strDMFieldAlias == _T("UDF_할인가"))
	{
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("UDF_발주가격_권"), nRow);
		strTmpData.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpData);
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pPrintDM->SetCellData(_T("UDF_변환할인가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_가격"))
	{
		CString strTmpData;
		//INT nBookCnt = 0;
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("BB_가격"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//strTmpData = m_pPrintDM->GetCellData(_T("UDF_책수"), nRow);
		//nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		//strTmpData.Format(_T("%10.0f"), dPrice*nBookCnt);
		strTmpData = TruncPrice(dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_RegObjDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_RegObjDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_RegObjDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_RegObjDataMgr::ShowRegDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_REGOBJ);
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
	dlg.SetParentType(VOLUME);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetRegObjDataInfo();
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

VOID CBO_ACQ_RegObjDataMgr::DeleteRegObjData()
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
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REGOBJ);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("삭제타입"), (INT)VOLUME_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환할인가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	ids = api.SPFExecutePath(_T("삭제"));
	if(ids == 0)
	{
		SetRegObjDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_RegObjDataMgr::ShowChangeCodeDlg()
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

BOOL CBO_ACQ_RegObjDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_RegObjDataMgr::RegTransfer(CString strPath)
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("처리단위"), _T("권"));
	api.SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	api.SetTempData(_T("수서정보키알리아스"), _T("BP_수서정보KEY"));
	api.SetTempData(_T("권일련번호알리아스"), _T("BB_권일련번호"));
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	ids = api.SPFExecutePath(strPath);
	CString strMsg;
	if(ids < 0)
	{
		strMsg.Format(_T("선정한 자료를 %s 처리할 수 없습니다."), strPath);
		ESLAfxMessageBox(strMsg);
		return;
	}
	strMsg.Format(_T("선정한 자료를 %s 처리 하였습니다."), strPath);
	ESLAfxMessageBox(strMsg);
	DisplayGrid();	
}

INT CBO_ACQ_RegObjDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_RegObjDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_RegObjDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_RegObjDataMgr)
	ON_EVENT(CBO_ACQ_RegObjDataMgr, IDC_gridREGOBJ_DATA, -601 /* DblClick */, OnDblClickgridREGOBJDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_RegObjDataMgr::OnDblClickgridREGOBJDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	ShowRegDataDlg(GUPDATE);
}

VOID CBO_ACQ_RegObjDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("검수완료"));
	mgr.DoModal();
}

UINT CBO_ACQ_RegObjDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids;
	if(lParam == BO_ACQ_PURCHASE_REGOBJ_DATA)
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
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pPrintVolumeDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("권리스트출력"));
		DeleteDataProcessingDlg();
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("출력데이터에 오류가 있습니다."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintVolumeDM);
		m_pPrintDlg->Print();
	}
	else if(lParam == BO_ACQ_CHECKIN_COMPLETE_DATA)
	{
		ShowDataProcessingDlg();
		CMakePrintDMMgr PrintMgr(this);
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_종가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인종가격"));
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("원본출력DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		INT ids = PrintMgr.SPFExecutePath(_T("검수완료자료출력"));
		if(ids < 0) return 0;
		DeleteDataProcessingDlg();
		m_pPrintDlg->SetIsSpecies(TRUE);
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();
	}
	else if(lParam == BO_ACQ_CHECKIN_STATISTICS)
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("검수완료"));
		dlg.SetSpeciesType(BO_REGOBJ);
		dlg.DoModal();		
	}
	return 0;
}

VOID CBO_ACQ_RegObjDataMgr::ShowDataProcessingDlg()
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
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_RegObjDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

CString CBO_ACQ_RegObjDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery;
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	m_pDM->GetOneValueQuery(strQuery, strValue);
	return strValue;
}

VOID CBO_ACQ_RegObjDataMgr::CheckInCancel()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}

	CCheckInCancelDlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return;
	
	if(dlg.m_nCheckInCancelType == 0)
	{
		CheckInCancelByDivno();	
	}
	else if(dlg.m_nCheckInCancelType == 1)
	{
		CheckInCancelBySelData();
	}
}

INT CBO_ACQ_RegObjDataMgr::CheckInCancelByDivno()
{
	CGroupCollection * pGroupCollection = NULL;
	CCheckInCancelDivnoSelectDlg dlg(this);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.DoModal();
	pGroupCollection = dlg.GetOrderCancelGroupCollection();
	if(pGroupCollection->GetGroupCount() == 0) return 0;

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)NULL);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)pGroupCollection);
	INT ids = api.SPFExecutePath(_T("그룹자료삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검수취소자료를 삭제할 수 없습니다."));
		return -1;
	}
	SetRegObjDataInfo();
	DisplayGrid();
	return 0;
}

INT CBO_ACQ_RegObjDataMgr::CheckInCancelBySelData()
{
	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	
	if(IDNO == ESLAfxMessageBox(_T("선정하신 자료를 미납처리 하시겠습니까?"), MB_YESNO))
		return 0;

	CStringArray arrAcqKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("PRINT_DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempData(_T("BOOK_CNT_ALIAS"), _T("UDF_책수"));
	
	ids = api.SPFExecutePath(_T("선정자료검수취소"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 주문취소 할 수 없습니다."));
		return -1;
	}
	SetRegObjDataInfo();
	DisplayGrid();
	return 0;
}

VOID CBO_ACQ_RegObjDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_REGOBJ);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentBookDM(m_pDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(FALSE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_RegObjDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_RegObjDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_RegObjDataMgr::InitSortItem()
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
	pTmpData->Add(_T("BP_발주일련번호"));
	pTmpData->Add(_T("BB_권일련번호"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	pMap->SetAt(_T("UDF_구입처부호설명"), _T("BP_구입처부호"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}


CString CBO_ACQ_RegObjDataMgr::TruncPrice(DOUBLE dPrice)
{
	INT nPos = -1;
	CString strTmpData, strTmp;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	if		( m_strPosNumber == _T("1") )   strTmpData = strTmp.Left(nPos+2);
	else if	( m_strPosNumber == _T("2") )  	strTmpData = strTmp.Left(nPos+3);
	else									strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

HBRUSH CBO_ACQ_RegObjDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}