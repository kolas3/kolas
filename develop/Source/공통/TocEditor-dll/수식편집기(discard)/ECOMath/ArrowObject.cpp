// ArrowObject.cpp: implementation of the CArrowObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "ArrowObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArrowObject::CArrowObject()
{
	m_bVisible = false;
	// KOL.UDF.022 시큐어코딩 보완
	m_chArrow = 0;
}

CArrowObject::~CArrowObject()
{

}

CString CArrowObject::GetObject()
{
	CString object;
	switch(m_chArrow) {
		case 0x2192 :
			object = _T("\\leftarrow");
			break;
		case 0x2190:
			object = _T("\\rightarrow");
			break;
		case 0x2194:
			object = _T("\\leftrightarrow");
			break;
		case 0x2191:
			object = _T("\\uparrow");
			break;
		case 0x2193:
			object = _T("\\downarrow");
			break;
		case 0x2195:
			object = _T("\\updownarrow");
			break;
		case 0x21D2:
			object = _T("\\Leftarrow");
			break;
		case 0x21D0:
			object = _T("\\Rightarrow");
			break;
		case 0x21D4:
			object = _T("\\Leftrightarrow");
			break;
		case 0x21D1:
			object = _T("\\Uparrow");
			break;
		case 0x21D3:
			object = _T("\\Downarrow");
			break;
		case 0x21D5:
			object = _T("\\Updownarrow");
			break;
		case 0x21A6:
			object = _T("\\mapsto");
			break;
		default :
			object = _T("[ ERROR ]");
	}

	return object;
}

VOID CArrowObject::GetSize(RECT &rect)
{
	rect.top = 20;

	switch(m_chArrow) {
		case 0x2192 :
			rect.right = 45;
			break;
		case 0x2190:
			rect.right = 45;
			break;
		case 0x2194:
			rect.right = 60;
			break;
		case 0x2191:
			rect.right = 20;
			break;
		case 0x2193:
			rect.right = 20;
			break;
		case 0x2195:
			rect.right = 20;
			break;
		case 0x21D2:
			rect.right = 45;
			break;
		case 0x21D0:
			rect.right = 45;
			break;
		case 0x21D4:
			rect.right = 60;
			break;
		case 0x21D1:
			rect.right = 30;
			break;
		case 0x21D3:
			rect.right = 30;
			break;
		case 0x21D5:
			rect.right = 30;
			break;
		case 0x21A6:
			rect.right = 45;
			break;
		default :
			rect.right = 40;			
	}

	rect.bottom = 0;
	rect.left = 0;
}

VOID CArrowObject::SetChar(UINT ch)
{
	m_chArrow = ch;
}

VOID CArrowObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	INT nCharWidth = 0; 

	if(m_chArrow < 0) return;
	WCHAR lpWide[2];


	CFont Font, *oldFont;

	lpWide[0] = m_chArrow;
	lpWide[1] = 0;

	CString strFont = _T("Lucida Sans Unicode");
	INT fontSize = 20;


	Font.CreateFont(fontSize, fontSize, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	if(GetCharWidth32(pDC ->m_hDC, m_chArrow, m_chArrow, &nCharWidth) == FALSE) {
		AfxMessageBox(_T("Fail")); 
	}

	TextOutW(pDC ->m_hDC, iX, iY, lpWide, 1);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}

bool CArrowObject::IsVisible()
{
	return m_bVisible;
}

VOID CArrowObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}