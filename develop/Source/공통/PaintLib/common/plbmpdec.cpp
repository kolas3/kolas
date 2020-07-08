/*
/--------------------------------------------------------------------
|
|      $Id: plbmpdec.cpp,v 1.1 2009/12/09 05:33:48 박종운 Exp $
|      Windows Bitmap Decoder Class
|
|      Windows bitmap file decoder. Decodes 1, 4, 8, 24 and 32 bpp
|      bitmap files (compressed and uncompressed) and returns a 32
|      bpp DIB.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plbmpdec.h"
#include "plexcept.h"

PLBmpDecoder::PLBmpDecoder
() : PLPicDecoder (),
     m_pBMI (0)
    // Creates a decoder
{
}


PLBmpDecoder::~PLBmpDecoder
    ()
{
}

void PLBmpDecoder::Open (PLDataSource * pDataSrc)
{
  m_pBMI = getInfoHeader (pDataSrc, &m_Pal[0]);

  int DestBPP = m_pBMI->biBitCount;
  bool bAlpha = false;
  if (DestBPP == 32)
    bAlpha = true;
  if (DestBPP > 8)
    DestBPP = 32;
  if (DestBPP < 8)
    DestBPP = 8;

  PLPoint DPI(0,0);
  if (m_pBMI->biXPelsPerMeter > 0)
    DPI.x = (int)((float)m_pBMI->biXPelsPerMeter / 39.37f+0.5);
  if (DPI.x <= 1)
    DPI.x = 0;
  if (m_pBMI->biYPelsPerMeter > 0)
    DPI.y = (int)((float)m_pBMI->biYPelsPerMeter / 39.37f+0.5);
  if (DPI.y <= 1)
    DPI.y = 0;

  SetBmpInfo (PLPoint (m_pBMI->biWidth, m_pBMI->biHeight), DestBPP, DPI, bAlpha, false);
}

void PLBmpDecoder::GetImage (PLBmp & Bmp)
{
  if (m_bpp == 8)
    Bmp.SetPalette (&m_Pal[0]);

  Bmp.Lock(false, true);

  try
  {
    switch (m_pBMI->biBitCount)
    {
      case 1:
        decode1bpp (m_pDataSrc, &Bmp);
        break;
      case 4:
        if (m_pBMI->biCompression == BI_RGB)
          decode4bpp (m_pDataSrc, &Bmp);
         else
          decodeRLE4 (m_pDataSrc, &Bmp);
        break;
      case 8:
        if (m_pBMI->biCompression == BI_RGB)
          decode8bpp (m_pDataSrc, &Bmp);
         else
          decodeRLE8 (m_pDataSrc, &Bmp);
        break;
      case 16:
        decodeHiColor (m_pDataSrc, &Bmp, m_pBMI);
        break;
      case 24:
      case 32:
        decodeTrueColor (m_pDataSrc, &Bmp, m_pBMI->biBitCount);
        break;
      default:
        // This is not a standard bmp file.
        raiseError (PL_ERRFORMAT_UNKNOWN,
                    "Decoding bmp: Illegal bpp value.");
    }

    Bmp.Unlock();
  }
  catch (...)
  {
    Bmp.Unlock();
    throw;
  }
}

void PLBmpDecoder::Close ()
{
  delete m_pBMI;

  PLPicDecoder::Close();
}


WINBITMAPINFOHEADER * PLBmpDecoder::getInfoHeader
    ( PLDataSource * pDataSrc,
      PLPixel32* pPal
    )
    // Decodes the bitmap file & info headers
{
  WINBITMAPFILEHEADER BFH;
  WINBITMAPINFOHEADER * pBMI;
  int offset = 0;

  BFH.bfType = ReadIWord (pDataSrc);

  while (BFH.bfType == 0x4142)
  {
    BFH.bfSize = ReadILong (pDataSrc);
    BFH.bfReserved1 = ReadIWord (pDataSrc);
    BFH.bfReserved2 = ReadIWord (pDataSrc);
    BFH.bfOffBits = ReadILong (pDataSrc);
    BFH.bfType = ReadIWord (pDataSrc);
    offset += 14;
  }

  BFH.bfSize = ReadILong (pDataSrc);
  BFH.bfReserved1 = ReadIWord (pDataSrc);
  BFH.bfReserved2 = ReadIWord (pDataSrc);
  BFH.bfOffBits = ReadILong (pDataSrc);

  // Check for bitmap file signature: First 2 bytes are 'BA', 'BM',
  // 'IC', 'PI', 'CI' or 'CP'
  if (!((BFH.bfType == 0x4142) || (BFH.bfType == 0x4d42) ||
        (BFH.bfType == 0x4349) || (BFH.bfType == 0x5043) ||
        (BFH.bfType == 0x4943) || (BFH.bfType == 0x5043)))
    raiseError (PL_ERRWRONG_SIGNATURE,
                "Bitmap decoder: This isn't a bitmap.");

  Trace (2, "Bitmap file signature found\n");

  pBMI = new WINBITMAPINFOHEADER;

  try
  {
    int Colors = 0;

    pBMI->biSize = ReadILong (pDataSrc);

    if (pBMI->biSize == 12)
    {
      // read as BITMAPCOREHEADER struct (OS2 1.x Format)
      pBMI->biWidth    = ReadIWord(pDataSrc);
      pBMI->biHeight   = ReadIWord(pDataSrc);
      pBMI->biPlanes   = ReadIWord(pDataSrc);
      pBMI->biBitCount = ReadIWord(pDataSrc);

      pBMI->biCompression = 0;
      pBMI->biSizeImage = 0;
      pBMI->biXPelsPerMeter = 0;
      pBMI->biYPelsPerMeter = 0;
      pBMI->biClrUsed = 0;
      pBMI->biClrImportant = 0;

      // Read palette if 8 bpp or less.
      if (pBMI->biBitCount <= 8)
      {
        Colors = readPalette (pBMI, pDataSrc, pPal, 3);
        pDataSrc->Skip(BFH.bfOffBits - 26 - 3 * Colors - offset);
      }
    }
    else if (pBMI->biSize == 40)
    {
      // read as BITMAPINFOHEADER struct
      pBMI->biWidth = ReadILong (pDataSrc);
      pBMI->biHeight = ReadILong (pDataSrc);
      pBMI->biPlanes = ReadIWord (pDataSrc);
      pBMI->biBitCount = ReadIWord (pDataSrc);
      pBMI->biCompression = ReadILong (pDataSrc);
      pBMI->biSizeImage = ReadILong (pDataSrc);
      pBMI->biXPelsPerMeter = ReadILong (pDataSrc);
      pBMI->biYPelsPerMeter = ReadILong (pDataSrc);
      pBMI->biClrUsed = ReadILong (pDataSrc);
      pBMI->biClrImportant = ReadILong (pDataSrc);

      // Read palette if 8 bpp or less.
      if (pBMI->biBitCount <= 8)
        Colors = readPalette (pBMI, pDataSrc, pPal, 4);
    }
    else
    {
      // read as BITMAPINFOHEADER struct (OS2 2.x Format)
      pBMI->biWidth = ReadILong (pDataSrc);
      pBMI->biHeight = ReadILong (pDataSrc);
      pBMI->biPlanes = ReadIWord (pDataSrc);
      pBMI->biBitCount = ReadIWord (pDataSrc);
      pBMI->biCompression = ReadILong (pDataSrc);
      pBMI->biSizeImage = ReadILong (pDataSrc);
      pBMI->biXPelsPerMeter = ReadILong (pDataSrc);
      pBMI->biYPelsPerMeter = ReadILong (pDataSrc);
      pBMI->biClrUsed = ReadILong (pDataSrc);
      pBMI->biClrImportant = ReadILong (pDataSrc);
      pDataSrc->Skip(pBMI->biSize - sizeof (WINBITMAPINFOHEADER));

      // Read palette if 8 bpp or less.
      if (pBMI->biBitCount <= 8)
        Colors = readPalette (pBMI, pDataSrc, pPal, 3);
    }

    Trace (2, "Bitmap header is ok.\n");

  }
  catch (PLTextException)
  {
    if (pBMI)
      delete pBMI;
    throw;
  }
  catch(...)
  {
    delete pBMI;
    throw;
  }
  return pBMI;
}

void PLBmpDecoder::decode1bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    )
    // Decodes a 2-color bitmap. Ignores the palette & just uses
    // black & white as 'colors' if decoding to 32 bit
{
  int i;
  int y;                           // Current row
  int x;                           // Current column

  PLBYTE * pDest;                    // Current destination.
  PLBYTE * pSrc;                     // Current position in file.
  PLBYTE   BTable[8];                // Table of bit masks.
  PLBYTE   SrcByte;                  // Source byte cache.
  int    XSize = pBmp->GetWidth(); // Width of bitmap in pixels.
  int    LineLen = ((XSize+7)/8 + 3) & ~3;
                                   // Width of source in bytes
                                   //   (DWORD-aligned).
  int    LinePadding = LineLen-((XSize+7)/8);
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                   // Pointers to dest. lines.

  int    OpaqueBlack = 0x00000000;
  *(((PLBYTE*)&OpaqueBlack)+PL_RGBA_ALPHA) = 0xFF;

  Trace (2, "Decoding 1 bit per pixel bitmap.\n");

  // Initialize bit masks.
  for (i=0; i<8; i++)
  {
    BTable[i] = 1<<i;
  }

  for (y=0; y<pBmp->GetHeight(); y++)
  { // For each line...
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    for (x=0; x<XSize/8; x++)
    { // For each source byte...
      pSrc = pDataSrc->Read1Byte();
      SrcByte = *(pSrc);
      for (i=7; i>=0; i--)
      { // For each bit...
        if (SrcByte & BTable[i]) // Test if bit i is set
          *pDest = 0x01;
         else
          *pDest = 0x00;
        pDest++;
      }
    }

    // Last few bits in line...
    if (XSize & 7)
    {
      pSrc = pDataSrc->Read1Byte();
      SrcByte = *(pSrc);
      for (i=7; i>7-(XSize & 7); i--)
      { // For each bit...
        if (SrcByte & BTable[i]) // Test if bit i is set
          *pDest = 0x01;
         else
          *pDest = 0x00;
        pDest++;
      }
    }
    pDataSrc->Skip (LinePadding);
  }
}


void PLBmpDecoder::decode4bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    )
    // Decodes an uncompressed 16-color-bitmap.
{
  int y;                            // Current row
  int x;                            // Current column

  PLBYTE * pDest;                     // Current destination.
  PLBYTE * pSrc;                      // Current position in file.
  PLBYTE   SrcByte;                   // Source byte cache.
  int    XSize = pBmp->GetWidth();  // Width of bitmap in pixels.
  int    LineLen = ((XSize+1)/2 + 3) & ~3;
                                    // Width of source in bytes
                                    //   (DWORD-aligned).
  int    LinePadding = LineLen-((XSize+1)/2);
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                   // Pointers to dest. lines.

  Trace (2, "Decoding uncompressed 4 bit per pixel bitmap.\n");

  for (y=0; y<pBmp->GetHeight(); y++)
  { // For each line...
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    for (x=0; x<XSize/2; x++)
    { // For each source byte...
      pSrc = pDataSrc->Read1Byte();
      SrcByte = *(pSrc);

      *pDest = SrcByte>>4;
      pDest++;
      *pDest = SrcByte & 15;
      pDest++;
   }

    // Last nibble in line if line length is odd.
    if (XSize & 1)
    {
      pSrc = pDataSrc->Read1Byte();
      *pDest = (*(pSrc))>>4;
      pDest++;
    }
    pDataSrc->Skip (LinePadding);
  }
}


void PLBmpDecoder::decode8bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    )
    // Decodes an uncompressed 256-color-bitmap.
{
  int y;                            // Current row
  int x;                            // Current column

  PLBYTE * pDest;                     // Current destination.
  PLBYTE * pSrc;                      // Current position in file.
  int    XSize = pBmp->GetWidth();// Width of bitmap in pixels.
  int    LineLen = (XSize + 3) & ~3;
                                    // Width of source in bytes
                                    //   (DWORD-aligned).
  int    LinePadding = LineLen-XSize;
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                   // Pointers to dest. lines.

  Trace (2, "Decoding uncompressed 8 bit per pixel bitmap.\n");

  for (y=0; y<pBmp->GetHeight(); y++)
  { // For each line...
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    for (x=0; x<XSize; x++)
    { // For each source byte...
      pSrc = pDataSrc->Read1Byte();
      *pDest = *pSrc;
      pDest++;
    }
    pDataSrc->Skip (LinePadding);
  }
}


void PLBmpDecoder::decodeRLE4
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    )
    // Decodes a compressed 16-color-bitmap.
{
  int y;                              // Current row

  PLBYTE * pSrc;
  PLBYTE * pDest;                       // Current destination.
  int    XSize = pBmp->GetWidth();  // Width of bitmap in pixels.
  PLBYTE   SrcByte;                     // Source byte cache.

  PLBYTE   RunLength;    // Length of current run.
  bool   bOdd;         // true if current run has odd length.

  bool   bEOL;         // true if end of line reached.
  bool   bEOF=false;   // true if end of file reached.

  PLBYTE * pLineBuf;     // Current line as uncompressed nibbles.
  PLBYTE * pBuf;         // Current position in pLineBuf.
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                   // Pointers to dest. lines.

  Trace (2, "Decoding RLE4-compressed bitmap.\n");

  // Allocate enough memory for DWORD alignment in original 4 bpp
  // bitmap.
  pLineBuf = new PLBYTE [XSize*4+28];

  for (y=0; y<pBmp->GetHeight() && !bEOF; y++)
  { // For each line...
    pBuf = pLineBuf;
    bEOL=false;
    while (!bEOL)
    { // For each packet do
      pSrc = pDataSrc->Read1Byte();
      RunLength = *pSrc;
      if (RunLength==0)
      { // Literal or escape.
        pSrc = pDataSrc->Read1Byte();
        RunLength = *pSrc;
        switch (RunLength)
        {
          case 0: // End of line escape
            bEOL = true;
            break;
          case 1: // End of file escape
            bEOF = true;
            bEOL = true;
            break;
          case 2: // Delta escape.
            // I have never seen a file using this.
            delete [] pLineBuf;
            raiseError (PL_ERRFORMAT_NOT_SUPPORTED,
                        "Encountered delta escape.");
            break;
          default:
            // Literal packet
            bOdd = (RunLength & 1);
            RunLength /= 2; // Convert pixels to bytes.
            for (int i=0; i<RunLength; i++)
            { // For each source byte...
              pSrc = pDataSrc->Read1Byte();
              decode2Nibbles (pBuf, *pSrc);
              pBuf += 2;
            }
            if (bOdd)
            { // Odd length packet -> one nibble left over
              pSrc = pDataSrc->Read1Byte();
              *pBuf = (*(pSrc))>>4;
              pBuf++;
            }
            // Word alignment at end of literal packet.
            if ((RunLength + bOdd) & 1) pDataSrc->Skip(1);
        }
      }
      else
      { // Encoded packet:
        // RunLength 4 bpp pixels with 2 alternating
        // values.
        pSrc = pDataSrc->Read1Byte();
        SrcByte = *pSrc;
        for (int i=0; i<RunLength/2; i++)
        {
          decode2Nibbles (pBuf, SrcByte);
          pBuf += 2;
        }
        if (RunLength & 1)
        {
          *pBuf = (*(pSrc))>>4;
          pBuf++;
        }
      }
    }
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    wmemcpy (pDest, pLineBuf, XSize);
  }
  delete [] pLineBuf;
}


void PLBmpDecoder::decodeRLE8
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    )
    // Decodes a compressed 256-color-bitmap.
{
  int y;                              // Current row

  PLBYTE * pDest;                       // Current destination.
  PLBYTE * pSrc;                        // Current position in file.
  PLBYTE   RunLength;                   // Length of current run.
  bool   bEOL;                        // true if end of line reached.
  bool   bEOF=false;                  // true if end of file reached.
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                      // Pointers to dest. lines.

  Trace (2, "Decoding RLE8-compressed bitmap.\n");

  for (y=0; y<pBmp->GetHeight() && !bEOF; y++)
  { // For each line...
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    bEOL=false;
    while (!bEOL)
    { // For each packet do
      pSrc = pDataSrc->Read1Byte();
      RunLength = *pSrc;
      if (RunLength==0)
      { // Literal or escape.
        pSrc = pDataSrc->Read1Byte();
        RunLength = *pSrc;
        switch (RunLength)
        {
          case 0: // End of line escape
            bEOL = true;
            break;
          case 1: // End of file escape
            bEOF = true;
            bEOL = true;
            break;
          case 2: // Delta escape.
            // I have never seen a file using this...
            raiseError (PL_ERRFORMAT_NOT_SUPPORTED,
                        "Encountered delta escape.");
            bEOL = true;
            bEOF = true;
            break;
          default:
            // Literal packet
            pSrc = pDataSrc->ReadNBytes(RunLength);
            wmemcpy (pDest, pSrc, RunLength);
            pDest += RunLength;
            // Word alignment at end of literal packet.
            if (RunLength & 1) pDataSrc->Skip(1);
        }
      }
      else
      { // Encoded packet:
        // RunLength pixels, all with the same value
        pSrc = pDataSrc->Read1Byte();
        wmemset (pDest, *pSrc, RunLength);
        pDest += RunLength;
      }
    }
  }
}

void PLBmpDecoder::decodeHiColor
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      WINBITMAPINFOHEADER * pBMI
    )
{
  if (pBMI->biCompression == BI_RGB)
    decodeTrueColor (pDataSrc, pBmp, 15);
  else
  {
    PLASSERT (pBMI->biCompression == BI_BITFIELDS);

    // since getinfoheader jumps to bitmap bits
    // we are already past the masks, so "go back "
    PLULONG *pBuff    = (PLULONG*)pDataSrc->GetBufferPtr(0);
    PLULONG dwRedMask   = *(pBuff-3);
    PLULONG dwGreenMask = *(pBuff-2);
    PLULONG dwBlueMask  = *(pBuff-1);

    if (dwRedMask == 0x7c00 && dwGreenMask == 0x03E0 && dwBlueMask == 0x001f)
      decodeTrueColor (pDataSrc, pBmp, 15);
    else
      decodeTrueColor (pDataSrc, pBmp, 16);
  }
}


void PLBmpDecoder::decodeTrueColor
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int SrcBPP
    )
    // Decodes true-color bitmap
{
  int y;                            // Current row
  PLBYTE * pDest;                     // Current destination.
  PLBYTE ** pLineArray = pBmp->GetLineArray();
                                   // Pointers to dest. lines.

  Trace (2, "Decoding true-color bitmap.\n");

  for (y=0; y<pBmp->GetHeight(); y++)
  { // For each line...
    pDest = pLineArray[pBmp->GetHeight()-y-1];
    switch (SrcBPP)
    {
      case 15:
        decode15bppLine (pDataSrc, pBmp, pDest);
        break;
      case 16:
        decode16bppLine (pDataSrc, pBmp, pDest);
        break;
      case 24:
        decode24bppLine (pDataSrc, pBmp, pDest);
        break;
      case 32:
        decode32bppLine (pDataSrc, pBmp, pDest);
        break;
      default:
        // This routine should never have been called if we get here.
        PLASSERT (false);
    }
  }
}

void PLBmpDecoder::decode15bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    )
{
  int LineLen = (pBmp->GetWidth()*2 + 3) & ~3; // Width of source in bytes
                                               // (DWORD-aligned).
  int LinePadding = LineLen - pBmp->GetWidth()*2;

  int x;                            // Current column
  for (x=0; x<pBmp->GetWidth(); x++)
  { // For each pixel...
    PLBYTE * pSrc = pDataSrc->ReadNBytes(2);     // Current position in file.
    PLWORD Pixel = *(PLWORD*)pSrc;
    *(pDest+PL_RGBA_RED) = PLBYTE (( Pixel >> 7 ) & 0x0F8);
    *(pDest+PL_RGBA_GREEN) = PLBYTE (( Pixel >> 2 ) & 0x0F8);
    *(pDest+PL_RGBA_BLUE) = PLBYTE (( Pixel & 0x1F ) * 8);
    *(pDest+PL_RGBA_ALPHA) = 0xFF;
    pDest += 4;
  }
  pDataSrc->Skip (LinePadding);
}

void PLBmpDecoder::decode16bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    )
{
  int LineLen = (pBmp->GetWidth()*2 + 3) & ~3; // Width of source in bytes
                                               // (DWORD-aligned).
  int LinePadding = LineLen - pBmp->GetWidth()*2;

  int x;                            // Current column
  for (x=0; x<pBmp->GetWidth(); x++)
  { // For each pixel...
    PLBYTE * pSrc = pDataSrc->ReadNBytes(2);     // Current position in file.
    PLWORD Pixel = *(PLWORD*)pSrc;
    *(pDest+PL_RGBA_RED) = PLBYTE (( Pixel >> 8 ) & 0x0F8);
    *(pDest+PL_RGBA_GREEN) = PLBYTE (( Pixel >> 3 ) & 0x0F8);
    *(pDest+PL_RGBA_BLUE) = PLBYTE (( Pixel & 0x1F ) * 8);
    *(pDest+PL_RGBA_ALPHA) = 0xFF;
    pDest += 4;
  }
  pDataSrc->Skip (LinePadding);
}


void PLBmpDecoder::decode24bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    )
{
  int LineLen = (pBmp->GetWidth()*3 + 3) & ~3; // Width of source in bytes
                                               // (DWORD-aligned).
  int LinePadding = LineLen - pBmp->GetWidth()*3;

  int x;                            // Current column
  for (x=0; x<pBmp->GetWidth(); x++)
  { // For each pixel...
    PLBYTE * pSrc;                    // Current position in file.
    pSrc = pDataSrc->ReadNBytes(3);
    *(pDest+PL_RGBA_BLUE) = ((WINRGBQUAD *)pSrc)->rgbBlue;
    *(pDest+PL_RGBA_GREEN) = ((WINRGBQUAD *)pSrc)->rgbGreen;
    *(pDest+PL_RGBA_RED) = ((WINRGBQUAD *)pSrc)->rgbRed;
    *(pDest+PL_RGBA_ALPHA) = 0xFF;
    pDest += 4;
  }
  pDataSrc->Skip (LinePadding);
}

void PLBmpDecoder::decode32bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    )
{
  PLBYTE * pSrc = pDataSrc->ReadNBytes (pBmp->GetWidth()*4);
  wmemcpy (pDest, pSrc, pBmp->GetWidth()*4);
}


void PLBmpDecoder::decode2Nibbles
    ( PLBYTE * pDest,
      PLBYTE SrcByte
    )
    // Decodes two 4-bit pixels.
{
  *pDest = SrcByte>>4;
  *(pDest+1) = SrcByte & 15;
}

int PLBmpDecoder::readPalette
    ( WINBITMAPINFOHEADER * pBMI,     // Pointer to bitmapinfoheader in file.
      PLDataSource * pDataSrc,
      PLPixel32 * pPal,
      int RGBSize
    )
    // Assumes 8 bpp or less.
{
  Trace (3, "Reading palette.\n");
  int i;

  int NumColors;
  if (pBMI->biClrUsed == 0)
    NumColors = 1<<(pBMI->biBitCount);
   else
    NumColors = pBMI->biClrUsed;

  PLBYTE * pFilePal = (PLBYTE *) pDataSrc->ReadNBytes (NumColors*RGBSize);
  WINRGBQUAD * pCurEntry;

  // Correct the byte ordering & copy the data.
  for (i=0; i<NumColors; i++)
  {
    pCurEntry = (WINRGBQUAD *)(pFilePal+(RGBSize)*i);
    pPal[i].Set (pCurEntry->rgbRed, pCurEntry->rgbGreen,
                 pCurEntry->rgbBlue, 0xFF);
  }

  return NumColors;
}
/*
/--------------------------------------------------------------------
|
|      $Log: plbmpdec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:48  박종운
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
|      Revision 1.1  2006/11/29 03:34:21  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:24  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:30  황정영
|      2006.05.17
|
|      Revision 1.10  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.9  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.8  2002/03/29 18:53:34  uzadow
|      Added support for 16 bpp bitmaps with nonstandard bitmasks.
|
|      Revision 1.7  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.6  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.5  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.4  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.3  2001/10/06 15:32:22  uzadow
|      Removed types LPBYTE, DWORD, UCHAR, VOID and INT from the code.
|
|      Revision 1.2  2001/10/05 21:15:09  uzadow
|      Improved support for OS/2 bitmap decoding.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.20  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.19  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.18  2001/01/14 15:32:21  uzadow
|      Unix compatibility changes.
|
|      Revision 1.17  2001/01/14 15:09:33  uzadow
|      Added support for decoding OS/2 bitmaps.
|
|      Revision 1.16  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.15  2000/12/09 13:45:55  uzadow
|      Added support for 16 bpp bitmaps.
|
|      Revision 1.14  2000/12/04 13:20:28  uzadow
|      Minor refactorings.
|
|      Revision 1.13  2000/10/24 22:57:42  uzadow
|      Fixed exception handling
|
|      Revision 1.12  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.11  2000/07/07 13:20:47  Ulrich von Zadow
|      Bugfix: Added #pragma pack to WINBITMAPFILEHEADER.
|
|      Revision 1.10  2000/05/23 10:19:11  Ulrich von Zadow
|      Minor unix compatibility changes.
|
|      Revision 1.9  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.8  2000/01/04 18:36:02  Ulrich von Zadow
|      Corrected handling of bitmap files with extended headers.
|
|      Revision 1.7  1999/12/30 15:54:02  Ulrich von Zadow
|      no message
|
|      Revision 1.6  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.5  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.4  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
