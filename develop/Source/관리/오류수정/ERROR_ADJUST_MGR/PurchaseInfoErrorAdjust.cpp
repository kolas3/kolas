// PurchaseInfoErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseInfoErrorAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoErrorAdjust dialog


CPurchaseInfoErrorAdjust::CPurchaseInfoErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CPurchaseInfoErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchaseInfoErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintDM = NULL;
}

CPurchaseInfoErrorAdjust::~CPurchaseInfoErrorAdjust()
{
	if(m_pSearchMgr)
	{
		delete m_pSearchMgr;
		m_pSearchMgr = NULL;
	}
	if(m_pErrorAdjustApi)
	{
		delete m_pErrorAdjustApi;
		m_pErrorAdjustApi = NULL;
	}
}

VOID CPurchaseInfoErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseInfoErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchaseInfoErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CPurchaseInfoErrorAdjust)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoErrorAdjust message handlers

BOOL CPurchaseInfoErrorAdjust::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CPurchaseInfoErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (InitESL_Mgr(_T("K2UPGRADE_구입정보오류수정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_구입정보오류수정"));
	m_pDM = FindDM(_T("DM_구입정보오류수정"));
	m_pPrintDM = FindDM(_T("DM_구입정보오류수정_종"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	m_pSearchMgr = new CErrorAdjustSearchMgr(0, this);
	m_pSearchMgr->SetErrorAdjustType(PURCHASEINFO_ERROR_ADJUST);
	
	m_pErrorAdjustApi = new CErrorAdjustApi(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchaseInfoErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASEINFO_ERROR_ADJUST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);	
}

INT CPurchaseInfoErrorAdjust::CMD_SEARCH()
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색쿼리를 생성할 수 없습니다."));
		return -1;
	}
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("구입정보 오류자료를 검색하고 있습니다."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색을 수행할 수 없습니다."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색결과가 없습니다."));
	}
	else
	{
		m_pDataProcessingDlg->SetMessage(_T("구입정보 오류자료를 종단위로 재 구축중입니다."));
		m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
		m_pErrorAdjustApi->SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
		m_pErrorAdjustApi->SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
		m_pErrorAdjustApi->SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = m_pErrorAdjustApi->SPFExecutePath(_T("책To종"));
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("종 자료를 구축할 수 없습니다."));
		}
	}
	DeleteDataProcessingDlg();
	m_pGrid->Display();
	return 0;
}

INT CPurchaseInfoErrorAdjust::CMD_REFRESH()
{
	return CMD_SEARCH();
}

INT CPurchaseInfoErrorAdjust::CMD_ERRORADJUST()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("구입정보를 생성할 수 없습니다."));
		return -1;
	}
	INT ids = CheckSelectDataValid(m_pPrintDM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("선정된 자료에 대한 새로운 구입정보를 생성합니다."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	m_pErrorAdjustApi->SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = m_pErrorAdjustApi->SPFExecutePath(_T("구입정보구축"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("구입정보를 생성하는 도중 에러가 발생하였습니다."));
		return ids;
	}
	
	m_pGrid->Display();
	return 0;
}
