// BO_CAT_PROC_0371.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0371.h"
//#include _T("BO_CAT_PROC_0373.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0371 dialog


CBO_CAT_PROC_0371::CBO_CAT_PROC_0371(CESL_Mgr* pParent, INT tmp /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_idx = tmp;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0371)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0371::~CBO_CAT_PROC_0371()
{
}

VOID CBO_CAT_PROC_0371::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0371)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0371, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0371)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0371 message handlers

BOOL CBO_CAT_PROC_0371::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0371::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0371")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	CString sTitle;
	sTitle = pDM->GetCellData( _T("IBS_본표제"), m_idx );

	SetControlData(_T("CM_BO_CAT_PROC_0371"), _T("IBS_본표제"), sTitle);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0371::OnOK() 
{
	// TODO: Add extra validation here
	CString sTitle;
	GetControlData(_T("CM_BO_CAT_PROC_0371"), _T("IBS_본표제"), sTitle);

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0371"));

	CString strWhere;	

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount = pDM->GetRowCount();

	if ( iRowCount > 0 ) {

	}
	
}

HBRUSH CBO_CAT_PROC_0371::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}