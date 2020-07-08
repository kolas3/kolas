// BoLocSoc.cpp : implementation file
//

#include "stdafx.h"
#include "BoLocSoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoLocSoc

CBoLocSoc::CBoLocSoc()
{
}

CBoLocSoc::~CBoLocSoc()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CBoLocSoc, CSocket)
	//{{AFX_MSG_MAP(CBoLocSoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CBoLocSoc member functions

void CBoLocSoc::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnReceive(nErrorCode);
}
