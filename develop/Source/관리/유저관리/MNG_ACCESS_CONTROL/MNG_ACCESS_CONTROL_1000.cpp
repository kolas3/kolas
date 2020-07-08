// MNG_ACCESS_CONTROL_1000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_ACCESS_CONTROL_2000.h"
#include "MNG_ACCESS_CONTROL_3000.h"
#include "MNG_ACCESS_CONTROL_4000.h"
#include "MNG_ACCESS_CONTROL_5000.h"
#include "..\..\..\ESL\KOLAS3_MAIN\KOLAS3_MAINApi.h"

//2004-05-06 이학중 추가 
#include "..\..\..\ESL\MENU_TOOLBAR_MGR\MENUTOOLBAR_Mgr.h"
/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_1000 dialog

CMNG_ACCESS_CONTROL_1000::CMNG_ACCESS_CONTROL_1000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ACCESS_CONTROL_1000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTop = 50;
	m_nCOLUMN[0] = 40;
	m_nCOLUMN[1] = 30;

	m_nTopBaseItemData = 1000000;
	m_cIs_MenuGroupAlias_Top = FALSE;		// 현재 편집중인 메뉴 그룹은 top 메뉴 그룹이 아님

	m_pComboBox = NULL;
	m_pMenu = NULL;

	m_cLoadDll = RGB(41, 41, 214);

	m_nTopMenuOrder = 0;


}

INT CMNG_ACCESS_CONTROL_1000::SetTopMenuInfo()
{
	/// 이동도서관일 경우
	if( m_pInfo->MODE == 10000 || ( m_pInfo->MODE == 30000 ) )
	{
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("관리메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("대출반납메뉴");
	}
	else
	{
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_수서대상관리메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_구입메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_기증메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_등록메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_정리메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_열람메뉴");

		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_구입메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_기증메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_제본메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_등록메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_정리메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_열람메뉴");

		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_관리메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_대출반납메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_목차초록메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_전거통제메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_분류기호메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_이용자자료찾기메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_업무용자료찾기메뉴");
		m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_공동목록메뉴");

		CStdioFile fd;
		CString strLine;
		if (fd.Open(_T("..\\CFG\\TOP_MENU_ORDER.CFG"), CFile::modeRead | CFile::typeBinary) == TRUE) {
			m_nTopMenuOrder = 0;
			TCHAR cUni;
			fd.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fd.SeekToBegin();
			}
			
			while(TRUE) {
				if (fd.ReadString(strLine) == FALSE) break;
				
				if (strLine.GetLength() == 0) continue;
				m_strTopMenuOrder[m_nTopMenuOrder++] = strLine;
			}

			fd.Close();
		}
	}

	return 0;
}

BOOL CMNG_ACCESS_CONTROL_1000::Create(CWnd* pParentWnd)
{
EFS_BEGIN
	return CDialog::Create(IDD, pParentWnd);
EFS_END
return FALSE;
}

VOID CMNG_ACCESS_CONTROL_1000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ACCESS_CONTROL_1000)
	DDX_Control(pDX, IDC_TREE_MAC, m_tMenuTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ACCESS_CONTROL_1000, CDialog)
	//{{AFX_MSG_MAP(CMNG_ACCESS_CONTROL_1000)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MAC, OnSelchangedTreeMac)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(4000,4003,OnUserMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_1000 message handlers

BOOL CMNG_ACCESS_CONTROL_1000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("MNG_ACCESS_CONTROL_1000")) < 0) {
		AfxMessageBox(_T("화면 초기화에 실패하였습니다. : [MNG_ACCESS_CONTROL_1000]"));
		return TRUE;
	}

	SetTopMenuInfo();


	// 사용자 레벨이 U 인경우 툴바중에서 _T("툴바추가"), _T("툴바삭제")의 두개의 버튼을 보이지 않게 처리한다.
	if (GetUserLEVEL() == _T("U")) {
		CString strRemoveButton;
		strRemoveButton = _T("툴바추가");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);
		strRemoveButton = _T("툴바삭제");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);
	}

	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_ACCESS_CONTROL_1000::OnSearch(VOID)
{
	CMNG_ACCESS_CONTROL_2000 dlg(this);
	if (dlg.DoModal() != IDOK) return -1;

	Search(dlg.FLAG, dlg.ID);
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::Search(TCHAR cFlag, CString strID)
{
	strID.TrimLeft();				strID.TrimRight();
	ID = strID;
	FLAG = cFlag;

	CString strMsg;
	if (FLAG == 'U') strMsg.Format(_T("사용자 [%s]를 편집중입니다."), ID);
	if (FLAG == 'S') strMsg.Format(_T("관리자 [%s]를 편집중입니다."), ID);
	if (FLAG == 'M') strMsg.Format(_T("메소드 [%s]를 편집중입니다."), ID);
	GetDlgItem(IDC_STATIC_MSG_ID)->SetWindowText(strMsg);
	
	REFRESH_MenuTree();
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::REFRESH_MenuTree(VOID)
{
	m_tMenuTree.DeleteAllItems();

	CString strOption;

	CESL_DataMgr *pDM_MENUGROUP = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴그룹"));
	if (pDM_MENUGROUP == NULL) return -1;
	pDM_MENUGROUP->RefreshDataManager(_T("SPF_USE_FLAG='y'"));
	INT nRows = pDM_MENUGROUP->GetRowCount();

	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return -1;
	strOption.Format(_T("SPF_USE_FLAG='y' AND IDX >= 0 AND FLAG='%c' AND USER_ID='%s' ORDER BY IDX"), FLAG, ID);
	pDM_MENU->RefreshDataManager(strOption);
	INT nMenuRows = pDM_MENU->GetRowCount();

	HTREEITEM hItem, hItemMenu;
	HTREEITEM hRoot = m_tMenuTree.GetRootItem();
	TVINSERTSTRUCT tvInsert;

	CString strMenuGroupCaption, strMenuGroupAlias, strMenu, strMenuAlias;
	INT i, j;
	INT nDMRowIdx;
	m_arrTopMenuIdx.RemoveAll();
	for (i = 0; i < nRows; i++) {
		if (pDM_MENUGROUP->GetCellData(_T("GROUPTYPE"), i) == _T("T")) {
			m_arrTopMenuIdx.Add(i);
		}
	}
	for (i = 0; i < m_nTopMenuOrder; i++) {
		nDMRowIdx = GetTopMenuIdx(pDM_MENUGROUP, m_strTopMenuOrder[i]);
		if (nDMRowIdx < 0) continue;
		strMenuGroupCaption = pDM_MENUGROUP->GetCellData(_T("CAPTION"), nDMRowIdx);
		strMenuGroupAlias = pDM_MENUGROUP->GetCellData(_T("ALIAS"), nDMRowIdx);
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.lParam = 0;
		tvInsert.item.pszText = strMenuGroupCaption.GetBuffer(0);
		tvInsert.item.iImage = 0;
		tvInsert.item.iSelectedImage = 0;
				
		hItem = m_tMenuTree.InsertItem(&tvInsert);
		m_tMenuTree.SetItemData(hItem, m_nTopBaseItemData+nDMRowIdx);


		for (j = 0; j < nMenuRows; j++) {
			if (pDM_MENU->GetCellData(_T("GROUP_ALIAS"), j) == strMenuGroupAlias) {
				strMenu = pDM_MENU->GetCellData(_T("CAPTION"), j);
				strMenuAlias = pDM_MENU->GetCellData(_T("ALIAS"), j);
				//if (strMenuAlias.Find(_T("작업선택")) >= 0 || strMenuAlias == _T("홈")) continue;
				tvInsert.hParent = hItem;
				tvInsert.hInsertAfter = NULL;
				tvInsert.item.mask = TVIF_TEXT;
				tvInsert.item.lParam = 0;
				tvInsert.item.pszText = strMenu.GetBuffer(0);
				tvInsert.item.iImage = 0;
				tvInsert.item.iSelectedImage = 0;

				hItemMenu = m_tMenuTree.InsertItem(&tvInsert);
				m_tMenuTree.SetItemData(hItemMenu, (DWORD)j);

				REFRESH_AddMenuTree(hItemMenu, strMenuAlias);
			}
		}
	}
	
	UpdateData(FALSE);
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::GetTopMenuIdx(CESL_DataMgr *pDM_MENUGROUP, CString strAlias)
{
	if (pDM_MENUGROUP == NULL) return -1;
	INT nRows = m_arrTopMenuIdx.GetSize();
	INT nDMRowIdx;
	for (INT i = 0; i < nRows; i++) {
		nDMRowIdx = m_arrTopMenuIdx.GetAt(i);
		if (pDM_MENUGROUP->GetCellData(_T("GROUPTYPE"), nDMRowIdx) != _T("T")) continue;
		if (pDM_MENUGROUP->GetCellData(_T("ALIAS"), nDMRowIdx) == strAlias) return nDMRowIdx;
	}

	return -1;
}

INT CMNG_ACCESS_CONTROL_1000::REFRESH_AddMenuTree(HTREEITEM hItemParent, CString strMenuGroupAlias)
{
	CESL_DataMgr *pDM_MENUGROUP = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴그룹"));
	if (pDM_MENUGROUP == NULL) return -1;
	INT nRows = pDM_MENUGROUP->GetRowCount();

	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return -1;
	INT nMenuRows = pDM_MENU->GetRowCount();

	INT j;
	CString strMenu, strMenuAlias;
	TVINSERTSTRUCT tvInsert;
	HTREEITEM hItemMenu;
	for (j = 0; j < nMenuRows; j++) {
		if (pDM_MENU->GetCellData(_T("GROUP_ALIAS"), j) == strMenuGroupAlias) {
			strMenu = pDM_MENU->GetCellData(_T("CAPTION"), j);
			strMenuAlias = pDM_MENU->GetCellData(_T("ALIAS"), j);
			//if (strMenuAlias.Find(_T("작업선택")) >= 0 || strMenuAlias == _T("홈")) continue;
			tvInsert.hParent = hItemParent;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT;
			tvInsert.item.lParam = 0;
			tvInsert.item.pszText = strMenu.GetBuffer(0);
			tvInsert.item.iImage = 0;
			tvInsert.item.iSelectedImage = 0;

			hItemMenu = m_tMenuTree.InsertItem(&tvInsert);
			m_tMenuTree.SetItemData(hItemMenu, (DWORD)j);

			if (pDM_MENU->GetCellData(_T("ACTION"), j) == _T("P")) {
				REFRESH_AddMenuTree(hItemMenu, pDM_MENU->GetCellData(_T("TARGET"), j));
			}
		}
	}

	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::RESIZE_Controls(VOID) 
{
	CWnd *pWnd;
	CWnd *pWnd2;

	CRect rcClient, rcControl, rcControl2;
	GetClientRect(rcClient);

	pWnd = GetDlgItem(IDC_TREE_MAC);
	if (pWnd) {
		rcControl = rcClient;
		rcControl.right = rcControl.left + (rcClient.Width() * m_nCOLUMN[0] / 100);
		rcControl.top = m_nTop;

		pWnd->MoveWindow(rcControl, TRUE);
		
		rcControl2 = rcControl;
		pWnd2 = GetDlgItem(IDC_STATIC_TREE_MAC);
		if (pWnd2) {
			rcControl2.top = m_nTop - 25;
			rcControl2.bottom = rcControl2.top + 20;
			pWnd2->MoveWindow(rcControl2, TRUE);
		}
	}
	pWnd = GetDlgItem(IDC_GRID_TOOLBAR);
	if (pWnd) {
		rcControl.left = rcControl.right + 5;
		rcControl.right = rcClient.right;
		rcControl.top = m_nTop;

		pWnd->MoveWindow(rcControl, TRUE);

		rcControl2 = rcControl;
		pWnd2 = GetDlgItem(IDC_STATIC_GRID_TOOLBAR);
		if (pWnd2) {
			rcControl2.top = m_nTop - 25;
			rcControl2.bottom = rcControl2.top + 20;
			pWnd2->MoveWindow(rcControl2, TRUE);
			pWnd2->ShowWindow(FALSE);
			pWnd2->ShowWindow(TRUE);
		}
	}
	
	return 0;
}

VOID CMNG_ACCESS_CONTROL_1000::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RESIZE_Controls();
}

VOID CMNG_ACCESS_CONTROL_1000::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	RESIZE_Controls();
}

BOOL CMNG_ACCESS_CONTROL_1000::IsInMenuGroupAlias(CString strMenuGroupAlias)
{
	CESL_DataMgr *pDM_MENUGROUP = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴그룹"));
	if (pDM_MENUGROUP == NULL) return -1;
	INT nRows = pDM_MENUGROUP->GetRowCount();

	for (INT i = 0; i < nRows; i++) {
		if (pDM_MENUGROUP->GetCellData(_T("ALIAS"), i) == strMenuGroupAlias) return TRUE;
	}
	return FALSE;
}

INT CMNG_ACCESS_CONTROL_1000::REFRESH_Toolbar(CString strMenuAlias)
{
	m_strMenuAlias = _T("");
	CString strOption;
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return -1;
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return -2;
	pToolbarGrid->Clear();
	pToolbarGrid->SetColumnInfo();

	INT i, ids;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_TOOLBAR->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT ALIAS FROM ESL_TOOLGROUP_TBL WHERE MENU_ALIAS='%s'"), strMenuAlias);
	ids = dm.RestructDataManager(strQuery);
	if (dm.GetRowCount() == 0) return -1;
	
	CString strTOOLGROUPALIAS, strTmp;
	strTOOLGROUPALIAS = _T("");
	for (i = 0; i < dm.GetRowCount(); i++) {
		if (i == 0) strTmp.Format(_T("'%s'"), dm.GetCellData(i, 0));
			else strTmp.Format(_T(", '%s'"), dm.GetCellData(i, 0));
		strTOOLGROUPALIAS += strTmp;
	}
	strOption.Format(_T("SPF_USE_FLAG='y' AND MENU_ALIAS = '%s' AND TOOLGROUP_ALIAS IN (%s) AND FLAG='%c' AND USER_ID='%s' ORDER BY TOOLGROUP_ALIAS, IDX"), 
					strMenuAlias, strTOOLGROUPALIAS, FLAG, ID
					);
	pDM_TOOLBAR->RefreshDataManager(strOption);
	INT nRows = pDM_TOOLBAR->GetRowCount();

	if (nRows == 0) return 0;
	pToolbarGrid->SetRows(nRows+1);
	CString str;
	for (i = 0; i < nRows; i++) {
		pToolbarGrid->SetTextMatrix(i+1, 1, pDM_TOOLBAR->GetCellData(_T("CAPTION"), i));
		if (pDM_TOOLBAR->GetCellData(_T("STYLE"), i) != _T("B")) {
			pToolbarGrid->SetTextMatrix(i+1, 1, _T("----------------"));
		}
		pToolbarGrid->SetTextMatrix(i+1, 2, pDM_TOOLBAR->GetCellData(_T("IDX"), i));
		str = pDM_TOOLBAR->GetCellData(_T("TYPE"), i);
		str.TrimLeft();				str.TrimRight();
		if (str.GetLength() == 0) {
			pToolbarGrid->SetTextMatrix(i+1, 3, _T("예"));
		} else {
			pToolbarGrid->SetTextMatrix(i+1, 3, _T("아니오"));
		}
	}
	m_strMenuAlias = strMenuAlias;
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::MENU_Copy(CESL_DataMgr *pDM, INT nSelectedRow, BOOL bRearrangeMenu)
{
	INT ids;

	CString strPK, strTmp, strMenuAlias, strFlag;
	strFlag.Format(_T("%c"), FLAG);
	strMenuAlias = pDM->GetCellData(_T("ALIAS"), nSelectedRow);
	pDM->MakeRecKey(strPK);
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("PK"),			_T("NUMERIC"),			strPK);
	pDM->AddDBFieldData(_T("USER_ID"),		_T("STRING"),			ID);
	pDM->AddDBFieldData(_T("ALIAS"),		_T("STRING"),			pDM->GetCellData(_T("ALIAS"), nSelectedRow));
	pDM->AddDBFieldData(_T("GROUP_ALIAS"),	_T("STRING"),			pDM->GetCellData(_T("GROUP_ALIAS"), nSelectedRow));
	pDM->AddDBFieldData(_T("IDX"),			_T("NUMERIC"),			pDM->GetCellData(_T("IDX"), nSelectedRow));
	pDM->AddDBFieldData(_T("CAPTION"),		_T("STRING"),			pDM->GetCellData(_T("CAPTION"), nSelectedRow));
	pDM->AddDBFieldData(_T("VISIBLE"),		_T("STRING"),			pDM->GetCellData(_T("VISIBLE"), nSelectedRow));
	pDM->AddDBFieldData(_T("STYLE"),		_T("STRING"),			pDM->GetCellData(_T("STYLE"), nSelectedRow));
	CString strAciton = pDM->GetCellData(_T("ACTION"), nSelectedRow);
	strAciton.TrimLeft();					strAciton.TrimRight();
	strAciton.MakeUpper();
	pDM->AddDBFieldData(_T("ACTION"),		_T("STRING"),			pDM->GetCellData(_T("ACTION"), nSelectedRow));
	pDM->AddDBFieldData(_T("TARGET"),		_T("STRING"),			pDM->GetCellData(_T("TARGET"), nSelectedRow));
	pDM->AddDBFieldData(_T("FLAG"),			_T("STRING"),			strFlag);
	pDM->AddDBFieldData(_T("SPF_USE_FLAG"),	_T("STRING"),			pDM->GetCellData(_T("SPF_USE_FLAG"), nSelectedRow));
	pDM->AddDBFieldData(_T("PROGRAME_USE_FLAG"),	_T("STRING"),			pDM->GetCellData(_T("PROGRAME_USE_FLAG"), nSelectedRow));
	pDM->MakeInsertFrame(_T("ESL_MENU_TBL"));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString strQuery;
	BOOL bAddToolBar = FALSE;
	if (strAciton == _T("D")) {
		if (AfxMessageBox(_T("메뉴에 속한 툴바도 함께 추가하시겠습니까?"), MB_YESNO) == IDYES) bAddToolBar = TRUE;
	}
	if (bAddToolBar == TRUE) {
		//                         0       1        2             3            4    5      6       7      8    9      10           11            12         13              14              15     
		strQuery.Format(_T("SELECT USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, FLAG, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG FROM ESL_TOOLBAR_TBL WHERE FLAG='S' AND USER_ID='Administrator' AND MENU_ALIAS='%s' ORDER BY TOOLGROUP_ALIAS, IDX"), strMenuAlias);
		ids = dm.RestructDataManager(strQuery);

		INT nRows = dm.GetRowCount();
		strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS='%s' AND FLAG='%c' AND USER_ID='%s';"), strMenuAlias, FLAG, ID);
		pDM->AddFrame(strQuery);
		for (INT i = 0; i < nRows; i++) {
			pDM->MakeRecKey(strPK);
			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("PK"),				_T("NUMERIC"),			strPK);
			pDM->AddDBFieldData(_T("USER_ID"),			_T("STRING"),			ID);
			pDM->AddDBFieldData(_T("ALIAS"),			_T("STRING"),			dm.GetCellData(i, 1));
			pDM->AddDBFieldData(_T("MENU_ALIAS"),		_T("STRING"),			dm.GetCellData(i, 2));
			pDM->AddDBFieldData(_T("TOOLGROUP_ALIAS"),	_T("STRING"),			dm.GetCellData(i, 3));
			pDM->AddDBFieldData(_T("FLAG"),				_T("STRING"),			strFlag);
			pDM->AddDBFieldData(_T("IDX"),				_T("STRING"),			dm.GetCellData(i, 5));
			pDM->AddDBFieldData(_T("CAPTION"),			_T("STRING"),			dm.GetCellData(i, 6));
			pDM->AddDBFieldData(_T("STYLE"),			_T("STRING"),			dm.GetCellData(i, 7));
			pDM->AddDBFieldData(_T("TYPE"),				_T("STRING"),			dm.GetCellData(i, 8));
			pDM->AddDBFieldData(_T("SM"),				_T("STRING"),			dm.GetCellData(i, 9));
			pDM->AddDBFieldData(_T("BITMAP_NORMAL"),	_T("STRING"),			dm.GetCellData(i, 10));
			pDM->AddDBFieldData(_T("BITMAP_HOT"),		_T("STRING"),			dm.GetCellData(i, 11));
			pDM->AddDBFieldData(_T("BITMAP_DISABLED"),	_T("STRING"),			dm.GetCellData(i, 12));
			pDM->AddDBFieldData(_T("TOOLTIP"),			_T("STRING"),			dm.GetCellData(i, 13));
			pDM->AddDBFieldData(_T("SPF_USE_FLAG"),		_T("STRING"),			dm.GetCellData(i, 14));
			pDM->AddDBFieldData(_T("PROGRAME_USE_FLAG"),_T("STRING"),			dm.GetCellData(i, 15));
			pDM->MakeInsertFrame(_T("ESL_TOOLBAR_TBL"));
		}
	}

	ids = pDM->SendFrame();
	pDM->EndFrame();

	if (ids < 0) {
		AfxMessageBox(_T("Database작업중 에러가 발생하였습니다."));
	} else {
		REFRESH_MenuTree();
		AfxMessageBox(_T("추가 되었습니다."));
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::MENU_Rearrange(CESL_DataMgr *pDM_OUT, CString strMenuGroupAlias)
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT PK, ALIAS, IDX FROM ESL_MENU_TBL WHERE IDX >= 0 AND FLAG='%c' AND USER_ID='%s' AND GROUP_ALIAS='%s' ORDER BY IDX"), FLAG, ID, m_strMenuGroupAlias);
	dm.RestructDataManager(strQuery);
	INT nRows = dm.GetRowCount();
	if (nRows == 0) return 0;

	pDM_OUT->StartFrame();
	INT nIdx = 0;
	CString strAlias, strPK;
	INT nRow_SelectMenu = -1;
	INT nRow_Home = -1;
	for (INT i = 0; i < nRows; i++) {
		strAlias = dm.GetCellData(i, 1);
		strAlias.TrimLeft();			strAlias.TrimRight();
		strPK = dm.GetCellData(i, 0);
		strPK.TrimLeft();				strPK.TrimRight();
		if (strAlias == _T("홈")) {
			nRow_Home = i;
			continue;
		}
		if (strAlias.Find(_T("작업선택")) >= 0) {
			nRow_SelectMenu = i;
			continue;
		}

		strQuery.Format(_T("UPDATE ESL_MENU_TBL SET IDX=%d WHERE PK=%s;"), nIdx, strPK);
		pDM_OUT->AddFrame(strQuery);
		
		nIdx++;
	}
	
	if (nRow_SelectMenu >= 0) {
		strPK = dm.GetCellData(nRow_SelectMenu, 0);
		strPK.TrimLeft();				strPK.TrimRight();
		strQuery.Format(_T("UPDATE ESL_MENU_TBL SET IDX=%d WHERE PK=%s;"), nIdx, strPK);
		pDM_OUT->AddFrame(strQuery);
		
		nIdx++;
	}
	if (nRow_Home >= 0) {
		strPK = dm.GetCellData(nRow_Home, 0);
		strPK.TrimLeft();				strPK.TrimRight();
		strQuery.Format(_T("UPDATE ESL_MENU_TBL SET IDX=%d WHERE PK=%s;"), nIdx, strPK);
		pDM_OUT->AddFrame(strQuery);
		
		nIdx++;
	}
	
	INT ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::MENUGROUP_Copy(CESL_DataMgr *pDM_OUT, CString strMenuGroupAlias, CArray<CString, CString> &arrGroupList, CArray<CString, CString> &arrCopyedGroupList, CArray<CString, CString> &arrCopyedMenuAliasList)
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);

	INT ids;
	CString strQuery, strPK, strFlag, strMenuAlias, strTarget;
	//                       0         1         2     3        4        5      6       7      8        9               10   
	strQuery.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, FLAG, SPF_USE_FLAG, PROGRAME_USE_FLAG FROM ESL_MENU_TBL WHERE FLAG='S' AND USER_ID='Administrator' AND GROUP_ALIAS='%s'"), strMenuGroupAlias);
	ids = dm.RestructDataManager(strQuery);

	strFlag.Format(_T("%c"), FLAG);
	INT nRows = dm.GetRowCount();
	for (INT i = 0; i < nRows; i++) {
		strMenuAlias = dm.GetCellData(i, 0);
		if (InArray(arrCopyedMenuAliasList, strMenuAlias) >= 0) continue;
		pDM_OUT->StartFrame();
		strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s' AND ALIAS='%s' AND GROUP_ALIAS='%s';"), FLAG, ID, dm.GetCellData(i, 0), dm.GetCellData(i, 1));
		pDM_OUT->AddFrame(strQuery);
		pDM_OUT->MakeRecKey(strPK);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddDBFieldData(_T("PK"),			_T("NUMERIC"),			strPK);
		pDM_OUT->AddDBFieldData(_T("USER_ID"),		_T("STRING"),			ID);
		pDM_OUT->AddDBFieldData(_T("ALIAS"),		_T("STRING"),			dm.GetCellData(i, 0));
		pDM_OUT->AddDBFieldData(_T("GROUP_ALIAS"),	_T("STRING"),			dm.GetCellData(i, 1));
		pDM_OUT->AddDBFieldData(_T("IDX"),			_T("NUMERIC"),			dm.GetCellData(i, 2));
		pDM_OUT->AddDBFieldData(_T("CAPTION"),		_T("STRING"),			dm.GetCellData(i, 3));
		pDM_OUT->AddDBFieldData(_T("VISIBLE"),		_T("STRING"),			dm.GetCellData(i, 4));
		pDM_OUT->AddDBFieldData(_T("STYLE"),		_T("STRING"),			dm.GetCellData(i, 5));
		pDM_OUT->AddDBFieldData(_T("ACTION"),		_T("STRING"),			dm.GetCellData(i, 6));
		strTarget = dm.GetCellData(i, 7);
		pDM_OUT->AddDBFieldData(_T("TARGET"),		_T("STRING"),			dm.GetCellData(i, 7));
		pDM_OUT->AddDBFieldData(_T("FLAG"),			_T("STRING"),			strFlag);
		pDM_OUT->AddDBFieldData(_T("SPF_USE_FLAG"),	_T("STRING"),			dm.GetCellData(i, 9));
		pDM_OUT->AddDBFieldData(_T("PROGRAME_USE_FLAG"),	_T("STRING"),	dm.GetCellData(i, 10));
		pDM_OUT->MakeInsertFrame(_T("ESL_MENU_TBL"));

		TOOLBAR_Copy(pDM_OUT, dm.GetCellData(i, 0));

		ids = pDM_OUT->SendFrame();
		pDM_OUT->EndFrame();

		if (dm.GetCellData(i, 6) == _T("P")) {
			if (InArray(arrCopyedGroupList, strTarget) == -1) {
				arrGroupList.Add(strTarget);
			}
		}
	}
	arrCopyedGroupList.Add(strMenuGroupAlias);
	return 0;
}

// return values
// -1 : 없음
// 0이상 : 있음
INT CMNG_ACCESS_CONTROL_1000::InArray(CArray <CString, CString> &arr, CString str) 
{
	INT nDatas = arr.GetSize();
	if (nDatas == 0) return -1;		

	for (INT i = 0; i < nDatas; i++) {
		if (arr.GetAt(i) == str) return i;
	}
	return -1;
}

INT CMNG_ACCESS_CONTROL_1000::TOOLBAR_Copy(CESL_DataMgr *pDM_OUT, CString strMenuAlias)
{
	INT ids;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);

	CString strQuery, strPK, strFlag;
	strFlag.Format(_T("%c"), FLAG);
	//                         0       1        2             3            4    5      6       7      8    9      10           11            12         13              14              15     
	strQuery.Format(_T("SELECT USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, FLAG, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG FROM ESL_TOOLBAR_TBL WHERE FLAG='S' AND USER_ID='Administrator' AND MENU_ALIAS='%s' ORDER BY TOOLGROUP_ALIAS, IDX"), strMenuAlias);
	ids = dm.RestructDataManager(strQuery);

	INT nRows = dm.GetRowCount();
	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS='%s' AND FLAG='%c' AND USER_ID='%s';"), strMenuAlias, FLAG, ID);
	pDM_OUT->AddFrame(strQuery);
	for (INT i = 0; i < nRows; i++) {
		pDM_OUT->MakeRecKey(strPK);
		pDM_OUT->InitDBFieldData();
		pDM_OUT->AddDBFieldData(_T("PK"),				_T("NUMERIC"),			strPK);
		pDM_OUT->AddDBFieldData(_T("USER_ID"),			_T("STRING"),			ID);
		pDM_OUT->AddDBFieldData(_T("ALIAS"),			_T("STRING"),			dm.GetCellData(i, 1));
		pDM_OUT->AddDBFieldData(_T("MENU_ALIAS"),		_T("STRING"),			dm.GetCellData(i, 2));
		pDM_OUT->AddDBFieldData(_T("TOOLGROUP_ALIAS"),	_T("STRING"),			dm.GetCellData(i, 3));
		pDM_OUT->AddDBFieldData(_T("FLAG"),				_T("STRING"),			strFlag);
		pDM_OUT->AddDBFieldData(_T("IDX"),				_T("STRING"),			dm.GetCellData(i, 5));
		pDM_OUT->AddDBFieldData(_T("CAPTION"),			_T("STRING"),			dm.GetCellData(i, 6));
		pDM_OUT->AddDBFieldData(_T("STYLE"),			_T("STRING"),			dm.GetCellData(i, 7));
		pDM_OUT->AddDBFieldData(_T("TYPE"),				_T("STRING"),			dm.GetCellData(i, 8));
		pDM_OUT->AddDBFieldData(_T("SM"),				_T("STRING"),			dm.GetCellData(i, 9));
		pDM_OUT->AddDBFieldData(_T("BITMAP_NORMAL"),	_T("STRING"),			dm.GetCellData(i, 10));
		pDM_OUT->AddDBFieldData(_T("BITMAP_HOT"),		_T("STRING"),			dm.GetCellData(i, 11));
		pDM_OUT->AddDBFieldData(_T("BITMAP_DISABLED"),	_T("STRING"),			dm.GetCellData(i, 12));
		pDM_OUT->AddDBFieldData(_T("TOOLTIP"),			_T("STRING"),			dm.GetCellData(i, 13));
		pDM_OUT->AddDBFieldData(_T("SPF_USE_FLAG"),		_T("STRING"),			dm.GetCellData(i, 14));
		pDM_OUT->AddDBFieldData(_T("PROGRAME_USE_FLAG"),_T("STRING"),			dm.GetCellData(i, 15));
		pDM_OUT->MakeInsertFrame(_T("ESL_TOOLBAR_TBL"));
	}

	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::TOOLBAR_Add(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	if (m_cIs_MenuGroupAlias_Top == TRUE) {
		AfxMessageBox(_T("TOP 메뉴 항목은 Toolbar를 가지지 않습니다."));
		return 0;
	}

	CMNG_ACCESS_CONTROL_4000 dlg(this);
	if (dlg.DoModal() != IDOK) return -1;
	if (dlg.m_nSelectedRow == -1) {
		AfxMessageBox(_T("선택된 자료가 없습니다."));
		return 0;
	}
	CESL_DataMgr *pDM = dlg.FindDM(_T("DM_MNG_ACCESS_CONTROL_4000"));
	if (pDM == NULL) return -1;
	
	pDM->StartFrame();
	CString strPK;
	pDM->MakeRecKey(strPK);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->InitDBFieldData();
	pDM->SetCellData(_T("PK"), strPK, dlg.m_nSelectedRow);
	CString strFlag;
	strFlag.Format(_T("%c"), FLAG);
	pDM->AddDBFieldData(_T("FLAG"), _T("STRING"), strFlag);
	pDM->AddDBFieldData(_T("USER_ID"), _T("STRING"), ID);
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("ESL_TOOLBAR_TBL"), dlg.m_nSelectedRow, pDM);

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	BOOL bRearrange = FALSE;
	//if (AfxMessageBox(_T("TOOLBAR 추가후 TOOLBAR를 재정렬 하시겠습니까?"), MB_YESNO) == IDYES) bRearrange = TRUE;
	bRearrange = TRUE;
	if (bRearrange == TRUE) TOOLBAR_Rearrange(pDM, m_strMenuAlias);
	if (ids < 0) {
		AfxMessageBox(_T("Database와 작업중에 에러가 발생하였습니다."));
	} else {
		REFRESH_Toolbar(m_strMenuAlias);		
		AfxMessageBox(_T("추가 되었습니다."));
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::TOOLBAR_Delete(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	if (m_cIs_MenuGroupAlias_Top == TRUE) {
		AfxMessageBox(_T("TOP 메뉴 항목은 Toolbar를 가지지 않습니다."));
		return 0;
	}

	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return -1;
	if (pDM_TOOLBAR->GetRowCount() == 0) return -1;
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return -2;

	INT nSelectedRow = pToolbarGrid->GetRow() -1;
	CString strPK;
	strPK = pDM_TOOLBAR->GetCellData(_T("PK"), nSelectedRow);

	CString strMsg;
	strMsg.Format(_T("툴바버튼 [%s]를(을) 정말로 삭제하시겠습니까?"), pDM_TOOLBAR->GetCellData(_T("CAPTION"), nSelectedRow));
	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES) return 0;

	pDM_TOOLBAR->StartFrame();
	pDM_TOOLBAR->InitDBFieldData();
	pDM_TOOLBAR->MakeDeleteFrame(_T("ESL_TOOLBAR_TBL"), _T("PK"), _T("NUMERIC"), strPK);

	INT ids = pDM_TOOLBAR->SendFrame();
	pDM_TOOLBAR->EndFrame();
	BOOL bRearrange = FALSE;
	//if (AfxMessageBox(_T("TOOLBAR 삭제후 TOOLBAR를 재정렬 하시겠습니까?"), MB_YESNO) == IDYES) bRearrange = TRUE;
	bRearrange = TRUE;
	if (bRearrange) TOOLBAR_Rearrange(pDM_TOOLBAR, m_strMenuAlias);
	if (ids < 0) {
		AfxMessageBox(_T("삭제중 에러가 발생하였습니다."));
	} else {
		REFRESH_Toolbar(m_strMenuAlias);
		AfxMessageBox(_T("삭제하였습니다."));
	}

	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::String_to_Integer(CString str)
{
	if (str.GetLength() == 0) return 0;
	
	return _ttoi(str);
}

INT CMNG_ACCESS_CONTROL_1000::METHOD_Manage(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	CMNG_ACCESS_CONTROL_5000 dlg(this);
	dlg.DoModal();

	return 0;
}

BEGIN_EVENTSINK_MAP(CMNG_ACCESS_CONTROL_1000, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_ACCESS_CONTROL_1000)
	ON_EVENT(CMNG_ACCESS_CONTROL_1000, IDC_GRID_TOOLBAR, -601 /* DblClick */, OnDblClickGridToolbar, VTS_NONE)
	ON_EVENT(CMNG_ACCESS_CONTROL_1000, IDC_GRID_TOOLBAR, -605 /* MouseDown */, OnMouseDownGridToolbar, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_ACCESS_CONTROL_1000::OnDblClickGridToolbar() 
{
	CreateComboBoxInToolbarGridSell();
}

VOID CMNG_ACCESS_CONTROL_1000::OnDestroy() 
{
	CDialog::OnDestroy();

	FreeComboBox();
	FreeMenu();
}

INT CMNG_ACCESS_CONTROL_1000::FreeComboBox(VOID)
{
	if (m_pComboBox) {
		delete m_pComboBox;
		m_pComboBox = NULL;
	}	

	return 0;
}

VOID CMNG_ACCESS_CONTROL_1000::CreateComboBoxInSubMenuGridSell()
{
	INT nEditCol = 3;

	INT nRows = m_arrSubMenu.GetSize();
	if(nRows == 0) return ;

	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return;
	CESL_Grid *pSubMenuGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("서브메뉴목록"));
	if (pSubMenuGrid == NULL) return;
	INT nRow = pSubMenuGrid->GetRow() - 1;
	if (pSubMenuGrid->GetCol() != nEditCol) return;
	FreeComboBox();

	m_pComboBox = new CSelectComboBox;
	m_pComboBox->m_pParentDlg = this;
	m_pComboBox->PARENT = _T("SUBMENU");
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), pSubMenuGrid, 6450);
	m_pComboBox->AddMSG(_T("예"));
	m_pComboBox->AddMSG(_T("아니오"));
	m_pComboBox->m_nSelectedRow = m_arrSubMenu.GetAt(nRow);
	m_pComboBox->m_nRow = nRow+1;
	m_pComboBox->m_nCol = nEditCol;
	if (pDM_MENU->GetCellData(_T("VISIBLE"), m_arrSubMenu.GetAt(nRow)) == _T("F")) m_pComboBox->Init(_T("아니오"));
		else m_pComboBox->Init(_T("예"));
	

	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);

	CRect rect;
	GetGridCellRect(pSubMenuGrid, rect);
	m_pComboBox->MoveWindow(rect, TRUE);

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMNG_ACCESS_CONTROL_1000::ApplyComboBoxInSubMenuGridSell(VOID)
{
	if (m_pComboBox == NULL) return 0;
	CString str;
	m_pComboBox->GetWindowText(str);
	str.TrimLeft();			str.TrimRight();
	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return 0;
	CESL_Grid *pSubMenuGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("서브메뉴목록"));
	if (pSubMenuGrid == NULL) return 0;
	if (str == _T("예")) pDM_MENU->SetCellData(_T("VISIBLE"), _T("T"), m_pComboBox->m_nSelectedRow);
		else pDM_MENU->SetCellData(_T("VISIBLE"), _T("F"), m_pComboBox->m_nSelectedRow);
	pSubMenuGrid->SetTextMatrix(m_pComboBox->m_nRow, m_pComboBox->m_nCol, str);
	
	return 0;
}	

INT CMNG_ACCESS_CONTROL_1000::GetGridCellRect(CESL_Grid *pGrid, CRect &rect)
{
	CRect gridRect;
	RECT parentRect;
	pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	//::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	::ClientToScreen(GetParent()->m_hWnd,(POINT*)&(parentRect.left));
	

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)pGrid->GetCellLeft()/15.0f)-2;
	rect.top = (long)((float)pGrid->GetCellTop()/15.0f)-4;
	rect.right = rect.left + (long)((float)pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)pGrid->GetCellHeight()/15.0f);

	return 0;
}

VOID CMNG_ACCESS_CONTROL_1000::CreateComboBoxInToolbarGridSell()
{
	INT nEditCol = 3;

	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return;
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return;
	INT nRow = pToolbarGrid->GetRow() - 1;
	if (pToolbarGrid->GetCol() != nEditCol) return;
	FreeComboBox();

	m_pComboBox = new CSelectComboBox;
	m_pComboBox->m_pParentDlg = this;
	m_pComboBox->PARENT = _T("TOOLBAR");
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), pToolbarGrid, 6450);
	m_pComboBox->AddMSG(_T("예"));
	m_pComboBox->AddMSG(_T("아니오"));
	m_pComboBox->m_nSelectedRow = nRow;
	m_pComboBox->m_nRow = nRow+1;
	m_pComboBox->m_nCol = nEditCol;

	CString strType = pDM_TOOLBAR->GetCellData(_T("TYPE"), nRow);
	if (strType == _T("R")) m_pComboBox->Init(_T("아니오"));
	if (strType == _T("I")) m_pComboBox->Init(_T("아니오"));
	if (strType == _T("D")) m_pComboBox->Init(_T("아니오"));
	if (strType != _T("R") && strType != _T("I") && strType != _T("D")) m_pComboBox->Init(_T("예"));

	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);

	CRect rect;
	GetGridCellRect(pToolbarGrid, rect);
	m_pComboBox->MoveWindow(rect, TRUE);

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMNG_ACCESS_CONTROL_1000::ApplyComboBoxInToolbarGridSell(VOID)
{
	if (m_pComboBox == NULL) return 0;
	CString str;
	m_pComboBox->GetWindowText(str);
	str.TrimLeft();			str.TrimRight();
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return 0;
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return 0;
	CString strCode = _T("");
	m_pComboBox->GetWindowText(str);
	str.TrimLeft();				str.TrimRight();
	if (str == _T("예")) strCode = _T("");
	if (str == _T("아니오")) strCode = _T("I");
	pDM_TOOLBAR->SetCellData(_T("TYPE"), strCode, m_pComboBox->m_nSelectedRow);
	pToolbarGrid->SetTextMatrix(m_pComboBox->m_nRow, m_pComboBox->m_nCol, str);
	return 0;
}	

INT CMNG_ACCESS_CONTROL_1000::ApplyComboBoxInToolbarGridSell(INT nRowIdx, BOOL bShow)
{
	CString str;
	if (bShow == TRUE) str = _T("예");
	if (bShow == FALSE) str = _T("아니오");
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return 0;
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return 0;
	CString strCode = _T("");
	if (str == _T("예")) strCode = _T("");
	if (str == _T("아니오")) strCode = _T("I");
	pDM_TOOLBAR->SetCellData(_T("TYPE"), strCode, nRowIdx);
	pToolbarGrid->SetTextMatrix(nRowIdx+1, 3, str);
	return 0;
}	

INT CMNG_ACCESS_CONTROL_1000::MENU_Apply(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	INT nRows = m_arrSubMenu.GetSize();
	if (nRows == 0) return 0;
	if (AfxMessageBox(_T("메뉴 VISIBLE 변경 사항을 적용하시겠습니까?"), MB_YESNO) != IDYES) return 0;
	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return 0;

	CString strVisible, strPK;
	pDM_MENU->StartFrame();
	for (INT i = 0; i < nRows; i++) {
		strVisible = pDM_MENU->GetCellData(_T("VISIBLE"), m_arrSubMenu.GetAt(i));
		strVisible.TrimLeft();		strVisible.TrimRight();
		strPK = pDM_MENU->GetCellData(_T("PK"), m_arrSubMenu.GetAt(i));
		strPK.TrimLeft();		strPK.TrimRight();

		pDM_MENU->InitDBFieldData();
		pDM_MENU->AddDBFieldData(_T("VISIBLE"), _T("STRING"), strVisible);
		pDM_MENU->MakeUpdateFrame(_T("ESL_MENU_TBL"), _T("PK"), _T("NUMERIC"), strPK);
	}
	INT ids = pDM_MENU->SendFrame();
	pDM_MENU->EndFrame();
	if (ids == 0) {
		AfxMessageBox(_T("메뉴 VISIBLE 변경 사항이 적용되었습니다.\r\n변경사항은 다음 로그인시부터 적용됩니다."));
	} else {
		AfxMessageBox(_T("메뉴 VISIBLE 변경 사항을 적용하던중 에러가 발생하였습니다."));
	}

	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::TOOLBAR_Apply(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return -1;
	INT nRows = pDM_TOOLBAR->GetRowCount();
	if (nRows == 0) return 0;
	if (AfxMessageBox(_T("툴바 변경 사항을 적용하시겠습니까?"), MB_YESNO) != IDYES) return 0;

	CString strType, strPK, strIDX;
	pDM_TOOLBAR->StartFrame();
	for (INT i = 0; i < nRows; i++) {
		strType = pDM_TOOLBAR->GetCellData(_T("TYPE"), i);
		strType.TrimLeft();		strType.TrimRight();
		strPK = pDM_TOOLBAR->GetCellData(_T("PK"), i);
		strPK.TrimLeft();		strPK.TrimRight();
		strIDX = pDM_TOOLBAR->GetCellData(_T("IDX"), i);
		strIDX.TrimLeft();		strIDX.TrimRight();

		pDM_TOOLBAR->InitDBFieldData();
		pDM_TOOLBAR->AddDBFieldData(_T("TYPE"), _T("STRING"), strType);
		pDM_TOOLBAR->AddDBFieldData(_T("IDX"), _T("NUMERIC"), strIDX);
		pDM_TOOLBAR->MakeUpdateFrame(_T("ESL_TOOLBAR_TBL"), _T("PK"), _T("NUMERIC"), strPK);
	}
	INT ids = pDM_TOOLBAR->SendFrame();
	pDM_TOOLBAR->EndFrame();
	if (ids == 0) {
		AfxMessageBox(_T("툴바 변경 사항이 적용되었습니다.\r\n변경사항은 다음 로그인시부터 적용됩니다."));
	} else {
		AfxMessageBox(_T("툴바 변경 사항을 적용하던중 에러가 발생하였습니다."));
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::TOOLBAR_Rearrange(CESL_DataMgr *pDM_OUT, CString strMenuAlias)
{
	CESL_DataMgr dm_toolgroup, dm_toolbar;
	dm_toolgroup.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	dm_toolbar.SetCONNECTION_INFO(pDM_OUT->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT PK, ALIAS FROM ESL_TOOLGROUP_TBL WHERE MENU_ALIAS='%s'"), strMenuAlias);
	dm_toolgroup.RestructDataManager(strQuery);
	INT nToolgroups = dm_toolgroup.GetRowCount();
	if (nToolgroups == 0) return 0;

	pDM_OUT->StartFrame();
	INT i, j, nIdx, nToolbars;
	CString strToolGroupAlias, strPK;
	for (i = 0; i < nToolgroups; i++) {
		strToolGroupAlias = dm_toolgroup.GetCellData(i, 1);
		strToolGroupAlias.TrimLeft();			strToolGroupAlias.TrimRight();

		strQuery.Format(_T("SELECT PK, ALIAS, IDX FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' ORDER BY IDX"), FLAG, ID, strMenuAlias, strToolGroupAlias);
		dm_toolbar.RestructDataManager(strQuery);
		nToolbars = dm_toolbar.GetRowCount();

		nIdx = 0;
		for (j = 0; j < nToolbars; j++) {
			strPK = dm_toolbar.GetCellData(j, 0);
			strQuery.Format(_T("UPDATE ESL_TOOLBAR_TBL SET IDX=%d WHERE PK=%s;"), nIdx, strPK);
			pDM_OUT->AddFrame(strQuery);
			
			nIdx++;
		}
	}
	
	INT ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();

	return 0;
}

VOID CMNG_ACCESS_CONTROL_1000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		OnSearch();
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CMNG_ACCESS_CONTROL_1000::IsAdmin(VOID)
{
	CString strLevel = GetUserLEVEL();
	strLevel.TrimLeft();		strLevel.TrimRight();
	strLevel.MakeUpper();
	if (strLevel == _T("A")) return TRUE;
	if (strLevel == _T("M")) return TRUE;
	
	return FALSE;
}

VOID CMNG_ACCESS_CONTROL_1000::OnSelchangedTreeMac(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	HTREEITEM hItem = m_tMenuTree.GetSelectedItem();
	DWORD dItemData = m_tMenuTree.GetItemData(hItem);
	
	INT nDataRow;
	CString strTmp;

	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return;

	if (dItemData < (DWORD)m_nTopBaseItemData) {
		nDataRow = (INT)dItemData;
		strTmp = pDM_MENU->GetCellData(_T("ACTION"), nDataRow);
		strTmp.TrimLeft();				strTmp.TrimRight();
		strTmp.MakeUpper();
		if (strTmp == _T("D")) {
			REFRESH_Toolbar(pDM_MENU->GetCellData(_T("ALIAS"), nDataRow));
		} else {
			CLEAR_ToolBarGrid();
		}
	}
	*pResult = 0;
}

VOID CMNG_ACCESS_CONTROL_1000::CLEAR_ToolBarGrid(VOID)
{
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return;
	pToolbarGrid->Clear();
	pToolbarGrid->SetColumnInfo();

	return;
}

INT CMNG_ACCESS_CONTROL_1000::MENU_Delete(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}

	CESL_DataMgr *pDM_MENUGROUP = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴그룹"));
	if (pDM_MENUGROUP == NULL) return -1;
	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return -1;

	UpdateData(TRUE);
	HTREEITEM hItem = m_tMenuTree.GetSelectedItem();
	DWORD dItemData = m_tMenuTree.GetItemData(hItem);
	if (dItemData >= (DWORD)m_nTopBaseItemData) {
		AfxMessageBox(_T("최상단 메뉴는 삭제할 수 없습니다."));
		return 0;
	}
	if (AfxMessageBox(_T("정말 삭제하시겠습니까?"), MB_YESNO) != IDYES) return 0;
	HTREEITEM hParentItem = m_tMenuTree.GetParentItem(hItem);
	if (hParentItem == NULL) {
		AfxMessageBox(_T("삭제할 메뉴를 선택하여야 합니다."));
		return 0;
	}
	DWORD dParentItemData = m_tMenuTree.GetItemData(hParentItem);
	
	CString strQuery;
	CString strMenuPK;
	// 최상단 메뉴에 속한 메뉴 그룹인경우
	if (dParentItemData >= (DWORD)m_nTopBaseItemData) {	
		strMenuPK = pDM_MENU->GetCellData(_T("PK"), (INT)dItemData);
		strMenuPK.TrimLeft();			strMenuPK.TrimRight();
		DELETE_Menu(hItem, strMenuPK);
		return 0;
	}
	CString strAction = pDM_MENU->GetCellData(_T("ACTION"), (INT)dParentItemData);
	strAction.TrimLeft();		strAction.TrimRight();
	strAction.MakeUpper();
	CString strMenuAlias = pDM_MENU->GetCellData(_T("ALIAS"), (INT)dParentItemData);

	strMenuPK = pDM_MENU->GetCellData(_T("PK"), (INT)dItemData);
	strMenuPK.TrimLeft();			strMenuPK.TrimRight();
	DELETE_Menu(hItem, strMenuPK, strMenuAlias, strAction);
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::DELETE_Menu(HTREEITEM hItem, CString strMenuPK, CString strMenuAlias, CString strAction)
{
	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return -1;

	INT ids;
	CString strQuery;
	pDM_MENU->StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE PK=%s;"), strMenuPK);
	pDM_MENU->AddFrame(strQuery);
	if (strMenuAlias.GetLength() > 0 && strAction == _T("D")) {
		strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS='%s';"), strMenuAlias);
		pDM_MENU->AddFrame(strQuery);
	}
	ids = pDM_MENU->SendFrame();
	pDM_MENU->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("삭제중 에러가 발생하였습니다."));
		return -1;
	} else {
		m_tMenuTree.DeleteItem(hItem);
		UpdateData(FALSE);
		//AfxMessageBox(_T("삭제 되었습니다."));
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::MENU_Add(VOID)
{
	if (IsAdmin() == FALSE) {
		AfxMessageBox(_T("해당 기능은 [시스템관리자] 혹은 [사용자관리자]만 가능한 기능입니다."));
		return 0;
	}
	HTREEITEM hItem = m_tMenuTree.GetSelectedItem();
	DWORD dItemData = m_tMenuTree.GetItemData(hItem);
	CESL_DataMgr *pDM_MENUGROUP = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴그룹"));
	if (pDM_MENUGROUP == NULL) return -1;
	CESL_DataMgr *pDM_MENU = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_메뉴"));
	if (pDM_MENU == NULL) return -1;

	INT nDataRow;
	// TOP 메뉴 그룹이 선택된경우
	if (dItemData >= (DWORD)m_nTopBaseItemData) {
		nDataRow = dItemData - m_nTopBaseItemData;
		m_strMenuGroupAlias = pDM_MENUGROUP->GetCellData(_T("ALIAS"), nDataRow);
		MENU_Add(m_strMenuGroupAlias);
		return 0;
	}

	HTREEITEM hParentItem = m_tMenuTree.GetParentItem(hItem);
	DWORD dParentItemData = m_tMenuTree.GetItemData(hParentItem);

	// TOP 메뉴에 바로 딸린 메뉴인경우
	if (dParentItemData >= (DWORD)m_nTopBaseItemData) {
		nDataRow = dItemData;
		m_strMenuGroupAlias = pDM_MENU->GetCellData(_T("ALIAS"), nDataRow);
		MENU_Add(m_strMenuGroupAlias);
		return 0;
	}
	CString strAction = pDM_MENU->GetCellData(_T("ACTION"), (INT)dItemData);
	strAction.TrimLeft();		strAction.TrimRight();
	strAction.MakeUpper();
	if (strAction == _T("D")) {
		AfxMessageBox(_T("LoadDLL에는 메뉴를 추가할 수 없습니다."));
		return 0;
	}

	// POPUP 메뉴로 해서 뜬 경우
	if (strAction == _T("P")) {
		m_strMenuGroupAlias = pDM_MENU->GetCellData(_T("TARGET"), (INT)dItemData);
		MENU_Add(m_strMenuGroupAlias);
		return 0;
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_1000::MENU_Add(CString strGroupAlias)
{
	m_strMenuGroupAlias = strGroupAlias;

	CMNG_ACCESS_CONTROL_3000 dlg(this);
	if (dlg.DoModal() != IDOK) return -1;
	if (dlg.m_nSelectedRow == -1) {
		AfxMessageBox(_T("선택된 자료가 없습니다."));
		return 0;
	}
	CESL_DataMgr *pDM = dlg.FindDM(_T("DM_MNG_ACCESS_CONTROL_3000"));
	if (pDM == NULL) return -1;

	CString strPK, strQuery, strFlag;
	INT ids;
	BOOL bAddToolBar = FALSE;

	INT nSelectedRow = dlg.m_nSelectedRow;
	ids = MENU_Copy(pDM, nSelectedRow, FALSE);
	return 0;
}

VOID CMNG_ACCESS_CONTROL_1000::OnMouseDownGridToolbar(short Button, short Shift, long x, long y) 
{
EFS_BEGIN
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return;
	INT nMouseRow = pToolbarGrid->GetMouseRow();
	if (nMouseRow == 0) return;
	if (nMouseRow != pToolbarGrid->GetRow()) pToolbarGrid->SetRow(nMouseRow);

	pToolbarGrid->SetReverse(pToolbarGrid->GetRow() -1);	
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return;

	if (Button != 2) return;
	// 오른쪽 버튼일 경우
	INT nRowCount = pDM_TOOLBAR->GetRowCount();
	if (nRowCount == 0) return;
	if (pToolbarGrid->GetMouseRow() == 0) return;
	if (pToolbarGrid->GetRow() == 0) return;
	INT nRow = pToolbarGrid->GetRow()-1;
	FreeMenu();
	m_pMenu = new CMenu;
	if (m_pMenu == NULL) return;
	m_pMenu->CreatePopupMenu();
	INT nCol = pToolbarGrid->GetCol();

	if (nCol == 1 || nCol == 2) {
		if (nRow > 0) m_pMenu->AppendMenu( MF_STRING , 4000 , _T("위로 이동") );
		if (nRow < nRowCount-1) m_pMenu->AppendMenu( MF_STRING , 4001 , _T("아래로 이동") );
	}		
	if (nCol == 3) {
		m_pMenu->AppendMenu( MF_STRING , 4002 , _T("보이기") );
		m_pMenu->AppendMenu( MF_STRING , 4003 , _T("안보이기") );
	}
	CPoint pos;
	GetCursorPos(&pos);
	SetForegroundWindow();
	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);

	return;
EFS_END
return;
}

VOID CMNG_ACCESS_CONTROL_1000::FreeMenu(VOID)
{
	if (m_pMenu) {
		delete m_pMenu;
		m_pMenu = NULL;
	}
	return;
}

VOID CMNG_ACCESS_CONTROL_1000::OnUserMenu( UINT  nIDC )
{
	CString str;
	str.Format(_T("%d"), nIDC);
	
	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return;
	INT nRow = pToolbarGrid->GetRow()-1;
	// 위로 이동
	if (nIDC == 4000) {
		SWAP_ToolBarDMRow(nRow, nRow-1);	
	}

	// 아래로 이동
	if (nIDC == 4001) {
		SWAP_ToolBarDMRow(nRow, nRow+1);
	}

	// 보이기
	if (nIDC == 4002) {
		ApplyComboBoxInToolbarGridSell(nRow, TRUE);
	}

	// 안보이기
	if (nIDC == 4003) {
		ApplyComboBoxInToolbarGridSell(nRow, FALSE);
	}

	return;
}

VOID CMNG_ACCESS_CONTROL_1000::SWAP_ToolBarDMRow(INT nSource, INT nTarget)
{
	CESL_DataMgr *pDM_TOOLBAR = FindDM(_T("DM_MNG_ACCESS_CONTROL_1000_툴바"));
	if (pDM_TOOLBAR == NULL) return;

	INT nAlias = 0;
	CString strAlias[64];
	strAlias[nAlias++] = _T("ALIAS");
	strAlias[nAlias++] = _T("CAPTION");
	strAlias[nAlias++] = _T("IDX");
	strAlias[nAlias++] = _T("PK");
	strAlias[nAlias++] = _T("SM");
	strAlias[nAlias++] = _T("STYLE");
	strAlias[nAlias++] = _T("TOOLGROUP_ALIAS");
	strAlias[nAlias++] = _T("TOOLTIP");
	strAlias[nAlias++] = _T("TYPE");

	CString strSource, strTarget;
	for (INT i = 0; i < nAlias; i++) {
		strSource = _T("");
		pDM_TOOLBAR->GetCellData(strAlias[i], nSource, strSource);

		strTarget = _T("");
		pDM_TOOLBAR->GetCellData(strAlias[i], nTarget, strTarget);

		pDM_TOOLBAR->SetCellData(strAlias[i], strSource, nTarget);
		pDM_TOOLBAR->SetCellData(strAlias[i], strTarget, nSource);
	}

	CESL_Grid *pToolbarGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_1000"), _T("툴바버튼목록"));
	if (pToolbarGrid == NULL) return;
	pToolbarGrid->Clear();
	pToolbarGrid->SetColumnInfo();
	INT nRows = pDM_TOOLBAR->GetRowCount();

	if (nRows == 0) return;
	pToolbarGrid->SetRows(nRows+1);
	CString str;
	for (i = 0; i < nRows; i++) {
		str.Format(_T("%d"), i);
		pDM_TOOLBAR->SetCellData(_T("IDX"), str, i);
		pToolbarGrid->SetTextMatrix(i+1, 1, pDM_TOOLBAR->GetCellData(_T("CAPTION"), i));
		if (pDM_TOOLBAR->GetCellData(_T("STYLE"), i) != _T("B")) {
			pToolbarGrid->SetTextMatrix(i+1, 1, _T("----------------"));
		}
		pToolbarGrid->SetTextMatrix(i+1, 2, pDM_TOOLBAR->GetCellData(_T("IDX"), i));
		str = pDM_TOOLBAR->GetCellData(_T("TYPE"), i);
		str.TrimLeft();				str.TrimRight();
		if (str.GetLength() == 0) {
			pToolbarGrid->SetTextMatrix(i+1, 3, _T("예"));
		} else {
			pToolbarGrid->SetTextMatrix(i+1, 3, _T("아니오"));
		}
	}

	return;
}
INT CMNG_ACCESS_CONTROL_1000::Upload_MenuToolbarFile()
{

	/// 이동도서관은 파일로 저장하지 않는다.
	if( m_pInfo->MODE == 10000 || ( m_pInfo->MODE == 30000 ) )
	{
	}
	else
	{
		CMenuToolbar_Mgr MenuToolbar_Mgr (this);
		MenuToolbar_Mgr.SetConnectionInfo (m_pInfo->CONNECTION_INFO );
		MenuToolbar_Mgr.SetDefalutValue (ID,FLAG,_T("y")) ;
		MenuToolbar_Mgr.SetUploadValue (true,true,true) ; 

		MenuToolbar_Mgr.Upload_File () ;
	}
	return 0 ;
}
