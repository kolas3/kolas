/*
/--------------------------------------------------------------------
|
|      $Id: WinBmpEx.cpp,v 1.1 2009/12/09 05:34:55 ¹ÚÁ¾¿î Exp $
|	     PaintLib windows DrawDib bitmap class implementation
|
|	     Copyright (c) 1998-2000 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "WinBmpEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PLWinBmpEx::PLWinBmpEx()
{
#ifndef _WITHOUT_VFW_
    m_hdd = 0;
#endif
    m_pal = 0;
}

PLWinBmpEx::~PLWinBmpEx()
{
#ifndef _WITHOUT_VFW_
    if (m_hdd)	::DrawDibClose(m_hdd);
#endif
    if (m_pal)	::DeleteObject(m_pal);
}


//////////////////////////////////////////////////////////////////////
// Draw DIB on caller's DC. Does stretching from source to destination
// rectangles. Generally, you can let the following default to zero/NULL:
//
//    nStretchMode= 0 means 'use DrawDib'; any other value must be
//                  a valid stretch-mode and will force GDI usage		
//    hPal        = palette, default=NULL, (use DIB's palette)
//    bForeground = realize in foreground (default true)
//
// If you are handling palette messages, you should use bForeground=false,
// since you will realize the foreground palette in WM_QUERYNEWPALETTE.
//
BOOL PLWinBmpEx::DrawEx( HDC dc, const RECT* rcDst, const RECT* rcSrc,
		        HPALETTE hPal, bool bForeground,
			int nStretchMode, DWORD ropcode )
{
    // Select, realize palette
    if (hPal==NULL)        // no palette specified:
        hPal = GetLogPalette();    // use default
    HPALETTE hOldPal = ::SelectPalette(dc, hPal, !bForeground);
    ::RealizePalette(dc);

    // Compute rectangles where NULL specified
    RECT rc;
    if (!rcSrc)
    {  // if no source rect, use whole bitmap
        rc.left = rc.top = 0;
        rc.right    = GetWidth();
        rc.bottom   = GetHeight();
        rcSrc = &rc;
    }
    if (!rcDst) // if no destination rect, use source
        rcDst = rcSrc;

    BOOL bRet = false;

#ifndef _WITHOUT_VFW_
    bool bUseDrawDib = nStretchMode < 0;
    if (bUseDrawDib)
    {
        if (!m_hdd)  VERIFY(m_hdd = DrawDibOpen());

        // Let DrawDib do the work!
        bRet = ::DrawDibDraw( m_hdd, dc,
                              rcDst->left,
                              rcDst->top,
                              rcDst->right  - rcDst->left,
                              rcDst->bottom - rcDst->top,
                              GetBMI(),
                              GetBits(),
                              rcSrc->left,
                              rcSrc->top,
                              rcSrc->right  - rcSrc->left,
                              rcSrc->bottom - rcSrc->top,
                              bForeground ? 0 : DDF_BACKGROUNDPAL);
    }
    else      // use normal draw function
#endif
    {
	// If user wanted DrawDib but vfw support is not linked in,
	// then try to use best looking GDI mode
	if (nStretchMode < 0) nStretchMode = HALFTONE;
	int osm = 0;
	if (nStretchMode > 0) osm = ::SetStretchBltMode( dc, nStretchMode );
#if 0	// way too slooooooooooooooooooow!        
	bRet = GDI_ERROR != ::StretchDIBits(
	                dc,
                        rcDst->left,
                        rcDst->top,
                        rcDst->right  - rcDst->left,
                        rcDst->bottom - rcDst->top,
                        rcSrc->left,
                        rcSrc->top,
                        rcSrc->right  - rcSrc->left,
                        rcSrc->bottom - rcSrc->top,
                        GetBits(),       // Pointer to bits
                        (BITMAPINFO*) GetBMI(),        // BITMAPINFO
                        DIB_RGB_COLORS,  // Options
                        ropcode);        // Raster operator code
#else
	HDC hSrcDC;
	hSrcDC = CreateCompatibleDC (dc);
	HGDIOBJ hOldBmp = ::SelectObject (hSrcDC, GetHandle());
	if (m_pClrTab)	// uses associated colormap
	    (void) ::SetDIBColorTable (hSrcDC, 0, 256, (RGBQUAD *)m_pClrTab);

	bRet = ::StretchBlt (	dc,
				rcDst->left,
				rcDst->top,
				rcDst->right  - rcDst->left,
				rcDst->bottom - rcDst->top,
				hSrcDC,
				rcSrc->left,
				rcSrc->top,
				rcSrc->right  - rcSrc->left,
				rcSrc->bottom - rcSrc->top,
				ropcode );

	::SelectObject (hSrcDC, hOldBmp);
	::DeleteDC(hSrcDC);

	// restore GDI stretch-mode
	if (osm) 
	    (void) ::SetStretchBltMode( dc, osm );
    }
#endif	// _WITHOUT_VFW_

    // restore DC palette
    if (hOldPal)
        ::SelectPalette(dc, hOldPal, true);

    return bRet;
}

// Draw DIB on DC.
void PLWinBmpEx::Draw( HDC dc, int x, int y, DWORD )
{
    // Compute destination rectangle
    RECT rcDst;
    rcDst.left	    = x;
    rcDst.top	    = y;
    rcDst.right	    = x + GetWidth();
    rcDst.bottom    = y + GetHeight();
    (void) DrawEx( dc, &rcDst );
}

// Draw part of the DIB on a DC.
BOOL PLWinBmpEx::DrawExtract( HDC dc, POINT pntDest, RECT rcSrc )
{
    // Compute destination rectangle
    RECT rcDst;
    rcDst.left	    = pntDest.x;
    rcDst.top	    = pntDest.y;
    rcDst.right	    = pntDest.x + abs(rcSrc.right  - rcSrc.left);
    rcDst.bottom    = pntDest.y + abs(rcSrc.bottom - rcSrc.top );
    return DrawEx( dc, &rcDst, &rcSrc );
}

// Play with the elastic bitmap...
void PLWinBmpEx::StretchDraw( HDC dc, int x, int y, double Factor, DWORD )
{
    // Compute rectangles
    RECT rcSrc, rcDst;
    rcSrc.left	    = 0;
    rcSrc.top	    = 0;
    rcSrc.right	    = GetWidth();
    rcSrc.bottom    = GetHeight();
    rcDst.left	    = x;
    rcDst.top	    = y;
    rcDst.right	    = x + int(Factor * GetWidth() );
    rcDst.bottom    = y + int(Factor * GetHeight());
    (void) DrawEx( dc, &rcDst, &rcSrc );
}

// Draws the bitmap on the given device context.
// Scales the bitmap so w is the width and h the height.
void PLWinBmpEx::StretchDraw( HDC hDC, int x, int y, int w, int h, DWORD )
{
    // Compute rectangles
    RECT rcSrc, rcDst;
    rcSrc.left	    = 0;
    rcSrc.top	    = 0;
    rcSrc.right	    = GetWidth();
    rcSrc.bottom    = GetHeight();
    rcDst.left	    = x;
    rcDst.top	    = y;
    rcDst.right	    = x + w;
    rcDst.bottom    = y + h;
    (void) DrawEx( hDC, &rcDst, &rcSrc );
}


//////////////////////////////////////////////////////////////////////
// all code hereunder related to palette management

#define PALVERSION 0x300  // magic number for LOGPALETTE

const int  MAXPALCOLORS = 256;

//////////////////
// Create the palette. Use halftone palette for hi-color bitmaps.
//
HPALETTE PLWinBmpEx::CreatePalette(void)
{
    // should not already have palette
    ASSERT( m_pal == 0 );

    HPALETTE hRet = HPALETTE(0);
    if (m_pClrTab)
    {
        // RGBQUAD* colors = new RGBQUAD[MAXPALCOLORS];
        UINT nColors = GetNumColors();  // 256, really...
        // Allocate memory for logical palette
        int len = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColors;
        LOGPALETTE* pLogPal = (LOGPALETTE*)new char[len];
        if (!pLogPal)
            return hRet;

        // set version and number of palette entries
        pLogPal->palVersion = PALVERSION;
        pLogPal->palNumEntries = (WORD) nColors;

        // copy color entries
        for (UINT i = 0; i < nColors; i++)
        {
            PLBYTE * pb = (PLBYTE *) (m_pClrTab+i);
            pLogPal->palPalEntry[i].peRed   = pb[PL_RGBA_RED  ];
            pLogPal->palPalEntry[i].peGreen = pb[PL_RGBA_GREEN];
            pLogPal->palPalEntry[i].peBlue  = pb[PL_RGBA_BLUE ];
            pLogPal->palPalEntry[i].peFlags = 0;
        }

        // create the palette and destroy LOGPAL
        hRet = ::CreatePalette(pLogPal);
        delete [] (char*)pLogPal;
    }
    else
    {
        // If the file itself did not contain a colormap, we will use
        // a default palette and rely on DrawDib's halftoning.
        // When displaying higher-color images on 8bpp hardware,
        // this is suboptimal though. Ideally we should build the
        // palette according to an histogram of the actual RGB values.
        // "SeeDib" from MS Source Code Samples does that nicely
        HDC dcScreen = ::GetWindowDC(NULL);
        hRet = ::CreateHalftonePalette(dcScreen);
        ::ReleaseDC (NULL, dcScreen);

    }
    return hRet;
}

// this is implicitly done the first time one calls GetLogPalette
// e.g. in DrawEx(); but one needs to call it when recycling a
// bitmap with a new contents (e.g. decoding a second file)
void PLWinBmpEx::BuildLogPalette(void)
{
    if (m_pal)
    {
        ::DeleteObject(m_pal);
        m_pal = 0;
    }

    m_pal = CreatePalette();
}


HPALETTE PLWinBmpEx::GetLogPalette(void)
{
    // build the palette the 1st time we're asked to provide it
    if ( ! m_pal )
        // the bitmap has hopefully been populated (by a decoder),
        // we can associate it with a logical palette
        m_pal = CreatePalette();

    return m_pal;
}

/*
/--------------------------------------------------------------------
|
|      $Log: WinBmpEx.cpp,v $
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
|      Revision 1.6  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.5  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/09/01 13:27:06  Administrator
|      Minor bugfixes
|
|      Revision 1.3  2000/07/07 14:00:26  Ulrich von Zadow
|      Minor bugfixes.
|
|      Revision 1.2  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delmée.
|
|
|
\--------------------------------------------------------------------
*/
