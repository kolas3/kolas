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
|      $Id: WinBmpEx.h,v 1.1 2009/12/09 05:34:55 ¹ÚÁ¾¿î Exp $
|	     PaintLib windows DrawDib bitmap class implementation
|
|      See Paul DiLascia's excellent january and march ´97 MSJ
|      articles. This version optionally draws using the 'video for
|      windows' DrawDib API. This is supposedly faster, and offers
|      dithering, too.
|
|      Copyright (c) 1998-2000 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#ifndef INCL_DRAWDIB
#define INCL_DRAWDIB

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "pldibsect.h"

class PLWinBmpEx: public PLDIBSection
{
public:
    PLWinBmpEx();
    virtual ~PLWinBmpEx();

    //! Extended Draw function; can use DrawDib or not.
    //! Using any stretch-mode (but the default) forces GDI usage
    BOOL DrawEx(HDC dc, const RECT* rcDst=NULL, const RECT* rcSrc=NULL,
                HPALETTE hPal=NULL, bool bForeground=true,
		int StretchMode=-1, DWORD rop=SRCCOPY);
    
    //! Draws the bitmap with top left corner at specified location.
    virtual void Draw( HDC dc, int x, int y, DWORD rop = -1 );

    //! Draws the bitmap on the given device context.
    //! Scales the bitmap by Factor.
    virtual void StretchDraw (HDC hDC, int x, int y, 
	double Factor, DWORD rop = -1);

    //! Draws the bitmap on the given device context.
    //! Scales the bitmap so w is the width and h the height.
    virtual void StretchDraw (HDC hDC, int x, int y, 
	int w, int h, DWORD rop = -1);
    
    //! Draws a portion of the bitmap on the given device context
    virtual BOOL DrawExtract( HDC hDC, POINT pntDest, RECT rcSrc );

    //! Call this whenever recycling the bitmap
    void BuildLogPalette();

    //! Retrieve windows logical palette.
    HPALETTE GetLogPalette();

private:
    HPALETTE m_pal;    // logical palette

    HPALETTE CreatePalette();

// if we ever wanted to build without pulling in vfw 
#ifndef _WITHOUT_VFW_
    HDRAWDIB m_hdd;    // for DrawDib API
#endif
};

#endif // INCL_DRAWDIB

/*
/--------------------------------------------------------------------
|
|      $Log: WinBmpEx.h,v $
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
|      Revision 1.1  2007/04/06 09:31:32  È²Á¤¿µ
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
|      Revision 1.3  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delmée.
|
|
|
\--------------------------------------------------------------------
*/
