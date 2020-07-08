// Preview.cpp : implementation file
//

#include "stdafx.h"
#include "Preview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreview

IMPLEMENT_DYNCREATE(CPreview, CView)

CPreview::CPreview()
{
	m_pDC = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nZoom = 1;
}

CPreview::~CPreview()
{
}


BEGIN_MESSAGE_MAP(CPreview, CView)
	//{{AFX_MSG_MAP(CPreview)
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreview drawing

VOID CPreview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	if( m_pDC == NULL ) return;
	
//	pDC->FillRect( CRect(10,10,m_nWidth+10,m_nWidth+10) , &Brush );

	CRect WndRect;
	GetWindowRect(WndRect);

	INT nCardWidth = 285;
	INT nCardHeight = 180;

	INT nZoomCardWidth = nCardWidth*m_nZoom;
	INT nZoomCardHeight = nCardHeight*m_nZoom;

	CBrush Gray(RGB(192,192,192));
	CBrush Black(RGB(0,0,0));

	INT nLeft = (WndRect.Width()-nZoomCardWidth)/2;
	INT nTop = (WndRect.Height()-nZoomCardHeight)/2;
	
	pDC->FillRect( CRect( nLeft +10 , nTop +10 , nLeft + (nZoomCardWidth)+10 , nTop + (nZoomCardHeight)+10 ) , &Gray );
	pDC->FillRect( CRect( nLeft - 1 , nTop - 1 , nLeft + (nZoomCardWidth)+1 , nTop + (nZoomCardHeight)+1 ) , &Black );
//	pDC->BitBlt( (WndRect.Width()-m_nWidth)/2 ,(WndRect.Height()- m_nHeight)/2 , m_nWidth , m_nHeight , m_pDC , 0 , 0 , SRCCOPY );
//	pDC->BitBlt( 10 , 10 , (285*1.5) , (180*1.5) , m_pDC , 0 , 0 , SRCCOPY );
	pDC->StretchBlt(  nLeft , nTop , nZoomCardWidth , nZoomCardHeight , m_pDC , 0 , 0 , nCardWidth , nCardHeight , SRCCOPY );
//	pDC->BitBlt( nLeft , nTop , 285 , 180 , m_pDC , 0 , 0 , SRCCOPY );
	
}

/////////////////////////////////////////////////////////////////////////////
// CPreview diagnostics

#ifdef _DEBUG
VOID CPreview::AssertValid() const
{
	CView::AssertValid();
}

VOID CPreview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreview message handlers

BOOL CPreview::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


INT CPreview::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0;
}


VOID CPreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
/*
	CPaintDC MemDC(NULL);
	MemDC.CreateCompatibleDC(&dc);

	MemDC.TextOut( 0 , 0 , _T("ª—∑¡¡÷¬°") );

	dc.BitBlt( 0 , 0 , 300 , 300 , &MemDC , 0 , 0 , SRCCOPY );
*/
	OnDraw(&dc);
}


INT CPreview::SetDC( CDC* pDC )
{
	m_pDC = pDC;

	return 0;
}





INT CPreview::ReDraw()
{
	CClientDC dc(this);

	OnDraw(&dc);

	return 0;
}
