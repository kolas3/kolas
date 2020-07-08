// ViewTransferDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ViewTransferDataDlg.h"
#include "../BO_ACQ_GROUP_MANAGER/GroupCollection.h"
#include "../BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../BO_ACQ_PRINT_MGR/BasicPrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTransferDataDlg dialog


CViewTransferDataDlg::CViewTransferDataDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CViewTransferDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewTransferDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CViewTransferDataDlg::~CViewTransferDataDlg()
{
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CViewTransferDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTransferDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTransferDataDlg, CDialog)
	//{{AFX_MSG_MAP(CViewTransferDataDlg)
	ON_BN_CLICKED(IDC_btnTVIEW_DIVNO, OnbtnTVIEWDIVNO)
	ON_BN_CLICKED(IDC_btnTVIEW_SEARCH, OnbtnTVIEWSEARCH)
	ON_BN_CLICKED(IDC_btnTVIEW_PRINT, OnbtnTVIEWPRINT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnTVIEW_SORT, OnbtnTVIEWSORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTransferDataDlg message handlers

BOOL CViewTransferDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_검수완료자료보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_검수완료자료보기"));
	m_pDM = FindDM(_T("DM_검수완료자료보기"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	InitSortItem();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CViewTransferDataDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nSerialNo, nWorkNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_차수정보"))
	{	
		strAcqYear = m_pDM->GetCellData(_T("BP_수입년도"), nRow);
		strWorkCode = m_pDM->GetCellData(_T("IBS_업무구분"), nRow);
		strGroupNo = m_pDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pDM->GetCellData(_T("BP_작업번호"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pDM->GetCellData(_T("BP_일련번호"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_구입처부호설명"))
	{
		strTmpData = m_pDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{
		strTmpData = m_pDM->GetCellData(_T("IBS_분류표구분"), nRow);
		
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
		else
		{
			str = _T("");
		}

		m_pDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_발주가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_변환할인가격"), str, nRow);
	}
	return 0;
}

VOID CViewTransferDataDlg::OnbtnTVIEWDIVNO() 
{
	// TODO: Add your control notification handler code here
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(PURCHASE);
	CString strAcqYear, strSelDivNo, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	CDivisionNo_List_Dlg dlg(this);
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkStatus(_T("검수완료"));
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDOK == dlg.DoModal())
	{
		strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("차수정보"), strSelDivNo);
		m_pCM->SetControlMgrData(_T("수입년도"), GroupCollection.GetAcqYear());
		m_pCM->SetControlMgrData(_T("업무구분"), GroupCollection.GetWorkCode());
		OnbtnTVIEWSEARCH();
	}
}

VOID CViewTransferDataDlg::OnbtnTVIEWSEARCH() 
{
	CString strSelGroup, strAcqYear, strQuery, strWorkCode;
	m_pCM->GetControlMgrData(_T("차수정보"), strSelGroup);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	if (strSelGroup.IsEmpty())
	{
		ESLAfxMessageBox(_T("차수정보를 입력하여 주십시오.")); 
		return;
	}

	if(strWorkCode.IsEmpty())
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1') AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE IS NULL AND I.WORKING_STATUS = 'BOR111N'"),
			strAcqYear, strSelGroup);
	}
	else
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1') AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO IN (%s) AND I.WORK_CODE = '%s' AND I.WORKING_STATUS = 'BOR111N'"),
			strAcqYear, strSelGroup, strWorkCode);
	}

	m_pDM->RefreshDataManager(strQuery);
	SortByGroupInfo();
	m_pGrid->Display();
}

VOID CViewTransferDataDlg::OnbtnTVIEWPRINT() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strGroupInfo;
	CBasicPrintMgr PrintMgr(this);
	PrintMgr.InitPrintData();
	m_pCM->GetControlMgrData(_T("차수정보"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
		
	PrintMgr.AddPrintData(_T("차수번호"), strGroupInfo);
	PrintMgr.AddPrintData(_T("수입년도"), strAcqYear);
	PrintMgr.SetPrintDM(m_pDM);
	PrintMgr.SetPrintType(0);
	PrintMgr.SetSIReportFilename(_T("K2UP_검수완료자료목록_보기"));
	PrintMgr.Print();
}

BOOL CViewTransferDataDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDC_btnTVIEW_SEARCH, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CViewTransferDataDlg::SortByGroupInfo()
{
	if(m_pDM == NULL) return 0;
	if(m_pDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_작업번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}
VOID CViewTransferDataDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
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

	CRect rect;
	GetClientRect(rect);
	rect.top += 54;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

VOID CViewTransferDataDlg::OnbtnTVIEWSORT() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal() && 1 > 0)
		m_pGrid->Display();	
}

BOOL CViewTransferDataDlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
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
	return TRUE;
}

HBRUSH CViewTransferDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
