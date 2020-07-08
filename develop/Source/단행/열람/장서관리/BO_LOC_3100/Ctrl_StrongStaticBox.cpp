// Ctrl_StrongStaticBox.cpp : implementation file
//

#include "stdafx.h"
#include "Ctrl_StrongStaticBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrl_StrongStaticBox

CCtrl_StrongStaticBox::CCtrl_StrongStaticBox()
{
	m_crBoxFontColor = RGB(0x40,0x80,0x80);

	m_Font.CreateFont(12, 6, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("»õ±¼¸²"));
}

CCtrl_StrongStaticBox::~CCtrl_StrongStaticBox()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCtrl_StrongStaticBox, CStatic)
	//{{AFX_MSG_MAP(CCtrl_StrongStaticBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrl_StrongStaticBox message handlers

void CCtrl_StrongStaticBox::OnPaint() 
{
	CPaintDC dc(this);
	
	OnDrawText();
}

VOID CCtrl_StrongStaticBox::OnDrawText()
{
	CClientDC dc(this);

	CRect rc;
	GetClientRect(rc);

	CString strText;
	GetWindowText(strText);
	
	CFont* pOldFont = dc.SelectObject(&m_Font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_crBoxFontColor);
	
	dc.DrawText(strText, rc, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	dc.SelectObject(pOldFont);
}