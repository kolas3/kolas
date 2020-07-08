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
|      $Id: plpixel8.h,v 1.1 2009/12/09 05:33:55 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPIXEL8
#define INCL_PLPIXEL8

#include "plpixeldefs.h"

//! 8 bit pixel class. A pixel in this class contains 8 bits of
//! image data. This class is meant to be
//! fast, so all methods are inlined.
class PLPixel8
{
  public:
    //!
    PLPixel8 ();
    //!
    PLPixel8 (PLBYTE val);
    //!
    void Set (PLBYTE val);
    //!
    PLBYTE Get () const;

    //!
    bool operator ==(const PLPixel8&);

    //!
    bool operator !=(const PLPixel8&);

    //! Simple and fast 'distance' between two pixels.
    int BoxDist (PLPixel8 Pix);

  private:
    PLBYTE m_Data;
};

inline PLPixel8::PLPixel8()
{
}


inline PLPixel8::PLPixel8(PLBYTE val)
{
  Set (val);
}


inline void PLPixel8::Set(PLBYTE val)
{
  m_Data = val;
}


inline PLBYTE PLPixel8::Get() const
{
  return m_Data;
}


inline int PLPixel8::BoxDist (PLPixel8 Pix)
{
  return abs ((int)Get()-Pix.Get());
}

inline bool PLPixel8::operator ==(const PLPixel8& Pix)
{
  return (Get() == Pix.Get());
}

inline bool PLPixel8::operator !=(const PLPixel8& Pix)
{
  return (!(*this == Pix));
}


#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plpixel8.h,v $
|      Revision 1.1  2009/12/09 05:33:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:02  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:05  황정영
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
|      Revision 1.3  2002/02/24 13:00:37  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2001/09/13 20:45:35  uzadow
|      Added 8-bpp pixel class.
|
|
\--------------------------------------------------------------------
*/
