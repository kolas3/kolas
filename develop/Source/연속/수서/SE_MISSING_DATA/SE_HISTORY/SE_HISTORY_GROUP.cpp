// SE_HISTORY_GROUP.cpp: implementation of the CSE_HISTORY_GROUP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SE_HISTORY_GROUP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSE_HISTORY_GROUP::CSE_HISTORY_GROUP()
{
    m_colorText = RGB(0,0,0);

	m_logFont.lfHeight = 12;
    m_logFont.lfWidth = 0;
	m_logFont.lfEscapement =0;
	m_logFont.lfOrientation = 0;
	m_logFont.lfWeight = FW_BOLD;
	m_logFont.lfItalic = FALSE;
	m_logFont.lfUnderline = FALSE;
	m_logFont.lfStrikeOut = FALSE;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = OUT_CHARACTER_PRECIS;
    m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
}

CSE_HISTORY_GROUP::~CSE_HISTORY_GROUP()
{

}

INT CSE_HISTORY_GROUP::DrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	//TRACE(_T("%s\n"),m_GROUP_TYPE);
	
	ids = SetDrawObject(pDC);

	/// BODY
	pDC->Rectangle(m_ObjectRect);

	//headerline
	CRect HeaderRect;
	
	HeaderRect = m_ObjectRect; 
	HeaderRect.top = m_ObjectRect.bottom + GROUP_HEADER_SIZE ;
    pDC->Rectangle(HeaderRect);
    
	///headercaption	 
	pDC->SetTextColor(m_colorText);
	pDC->SetBkMode(OPAQUE);
	DrawTextWithFont(pDC, m_ALIAS, HeaderRect, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);


	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);
	
	return 0;

}

INT CSE_HISTORY_GROUP::UnDrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	ids = SetUnDrawObject(pDC);

	/// BODY
	pDC->Rectangle(m_ObjectRect);

	/// Header Line	
	CRect Header;
	CPoint Tmp;
	Header= m_ObjectRect;
	Header.top = Header.bottom + GROUP_HEADER_SIZE ; 
	pDC->Rectangle(Header);

	Tmp.x  = Header.left;
	Tmp.y  = Header.top;
	pDC->MoveTo(Tmp);

	Tmp.x  = Header.right;
	pDC->LineTo(Tmp);

	
	//RETURN PEN or Brush  
    ids = ReturnDrawObject(pDC);
	
	return 0;

}

INT CSE_HISTORY_GROUP::DrawTextWithFont(CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize = logfont.lfHeight;
	CFont font;

	logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);


	CRect tmpRect;
	tmpRect = rect;
	
	if (tmpRect.top > tmpRect.bottom) {
		tmpRect.top = rect.bottom;
		tmpRect.bottom = rect.top;
	}

	if (tmpRect.left > tmpRect.right) {
		tmpRect.left = rect.right;
		tmpRect.right = rect.left;
	}
		
	height = dc->DrawText(str, tmpRect, nFormat);
	dc->SelectObject(pOldFont);
	return height;

}