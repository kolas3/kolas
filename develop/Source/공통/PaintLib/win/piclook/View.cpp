/*
/--------------------------------------------------------------------
|
|      $Id: View.cpp,v 1.1 2009/12/09 05:34:55 박종운 Exp $
|       view class implementation
|
|      In the document/view model, this is where document rendering
|      (drawing in this case) occurs. Also handles palette issues.
|
|      Copyright (c) 1998 Bernard Delm?
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "piclook.h"
#include "doc.h"
#include "view.h"
#include "mainfrm.h"

#include "filterdialog.h"
#include "cropfilterdlg.h"
#include "ditherdialog.h"
#include "ThresholdDlg.h"
#include "palviewdlg.h"
#include "contrastdlg.h"
#include "intensitydlg.h"
#include "lightnessdlg.h"

#include "filter/PLFilterContrast.h"
#include "filter/PLFilterThreshold.h"
#include "filter/PLFilterCrop.h"
#include "filter/PLFilterResizeHamming.h"
#include "filter/PLFilterResizeBilinear.h"
#include "filter/PLFilterResizeBox.h"
#include "filter/PLFilterResizeGaussian.h"
#include "filter/PLFilterIntensity.h"
#include "filter/PLFilterLightness.h"
#include "filter/PLFilterGrayscale.h"
#include "filter/PLFilterVideoInvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef OWND
#include <OwndDll.h>
#endif

//This is for banded printing
#define PRINT_BAND_SIZE 1000
#define PRINT_BAND_OVERLAP 100

/////////////////////////////////////////////////////////////////////////////
// CPLView

#ifdef USES_DRAWDIB
IMPLEMENT_DYNCREATE(CPLView, CScrollView)
#else
IMPLEMENT_DYNCREATE(CPLView, CZoomView)
#endif

#ifdef USES_DRAWDIB
BEGIN_MESSAGE_MAP(CPLView, CScrollView)
#else
BEGIN_MESSAGE_MAP(CPLView, CZoomView)
#endif
    //{{AFX_MSG_MAP(CPLView)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FITIMAGE, OnUpdateFitImage)
    ON_COMMAND(ID_VIEW_FITIMAGE, OnFitImage)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DITHER, OnUpdateDither)
    ON_COMMAND(ID_VIEW_DITHER, OnDither)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_FILTER_BILINEAR, OnFilterBilinear)
    ON_COMMAND(ID_FILTER_BOX, OnFilterBox)
    ON_COMMAND(ID_FILTER_GAUSS, OnFilterGauss)
    ON_COMMAND(ID_FILTER_HAMMING, OnFilterHamming)
    ON_COMMAND(ID_FILTER_CROP, OnFilterCrop)
    ON_COMMAND(ID_FILTER_GRAYSCALE, OnFilterGrayscale)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_WM_LBUTTONDOWN()
    ON_WM_MBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(ID_WINDOW_FIT, OnSizeToFit)
    ON_WM_SETCURSOR()
    ON_UPDATE_COMMAND_UI(ID_ZOOM_MODE, OnUpdateZoomMode)
    ON_COMMAND(ID_ZOOM_MODE, OnZoomMode)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILTER_THRESHOLD, OnFilterThreshold)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWPALETTE, OnUpdateViewShowpalette)
	ON_COMMAND(ID_VIEW_SHOWPALETTE, OnViewShowpalette)
	ON_COMMAND(ID_FILTER_CONTRAST, OnFilterContrast)
	ON_COMMAND(ID_FILTER_INTENSITY, OnFilterIntensity)
	ON_COMMAND(ID_FILTER_LIGHTNESS, OnFilterLightness)
	ON_UPDATE_COMMAND_UI(ID_FILTER_CONTRAST, OnUpdateTrueColFilter)
	ON_COMMAND(ID_FILTER_INVERT, OnFilterInvert)
    ON_COMMAND(ID_CONVERT_TO_8BPP, OnConvertTo8BPP)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAMMA, OnUpdateTrueColFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER_BILINEAR, OnUpdateTrueColFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER_HAMMING, OnUpdateTrueColFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER_BOX, OnUpdateTrueColFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GRAYSCALE, OnUpdateTrueColFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER_THRESHOLD, OnUpdateTrueColFilter)
	//}}AFX_MSG_MAP

    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, Super::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, Super::OnFilePrintPreview)

    ON_UPDATE_COMMAND_UI(ID_BMPINFO, OnUpdateBmpInfo)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_PIXELX, OnUpdatePixelXIndicator)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_PIXELY, OnUpdatePixelYIndicator)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZRATIO, OnUpdateRatioIndicator)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLView construction/destruction

CPLView::CPLView()
{
    m_bFit = false;    // use real graphics dimensions
    m_bDither = true;  // use vfw's DrawDib engine
    m_bZooming = false;

    m_curPoint = AfxGetApp()->LoadCursor(IDC_POINTCURSOR);
    m_curMove  = AfxGetApp()->LoadCursor(IDC_PULLCURSOR);

	// KOL.UDF.022 시큐어코딩 보완
	m_nDocPosX = 0;
	m_nDocPosY = 0;
	m_hZoomCursor = NULL;
}


CPLView::~CPLView()
{
    if (m_curPoint != NULL) DestroyCursor(m_curPoint);
    if (m_curMove  != NULL) DestroyCursor(m_curMove );
}

/////////////////////////////////////////////////////////////////////////////
// CPLView drawing


void CPLView::OnDraw(CDC* pDC)
{
    CPLViewerDoc* pDoc = GetDocument();
    if ( ! pDoc ) return;

    PLWinBmpEx* pDib = pDoc->GetDib();
    if ( ! pDib ) return;

    // else, we have a bitmap to show!
	// CWaitCursor wc;

    CSize docsize = pDoc->GetDocSize();
    CRect rcImg(0,0,docsize.cx,docsize.cy);
    CRect rcDest;
    if (! pDC->IsPrinting())   // display DC
    {
	if (m_bFit)
	    GetClientRect( rcDest );
	else
	{
	    rcDest = rcImg = m_InvalidRect;
	    if ( m_InvalidRect.IsRectEmpty() )
		TRACE0( "CTiffView::OnDraw(): invalidated region is empty\n" );
	}
    }
    else  // printer device context
    {
        // get size of printer page (in pixels)
        int cxPage = pDC->GetDeviceCaps(HORZRES);
        int cyPage = pDC->GetDeviceCaps(VERTRES);
        // get printer pixels per inch
        int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
        int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

        // create a rectangle which preserves the document's aspect ratio,
        // and fills the page taking into account the relative size of a
        // printed pixel (cyInch / cxInch).

        rcDest.top = rcDest.left = 0;
        rcDest.right = cxPage;
        rcDest.bottom = MulDiv(docsize.cy * cyInch, cxPage, docsize.cx * cxInch);
        if (rcDest.bottom > cyPage)
        {
            rcDest.bottom = cyPage;
            rcDest.right  = MulDiv(docsize.cx * cxInch, cyPage, docsize.cy * cyInch);
        }
	// TRACE("(%d,%d);(%d,%d,%d,%d)\n", cxInch, cyInch,
	//    rcDest.left, rcDest.top, rcDest.right, rcDest.bottom );
    }

#ifndef USE_BANDING
#ifdef USES_DRAWDIB
    pDib->DrawEx(pDC->GetSafeHdc(), &rcDest, &rcImg, NULL, false, m_bDither ? -1 : 0);
#else
    int nStretchMode = COLORONCOLOR;
    int nScreenBPP   = thisApp.GetScreenBPP();
    // when in 256-color mode, see how haltoning performs
    if ( m_bDither && nScreenBPP <= 8 && nScreenBPP < pDib->GetBitsPerPixel() )
      nStretchMode = HALFTONE;

    pDib->DrawEx(
      pDC->GetSafeHdc(),
      &rcDest,
      &rcImg,
      NULL,	// default palette
      false,	// release in the background (until WM_QUERYNEWPALETTE)
              // adjust stretch mode to display color depth
	    nStretchMode);
#endif
#else
	//The following is the "striping" modification for HP printer
  //support of large images - Kal Krishnan.
	int iDestStripSize = (int)(((double)PRINT_BAND_SIZE*(double)rcDest.Height())/
                              (double)(rcImg.Height()));
	int iDestStripOverlap = (int)(((double)PRINT_BAND_OVERLAP*(double)rcDest.Height())/
                                (double)(rcImg.Height()));
	int iImgTop = rcImg.top;   // variable to hold the current
                             // position on source rect
	int iDestTop = rcDest.top; // same for destination
	int iDestBottom = rcDest.bottom;
	int iImgBottom = rcImg.bottom;
	for (; iDestTop < iDestBottom; iDestTop+=iDestStripSize-iDestStripOverlap)
	{
		rcDest.top = iDestTop;
		rcDest.bottom = iDestTop + iDestStripSize;
		rcImg.top = iImgTop;
		rcImg.bottom = rcImg.top + PRINT_STRIP_SIZE;
		if (rcDest.bottom > iDestBottom)	//last strip
		{
			rcDest.bottom = iDestBottom;
			rcImg.bottom = iImgBottom;
		}
		pDib->DrawEx(
			pDC->GetSafeHdc(),
			&rcDest,
      &rcImg,
      NULL,	// default palette
      false,	// release in the background (until WM_QUERYNEWPALETTE)
      nStretchMode); // adjust stretch mode to display color depth
		iImgTop+=PRINT_BAND_SIZE-PRINT_BAND_OVERLAP;
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CPLView printing

BOOL CPLView::OnPreparePrinting(CPrintInfo* pInfo)
{
    pInfo->SetMaxPage(1);
    // default preparation
    return DoPreparePrinting(pInfo);
}

// We shouldn't have to override this, but I noticed printing
// was broken when going through CZoomView...
void CPLView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    if ( ! pDC->IsPrinting())
	Super::OnPrepareDC( pDC, pInfo );
    else
	CScrollView::OnPrepareDC( pDC, pInfo );
}

////////////////////////////////////////
//  panning helper class

class CHandTracker : public CRectTracker
{
    CPLView *m_pView;
public:
    CHandTracker(CPLView *pView);
    virtual void OnChangedRect(const CRect& rectOld);
    virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
	{ /* no-op ! */ }
};


CHandTracker::CHandTracker(CPLView *pView)
{
    ASSERT(pView != NULL);
    m_pView = pView;
    m_sizeMin = CSize(1, 1);
}


void CHandTracker::OnChangedRect(const CRect& rectOld)
{
    CPoint delta;
    // call the view to update scroll position
    delta.x = m_rect.Width()  - rectOld.Width();
    delta.y = m_rect.Height() - rectOld.Height();
    // m_pView->ViewDPtoLP( &delta );
    CPoint anchor = m_pView->GetScrollPosition();
    anchor -= delta;

    if (anchor.x < 0) anchor.x = 0;
    if (anchor.y < 0) anchor.y = 0;
    if (! (m_pView->GetStyle() & WS_VSCROLL)) anchor.x = 0;
    if (! (m_pView->GetStyle() & WS_HSCROLL)) anchor.x = 0;

    if ( delta.x || delta.y )
	m_pView->ScrollToPosition( anchor );
}


/////////////////////////////////////////////////////////////////////////////
// CPLView commands

/*
 *  Woosh, we got the WM_DOREALIZE message from MDI supervisor,
 *  time to shine our brightest colors
 */
LRESULT CPLView::OnDoRealize(WPARAM wParam, LPARAM)
{
    bool bRet = false;
    ASSERT(wParam != NULL);
    CPLViewerDoc* pDoc = GetDocument();
    if (pDoc->GetDib() == NULL)
        return bRet;  // must be a new document

    CPalette* pPal = CPalette::FromHandle( pDoc->GetDocPalette() );
    if (pPal != NULL)
    {
        CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
        ASSERT_KINDOF(CMainFrame, pAppFrame);

        CClientDC appDC(pAppFrame);
        // All views but one should be a background palette.
        // wParam contains a handle to the active view, so the SelectPalette
        // bForceBackground flag is false only if wParam == m_hWnd (this view)
        CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);

        if (oldPalette != NULL)
        {
            UINT nColorsChanged = appDC.RealizePalette();
            if (nColorsChanged > 0)
            {
                //pDoc->UpdateAllViews(NULL);
                Invalidate(false);
                //UpdateWindow();
                bRet = true;
            }
            appDC.SelectPalette(oldPalette, true);
        }
        else
        {
            TRACE0("\tSelectPalette failed in CPLView::OnDoRealize()\n");
            //MessageBeep(0);
        }

    }

    return (LRESULT) bRet;
}

void CPLView::NotifyRanges()
{
#ifdef USES_DRAWDIB
    SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
#else
    SetZoomSizes(GetDocument()->GetDocSize());
    SetZoomMode( MODE_ZOOMOFF );
    m_bZooming = false;
    SetZoomRatio(1.0);	// not adjusted by CZoomView ?!?
#endif
}

void CPLView::OnInitialUpdate()
{
    Super::OnInitialUpdate();
    ASSERT(GetDocument() != NULL);

    // set scrolling ranges
    NotifyRanges();

    // size window perfectly around frame
    GetParentFrame()->RecalcLayout();
    OnSizeToFit();
}

void CPLView::OnActivateView(bool bActivate, CView* pActivateView,
                             CView* pDeactiveView)
{
    Super::OnActivateView(bActivate, pActivateView, pDeactiveView);

    if (bActivate)
        //if (pDeactiveView != pActivateView)
    {
        // probably we switched from view/doc within MDI app,
        // so this view deserves foreground palette
        ASSERT(pActivateView == this);
        OnDoRealize((WPARAM)m_hWnd, 0);    // same as SendMessage(WM_DOREALIZE);
    }
}

BOOL CPLView::OnEraseBkgnd(CDC* pDC)
{
#ifdef USES_DRAWDIB
    if (m_bFit)
        // in this case, we cover the whole client area with our
        // drawing, so there's no need to erase
        return true;
#else
    if ((GetStyle() & WS_VSCROLL) &&
	(GetStyle() & WS_HSCROLL))  // page occupies whole view
	return true;
#endif
    else
        return Super::OnEraseBkgnd(pDC);
}


void CPLView::OnSize(UINT nType, int cx, int cy)
{
    Super::OnSize(nType, cx, cy);

    if (m_bFit)  // track new dimension, avoid scrollbars
        SetScaleToFitSize(CSize(cx,cy));
}


// In order to perform minimal redrawing, we need to capture
// the update area, which is unreachable in OnDraw()
void CPLView::OnPaint()
{
    if ( GetUpdateRect( m_InvalidRect ))
    {
	m_InvalidRect.NormalizeRect();
	// TRACE_RECT( m_InvalidRect );
#ifndef USES_DRAWDIB
	ViewDPtoLP( (LPPOINT) &m_InvalidRect, 2 );
#endif
	CRect docrect( CPoint(0,0), GetDocument()->GetDocSize() );
	m_InvalidRect.InflateRect( 1, 1 );	//?!?
	m_InvalidRect &= docrect;
    }
    else
	m_InvalidRect.SetRect( 0,0,0,0 );

    Super::OnPaint();
}

// user toggles the 'fit image to viewable area' button
void CPLView::OnFitImage()
{
    if (m_bFit = ! m_bFit)
    {
        CRect cr;
        GetClientRect( cr );
        SetScaleToFitSize(cr.Size());
    }
    else
        NotifyRanges();

    //pDoc->UpdateAllViews(NULL);
    Invalidate(false);
}

void CPLView::OnUpdateFitImage(CCmdUI* pCmdUI)
{
#ifndef USES_DRAWDIB
    pCmdUI->Enable(false);
#else
    pCmdUI->SetCheck(m_bFit);
#endif
}

void CPLView::OnUpdateViewShowpalette(CCmdUI* pCmdUI)
{
  PLWinBmpEx* pDib = GetDocument()->GetDib();
  if (pDib->GetBitsPerPixel() == 8)
    pCmdUI->Enable(true);
  else
    pCmdUI->Enable(false);
}

void CPLView::OnZoomMode()
{
#ifndef USES_DRAWDIB
    m_bZooming = ! m_bZooming;
    SetZoomMode(m_bZooming ? MODE_ZOOMIN : MODE_ZOOMOFF);
#endif
}

void CPLView::OnUpdateZoomMode(CCmdUI* pCmdUI)
{
#ifdef USES_DRAWDIB
    pCmdUI->Enable(false);
#else
    pCmdUI->SetCheck(m_bZooming);
#endif
}

// experiment GDI vs VFW DIB drawing

void CPLView::OnDither()
{
    m_bDither = ! m_bDither;
    Invalidate(false);
}

void CPLView::OnUpdateDither(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bDither);
}

void CPLView::OnEditCopy()
{
    PLWinBmpEx* pDib = GetDocument()->GetDib();
    pDib->ToClipboard();
}

void CPLView::OnEditPaste()
{
    PLWinBmpEx* pDib = GetDocument()->GetDib();
    pDib->FromClipboard();
    Invalidate(true);
    NotifyRanges();
}


// a Super has no keyboard interface, so let's roll our own
void CPLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch( nChar )
    {
    case VK_HOME:
        SendMessage(::GetAsyncKeyState(VK_CONTROL) ?
                    WM_VSCROLL : WM_HSCROLL,SB_TOP);
        break;
    case VK_END:
        SendMessage(::GetAsyncKeyState(VK_CONTROL) ?
                    WM_VSCROLL : WM_HSCROLL,SB_BOTTOM);
        break;
    case VK_PRIOR:
        SendMessage(WM_VSCROLL,SB_PAGEUP);
        break;
    case VK_NEXT:
        SendMessage(WM_VSCROLL,SB_PAGEDOWN);
        break;
    case VK_UP:
        SendMessage(WM_VSCROLL,SB_LINEUP);
        break;
    case VK_DOWN:
        SendMessage(WM_VSCROLL,SB_LINEDOWN);
        break;
    case VK_LEFT:
        SendMessage(WM_HSCROLL,SB_LINEUP);
        break;
    case VK_RIGHT:
        SendMessage(WM_HSCROLL,SB_LINEDOWN);
        break;
    default:
        Super::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void CPLView::OnUpdateTrueColFilter(CCmdUI* pCmdUI) 
{
  PLWinBmpEx* pDib = GetDocument()->GetDib();
  if (pDib->GetBitsPerPixel() < 24)
    pCmdUI->Enable (false);
  else
    pCmdUI->Enable (true);
}


void CPLView::OnFilterBilinear()
{
  PLFilterDialog Dlg (this, GetDocument()->GetDib(), "Bilinear Resize", false);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterResizeBilinear Filter (Dlg.GetWidth(), Dlg.GetHeight());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterBox()
{
  PLFilterDialog Dlg (this, GetDocument()->GetDib(), "Box filter", false);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterResizeBox Filter (Dlg.GetWidth(), Dlg.GetHeight());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterGauss()
{
  PLFilterDialog Dlg (this, GetDocument()->GetDib(), "Gaussian blur", true);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterResizeGaussian Filter (Dlg.GetWidth(), Dlg.GetHeight(), Dlg.GetRadius());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterHamming()
{
  PLFilterDialog Dlg (this, GetDocument()->GetDib(), "Hamming", true);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterResizeHamming Filter (Dlg.GetWidth(), Dlg.GetHeight(), Dlg.GetRadius());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterCrop()
{
  PLWinBmpEx* pDib = GetDocument()->GetDib();
  if (pDib->GetBitsPerPixel() == 1)
  {
      MessageBox ("This filter doesn't work for b/w bitmaps.", "piclook");
      return;
  }

  CCropFilterDlg Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterCrop Filter (Dlg.m_XMin, Dlg.m_YMin, Dlg.m_XMax, Dlg.m_YMax);
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterGrayscale()
{
  CWaitCursor wc;
  PLWinBmpEx* pDib = GetDocument()->GetDib();

  pDib->ApplyFilter (PLFilterGrayscale());
  Invalidate(true);
  NotifyRanges();
}

void CPLView::OnFilterThreshold()
{
  CThresholdDlg Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterThreshold Filter (Dlg.GetMinThreshold(), Dlg.GetMaxThreshold(),Dlg.GetChannel());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterContrast() 
{
  CContrastDlg Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterContrast Filter (Dlg.GetContrast(), Dlg.GetOffset());
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterIntensity() 
{
  CIntensityDlg Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterIntensity Filter (Dlg.m_Intensity, Dlg.m_Offset, Dlg.m_Exponent);
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterLightness() 
{
  CLightnessDlg Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    PLFilterLightness Filter (Dlg.m_Lightness);
    applyFilterAndShow (&Filter);
  }
}

void CPLView::OnFilterInvert() 
{
  PLFilterVideoInvert Filter;
  applyFilterAndShow (&Filter);
}

void CPLView::OnConvertTo8BPP()
{
  CDitherDialog Dlg(this);
  if (Dlg.DoModal() == IDOK)
  {
    CWaitCursor wc;

    GetDocument()->ConvertTo8BPP(Dlg.m_iDitherPaletteType, Dlg.m_iDitherType);
    InvalidateRect(NULL);
    NotifyRanges();
  }
}

void CPLView::OnViewShowpalette()
{
  PLWinBmpEx* pDib = GetDocument()->GetDib();
  CPalViewDlg Dlg (this, pDib->GetPalette());
  Dlg.DoModal();
}

BOOL CPLView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest != HTCLIENT)
        m_nDocPosX = m_nDocPosY = -1;

    //    if (GetZoomMode() == MODE_ZOOMOFF && nHitTest == HTCLIENT && IsMovable() )
    if (! m_bZooming && nHitTest == HTCLIENT && IsMovable() )
    {
        ::SetCursor(m_curPoint);
        return true;
    }
    else
        return Super::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CPLView::IsMovable() const
{
    return (GetStyle() & (WS_VSCROLL | WS_HSCROLL));
}

void CPLView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // if (GetZoomMode() == MODE_ZOOMOFF && IsMovable())
    if (! m_bZooming && IsMovable())
    {
	HCURSOR prvCur = ::SetCursor(m_curMove);

	if ( CHandTracker(this).TrackRubberBand(this, point) )
	    Invalidate(false);

	::SetCursor(prvCur);
    }
#ifndef USES_DRAWDIB
    else if (m_bZooming)
    {
	// emulate 3-button mouse with <Ctrl> key
	if ( nFlags & MK_CONTROL )
	    OnMButtonDown(nFlags, point);
	else
	    Super::OnLButtonDown(nFlags, point);
    }
#endif
}

void CPLView::OnRButtonDown(UINT nFlags, CPoint point)
{
#ifndef USES_DRAWDIB
    if (GetZoomMode() == MODE_ZOOMOFF)
	NotifyRanges();
    else
    {
	BOOL bWasZoomed = IsMovable();
	ViewDPtoLP( &point );
	DoZoomOut( &point );
    }
#endif
}

void CPLView::OnMButtonDown(UINT nFlags, CPoint point)
{
#ifndef USES_DRAWDIB
    if (GetZoomMode() != MODE_ZOOMOFF)
	DoZoomFull();
#ifdef OWND
    else if ( IsMovable() )
	StartPanning( this, GetStyle() & WS_VSCROLL, GetStyle() & WS_HSCROLL, point );
#endif	// OWND
#endif	// DRAWDIB
}

void CPLView::OnMouseMove(UINT nFlags, CPoint point)
{
    CPoint pt( point );
#ifndef USES_DRAWDIB
    ViewDPtoLP( &pt );
#endif
    CSize docsize = GetDocument()->GetDocSize();
    if ( pt.x >= 0 && pt.x < docsize.cx
	    && pt.y >= 0 && pt.y < docsize.cy )
    {
        m_nDocPosX = pt.x;
        m_nDocPosY = pt.y;
    }
    else
        m_nDocPosX = m_nDocPosY = -1;

    Super::OnMouseMove(nFlags, point);
}

void CPLView::OnUpdateBmpInfo(CCmdUI *pCmdUI)
{
  PLWinBmpEx* pDib = GetDocument()->GetDib();
  ASSERT( pDib );
  CString str;
  str.Format(" %d x %d; %d bpp; %d x %d dpi; %d kb",
             pDib->GetWidth(),
             pDib->GetHeight(),
             pDib->GetBitsPerPixel(),
             pDib->GetResolution().x,
             pDib->GetResolution().y,
             pDib->GetMemUsed() / 1024
            );
  pCmdUI->SetText( str );
}

void CPLView::OnUpdatePixelXIndicator(CCmdUI *pCmdUI)
{
    char cbuf[8] = "";
    if (m_nDocPosX >= 0) wsprintf( cbuf, "%5d", m_nDocPosX );
    pCmdUI->SetText( cbuf );
}


void CPLView::OnUpdatePixelYIndicator(CCmdUI *pCmdUI)
{
    char cbuf[8] = "";
    if (m_nDocPosY >= 0) wsprintf( cbuf, "%5d", m_nDocPosY );
    pCmdUI->SetText( cbuf );
}

void CPLView::OnUpdateRatioIndicator(CCmdUI *pCmdUI)
{
#ifndef USES_DRAWDIB
    char cbuf[8];
    double r = GetZoomRatio();
    if ( r >= 1.0 )
	sprintf( cbuf, "%.2f:1", r );
    else
	sprintf( cbuf, "1:%.2f", 1/r );
    pCmdUI->SetText( cbuf );
#endif
}

//////////////////
// Size To Fit command (courtesy of Paul DiLascia!)
//
void CPLView::OnSizeToFit()
{
    ResizeParentToFit(false);

    // MFC might have grown the window off the screen--I'll fix it
    CRect rc, rcMax;
    CFrameWnd* pFrame = GetParentFrame();
    ASSERT_VALID(pFrame);
    pFrame->GetWindowRect(&rc);
    CWnd* pGrandParent = pFrame->GetParent();
    if (pGrandParent)
    {
        // use top level window as maximum rectangle
        pGrandParent->ScreenToClient(&rc);
        pGrandParent->GetClientRect(&rcMax);
    }
    else
    {
        // use whole screen as maximum rectangle
        rcMax.SetRect(0,0,GetSystemMetrics(SM_CXSCREEN),
                      GetSystemMetrics(SM_CYSCREEN));
    }

    bool bTooBig = false;
    if (rc.bottom > rcMax.bottom)
    {
        rc.bottom = rcMax.bottom;
        rc.right += GetSystemMetrics(SM_CXVSCROLL) +
                    GetSystemMetrics(SM_CXBORDER);
        bTooBig = true;
    }
    if (rc.right > rcMax.right)
    {
        rc.right = rcMax.right;
        bTooBig = true;
    }
    if (bTooBig)
    {
        pFrame->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(),
                             SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    }
}


void CPLView::applyFilterAndShow (PLFilter * pFilter)
{
	PLWinBmpEx* pDib = GetDocument()->GetDib();
  CWaitCursor wc;
  pFilter->ApplyInPlace (pDib);
  Invalidate(true);
	NotifyRanges();
}

/*
/--------------------------------------------------------------------
|
|      $Log: View.cpp,v $
|      Revision 1.1  2009/12/09 05:34:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:22  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:33  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:31  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:50  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:11  황정영
|      2006.05.17
|
|      Revision 1.30  2002/03/29 19:18:23  uzadow
|      Fixed bug in crop dialog.
|
|      Revision 1.29  2002/02/24 13:00:56  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.28  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.27  2001/10/16 17:12:27  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.26  2001/09/30 16:57:26  uzadow
|      Improved speed of 2passfilter.h, code readability changes.
|
|      Revision 1.25  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.24  2001/01/15 15:05:32  uzadow
|      Added CBmp::ApplyFilter() and CBmp::CreateFilteredCopy()
|
|      Revision 1.23  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.22  2000/12/05 00:00:06  uzadow
|      Added lightness dialog.
|
|      Revision 1.21  2000/11/06 23:22:52  uzadow
|      Added dialogs for Contrast and Intensity
|
|      Revision 1.20  2000/10/23 21:13:29  uzadow
|      Removed Filter
|
|      Revision 1.19  2000/10/12 21:59:34  uzadow
|      Added CreateFromHDIBBitmap() and CopyPalette() to PLWinBmp
|      Added CF_DIB support to PLWinBmp::FromClipboard() (Richard Hollis)
|
|      Revision 1.18  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|      Revision 1.17  2000/09/26 12:14:50  Administrator
|      Refactored quantization.
|
|      Revision 1.16  2000/07/11 17:18:54  Ulrich von Zadow
|      Added support for banded printing.
|
|      Revision 1.15  2000/07/07 14:00:26  Ulrich von Zadow
|      Minor bugfixes.
|
|      Revision 1.14  2000/05/28 11:09:56  Ulrich von Zadow
|      no message
|
|      Revision 1.13  2000/03/31 12:20:07  Ulrich von Zadow
|      Video invert filter (beta)
|
|      Revision 1.12  2000/03/31 11:53:32  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.11  2000/03/30 21:52:16  Ulrich von Zadow
|      Minor bugfix.
|
|      Revision 1.10  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delm?.
|
|      Revision 1.8  2000/01/10 23:53:02  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.7  1999/12/30 15:54:48  Ulrich von Zadow
|      Added PLWinBmp::FromClipBoard() and CreateFromHBitmap().
|
|      Revision 1.6  1999/12/02 17:07:36  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.5  1999/10/21 18:48:18  Ulrich von Zadow
|      no message
|
|      Revision 1.4  1999/10/21 16:07:06  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.3  1999/10/19 21:33:49  Ulrich von Zadow
|      Added filter support.
|
|
\--------------------------------------------------------------------
*/
