// BO_ACQ_BasicDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_BasicDataMgr.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"

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
	m_pPrintDM = NULL;
	m_pSearchDlg = NULL;
}

CBO_ACQ_BasicDataMgr::~CBO_ACQ_BasicDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
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
	if (InitESL_Mgr(_T("K2UPGRADE_기초자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_기초자료관리"));
	m_pDM = FindDM(_T("DM_기초자료관리"));
	m_pPrintDM = FindDM(_T("DM_기초자료관리_종"));
	
	GetDlgItem(IDC_stcGROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUPNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcSEARCH_RESULT)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_BasicDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_STATIC);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(5, 5, cx-10, 30);

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcGROUPNAME);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcSEARCH_RESULT);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(220, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtGROUPNAME);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(73, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBASIC_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 40, cx-10, cy-45);		
}

BOOL CBO_ACQ_BasicDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_BasicDataMgr::ShowSearchDlg()
{
	if(m_pSearchDlg == NULL)
		m_pSearchDlg = new CSearchMgr(this, BO_ACQ_BASIC, ACQ_TYPE_INVALID, m_pDM->CONNECTION_INFO);
	else
		m_pSearchDlg->ShowSearchDlg(m_pDM->CONNECTION_INFO);
}

VOID CBO_ACQ_BasicDataMgr::ShowPrintDlg()
{

}

VOID CBO_ACQ_BasicDataMgr::ShowBasicDataDlg(BOOL isInsert)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CSpecies_Form dlg(this);
	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_BASIC);
	if(isInsert) dlg.SetOpType(SINSERT);
	else dlg.SetOpType(SUPDATE);
	dlg.SetParentSpeciesDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();
}

VOID CBO_ACQ_BasicDataMgr::DeleteBasicData()
{

}

VOID CBO_ACQ_BasicDataMgr::PurchaseBasicData()
{

}

VOID CBO_ACQ_BasicDataMgr::DonateBasicData()
{

}

VOID CBO_ACQ_BasicDataMgr::ShowChangeCodeDlg()
{
	CCodeChange dlg(this);
	dlg.DoModal();
}

VOID CBO_ACQ_BasicDataMgr::ShowGroupMgrDlg()
{
	CGroup_Mgr mgr(this);
	mgr.SetOpenMode(0);
	mgr.DoModal();
}

UINT CBO_ACQ_BasicDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strMsg;
	GetDlgItem(IDC_stcSEARCH_RESULT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcSEARCH_RESULT)->SetWindowText(_T("기초자료를 검색하고 있습니다."));
	GetDlgItem(IDC_stcSEARCH_RESULT)->ShowWindow(SW_SHOW);
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery);
	INT rowCnt = m_pDM->GetRowCount();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM);
	INT ids = mgr.SPFExecutePath(_T("책To종"));
	SetBasicDataInfo();
	DisplayGrid(m_pCM->CM_ALIAS, _T("MainGrid"));
	return 0;
}

INT CBO_ACQ_BasicDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	return 0;
}

VOID CBO_ACQ_BasicDataMgr::DisplayGrid(CString cmAlias, CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(cmAlias, gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

VOID CBO_ACQ_BasicDataMgr::SetBasicDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt == 0)
		strMsg.Format(_T("검색된 기초자료가 없습니다."));
	else
		strMsg.Format(_T("%d건의 기초자료가 검색되었습니다."), nRowCnt);
	GetDlgItem(IDC_stcSEARCH_RESULT)->SetWindowText(strMsg);
}