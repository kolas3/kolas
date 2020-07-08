// PRICE_SUM_LIGHT.cpp : implementation file
//

#include "stdafx.h"
#include "PRICE_SUM_LIGHT.h"
#include "PriceSum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPRICE_SUM_LIGHT dialog


CPRICE_SUM_LIGHT::CPRICE_SUM_LIGHT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPRICE_SUM_LIGHT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPRICE_SUM_LIGHT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_OpenMode = -1;
	m_PriceMgr = NULL;
	m_OpType = 0;
	m_IsMissingData = FALSE;
	m_IsSpecies = FALSE;
}

CPRICE_SUM_LIGHT::~CPRICE_SUM_LIGHT()
{
	if(m_PriceMgr)
	{
		delete m_PriceMgr;
		m_PriceMgr = NULL;
	}
}

VOID CPRICE_SUM_LIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPRICE_SUM_LIGHT)
	DDX_Control(pDX, IDC_tabPRICE_SUM, m_tabPriceSum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPRICE_SUM_LIGHT, CDialog)
	//{{AFX_MSG_MAP(CPRICE_SUM_LIGHT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPRICE_SUM, OnSelchangetabPRICESUM)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_CBN_SELCHANGE(IDC_cmbPSUM_CLASS, OnSelchangecmbPSUMCLASS)
	ON_BN_CLICKED(IDC_btnPSUM, OnbtnPSUM)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPRICE_SUM_LIGHT message handlers
/*VOID CPRICE_SUM_LIGHT::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}
*/BOOL CPRICE_SUM_LIGHT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabPRICE_SUM);
	pTab->InsertItem(0, _T("KDC주제별"));
	pTab->InsertItem(1, _T("DDC주제별"));
	pTab->InsertItem(2, _T("등록구분별"));
	pTab->InsertItem(3, _T("별치구분별"));
	pTab->InsertItem(4, _T("형식구분별"));

	if (InitESL_Mgr(_T("K2UP_가격합산")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_PriceMgr = new CPriceSum(this);
	INT ids = m_PriceMgr->InitPriceSum(_T("CM_LIGHT_단행_수서"), m_tabCount, m_IsSpecies, m_IsMissingData);
	CString errstr;
	if(ids == 0)
	{
		CString totalprice, totalbookcnt;
		totalprice.Format(_T("%.2f"), m_PriceMgr->m_TotalPrice);
		totalbookcnt.Format(_T("%d"), m_PriceMgr->m_TotalBookCnt);
		this->SetControlData(_T("CM_가격합산"), _T("가격총합"), totalprice);
		this->SetControlData(_T("CM_가격합산"), _T("총책수"), totalbookcnt);

		this->AllControlDisplay(_T("CM_가격합산"));
		m_OpType = 1;
	}
	else m_OpType = -1;

	ShowCtrlByOpenMode();

	CString dateType[3][2] =
	{
		_T("0"), _T("입력일"),
		_T("1"), _T("주문일"),
		_T("2"), _T("검수일"),
	};

	CString priceSumType[2][2] =
	{
		_T("0"), _T("월별가격합산"),
		_T("1"), _T("차수간가격합산")
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(_T("CM_가격합산"), _T("입력일"), dateType, _T("가격합산날짜구분"), 3);
	mgr.SetMulTiComboBoxStrData(_T("CM_가격합산"), _T("월별가격합산"), priceSumType, _T("가격합산구분"), 2);

	// 그리드 정렬기능 해제
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_가격합산"), _T("MainGrid"));
	if(pGrid) pGrid->SetSort(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPRICE_SUM_LIGHT::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);	
}

VOID CPRICE_SUM_LIGHT::OnSelchangetabPRICESUM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT count = m_tabPriceSum.GetCurSel();
	switch(count)
	{
	case 0:
		m_PriceMgr->MakePriceDM(0, m_IsSpecies, _T("KDC주제구분"), m_OpType);
		break;
	case 1:
		m_PriceMgr->MakePriceDM(1, m_IsSpecies, _T("LDC주제구분"), m_OpType);
		break;
	case 2:
		m_PriceMgr->MakePriceDM(2, m_IsSpecies, _T("등록구분"), m_OpType);
		break;
	case 3:
		m_PriceMgr->MakePriceDM(3, m_IsSpecies, _T("별치구분"), m_OpType);
		break;
	case 4:
		m_PriceMgr->MakePriceDM(4, m_IsSpecies, _T("형식구분"), m_OpType);
		break;
	}

	AllControlDisplay(_T("CM_가격합산"));

	*pResult = 0;
}

VOID CPRICE_SUM_LIGHT::SetIsSpecies(BOOL isSpecies)
{
	m_IsSpecies = isSpecies;
}

VOID CPRICE_SUM_LIGHT::SetTabCount(INT tabCount)
{
	m_tabCount = tabCount;
}

VOID CPRICE_SUM_LIGHT::OnbPRINT() 
{
	// TODO: Add your control notification handler code here
	CBasicPrintMgr mgr(this);
	mgr.SetPrintDMAlias(_T("DM_가격합산"));
	mgr.SetMainCMAlias(_T("CM_가격합산"));
	mgr.SetPrintType(BO_ACQ_LIGHT_GRID_PRICESUM);
	INT tabCount = m_tabPriceSum.GetCurSel(); 
	if(tabCount == 0)
		mgr.SetSIReportFilename(_T("KDC주제별가격합산"));
	else if(tabCount == 1)
		mgr.SetSIReportFilename(_T("DDC주제별가격합산"));
	else if(tabCount == 2)
		mgr.SetSIReportFilename(_T("등록구분별가격합산"));
	else if(tabCount == 3)
		mgr.SetSIReportFilename(_T("별치구분별가격합산"));
	else if(tabCount == 4)
		mgr.SetSIReportFilename(_T("형식구분별가격합산"));
	mgr.Print();
}

VOID CPRICE_SUM_LIGHT::SetOpenMode(INT openMode)
{
	m_OpenMode = openMode;
}

VOID CPRICE_SUM_LIGHT::ShowCtrlByOpenMode()
{
	if(m_OpenMode < 0)
	{
		ESLAfxMessageBox(_T("먼저 가격합산 오픈모드를 설정해 주십시요."));
		EndDialog(IDCANCEL);
		return;
	}
	GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(TRUE);

	if(m_OpenMode == PRICE_SUM_LIGHT)
		GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(FALSE);
}

VOID CPRICE_SUM_LIGHT::OnSelchangecmbPSUMCLASS() 
{
	// TODO: Add your control notification handler code here
	CString priceSumClass = _T("");
	GetControlData(_T("CM_가격합산"), _T("가격합산구분"), priceSumClass);
	ShowCtrlByPriceSumClass(priceSumClass);
}

VOID CPRICE_SUM_LIGHT::ShowCtrlByPriceSumClass(CString priceSumClass)
{
	GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_SHOW);
	if(priceSumClass == _T("0"))
	{
		GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);	
	}
	else if(priceSumClass == _T("1"))
	{
		GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_HIDE);
	}
	else
		return;
}

VOID CPRICE_SUM_LIGHT::OnbtnPSUM() 
{
	// TODO: Add your control notification handler code here
	m_IsSpecies = FALSE;
	INT count = m_tabPriceSum.GetCurSel();
	INT ids = 0;
	switch(count)
	{
	case 0:
		ids = m_PriceMgr->SetReckeyList(_T("CM_가격합산"), _T("KDC주제구분"));
		break;
	case 1:
		ids = m_PriceMgr->SetReckeyList(_T("CM_가격합산"), _T("LDC주제구분"));
		break;
	case 2:
		ids = m_PriceMgr->SetReckeyList(_T("CM_가격합산"), _T("등록구분"));
		break;
	case 3:
		ids = m_PriceMgr->SetReckeyList(_T("CM_가격합산"), _T("별치구분"));
		break;
	case 4:
		ids = m_PriceMgr->SetReckeyList(_T("CM_가격합산"), _T("형식구분"));
		break;
	}
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("가격합산을 위한 DB작업에 문제가 있습니다."));
		return;
	}
	else if( ids > 0) return ;
	CString totalprice, totalbookcnt;
	totalprice.Format(_T("%.2f"), m_PriceMgr->m_TotalPrice);
	totalbookcnt.Format(_T("%d"), m_PriceMgr->m_TotalBookCnt);
	this->SetControlData(_T("CM_가격합산"), _T("가격총합"), totalprice);
	this->SetControlData(_T("CM_가격합산"), _T("총책수"), totalbookcnt);

	this->AllControlDisplay(_T("CM_가격합산"));
	m_OpType = -1;
}

VOID CPRICE_SUM_LIGHT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
}

VOID CPRICE_SUM_LIGHT::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}
