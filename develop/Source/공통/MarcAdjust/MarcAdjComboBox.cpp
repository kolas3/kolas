// MarcAdjComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjComboBox

CMarcAdjComboBox::CMarcAdjComboBox(CESL_Control_Element *pCE)
	:CESL_ControlMultiComboBox(pCE)
{
}

CMarcAdjComboBox::~CMarcAdjComboBox()
{
}


BEGIN_MESSAGE_MAP(CMarcAdjComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMarcAdjComboBox)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjEdit Operations

VOID CMarcAdjComboBox::NotifyModify()
{
	GetParent()->PostMessage(WM_COMBO_MODIFY, 1, 0);
}

VOID CMarcAdjComboBox::NotifyCancel()
{
	GetParent()->PostMessage(WM_COMBO_MODIFY, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjComboBox message handlers

VOID CMarcAdjComboBox::OnKillfocus() 
{
	NotifyModify();	
}

VOID CMarcAdjComboBox::OnSelchange() 
{
	NotifyModify();		
}

VOID CMarcAdjComboBox::OnSelendcancel() 
{
	NotifyCancel();		
}

BOOL CMarcAdjComboBox::PreTranslateMessage(MSG* pMsg) 
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
