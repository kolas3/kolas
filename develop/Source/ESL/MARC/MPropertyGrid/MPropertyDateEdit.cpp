// MPropertyDateEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MPropertyDateEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateEdit

CMPropertyDateEdit::CMPropertyDateEdit(CESL_Control_Element *ELEMENT)
	:CECO_ControlDateEdit(ELEMENT)
{
	m_pParentGrid = NULL;
}

CMPropertyDateEdit::~CMPropertyDateEdit()
{
}


BEGIN_MESSAGE_MAP(CMPropertyDateEdit, CEdit)
	//{{AFX_MSG_MAP(CMPropertyDateEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateEdit Operations

VOID CMPropertyDateEdit::NotifyModify()
{
	//m_pParentGrid->PostMessage(WM_DATE_EDIT_MODIFY_MG, 1, 0);
}

VOID CMPropertyDateEdit::NotifyCancel()
{
	//m_pParentGrid->PostMessage(WM_DATE_EDIT_MODIFY_MG, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateEdit message handlers

VOID CMPropertyDateEdit::OnKillfocus() 
{
	NotifyModify();
}

BOOL CMPropertyDateEdit::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify();
			return TRUE;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}