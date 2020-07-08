// MiscObject.cpp: implementation of the CMiscObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "MiscObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiscObject::CMiscObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_chMise = 0;
}

CMiscObject::~CMiscObject()
{

}


CString CMiscObject::GetObject()
{
	CString object;

	switch(m_chMise) {
		case 0x2202:
			object = _T("\\partial");
			break;
		case 0x2207:
			object = _T("\\nabla");
			break;
		case 0x2111:
			object = _T("\\im");
			break;
		case 0x211B:
			object = _T("\\re");
			break;
		case 0x2220:
			object = _T("\\angle");
			break;
		case 0x22A5:
			object = _T("\\bot");
			break;
		case 0x22CA:
			object = _T("\\diamondsuit");
			break;
		case 0x00BA:
			object = _T("^\\circ");
			break;
		case 0x210F:
			object = _T("\\hbar");
			break;
		case 0x222B:
			object = _T("\\smallint");
			break;
		case 0x2211:
			object = _T("\\sum");
			break;
		case 0x220F:
			object = _T("\\prod");
			break;
		case 0x2210:
			object = _T("\\coprod");
			break;
/*
		case :
			object = _T("\\aleph");
			break;
		case :
			object = _T("\\ell");
			break;
		case :
			object = _T("\\wp");
			break;
*/
		default :
			object = _T("[ ERROR ]");
	}
	return object;
}

VOID CMiscObject::GetSize(RECT &rect)
{
	rect.right = 40;
	rect.top = 20;

	rect.bottom = 0;
	rect.left = 0;
}

VOID CMiscObject::SetChar(UINT ch)
{
	m_chMise = ch;
}

bool CMiscObject::IsVisible()
{
	return m_bVisible;
}

VOID CMiscObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CMiscObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chMise < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[1] = 0;

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 20;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	lpWide[0] = m_chMise;
	TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}