// MNG_ACCESS_CONTROL_2000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_2000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_2000 dialog


CMNG_ACCESS_CONTROL_2000::CMNG_ACCESS_CONTROL_2000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ACCESS_CONTROL_2000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_ACCESS_CONTROL_2000::~CMNG_ACCESS_CONTROL_2000()
{
}


VOID CMNG_ACCESS_CONTROL_2000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ACCESS_CONTROL_2000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ACCESS_CONTROL_2000, CDialog)
	//{{AFX_MSG_MAP(CMNG_ACCESS_CONTROL_2000)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTITLE, OnSelchangetabTITLE)
	ON_BN_CLICKED(IDC_bSELECT, OnbSELECT)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_2000 message handlers

BOOL CMNG_ACCESS_CONTROL_2000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("MNG_ACCESS_CONTROL_2000")) < 0) {
		AfxMessageBox(_T("화면 초기화에 실패하였습니다. : [MNG_ACCESS_CONTROL_2000]"));
		return TRUE;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_사용자"));
	if (pDM == NULL) return -1;
	pDM->RefreshDataManager(_T(""));

	SetInitControl();
	ViewSearchList();

	SetTimer(0, 100, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_ACCESS_CONTROL_2000::SetInitControl()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);

	pTab->InsertItem( 0 , _T("사용자 검색") );
	pTab->InsertItem( 1 , _T("디폴트권한 검색") );

	return 0;
}

INT CMNG_ACCESS_CONTROL_2000::ViewSearchList()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cSEARCH_LIST);
	pCombo->ResetContent();

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_사용자"));
	if (pDM == NULL) return -1;

	CString strTmp, strFlag;

	INT nCount = pDM->GetRowCount();
/*
	if( pTab->GetCurSel() == 0 ) {
		pCombo->AddString(_T("Administrator"));
	}
*/
	for (INT i = 0;  i < nCount; i++)
	{
		strFlag = pDM->GetCellData(_T("FLAG"), i);
		strFlag.TrimLeft();		strFlag.TrimRight();
		strFlag.MakeUpper();
		if( pTab->GetCurSel() == 0 ) {
			if (strFlag == _T("U") && pDM->GetCellData(_T("USER_ID"), i) == _T("ECO8")) continue;
			if (strFlag == _T("U") || strFlag == _T("S")) {
				pCombo->AddString(pDM->GetCellData(_T("USER_ID"), i));
			}
		} 
		if( pTab->GetCurSel() == 1 ) {
			if (strFlag == _T("M")) {
				pCombo->AddString(pDM->GetCellData(_T("METHOD_NAME"), i));
			}
		} 
	}
	pCombo->SetCurSel(0);

	return 0;
}

VOID CMNG_ACCESS_CONTROL_2000::OnSelchangetabTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ViewSearchList();

	*pResult = 0;
}

VOID CMNG_ACCESS_CONTROL_2000::OnbSELECT() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cSEARCH_LIST);
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);

	FLAG = 'U';
	ID = _T("");
	if (pCombo != NULL && pTab != NULL) {
		pCombo->GetWindowText(ID);
		ID.TrimLeft();				ID.TrimRight();
		if (pTab->GetCurSel() == 0) FLAG = 'U';
		if (pTab->GetCurSel() == 1) FLAG = 'M';

		if (FLAG == 'U' && ID == _T("Administrator")) FLAG = 'S';
	}

	CDialog::OnOK();	
}

VOID CMNG_ACCESS_CONTROL_2000::OnbCLOSE() 
{
	CDialog::OnCancel();
}

VOID CMNG_ACCESS_CONTROL_2000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		
		CString strLEVEL = GetUserLEVEL();
		strLEVEL.TrimLeft();		strLEVEL.TrimRight();
		strLEVEL.MakeUpper();
		CString strID = GetUserID();
		strID.TrimLeft();			strID.TrimRight();
		if (strLEVEL == _T("U")) {
			FLAG = 'U';
			ID = strID;
			CDialog::OnOK();
		}
	}
	CDialog::OnTimer(nIDEvent);
}



HBRUSH CMNG_ACCESS_CONTROL_2000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}