// GridEdt.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "GridEdt.h"

#include "ExcelHeaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridEdt

CGridEdt::CGridEdt()
{
	m_tab = false;

	col = 0;
	row = 0;
	parent = NULL;
}

CGridEdt::~CGridEdt()
{
}


BEGIN_MESSAGE_MAP(CGridEdt, CEdit)
	//{{AFX_MSG_MAP(CGridEdt)
	ON_WM_CLOSE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEdt message handlers

VOID CGridEdt::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnClose();
}

VOID CGridEdt::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here	
}

VOID CGridEdt::SetGridText( INT option )
{
	::PostMessage( parent->GetSafeHwnd(), UPDATE_GRID_EDT, option, 0 );
}

BOOL CGridEdt::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
		if( pMsg->wParam == VK_RETURN )
		{
			SetGridText( 1 );
			return true;
		}
		else if( pMsg->wParam == VK_TAB )
		{
			SetGridText( 1 );
			m_tab = true;
			return true;
		}
		else if( pMsg->wParam == VK_ESCAPE )
		{
			SetGridText( 0 );
			return true;
		}
	
	return CEdit::PreTranslateMessage(pMsg);
}
