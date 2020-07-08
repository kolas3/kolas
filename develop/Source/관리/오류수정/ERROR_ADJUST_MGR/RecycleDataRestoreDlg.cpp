// RecycleDataRestoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecycleDataRestoreDlg.h"
#include "RecycleDataSearchDlg.h"
#include "RecycleDataDetailInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataRestoreDlg dialog


CRecycleDataRestoreDlg::CRecycleDataRestoreDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CRecycleDataRestoreDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecycleDataRestoreDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintDM = NULL;
}


CRecycleDataRestoreDlg::~CRecycleDataRestoreDlg()
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

VOID CRecycleDataRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycleDataRestoreDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycleDataRestoreDlg, CDialog)
	//{{AFX_MSG_MAP(CRecycleDataRestoreDlg)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()                                                                  

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataRestoreDlg message handlers

BOOL CRecycleDataRestoreDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (InitESL_Mgr(_T("K2UPGRADE_재활용자료복원")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_재활용자료복원"));
	m_pDM = FindDM(_T("DM_재활용자료복원"));
	m_pPrintDM = FindDM(_T("DM_재활용자료복원_종"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	

	m_pSearchMgr = new CRecycleDataSearchDlg(this);
	m_pSearchMgr->SetErrorAdjustType(RECYCLE_DATA_RESTORE);
	
	m_pErrorAdjustApi = new CErrorAdjustApi(this);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRecycleDataRestoreDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRECYCLE_DATA_RESTORE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);	
}

BOOL CRecycleDataRestoreDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CRecycleDataRestoreDlg::CMD_ERRORADJUST()
{
	CRecycleDataDetailInfo dlg(this);
	dlg.SetOpenMode(1000);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.SetErrorAdjustApi(m_pErrorAdjustApi);
	dlg.DoModal();
	return 0;
}

INT CRecycleDataRestoreDlg::CMD_SEARCH()
{
	INT ids = ShowSearchDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색화면을 생성할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CRecycleDataRestoreDlg::CMD_VIEWERRORINFO()
{
	return 0;
}

UINT CRecycleDataRestoreDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색 질의를 생성할 수 없습니다."));
		return -1;
	}
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("재활용 복원 자료를 검색중입니다."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색 질의를 수행할 수 없습니다."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
	}
	else 
	{
		m_pDataProcessingDlg->SetMessage(_T("재활용 복원 자료를 종단위로 재 구축중입니다."));
		
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
	return 0;
}

INT CRecycleDataRestoreDlg::ShowSearchDlg()
{
	if(m_pSearchMgr == NULL) return -1;
	
	if(!m_pSearchMgr->GetCreate())
	{
		if(!m_pSearchMgr->CreateSearchDlg(this))
			return -1;
	}
	
	m_pSearchMgr->ShowWindow(SW_SHOW);
	m_pSearchMgr->UpdateWindow();
	m_pSearchMgr->CenterWindow(this);
	
	return 0;
}


HBRUSH CRecycleDataRestoreDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


