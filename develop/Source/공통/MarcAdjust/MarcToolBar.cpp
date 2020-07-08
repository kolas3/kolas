// MarcToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MarcToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcToolBar

CMarcToolBar::CMarcToolBar()
{
	bSizing = FALSE;
	m_bSizingStart = FALSE;
	m_pBtn = NULL;
	m_strArrayItemAlias.RemoveAll();	
	m_DropIdx = -1;
	m_pParent = NULL;
	m_ContextMenu = NULL;
}

CMarcToolBar::~CMarcToolBar()
{

}


BEGIN_MESSAGE_MAP(CMarcToolBar, CToolBar)
	//{{AFX_MSG_MAP(CMarcToolBar)
	ON_WM_SIZE()	
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(32000, 32040, UserSelectProc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcToolBar message handlers

VOID CMarcToolBar::OnSize(UINT nType, INT cx, INT cy) 
{
	ToolBarSizing();	

	CToolBar::OnSize(nType, cx, cy);	
}

VOID CMarcToolBar::ToolBarSizing()
{
	if ( m_bSizingStart == FALSE ) return;

	CToolBarCtrl &pToolBar = GetToolBarCtrl();

	INT nCount = pToolBar.GetButtonCount();

	INT nID = -1;
		
	CStringArray StringArray;	
	CString	strTmp;
	INT		nTmpSize;
	INT		nCountSize;
	INT		nButtonSize;

	nButtonSize = 0;
	nCountSize = 0;
	nTmpSize = 0;

	TBBUTTON tb;
	
	CRect rect;
	CRect ToolBarRect;
	INT		n, i;
	INT		nToolBarSize;
	INT		nAliasCnt;
	CString strAlias;
	BOOL bFind;

	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem->GetHeadPosition();
	pToolBar.GetWindowRect(&ToolBarRect);
	nToolBarSize = ToolBarRect.Width();
	n=0;
	
	m_DropIdx = -1;
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem->GetNext(pos));
		if (pToolBarItem == NULL) continue;
				
		nID = pToolBarItem->m_nID;
		pToolBar.GetRect(nID,&rect);		

		bFind = FALSE;
		nAliasCnt = m_strArrayItemAlias.GetSize();
		for ( i = 0 ; i < nAliasCnt ; i++ )
		{
			strAlias.Format(_T("%s"), m_strArrayItemAlias.GetAt(i) );
			if ( strAlias == pToolBarItem->m_strAlias )
			{
				bFind = TRUE;
				break;
			}
		}
		if (bFind == FALSE ) continue;
		

		
		nButtonSize += rect.Width();
		if ( nButtonSize+15 > nToolBarSize )
		{			
			pToolBar.SetState(nID, TBSTATE_HIDDEN );			
		}
		else
		{
			pToolBar.SetState(nID, TBSTATE_ENABLED );			
			m_DropIdx = n;
		}
		n++;		
	}
	
	if ( nButtonSize == 0 ) return;

	CString szBuf;
	
	if ( nButtonSize+15 > nToolBarSize ) 
	{
		if ( bSizing == FALSE )
		{
			if ( m_pBtn == NULL )
			{
				CRect BtnRect;
				
				BtnRect.left = ToolBarRect.right-15;
				BtnRect.right = ToolBarRect.right;
				BtnRect.top = ToolBarRect.top;
				BtnRect.bottom = ToolBarRect.bottom;				
				
				ScreenToClient(BtnRect);			

				m_pBtn = new CTOOLBARBTN;				
				m_pBtn->Create( NULL, NULL, WS_CHILD| WS_VISIBLE, BtnRect, this , 4000, NULL );				
				//m_pBtn->CreateEx( NULL, NULL, NULL, WS_CHILD| WS_VISIBLE, BtnRect, this , 4000, NULL );
				
			}
			m_pBtn->ShowWindow(SW_SHOW);
			m_pBtn->EnableWindow(TRUE);
			
			bSizing = TRUE;
		}		
	}	
	else
	{
		bSizing = FALSE;
		if ( m_pBtn != NULL )
		{
			m_pBtn->ShowWindow(SW_HIDE);
			m_pBtn->EnableWindow(FALSE);
		}
	}

	
	if ( m_pBtn != NULL ) 
	{		
		CRect BtnRect;
		BtnRect.left = ToolBarRect.right-15;
		BtnRect.right = ToolBarRect.right;
		BtnRect.top = ToolBarRect.top;
		BtnRect.bottom = ToolBarRect.bottom;	
		ScreenToClient(BtnRect);
		
		m_pBtn->MoveWindow(BtnRect);
	}	
}

VOID CMarcToolBar::ExtMenu()
{		
	CString		strAlias;
	CRect		s_rect;   
	INT			i;
	UINT		nID;
	
	delete m_ContextMenu;
	m_ContextMenu = NULL;
    if ( m_ContextMenu == NULL ) 
	{
		m_ContextMenu = new CMenu;
		m_ContextMenu->CreatePopupMenu();		
	}	

	for ( i=m_DropIdx+1 ; i<m_strArrayItemAlias.GetSize() ; i++ )
	{
		strAlias.Format(_T("%s"), m_strArrayItemAlias.GetAt(i));
		nID = GetToolBarItemID(strAlias);
		if ( strAlias == _T("SYS_CLOSE") ) strAlias = _T("´Ý±â");

		m_ContextMenu->AppendMenu( MF_STRING , nID, strAlias );		
	}
    
    m_pBtn->GetWindowRect( &s_rect );
	m_ContextMenu->TrackPopupMenu( TPM_LEFTALIGN, s_rect.right, s_rect.bottom, this );
	m_pBtn->NormalStatus();
}

VOID CMarcToolBar::ToolBarItemSet()
{	
	CString		strAlias;
	INT			nID = -1;
	INT			n = 0;
	BOOL		bCheck;

	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem->GetHeadPosition();
	CToolBarCtrl &pToolBar = GetToolBarCtrl();

	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem->GetNext(pos));
		if (pToolBarItem == NULL) continue;
				
		nID = pToolBarItem->m_nID;		
		bCheck = pToolBar.IsButtonEnabled(nID);
	
		if ( !bCheck )
		{
			continue;
		}
		
		strAlias.Format(_T("%s"), pToolBarItem->m_strAlias );
		m_strArrayItemAlias.Add(strAlias);
	}
}

INT CMarcToolBar::GetToolBarItemID(CString strAlias)
{
	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem->GetHeadPosition();
	CToolBarCtrl &pToolBar = GetToolBarCtrl();
	INT			nID = -1;	

	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem->GetNext(pos));
		if (pToolBarItem == NULL) continue;
				
		nID = pToolBarItem->m_nID;
		if ( pToolBarItem->m_strAlias == strAlias )
		{
			return nID;
		}
		
	}

	return 0;
}

CString CMarcToolBar::GetToolBarItemAlias(INT f_nID)
{	
	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem->GetHeadPosition();
	CToolBarCtrl &pToolBar = GetToolBarCtrl();	
	INT			nID;

	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem->GetNext(pos));
		if (pToolBarItem == NULL) continue;
				
		nID = pToolBarItem->m_nID;
		if ( f_nID == nID )
		{
			return pToolBarItem->m_strAlias;
		}
		
	}

	return _T("");
}

VOID CMarcToolBar::UserSelectProc(UINT nIDC)
{
	m_pParent->m_pFrameWnd->SendMessage(WM_COMMAND, nIDC);
}
