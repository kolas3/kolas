// DBSyncDetail.cpp : implementation file
//

#include "stdafx.h"
#include "DBSync.h"
#include "DBSyncDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBSyncDetail dialog


CDBSyncDetail::CDBSyncDetail(CDBSyncDlg* pParent /*=NULL*/)
	: CDialog(CDBSyncDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBSyncDetail)
	m_table = _T("");
	m_where = _T("");
	m_src_cnt = _T("");
	m_dst_cnt = _T("");
	m_field = _T("");
	m_compare = _T("");
	//}}AFX_DATA_INIT
    m_pParent = pParent;
    m_pGridSrc = NULL;
    m_pGridDst = NULL;

	// KOL.UDF.022
	m_nType = 1;
	m_nIndex = 0;
	m_pParent = NULL;
	m_bReadOnly = FALSE;
}

CDBSyncDetail::~CDBSyncDetail()
{
    InitMemory();
}

VOID CDBSyncDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBSyncDetail)
	DDX_Text(pDX, IDC_eTABLE, m_table);
	DDX_Text(pDX, IDC_eWHERE, m_where);
	DDX_Text(pDX, IDC_eSRC_CNT, m_src_cnt);
	DDX_Text(pDX, IDC_eDST_CNT, m_dst_cnt);
	DDX_Text(pDX, IDC_eFIELD, m_field);
	DDX_Text(pDX, IDC_eCOMPARE, m_compare);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBSyncDetail, CDialog)
	//{{AFX_MSG_MAP(CDBSyncDetail)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CDBSyncDetail::InitMemory()
{
	if ( m_pGridSrc != NULL ) {
		delete m_pGridSrc;
		m_pGridSrc = NULL;
	}
	if ( m_pGridDst != NULL ) {
		delete m_pGridDst;
		m_pGridDst = NULL;
	}
}

INT CDBSyncDetail::LoadSchema(CString table)
{
    INT nError;
    nError = m_pParent->RunSchemaGetSrc(table);
    if ( nError < 0 ) return -1;
    else if ( nError > 0 ) return 1;

    nError = m_pParent->RunSchemaGetDst(table);
    if ( nError < 0 ) return -1;
    else if ( nError > 0 ) return 1;

    m_compare = m_pParent->RunSchemaCompare();
    m_pParent->m_pGrid->SetAt(m_nIndex, 2, m_compare);

    return 0;
} 

INT CDBSyncDetail::LoadData(CString table, CString field, CString where)
{
    INT nError;
    nError = m_pParent->RunDataGetSrc(table, field, where);
    if ( nError < 0 ) return -1;
    else if ( nError > 0 ) return 1;

    nError = m_pParent->RunDataGetDst(table, field, where);
    if ( nError < 0 ) return -1;
    else if ( nError > 0 ) return 1;

    m_compare = m_pParent->RunDataCompare();
    m_src_cnt.Format(_T("%d"), m_pParent->m_dmSrc.GetRowCount());
    m_dst_cnt.Format(_T("%d"), m_pParent->m_dmDst.GetRowCount());
    m_pParent->m_pGrid->SetAt(m_nIndex, 4, m_compare);
    m_pParent->m_pGrid->SetAt(m_nIndex, 5, m_src_cnt);
    m_pParent->m_pGrid->SetAt(m_nIndex, 6, m_dst_cnt);

    return 0;
} 

INT CDBSyncDetail::DisplaySchema()
{
	m_pGridSrc->RemoveAll();
	m_pGridSrc->InitColumn(5);
	m_pGridSrc->SetColumnInfo(0, _T("필드명"), 100, 1);
	m_pGridSrc->SetColumnInfo(1, _T("TYPE"), 100, 1);
	m_pGridSrc->SetColumnInfo(2, _T("길이"), 50, 1);
	m_pGridSrc->SetColumnInfo(3, _T("NULL"), 50, 1);
	m_pGridSrc->SetColumnInfo(4, _T("기본값"), 50, 1);

	m_pGridSrc->m_bGridEdit = false;
	m_pGridSrc->m_bMenu = false;
	m_pGridSrc->m_bCheck = false;

	m_pGridDst->RemoveAll();
	m_pGridDst->InitColumn(5);
	m_pGridDst->SetColumnInfo(0, _T("필드명"), 100, 1);
	m_pGridDst->SetColumnInfo(1, _T("TYPE"), 100, 1);
	m_pGridDst->SetColumnInfo(2, _T("길이"), 50, 1);
	m_pGridDst->SetColumnInfo(3, _T("NULL"), 50, 1);
	m_pGridDst->SetColumnInfo(4, _T("기본값"), 50, 1);

	m_pGridDst->m_bGridEdit = false;
	m_pGridDst->m_bMenu = false;
	m_pGridDst->m_bCheck = false;

    INT nSrcRow;
    nSrcRow = m_pParent->m_dmSrc.GetRowCount();

    INT i;
    for ( i = 0 ; i < nSrcRow ; i++ ) {
        m_pGridSrc->SetAt(i, 0, m_pParent->m_dmSrc.GetCellData(i, 0));
        m_pGridSrc->SetAt(i, 1, m_pParent->m_dmSrc.GetCellData(i, 1));
        m_pGridSrc->SetAt(i, 2, m_pParent->m_dmSrc.GetCellData(i, 2));
        m_pGridSrc->SetAt(i, 3, m_pParent->m_dmSrc.GetCellData(i, 3));
        m_pGridSrc->SetAt(i, 4, m_pParent->m_dmSrc.GetCellData(i, 4));
    }
    m_pGridSrc->DisplayNumber();

    INT nDstRow;
    nDstRow = m_pParent->m_dmDst.GetRowCount();

    for ( i = 0 ; i < nDstRow ; i++ ) {
        m_pGridDst->SetAt(i, 0, m_pParent->m_dmDst.GetCellData(i, 0));
        m_pGridDst->SetAt(i, 1, m_pParent->m_dmDst.GetCellData(i, 1));
        m_pGridDst->SetAt(i, 2, m_pParent->m_dmDst.GetCellData(i, 2));
        m_pGridDst->SetAt(i, 3, m_pParent->m_dmDst.GetCellData(i, 3));
        m_pGridDst->SetAt(i, 4, m_pParent->m_dmDst.GetCellData(i, 4));
    }
    m_pGridDst->DisplayNumber();

    return 0;
}

INT CDBSyncDetail::DisplayData()
{
	m_pGridSrc->RemoveAll();
	m_pGridSrc->InitColumn(1);
	m_pGridSrc->SetColumnInfo(0, _T("데이타"), 350, 1);

	m_pGridSrc->m_bGridEdit = false;
	m_pGridSrc->m_bMenu = false;
	m_pGridSrc->m_bCheck = false;

	m_pGridDst->RemoveAll();
	m_pGridDst->InitColumn(1);
	m_pGridDst->SetColumnInfo(0, _T("데이타"), 350, 1);

	m_pGridDst->m_bGridEdit = false;
	m_pGridDst->m_bMenu = false;
	m_pGridDst->m_bCheck = false;

    INT nSrcRow;
    nSrcRow = m_pParent->m_dmSrc.GetRowCount();

    INT i;
    for ( i = 0 ; i < nSrcRow ; i++ ) {
        m_pGridSrc->SetAt(i, 0, m_pParent->m_dmSrc.GetCellData(i, 0));
    }
    m_pGridSrc->DisplayNumber();

    INT nDstRow;
    nDstRow = m_pParent->m_dmDst.GetRowCount();

    for ( i = 0 ; i < nDstRow ; i++ ) {
        m_pGridDst->SetAt(i, 0, m_pParent->m_dmDst.GetCellData(i, 0));
    }
    m_pGridDst->DisplayNumber();

    return 0;
}

VOID CDBSyncDetail::MoveAndDisplay(INT dir)
{
    switch(dir) {
    case 0:
        m_nIndex = m_pParent->m_pGrid->GetRowIdx();
        break;
    case -1:
        if ( m_nIndex == 0 ) {
            AfxMessageBox(_T("제일 처음 데이터 입니다."), MB_ICONEXCLAMATION);
            return;
        }
        m_nIndex--;
        break;
    case 1:
        if ( m_nIndex == m_pParent->m_pGrid->GetCount()-1) {
            AfxMessageBox(_T("제일 마지막 데이터 입니다."), MB_ICONEXCLAMATION);
            return;
        }
        m_nIndex++;
        break;
    default:
        return;
        break;
    }

    if ( m_nIndex == 0 ) {
        GetDlgItem(IDC_bPREV)->EnableWindow(false);
    } else {
        GetDlgItem(IDC_bPREV)->EnableWindow(true);
    }

    if ( m_nIndex == m_pParent->m_pGrid->GetCount()-1) {
        GetDlgItem(IDC_bNEXT)->EnableWindow(false);
    } else {
        GetDlgItem(IDC_bNEXT)->EnableWindow(true);
    }

    if ( m_nType == 1 ) {
        GetDlgItem(IDC_STATIC1)->EnableWindow(false);
        GetDlgItem(IDC_STATIC2)->EnableWindow(false);
        GetDlgItem(IDC_STATIC3)->EnableWindow(false);
        GetDlgItem(IDC_STATIC4)->EnableWindow(false);
        GetDlgItem(IDC_eFIELD)->EnableWindow(false);
        GetDlgItem(IDC_eWHERE)->EnableWindow(false);
        GetDlgItem(IDC_eSRC_CNT)->EnableWindow(false);
        GetDlgItem(IDC_eDST_CNT)->EnableWindow(false);

        m_table = m_pParent->m_pGrid->GetAt(m_nIndex, 1);

        INT nError;
        nError = LoadSchema(m_table);
        if ( nError > 0 ) {
            AfxMessageBox(_T("Schema 정보를 DB에서 가져오는데 실패 했습니다."), MB_ICONSTOP);
            return;
        } else if ( nError > 0 ) {
            return;
        }

        nError = DisplaySchema();
        if ( nError > 0 ) {
            AfxMessageBox(_T("가져온 Schema 정보를 Display할 수 없습니다."), MB_ICONSTOP);
            return;
        } else if ( nError > 0 ) {
            return;
        }

    } else if ( m_nType == 2 ) {
        GetDlgItem(IDC_STATIC1)->EnableWindow(true);
        GetDlgItem(IDC_STATIC2)->EnableWindow(true);
        GetDlgItem(IDC_STATIC3)->EnableWindow(true);
        GetDlgItem(IDC_STATIC4)->EnableWindow(true);
        GetDlgItem(IDC_eFIELD)->EnableWindow(true);
        GetDlgItem(IDC_eWHERE)->EnableWindow(true);
        GetDlgItem(IDC_eSRC_CNT)->EnableWindow(true);
        GetDlgItem(IDC_eDST_CNT)->EnableWindow(true);

        m_table = m_pParent->m_pGrid->GetAt(m_nIndex, 1);
        m_field = m_pParent->m_pGrid->GetAt(m_nIndex, 2);
        m_where = m_pParent->m_pGrid->GetAt(m_nIndex, 3);

        INT nError;
        nError = LoadData(m_table, m_field, m_where);
        if ( nError > 0 ) {
            AfxMessageBox(_T("Data 정보를 DB에서 가져오는데 실패 했습니다."), MB_ICONSTOP);
            return;
        } else if ( nError > 0 ) {
            return;
        }

        nError = DisplayData();
        if ( nError > 0 ) {
            AfxMessageBox(_T("가져온 Data 정보를 Display할 수 없습니다."), MB_ICONSTOP);
            return;
        } else if ( nError > 0 ) {
            return;
        }


    } else {
        return;
    }

    m_pParent->m_pGrid->SetReverse(m_nIndex);

    CString title;
    title.Format(_T("상세보기 - %d"), m_nIndex+1);
    SetWindowText(title);

    UpdateData(false);

}

/////////////////////////////////////////////////////////////////////////////
// CDBSyncDetail message handlers

BOOL CDBSyncDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	VERIFY(m_btnPrev.AutoLoad(IDC_bPREV, this));
    VERIFY(m_btnNext.AutoLoad(IDC_bNEXT, this));
	VERIFY(m_btnOk.AutoLoad(IDOK, this));
	VERIFY(m_btnCancel.AutoLoad(IDCANCEL, this));

    m_pGridSrc = new CESL_Grid();
	m_pGridSrc->SubclassDlgItem(IDC_gDATA1, this);

    m_pGridDst = new CESL_Grid();
    m_pGridDst->SubclassDlgItem(IDC_gDATA2, this);

    MoveAndDisplay(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDBSyncDetail::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	MoveAndDisplay(-1);
}

VOID CDBSyncDetail::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	MoveAndDisplay(1);
}
