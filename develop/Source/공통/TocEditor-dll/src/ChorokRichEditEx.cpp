// ChorokRichEditEx.cpp : implementation file
//

#include "stdafx.h"
#include "ChorokRichEditEx.h"

#include "ChorokDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChorokRichEditEx

CChorokRichEditEx::CChorokRichEditEx()
{
	m_pParent= NULL ;
	m_pTocEditorDlgWnd= NULL ;
}

CChorokRichEditEx::~CChorokRichEditEx()
{
}


BEGIN_MESSAGE_MAP(CChorokRichEditEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CChorokRichEditEx)
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDC_BUTTON_SOOSIKEDITOR, OnSoosikEditor)
	ON_COMMAND(IDC_BUTTON_VIEWER, OnViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChorokRichEditEx message handlers
VOID CChorokRichEditEx::OnSoosikEditor()
{
	((CChorokDlg*)m_pParent)->ExecuteSoosikEditor() ;
}

VOID CChorokRichEditEx::OnViewer()
{
	((CChorokDlg*)m_pParent)->ShowView() ;
}

VOID CChorokRichEditEx::SetParentWnd(CWnd *pParentWnd, CWnd* pTocEditorDlgWnd)
{
	m_pParent= pParentWnd ;
	m_pTocEditorDlgWnd= pTocEditorDlgWnd ;
}

BOOL CChorokRichEditEx::PreTranslateMessage(MSG* pMsg) 
{

	if ( pMsg -> message == WM_RBUTTONDOWN ) {
		SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, pMsg->lParam ) ;

	} else if ( pMsg -> message == WM_RBUTTONUP ) {
		CMenu menu;
		menu.CreatePopupMenu();

//		menu.AppendMenu(MF_STRING|MF_ENABLED, IDC_BUTTON_SOOSIKEDITOR, _T("수식편집기(&S)"));
		menu.AppendMenu(MF_STRING|MF_ENABLED, IDC_BUTTON_VIEWER, _T("뷰어(&V)"));
		menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, pMsg->pt.x, pMsg->pt.y, this);

	}

	return CRichEditCtrl::PreTranslateMessage(pMsg);
}
