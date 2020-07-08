/*
/--------------------------------------------------------------------
|
|      $Id: plwinbmp.cpp,v 1.1 2009/12/09 05:34:48 박종운 Exp $
|      Device independent bitmap class
|
|        Windows version
|
|        Manipulates uncompressed device independent bitmaps
|        of all color depths.
|
|        Header and bits are in one buffer.
|        The bottom line is stored first. Color tables for 16-,
|        24-, and 32- bpp-bitmaps are not supported. biClrUsed is
|        always 0.
|
|        A PLWinBmp can contain alpha channel information. As in TGA
|        files, 0 is completely transparent and 255 completely opaque.
|
|      Copyright (c) 1996-1998 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plwinbmp.h"
#include "plexcept.h"

#include <stdio.h>


PLWinBmp::PLWinBmp ()
  : PLBmp(),
    m_pBMI (NULL),
    m_pBits (NULL)
// Creates an empty bitmap.
{
  internalCreate(16, 16, 8, false, false);

  PLASSERT_VALID(this);
}


PLWinBmp::~PLWinBmp ()
{
  // Free the memory.
  freeMembers();
}


#ifdef _DEBUG
void PLWinBmp::AssertValid () const
{
  // call inherited PLASSERT_VALID first
  PLBmp::AssertValid();

  // Bitmap must exist
  PLASSERT (m_pBMI);

  // Bitmapinfo must equal member variables
  PLASSERT (m_pBMI->biHeight == m_Size.y);
  PLASSERT (m_pBMI->biWidth == m_Size.x);
  PLASSERT (m_pBMI->biBitCount == m_bpp);

  // Only uncompressed bitmaps allowed.
  PLASSERT (m_pBMI->biCompression == BI_RGB);

  // No optimized color tables allowed.
  PLASSERT (m_pBMI->biClrUsed == 0 ||
          m_pBMI->biClrUsed == (DWORD)(1 << m_bpp));

  // Bits are always accessible
  PLASSERT (m_LockCount >= 1);
}
#endif


/////////////////////////////////////////////////////////////////////
// PLWinBmp manipulation

void PLWinBmp::AlphaBlt (PLWinBmp * pSrPLBmp, int x, int y)
// Do a bitblt using the alpha channel of pSrPLBmp.
// Legacy routine. Should not be used.
{
  PLASSERT_VALID (this);
  PLASSERT (GetBitsPerPixel() == 32);

  PLASSERT_VALID (pSrPLBmp);

  // Overlay picture
  int DestLineLen = GetWidth()*4;
  int SrcLineLen = pSrPLBmp->GetBytesPerLine();
  PLPixel32 * pPal = pSrPLBmp->GetPalette();

  // Perform clipping.
  int maxy = min (pSrPLBmp->GetHeight(),
                  GetHeight()-y);
  int maxx = min (pSrPLBmp->GetWidth(),
                  GetWidth()-x);
  int miny = max (0,-y);
  int minx = max (0,-x);

  if (pSrPLBmp->m_bAlphaChannel)
  {
    int alpha, negalpha;

    for (int sy = miny; sy<maxy; sy++)
    { // For each line
      BYTE * pDest = m_pBits+DestLineLen*(y+sy)+x*4;
      BYTE * pSrc = pSrPLBmp->m_pBits+SrcLineLen*sy;

      for (int sx = minx; sx<maxx; sx++)
      { // For each pixel
        if (pPal)
        {
          BYTE * pPixel = (BYTE *)&(pPal[*pSrc]);
          alpha = pPixel[3];
          negalpha = 255-alpha;
          pDest[0] = (pDest[0]*negalpha+pPixel[0]*alpha)>>8;
          pDest[1] = (pDest[1]*negalpha+pPixel[1]*alpha)>>8;
          pDest[2] = (pDest[2]*negalpha+pPixel[2]*alpha)>>8;

          pSrc++;
        }
        else
        {
          alpha = pSrc[3];
          negalpha = 255-alpha;
          pDest[0] = (pDest[0]*negalpha+pSrc[0]*alpha)>>8;
          pDest[1] = (pDest[1]*negalpha+pSrc[1]*alpha)>>8;
          pDest[2] = (pDest[2]*negalpha+pSrc[2]*alpha)>>8;
          pSrc += 4;
        }

        pDest += 4;
      }
    }
  }
  else
  {
    for (int sy = miny; sy<maxy; sy++)
    { // For each line
      if (pPal)
      {
        BYTE * pDest = m_pBits+DestLineLen*(y+sy)+x*4;
        BYTE * pSrc = pSrPLBmp->m_pBits+SrcLineLen*sy;

        for (int sx = minx; sx<maxx; sx++)
        { // For each pixel
          *((PLPixel32 *)pDest) = pPal[*pSrc];

          pDest += 4;
          pSrc++;
        }
      }
      else
      {
        BYTE * pDest = m_pBits+DestLineLen*(y+sy)+x*4;
        BYTE * pSrc = pSrPLBmp->m_pBits+SrcLineLen*sy;

        wmemcpy (pDest, pSrc, 4*(maxx-minx));
      }
    }
  }
  PLASSERT_VALID (this);
}

/////////////////////////////////////////////////////////////////////
// PLWinBmp information

long PLWinBmp::GetMemUsed ()
// Returns the memory used by the object.
{
  PLASSERT_VALID (this);

  return GetMemNeeded (GetWidth(), GetHeight(), m_pBMI->biBitCount)+
         sizeof (*this);
}


long PLWinBmp::GetBytesPerLine ()
// Returns number of bytes used per line.
{
  // bdelmee code change
  int nBytes = m_Size.x*m_bpp / 8;
  if (m_bpp == 1 && m_Size.x % 8)
    ++nBytes;
  // adjust to nearest DWORD-multiple
  return (nBytes + 3) & ~3;
}


/////////////////////////////////////////////////////////////////////
// Windows-specific interface


void PLWinBmp::CreateRes ( HINSTANCE lh_ResInst, int ID)
    // Loads a DIB from a resource. Fails if the bitmap is compressed.
{
  HRSRC  hRsrc;
  HGLOBAL hGlobal;
  BITMAPINFOHEADER * pBMI;

  PLASSERT_VALID (this);

  hRsrc = FindResource(lh_ResInst,
                       MAKEINTRESOURCE (ID),
                       RT_BITMAP);

  PLASSERT (hRsrc);  // Make sure resource exists.

  hGlobal = LoadResource(lh_ResInst, hRsrc);
  pBMI = (BITMAPINFOHEADER *) LockResource(hGlobal);

  // Delete any existing stuff.
  freeMembers ();

  // Copy data into local memory & init locals.
  internalCreate (pBMI);

  PLASSERT_VALID (this);
}


void PLWinBmp::CreateFromHBitmap(HBITMAP hBitMap)
{
  HDC hdc = ::GetDC (NULL);
  BITMAPINFO * pBMI;
  pBMI = (BITMAPINFO *)malloc (sizeof (BITMAPINFOHEADER)+256*4);
  BITMAPINFOHEADER * pBIH = (BITMAPINFOHEADER *)pBMI;
  pBIH->biSize = sizeof (BITMAPINFOHEADER);
  pBIH->biBitCount = 0;
  pBIH->biPlanes = 1;
  pBIH->biSizeImage = 0;
  pBIH->biXPelsPerMeter = 0;
  pBIH->biYPelsPerMeter = 0;
  pBIH->biClrUsed = 0;           // Always use the whole palette.
  pBIH->biClrImportant = 0;

  // Get bitmap format.
  int rc = ::GetDIBits(hdc, hBitMap, 0, 0, NULL, pBMI, DIB_RGB_COLORS);
  PLASSERT (rc);

  // Convert to format paintlib can use.
  if (pBIH->biBitCount > 8)
    pBIH->biBitCount = 32;
  else if (pBIH->biBitCount < 8 && pBIH->biBitCount != 1)
    pBIH->biBitCount = 8;

  pBIH->biCompression = BI_RGB;   // No compression

  freeMembers ();

  internalCreate(pBIH->biWidth, pBIH->biHeight, pBIH->biBitCount, false, false);

  rc = ::GetDIBits (hdc, hBitMap, 0, GetHeight(), GetBits(),
                    (BITMAPINFO *)GetBMI(), DIB_RGB_COLORS);
  PLASSERT (rc);
  free (pBMI);
  ::ReleaseDC(NULL, hdc);
  PLASSERT_VALID (this);
}

void PLWinBmp::CreateFromHDIBBitmap (BITMAPINFOHEADER* pBIH, HPALETTE hPal)
// Creates a PLWinBmp from an existing bitmap pointer.
// Assumes that no memory is allocated before the call.
{
	freeMembers();

	internalCreate(pBIH);

	if (hPal && GetBitsPerPixel() <= 8)
	{
		PALETTEENTRY pe;
		WORD wEntries;
		if(GetObject(hPal, sizeof(WORD), &wEntries))
		{
			if(wEntries)
			{
				int rc;
				for(int n = 0 ; n < wEntries; n++)
				{
					rc = GetPaletteEntries(hPal, n, 1, &pe);
					PLASSERT(rc);
					SetPaletteEntry(n, pe.peRed, pe.peGreen, pe.peBlue, 0);
				}
			}

//			TracePalette();
		}
	}

  PLASSERT_VALID (this);
}

SIZE PLWinBmp::GetSize ()
    // Returns size in pixels
{
  SIZE sz;

  PLASSERT_VALID (this);

  sz.cx = GetWidth();
  sz.cy = GetHeight();

  return sz;
}


BITMAPINFOHEADER * PLWinBmp::GetBMI ()
{
  PLASSERT_VALID (this);

  return m_pBMI;
}


/////////////////////////////////////////////////////////////////////
// PLWinBmp output

void PLWinBmp::Draw (HDC hDC, int x, int y, DWORD rop /* = SRCCOPY */)
// Draw the DIB to a given DC.
{
  PLASSERT_VALID (this);

  ::StretchDIBits(hDC,
                  x,                        // Destination x
                  y,                        // Destination y
                  GetWidth(),               // Destination width
                  GetHeight(),              // Destination height
                  0,                        // Source x
                  0,                        // Source y
                  GetWidth(),               // Source width
                  GetHeight(),              // Source height
                  m_pBits,                  // Pointer to bits
                  (BITMAPINFO *) m_pBMI,    // BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  rop);                     // Raster operator code
}


void PLWinBmp::StretchDraw (HDC hDC, int x, int y, double Factor,
                           DWORD rop /* = SRCCOPY */)
// Draw the DIB to a given DC.
{
  PLASSERT_VALID (this);

  ::StretchDIBits(hDC,
                  x,                        // Destination x
                  y,                        // Destination y
                  int (Factor*GetWidth()),  // Destination width
                  int (Factor*GetHeight()), // Destination height
                  0,                        // Source x
                  0,                        // Source y
                  GetWidth(),               // Destination width
                  GetHeight(),              // Destination height
                  m_pBits,                  // Pointer to bits
                  (BITMAPINFO *) m_pBMI,    // BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  rop);                     // Raster operator code
}


void PLWinBmp::StretchDraw (HDC hDC, int x, int y, int w, int h,
                           DWORD rop /* = SRCCOPY */)
// Draw the DIB to a given DC.
{
  PLASSERT_VALID (this);

  ::StretchDIBits(hDC,
                  x,                        // Destination x
                  y,                        // Destination y
                  w,            // Destination width
                  h,            // Destination height
                  0,                        // Source x
                  0,                        // Source y
                  GetWidth(),               // Destination width
                  GetHeight(),              // Destination height
                  m_pBits,                  // Pointer to bits
                  (BITMAPINFO *) m_pBMI,    // BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  rop);                     // Raster operator code
}

BOOL PLWinBmp::DrawExtract (HDC hDC, POINT pntDest, RECT rcSrc)
// Draw part of the DIB on a DC.
{
  PLASSERT_VALID (this);
  return ::SetDIBitsToDevice (hDC,
                              pntDest.x,         // Destination x
                              pntDest.y,         // Destination y
                              rcSrc.right-rcSrc.left,    // Source width
                              rcSrc.bottom-rcSrc.top,   // Source height
                              rcSrc.left,        // Source x
                              m_pBMI->biHeight - rcSrc.bottom,
                              // Source lower y
                              0,
                              m_pBMI->biHeight,
                              m_pBits,
                              (BITMAPINFO *) m_pBMI,
                              DIB_RGB_COLORS );
}


void PLWinBmp::ToClipboard ()
// Puts a copy of the DIB in the clipboard
{
  PLASSERT_VALID (this);

  if (::OpenClipboard(NULL))
  {
    EmptyClipboard();
    HANDLE hDIB = createCopyHandle ();
    SetClipboardData (CF_DIB, hDIB);
    CloseClipboard();
  }
}


bool PLWinBmp::FromClipboard(UINT uFormat)
{
  if (IsClipboardFormatAvailable (uFormat))
  {
    if (OpenClipboard(NULL))
    {
      HBITMAP hbm = (HBITMAP)GetClipboardData(uFormat);
      if ( hbm != NULL )
      {
		
		    if(uFormat == CF_BITMAP)
		    {
			    CreateFromHBitmap(hbm);
		    }
		    else if(uFormat == CF_DIB)
		    {
          LPSTR lpBits = (LPSTR) ::GlobalLock((HGLOBAL) hbm);
			    HPALETTE hPal = 0;
			    if(IsClipboardFormatAvailable (CF_PALETTE))
				    hPal = (HPALETTE) GetClipboardData(CF_PALETTE);

			    CreateFromHDIBBitmap ((BITMAPINFOHEADER*)lpBits, hPal);
          GlobalUnlock(hbm);
		    }
		    else
          // uFormat must be either CF_BITMAP or CF_DIB
    			PLASSERT (false);

        CloseClipboard();
        return true;
      }
      else
      {
        CloseClipboard();
        return false;
      }
    }
    else
      return false;
  }
  else
    return false;
}

void PLWinBmp::CopyPalette (PLWinBmp * pSrPLBmp)
{
	if(pSrPLBmp->GetBitsPerPixel() <= 8 && 
		this->GetPalette() && pSrPLBmp->GetPalette() )
	{
		for (UINT i = 0; i < (UINT)pSrPLBmp->GetNumColors(); i++)
		{
			BYTE * pbSrc = (BYTE *) (pSrPLBmp->GetPalette() + i);
			BYTE * pbDest = (BYTE *) (this->GetPalette() + i);
			pbDest[PL_RGBA_RED] = pbSrc[PL_RGBA_RED];
			pbDest[PL_RGBA_GREEN] = pbSrc[PL_RGBA_GREEN];
			pbDest[PL_RGBA_BLUE] = pbSrc[PL_RGBA_BLUE];
			pbDest[PL_RGBA_ALPHA] = pbSrc[PL_RGBA_ALPHA];
		}
	}
}


void PLWinBmp::TracePalette()
{
	if( GetBitsPerPixel() <= 8 && m_pClrTab)
  {
    UINT nColors = GetNumColors();  // 256, really...

		for (UINT i = 0; i < nColors; i++)
		{
			BYTE * pb = (BYTE *) (m_pClrTab+i);
			PLTRACE("%d: %d, %d, %d\n", i, pb[PL_RGBA_RED], pb[PL_RGBA_GREEN], pb[PL_RGBA_BLUE]);
		}
  }
}



/////////////////////////////////////////////////////////////////////
// Static functions

int PLWinBmp::GetLineMemNeeded (LONG width, WORD BitsPerPixel)
{
  // Calculate memory per line.
  int LineMem = width * BitsPerPixel / 8;

  if (BitsPerPixel == 1 && width % 8)
    ++LineMem;

  // DWORD alignment
  return ((LineMem + 3) & ~3);
}

long PLWinBmp::GetBitsMemNeeded (LONG width, LONG height, WORD BitsPerPixel)
// Returns memory needed by bitmap bits.
{
  return height * GetLineMemNeeded (width, BitsPerPixel);
}


long PLWinBmp::GetMemNeeded (LONG width, LONG height, WORD BitsPerPixel)
// Returns memory needed by a bitmap with the specified attributes.
{
  int HeaderMem = sizeof(BITMAPINFOHEADER); // Header memory
  if (BitsPerPixel < 16)
  { // Palette memory
    HeaderMem += (1 << BitsPerPixel)*sizeof (RGBQUAD);
  }

  return HeaderMem+GetBitsMemNeeded (width, height, BitsPerPixel);
}


/////////////////////////////////////////////////////////////////////
// Protected callbacks

void PLWinBmp::internalCreate (LONG Width, LONG Height, WORD BitsPerPixel,
                              bool bAlphaChannel, bool bIsGreyscale)
// Create a new empty DIB. Bits are uninitialized.
// Assumes that no memory is allocated before the call.
{
  // Allocate memory
  int MemNeeded = GetMemNeeded (Width, Height, BitsPerPixel);

#ifdef MAX_BITMAP_SIZE
  if (MemNeeded > MAX_BITMAP_SIZE)
    throw PLTextException(PL_ERRDIB_TOO_LARGE, "Bitmap size too large.\n");
#endif

  m_pBMI = (BITMAPINFOHEADER*) malloc (MemNeeded);
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

  Lock (true, true);

  // Set color table pointer & pointer to bits.
  initPointers ();

  initLocals (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);


  PLASSERT_VALID (this);
}


void PLWinBmp::internalCreate (BITMAPINFOHEADER* pBMI)
// Creates a PLWinBmp from an existing bitmap pointer.
// Assumes that no memory is allocated before the call.
{
  int MemNeeded = GetMemNeeded (pBMI->biWidth, pBMI->biHeight,
                                pBMI->biBitCount);
#ifdef MAX_BITMAP_SIZE
  if (MemNeeded > MAX_BITMAP_SIZE)
    throw PLTextException(PL_ERRDIB_TOO_LARGE, "Bitmap size too large.\n");
#endif

  m_pBMI = (BITMAPINFOHEADER *) malloc (MemNeeded);

	// out of memory?
	if (!m_pBMI)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory creating bitmap."));

	wmemcpy (m_pBMI, pBMI, MemNeeded);
  Lock (true, true);

  // Set color table pointer & pointer to bits.
  initPointers ();

  initLocals (m_pBMI->biWidth, m_pBMI->biHeight,
              m_pBMI->biBitCount, false, false);

  PLASSERT_VALID (this);
}


void PLWinBmp::freeMembers ()
{
  free(m_pBMI);
  m_pBMI = NULL;

  delete [] m_pLineArray;
  m_pLineArray = NULL;

  Unlock();
}


void PLWinBmp::initLineArray ()
{
  m_pLineArray = new BYTE * [m_Size.y];
  int LineLen = GetBytesPerLine();

  for (int y=0; y<m_Size.y; y++)
    m_pLineArray[y] = m_pBits + (m_Size.y-y-1)*LineLen;
}


HANDLE PLWinBmp::createCopyHandle ()
// Creates a copy of the current bitmap in a global memory block
// and returns a handle to this block.
{
  HANDLE  hCopy;

  int MemUsed = GetMemUsed();

  hCopy = (HANDLE) ::GlobalAlloc (GMEM_MOVEABLE | GMEM_DDESHARE,
                                  MemUsed);
  if (hCopy == NULL)
         throw (PLTextException (PL_ERRNO_MEMORY, "Can't allocate global memory block."));

  long * lpCopy = (long *) ::GlobalLock((HGLOBAL) hCopy);
  wmemcpy (lpCopy, m_pBMI, MemUsed);

  ::GlobalUnlock((HGLOBAL) hCopy);

  return hCopy;
}


void PLWinBmp::initPointers ()
// Set color table pointer & pointer to bits based on m_pBMI.
{
  if (m_pBMI->biBitCount < 16)
  { // Color table exists
    m_pClrTab = (PLPixel32 *)(((BITMAPINFO *) (m_pBMI))->bmiColors);

    m_pBits = (BYTE *)m_pClrTab +
              (1 << m_pBMI->biBitCount)*sizeof (RGBQUAD);
  }
  else
  { // No color table for 16 bpp and up.
    m_pClrTab = NULL;
    m_pBits =  (BYTE *)(((BITMAPINFO *) (m_pBMI))->bmiColors);
  }
}


BYTE * PLWinBmp::GetBits ()
{
  return m_pBits;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plwinbmp.cpp,v $
|      Revision 1.1  2009/12/09 05:34:48  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:44  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:23  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.2  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.18  2001/09/15 20:02:46  uzadow
|      CreateFromHBitmap now works for all bit depths.
|
|      Revision 1.17  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.16  2001/01/21 14:28:22  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.15  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.14  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.13  2000/11/21 20:29:39  uzadow
|      Added test project.
|
|      Revision 1.12  2000/10/12 21:59:34  uzadow
|      Added CreateFromHDIBBitmap() and CopyPalette() to PLWinBmp
|      Added CF_DIB support to PLWinBmp::FromClipboard() (Richard Hollis)
|
|      Revision 1.11  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.10  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.9  2000/08/02 13:38:50  Administrator
|      Corrected memory overwrite bug in PLWinBmp::CreateFromHBitmap()
|
|      Revision 1.8  2000/07/11 17:14:08  Ulrich von Zadow
|      Corrected error message for out of memory errors.
|
|      Revision 1.7  2000/05/28 10:12:36  Ulrich von Zadow
|      Fixed bug in CreateFromHBitmap.
|
|      Revision 1.6  2000/01/17 23:37:12  Ulrich von Zadow
|      Corrected bug in assignment operator.
|
|      Revision 1.5  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
\--------------------------------------------------------------------
*/
