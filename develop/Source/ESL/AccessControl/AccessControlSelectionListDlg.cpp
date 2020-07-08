// AccessControlSelectionListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlSelectionListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionListDlg dialog


CAccessControlSelectionListDlg::CAccessControlSelectionListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlSelectionListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlSelectionListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pB_OK = new CNewButton;
	pB_CANCEL = new CNewButton;

	MODE = 0;
	m_pDM = NULL;
	m_pGrid = new CESL_Grid(NULL);
	nSelectedRow = -1;
}


VOID CAccessControlSelectionListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlSelectionListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlSelectionListDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlSelectionListDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlSelectionListDlg message handlers

BOOL CAccessControlSelectionListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Bitmap Button을 설정하자.
	pB_CANCEL->SubclassDlgItem(IDC_bCANCEL, this);
	pB_CANCEL->LoadBitmap(IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);
	pB_OK->SubclassDlgItem(IDC_bOK, this);
	pB_OK->LoadBitmap(IDB_OK_U, IDB_OK_D, IDB_OK_F);

	// Grid 를 CESL_GRID로 연결하자
	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);

	if (MODE == 1) {		// 메뉴 선택 다이알 로그
		InitMenuSelectGrid();
	}
	if (MODE == 2) {		// 툴바 선택 다이알 로그
		InitMenuSelectGrid();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAccessControlSelectionListDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

VOID CAccessControlSelectionListDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	if (pB_OK) {
		delete pB_OK;
		pB_OK = NULL;
	}
	if (pB_CANCEL) {
		delete pB_CANCEL;
		pB_CANCEL = NULL;
	}
	
}

INT CAccessControlSelectionListDlg::InitMenuSelectGrid(VOID)
{
	if (!m_pDM) return -1;
	if (m_pDM->GetRowCount() == 0) return -1;

	m_pGrid->SetFixedCols(1);
	m_pGrid->SetFixedRows(1);
	m_pGrid->SetRows(2);

	m_pGrid->SetCols(0,4);
	m_pGrid->SetColumnInfo(0-1, _T("")			, 30		,		1);
	m_pGrid->SetColumnInfo(1-1, _T("ALIAS")		, 100		,		1);
	m_pGrid->SetColumnInfo(2-1, _T("Idx")		, 30		,		7);
	m_pGrid->SetColumnInfo(3-1, _T("Caption")	, 120		,		1);

	m_pGrid->SetRows(m_pDM->GetRowCount() + 1);
	INT nRow = m_pDM->GetRowCount();
	CString msg;
	for (INT i = 0 ; i < nRow; i++) {
		msg.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(i+1, 0, msg);
		m_pGrid->SetTextMatrix(i+1, 1, m_pDM->GetCellData(i, 0));
		m_pGrid->SetTextMatrix(i+1, 2, m_pDM->GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 3, m_pDM->GetCellData(i, 2));
	}
	return 0;
}


BEGIN_EVENTSINK_MAP(CAccessControlSelectionListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAccessControlSelectionListDlg)
	ON_EVENT(CAccessControlSelectionListDlg, IDC_MSHFLEXGRID1, -605 /* MouseDown */, OnMouseDownMshflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlSelectionListDlg::OnMouseDownMshflexgrid1(short Button, short Shift, long x, long y) 
{
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_pGrid->SetReverse(row-1);
	}	
}

VOID CAccessControlSelectionListDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

VOID CAccessControlSelectionListDlg::OnbOK() 
{
	nSelectedRow = m_pGrid->GetRow() - 1;
	CDialog::OnOK();
}
