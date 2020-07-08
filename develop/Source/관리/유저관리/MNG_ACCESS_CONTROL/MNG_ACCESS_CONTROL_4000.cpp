// MNG_ACCESS_CONTROL_4000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_4000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_ACCESS_CONTROL_1000.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_4000 dialog


CMNG_ACCESS_CONTROL_4000::CMNG_ACCESS_CONTROL_4000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ACCESS_CONTROL_4000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_ACCESS_CONTROL_4000::~CMNG_ACCESS_CONTROL_4000()
{
}


VOID CMNG_ACCESS_CONTROL_4000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ACCESS_CONTROL_4000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ACCESS_CONTROL_4000, CDialog)
	//{{AFX_MSG_MAP(CMNG_ACCESS_CONTROL_4000)
	ON_BN_CLICKED(IDC_bSELECT, OnbSELECT)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_4000 message handlers

BOOL CMNG_ACCESS_CONTROL_4000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("MNG_ACCESS_CONTROL_4000")) < 0) {
		AfxMessageBox(_T("화면 초기화에 실패하였습니다. : [MNG_ACCESS_CONTROL_4000]"));
		return TRUE;
	}

	m_nSelectedRow = -1;

	SetTimer(0, 100, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_ACCESS_CONTROL_4000::OnbSELECT() 
{
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_4000"), _T("목록"));
	if (pGrid) {
		m_nSelectedRow = pGrid->GetRow() - 1;
	}
	CDialog::OnOK();	
}

VOID CMNG_ACCESS_CONTROL_4000::OnbCLOSE() 
{
	CDialog::OnCancel();	
}

INT CMNG_ACCESS_CONTROL_4000::REFRESH_List(VOID)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_4000"));
	if (pDM == NULL) return -1;
	CESL_DataMgr *pParentDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pParentDM == NULL) return -1;
	CMNG_ACCESS_CONTROL_1000 *pDlg = (CMNG_ACCESS_CONTROL_1000 *)pParentMgr;
	CString strOption;
	strOption.Format(_T("SPF_USE_FLAG='y' AND FLAG='S' AND USER_ID='Administrator' AND MENU_ALIAS='%s' ORDER BY IDX"), pDlg->m_strMenuAlias);
	pDM->RefreshDataManager(strOption);

	INT i, j;
	INT nData = pParentDM->GetRowCount();
	INT nRows = pDM->GetRowCount();
	CString strAlias, strToolGroupAlias;
	CString strTmpAlias, strTmpToolGroupAlias;
	for (i = 0; i < nRows; i++) {
		strAlias = pDM->GetCellData(_T("ALIAS"), i);
		strToolGroupAlias = pDM->GetCellData(_T("TOOLGROUP_ALIAS"), i);
		for (j = 0; j < nData; j++) {
			strTmpAlias = pParentDM->GetCellData(_T("ALIAS"), j);
			strTmpToolGroupAlias = pParentDM->GetCellData(_T("TOOLGROUP_ALIAS"), j);
			if (strToolGroupAlias == strTmpToolGroupAlias && strAlias == strTmpAlias) {
				pDM->SetCellData(_T("DELETE"), _T("V"), i);
				break;
			}
		}
	}
	for (i = nRows-1; i >= 0; i--) {
		if (pDM->GetCellData(_T("DELETE"), i) == _T("V")) {
			pDM->DeleteRow(i);
		}
	}


	if (pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("더이상 추가할 수 있는 항목이 없습니다."));
		PostMessage(WM_CLOSE, NULL, NULL);
	} else {
		AllControlDisplay(_T("CM_MNG_ACCESS_CONTROL_4000"));
	}
	return 0;
}

VOID CMNG_ACCESS_CONTROL_4000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		REFRESH_List();
	}
	CDialog::OnTimer(nIDEvent);
}


HBRUSH CMNG_ACCESS_CONTROL_4000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

