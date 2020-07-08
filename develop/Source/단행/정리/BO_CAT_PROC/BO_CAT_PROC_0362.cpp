// BO_CAT_PROC_0362.cpp : implementation file
//

#include "stdafx.h"

#include "BO_CAT_PROC_0362.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0362 dialog


CBO_CAT_PROC_0362::CBO_CAT_PROC_0362(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0362)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0362::~CBO_CAT_PROC_0362()
{
}

VOID CBO_CAT_PROC_0362::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0362)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0362, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0362)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_End, OnEnd)
	ON_BN_CLICKED(IDC_bCatCssFIND, OnbCatCssFIND)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0362 message handlers

BOOL CBO_CAT_PROC_0362::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0362::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0361")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0362::OnEnd() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();	
	
}

VOID CBO_CAT_PROC_0362::OnbCatCssFIND() 
{
	// TODO: Add your control notification handler code here
	CString sGubun;
	GetControlData(_T("CM_BO_CAT_PROC_0361"), _T("구분"), sGubun);

	if ( sGubun == _T("KDC") ) {

	}


	CString H_BUF;
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0361"));

	CString sSearch;
	GetControlData(_T("CM_BO_CAT_PROC_0361"), _T("검색어"), sSearch);

	if ( sSearch.GetLength() < 3 ) {
		AfxMessageBox( _T("분류기호를 3자리 이상 넣어 주십시요 !") );
		return;
	}

	CString sWhere = _T("CLASS_VAL = '52'");

	if ( sSearch != _T("") ){
		sWhere.Format( _T("CLASS_VAL = '%s'") , sSearch );
	}
	if (pDM->RefreshDataManager(sWhere) < 0)	return;

	INT count = pDM->GetRowCount();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0361"));
	pCM->AllControlDisplay();	
}

BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0362, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0362)
	ON_EVENT(CBO_CAT_PROC_0362, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_PROC_0362::OnClickMshflexgrid() 
{
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0361"));
	pCM->FindControl(_T("분류기호참조_그리드"));
	
	CESL_Grid * pGrid = NULL;
	pGrid = (CESL_Grid*)pCM->FindControl(_T("분류기호참조_그리드"));

	INT nCursorRow = pGrid->GetRow();
	CString strData;
	strData = pGrid->GetTextMatrix( nCursorRow, 1 );
	if (strData.GetLength() == 0) {
		
	} 
	else {
		strData = _T(" ") + pGrid->GetTextMatrix( nCursorRow, 2 );
		sResult = sResult + strData;
		SetControlData(_T("CM_BO_CAT_PROC_0361"), _T("결과"), _T("056 ") + sResult);
	}
	
}

HBRUSH CBO_CAT_PROC_0362::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
