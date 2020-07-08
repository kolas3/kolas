// TestRichEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditor_Test.h"
#include "TestRichEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestRichEditDlg dialog


CTestRichEditDlg::CTestRichEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRichEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestRichEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CTestRichEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestRichEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestRichEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTestRichEditDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestRichEditDlg message handlers
