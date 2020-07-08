// ButtonMonthEng.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "ButtonMonthEng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonMonthEng

CButtonMonthEng::CButtonMonthEng()
{
	// KOL.UDF.022
	m_uMonth = 0;
}

CButtonMonthEng::~CButtonMonthEng()
{
}


BEGIN_MESSAGE_MAP(CButtonMonthEng, CButton)
	//{{AFX_MSG_MAP(CButtonMonthEng)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonMonthEng message handlers

void CButtonMonthEng::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CClientDC dc(this);	
	CBitmap MemBitmap, Bitmap;
	CDC DC;
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, 92, 15);
	MemDC.SelectObject(&MemBitmap);

	DC.CreateCompatibleDC(&dc);	
	Bitmap.LoadBitmap(IDB_BITMAP_MONTH_ENG_1 + (m_uMonth -1));		
	DC.SelectObject(&Bitmap);
	
	MemDC.BitBlt(0,0,92,15, &DC,0,0,SRCCOPY);

	CDC* pDC = GetDC();
	pDC->BitBlt(0,0,92,15,&MemDC,0,0,SRCCOPY);
	ReleaseDC(pDC);
}

void CButtonMonthEng::SetMonth(UINT uMonth)
{
	m_uMonth = uMonth;
}

HBRUSH CButtonMonthEng::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CButtonMonthEng::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;	
//DEL 	return CButton::OnEraseBkgnd(pDC);
}
