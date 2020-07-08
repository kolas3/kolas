//********************************************************
//                                                            
//	FILENAME	: CBmpMenu.cpp
//	CONTENTS	: Defines the class behaviors for the CBmpMenu class.
//
//  Copyright (c), 1999-2000.					  
//	All rights reserved									  
//                                                        
//  Author(s):  Dipti Deogade
//                                                        
//*********************************************************

//** REVISION LIST ****************************************
//				 										  
// Number	Date		Author		Description
//
//
//*********************************************************

#include "stdafx.h"
#include "CBmpMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpMenu
// constructor

CBmpMenu::CBmpMenu(INT nBitmapW, BOOL bShowBmp4SubMenu, HBITMAP hBitmap, BOOL bStretchBmp)
{
	//check if hbitmap is a valid handle
	BITMAP bitmap;
	if(GetObject(hBitmap, sizeof(bitmap), &bitmap))
	{
		m_hBitmap   = hBitmap;
	}
	else
	{
		m_hBitmap	= NULL;
	}

	//initialize internal data
	m_bShowBmpAll	= bShowBmp4SubMenu;
	m_pOwnerWnd		= m_pToolbar = 0;
	m_bSubMenu		= FALSE;
	m_pSubMenuWnd	= 0;
	m_bStretchBmp	= bStretchBmp;
	m_nTBOffSet		= nBitmapW;

	// KOL.UDF.022 시큐어코딩 보완
	m_bEndModalLoop = FALSE;
}

CBmpMenu::~CBmpMenu()
{
}

IMPLEMENT_DYNAMIC( CBmpMenu, CWnd )

BEGIN_MESSAGE_MAP(CBmpMenu, CWnd)
	//{{AFX_MSG_MAP(CBmpMenu)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBmpMenu message handlers

BOOL
CBmpMenu :: Attach( HMENU hMenu )
{
	return CMenu::Attach(hMenu);
}

HMENU
CBmpMenu :: Detach()
{
	return CMenu::Detach();
}


BOOL CBmpMenu::DestroyWindow() 
{
	//Send WM_EXITMENULOOP to the owner window
	m_pOwnerWnd->SendMessage(WM_EXITMENULOOP, FALSE);

	//destroy the current window and set focus to parent if parent is of the same class
	if(::IsWindow(GetParent()->GetSafeHwnd()) && 
		GetParent()->IsKindOf(RUNTIME_CLASS(CBmpMenu)))
	{
		GetParent()->SetFocus();
	}

	return CWnd::DestroyWindow();
}

BOOL
CBmpMenu :: TrackPopupMenu( UINT nFlags, INT x, INT y, CWnd* pParentWnd, CRect* pItemRect)
{
	//if this is a _tmain menu, then parent window is same as owner window
	if(!m_bSubMenu)
	{
		m_pOwnerWnd = pParentWnd;
	}

	//Send menu messages to owner window
	if(m_pOwnerWnd && IsWindow(m_pOwnerWnd->m_hWnd))
		m_pOwnerWnd->SendMessage(m_bSubMenu?WM_INITMENUPOPUP:WM_INITMENU, (WPARAM)m_hMenu, 0);
	else
		return FALSE;

	//create the _tmain window...child of this will be a toolbar which acts as a kind of menu
	if(!CreateEx(WS_EX_DLGMODALFRAME|WS_EX_PALETTEWINDOW|WS_EX_CONTROLPARENT
					, 0, 0, WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_BORDER|WS_CHILD, 
					CRect(0,0,0,0), pParentWnd, 0))
		return FALSE;

	//set the data of parent popup menu
	if(m_bSubMenu)
	{
		((CBmpMenu*)GetParent())->m_pSubMenuWnd = this;
	}

	//create toolbar
	m_pToolbar = new MenuToolBar;

	//We add a custom draw toolbar which will act as a menu
	m_pToolbar->CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST|TBSTYLE_CUSTOMERASE|TBSTYLE_WRAPABLE,
							WS_CLIPCHILDREN|CCS_NODIVIDER|CCS_NORESIZE|CCS_NOPARENTALIGN|CBRS_TOOLTIPS);

	//Initialize toolbar data and place the menu window on screen
	InitToolBarData(m_pToolbar, CPoint(x,y), pItemRect);

	//show the menu window without activating it....before that make the owner window as foreground window
	if(!m_bSubMenu)
	{
		m_pOwnerWnd->SetForegroundWindow();
	}
	
	ShowWindow(SW_SHOWNA);	

	//capture keyboard input
	SetFocus();

	//if this is a submenu and user had opened it by pressing right arrow key
	if(m_bSubMenu & MENU_SELECTFIRSTITEM)
	{
		m_pToolbar->SendMessage(TB_SETHOTITEM, 0, 0);
	}

	m_bSubMenu &= ~MENU_SELECTFIRSTITEM;

	//send a notification message to owner
	if(m_pOwnerWnd && IsWindow(m_pOwnerWnd->m_hWnd))
		m_pOwnerWnd->SendMessage(WM_ENTERMENULOOP, (WPARAM)m_hMenu, (LPARAM)m_hWnd);

	//run the modal loop so that the menu window acts as a kind of dialog
	RunModalLoop();

	//delete allocated memory and cleanup other stuff
	Cleanup();

	return TRUE;
}
//wp gives a point on which left mouse button was clicked for the parent menu
VOID CBmpMenu::PopupSubMenu(WPARAM wp, BOOL bSelectFirstItem)
{
	CPoint point(LOWORD(wp), HIWORD(wp));

	//Get the button on which left mouse button was clicked
	INT nBtnIndex = (m_pToolbar->GetToolBarCtrl()).HitTest(&point);
	TBBUTTON tbb;
	(m_pToolbar->GetToolBarCtrl()).GetButton(nBtnIndex, &tbb);

	//Get window rectangle of this button
	CRect rect;
	(m_pToolbar->GetToolBarCtrl()).GetItemRect(nBtnIndex, &rect);
	(m_pToolbar->GetToolBarCtrl()).ClientToScreen(&rect);

	//Construct popup menu
	CBmpMenu oSubMenu(m_bShowBmpAll?m_nTBOffSet:0, m_bShowBmpAll, m_bShowBmpAll?m_hBitmap:0, m_bStretchBmp);
	
	//intialize data of popup menu
	oSubMenu.m_pOwnerWnd = m_pOwnerWnd;

	//When user created an instance of CBmpMenu, automatically a handle was associated with root menu
	//when he called loadMenu or CreatePopupMenu. But we are not doing any of it for submenus..so just attach
	//the menu handle of the submenu to this submenu window

	oSubMenu.Attach(((MENUITEMINFO*)(tbb.dwData))->hSubMenu);

	//Set data to indicate whther first item should be set as a hot item
	oSubMenu.m_bSubMenu = TRUE | bSelectFirstItem;

	//Add the blank space with to get bounds rect of this button in root menu
	rect.left -= m_nTBOffSet;

	//Show the submenu window
	oSubMenu.TrackPopupMenu(0, 0, 0, this, &rect);

	//detach the menu handle
	oSubMenu.Detach();
}

VOID CBmpMenu::InitToolBarData(CToolBar *pToolBar, CPoint pt, CRect* pItemRect)
{
	//enable tooltips
	pToolBar->EnableToolTips(TRUE);

	//set toolbar buutons bitmap size first. This should be equal to checkmark dimensions
	//button size will be automatically calculated by toolbar
	pToolBar->SendMessage(TB_SETBITMAPSIZE, 0, MAKELPARAM(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK)));

	//for each menu item...add corrsponding buttons to the toolbar
	for(INT i=0; i<(INT)GetMenuItemCount(); i++)
	{
		MENUITEMINFO	menuInfo, *pMenuInfo;
		TBBUTTON		buttonInfo;
		TCHAR			*pszBuffer;

		//initialize
		menuInfo.cbSize = sizeof(menuInfo);
		menuInfo.fMask = MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE;
		pszBuffer = new TCHAR[MAX_PATH];		//buffer for button text
		menuInfo.cch = MAX_PATH;
		menuInfo.dwTypeData = pszBuffer;
		ZeroMemory(&buttonInfo, sizeof(buttonInfo));

		GetMenuItemInfo(i, &menuInfo, TRUE);
		
		if(menuInfo.dwTypeData == 0)
			// KOL.UDF.022 시큐어코딩 보완
			delete[] pszBuffer;
			//delete pszBuffer;

		//store the app defined data pointer
		pMenuInfo = new MENUITEMINFO;
		*pMenuInfo = menuInfo;
		buttonInfo.dwData = (ULONG)pMenuInfo;

		//set button info from menuInfo...default state is enabled
		buttonInfo.fsState = TBSTATE_ENABLED|TBSTATE_WRAP;

		//check if its a separator
		if(menuInfo.fType & MFT_SEPARATOR)
		{
			buttonInfo.fsStyle = TBSTYLE_SEP;
		}

		//check if the menu item is disabled or grayed
		if((menuInfo.fState & MF_GRAYED) || (MF_DISABLED & menuInfo.fState))
			buttonInfo.fsState = 0;

		//set button command id
		buttonInfo.idCommand = menuInfo.wID;

		//add the button
		(pToolBar->GetToolBarCtrl()).AddButtons(1, &buttonInfo);

		//Set button text
		if(menuInfo.dwTypeData)
		{
			pToolBar->SetButtonText(i, pszBuffer);
		}
	}

	//Get the button width and add width of arrow mark to be drawn for popup menus
	INT nWidth = LOWORD(pToolBar->GetToolBarCtrl().GetButtonSize());
	nWidth += GetSystemMetrics(SM_CXMENUCHECK);
	//add width for showing arrows for submenus
	pToolBar->GetToolBarCtrl().SetButtonWidth(nWidth, nWidth);

	//set the toolbar button size
	CRect rect1, rect2;

	(pToolBar->GetToolBarCtrl()).GetItemRect(0, &rect1);
	//if all the buttons have dropdown arrow...the toolbar returns extra height equal to one row
	(pToolBar->GetToolBarCtrl()).GetItemRect(GetMenuItemCount()-1, &rect2);

	//set the total toolbar size
	//offset of 3 pixels to either side of the toolbar
	//If another control(bitmap or slider) is to be placed by the side of toolbar then we have to take 
	//into account the width of that control=m_nTBOffSet
	//Also we need to check that this window rect lies in the screen rect
	PositionMenuWindow(pt, pItemRect, CRect(0, 0, rect1.Width()+6+m_nTBOffSet, rect2.bottom-rect1.top+6));

	//place the toolbar window in the menu window
	pToolBar->MoveWindow(m_nTBOffSet, 0, rect1.Width()+6, rect2.bottom-rect1.top+6);

	//Show the toolbar window without activating it
	pToolBar->ShowWindow(SW_SHOWNOACTIVATE);
}


LRESULT CBmpMenu::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd*	pWnd = 0;
	BOOL	bFlag = FALSE;

	switch(message)
	{
	case WM_ACTIVATE:
		{
			//Close all menu windows, if another window is being activated because of a mouse click outside menu window
			CWnd oWnd;
			if(lParam && IsWindow(HWND(lParam)) && (LOWORD(wParam) == WA_INACTIVE))
				if(!oWnd.FromHandle(HWND(lParam))->IsKindOf(RUNTIME_CLASS(CBmpMenu)))
					DestroyRootMenu();

			if(!lParam)
				DestroyRootMenu();

			return 0L;
		}
		break;

	case WM_SYSKEYDOWN:
		//destroy root menu when alt or any syskey is pressed
		DestroyRootMenu();
		break;

	case WM_KEYDOWN:
		//pass the keydown event to toolbar
		//If a submenu is being shown for this menu, then key down should be handled by submenu window
		//else pass it to toolbar
		if(m_pSubMenuWnd)
			m_pSubMenuWnd->SendMessage(WM_KEYDOWN, wParam, lParam);
		else
			m_pToolbar->SendMessage(WM_KEYDOWN, wParam, lParam);
		return 0L;

	case WM_RESETDATA:
		//User defined message to be sent to toolbar to reinitialize internal data
		m_pToolbar->SendMessage(WM_RESETDATA, 0, 0);
		return 0L;

	case WM_COMMAND:
		//pass on control specific messages(if any) to owner window
		m_pOwnerWnd->PostMessage(WM_COMMAND, wParam, lParam);
		return 0L;

	case WM_POPUPSUBMENU:
		//popup the submenu
		PopupSubMenu(wParam, lParam);
		return 0L;
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

//destroys all menu window
VOID CBmpMenu::DestroyRootMenu()
{
	if(m_hWnd && IsWindow(m_hWnd))
	{
		CWnd *pParent = GetParent();

		if(pParent && IsWindow(pParent->m_hWnd))
		{
			if(pParent->IsKindOf(RUNTIME_CLASS(CBmpMenu)))
				((CBmpMenu*)pParent)->DestroyRootMenu();	
		}

		if(m_hWnd && IsWindow(m_hWnd))
		{
			DestroySubMenus();

			//end the modal loop started using RunModalLoop in TrackPopupMenu function
			EndModalLoop(0);

			m_pSubMenuWnd = 0;
		}
	}
}

//destroy all the submenu window associated with this menu
VOID CBmpMenu::DestroySubMenus()
{
	if(m_pSubMenuWnd)
	{
		m_pSubMenuWnd->DestroySubMenus();

		//end the modal loop started using RunModalLoop in TrackPopupMenu function
		m_pSubMenuWnd->EndModalLoop(0);

		m_pSubMenuWnd = 0;
	}
}

VOID CBmpMenu::Cleanup()
{
	//_tremove all allocated memory
	if(m_pToolbar && IsWindow(m_pToolbar->m_hWnd))
	{
		for(INT i = 0; i<(m_pToolbar->GetToolBarCtrl()).GetButtonCount(); i++)
		{
			TBBUTTON tbb;
			(m_pToolbar->GetToolBarCtrl()).GetButton(i, &tbb);
			if(tbb.dwData)
			{
				if(((MENUITEMINFO*)(tbb.dwData))->dwTypeData)
					delete (TCHAR*)(((MENUITEMINFO*)(tbb.dwData))->dwTypeData);
				delete (MENUITEMINFO*)(tbb.dwData);
				tbb.dwData = 0;
			}
		}

		//destroy our windows
		m_pToolbar->DestroyWindow();
		delete m_pToolbar;
		m_pToolbar = 0;
	}

	if(IsWindow(m_hWnd))
		DestroyWindow();
}

BOOL CBmpMenu::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bRet = CWnd::PreCreateWindow(cs);

	WNDCLASS wc; 

	::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wc);

	//register new class with same info but different class
	wc.lpszClassName = cs.lpszClass = _T("BitmapMenu");

	AfxRegisterClass(&wc);

	return bRet;
}


VOID CBmpMenu::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	//Fill the blank space with whiteness
	dc.FillSolidRect(&rect, GetSysColor(COLOR_BTNHIGHLIGHT));

	//draw the vertical bitmap if required
	if(m_hBitmap)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(memDC.GetSafeHdc(), m_hBitmap);
		
		BITMAP bitmap;
		GetObject(m_hBitmap, sizeof(bitmap), &bitmap);
		
		//draw the bitmap
		if(m_bStretchBmp)
			dc.StretchBlt(0, 0, m_nTBOffSet, rect.Height(), &memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 
				SRCCOPY);
		else
		{
			//draw using pattern brush
			HBRUSH hPatternBr = CreatePatternBrush(m_hBitmap);
			RECT rect1={0, 0, m_nTBOffSet, rect.Height()};
			FillRect(dc.GetSafeHdc(), &rect1, hPatternBr);
			DeleteObject(hPatternBr);
		}

		//restore objects
		memDC.SelectObject(hOldBmp);
		memDC.DeleteDC();
	}
}

//Used to place the menu window
VOID CBmpMenu::PositionMenuWindow(CPoint pt, CRect* pItemRect, CRect menuRect)
{
	CRect deskRect;

	GetDesktopWindow()->GetWindowRect(&deskRect);

	//Check if this is a submenu...then we need to check either right top or right bottom point of menuRect
	if(pItemRect)
	{
		if(PositionSubMenu(CPoint(pItemRect->right, pItemRect->top), menuRect, TRUE, TRUE) == FALSE)
		{
			if(PositionSubMenu(CPoint(pItemRect->right, pItemRect->bottom), menuRect, TRUE, FALSE) == FALSE)
			{
				if(PositionSubMenu(CPoint(pItemRect->left, pItemRect->top), menuRect, FALSE, TRUE) == FALSE)
				{
					PositionSubMenu(CPoint(pItemRect->left, pItemRect->bottom), menuRect, FALSE, FALSE);
				}	
			}	
		}
		return;
	}

	//we need to check which position is best for showing menu
	//check for left top alignment with pt
	if((pt.x+menuRect.Width() < deskRect.right) &&
		(pt.y+menuRect.Height() < deskRect.bottom))
	{
		MoveWindow(pt.x, pt.y, menuRect.Width(), menuRect.Height());	
	}
	else //right top
	if((pt.x-menuRect.Width() > deskRect.left) && 
		(pt.y+menuRect.Height() < deskRect.bottom))
	{
		MoveWindow(pt.x-menuRect.Width(), pt.y, menuRect.Width(), menuRect.Height());
	}
	else
	//check for left bottom alignment with pt
	if((pt.x+menuRect.Width() < deskRect.right) && 
		(pt.y-menuRect.Height() > deskRect.top))
	{
		MoveWindow(pt.x, pt.y-menuRect.Height(), menuRect.Width(), menuRect.Height());
	}
	else
	//check for right bottom alignment with pt
	if((pt.x-menuRect.Width() > deskRect.left) && 
		(pt.y-menuRect.Height() > deskRect.top))
	{
		MoveWindow(pt.x-menuRect.Width(), pt.y-menuRect.Height(), menuRect.Width(), menuRect.Height());
	}
	else //left top is default
		MoveWindow(pt.x, pt.y, menuRect.Width(), menuRect.Height());
}

BOOL
CBmpMenu::PositionSubMenu(CPoint pt, CRect menuRect, BOOL bRtAlign, BOOL bDnAlign)
{
	CRect deskRect;

	GetDesktopWindow()->GetWindowRect(&deskRect);

	if(bRtAlign && bDnAlign)
	{
		if((pt.x+menuRect.Width() < deskRect.right) &&
			(pt.y+menuRect.Height() < deskRect.bottom))
		{
			MoveWindow(pt.x, pt.y, menuRect.Width(), menuRect.Height());	
		}
		else
		{
			return FALSE;
		}
	}

	if(bRtAlign && ! bDnAlign)
	{
		if((pt.x+menuRect.Width() < deskRect.right) && 
			(pt.y-menuRect.Height() > deskRect.top))
		{
			MoveWindow(pt.x, pt.y-menuRect.Height(), menuRect.Width(), menuRect.Height());
		}
		else
		{
			return FALSE;
		}
	}

	if(!bRtAlign && bDnAlign)
	{
		if((pt.x-menuRect.Width() > deskRect.left) && 
			(pt.y+menuRect.Height() < deskRect.bottom))
		{
			MoveWindow(pt.x-menuRect.Width(), pt.y, menuRect.Width(), menuRect.Height());
		}
		else
		{
			return FALSE;
		}
	}

	if(!bRtAlign && !bDnAlign)
	{
		if((pt.x-menuRect.Width() > deskRect.left) && 
			(pt.y-menuRect.Height() > deskRect.top))
		{
			MoveWindow(pt.x-menuRect.Width(), pt.y-menuRect.Height(), menuRect.Width(), menuRect.Height());
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// MenuToolBar

MenuToolBar::MenuToolBar()
{
	m_nLastLBDownIndex = -1;
	m_nLastHoverIndex  = -1;
	m_oHoverPt.x = -1;
	m_oHoverPt.y = -1;
	m_nSelectedItem = -1;
}

MenuToolBar::~MenuToolBar()
{
	m_oMenuFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(MenuToolBar, CToolBar)
	//{{AFX_MSG_MAP(MenuToolBar)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW , OnCustomDrawNotify)
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LBUTTONDOWNAFTER, OnPostLbuttonMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MenuToolBar message handlers

VOID MenuToolBar::OnCustomDrawNotify(LPARAM lParam, LRESULT* pResult )
{
	LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW) lParam;

	if(!lParam)
		return;

	if(lpNMCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW ;	//we need CDDS_ITEMPREPAINT notifications
		return;
	}

	if(lpNMCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		MENUITEMINFO menuInfo = *(MENUITEMINFO*)(lpNMCustomDraw->nmcd.lItemlParam);
		CRect rcItem = lpNMCustomDraw->nmcd.rc;
		CDC dc;
		CFont* pOldFont;
		dc.Attach(lpNMCustomDraw->nmcd.hdc);

		//Check if this is a hot item
		if(lpNMCustomDraw->nmcd.uItemState & CDIS_HOT)
		{
			lpNMCustomDraw->clrText = GetSysColor(COLOR_HIGHLIGHTTEXT);
			lpNMCustomDraw->clrHighlightHotTrack = GetSysColor(COLOR_HIGHLIGHT);

			//check if last hot item was same as this hot item. If not then we need to send notification
			//to owner window
			if(m_nSelectedItem != (INT)lpNMCustomDraw->nmcd.dwItemSpec)
			{
				INT ndx = lpNMCustomDraw->nmcd.dwItemSpec;

				//for submenus we need to send index of this item rather than Command Id
				if(menuInfo.hSubMenu)
				{
					ndx = CommandToIndex(lpNMCustomDraw->nmcd.dwItemSpec);
					menuInfo.fState = MF_POPUP;
				}

				//Send WM_MENUSELECT notification message to owner window
				((CBmpMenu*)GetParent())->m_pOwnerWnd->SendMessage(WM_MENUSELECT, 
									MAKEWPARAM(ndx, menuInfo.fState), 
									(LPARAM)((CBmpMenu*)GetParent())->m_hMenu);

			}
			//store th hot item index
			m_nSelectedItem = lpNMCustomDraw->nmcd.dwItemSpec;
		}
		else
		{
			lpNMCustomDraw->clrText = GetSysColor(COLOR_MENUTEXT);
			lpNMCustomDraw->clrHighlightHotTrack = GetSysColor(COLOR_MENU);
		}
		
		if(menuInfo.fState & MF_GRAYED)
		{
			lpNMCustomDraw->clrText = GetSysColor(COLOR_GRAYTEXT);
			//this is required for checkmark color
			dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		}
		else
			dc.SetTextColor(GetSysColor(COLOR_MENUTEXT));

		//leave a one pixel gap
		rcItem.left++;

		//draw the background rectangle first
		dc.FillSolidRect(rcItem, lpNMCustomDraw->clrHighlightHotTrack);

		//Select menu font
		pOldFont = dc.SelectObject(&m_oMenuFont);

		//check if we need to draw the checkmark for this menu item
		if(menuInfo.fState & MF_CHECKED)
		{
			CRect CheckRect = rcItem;
			CheckRect.right = CheckRect.left + CheckRect.Height();
			//draw the checkmarked image
			Draw3DCheckmark(dc, CheckRect, lpNMCustomDraw->nmcd.uItemState & CDIS_HOT, 
				menuInfo.hbmpChecked, TRUE, menuInfo.fState & MF_GRAYED);
		}
		else
		{
			//if unchecked bmp is provided
			if(menuInfo.hbmpUnchecked)
			{
				CRect CheckRect = rcItem;
				CheckRect.right = CheckRect.left + CheckRect.Height();
				//draw the UnCheckmarked image
				Draw3DCheckmark(dc, CheckRect, lpNMCustomDraw->nmcd.uItemState & CDIS_HOT, 
					menuInfo.hbmpUnchecked, FALSE, menuInfo.fState & MF_GRAYED);
			}
		}

		//draw the background rectangle for this button and then draw menu text
		dc.SetTextColor(lpNMCustomDraw->clrText);
		dc.SetBkMode(TRANSPARENT);

		//calculate text rectangle. The width of Checkmark has to be added
		rcItem.left += rcItem.Height() + 2;

		//Check if the item is disabled or grayed. Then we need to draw embossed text
		if((menuInfo.fState & MF_GRAYED) && !(lpNMCustomDraw->nmcd.uItemState & CDIS_HOT))
		{
			rcItem.OffsetRect(1,1);
			dc.SetTextColor(GetSysColor(COLOR_3DHILIGHT));
			dc.DrawText(menuInfo.dwTypeData, rcItem, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			dc.SetTextColor(lpNMCustomDraw->clrText);
			rcItem.OffsetRect(-1,-1);
		}

		//draw the text
		dc.DrawText(menuInfo.dwTypeData, rcItem, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		dc.SelectObject(pOldFont);

		//draw the popup arrow mark
		if(menuInfo.hSubMenu)
		{
			rcItem.left = rcItem.right-GetSystemMetrics(SM_CXMENUCHECK)*3/4;
			rcItem.right = rcItem.left + GetSystemMetrics(SM_CXMENUCHECK);
			CRect arrowRect = rcItem;
			arrowRect.top += (rcItem.Height()-GetSystemMetrics(SM_CXMENUCHECK))/2;
			arrowRect.bottom = arrowRect.top + GetSystemMetrics(SM_CXMENUCHECK);

			arrowRect.right -= GetSystemMetrics(SM_CXMENUCHECK)*3/10;
			arrowRect.left += GetSystemMetrics(SM_CXMENUCHECK)*3/10;
			arrowRect.top += GetSystemMetrics(SM_CXMENUCHECK)*3/10;
			arrowRect.bottom -= GetSystemMetrics(SM_CXMENUCHECK)*3/10;

			POINT points[3];
			points[0].x = arrowRect.left;
			points[0].y = arrowRect.top;
			points[1].x = arrowRect.left+arrowRect.Width()/2;
			points[1].y = arrowRect.top+arrowRect.Height()/2;
			points[2].x = points[0].x;
			points[2].y = points[1].y + arrowRect.Height()/2;

			CPen oPen(PS_SOLID, 1, lpNMCustomDraw->clrText), *pOldPen;
			pOldPen=dc.SelectObject(&oPen);
			
			dc.MoveTo(points[0]);
			dc.LineTo(points[1]);
			dc.LineTo(points[2]);
			dc.LineTo(points[0]);

			dc.SelectObject(pOldPen);

			CBrush oBrush(lpNMCustomDraw->clrText), *pOldBrush;
			pOldBrush = dc.SelectObject(&oBrush);
			dc.FloodFill(points[0].x +1, points[0].y +2, lpNMCustomDraw->clrText);
			dc.SelectObject(pOldBrush);
		}

		//detach DC
		dc.Detach();
		*pResult = CDRF_SKIPDEFAULT;
	}
	else
		*pResult = 0;

}

//////////////////
// Draw 3D checkmark
BOOL MenuToolBar::Draw3DCheckmark(CDC& dc, CRect rc,
	BOOL bSelected, HBITMAP hbmCheck, BOOL bDrawSunkenBdr, BOOL bGrayImage)
{
	// get checkmark bitmap if none, use Windows standard
	HBITMAP hbm = hbmCheck;

	if (!hbmCheck) {
		CBitmap bm;
		bm.LoadOEMBitmap(OBM_CHECK);
		hbm = (HBITMAP)bm.Detach();
		if(!hbm)
			return FALSE;
	}
	
	// center bitmap in caller's rectangle
	BITMAP bm;
	::GetObject(hbm, sizeof(bm), &bm);
	INT cx = bm.bmWidth;
	INT cy = bm.bmHeight;
	CRect rcDest = rc;
	CPoint p(0,0);
	CSize delta(CPoint((rc.Width() - cx)/2, (rc.Height() - cy)/2));
	if (rc.Width() > cx)
		rcDest = CRect(rc.TopLeft() + delta, CSize(cx, cy));
	else
		p -= delta;

	//draw background rectangle first
	if(hbmCheck && bDrawSunkenBdr)
		dc.FillSolidRect(rc, GetSysColor(COLOR_MENU));

	// select checkmark into memory DC
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);

	//change the background colors of bitmap
	if(hbmCheck)
	{
		hbm = GetSysColorBitmap(dc.GetSafeHdc(), hbm, bGrayImage, bSelected);
	}

	HBITMAP hOldBM = (HBITMAP)::SelectObject(memdc, hbm);

	// set BG color based on selected state
	COLORREF colorOld = dc.SetBkColor(GetSysColor((bSelected && !bGrayImage)?COLOR_HIGHLIGHT:COLOR_MENU));
	dc.BitBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
		&memdc, p.x, p.y, SRCCOPY);
	dc.SetBkColor(colorOld);

	::SelectObject(memdc, hOldBM); //restore

	//Delete hbm object..it is either created by GetSysColorBitmap function or LoadOEMBitmap
	DeleteObject(hbm);			
	memdc.DeleteDC();

	//Check if we need to draw sunken border
	if(bDrawSunkenBdr)
	{
		dc.DrawEdge(rc, BDR_SUNKENOUTER, BF_RECT);

		//draw background
		CBrush brush;
		CBitmap bmp;
		WORD      Bits[8] = { 0x0055, 0x00aa, 0x0055, 0x00aa,
							 0x0055, 0x00aa, 0x0055, 0x00aa };
		bmp.CreateBitmap( 8, 8, 1, 1, &Bits );
		//hatched background
		brush.CreatePatternBrush(&bmp);

		//if item is selected, then we draw plain background
		CBrush brush1(GetSysColor(COLOR_MENU));

		CBrush* oldBr = dc.SelectObject(bSelected ? &brush1 : &brush);

		//the text color is used by pattern brush
		dc.SetTextColor(GetSysColor(COLOR_3DHIGHLIGHT));
		dc.SetBkColor(GetSysColor(COLOR_MENU));

		if(!hbmCheck)
			dc.ExtFloodFill(rc.left+1, rc.top+1, GetSysColor(bSelected?COLOR_HIGHLIGHT:COLOR_MENU), FLOODFILLSURFACE);
		else
			dc.ExtFloodFill(rc.left+1, rc.top+1, GetSysColor(COLOR_MENU), FLOODFILLSURFACE);

		//cleanup
		dc.SelectObject(oldBr);
		brush.DeleteObject();
		brush1.DeleteObject();
		bmp.DeleteObject();
	}

	return TRUE;
}

INT MenuToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style &= ~WS_VISIBLE;
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	NONCLIENTMETRICS nc;
	ZeroMemory(&nc, sizeof(nc));
	nc.cbSize = sizeof(nc);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nc, 0);
	m_oMenuFont.CreateFontIndirect(&nc.lfMenuFont);
	SetFont(&m_oMenuFont);

	return 0;
}

VOID MenuToolBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	INT nBtnIndex = GetToolBarCtrl().HitTest(&point);

	if(nBtnIndex < 0 ) return;// fix here :) the same with on_mouse move :)

	TBBUTTON tbb;
	GetToolBarCtrl().GetButton(nBtnIndex, &tbb);

	BOOL bRet = (((MENUITEMINFO*)(tbb.dwData))->fState & MF_GRAYED) || 
					(((MENUITEMINFO*)(tbb.dwData))->fState & MF_DISABLED);

	if(bRet)
		return;


	if(m_nLastLBDownIndex != nBtnIndex)
	{
		//If already another popup menu was opened, then we need to close that window
		((CBmpMenu*)GetParent())->DestroySubMenus();

		PostMessage(WM_LBUTTONDOWNAFTER, nFlags, MAKELPARAM(point.x, point.y));

		m_nLastLBDownIndex = nBtnIndex;
	}


}

VOID MenuToolBar::OnPostLbuttonMsg(UINT nFlags, LPARAM lp)
{
	//claculate the button index
	CPoint point(LOWORD(lp), HIWORD(lp));

	INT nBtnIndex = GetToolBarCtrl().HitTest(&point);
	TBBUTTON tbb;
	GetToolBarCtrl().GetButton(nBtnIndex, &tbb);

	//if the click was on a button corresponding to popup item, then we need to open up popup menu 
	//and don't pass the message up windows hirarchy.
	if(tbb.dwData && ((MENUITEMINFO*)(tbb.dwData))->hSubMenu)
	{
		((CBmpMenu*)GetParent())->SendMessage(WM_POPUPSUBMENU, lp, (MENU_SELECTFIRSTITEM & nFlags)?MENU_SELECTFIRSTITEM:0);
		return;
	}

	//If this is a valid button and no submenu then send wm_command message to owner and close all menus
	((CBmpMenu*)GetParent())->m_pOwnerWnd->PostMessage(WM_COMMAND, MAKEWPARAM(tbb.idCommand, 0), 0);
	((CBmpMenu*)GetParent())->DestroyRootMenu();
}


VOID MenuToolBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	INT nBtnIndex = GetToolBarCtrl().HitTest(&point);

    //if not on button...then we simply return
    if (nBtnIndex < 0)
        return;

	//skip mouse move message if this button is disabled or grayed or inactive
	TBBUTTON tbb;
	GetToolBarCtrl().GetButton(nBtnIndex, &tbb);

	//if this button is disabled or grayed down...then we simply return
	BOOL bRet = (((MENUITEMINFO*)(tbb.dwData))->fState & MF_GRAYED) || 
					(((MENUITEMINFO*)(tbb.dwData))->fState & MF_DISABLED);

	if(bRet)
		return;

	//skip WM_MOUSEMOVE message if the last mouse position is same as curent one.
	static CPoint pt(0,0);

	if(point != pt)
		pt = point;
	else
		return;

	if(m_nLastHoverIndex != nBtnIndex)
	{
		m_nLastHoverIndex = nBtnIndex;
		m_oHoverPt = point;

		//Hovetrack only for submenu items
		TRACKMOUSEEVENT tme = {sizeof(tme), TME_HOVER, m_hWnd, 0};

		_TrackMouseEvent(&tme);
	}
	CToolBar::OnMouseMove(nFlags, point);

}

LRESULT MenuToolBar::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Garb WM_MOUSEHOVER message
	if(message == WM_MOUSEHOVER)
	{
		if(m_nLastHoverIndex >= 0)
		{
			TBBUTTON tbb;
			GetToolBarCtrl().GetButton(m_nLastHoverIndex, &tbb);

			if(tbb.dwData)
			{
				//pass on left mouse down event for submenu items
				if(((MENUITEMINFO*)(tbb.dwData))->hSubMenu)
				{
					SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(m_oHoverPt.x, m_oHoverPt.y));		
					SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(m_oHoverPt.x, m_oHoverPt.y));	
				}
				else //delete any submenus present for this menu
				{
					((CBmpMenu*)GetParent())->DestroySubMenus();
					m_nLastLBDownIndex = -1; //reset after destroying submenus
				}
			}
		}
	}

	if(message == WM_RESETDATA)
		m_nLastLBDownIndex = -1;

	return CToolBar::DefWindowProc(message, wParam, lParam);
}

//Grab Key down events
BOOL MenuToolBar::KeyboardFilter(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//On escape key, we close all menu windows
	if(nChar==VK_ESCAPE)
	{
		((CBmpMenu*)GetParent())->DestroyRootMenu();
		return TRUE;
	}

	//On left arrow...we need to close the current submenu and go to parent
	if(nChar == VK_LEFT)
	{
		CWnd *pWnd = GetParent()->GetParent();

		if(pWnd && IsWindow(pWnd->m_hWnd) && pWnd->IsKindOf(RUNTIME_CLASS(CBmpMenu)))
		{
			((CBmpMenu*)pWnd)->DestroySubMenus();

			(CBmpMenu*)pWnd->SendMessage(WM_RESETDATA, 0, 0);
		}
		return TRUE;
	}

	if((nChar == VK_RIGHT) || (nChar == VK_RETURN))
	{	
		if(m_nSelectedItem != -1)
		{
			INT ndx = CommandToIndex(m_nSelectedItem);

			if(ndx >= 0)
			{
				TBBUTTON tbb;
				GetToolBarCtrl().GetButton(ndx, &tbb);

				CRect rect;
				GetToolBarCtrl().GetItemRect(ndx, &rect);

				if(tbb.dwData)
				{
					//pass on left mouse down event for submenu items..also need to select the first submenu item
					if((((MENUITEMINFO*)(tbb.dwData))->hSubMenu) || (nChar == VK_RETURN))
					{
						m_nLastLBDownIndex = -1;
						SendMessage(WM_LBUTTONDOWN, MK_LBUTTON|MENU_SELECTFIRSTITEM, MAKELPARAM(rect.left+1, rect.top+1));		
						SendMessage(WM_LBUTTONUP, MK_LBUTTON|MENU_SELECTFIRSTITEM, MAKELPARAM(rect.left+1, rect.top+1));
					}
				}
			}
		}

		return TRUE;
	}

	//pass on for further processing
	OnChar(nChar, nRepCnt, nFlags);

	return FALSE;
}

VOID MenuToolBar::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(KeyboardFilter(nChar,nRepCnt,nFlags)) 
		return;

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// User typed a TCHAR into menu. Look for item with & preceeding the TCHAR typed.
VOID MenuToolBar::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//////////////////
	UINT iCurrentItem = (UINT)-1; // guaranteed higher than any command ID
	CUIntArray arItemsMatched;		// items that match the character typed

	UINT nItem = GetToolBarCtrl().GetButtonCount();
	
	for (UINT i=0; i< nItem; i++) 
	{
		// get menu info
		TBBUTTON tbb;
		GetToolBarCtrl().GetButton(i, &tbb);

		if(tbb.dwData)
		{
			if(((MENUITEMINFO*)(tbb.dwData))->dwTypeData)
			{
				CString text = ((MENUITEMINFO*)(tbb.dwData))->dwTypeData;
				INT iAmpersand = text.Find('&');
				if (iAmpersand >=0 && _totupper(nChar)==_totupper(text[iAmpersand+1]))
					arItemsMatched.Add(i);
			}
			if ((UINT)SendMessage(TB_GETHOTITEM, 0, 0) == i)
				iCurrentItem = i; // note index of current item
		}
	}
	// arItemsMatched now contains indexes of items that match the TCHAR typed.
	UINT nFound = arItemsMatched.GetSize();
	if (nFound == 0)
	{
		//notify owner window and take corresponding action
		UINT lRet = (((CBmpMenu*)GetParent())->m_pOwnerWnd)->SendMessage(WM_MENUCHAR, MAKEWPARAM(nChar, MF_POPUP), 
													(LPARAM)((CBmpMenu*)GetParent())->m_hMenu);

		switch(HIWORD(lRet))
		{
		case MNC_CLOSE:
			((CBmpMenu*)GetParent())->DestroyRootMenu();
			break;

		case MNC_SELECT:
			//select the menu item
			break;

		case MNC_EXECUTE:
			(((CBmpMenu*)GetParent())->m_pOwnerWnd)->SendMessage(WM_COMMAND, LOWORD(lRet), 0);			
			break;
		}

		return;
	}

	else if (nFound==1)
	{
		CRect rect;
		GetToolBarCtrl().GetItemRect(arItemsMatched[0], &rect);
		SendMessage(TB_SETHOTITEM, arItemsMatched[0], 0);
		m_nLastLBDownIndex = -1;
		SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.left+1, rect.top+1));		
		SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(rect.left+1, rect.top+1));
		return;
	}

	// more than one found--return 1st one past current selected item;
	UINT iSelect = 0;
	for (i=0; i < nFound; i++) {
		if (arItemsMatched[i] > iCurrentItem) {
			iSelect = i;
			break;
		}
	}

	SendMessage(TB_SETHOTITEM, iSelect, 0);
}

HBITMAP 
GetSysColorBitmap(HDC hDC, HBITMAP hSourceBitmap, BOOL bMono, BOOL bSelected)
{
	struct _COLORMAPTABLE{
		COLORREF	rgb;
		INT			nSysColor;
	};

	_COLORMAPTABLE _ColorMap[]=
	{
		// mapping from color in DIB to _tsystem color
		{ RGB(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
		{ RGB(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark gray
		{ RGB(0xC0, 0xC0, 0xC0),  COLOR_MENU },       // bright gray
		{ RGB(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
	};

	HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap;
	HDC hMemSrc, hMemDest;
	INT height, width;
	INT i, j;
	BITMAP SrcBitmap;

	if(bSelected && !bMono)
		_ColorMap[2].nSysColor= COLOR_HIGHLIGHT;
	else
		_ColorMap[2].nSysColor= COLOR_MENU;

	// Step 1: Create a memory DC for the source and destination bitmaps
	//         compatible with the device used.

	hMemSrc = CreateCompatibleDC(hDC);
	hMemDest= CreateCompatibleDC(hDC);


	// Step 2: Get the height and width of the source bitmap.

	GetObject(hSourceBitmap, sizeof(BITMAP), (LPCTSTR)&SrcBitmap);
	width = SrcBitmap.bmWidth;
	height = SrcBitmap.bmHeight;


	// Step 3: Select the source bitmap into the source DC. Create a
	//         destination bitmap, and select it into the destination DC.

	hOldSourceBitmap = (HBITMAP)SelectObject(hMemSrc, hSourceBitmap);
	hDestBitmap = CreateBitmap(height, width, (bMono)?1:SrcBitmap.bmPlanes,
		(bMono)?1:SrcBitmap.bmBitsPixel, NULL);

	if (hDestBitmap)
	{
		hOldDestBitmap = (HBITMAP)SelectObject(hMemDest, hDestBitmap);

		// Step 4: Copy the pixels from the source to the destination.

		for (i = 0; i < width; ++i)
		{
			for (j = 0; j < height; ++j)
			{
				//Get the color of source bitmap
				COLORREF rgb = GetPixel(hMemSrc, i, j);
				
				SetPixel(hMemDest, i, j, rgb);

				if(!bMono)
				{
					//check if we need to change this color
					for(INT k=0; k<sizeof(_ColorMap)/sizeof(_ColorMap[0]); k++)
					{
						if(rgb == _ColorMap[k].rgb)
						{
							SetPixel(hMemDest, i, j, GetSysColor(_ColorMap[k].nSysColor));
							break;
						}
					}
				}
			}
		}
	}


	// Step 5: Destroy the DCs.

	SelectObject(hMemSrc, hOldSourceBitmap);
	SelectObject(hMemDest, hOldDestBitmap);
	DeleteDC(hMemDest);
	DeleteDC(hMemSrc);


	// Step 6: Return the rotated bitmap.

	return(hDestBitmap);

}


INT CBmpMenu::RunModalLoop(DWORD dwFlags)
{
	MSG msg; 
	m_bEndModalLoop = false; 
	while (m_bEndModalLoop==false) { 
		while ( PeekMessage (&msg, 0, 0, 0, PM_REMOVE) ) { 
			TranslateMessage (&msg) ; 
			DispatchMessage (&msg) ; 
		} 
	} 
	return 0; 
}

VOID CBmpMenu::EndModalLoop(INT)
{
	m_bEndModalLoop = true; 
	::ShowWindow(m_hWnd, SW_HIDE); 
}
