// PriceSumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PriceSumDlg.h"
#include "../BO_ACQ_PRINT_MGR/BasicPrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPriceSumDlg dialog


CPriceSumDlg::CPriceSumDlg(CESL_Mgr * pParent /*=NULL*/)
	: CESL_Mgr(CPriceSumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPriceSumDlg)
	m_bApplyDiscRate = FALSE;
	//}}AFX_DATA_INIT
	m_OpType = GRID_SELECT;
	m_nPriceDMType = GRID_SELECT;
	m_nSpeciesType	 = SPECIES_TYPE_INVALID;
	m_OpenMode = K2UPGRADE;
	m_bApplyDiscRate = TRUE;
	m_bSpeciesYN	 = TRUE;
	m_strOrderPrice_Unit	 = _T("");	
	
	m_pGroupCollection		 = NULL;
	m_pParentGroupCollection = NULL;	
	m_PriceMgr = NULL;
	m_pCM = NULL;
	m_pParentGrid			 = NULL;
	m_pGrid			   		 = NULL;
	m_pParentDM = NULL;
	m_pDM = NULL;
	m_pParentBookDM = NULL;
	m_pDataProcessingDlg	 = NULL;
}

CPriceSumDlg::~CPriceSumDlg()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if(m_PriceMgr)
	{
		delete m_PriceMgr;
		m_PriceMgr = NULL;
	}
	DeleteDataProcessingDlg();
}

VOID CPriceSumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPriceSumDlg)
	DDX_Control(pDX, IDC_tabPRICE_SUM, m_tabPriceSum);
	DDX_Check(pDX, IDC_chkAPPLY_DISC_RATE, m_bApplyDiscRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPriceSumDlg, CDialog)
	//{{AFX_MSG_MAP(CPriceSumDlg)
	ON_CBN_SELCHANGE(IDC_cmbPSUM_CLASS, OnSelchangecmbPSUMCLASS)
	ON_BN_CLICKED(IDC_btnVIEW_DIVISION_NO, OnbtnVIEWDIVISIONNO)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPRICE_SUM, OnSelchangetabPRICESUM)
	ON_BN_CLICKED(IDC_btnPSUM, OnbtnPSUM)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_BN_CLICKED(IDC_chkAPPLY_DISC_RATE, OnchkAPPLYDISCRATE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPriceSumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT nCount = 0;
	INT nUseCodeCnt = 0;
	CString strUseCodeYN, strYN, strName, strIs64;

	if (InitESL_Mgr(_T("K2UPGRADE_가격합산")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_공통가격합산"));
	m_pDM = FindDM(_T("DM_공통가격합산"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pDM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM/DM_공통가격합산]을 설정할 수가 없습니다."));
		return FALSE;
	}

	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	m_tabPriceSum.InsertItem(nCount, _T("KDC주제별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("DDC주제별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("등록구분별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("별치구분별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("형식구분별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("매체구분별"));		nCount++;
	m_tabPriceSum.InsertItem(nCount, _T("보조등록구분별")); nCount++;
	
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), strUseCodeYN);	
	if( strUseCodeYN == _T("Y") )
	{
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
		if( strYN == _T("Y") )
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strName);
			m_tabPriceSum.InsertItem(nCount, strName);
			nCount++;
			m_nUseCodeNo[nUseCodeCnt] = 1;
			m_strUseCodeAlias[nUseCodeCnt] = strName;
			nUseCodeCnt++;
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
		if( strYN == _T("Y")) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strName);
			m_tabPriceSum.InsertItem(nCount, strName);
			nCount++;
			m_nUseCodeNo[nUseCodeCnt] = 2;
			m_strUseCodeAlias[nUseCodeCnt] = strName;
			nUseCodeCnt++;
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
		if( strYN == _T("Y")) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strName);
			m_tabPriceSum.InsertItem(nCount, strName);
			nCount++;
			m_nUseCodeNo[nUseCodeCnt] = 3;
			m_strUseCodeAlias[nUseCodeCnt] = strName;
			nUseCodeCnt++;
		}
	}

	if( GetManageValue(_T("수서"), _T("단행"), _T("구입가격단위설정"), _T(""), m_strOrderPrice_Unit	) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}

	UpdateData(FALSE);

	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("가격합산 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPriceSumDlg::Init()
{
	INT ids ;
	CString errstr;
	CString dateType[3][2] ={	_T("0"), _T("입력일"),
								_T("1"), _T("주문일"),
								_T("2"), _T("검수일"),	};
	CString priceSumType[2][2] ={	_T("0"), _T("월별가격합산"),
									_T("1"), _T("차수별가격합산")	};
	CMulitComboBoxManager mgr(this);
	CESL_Grid* pGrid = NULL;

	m_pGroupCollection = new CGroupCollection(this);
	if( m_pParentGroupCollection != NULL )
	{
		m_pParentGroupCollection->CopyData(m_pGroupCollection);
	}

	UpdateData(TRUE);

	if( m_PriceMgr == NULL )	 m_PriceMgr = new CPriceSum(this);
	m_PriceMgr->SetApplyDiscRateYN(m_bApplyDiscRate);
	m_PriceMgr->SetParentBookDM(m_pParentBookDM);
	m_PriceMgr->SetOrderPriceUnit(m_strOrderPrice_Unit);
	
	m_PriceMgr->m_TotalSpeciesCnt = 0;
	m_PriceMgr->m_TotalBookCnt = 0;
	m_PriceMgr->m_TotalPrice = 0;

	ids = m_PriceMgr->InitPriceSum(m_pParentDM, m_pParentGrid, m_nSpeciesType, m_bSpeciesYN);

	if(ids == 0)
	{
		SetTotalInfo();
		m_pCM->ControlDisplay(_T("MainGrid"));
		m_nPriceDMType = GRID_SELECT;
	}
	else m_OpType = DIVISION_NO;

	ShowCtrlByOpenMode();

	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("입력일"), dateType, _T("가격합산날짜구분"), 3);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("차수별가격합산"), priceSumType, _T("가격합산구분"), 2);

	pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if( pGrid != NULL )		pGrid->SetSort(FALSE);
	

	OnSelchangecmbPSUMCLASS();
	
	return TRUE;
}

VOID CPriceSumDlg::ShowCtrlByOpenMode()
{
	GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPSUM_ACQYEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(TRUE);

	if(m_OpenMode == K2UP_LIGHT)
		GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(FALSE);
	else
	{
		GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_ACQ_YEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcPSUM_ACQYEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(TRUE);
	}
}

VOID CPriceSumDlg::OnSelchangecmbPSUMCLASS() 
{
	CString strOpType;

	m_pCM->GetControlMgrData(_T("가격합산구분"), strOpType);
	if( m_OpType != GRID_SELECT )
	{
		if		( strOpType == _T("0") )	m_OpType = MONTHLY;
		else if ( strOpType == _T("1") )	m_OpType = DIVISION_NO;
	}

	ShowControlByOpType();
}

VOID CPriceSumDlg::ShowControlByOpType()
{
	CString strOpType;

	m_pCM->GetControlMgrData(_T("가격합산구분"), strOpType);
	if(m_OpType == MONTHLY)
	{
		GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_ACQ_YEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcPSUM_ACQYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcRANGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcPSUM_WORK_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbPSUM_WORK_CODE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_ACQ_YEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcPSUM_ACQYEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcRANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcPSUM_WORK_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbPSUM_WORK_CODE)->ShowWindow(SW_SHOW);
		CString strGroupInfo, strWorkCode;
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		strWorkCode = m_pGroupCollection->GetWorkCode();
		m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
		m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);
		m_pCM->SetControlMgrData(_T("업무구분"), strWorkCode);
		
		if( m_OpType == GRID_SELECT )
		{
			GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(FALSE);
		}
	}
}

VOID CPriceSumDlg::OnbtnVIEWDIVISIONNO() 
{
	CString strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	CDivisionNo_List_Dlg dlg(this);
	if(m_nSpeciesType == BO_PURCHASE)
		dlg.SetWorkStatus(_T("구입대상"));
	else if(m_nSpeciesType == BO_ORDER)
		dlg.SetWorkStatus(_T("주문"));
	else if(m_nSpeciesType == BO_REGOBJ)
		dlg.SetWorkStatus(_T("검수완료"));
	else if(m_nSpeciesType == BO_MISSING)
		dlg.SetWorkStatus(_T("검수완료"));
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(m_pGroupCollection);
	if(IDOK == dlg.DoModal())
	{
		m_pCM->SetControlMgrData(_T("차수번호"), dlg.GetSelDivNo());
		m_pCM->SetControlMgrData(_T("수입년도"), dlg.GetAcqYear());
		m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode());
	}
}

VOID CPriceSumDlg::OnSelchangetabPRICESUM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT ids = MakePrintDM();

	if(ids < 0)
	{
		ESLAfxMessageBox(_T("가격합산 기능을 수행하지 못하였습니다."));
		return ;
	}

	SetTotalInfo();
	m_pCM->ControlDisplay(_T("MainGrid"));

	*pResult = 0;
}

VOID CPriceSumDlg::OnbtnPSUM() 
{
	INT nCount, ids = 0;
	CString strOpType, strWorkCode;
	
	m_pCM->GetControlMgrData(_T("가격합산구분"), strOpType);
	
	if		( strOpType == _T("0") )
	{
		m_OpType = MONTHLY;
		m_nPriceDMType = MONTHLY;
	}
	else if	( strOpType == _T("1") )
	{
		m_OpType = DIVISION_NO;
		m_nPriceDMType = DIVISION_NO;
		ids = CheckValidationGroupInfo();
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("차수정보가 올바르지 않습니다."));
			return ;
		}
	}
	else	return;
	
	m_bSpeciesYN = FALSE;

	nCount = m_tabPriceSum.GetCurSel();	
	switch( nCount )
	{
	case 0:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("KDC주제구분"), m_OpType);
		break;
	case 1:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("LDC주제구분"), m_OpType);
		break;
	case 2:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("등록구분"), m_OpType);
		break;
	case 3:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("별치구분"), m_OpType);
		break;
	case 4:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("형식구분"), m_OpType);
		break;
	case 5:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("매체구분"), m_OpType);
		break;
	case 6:		ids = m_PriceMgr->SetReckeyList(m_pCM, _T("보조등록구분"), m_OpType);
		break;
	case 7:
		{
			if		( m_nUseCodeNo[0] == 1 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드1"), m_OpType);
			else if	( m_nUseCodeNo[0] == 2 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드2"), m_OpType);
			else if	( m_nUseCodeNo[0] == 3 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드3"), m_OpType);
			else							return;
		}
		break;
	case 8:
		{
			if		( m_nUseCodeNo[1] == 1 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드1"), m_OpType);
			else if	( m_nUseCodeNo[1] == 2 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드2"), m_OpType);
			else if	( m_nUseCodeNo[1] == 3 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드3"), m_OpType);
			else							return;
		}
		break;
	case 9:
		{
			if		( m_nUseCodeNo[2] == 1 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드1"), m_OpType);
			else if	( m_nUseCodeNo[2] == 2 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드2"), m_OpType);
			else if	( m_nUseCodeNo[2] == 3 )	ids = m_PriceMgr->SetReckeyList(m_pCM, _T("도서분류코드3"), m_OpType);
			else							return;
		}
		break;
	default:	return;
		break;
	}

	if( ids == 0 ) 
	{
		SetTotalInfo();
		m_pCM->ControlDisplay(_T("MainGrid"));
	}
	else if ( ids < 0 )
	{
		ESLAfxMessageBox(_T("가격합산을 완료할 수 없습니다."));
		return;
	}
	else	return ;
	
}

VOID CPriceSumDlg::SetParentGroupCollection(CGroupCollection *pGroupCollection)
{
	m_pParentGroupCollection = pGroupCollection;
}

INT CPriceSumDlg::CheckValidationGroupInfo()
{
	CString strGroupInfo, strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	m_pCM->GetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty())
		return -1;
	strGroupInfo = m_pGroupCollection->CheckValidate(strGroupInfo, strAcqYear, PURCHASE, strWorkCode);
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);
	if(strGroupInfo.IsEmpty()) return -1;
	return 0;
}

VOID CPriceSumDlg::OnbPRINT() 
{
	INT nCurSel ;
	CString strPrintTitle, strCondition;
	CBasicPrintMgr mgr(this);

	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("출력할 자료가 없습니다."));
		return ;
	}

	
	nCurSel = m_tabPriceSum.GetCurSel();
	switch( nCurSel )
	{
	case 0:
		strPrintTitle = _T("KDC주제구분별 가격합산");
		break;
	case 1:
		strPrintTitle = _T("DDC주제구분별 가격합산");
		break;
	case 2:
		strPrintTitle = _T("등록구분별 가격합산");
		break;
	case 3:
		strPrintTitle = _T("별치구분별 가격합산");
		break;
	case 4:
		strPrintTitle = _T("형식구분별 가격합산");
		break;
	case 5:
		strPrintTitle = _T("매체구분별 가격합산");
		break;
	case 6:
		strPrintTitle = _T("보조등록구분별 가격합산");
		break;
	case 7:
		strPrintTitle.Format(_T("%s별 가격합산"), m_strUseCodeAlias[0]);
		break;
	case 8:
		strPrintTitle.Format(_T("%s별 가격합산"), m_strUseCodeAlias[1]);
		break;
	case 9:
		strPrintTitle.Format(_T("%s별 가격합산"), m_strUseCodeAlias[2]);
		break;
	default:
		return ;
	}

	mgr.InitPrintData();
	mgr.AddPrintData(_T("제목"), strPrintTitle);
	mgr.AddPrintData(_T("가격합산구분"), GetPriceSumType());
	strCondition = GetPriceSumCondition();
	if( strCondition.IsEmpty() == FALSE )		mgr.AddPrintData(_T("조건"), strCondition);
	mgr.SetPrintDM(m_pDM);
	mgr.SetPrintType(BO_ACQ_PRICESUM);
	mgr.SetSIReportFilename(_T("K2UP_가격합산"));
	mgr.Print();
}

CString CPriceSumDlg::GetPriceSumType()
{
	CString strTmpData;
	if(m_nPriceDMType == GRID_SELECT) strTmpData = _T("그리드선정자료");
	else if(m_nPriceDMType == MONTHLY) strTmpData = _T("기간별가격합산");
	else if(m_nPriceDMType == DIVISION_NO) strTmpData = _T("차수별가격합산");
	return strTmpData;
}

CString CPriceSumDlg::GetPriceSumCondition()
{
	CString strTmpData, strDate1, strDate2, strGroupInfo;
	if(m_nPriceDMType == GRID_SELECT) strTmpData = _T("");
	else if(m_nPriceDMType == MONTHLY)
	{
		m_pCM->GetControlMgrData(_T("가격합산날짜구분"), strTmpData);
		if(strTmpData == _T("0")) strTmpData = _T("입력일 : ");
		else if(strTmpData == _T("1")) strTmpData = _T("주문일 : ");
		else if(strTmpData == _T("2")) strTmpData = _T("검수일 : ");
		m_pCM->GetControlMgrData(_T("날짜1"), strDate1);
		m_pCM->GetControlMgrData(_T("날짜2"), strDate2);
		if(strDate1.IsEmpty() && strDate2.IsEmpty()) strTmpData += _T("전체자료");
		else 
		{
			strTmpData += strDate1;
			if(!strDate2.IsEmpty()) strTmpData += _T(" - ") + strDate2;
		}
	}
	else if(m_nPriceDMType == DIVISION_NO)
	{
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		strTmpData.Format(_T("차수번호 : (%s)%s"), m_pGroupCollection->GetAcqYear(), strGroupInfo);
	}
	return strTmpData;
}

INT CPriceSumDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("금액"))
	{	
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	return 0;
}

VOID CPriceSumDlg::OnchkAPPLYDISCRATE() 
{
	// TODO: Add your control notification handler code here
	m_bApplyDiscRate = !m_bApplyDiscRate;
	UpdateData(FALSE);
	m_PriceMgr->SetApplyDiscRateYN(m_bApplyDiscRate);
	BOOL bSpeciesType = m_bSpeciesYN;
	if(m_OpType != GRID_SELECT) bSpeciesType = FALSE;
	INT ids = m_PriceMgr->InitTotalValue(bSpeciesType, m_OpType);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("총계정보를 설정 할 수 없습니다."));
		return ;
	}
	ids = MakePrintDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("가격합산 기능을 수행하지 못하였습니다."));
		return ;
	}
	SetTotalInfo();
	m_pCM->ControlDisplay(_T("MainGrid"));
}

INT CPriceSumDlg::MakePrintDM()
{
	INT ids;
	INT count = m_tabPriceSum.GetCurSel();
	CString str;

	switch( count )
	{
	case 0:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("KDC주제구분"), m_OpType);
		break;	
	case 1:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("LDC주제구분"), m_OpType);
		break;
	case 2:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("등록구분"), m_OpType);
		break;
	case 3:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("별치구분"), m_OpType);
		break;
	case 4:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("형식구분"), m_OpType);
		break;
	case 5:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("매체구분"), m_OpType);
		break;
	case 6:		ids = m_PriceMgr->MakePriceDM(count, m_bSpeciesYN, _T("보조등록구분"), m_OpType);
		break;
	case 7:
		{
			if( m_nUseCodeNo[0] == 1 )
			{
			ids = m_PriceMgr->MakePriceDM(6, m_bSpeciesYN, _T("도서분류코드1"), m_OpType);
			}
			else if( m_nUseCodeNo[0] == 2 )
			{
			ids = m_PriceMgr->MakePriceDM(6, m_bSpeciesYN, _T("도서분류코드2"), m_OpType);
			}
		else if(m_nUseCodeNo[0] == 3)
			{
			ids = m_PriceMgr->MakePriceDM(6, m_bSpeciesYN, _T("도서분류코드3"), m_OpType);
			}
		}
		break;
	case 8:
		{
		if(m_nUseCodeNo[1] == 1)
			{
			ids = m_PriceMgr->MakePriceDM(7, m_bSpeciesYN, _T("도서분류코드1"), m_OpType);
			}
		else if(m_nUseCodeNo[1] == 2)
			{
			ids = m_PriceMgr->MakePriceDM(7, m_bSpeciesYN, _T("도서분류코드2"), m_OpType);
			}
		else if(m_nUseCodeNo[1] == 3)
			{
			ids = m_PriceMgr->MakePriceDM(7, m_bSpeciesYN, _T("도서분류코드3"), m_OpType);
			}
		}
		break;
	case 9:
		{
		if(m_nUseCodeNo[2] == 1)
			{
			ids = m_PriceMgr->MakePriceDM(8, m_bSpeciesYN, _T("도서분류코드1"), m_OpType);
			}
		else if(m_nUseCodeNo[2] == 2)
			{
			ids = m_PriceMgr->MakePriceDM(8, m_bSpeciesYN, _T("도서분류코드2"), m_OpType);
			}
		else if(m_nUseCodeNo[2] == 3)
			{
			ids = m_PriceMgr->MakePriceDM(8, m_bSpeciesYN, _T("도서분류코드3"), m_OpType);
			}
		}
		break;
	default:		ids = -1;
		break;
	}	
	
	return ids;
}

INT CPriceSumDlg::SetTotalInfo()
{
	DOUBLE dPrice = 0.0;
	CString totalprice, totalbookcnt, totalspeciescnt, strTmpData;
	
	totalbookcnt.Format(_T("%d"), m_PriceMgr->m_TotalBookCnt);
	totalspeciescnt.Format(_T("%d"), m_PriceMgr->m_TotalSpeciesCnt);
	
	if		( m_strOrderPrice_Unit == _T("0") )		totalprice.Format(_T("%.0f"), m_PriceMgr->m_TotalPrice);
	else if ( m_strOrderPrice_Unit == _T("1") )		totalprice.Format(_T("%.1f"), m_PriceMgr->m_TotalPrice);
	else if ( m_strOrderPrice_Unit == _T("2") )		totalprice.Format(_T("%.2f"), m_PriceMgr->m_TotalPrice);
	else											totalprice.Format(_T("%.0f"), m_PriceMgr->m_TotalPrice);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(totalprice, totalprice);

	m_pCM->SetControlMgrData(_T("총책수"), totalbookcnt);
	m_pCM->SetControlMgrData(_T("총종수"), totalspeciescnt);
	m_pCM->SetControlMgrData(_T("가격총합"), totalprice);
	return 0;
}


VOID CPriceSumDlg::ShowDataProcessingDlg()
{
	if( m_pDataProcessingDlg == NULL ) 
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
	
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);

	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->SetMessage(_T("가격합산정보를 구축중입니다."));
}


VOID CPriceSumDlg::DeleteDataProcessingDlg()
{
	if( m_pDataProcessingDlg != NULL )
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

HBRUSH CPriceSumDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
