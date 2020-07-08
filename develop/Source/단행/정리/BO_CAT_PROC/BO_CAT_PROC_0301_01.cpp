#include "stdafx.h"
#include "BO_CAT_PROC_0301_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_01 dialog


CBO_CAT_PROC_0301_01::CBO_CAT_PROC_0301_01(CESL_Mgr* pParent, CString sRecKeyArg /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	sRecKey = sRecKeyArg;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0301_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0301_01::~CBO_CAT_PROC_0301_01()
{
}

VOID CBO_CAT_PROC_0301_01::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0301_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0301_01, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0301_01)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_01 message handlers

BOOL CBO_CAT_PROC_0301_01::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0301_01::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0301_01")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0301_01"));

	CString strWhere = _T("DUP_SPECIES_KEY = '") + sRecKey + _T("'");
	if (pDM->RefreshDataManager(strWhere) < 0)	return FALSE;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	INT iRowCount = pDM->GetRowCount();

	for ( INT i = 1 ; i <= iRowCount ; i ++ ) {
		CString sDupYn;
		pDM->GetCellData( _T("BS_복본여부"), i-1, sDupYn );
		if ( sDupYn == _T("V") ) { 
			pDM->SetCellData( _T("BS_복본여부"), _T("복본"), i-1 );
		}
		else {
			pDM->SetCellData( _T("BS_복본여부"), _T("원본"), i-1 );
		}
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0301_01"));
	pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

HBRUSH CBO_CAT_PROC_0301_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
