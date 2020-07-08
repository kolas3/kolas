// AccessControlSmartIDMenuListInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "accesscontrol.h"
#include "AccessControlSmartIDMenuListInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListInsertDlg dialog


CAccessControlSmartIDMenuListInsertDlg::CAccessControlSmartIDMenuListInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSmartIDMenuListInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSmartIDMenuListInsertDlg)
	//}}AFX_DATA_INIT

	FLAG = 'S';
	USER_ID = _T("Administrator");

	m_strRETURN_WORKING_STATUS = _T("");
	m_strRETURN_TopMenuGroupAlias = _T("");
	m_strRETURN_MenuGroupAlias = _T("");
	m_strRETURN_MenuAlias = _T("");
	m_strRETURN_TITLE_DESC = _T("");

	// KOL.UDF.022
	INT i = 0;

	m_nWorkingStatus = 0;
	m_pWorkingStatus = NULL;
	m_pWorkingStatusDesc = NULL;
	m_strWorkingStatus = _T("");
	m_pGrid = NULL;
	CONNECTION_INFO = _T("");
	m_nTopMenuGroup = 0;

	for(i=0;i<128;i++)
		m_strTopMenuGroup[i] = _T("");
}


VOID CAccessControlSmartIDMenuListInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSmartIDMenuListInsertDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	DDX_Control(pDX, IDC_cWORKING_STATUS, m_cWorkingStatus);
	DDX_Control(pDX, IDC_cTOP_MENU_GROUP, m_cTopMenuGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSmartIDMenuListInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSmartIDMenuListInsertDlg)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_CBN_SELCHANGE(IDC_cWORKING_STATUS, OnSelchangecWORKINGSTATUS)
	ON_CBN_SELCHANGE(IDC_cTOP_MENU_GROUP, OnSelchangecTOPMENUGROUP)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListInsertDlg message handlers

BOOL CAccessControlSmartIDMenuListInsertDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	FreeMenuGroupList();

	return CDialog::DestroyWindow();
}

BOOL CAccessControlSmartIDMenuListInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gLIST, this);

	m_dm.SetCONNECTION_INFO(CONNECTION_INFO);

	SetTimer(0, 50, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlSmartIDMenuListInsertDlg::OnbOK() 
{
	UpdateData(TRUE);
	if (m_arrAlias.GetSize() == 0) {
		AfxMessageBox(_T("검색된 메뉴가 없습니다."));
		return;
	}

	INT nRow = m_pGrid->GetRow() - 1;
	GetDlgItem(IDC_cWORKING_STATUS)->GetWindowText(m_strRETURN_WORKING_STATUS);
	m_strRETURN_WORKING_STATUS.TrimLeft();		m_strRETURN_WORKING_STATUS.TrimRight();
	m_strRETURN_WORKING_STATUS.MakeUpper();
	m_cTopMenuGroup.GetWindowText(m_strRETURN_TopMenuGroupAlias);
	m_strRETURN_TopMenuGroupAlias.TrimLeft();	m_strRETURN_TopMenuGroupAlias.TrimRight();
	m_strRETURN_MenuGroupAlias = m_arrGroupAlias.GetAt(nRow);
	m_strRETURN_MenuAlias = m_arrAlias.GetAt(nRow);
	m_strRETURN_TITLE_DESC = m_arrTitleDesc.GetAt(nRow);

	CDialog::OnOK();
}

VOID CAccessControlSmartIDMenuListInsertDlg::OnbCANCEL() 
{
	CDialog::OnCancel();
}

VOID CAccessControlSmartIDMenuListInsertDlg::InitComboBox(VOID)
{
	m_cWorkingStatus.ResetContent();
	
	INT nSelect = 0;
	for (INT i = 0; i < m_nWorkingStatus; i++) {
		m_cWorkingStatus.AddString(m_pWorkingStatus[i]);
	}
	if (nSelect >= 0) m_cWorkingStatus.SetCurSel(nSelect);

	UpdateData(FALSE);
	return;
}

VOID CAccessControlSmartIDMenuListInsertDlg::ShowWorkingStatusDesc(VOID)
{
	CString str;
	CString strDesc = _T("");

	UpdateData(TRUE);
	INT nSelect = m_cWorkingStatus.GetCurSel();
	if (nSelect < 0) return;
	str = m_pWorkingStatus[nSelect];
	str.TrimLeft();		str.TrimRight();

	for (INT i = 0; i < m_nWorkingStatus; i++) {
		if (m_pWorkingStatus[i] == str) {
			strDesc = m_pWorkingStatusDesc[i];
			break;
		}
	}

	GetDlgItem(IDC_STATIC_WORKING_STATUS_DESC)->SetWindowText(strDesc);

}

VOID CAccessControlSmartIDMenuListInsertDlg::OnSelchangecWORKINGSTATUS() 
{
	ShowWorkingStatusDesc();
}

INT CAccessControlSmartIDMenuListInsertDlg::InitTopMenuGroupComboBox(VOID)
{
	m_dm.RestructDataManager(_T("SELECT ALIAS FROM ESL_MENUGROUP_TBL WHERE GROUPTYPE='T' ORDER BY ALIAS desc"));

	m_cTopMenuGroup.ResetContent();
	m_nTopMenuGroup = m_dm.GetRowCount();

	for (INT i = 0; i < m_nTopMenuGroup; i++) {
		m_strTopMenuGroup[i] = m_dm.GetCellData(i, 0);
		m_cTopMenuGroup.AddString(m_strTopMenuGroup[i]);
	}
	m_cTopMenuGroup.SetCurSel(0);

	UpdateData(FALSE);
	return 0;
}

VOID CAccessControlSmartIDMenuListInsertDlg::OnSelchangecTOPMENUGROUP() 
{
	UpdateData(TRUE);
	INT nIdx = m_cTopMenuGroup.GetCurSel();
	ShowTopMenuGroupList(nIdx);
}

INT CAccessControlSmartIDMenuListInsertDlg::ShowTopMenuGroupList(INT nIdx)
{
	CString strTopMenuGroupAlias = m_strTopMenuGroup[nIdx];

	RefreshData(strTopMenuGroupAlias);
	return 0;
}

INT CAccessControlSmartIDMenuListInsertDlg::RefreshData(CString strTopMenuGroupAlias)
{
	INT ids;

	FreeMenuGroupList();
	m_arrGroupAlias.RemoveAll();
	m_arrAlias.RemoveAll();
	m_arrTitleDesc.RemoveAll();
		
	CString strQuery;
	strQuery.Format(_T("SELECT ALIAS, TITLE_DESC FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s' AND IDX >= 0 AND GROUP_ALIAS='%s'"), FLAG, USER_ID, strTopMenuGroupAlias);
	ids = m_dm.RestructDataManager(strQuery, &m_Progress1);

	INT nRows = m_dm.GetRowCount();
	if (nRows == 0) return -1;
	INT i;
	CString strAlias;
	for (i = 0; i < nRows; i++) {
		strAlias = m_dm.GetCellData(i, 0);
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

INT CAccessControlSmartIDMenuListInsertDlg::FreeMenuGroupList(VOID)
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

INT CAccessControlSmartIDMenuListInsertDlg::AddMenuGroup(CString strGroupAlias)
{
	CString *pData = new CString;
	pData->Format(_T("%s"), strGroupAlias);

	m_MenuGroupList.AddTail((CObject*)pData);
	return 0;
}

INT CAccessControlSmartIDMenuListInsertDlg::GetMenu(CString strGroupAlias)
{
	INT ids;

	CString strQuery;
	//                           0         1         2        3      4        5
	strQuery.Format(_T("SELECT GROUP_ALIAS, ALIAS, TITLE_DESC, STYLE, ACTION, TARGET FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s' AND GROUP_ALIAS='%s'"), FLAG, USER_ID, strGroupAlias);
	ids = m_dm.RestructDataManager(strQuery, &m_Progress1);

	INT nRows = m_dm.GetRowCount();
	CString strStyle, strAction, strAlias, strTarget, strTitleDesc;
	for (INT i = 0; i < nRows; i++) {
		strStyle = m_dm.GetCellData(i, 3);
		if (strStyle == _T("P")) continue;
		strAction = m_dm.GetCellData(i, 4);
		strGroupAlias = m_dm.GetCellData(i, 0);
		strAlias = m_dm.GetCellData(i, 1);
		strTitleDesc = m_dm.GetCellData(i, 2);
		strTarget = m_dm.GetCellData(i, 5);
		if (strAction == _T("D")) {
			AddMenu(strGroupAlias, strAlias, strTitleDesc);
		}
		if (strAction == _T("P")) {
			AddMenuGroup(strTarget);
		}
	}
	return 0;
}

INT CAccessControlSmartIDMenuListInsertDlg::AddMenu(CString strGroupAlias, CString strAlias, CString strTitleDesc)
{
	if (strAlias == _T("홈")) return -1;
	m_arrGroupAlias.Add(strGroupAlias);
	m_arrAlias.Add(strAlias);
	m_arrTitleDesc.Add(strTitleDesc);

	return 0;
}

INT CAccessControlSmartIDMenuListInsertDlg::RemoveHeadGroup(CString &strGroupAlias)
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

INT CAccessControlSmartIDMenuListInsertDlg::ShowList()
{
	INT nRows = m_arrAlias.GetSize();
	
	m_pGrid->Clear();
	m_pGrid->SetCols(0, 4);
	if (nRows == 0) m_pGrid->SetRows(2);
		else m_pGrid->SetRows(nRows+1);
	m_pGrid->SetColWidth(0, 0, 30*15);
	m_pGrid->SetColumnInfo(0, _T("GroupAlias"), 135, 1);
	m_pGrid->SetColumnInfo(1, _T("Alias"), 155, 1);
	m_pGrid->SetColumnInfo(2, _T("Title"), 220, 1);

	for (INT i = 0; i < nRows; i++) {
		m_pGrid->SetTextMatrix(i+1, 1, m_arrGroupAlias.GetAt(i));
		m_pGrid->SetTextMatrix(i+1, 2, m_arrAlias.GetAt(i));
		m_pGrid->SetTextMatrix(i+1, 3, m_arrTitleDesc.GetAt(i));
	}
	return 0;
}

VOID CAccessControlSmartIDMenuListInsertDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		InitComboBox();
		ShowWorkingStatusDesc();

		InitTopMenuGroupComboBox();

		ShowTopMenuGroupList(0);
	}

	CDialog::OnTimer(nIDEvent);
}
