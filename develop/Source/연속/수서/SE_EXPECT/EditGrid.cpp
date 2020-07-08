// EditGrid.cpp : implementation file
//

#include "stdafx.h"
#include "EditGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditGrid

CEditGrid::CEditGrid()
{
	m_nCol = 0;
	m_nRow = 0;
	m_bTab = FALSE;
}

CEditGrid::~CEditGrid()
{
}


BEGIN_MESSAGE_MAP(CEditGrid, CEdit)
	//{{AFX_MSG_MAP(CEditGrid)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditGrid message handlers
VOID CEditGrid::SetGridText(INT nType)
{
	::PostMessage( parent->GetSafeHwnd(), UPDATE_GRID_EDIT, nType, 0 );
}

VOID CEditGrid::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	SetGridText(1);

	// TODO: Add your message handler code here
}

BOOL CEditGrid::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{

		if( pMsg->wParam == VK_RETURN )
		{
			SetGridText(1);

			return TRUE;
		}
		else if( pMsg->wParam == VK_TAB )
		{
			
			m_bTab = TRUE;
			SetGridText(1);

			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE )
		{
			SetGridText(0);

			return TRUE;
		}

	}

	return CEdit::PreTranslateMessage(pMsg);
}
