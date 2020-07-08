// IntegralObject.cpp: implementation of the CIntegralObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "IntegralObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntegralObject::CIntegralObject()
{
	m_bLeft = false;
	m_bMiddle = false;
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_iType = -1;

}

CIntegralObject::~CIntegralObject()
{

}

VOID CIntegralObject::SetType(INT type)
{
	m_iType = type;
}

VOID CIntegralObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}

VOID CIntegralObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}

bool CIntegralObject::IsVisible()
{
	return m_bVisible;
}

VOID CIntegralObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CIntegralObject::GetSize(RECT &rect)
{
	rect.bottom = 0;
	rect.left = 0;

	switch(m_iType) {
		case 0 :
		case 1 :
		case 2 :
			rect.right = 55;
			rect.top = 20;
			break;
		case 3 :
		case 4 :
		case 5 :
			rect.right = 80;
			rect.top = 20;
			break;
		case 6 :
		case 7 :
		case 8 :
			rect.right = 110;
			rect.top = 20;
			break;
	}

}

CString CIntegralObject::GetObject()
{
	CString object = _T("Not Implemented[IntegralObject]");

	if(m_iType == 0) {
		if(m_bLeft == true)
			object = _T("\\int {");
		else
			object = _T("}");
	}
	else if(m_iType == 1) {
		if(m_bLeft == true && m_bMiddle == false) {
			object = _T("\\int\\limits_{");
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
	else if(m_iType == 2) {
		if(m_bLeft == true && m_bMiddle == false) {
			object = _T("\\int_{");
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

VOID CIntegralObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	CFont Font, *oldFont;
	WCHAR lpWide[2];

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 40;

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);


	oldFont = pDC ->SelectObject(&Font);

	/////////////////////////////////////////////////////////////////
	
	if(m_iType == 0) {
		if(m_bLeft == true) {
			lpWide[0] = 0x222B;
			TextOutW(pDC ->m_hDC, iX, iY - 10, lpWide, 1);
		}
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) {
			lpWide[0] = 0x222B;
			TextOutW(pDC ->m_hDC, iX, iY - 10, lpWide, 1);
		}
	}
	else if(m_iType == 2) {
		if(m_bLeft == true) {
			lpWide[0] = 0x222B;
			TextOutW(pDC ->m_hDC, iX, iY - 10, lpWide, 1);
		}
	}
	else if(m_iType == 3) {
		lpWide[0] = 0x222C;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	else if(m_iType == 4) {
		lpWide[0] = 0x222C;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	else if(m_iType == 5) {
		lpWide[0] = 0x222C;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	else if(m_iType == 6) {
		lpWide[0] = 0x222D;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	else if(m_iType == 7) {
		lpWide[0] = 0x222D;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	else if(m_iType == 8) {
		lpWide[0] = 0x222D;
		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}
	/////////////////////////////////////////////////////////////////
	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}