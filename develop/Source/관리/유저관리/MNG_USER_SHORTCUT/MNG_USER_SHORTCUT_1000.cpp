// MNG_USER_SHORTCUT_1000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_SHORTCUT_1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_USER_SHORTCUT_2000.h"
#include "MNG_USER_SHORTCUT_3000.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_1000 dialog


CMNG_USER_SHORTCUT_1000::CMNG_USER_SHORTCUT_1000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_SHORTCUT_1000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CMNG_USER_SHORTCUT_1000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_SHORTCUT_1000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CMNG_USER_SHORTCUT_1000::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BEGIN_MESSAGE_MAP(CMNG_USER_SHORTCUT_1000, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_SHORTCUT_1000)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_1000 message handlers

BOOL CMNG_USER_SHORTCUT_1000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_USER_SHORTCUT_1000") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_SHORTCUT_1000") );
		return false;
	}
	
	SetTimer(0, 300, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_USER_SHORTCUT_1000::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RearrangeControl();
}

VOID CMNG_USER_SHORTCUT_1000::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	RearrangeControl();
}

INT CMNG_USER_SHORTCUT_1000::RearrangeControl(VOID)
{
	CWnd *pWnd;
	CRect rect;
	
	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_gLIST);
	if (pWnd) {
		//rect.top = rect.top + 5;
		//rect.bottom = rect.bottom - 5;
		//rect.left = rect.left + 5;
		//rect.right = rect.right - 5;

		pWnd->MoveWindow(&rect);
	}
	return 0;
}

INT CMNG_USER_SHORTCUT_1000::RefreshData(VOID)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_1000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_1000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return -1;
	}
/*
	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("r"));
	if (fd) {
		fclose(fd);
		pGrid->m_bGridEdit = TRUE;
	}
*/
	pDM->RefreshDataManager(_T(""));
	pGrid->Display();

	CString strQuery, strGroupAlias, strDefault;
	CString strResult[8];

	strQuery.Format(_T("SELECT DEFAULT_LOAD_TOPMENU_ALIAS FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s'"), m_pInfo->USER_ID);
	strDefault = _T("");
	ids = pDM->GetOneValueQuery(strQuery, strDefault);

	INT nRows = pDM->GetRowCount();
	for (INT i = 0; i < nRows; i++) {
		if (pDM->GetCellData(_T("ALIAS"), i) == strDefault) {
			pGrid->SetRow(i+1);
			pGrid->SetCol(4);
			pGrid->SetTextMatrix(i+1, 4, _T("V"));
			pGrid->SetCellFontBold(TRUE);
		}
		strGroupAlias = pDM->GetCellData(_T("ALIAS"), i);
		strQuery.Format(_T("SELECT PK, CAPTION, TARGET FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s' AND GROUP_ALIAS='%s' AND IDX=-1"), m_pInfo->USER_ID, strGroupAlias);
		strResult[0] = _T("");
		strResult[1] = _T("");
		strResult[2] = _T("");
		ids = pDM->GetValuesQuery(strQuery, strResult, 3);

		if (ids < 0) continue;
		pGrid->SetTextMatrix(i+1, 2, strResult[2]);
		pGrid->SetTextMatrix(i+1, 3, strResult[1]);
	}

	return 0;
}

VOID CMNG_USER_SHORTCUT_1000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		
		RefreshData();
	}
	CDialog::OnTimer(nIDEvent);
}

INT CMNG_USER_SHORTCUT_1000::OnModifyShortCut()
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_1000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}
	if (pDM->GetRowCount() ==  0) {
		AfxMessageBox(_T("정보가 없습니다."));
		return -1;
	}
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_1000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return -1;
	}
	INT nRow = pGrid->GetRow() - 1;

	CMNG_USER_SHORTCUT_2000 dlg(this);
	dlg.m_strTOPMENUGROUP_ALIAS = pDM->GetCellData(_T("ALIAS"), nRow);
	if (dlg.DoModal() != IDOK) return 0;

	if (dlg.ENDMODE == -1) {
		RefreshData();
		return 0;
	}
	CString strQuery, strPK;
	pDM->MakeRecKey(strPK);
	pDM->StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s' AND IDX=-1 AND GROUP_ALIAS='%s';"), GetUserID(), dlg.m_strTOPMENUGROUP_ALIAS);
	pDM->AddFrame(strQuery);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("PK"),				_T("NUMERIC"),		strPK);
	pDM->AddDBFieldData(_T("FLAG"),				_T("STRING"),		_T("U"));
	pDM->AddDBFieldData(_T("USER_ID"),			_T("STRING"),		GetUserID());
	pDM->AddDBFieldData(_T("GROUP_ALIAS"),		_T("STRING"),		dlg.m_strTOPMENUGROUP_ALIAS);
	pDM->AddDBFieldData(_T("ALIAS"),			_T("STRING"),		dlg.m_strAlias);
	pDM->AddDBFieldData(_T("CAPTION"),			_T("STRING"),		dlg.m_strCaption);
	pDM->AddDBFieldData(_T("IDX"),				_T("NUMERIC"),		_T("-1"));
	pDM->AddDBFieldData(_T("ACTION"),			_T("STRING"),		_T("U"));
	pDM->AddDBFieldData(_T("TARGET"),			_T("STRING"),		dlg.m_strAlias);
	pDM->MakeInsertFrame(_T("ESL_MENU_TBL"));

	ids = pDM->SendFrame();
	RefreshData();
	if (ids == 0) AfxMessageBox(_T("변경되었습니다."));
		else AfxMessageBox(_T("Database 작업중 에러가 발생하였습니다."));

	return 0;
}

INT CMNG_USER_SHORTCUT_1000::OnModifyLoginShortCut()
{
	CMNG_USER_SHORTCUT_3000 dlg(this);
	if (dlg.DoModal() != IDOK) return 0;

	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_1000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}
	CString strQuery;
	pDM->StartFrame();
	strQuery.Format(_T("UPDATE ESL_MENU_METHOD_TBL SET DEFAULT_LOAD_TOPMENU_ALIAS='%s' WHERE FLAG='U' AND USER_ID='%s';"), dlg.m_strTOPMENUGROUP_ALIAS, GetUserID());
	pDM->AddFrame(strQuery);

	ids = pDM->SendFrame();
	RefreshData();
	if (ids == 0) AfxMessageBox(_T("변경되었습니다."));
		else AfxMessageBox(_T("Database 작업중 에러가 발생하였습니다."));
	return 0;
}

BEGIN_EVENTSINK_MAP(CMNG_USER_SHORTCUT_1000, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_USER_SHORTCUT_1000)
	ON_EVENT(CMNG_USER_SHORTCUT_1000, IDC_gLIST, -605 /* MouseDown */, OnMouseDowngLIST, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_USER_SHORTCUT_1000::OnMouseDowngLIST(short Button, short Shift, long x, long y) 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_1000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pGrid) {
		pGrid->SetReverse(pGrid->GetRow()-1);
	}
}
