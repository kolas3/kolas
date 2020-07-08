// DlgSearch.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "DlgSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch dialog


CDlgSearch::CDlgSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSearch, CDialog)
	//{{AFX_MSG_MAP(CDlgSearch)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch message handlers
