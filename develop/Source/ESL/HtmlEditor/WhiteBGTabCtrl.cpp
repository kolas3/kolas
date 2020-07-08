// WhiteBGTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "htmledit.h"
#include "WhiteBGTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhiteBGTabCtrl

CWhiteBGTabCtrl::CWhiteBGTabCtrl()
{
}

CWhiteBGTabCtrl::~CWhiteBGTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CWhiteBGTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CWhiteBGTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhiteBGTabCtrl message handlers

//DEL HBRUSH CWhiteBGTabCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//DEL {
//DEL 	HBRUSH hbr = CTabCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
//DEL 	
//DEL 	// TODO: Change any attributes of the DC here
//DEL //	if(nCtlColor ==CTLCOLOR_DLG)
//DEL 	{
//DEL 		hbr = CreateSolidBrush(RGB(255,255,255)); 	
//DEL 		return hbr;
//DEL 	}
//DEL 	// TODO: Return a different brush if the default is not desired
//DEL 	return hbr;
//DEL }

BOOL CWhiteBGTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255)); 	
	CBrush* pOldBrush = pDC->SelectObject(&br);
	CRect rect;
	GetClientRect(rect);
	pDC->Rectangle(rect);
	return CTabCtrl::OnEraseBkgnd(pDC);
}

HBRUSH CWhiteBGTabCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
//	switch(nCtlColor)
	{		
//	case CTLCOLOR_STATIC:		
//	case CTLCOLOR_DLG:		
//		pDC->SetBkMode(pDC->m_hDC, TRANSPARENT );		
//		return (HBRUSH)::GetStockObject (NULL_BRUSH);
//		hbr = GetSysColorBrush( IsAppThemed() ? COLOR_WINDOW : COLOR_BTNFACE ); 		
	} 	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//DEL void CWhiteBGTabCtrl::OnPaint() 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	CTabCtrl::OnPaint();
//DEL 	// TODO: Add your message handler code here
//DEL 	/*
//DEL 	CRect rect;
//DEL 	GetClientRect(rect);
//DEL 	CDC* pDC = GetDC();
//DEL 	CPen Pen, *OldPen;
//DEL 	CBrush Brush, *OldBrush;
//DEL 	Pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
//DEL 	Brush.CreateSolidBrush(RGB(255,255,255));
//DEL 	OldPen = pDC->SelectObject(&Pen);
//DEL 	OldBrush = pDC->SelectObject(&Brush);	
//DEL 	pDC->Rectangle(rect);
//DEL 	*/
//DEL 	// Do not call CTabCtrl::OnPaint() for painting messages
//DEL }
