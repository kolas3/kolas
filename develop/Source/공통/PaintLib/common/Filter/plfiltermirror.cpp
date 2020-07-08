/*
/--------------------------------------------------------------------
|
|      $Id: plfiltermirror.cpp,v 1.1 2009/12/09 05:33:41 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|      Original code by Richard Hollis
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plbitmap.h"
#include "plfiltermirror.h"
#include "plpoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PLFilterMirror::PLFilterMirror() : PLFilter()
{}


PLFilterMirror::~PLFilterMirror()
{}


void PLFilterMirror::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  PLASSERT (pBmpSource->GetBitsPerPixel() == 32);

  int nWidth = pBmpSource->GetWidth();
  int nHeight= pBmpSource->GetHeight();

  int x,y;

  //ROTATE the bitmap
  pBmpDest->Create (nWidth, nHeight, pBmpSource->GetBitsPerPixel(), 
                    pBmpSource->HasAlpha(), pBmpSource->IsGreyscale(), 
                    pBmpSource->GetResolution());
  PLPixel32 pix;

  for (y = 0; y<nHeight; y++)
  {
    for (x = 0; x<nWidth; x++)
    {
      pix = pBmpSource->GetPixel(x,y);
      pBmpDest->SetPixel((nWidth- x)-1,y,pix);
    }
  }

  PLPixel32 * pPalette = pBmpSource->GetPalette();
  if ((pBmpSource->GetBitsPerPixel()<16) && (pPalette))
    pBmpDest->SetPalette(pPalette);
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfiltermirror.cpp,v $
|      Revision 1.1  2009/12/09 05:33:41  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:09  박대우
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
|      Revision 1.2  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.2  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.1  2001/01/13 20:06:16  uzadow
|      Added Flip and Mirror filters.
|
|
|
\--------------------------------------------------------------------
*/
