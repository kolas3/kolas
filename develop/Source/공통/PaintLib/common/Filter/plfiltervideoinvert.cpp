/*
/--------------------------------------------------------------------
|
|      $Id: plfiltervideoinvert.cpp,v 1.1 2009/12/09 05:33:45 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilter.h"
#include "plfiltervideoinvert.h"
#include "plbitmap.h"
#include "plhsvconvert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


PLFilterVideoInvert::PLFilterVideoInvert() : PLFilter()
{

}

PLFilterVideoInvert::~PLFilterVideoInvert()
{

}

void PLFilterVideoInvert::Apply(PLBmp *pBmpSource, PLBmp *pBmpDest) const
{
  // Only works for 32 bpp bitmaps at the moment.
  PLASSERT (pBmpSource->GetBitsPerPixel() == 32);

  pBmpDest->Create (pBmpSource->GetWidth(), pBmpSource->GetHeight(),
                    32, false, false, pBmpSource->GetResolution());
  PLBYTE ** pSrcLines = pBmpSource->GetLineArray();
  PLBYTE ** pDstLines = pBmpDest->GetLineArray();

  for (int y = 0; y<pBmpDest->GetHeight(); ++y)
  { // For each line
    PLBYTE * pSrcPixel = pSrcLines[y];
    PLBYTE * pDstPixel = pDstLines[y];

    for (int x = 0; x < pBmpDest->GetWidth(); ++x)
    { // For each pixel
      double v1, v2, v3;

      v1 = (double)pSrcPixel[PL_RGBA_RED];
      v2 = (double)pSrcPixel[PL_RGBA_GREEN];
      v3 = (double)pSrcPixel[PL_RGBA_BLUE];

      fp_rgb_to_hsv(&v1, &v2, &v3);
      v3 = 255.0-v3;
      fp_hsv_to_rgb(&v1, &v2, &v3);

      *pDstPixel = (int)v3;
    pDstPixel++;
      *pDstPixel = (int)v2;
    pDstPixel++;
      *pDstPixel = (int)v1;
    pDstPixel++;

    pDstPixel++;

    /*PLBYTE (pSrcPixel[PL_RGBA_RED]*0.212671+
                         pSrcPixel[PL_RGBA_GREEN]*0.715160+
                         pSrcPixel[PL_RGBA_BLUE]*0.072169);*/
      pSrcPixel += sizeof(PLPixel32);
    }
  }

}

/*
/--------------------------------------------------------------------
|
|      $Log: plfiltervideoinvert.cpp,v $
|      Revision 1.1  2009/12/09 05:33:45  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:11  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:54  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:50  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:18  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.5  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/16 17:12:27  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.4  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.3  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.2  2000/10/12 21:56:12  uzadow
|      Moved local functions from VideoInvertFilter.cpp to
|      hsvconvert.*
|
|      Revision 1.1  2000/03/31 12:20:06  Ulrich von Zadow
|      Video invert filter (beta)
|
|
|
\--------------------------------------------------------------------
*/
