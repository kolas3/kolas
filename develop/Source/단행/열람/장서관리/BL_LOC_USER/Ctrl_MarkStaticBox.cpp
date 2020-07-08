#include "stdafx.h"
#include "Ctrl_MarkStaticBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCtrl_MarkStaticBox::CCtrl_MarkStaticBox()
{
	m_crBoxBKColor   = RGB(0xFF,0xFF,0xFF);
	m_crBoxFontColor = RGB(0x00,0x00,0x00);


	m_Bitmap.LoadBitmap(IDB_ITEMMARK);

	BITMAP bitmap;
	m_Bitmap.GetBitmap(&bitmap);

	m_nBitmapHeight = bitmap.bmHeight;
	m_nBitmapWidth  = bitmap.bmWidth;

	m_nInterval = 15;
	
	m_Font.CreateFont(12, 6, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("»õ±¼¸²"));
}

CCtrl_MarkStaticBox::~CCtrl_MarkStaticBox()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCtrl_MarkStaticBox, CStatic)
	//{{AFX_MSG_MAP(CCtrl_MarkStaticBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCtrl_MarkStaticBox::OnPaint() 
{
	CPaintDC dc(this);

	OnDrawBox();	
	OnDrawMark();	
	OnDrawText();
}

VOID CCtrl_MarkStaticBox::OnDrawBox()
{
	CClientDC dc(this);

	dc.SetBkColor(m_crBoxBKColor);
}

VOID CCtrl_MarkStaticBox::OnDrawMark()
{
	CClientDC dc(this);

	CRect rc;
	GetClientRect(rc);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_Bitmap);
	
	dc.BitBlt(0, ((rc.Height()-m_nBitmapHeight)/2), m_nBitmapWidth, m_nBitmapHeight, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
}

VOID CCtrl_MarkStaticBox::OnDrawText()
{
	CClientDC dc(this);

	CRect rc;
	GetClientRect(rc);

	CString strText;
	GetWindowText(strText);
	
	CFont* pOldFont = dc.SelectObject(&m_Font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_crBoxFontColor);

	rc.left += m_nInterval;
	dc.DrawText(strText, rc, DT_VCENTER | DT_END_ELLIPSIS);

	dc.SelectObject(pOldFont);
}