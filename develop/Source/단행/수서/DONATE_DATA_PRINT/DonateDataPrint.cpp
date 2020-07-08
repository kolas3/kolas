// DonateDataPrint.cpp : implementation file
//

#include "stdafx.h"
#include "DonateDataPrint.h"
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
// CDonateDataPrint dialog


CDonateDataPrint::CDonateDataPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateDataPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateDataPrint)
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

CDonateDataPrint::~CDonateDataPrint()
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

VOID CDonateDataPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateDataPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateDataPrint, CDialog)
	//{{AFX_MSG_MAP(CDonateDataPrint)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateDataPrint message handlers

BOOL CDonateDataPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_기증자료출력")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_기증자료출력"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pDM = FindDM(_T("DM_기증접수자료관리"));
	m_pPrintDM = FindDM(_T("DM_기증접수자료관리_종"));
	m_pPrintObjDM = FindDM(_T("DM_기증접수자료관리_종_출력용"));

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("기증자료출력");
	}

	GetDlgItem(IDC_stcDONATE_PRINT_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_PRINT_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcDONATE_PRINT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_PRINT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_DONATE_PRINT, DONATE, SPECIES_TYPE_INVALID, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_DONATE_DATA_PRINT, DONATE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("기증자료를 출력합니다.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	SetDonatePrintDataInfo();
	InitPopupMenuItem();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonateDataPrint::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcDONATE_PRINT_DATA);
//	if(pBtn->GetSafeHwnd() == NULL) return;
//	pBtn->MoveWindow(5, 5, cx-10, 30);
   //1
//	CRect rect;
//	CButton *pBtn;
//	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_PRINT_GROUP_CREATE_YEAR);
//	if(pBtn->GetSafeHwnd() == NULL) return;
//	pBtn->GetWindowRect(&rect);
//	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
//	pBtn->ShowWindow(SW_SHOW);
////2
//	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_PRINT_GROUP_CREATE_YEAR);
//	pEdt->GetWindowRect(&rect);
//	pEdt->MoveWindow(90, 10, rect.Width(), rect.Height());
//	pEdt->ShowWindow(SW_SHOW);
////3	
//	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_PRINT_DIVNO);
//	pBtn->GetWindowRect(&rect);
//	pBtn->MoveWindow(180, 13, rect.Width(), rect.Height());
//	pBtn->ShowWindow(SW_SHOW);
////4
//	pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_PRINT_DIVNO);
//	pEdt->GetWindowRect(&rect);
//	pEdt->MoveWindow(235, 10, rect.Width(), rect.Height());
//	pEdt->ShowWindow(SW_SHOW);
	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_PRINT_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_PRINT_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_PRINT_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(160, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_PRINT_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(220, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDONATE_PRINT_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 35, cx+2, cy);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================
	
}

VOID CDonateDataPrint::SetDonatePrintDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d종 %d책의 기증자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

VOID CDonateDataPrint::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

INT CDonateDataPrint::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	INT nGroupNo;
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

	if(strDMFieldAlias == _T("UDF_접수번호정보"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_수입년도"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		str.Format(_T("%s-%04d"), strAcqYear, nGroupNo);// = strAcqYear+_T("-")+strGroupNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		strGroupNo.Format(_T("%04d"), nGroupNo);
		m_pPrintDM->SetCellData(_T("UDF_출력접수번호정보"), strGroupNo, nRow);
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
		//===================================================
		//2009.05.19 UPDATE BY LKS : 재정리 자료 및 상호대차 자료상태 추가
		else if(str == _T("BOL116O")) str = _T("재정리자료");
		else if(str == _T("BOL218O")) str = _T("상호대차자료");
		//===================================================
		// 17/07/20 김혜영 : 책나래대출자료 상태 추가
//*/ ADD ---------------------------------------------------------------------------
		else if(str == _T("BOL219O")) str = _T("책나래대출자료");
//*/ END ---------------------------------------------------------------------------
		m_pPrintDM->SetCellData(_T("UDF_자료상태설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_책수"))
		m_pPrintDM->SetCellData(_T("BP_납품책수"), str, nRow);
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CDonateDataPrint::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CDonateDataPrint::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	m_pPrintDlg->PrivacyPrintLogYN = TRUE;
}

BOOL CDonateDataPrint::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

UINT CDonateDataPrint::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("기증자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetDonatePrintDataInfo();
		DisplayGrid();
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
	SetDonatePrintDataInfo();
//	SortByGroupInfo();
	DisplayGrid();
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	
	return 0;
}

UINT CDonateDataPrint::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids;
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
	
	if(lParam == BO_DONATE_DATA)
	{
		ShowDataProcessingDlg();
		
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("BP_가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T(""));
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
			ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();
	}
	if(lParam == BO_ACQ_DEFAULT)
	{
		PrintDonatorData(&MapDivValue);
	}
	return 0;
}

VOID CDonateDataPrint::ShowDataProcessingDlg()
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

VOID CDonateDataPrint::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CDonateDataPrint::DisplayGrid()
{
	m_pGrid->Display();
}

INT CDonateDataPrint::PrintDonatorData(CMapStringToString * pMapDivValue)
{
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	if(strQuery.IsEmpty()) return -1;

	CESL_DataMgr * pDonatorDM = FindDM(_T("DM_기증자관리"));
	if(pDonatorDM == NULL ) return -1;

	strQuery.MakeUpper();
	CString strTblName = m_pDM->TBL_NAME;
	CString strWhere;
	
	strWhere.Format(_T("REC_KEY IN (SELECT C.REC_KEY FROM %s WHERE %s) AND MANAGE_CODE = UDF_MANAGE_CODE"), strTblName, strQuery);
	INT ids = pDonatorDM->RefreshDataManager(strWhere);
	if(ids < 0) return ids;
	if(pDonatorDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("출력할 기증자 정보가 없습니다."));
		return 1;
	}

	m_pPrintDlg->SetPrintDM(pDonatorDM);
	m_pPrintDlg->Print();

	return 0;
}

VOID CDonateDataPrint::ShowSortDlg(INT nType/*=1*/)
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

UINT CDonateDataPrint::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CDonateDataPrint::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("BP_그룹번호"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_일련번호"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

HBRUSH CDonateDataPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}