// DivisionNo_Data_View.cpp : implementation file
//

#include "stdafx.h"
#include "	\ add additional includes here"
#include "DivisionNo_Data_View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Data_View dialog


CDivisionNo_Data_View::CDivisionNo_Data_View(CWnd* pParent /*=NULL*/)
	: CDialog(CDivisionNo_Data_View::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Data_View)
	m_bCopyYN = FALSE;
	m_bReviewYN = FALSE;
	m_bDupYN = FALSE;
	//}}AFX_DATA_INIT
}


VOID CDivisionNo_Data_View::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Data_View)
	DDX_Check(pDX, IDC_chkDIVNO_WORK_DUP_SEARCH, m_bCopyYN);
	DDX_Check(pDX, IDC_chkDIVNO_PURCHASE_CHECK, m_bReviewYN);
	DDX_Check(pDX, IDC_chkDIVNO_HOLD_DUP_SEARCH, m_bDupYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Data_View, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Data_View)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Data_View message handlers
