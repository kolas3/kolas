// GridEdt.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "GridEdt.h"

//#include _T("BOSpeciesPI_SecondTAB.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridEdt

CGridEdt::CGridEdt()
{
	m_bUseNotMode = FALSE;
}

CGridEdt::~CGridEdt()
{
}


BEGIN_MESSAGE_MAP(CGridEdt, CEdit)
	//{{AFX_MSG_MAP(CGridEdt)
	ON_WM_CLOSE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
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

	//2004-12-20
	//modified by loveisverb
	SetGridText( 1 );
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
		else if( pMsg->wParam == VK_ESCAPE )
		{
			SetGridText( 0 );
			return true;
		}
	
	return CEdit::PreTranslateMessage(pMsg);
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 대출기한 입력창에서 숫자만 입력하도록 하기 위해서
VOID CGridEdt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( ( nChar >= '0' && nChar <= '9' ) ||  nChar == VK_RETURN || nChar == VK_ESCAPE || nChar == VK_BACK )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}