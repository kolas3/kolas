// SearchResult.cpp : implementation file
//

#include "stdafx.h"
#include "shelf_loc_register.h"
#include "SearchResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchResult dialog


CSearchResult::CSearchResult(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSearchResult::~CSearchResult()
{
	
}


void CSearchResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchResult)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchResult, CDialog)
	//{{AFX_MSG_MAP(CSearchResult)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchResult message handlers

BOOL CSearchResult::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_RESULT_BO")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_SHELF_LOC_REG_RESULT_BO"));
	pCM->AllControlDisplay();

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SetSort(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSearchResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
