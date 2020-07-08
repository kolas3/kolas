// GridUpdateEdt.cpp : implementation file
//

#include "stdafx.h"
#include "GridUpdateEdt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridUpdateEdt

CGridUpdateEdt::CGridUpdateEdt()
{
	m_pParent = NULL;
	// KOL.UDF.022 시큐어코딩 보완
	m_nCol = 0;
	m_nRow = 0;
}

CGridUpdateEdt::~CGridUpdateEdt()
{
}


BEGIN_MESSAGE_MAP(CGridUpdateEdt, CEdit)
	//{{AFX_MSG_MAP(CGridUpdateEdt)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridUpdateEdt message handlers

INT CGridUpdateEdt::SetGridText(INT nOption)
{
	::PostMessage( GetParent()->GetSafeHwnd(), UPDATE_GRIDTEXT, nOption, 0 );
	return 0;
}

INT CGridUpdateEdt::SetParentWnd(CWnd *pParent)
{
	if ( !pParent ) 
	{
		// 인자 포인트가 잘못 되었다 
		return -1;
	}

	m_pParent = pParent;
	return 0;
}

VOID CGridUpdateEdt::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	SetGridText( 1 );
	
}

BOOL CGridUpdateEdt::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			SetGridText( 1 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE )
		{
			SetGridText( 0 );
			return TRUE;
		}
	}		

	return CEdit::PreTranslateMessage(pMsg);
}
