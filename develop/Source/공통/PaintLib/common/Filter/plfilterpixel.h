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
|      $Id: plfilterpixel.h,v 1.1 2009/12/09 05:33:42 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_FILTERPIXEL
#define INCL_FILTERPIXEL

#if _MSC_VER > 1000
#pragma once
#endif

#include "plfilter.h"

#include "plpixel32.h"
#include "plpixel24.h"
#include "plpixel8.h"

#include <typeinfo.h>

template <class PixelC, class PixelOp>
class PLFilterPixel : public PLFilter
{
public:
  PLFilterPixel(const PixelOp & Op);

  virtual ~PLFilterPixel();

  void ApplyInPlace(PLBmp * pBmp) const;

private:
  PixelOp m_Op;
};

template <class PixelC, class PixelOp>
PLFilterPixel<PixelC, PixelOp>::PLFilterPixel
    (const PixelOp & Op)
  : m_Op (Op)
{
}

template <class PixelC, class PixelOp>
PLFilterPixel<PixelC, PixelOp>::~PLFilterPixel ()
{
}

template <class PixelC, class PixelOp>
void PLFilterPixel<PixelC, PixelOp>::ApplyInPlace (PLBmp *pBmp) const
{
  PixelC** ppLines = (PixelC**)(pBmp->GetLineArray());

  switch (pBmp->GetBitsPerPixel())
  {
    case 32:
      PLASSERT (typeid(PixelC) == typeid (PLPixel32));
      break;
    case 24:
      PLASSERT (typeid(PixelC) == typeid (PLPixel24));
      break;
    case 8:
      PLASSERT (typeid(PixelC) == typeid (PLPixel8));
      break;
    default:
      PLASSERT (false);
  }

  for (int y=0; y<pBmp->GetHeight(); ++y)
  {
    PixelC* pLine = ppLines[y];
    for (int x=0; x<pBmp->GetWidth(); ++x)
      pLine[x] = m_Op.Apply(pLine[x]);
  }
}

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterpixel.h,v $
|      Revision 1.1  2009/12/09 05:33:42  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:52  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:49  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:01  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:17  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:22  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:26  황정영
|      2006.05.17
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/02/24 13:00:47  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2002/02/10 22:53:26  uzadow
|      Fixed cdoc problems.
|
|      Revision 1.1  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|
\--------------------------------------------------------------------
*/
