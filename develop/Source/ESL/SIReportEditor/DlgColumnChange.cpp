// DlgColumnChange.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgColumnChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgColumnChange dialog


CDlgColumnChange::CDlgColumnChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColumnChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgColumnChange)
	m_nColumn = 1;
	//}}AFX_DATA_INIT
}


VOID CDlgColumnChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgColumnChange)
	DDX_Text(pDX, IDC_eCOLUMN, m_nColumn);
	DDV_MinMaxInt(pDX, m_nColumn, 1, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgColumnChange, CDialog)
	//{{AFX_MSG_MAP(CDlgColumnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgColumnChange message handlers
