// SetObject.cpp: implementation of the CSetObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "SetObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetObject::CSetObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_chSet = 0;
}	

CSetObject::~CSetObject()
{

}

CString CSetObject::GetObject()
{
	CString object;

	switch(m_chSet) {
		case  0x2208:
			object = _T("\\in");
			break;
		case  0x220B:
			object = _T("\\owns");
			break;
		case  0x222A:
			object = _T("\\cup");
			break;
		case  0x2229:
			object = _T("\\cap");
			break;
		case  0x2282:
			object = _T("\\subset");
			break;
		case  0x2283:
			object = _T("\\supset");
			break;
		case  0x2286:
			object = _T("\\subseteq");
			break;
		case  0x2287:
			object = _T("\\supseteq");
			break;
		case  0x2284:
			object = _T("\\not\\subset");
			break;
		case  0x2205:
			object = _T("\\emptyset");
			break;
		default :
			object = _T("[ ERROR ]");
	}

	return object;
}

VOID CSetObject::GetSize(RECT &rect)
{
	rect.right = 40;
	rect.top = 20;

	rect.bottom = 0;
	rect.left = 0;
}

bool CSetObject::IsVisible()
{
	return m_bVisible;
}

VOID CSetObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CSetObject::SetChar(UINT ch)
{
	m_chSet = ch;
}

VOID CSetObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chSet < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[0] = m_chSet;
	lpWide[1] = 0;

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 20;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}
