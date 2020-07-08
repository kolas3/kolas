// MNG_USER_SHORTCUT_3000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_SHORTCUT_3000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_3000 dialog


CMNG_USER_SHORTCUT_3000::CMNG_USER_SHORTCUT_3000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_SHORTCUT_3000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strTOPMENUGROUP_ALIAS = _T("");
}

CMNG_USER_SHORTCUT_3000::~CMNG_USER_SHORTCUT_3000()
{	
	
}


VOID CMNG_USER_SHORTCUT_3000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_SHORTCUT_3000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_SHORTCUT_3000, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_SHORTCUT_3000)
	ON_BN_CLICKED(IDC_bSHORTCUT_CANCEL, OnbSHORTCUTCANCEL)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_3000 message handlers

BOOL CMNG_USER_SHORTCUT_3000::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( InitESL_Mgr( _T("MNG_USER_SHORTCUT_3000") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_SHORTCUT_3000") );
		return false;
	}
	
	RefreshData();	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_USER_SHORTCUT_3000::RefreshData(VOID)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_3000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_3000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return -1;
	}

	ids = pDM->RefreshDataManager(_T(""));
	pGrid->Display();

	return 0;
}

VOID CMNG_USER_SHORTCUT_3000::OnOK() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_3000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return;
	}
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_3000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return;
	}

	if (pDM->GetRowCount() == 0) {
		m_strTOPMENUGROUP_ALIAS = _T("");
		CDialog::OnCancel();
		return;
	}

	m_strTOPMENUGROUP_ALIAS = pDM->GetCellData(_T("ALIAS"), pGrid->GetRow() - 1);
	CDialog::OnOK();
}

VOID CMNG_USER_SHORTCUT_3000::OnCancel() 
{
	
	CDialog::OnCancel();
}

BEGIN_EVENTSINK_MAP(CMNG_USER_SHORTCUT_3000, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_USER_SHORTCUT_3000)
	ON_EVENT(CMNG_USER_SHORTCUT_3000, IDC_gLIST, -605 /* MouseDown */, OnMouseDowngLIST, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_USER_SHORTCUT_3000::OnMouseDowngLIST(short Button, short Shift, long x, long y) 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_3000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pGrid) {
		pGrid->SetReverse(pGrid->GetRow()-1);
	}
}

VOID CMNG_USER_SHORTCUT_3000::OnbSHORTCUTCANCEL() 
{
	m_strTOPMENUGROUP_ALIAS = _T("");
	CDialog::OnOK();
}


HBRUSH CMNG_USER_SHORTCUT_3000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
