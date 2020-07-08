// SIReportObject_Line.cpp: implementation of the CSIReportObject_Line class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Line, CSIReportObject)
CSIReportObject_Line::CSIReportObject_Line()
{
	SetObjType(OBJECT_LINE);

	m_sizePickPoint = CSize(5, 5);

	m_strName = _T("LINE");
	style = 0;
	thick = 0;
	visible = 1;
}

VOID CSIReportObject_Line::Clone(CSIReportObject *pObject)
{
EFS_BEGIN
	CSIReportObject::Clone(pObject);

	((CSIReportObject_Line*)pObject)->style = style;
	((CSIReportObject_Line*)pObject)->thick = thick;

EFS_END
}

CSIReportObject_Line::~CSIReportObject_Line()
{

}

/////////////////////////////////////////////////////////////////////////////
// CMObject Operation
CRect CSIReportObject_Line::GetResizeRect()
{
	return m_rectResize;
}

BOOL CSIReportObject_Line::PtInSIReportObject(POINT point)
{
EFS_BEGIN

	CRect rect = GetRect();
	rect.InflateRect(3, 3);
	
	CRgn rgn;
	rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

	rgn.GetRgnBox(&rect);
	

	//if (!rect.PtInRect(point)) return FALSE;

	if (GetRect().Width() == 0)
	{
		if (point.x > rect.left - 1 && point.x < rect.right + 1) return true;
	}		
	else
	{
		DOUBLE nInclinedRate = 0.0;
		nInclinedRate = (double)rect.Height() / (double)rect.Width();

		INT nX;
		INT nY;

		rect.top < rect.bottom ? nX = rect.top : nX = rect.bottom;
		rect.left < rect.right ? nY = rect.left : nY = rect.right;
			
		//double b = (double)rect.top - nInclinedRate*(double)rect.left;
		double b = (double)nX - nInclinedRate*(double)nY;
		double y = nInclinedRate * (double)point.x + b;
		if (point.y < y + m_sizePickPoint.cy && point.y > y - m_sizePickPoint.cy) 
			return TRUE;
	}
		
	if (PtInResizeRect(point))
		return TRUE;

	return FALSE;

EFS_END
return FALSE;
}

INT CSIReportObject_Line::PtInResizeRect(POINT point)
{
EFS_BEGIN
	CRect rect;

	// LEFT_TOP
	rect = GetResizePickRect(CPoint(GetRect().left, GetRect().top));
	if (rect.PtInRect(point)) return LEFT_TOP;
	
	// RIGHT_BOTTOM
	rect = GetResizePickRect(CPoint(GetRect().right, GetRect().bottom));
	if (rect.PtInRect(point)) return RIGHT_BOTTOM;

	return NONE;

EFS_END
return -1;
}
/////////////////////////////////////////////////////////////////////////////
// CMObject Draw
VOID CSIReportObject_Line::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN
	CPen pen(style, thick, RGB(0,0,0));
	CPen *pOldPen = pDC->SelectObject(&pen);
	
	DrawWidget(pDC, GetRect());
	
	pDC->SelectObject(pOldPen);	

EFS_END
}

VOID CSIReportObject_Line::DrawWidget(CDC *pDC, CRect rect, BOOL bIsVisible /*= TRUE*/, BOOL bHorzLine /*= TRUE */, BOOL bVertLine /*= TRUE*/)
{
EFS_BEGIN
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.BottomRight());
EFS_END
}

VOID CSIReportObject_Line::DrawSelected(CDC *pDC, CSize sizePickPoint, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN
	m_sizePickPoint = sizePickPoint;
    
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
	
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left, GetRect().top)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().right, GetRect().bottom)));
	
	pDC->SelectObject(pOldBrush);
EFS_END
}
