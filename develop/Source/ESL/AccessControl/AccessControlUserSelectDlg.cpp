// AccessControlUserSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlUserSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "NewButton.h"

/////////////////////////////////////////////////////////////////////////////
// CAccessControlUserSelectDlg dialog


CAccessControlUserSelectDlg::CAccessControlUserSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlUserSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlUserSelectDlg)
	// KOL.UDF.022
	m_pGrid = NULL;
	m_pDM = NULL;
	FLAG = ' ';
	NAME = _T("");
	//}}AFX_DATA_INIT
}


VOID CAccessControlUserSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlUserSelectDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlUserSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlUserSelectDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlUserSelectDlg message handlers

BOOL CAccessControlUserSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;
	
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);

	SetTimer (0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlUserSelectDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		KillTimer(0);
		ShowUserList();
	}
	CDialog::OnTimer(nIDEvent);
}

VOID CAccessControlUserSelectDlg::ShowUserList(VOID)
{
	if (ReadCfgFile.ReadCfgFile(_T("")) < 0) return;
	if (!m_pDM) return;

	INT ids;
	// 접속정보를 설정하고
	m_pDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// 데이타를 SELECT해온다.
	ids = m_pDM->RestructDataManager(_T("SELECT PK, FLAG, USER_ID, METHOD_NAME, USER_METHOD_NAME FROM ESL_MENU_METHOD_TBL ORDER BY FLAG"));
	if (ids < 0) {
		AfxMessageBox(_T("메소드와 유져 정보를 읽지 못하였습니다."));
		return;
	}
	INT nRows = m_pDM->GetRowCount();

	m_pGrid->Clear();
	m_pGrid->SetRows(nRows+2);

	m_pGrid->SetColWidth(0, 0, 40*15);
	m_pGrid->SetColAlignment(0, 4);
	m_pGrid->SetTextMatrix(0, 0, _T("분류"));

	m_pGrid->SetColumnInfo(1-1, _T("ID"), 200);

	if (nRows == 0) {
		AfxMessageBox(_T("메소드나 사용자 정보가 없습니다."));
		return;
	}

	// ADMIN 설정 
	m_pGrid->SetTextMatrix(1, 0, _T("S"));
	m_pGrid->SetTextMatrix(1, 1, _T("Administrator"));

	// 데이타를 출력한다.
	CString title;
	for ( INT i = 0 ; i < nRows ; i++ ) {
		m_pGrid->SetTextMatrix(i+2, 0, m_pDM->GetCellData(i, 1));
		
		if (m_pDM->GetCellData(i, 1) == _T("M")) m_pGrid->SetTextMatrix(i+2, 1, m_pDM->GetCellData(i, 3));
		if (m_pDM->GetCellData(i, 1) == _T("U")) m_pGrid->SetTextMatrix(i+2, 1, m_pDM->GetCellData(i, 2));
	}
	INT nGridRows = m_pGrid->GetRows();
	CString str;
	for ( i = 1 ; i < nGridRows ; i++ ) {
		str = _T("");
		str = m_pGrid->GetTextMatrix(i, 0);
		m_pGrid->SetRow(i);
		m_pGrid->SetCol(0);
		if (str == _T("S")) m_pGrid->SetCellBackColor(RGB(255, 207, 255));
		if (str == _T("M")) m_pGrid->SetCellBackColor(RGB(255, 255, 156));
		if (str == _T("U")) m_pGrid->SetCellBackColor(RGB(206, 255, 206));
	}
	return;
}

VOID CAccessControlUserSelectDlg::OnbOK() 
{
	OnDblClickMshflexgrid1();
}

VOID CAccessControlUserSelectDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

VOID CAccessControlUserSelectDlg::OnDestroy() 
{
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}	
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	CDialog::OnDestroy();	
}

BEGIN_EVENTSINK_MAP(CAccessControlUserSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAccessControlUserSelectDlg)
	ON_EVENT(CAccessControlUserSelectDlg, IDC_MSHFLEXGRID1, -605 /* MouseDown */, OnMouseDownMshflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CAccessControlUserSelectDlg, IDC_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlUserSelectDlg::OnMouseDownMshflexgrid1(short Button, short Shift, long x, long y) 
{
	m_pGrid->SetReverse(m_pGrid->GetRow()-1);	
}

VOID CAccessControlUserSelectDlg::OnDblClickMshflexgrid1() 
{
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_pGrid->GetRow() -1;

	if(SelectedRow == 0)
	{
		FLAG = 'S';
		NAME = _T("Administrator");
	}
	else
	if (m_pDM->GetCellData(SelectedRow-1, 1) == _T("M")) {
		FLAG = 'M';
		NAME = m_pDM->GetCellData(SelectedRow-1, 3);
	}
	else
	if (m_pDM->GetCellData(SelectedRow-1, 1) == _T("U")) {
		FLAG = 'U';
		NAME = m_pDM->GetCellData(SelectedRow-1, 2);
	}

	CDialog::OnOK();
}

INT CAccessControlUserSelectDlg::OnClear_ESL_TOOLBAR_TBL(VOID)
{
	CESL_DataMgr dm1;
	dm1.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	dm1.RestructDataManager(_T("SELECT PK, MENU_ALIAS FROM ESL_TOOLBAR_TBL"));
	INT nRows = dm1.GetRowCount();
	INT i, ids = 0;

	CString strQuery, str;
	INT nDelete = 0;
	for (i = 0; i < nRows; i++) {
		strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_TBL WHERE ALIAS='%s'"), dm1.GetCellData(i, 1));
		str = _T("");
		dm1.GetOneValueQuery(strQuery, str);
		str.TrimLeft();				str.TrimRight();

		if (str == _T("0")) {
			/*
			dm1.StartFrame();
			strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE PK=%s;"), dm1.GetCellData(i, 0));
			dm1.AddFrame(strQuery);
			ids = dm1.SendFrame();
			dm1.EndFrame();
			*/
			nDelete++;
		}
	}

	str.Format(_T("%d"), nDelete);
	AfxMessageBox(str);
	return 0;
}

INT CAccessControlUserSelectDlg::OnClear_ESL_TOOLGROUP_TBL(VOID)
{
	CESL_DataMgr dm1;
	dm1.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	dm1.RestructDataManager(_T("SELECT PK, MENU_ALIAS FROM ESL_TOOLGROUP_TBL"));
	INT nRows = dm1.GetRowCount();
	INT i, ids = 0;

	CString strQuery, str;
	INT nDelete = 0;
	for (i = 0; i < nRows; i++) {
		strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_TBL WHERE ALIAS='%s'"), dm1.GetCellData(i, 1));
		str = _T("");
		dm1.GetOneValueQuery(strQuery, str);
		str.TrimLeft();				str.TrimRight();

		if (str == _T("0")) {
			/*
			dm1.StartFrame();
			strQuery.Format(_T("DELETE FROM ESL_TOOLGROUP_TBL WHERE PK=%s;"), dm1.GetCellData(i, 0));
			dm1.AddFrame(strQuery);
			ids = dm1.SendFrame();
			dm1.EndFrame();
			*/
			nDelete++;
		}
	}

	str.Format(_T("%d"), nDelete);
	AfxMessageBox(str);
	return 0;
}