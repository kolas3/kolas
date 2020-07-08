// BO_PurchaseErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "BO_PurchaseErrorAdjust.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_PurchaseErrorAdjust dialog


CBO_PurchaseErrorAdjust::CBO_PurchaseErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_PurchaseErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_PurchaseErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pPrintDM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
}

CBO_PurchaseErrorAdjust::~CBO_PurchaseErrorAdjust()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
}

VOID CBO_PurchaseErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_PurchaseErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_PurchaseErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CBO_PurchaseErrorAdjust)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_PurchaseErrorAdjust message handlers

BOOL CBO_PurchaseErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_오류수정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_오류수정"));
	m_pDM = FindDM(_T("DM_오류수정"));
	m_pPrintDM = FindDM(_T("DM_오류수정_종"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	
	m_pSearchDlg = new CIndexRestructSearchDlg(this);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_PurchaseErrorAdjust::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

VOID CBO_PurchaseErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_ERROR_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 5, cx-10, cy-45);		
}

VOID CBO_PurchaseErrorAdjust::MakePurchaseErrorData(INT nOption)
{
	CString strQuery;
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND S.REC_KEY=B.SPECIES_KEY AND B.ACQ_CODE='1' AND B.ACQ_KEY IS NULL AND B.WORKING_STATUS < 'BOC'"));
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return ;
	m_pPrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	ids = mgr.SPFExecutePath(_T("책To종"));

	m_pGrid->Display();
}

VOID CBO_PurchaseErrorAdjust::AdjustPurchaseDataError()
{
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	INT ids = mgr.SPFExecutePath(_T("구입정보오류수정"));
	if(ids < 0)
	{
		this->ESLAfxMessageBox(_T("구입오류를 수정할 수 없습니다."));
		return ;
	}
	MakePurchaseErrorData(0);
}

VOID CBO_PurchaseErrorAdjust::SynchronizedBookToIndex()
{
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	INT ids = mgr.SPFExecutePath(_T("책색인정보동기화"));
	if(ids < 0)
	{
		this->ESLAfxMessageBox(_T("책색인정보동기화를 수행할 수 없습니다."));
		return ;
	}
	ESLAfxMessageBox(_T("책과 색인정보를 동기화시켰습니다."));
	return;
}

BOOL CBO_PurchaseErrorAdjust::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

VOID CBO_PurchaseErrorAdjust::RestructIndexField()
{
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	INT ids = mgr.SPFExecutePath(_T("색인재구축"));
	if(ids < 0)
	{
		this->ESLAfxMessageBox(_T("색인정보를 재구축 할 수 없습니다."));
		return ;
	}
	m_pGrid->Display();
}

VOID CBO_PurchaseErrorAdjust::ShowIndexRestructSearchDlg()
{
	if(!m_pSearchDlg->GetCreate()) m_pSearchDlg->Create(this);
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	m_pSearchDlg->CenterWindow();
}

UINT CBO_PurchaseErrorAdjust::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery);
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	INT ids = mgr.SPFExecutePath(_T("책To종"));
	
	m_pGrid->Display();
	
	return 0;
}

VOID CBO_PurchaseErrorAdjust::SearchWasteData()
{
	CString strQuery;
	
	m_pDM->RefreshDataManager(strQuery);
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)NULL);
	INT ids = mgr.SPFExecutePath(_T("책To종"));
	
	m_pGrid->Display();
	
	return;
}

VOID CBO_PurchaseErrorAdjust::DeleteWasteData()
{

}

HBRUSH CBO_PurchaseErrorAdjust::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
