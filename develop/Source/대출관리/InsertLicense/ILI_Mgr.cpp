// ILI_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "InsertLicense.h"
#include "ILI_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILI_Mgr dialog


CILI_Mgr::CILI_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CILI_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CILI_Mgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CILI_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILI_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILI_Mgr, CDialog)
	//{{AFX_MSG_MAP(CILI_Mgr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILI_Mgr message handlers

void CILI_Mgr::InitGrid()
{

}