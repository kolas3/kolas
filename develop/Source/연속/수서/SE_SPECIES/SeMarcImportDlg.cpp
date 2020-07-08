// SeMarcImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeMarcImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImportDlg dialog


CSeMarcImportDlg::CSeMarcImportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMarcImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMarcImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSeMarcImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMarcImportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMarcImportDlg, CDialog)
	//{{AFX_MSG_MAP(CSeMarcImportDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImportDlg message handlers
