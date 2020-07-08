// HPhoneNumCombo.cpp : implementation file
//

#include "stdafx.h"
#include "HPhoneNumCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHPhoneNumCombo

CHPhoneNumCombo::CHPhoneNumCombo()
{
	InsertString( 0, _T("0") );
	InsertString( 1, _T("1") );
	SetCurSel( 1 );

}

CHPhoneNumCombo::~CHPhoneNumCombo()
{
}


BEGIN_MESSAGE_MAP(CHPhoneNumCombo, CComboBox)
	//{{AFX_MSG_MAP(CHPhoneNumCombo)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHPhoneNumCombo message handlers
