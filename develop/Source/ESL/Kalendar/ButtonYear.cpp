// ButtonYear.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "ButtonYear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonYear

CButtonYear::CButtonYear()
{
	m_uYear = 0;
}

CButtonYear::~CButtonYear()
{
}


BEGIN_MESSAGE_MAP(CButtonYear, CButton)
	//{{AFX_MSG_MAP(CButtonYear)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonYear message handlers

void CButtonYear::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CClientDC dc(this);	
	CBitmap MemBitmap, Bitmap[4];
	CDC DC[4];
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, 40, 17);
	MemDC.SelectObject(&MemBitmap);

	UINT uNum[4] = {0,0,0,0};
	UINT uTmp = m_uYear;
	for(UINT i=0;i<4;i++)
	{
		uNum[3-i] = uTmp % 10;
		uTmp = uTmp / 10;
	}
	for(i=0;i<4;i++)
	{
		DC[i].CreateCompatibleDC(&dc);	
		Bitmap[i].LoadBitmap(IDB_BITMAP_YEAR_0 + uNum[i]);		
		DC[i].SelectObject(&Bitmap[i]);
	}
	for(i=0;i<4;i++)
	{
		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
	}	
	CDC* pDC = GetDC();
	pDC->BitBlt(0,0,40,17,&MemDC,0,0,SRCCOPY);
	ReleaseDC(pDC);

}

void CButtonYear::SetYear(UINT uYear)
{
	m_uYear = uYear;
}

HBRUSH CButtonYear::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CButtonYear::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
//DEL 	return CButton::OnEraseBkgnd(pDC);
}
