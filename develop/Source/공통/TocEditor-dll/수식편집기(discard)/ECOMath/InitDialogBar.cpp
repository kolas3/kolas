// InitDialogBar.cpp : implementation file
//

#include "stdafx.h"
//#include <afxpriv.h>
#include "InitDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInitDialogBar dialog


CInitDialogBar::CInitDialogBar()
{
}


VOID CInitDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitDialogBar)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CInitDialogBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitDialogBar message handlers

BOOL CInitDialogBar::Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName,
	UINT nStyle, UINT nID)
{	
	// Let MFC Create the control
	if(!CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID))
		return FALSE;	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())		
		return FALSE;	
	return TRUE;
}

BOOL CInitDialogBar::Create(CWnd * pParentWnd, UINT nIDTemplate,
	UINT nStyle, UINT nID)
{
	if(!CDialogBar::Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID))
		return FALSE;	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())		
		return FALSE;	
	return TRUE;
}


BOOL CInitDialogBar::OnInitDialogBar() 
{
	// Support for the MFC DDX model 
	// If you do not want this do not call the base class	// from derived classes
	UpdateData(FALSE);	
	return TRUE;
}

BOOL CInitDialogBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls( this, TRUE );
	return CDialogBar::PreTranslateMessage(pMsg);
}
/*
LRESULT CInitDialogBar::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogControls(this, FALSE);
    return 0;
}
*/
/*
LRESULT CInitDialogBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
    UpdateDialogControls(this, FALSE);
    return 0L;
}
*/