// TableInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LibInfoCollector.h"
#include "TableInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableInfoDlg dialog


CTableInfoDlg::CTableInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CTableInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTableInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CTableInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableInfoDlg message handlers
