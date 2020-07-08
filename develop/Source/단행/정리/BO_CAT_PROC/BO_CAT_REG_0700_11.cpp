// BO_CAT_REG_0700_11.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0700_11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0700_11 dialog


CBO_CAT_REG_0700_11::CBO_CAT_REG_0700_11(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0700_11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0700_11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0700_11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0700_11, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0700_11)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0700_11 message handlers

BOOL CBO_CAT_REG_0700_11::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}