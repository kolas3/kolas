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
|      $Id: dibgrit.h,v 1.1 2009/12/09 05:35:02 박종운 Exp $
|      Bitmap Graphic item class
|
|      A bitmap on a canvas.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_DIBGRIT
#define INCL_DIBGRIT

#include "gritem.h"

class PLBmp;

class CDIBGrItem : public CGrItem
{

DECLARE_DYNAMIC (CDIBGrItem);

public:
  CDIBGrItem
    ( int x,            // Position on the canvas
      int y,
      int w,            // Width
      int h,            // Height
      int z,            // Position in z-Order
      BYTE Opacity,     // Opacity of the object. 255 is completely
                        // opaque, 0 is completely transparent.
      PLBmp * pBmp      // The bitmap.
    );

  ~CDIBGrItem
    ();

  virtual void Draw
    ( PLBmp * pCanvas,
      CRect * pUpdateRect
    );
    // Responsible for drawing the object on the canvas.

  PLBmp * GetBmp
    ()
  {
    return m_pBmp;
  }

private:
  void drawClippedNoScale
    ( PLBmp * pCanvas,
      CRect * pRect
    );
    // Draws the object. pRect must have been clipped already.
    // Assumes that no scaling is nessesary.

  void drawAlphaLine
    ( BYTE * pDest,
      BYTE * pSrc,
      CRect * pRect
    );

  void drawFadeLine
    ( BYTE * pDest,
      BYTE * pSrc,
      CRect * pRect
    );

  void drawClipped
    ( PLBmp * pCanvas,
      CRect * pRect
    );

  void drawClippedNoAlpha
    ( PLBmp * pCanvas,
      CRect * pRect
    );

  // Member variables.

  PLBmp * m_pBmp;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: dibgrit.h,v $
|      Revision 1.1  2009/12/09 05:35:02  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:14  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:26  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:51  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:34  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:54  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:02  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.6  2002/02/24 13:00:57  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.5  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/08/13 12:11:44  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.3  2000/01/16 20:43:19  anonymous
|      Removed MFC dependencies
|
|
\--------------------------------------------------------------------
*/
