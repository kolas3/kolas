// BO_ACQ_ImportDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_ImportDataMgr.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "..\..\..\공통\Duplicate_Data_Search\DUP_KEY_SETTING.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/Group_Select_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_Select_Dlg.h"
#include "../../../공통/BO_SPECIES_API/GroupInfo.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/CodeChange.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/AcqInfoChange.h"

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
// CBO_ACQ_ImportDataMgr dialog


CBO_ACQ_ImportDataMgr::CBO_ACQ_ImportDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_ImportDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ImportDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pBOSpeciesApi = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CBO_ACQ_ImportDataMgr::~CBO_ACQ_ImportDataMgr()
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
	if(m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
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

VOID CBO_ACQ_ImportDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ImportDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ImportDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ImportDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ImportDataMgr message handlers

VOID CBO_ACQ_ImportDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_ImportDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서반입자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_반입자료관리"));
	m_pPrintDM = FindDM(_T("DM_반입자료관리_종"));
	m_pPrintObjDM = FindDM(_T("DM_반입자료관리_종_출력용"));
	m_pCM = FindCM(_T("CM_반입자료관리"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// 2005-01-13
	// 가격 자릿수 설정값을 가져온다.
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);

	GetDlgItem(IDC_edtIMPORT_GROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcIMPORT_GROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtIMPORT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcIMPORT_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_IMPORT, ACQ_TYPE_INVALID, BO_IMPORT, m_pDM->CONNECTION_INFO);
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_IMPORT, ACQ_TYPE_INVALID, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("수서 반입자료를 관리합니다.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));

	SetImportDataInfo();
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_ImportDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//CButton *pBtn = (CButton *)GetDlgItem(IDC_stcIMPORT_DATA);
	//if(pBtn->GetSafeHwnd() == NULL) return;
	//pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcIMPORT_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtIMPORT_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(100, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcIMPORT_GROUPNAME);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(185, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtIMPORT_GROUPNAME);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(240, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridIMPORT_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 35, cx+2, cy);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================			
}

VOID CBO_ACQ_ImportDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_ImportDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();	
}

VOID CBO_ACQ_ImportDataMgr::ShowFileImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("수서"), _T("BOT112O"));
	dlg.SetOpenMode(4000);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetImportDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt + 1);
	}
//	DisplayGrid();
}
//===================================================================
//20080618 UPDATE BY PJW : 반입자료관리에 입력기능 추가위해 수정
//VOID CBO_ACQ_ImportDataMgr::ShowImportDataDlg(OPERATION_TYPE nOpType)
//{
//	if(0 == m_pPrintDM->GetRowCount()) 
//	{
//		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
//		return ;
//	}
//	CSpecies_Form dlg(this);
//	dlg.SetAcqType(ACQ_TYPE_INVALID);
//	dlg.SetSpeciesType(BO_IMPORT);
//	dlg.SetOpType(nOpType);
//	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
//	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
//	dlg.SetParentType(SPECIES);
//	dlg.SetParentDM(m_pPrintDM);
//	dlg.SetParentBookDM(m_pDM);
//	dlg.SetParentGrid(m_pGrid);
//	dlg.DoModal();	
//	SetImportDataInfo();
////	DisplayGrid();
//}
VOID CBO_ACQ_ImportDataMgr::ShowImportDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_IMPORT);
	dlg.SetOpType(nOpType);
	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
			return ;
		}
	}

	//===================================================
	//2009.04.08 UPDATE BY PJW : 그리드 수정을 할경우 선정된자료가 있을경우 현재커서를 선정에 추가한다.
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
	
	
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(SPECIES);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();	
	SetImportDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pPrintDM->GetRowCount();
		if(nRowCnt < nCurCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}
//===================================================================

VOID CBO_ACQ_ImportDataMgr::DeleteImportData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_IMPORT);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	api.SetTempData(_T("삭제타입"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	ids = api.SPFExecutePath(_T("삭제"));
	if(ids == 0)
	{
		SetImportDataInfo();
		DisplayGrid();	
	}
/*
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_IMPORT);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	api.SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	api.SetTempData(_T("수서정보키알리아스"), _T(""));
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
		INT nCurRow = m_pGrid->GetTopRow();
		DisplayGrid();
		m_pGrid->SetTopRow(nCurRow);
		SetImportDataInfo();
	}
*/
}

VOID CBO_ACQ_ImportDataMgr::PurchaseImportData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetWorkStatus(_T("구입대상"));
	dlg.SetSourceDM(m_pPrintDM);
	dlg.SetSourceGrid(m_pGrid);
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_IMPORT);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)m_pDM);
	CESL_Grid * pGrid = NULL;
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	ids = api.SPFExecutePath(_T("구입처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 구입처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 구입처리 하였습니다."));
	DisplayGrid();
	SetImportDataInfo();
}

VOID CBO_ACQ_ImportDataMgr::DonateImportData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CRequisitionNo_Select_Dlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();
		
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_IMPORT);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	CESL_Grid * pGrid = NULL;
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	ids = api.SPFExecutePath(_T("기증처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 기증처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 기증처리 하였습니다."));
	SetImportDataInfo();
	DisplayGrid();
}

VOID CBO_ACQ_ImportDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetSubjectCodeEnable(FALSE);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_ImportDataMgr::ShowGroupMgrDlg()
{
	CGroup_Mgr mgr(this);
	mgr.SetOpenMode(0);
	mgr.DoModal();
}

BOOL CBO_ACQ_ImportDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CBO_ACQ_ImportDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode;
	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetCurrencyCode(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_화폐구분"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
//		strTmpData.Empty();	ids = m_MarcConvertMgr.GetPriceInfo(&marc, strTmpData);
//		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_가격"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_가격"))
	{
		//2004.09.14 화폐구분값을 소수점 2자리까지 나타낸다.
		DOUBLE strTemp;
		//strTemp = (DOUBLE)atof(m_pPrintDM->GetCellData(_T("BB_가격"), nRow));
		strTemp = _tcstod(m_pPrintDM->GetCellData(_T("BB_가격"), nRow), &pEndPtr);

		//===========================================
		//2008.07.16 UPDATE BY PJW : 원화일경우 소수점을 나타내지 않는다.
		//str = TruncPrice(m_pPrintDM, strTemp);
		CString strCurrencyCode;
		CString strTempPrice;
		INT nPos;

		m_pPrintDM->GetCellData(_T("UDF_화폐구분"), nRow, strCurrencyCode);
		if( 0 <= strCurrencyCode.Find(_T("\\")))
		{
			strTempPrice.Format(_T("%.2f"), strTemp);
			nPos = strTempPrice.Find(_T("."));
			str = strTempPrice.Left(nPos);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, strTemp);
		}
		//===========================================
		

		m_pPrintDM->SetCellData(_T("UDF_가격"), str, nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BS_주제구분"))
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
		else /*if(strTmpData.IsEmpty())*/
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		//2004.09.14 화폐구분값을 소수점 2자리까지 나타낸다.
		DOUBLE strTemp;
		str.Replace(_T(","), _T(""));
		//strTemp = (DOUBLE)atof(str);
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		//===========================================
		//2008.07.16 UPDATE BY PJW : 원화일경우 소수점을 나타내지 않는다.
		//str = TruncPrice(m_pPrintDM, strTemp);
		CString strCurrencyCode;
		CString strTempPrice;
		INT nPos;

		m_pPrintDM->GetCellData(_T("UDF_화폐구분"), nRow, strCurrencyCode);
		if( 0 <= strCurrencyCode.Find(_T("\\")))
		{
			strTempPrice.Format(_T("%.2f"), strTemp);
			nPos = strTempPrice.Find(_T("."));
			str = strTempPrice.Left(nPos);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, strTemp);
		}
		//===========================================		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_ImportDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

UINT CBO_ACQ_ImportDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("반입자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		DisplayGrid();
		SetImportDataInfo();
		return 0;
	}
		
	// 중복된 정보를 제거해야 한다.
	CMapStringToString tmpMap;	tmpMap.RemoveAll();
	CString strTmpData, strValue;
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		strTmpData = m_pDM->GetCellData(_T("BB_책KEY"), i);
		if(tmpMap.Lookup(strTmpData, strValue))
			m_pDM->DeleteRow(i);
		else
			tmpMap.SetAt(strTmpData, _T(""));
		
		// 그룹에 값이 없을 경우 적용안함으로 설정한다.
		// 2005.10.11 강봉수
		//
		if( m_pDM->GetCellData( _T("UDF_그룹명"), i) == _T("") )
		{
			m_pDM->SetCellData( _T("UDF_그룹명"), _T("적용안함"), i );
		}
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
	SetImportDataInfo();
	DisplayGrid();
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	
	return 0;
}

VOID CBO_ACQ_ImportDataMgr::SetImportDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d종 %d책의 수서 반입자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹명"), strMsg);
	m_pCM->SetControlMgrData(_T("그룹생성년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_ImportDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_ImportDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);		
}

VOID CBO_ACQ_ImportDataMgr::DupPurchaseDataSearch()
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
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("반입자료복본조사"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

VOID CBO_ACQ_ImportDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

UINT CBO_ACQ_ImportDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_ImportDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	if(lParam == BO_ACQ_IMPORT_DATA)
	{
		CString strDivAlias = _T("IBS_종KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			INT ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), strDivAlias.GetBuffer(0));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T(""));
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("원본출력DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		INT ids = PrintMgr.SPFExecutePath(_T("일반리스트출력"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_ImportDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_ImportDataMgr)
	ON_EVENT(CBO_ACQ_ImportDataMgr, IDC_gridIMPORT_DATA, -601 /* DblClick */, OnDblClickgridIMPORTDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_ImportDataMgr::OnDblClickgridIMPORTDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	
	ShowImportDataDlg(GUPDATE);
}

VOID CBO_ACQ_ImportDataMgr::ChangeGroup()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CGroupInfo TargetGroup;
	CGroup_Select_Dlg dlg(this);
	dlg.SetSpeciesType(BO_IMPORT);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());

	if(IDOK == dlg.DoModal())
	{
		TargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
		TargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
		TargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());
	}
	else return ;
	
	CString strQuery, strResult;
	strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR='%s' AND GROUP_SEQ=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
		TargetGroup.GetAcqYear(), TargetGroup.GetGroupNo());
	m_pDM->GetOneValueQuery(strQuery, strResult);
	INT nLastSerialNo = _ttoi(strResult.GetBuffer(0));
	TargetGroup.SetLastSerialNo(nLastSerialNo);

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)&TargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_IMPORT);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	ids = api.SPFExecutePath(_T("그룹정보변경"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("그룹정보를 변경할 수 없습니다."));
		return ;
	}
	ESLAfxMessageBox(_T("그룹정보를 변경하였습니다."));

	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(&TargetGroup);
	SetImportDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}

VOID CBO_ACQ_ImportDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_ImportDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

UINT CBO_ACQ_ImportDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
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
	SetImportDataInfo();
	return 0;
}

BOOL CBO_ACQ_ImportDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_그룹명"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BS_주제구분"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_ImportDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_ImportDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

VOID CBO_ACQ_ImportDataMgr::ChangeAcqInfo()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	
	CAcqInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}	
}

CString CBO_ACQ_ImportDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
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

HBRUSH CBO_ACQ_ImportDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

