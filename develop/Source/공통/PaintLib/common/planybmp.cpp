/*
/--------------------------------------------------------------------
|
|      $Id: planybmp.cpp,v 1.1 2009/12/09 05:33:47 박종운 Exp $
|      Device independent bitmap class
|
|        Plattform-independent version
|
|        Manipulates uncompressed device independent bitmaps.
|
|        Supported data formats are 8, 24 (partial) and 32 bpp. The 
|        data is stored sequentially without padding in the bitmap.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "planybmp.h"
#include "plexcept.h"


PLAnyBmp::PLAnyBmp
  () : m_pBits(NULL)
    // Creates an empty bitmap.
{
  internalCreate(16, 16, 8, false, false);

  PLASSERT_VALID(this);
}


PLAnyBmp::~PLAnyBmp
    ()
{
  // Free the memory.
  freeMembers();
}


long PLAnyBmp::GetMemUsed
    ()
    // Returns the memory used by the object.
{
  PLASSERT_VALID (this);

  return GetMemNeeded (GetWidth(), GetHeight(), GetBitsPerPixel())+
                                                sizeof (*this);
}


long PLAnyBmp::GetBytesPerLine
    ()
    // Returns number of bytes used per line.
{
  // bdelmee code change
  int nBytes = GetWidth() * GetBitsPerPixel() / 8;
  if (GetBitsPerPixel() == 1 && GetWidth() % 8)
    ++nBytes;
  return nBytes;
}


/////////////////////////////////////////////////////////////////////
// Static functions

long PLAnyBmp::GetBitsMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    )
    // Returns memory needed by bitmap bits.
{
  // Calculate memory per line.
  int LineMem = width*BitsPerPixel/8;

  // bdelmee code change
  if (BitsPerPixel == 1 && width % 8)
    ++LineMem;

  // Multiply by number of lines
  return LineMem*height;
}


long PLAnyBmp::GetMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    )
    // Returns memory needed by a bitmap with the specified attributes.
{
  int HeaderMem = sizeof (PLAnyBmp);
  if (BitsPerPixel < 16)
  { // Palette memory
    HeaderMem += (1 << BitsPerPixel)*sizeof (PLPixel32);
  }

  return HeaderMem+GetBitsMemNeeded (width, height, BitsPerPixel);
}


/////////////////////////////////////////////////////////////////////
// Local functions


void PLAnyBmp::internalCreate
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    )
    // Create a new empty bitmap. Bits are uninitialized.
    // Assumes that no memory is allocated before the call.
{
  // Allocate memory

#ifdef MAX_BITMAP_SIZE
  int MemNeeded = GetMemNeeded (Width, Height, BitsPerPixel);

  if (MemNeeded > MAX_BITMAP_SIZE)
    throw PLTextException(PL_ERRDIB_TOO_LARGE, "Bitmap size too large.\n");
#endif

  m_pBits = new PLBYTE [GetBitsMemNeeded (Width, Height, BitsPerPixel)];

  if (BitsPerPixel <= 8)
    m_pClrTab = new PLPixel32 [1 << BitsPerPixel];
   else
    m_pClrTab = NULL;
  initLocals (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);

  PLASSERT_VALID (this);
}


void PLAnyBmp::initLineArray
    ()
{
  m_pLineArray = new PLBYTE * [m_Size.y];
  int LineLen = GetBytesPerLine();

  for (int y=0; y<m_Size.y; y++)
    m_pLineArray[y] = m_pBits + y*LineLen;
}

void PLAnyBmp::freeMembers
    ()
{
  delete [] m_pBits;
  m_pBits = NULL;

  if (m_pClrTab)
  {
    delete [] m_pClrTab;
    m_pClrTab = NULL;
  }

  delete [] m_pLineArray;
  m_pLineArray = NULL;
}

/*
/--------------------------------------------------------------------
|
|      $Log: planybmp.cpp,v $
|      Revision 1.1  2009/12/09 05:33:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:12  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:55  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:19  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/02/24 13:00:12  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.10  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.9  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.8  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.7  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.6  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
