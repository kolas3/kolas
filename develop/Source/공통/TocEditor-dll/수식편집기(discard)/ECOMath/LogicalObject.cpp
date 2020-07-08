// LogicalObject.cpp: implementation of the CLogicalObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "LogicalObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicalObject::CLogicalObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_chLogical = 0;
}

CLogicalObject::~CLogicalObject()
{

}

CString CLogicalObject::GetObject()
{
	CString object;
	switch(m_chLogical) {
		case 0x2234:
			object = _T("\\therefore");
			break;
		case 0x2235:
			object = _T("\\because");
			break;
		case 0x2203:
			object = _T("\\exists");
			break;
		case 0x2200:
			object = _T("\\forall");
			break;
		case 0x2510:
			object = _T("\\neg");
			break;
		case 0x2227:
			object = _T("\\edge");
			break;
		case 0x2228:
			object = _T("\\vee");
			break;
		default :
			object = _T("[ ERROR ]");
	}

	return object;
}

VOID CLogicalObject::GetSize(RECT &rect)
{
	rect.right = 40;
	rect.top = 20;

	rect.bottom = 0;
	rect.left = 0;
}

VOID CLogicalObject::SetChar(UINT ch)
{
	m_chLogical = ch;
}

bool CLogicalObject::IsVisible()
{
	return m_bVisible;
}

VOID CLogicalObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CLogicalObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chLogical < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[0] = m_chLogical;
	lpWide[1] = 0;

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 15;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}
