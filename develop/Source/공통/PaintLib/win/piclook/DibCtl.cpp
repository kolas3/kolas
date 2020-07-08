/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Jorge Lodos
// All rights reserved
//
// Distribute and use freely, except:
// 1. Don't alter or remove this notice.
// 2. Mark the changes you made
//
// Send bug reports, bug fixes, enhancements, requests, etc. to:
//    lodos@cigb.edu.cu
/////////////////////////////////////////////////////////////////////////////

/*
/--------------------------------------------------------------------
|
|      $Id: DibCtl.cpp,v 1.1 2009/12/09 05:34:49 박종운 Exp $
|      graphic file preview in static control implementation
|
|      Adapted from Mr.Lodos's code in order to use PaintLib's
|      windows bitmap class and multi-format decoding
|
|      B.Delm? 1998
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "PicLook.h"
#include "DIBCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBStatic

BEGIN_MESSAGE_MAP(CDIBStatic, CStatic)
    //{{AFX_MSG_MAP(CDIBStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


bool CDIBStatic::LoadDib(LPCTSTR lpszFileName)
{
    m_bHasFile = false;
    if (! (::GetFileAttributes(lpszFileName) & FILE_ATTRIBUTE_DIRECTORY))
    {
        try
        {
	    CWaitCursor wc;
            thisApp.GetDecoder()->MakeBmpFromFile(lpszFileName, &m_DIB, 0);
            m_bHasFile = true;
            // as we keep reusing the same bitmap,
            // we need to refresh the palette for each new file
            m_DIB.BuildLogPalette();
            (void) DoRealizePalette(false);
        }
        catch (PLTextException &e)
        {
            //AfxMessageBox(e);
            TRACE0((LPCSTR) e );
            //m_DIB.freeMembers();
            m_bHasFile = false;
        }
    }
    UpdateDib();
    return m_bHasFile;
}

void CDIBStatic::ClearDib()
{
    ASSERT_VALID(this);

    CClientDC dc(this);
    CRect rectPaint;
    GetClientRect(&rectPaint);
    rectPaint.InflateRect(-1,-1);

    dc.FillSolidRect(&rectPaint, m_bHasFile ?
                     ::GetSysColor(COLOR_3DFACE) :
                     ::GetSysColor(COLOR_3DSHADOW) );
    //  ModifyStyle(m_bHasFile ? WS_BORDER : 0, m_bHasFile ? 0 : WS_BORDER);
}


void CDIBStatic::PaintDib(bool bDibValid)
{
    ASSERT_VALID(this);
    ClearDib();

    if (bDibValid)
    {
        CRect PaintRect;
        GetClientRect(&PaintRect);
        PaintRect.InflateRect(-1, -1);
        CClientDC dc(this);

        double stretchFactor = 1.0;
        if (m_DIB.GetWidth() > PaintRect.Width() || m_DIB.GetHeight() > PaintRect.Height())
        { // The bitmap doesn't fit, scale to fit
            double rx = (double) m_DIB.GetWidth() / PaintRect.Width();
            double ry = (double) m_DIB.GetHeight() / PaintRect.Height();
            stretchFactor = 1.0 / max( rx, ry );
        }

        int w, h, nDestX, nDestY;
        w = PaintRect.right  - PaintRect.left;
        h = PaintRect.bottom - PaintRect.top;
        nDestX = PaintRect.left + (w - int(stretchFactor * m_DIB.GetWidth() )) / 2;
        nDestY = PaintRect.top  + (h - int(stretchFactor * m_DIB.GetHeight())) / 2;
        m_DIB.StretchDraw( dc.GetSafeHdc(), nDestX, nDestY, stretchFactor );
    }
}

void CDIBStatic::UpdateDib()
{
    ASSERT_VALID(this);
    PaintDib(IsValidDib());
}

/////////////////////////////////////////////////////////////////////////////
// CDIBStatic message handlers

HBRUSH CDIBStatic::CtlColor(CDC* /* pDC */, UINT /* nCtlColor */)
{
    UpdateDib();

    // TODO: Return a non-NULL brush if the parent's handler should not be called
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}


// REMARK: We actually never get to see the palette negotiation
// messages, which are not forwarded to us by the MFC mainframe;
// so we just take our chances in LoadDib()

BOOL CDIBStatic::OnQueryNewPalette()
{
    return DoRealizePalette(false);
}

void CDIBStatic::OnPaletteChanged(CWnd* /* pFocusWnd */)
{
    (void) DoRealizePalette(true);
}

bool CDIBStatic::DoRealizePalette(bool bForceBackGround)
{
    if (IsValidDib())
    {
        CPalette *pPal = CPalette::FromHandle( m_DIB.GetLogPalette() );
        if (pPal == NULL)
            return false;

        CClientDC dc(this);
        CPalette *pOldPalette = dc.SelectPalette( pPal, bForceBackGround );
        UINT nChanged = dc.RealizePalette();
        dc.SelectPalette( pOldPalette, true );

        if (nChanged == 0)  // no change to our mapping
            return false;

        // some changes have been made; invalidate
        UpdateDib();
    }

    return true;
}

void CDIBStatic::RemoveDib()
{
    //m_DIB.Invalidate();
    //m_DIB.freeMembers();
    m_bHasFile = false;
    UpdateDib();
}
/*
/--------------------------------------------------------------------
|
|      $Log: DibCtl.cpp,v $
|      Revision 1.1  2009/12/09 05:34:49  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:20  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:31  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:09  황정영
|      2006.05.17
|
|      Revision 1.9  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.8  2002/01/27 18:20:18  uzadow
|      Updated copyright message; corrected pcx decoder bug.
|
|      Revision 1.7  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.5  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
