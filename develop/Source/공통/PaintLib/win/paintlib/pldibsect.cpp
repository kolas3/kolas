/*
/--------------------------------------------------------------------
|
|      $Id: pldibsect.cpp,v 1.1 2009/12/09 05:34:46 박종운 Exp $
|      Bitmap class using windows DIB sections.
|
|      This is a windows DIBSection wrapped in a CBmp-derived class.
|      It can be used just like a PLWinBmp can be used. In addition,
|      PLDIBSection can give access to the bitmap as a GDI bitmap handle.
|      This bitmap handle can be selected into a device context. All
|      normal GDI drawing functions can be used to write on the bitmap
|      in this way.
|
|      Internally, PLDIBSections are stored with header and bits in two
|      separate buffers.
|
|      Copyright (c) 1998 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "pldibsect.h"
#include "plexcept.h"

#include <stdio.h>


PLDIBSection::PLDIBSection
    ()
    : PLWinBmp(),
      m_hBitmap(NULL),
      m_bOwnsBitmap(true)
    // Creates an empty bitmap.
{
  // Delete everything the base class allocated.
  free(m_pBMI);
  m_pBMI = NULL;

  delete [] m_pLineArray;
  m_pLineArray = NULL;

  internalCreate(16, 16, 8, false, false);

  PLASSERT_VALID(this);
}


PLDIBSection::~PLDIBSection
    ()
{
  // Free the memory.
  freeMembers();
}


#ifdef _DEBUG
void PLDIBSection::AssertValid
    () const
{
  // call inherited PLASSERT_VALID first
  PLWinBmp::AssertValid();

  // Bitmap must exist
  PLASSERT (m_hBitmap);
}
#endif

void PLDIBSection::Detach
    ()
{
  delete [] m_pLineArray;

  internalCreate(16, 16, 8, false, false);

  PLASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////
// Windows-specific interface

void PLDIBSection::Draw
    ( HDC hDC,
      int x,
      int y,
      DWORD rop /* = SRCCOPY */
    )
    // Draw the DIB to a given DC.
{
  PLASSERT_VALID (this);

  HDC hSrcDC;
  hSrcDC = CreateCompatibleDC (hDC);
  HGDIOBJ hOldBmp = ::SelectObject (hSrcDC, m_hBitmap);
  if (m_pClrTab)
    ::SetDIBColorTable (hSrcDC, 0, 256, (RGBQUAD *)m_pClrTab);


  ::BitBlt (hDC,
            x, y,
            GetWidth(),               // Destination width
            GetHeight(),              // Destination height
            hSrcDC,
            0, 0,
            rop);

  ::SelectObject (hSrcDC, hOldBmp);
  ::DeleteDC(hSrcDC);
}


BOOL PLDIBSection::DrawExtract
    ( HDC hDC,
      POINT pntDest,
      RECT rcSrc
    )
    // Draw part of the DIB on a DC.
{
  PLASSERT_VALID (this);

  HDC hSrcDC;
  hSrcDC = CreateCompatibleDC (hDC);
  HGDIOBJ hOldBmp = ::SelectObject (hSrcDC, m_hBitmap);

  BOOL bOK = ::BitBlt (hDC,
                       pntDest.x,         // Destination x
                       pntDest.y,         // Destination y
                       rcSrc.right-rcSrc.left,    // Source width
                       rcSrc.bottom-rcSrc.top,   // Source height
                       hSrcDC,
                       rcSrc.left,        // Source x
                       rcSrc.top,
                       SRCCOPY);

  ::SelectObject (hSrcDC, hOldBmp);
  ::DeleteDC(hSrcDC);

  return bOK;
}

/////////////////////////////////////////////////////////////////////
// PLDIBSection member access

HBITMAP PLDIBSection::GetHandle
    ()
    // Returns a GDI handle to the bitmap. This handle can be selected
    // into a DC and used in normal GDI operations. It must be released
    // with ReleaseHandle() before the bitmap pointer can be used again
    // since GDI operations can be queued.
{
  return m_hBitmap;
}


/////////////////////////////////////////////////////////////////////
// Protected callbacks

void PLDIBSection::internalCreate
    ( LONG Width,
      LONG Height,
      WORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    )
    // Create a new empty DIBSection.
    // Assumes that no memory is allocated before the call.
{
#ifdef MAX_BITMAP_SIZE
	if (GetMemNeeded (Width, Height, BitsPerPixel)
      > MAX_BITMAP_SIZE)
	  throw PLTextException(PL_ERRDIB_TOO_LARGE, "Bitmap size too large.\n");
#endif

  int MemNeeded = sizeof (BITMAPINFOHEADER);
  if (BitsPerPixel < 16)
  { // Color table exists
    MemNeeded += (1 << BitsPerPixel)*sizeof (RGBQUAD);
  }

  m_pBMI = (BITMAPINFOHEADER*) malloc (MemNeeded);
  // out of memory?
  if (!m_pBMI)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory creating bitmap."));

  // Fill in the header info.
  m_pBMI->biSize = sizeof(BITMAPINFOHEADER);
  m_pBMI->biWidth = Width;
  m_pBMI->biHeight = Height;
  m_pBMI->biPlanes = 1;
  m_pBMI->biBitCount = BitsPerPixel;
  m_pBMI->biCompression = BI_RGB;   // No compression
  m_pBMI->biSizeImage = 0;
  m_pBMI->biXPelsPerMeter = 0;
  m_pBMI->biYPelsPerMeter = 0;
  m_pBMI->biClrUsed = 0;           // Always use the whole palette.
  m_pBMI->biClrImportant = 0;

  // Create DIB section in shared memory
  m_hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)m_pBMI,
                                DIB_RGB_COLORS, (void **)&m_pBits, 0, 0l);
  if (!m_hBitmap)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory creating bitmap."));

  // Set color table pointer & pointer to bits.
  initPointers ();

  initLocals (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);

  PLASSERT_VALID (this);
}


void PLDIBSection::internalCreate
    ( BITMAPINFOHEADER* pBMI
    )
    // Creates a PLDIBSection from an existing bitmap pointer.
    // Assumes that no memory is allocated before the call.
{
#ifdef MAX_BITMAP_SIZE
	if (GetMemNeeded (pBMI->biWidth, pBMI->biHeight, pBMI->biBitCount)
      > MAX_BITMAP_SIZE)
	  throw PLTextException(PL_ERRDIB_TOO_LARGE, "Bitmap size too large.\n");
#endif

  int MemNeeded = sizeof (BITMAPINFOHEADER);
  if (pBMI->biBitCount < 16)
  { // Color table exists
    MemNeeded += (1 << pBMI->biBitCount)*sizeof (RGBQUAD);
  }

  m_pBMI = (BITMAPINFOHEADER *) malloc (MemNeeded);

  wmemcpy (m_pBMI, pBMI, MemNeeded);

  // Create DIB section in shared memory
  m_hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)m_pBMI,
                                DIB_RGB_COLORS, (void **)&m_pBits, 0, 0l);
  if (!m_hBitmap)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory creating bitmap."));

  // Set color table pointer & pointer to bits.
  initPointers ();

  initLocals (m_pBMI->biWidth, m_pBMI->biHeight,
              m_pBMI->biBitCount, false, false);

  PLASSERT_VALID (this);
}


void PLDIBSection::freeMembers
    ()
{
  delete [] m_pLineArray;
  m_pLineArray = NULL;

  free(m_pBMI);
  DeleteObject (m_hBitmap);

  m_pBMI = NULL;
}


HANDLE PLDIBSection::createCopyHandle
    ()
    // Creates a copy of the current bitmap in a global memory block
    // and returns a handle to this block.
{
  HANDLE  hCopy;

  int MemUsed = GetMemUsed();

  hCopy = (HANDLE) ::GlobalAlloc (GMEM_MOVEABLE | GMEM_DDESHARE,
                                  MemUsed);
  if (hCopy == NULL)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory creating bitmap."));

  long * lpCopy = (long *) ::GlobalLock((HGLOBAL) hCopy);

  int BPP = GetBitsPerPixel();
  int HeaderMem = sizeof (BITMAPINFOHEADER);
  if (BPP < 16)
  { // Color table exists
    HeaderMem += (1 << BPP)*sizeof (RGBQUAD);
  }
  wmemcpy (lpCopy, m_pBMI, HeaderMem);
  int BitsMem = GetBitsMemNeeded (GetWidth(), GetHeight(), BPP);
  wmemcpy (((PLBYTE *)lpCopy)+HeaderMem, m_pBits, BitsMem);

  ::GlobalUnlock((HGLOBAL) hCopy);

  return hCopy;
}


void PLDIBSection::initPointers
    ()
    // Set color table pointer based on m_pBMI. In this derived class,
    // m_pBits is set in internalCreate().
{
  if (m_pBMI->biBitCount < 16)
  { // Color table exists
    m_pClrTab = (PLPixel32 *)(((BITMAPINFO *) (m_pBMI))->bmiColors);
  }
  else
  { // No color table for 16 bpp and up.
    m_pClrTab = NULL;
  }
}
/*
/--------------------------------------------------------------------
|
|      $Log: pldibsect.cpp,v $
|      Revision 1.1  2009/12/09 05:34:46  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:06  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:23  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:46  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.3  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.10  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.9  2001/01/21 14:28:22  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.8  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.7  2000/07/19 12:23:14  Ulrich von Zadow
|      Changed HANDLE to HBITMAP.
|
|      Revision 1.6  2000/07/11 17:14:08  Ulrich von Zadow
|      Corrected error message for out of memory errors.
|
|      Revision 1.5  2000/01/17 23:37:12  Ulrich von Zadow
|      Corrected bug in assignment operator.
|
|      Revision 1.4  2000/01/16 20:43:17  anonymous
|      Removed MFC dependencies
|
|
\--------------------------------------------------------------------
*/
