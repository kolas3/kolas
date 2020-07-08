// PurchaseDataPrint.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseDataPrint.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"

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
// CPurchaseDataPrint dialog


CPurchaseDataPrint::CPurchaseDataPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPurchaseDataPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchaseDataPrint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSearchDlg = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CPurchaseDataPrint::~CPurchaseDataPrint()
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

}

VOID CPurchaseDataPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseDataPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchaseDataPrint, CDialog)
	//{{AFX_MSG_MAP(CPurchaseDataPrint)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseDataPrint message handlers

BOOL CPurchaseDataPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입자료출력")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_구입자료출력"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pDM = FindDM(_T("DM_구입자료관리"));
	m_pPrintDM = FindDM(_T("DM_구입자료관리_종"));
	m_pPrintObjDM = FindDM(_T("DM_구입자료관리_출력용"));

	GetDlgItem(IDC_stcPURCHASE_PRINT_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_PRINT_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_PRINT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_PRINT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_PURCHASE_PRINT, PURCHASE, SPECIES_TYPE_INVALID, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_PURCHASE_DATA_PRINT, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("구입자료를 출력합니다.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	SetPurchasePrintDataInfo();
	InitPopupMenuItem();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchaseDataPrint::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn;

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_PRINT_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_PRINT_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_PRINT_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_PRINT_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_PRINT_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 35, cx+2, cy-30);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================
}

VOID CPurchaseDataPrint::SetPurchasePrintDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d종 %d책의 구입자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CPurchaseDataPrint::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strTmpData;
	INT nGroupNo, nWorkNo, nSerialNo;
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
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		strGroupNo.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pPrintDM->SetCellData(_T("UDF_출력차수정보"), strGroupNo, nRow);
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
	if(strDMFieldAlias == _T("UDF_자료상태설명"))
	{
		if(str == _T("BOT111O")) str = _T("기초자료");
		else if(str == _T("BOT112O")) str = _T("반입자료");
		else if(str == _T("BOT211O")) str = _T("수서복본추기자료");
		else if(str == _T("BOT212O")) str = _T("정리복본추기자료");
		else if(str == _T("BOA111N")) str = _T("구입대상자료");
		else if(str == _T("BOA112N")) str = _T("주문자료");
		else if(str == _T("BOA113O")) str = _T("미납품자료");
		else if(str == _T("BOA114O")) str = _T("구입인계대상자료");
		else if(str == _T("BOA211O")) str = _T("기증접수자료");
		else if(str == _T("BOA212O")) str = _T("기증부당자료");
		else if(str == _T("BOR111N")) str = _T("등록인계자료");
		else if(str == _T("BOR112N")) str = _T("등록자료");
		else if(str == _T("BOR113O")) str = _T("가원부번호부여자료");
		else if(str == _T("BOC111O")) str = _T("복본조사대상자료");
		else if(str == _T("BOC112O")) str = _T("분류대상자료");
		else if(str == _T("BOC113N")) str = _T("정리대상자료");
		else if(str == _T("BOC114O")) str = _T("열람인계대상자료");
		else if(str == _T("BOL111O")) str = _T("열람인계자료");
		else if(str == _T("BOL112N")) str = _T("배가자료");
		else if(str == _T("BOL113O")) str = _T("수리제본자료");
		else if(str == _T("BOL114O")) str = _T("파손자료");
		else if(str == _T("BOL115O")) str = _T("제적자료");
		else if(str == _T("BOL211O")) str = _T("관외대출자료");
		else if(str == _T("BOL212O")) str = _T("관내대출자료");
		//=====================================================
		//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
		// 18.09.27 JMJ KOL.RED.2018.008
		else if(str == _T("BOL411O")) str = _T("단행책두레업무진행자료");
		else if(str == _T("BOL511O")) str = _T("통합도서타관반납");
		// 2013.12.01 ADD BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
		else if(str == _T("BOL611O")) str = _T("통합도서타관대출");
		//=====================================================
		else if(str == _T("BOL213O")) str = _T("특별대출자료");
		else if(str == _T("BOL214O")) str = _T("긴급대출자료");
		else if(str == _T("BOL215O")) str = _T("배달자료");
		m_pPrintDM->SetCellData(_T("UDF_자료상태설명"), str, nRow);
	}
	if(strDMFieldAlias == _T("BP_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환종가격"), str, nRow);
	}
	if(strDMFieldAlias == _T("BP_발주가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환할인종가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

UINT CPurchaseDataPrint::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetPurchasePrintDataInfo();
		m_pGrid->Display();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("책To종"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetPurchasePrintDataInfo();
//	SortByGroupInfo();
	DisplayGrid();
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	return 0;
}

VOID CPurchaseDataPrint::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CPurchaseDataPrint::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CPurchaseDataPrint::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

BOOL CPurchaseDataPrint::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

UINT CPurchaseDataPrint::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids = -1;
	
	if(lParam == BO_PURCHASE_DATA)
	{
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
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
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
		DeleteDataProcessingDlg();
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("출력 데이터를 구축할 수 없습니다."));
			return 0;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();
	}
	return 0;
}

VOID CPurchaseDataPrint::ShowDataProcessingDlg()
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

VOID CPurchaseDataPrint::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CPurchaseDataPrint::DisplayGrid()
{
/*	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입자료를 출력중입니다."));
	INT nRowCnt = m_pPrintDM->GetRowCount();
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
*/	m_pGrid->Display();
//	DeleteDataProcessingDlg();
}

/*
INT CPurchaseDataPrint::SortByGroupInfo()
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
*/

VOID CPurchaseDataPrint::ShowSortDlg(INT nType/*=1*/)
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

UINT CPurchaseDataPrint::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CPurchaseDataPrint::InitSortItem()
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
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

HBRUSH CPurchaseDataPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}