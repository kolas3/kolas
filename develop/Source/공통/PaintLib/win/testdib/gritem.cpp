/*
/--------------------------------------------------------------------
|
|      $Id: gritem.cpp,v 1.1 2009/12/09 05:35:03 박종운 Exp $
|      Graphic item class
|
|      Abstract base class for an item on a canvas.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "gritem.h"
#include "plbitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CGrItem, CObject);


CGrItem::CGrItem
    ( int x,            // Position on the canvas
      int y,
      int w,            // Width
      int h,            // Height
      int z,            // Position in z-Order
      BYTE Opacity      // Opacity of the object. 255 is completely
                        // opaque, 0 is completely transparent.
    )
{
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
  m_z = z;
  m_Opacity = Opacity;
}


CGrItem::~CGrItem
    ()
{
}


void CGrItem::GetRect
    ( CRect * pRect
    )
    // Returns dimensions as a rectangle.
{
  *pRect = CRect (CPoint (m_x, m_y), CSize (m_w, m_h));
}


void CGrItem::clip
    ( PLBmp * pDestBmp,
      CRect * pUpdRect
    )
    // Clips pUpdRect to the item and the canvas.
{
  CRect TempRect;
  CRect ItemRect (CPoint (m_x, m_y), CSize (m_w, m_h));
  CRect CanvasRect (CPoint (0,0),
                    CSize (pDestBmp->GetWidth()-1,
                           pDestBmp->GetHeight()-1));

  TempRect.IntersectRect (pUpdRect, &ItemRect);

  pUpdRect->IntersectRect (&TempRect, &CanvasRect);
}
/*
/--------------------------------------------------------------------
|
|      $Log: gritem.cpp,v $
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
|      Revision 1.3  2000/01/16 20:43:19  anonymous
|      Removed MFC dependencies
|
|
\--------------------------------------------------------------------
*/
