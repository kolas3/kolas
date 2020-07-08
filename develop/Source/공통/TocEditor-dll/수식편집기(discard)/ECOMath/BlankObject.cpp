// BlankObject.cpp: implementation of the CBlankObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "BlankObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlankObject::CBlankObject()
{
	m_chBlank = 0;
	m_bVisible = true;
}

CBlankObject::~CBlankObject()
{

}

VOID CBlankObject::DrawObject(CDC *pDC)
{
	RECT rect;
	CPen pen, *oldpen;

	pen.CreatePen(PS_DOT, 1, RGB(127, 127, 127));
	oldpen = pDC ->SelectObject(&pen);

	rect.bottom = m_iBaseY;
	rect.left = m_iBaseX;
	rect.right = m_iBaseX + m_iWidth;
	rect.top = m_iBaseY + m_iHeight;
	
	pDC ->Rectangle(&rect);

	pDC ->SelectObject(oldpen);
	pen.DeleteObject();
}

VOID CBlankObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	RECT rect;
	CPen pen, *oldpen;

	pen.CreatePen(PS_DOT, 1, RGB(127, 127, 127));

	oldpen = pDC ->SelectObject(&pen);

	if(m_chBlank == 0) {
		rect.bottom = iY;
		rect.left = iX;
		rect.right = iX + 10;
		rect.top = iY + 10;

		pDC ->Rectangle(&rect);
	}

	pDC ->SelectObject(oldpen);
	pen.DeleteObject();
}

VOID CBlankObject::SetPosition(INT iX, INT iY)
{

}

VOID CBlankObject::SetSize(INT iWidth, INT iHeight)
{
	m_iHeight = iHeight;
	m_iWidth = iWidth;
}

VOID CBlankObject::GetSize(RECT &rect)
{

	rect.bottom = 0;
	rect.left = 0;

	if(m_chBlank > 0) {
		rect.right = 0;
		rect.top = 0;
	}
	else {
		rect.right = 20;
		rect.top = 20;
	}
}

VOID CBlankObject::SetChar(TCHAR ch)
{
	m_chBlank = ch;
}

CString CBlankObject::GetObject()
{
	CString object;

	if(m_chBlank > 0)
		object.Format(_T("%c"), m_chBlank);
	else 
		object = _T("");

	return object;
}

bool CBlankObject::IsVisible()
{
	return m_bVisible;
}

VOID CBlankObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}