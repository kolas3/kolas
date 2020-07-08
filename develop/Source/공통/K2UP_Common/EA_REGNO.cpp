// BO_CHECKIN_MGR.cpp : implementation file
//

#include "stdafx.h"
#include "CEA_REGNO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CHECKIN_MGR dialog


CEA_REGNO::CEA_REGNO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CEA_REGNO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CHECKIN_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CEA_REGNO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEA_REGNO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEA_REGNO, CDialog)
	//{{AFX_MSG_MAP(CEA_REGNO)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CHECKIN_MGR message handlers

VOID CEA_REGNO::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CEA_REGNO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CHECKIN_MGR")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEA_REGNO::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);	
}
