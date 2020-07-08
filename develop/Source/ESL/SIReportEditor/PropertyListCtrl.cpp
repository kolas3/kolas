// PropertyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// CString sory Array
VOID CSortStringArray::Sort()
{
EFS_BEGIN

	BOOL bNotDone = TRUE;   
	while (bNotDone)   
	{      bNotDone = FALSE;
      for(INT pos = 0;pos < GetUpperBound();pos++)
        bNotDone |= CompareAndSwap(pos);   
	}

EFS_END
}

BOOL CSortStringArray::CompareAndSwap( INT pos )
{
EFS_BEGIN

   CString temp;   
   INT posFirst = pos;   
   INT posNext = pos + 1;
   if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)   
   {
      temp = GetAt(posFirst);      
	  SetAt(posFirst, GetAt(posNext));
      SetAt(posNext, temp);      
	  return TRUE;   
   }   
   
   return FALSE;

EFS_END
return FALSE;

}

INT CSortStringArray::FindString(CString csText)
{
EFS_BEGIN

	INT nPos = 1;

	// Loop through all items
	for( INT Item = 0; Item < GetSize(); Item++)
	{
		if(GetAt(Item).CompareNoCase(csText) == 0)
			return Item;
	}

	return nPos;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CComboButton
CComboButton::CComboButton()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_pBkPen = NULL;
	m_pGrayPen = NULL;
	m_pBkBrush = NULL; 
	m_pBlackBrush = NULL;
}

CComboButton::~CComboButton()
{
	// Delete the objects created
	delete m_pBkBrush;
	delete m_pBlackBrush;
	delete m_pGrayPen;
	delete m_pBkPen;
}

BEGIN_MESSAGE_MAP(CComboButton, CButton)
	//{{AFX_MSG_MAP(CComboButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboButton message handlers
BOOL CComboButton::Create( CRect Rect, CWnd* pParent, UINT uID)
{
EFS_BEGIN

	// Create the Brushes and Pens
	m_pBkBrush		= new CBrush( GetSysColor(COLOR_BTNFACE));
	m_pBkPen		= new CPen( PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
	m_pGrayPen		= new CPen( PS_SOLID, 1, RGB(128,128,128));
	m_pBlackBrush	= new CBrush(RGB(0,0,0)); 

    // Create the CButton
	if( !CButton::Create( _T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, Rect, pParent, uID ))
		return FALSE;
		
	return 0;

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// Draw the Button
VOID CComboButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
EFS_BEGIN

	CDC*	pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect 	ButtonRect  = lpDrawItemStruct->rcItem;
    
	// Fill the Background
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject( m_pBkBrush );
	CPen* pOldPen = (CPen*)pDC->SelectObject(m_pBkPen);
	pDC->Rectangle(ButtonRect);
		
	// Draw the Correct Border
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		pDC->DrawEdge(ButtonRect, EDGE_SUNKEN, BF_RECT);
		ButtonRect.left++;
		ButtonRect.right++;
		ButtonRect.bottom++;
		ButtonRect.top++;
	}
	else
		pDC->DrawEdge(ButtonRect, EDGE_RAISED, BF_RECT);
	
	// Draw the Triangle
	ButtonRect.left		+= 3;
	ButtonRect.right	-= 4;
	ButtonRect.top		+= 5;
	ButtonRect.bottom	-= 5;
	DrawTriangle(pDC, ButtonRect);

	// Return what was used
	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );

EFS_END
}

VOID CComboButton::DrawTriangle(CDC* pDC, CRect Rect)
{
EFS_BEGIN

	POINT 	ptArray[3];	

	// Figure out the Top left
	ptArray[0].x = Rect.left;
	ptArray[0].y = Rect.top;
	ptArray[1].x = Rect.right;
	ptArray[1].y = Rect.top;
	ptArray[2].x = Rect.right - (Rect.Width() / 2);
	ptArray[2].y = Rect.bottom;

	// Select the Brush and Draw the triangle
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(m_pBlackBrush);
	pDC->Polygon(ptArray, 3 );

EFS_END
}
VOID CComboButton::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
EFS_BEGIN


EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CPropertyListCtrl
CPropertyListCtrl::CPropertyListCtrl()
{
	m_nWidestItem			= 0;
	m_bDeleteFont			= TRUE;
	m_bBoldSelection		= FALSE;

	m_pBkBrush				= NULL;
	m_pBkPropertyBrush		= NULL;
	m_pEditWnd				= NULL;
	m_pMultiEditWnd			= NULL;
	m_pColourPicker			= NULL;
	m_pFontButton			= NULL;
	m_pPathButton			= NULL;
	m_pComboButton			= NULL;
	m_pListBox				= NULL;
	m_pBkHighlightBrush		= NULL;
	m_pSelectedFont			= NULL;
	m_pBorderPen			= NULL;
	m_pCurItem				= NULL;
	m_pCurFont				= NULL;
	m_pCurDrawItem			= NULL;
	m_pTextFont				= NULL;
	m_pSelectedFont			= NULL;
	m_pBorderPen			= NULL;

	m_crBorderColor			= RGB(192,192,192);
	m_crBkColor				= GetSysColor(COLOR_WINDOW);
	m_crPropertyBkColor		= m_crBkColor;
	m_crTextColor			= GetSysColor(COLOR_WINDOWTEXT);
	m_crPropertyTextColor	= m_crTextColor;
	m_crHighlightColor		= GetSysColor(COLOR_HIGHLIGHT);
	m_crTextHighlightColor	= GetSysColor(COLOR_HIGHLIGHTTEXT);

	// KOL.UDF.022
	
	m_bChanged = FALSE;
	m_CurRect = NULL;
	m_csText = _T("");
	m_pCurBrush = NULL;
	m_crFont = NULL;
	m_strAlias = _T("");
}

CPropertyListCtrl::~CPropertyListCtrl()
{
	// Clean up
	if(m_bDeleteFont)		delete m_pTextFont;

	if(m_pEditWnd)			delete m_pEditWnd;
	if(m_pMultiEditWnd)		delete m_pMultiEditWnd;
	if(m_pColourPicker)		delete m_pColourPicker;
	if(m_pFontButton)		delete m_pFontButton;
	if(m_pPathButton)		delete m_pPathButton;
	if(m_pListBox)			delete m_pListBox;
	if(m_pComboButton)		delete m_pComboButton;

	if(m_pBkBrush)			delete m_pBkBrush;
	if(m_pBkPropertyBrush)	delete m_pBkPropertyBrush;
	if(m_pBkHighlightBrush) delete m_pBkHighlightBrush;
	if(m_pSelectedFont)		delete m_pSelectedFont;
	if(m_pBorderPen)		delete m_pBorderPen;

	// Clear items
	Reset();
}

CPropertyListCtrl::Create(const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CListBox::Create(WS_CHILD|WS_BORDER|WS_VISIBLE|LBS_NOTIFY|WS_VSCROLL|WS_HSCROLL|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED, rect, pParentWnd, nID);
}

VOID CPropertyListCtrl::Reset()
{	
EFS_BEGIN

	// Clear the List
	POSITION Pos = m_Items.GetHeadPosition();
	while(Pos)
	{
		m_pCurItem = (PROPERTYITEM*)m_Items.GetNext(Pos);
		if(m_pCurItem->pBrush)
			delete m_pCurItem->pBrush;
		delete m_pCurItem;
	}
	m_Items.RemoveAll();

EFS_END
}
	
BEGIN_MESSAGE_MAP(CPropertyListCtrl, CListBox)
	//{{AFX_MSG_MAP(CPropertyListCtrl)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_EN_KILLFOCUS( ID_PROPERTY_TEXT, OnEditLostFocus )
	ON_EN_KILLFOCUS( ID_PROPERTY_TEXT_MULTI, OnEditLostFocus )
	ON_EN_CHANGE( ID_PROPERTY_TEXT, OnEditChange )
	ON_EN_CHANGE( ID_PROPERTY_TEXT_MULTI, OnEditChange )
	ON_BN_CLICKED( ID_PROPERTY_FONT, OnFontPropertyClick )
	ON_BN_CLICKED( ID_PROPERTY_PATH, OnPathPropertyClick )
	ON_BN_CLICKED( ID_PROPERTY_COMBO_BTN, OnComboBoxClick )
	ON_LBN_SELCHANGE(ID_PROPERTY_COMBO_LIST, OnSelChange)
	ON_LBN_KILLFOCUS(ID_PROPERTY_COMBO_LIST, OnListboxLostFocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnColourPickerSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyListCtrl message handlers
INT CPropertyListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
EFS_BEGIN

	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the default font and set it
	m_pTextFont = new CFont();
	m_pTextFont->CreateFont( 14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, 
							 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,
							 _T("ARIAL"));	

	// Create the Heading font and set it
	m_pSelectedFont = new CFont();
	m_pSelectedFont->CreateFont( 14, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, 
							 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,
							 _T("ARIAL"));	

	// Create the Border Pen
	m_pBorderPen = new CPen(PS_SOLID, 1, m_crBorderColor);
	
	// Create the Selected Background brush
	m_pBkHighlightBrush = new CBrush(m_crHighlightColor);
	m_pBkBrush			= new CBrush(m_crBkColor);

	// Set the row height
	SetItemHeight(-1,16);
	return 0;

EFS_END
return -1;

}
HBRUSH CPropertyListCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{	
	return (HBRUSH)m_pBkBrush->GetSafeHandle();
}

HBRUSH CPropertyListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor == CTLCOLOR_EDIT) 
	{
		pDC->SetBkColor(m_crPropertyBkColor);
		pDC->SetTextColor(m_crPropertyTextColor);
	}

	if(m_pBkPropertyBrush)
		return (HBRUSH)(m_pBkPropertyBrush->GetSafeHandle() );
	else
		return hbr;
}
VOID CPropertyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
EFS_BEGIN

	// Make sure its a valid item
	if( lpDrawItemStruct->itemID == LB_ERR )
		return;
     
  	// Obtain the text for this item
	m_csText.Empty();
	GetText(lpDrawItemStruct->itemID, m_csText);

	// Get the drawing DC
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	// Set the Current member we are drawing
    m_pCurDrawItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(lpDrawItemStruct->itemID));
	
	// Obtain the Item Rect
	CRect ItemRect(lpDrawItemStruct->rcItem);
	
    // Draw This item
	DrawItem( pDC, ItemRect, lpDrawItemStruct->itemState & ODS_SELECTED);

EFS_END
}
VOID CPropertyListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
EFS_BEGIN


EFS_END
}
VOID CPropertyListCtrl::OnDblclk() 
{
EFS_BEGIN

	// Get the Course Position
	POINT pPoint;
	GetCursorPos(&pPoint);
	
	// Convert this rect to coordinates of the desktop	
	CRect TempRect = m_CurRect;
	MapWindowPoints(GetDesktopWindow(), TempRect);

	// Display the Correct Control
	switch(m_pCurItem->nType)
	{
		case	ID_PROPERTY_BOOL:	
				// Is the Mouse in this area
				if(TempRect.PtInRect(pPoint))
				{
					// Reverse the Selection
					m_pCurItem->nPropertySelected = !m_pCurItem->nPropertySelected;
					
					// Redraw this item
					RedrawWindow();

					// Send the message that a property has changed
					GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);
				}
				break;

		case	ID_PROPERTY_FONT:
				m_pFontButton->SetFocus();		
				OnFontPropertyClick();
				break;

		case	ID_PROPERTY_PATH:
				//m_pPathButton->SetFocus();		
				//OnPathPropertyClick();
				break;

		case	ID_PROPERTY_COLOR:
				/*LOGBRUSH lb;
				m_pCurItem->pBrush->GetLogBrush(&lb);
				CColorDialog ColorDialog(lb.lbColor, 0, GetParent());
				if(ColorDialog.DoModal() != IDOK)
					return;

				// Destroy the Brush and create a new one
				if(m_pCurItem->pBrush) delete m_pCurItem->pBrush;
				m_pCurItem->pBrush = new CBrush(ColorDialog.GetColor());

				// Redraw the Widow (Theres probably a better way)
				RedrawWindow();

				// Send the message that a property has changed
				GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);*/
				break;
	}
	

EFS_END
}

VOID CPropertyListCtrl::OnSelchange() 
{	
EFS_BEGIN
	
	HideControls();

	// Display the Correct Control
	long nLeft;
	LOGBRUSH lb;
	CRect TempRect = m_CurRect;
	TempRect.InflateRect(-1,-1);
	switch(m_pCurItem->nType)
	{
		case	ID_PROPERTY_TEXT:
				TempRect.left += 1;
				m_pEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(0));
				m_pEditWnd->MoveWindow(TempRect);
				m_pEditWnd->ShowWindow(SW_SHOWNORMAL);
				m_pEditWnd->SetFocus();
				break;
		
		case    ID_PROPERTY_TEXT_MULTI:
				TempRect.left += 1;
				m_pMultiEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(0));
				m_pMultiEditWnd->MoveWindow(TempRect.left, TempRect.top, TempRect.Width(), 100 );			//monamy
				m_pMultiEditWnd->ShowWindow(SW_SHOWNORMAL);
				m_pMultiEditWnd->SetFocus();
				break;

		case	ID_PROPERTY_FONT:
				TempRect.left = TempRect.right - 17;
				m_pFontButton->MoveWindow(TempRect);
				m_pFontButton->ShowWindow(SW_SHOWNORMAL);
				break;

		case	ID_PROPERTY_PATH:
				nLeft = TempRect.left;
				TempRect.left = TempRect.right - 17;
				m_pPathButton->MoveWindow(TempRect);
				m_pPathButton->ShowWindow(SW_SHOWNORMAL);
				m_pPathButton->SetFocus();

				TempRect.left = m_CurRect.left + 2;
				TempRect.right -= 17;
				if(m_pCurItem->bComboEditable)
				{
					m_pEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(0));
					m_pEditWnd->MoveWindow(TempRect);
					m_pEditWnd->ShowWindow(SW_SHOWNORMAL);
					m_pEditWnd->SetFocus();
				}

				break;

		case	ID_PROPERTY_COLOR:

				m_pCurItem->pBrush->GetLogBrush(&lb);
				m_pColourPicker->SetColour(lb.lbColor);

				// Move the Colour Picker
				TempRect.top = TempRect.bottom;
				m_pColourPicker->MoveWindow(TempRect);
				m_pColourPicker->OnClick();
				break;

		case	ID_PROPERTY_COMBO_LIST:
				TempRect.left = TempRect.right - 17;
				m_pComboButton->MoveWindow(TempRect);
				m_pComboButton->ShowWindow(SW_SHOWNORMAL);

				TempRect.left = m_CurRect.left + 2;
				TempRect.right -= 17;
				if(m_pCurItem->bComboEditable)
				{
					m_pEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(m_pCurItem->nPropertySelected));
//					m_pEditWnd->SetFocus();
//					m_pEditWnd->SetSel(0,-1);
					m_pEditWnd->MoveWindow(TempRect);
					m_pEditWnd->ShowWindow(SW_SHOWNORMAL);
				}

				// Move the List box
				TempRect.left--;
				TempRect.right += 18;
				TempRect.top = TempRect.bottom;

				// Set the Bottom Height
				if(m_pCurItem->csProperties.GetSize() > 5)
					TempRect.bottom += GetItemHeight(0) * 10; 
					// TempRect.bottom += GetItemHeight(0) * 5; // MODIFIED
				else
					TempRect.bottom += GetItemHeight(0) * m_pCurItem->csProperties.GetSize();
				m_pListBox->MoveWindow(TempRect);

				// Force the Expansion
				OnComboBoxClick();
				break;

	}

EFS_END
}

VOID CPropertyListCtrl::OnEditLostFocus()
{
EFS_BEGIN

	// Get the text
	CString csText;

	if (m_pCurItem->nType == ID_PROPERTY_TEXT_MULTI)
		m_pMultiEditWnd->GetWindowText(csText);
	else
		m_pEditWnd->GetWindowText(csText);

	// Is the current item a text item
	if(m_pCurItem->nType == ID_PROPERTY_TEXT ||  m_pCurItem->nType == ID_PROPERTY_TEXT_MULTI)
	{
		// Did the text change
		if(!m_bChanged)
			return;

		m_pCurItem->csProperties.SetAt(0, csText);
		
		// Send the message that a property has changed
		GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);
	}
	else
	{
		// Get the window that has the focus now
		if(GetFocus() == m_pComboButton || !m_pListBox->GetCount())
			return;
		
		// Did the text change
		if(!m_bChanged)
			return;

		// Send the message that a property has changed
		GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);

		// Look for this text
		m_bChanged = FALSE;
		if( m_pListBox->FindStringExact(-1,csText) != LB_ERR)
			return;

		// Add it and select it
		m_pCurItem->nPropertySelected = m_pCurItem->csProperties.Add(csText);

		// Resort the strings is necessary
		if(m_pCurItem->bComboSorted)
		{
			m_pCurItem->csProperties.Sort();
	
			// Search the the string and set its positon the selected one
			m_pCurItem->nPropertySelected = m_pCurItem->csProperties.FindString(csText);
		}
	}

EFS_END
}

VOID CPropertyListCtrl::OnEditChange()
{
EFS_BEGIN

	m_bChanged = TRUE;

EFS_END
}

LONG CPropertyListCtrl::OnColourPickerSelEndOK(UINT /*lParam*/, LONG /*wParam*/)
{
EFS_BEGIN

	// Destroy the Brush and create a new one
	if(m_pCurItem->pBrush) delete m_pCurItem->pBrush;
	m_pCurItem->pBrush = new CBrush(m_pColourPicker->GetColour());

	// Redraw the Widow (Theres probably a better way)
	RedrawWindow();

	// Send the message that a property has changed
	GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);

    return TRUE;

EFS_END
return -1;

}

VOID CPropertyListCtrl::OnFontPropertyClick()
{
EFS_BEGIN

	// Show the Dialog
	CFontDialog FontDialog(&m_pCurItem->LogFont);
	FontDialog.m_cf.rgbColors = m_crFont;

	if(FontDialog.DoModal() != IDOK)
		return;
	
	// Set the Font data 
	FontDialog.GetCurrentFont(&m_pCurItem->LogFont);
	m_pCurItem->nFontSize = FontDialog.GetSize()/10;
	m_pCurItem->colorFont = FontDialog.GetColor();

	// Redraw
	RedrawWindow();

	// Send the message that a property has changed
	GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);

EFS_END
}

VOID CPropertyListCtrl::OnPathPropertyClick()
{
EFS_BEGIN

	// Look for a ending tag
	CString csExt = _T("*");
	CString csPath = m_pCurItem->csProperties.GetAt(0);
	INT nPos = csPath.ReverseFind('.');
	if(nPos)
		csExt = csPath.Right(csPath.GetLength() - nPos - 1);

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	memset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = _T("BMP파일(*.BMP)\0*.BMP\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("BMP 파일 열기...");
	OFN.lpstrInitialDir = csPath;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("BMP");
	OFN.Flags = OFN_OVERWRITEPROMPT;
	
	if (::GetOpenFileName(&OFN) == 0) 
	{
		SetCurrentDirectory(szBuffer);
		return;
	}
	SetCurrentDirectory(szBuffer);
	
	m_pCurItem->csProperties.SetAt(0, OFN.lpstrFile);

	if(m_pCurItem->bComboEditable)
		m_pEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(m_pCurItem->nPropertySelected));
	else
		// Redraw
		RedrawWindow();
	
	// Send the message that a property has changed
	GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);

EFS_END
}

VOID CPropertyListCtrl::OnComboBoxClick()
{
EFS_BEGIN

	// Add the items
	m_pListBox->ResetContent();

	// Loop for all items
	for( INT nItem = 0; nItem < m_pCurItem->csProperties.GetSize(); nItem++)
		m_pListBox->AddString(m_pCurItem->csProperties.GetAt(nItem));

	// Select the correct item
	m_pListBox->SetCurSel(m_pCurItem->nPropertySelected);
	m_pListBox->SetTopIndex(m_pCurItem->nPropertySelected);
	
	// Show the List box
	m_pListBox->ShowWindow(SW_NORMAL);	

EFS_END
}

VOID CPropertyListCtrl::OnSelChange()
{
EFS_BEGIN

	// Set the new current item
	m_pCurItem->nPropertySelected = m_pListBox->GetCurSel();

	// Hide the Windows
	m_pListBox->ShowWindow(SW_HIDE);

	if(m_pCurItem->bComboEditable)
	{
		if (m_pCurItem->nType == ID_PROPERTY_TEXT)
			m_pEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(m_pCurItem->nPropertySelected));
		else if (m_pCurItem->nType == ID_PROPERTY_TEXT_MULTI)
			m_pMultiEditWnd->SetWindowText(m_pCurItem->csProperties.GetAt(m_pCurItem->nPropertySelected));
	}
	else
		RedrawWindow();

	// Send the message that a property has changed
	GetParent()->PostMessage(ID_PROPERTY_CHANGED, GetCurSel(), m_pCurItem->nType);
	m_pComboButton->SetFocus();

EFS_END
}

VOID CPropertyListCtrl::OnListboxLostFocus()
{
EFS_BEGIN

	m_pListBox->ShowWindow(SW_HIDE);

EFS_END
}

VOID CPropertyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	// is there an item at this point
	BOOL bOutside;
	UINT uItem = ItemFromPoint(point, bOutside);

	// Is this outside the client
	if(bOutside)
		HideControls();
	
	CListBox::OnLButtonDown(nFlags, point);

EFS_END
}

VOID CPropertyListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
EFS_BEGIN

	// Hide the Controls
	HideControls();

	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// GUI User Functions
/////////////////////////////////////////////////////////////////////////////
VOID CPropertyListCtrl::SetFont(CFont* pFont)
{
EFS_BEGIN

	// Delete our font and set our font to theirs
	if(m_pTextFont)		delete m_pTextFont;
	if(m_pSelectedFont) delete m_pSelectedFont;
	m_pTextFont = pFont;
	m_bDeleteFont = FALSE;
	
	// Figure out the text size
	LOGFONT lpLogFont;
	m_pTextFont->GetLogFont(&lpLogFont);

	// Set the font and redraw
	CWnd::SetFont(m_pTextFont, FALSE);

	// Create the heading font with the bold attribute
	lpLogFont.lfWeight = FW_BOLD;
	m_pSelectedFont = new CFont();
	m_pSelectedFont->CreateFontIndirect(&lpLogFont);

	// Set the Row height
	SetItemHeight(-1,lpLogFont.lfHeight + 2);

	// ** IMPLEMENT LATER ?? **
	// Recalculate the Width Position


EFS_END
}
VOID CPropertyListCtrl::SetLineStyle(COLORREF crColor, INT nStyle)
{
EFS_BEGIN

	// Delete the old Pen
	if(m_pBorderPen) delete m_pBorderPen;
	
	// Create the brush
	m_pBorderPen = new CPen(nStyle, 1, crColor);
	m_crBorderColor = crColor;

EFS_END
}
VOID CPropertyListCtrl::SetBkColor(COLORREF crColor)
{
EFS_BEGIN

	// Delete the old brush
	if(m_pBkBrush) delete m_pBkBrush;
	
	// Create the brush
	m_pBkBrush = new CBrush(crColor);
	m_crBkColor = crColor;

EFS_END
}
VOID CPropertyListCtrl::SetPropertyBkColor(COLORREF crColor)
{
EFS_BEGIN

	// Delete the old brush
	if(m_pBkPropertyBrush) delete m_pBkPropertyBrush;
	
	// Create the brush
	m_pBkPropertyBrush = new CBrush(crColor);
	m_crPropertyBkColor = crColor;

EFS_END
}

VOID CPropertyListCtrl::SetHighlightColor(COLORREF crColor)
{
EFS_BEGIN

	// Delete the old brush
	if(m_pBkHighlightBrush) delete m_pBkHighlightBrush;
	
	// Create the brush
	m_pBkHighlightBrush = new CBrush(crColor);
	m_crHighlightColor = crColor;

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// Add Properties Functions
/////////////////////////////////////////////////////////////////////////////
BOOL CPropertyListCtrl::AddString(CString csText, CString strAlias)
{
EFS_BEGIN

	// Call our function (assume its a text Item)
	return AddString(csText, strAlias, ID_PROPERTY_STATIC, _T("<No Property Set>"));

EFS_END
return FALSE;

}
BOOL CPropertyListCtrl::AddString(CString csText, CString strAlias, INT nType, CString csData, INT nPropertySelected, INT nAlignment, BOOL bComboEditable, BOOL bComboSorted)
{
EFS_BEGIN

	// Is this a valid Control type
	if(nType > ID_PROPERTY_COMBO_LIST)
		return FALSE;
		
	// Create a new Structure to hold it
	PROPERTYITEM*  pNewItem = new PROPERTYITEM;
	pNewItem->strAlias = strAlias;
	pNewItem->nType = nType;
	pNewItem->nAlignment = nAlignment;
	pNewItem->pBrush = NULL;
	pNewItem->bComboEditable = bComboEditable;
	pNewItem->bComboSorted = bComboSorted;

	// Calculate the Width of the string based on the font set
	CDC* pDC = GetDC();
	pDC->SelectObject(m_pSelectedFont);
	CSize Size = pDC->GetTextExtent(csText);
	if(Size.cx + 10 > m_nWidestItem)
		m_nWidestItem = Size.cx + 10;
	ReleaseDC(pDC);
	pNewItem->nWidth = Size.cx;
	pNewItem->nPropertySelected = nPropertySelected;

	// Set Property
	if(!SetProperty(pNewItem, nType, csData))
	{
		delete pNewItem;
		return FALSE;
	}

	// Is the item set bigger than the number of properties
	if(pNewItem->nPropertySelected > pNewItem->csProperties.GetSize())
	{
		delete pNewItem;
		return FALSE;
	}

	// Add to the list
	m_Items.AddTail(pNewItem);

	// Add the string to the list box
	INT nPos = CListBox::AddString(csText);

	// Create the Control if Needed
	CreateControl(nType);
	return TRUE;

EFS_END
return FALSE;

}
BOOL CPropertyListCtrl::AddString(CString csText, CString strAlias, COLORREF crColor, INT nAlignment)
{
EFS_BEGIN

	// Create a new brush based on this color
	m_pCurBrush = new CBrush(crColor);
	
	// Call the other functions
	return AddString(csText, strAlias, ID_PROPERTY_COLOR, _T(""), 0, nAlignment);

EFS_END
return FALSE;

}
BOOL CPropertyListCtrl::AddString(CString csText, CString strAlias, CFont* pFont, COLORREF color, INT nAlignment)
{
EFS_BEGIN

	// Safe the Font
	m_pCurFont = pFont;
	m_crFont = color;
	
	// Call the other functions
	return AddString(csText, strAlias, ID_PROPERTY_FONT, _T(""), 0, nAlignment);

EFS_END
return FALSE;

}
		
/////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////
VOID CPropertyListCtrl::DrawItem(CDC* pDC, CRect ItemRect, BOOL bSelected)
{
EFS_BEGIN

	/////////////////////////////////////////
	// Paint the Background rectangle (Property Value)
	if(m_pCurDrawItem->nType == ID_PROPERTY_COLOR)
		pDC->SelectObject(m_pCurDrawItem->pBrush);
	else
		pDC->SelectObject(m_pBkBrush);
	pDC->SelectObject(m_pBorderPen);

	// Draw the Rectangle
	ItemRect.left = m_nWidestItem - 1;
	ItemRect.top--;
	ItemRect.right++;
	pDC->Rectangle(ItemRect);
	CRect OrginalRect = ItemRect;
	
	/////////////////////////////////////////
	// Draw the Property Text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_pBkBrush);
	pDC->SelectObject(m_pTextFont);
	pDC->SetTextColor(m_crTextColor);
	DrawPropertyText(pDC, ItemRect);
	
	/////////////////////////////////////////
	// Paint the Background rectangle (Property Name)
	if( bSelected )
		pDC->SelectObject(m_pBkHighlightBrush);
	
	// Draw the Rectangle
	ItemRect.right = m_nWidestItem;
	ItemRect.left = -1;
	pDC->Rectangle(ItemRect);
	
	/////////////////////////////////////////
	// Paint the Property name Text
	// Is this item selected?
	if( bSelected )
	{		
		if(m_bBoldSelection) pDC->SelectObject(m_pSelectedFont);
		pDC->SetTextColor(m_crTextHighlightColor);
		m_pCurItem = m_pCurDrawItem;
		m_CurRect = OrginalRect;
	}
	
	// Draw the Text
	ItemRect.left += 6;
	ItemRect.right -= 5;
	if (m_pCurItem != NULL && m_pCurItem->nType == ID_PROPERTY_TEXT_MULTI)
		pDC->DrawText( m_csText, m_csText.GetLength(), ItemRect, DT_WORDBREAK|m_pCurDrawItem->nAlignment);
	else
		pDC->DrawText( m_csText, m_csText.GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|m_pCurDrawItem->nAlignment);

EFS_END
}

VOID CPropertyListCtrl::DrawPropertyText(CDC* pDC, CRect ItemRect)
{
EFS_BEGIN

	ItemRect.left += 5;
	switch(m_pCurDrawItem->nType)
	{
		case	ID_PROPERTY_BOOL:
		case	ID_PROPERTY_COMBO_LIST:
				pDC->DrawText( m_pCurDrawItem->csProperties.GetAt(m_pCurDrawItem->nPropertySelected), m_pCurDrawItem->csProperties.GetAt(m_pCurDrawItem->nPropertySelected).GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				break;
		
		case	ID_PROPERTY_STATIC:
		case	ID_PROPERTY_TEXT:
		case	ID_PROPERTY_PATH:
				pDC->DrawText( m_pCurDrawItem->csProperties.GetAt(0), m_pCurDrawItem->csProperties.GetAt(0).GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				break;
		case	ID_PROPERTY_TEXT_MULTI:
				pDC->DrawText( m_pCurDrawItem->csProperties.GetAt(0), m_pCurDrawItem->csProperties.GetAt(0).GetLength(), ItemRect, DT_VCENTER|DT_LEFT);	//monamy
				break;
		
		case	ID_PROPERTY_FONT:
				if(m_pCurDrawItem->LogFont.lfHeight)
					pDC->DrawText( m_pCurDrawItem->LogFont.lfFaceName, _tcsclen(m_pCurDrawItem->LogFont.lfFaceName), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				break;
	}

EFS_END
}
	
VOID CPropertyListCtrl::CreateControl(INT nType)
{
EFS_BEGIN

	switch(nType)
	{
		// Edit Window
		case	ID_PROPERTY_TEXT:
				if(!m_pEditWnd)
				{
					m_pEditWnd = new CEdit();
					m_pEditWnd->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT, CRect(0,0,100,100), this, ID_PROPERTY_TEXT);	//monamy
					m_pEditWnd->SetFont(m_pTextFont);
				}
				break;

		case    ID_PROPERTY_TEXT_MULTI:
				if(!m_pMultiEditWnd)
				{
					m_pMultiEditWnd = new CEdit();
					m_pMultiEditWnd->Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN , CRect(0,0,100,100), this, ID_PROPERTY_TEXT_MULTI);	//monamy
					m_pMultiEditWnd->SetFont(m_pTextFont);
				}
				break;

		// Colour Picker
		case	ID_PROPERTY_COLOR:
				if(!m_pColourPicker)
				{
					m_pColourPicker = new CColourPicker();
					m_pColourPicker->Create(_T("..."), WS_CHILD, CRect(0,0,100,100), this, ID_PROPERTY_COLOR);
					m_pColourPicker->SetFont(m_pTextFont);
				}
				break;

		// Font Button
		case	ID_PROPERTY_FONT:
				if(!m_pFontButton)
				{
					m_pFontButton = new CButton();
					m_pFontButton->Create(_T("..."), WS_CHILD|BS_PUSHBUTTON, CRect(0,0,100,100), this, ID_PROPERTY_FONT);
					m_pFontButton->SetFont(m_pTextFont);
				}
				break;

		case	ID_PROPERTY_PATH:
				if(!m_pEditWnd)
				{
					m_pEditWnd = new CEdit();
					m_pEditWnd->Create(WS_CHILD|ES_AUTOHSCROLL|ES_LEFT, CRect(0,0,100,100), this, ID_PROPERTY_TEXT);
					m_pEditWnd->SetFont(m_pTextFont);
				}
				if(!m_pPathButton)
				{
					m_pPathButton = new CButton();
					m_pPathButton->Create(_T("..."), WS_CHILD|BS_PUSHBUTTON, CRect(0,0,100,100), this, ID_PROPERTY_PATH);
					m_pPathButton->SetFont(m_pTextFont);
				}
				break;

		case	ID_PROPERTY_COMBO_LIST:
				if(!m_pEditWnd)
				{
					m_pEditWnd = new CEdit();
					m_pEditWnd->Create(WS_CHILD|ES_AUTOHSCROLL|ES_LEFT, CRect(0,0,100,100), this, ID_PROPERTY_TEXT);
					m_pEditWnd->SetFont(m_pTextFont);
				}
				if(!m_pListBox)
				{
					m_pListBox = new CListBox();
					m_pListBox->Create(WS_CHILD|WS_BORDER|LBS_NOTIFY|WS_VSCROLL|LBS_HASSTRINGS, CRect(0,0,100,500), this, ID_PROPERTY_COMBO_LIST);
					m_pListBox->SetFont(m_pTextFont);

					m_pComboButton = new CComboButton();
					m_pComboButton->Create(CRect(0,0,0,0), this, ID_PROPERTY_COMBO_BTN ); 
				}
				break;
	}

EFS_END
}

BOOL CPropertyListCtrl::SetProperty(PROPERTYITEM* pPropertyItem, INT nType, CString csData)
{
EFS_BEGIN

	switch(nType)
	{
		case	ID_PROPERTY_BOOL:
		case	ID_PROPERTY_PATH:
				ParseProperties(pPropertyItem, csData);

				// Is the item selected more than items in the array?
				if(pPropertyItem->csProperties.GetSize() != 2 )
					return FALSE;
				break;

		case	ID_PROPERTY_TEXT:
		case    ID_PROPERTY_TEXT_MULTI:
		case	ID_PROPERTY_STATIC:
				pPropertyItem->csProperties.Add(csData);
				break;

		case	ID_PROPERTY_FONT:
				memset(&pPropertyItem->LogFont, 0, sizeof(pPropertyItem->LogFont));
				if(m_pCurFont)
				{
					m_pCurFont->GetLogFont(&pPropertyItem->LogFont);			
					m_pCurFont = NULL;
				}
				break;

		case	ID_PROPERTY_COLOR:
				pPropertyItem->pBrush = m_pCurBrush;
				break;	

		case	ID_PROPERTY_COMBO_LIST:
				ParseProperties(pPropertyItem, csData);
				if(pPropertyItem->bComboSorted)
					pPropertyItem->csProperties.Sort();
				break;

	}

	return TRUE;

EFS_END
return FALSE;

}

VOID CPropertyListCtrl::ParseProperties(PROPERTYITEM* pPropertyItem, CString csData)
{
EFS_BEGIN

	// Parse the Items
	TCHAR* pText = csData.GetBuffer( csData.GetLength() );
	TCHAR* pWord;	
	TCHAR  Separations[]   = _T("!");

	// Establish string and get the first token:
	pWord = _tcstok( pText, Separations);
	while( pWord != NULL )
	{
		// Add this to the Array
		pPropertyItem->csProperties.Add(pWord);
		
		// Get next token
		pWord = _tcstok( NULL, Separations );
	}
	
	// Release the buffer
	csData.ReleaseBuffer();

EFS_END
}

VOID CPropertyListCtrl::HideControls()
{
EFS_BEGIN

	// Hide the controls
	if(m_pEditWnd)		m_pEditWnd->ShowWindow(SW_HIDE);
	if(m_pMultiEditWnd)	m_pMultiEditWnd->ShowWindow(SW_HIDE);
	if(m_pFontButton)	m_pFontButton->ShowWindow(SW_HIDE);
	if(m_pPathButton)	m_pPathButton->ShowWindow(SW_HIDE);
	if(m_pListBox)		m_pListBox->ShowWindow(SW_HIDE);
	if(m_pComboButton)	m_pComboButton->ShowWindow(SW_HIDE);	

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// Get Properties Functions
/////////////////////////////////////////////////////////////////////////////
CString CPropertyListCtrl::GetItemAlias(INT nItem)
{
EFS_BEGIN

	if(nItem + 1 > GetCount())
		return _T("");

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));

	if (pItem != NULL)
		return pItem->strAlias;
	else
		return _T("");

EFS_END
return _T("");

}
bool CPropertyListCtrl::GetProperty(INT nItem, CString* pText)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));
	if(pItem->nType != ID_PROPERTY_TEXT && pItem->nType != ID_PROPERTY_TEXT_MULTI && pItem->nType != ID_PROPERTY_PATH && pItem->nType != ID_PROPERTY_STATIC)
		return false;

	// Copy the item
	*pText = pItem->csProperties.GetAt(0);
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, CString* pText)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		// KOL.UDF.022
		//if (pItem == NULL && pItem->nType != ID_PROPERTY_TEXT && pItem->nType != ID_PROPERTY_TEXT_MULTI && pItem->nType != ID_PROPERTY_PATH && pItem->nType != ID_PROPERTY_STATIC)
		if (&pItem == NULL && pItem->nType != ID_PROPERTY_TEXT && pItem->nType != ID_PROPERTY_TEXT_MULTI && pItem->nType != ID_PROPERTY_PATH && pItem->nType != ID_PROPERTY_STATIC)
			continue;
		if (pItem->strAlias == strAlias)
		{
			*pText = pItem->csProperties.GetAt(0);
			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(INT nItem, bool* bValue)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));
	if(pItem->nType != ID_PROPERTY_BOOL)
		return false;

	// Copy the item
	//*bValue = (bool)pItem->nPropertySelected;
	if (pItem->nPropertySelected == 0)
		*bValue = false;
	else 
		*bValue = true;
	
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, bool* bValue)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		// KOL.UDF.022
		//if (pItem == NULL && pItem->nType != ID_PROPERTY_BOOL)
		if (&pItem == NULL && pItem->nType != ID_PROPERTY_BOOL)
			continue;
		if (pItem->strAlias == strAlias)
		{
			if (pItem->nPropertySelected == 0)
				*bValue = false;
			else 
				*bValue = true;
			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(INT nItem, COLORREF* crColor)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));
	if(pItem->nType != ID_PROPERTY_COLOR)
		return false;

	// Copy the item
	LOGBRUSH LogBrush;
	pItem->pBrush->GetLogBrush(&LogBrush);
	*crColor = LogBrush.lbColor;
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, COLORREF* crColor)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		// KOL.UDF.022
		//if (pItem == NULL && pItem->nType != ID_PROPERTY_COLOR)
		if (&pItem == NULL && pItem->nType != ID_PROPERTY_COLOR)
			continue;
		if (pItem->strAlias == strAlias)
		{
			// Copy the item
			LOGBRUSH LogBrush;
			pItem->pBrush->GetLogBrush(&LogBrush);
			*crColor = LogBrush.lbColor;
			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(INT nItem, LOGFONT* LogFont, INT *nFontSize, COLORREF *pColor)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));
	if(pItem->nType != ID_PROPERTY_FONT)
		return false;

	// Copy the item
	*LogFont = pItem->LogFont;
	*nFontSize = pItem->nFontSize;
	*pColor = pItem->colorFont;
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, LOGFONT* LogFont, INT *nFontSize, COLORREF *pColor)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		// KOL.UDF.022
//		if (pItem == NULL && pItem->nType != ID_PROPERTY_FONT)
		if (&pItem == NULL && pItem->nType != ID_PROPERTY_FONT)
			continue;
		if (pItem->strAlias == strAlias)
		{
			// Copy the item
			*LogFont = pItem->LogFont;
			*nFontSize = pItem->nFontSize;
			*pColor = pItem->colorFont;
			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(INT nItem, CStringArray* pArray, INT* SelectedItem)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));
	if(pItem->nType != ID_PROPERTY_COMBO_LIST)
		return false;

	// I do NOT want to send them a pointer to my array so I loop through and copy the item to thiers
	for( INT nString = 0; nString < pItem->csProperties.GetSize(); nString++)
		pArray->Add(pItem->csProperties.GetAt(nString));
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, CStringArray* pArray, INT* SelectedItem)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		// KOL.UDF.022
		//if (pItem == NULL && pItem->nType != ID_PROPERTY_COMBO_LIST)
		if (&pItem == NULL && pItem->nType != ID_PROPERTY_COMBO_LIST)
			continue;
		if (pItem->strAlias == strAlias)
		{
			// I do NOT want to send them a pointer to my array so I loop through and copy the item to thiers
			for( INT nString = 0; nString < pItem->csProperties.GetSize(); nString++)
				pArray->Add(pItem->csProperties.GetAt(nString));
			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(INT nItem, INT* SelectedItem, CString* pText)
{
EFS_BEGIN

	// is the item to high
	if(nItem + 1 > GetCount())
		return false;

	// Make sure this item is the correct type
	PROPERTYITEM* pItem;
	pItem = (PROPERTYITEM*)m_Items.GetAt(m_Items.FindIndex(nItem));

	// Copy the item
	*SelectedItem = pItem->nPropertySelected;

	// Do they want the text
	if(pText != NULL && pItem->nType == ID_PROPERTY_COMBO_LIST)
		*pText = pItem->csProperties.GetAt(pItem->nPropertySelected);
	return true;

EFS_END
return FALSE;

}

bool CPropertyListCtrl::GetProperty(CString strAlias, INT* SelectedItem, CString* pText)
{
EFS_BEGIN

	POSITION pos = m_Items.GetHeadPosition();
	while (pos != NULL)
	{
		PROPERTYITEM* pItem = (PROPERTYITEM*)m_Items.GetNext(pos);
		if (pItem == NULL)
			continue;
		if (pItem->strAlias == strAlias)
		{
			// Copy the item
			*SelectedItem = pItem->nPropertySelected;
			// Do they want the text
			if(pText != NULL && pItem->nType == ID_PROPERTY_COMBO_LIST)
				*pText = pItem->csProperties.GetAt(pItem->nPropertySelected);

			return true;
		}
	}
	return false;

EFS_END
return FALSE;

}
