// ASCIIObject.cpp: implementation of the CASCIIObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "ASCIIObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASCIIObject::CASCIIObject()
{
	m_bVisible = false;
	// KOL.UDF.022 시큐어코딩 보완
	m_chAscii[0] = '\0';
}

CASCIIObject::~CASCIIObject()
{

}

VOID CASCIIObject::SetChar(TCHAR ch)
{
	m_chAscii = ch;
}

VOID CASCIIObject::GetSize(RECT &rect)
{
	rect.right = 25;
	rect.top = 20;

	rect.bottom = 0;
	rect.left = 0;
}

CString CASCIIObject::GetObject()
{
	CString object;
	object.Format(_T("%c"), m_chAscii);

	return object;
}

VOID CASCIIObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chAscii < 0) return;
	WCHAR lpWide[10] = _T("∏∑");


	CFont Font, *oldFont;

	lpWide[0] = 0x2200;
	lpWide[1] = 0x2200 + 1;
	lpWide[2] = 0x2200 + 2;
	lpWide[3] = 0x2200 + 3;
	lpWide[4] = 0x2200 + 4;
	lpWide[5] = 0x2200 + 5;
	lpWide[6] = 0x2200 + 6;
	lpWide[7] = 0x2200 + 7;
	lpWide[8] = 0x2200 + 8;
	lpWide[9] = 0x2200 + 9;


	INT fontSize = 20;
	CString strFont = _T("Times New Roman");

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);
	CString msg;
	msg.Format(_T("%c"), m_chAscii);
	pDC ->TextOut(iX, iY, msg);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();

/*
	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 15;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	lpWide[0] = m_chAscii;
	TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
*/
}

bool CASCIIObject::IsVisible()
{
	return m_bVisible;
}

VOID CASCIIObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}