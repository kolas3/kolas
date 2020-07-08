// TexObject.cpp: implementation of the CTexObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "TexObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexObject::CTexObject()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_iBaseX = -1;
	m_iBaseY = -1;
	m_iHeight = -1;
	m_iWidth = -1;
}

CTexObject::~CTexObject()
{

}

VOID CTexObject::DrawObject(CDC *pDC)
{

}

VOID CTexObject::SetPosition(INT iX, INT iY)
{

}

VOID CTexObject::SetSize(INT iWidth, INT iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}