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
|      $Id: gritem.h,v 1.1 2009/12/09 05:35:03 박종운 Exp $
|      Graphic item class
|
|      Abstract base class for an item on a canvas. Saves position
|      and size information as well as opacity of the item. The
|      actual draw method is implemented in derived classes.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_GRITEM
#define INCL_GRITEM

class PLBmp;

class CGrItem : public CObject
{

DECLARE_DYNAMIC (CGrItem);

public:
  CGrItem
    ( int x,            // Position on the canvas
      int y,
      int w,            // Width
      int h,            // Height
      int z,            // Position in z-Order
      BYTE Opacity      // Opacity of the object. 255 is completely
                        // opaque, 0 is completely transparent.
    );
    // Create and initialize a GrItem.

  ~CGrItem
    ();

  virtual void Draw
    ( PLBmp * pCanvas,
      CRect * pUpdateRect
    ) = 0;
    // Responsible for drawing the object on the canvas. Implemented
    // in a derived class.


  void GetRect
    ( CRect * pRect
    );
    // Returns dimensions as a rectangle.

  // Member variables.

  int    m_x, m_y;     // Position on the canvas
  int    m_w, m_h;     // Width & Height
  int    m_z;          // Position in z-Order
  BYTE   m_Opacity;    // Opacity of the object. 255 is completely
                       // opaque, 0 is completely transparent.

protected:
  void clip
    ( PLBmp * pDestBmp,
      CRect * pUpdRect
    );
    // Clips pUpdRect to the current item and canvas.

};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: gritem.h,v $
|      Revision 1.1  2009/12/09 05:35:03  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:27  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:51  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:36  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:35:13  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.5  2002/02/24 13:00:57  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.4  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
--------------------------------------------------------------------
*/
