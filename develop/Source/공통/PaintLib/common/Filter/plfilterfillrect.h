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
|      $Id: plfilterfillrect.h,v 1.1 2009/12/09 05:33:39 박종운 Exp $
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERFILLRECT)
#define INCL_PLFILTERFILLRECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plfilter.h"
#include "../plpoint.h"

class PLBmp;

// Fills a rectangle in a Bitmap with a color
template<class PixelC> class PLFilterFillRect: public PLFilter
{
public:
    PLFilterFillRect (PLPoint min, PLPoint max, const PixelC& Color);
    virtual ~PLFilterFillRect();
    virtual void ApplyInPlace(PLBmp *pBmp) const;

private:
    PixelC m_Color;
    PLPoint m_min;
    PLPoint m_max;
};

template<class PixelC>
PLFilterFillRect<PixelC>::PLFilterFillRect
    (PLPoint min, PLPoint max, const PixelC& Color)
{
  m_min = min;
  m_max = max;
  m_Color = Color;
}

template<class PixelC>
PLFilterFillRect<PixelC>::~PLFilterFillRect ()
{

}

template<class PixelC>
void PLFilterFillRect<PixelC>::ApplyInPlace (PLBmp *pBmp) const
{
  PixelC** ppLines = (PixelC**)(pBmp->GetLineArray());
  for (int y=m_min.y; y<m_max.y; ++y)
  {
    PixelC* pLine = ppLines[y];
    for (int x=m_min.x; x<m_max.x; ++x)
      pLine[x] = m_Color;
  }
}

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterfillrect.h,v $
|      Revision 1.1  2009/12/09 05:33:39  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:51  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:47  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:00  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:16  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:20  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:22  황정영
|      2006.05.17
|
|      Revision 1.4  2002/02/10 22:53:26  uzadow
|      Fixed cdoc problems.
|
|      Revision 1.3  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2001/09/13 20:48:42  uzadow
|      Added fill filters.
|
|      Revision 1.1  2001/09/13 10:39:31  uzadow
|      Added FilterFillRect
|
|
\--------------------------------------------------------------------
*/
