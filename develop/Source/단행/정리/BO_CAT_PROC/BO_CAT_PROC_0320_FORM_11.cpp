// BO_CAT_PROC_0320_FORM_11.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM_11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_11 dialog


CBO_CAT_PROC_0320_FORM_11::CBO_CAT_PROC_0320_FORM_11(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM_11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CBO_CAT_PROC_0320_FORM_11::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_PROC_0320_FORM_11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM_11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM_11, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM_11)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_11 message handlers

BOOL CBO_CAT_PROC_0320_FORM_11::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_11")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("자료범주표시"), _T("c"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
