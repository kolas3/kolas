// ASCIIObject.cpp: implementation of the CASCIIObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "BARObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarObject::CBarObject()
{
	SetSize(40, 40);
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_bLeft = FALSE;
	m_iType = -1;
	m_bMiddle = FALSE;
}

CBarObject::~CBarObject()
{
	
}

VOID CBarObject::GetSize(RECT &rect)
{
	rect.right = 40;
	rect.top = 40;

	rect.bottom = 0;
	rect.left = 0;
}

CString CBarObject::GetObject()
{
	CString object = _T("Not Implemented[BarObject]");

	if(m_iType == 0) {
		if(m_bLeft == true) 
			object = _T("\\overline { ");
		else
			object = _T(" }");
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) 
			object = _T("\\underline { ");
		else
			object = _T(" }");
	}
	else if(m_iType == 2) {
		if(m_bLeft == true) 
			object = _T("\\overleftarrow { ");
		else
			object = _T(" }");
	}
	else if(m_iType == 3) {
		if(m_bLeft == true) 
			object = _T("\\overleftarrow { ");
		else
			object = _T(" }");
	}
	else if(m_iType == 4) {
		if(m_bLeft == true) 
			object = _T("\\underleftarrow { ");
		else
			object = _T(" }");
	}
	else if(m_iType == 5) {
		if(m_bLeft == true) 
			object = _T("\\underleftarrow { ");
		else
			object = _T(" }");
	}
	return object;
}

VOID CBarObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}

VOID CBarObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}

VOID CBarObject::SetType(INT type)
{
	m_iType = type;
}

bool CBarObject::IsVisible()
{
	return m_bVisible;
}

VOID CBarObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CBarObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[1] = 0;

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 20;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	if(m_iType == 0) {
		if(m_bLeft == true) {
			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX, iY + 30);
			pDC ->LineTo(iX + m_iWidth - 5, iY + 30);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();
		}
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) {
			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX, iY + 30);
			pDC ->LineTo(iX + m_iWidth - 5, iY + 30);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();
		}
	}
	else if(m_iType == 2) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2192;
			TextOutW(pDC ->m_hDC, iX + m_iWidth - 40, iY, lpWide, 1);

			if(m_iWidth > 70) {
				CPen pen, *oldPen;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = pDC ->SelectObject(&pen);

				pDC ->MoveTo(iX, iY + 12);
				pDC ->LineTo(iX + m_iWidth - 40 + 6, iY + 12);

				pDC ->SelectObject(oldPen);
				pen.DeleteObject();
			}
		}
	}
	else if(m_iType == 3) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2190;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

			if(m_iWidth > 70) {
				CPen pen, *oldPen;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = pDC ->SelectObject(&pen);

				pDC ->MoveTo(iX + 40, iY + 12);
				pDC ->LineTo(iX + m_iWidth - 40 + 6, iY + 12);

				pDC ->SelectObject(oldPen);
				pen.DeleteObject();
			}
		}
	}
	else if(m_iType == 4) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2192;
			TextOutW(pDC ->m_hDC, iX + m_iWidth - 40, iY, lpWide, 1);

			if(m_iWidth > 70) {
				CPen pen, *oldPen;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = pDC ->SelectObject(&pen);

				pDC ->MoveTo(iX, iY + 12);
				pDC ->LineTo(iX + m_iWidth - 40 + 6, iY + 12);

				pDC ->SelectObject(oldPen);
				pen.DeleteObject();
			}
		}
	}
	else if(m_iType == 5) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2190;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

			if(m_iWidth > 70) {
				CPen pen, *oldPen;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = pDC ->SelectObject(&pen);

				pDC ->MoveTo(iX + 40, iY + 12);
				pDC ->LineTo(iX + m_iWidth - 40 + 6, iY + 12);

				pDC ->SelectObject(oldPen);
				pen.DeleteObject();
			}
		}
	}

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}
