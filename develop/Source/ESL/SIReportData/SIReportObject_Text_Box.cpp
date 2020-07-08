// SIReportObject_Text_Box.cpp: implementation of the CSIReportObject_Text_Box class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Text_Box.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Text_Box, CSIReportObject)
CSIReportObject_Text_Box::CSIReportObject_Text_Box() : CSIReportObject()
{
	SetObjType(OBJECT_TEXT_BOX);
	m_strName = _T("TEXT_BOX");

	fill_color = RGB(255, 255, 255);
	horz_align = 0;
	vert_align = 1;
	indent_len = 1;

	horz_line = 1;
	vert_line = 1;

	focus = 0;
	char_max = 60;
	char_len = 60;
}

VOID CSIReportObject_Text_Box::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	CSIReportObject::Clone(pObject);

	((CSIReportObject_Text_Box*)pObject)->char_len = char_len;
	((CSIReportObject_Text_Box*)pObject)->fill_color = fill_color;
	((CSIReportObject_Text_Box*)pObject)->horz_align = horz_align;
	((CSIReportObject_Text_Box*)pObject)->vert_align = vert_align;
	((CSIReportObject_Text_Box*)pObject)->indent_len = indent_len;
	((CSIReportObject_Text_Box*)pObject)->horz_line = horz_line;
	((CSIReportObject_Text_Box*)pObject)->vert_line = vert_line;
	((CSIReportObject_Text_Box*)pObject)->text = text;

EFS_END
}



CSIReportObject_Text_Box::~CSIReportObject_Text_Box()
{

}

/////////////////////////////////////////////////////////////////////////////
// CMObject Operation
CSIReportObject& CSIReportObject_Text_Box::operator =(CSIReportObject& rhs)
{	
	if (this == &rhs)
		return *this;

	return CSIReportObject::operator =(rhs);
}

/////////////////////////////////////////////////////////////////////////////
// CMObject Draw
VOID CSIReportObject_Text_Box::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	CBrush fg_brush;
	fg_brush.CreateSolidBrush(m_colorBk);
	CBrush *pOldBrush = pDC->SelectObject(&fg_brush);
	
	pDC->SelectObject(&pOldBrush);

	CRect rect = GetRect();
	// fill color
	pDC->FillSolidRect(rect, fill_color);

	// draw text
	CFont font;
	LOGFONT logfont;
	logfont = text.font;	
	font.CreateFontIndirect(&logfont);
	CFont *pOldFont = pDC->SelectObject(&font);
	
	CRect rectText = rect;
	rectText.DeflateRect(indent_len, indent_len);

	// find line count;
	INT nLineCount = 1;
	INT nStart = 0;
	do
	{
		nStart = text.text.Find(_T("\r\n"), nStart);
		if (nStart == -1)
			break;
		
		nLineCount++;
		nStart++;
	}
	while (TRUE);

	DWORD dwStyle = DT_NOPREFIX;
	if (horz_align == 0)
		dwStyle |= DT_LEFT;
	else if (horz_align == 1)
		dwStyle |= DT_CENTER;
	else if (horz_align == 2)
		dwStyle |= DT_RIGHT;

	if (vert_align == 0)
	{
	}
	else if (vert_align == 1)
	{
		rectText.top = rectText.top + (rectText.Height() + (nLineCount * text.font.lfHeight)) / 2;

	}		
	else if (vert_align == 2)
	{
		rectText.top = rectText.top + (rectText.Height() + (nLineCount * text.font.lfHeight));
	}

	if (rectText.top < rect.top)
		rectText = rect;
		
	pDC->SetTextColor(text.color);
	pDC->DrawText(text.text, rectText, dwStyle);

	// draw outline
	CPen pen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen *pOldPen = pDC->SelectObject(&pen);
	DrawWidget(pDC, rect, TRUE, horz_line, vert_line);
	
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

EFS_END
}

