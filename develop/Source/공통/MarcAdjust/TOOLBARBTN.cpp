// TOOLBARBTN.cpp : implementation file
//

#include "stdafx.h"
#include "TOOLBARBTN.h"
#include "MarcToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTOOLBARBTN dialog
CMarcToolBar  *m_MarcToolParent;

CTOOLBARBTN::CTOOLBARBTN(CWnd* pParent /*=NULL*/)
	: CDialog(CTOOLBARBTN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTOOLBARBTN)
	//}}AFX_DATA_INIT
	m_MarcToolParent = NULL;
	m_bOn = FALSE;
}


VOID CTOOLBARBTN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTOOLBARBTN)	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTOOLBARBTN, CDialog)
	//{{AFX_MSG_MAP(CTOOLBARBTN)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTOOLBARBTN message handlers

BOOL CTOOLBARBTN::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_MarcToolParent = (CMarcToolBar*)pParentWnd;		
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CTOOLBARBTN::OnLButtonDown(UINT nFlags, CPoint point) 
{
	KillTimer(1);
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT) )->ShowWindow(SW_HIDE);		
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT2) )->ShowWindow(SW_SHOW);
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT3) )->ShowWindow(SW_HIDE);

	m_MarcToolParent->ExtMenu();
		
	CDialog::OnLButtonDown(nFlags, point);
}

VOID CTOOLBARBTN::OnRButtonDown(UINT nFlags, CPoint point) 
{
	KillTimer(1);
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT) )->ShowWindow(SW_HIDE);		
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT2) )->ShowWindow(SW_SHOW);
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT3) )->ShowWindow(SW_HIDE);	
	m_MarcToolParent->ExtMenu();
	
	CDialog::OnRButtonDown(nFlags, point);
}

VOID CTOOLBARBTN::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

BOOL CTOOLBARBTN::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	if(m_bOn == FALSE)
	{
		( (CStatic*) GetDlgItem (IDC_STATIC_TEXT) )->ShowWindow(SW_HIDE);		
		( (CStatic*) GetDlgItem (IDC_STATIC_TEXT2) )->ShowWindow(SW_HIDE);
		( (CStatic*) GetDlgItem (IDC_STATIC_TEXT3) )->ShowWindow(SW_SHOW);
		m_bOn = TRUE;
		SetTimer(1, 100, NULL);
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CTOOLBARBTN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_COMMAND )
	{
		if ( m_MarcToolParent != NULL )
		{
			m_MarcToolParent->PreTranslateMessage(pMsg);
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CTOOLBARBTN::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 && m_bOn == TRUE)
	{		
		CPoint	pt;	
		pt = GetCaretPos();
		if ( (pt.x <= 0 || pt.y <= 0)  ||
			 (pt.x >= 65 || pt.y >= 38) )
		{
			( (CStatic*) GetDlgItem (IDC_STATIC_TEXT) )->ShowWindow(SW_SHOW);		
			( (CStatic*) GetDlgItem (IDC_STATIC_TEXT2) )->ShowWindow(SW_HIDE);
			( (CStatic*) GetDlgItem (IDC_STATIC_TEXT3) )->ShowWindow(SW_HIDE);
			m_bOn = FALSE;
			KillTimer(1);
		}	
	}	
	
	CDialog::OnTimer(nIDEvent);
}



VOID CTOOLBARBTN::NormalStatus()
{
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT) )->ShowWindow(SW_SHOW);		
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT2) )->ShowWindow(SW_HIDE);
	( (CStatic*) GetDlgItem (IDC_STATIC_TEXT3) )->ShowWindow(SW_HIDE);
	m_bOn = FALSE;
}


