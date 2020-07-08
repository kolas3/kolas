/////////////////////////////////////////////////////////////////////////////
// Check List Box for 32 bit
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChkListbox.h"

////////////////////////////////////////////////////////////////////////////
// ChkLstbx
/////////////////////////////////////////////////////////////////////////////
CCheckControl::CCheckControl()
{
	m_nTop = 0;
	m_nLeft = 0;
	m_pPrevSelItem = NULL;
	m_pParent = NULL;
}

CCheckControl::~CCheckControl()
{
}

BEGIN_MESSAGE_MAP(CCheckControl, CWnd)
	//{{AFX_MSG_MAP(CCheckControl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckControl message handlers
BOOL CCheckControl::Create( CRect Rect, CCheckList* pParent, UINT nID, CWnd* pSuperParent, CPtrList* pRootPtrList, COLORREF crBkColor )
{
    // Gets window colors, brushes
	HCURSOR hCursor 		= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	HBRUSH	Brush			= CreateSolidBrush(crBkColor);
 	CString	csWndClass 		= AfxRegisterWndClass( CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW, hCursor, Brush );

	// Adjust the Rect for the border
	Rect.top 	+= 2;
	Rect.left 	+= 2;
	Rect.bottom -= 2;
	Rect.right 	-= 2;
	
	// Create the Contol
	if( !CWnd::Create( csWndClass, _T("Check Control"), WS_CHILD|WS_VISIBLE, Rect, pParent, nID ))
    	return FALSE;

	// Save the window Rect
	GetClientRect(m_WindowRect);
	m_nViewableRows = m_WindowRect.bottom / pParent->GetLineHeight();

	// Save the Parent
	m_pParent = pParent;
	m_pSuperParent = pSuperParent;
	m_pRootPtrList = pRootPtrList;
 	return TRUE;
}

VOID CCheckControl::OnSize(UINT nType, INT cx, INT cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	GetClientRect(m_WindowRect);
	
	if(m_pParent)
	{
		m_nViewableRows = m_WindowRect.bottom / m_pParent->GetLineHeight();
		UpdateScrollBar();
	}
}

VOID CCheckControl::UpdateScrollBar()
{
	// Update the H scrollbar
	GetClientRect(m_WindowRect);

	// Do we need a scroll bar?
	if( m_pParent->GetCount() > m_nViewableRows )
	{
		// Sets the scrollbar range up
		ShowScrollBar(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, m_pParent->GetCount() - m_nViewableRows, TRUE);
		SetScrollPos(SB_VERT, m_nTop, TRUE);
	}
	else
	{
		// Sets the scrollbar range up
		ShowScrollBar(SB_VERT, FALSE);
	}
	
	if( m_pParent->GetWidestItem() > m_WindowRect.right )
	{
		// Sets the scrollbar range up
		ShowScrollBar(SB_HORZ, TRUE);
		SetScrollRange(SB_HORZ, 0, m_pParent->GetWidestItem() - m_WindowRect.right, TRUE);
		SetScrollPos(SB_HORZ, m_nLeft, TRUE);
	}
	else
	{
		// Sets the scrollbar range up
		ShowScrollBar(SB_HORZ, FALSE);
	}		
}

VOID CCheckControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Reposition the m_nTop
	switch( nSBCode )
	{
		case	SB_LINELEFT:		
			 	if( m_nLeft > 0 )		 	
				{
					m_nLeft -= HSCROLLAMOUNT;
					SetScrollPos(SB_HORZ, m_nLeft, TRUE);
					ScrollWindow( HSCROLLAMOUNT, 0 );			
				}						
			 	break;		
		
		case 	SB_LINERIGHT:		
				if( m_nLeft < m_pParent->GetWidestItem() - m_WindowRect.right )			
				{
					m_nLeft += HSCROLLAMOUNT;
					SetScrollPos(SB_HORZ, m_nLeft, TRUE);
					ScrollWindow( -HSCROLLAMOUNT, 0);			
				}			
				break;		

		case	SB_PAGELEFT:		
				if( m_nLeft == 0 )
					return;
				if( m_nLeft < m_WindowRect.right )
				{
					// Only scroll for remaining rows
					ScrollWindow( m_nLeft, 0 );
					m_nLeft = 0;
				}
				else
				{
				    // Scroll for the total amount of viewable rows
					m_nLeft -= m_WindowRect.right - 1;
					ScrollWindow( m_nLeft * HSCROLLAMOUNT, 0 );
				}
				SetScrollPos(SB_HORZ, m_nLeft, TRUE);
				break;		
		
		case	SB_PAGERIGHT:
				if( m_pParent->GetWidestItem() - m_WindowRect.right - m_nLeft < m_WindowRect.right  )
				{
					// Only scroll for remaining rows
					m_nLeft += m_pParent->GetWidestItem() - m_WindowRect.right - m_nLeft;
				}
				else
				{
				    // Scroll for the total amount of viewable rows
					m_nLeft += m_WindowRect.right - 1;
				}
				ScrollWindow( m_nLeft * -HSCROLLAMOUNT, 0 );
				SetScrollPos(SB_HORZ, m_nLeft, TRUE);
				break;

		case	SB_THUMBTRACK:
				INT nScrollBy;
				if( (INT)nPos == m_nLeft )
				{
					return;
				}
				else
				{
					nScrollBy = (INT)nPos - m_nLeft;				
				}
				m_nLeft += nScrollBy;
				SetScrollPos(SB_HORZ, m_nLeft, TRUE);
				ScrollWindow( -nScrollBy, 0 );				
				break;
	}				
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

VOID CCheckControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Reposition the m_nTop
	switch( nSBCode )
	{
		case	SB_LINEUP:		
			 	if( m_nTop > 0 )		 	
				{
					m_nTop--;
					SetScrollPos(SB_VERT, m_nTop, TRUE);
					ScrollWindow( 0, m_pParent->GetLineHeight() );			
				}						
			 	break;		
		
		case 	SB_LINEDOWN:		
				if( m_nTop < m_pParent->GetCount() - m_nViewableRows )			
				{
					m_nTop++;
					SetScrollPos(SB_VERT, m_nTop, TRUE);
					ScrollWindow( 0, -m_pParent->GetLineHeight());			
				}			
				break;		

		case	SB_PAGEDOWN:		
				if( m_nTop + m_nViewableRows == m_pParent->GetCount())
					return;
				if( m_pParent->GetCount() - m_nViewableRows  - m_nTop < m_nViewableRows  )
				{
					// Only scroll for remaining rows
					m_nTop += m_pParent->GetCount() - m_nViewableRows - m_nTop;
					ScrollWindow( 0, m_nTop * -m_pParent->GetLineHeight() );
				}
				else
				{
				    // Scroll for the total amount of viewable rows
					m_nTop += m_nViewableRows - 1;
					ScrollWindow( 0, m_nTop * -m_pParent->GetLineHeight() );
				}
				SetScrollPos(SB_VERT, m_nTop, TRUE);
				break;

		case	SB_PAGEUP:
				if( m_nTop == 0 )
					return;
				if( m_nTop < m_nViewableRows )
				{
					// Only scroll for remaining rows
					ScrollWindow( 0, m_nTop * m_pParent->GetLineHeight() );
					m_nTop = 0;
				}
				else
				{
				    // Scroll for the total amount of viewable rows
					m_nTop -= m_nViewableRows - 1;
					ScrollWindow( 0, m_nTop * m_pParent->GetLineHeight() );
				}
				SetScrollPos(SB_VERT, m_nTop, TRUE);
				break;		

		case	SB_THUMBTRACK:
				INT nScrollBy;
				if( (INT)nPos == m_nTop )
				{
					return;
				}
				else
				{
					nScrollBy = (INT)nPos - m_nTop;				
				}
				m_nTop += nScrollBy;
				SetScrollPos(SB_VERT, m_nTop, TRUE);
				ScrollWindow( 0, -m_pParent->GetLineHeight() * nScrollBy );				
				break;
	}				
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

VOID CCheckControl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Starting positions
	m_nCurrentRow = 0;
	m_nScreenRow = 0;

	// Calculate the item clicked on
	INT nItem = point.y / m_pParent->GetLineHeight() + m_nTop;
	LISTITEM* pItem = GetItemClickedOn( m_pRootPtrList, nItem + 1 );
	
	// Is this item out of view (if so then scroll window)
	if( nItem >= m_nTop + m_nViewableRows )
	{
		m_nTop++;
		ScrollWindow( 0, -m_pParent->GetLineHeight() );			
		SetScrollPos(SB_VERT, m_nTop, TRUE);
	}
	
	// Did we find the item?
	if( pItem )
	{
		// Set the screen item 
		m_nScreenRow = nItem - m_nTop;		
		CDC* pDC = GetWindowDC();

		// Unselect the previous one
		INT nTempRow = m_nScreenRow;
		if( m_pPrevSelItem )
		{
			// Is this item on the screen? (Otherwise don't waste the time
			m_pPrevSelItem->bSelected = false;
			if( m_nPrevSelRow >= m_nTop && m_nPrevSelRow < m_nTop + m_nViewableRows )
			{
				// Draw this item unselected
				m_nScreenRow = m_nPrevSelRow - m_nTop;
				DrawItem(pDC, m_pPrevSelItem);
				m_nScreenRow = nTempRow;
			}
		}

		// Save the this item as the previous item
		m_pPrevSelItem	= pItem;
		m_nPrevSelRow	= m_nTop + m_nScreenRow;
		
		// Select this item
		pItem->bSelected = true;
		
		// Did they click on the bitmap?
		CRect ImageRect;
		ImageRect.left  = ( (m_pParent->GetImageWidth() * (pItem->nLevel - 1)) + PADDING );
		ImageRect.right = ImageRect.left + m_pParent->GetImageWidth();
		ImageRect.top	= (m_nScreenRow * m_pParent->GetImageHeight()) + PADDING;
		ImageRect.bottom= ImageRect.top * m_pParent->GetImageHeight();
		
		// Common calls
		if( ImageRect.PtInRect(point))
		{
			// Select this item
			if( pItem->nCheckedState == UNCHECKED )
				pItem->nCheckedState = CHECKED;
			else
				pItem->nCheckedState = UNCHECKED;

			// Draw This Item
			DrawItem(pDC, pItem);

			// Draw all children items
			INT nOldRowPos = m_nScreenRow++;
			DrawItems(pDC, &pItem->m_ItemList, true, pItem->nCheckedState );

			// Change all parent items if nessary
			m_nScreenRow = nOldRowPos;
			ChangeParentItems(pDC, pItem);

			ReleaseDC(pDC);
		}
		else
		{
			// Draw This Item
			DrawItem(pDC, pItem);
		}
		ReleaseDC(pDC);
	}

	CWnd::OnLButtonDown(nFlags, point);
}
		
VOID CCheckControl::ChangeParentItems(CDC* pDC, LISTITEM* pItem)
{
	// Get this items parent
	if(pItem->pParentItem == NULL)
		return;

	// Set the items
	bool bCheckedItem = false;
	bool bUnCheckedItem = false;
	LISTITEM* pSubItem;

	// Are all these item checked
	INT nMoveUpAmount = 1;
	bool bIncreaseMoveUpAmount = true;
	POSITION Pos = pItem->pParentItem->m_ItemList.GetHeadPosition();
	while(Pos)
	{
		// Find a selected Item
		pSubItem = (LISTITEM*)pItem->pParentItem->m_ItemList.GetNext(Pos);
		if( pSubItem->nCheckedState == CHECKED || pSubItem->nCheckedState == INDETERMINATE)
			bCheckedItem = true;
		else if( pSubItem->nCheckedState == UNCHECKED )
			bUnCheckedItem = true;

		// Was this the orginal item sent in?
		if( pItem == pSubItem )
			bIncreaseMoveUpAmount = false;

		// Are we still moving up a row?
		if( bIncreaseMoveUpAmount )
			nMoveUpAmount++;
	}

	// What should we set it too?
	if(bCheckedItem && !bUnCheckedItem)
	{
		// All Items are checked
		m_nScreenRow -= nMoveUpAmount;
		pItem->pParentItem->nCheckedState = CHECKED;
		DrawItem(pDC, pItem->pParentItem);
	}
	else if(!bCheckedItem && bUnCheckedItem)
	{
		// All Items are Unchecked
		m_nScreenRow -= nMoveUpAmount;
		pItem->pParentItem->nCheckedState = UNCHECKED;
		DrawItem(pDC, pItem->pParentItem);
	}
	else
	{
		// Combination
		m_nScreenRow -= nMoveUpAmount;
		pItem->pParentItem->nCheckedState = INDETERMINATE;
		DrawItem(pDC, pItem->pParentItem);
	}

	// Do we call this again?
	if(pItem->pParentItem->pParentItem != NULL)
		ChangeParentItems(pDC, pItem->pParentItem);
}

LISTITEM* CCheckControl::GetItemClickedOn( CPtrList* pItemList, INT nItem )
{
	// loop for all items
	LISTITEM*  pListItem = NULL;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos)
	{
		// Get this item
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);
		m_nCurrentRow++;

		// Is this the item
		if( m_nCurrentRow == nItem)
			return pListItem;
		
		// Are there any open items in this?
		if( pListItem->m_ItemList.GetCount() != 0 )
		{
			pListItem = GetItemClickedOn( &pListItem->m_ItemList, nItem);
			if( pListItem )
				return pListItem;
		}
	}

	return NULL;
}

VOID CCheckControl::SetTopIndex(INT nTop)
{
	m_nTop = nTop;
	UpdateScrollBar();
	RedrawWindow();	
}

INT CCheckControl::SetCurSel(INT nSelection)
{
	// Is this for no selection? SetCurSel
	if( nSelection == -1 )
	{
		m_pPrevSelItem	= NULL;
		m_nPrevSelRow	= -1;
		return 0;
	}

	// Get the item they have selected
	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItemClickedOn( m_pRootPtrList, nSelection + 1 );
	if( pItem == NULL )
		return LB_ERR;
	
	// Unpaint the last selection
	m_nScreenRow = nSelection - m_nTop;		
	CDC* pDC = GetWindowDC();

	// Unselect the previous one
	INT nTempRow = m_nScreenRow;
	if( m_pPrevSelItem )
	{
		// Is this item on the screen? (Otherwise don't waste the time
		m_pPrevSelItem->bSelected = false;
		if( m_nPrevSelRow >= m_nTop && m_nPrevSelRow < m_nTop + m_nViewableRows )
		{
			// Draw this item unselected
			m_nScreenRow = m_nPrevSelRow - m_nTop;
			DrawItem(pDC, m_pPrevSelItem);
			m_nScreenRow = nTempRow;
		}
	}

	// Set the new selected item
	m_pPrevSelItem	= pItem;
	m_nPrevSelRow	= m_nTop + m_nScreenRow;
	pItem->bSelected = true;
	
	// Should we spend the time redraw this item (NO-if it can't be seen)
	if(nSelection >= m_nTop && nSelection <= m_nTop + m_nViewableRows)
		DrawItem(pDC, pItem);
	ReleaseDC(pDC);
	
	return 0;
}

INT CCheckControl::SetCheck(INT nItem, INT nCheckState)
{
	// Get the item they have selected
	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItemClickedOn( m_pRootPtrList, nItem + 1 );
	if( pItem == NULL )
		return LB_ERR;
	
	// Set the attribute
	m_nScreenRow = nItem - m_nTop;		
	CDC* pDC = GetWindowDC();
	pItem->nCheckedState = nCheckState;
	
	// Should we spend the time redraw this item (NO-if it can't be seen)
	if(nItem >= m_nTop && nItem <= m_nTop + m_nViewableRows)
		DrawItem(pDC, pItem);

	// Update the parents
	ChangeParentItems(pDC, pItem);
	ReleaseDC(pDC);
	
	return 0;
}

VOID CCheckControl::OnPaint() 
{
	INT	nSize = m_pRootPtrList->GetCount();
	CPaintDC dc(this); // device context for painting
				
	// Starting positions
	m_nCurrentRow = 0;
	m_nScreenRow = 0;
	
	// Draw the items
	DrawItems(&dc, m_pRootPtrList);
}

bool CCheckControl::DrawItems(CDC* pDC, CPtrList* pItemList, bool bChangeCheckedState, INT nCheckedState)
{
	// loop for all items
	bool bKeepPainting = true;
	LISTITEM*  pListItem;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos && bKeepPainting)
	{
		// Get this item (is it open?
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);
		
		// Should we change the checked state?
		if( bChangeCheckedState )
			pListItem->nCheckedState = nCheckedState;

		// Draw this item
		if( m_nCurrentRow >= m_nTop )
		{
			DrawItem( pDC, pListItem );
			m_nScreenRow++;
		}
		m_nCurrentRow++;  //Increment the row encountered

    	// Only draw the maximum to fit on the screen
		if( m_nScreenRow > m_nViewableRows )
			bKeepPainting = false;

		// Are there any open items in this?
		if( pListItem->m_ItemList.GetCount() != 0 && bKeepPainting)
			bKeepPainting = DrawItems( pDC, &pListItem->m_ItemList, bChangeCheckedState, nCheckedState);
	}
	return bKeepPainting;
}

VOID CCheckControl::DrawItem(CDC* pDC, LISTITEM* pListItem)
{
	CPen*	pOldPen		= pDC->SelectObject(m_pParent->GetBkPen());
	CFont*	pOldFont	= pDC->SelectObject(m_pParent->GetTextFont());
	CBrush*	pOldBrush	= pDC->SelectObject(m_pParent->GetBkBrush());
	
	// Positioning
	INT nYPos = (m_nScreenRow * m_pParent->GetLineHeight()) + PADDING;

	// Draw Rectangle
	CRect TextRect( -m_nLeft, nYPos, m_WindowRect.Width(), nYPos + m_pParent->GetLineHeight() );
	pDC->Rectangle( TextRect );
	
	// Draw the image (Calculate the center position)
	TextRect.left += (pListItem->nLevel - 1) * m_pParent->GetImageWidth() + PADDING;
	CSize ImagePos( TextRect.left, nYPos );
	if( m_pParent->GetLineHeight() > m_pParent->GetImageHeight() )
		ImagePos.cy = nYPos + (m_pParent->GetLineHeight()  - m_pParent->GetImageHeight() ) / 2;
	DrawBitmap( pDC, ImagePos, pListItem->nCheckedState );
	
	// Is this a selected item
	TextRect.left += m_pParent->GetImageWidth();
	pDC->SetTextColor( pListItem->crTextColor );		
	if( pListItem->bSelected )
	{
		// Draw Highlight
		CRect FocusRect = TextRect;
		pDC->DrawText( pListItem->csText, pListItem->csText.GetLength(), FocusRect, DT_CALCRECT|DT_LEFT|DT_VCENTER );
		
		// Is this past the window
		if( FocusRect.right > m_WindowRect.right )
			FocusRect.right = m_WindowRect.right - 4;
		FocusRect.right += 5; FocusRect.left -= 4;
		pDC->SetTextColor( RGB(0,0,0) );
		pDC->DrawFocusRect( FocusRect );		
	
		// Set highlight color
		pDC->SetTextColor( pListItem->crTextHighColor );		
		
		// Draw the selected background
		CBrush	HighlightBgBrush(pListItem->crBgHighlightColor);
		FocusRect.InflateRect(-1, -1 );
		pDC->FillRect(FocusRect, &HighlightBgBrush);
	}

	// Draw the Text
	INT		nOldBkMode	= pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( pListItem->csText, pListItem->csText.GetLength(), TextRect, DT_LEFT|DT_VCENTER );		
	
	// Set back the old stuff
	pDC->SetBkMode( nOldBkMode );
	pDC->SelectObject( pOldFont );
	pDC->SelectObject( pOldBrush );
	pDC->SelectObject( pOldPen );
}

VOID CCheckControl::DrawBitmap( CDC* pdc, CSize ImagePos, INT nCheckedState )
{
	CDC			dcBM;
	BITMAP 		bm;
	CBitmap* 	pOldBM;
	CBitmap* 	pBitmap;
    
	// Obtain the Bitmap to draw
	if( nCheckedState  == CHECKED )
		pBitmap = m_pParent->GetCheckImage();
	else if( nCheckedState == UNCHECKED )
		pBitmap = m_pParent->GetUnCheckImage();
	else
		pBitmap = m_pParent->GetMiddleImage();

	// Create a compatible DC
	dcBM.CreateCompatibleDC(pdc);

	// Draw the Bitmap
	pBitmap->GetObject(sizeof(bm), &bm );
	pOldBM = dcBM.SelectObject(pBitmap);
	pdc->BitBlt( ImagePos.cx, ImagePos.cy, bm.bmWidth, bm.bmHeight, &dcBM, 0,0, SRCCOPY);

	// Select the old object
	pdc->SelectObject(pOldBM);
}

////////////////////////////////////////////////////////////////////////////////
// Main window
////////////////////////////////////////////////////////////////////////////////
CCheckList::CCheckList()
{
	m_pCheckControl = NULL;
	m_pBkPen		= NULL;
	m_pBkBrush		= NULL;
	m_pCheck		= NULL;
	m_pUnCheck		= NULL;
	m_pMiddleCheck	= NULL;
	m_bDeleteFont	= false;
	m_nTotalRowCount= 0;
	m_nWidestItem	= 0;
}

CCheckList::~CCheckList()
{
	// Delete the data
	DeleteItems(&m_RootItemList);
	
	// Delete the control
	if( m_pCheckControl ) delete m_pCheckControl;
	
	// Delete the images
	if(m_pCheck)		delete m_pCheck;
	if(m_pUnCheck)		delete m_pUnCheck;
	if(m_pMiddleCheck)	delete m_pMiddleCheck;

	// Delete GDI Items
	if(m_pBkPen)		delete m_pBkPen;
	if(m_pBkBrush)		delete m_pBkBrush;
	if(m_bDeleteFont)	delete m_pTextFont;
}

VOID CCheckList::DeleteItems(CPtrList* pItemList)
{
	// Delete list item Structures
	LISTITEM*  pListItem;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos)
	{
		// Get this item
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);
		
		// Are there items in this 
		if( pListItem->m_ItemList.GetCount() != 0 )
			DeleteItems( &pListItem->m_ItemList);
		
		// Whas this item selected
		if( pListItem->bSelected )
			m_pCheckControl->SetCurSel(-1);
		m_nTotalRowCount--;
	    delete pListItem;
	}
	// Remove the pointers
	pItemList->RemoveAll();
}
	
BEGIN_MESSAGE_MAP(CCheckList, CWnd)
	//{{AFX_MSG_MAP(CCheckList)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCheckList::Create( CRect Rect, CWnd* pParent, UINT uID, UINT nCheckBitmap, UINT nUnCheckBitmap, UINT nMiddleCheckBitmap, COLORREF crBkColor, CFont* pCustomFont )
{
    // Gets window colors, brushes
	HCURSOR hCursor 	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	HBRUSH	Brush		= CreateSolidBrush(crBkColor);
 	CString	csWndClass 	= AfxRegisterWndClass( CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW, hCursor, Brush );

	// First load the bitmaps
	if( !LoadBitmaps(nCheckBitmap, nUnCheckBitmap, nMiddleCheckBitmap) )
		return FALSE;	
	
	// Create the Contol
	if( !CWnd::Create( csWndClass, _T("CLISTBOX"), WS_CHILD|WS_VISIBLE, Rect, pParent, uID ))
    	return FALSE;
	
	// Create the standard brush and pens and save the colors desired
	m_pBkPen			= new CPen(PS_SOLID, 1, crBkColor);
	m_pBkBrush			= new CBrush(crBkColor);
	
	// Did they send a custom Font
	m_pTextFont = pCustomFont;
	if(m_pTextFont==NULL)
	{
		// Create the default font
		m_bDeleteFont = true;
		m_pTextFont = new CFont();
		m_pTextFont->CreateFont( 14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, 
									 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,
									 _T("TAHOMA"));
	}
	
	// Set the height of an item
	CDC* pdc = GetWindowDC();
	TEXTMETRIC tm;
	CFont* pOldFont = pdc->SelectObject(m_pTextFont);
	pdc->GetTextMetrics(&tm);
    pdc->SelectObject( pOldFont );  
	ReleaseDC(pdc);

	// Is the font greater than the bitmap heights
	if( tm.tmHeight + tm.tmDescent > m_nLineHeight )
		m_nLineHeight = tm.tmHeight + tm.tmDescent; 

    // The number of viewable rows
	GetClientRect(m_WindowRect);

	// Call the create of the child
	m_pCheckControl	= new CCheckControl();
	if( !m_pCheckControl->Create( m_WindowRect, this, uID + 1, pParent, &m_RootItemList, crBkColor ) )
		return FALSE;

 	return TRUE;
}

bool CCheckList::LoadBitmaps(UINT nCheckBitmap, UINT nUnCheckBitmap, UINT nMiddleCheckBitmap)
{
	// Create two new bitmap and load them 
	m_pCheck = new CBitmap();
	if( !m_pCheck->LoadBitmap(nCheckBitmap) )
	{
		AfxMessageBox(_T("Unable to load CHECK bitmap."));
		return false;
	}
	m_pUnCheck = new CBitmap();
	if( !m_pUnCheck->LoadBitmap(nUnCheckBitmap) )
	{
		AfxMessageBox(_T("Unable to load UNCHECK bitmap."));
		return false;
	}
	m_pMiddleCheck = new CBitmap();
	if( !m_pMiddleCheck->LoadBitmap(nMiddleCheckBitmap) )
	{
		AfxMessageBox(_T("Unable to load MIDDLE Check bitmap."));
		return false;
	}

	// Make sure the hight and width are the same for both images
	BITMAP checkbm, uncheckbm, middlecheckbm;
	m_pCheck->GetObject(sizeof(checkbm), &checkbm );
	m_pUnCheck->GetObject(sizeof(uncheckbm), &uncheckbm );
	m_pMiddleCheck->GetObject(sizeof(middlecheckbm), &middlecheckbm );

	if( checkbm.bmHeight != uncheckbm.bmHeight || checkbm.bmWidth != uncheckbm.bmWidth || checkbm.bmHeight != middlecheckbm.bmHeight || checkbm.bmWidth != middlecheckbm.bmWidth)
	{
		AfxMessageBox(_T("Images must be the same size!"));
		return false;
	}
	m_cBitmapDimen.cx = uncheckbm.bmWidth;
	m_cBitmapDimen.cy = uncheckbm.bmHeight;
	
	// Increase the dimensions for spacing
	m_cBitmapDimen.cx += 6;
	m_cBitmapDimen.cy += 6;

	// Set the Line height
	m_nLineHeight = m_cBitmapDimen.cy;
	return true;
}
	
////////////////////////////////////////////////////////////////////////////
// User Functions
/////////////////////////////////////////////////////////////////////////////
LISTITEM* CCheckList::AddString( CString csText, LISTITEM* pParentItem, INT nCheckState, DWORD dwID, COLORREF crTextColor, COLORREF crTextHighColor, COLORREF crBgHighlightColor )
{
	// Calculate the Items width and height
	CDC* pdc = GetWindowDC();
	CFont* pOldFont = pdc->SelectObject(m_pTextFont);
	CSize cText = pdc->GetTextExtent(csText);
    pdc->SelectObject( pOldFont );  
	ReleaseDC(pdc);

	// Make sure its the correct kind
	if( nCheckState < UNCHECKED || nCheckState > INDETERMINATE )
		return NULL;

	// Create a new item
	LISTITEM* pListItem			= new LISTITEM;
	pListItem->csText			= csText;
	pListItem->dwID				= dwID;
	pListItem->nLevel			= 1;
	pListItem->nTextLength 		= cText.cx;
	pListItem->bSelected		= false;
	pListItem->nCheckedState 	= nCheckState;
	pListItem->crTextColor		= crTextColor;
	pListItem->crTextHighColor	= crTextHighColor;
	pListItem->crBgHighlightColor = crBgHighlightColor;
	pListItem->pParentItem		= NULL;
	
	// Add it to the correct list
	if(pParentItem==NULL) 
		m_RootItemList.AddTail(pListItem);
	else
	{
		pParentItem->m_ItemList.AddTail(pListItem);
		pListItem->pParentItem = pParentItem;
	}

	// Set the level this item is at
	LISTITEM* pCurrentItem = pListItem->pParentItem;
	while( pCurrentItem )
	{
		pListItem->nLevel++;	
		pCurrentItem = pCurrentItem->pParentItem;
	}
	
	// Set this items width
	pListItem->nTotalWidth = cText.cx + (pListItem->nLevel * m_cBitmapDimen.cx) + PADDING;

	// Is this the longest item?
	if( pListItem->nTotalWidth > m_nWidestItem )
		m_nWidestItem = pListItem->nTotalWidth;

	// Update the scroll bar
	m_nTotalRowCount++;
	m_pCheckControl->UpdateScrollBar();

	return pListItem;
}
INT CCheckList::DeleteString(INT nItem)
{
	// Is this a valid item?
	if( nItem < -1 || nItem > m_nTotalRowCount - 1 || m_pCheckControl == NULL)
		return LB_ERR;

	// Find this item
	m_nCurrentRow = 0;
	LISTITEM* pFoundItem = GetItem( &m_RootItemList, nItem );
	if( pFoundItem == NULL )
		return LB_ERR;
	
	// Recursivly delete the children items
	DeleteItems(&pFoundItem->m_ItemList);

	// Delete this item (must find it in the parent list)
	// Set the proper list to loop through
	CPtrList* pParentList = &m_RootItemList;
	if( pFoundItem->pParentItem != NULL )
		pParentList = &pFoundItem->pParentItem->m_ItemList;

	LISTITEM* pCurItem;
	POSITION Pos = pParentList->GetHeadPosition();
	POSITION OldPos;
	while(Pos)
	{
		OldPos = Pos;
		pCurItem = (LISTITEM*)pParentList->GetNext(Pos);
		if( pCurItem == pFoundItem )
		{
			pParentList->RemoveAt(OldPos);
			delete pCurItem;
			break;
		}
	}

	// Recalculate the widest item in the list
	m_nWidestItem = 0;
	GetWidestItem(&m_RootItemList); // (sets the member variable)

	// Redraw from this element down
	m_pCheckControl->UpdateScrollBar();
	m_pCheckControl->RedrawWindow();
	return 0;
}
INT CCheckList::GetCount(LISTITEM* pParentItem)
{
	if(pParentItem)
		return pParentItem->m_ItemList.GetCount();
	
	return m_nTotalRowCount;
}
INT	CCheckList::GetTopIndex()
{
	if(m_pCheckControl)
		return m_pCheckControl->GetTopIndex();
	return -1;	
}
INT CCheckList::SetTopIndex(INT nTop)
{
	if(m_pCheckControl)
	{
		// Is this too high
		if( nTop > m_nTotalRowCount )
			return LB_ERR;
		
		// Are there not enough items to scroll
		if( m_pCheckControl->GetViewableRows() > m_nTotalRowCount  )
			return 0;
		
		// Set the top
		if( nTop > m_nTotalRowCount - m_pCheckControl->GetViewableRows() )
			nTop = m_nTotalRowCount - m_pCheckControl->GetViewableRows();
		
		// Set the top
		m_pCheckControl->SetTopIndex(nTop);
	}

	return 0;
}
DWORD CCheckList::GetItemData(INT nItem)
{
	// Find the item
	m_nCurrentRow = 0;
	LISTITEM* pFoundItem = GetItem( &m_RootItemList, nItem );
	if( pFoundItem )
		return pFoundItem->dwID;
	return LB_ERR;
}
INT CCheckList::SetItemData(INT nItem, DWORD dwID)
{
	// Find the item
	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItem( &m_RootItemList, nItem );
	if( pItem == NULL )
		return LB_ERR;

	pItem->dwID = dwID;
	return 0;
}
LISTITEM* CCheckList::GetItem(INT nItem)
{
	// Find the item
	m_nCurrentRow = 0;
	return GetItem( &m_RootItemList, nItem );	
}
LISTITEM* CCheckList::GetItem(DWORD dwID)
{
	// Find the item
	return GetItem( &m_RootItemList, dwID );	
}
INT CCheckList::GetText(INT nItem, CString* pString )
{
	// Find the item
	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItem( &m_RootItemList, nItem );
	if( pItem == NULL )
		return LB_ERR;

	// Copy the string
	*pString = pItem->csText;
	return 0;
}
INT CCheckList::GetTextLen(INT nItem )
{
	// Find the item
	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItem( &m_RootItemList, nItem );
	if( pItem == NULL )
		return LB_ERR;

	return pItem->csText.GetLength();
}
INT CCheckList::GetCurSel()
{
	if( m_pCheckControl == NULL)
		return LB_ERR;

	m_pCheckControl->GetCurSel();
	return 0;
}
INT CCheckList::SetCurSel(INT nItem)
{
	// Is this a valid item?
	if( nItem < -1 || nItem > m_nTotalRowCount - 1 || m_pCheckControl == NULL)
		return LB_ERR;
	
	return m_pCheckControl->SetCurSel(nItem);
}
VOID CCheckList::ResetContent()
{
	// Delete all elements		
	DeleteItems(&m_RootItemList);
	m_nTotalRowCount = 0;
	m_nWidestItem = 0;
	m_pCheckControl->UpdateScrollBar();
	m_pCheckControl->RedrawWindow();
}
INT CCheckList::SetCheck(INT nItem, INT nCheckState )
{
	// Is this a valid item?
	if( nItem < -1 || nItem > m_nTotalRowCount - 1 || m_pCheckControl == NULL)
		return LB_ERR;
	
	// Make sure its the correct kind
	if( nCheckState < UNCHECKED ||  nCheckState > INDETERMINATE )
		return LB_ERR;

	return m_pCheckControl->SetCheck(nItem, nCheckState);
}
INT CCheckList::GetCheck(INT nItem)
{
	// Is this a valid item?
	if( nItem < -1 || nItem > m_nTotalRowCount - 1 || m_pCheckControl == NULL)
		return LB_ERR;

	m_nCurrentRow = 0;
	LISTITEM* pItem = GetItem( &m_RootItemList, nItem );
	if( pItem == NULL )
		return LB_ERR;
	
	return pItem->nCheckedState;
}

////////////////////////////////////////////////////////////////////////////
// Painting routines
/////////////////////////////////////////////////////////////////////////////
VOID CCheckList::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//////////////////////////////
	// Paint the 3-D Border
	CRect Rect;
	GetClientRect(Rect);

	CPen*		pOldPen;
	CPen		DkGrayPen( PS_SOLID, 1, RGB(128,128,128));
	CPen		GrayPen( PS_SOLID, 1, RGB(192,192,192));
	CPen		WhitePen( PS_SOLID, 1, RGB(255,255,255));
	CPen		BlackPen( PS_SOLID, 1, RGB(0,0,0 ));

	// Draw left and top
	pOldPen		= dc.SelectObject(&DkGrayPen);	
	dc.MoveTo( Rect.left,  	Rect.bottom);
	dc.LineTo( Rect.left,	Rect.top );
	dc.LineTo( Rect.right,	Rect.top );
	
	// Draw the Black inner shadow
	dc.SelectObject(&BlackPen);	
	dc.MoveTo( Rect.left + 1,  	Rect.bottom - 1);
	dc.LineTo( Rect.left + 1,	Rect.top + 1 );
	dc.LineTo( Rect.right - 1,	Rect.top + 1 );
	
	// Draw White sides
	dc.SelectObject(&WhitePen);
	dc.MoveTo( Rect.right - 1, 	Rect.top + 1);
	dc.LineTo( Rect.right - 1, 	Rect.bottom - 1);
	dc.LineTo( Rect.left + 1, 	Rect.bottom - 1);
	
	// Draw gray sides
	dc.SelectObject(&GrayPen);
	dc.MoveTo( Rect.right - 2, 	Rect.top + 2);
	dc.LineTo( Rect.right - 2, 	Rect.bottom - 2);
	dc.LineTo( Rect.left + 2, 	Rect.bottom - 2);
	
	// Select the old tools
 	dc.SelectObject( pOldPen );	
}

VOID CCheckList::OnSize(UINT nType, INT cx, INT cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// Move the Control	
	GetClientRect(m_WindowRect);
	if( m_pCheckControl )
	{
		CRect ControlRect(m_WindowRect);
		ControlRect.InflateRect(-2, -2);
		m_pCheckControl->MoveWindow(ControlRect);
	}
}

LISTITEM* CCheckList::GetItem( CPtrList* pItemList, INT nItem )
{
	// loop for all items
	LISTITEM*  pListItem = NULL;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos)
	{
		// Get this item
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);

		// Is this the item?
		if( m_nCurrentRow == nItem )
			return pListItem;
		
		// Are there any open items in this?
		m_nCurrentRow++;
		if( pListItem->m_ItemList.GetCount() != 0 )
		{
			pListItem = GetItem( &pListItem->m_ItemList, nItem);
			if( pListItem )
				return pListItem;
		}
	}

	return NULL;
}

LISTITEM* CCheckList::GetItem( CPtrList* pItemList, DWORD dwID )
{
	// loop for all items
	LISTITEM*  pListItem = NULL;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos)
	{
		// Get this item
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);

		// Is this the item?
		if( pListItem->dwID == dwID )
			return pListItem;
		
		// Are there any open items in this?
		if( pListItem->m_ItemList.GetCount() != 0 )
		{
			pListItem = GetItem( &pListItem->m_ItemList, dwID );
			if( pListItem )
				return pListItem;
		}
	}

	return NULL;
}

VOID CCheckList::GetWidestItem(CPtrList* pItemList)
{
	// loop for all items
	LISTITEM*  pListItem = NULL;
	POSITION Pos = pItemList->GetHeadPosition();
	while(Pos)
	{
		// Get this item
		pListItem = (LISTITEM*)pItemList->GetNext(Pos);

		// Is this item wider
		if( pListItem->nTotalWidth > m_nWidestItem )
			m_nWidestItem = pListItem->nTotalWidth;
		
		// Are there any open items in this?
		if( pListItem->m_ItemList.GetCount() != 0 )
			GetWidestItem( &pListItem->m_ItemList );
	}
}
