// BO_CAT_PROC_0320_FORM_07.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM_07.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_07 dialog


CBO_CAT_PROC_0320_FORM_07::CBO_CAT_PROC_0320_FORM_07(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM_07)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CBO_CAT_PROC_0320_FORM_07::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_PROC_0320_FORM_07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM_07)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM_07, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM_07)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_07 message handlers

BOOL CBO_CAT_PROC_0320_FORM_07::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_07")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("자료범주표시"), _T("s"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
