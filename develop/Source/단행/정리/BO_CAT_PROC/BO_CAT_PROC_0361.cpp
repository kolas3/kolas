// BO_CAT_PROC_0361.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_0361.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0361 dialog


CBO_CAT_PROC_0361::CBO_CAT_PROC_0361(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_CAT_PROC_0361::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0361)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_PROC_0361::~CBO_CAT_PROC_0361()
{
}

VOID CBO_CAT_PROC_0361::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0361)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0361, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0361)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0361 message handlers

BOOL CBO_CAT_PROC_0361::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0361::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CSS_1_1 = new CBO_CAT_PROC_0362(this);
			
	m_CSS_1_1->Create(NULL,NULL,WS_VISIBLE,m_rect,this,IDD_BO_CAT_PROC_0361,NULL);
	
	m_CSS_1_1->ShowWindow(SW_SHOW);
	m_CSS_1_1->UpdateWindow();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBO_CAT_PROC_0361::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}