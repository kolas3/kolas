//File: ToolTip2.cpp
//Implementation file for class CBitToolTip
//Copyright (C) 2000  Dhandapani Ammasai( dammasai@hotmail.com )
//Mail your comments, criticism, bugs to the above e-mail address.
#include "stdafx.h"
#include "bittooltip.h"

IMPLEMENT_DYNAMIC( CBitToolTip, CWnd );

BEGIN_MESSAGE_MAP(CBitToolTip, CWnd)
	//{{AFX_MSG_MAP(CBitToolTip)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Default Constructor
CBitToolTip::CBitToolTip()
{
	m_szText = _T("");
	m_pParentWnd = NULL;
	m_bShowStatus = FALSE;
	m_bActive = FALSE;
}

//Destructor
CBitToolTip::~CBitToolTip()
{
}

//Create Tool Tip
BOOL CBitToolTip::Create(CWnd* pParentWnd)
{
	ASSERT(this != NULL );
	ASSERT(pParentWnd != NULL);

//	m_pBitmap.LoadBitmap(IDB_TTT);

	m_pParentWnd = pParentWnd;
	//Create font
	m_font.CreateFont(15, 0, 0, 0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0, _T("MS Sans Serif"));
	CRect rectInitialSize(0,0,0,0);	//Initial Window size. Will be dynamically changed later.
	return CreateEx(NULL, NULL, NULL,WS_POPUP |  WS_CHILD | WS_CLIPSIBLINGS,
		rectInitialSize,pParentWnd, NULL, NULL);
}

//Set tooltip text
VOID CBitToolTip::SetText(const CString& rsText)
{
	ASSERT(this != NULL );
	m_szText = rsText;
	m_szText.TrimRight();
}

//Show tooltip
//rCurrentPoint specifies current mouse position and it is in client 
//coordinates of parent window(Not in screen coordinates).
BOOL CBitToolTip::Show(const CPoint& rCurrentPoint, CBitmap* m_Math)
{
	ASSERT(this != NULL );
//	ASSERT(m_hWnd != NULL );

	m_preMath = m_Math;
	CPoint point;
	SIZE  sz = {20, 10};
//	sz.cx = 10;
//	sz.cy = 10;

	m_bActive = TRUE;
	point = rCurrentPoint + sz;
	
	//Is text empty or tool tip already displayed?
	if ( m_bShowStatus)
		Close();
//		return FALSE;

	m_ptCurrent = point;
	m_bShowStatus = TRUE;
	//Show tool tip
	DisplayToolTip(point, m_Math);
	
	return TRUE;
}

//Close the tooltip
VOID CBitToolTip::Close()
{
	ASSERT(this != NULL );
	ASSERT(m_hWnd != NULL );

	ShowWindow(SW_HIDE); //Hide tooltip
	m_bShowStatus = FALSE;
}

VOID CBitToolTip::OnPaint()
{
	CPaintDC dc(this);

	DisplayToolTip(m_ptCurrent ,m_preMath);
}

//Display tooltip
VOID CBitToolTip::DisplayToolTip(const CPoint& rCurrentPoint, CBitmap* m_pBitmap)
{
	CDC* pDC = GetDC();
	CSize size;
	BITMAP bitmap;
	m_pBitmap->GetBitmap(&bitmap);
	
	CRect rectToolTip(rCurrentPoint.x, rCurrentPoint.y, 
		rCurrentPoint.x+bitmap.bmWidth + 2, rCurrentPoint.y+bitmap.bmHeight+2);

	pDC->SetBkMode(TRANSPARENT);

	CDC pMemDC;
	pMemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldbitmap = pMemDC.SelectObject(m_pBitmap);
	pDC->Rectangle(0,0,rectToolTip.Width(),rectToolTip.Height());
	pDC->BitBlt(1,1,bitmap.bmWidth, bitmap.bmHeight,&pMemDC,0,0,SRCCOPY);

	

	CRect rectWnd = rectToolTip;
	m_pParentWnd->ClientToScreen(rectWnd); //Convert from client to screen coordinates
	CPoint ptToolTipLeft = rectWnd.TopLeft();

	//Now display tooltip
	SetWindowPos(&wndTop,ptToolTipLeft.x+1, ptToolTipLeft.y+1, rectWnd.Width(), 
//		rectWnd.Height(), SWP_SHOWWINDOW|SWP_NOACTIVATE);
		rectWnd.Height(), SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
	pMemDC.SelectObject(pOldbitmap);
	ReleaseDC(pDC);

}


