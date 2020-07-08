// SPFInsertTask.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "SPFInsertTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SPFInsertTask dialog


SPFInsertTask::SPFInsertTask(CWnd* pParent /*=NULL*/)
	: CDialog(SPFInsertTask::IDD, pParent)
{
	//{{AFX_DATA_INIT(SPFInsertTask)
	m_DESC = _T("");
	m_NAME = _T("");
	//}}AFX_DATA_INIT
}


VOID SPFInsertTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SPFInsertTask)
	DDX_Text(pDX, IDC_TASK_DESC, m_DESC);
	DDX_Text(pDX, IDC_TASK_NAME, m_NAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SPFInsertTask, CDialog)
	//{{AFX_MSG_MAP(SPFInsertTask)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SPFInsertTask message handlers

BOOL SPFInsertTask::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID SPFInsertTask::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CDialog::OnOK();	
}
