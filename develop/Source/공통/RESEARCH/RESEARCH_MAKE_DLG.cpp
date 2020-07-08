// RESEARCH_MAKE_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_MAKE_DLG.h"
#include "RESEARCH_PREVIEW_DLG.h"
#include "RESEARCH_INFO_DLG.h"
#include "RESEARCH_QUESTION_DLG.h"
#include "RESEARCH_QUES_INFO_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_MAKE_DLG dialog


CRESEARCH_MAKE_DLG::CRESEARCH_MAKE_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_MAKE_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
BOOL CRESEARCH_MAKE_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}


VOID CRESEARCH_MAKE_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_MAKE_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_MAKE_DLG, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_MAKE_DLG)
	ON_BN_CLICKED(IDC_bPREVIEW, OnbPREVIEW)
	ON_BN_CLICKED(IDC_bINFO_INPUT, OnbINFOINPUT)
	ON_BN_CLICKED(IDC_bSUB_INPUT, OnbSUBINPUT)
	ON_BN_CLICKED(IDC_bSUBCHECK_INPUT, OnbSUBCHECKINPUT)
	ON_BN_CLICKED(IDC_bCOLSE, OnbCOLSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_MAKE_DLG message handlers

VOID CRESEARCH_MAKE_DLG::OnbPREVIEW() 
{
	// TODO: Add your control notification handler code here
	CRESEARCH_PREVIEW_DLG dlg(this) ;
	dlg.DoModal () ;
}

VOID CRESEARCH_MAKE_DLG::OnbINFOINPUT() 
{
	// TODO: Add your control notification handler code here
	CRESEARCH_INFO_DLG dlg(this) ;
	dlg.DoModal () ;
}

VOID CRESEARCH_MAKE_DLG::OnbSUBINPUT() 
{
	// TODO: Add your control notification handler code here
	CRESEARCH_QUESTION_DLG dlg(this) ;
	dlg.DoModal () ;
}

VOID CRESEARCH_MAKE_DLG::OnbSUBCHECKINPUT() 
{
	// TODO: Add your control notification handler code here
	CRESEARCH_QUES_INFO_DLG dlg(this);
	dlg.DoModal () ;
}

VOID CRESEARCH_MAKE_DLG::OnbCOLSE() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

HBRUSH CRESEARCH_MAKE_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
