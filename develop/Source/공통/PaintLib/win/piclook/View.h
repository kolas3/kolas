/*The zlib/libpng License

Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/
/*
/--------------------------------------------------------------------
|
|      $Id: View.h,v 1.1 2009/12/09 05:34:55 ¹ÚÁ¾¿î Exp $
|      view class declaration
|
|      Copyright (c) 1998 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#ifndef INCL_VIEW
#define INCL_VIEW

#include "ZoomView.h"

#ifdef USES_DRAWDIB
#define CPLVIEW_BASE_CLASS CScrollView
#else
#define CPLVIEW_BASE_CLASS CZoomView
#endif

class CPLView : public CPLVIEW_BASE_CLASS
{
protected: // create from serialization only
    CPLView();
    DECLARE_DYNCREATE(CPLView)

    // Attributes
public:
    CPLViewerDoc* GetDocument()
    {
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPLViewerDoc)));
        return (CPLViewerDoc*) m_pDocument;
    }

    // Operations
public:

    // Implementation
public:
    virtual ~CPLView();
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view

    virtual void OnInitialUpdate();
    virtual void OnActivateView(bool bActivate, CView* pActivateView,
                                CView* pDeactiveView);
    void OnConvertTo8BPP();
//  CString GetInfoString();

    // Printing support
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

private:
    void applyFilterAndShow (PLFilter * pFilter);

    typedef CPLVIEW_BASE_CLASS Super;

    bool    m_bFit;	// whether to fit image to view
    bool    m_bDither;  // whether to use palette dithering;
    bool    m_bZooming;
    HCURSOR m_curMove;
    HCURSOR m_curPoint;
    HCURSOR m_hZoomCursor;
    CRect   m_InvalidRect;
    int	    m_nDocPosX;
    int	    m_nDocPosY;

    void NotifyRanges();
    BOOL IsMovable() const;

    // Generated message map functions
protected:
    //{{AFX_MSG(CPLView)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
    afx_msg void OnUpdateFitImage(CCmdUI* pCmdUI);
    afx_msg void OnFitImage();
    afx_msg void OnUpdateDither(CCmdUI* pCmdUI);
    afx_msg void OnDither();
    afx_msg void OnEditCopy();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnFilterBilinear();
    afx_msg void OnFilterBox();
    afx_msg void OnFilterGauss();
    afx_msg void OnFilterHamming();
    afx_msg void OnFilterCrop();
    afx_msg void OnFilterGrayscale();
    afx_msg void OnEditPaste();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSizeToFit();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnUpdateZoomMode(CCmdUI* pCmdUI);
    afx_msg void OnZoomMode();
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	  afx_msg void OnFilterThreshold();
	  afx_msg void OnUpdateViewShowpalette(CCmdUI* pCmdUI);
	  afx_msg void OnViewShowpalette();
	  afx_msg void OnFilterContrast();
	  afx_msg void OnFilterIntensity();
	  afx_msg void OnFilterLightness();
	  afx_msg void OnUpdateTrueColFilter(CCmdUI* pCmdUI);
	  afx_msg void OnFilterInvert();
	//}}AFX_MSG
    afx_msg void OnUpdateBmpInfo(CCmdUI *);
    afx_msg void OnUpdatePixelXIndicator(CCmdUI *pCmdUI);
    afx_msg void OnUpdatePixelYIndicator(CCmdUI *pCmdUI);
    afx_msg void OnUpdateRatioIndicator(CCmdUI *pCmdUI);

    DECLARE_MESSAGE_MAP()
};

#undef CPLVIEW_BASE_CLASS

#endif  // INCL_VIEW

/*
/--------------------------------------------------------------------
|
|      $Log: View.h,v $
|      Revision 1.1  2009/12/09 05:34:55  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:10  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:22  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:35  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:31  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:52  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:31:11  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.20  2002/02/24 13:00:57  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.19  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.18  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.17  2000/11/06 23:22:53  uzadow
|      Added dialogs for Contrast and Intensity
|
|      Revision 1.16  2000/10/23 21:13:29  uzadow
|      Removed Filter
|
|      Revision 1.15  2000/10/12 21:59:34  uzadow
|      Added CreateFromHDIBBitmap() and CopyPalette() to PLWinBmp
|      Added CF_DIB support to PLWinBmp::FromClipboard() (Richard Hollis)
|
|      Revision 1.14  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|      Revision 1.13  2000/09/26 12:14:50  Administrator
|      Refactored quantization.
|
|      Revision 1.12  2000/03/31 12:20:07  Ulrich von Zadow
|      Video invert filter (beta)
|
|      Revision 1.11  2000/03/31 11:53:32  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.10  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delmée.
|
|      Revision 1.8  2000/01/10 23:53:03  Ulrich von Zadow
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
