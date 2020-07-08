#include "stdafx.h"
#include "Ctrl_ColorGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCtrl_ColorGroupBox::CCtrl_ColorGroupBox()
{
	
	m_crGroupBoxBKColor   = RGB(240,240,240);	
	m_crGroupBoxLineColor = RGB(180,180,180);	

}

CCtrl_ColorGroupBox::~CCtrl_ColorGroupBox()
{
}


BEGIN_MESSAGE_MAP(CCtrl_ColorGroupBox, CButton)
	//{{AFX_MSG_MAP(CCtrl_ColorGroupBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CCtrl_ColorGroupBox::OnPaint() 
{
	CPaintDC dc(this); 

	CRect rc;
	GetClientRect(rc);

	CFont* pFont = GetFont();
	
	if( pFont )
	{
		LOGFONT LogFont;	
		pFont->GetLogFont(&LogFont);
		INT nfHeight = (0>LogFont.lfHeight) ? - (LogFont.lfHeight) : LogFont.lfHeight;
		rc.top = nfHeight/2;
	}

	rc.DeflateRect(1,1);
	
	dc.FillSolidRect(rc, m_crGroupBoxBKColor);
	DrawLine(&dc, CPoint(rc.left,rc.top-1), CPoint(rc.right,rc.top-1), m_crGroupBoxLineColor, 1);	
	DrawLine(&dc, CPoint(rc.right,rc.top), CPoint(rc.right,rc.bottom), m_crGroupBoxLineColor, 1);	
	DrawLine(&dc, CPoint(rc.left,rc.bottom), CPoint(rc.right,rc.bottom), m_crGroupBoxLineColor, 1);	
	DrawLine(&dc, CPoint(rc.left-1,rc.top), CPoint(rc.left-1,rc.bottom), m_crGroupBoxLineColor, 1);

}

VOID CCtrl_ColorGroupBox::DrawLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF crLineColor, INT nLineWidth/*=1*/)
{
	CPen pPen, *pOldPen;
	pPen.CreatePen(PS_SOLID, nLineWidth, crLineColor);
	pOldPen = pDC->SelectObject(&pPen);

	pDC->MoveTo(ptStart.x, ptStart.y);
	pDC->LineTo(ptEnd.x, ptEnd.y);

	pDC->SelectObject(pOldPen);
	pPen.DeleteObject();
}

