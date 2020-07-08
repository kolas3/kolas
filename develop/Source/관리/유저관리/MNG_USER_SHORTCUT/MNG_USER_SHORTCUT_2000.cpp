// MNG_USER_SHORTCUT_2000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_SHORTCUT_2000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_2000 dialog


CMNG_USER_SHORTCUT_2000::CMNG_USER_SHORTCUT_2000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_SHORTCUT_2000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strTOPMENUGROUP_ALIAS = _T("");
	m_strAlias = _T("");
	m_strCaption = _T("");

	ENDMODE = TRUE;
}
CMNG_USER_SHORTCUT_2000::~CMNG_USER_SHORTCUT_2000()
{
}



VOID CMNG_USER_SHORTCUT_2000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_SHORTCUT_2000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_SHORTCUT_2000, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_SHORTCUT_2000)
	ON_BN_CLICKED(IDC_bSHORTCUT_CANCEL, OnbSHORTCUTCANCEL)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_SHORTCUT_2000 message handlers

VOID CMNG_USER_SHORTCUT_2000::OnOK() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_2000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return;
	}
	CESL_DataMgr *pDM_LIST = FindDM(_T("DM_MNG_USER_SHORTCUT_메뉴목록"));
	if (pDM_LIST == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_2000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return;
	}

	INT nRow = pGrid->GetRow() - 1;
	m_strAlias = m_arrAlias.GetAt(nRow);
	m_strCaption = m_arrCaption.GetAt(nRow);

	CDialog::OnOK();
}

VOID CMNG_USER_SHORTCUT_2000::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CMNG_USER_SHORTCUT_2000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_USER_SHORTCUT_2000") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_SHORTCUT_2000") );
		return false;
	}
	
	RefreshData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
INT CMNG_USER_SHORTCUT_2000::RefreshData(VOID)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_2000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}
	CESL_DataMgr *pDM_LIST = FindDM(_T("DM_MNG_USER_SHORTCUT_메뉴목록"));
	if (pDM_LIST == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_2000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return -1;
	}

	CString strOption, strTmp, strData;
	strOption.Format(_T("FLAG='U' AND USER_ID='%s' AND IDX >= 0 AND GROUP_ALIAS='%s'"), GetUserID(), m_strTOPMENUGROUP_ALIAS);
	ids = pDM_LIST->RefreshDataManager(strOption);

	INT nRows = pDM_LIST->GetRowCount();
	if (nRows > 0) {
		strData = _T("");
		for (INT i = 0; i < nRows; i++) {
			if (pDM_LIST->GetCellData(_T("ALIAS"), i).Find(_T("작업선택")) >= 0) continue;
			if (pDM_LIST->GetCellData(_T("ALIAS"), i) == _T("홈") ) continue;
			if (strTmp.GetLength() == 0) strTmp.Format(_T("'%s'"),pDM_LIST->GetCellData(_T("ALIAS"), i));
				else strTmp.Format(_T(", '%s'"), pDM_LIST->GetCellData(_T("ALIAS"), i));
			strData += strTmp;
		}
		strOption.Format(_T("FLAG='U' AND USER_ID='%s' AND IDX >= 0 AND GROUP_ALIAS IN (%s) AND ACTION IN('D', 'A') ORDER BY ALIAS"), GetUserID(), strData);
		pDM->RefreshDataManager(strOption);
	}
	pGrid->Display();

	return 0;
}
*/

INT CMNG_USER_SHORTCUT_2000::RefreshData(VOID)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_2000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}
	CESL_DataMgr *pDM_LIST = FindDM(_T("DM_MNG_USER_SHORTCUT_메뉴목록"));
	if (pDM_LIST == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_2000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pCM == NULL || pGrid == NULL) {
		AfxMessageBox(_T("Grid 정보를 찾을 수 없습니다."));
		return -1;
	}

	FreeMenuGroupList();
	m_arrAlias.RemoveAll();
	m_arrCaption.RemoveAll();

		
	CString strOption;
	strOption.Format(_T("FLAG='U' AND USER_ID='%s' AND IDX >= 0 AND GROUP_ALIAS='%s'"), GetUserID(), m_strTOPMENUGROUP_ALIAS);
	ids = pDM_LIST->RefreshDataManager(strOption);

	INT nRows = pDM_LIST->GetRowCount();
	if (nRows == 0) return -1;
	INT i;
	CString strAlias;
	for (i = 0; i < nRows; i++) {
		strAlias = pDM_LIST->GetCellData(_T("ALIAS"), i);
		if (strAlias.Find(_T("작업선택")) >= 0) continue;
		AddMenuGroup(strAlias);
	}

	CString strGroupAlias;
	while(TRUE) {
		ids = RemoveHeadGroup(strGroupAlias);
		if (ids < 0) break;
		GetMenu(strGroupAlias);				
	}

	FreeMenuGroupList();

	ShowList();
	return 0;
}

BEGIN_EVENTSINK_MAP(CMNG_USER_SHORTCUT_2000, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_USER_SHORTCUT_2000)
	ON_EVENT(CMNG_USER_SHORTCUT_2000, IDC_gLIST, -605 /* MouseDown */, OnMouseDowngLIST, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CMNG_USER_SHORTCUT_2000::ShowList()
{
	INT nRows = m_arrAlias.GetSize();
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_2000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pGrid == NULL) return -1;
	
	pGrid->Clear();
	pGrid->SetCols(0, 3);
	if (nRows == 0) pGrid->SetRows(2);
		else pGrid->SetRows(nRows+1);
	pGrid->SetColumnInfo(0, _T("알리아스"), 200, 1);
	pGrid->SetColumnInfo(1, _T("캡션"), 200, 1);

	for (INT i = 0; i < nRows; i++) {
		pGrid->SetTextMatrix(i+1, 1, m_arrAlias.GetAt(i));
		pGrid->SetTextMatrix(i+1, 2, m_arrCaption.GetAt(i));
		
	}
	return 0;
}

VOID CMNG_USER_SHORTCUT_2000::OnMouseDowngLIST(short Button, short Shift, long x, long y) 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_USER_SHORTCUT_2000"));
	CESL_Grid *pGrid;
	if (pCM) pGrid = (CESL_Grid*)pCM->FindControl(_T("목록"));
	if (pGrid) {
		pGrid->SetReverse(pGrid->GetRow()-1);
	}
}

INT CMNG_USER_SHORTCUT_2000::FreeMenuGroupList(VOID)
{
	INT nCount = m_MenuGroupList.GetCount();
	if (nCount == 0) return 0;
	CString *pData;
	for (INT i = 0; i < nCount; i++) {
		pData = (CString*)m_MenuGroupList.RemoveHead();
		if (pData) delete pData;
	}
	m_MenuGroupList.RemoveAll();
	return(nCount);
}

INT CMNG_USER_SHORTCUT_2000::AddMenuGroup(CString strGroupAlias)
{
	CString *pData = new CString;
	pData->Format(_T("%s"), strGroupAlias);

	m_MenuGroupList.AddTail((CObject*)pData);
	return 0;
}

INT CMNG_USER_SHORTCUT_2000::AddMenu(CString strAlias, CString strCaption)
{
	if (strAlias == _T("홈")) return -1;
	m_arrAlias.Add(strAlias);
	m_arrCaption.Add(strCaption);

	return 0;
}

INT CMNG_USER_SHORTCUT_2000::GetMenu(CString strGroupAlias)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_2000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return -1;
	}

	CString strOption;
	strOption.Format(_T("FLAG='U' AND USER_ID='%s' AND GROUP_ALIAS='%s'"), GetUserID(), strGroupAlias);
	ids = pDM->RefreshDataManager(strOption);

	INT nRows = pDM->GetRowCount();
	CString strStyle, strAction, strAlias, strCaption, strTarget;
	for (INT i = 0; i < nRows; i++) {
		strStyle = pDM->GetCellData(_T("STYLE"), i);
		if (strStyle == _T("P")) continue;
		strAction = pDM->GetCellData(_T("ACTION"), i);
		strAlias = pDM->GetCellData(_T("ALIAS"), i);
		strCaption = pDM->GetCellData(_T("CAPTION"), i);
		strTarget = pDM->GetCellData(_T("TARGET"), i);
		if (strAction == _T("D")) {
			AddMenu(strAlias, strCaption);
		}
		if (strAction == _T("P")) {
			AddMenuGroup(strTarget);
		}
	}
	return 0;
}

INT CMNG_USER_SHORTCUT_2000::RemoveHeadGroup(CString &strGroupAlias)
{
	strGroupAlias = _T("");
	INT nCount = m_MenuGroupList.GetCount();
	if (nCount == 0) return -1;
	CString *pData = (CString *)m_MenuGroupList.RemoveHead();
	if (pData) {
		strGroupAlias.Format(_T("%s"), *pData);
		delete pData;
		pData = NULL;
	}
	return 0;
}
VOID CMNG_USER_SHORTCUT_2000::OnbSHORTCUTCANCEL() 
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_SHORTCUT_2000"));
	if (pDM == NULL) {
		AfxMessageBox(_T("DataBase 정보를 찾을 수 없습니다."));
		return;
	}
	
	if (AfxMessageBox(_T("바로가기를 제거 하시겠습니까?"), MB_YESNO) != IDYES) return;

	CString strQuery;
	pDM->StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s' AND IDX=-1 AND GROUP_ALIAS='%s';"), GetUserID(), m_strTOPMENUGROUP_ALIAS);	
	pDM->AddFrame(strQuery);
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("Database작업중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("제거 되었습니다. 변경 사항은 다음 로그인시부터 적용됩니다."));
		ENDMODE = -1;
	}
	CDialog::OnOK();
}

HBRUSH CMNG_USER_SHORTCUT_2000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

