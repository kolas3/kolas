// AccessControlSelectionMenuListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlSelectionMenuListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionMenuListDlg dialog


CAccessControlSelectionMenuListDlg::CAccessControlSelectionMenuListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSelectionMenuListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSelectionMenuListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strTOPMENUALIAS = _T("");
	m_cUSERFLAG = 'U';

	// KOL.UDF.022
	m_pGrid = NULL;
	m_strALIAS = _T("");
	m_strCAPTION = _T("");
}


VOID CAccessControlSelectionMenuListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSelectionMenuListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSelectionMenuListDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSelectionMenuListDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionMenuListDlg message handlers

BOOL CAccessControlSelectionMenuListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gLIST, this);

	ReadCfgFile.ReadCfgFile(_T(""));
	m_DM.SetDB(ReadCfgFile.m_sDBKIND);
	m_DM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	RefreshList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlSelectionMenuListDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

BEGIN_EVENTSINK_MAP(CAccessControlSelectionMenuListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAccessControlSelectionMenuListDlg)
	ON_EVENT(CAccessControlSelectionMenuListDlg, IDC_gLIST, -601 /* DblClick */, OnDblClickgLIST, VTS_NONE)
	ON_EVENT(CAccessControlSelectionMenuListDlg, IDC_gLIST, -605 /* MouseDown */, OnMouseDowngLIST, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlSelectionMenuListDlg::OnDblClickgLIST() 
{
	OnbOK();
}

VOID CAccessControlSelectionMenuListDlg::OnMouseDowngLIST(short Button, short Shift, long x, long y) 
{
	m_pGrid->SetReverse(m_pGrid->GetRow()-1);
}

INT CAccessControlSelectionMenuListDlg::RefreshList(VOID)
{
	INT ids;
	m_pGrid->Clear();

	CString strQuery;
	if (m_cUSERFLAG == 'S') {
		strQuery = _T("SELECT PK, ALIAS, CAPTION FROM ESL_MENUGROUP_TBL WHERE GROUPTYPE <> 'T' OR GROUPTYPE IS NULL ORDER BY ALIAS");
	} else {
		strQuery.Format(_T("SELECT PK, ALIAS, CAPTION FROM ESL_MENU_TBL WHERE FLAG='S' AND USER_ID='Administrator' AND GROUP_ALIAS='%s' ORDER BY IDX, ALIAS"), m_strTOPMENUALIAS);
	}
	ids = m_DM.RestructDataManager(strQuery);
	if (ids < 0) {
		AfxMessageBox(_T("데이타를 읽어오던중 에러가 발생하였습니다."));
		return -1;
	}

	INT nRows = m_DM.GetRowCount();
	if (nRows == 0) m_pGrid->SetRows(2);
		else m_pGrid->SetRows(nRows+1);
	m_pGrid->SetCols(0, 3);
	m_pGrid->SetColWidth(0, 0, 30*15);
	m_pGrid->SetColumnInfo(0, _T("Alias"), 180, 1);
	m_pGrid->SetColumnInfo(1, _T("Caption"), 180, 1);

	CString str;
	for (INT i = 0; i < nRows; i++) {
		str.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(i+1, 0, str);
		m_pGrid->SetTextMatrix(i+1, 1, m_DM.GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, m_DM.GetCellData(i, 2));
	}
	return 0;
}

VOID CAccessControlSelectionMenuListDlg::OnbOK() 
{
	m_strALIAS = _T("");
	m_strCAPTION = _T("");
	if (m_DM.GetRowCount() == 0) {
		CDialog::OnCancel();
		return;
	}
	INT nRow = m_pGrid->GetRow() -1;
	m_strALIAS = m_DM.GetCellData(nRow, 1);
	m_strALIAS.TrimLeft();			m_strALIAS.TrimRight();
	m_strCAPTION = m_DM.GetCellData(nRow, 2);
	m_strCAPTION.TrimLeft();		m_strCAPTION.TrimRight();
	CDialog::OnOK();
}

VOID CAccessControlSelectionMenuListDlg::OnbCANCEL() 
{
	CDialog::OnCancel();
}
