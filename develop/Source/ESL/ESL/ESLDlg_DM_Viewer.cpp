// ESLDlg_DM_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_DM_Viewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_DM_Viewer dialog


CESLDlg_DM_Viewer::CESLDlg_DM_Viewer(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_DM_Viewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_DM_Viewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nDMs = NULL;
	m_nRowIdx = 0;
	m_nDMCursor = -1;
	m_pGrid = new CESL_Grid(NULL);
	pMgr = NULL;

	for (INT i = 0; i < MAX_DM; i++) {
		m_pDMList[i] = NULL;
	}
}


VOID CESLDlg_DM_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_DM_Viewer)
	DDX_Control(pDX, IDC_COM_cDMLIST, m_cDMList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_DM_Viewer, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_DM_Viewer)
	ON_BN_CLICKED(IDC_COM_bVIEW, OnCOMbVIEW)
	ON_BN_CLICKED(IDC_COM_bPREV, OnCOMbPREV)
	ON_BN_CLICKED(IDC_COM_bNEXT, OnCOMbNEXT)
	ON_BN_CLICKED(IDC_COM_bCLOSE, OnCOMbCLOSE)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COM_cDMLIST, OnSelchangeCOMcDMLIST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_DM_Viewer message handlers

BOOL CESLDlg_DM_Viewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (pMgr == NULL) return FALSE;
	if (m_pGrid == NULL) return FALSE;
	InitDMList();
	if (m_nDMs == 0) return FALSE;
	m_cDMList.ResetContent();
	for (INT i = 0; i < m_nDMs; i++) {
		m_cDMList.AddString(m_pDMList[i]->GETDMAlias());
		m_cDMList.SetItemData(i, (DWORD)i);
	}
	m_cDMList.SetCurSel(0);
	m_pGrid->SubclassDlgItem(IDC_COM_gDATAVIEW, this);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CESLDlg_DM_Viewer::OnCOMbVIEW() 
{
	UpdateData(TRUE);
	m_nDMCursor = m_cDMList.GetItemData(m_cDMList.GetCurSel());
	CString strDMAlias = m_pDMList[m_nDMCursor]->GETDMAlias();

	/*
	if (m_pDMList[m_nDMCursor]->GetRowCount() == 0) {
		AfxMessageBox(_T("선택하신 DM에는 데이타가 한개도 없습니다."));
		return;
	}
	*/
	m_nRowIdx = 0;
	ShowData(m_nRowIdx);
}

INT CESLDlg_DM_Viewer::ShowData(INT nRowIdx)
{
	InitGrid();
	if (m_pDMList[m_nDMCursor] == NULL) return -1;

	INT nCols = m_pDMList[m_nDMCursor]->RefList.GetCount();
	if (nCols == 0) return -1;
	//SORT(0);
	m_pGrid->SetRows(nCols+1);

	INT ids;
	CString strTmp;
	CString field_alias, field_name, field_type, data_type, strData;
	for (INT i = 0; i < nCols; i++) {
		strTmp.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(i+1, 0, strTmp);

		strData = _T("");
		ids = m_pDMList[m_nDMCursor]->FindColumnFieldName(i, field_alias, field_name, field_type, data_type, nRowIdx, strData);
		m_pGrid->SetTextMatrix(i+1, 1, field_alias);
		m_pGrid->SetTextMatrix(i+1, 2, field_name);
		m_pGrid->SetTextMatrix(i+1, 3, field_type);
		m_pGrid->SetTextMatrix(i+1, 4, strData);
	}

	INT nRows = m_pDMList[m_nDMCursor]->GetRowCount();
	strTmp.Format(_T("%d / %d"), nRowIdx+1, nRows);
	GetDlgItem(IDC_COM_sINDEX)->SetWindowText(strTmp);
	return 0;
}

VOID CESLDlg_DM_Viewer::OnCOMbPREV() 
{
	INT nRows = m_pDMList[m_nDMCursor]->GetRowCount();
	if (m_nRowIdx == 0) {
		AfxMessageBox(_T("여기가 처음입니다."));
		return;
	}
	m_nRowIdx--;
	ShowData(m_nRowIdx);
}

VOID CESLDlg_DM_Viewer::OnCOMbNEXT() 
{
	INT nRows = m_pDMList[m_nDMCursor]->GetRowCount();
	if (m_nRowIdx >= nRows-1) {
		AfxMessageBox(_T("여기가 마지막입니다."));
		return;
	}
	m_nRowIdx++;
	ShowData(m_nRowIdx);
}

VOID CESLDlg_DM_Viewer::OnCOMbCLOSE() 
{
	CDialog::OnOK();
}

INT CESLDlg_DM_Viewer::InitGrid(VOID)
{
	m_pGrid->SetCols(0, 5);
	m_pGrid->SetRows(2);

	SetColumnInfo(0, _T(""), 30, 4);
	SetColumnInfo(1, _T("Field Alias"), 100, 1);
	SetColumnInfo(2, _T("Field Name"), 100, 1);
	SetColumnInfo(3, _T("Type"), 60, 4);
	SetColumnInfo(4, _T("Field Data"), 250, 1);
	
	return 0;
}

INT CESLDlg_DM_Viewer::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	m_pGrid->SetColAlignmentFixed(col, 4);
	m_pGrid->SetTextMatrix(0, col, str);
	m_pGrid->SetColWidth(col, 0, width*15);
	m_pGrid->SetColAlignment(col, alignment);

	return 0;
}

VOID CESLDlg_DM_Viewer::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

BOOL CESLDlg_DM_Viewer::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CESLDlg_DM_Viewer::InitDMList(VOID)
{
	if (pMgr == NULL) return -1;
	POSITION pos;
	INT i, nCount;
	CESL_Mgr *pTmpMgr = pMgr;
	while(TRUE) {
		nCount = 0;
		nCount = pTmpMgr->DMList.GetCount();
		pos = pTmpMgr->DMList.GetHeadPosition();	
		for (i = 0; i < nCount; i++) {
			m_pDMList[m_nDMs] = (CESL_DataMgr*)pTmpMgr->DMList.GetNext(pos);	
			m_nDMs++;
		}
		if (pTmpMgr->pParentMgr == NULL) break;
		pTmpMgr = pTmpMgr->pParentMgr;
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CESLDlg_DM_Viewer, CDialog)
    //{{AFX_EVENTSINK_MAP(CESLDlg_DM_Viewer)
	ON_EVENT(CESLDlg_DM_Viewer, IDC_COM_gDATAVIEW, -600 /* Click */, OnClickCOMgDATAVIEW, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CESLDlg_DM_Viewer::OnClickCOMgDATAVIEW() 
{
	INT nRow = m_pGrid->GetRow();
	m_pGrid->SetReverse(nRow-1);
}

void CESLDlg_DM_Viewer::OnSelchangeCOMcDMLIST() 
{
	OnCOMbVIEW();		
}
