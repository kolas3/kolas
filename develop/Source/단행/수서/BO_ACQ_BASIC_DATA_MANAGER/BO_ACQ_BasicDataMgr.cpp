// BO_ACQ_BasicDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_BasicDataMgr.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_Select_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/Group_Select_Dlg.h"
#include "..\..\..\공통\Duplicate_Data_Search\DUP_KEY_SETTING.h"
#include "../../../공통/BO_SPECIES_API/GroupInfo.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/CodeChange.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/AcqInfoChange.h"
#include "..\..\..\공통\FileImport\FileImportDlg.h"  // 2005-01-31
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_BasicDataMgr dialog

CBO_ACQ_BasicDataMgr::CBO_ACQ_BasicDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_BasicDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_BasicDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pBOSpeciesApi = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
	m_strPosNumber.Empty();
}

CBO_ACQ_BasicDataMgr::~CBO_ACQ_BasicDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pDuplicateSearchMgr != NULL)
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
		m_pSearchDlg = NULL;
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

VOID CBO_ACQ_BasicDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_BasicDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_BasicDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_BasicDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_BasicDataMgr message handlers

VOID CBO_ACQ_BasicDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CBO_ACQ_BasicDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서기초자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_기초자료관리"));
	m_pPrintDM = FindDM(_T("DM_기초자료관리_종"));
	m_pPrintObjDM = FindDM(_T("DM_기초자료관리_종_출력용"));
	m_pCM = FindCM(_T("CM_기초자료관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 2005-01-13
	// 가격 자릿수 설정값을 가져온다.
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
		
	GetDlgItem(IDC_edtBASIC_GROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcBASIC_GROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtBASIC_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcBASIC_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_BASIC, ACQ_TYPE_INVALID, BO_BASIC, m_pDM->CONNECTION_INFO);
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_BASIC, ACQ_TYPE_INVALID, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strMsg = _T("수서 기초자료를 관리합니다.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));

	SetBasicDataInfo();
	InitPopupMenuItem();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_BasicDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcBASIC_DATA);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcBASIC_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtBASIC_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(100, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcBASIC_GROUPNAME);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(180, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtBASIC_GROUPNAME);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(240, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBASIC_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);		
	pGrid->MoveWindow(-2, 35, cx+2, cy);
}

BOOL CBO_ACQ_BasicDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_BasicDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_BasicDataMgr::ShowPrintDlg()
{
	if(!(m_pPrintDlg->GetCreate())) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_BasicDataMgr::ShowBasicDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_BASIC);
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
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(SPECIES);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetBasicDataInfo();
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

VOID CBO_ACQ_BasicDataMgr::DeleteBasicData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	INT nCurRow = m_pGrid->GetRow();
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_BASIC);
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
		SetBasicDataInfo();
		DisplayGrid();	
	}
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ] 
// - 구입처리
// 
// [ 미니스펙 ]
// 1. 그리드의 선정체크를 확인한다.
// 2. 차수선택창에 작업상태를 설정한다.
// 3. K2UP버젼이 Light라면 AllowNullGroup을 True로 설정한다.
// 4. Business API를 초기화하고 데이터 및 포인터를 설정한다.
// 5. API에 _T("구입처리")로 실행한다.
// 6. 선택된 차수에대한 정보를 CM에 설정한다.
// 7. 그리드를 다시 디스플레이한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_BasicDataMgr::PurchaseBasicData()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids ;
	// 차수선택 창
	CDivisionNo_Select_Dlg dlg(this);
	CGroupInfo* groupInfo;
	CBO_ACQ_Business_Api api(this);
	CESL_Grid* pGrid = NULL;


	//=======================================================================
	// 1. 그리드의 선정체크를 확인한다.
	//=======================================================================
	ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	

	//=======================================================================
	// 2. 차수선택창에 작업상태를 설정한다.
	//=======================================================================
	dlg.SetWorkStatus(_T("구입대상"));
	dlg.SetSourceDM(m_pPrintDM);
	dlg.SetSourceGrid(m_pGrid);
	

	//=======================================================================
	// 3. K2UP버젼이 Light라면 AllowNullGroup을 True로 설정한다.
	//=======================================================================
	if( m_pInfo->GetKIIUPClientVersion() == 'L' ) dlg.SetAllowNULLGroup(TRUE);
	if( IDOK != dlg.DoModal() )		return ;
	
	
	
	//=======================================================================
	// 4. Business API를 초기화하고 데이터 및 포인터를 설정한다.
	//=======================================================================
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_BASIC);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)m_pDM);
	
	pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	api.SetTempPtr(_T("GRID"), (DWORD*)pGrid);

	groupInfo = dlg.GetSelGroup();
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);

	//=======================================================================
	// 5. API에 _T("구입처리")로 실행한다.
	//=======================================================================
	ids = api.SPFExecutePath(_T("구입처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 구입처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 구입처리 하였습니다."));

	//=======================================================================
	// 6. 선택된 차수에대한 정보를 CM에 설정한다.
	//=======================================================================
	SetBasicDataInfo();

	//=======================================================================
	// 7. 그리드를 다시 디스플레이한다.
	//=======================================================================
	DisplayGrid();
}

VOID CBO_ACQ_BasicDataMgr::DonateBasicData()
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
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_BASIC);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	CESL_Grid * pGrid = NULL;
	pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	api.SetTempPtr(_T("GRID"), (DWORD*)pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	ids = api.SPFExecutePath(_T("기증처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 기증처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 기증처리 하였습니다."));
	
	SetBasicDataInfo();
	DisplayGrid();
}

VOID CBO_ACQ_BasicDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetSubjectCodeEnable(FALSE);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_BasicDataMgr::ShowGroupMgrDlg()
{
	CGroup_Mgr mgr(this);
	mgr.SetOpenMode(0);
	mgr.DoModal();
}

UINT CBO_ACQ_BasicDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("기초자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	
	m_pPrintDM->FreeData();
	INT rowCnt = m_pDM->GetRowCount();
	if(rowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		DisplayGrid();
		SetBasicDataInfo();
		return 0;
	}

	// 중복된 정보를 제거해야 한다.
	CMapStringToString tmpMap;	tmpMap.RemoveAll();
	CString strTmpData, strValue;
	for(INT i=rowCnt-1 ; i>=0 ; i--)
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
	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("책To종"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetBasicDataInfo();
	DisplayGrid();
	return 0;
}

INT CBO_ACQ_BasicDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetCurrencyCode(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_화폐구분"), strTmpData, nRow);		
		//strTmpData.Empty();	ids = m_MarcConvertMgr.GetPriceInfo(&marc, strTmpData);
		//if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_가격"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_가격"))
	{		
		// 2004.09.14  화폐구분값을 소수점 2자리까지 나타낸다.
		
		DOUBLE strTemp;
		TCHAR* pEndPtr = NULL;
		//=======================================================
		//20080523 ADD BY PJW : 책수만큼의 가격을 준다
		INT nDmRow;
		CString strPrice;
		FLOAT fPrice;
		CString strPrintDMSpeciesRecKey;
		CString strDMSpeciesRecKey;
		fPrice = 0.0;
		strPrintDMSpeciesRecKey = m_pPrintDM->GetCellData(_T("IBS_종KEY"), nRow);
		nDmRow = m_pDM->GetRowCount();
		for(INT i = 0; i < nDmRow ; i++)
		{
			strDMSpeciesRecKey = m_pDM->GetCellData(_T("IBS_종KEY"), i);
			if(strPrintDMSpeciesRecKey == strDMSpeciesRecKey)
			{
				strPrice = m_pDM->GetCellData(_T("UDF_가격"), i);
				if( strPrice.IsEmpty()) 
				{
					strPrice = m_pDM->GetCellData(_T("UDF_단가"), i);
					if( strPrice.IsEmpty()) 
					{
						strPrice = _T("0.0");
					}
				}
				fPrice +=  _tcstod(strPrice.GetBuffer(0), &pEndPtr);
			}
		}
		strPrice.Format(_T("%.2f"),fPrice);
		m_pPrintDM->SetCellData(_T("UDF_가격"), strPrice,nRow);
		//=======================================================


		//strTemp = (DOUBLE)atof(m_pPrintDM->GetCellData(_T("UDF_가격"), nRow));
		
		strTemp = _tcstod(m_pPrintDM->GetCellData(_T("UDF_가격"), nRow), &pEndPtr);

		str = TruncPrice(m_pPrintDM, strTemp);		

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
		// 2004.09.14  화폐구분값을 소수점 2자리까지 나타낸다.
		DOUBLE strTemp;
		//strTemp = (DOUBLE)atof(str);
		TCHAR* pEndPtr = NULL;
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		str = TruncPrice(m_pPrintDM, strTemp);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_BasicDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();

	if(nCurSel < 0) bSetTopRow = FALSE;

	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;

	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;

	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_BasicDataMgr::SetBasicDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d종 %d책의 수서 기초자료가 검색되었습니다."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("그룹명"), strMsg);
	m_pCM->SetControlMgrData(_T("그룹생성년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_BasicDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_BasicDataMgr)
//	ON_EVENT(CBO_ACQ_BasicDataMgr, IDC_gridBASIC_DATA, -600 /* Click */, OnClickgridBASICDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_BasicDataMgr, IDC_gridBASIC_DATA, -601 /* DblClick */, OnDblClickgridBASICDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//DEL VOID CBO_ACQ_BasicDataMgr::OnClickgridBASICDATA() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL }

//DEL BOOL CBO_ACQ_BasicDataMgr::PreTranslateMessage(MSG* pMsg) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL /*	if(VK_UP == pMsg->wParam || VK_DOWN == pMsg->wParam)
//DEL 	{
//DEL 		if (GetDlgItem(IDC_gridBASIC_DATA)->m_hWnd == pMsg->hwnd)
//DEL 		{
//DEL 			CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
//DEL 			pGrid->SetReverse((INT)pGrid->GetRow());
//DEL 		}
//DEL 	}
//DEL */	return CDialog::PreTranslateMessage(pMsg);
//DEL }

INT CBO_ACQ_BasicDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	if(!m_pGrid) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

INT CBO_ACQ_BasicDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	return 0;
}

VOID CBO_ACQ_BasicDataMgr::DupPurchaseDataSearch()
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

	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("기초자료복본조사"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

VOID CBO_ACQ_BasicDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

UINT CBO_ACQ_BasicDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_BasicDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids ;
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	if(lParam == BO_ACQ_BASIC_DATA) 
	{	
		CString strDivAlias = _T("IBS_종KEY");
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
		PrintMgr.SetTempData(_T("구분알리아스"),  strDivAlias.GetBuffer(0));
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
	return 0;
}

VOID CBO_ACQ_BasicDataMgr::OnDblClickgridBASICDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	
	ShowBasicDataDlg(GUPDATE);
}

VOID CBO_ACQ_BasicDataMgr::ChangeGroup()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CGroupInfo pTargetGroup;
	CGroup_Select_Dlg dlg(this);
	dlg.SetSpeciesType(BO_BASIC);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());

	if(IDOK == dlg.DoModal())
	{
		pTargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
		pTargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
		pTargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());
	}
	else return ;
	
	CString strQuery, strResult;

	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR='%s' AND GROUP_SEQ=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
		pTargetGroup.GetAcqYear(), pTargetGroup.GetGroupNo());
	//strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR='%s' AND GROUP_SEQ=%s"),
	//	pTargetGroup.GetAcqYear(), pTargetGroup.GetGroupNo());
	//--2008.10.15 UPDATE BY PWR --}}
	m_pDM->GetOneValueQuery(strQuery, strResult);
	INT nLastSerialNo = _ttoi(strResult.GetBuffer(0));
	pTargetGroup.SetLastSerialNo(nLastSerialNo);

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)&pTargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_BASIC);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	ids = api.SPFExecutePath(_T("그룹정보변경"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("그룹정보를 변경할 수 없습니다."));
		return ;
	}
	ESLAfxMessageBox(_T("그룹정보를 변경하였습니다."));

	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(&pTargetGroup);
	SetBasicDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}

VOID CBO_ACQ_BasicDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_BasicDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

UINT CBO_ACQ_BasicDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
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
	SetBasicDataInfo();
	return 0;
}

BOOL CBO_ACQ_BasicDataMgr::InitSortItem()
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

VOID CBO_ACQ_BasicDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_BasicDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

VOID CBO_ACQ_BasicDataMgr::ChangeAcqInfo()
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

CString CBO_ACQ_BasicDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
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

VOID CBO_ACQ_BasicDataMgr::ShowFileImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("수서기초"), _T("BOT111O"));
	dlg.SetOpenMode(4000);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	SetBasicDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt + 1);
	}
}

VOID CBO_ACQ_BasicDataMgr::ShowFileExport()
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
