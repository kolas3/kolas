// FractionObject.cpp: implementation of the CFractionObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "FractionObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFractionObject::CFractionObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_bLeft = FALSE;
	m_iType = -1;
	m_bMiddle = FALSE;
}	

CFractionObject::~CFractionObject()
{

}

VOID CFractionObject::SetType(INT type)
{
	m_iType = type;
}

VOID CFractionObject::GetSize(RECT &rect)
{
	struct _Cell *top;
	INT alpha = 0;

	top = m_Math.GetFirstCell();

	rect.bottom = 0;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;

	while(top != NULL) {
		(top ->cell)->GetSize(rect);
		alpha += rect.right;
		top = top -> next;
	}

	rect.right = alpha + 40;
	rect.top = 20;
}

bool CFractionObject::IsVisible()
{
	return m_bVisible;
}

VOID CFractionObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

CString CFractionObject::GetObject()
{
	CString object = _T("Not Implemented[FractionObject]");

	if(m_bMiddle == true) {
		if(m_iType == 0) {
			object = _T(" }{ ");
		}
		else if(m_iType == 1) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}
		else if(m_iType == 2) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}
	}
	else if(m_bLeft == true) {
		if(m_iType == 0) {
			object = _T("\\frac{ ");
		}
		else if(m_iType == 1) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}
		else if(m_iType == 2) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}

	}
	else {
		if(m_iType == 0) {
			object = _T(" }");

		}
		else if(m_iType == 1) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}
		else if(m_iType == 2) {
			object = _T("\\left(");
			object += _T(" { } ");
			object += _T("\\right)");
		}

	}


	return object;
}

VOID CFractionObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	CFont Font, *oldFont;
	WCHAR lpWide[2];

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 20;

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	/////////////////////////////////////////////////////////////////
	if(m_bMiddle == false) {

	}
	else {
		if(m_iType == 0) {
			//lpWide[0] = 0x2015;
			//TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX, iY + 2);
			pDC ->LineTo(iX + m_iWidth - 5, iY + 2);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();

		}
		else if(m_iType == 1) {
			lpWide[0] = 0x2044;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
		else if(m_iType == 2) {
			lpWide[0] = 0x2215;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}

	/////////////////////////////////////////////////////////////////
	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}


VOID CFractionObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}

VOID CFractionObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}
