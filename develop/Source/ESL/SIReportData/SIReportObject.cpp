// SIReportObject.cpp: implementation of the CSIReportObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject, CObject)

CSIReportObject::CSIReportObject()
{
	m_pParenWnd = NULL;
	m_pParentObject = NULL;
	
	m_colorBk = RGB(255, 255, 255);
	m_sizePickPoint = CSize(0, 0);

	m_bIsDragging = FALSE;
	m_bIsResizing = FALSE;
	m_ResizeMode = NONE;

	// 첫페이지만 보이기 ( 기본값은 0 - 모든페이지 보이기)
	m_nOnlyShowFirstPage = 0;
}

VOID CSIReportObject::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	pObject->SetObjType(GetObjType());
	pObject->SetBkColor(GetBkColor());
	pObject->SetRect(GetRect());
	pObject->SetName(GetName());

EFS_END
}

CSIReportObject::~CSIReportObject()
{

}

CSIReportObject& CSIReportObject::operator =(CSIReportObject& rhs)
{
	if (this == &rhs)
		return *this;
	
	m_colorBk = rhs.m_colorBk;
	m_nObjType = rhs.m_nObjType;
	m_pParentObject = rhs.m_pParentObject;
	
	m_pointDrag = rhs.m_pointDrag;
	m_pointResizing = rhs.m_pointResizing;
	
	m_pParentObject = rhs.m_pParentObject;
	m_pParenWnd = rhs.m_pParenWnd;
	
	m_rect = rhs.m_rect;

	m_rectDrag = rhs.m_rectDrag;
	m_rectResize = rhs.m_rectResize;

	m_ResizeMode = rhs.m_ResizeMode;
	m_sizePickPoint = rhs.m_sizePickPoint;

	m_strName = rhs.m_strName;
	m_strParentName = rhs.m_strParentName;

	return *this;
}

BOOL CSIReportObject::Create(CRect rect, CWnd *pParentWnd /*=NULL*/, CSIReportObject *pParentObject /*=NULL*/)
{
EFS_BEGIN

	SetRect(rect);

	m_pParenWnd = pParentWnd;
	m_pParentObject = pParentObject;

	m_rectDrag = rect;
	m_rectResize = rect;
	m_pointDrag = CPoint(0, 0);
	
	return TRUE;

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CMObject Operation

VOID CSIReportObject::InitDrag(POINT point)
{
EFS_BEGIN

	m_rectDrag = m_rect; 
	m_bIsDragging = TRUE;
	m_pointDrag = point;

EFS_END
}

VOID CSIReportObject::InitResize(INT resizeMode, POINT point)
{
EFS_BEGIN

	m_rectResize = m_rect;
	m_bIsResizing = TRUE;
	m_ResizeMode = (eResizeMode)resizeMode;
	m_pointResizing = point;

EFS_END
}

CRect CSIReportObject::GetResizeRect()
{
	INT nTemp;
	if (m_rectResize.top > m_rectResize.bottom)
	{
		nTemp = m_rectResize.top;
		m_rectResize.top = m_rectResize.bottom;
		m_rectResize.bottom = nTemp;
	}
	if (m_rectResize.left > m_rectResize.right)
	{
		nTemp = m_rectResize.left;
		m_rectResize.left = m_rectResize.right;
		m_rectResize.right = nTemp;
	}
	return m_rectResize;
}

BOOL CSIReportObject::PtInSIReportObject(POINT point)
{
EFS_BEGIN

	if (GetRect().PtInRect(point))
	{
		return TRUE;
	}
	else if (PtInResizeRect(point))
		return TRUE;
	else
		return FALSE;

EFS_END
return FALSE;

}

INT CSIReportObject::PtInResizeRect(POINT point)
{
EFS_BEGIN

	CRect rect;
	
	// LEFT_TOP
	rect = GetResizePickRect(CPoint(GetRect().left, GetRect().top));
	if (rect.PtInRect(point)) return LEFT_TOP;
	// MIDDLE_TOP
	rect = GetResizePickRect(CPoint(GetRect().left + GetRect().Width()/2, GetRect().top));
	if (rect.PtInRect(point)) return MIDDLE_TOP;
	// RIGHT_TOP
	rect = GetResizePickRect(CPoint(GetRect().right, GetRect().top));
	if (rect.PtInRect(point)) return RIGHT_TOP;
	// LEFT_MIDDLE
	rect = GetResizePickRect(CPoint(GetRect().left, GetRect().top + GetRect().Height()/2));
	if (rect.PtInRect(point)) return LEFT_MIDDLE;
	// RIGHT_MIDDLE
	rect = GetResizePickRect(CPoint(GetRect().right, GetRect().top + GetRect().Height()/2));
	if (rect.PtInRect(point)) return RIGHT_MIDDLE;
	// LEFT_BOTTOM
	rect = GetResizePickRect(CPoint(GetRect().left, GetRect().bottom));
	if (rect.PtInRect(point)) return LEFT_BOTTOM;
	// MIDDLE_BOTTOM
	rect = GetResizePickRect(CPoint(GetRect().left + GetRect().Width()/2, GetRect().bottom));
	if (rect.PtInRect(point)) return MIDDLE_BOTTOM;
	// RIGHT_BOTTOM
	rect = GetResizePickRect(CPoint(GetRect().right, GetRect().bottom));
	if (rect.PtInRect(point)) return RIGHT_BOTTOM;

	return NONE;

EFS_END
return -1;

}

CRect CSIReportObject::GetResizePickRect(POINT point)
{
	CRect rect;

	rect.left = point.x - m_sizePickPoint.cx/2;
	rect.top = point.y - m_sizePickPoint.cy/2;
	rect.right = point.x + m_sizePickPoint.cx/2;
	rect.bottom = point.y + m_sizePickPoint.cy/2;

	return rect;
}

/////////////////////////////////////////////////////////////////////////////
// CMObject Draw
VOID CSIReportObject::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	return;

EFS_END
}

VOID CSIReportObject::DrawWidget(CDC *pDC, CRect rect, BOOL bIsVisible /*= TRUE*/, BOOL bHorzLine /*= TRUE */, BOOL bVertLine /*= TRUE*/)
{
EFS_BEGIN
	if (!bIsVisible) return;

	CPen* pOldPen = pDC->GetCurrentPen();
	CPen penInvisible(PS_DOT, 0, RGB(255, 0, 0));
	
	pDC->MoveTo(rect.left, rect.top);
	
	if (!bHorzLine)
		pOldPen = pDC->SelectObject(&penInvisible);
	else 
		pDC->SelectObject(pOldPen);
	
	pDC->LineTo(rect.right, rect.top);

	pDC->SelectObject(pOldPen);

	if (!bVertLine)
		pOldPen = pDC->SelectObject(&penInvisible);
	else 
		pDC->SelectObject(pOldPen);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->SelectObject(pOldPen);

	if (!bHorzLine)
		pOldPen = pDC->SelectObject(&penInvisible);
	else 
		pDC->SelectObject(pOldPen);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->SelectObject(pOldPen);

	if (!bVertLine)
		pOldPen = pDC->SelectObject(&penInvisible);
	else 
		pDC->SelectObject(pOldPen);
	pDC->LineTo(rect.left, rect.top);
	pDC->SelectObject(pOldPen);

	pDC->SelectObject(pOldPen);
EFS_END
}

VOID CSIReportObject::DrawSelected(CDC *pDC, CSize sizePickPoint, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	m_sizePickPoint = sizePickPoint;
    
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
	
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left, GetRect().top)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left + GetRect().Width()/2, GetRect().top)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().right, GetRect().top)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left, GetRect().top + GetRect().Height()/2)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().right, GetRect().top + GetRect().Height()/2)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left, GetRect().bottom)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().left + GetRect().Width()/2, GetRect().bottom)));
	pDC->Rectangle(GetResizePickRect(CPoint(GetRect().right, GetRect().bottom)));

	pDC->SelectObject(pOldBrush);

EFS_END
}

VOID CSIReportObject::DrawResizeImage(CDC *pDC, POINT point, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	if (m_pointResizing == point) return;
	
	CPen pen(PS_DOT, 0, RGB(0,0,0));

	INT		nOldMode	= pDC->SetROP2 (R2_NOT);
	CBrush* pOldBrush	= (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	CPen*	pOldPen		= pDC->SelectObject(&pen);
	
	pDC->SetBkMode(TRANSPARENT);

	// Erase Old Object
	DrawWidget(pDC, m_rectResize, bIsVisible);
	
	// Calculate ..
	INT nVertMove = m_pointResizing.y - point.y;
	INT nHorzMove = m_pointResizing.x - point.x;

	// switch mode
	switch(m_ResizeMode)
	{
	case MIDDLE_TOP:
		m_rectResize.top -= nVertMove;
		break;

	case MIDDLE_BOTTOM:
		m_rectResize.bottom -= nVertMove;
		break;

	case LEFT_MIDDLE:
		m_rectResize.left -= nHorzMove;
		break;

	case RIGHT_MIDDLE:
		m_rectResize.right -= nHorzMove;
		break;

	case LEFT_TOP:
		m_rectResize.left -= nHorzMove;
		m_rectResize.top -= nVertMove;
		break;

	case RIGHT_TOP:
		m_rectResize.right -= nHorzMove;
		m_rectResize.top -= nVertMove;
		break;

	case LEFT_BOTTOM:
		m_rectResize.left -= nHorzMove;
		m_rectResize.bottom -= nVertMove;
		break;

	case RIGHT_BOTTOM:
		m_rectResize.right -= nHorzMove;
		m_rectResize.bottom -= nVertMove;
		break;
	}

	m_pointResizing = point;
		
	// Draw New Object
	DrawWidget(pDC, m_rectResize, bIsVisible);
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SetROP2 (nOldMode);

EFS_END
}

VOID CSIReportObject::DrawDragImage(CDC *pDC, POINT point, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	if (m_pointDrag == point) return;

	CPen pen(PS_DOT, 0, RGB(0,0,0));
	INT nOldMode = pDC->SetROP2 (R2_NOT);
	CPen *pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	
	// Erase Old Object
	DrawWidget(pDC, m_rectDrag, bIsVisible);
	
	// Calculate ..
	INT nVertMove = m_pointDrag.y - point.y;
	INT nHorzMove = m_pointDrag.x - point.x;

	m_rectDrag.left -= nHorzMove;
	m_rectDrag.top -= nVertMove;
	m_rectDrag.right -= nHorzMove;
	m_rectDrag.bottom -= nVertMove;
	
	m_pointDrag = point;
	
	// Draw New Object
	DrawWidget(pDC, m_rectDrag, bIsVisible);
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SetROP2 (nOldMode);

EFS_END
}

INT CSIReportObject::ReCalcAllRect(INT nLeft, INT nTop)
{
EFS_BEGIN

	if (m_nObjType == OBJECT_TABLE) return -1;

	m_rect.top += nTop;
	m_rect.bottom += nTop;

	m_rect.left += nLeft;
	m_rect.right += nLeft;
	return 0;

EFS_END
return -1;

}

