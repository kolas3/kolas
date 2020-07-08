// RadicalObject.cpp: implementation of the CRadicalObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "RadicalObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadicalObject::CRadicalObject()
{
	m_bMiddle = false;
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_bLeft = FALSE;
	m_iType = -1;
}

CRadicalObject::~CRadicalObject()
{

}

VOID CRadicalObject::SetType(INT type)
{
	m_iType = type;
}

VOID CRadicalObject::GetSize(RECT &rect)
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

bool CRadicalObject::IsVisible()
{
	return m_bVisible;
}

VOID CRadicalObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

CString CRadicalObject::GetObject()
{
	CString object = _T("Not Implemented[RadicalObject]");

	if(m_iType == 0) {
		if(m_bLeft == true) {
			object = _T("\\sqrt{{ ");
		}
		else {
			object = _T(" }} ");
		}
	}
	else {
		if(m_bLeft == true) {
			object = _T("\\sqrt[ ");
		}
		else if(m_bMiddle == true) {
			object = _T(" ]{{ ");
		}
		else {
			object = _T(" }} ");
		}
	}

	return object;
}

VOID CRadicalObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	CFont Font, *oldFont;
	CString msg;
	INT fontSize = 20;
	CString strFont = _T("Symbol");

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	/////////////////////////////////////////////////////////////////
	if(m_iType == 0) {
		if(m_bLeft == true) {
			msg.Format(_T("%c"), 0xD6);
			pDC ->TextOut(iX, iY, msg);

			//msg.Format(_T("%c"), 0x5F);
			//pDC ->TextOut(iX + 18, iY - 18, msg);

			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX + 18, iY);
			pDC ->LineTo(iX + 18 + m_iWidth, iY);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();
		}
	}
	else {
		if(m_bLeft == true) {

		}
		else if(m_bMiddle == true) {
			msg.Format(_T("%c"), 0xD6);
			pDC ->TextOut(iX, iY, msg);

			//msg.Format(_T("%c"), 0x5F);
			//pDC ->TextOut(iX + 18, iY - 18, msg);

			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX + 18, iY);
			pDC ->LineTo(iX + 18 + m_iWidth, iY);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();
		}
		else {

		}
	}


	/////////////////////////////////////////////////////////////////
	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}

VOID CRadicalObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}

VOID CRadicalObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}