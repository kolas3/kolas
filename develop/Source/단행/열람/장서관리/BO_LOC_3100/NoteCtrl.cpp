// NoteCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NoteCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoteCtrl

CNoteCtrl::CNoteCtrl()
{
	m_lpWndCls = AfxRegisterWndClass(0);

	m_hLinkCursor = NULL;
	m_crBK = RGB(255, 255, 225);
	m_crLine = RGB(0, 0, 0);
	m_crText = ::GetSysColor(COLOR_WINDOWTEXT);

	ZeroMemory(&m_logfont,sizeof(m_logfont));
	_tcscpy(m_logfont.lfFaceName,_T("±¼¸²Ã¼"));
	m_logfont.lfHeight = 12;
	m_logfont.lfCharSet = DEFAULT_CHARSET;
}

CNoteCtrl::~CNoteCtrl()
{
}


BEGIN_MESSAGE_MAP(CNoteCtrl, CWnd)
	//{{AFX_MSG_MAP(CNoteCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteCtrl Operations

VOID CNoteCtrl::OnDraw(CDC *pDC)
{
	INT nSaveDC = pDC->SaveDC();


	pDC->SetBkMode(TRANSPARENT);
	
	CRect rcWnd;
	CRect rcText;

	GetClientRect(rcWnd);

	CBrush backBrush(m_crBK);
	pDC->FillRect(rcWnd, &backBrush);

	CFont* pOldFont = pDC->SelectObject(&m_Font);

	rcText = rcWnd;
	rcText.DeflateRect(10, 5);//10);
	pDC->DrawText(m_strNote, rcText, DT_WORDBREAK);

	m_rcCloseButton = rcWnd;
	m_rcCloseButton.DeflateRect(5, 5);
	m_rcCloseButton.left = m_rcCloseButton.right - 10;
	m_rcCloseButton.bottom = m_rcCloseButton.top + 10;

	CPen* pOldPen = pDC->SelectObject(&m_Pen);	

	pDC->MoveTo(m_rcCloseButton.TopLeft());
	pDC->LineTo(m_rcCloseButton.right, m_rcCloseButton.top);
	pDC->LineTo(m_rcCloseButton.right, m_rcCloseButton.bottom);
	pDC->LineTo(m_rcCloseButton.left, m_rcCloseButton.bottom);
	pDC->LineTo(m_rcCloseButton.left, m_rcCloseButton.top);

	pDC->MoveTo(m_rcCloseButton.TopLeft());
	pDC->LineTo(m_rcCloseButton.BottomRight());

	pDC->MoveTo(m_rcCloseButton.right, m_rcCloseButton.top);
	pDC->LineTo(m_rcCloseButton.left, m_rcCloseButton.bottom);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

VOID CNoteCtrl::SetDefaultCursor()
{
	if (m_hLinkCursor == NULL)
	{
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNoteCtrl message handlers

BOOL CNoteCtrl::Create(POINT &point, INT nWidth, INT nHeight, CWnd* pParentWnd)
{
	BOOL bSuccess = CreateEx(WS_EX_TRANSPARENT, m_lpWndCls, NULL, WS_CHILD, point.x, point.y, nWidth, nHeight, pParentWnd->GetSafeHwnd(), NULL, NULL );
	if (bSuccess)
	{
		SetOwner(pParentWnd);
		BringWindowToTop();
	}

	return bSuccess;
}

INT CNoteCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetDefaultCursor();
	m_Font.CreateFontIndirect(&m_logfont);
	m_Pen.CreatePen(PS_SOLID, 0, m_crLine);

	return 0;
}


VOID CNoteCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	OnDraw(&dc);
	
	// Do not call CWnd::OnPaint() for painting messages
}

VOID CNoteCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);


	if (m_rcCloseButton.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		GetParent()->SendMessage(NOTE_CLICKED, 0, 0);
		ShowWindow(SW_HIDE);
	}
}

VOID CNoteCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
	{
		SetFocus();
		RedrawWindow();
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CNoteCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hLinkCursor)
	{
		SetCursor(m_hLinkCursor);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
