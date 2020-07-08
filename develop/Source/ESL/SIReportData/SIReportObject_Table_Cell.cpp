// SIReportObject_Table_Cell.cpp: implementation of the CSIReportObject_Table_Cell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Table_Cell.h"
#include "SIReportObject_Table.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Table_Cell, CSIReportObject)
CSIReportObject_Table_Cell::CSIReportObject_Table_Cell()
{
	child_count = 0;

	bind_status = 0;
	bind_row = 0;
	bind_col = 0;
	slash = 0;
	focus = FALSE;

	char_len = 50;
	char_max = 300;
	indent_len = 1;
	horz_align = 0;
	vert_align = 1;
	orientation = 1;

	char_cx = 0;
	char_cy = 0;
	
	orign_height = 0;
	orign_width = 0;

	part_sum = 0;
	total_sum = 0;

	fill_color = RGB(255, 255, 255);
}

VOID CSIReportObject_Table_Cell::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	CSIReportObject::Clone(pObject);

	((CSIReportObject_Table_Cell*)pObject)->child_count = child_count;
	((CSIReportObject_Table_Cell*)pObject)->child_name[0] = child_name[0];
	((CSIReportObject_Table_Cell*)pObject)->bind_status = bind_status;
	((CSIReportObject_Table_Cell*)pObject)->bind_row = bind_row;
	((CSIReportObject_Table_Cell*)pObject)->bind_col = bind_col;
	((CSIReportObject_Table_Cell*)pObject)->slash = slash;
	((CSIReportObject_Table_Cell*)pObject)->focus = focus;
	((CSIReportObject_Table_Cell*)pObject)->char_len = char_len;
	((CSIReportObject_Table_Cell*)pObject)->char_max = char_max;
	((CSIReportObject_Table_Cell*)pObject)->indent_len = indent_len;
	((CSIReportObject_Table_Cell*)pObject)->horz_align = horz_align;
	((CSIReportObject_Table_Cell*)pObject)->vert_align = vert_align;
	((CSIReportObject_Table_Cell*)pObject)->orientation = orientation;
	((CSIReportObject_Table_Cell*)pObject)->char_cx = char_cx;
	((CSIReportObject_Table_Cell*)pObject)->char_cy = char_cy;
	((CSIReportObject_Table_Cell*)pObject)->orign_height = orign_height;
	((CSIReportObject_Table_Cell*)pObject)->orign_width = orign_width;
	((CSIReportObject_Table_Cell*)pObject)->part_sum = part_sum;
	((CSIReportObject_Table_Cell*)pObject)->total_sum = total_sum;
	((CSIReportObject_Table_Cell*)pObject)->fill_color = fill_color;
	((CSIReportObject_Table_Cell*)pObject)->text = text;

EFS_END
}

CSIReportObject_Table_Cell::~CSIReportObject_Table_Cell()
{

}

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table_Cell Operation

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table_Cell Draw
VOID CSIReportObject_Table_Cell::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	if (bind_status == 2) return;

	CBrush fg_brush;
	fg_brush.CreateSolidBrush(m_colorBk);
	CBrush *pOldBrush = pDC->SelectObject(&fg_brush);
		
	CFont font;
	LOGFONT logfont;
	logfont = text.font;	
	font.CreateFontIndirect(&logfont);
	CFont *pOldFont = pDC->SelectObject(&font);

	CRect rect = GetRect();

	if (bind_status == 1)
	{
		CSIReportObject_Table_Cell *pCell = ((CSIReportObject_Table*)m_pParentObject)->GetCell(bind_row, bind_col);
		rect.right = pCell->GetRect().right;
		rect.bottom = pCell->GetRect().bottom;
	}

	// fill color
	pDC->FillSolidRect(rect, fill_color);
	
	// draw text
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
	DrawWidget(pDC, rect, TRUE, ((CSIReportObject_Table*)GetParentObject())->horz_line, ((CSIReportObject_Table*)GetParentObject())->vert_line);
	
	// slash
	if (slash == 1)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.BottomRight());
	}
	else if (slash == 2)
	{
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.left, rect.bottom);
	}
	// focus
	if (focus)
	{
		INT oldMode = pDC->SetROP2(R2_XORPEN);
		rect.DeflateRect(2, 2);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle(rect);
		pDC->SetROP2(oldMode);
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);	
	pDC->SelectObject(pOldFont);

EFS_END
}
