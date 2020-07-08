/*
/--------------------------------------------------------------------
|
|      $Id: plfilterresizehamming.cpp,v 1.1 2009/12/09 05:33:44 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterresizehamming.h"
#include "pl2passscale.h"
#include "plbitmap.h"


PLFilterResizeHamming::PLFilterResizeHamming (int NewXSize, int NewYSize, double NewRadius)
  : PLFilterResize (NewXSize, NewYSize),
    m_NewRadius(NewRadius)
{
}

void PLFilterResizeHamming::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  PLASSERT(pBmpSource->GetBitsPerPixel()==32);

  // Create a new Bitmap 
  pBmpDest->Create(m_NewXSize,
                   m_NewYSize,
                   pBmpSource->GetBitsPerPixel(),
                   pBmpSource->HasAlpha(),
                   pBmpSource->IsGreyscale(),
                   pBmpSource->GetResolution());
                     
  // Create a Filter Class from template
  PLHammingContribDef f(m_NewRadius);
  C2PassScale <CDataRGBA_UBYTE> sS(f);
  sS.Scale ((CDataRGBA_UBYTE::_RowType *) pBmpSource->GetLineArray(), 
             pBmpSource->GetWidth(), 
             pBmpSource->GetHeight(), 
             (CDataRGBA_UBYTE::_RowType *) pBmpDest->GetLineArray(),
             pBmpDest->GetWidth(),
             pBmpDest->GetHeight());
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterresizehamming.cpp,v $
|      Revision 1.1  2009/12/09 05:33:44  박종운
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
|      Revision 1.1  2007/04/06 09:31:01  황정영
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
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/10/16 17:12:27  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/09/30 16:57:26  uzadow
|      Improved speed of 2passfilter.h, code readability changes.
|
|      Revision 1.2  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.3  2000/01/16 20:43:16  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
|
|      Revision 1.1  1999/10/21 16:05:18  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.1  1999/10/19 21:29:45  Ulrich von Zadow
|      Added filters.
|
|
\--------------------------------------------------------------------
*/
