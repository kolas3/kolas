// AccessControlSmartIDMenuListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "accesscontrol.h"
#include "AccessControlSmartIDMenuListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "AccessControlSmartIDMenuListModify.h"
#include "AccessControlSmartIDMenuListInsertDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListDlg dialog


CAccessControlSmartIDMenuListDlg::CAccessControlSmartIDMenuListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSmartIDMenuListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSmartIDMenuListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = NULL;

	FLAG = 'S';
	USER_ID = _T("Administrator");

	// KOL.UDF.022
	INT i=0;
	
	CONNECTION_INFO = _T("");
	m_nWorkingStatus = 0;
	for(i=0;i<256;i++){
		m_strWorkingStatus[i] = _T("");
		m_strWorkingStatusDesc[i] = _T("");
	}
}


VOID CAccessControlSmartIDMenuListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSmartIDMenuListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSmartIDMenuListDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSmartIDMenuListDlg)
	ON_BN_CLICKED(IDC_bSMARTIDINSERT, OnbSMARTIDINSERT)
	ON_BN_CLICKED(IDC_bSMARTIDDELETE2, OnbSMARTIDDELETE2)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bSMARTIDMODIFY, OnbSMARTIDMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSmartIDMenuListDlg message handlers

VOID CAccessControlSmartIDMenuListDlg::OnbSMARTIDINSERT() 
{
	CAccessControlSmartIDMenuListInsertDlg dlg;
	dlg.FLAG = FLAG;
	dlg.USER_ID = USER_ID;
	dlg.m_nWorkingStatus = m_nWorkingStatus;
	dlg.m_pWorkingStatus = m_strWorkingStatus;
	dlg.m_pWorkingStatusDesc = m_strWorkingStatusDesc;
	dlg.CONNECTION_INFO = CONNECTION_INFO;
	if (dlg.DoModal() != IDOK) return;

	CString strRecKey;
	m_dm.MakeRecKey(strRecKey);
	m_dm.StartFrame();

	m_dm.InitDBFieldData();
	m_dm.AddDBFieldData(_T("REC_KEY"),				_T("NUMERIC"), strRecKey);
	m_dm.AddDBFieldData(_T("WORKING_STATUS"),		_T("STRING"), dlg.m_strRETURN_WORKING_STATUS);
	m_dm.AddDBFieldData(_T("TopMenuGroupAlias"),	_T("STRING"), dlg.m_strRETURN_TopMenuGroupAlias);
	m_dm.AddDBFieldData(_T("MenuGroupAlias"),		_T("STRING"), dlg.m_strRETURN_MenuGroupAlias);
	m_dm.AddDBFieldData(_T("MenuAlias"),			_T("STRING"), dlg.m_strRETURN_MenuAlias);
	m_dm.AddDBFieldData(_T("TITLE_DESC"),			_T("STRING"), dlg.m_strRETURN_TITLE_DESC);
	m_dm.MakeInsertFrame(_T("ESL_SMARTID_MENU_TBL"));

	INT ids = SENDFRAME(&m_dm);

	if (ids < 0) {
		AfxMessageBox(_T("입력작업중에 에러가 발생하였습니다."));
	} else {
		OnbREFRESH();
		AfxMessageBox(_T("입력되었습니다."));
	}
}

VOID CAccessControlSmartIDMenuListDlg::OnbSMARTIDDELETE2() 
{
	if (m_dm.GetRowCount() == 0) return;
	
	INT nRow = m_pGrid->GetRow()-1;
	CString strRecKey;
	strRecKey = m_dm.GetCellData(nRow, 0);
	strRecKey.TrimLeft();		strRecKey.TrimRight();

	if (AfxMessageBox(_T("정말 삭제하시겠습니까?"), MB_YESNO) != IDYES) return;

	CString strQuery;
	m_dm.StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_SMARTID_MENU_TBL WHERE REC_KEY=%s;"), strRecKey);
	m_dm.AddFrame(strQuery);

	INT ids = SENDFRAME(&m_dm);
	if (ids < 0) {
		AfxMessageBox(_T("삭제작업중에 에러가 발생하였습니다."));
	} else {
		OnbREFRESH();
		AfxMessageBox(_T("삭제되었습니다."));
	}
}

VOID CAccessControlSmartIDMenuListDlg::OnbOK() 
{
	CDialog::OnOK();	
}

BOOL CAccessControlSmartIDMenuListDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	return CDialog::DestroyWindow();
}

BOOL CAccessControlSmartIDMenuListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitWorkingStatus();

	m_dm.SetCONNECTION_INFO(CONNECTION_INFO);

	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gLIST, this);

	SetTimer (0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CAccessControlSmartIDMenuListDlg::RefreshList(VOID)
{
	CString strQuery;
	//                         0            1                2                3             4           5
	strQuery.Format(_T("SELECT REC_KEY, WORKING_STATUS, TopMenuGroupAlias, MenuGroupAlias, MenuAlias, TITLE_DESC FROM ESL_SMARTID_MENU_TBL ORDER BY WORKING_STATUS"));

	INT ids = m_dm.RestructDataManager(strQuery);
	return ids;
}

INT CAccessControlSmartIDMenuListDlg::OnbREFRESH(VOID)
{
	INT ids;
	ids = RefreshList();
	
	InitGrid();
	INT nRows = m_dm.GetRowCount();
	if (nRows == 0) m_pGrid->SetRows(2);
		else m_pGrid->SetRows(nRows+1);

	for (INT i = 0; i < nRows; i++) {
		m_pGrid->SetTextMatrix(i+1, 1, m_dm.GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, m_dm.GetCellData(i, 2));
		m_pGrid->SetTextMatrix(i+1, 3, m_dm.GetCellData(i, 3));
		m_pGrid->SetTextMatrix(i+1, 4, m_dm.GetCellData(i, 4));
		m_pGrid->SetTextMatrix(i+1, 5, m_dm.GetCellData(i, 5));
	}
	
	CString strVersion;
	m_dm.GetOneValueQuery(_T("SELECT VERSION FROM ESL_VERSION_TBL WHERE NAME='ESL_SMARTID_MENU_TBL'"), strVersion);
	strVersion.TrimLeft();				strVersion.TrimRight();
	CString strTitle;
	strTitle.Format(_T("SmartID Menu List 관리 (VERSION : %s)"), strVersion);
	SetWindowText(strTitle);

	return 0;
}

VOID CAccessControlSmartIDMenuListDlg::InitGrid(VOID)
{
	m_pGrid->RemoveAll();
	m_pGrid->SetRows(2);
	m_pGrid->SetCols(0, 6);
	m_pGrid->SetColWidth(0, 0, 30*15);
	m_pGrid->SetColumnInfo(1-1, _T("Working"), 60, 4);
	m_pGrid->SetColumnInfo(2-1, _T("TopMenu"), 140, 1);
	m_pGrid->SetColumnInfo(3-1, _T("MenuGroup"), 130, 1);
	m_pGrid->SetColumnInfo(4-1, _T("MenuAlias"), 130, 1);
	m_pGrid->SetColumnInfo(5-1, _T("Title"), 180, 1);

	return;
}

VOID CAccessControlSmartIDMenuListDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		OnbREFRESH();
	}
	
	CDialog::OnTimer(nIDEvent);
}

INT CAccessControlSmartIDMenuListDlg::SENDFRAME(CESL_DataMgr *pDM)
{
	if (pDM == NULL) return -1;
	pDM->AddFrame(_T("UPDATE ESL_VERSION_TBL SET VERSION=VERSION+1 WHERE NAME='ESL_SMARTID_MENU_TBL';"));

	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	return ids;
}

VOID CAccessControlSmartIDMenuListDlg::InitWorkingStatus(VOID)
{
	m_nWorkingStatus = 0;

	CStdioFile fd;
	if (fd.Open(_T("..\\CFG\\K2Upgrade-흐름설정.txt"), CFile::modeRead | CFile::typeBinary) == FALSE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	CString strLine;
	CString strWorkingStatus, strWorkingStatusDesc;
	INT nPos;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();				strLine.TrimRight();
		nPos = strLine.Find('|', 0);
		if (nPos == -1) continue;
		m_strWorkingStatus[m_nWorkingStatus] = strLine.Mid(0, nPos);
		m_strWorkingStatus[m_nWorkingStatus].TrimLeft();		m_strWorkingStatus[m_nWorkingStatus].TrimRight();
		m_strWorkingStatusDesc[m_nWorkingStatus] = strLine.Mid(nPos+1);
		m_strWorkingStatusDesc[m_nWorkingStatus].TrimLeft();	m_strWorkingStatusDesc[m_nWorkingStatus].TrimRight();

		m_nWorkingStatus++;
	}

	fd.Close();
	return;
}

VOID CAccessControlSmartIDMenuListDlg::OnbSMARTIDMODIFY() 
{
	if (m_dm.GetRowCount() == 0) return;

	CAccessControlSmartIDMenuListModify dlg;
	dlg.m_nWorkingStatus = m_nWorkingStatus;
	dlg.m_pWorkingStatus = m_strWorkingStatus;
	dlg.m_pWorkingStatusDesc = m_strWorkingStatusDesc;

	INT nRow = m_pGrid->GetRow()-1;
	CString strRecKey;
	strRecKey = m_dm.GetCellData(nRow, 0);
	strRecKey.TrimLeft();		strRecKey.TrimRight();
	dlg.m_strWorkingStatus = m_dm.GetCellData(nRow, 1);
	dlg.m_strTITLE_DESC = m_dm.GetCellData(nRow, 5);

	if (dlg.DoModal() != IDOK) return;

	CString strQuery;
	m_dm.StartFrame();
	strQuery.Format(_T("UPDATE ESL_SMARTID_MENU_TBL SET WORKING_STATUS='%s', TITLE_DESC='%s' WHERE REC_KEY=%s;"), dlg.m_strWorkingStatus, dlg.m_strTITLE_DESC, strRecKey);
	m_dm.AddFrame(strQuery);

	INT ids = SENDFRAME(&m_dm);
	if (ids < 0) {
		AfxMessageBox(_T("수정작업중에 에러가 발생하였습니다."));
	} else {
		OnbREFRESH();
		AfxMessageBox(_T("수정되었습니다."));
	}


}
