/*
/--------------------------------------------------------------------
|
|      $Id: plfilterlightness.cpp,v 1.1 2009/12/09 05:33:41 ╧за╬©Н Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterlightness.h"
#include "plbitmap.h"

PLFilterLightness::PLFilterLightness(int lightness)
: PLFilter(),
  m_lightness(lightness)
{
}

PLFilterLightness::~PLFilterLightness()
{

}

void PLFilterLightness::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  int total = 0;

  double lightness = 2.55 * (double) (m_lightness);

  PLASSERT (pBmpSource->GetBitsPerPixel() == 32);

  pBmpDest->Create (pBmpSource->GetWidth(),
                    pBmpSource->GetHeight(),
                    pBmpSource->GetBitsPerPixel(),
                    pBmpSource->HasAlpha(),
                    pBmpSource->IsGreyscale(),
                    pBmpSource->GetResolution());

  PLBYTE ** pSrcLines = pBmpSource->GetLineArray();
  PLBYTE ** pDstLines = pBmpDest->GetLineArray();

  int destWidth = pBmpDest->GetWidth();

  double red, green, blue;

  for (int y = 0; y < pBmpDest->GetHeight(); ++y)
  { // For each line
    PLBYTE * pSrcPixel = pSrcLines[y];
    PLBYTE * pDstPixel = pDstLines[y];

    for (int x = 0; x < destWidth; ++x)
    {
       red   = (double) pSrcPixel[PL_RGBA_RED] + lightness;
       green = (double) pSrcPixel[PL_RGBA_GREEN] + lightness;
       blue  = (double) pSrcPixel[PL_RGBA_BLUE] + lightness;

       if(red >= 255.0)
        pDstPixel[PL_RGBA_RED] = (PLBYTE) 255;
       else if (red < 0.0)
        pDstPixel[PL_RGBA_RED] = (PLBYTE) 0;
       else
        pDstPixel[PL_RGBA_RED] = (PLBYTE) red;

       if(green >= 255.0)
        pDstPixel[PL_RGBA_GREEN] = (PLBYTE) 255;
       else if (green < 0.0)
        pDstPixel[PL_RGBA_GREEN] = (PLBYTE) 0;
       else
        pDstPixel[PL_RGBA_GREEN] = (PLBYTE) green;

       if(blue >= 255.0)
        pDstPixel[PL_RGBA_BLUE] = (PLBYTE) 255;
       else if (blue < 0.0)
        pDstPixel[PL_RGBA_BLUE] = (PLBYTE) 0;
       else
        pDstPixel[PL_RGBA_BLUE] = (PLBYTE) blue;

       pSrcPixel += 4;
       pDstPixel += 4;
    }
  }
}
