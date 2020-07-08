// SearchResult_SE.cpp : implementation file
//

#include "stdafx.h"
#include "shelf_loc_register.h"
#include "SearchResult_SE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchResult_SE dialog


CSearchResult_SE::CSearchResult_SE(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchResult_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSearchResult_SE::~CSearchResult_SE()
{
}

void CSearchResult_SE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchResult_SE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchResult_SE, CDialog)
	//{{AFX_MSG_MAP(CSearchResult_SE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchResult_SE message handlers

HBRUSH CSearchResult_SE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSearchResult_SE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_RESULT_SE")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_SHELF_LOC_REG_RESULT_SE"));
	pCM->AllControlDisplay();

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SetSort(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
