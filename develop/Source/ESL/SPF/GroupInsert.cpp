// GroupInsert.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "GroupInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupInsert dialog


CGroupInsert::CGroupInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupInsert)
	m_DESC = _T("");
	m_NAME = _T("");
	m_Type = FALSE;
	//}}AFX_DATA_INIT
}


VOID CGroupInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupInsert)
	DDX_Text(pDX, IDC_GROUP_DESC, m_DESC);
	DDX_Text(pDX, IDC_GROUP_NAME, m_NAME);
	DDX_Check(pDX, IDC_GROUP_TYPE, m_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupInsert, CDialog)
	//{{AFX_MSG_MAP(CGroupInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupInsert message handlers

VOID CGroupInsert::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
