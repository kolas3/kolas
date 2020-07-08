// BO_CAT_AUXILIARY_WORD_0000.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_AUXILIARY_WORD_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD_0000 dialog


CBO_CAT_AUXILIARY_WORD_0000::CBO_CAT_AUXILIARY_WORD_0000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_AUXILIARY_WORD_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_AUXILIARY_WORD_0000::~CBO_CAT_AUXILIARY_WORD_0000()
{
}

VOID CBO_CAT_AUXILIARY_WORD_0000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_AUXILIARY_WORD_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_AUXILIARY_WORD_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_AUXILIARY_WORD_0000)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD_0000 message handlers

BOOL CBO_CAT_AUXILIARY_WORD_0000::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CBO_CAT_AUXILIARY_WORD_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

BOOL CBO_CAT_AUXILIARY_WORD_0000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
