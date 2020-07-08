// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
	m_crButtonColor = RGB(0,0,0);
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CRect rect;
	GetClientRect(rect);
	
	CBrush Brush;
	Brush.CreateSolidBrush(m_crButtonColor);
//	CPen Pen;
//	Pen.CreatePen(PS_SOLID,0,m_crButtonColor);

	CDC* pDC = GetDC();
	CBrush* OldBrush = pDC->SelectObject(&Brush);
//	CPen* OldPen = pDC->SelectObject(&Pen);

	pDC->Rectangle(rect);	
	pDC->SelectObject(&OldBrush);
//	pDC->SelectObject(&OldPen);
}

void CColorButton::SetButtonColor(COLORREF crNewColor)
{
	m_crButtonColor = crNewColor;
	DrawItem(NULL);
}
