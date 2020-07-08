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
|      $Id: plrect.h,v 1.1 2009/12/09 05:33:57 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLRECT
#define INCL_PLRECT

#include "plpoint.h"

//!
class PLRect
{
public:
  PLPoint tl;
  PLPoint br;

  //!
  PLRect
  ();

  //!
  PLRect
  ( int left,
    int top,
    int right,
    int bottom
  );

  //!
  PLRect
  ( const PLPoint& TL,
    const PLPoint& BR
  );

  //!
  bool operator ==
  ( const PLRect & rect
  ) const;

  //!
  bool operator !=
  ( const PLRect & rect
  ) const;

  //!
  int Width 
  () const;

  //!
  int Height
  () const;

};

inline PLRect::PLRect
()
{}

inline PLRect::PLRect
  ( const PLPoint& TL,
    const PLPoint& BR
  ): tl(TL), br(BR)
{}

inline PLRect::PLRect
  ( int left,
    int top,
    int right,
    int bottom
  ) : tl(left, top), 
      br (right, bottom)
{}

inline bool PLRect::operator ==
( const PLRect & rect
) const
{
  return (tl == rect.tl && br == rect.br);
}

inline bool PLRect::operator !=
( const PLRect & rect
) const
{
  return !(rect==*this);
}

inline int PLRect::Width 
() const
{
  return br.x-tl.x;
}

inline int PLRect::Height
() const
{
  return br.y-tl.y;
}

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plrect.h,v $
|      Revision 1.1  2009/12/09 05:33:57  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:05  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:26  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:44  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.3  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/24 14:24:52  uzadow
|      Added PLRect.
|
|      Revision 1.4  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/11/21 20:20:36  uzadow
|      Changed bool to bool.
|
|      Revision 1.2  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.1  1999/12/09 16:35:58  Ulrich von Zadow
|      Added PLRect.
|
|
\--------------------------------------------------------------------
*/
