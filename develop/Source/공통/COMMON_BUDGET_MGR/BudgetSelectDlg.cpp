// BudgetSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetSelectDlg dialog


CBudgetSelectDlg::CBudgetSelectDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;

	m_nCurrentIdx = -1;
	m_strBUDGET_COMPILATION_KEY = _T("");
}

CBudgetSelectDlg::~CBudgetSelectDlg()
{
}

VOID CBudgetSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CBudgetSelectDlg)
	ON_BN_CLICKED(IDC_btnBUDGET_SELECT_SEARCH, OnbtnBUDGETSELECTSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetSelectDlg message handlers

BOOL CBudgetSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr( _T("예산관리_예산편성_선택") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성_선택") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_예산편성_선택"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_예산편성_선택"));
	
	if(!m_pDM || !m_pCM ||!m_pGrid) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_예산편성_선택,DM_예산편성_선택,그리드") );
		return FALSE;
	}

	m_pCM->SetControlMgrData( _T("예산편성상태"), _T("1"));
	OnbtnBUDGETSELECTSEARCH();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CBudgetSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetSelectDlg)
	ON_EVENT(CBudgetSelectDlg, IDC_gridBUDGET_SELECT, -600 /* Click */, OnClickgridBUDGETSELECT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetSelectDlg::OnClickgridBUDGETSELECT() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
		}
	
	}
}

VOID CBudgetSelectDlg::OnbtnBUDGETSELECTSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strYear,strClass, strStatus, strSubClass;
	m_pCM->GetControlMgrData(_T("예산편성년도"),strYear);
	m_pCM->GetControlMgrData(_T("예산편성구분"),strClass);
	m_pCM->GetControlMgrData(_T("예산편성상태"),strStatus);
	m_pCM->GetControlMgrData(_T("예산편성보조구분"),strSubClass);


	CString strQuery;
	CString tmpquery;

	BOOL bNeedAnd= FALSE;
	if(!strYear.IsEmpty())
	{
		tmpquery.Format(_T("BUDGET_YEAR = '%s'"),strYear);
		strQuery += tmpquery;
		bNeedAnd= TRUE;
	}

	if(!strClass.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_CLASS = '%s'"),strClass);
		strQuery += tmpquery;
	}

	if(!strStatus.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_STATUS = '%s'"),strStatus);
		strQuery += tmpquery;
	}

	if(!strSubClass.IsEmpty())
	{
		if(bNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_SUB_CLASS = '%s'"),strSubClass);
		strQuery += tmpquery;
	}

	m_pDM->RefreshDataManager(strQuery);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성보조구분"),_T("UDF_예산편성보조구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성구분"),_T("UDF_예산편성구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성상태"),_T("UDF_예산편성상태"));
	
	m_nCurrentIdx = -1;
	m_pGrid->Display();
}

VOID CBudgetSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// 적용 함수
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	m_strBUDGET_COMPILATION_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);

	CDialog::OnOK();
}

HBRUSH CBudgetSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
