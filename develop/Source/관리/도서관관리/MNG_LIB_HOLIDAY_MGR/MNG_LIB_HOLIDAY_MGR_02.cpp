// MNG_LIB_HOLIDAY_MGR_02.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIB_HOLIDAY_MGR_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_LIB_HOLIDAY_MGR_04.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_02 dialog


CMNG_LIB_HOLIDAY_MGR_02::CMNG_LIB_HOLIDAY_MGR_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOLIDAY_MGR_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_LIB_HOLIDAY_MGR_02::~CMNG_LIB_HOLIDAY_MGR_02()
{

} 

VOID CMNG_LIB_HOLIDAY_MGR_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOLIDAY_MGR_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOLIDAY_MGR_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOLIDAY_MGR_02)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_02 message handlers

VOID CMNG_LIB_HOLIDAY_MGR_02::OnbINSERT() 
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_HOLIDAY_MGR_02") );
	if (pDM == NULL) return;
	INT ids;
	CMNG_LIB_HOLIDAY_MGR_04	dlg;
	dlg.MODE = _T("INSERT");
	if (dlg.DoModal() != IDOK) return;

	CString strMonth, strDay;
	strMonth.Format(_T("%d"), dlg.m_nMonth);
	strDay.Format(_T("%d"), dlg.m_nDay);

	CString strRecKey;
	pDM->MakeRecKey(strRecKey);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY")	, _T("NUMERIC")	, strRecKey);
	pDM->AddDBFieldData(_T("MONTH")		, _T("NUMERIC")	, strMonth);
	pDM->AddDBFieldData(_T("DAY")		, _T("NUMERIC")	, strDay);
	pDM->AddDBFieldData(_T("NAME")		, _T("STRING")	, dlg.m_strDesc);
	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("휴관일관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeInsertFrame(_T("MN_LIB_LEGAL_HOLIDAY_TBL"));
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("DB 작업중 오류가 발생하였습니다."));
	} else {
		RefreshList();
		AfxMessageBox(_T("입력되었습니다."));
	}
	return;
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnbMODIFY() 
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pDM == NULL) return;
	if (pDM->GetRowCount() == 0) return;
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pCM == NULL) return;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("공휴일목록"));
	if (pGrid == NULL) return;

	INT nRow = pGrid->GetRow() -1;
	CString strMonth, strDay, strRecKey, strName;
	pDM->GetCellData(_T("MONTH"), nRow, strMonth);
	strMonth.TrimLeft();		strMonth.TrimRight();
	pDM->GetCellData(_T("DAY"), nRow, strDay);
	strDay.TrimLeft();			strDay.TrimRight();
	pDM->GetCellData(_T("REC_KEY"), nRow, strRecKey);
	pDM->GetCellData(_T("NAME"), nRow, strName);

	CMNG_LIB_HOLIDAY_MGR_04	dlg;
	dlg.MODE = _T("MODIFY");
	if (strMonth.GetLength() > 0) dlg.m_nMonth = _ttoi(strMonth);
	if (strDay.GetLength() > 0) dlg.m_nDay = _ttoi(strDay);
	dlg.m_strDesc = strName;
	if (dlg.DoModal() != IDOK) return;
	strMonth.Format(_T("%d"), dlg.m_nMonth);
	strDay.Format(_T("%d"), dlg.m_nDay);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MONTH")		, _T("NUMERIC")	, strMonth);
	pDM->AddDBFieldData(_T("DAY")		, _T("NUMERIC")	, strDay);
	pDM->AddDBFieldData(_T("NAME")		, _T("STRING")	, dlg.m_strDesc);
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("휴관일관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame(_T("MN_LIB_LEGAL_HOLIDAY_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("DB 작업중 오류가 발생하였습니다."));
	} else {
		RefreshList();
		AfxMessageBox(_T("수정되었습니다."));
	}
	return;
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnbDELETE() 
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pDM == NULL) return;
	if (pDM->GetRowCount() == 0) return;
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pCM == NULL) return;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("공휴일목록"));
	if (pGrid == NULL) return;

	INT nRow = pGrid->GetRow() -1;
	CString strRecKey;
	pDM->GetCellData(_T("REC_KEY"), nRow, strRecKey);
	strRecKey.TrimLeft();		strRecKey.TrimRight();
	if (AfxMessageBox(_T("선택하신 자료를 삭제하시겠습니까?"), MB_YESNO) != IDYES) return;

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->MakeDeleteFrame(_T("MN_LIB_LEGAL_HOLIDAY_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("DB 작업중 오류가 발생하였습니다."));
	} else {
		RefreshList();
		AfxMessageBox(_T("삭제되었습니다."));
	}
	return;
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnbCLOSE() 
{
	CDialog::OnOK();	
}

BOOL CMNG_LIB_HOLIDAY_MGR_02::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( InitESL_Mgr( _T("MNG_LIB_HOLIDAY_MGR_02") ) < 0 ) 
	{
		AfxMessageBox( _T("화면을 초기화 하지 못하였습니다.") );
	}
	SetTimer(0, 100, NULL);


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CMNG_LIB_HOLIDAY_MGR_02, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_LIB_HOLIDAY_MGR_02)
	ON_EVENT(CMNG_LIB_HOLIDAY_MGR_02, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CMNG_LIB_HOLIDAY_MGR_02::RefreshList(VOID)
{
	INT ids;
	CString strMsg;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pDM == NULL) return -1;

	ids = pDM->RefreshDataManager(_T(""));
	if (ids < 0) {
		strMsg.Format(_T("검색중 에러가 발생하였습니다.[%d]"), ids);
		AfxMessageBox(strMsg);
		return -2;
	}

	AllControlDisplay(_T("CM_MNG_LIB_HOLIDAY_MGR_02"));
	return 0;
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnbSEARCH() 
{
	RefreshList();	
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnClickmngGRID1() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pDM == NULL) return;
	if (pDM->GetRowCount() == 0) return;
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_LIB_HOLIDAY_MGR_02"));
	if (pCM == NULL) return;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("공휴일목록"));
	if (pGrid == NULL) return;

	INT nRow = pGrid->GetRow();
	pGrid->SetReverse(nRow-1);
}

VOID CMNG_LIB_HOLIDAY_MGR_02::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		RefreshList();
	}
	CDialog::OnTimer(nIDEvent);
}



HBRUSH CMNG_LIB_HOLIDAY_MGR_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

