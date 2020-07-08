// BO_ACQ_Purchase_Statistics_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_Purchase_Statistics_Mgr.h"
#include "..\..\..\관리\오류수정\ERROR_ADJUST_MGR\ErrorAdjustLauncher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_Purchase_Statistics_Mgr dialog


CBO_ACQ_Purchase_Statistics_Mgr::CBO_ACQ_Purchase_Statistics_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CStatistics(CBO_ACQ_Purchase_Statistics_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_Purchase_Statistics_Mgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_ACQ_Purchase_Statistics_Mgr::~CBO_ACQ_Purchase_Statistics_Mgr()
{
/*
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}
	if(m_pConditionDlg)
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
*/
}

VOID CBO_ACQ_Purchase_Statistics_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_Purchase_Statistics_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_Purchase_Statistics_Mgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_Purchase_Statistics_Mgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(CHANGE_CLASS_NO_TYPE, OnChangeClassNoType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_Purchase_Statistics_Mgr message handlers

BOOL CBO_ACQ_Purchase_Statistics_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIs90 = _T(""), strSM;
	
	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T(""), strIs90);
	/*
	if( _T("Y") == strIs90 )
	{
		strSM = _T("K2UPGRADE_구입통계_국방대");
	}
	else*/
	{
		strSM = _T("K2UPGRADE_구입통계");
	}
	
	if( InitESL_Mgr(strSM) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	/*
	// CM/DM/Grid를 등록한다.	
	if( _T("Y") == strIs90 )
	{
		SetCM(FindCM(_T("CM_구입통계(국방대)")));
		SetDM(FindDM(_T("DM_통계자료(국방대)")));
	}
	else*/
	{
	SetCM(FindCM(_T("CM_구입통계")));
	SetDM(FindDM(_T("DM_통계자료")));
	}
	
	SetGrid((CESL_Grid*)GetCM()->FindControl(_T("MainGrid")));
	GetGrid()->SetSort(FALSE);
	
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_CONDITION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS_CONDITION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_UNIT)->ShowWindow(SW_HIDE);

	CreateConditionDlg(PURCHASE);
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_Purchase_Statistics_Mgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

VOID CBO_ACQ_Purchase_Statistics_Mgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_STATISTICS_DATA);
	//if(pBtn->GetSafeHwnd() == NULL) return;
	//pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_STATISTICS_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_STATISTICS_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_STATISTICS);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(170, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_STATISTICS);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(210, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_STATISTICS_CONDITION);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(390, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_STATISTICS_CONDITION);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(430, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_STATISTICS_UNIT);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(670, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_STATISTICS_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 35, cx+2, cy-30);
	pGrid->MoveWindow(-2, 40, cx+2, cy-38);		
	//==========================================			
}

UINT CBO_ACQ_Purchase_Statistics_Mgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	SetStatisticsInfo();
	DisplayGrid();
	return 0;
}

UINT CBO_ACQ_Purchase_Statistics_Mgr::OnChangeClassNoType(LPARAM lParam, WPARAM wParam)
{
	ChangeColumnHeaderOfSubjectCode();
	return 0;
}

VOID CBO_ACQ_Purchase_Statistics_Mgr::ShowTestFrameWnd()
{
	m_Launcher.LaunchErrorAdjustFrame(GetParentFrame(),this);
}
/*
VOID CBO_ACQ_Purchase_Statistics_Mgr::CreateConditionDlg()
{
	if(m_pConditionDlg)
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}

	m_pConditionDlg = new CStatisticsConditionDlg(this);
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_pCondition = new CStatisticsCondition(this);

	m_pConditionDlg->SetAcqType(PURCHASE);
	m_pConditionDlg->SetCondition(m_pCondition);
	m_pConditionDlg->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
}
*/

HBRUSH CBO_ACQ_Purchase_Statistics_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}