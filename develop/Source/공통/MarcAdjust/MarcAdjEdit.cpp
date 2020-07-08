// MarcAdjEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjEdit

CMarcAdjEdit::CMarcAdjEdit()
{

}

CMarcAdjEdit::~CMarcAdjEdit()
{
}


BEGIN_MESSAGE_MAP(CMarcAdjEdit, CEdit)
	//{{AFX_MSG_MAP(CMarcAdjEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjEdit Operations

VOID CMarcAdjEdit::NotifyModify(UINT nChar)
{
	GetParent()->PostMessage(WM_EDIT_MODIFY, 1, nChar);
}

VOID CMarcAdjEdit::NotifyCancel()
{
	GetParent()->PostMessage(WM_EDIT_MODIFY, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjEdit message handlers

VOID CMarcAdjEdit::OnKillfocus() 
{
	NotifyModify(0);	
}

BOOL CMarcAdjEdit::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify(0);
			return TRUE;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}
