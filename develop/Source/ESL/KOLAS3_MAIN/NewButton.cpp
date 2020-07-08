// NewButton.cpp : implementation file
//

#include "stdafx.h"
#include "NewButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewButton

CNewButton::CNewButton()
{
	m_pBitmapUp = NULL;
	m_pBitmapDown = NULL;
	m_pBitmapFocus = NULL;

	m_bHover = FALSE;
	m_bTracking = FALSE;
}

CNewButton::~CNewButton()
{
	FreeBitmaps();
}


BEGIN_MESSAGE_MAP(CNewButton, CBitmapButton)
	//{{AFX_MSG_MAP(CNewButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewButton message handlers

VOID CNewButton::FreeBitmaps(VOID)
{
	if (m_pBitmapUp) {
		delete m_pBitmapUp;
		m_pBitmapUp = NULL;
	}
	if (m_pBitmapDown) {
		delete m_pBitmapDown;
		m_pBitmapDown = NULL;
	}
	if (m_pBitmapFocus) {
		delete m_pBitmapFocus;
		m_pBitmapFocus = NULL;
	}
}

BOOL CNewButton::LoadBitmap(CString bitmapup, CString bitmapdown, CString bitmapfocus)
{
	FreeBitmaps();
	m_pBitmapUp = new CBitmap;
	m_pBitmapDown = new CBitmap;
	m_pBitmapFocus = new CBitmap;

	m_pBitmapUp->Attach(::LoadImage(NULL,bitmapup, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	m_pBitmapDown->Attach(::LoadImage(NULL,bitmapdown, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	m_pBitmapFocus->Attach(::LoadImage(NULL,bitmapfocus, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));

	BITMAP	bitmapbits;
	m_pBitmapUp->GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
	m_ButtonSize.cx=bitmapbits.bmWidth;
	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER   );
	return TRUE;
}

VOID CNewButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}	
	CBitmapButton::OnMouseMove(nFlags, point);
}

VOID CNewButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();
}


LRESULT CNewButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

BOOL CNewButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBitmapButton::PreTranslateMessage(pMsg);
}

VOID CNewButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(mydc);

	CBitmap * pOldBitmap;
	
	CPoint point(0,0);	
		
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		pOldBitmap = pMemDC -> SelectObject(m_pBitmapDown);
//		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
	}
	else
	{
		if(m_bHover)
		{
			pOldBitmap = pMemDC -> SelectObject(m_pBitmapFocus);
//			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}else
		{
			pOldBitmap = pMemDC -> SelectObject(m_pBitmapUp);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}	
	}

	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
}
