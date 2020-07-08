// RelationObject.cpp: implementation of the CRelationObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "RelationObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelationObject::CRelationObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_chRelation = 0;
}

CRelationObject::~CRelationObject()
{

}

VOID CRelationObject::SetChar(UINT ch)
{
	m_chRelation = ch;
}

CString CRelationObject::GetObject()
{
	CString object;

	switch(m_chRelation) {
		case  0x3C:
			object = _T("<");
			break;
		case  0x3D:
			object = _T("=");
			break;
		case  0x3E:
			object = _T(">");
			break;
		case  0x2266:
			object = _T("\\le");
			break;
		case  0x2267:
			object = _T("\\ge");
			break;
		case  0x227A:
			object = _T("\\prec");
			break;
		case  0x227B:
			object = _T("\\succ");
			break;
		case  0x22B3:
			object = _T("\\triangleleft");
			break;
		case  0x22B2:
			object = _T("\\triangleright");
			break;
		case  0x2260:
			object = _T("\\ne");
			break;
		case  0x2261:
			object = _T("\\equiv");
			break;
		case  0x2248:
			object = _T("\\approx");
			break;
		case  0x2245:
			object = _T("\\cong");
			break;
		case  0x221D:
			object = _T("\\propto");
			break;
		default :
			object = _T("[ ERROR ]");
	}

	return object;
}

VOID CRelationObject::GetSize(RECT &rect)
{
	rect.right = 40;
	rect.top = 15;

	rect.bottom = 0;
	rect.left = 0;
}

bool CRelationObject::IsVisible()
{
	return m_bVisible;
}

VOID CRelationObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CRelationObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chRelation < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[0] = m_chRelation;
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