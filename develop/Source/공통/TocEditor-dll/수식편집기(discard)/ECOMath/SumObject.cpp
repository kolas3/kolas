// SumObject.cpp: implementation of the CSumObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "SumObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSumObject::CSumObject()
{
	m_bLeft = false;
	m_bMiddle = false;
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_iType = -1;
}

CSumObject::~CSumObject()
{

}

VOID CSumObject::SetType(INT type)
{
	m_iType = type;
}

VOID CSumObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}

VOID CSumObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}

bool CSumObject::IsVisible()
{
	return m_bVisible;
}

VOID CSumObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CSumObject::GetSize(RECT &rect)
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

CString CSumObject::GetObject()
{
	CString object = _T("Not Implemented[SumObject]");

	if(m_iType == 0) {
		if(m_bLeft == true) {
			object = _T("\\sum ");
		}
		else 
			object = _T("");
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) {
			object = _T("\\sum\\limits_{");
		}
		else if(m_bMiddle == true) {
			object = _T(" } {");
		}
		else if(m_bLeft == false) {
			object = _T(" }");
		}
	}
	else if(m_iType == 2) {
		if(m_bLeft == true && m_bMiddle == false) {
			object = _T("\\sum\\limits_{");
		}
		else if(m_bLeft == true && m_bMiddle == true) {
			object = _T(" }^{");
		}
		else if(m_bLeft == false && m_bMiddle == true) {
			object = _T(" } {");
		}
		else if(m_bLeft == false && m_bMiddle == false) {
			object = _T(" }");
		}
	}
	else if(m_iType == 3) {
		if(m_bLeft == true) {
			object = _T("\\sum\\nolimits_{");
		}
		else if(m_bMiddle == true) {
			object = _T(" } {");
		}
		else if(m_bLeft == false) {
			object = _T(" }");
		}
	}
	else if(m_iType == 4) {
		if(m_bLeft == true && m_bMiddle == false) {
			object = _T("\\sum\\nolimits_{");
		}
		else if(m_bLeft == true && m_bMiddle == true) {
			object = _T(" }^{");
		}
		else if(m_bLeft == false && m_bMiddle == true) {
			object = _T(" } {");
		}
		else if(m_bLeft == false && m_bMiddle == false) {
			object = _T(" }");
		}
	}

	return object;
}

VOID CSumObject::DrawObject(CDC *pDC, INT iX, INT iY)
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
			lpWide[0] = 0x2211;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2211;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}
	else if(m_iType == 2) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2211;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}
	else if(m_iType == 3) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2211;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}
	else if(m_iType == 4) {
		if(m_bLeft == true) {
			lpWide[0] = 0x2211;
			TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
		}
	}


	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}
