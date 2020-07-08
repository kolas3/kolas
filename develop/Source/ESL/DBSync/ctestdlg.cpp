// ctestdlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBSync.h"
#include "ctestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ctestdlg dialog


ctestdlg::ctestdlg(CWnd* pParent /*=NULL*/)
	: CDialog(ctestdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ctestdlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID ctestdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ctestdlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ctestdlg, CDialog)
	//{{AFX_MSG_MAP(ctestdlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ctestdlg message handlers
