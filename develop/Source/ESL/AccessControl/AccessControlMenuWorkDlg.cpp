// AccessControlMenuWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlMenuWorkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlMenuWorkDlg dialog


CAccessControlMenuWorkDlg::CAccessControlMenuWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlMenuWorkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlMenuWorkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CAccessControlMenuWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlMenuWorkDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlMenuWorkDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlMenuWorkDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlMenuWorkDlg message handlers
