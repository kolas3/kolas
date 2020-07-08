/*
/--------------------------------------------------------------------
|
|      $Id: plbitmap.cpp,v 1.1 2009/12/09 05:33:47 박종운 Exp $
|      Device independent bitmap class
|
|      Manipulates uncompressed device independent bitmaps
|      of all color depths.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plbitmap.h"
#include "plexcept.h"
#include "plfilterquantize.h"
#include "plpixel24.h"
#include "plpixel32.h"

template<class DestPixelC>
void createTrueColorCopy (PLBmp & rDestBmp, const PLBmp & rSrcBmp,
                                      DestPixelC Dummy);

// Creates an empty bitmap.
// Derived classes have to create a small bitmap here so the
// class can assume that a valid bitmap is available all the
// time.
PLBmp::PLBmp ()
: m_LockCount(0),
  m_DitherPaletteType(PLDTHPAL_DEFAULT),   // defaults for 8Bit color quantization
  m_DitherType(PLDTH_NONE)
{
	// KOL.UDF.022 시큐어코딩 보완
	m_pClrTab = NULL;
	m_pLineArray = NULL;
	m_LockCount = -1;
	m_DitherType = -1;
	m_DitherPaletteType = -1;
}


PLBmp::~PLBmp
    ()
{
  PLASSERT (m_LockCount == 0);
}

bool const PLBmp::operator ==
    ( PLBmp const &Other
    )
{
  return AlmostEqual (Other, 0);

}

// The dummy parameter is nessesary to work around an MSVC bug. If it isn't
// there, the function will only get instantiated once :-(.
template<class DestPixelC>
void createTrueColorCopy (PLBmp & rDestBmp, const PLBmp & rSrcBmp,
                          DestPixelC Dummy)
{
  // Make sure we're using the right template.
  PLASSERT (rDestBmp.GetBitsPerPixel() == sizeof (DestPixelC)*8);

  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  rDestBmp.Lock(false, true);
  // We'll change rSrcBmp back to the original state before returning,
  // so the const_cast is ok.
  const_cast<PLBmp &>(rSrcBmp).Lock(true, false);
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  DestPixelC ** pDstLines = (DestPixelC**) rDestBmp.GetLineArray();
  int SrcLineLen = rSrcBmp.GetWidth();

  for (int y = 0; y<rSrcBmp.GetHeight(); ++y)
  { // For each line
    DestPixelC * pDstPixel = pDstLines[y];
    switch (BPPSrc)
    {
      case 32:
        {
          PLPixel32 * pSrcPixel = (PLPixel32 *)pSrcLines[y];
          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = *pSrcPixel;
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 24:
        {
          PLPixel24 * pSrcPixel = (PLPixel24 *)pSrcLines[y];
          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = *pSrcPixel;
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 8:
        {
          PLPixel32 *pPal = rSrcBmp.GetPalette();

          PLBYTE * pSrcPixel = pSrcLines[y];

          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = pPal[*pSrcPixel];
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 1:
        {
          PLPixel32 * pPal = rSrcBmp.GetPalette();
          DestPixelC blackDot, whiteDot;
          // if bi-tonal "palette" exists, use it...
          if (pPal)
          {
            whiteDot = pPal[0];
            blackDot = pPal[1];
          }
          else
          {
            whiteDot.Set (255,255,255);
            blackDot.Set (0,0,0);
          }

          // assume msb is leftmost
          PLBYTE * pSrcPixel = pSrcLines[y];

          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            if (pSrcPixel[x / 8] & (128 >> (x & 7)))  // black pixel
              *pDstPixel = blackDot;
            else
              *pDstPixel = whiteDot;
            pDstPixel++;
          }
        }
        break;
      default:
        PLASSERT (false);
    }
  }
  const_cast<PLBmp &>(rSrcBmp).Unlock();
  rDestBmp.Unlock();
}

void PLBmp::CreateCopy (const PLBmp & rSrcBmp, int BPPWanted)
// Creates a copy of rSrcBmp, converting color depth if nessesary.
// Supports 1, 8 and 32 BPP. Alpha channel information is preserved.
{
  PLASSERT_VALID (&rSrcBmp);

  PLASSERT (BPPWanted == 32 || BPPWanted == 24 || BPPWanted == 8 ||
          BPPWanted == 1 || BPPWanted == 0);
  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  PLASSERT (BPPSrc == 32 || BPPSrc == 24 || BPPSrc == 8 || BPPSrc == 1);

  if (BPPWanted == BPPSrc || BPPWanted == 0)
  {
    if (&rSrcBmp != this)
    {
      // Create empty bitmap.
      freeMembers ();
      internalCopy (rSrcBmp);
    }
  }
  else
  {
    // Can't copy to self while changing bit depth.
    PLASSERT (&rSrcBmp != this);

    bool bDestAlpha = rSrcBmp.HasAlpha() && BPPWanted == 32;
    bool bDestGreyscale = rSrcBmp.IsGreyscale() && BPPWanted == 8;
    Create (rSrcBmp.GetWidth(), rSrcBmp.GetHeight(),
            BPPWanted, bDestAlpha, bDestGreyscale);

    switch (BPPWanted)
    {
      case 32:
        createTrueColorCopy (*this, rSrcBmp, PLPixel32());
        break;

      case 24:
        createTrueColorCopy (*this, rSrcBmp, PLPixel24());
        break;

      case 8:
        create8BPPCopy (rSrcBmp);
        break;

      case 1:
        create1BPPCopy (rSrcBmp);
        break;
      default:
        PLASSERT(false);
    }
    SetResolution (rSrcBmp.GetResolution());
    PLASSERT_VALID (this);
  }
}

void PLBmp::CreateFilteredCopy (PLBmp & rSrcBmp, const PLFilter & rFilter)
{
  PLASSERT_VALID (&rSrcBmp);
  rFilter.Apply (&rSrcBmp, this);
}

void PLBmp::SetQuantizationMode (int DitherType, int DitherPaletteType)
{
  PLASSERT_VALID (this);
  m_DitherType = DitherType;
  m_DitherPaletteType = DitherPaletteType;
}

#ifdef _DEBUG
void PLBmp::AssertValid () const
{
  if (m_Size != PLPoint(0,0))
  {
    // Color table only if 8 bpp.
    PLASSERT ((m_bpp > 8) == (m_pClrTab == NULL));
    PLASSERT (m_Size.y >= 0);
    PLASSERT (m_Size.x >= 0);

    // Greyscale only if 8 bpp
    PLASSERT ((m_bpp <= 8) || !m_bIsGreyscale);
  }
  PLASSERT (m_LockCount >= 0);
}
#endif


/////////////////////////////////////////////////////////////////////
// PLBmp creation

void PLBmp::Create (PLLONG Width, PLLONG Height, PLWORD BitsPerPixel,
                    bool bAlphaChannel, bool bIsGreyscale,
                    const PLPoint& Resolution)
// Create a new empty bitmap. Bits are uninitialized.
{
  PLASSERT_VALID (this);

  freeMembers ();
  internalCreate (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);
  m_Resolution = Resolution;

  PLASSERT_VALID (this);
}

void PLBmp::Create (const PLBmpInfo& Info)
{
  Create (Info.GetWidth(), Info.GetHeight(), Info.GetBitsPerPixel(),
          Info.HasAlpha(), Info.IsGreyscale(), Info.GetResolution());
}


/////////////////////////////////////////////////////////////////////
// PLBmp manipulation

void PLBmp::SetGrayPalette ()
// Fills the color table with a grayscale palette.
{
  PLASSERT (m_pClrTab); // Bitmap must contain a palette!

  int i;
  int NumColors = GetNumColors();
  double ColFactor = 255/(NumColors-1);

  for (i=0; i<NumColors; i++)
    SetPaletteEntry (i, int(i*ColFactor), int(i*ColFactor), int(i*ColFactor), 0xFF);
}

void PLBmp::SetPalette (PLPixel32 * pPal)
{
  PLASSERT (m_pClrTab); // Bitmap must contain a palette!

  // bdelmee code change
  wmemcpy (m_pClrTab, pPal, GetNumColors() * sizeof(PLPixel32));
}


void PLBmp::SetAlphaChannel (PLBmp * pAlphaBmp)
// Replaces the alpha channel with a new one.
{
  PLBYTE * pAlphaLine;
  int x,y;
  PLPixel32 ** pLineArray;
  PLBYTE ** pAlphaLineArray;

  PLASSERT_VALID (this);
  // Current bitmap must be 32 bpp.
  PLASSERT (GetBitsPerPixel() == 32);

  PLASSERT_VALID (pAlphaBmp);
  // Alpha channel must be 8 bpp.
  PLASSERT (pAlphaBmp->GetBitsPerPixel() == 8);

  // The two bitmaps must have the same dimensions
  PLASSERT (pAlphaBmp->GetWidth() == GetWidth());
  PLASSERT (pAlphaBmp->GetHeight() == GetHeight());

  Lock(false, true);
  pAlphaBmp->Lock(true, false) ;
  pLineArray = GetLineArray32();
  pAlphaLineArray = pAlphaBmp->GetLineArray();

  for (y=0; y < GetHeight(); y++)
  {
    PLPixel32 * pLine = pLineArray[y];
    pAlphaLine = pAlphaLineArray[y];
    for (x=0; x < GetWidth(); x++)
    {
      pLine[x].SetA(pAlphaLine[x]);
    }
  }
  pAlphaBmp->Unlock();
  Unlock();

  m_bAlphaChannel = true;

  PLASSERT_VALID (this);
}

void PLBmp::ApplyFilter
    ( const PLFilter& Filter
    )
{
  Filter.ApplyInPlace (this);
}

void PLBmp::Lock (bool bReadable, bool bWriteable)
{
  PLASSERT (m_LockCount >= 0);
  PLASSERT (bReadable || bWriteable);
  m_LockCount++;
}

void PLBmp::Unlock
    ()
{
  m_LockCount--;
  PLASSERT (m_LockCount >= 0);
}

PLBYTE PLBmp::FindNearestColor (PLPixel32 col)
{
  PLPixel32 * pPalette = GetPalette();
  PLASSERT (pPalette);

  int d1;
  int dMin = 100000;
  int i;
  int index;
  for (i = 0; i<GetNumColors(); i++)
  {
    d1 = col.BoxDist (pPalette[i]);
    if (d1 < dMin)
    {
      dMin = d1;
      index = i;
    }
  }
  return index;
}

bool PLBmp::AlmostEqual
    ( const PLBmp& Bmp,
      int epsilon
    ) const
{
  if (GetWidth() != Bmp.GetWidth() ||
      GetHeight() != Bmp.GetHeight() ||
      HasAlpha() != Bmp.HasAlpha() ||
      IsGreyscale() != Bmp.IsGreyscale() ||
      GetBitsPerPixel() != Bmp.GetBitsPerPixel())
    return false;

  if (m_Resolution != Bmp.GetResolution())
    return false;

  PLBYTE ** ppLines1 = GetLineArray();
  PLBYTE ** ppLines2 = Bmp.GetLineArray();
  int y,x;
  for (y=0; y<GetHeight(); y++)
    for (x=0; x<GetWidth(); x++)
      switch (GetBitsPerPixel())
      {
        case 8:
          if (abs (ppLines1[y][x] - ppLines2[y][x]) > epsilon)
            return false;
          break;
        case 24:
          if (ppLines1[y][x*3+PL_RGBA_RED] != ppLines2[y][x*3+PL_RGBA_RED] ||
              ppLines1[y][x*3+PL_RGBA_GREEN] != ppLines2[y][x*3+PL_RGBA_GREEN] ||
              ppLines1[y][x*3+PL_RGBA_BLUE] != ppLines2[y][x*3+PL_RGBA_BLUE])
            return false;
          break;
        case 32:
          if (abs (ppLines1[y][x*4+PL_RGBA_RED] - ppLines2[y][x*4+PL_RGBA_RED]) > epsilon ||
              abs (ppLines1[y][x*4+PL_RGBA_GREEN] - ppLines2[y][x*4+PL_RGBA_GREEN]) > epsilon ||
              abs (ppLines1[y][x*4+PL_RGBA_BLUE] - ppLines2[y][x*4+PL_RGBA_BLUE]) > epsilon)
            return false;
          if (HasAlpha() &&
              abs (ppLines1[y][x*4+3] - ppLines2[y][x*4+3]) > epsilon)
            return false;
          break;
        default:
          // Unsupported BPP.
          PLASSERT (false);
      }

  // Test if the palettes are the same for paletted bitmaps.
  if (GetBitsPerPixel() == 8)
  {
    int i;
    PLPixel32 * pPal1 = GetPalette();
    PLPixel32 * pPal2 = Bmp.GetPalette();
    for (i=0; i<255; i++)
    {
      if (abs (pPal1[i].GetR() - pPal2[i].GetR()) > epsilon ||
          abs (pPal1[i].GetG() - pPal2[i].GetG()) > epsilon ||
          abs (pPal1[i].GetB() - pPal2[i].GetB()) > epsilon)
        return false;
    }
  }

  return true;
}



/////////////////////////////////////////////////////////////////////
// Local functions


void PLBmp::initLocals (PLLONG Width, PLLONG Height, PLWORD BitsPerPixel,
                        bool bAlphaChannel, bool bIsGreyscale)
{
  SetBmpInfo (PLPoint (Width, Height), BitsPerPixel, PLPoint(0,0),
              bAlphaChannel, bIsGreyscale);

  // Initialize pointers to lines.
  initLineArray ();

  if (BitsPerPixel < 16)
  { // Color table exists
    SetGrayPalette ();
  }

  PLASSERT_VALID (this);
}

void PLBmp::internalCopy (const PLBmp & rSrcBmp)
{
  PLASSERT_VALID(&rSrcBmp);

  // Create empty bitmap.
  internalCreate (rSrcBmp.GetWidth(), rSrcBmp.GetHeight(), rSrcBmp.GetBitsPerPixel(),
                  rSrcBmp.HasAlpha(), rSrcBmp.IsGreyscale());
  // We'll change rSrcBmp back to the original state before returning,
  // so the const_cast is ok.
  const_cast<PLBmp &>(rSrcBmp).Lock(true, false);
  Lock(false, true);
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  PLBYTE ** pDstLines = GetLineArray();
  // Minimum possible line length.
  int LineLen = GetBytesPerLine();

  for (int y = 0; y<GetHeight(); y++)
    wmemcpy (pDstLines[y], pSrcLines[y], LineLen);

  Unlock();
  const_cast<PLBmp &>(rSrcBmp).Unlock();

  if (GetBitsPerPixel() <= 8)
    SetPalette (rSrcBmp.GetPalette());

  SetResolution (rSrcBmp.GetResolution());

  PLASSERT_VALID (this);
}

void PLBmp::create8BPPCopy (const PLBmp & rSrcBmp)
{
  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  Lock(false, true);
  const_cast<PLBmp &>(rSrcBmp).Lock(true, false);

  if (BPPSrc == 32) // Conversion 32->8 BPP
  {
    PLFilterQuantize Filter (m_DitherPaletteType, m_DitherType);
    Filter.Apply (&(const_cast<PLBmp &>(rSrcBmp)), this);
  }
  else // 1 -> 8
  {
    PLPixel32 *pPal = rSrcBmp.GetPalette();
    // if bi-tonal "palette" exists, use it...
    if (pPal)
    {
      PLBYTE *pWhite = (PLBYTE *) pPal;
      PLBYTE *pBlack = (PLBYTE *) (pPal+1);
      SetPaletteEntry(0,
                      pWhite[PL_RGBA_RED],pWhite[PL_RGBA_GREEN],pWhite[PL_RGBA_BLUE],
                      255);
      SetPaletteEntry(1,
                      pBlack[PL_RGBA_RED],pBlack[PL_RGBA_GREEN],pBlack[PL_RGBA_BLUE],
                      255);
    }
    else
    {
      SetPaletteEntry(0,255,255,255,255);
      SetPaletteEntry(1,0,0,0,255);
    }

    int SrcLineLen = rSrcBmp.GetWidth();
    // assume msb is leftmost
    for (int y = 0; y<GetHeight(); ++y)
    { // For each line
      PLBYTE * pSrcPixel = rSrcBmp.GetLineArray()[y];
      PLBYTE * pDstPixel = GetLineArray()[y];
      for (int x = 0; x < SrcLineLen; ++x)  // For each pixel
        pDstPixel[x] = pSrcPixel[x / 8] & (128 >> (x & 7)) ? 1 : 0;
    }
  }
  const_cast<PLBmp &>(rSrcBmp).Unlock();
  Unlock();
}

void PLBmp::create1BPPCopy (const PLBmp & rSrcBmp)
{
  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  Lock(false, true);
  const_cast<PLBmp &>(rSrcBmp).Lock(true, false);
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  PLBYTE ** pDstLines = GetLineArray();
  int SrcLineLen = GetWidth();

  SetPaletteEntry(0,255,255,255,255);
  SetPaletteEntry(1,0,0,0,255);

  // downgrade to monochrome
  PLPixel32 *pPal = rSrcBmp.GetPalette();
  PLBYTE *pRGBA;
  int DstLineLen = GetBytesPerLine();

  for (int y = 0; y < GetHeight(); ++y)
  { // For each line
    PLBYTE * pSrcPixel = pSrcLines[y];
    PLBYTE * pDstPixel = pDstLines[y];
    // fill with background (index 0) color
    wmemset(pDstPixel,0,DstLineLen);

    for (int x = 0; x < SrcLineLen; ++x)  // For each pixel
    {
      pRGBA = BPPSrc == 8 ? (PLBYTE*) &pPal[*pSrcPixel] : pSrcPixel;
      // the following criterion supposedly finds "dark" pixels; it may
      // need some twiddling and maybe use the alpha channel as well
      if (pRGBA[PL_RGBA_RED] < 128 &&
          pRGBA[PL_RGBA_GREEN] < 128 &&
          pRGBA[PL_RGBA_BLUE] < 128 )
        pDstPixel[x / 8] |= 128 >> (x & 7);
      pSrcPixel += BPPSrc == 8 ? 1 : sizeof(PLPixel32);
    }
  }
  const_cast<PLBmp &>(rSrcBmp).Unlock();
  Unlock();
}

/*
/--------------------------------------------------------------------
|
|      $Log: plbitmap.cpp,v $
|      Revision 1.1  2009/12/09 05:33:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:56  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:19  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:03  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:19  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:24  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.11  2002/11/02 20:33:27  uzadow
|      Added gif transparency handling (Mark Richarme)
|
|      Revision 1.10  2002/08/05 19:06:30  uzadow
|      no message
|
|      Revision 1.9  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.6  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.5  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.4  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.3  2001/09/30 19:54:17  uzadow
|      Improved AlmostEqual();
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.31  2001/09/15 14:30:20  uzadow
|      Fixed PLPixel32 initialization bug.
|
|      Revision 1.30  2001/09/13 20:47:36  uzadow
|      Removed commented-out lines.
|
|      Revision 1.29  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.28  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.27  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.26  2000/12/03 21:15:43  uzadow
|      Fixed png handling in configure; test/ adapted to linux
|
|      Revision 1.25  2000/11/21 20:18:03  uzadow
|      Added operator ==
|
|      Revision 1.24  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.23  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|      Revision 1.22  2000/09/26 12:14:51  Administrator
|      Refactored quantization.
|
|      Revision 1.21  2000/09/01 14:30:03  Administrator
|      no message
|
|      Revision 1.20  2000/09/01 14:19:46  Administrator
|      no message
|
|      Revision 1.19  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.18  2000/05/28 11:08:51  Ulrich von Zadow
|      Corrected alpha channel bug in CreateCopy.
|
|      Revision 1.17  2000/03/31 12:20:05  Ulrich von Zadow
|      Video invert filter (beta)
|
|      Revision 1.16  2000/03/31 11:53:29  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.15  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.14  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.13  1999/12/31 17:59:54  Ulrich von Zadow
|      Corrected error in PLBmp::SetPixel for 1 bpp.
|
|      Revision 1.12  1999/12/30 15:54:47  Ulrich von Zadow
|      Added PLWinBmp::FromClipBoard() and CreateFromHBitmap().
|
|      Revision 1.11  1999/12/10 01:27:26  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.10  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.9  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.8  1999/11/08 22:10:53  Ulrich von Zadow
|      no message
|
|      Revision 1.7  1999/10/22 21:25:51  Ulrich von Zadow
|      Removed buggy octree quantization
|
|      Revision 1.6  1999/10/21 18:47:26  Ulrich von Zadow
|      Added Rotate, GetPixel, SetPixel and FindNearestColor
|
|      Revision 1.5  1999/10/21 16:05:43  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.4  1999/10/19 21:29:44  Ulrich von Zadow
|      Added filters.
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/

