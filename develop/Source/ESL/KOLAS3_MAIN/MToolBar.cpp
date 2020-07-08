// MToolBar.cpp: implementation of the CMToolBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "MToolBar.h"

#include <afxpriv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMToolBar, CToolBar)

CMToolBar::CMToolBar()
{
	m_bIsMenuRemoved = FALSE;
	m_ContextMenu = NULL;
	m_nDropIdx = -1;

	m_Bitmap.LoadBitmap(IDB_TOOLBAR_BK);	
	m_Dlg_brush.CreatePatternBrush(&m_Bitmap);	
}

CMToolBar::~CMToolBar()
{
	m_Dlg_brush.DeleteObject();
	delete m_ContextMenu;
	m_ContextMenu = NULL;
}

BEGIN_MESSAGE_MAP(CMToolBar, CToolBar)
//{{AFX_MSG_MAP(CCBToolBar)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CTLCOLOR()
	ON_WM_INITMENU()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+100, OnExtMenu)
END_MESSAGE_MAP()

VOID CMToolBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CToolBar::OnWindowPosChanged(lpwndpos);
	
	// should only be called once, when floated.
	if( IsFloating() )
	{
		if( m_pDockBar && !m_bIsMenuRemoved )
		{
			CWnd* pParent = m_pDockBar->GetParent();
			if( pParent->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
			{
				pParent->ModifyStyle( WS_SYSMENU, 0, 0 );
				m_bIsMenuRemoved = TRUE;
			}
			
		}
	}
	else if( m_bIsMenuRemoved ) {
		m_bIsMenuRemoved = FALSE;
	}	
}


HBRUSH CMToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) 
	{
		hbr= HBRUSH(m_Dlg_brush);	
	}
	else 
	{
		hbr= HBRUSH(m_Dlg_brush);	
		//hbr = CMToolBar::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	return hbr;
}

void CMToolBar::OnInitMenu(CMenu* pMenu) 
{
	CToolBar::OnInitMenu(pMenu);
	
}

BOOL CMToolBar::OnEraseBkgnd(CDC* pDC) 
{
	/*
	COLORREF color;       
    color= RGB(255,255,255);

    CRect rc;
    GetWindowRect(&rc);


    pDC->FillSolidRect(0, 0, rc.Width(), rc.Height(), color); 
    return true;
	*/

	CRect rect;
	GetClientRect(&rect);
		
	pDC->FillRect(&rect, &m_Dlg_brush);

	return TRUE;
}


void CMToolBar::OnSize(UINT nType, INT cx, INT cy) 
{
	CToolBar::OnSize(nType, cx, cy);	
	ToolBarSizing(cx);
}

void CMToolBar::ToolBarSizing(INT cx)
{
	CRect rToolBar;
	GetClientRect(rToolBar);

	if(!m_Btn.GetSafeHwnd())
	{
		m_Btn.Create(this);
	}
	if(-1 == cx)
	{
		cx = rToolBar.Width();
	}
	CToolBarCtrl &pToolBar = GetToolBarCtrl();
	const INT MAX_CNT = pToolBar.GetButtonCount();

	CRect rToolBarItem;
	TBBUTTON Button;
	CToolBarItem* pItem = NULL;
	INT nSize = 0, nIdx = 0, nPrevID = 0;
	for(INT nCnt = 0; nCnt < MAX_CNT; nCnt++)
	{		
		pToolBar.GetButton(nCnt, &Button);
		pItem = (CToolBarItem*)Button.dwData;

		if(pItem && pItem->m_bIsVisible)
		{				
			pToolBar.GetRect(Button.idCommand, rToolBarItem);	
			nSize += pItem->m_nWidth;
		}
		else
		{
			continue;
		}

		if(nSize < cx - 20)
		{
			nIdx = nCnt+1;
			pToolBar.SetState(Button.idCommand, TBSTATE_ENABLED);
		}
		else
		{
			pToolBar.SetState(Button.idCommand, TBSTATE_HIDDEN);
		}
		
//		기존 로직 제거
// 		if(nSize < cx - 20)
// 		{
// 			if(Button.fsState != TBSTATE_HIDDEN)
// 			{
//  				if(pItem && pItem->m_bIsVisible)
// 				{
// 					pToolBar.GetRect(Button.idCommand, rToolBarItem);
// 					if(rToolBarItem.right == nSize)
// 					{
// 						nSize = cx;
// 					}
// 					else
// 					{
// 						nSize = rToolBarItem.right;
// 					}
// 				}
// 			}
// 			else
// 			{
//  				if(pItem && pItem->m_bIsVisible)
// 				{
// 					nSize += pItem->m_nWidth;
// 				}
// 			}
// 		}
// 
// 		if(nSize >= cx -20)
// 		{
// 			if(Button.fsState != TBSTATE_HIDDEN)
// 			{
//  				pItem->m_nWidth = rToolBarItem.Width();
// 				pToolBar.SetState(Button.idCommand, TBSTATE_HIDDEN);				
// 			}
// 		}
// 		else
// 		{
//  			if(pItem && pItem->m_bIsVisible)
// 			{
// 				nIdx++;
// 				if(Button.fsState != TBSTATE_ENABLED)
// 				{
// 					pToolBar.SetState(Button.idCommand, TBSTATE_ENABLED);
// 				}
// 			}
// 		}
	}
	m_nDropIdx = nIdx;

	if(nSize >= cx -20)
	{
		m_Btn.EnableWindow(TRUE);
		m_Btn.MoveWindow(CRect(rToolBar.right-20, 3, rToolBar.right-5, rToolBar.bottom-3));
		m_Btn.ShowWindow(SW_SHOW);
	}
	else
	{
 		m_Btn.ShowWindow(SW_HIDE);
	}
}

LRESULT CMToolBar::OnExtMenu(WPARAM wParam, LPARAM lParam)
{
	CString		strAlias;
	CRect		s_rect;   
	
	delete m_ContextMenu;
	m_ContextMenu = NULL;
    if ( m_ContextMenu == NULL ) 
	{
		m_ContextMenu = new CMenu;
		m_ContextMenu->CreatePopupMenu();		
	}	

// 	for ( i=m_DropIdx+1 ; i<m_strArrayItemAlias.GetSize() ; i++ )
// 	{
// 		strAlias.Format(_T("%s"), m_strArrayItemAlias.GetAt(i));
// 		nID = GetToolBarItemID(strAlias);
// 		if ( strAlias == _T("SYS_CLOSE") ) strAlias = _T("닫기");
// 
// 		m_ContextMenu->AppendMenu( MF_STRING , nID, strAlias );		
// 	}
	if(0 > m_nDropIdx)
	{
		return -1000;
	}

	TBBUTTON Button;
	CToolBarItem* pItem = NULL;
	CToolBarCtrl &pToolBar = GetToolBarCtrl();
	const INT MAX_CNT = pToolBar.GetButtonCount();
	for(INT nIdx = m_nDropIdx; nIdx < MAX_CNT; nIdx++)
	{
		pToolBar.GetButton(nIdx, &Button);
		pItem = (CToolBarItem*)Button.dwData;	
		if(pItem && Button.fsState != TBSTATE_HIDDEN)
		{
 			if(pItem->m_bIsVisible)
			{
				m_ContextMenu->AppendMenu(MF_STRING , pItem->m_nID, pItem->m_strCaption);
			}
		}
	}
    
    m_Btn.GetWindowRect( &s_rect );
	m_ContextMenu->TrackPopupMenu( TPM_LEFTALIGN, s_rect.right, s_rect.bottom, this );
// 	m_Btn.NormalStatus();

	return 0;
}

// 리사이징시 크기계산을 위해 각 툴바버튼 사이즈를 저장시켜 놓는다.
void CMToolBar::InitToolbar()
{
	CRect rToolBar;
	GetClientRect(rToolBar);

	if(!m_Btn.GetSafeHwnd())
	{
		m_Btn.Create(this);
	}

	CToolBarCtrl &pToolBar = GetToolBarCtrl();
	const INT MAX_CNT = pToolBar.GetButtonCount();

	CRect rToolBarItem;
	TBBUTTON Button;
	CToolBarItem* pItem = NULL;
	INT nSize = 0, nIdx = 0, nPrevID = 0;
	for(INT nCnt = 0; nCnt < MAX_CNT; nCnt++)
	{		
		pToolBar.GetButton(nCnt, &Button);
		pItem = (CToolBarItem*)Button.dwData;

 		if(Button.fsState != TBSTATE_HIDDEN)
 		{
 				pToolBar.GetRect(Button.idCommand, rToolBarItem);
 		}
		if(Button.fsState != TBSTATE_HIDDEN)
		{
			pItem->m_nWidth = rToolBarItem.Width();
			pToolBar.SetState(Button.idCommand, TBSTATE_HIDDEN);				
		}
	}

	return;
}