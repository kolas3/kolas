// DonateDataView.cpp : implementation file
//

#include "stdafx.h"
#include "DonateDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateDataView dialog


CDonateDataView::CDonateDataView(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateDataView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateDataView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBO_DM = NULL;
	m_pBO_PrintDM = NULL;
	m_pSE_DM = NULL;
	m_pCM = NULL;
	m_pBOGrid = NULL;
	m_pSEGrid = NULL;
	m_pMainGrid = NULL;
	m_pMainDM = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_bBO_Select = FALSE;
	m_bSE_Select = FALSE;
	m_pPrintMgr = NULL;
	m_pPrintDM = NULL;
	m_pDonateDataSearchDlg = NULL;
	m_nRow = -1;
}

CDonateDataView::~CDonateDataView()
{

	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	if(m_pDonateDataSearchDlg)
	{
		delete m_pDonateDataSearchDlg;
		m_pDonateDataSearchDlg = NULL;
	}
}

VOID CDonateDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateDataView)
	DDX_Control(pDX, IDC_tabDVIEW_DONATE_DATA, m_tabDonateData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateDataView, CDialog)
	//{{AFX_MSG_MAP(CDonateDataView)
	ON_BN_CLICKED(IDC_btnDVIEW_PRINT, OnbtnDVIEWPRINT)
	ON_BN_CLICKED(IDC_btnDVIEW_PREV, OnbtnDVIEWPREV)
	ON_BN_CLICKED(IDC_btnDVIEW_NEXT, OnbtnDVIEWNEXT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabDVIEW_DONATE_DATA, OnSelchangetabDVIEWDONATEDATA)
	ON_BN_CLICKED(IDC_btnDONATE_DATE_SEARCH, OnbtnDONATEDATESEARCH)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateDataView message handlers

BOOL CDonateDataView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_pMainDM == NULL || m_pMainGrid == NULL)
	{
		ESLAfxMessageBox(_T("기증자관련 정보가 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	if (InitESL_Mgr(_T("K2UPGRADE_기증자료보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(m_rcInitial);

	m_pCM = FindCM(_T("CM_기증자료보기"));
	m_pBO_DM = FindDM(_T("DM_기증접수자료관리"));
	m_pBO_PrintDM = FindDM(_T("DM_기증접수자료관리_종"));	
	m_pSE_DM = FindDM(_T("DM_연속_기증자료"));
	m_pPrintDM = FindDM(_T("DM_통합_기증자료"));
	
	m_pDonateDataSearchDlg = new CDonateDataSearchDlg(this);
	
	m_pBOGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_BO"));
	m_pSEGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_SE"));
	m_pBOGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pSEGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//JOB.2019.0017 ADD BY KYJ
	//개인정보 엑셀 반출,로그 설정값 적용
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pBOGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		m_pSEGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pBOGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pBOGrid->m_strPrintInfo = _T("기증자료(단행) 관리");
		m_pSEGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pSEGrid->m_strPrintInfo = _T("기증자료(연속) 관리");
	}
	//-------------------------------------------------------------------------
		
	m_tabDonateData.InsertItem(0, _T("단행"));
	m_tabDonateData.InsertItem(1, _T("연속"));
	m_tabDonateData.SetCurSel(0);
	m_pBOGrid->ShowWindow(SW_SHOW);
	m_pSEGrid->ShowWindow(SW_HIDE);
	
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	PostMessage(WM_SIZE);
	
	CLibInfoMgr mgr(this);
	mgr.GetLibInfo(GetUserID(), &m_cLibInfo);
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CDonateDataView::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	INT nGroupNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = GetActiveDM()->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = GetActiveDM()->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == GetActiveDM()->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_접수번호정보"))
	{	
		strAcqYear = GetActiveDM()->GetCellData(_T("BP_수입년도"), nRow);
		strGroupNo = GetActiveDM()->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		str.Format(_T("%s-%03d"), strAcqYear, nGroupNo);// = strAcqYear+_T("-")+strGroupNo;
		GetActiveDM()->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{
		strTmpData = GetActiveDM()->GetCellData(_T("IBS_분류표구분"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		GetActiveDM()->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		GetActiveDM()->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("SV_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		GetActiveDM()->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_책수"))
		GetActiveDM()->SetCellData(_T("BP_납품책수"), str, nRow);
	return 0;
}

VOID CDonateDataView::OnbtnDVIEWPRINT() 
{
	// TODO: Add your control notification handler code here
	INT ids = 0;
	if(!m_bBO_Select) ids = MakeBO_DonateData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, m_nIdx);
	if(ids < 0) return ;
	if(!m_bSE_Select) ids = MakeSE_DonateData(m_pMainDM, m_pSE_DM, m_nIdx);
	if(ids < 0) return ;
	ids = MakeMergeDonateData(m_pBO_PrintDM, m_pSE_DM, m_pPrintDM);
	if(ids < 0) return ;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("출력할 기증자료가 없습니다."));
		return ;
	}

	ids = SetPrintDonateConfirmData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, m_pSE_DM, m_pPrintDM, m_pPrintMgr, &m_cLibInfo, m_nIdx);
	if(ids < 0) return;
	m_pPrintMgr->Print();	
}

VOID CDonateDataView::OnbtnDVIEWPREV() 
{
 	Display(-1);
}

VOID CDonateDataView::OnbtnDVIEWNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

INT CDonateDataView::Display(INT nIdx)
{
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = m_nFirstIdx;
		if(m_nIdx < 0) m_nIdx = 0;
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = m_pMainGrid->SelectGetPrev();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = m_pMainGrid->SelectGetNext();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1000:
		break;
	default:
		return 0;
	}

	m_pCM->AllControlDisplay(m_nIdx);
	if(m_pDonateDataSearchDlg)
	{
		m_pCM->SetControlMgrData(_T("기증일자1"), m_pDonateDataSearchDlg->GetDonateDate1());
		m_pCM->SetControlMgrData(_T("기증일자2"), m_pDonateDataSearchDlg->GetDonateDate2());
	}
	m_bBO_Select = FALSE;
	m_bSE_Select = FALSE;
	
	INT ids = MakeDonateData(m_nIdx);
	if(ids < 0) return -1;
	GetActiveGrid()->Display();
	m_pMainGrid->SetReverse(m_nIdx);
	
	GetDlgItem(IDC_edtDONATE_DATE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_edtDONATE_DATE2)->EnableWindow(FALSE);

	return 0;	
}

VOID CDonateDataView::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
	if(ids < 0) return;
	m_nLastIdx = m_pMainGrid->SelectGetTailPosition();
	m_nFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	
/*
	INT cnt = m_pMainDM->GetRowCount();
	m_nFirstIdx = 0;
	m_nLastIdx = cnt-1;
*/
}

VOID CDonateDataView::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDVIEW_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDVIEW_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx== idx || idx < 0)
		GetDlgItem(IDC_btnDVIEW_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDVIEW_NEXT)->EnableWindow(FALSE);
}

INT CDonateDataView::MakeDonateData(INT nIdx)
{
	INT nCurSel = m_tabDonateData.GetCurSel();
	INT ids = 0;
	if(nCurSel == 0)
	{
		if(!m_bBO_Select)
		{
			ids = MakeBO_DonateData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, nIdx);
			m_bBO_Select = TRUE;
		}
	}
	else if(nCurSel == 1)
	{
		if(!m_bSE_Select)
		{
			ids = MakeSE_DonateData(m_pMainDM, m_pSE_DM, nIdx);
			m_bSE_Select = TRUE;
		}
	}

	return ids;
}

CESL_DataMgr * CDonateDataView::GetActiveDM()
{
	CESL_DataMgr * pDM = NULL;
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
		pDM = m_pBO_PrintDM;
	else if(nCurSel == 1)
		pDM = m_pSE_DM;
	return pDM;
}

CESL_Grid * CDonateDataView::GetActiveGrid()
{
	CESL_Grid * pGrid = NULL;
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
		pGrid = m_pBOGrid;
	else if(nCurSel == 1)
		pGrid = m_pSEGrid;
	return pGrid;
}

INT CDonateDataView::MakeSE_DonateData(CESL_DataMgr * pDonatorDM, CESL_DataMgr * pSE_DM, INT nIdx)
{
	if(m_pDonateDataSearchDlg == NULL) return -1;
	CString strDonateDate1, strDonateDate2, strTmpQuery, strDateQuery;
	strDonateDate1 = m_pDonateDataSearchDlg->GetDonateDate1();
	strDonateDate2 = m_pDonateDataSearchDlg->GetDonateDate2();
	
	strTmpQuery = MakeDateRangeQuery(_T("ACQ_DATE"), strDonateDate1, strDonateDate2);
	if(!strTmpQuery.IsEmpty())
	{
		strDateQuery.Format(_T(" AND A.REC_KEY IN (SELECT ACQ_TYPE_VOL_KEY FROM SE_BOOK_TBL WHERE REC_KEY IS NOT NULL %s)"), strTmpQuery);
	}

	CString strQuery, strDonatorKey;
	strDonatorKey = pDonatorDM->GetCellData(_T("기증자정보KEY"), nIdx);
	if(strDonatorKey.IsEmpty()) return -1;

	strQuery.Format(_T("I.REC_KEY=D.SPECIES_KEY AND D.REC_KEY=A.ACQ_KEY AND A.VOL_KEY=V.REC_KEY AND D.DONATOR_KEY=%s"), strDonatorKey);
	strQuery += strDateQuery;
	pSE_DM->FreeData();
	INT ids = pSE_DM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;

	pSE_DM->SORT(_T("UDF_입수일"));

	return 0;
}

INT CDonateDataView::MakeBO_DonateData(CESL_DataMgr * pDonatorDM, CESL_DataMgr * pBO_DM, CESL_DataMgr * pBO_PrintDM, INT nIdx)
{
	//기증일자 가져오기
	if(m_pDonateDataSearchDlg == NULL) return -1;
	CString strDonateDate1, strDonateDate2, strDateQuery;
	strDonateDate1 = m_pDonateDataSearchDlg->GetDonateDate1();
	strDonateDate2 = m_pDonateDataSearchDlg->GetDonateDate2();
	
	strDateQuery = MakeDateRangeQuery(_T("P.INPUT_DATE"), strDonateDate1, strDonateDate2);

	CString strQuery, strDonatorKey;
	strDonatorKey = pDonatorDM->GetCellData(_T("기증자정보KEY"), nIdx);
	if(strDonatorKey.IsEmpty()) return -1;
	
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND P.DONATOR_KEY=%s"), strDonatorKey);
	strQuery += strDateQuery;
	
	pBO_DM->FreeData();
	INT ids = pBO_DM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	pBO_PrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)pBO_DM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)pBO_PrintDM);
	ids = mgr.SPFExecutePath(_T("책To종"));
	if(ids < 0) return ids;

	pBO_PrintDM->SORT(_T("BP_입력일"));

	INT nRowCnt = pBO_PrintDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pBO_PrintDM->SetCellData(_T("BP_납품책수"), pBO_PrintDM->GetCellData(_T("UDF_책수"), i), i);
	}
	return 0;
}

VOID CDonateDataView::OnSelchangetabDVIEWDONATEDATA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetTabSelect();
	*pResult = 0;
}

VOID CDonateDataView::SetTabSelect()
{
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
	{
		m_pBOGrid->ShowWindow(SW_SHOW);
		m_pSEGrid->ShowWindow(SW_HIDE);
	}
	else if(nCurSel == 1)
	{
		m_pBOGrid->ShowWindow(SW_HIDE);
		m_pSEGrid->ShowWindow(SW_SHOW);
	}
	MakeDonateData(m_nIdx);
	GetActiveGrid()->Display();
}

INT CDonateDataView::MakeMergeDonateData(CESL_DataMgr *pBO_DM, CESL_DataMgr *pSE_DM, CESL_DataMgr *pMerge_DM)
{
	pMerge_DM->FreeData();
	INT nRowCnt = pBO_DM->GetRowCount();
	CString strTmpData;
	INT nCurRow;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pMerge_DM->InsertRow(-1);
		nCurRow = pMerge_DM->GetRowCount()-1;
		strTmpData.Format(_T("단행-%d"), i+1);
		pMerge_DM->SetCellData(_T("일련번호"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("기증일자"), pBO_DM->GetCellData(_T("BP_입력일"), i), nCurRow);
		pMerge_DM->SetCellData(_T("자료명"), pBO_DM->GetCellData(_T("UDF_표제사항"), i), nCurRow);
		pMerge_DM->SetCellData(_T("책수"), pBO_DM->GetCellData(_T("BP_납품책수"), i), nCurRow);
		pMerge_DM->SetCellData(_T("형식구분"), pBO_DM->GetCellData(_T("IBS_형식구분"), i), nCurRow);
		pMerge_DM->SetCellData(_T("매체구분"), pBO_DM->GetCellData(_T("IBS_매체구분"), i), nCurRow);
	}

	nRowCnt = pSE_DM->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		pMerge_DM->InsertRow(-1);
		nCurRow = pMerge_DM->GetRowCount()-1;
		strTmpData.Format(_T("연속-%d"), i+1);
		pMerge_DM->SetCellData(_T("일련번호"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("기증일자"), pSE_DM->GetCellData(_T("SD_기증시작일"), i), nCurRow);
		strTmpData.Format(_T("%s-%s"), pSE_DM->GetCellData(_T("ISS_표제사항"), i), pSE_DM->GetCellData(_T("SV_권호명"), i));
		pMerge_DM->SetCellData(_T("자료명"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("책수"), pSE_DM->GetCellData(_T("UDF_책수"), i), nCurRow);
		pMerge_DM->SetCellData(_T("형식구분"), pSE_DM->GetCellData(_T("SV_형식구분_코드"), i), nCurRow);
		pMerge_DM->SetCellData(_T("매체구분"), pSE_DM->GetCellData(_T("SV_매체구분_코드"), i), nCurRow);
	}
	return 0;
}

//==================================================================
// [함수설명]
//  연속 종수를 구한다.
//==================================================================
INT CDonateDataView::GetSESpeicesCount(CESL_DataMgr *pSE_DM)
{
	pSE_DM->SORT(_T("ISS_종KEY"));
	INT nRowCnt = pSE_DM->GetRowCount();
	INT nTotalCnt = 0;
	CString strKey, strPrevKey;
	
	//2005-11-03 황정영 수정
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strKey = pSE_DM->GetCellData(_T("ISS_종KEY"), i);
		if(strKey != strPrevKey)
		{
			nTotalCnt++;
			strPrevKey = strKey;
		}
	}
	return nTotalCnt;
}

//==================================================================
// [함수설명]
//  연속 책수를 구한다.
//==================================================================
INT CDonateDataView::GetSEBookCount(CESL_DataMgr *pSE_DM)
{
	INT nRowCnt = pSE_DM->GetRowCount();
	INT nTotalCnt = 0;
	CString strTmpData;
	
	//2005-11-03 황정영 수정
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pSE_DM->GetCellData(_T("UDF_책수"), i);
		nTotalCnt += _ttoi(strTmpData.GetBuffer(0));
	}
	return nTotalCnt;	
}

INT CDonateDataView::SetPrintDonateConfirmData(CESL_DataMgr *pDonatorDM,
											   CESL_DataMgr *pBO_BookDM, 
											   CESL_DataMgr *pBO_PrintDM, 
											   CESL_DataMgr *pSE_DM, 
											   CESL_DataMgr *pPrintDM, 
											   CBasicPrintMgr *pPrintMgr, 
											   CLibInfo * pLibInfo, 
											   INT nIdx)
{
	if(pDonatorDM == NULL || pBO_BookDM == NULL || pBO_PrintDM == NULL ||
		pSE_DM == NULL || pPrintDM == NULL || pPrintMgr == NULL || pLibInfo == NULL) 
		return -1;
	
	CString strBOSpeciesCnt, strBOBookCnt, strSESpeciesCnt, strSEBookCnt, strMsg, strDonator, strDonateData;
	pPrintMgr->SetPrintDM(pPrintDM);
	pPrintMgr->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	pPrintMgr->SetSIReportFilename(_T("K2UP_기증자료인수증"));
	pPrintMgr->InitPrintData();
	strBOSpeciesCnt.Format(_T("%d"), pBO_PrintDM->GetRowCount());
	strBOBookCnt.Format(_T("%d"), pBO_BookDM->GetRowCount());
	strSESpeciesCnt.Format(_T("%d"), GetSESpeicesCount(pSE_DM));
	strSEBookCnt.Format(_T("%d"), GetSEBookCount(pSE_DM));
	strDonator = pDonatorDM->GetCellData(_T("기증자"), nIdx);
	strDonateData = pPrintDM->GetCellData(_T("자료명"), 0);
	pPrintMgr->AddPrintData(_T("기증자"), pDonatorDM->GetCellData(_T("기증자"), nIdx));
	pPrintMgr->AddPrintData(_T("주소"), pDonatorDM->GetCellData(_T("주소"), nIdx));
	pPrintMgr->AddPrintData(_T("단행종수"), strBOSpeciesCnt);
	pPrintMgr->AddPrintData(_T("단행책수"), strBOBookCnt);
	pPrintMgr->AddPrintData(_T("연속종수"), strSESpeciesCnt);
	pPrintMgr->AddPrintData(_T("연속책수"), strSEBookCnt);
	pPrintMgr->AddPrintData(_T("도서관명"), pLibInfo->GetLibName());
	pPrintMgr->AddPrintData(_T("기증자료"), strDonateData);
	return 0;
}

VOID CDonateDataView::OnbtnDONATEDATESEARCH() 
{
	// TODO: Add your control notification handler code here
	ShowDonateDataSearchDlg();
}

UINT CDonateDataView::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	Display(1000);
	return 0;
}

CString CDonateDataView::MakeDateRangeQuery(CString strFieldName, CString strDate1, CString strDate2)
{
	CString strQuery;
	if(!strDate1.IsEmpty() && strDate2.IsEmpty())
	{
		strQuery.Format(_T(" AND %s = '%s'"), strFieldName, strDate1);
	}
	else if(!strDate1.IsEmpty() && !strDate2.IsEmpty())
	{
		strQuery.Format(_T(" AND %s BETWEEN '%s' AND '%s'"), strFieldName, strDate1, strDate2);
	}
	return strQuery;
}

VOID CDonateDataView::ShowDonateDataSearchDlg()
{
	if(m_pDonateDataSearchDlg == NULL) 
		return;
	if(!m_pDonateDataSearchDlg->GetCreate()) 
		m_pDonateDataSearchDlg->Create(this);
	m_pDonateDataSearchDlg->ShowWindow(SW_SHOW);
	m_pDonateDataSearchDlg->UpdateWindow();
	m_pDonateDataSearchDlg->CenterWindow();
}

VOID CDonateDataView::ShowDonateDataSearchDlgByModal()
{
	if(m_pDonateDataSearchDlg == NULL) 
		m_pDonateDataSearchDlg = new CDonateDataSearchDlg(pParentMgr);
	m_pDonateDataSearchDlg->DoModal();	
}

VOID CDonateDataView::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CWnd * pTab = GetDlgItem(IDC_tabDVIEW_DONATE_DATA);
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 87;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	if(pTab) pTab->MoveWindow(rect);		

	rect.top += 24;
	rect.bottom -= 6;
	rect.left += 6;
	rect.right -= 6;
	if(m_pBOGrid) m_pBOGrid->MoveWindow(rect);
	if(m_pSEGrid) m_pSEGrid->MoveWindow(rect);
}

HBRUSH CDonateDataView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
