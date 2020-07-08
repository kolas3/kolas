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
|      $Id: plpoint.h,v 1.1 2009/12/09 05:33:55 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPOINT
#define INCL_PLPOINT

//! Simple point class for 2d coordinate manipulations. This class
//! contains only inline functions and nothing virtual. Its member variables
//! are public.
class PLPoint
{
public:
  int x;
  int y;

  //!
  PLPoint
  ();

  //!
  PLPoint
  ( int X,
    int Y
  );

  //!
  bool operator ==
  ( const PLPoint & pt
  ) const;

  //!
  bool operator !=
  ( const PLPoint & pt
  ) const;

  //!
  void operator +=
  ( const PLPoint & pt
  );

  //!
  void operator -=
  ( const PLPoint & pt
  );

  //!
  PLPoint operator -
  () const;

  //!
  PLPoint operator +
  ( const PLPoint & pt
  ) const;

  //!
  PLPoint operator -
  ( const PLPoint & pt
  ) const;

  //!
  PLPoint operator /
  ( double f
  ) const;
};

inline PLPoint::PLPoint
()
{}


inline PLPoint::PLPoint
( int X,
  int Y
)
{
  x = X;
  y = Y;
}

inline bool PLPoint::operator ==
( const PLPoint & pt
) const
{
  return (x == pt.x && y == pt.y);
}

inline bool PLPoint::operator !=
( const PLPoint & pt
) const
{
  return (x != pt.x || y != pt.y);
}

inline void PLPoint::operator +=
( const PLPoint & pt
)
{
  x += pt.x;
  y += pt.y;
}

inline void PLPoint::operator -=
( const PLPoint & pt
)
{
  x -= pt.x;
  y -= pt.y;
}

inline PLPoint PLPoint::operator -
() const
{
  return PLPoint(-x, -y);
}

inline PLPoint PLPoint::operator +
( const PLPoint & pt
) const
{
  return PLPoint(x + pt.x, y + pt.y);
}

inline PLPoint PLPoint::operator -
( const PLPoint & pt
) const
{
  return PLPoint(x - pt.x, y - pt.y);
}

inline PLPoint PLPoint::operator /
  ( double f
  ) const
{
  return PLPoint (int(x/f), int(y/f));
}

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plpoint.h,v $
|      Revision 1.1  2009/12/09 05:33:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:03  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.8  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.7  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.6  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.5  2001/09/24 14:18:42  uzadow
|      Added operator -, improved const-correctness.
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
|      Added PLPoint.
|
|
\--------------------------------------------------------------------
*/
