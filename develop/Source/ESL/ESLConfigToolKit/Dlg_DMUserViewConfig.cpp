// Dlg_DMUserViewConfig.cpp : implementation file
//

#include "stdafx.h"
#include "ESLConfigToolKit.h"
#include "Dlg_DMUserViewConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Dlg_DMUSERViewConfig_Edit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlg_DMUserViewConfig dialog


CDlg_DMUserViewConfig::CDlg_DMUserViewConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_DMUserViewConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_DMUserViewConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = new CESL_DataMgr;
	m_pGrid = new CESL_Grid(NULL);

	m_sSEQ = _T("ESL_SEQ");
}


VOID CDlg_DMUserViewConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_DMUserViewConfig)
	DDX_Control(pDX, IDC_cDMLIST, m_cDMLIST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_DMUserViewConfig, CDialog)
	//{{AFX_MSG_MAP(CDlg_DMUserViewConfig)
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_CBN_SELCHANGE(IDC_cDMLIST, OnSelchangecDMLIST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_DMUserViewConfig message handlers

VOID CDlg_DMUserViewConfig::OnbREFRESH() 
{
	if (!m_pGrid) return;

	CString strDB_MGR_ALIAS;
	UpdateData(TRUE);
	m_cDMLIST.GetWindowText(strDB_MGR_ALIAS);
	strDB_MGR_ALIAS.TrimLeft();			strDB_MGR_ALIAS.TrimRight();
	if (strDB_MGR_ALIAS.GetLength() == 0) return;
	
	m_pGrid->SetCols(0, 9);
	m_pGrid->SetColWidth(0, 0, 30*15);
	m_pGrid->SetColumnInfo(1-1, _T("View"), 40, 4);
	m_pGrid->SetColumnInfo(2-1, _T("USER ALIAS"), 150, 1);
	m_pGrid->SetColumnInfo(3-1, _T("USER DESC"), 80, 1);
	m_pGrid->SetColumnInfo(4-1, _T("ALIAS"), 150, 1);
	m_pGrid->SetColumnInfo(5-1, _T("이름"), 100, 1);
	m_pGrid->SetColumnInfo(6-1, _T("타입"), 60, 4);
	m_pGrid->SetColumnInfo(7-1, _T("초기값"), 60, 1);
	m_pGrid->SetColumnInfo(8-1, _T("데이터형태"), 80, 4);

	CString sQuery;
	sQuery.Format(_T("SELECT S.PK, S.USER_VIEW, S.USER_ALIAS, S.USER_DESCRIP, S.FIELD_ALIAS, S.FIELD_NAME, S.FIELD_TYPE, S.INIT_VALUE, S.DATA_TYPE FROM ESL_DB_MGR_TBL D, ESL_DB_MGR_SUB_TBL S WHERE D.PK=S.DB_MGR_PK AND D.DB_MGR_ALIAS='%s' ORDER BY S.USER_ALIAS, S.FIELD_ALIAS"), strDB_MGR_ALIAS); 
	m_pDM->RestructDataManager(sQuery);

	INT i, nRow;
	nRow = m_pDM->GetRowCount();
	if (nRow == 0) m_pGrid->SetRows(2);
		else m_pGrid->SetRows(nRow+1);
	CString strTmp;
	for (i = 0; i < nRow; i++) {
		strTmp.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(i+1, 0, strTmp);							// row idx
		m_pGrid->SetTextMatrix(i+1, 1, m_pDM->GetCellData(i, 1));			// User view
		m_pGrid->SetTextMatrix(i+1, 2, m_pDM->GetCellData(i, 2));			// User alias
		m_pGrid->SetTextMatrix(i+1, 3, m_pDM->GetCellData(i, 3));			// User descrip
		m_pGrid->SetTextMatrix(i+1, 4, m_pDM->GetCellData(i, 4));			// Field alias
		m_pGrid->SetTextMatrix(i+1, 5, m_pDM->GetCellData(i, 5));			// Field name
		m_pGrid->SetTextMatrix(i+1, 6, m_pDM->GetCellData(i, 6));			// Field type
		m_pGrid->SetTextMatrix(i+1, 7, m_pDM->GetCellData(i, 7));			// Init value
		m_pGrid->SetTextMatrix(i+1, 8, m_pDM->GetCellData(i, 8));			// row idx

	}

}

VOID CDlg_DMUserViewConfig::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	ReArrangeControls();
}

BOOL CDlg_DMUserViewConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pGrid->SubclassDlgItem(IDC_gLIST1, this);
	SetTimer(0, 50, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDlg_DMUserViewConfig::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		ReArrangeControls();
		ShowDMList();
	}
	CDialog::OnTimer(nIDEvent);
}

INT CDlg_DMUserViewConfig::ReArrangeControls(VOID)
{
	CWnd *pWnd;
	CRect rect;

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_cDMLIST);
	if (pWnd) {
		rect.top = 5;
		rect.left = 5;
		rect.bottom = rect.top + 20;
		rect.right = rect.right - (90+90+90+90);

		pWnd->MoveWindow(&rect);
	}

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_bINSERT);
	if (pWnd) {
		rect.top = 5;
		rect.left = rect.right - (90+90+90+80);
		rect.bottom = rect.top + 20;
		rect.right = rect.right - (90+90+90);

		pWnd->MoveWindow(&rect);
	}

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_bMODIFY);
	if (pWnd) {
		rect.top = 5;
		rect.left = rect.right - (90+90+80);
		rect.bottom = rect.top + 20;
		rect.right = rect.right - (90+90);

		pWnd->MoveWindow(&rect);
	}

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_bDELETE);
	if (pWnd) {
		rect.top = 5;
		rect.left = rect.right - (90+80);
		rect.bottom = rect.top + 20;
		rect.right = rect.right - 90;

		pWnd->MoveWindow(&rect);
	}

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_bREFRESH);
	if (pWnd) {
		rect.top = 5;
		rect.left = rect.right - 85;
		rect.bottom = rect.top + 20;
		rect.right = rect.right - 5;

		pWnd->MoveWindow(&rect);
	}

	GetClientRect(&rect);
	pWnd = GetDlgItem(IDC_gLIST1);
	if (pWnd) {
		rect.top = 30;
		rect.left = 5;
		rect.bottom = rect.bottom - 5;
		rect.right = rect.right - 5;

		pWnd->MoveWindow(&rect);
	}
	return 0;
}

VOID CDlg_DMUserViewConfig::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

INT CDlg_DMUserViewConfig::ShowDMList(VOID)
{
	CString sQuery = _T("SELECT PK, DB_MGR_ALIAS FROM ESL_DB_MGR_TBL ORDER BY DB_MGR_ALIAS");
	m_pDM->RestructDataManager(sQuery);

	m_cDMLIST.ResetContent();
	INT i, nRow;
	nRow = m_pDM->GetRowCount();
	for (i = 0; i < nRow; i++) {
		m_cDMLIST.AddString(m_pDM->GetCellData(i, 1));
		m_cDMLIST.SetItemData(i, _ttoi(m_pDM->GetCellData(i, 0)));
	}
	m_cDMLIST.SetCurSel(0);
	UpdateData(FALSE);
	return 0;	
}

BEGIN_EVENTSINK_MAP(CDlg_DMUserViewConfig, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlg_DMUserViewConfig)
	ON_EVENT(CDlg_DMUserViewConfig, IDC_gLIST1, -600 /* Click */, OnClickgLIST1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDlg_DMUserViewConfig::OnClickgLIST1() 
{
	if (m_pGrid) {
		if (m_pGrid->GetRow() != 0) {
			m_pGrid->SetReverse(m_pGrid->GetRow()-1);
		}
	}
}

VOID CDlg_DMUserViewConfig::OnbINSERT() 
{
	if (AfxMessageBox(_T("정말 입력하시겠습니까? 신중하게선택하십시오."), MB_YESNO) != IDYES) return;

	INT row = m_pGrid->GetRow()-1;
	INT ids;
	CString strPK = m_pDM->GetCellData(row, 0);

	CDlg_DMUSERViewConfig_Edit dlg;
	dlg.m_sUSERVIEW = _T("T");
	dlg.m_sTYPE = _T("str");
	if (dlg.DoModal() != IDOK) return;

	CString strDB_MGR_ALIAS, strDB_MGR_PK, sQuery;
	UpdateData(TRUE);
	m_cDMLIST.GetWindowText(strDB_MGR_ALIAS);
	strDB_MGR_ALIAS.TrimLeft();			strDB_MGR_ALIAS.TrimRight();
	sQuery.Format(_T("SELECT PK FROM ESL_DB_MGR_TBL WHERE DB_MGR_ALIAS='%s'"), strDB_MGR_ALIAS);
	m_pDM->GetOneValueQuery(sQuery, strDB_MGR_PK);
	strDB_MGR_PK.TrimLeft();		strDB_MGR_PK.TrimRight();
	if (strDB_MGR_PK.GetLength() == 0) {
		AfxMessageBox(_T("DM 정보를 찾을 수 없습니다."));
		return;
	}
	CString strSEQ;
	strSEQ.Format(_T("%s.NEXTVAL"), m_sSEQ);
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData(_T("PK"), _T("NUMERIC"), strSEQ);
	m_pDM->AddDBFieldData(_T("DB_MGR_PK"), _T("NUMERIC"), strDB_MGR_PK);
	m_pDM->AddDBFieldData(_T("USER_VIEW"), _T("STRING"), dlg.m_sUSERVIEW);
	m_pDM->AddDBFieldData(_T("USER_ALIAS"), _T("STRING"), dlg.m_sUSERALIAS);
	m_pDM->AddDBFieldData(_T("USER_DESCRIP"), _T("STRING"), dlg.m_sUSERDESC);
	m_pDM->AddDBFieldData(_T("FIELD_ALIAS"), _T("STRING"), dlg.m_sALIAS);
	m_pDM->AddDBFieldData(_T("FIELD_NAME"), _T("STRING"), dlg.m_sNAME);
	m_pDM->AddDBFieldData(_T("FIELD_TYPE"), _T("STRING"), dlg.m_sTYPE);
	m_pDM->AddDBFieldData(_T("INIT_VALUE"), _T("STRING"), dlg.m_sINIT);
	m_pDM->AddDBFieldData(_T("DATA_TYPE"), _T("STRING"), dlg.m_sUDF);
	m_pDM->MakeInsertFrame(_T("ESL_DB_MGR_SUB_TBL"));

	ids = m_pDM->SendFrame();
	if (ids == 0) {
		AfxMessageBox(_T("입력되었습니다."));
	} else {
		AfxMessageBox(_T("입력에 실패하였습니다."));
	}
	m_pDM->EndFrame();

	OnbREFRESH();
}

VOID CDlg_DMUserViewConfig::OnbMODIFY() 
{
	if (m_pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("데이타가 없습니다."));
		return;
	}
	//SELECT S.PK, S.USER_VIEW, S.USER_ALIAS, S.USER_DESCRIP, S.FIELD_ALIAS, S.FIELD_NAME, S.FIELD_TYPE, S.INIT_VALUE, S.DATA_TYPE FROM ESL_DB_MGR_TBL D, ESL_DB_MGR_SUB_TBL S
	INT row = m_pGrid->GetRow()-1;
	INT ids;
	CString strPK = m_pDM->GetCellData(row, 0);

	CDlg_DMUSERViewConfig_Edit dlg;
	dlg.m_sUSERVIEW = m_pDM->GetCellData(row, 1);
	dlg.m_sUSERALIAS = m_pDM->GetCellData(row, 2);
	dlg.m_sUSERDESC = m_pDM->GetCellData(row, 3);
	dlg.m_sALIAS = m_pDM->GetCellData(row, 4);
	dlg.m_sNAME = m_pDM->GetCellData(row, 5);
	dlg.m_sTYPE = m_pDM->GetCellData(row, 6);
	dlg.m_sINIT = m_pDM->GetCellData(row, 7);
	dlg.m_sUDF = m_pDM->GetCellData(row, 8);

	if (dlg.DoModal() != IDOK) return;

	CString sQuery, strResult;
	sQuery.Format(_T("SELECT COUNT(*) FROM ESL_DB_MGR_SUB_TBL WHERE PK='%s'"), strPK);
	ids = m_pDM->GetOneValueQuery(sQuery, strResult);
	strResult.TrimLeft();			strResult.TrimRight();
	if (strResult == _T("0")) {
		AfxMessageBox(_T("DM 이 다른 누군가에 의해서 수정되었습니다. [REFRESH]후 다시 작업하세요."));
		return;
	}
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData(_T("USER_VIEW"), _T("STRING"), dlg.m_sUSERVIEW);
	m_pDM->AddDBFieldData(_T("USER_ALIAS"), _T("STRING"), dlg.m_sUSERALIAS);
	m_pDM->AddDBFieldData(_T("USER_DESCRIP"), _T("STRING"), dlg.m_sUSERDESC);
	m_pDM->AddDBFieldData(_T("FIELD_ALIAS"), _T("STRING"), dlg.m_sALIAS);
	m_pDM->AddDBFieldData(_T("FIELD_NAME"), _T("STRING"), dlg.m_sNAME);
	m_pDM->AddDBFieldData(_T("FIELD_TYPE"), _T("STRING"), dlg.m_sTYPE);
	m_pDM->AddDBFieldData(_T("INIT_VALUE"), _T("STRING"), dlg.m_sINIT);
	m_pDM->AddDBFieldData(_T("DATA_TYPE"), _T("STRING"), dlg.m_sUDF);
	m_pDM->MakeUpdateFrame(_T("ESL_DB_MGR_SUB_TBL"), _T("PK"), _T("NUMERIC"), strPK);

	ids = m_pDM->SendFrame();
	if (ids == 0) {
		AfxMessageBox(_T("수정되었습니다."));
	} else {
		AfxMessageBox(_T("수정에 실패하였습니다."));
	}
	m_pDM->EndFrame();

	OnbREFRESH();
}

VOID CDlg_DMUserViewConfig::OnbDELETE() 
{
	if (AfxMessageBox(_T("정말 지우시겠습니까? 신중하게 선택하십시오."), MB_YESNO) != IDYES) return;

	INT row = m_pGrid->GetRow()-1;
	INT ids;
	CString strPK = m_pDM->GetCellData(row, 0);

	CString sQuery, strResult;
	sQuery.Format(_T("SELECT COUNT(*) FROM ESL_DB_MGR_SUB_TBL WHERE PK='%s'"), strPK);
	ids = m_pDM->GetOneValueQuery(sQuery, strResult);
	strResult.TrimLeft();			strResult.TrimRight();
	if (strResult == _T("0")) {
		AfxMessageBox(_T("DM 이 다른 누군가에 의해서 수정되었습니다. [REFRESH]후 다시 작업하세요."));
		return;
	}

	m_pDM->StartFrame();
	m_pDM->MakeDeleteFrame(_T("ESL_DB_MGR_SUB_TBL"), _T("PK"), _T("NUMERIC"), strPK);
	ids = m_pDM->SendFrame();
	if (ids == 0) {
		AfxMessageBox(_T("삭제되었습니다."));
	} else {
		AfxMessageBox(_T("삭제에 실패하였습니다."));
	}
	m_pDM->EndFrame();

	OnbREFRESH();	
}

VOID CDlg_DMUserViewConfig::OnSelchangecDMLIST() 
{
	OnbREFRESH();	
}
