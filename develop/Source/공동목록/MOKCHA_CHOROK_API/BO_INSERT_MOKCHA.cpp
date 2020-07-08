// BO_INSERT_MOKCHA.cpp : implementation file
//

#include "stdafx.h"
#include "BO_INSERT_MOKCHA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_INSERT_MOKCHA

CBO_INSERT_MOKCHA::CBO_INSERT_MOKCHA(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
/*
BOOL CBO_INSERT_MOKCHA::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
*/
CBO_INSERT_MOKCHA::~CBO_INSERT_MOKCHA()
{
}


BEGIN_MESSAGE_MAP(CBO_INSERT_MOKCHA, CWnd)
	//{{AFX_MSG_MAP(CBO_INSERT_MOKCHA)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBO_INSERT_MOKCHA message handlers
