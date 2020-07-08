// GreekObject.cpp: implementation of the CGreekObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "GreekObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGreekObject::CGreekObject()
{
	m_chGreek = -1;
	m_bVisible = false;
}

CGreekObject::~CGreekObject()
{

}

VOID CGreekObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	if(m_chGreek < 0) return;

	CFont Font, *oldFont;

	INT fontSize = 20;
	CString strFont = _T("Symbol");

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);
	CString msg;
	msg.Format(_T("%c"), m_chGreek);
	pDC ->TextOut(iX, iY, msg);

	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}

VOID CGreekObject::SetChar(TCHAR ch)
{
	m_chGreek = ch;
}

VOID CGreekObject::GetSize(RECT &rect)
{
	if(m_chGreek >= 'A' && m_chGreek <= 'Z') {
		rect.right = 30;
		rect.top = 20;
	}
	else {
		rect.right = 20;
		rect.top = 20;
	}

	rect.bottom = 0;
	rect.left = 0;
}

bool CGreekObject::IsVisible()
{
	return m_bVisible;
}

VOID CGreekObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

CString CGreekObject::GetObject()
{
	CString object;

	switch(m_chGreek) {
		case 'a' :
			object = _T("\\alpha");
			break;
		case 'b' :
			object = _T("\\beta");
			break;
		case 'c' :
			object = _T("\\chi");
			break;
		case 'd' :
			object = _T("\\delta");
			break;
		case 'e' :
			object = _T("\\epsilon");
			break;
		case 'f' :
			object = _T("\\phi");
			break;
		case 'g' :
			object = _T("\\gamma");
			break;
		case 'h' :
			object = _T("\\eta");
			break;
		case 'i' :
			object = _T("\\iota");
			break;
		case 'k' :
			object = _T("\\kappa");
			break;
		case 'l' :
			object = _T("\\lammda");
			break;
		case 'm' :
			object = _T("\\mu");
			break;
		case 'n' :
			object = _T("\\nu");
			break;
		case 'o' :
			object = _T("\\omicron");
			break;
		case 'p' :
			object = _T("\\pi");
			break;
		case 'q' :
			object = _T("\\theta");
			break;
		case 'r' :
			object = _T("\\rho");
			break;
		case 's' :
			object = _T("\\sigma");
			break;
		case 't' :
			object = _T("\\tau");
			break;
		case 'u' :
			object = _T("\\upsilon");
			break;
		case 'w' :
			object = _T("\\omega");
			break;
		case 'x' :
			object = _T("\\xi");
			break;
		case 'y' :
			object = _T("\\psi");
			break;
		case 'z' :
			object = _T("\\zeta");
			break;
		/////////////////////////////////
		// Uppercase
		case 'G' :
			object = _T("\\Gamma");
			break;
		case 'D' :
			object = _T("\\Delta");
			break;
		case 'Q' :
			object = _T("\\Theta");
			break;
		case 'L' :
			object = _T("\\Lambda");
			break;
		case 'X' :
			object = _T("\\Xi");
			break;
		case 'P' :
			object = _T("\\Pi");
			break;
		case 'S' :
			object = _T("\\Sigma");
			break;
		case 'U' :
			object = _T("\\Upsilon");
			break;
		case 'F' :
			object = _T("\\Phi");
			break;
		case 'Y' :
			object = _T("\\Psi");
			break;
		case 'W' :
			object = _T("\\Omega");
			break;
	}

	return object;
}
