// Kolas2up_ImageEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "Kolas2up_ImageEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\paintlib\common\Filter\plfilterresizebilinear.h"
#include "..\paintlib\common\Filter\PLFilterCrop.h"
#include "..\paintlib\common\pljpegenc.h"
#include "..\paintlib\common\plbmpenc.h"

#include "CharConvert.h"

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorView

IMPLEMENT_DYNCREATE(CKolas2up_ImageEditorView, CScrollView)

CKolas2up_ImageEditorView::CKolas2up_ImageEditorView()
{
	m_pWnd = NULL;
	m_pBmp = NULL;
	m_pBmpUndo = NULL;
	ResetCurrentSel();
}

CKolas2up_ImageEditorView::~CKolas2up_ImageEditorView()
{
	FreeBitmap();
}


BEGIN_MESSAGE_MAP(CKolas2up_ImageEditorView, CScrollView)
	//{{AFX_MSG_MAP(CKolas2up_ImageEditorView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorView drawing

VOID CKolas2up_ImageEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

VOID CKolas2up_ImageEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	m_pBmp->Draw (pDC->m_hDC, 0, 0);
	if (IsCurrentSel() == TRUE) {
		DrawBox(pDC, m_rCurrentSel.left, m_rCurrentSel.top, m_rCurrentSel.right, m_rCurrentSel.bottom);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorView diagnostics

#ifdef _DEBUG
VOID CKolas2up_ImageEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

VOID CKolas2up_ImageEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorView message handlers

VOID CKolas2up_ImageEditorView::FreeBitmap(VOID)
{
	if (m_pBmp) {
		delete m_pBmp;
		m_pBmp = NULL;
	}
	if (m_pBmpUndo) {
		delete m_pBmpUndo;
		m_pBmpUndo = NULL;
	}
	return;
}

INT CKolas2up_ImageEditorView::LoadBitmap(CString strFileName)
{
	FreeBitmap();
	m_pBmp = new PLWinBmp;
	if (m_pBmp == NULL) return -1;

	///@20071107 PDW
	///@{{++
	///@UNICODE WORK
	/*//before
	m_Decoder.MakeBmpFromFile ((LPSTR)(LPCTSTR)strFileName, m_pBmp);
	*///after
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strFileName.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{
		m_Decoder.MakeBmpFromFile (szTmp, m_pBmp);	
		delete [] szTmp;
	}	
	///@--}}

	return 0;
}

INT CKolas2up_ImageEditorView::Resize(INT XSize, INT YSize)
{
	PLFilterResizeBilinear Filter (XSize, YSize);
	// Replace with Apply(pSrc, pDst) if you want to
	// keep the original bitmap.  
	Filter.ApplyInPlace (m_pBmp);   

	ResetCurrentSel();
	Invalidate(TRUE);

	return 0;
}

INT CKolas2up_ImageEditorView::Crop(INT XMin, INT YMin, INT XMax, INT YMax)
{
	PLFilterCrop Filter(XMin, YMin, XMax, YMax);

	Filter.ApplyInPlace (m_pBmp);   
	ResetCurrentSel();
	Invalidate(TRUE);

	CPoint pt;
	pt.x = 0;
	pt.y = 0;

	ScrollToPosition(pt);
	return 0;
}

INT CKolas2up_ImageEditorView::SaveBitmap(CString strFileName)
{
	if (m_pBmp == NULL) return -1;

	PLBmpEncoder Encoder;

	///@20071107 PDW
	///@{{++
	///@UNICODE WORK
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strFileName.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{			
		Encoder.MakeFileFromBmp(szTmp, m_pBmp);
		delete [] szTmp;
	}	
	///@--}}

	return 0;
}

INT CKolas2up_ImageEditorView::SaveJpeg(CString strFileName)
{
	if (m_pBmp == NULL) return -1;

	PLJPEGEncoder Encoder;

	///@20071107 PDW
	///@{{++
	///@UNICODE WORK	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strFileName.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{			
		Encoder.MakeFileFromBmp(szTmp, m_pBmp);
		delete [] szTmp;
	}	
	///@--}}
	
	return 0;
}

INT CKolas2up_ImageEditorView::DrawBox(CDC *pDC, INT left, INT top, INT right, INT bottom)
{
	if (!pDC) return 0;

	pDC->SetROP2(R2_XORPEN);
    CPen   pen;
	pen.CreatePen(PS_DOT, m_nLineThick, m_cLineColor);
    CPen*   pOldPen   = (CPen*)pDC->SelectObject(&pen);

	pDC->MoveTo(left, bottom);
	pDC->LineTo(left, top);
	pDC->MoveTo(left, top);
	pDC->LineTo(right, top);
	pDC->MoveTo(right, top);
	pDC->LineTo(right, bottom);
	pDC->MoveTo(right, bottom);
	pDC->LineTo(left, bottom);

    pDC->SelectObject(pOldPen);
    pen.DeleteObject();

	return 0;
}

VOID CKolas2up_ImageEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_pBmp == NULL) return;

	SetCropBox(point);
	Invalidate(TRUE);

	m_CropBoxPoint = point;

	CScrollView::OnLButtonDown(nFlags, point);
}

BOOL CKolas2up_ImageEditorView::IsCurrentSel(VOID)
{
	if (m_rCurrentSel.left == 0 && m_rCurrentSel.top == 0 && m_rCurrentSel.right == 0 && m_rCurrentSel.bottom == 0) return FALSE;
	
	return TRUE;
}

VOID CKolas2up_ImageEditorView::ResetCurrentSel(VOID)
{
	m_rCurrentSel.top = 0;
	m_rCurrentSel.left = 0;
	m_rCurrentSel.bottom = 0;
	m_rCurrentSel.right = 0;

	return;
}

SIZE CKolas2up_ImageEditorView::GetSize(VOID)
{
	SIZE s;
	s.cx = 0;
	s.cy = 0;

	if (m_pBmp == NULL) return s;

	s = m_pBmp->GetSize();
	return s;
}

VOID CKolas2up_ImageEditorView::SaveUndoBitmap(VOID)
{
	if (m_pBmpUndo == NULL) {
		m_pBmpUndo = new PLWinBmp;
	}
	if (m_pBmpUndo == NULL) return;

	*m_pBmpUndo = *m_pBmp;
	return;
}

VOID CKolas2up_ImageEditorView::LoadUndoBitmap(VOID)
{
	if (m_pBmpUndo == NULL) return;

	*m_pBmp = *m_pBmpUndo;

	Invalidate(TRUE);
	return;
}


VOID CKolas2up_ImageEditorView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pWnd) {
		m_point = point;
		m_pWnd->PostMessage(WM_USER+32000);
	}
	
	CScrollView::OnRButtonUp(nFlags, point);
}

VOID CKolas2up_ImageEditorView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags == 1) {
		SIZE s = m_pBmp->GetSize();
		CPoint pt = GetScrollPosition();
		CPoint newpt = point;
		newpt = newpt + pt;

		if (newpt.x + m_nCropWidth > s.cx) return;
		if (newpt.y + m_nCropHeight > s.cy) return;

		CDC *pDC = GetDC();
		if (IsCurrentSel() == TRUE) {
			DrawBox(pDC, m_CropBoxPoint);
		}
		m_CropBoxPoint = point;
		SetCropBox(point);
		DrawBox(pDC, m_CropBoxPoint);
		ReleaseDC(pDC);
	}
	CScrollView::OnMouseMove(nFlags, point);
}

VOID CKolas2up_ImageEditorView::SetCropBox(CPoint point)
{
	SIZE s = m_pBmp->GetSize();
	CPoint pt = GetScrollPosition();

	point = point + pt;

	if (point.x + m_nCropWidth > s.cx) return;
	if (point.y + m_nCropHeight > s.cy) return;

	m_rCurrentSel.left = point.x;
	m_rCurrentSel.top = point.y;
	m_rCurrentSel.right = point.x + m_nCropWidth;
	m_rCurrentSel.bottom = point.y + m_nCropHeight;

	return;
}

VOID CKolas2up_ImageEditorView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonUp(nFlags, point);
}

VOID CKolas2up_ImageEditorView::DrawBox(CDC *pDC, CPoint point)
{
	INT l, t, r, b;
	l = point.x;
	t = point.y;
	r = point.x + m_nCropWidth;
	b = point.y + m_nCropHeight;

	DrawBox(pDC, l, t, r, b);
	return;
}

INT CKolas2up_ImageEditorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0;
}

