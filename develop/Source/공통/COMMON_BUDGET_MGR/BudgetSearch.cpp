// BudgetSearch.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetSearch dialog


CBudgetSearch::CBudgetSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMgr = pParentMgr;
	m_pCM = NULL;
	m_strQuery = _T("");


}
CBudgetSearch::~CBudgetSearch()
{
}

VOID CBudgetSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CBudgetSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetSearch, CDialog)
	//{{AFX_MSG_MAP(CBudgetSearch)
	ON_BN_CLICKED(IDC_btnBUDGET_SEARCH_CLEAR, OnbtnBUDGETSEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetSearch message handlers

BOOL CBudgetSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("예산관리_검색") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_검색") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_예산관리_검색"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("CM Error : CM_예산관리_검색") );
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CBudgetSearch::MakeQuery()
{
	CString strBUDGET_YEAR = _T("");				m_pCM->GetControlMgrData(_T("예산편성년도"),strBUDGET_YEAR);
	CString strBUDGET_CLASS = _T("");				m_pCM->GetControlMgrData(_T("예산편성구분"),strBUDGET_CLASS);
	CString strBUDGET_STATUS = _T("");				m_pCM->GetControlMgrData(_T("예산편성상태"),strBUDGET_STATUS);
	CString strBUDGET_NAME = _T("");				m_pCM->GetControlMgrData(_T("예산편성명"),strBUDGET_NAME);
	CString strBUDGET_SUBCLASS = _T("");				m_pCM->GetControlMgrData(_T("예산편성보조구분"),strBUDGET_SUBCLASS);
	
	
	strBUDGET_YEAR = TrimAll(strBUDGET_YEAR);
	strBUDGET_CLASS = TrimAll(strBUDGET_CLASS);
	strBUDGET_STATUS = TrimAll(strBUDGET_STATUS);
	strBUDGET_NAME = TrimAll(strBUDGET_NAME);
	strBUDGET_SUBCLASS = TrimAll(strBUDGET_SUBCLASS);

	
	BOOL bIsNeedAnd = FALSE;

	CString strQuery;
	CString tmpquery;


	//일치 검색 
	if(!strBUDGET_YEAR.IsEmpty())
	{
		tmpquery.Format(_T("BUDGET_YEAR = '%s'"), strBUDGET_YEAR);
		strQuery += tmpquery;
		bIsNeedAnd = TRUE;
	}
	
	if(!strBUDGET_CLASS.IsEmpty())
	{
		if(bIsNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_CLASS = '%s'"), strBUDGET_CLASS);
		strQuery += tmpquery;
		bIsNeedAnd = TRUE;
	}
	
	if(!strBUDGET_STATUS.IsEmpty())
	{
		if(bIsNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_STATUS = '%s'"), strBUDGET_STATUS);
		strQuery += tmpquery;
		bIsNeedAnd = TRUE;
	}
	if(!strBUDGET_SUBCLASS.IsEmpty())
	{
		if(bIsNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_SUB_CLASS = '%s'"), strBUDGET_SUBCLASS);
		strQuery += tmpquery;
		bIsNeedAnd = TRUE;
	}

	//LIKE 검색
	if(!strBUDGET_NAME.IsEmpty())
	{
		if(bIsNeedAnd) strQuery += _T(" AND ");
		tmpquery.Format(_T("BUDGET_NAME LIKE '%%%s%%'"), strBUDGET_NAME);
		strQuery += tmpquery;
		bIsNeedAnd = TRUE;
	}	

	m_strQuery = strQuery;

	return 0;
}

VOID CBudgetSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

VOID CBudgetSearch::OnOK() 
{
	// TODO: Add extra validation here
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;
	CDialog::OnOK();
}

VOID CBudgetSearch::OnbtnBUDGETSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	
	m_pCM->AllControlClear();

}

CString CBudgetSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

BOOL CBudgetSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnOK(); 
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBudgetSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
