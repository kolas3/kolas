// BiOpObject.cpp: implementation of the CBiOpObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "BiOpObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiOpObject::CBiOpObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_chOp = 0;
}

CBiOpObject::~CBiOpObject()
{

}

CString CBiOpObject::GetObject()
{
	CString object;
	switch(m_chOp) {
		case 0x2B:
			object = _T("+");
			break;
		case 0x2D:
			object = _T("-");
			break;
		case 0xB1:
			object = _T("\\pm");
			break;
		case 0x2213:
			object = _T("\\mp");
			break;
		case 0xD7:
			object = _T("\\times");
			break;
		case 0xF7:
			object = _T("\\div");
			break;
		case 0x2217:
			object = _T("\\ast");
			break;
		case 0x2E:
			object = _T("\\cdot");
			break;
		case 0x2218:
			object = _T("\\circ");
			break;
		case 0x2219:
			object = _T("\\bullet");
			break;
		case 0x2297:
			object = _T("\\otimes");
			break;
		case 0x2295:
			object = _T("\\oplus");
			break;
/*
		case 0x21:
			object = _T("\\langle");
			break;
		case 0x21:
			object = _T("\\rlangle");
			break;
*/
		default :
			object = _T("[ ERROR ]");
	}

	return object;
}

VOID CBiOpObject::GetSize(RECT &rect)
{
	if(m_chOp == 0 || m_chOp == 1) {
		rect.right = 20;
		rect.top = 20;
	}
	else {
		rect.right = 40;
		rect.top = 20;
	}

	rect.bottom = 0;
	rect.left = 0;
}

VOID CBiOpObject::SetChar(UINT ch)
{
	m_chOp = ch;
}

bool CBiOpObject::IsVisible()
{
	return m_bVisible;
}

VOID CBiOpObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CBiOpObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	INT nCharWidth = 0; 

	if(m_chOp < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;
	CString strFont, msg;
	INT fontSize = 20;

	if(m_chOp == 0 || m_chOp == 1) {
		strFont = _T("Symbol");
		
		Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					PROOF_QUALITY, DEFAULT_PITCH, strFont);

		oldFont = pDC ->SelectObject(&Font);

		if(m_chOp == 0) msg.Format(_T("%c"), 0xE1);
		else msg.Format(_T("%c"), 0xF1);

		pDC ->TextOut(iX, iY, msg);
	}
	else {
		lpWide[0] = m_chOp;
		lpWide[1] = 0;

		strFont = _T("Lucida Sans Unicode");
		
		Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					PROOF_QUALITY, DEFAULT_PITCH, strFont);

		oldFont = pDC ->SelectObject(&Font);

		TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);
	}

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}
