// MarcAdjEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MPropertyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPropertyEdit

CMPropertyEdit::CMPropertyEdit()
{
	m_nEditMode = editEasy;
	m_pParentGrid = NULL;
}

CMPropertyEdit::~CMPropertyEdit()
{
}


BEGIN_MESSAGE_MAP(CMPropertyEdit, CEdit)
	//{{AFX_MSG_MAP(CMPropertyEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPropertyEdit Operations

VOID CMPropertyEdit::NotifyModify(UINT nChar)
{
	m_pParentGrid->PostMessage(WM_EDIT_MODIFY_MG, 1, nChar);
}

VOID CMPropertyEdit::NotifyCancel()
{
	m_pParentGrid->PostMessage(WM_EDIT_MODIFY_MG, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyEdit message handlers

VOID CMPropertyEdit::OnKillfocus() 
{
	NotifyModify(0);	
}

VOID CMPropertyEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nEditMode == editEasy)
	{
		if (nChar == VK_UP || nChar == VK_DOWN)
		{
			NotifyModify(nChar);
		}
	}
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CMPropertyEdit::PreTranslateMessage(MSG* pMsg) 
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