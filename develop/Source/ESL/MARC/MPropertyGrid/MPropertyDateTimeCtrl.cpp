// MPropertyDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MPropertyDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateTimeCtrl

CMPropertyDateTimeCtrl::CMPropertyDateTimeCtrl(CESL_Control_Element * ELEMENT)
	:CESL_DateTimeCtrl(ELEMENT)
{
	m_pParentGrid = NULL;
}

CMPropertyDateTimeCtrl::~CMPropertyDateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMPropertyDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CMPropertyDateTimeCtrl)
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateTimeCtrl Operations

VOID CMPropertyDateTimeCtrl::NotifyModify()
{
	m_pParentGrid->PostMessage(WM_DATE_TIME_MODIFY_MG, 1, 0);
}

VOID CMPropertyDateTimeCtrl::NotifyCancel()
{
	m_pParentGrid->PostMessage(WM_DATE_TIME_MODIFY_MG, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyDateTimeCtrl message handlers

BOOL CMPropertyDateTimeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify();
			return true;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return true;
		}
		if (pMsg->wParam == VK_TAB)
		{
			if (GetAsyncKeyState(VK_SHIFT))
				pMsg->wParam = VK_LEFT;
			else
				pMsg->wParam = VK_RIGHT;
		}
	}

	return CDateTimeCtrl::PreTranslateMessage(pMsg);
}

VOID CMPropertyDateTimeCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	if (GetMonthCalCtrl()->GetSafeHwnd() != GetFocus()->GetSafeHwnd())
		NotifyCancel();
}

VOID CMPropertyDateTimeCtrl::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NotifyModify();
	*pResult = 0;
}