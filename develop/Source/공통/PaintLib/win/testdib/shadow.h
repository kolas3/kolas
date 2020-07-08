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
|      $Id: shadow.h,v 1.1 2009/12/09 05:35:04 박종운 Exp $
|      Shadow Graphic item class
|
|      A shadowed region on a canvas. The item is basically a
|      partially transparent region. Transparency is defined by an
|      alpha DIB and a global transparency value. Most of the code
|      was just copied from CDIBGrItem & changed a bit.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_SHADOW
#define INCL_SHADOW

#include "plbitmap.h"
#include "gritem.h"

class CShadowItem : public CGrItem
{

DECLARE_DYNAMIC (CShadowItem);

public:
  CShadowItem
    ( int x,             // Position on the canvas
      int y,
      int w,             // Width
      int h,             // Height
      int z,             // Position in z-Order
      PLBYTE Opacity,      // Opacity of the object. 255 is completely
                         // opaque, 0 is completely transparent.
      PLPixel32 * pColor, // Shadow color.
      PLBmp * pAlphaBmp   // Alpha channel (must be 8 bpp). Can be NULL.
    );

  ~CShadowItem
    ();

  virtual void Draw
    ( PLBmp * pCanvas,
      CRect * pUpdateRect
    );
    // Responsible for drawing the object on the canvas.

  // Member variables.

  PLPixel32  m_Color;        // Shadow color
  PLBmp    * m_pAlphaBmp;
  PLBYTE   ** m_pLineArray;

private:

  void drawClippedNoScale
    ( PLBmp * pCanvas,
      CRect * pRect
    );
    // Draws the object. pRect must have been clipped already.
    // Assumes that no scaling is nessesary.

  void drawAlphaLine
    ( PLBYTE * pDest,
      PLBYTE * pAlpha,
      CRect * pRect
    );

  void drawFadeLine
    ( PLBYTE * pDest,
      CRect * pRect
    );
    // Draws one line. No scaling. Assumes alpha channel doesn't
    // exist.

  void drawClipped
    ( PLBmp * pCanvas,
      CRect * pRect
    );
    // Draws the object. pRect must have been clipped already.

  void drawScaleLine
    ( PLBYTE * pDest,
      PLBYTE * pAlpha,
      CRect * pRect
    );

};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: shadow.h,v $
|      Revision 1.1  2009/12/09 05:35:04  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:28  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:32:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:35:13  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:16  황정영
|      2006.05.17
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
--------------------------------------------------------------------
*/
