// DLG_008_RB.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DLG_008_RB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_RB dialog


CDLG_008_RB::CDLG_008_RB(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_008_RB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_008_RB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLG_008_RB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_008_RB)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_008_RB, CDialog)
	//{{AFX_MSG_MAP(CDLG_008_RB)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_RB message handlers

BOOL CDLG_008_RB::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
