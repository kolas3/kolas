// DlgMarcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMarcEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMarcEdit dialog


CDlgMarcEdit::CDlgMarcEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMarcEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMarcEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}


VOID CDlgMarcEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMarcEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMarcEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgMarcEdit)
	ON_NOTIFY(NM_CLICK, IDC_RICHEDIT1, OnClickRichedit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMarcEdit message handlers

VOID CDlgMarcEdit::OnClickRichedit1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
