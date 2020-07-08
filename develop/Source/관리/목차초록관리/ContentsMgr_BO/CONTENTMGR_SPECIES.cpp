// CONTENTMGR_SPECIES.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_SPECIES.h"
#include "CONTENTMGR_02.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SPECIES dialog


CCONTENTMGR_SPECIES::CCONTENTMGR_SPECIES(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCONTENTMGR_SPECIES::~ CCONTENTMGR_SPECIES()
{
	
} 


BOOL CCONTENTMGR_SPECIES::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_SPECIES::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_SPECIES)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SPECIES, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_SPECIES)
	ON_BN_CLICKED(IDC_bEDIT, OnbEDIT)
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SPECIES message handlers

VOID CCONTENTMGR_SPECIES::OnbEDIT() 
{
	// TODO: Add your control notification handler code here
	CCONTENTMGR_02 dlg(0,this);
	dlg.DoModal() ;
}

BOOL CCONTENTMGR_SPECIES::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCONTENTMGR_SPECIES::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
