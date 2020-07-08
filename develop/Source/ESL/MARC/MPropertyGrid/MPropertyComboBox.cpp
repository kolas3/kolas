// MarcAdjComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MPropertyComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPropertyComboBox

CMPropertyComboBox::CMPropertyComboBox(CESL_Control_Element *pCE)
	:CESL_ControlMultiComboBox(pCE)
{
	m_pParentGrid = NULL;
}

CMPropertyComboBox::~CMPropertyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMPropertyComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMPropertyComboBox)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjEdit Operations

VOID CMPropertyComboBox::NotifyModify()
{
	m_pParentGrid->PostMessage(WM_COMBO_MODIFY_MG, 1, 0);
}

VOID CMPropertyComboBox::NotifyCancel()
{
	m_pParentGrid->PostMessage(WM_COMBO_MODIFY_MG, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyComboBox message handlers

VOID CMPropertyComboBox::OnKillfocus() 
{
	NotifyModify();	
}

VOID CMPropertyComboBox::OnSelchange() 
{
	NotifyModify();		
}

VOID CMPropertyComboBox::OnSelendcancel() 
{
	NotifyCancel();		
}

BOOL CMPropertyComboBox::PreTranslateMessage(MSG* pMsg) 
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
