/*
/--------------------------------------------------------------------
|
|      $Id: plfiltercontrast.cpp,v 1.1 2009/12/09 05:33:38 ¹ÚÁ¾¿î Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfiltercontrast.h"
#include "plbitmap.h"
#include "plpaintlibdefs.h"

PLFilterContrast::PLFilterContrast(double contrast, PLBYTE offset)
    : PLFilter(), 
    m_contrast(contrast), 
    m_offset((double) offset-125.0)
{
}

PLFilterContrast::~PLFilterContrast()
{

}

void PLFilterContrast::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  // Consider a coordinate system with two axes: The x axis represents
  // an input RGB component or intensity, the y axis an output RGB
  // component or intensity. The ranges of input and output values go 
  // from 0 - 255. 
  // If I take a straight line that intersects (0,0) through (255,255), 
  // I have defined a Null filter that leaves the image unchanged. 

  // Next step: Let's rotate this straight line around the point (125,125).
  // If I increase the slope, output values above (125,125) are increased 
  // by the slope factor, output values below (125, 125) are decreased 
  // likewise. This is how I create the contrast enhancement effect.

  // Now we are having a problem: Increasing the slope means that our
  // straight line intersects the line (x, 255), creating output values 
  // above 255 which are not defined in our RGB model. Since in the RGB world
  // there is nothing whiter than white, I'll set all these values to 255. A 
  // similar thing happens  on the lower left of our imaginary diagram: All
  // values smaller than 0 have to be set 0 - according to the fact that 
  // there is nothing darker than absolute darkness...

  // Ok, now what is that offset value good for? Remember the point 
  // (125,125). At this position, input and output values of our filter 
  // remain unchanged, independent of the slope of our straight line. Now 
  // imagine we are shifting our line parallel to the x axis. Our filter 
  // works quite differently now: We create a cut-off for the higher output 
  // value range (shift right), or the lower ones (shift left).

  int total = 0;
  register int inc;

  double contrast = m_contrast;
 
  PLASSERT (pBmpSource->GetBitsPerPixel() >= 24);

  pBmpDest->Create (pBmpSource->GetWidth(), 
                    pBmpSource->GetHeight(),
                    pBmpSource->GetBitsPerPixel(),
                    pBmpSource->HasAlpha(),
                    pBmpSource->IsGreyscale(),
                    pBmpSource->GetResolution());

  PLBYTE ** pSrcLines = pBmpSource->GetLineArray();
  PLBYTE ** pDstLines = pBmpDest->GetLineArray();

  register int destWidth = pBmpDest->GetWidth();

  if(pBmpSource->GetBitsPerPixel() == 24)
   inc = 3;

  if(pBmpSource->GetBitsPerPixel() == 32)
   inc = sizeof(PLPixel32);

  register double red, green, blue;
  register double csupp = contrast * (m_offset - 125.0) + 125.0;

  for (int y = 0; y < pBmpDest->GetHeight(); ++y)
  { // For each line
    register PLBYTE * pSrcPixel = pSrcLines[y];
    register PLBYTE * pDstPixel = pDstLines[y];

    for (register int x = 0; x < destWidth; ++x)
    {
       // Formel für Kontrastberechnung:
       // v = (contrast * (v - 125.0 + m_offset) + 125.0);
       red   = contrast * ((double) (pSrcPixel[PL_RGBA_RED])) + csupp;
       green = contrast * ((double) (pSrcPixel[PL_RGBA_GREEN])) + csupp;
       blue  = contrast * ((double) (pSrcPixel[PL_RGBA_BLUE])) + csupp;

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

       pSrcPixel += inc;
       pDstPixel += inc;
    }
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfiltercontrast.cpp,v $
|      Revision 1.1  2009/12/09 05:33:38  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:07  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:51  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:46  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:30:27  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:14  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:20  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:30:22  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 15:32:22  uzadow
|      Removed types LPBYTE, DWORD, UCHAR, VOID and INT from the code.
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
|      Revision 1.3  2001/01/12 23:30:01  uzadow
|      Doc update.
|
|      Revision 1.2  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.1  2000/11/06 23:20:22  uzadow
|      Added Contrast, Intensity and Lightness filters by
|      Thomas Hirschmann
|
|
\--------------------------------------------------------------------
*/
