// ScriptObject.cpp: implementation of the CScriptObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "ScriptObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptObject::CScriptObject()
{
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_bLeft = FALSE;
	m_iType = -1;
	m_bMiddle = FALSE;
}

CScriptObject::~CScriptObject()
{

}

VOID CScriptObject::GetSize(RECT &rect)
{
	struct _Cell *top;
	INT alpha = 0;

	top = m_Math.GetFirstCell();

	rect.bottom = 0;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;

	while(top != NULL) {
		(top ->cell)->GetSize(rect);
		alpha += rect.right;
		top = top -> next;
	}

	rect.right = alpha + 40;
	rect.top = 20;
}

bool CScriptObject::IsVisible()
{
	return m_bVisible;
}

VOID CScriptObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CScriptObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	CFont Font, *oldFont;
	CString msg;
	INT fontSize = 20;
	CString strFont = _T("Symbol");

	Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);

	oldFont = pDC ->SelectObject(&Font);

	/////////////////////////////////////////////////////////////////
	if(m_bLeft == true) {

	}
	else {

	}


	/////////////////////////////////////////////////////////////////
	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}

CString CScriptObject::GetObject()
{
	CString object = _T("Not Implemented[RadicalObject]");


	if(m_iType == 0) {
		if(m_bLeft == true) 
			object = _T("^{ ");
		else
			object = _T(" }");
	}
	else if(m_iType == 1) {
		if(m_bLeft == true) 
			object = _T("_{ ");	
		else
			object = _T(" }");
	}
	else if(m_iType == 2) {
		if(m_bLeft == true) {
			object = _T("^{ ");	
		}
		else if(m_bMiddle == true) {
			object = _T(" } _{ ");
		}
		else {
			object = _T(" }");
		}
	}


	return object;
}

VOID CScriptObject::SetIsLeft(bool bLeft)
{
	m_bLeft = bLeft;
}

VOID CScriptObject::SetIsMiddle(bool bMiddle)
{
	m_bMiddle = bMiddle;
}

VOID CScriptObject::SetType(INT type)
{
	m_iType = type;
}