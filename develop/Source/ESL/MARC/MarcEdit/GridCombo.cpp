// GridCombo.cpp : implementation file
//

#include "stdafx.h"
#include "GridCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCombo

CGridCombo::CGridCombo()
{
}

CGridCombo::~CGridCombo()
{
}


BEGIN_MESSAGE_MAP(CGridCombo, CComboBox)
	//{{AFX_MSG_MAP(CGridCombo)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridCombo message handlers

BOOL CGridCombo::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify();
			return true;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return true;
		}
	}	
	return CComboBox::PreTranslateMessage(pMsg);
}

VOID CGridCombo::OnSelchange() 
{
	NotifyModify();	
}

VOID CGridCombo::OnSelendcancel() 
{
	NotifyCancel();	
}

VOID CGridCombo::OnKillfocus() 
{
	NotifyModify();
}

VOID CGridCombo::NotifyModify()
{
	::PostMessage(parent->GetSafeHwnd(), WM_GRID_COMBO_MODIFY, 1, 0);
}

VOID CGridCombo::NotifyCancel()
{
	::PostMessage(parent->GetSafeHwnd(), WM_GRID_COMBO_MODIFY, 0, 0);
}