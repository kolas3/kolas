// ButtonMonth.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "ButtonMonth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonMonth

CButtonMonth::CButtonMonth()
{
	CTime t = CTime::GetCurrentTime();
	m_uMonth = t.GetMonth();
}

CButtonMonth::~CButtonMonth()
{
}


BEGIN_MESSAGE_MAP(CButtonMonth, CButton)
	//{{AFX_MSG_MAP(CButtonMonth)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonMonth message handlers

void CButtonMonth::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CClientDC dc(this);	
	CBitmap MemBitmap, Bitmap;
	CDC DC;
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, 82, 46);
	MemDC.SelectObject(&MemBitmap);

	DC.CreateCompatibleDC(&dc);	
	Bitmap.LoadBitmap(IDB_BITMAP_MONTH_1 + (m_uMonth -1));		
	DC.SelectObject(&Bitmap);
	
	MemDC.BitBlt(0,0,82,46, &DC,0,0,SRCCOPY);

	CDC* pDC = GetDC();
	pDC->BitBlt(0,0,82,46,&MemDC,0,0,SRCCOPY);
	ReleaseDC(pDC);
}

void CButtonMonth::SetMonth(UINT uMonth)
{
	m_uMonth = uMonth;
}


HBRUSH CButtonMonth::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CButtonMonth::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//DEL 	return CButton::OnEraseBkgnd(pDC);
}
