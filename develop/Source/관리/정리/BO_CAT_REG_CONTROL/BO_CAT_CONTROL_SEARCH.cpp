// BO_CAT_CONTROL_SEARCH.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_CONTROL_SEARCH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CONTROL_SEARCH dialog


CBO_CAT_CONTROL_SEARCH::CBO_CAT_CONTROL_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_CONTROL_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_CONTROL_SEARCH::~CBO_CAT_CONTROL_SEARCH()
{
}

VOID CBO_CAT_CONTROL_SEARCH::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_CONTROL_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_CONTROL_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_CONTROL_SEARCH)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CONTROL_SEARCH message handlers

BOOL CBO_CAT_CONTROL_SEARCH::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_CONTROL_SEARCH::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_CONTROL")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_CONTROL_SEARCH::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_REG_CONTROL);
	if (pGrid->GetSafeHwnd() == NULL) return;

//	CButton *pButton = (CButton*)GetDlgItem(IDC_bDELETE);
//	if (pButton->GetSafeHwnd() == NULL) return;
	
//	pButton->SetWindowPos(NULL, cx/2, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	//pGrid->MoveWindow(-2, 30, cx+4, cy-28);
	pGrid->MoveWindow(-2, 35, cx+2, cy-32);

EFS_END
}


HBRUSH CBO_CAT_CONTROL_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
