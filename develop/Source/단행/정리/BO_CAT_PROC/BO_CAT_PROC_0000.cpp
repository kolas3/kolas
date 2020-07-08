// BO_CAT_PROC_0000.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0000.h"
#include "BO_CAT_PROC_0300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0000 dialog


CBO_CAT_PROC_0000::CBO_CAT_PROC_0000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDuplicateSearchMgr = NULL;
}

CBO_CAT_PROC_0000::~CBO_CAT_PROC_0000()
{
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}
}

VOID CBO_CAT_PROC_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0000)
	ON_WM_SIZE()
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDMRefresh)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0000 message handlers

BOOL CBO_CAT_PROC_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0000")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	pGrid_MainGrid_All->SetProtectSelectColumn(TRUE);
	if ( pGrid_MainGrid_All) {				
		pGrid_MainGrid_All->InitPopupMenuItem();
		pGrid_MainGrid_All->AddPopupMenuItem((CView*)pMain, this->VIEW_STYLE, &RemoveListArray);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0000::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx+2, cy);
	

EFS_END
}

BOOL CBO_CAT_PROC_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0000, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0000)
	ON_EVENT(CBO_CAT_PROC_0000, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	ON_EVENT(CBO_CAT_PROC_0000, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_PROC_0000::OnDblClickMshflexgrid() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("ProcGrid"));
	INT iRow = pGrid_sub->GetRow();
	INT iMouseRow = pGrid_sub->GetMouseRow();
	if ( iMouseRow == 0 ) {
		return;
	}
	INT iRowCount = pGrid_sub->GetRows();
	for ( INT i = 1 ; i < iRowCount ; i ++ ) {
		pGrid_sub->SetTextMatrix( i, 1, _T("") );
	}

	pGrid_sub->SetTextMatrix( iRow, 1, _T("V") );


	CBO_CAT_PROC_0300 dlg(this);
	dlg.SetAlias( _T("CM_BO_CAT_PROC_0000"), _T("ProcGrid") );
	dlg.DoModal();

EFS_END
}
VOID CBO_CAT_PROC_0000::ReSearch()
{
EFS_BEGIN

	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_DataMgr *pWhere;
	pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
	if ( pWhere == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
		return;
	}
	CString sWhere;
	pWhere->GetCellData( _T("조건절"), 0, sWhere );

	if ( sWhere == _T("") ) {

		return;
	}

	if (pDM_PROC_0000->RefreshDataManager(sWhere) < 0)	return;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}
VOID CBO_CAT_PROC_0000::OnClickMshflexgrid() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("ProcGrid"));
	INT iRow = pGrid_sub->GetRow();
	INT iMouseRow = pGrid_sub->GetMouseRow();
	if ( iMouseRow == 0 ) {
		return;
	}

EFS_END
}

VOID CBO_CAT_PROC_0000::SearchDuplicateData()
{
EFS_BEGIN

	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}
	CString MainDMAlias = _T("DM_BO_CAT_PROC_0000");
	CString GridAlias = _T("ProcGrid");
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("정리복본조사"), MainDMAlias, _T("CM_BO_CAT_PROC_0000"), GridAlias);	

EFS_END
}

VOID CBO_CAT_PROC_0000::OnDMRefresh(WPARAM w, LPARAM l)
{
EFS_BEGIN

	ReSearch();

EFS_END
}

VOID CBO_CAT_PROC_0000::OnShowSearchResult(WPARAM w, LPARAM l)
{
EFS_BEGIN

	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();

EFS_END
}

HBRUSH CBO_CAT_PROC_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
