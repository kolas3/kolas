// VolIndexErrorAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "VolIndexErrorAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolIndexErrorAdjust dialog


CVolIndexErrorAdjust::CVolIndexErrorAdjust(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustDlg(CVolIndexErrorAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolIndexErrorAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintDM = NULL;
}

CVolIndexErrorAdjust::~CVolIndexErrorAdjust()
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

VOID CVolIndexErrorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolIndexErrorAdjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolIndexErrorAdjust, CDialog)
	//{{AFX_MSG_MAP(CVolIndexErrorAdjust)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolIndexErrorAdjust message handlers

BOOL CVolIndexErrorAdjust::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CVolIndexErrorAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (InitESL_Mgr(_T("K2UPGRADE_책색인동기화")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_책색인동기화"));
	m_pDM = FindDM(_T("DM_책색인동기화"));
	m_pPrintDM = FindDM(_T("DM_책색인동기화_종"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	m_pSearchMgr = new CErrorAdjustSearchMgr(0, this);
	m_pSearchMgr->SetErrorAdjustType(VOLINDEX_ERROR_ADJUST);

	m_pErrorAdjustApi = new CErrorAdjustApi(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CVolIndexErrorAdjust::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridVOLINDEX_ERROR_ADJUST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	//rect.top += 5;
	//rect.bottom -= 5;
	//rect.left += 5;
	//rect.right -= 5;
	
	pGrid->MoveWindow(rect);	
}

INT CVolIndexErrorAdjust::CMD_SEARCH()
{
	CString strQuery;
	strQuery = m_pSearchMgr->GetQuery();
	if(strQuery.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색쿼리를 생성할 수 없습니다."));
		return -1;
	}
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("책-색인 동기화자료를 검색중 입니다."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	INT ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색을 수행할 수 없습니다."));
	}
	else if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
	}
	else 
	{
		m_pDataProcessingDlg->SetMessage(_T("책-색인 동기화자료를 종단위로 재 구축중입니다."));

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

INT CVolIndexErrorAdjust::CMD_REFRESH()
{
	return CMD_SEARCH();
}

INT CVolIndexErrorAdjust::CMD_ERRORADJUST()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("책-색인정보 동기화 작업을 수행할 수 없습니다."));
		return -1;
	}
	INT ids = CheckSelectDataValid(m_pPrintDM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("선정된 자료의 권/색인정보 동기화 작업을 수행합니다."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	m_pErrorAdjustApi->SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = m_pErrorAdjustApi->SPFExecutePath(_T("책색인동기화"));
	
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책-색인정보 동기화 작업을 수행하는 도중 에러가 발생하였습니다."));
		return ids;
	}
	
	m_pGrid->Display();
	return 0;
}

INT CVolIndexErrorAdjust::CMD_ERRORADJUST1()
{
	if(m_pErrorAdjustApi == NULL)
	{
		ESLAfxMessageBox(_T("책-색인정보 동기화 작업을 수행할 수 없습니다."));
		return -1;
	}
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("책/부록 플래그가 NULL인 자료를 'B'로 설정합니다."));
	
	m_pErrorAdjustApi->InitTempData();	m_pErrorAdjustApi->InitTempPtr();
	m_pErrorAdjustApi->SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	m_pErrorAdjustApi->SetTempPtr(_T("ProgressCTRL"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = m_pErrorAdjustApi->SPFExecutePath(_T("책부록플래그설정"));
	DeleteDataProcessingDlg();
	if(ids == 1000)
	{
		ESLAfxMessageBox(_T("책/부록 플래그를 설정할 자료가 없습니다."));
		return 0;
	}
	else if(ids < 0)
	{
		ESLAfxMessageBox(_T("책/부록 플래그를 설정할 수 없습니다."));
		return 0;
	}

	ESLAfxMessageBox(_T("책/부록 플래그가 NULL인 자료를 'B'로 설정하였습니다."));
	return 0;
}