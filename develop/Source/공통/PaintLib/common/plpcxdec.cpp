/*
/--------------------------------------------------------------------
|
|      $Id: plpcxdec.cpp,v 1.1 2009/12/09 05:33:53 박종운 Exp $
|      PCX Decoder Class
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include <iostream>

#include "plstdpch.h"
#include "plpcxdec.h"
#include "plexcept.h"

PLPCXDecoder::PLPCXDecoder()
  : PLPicDecoder()
// Creates a decoder
{
	// KOL.UDF.022 시큐어코딩 보완
	m_PcxHeader = NULL;
}


PLPCXDecoder::~PLPCXDecoder()
{}


void PLPCXDecoder::Open(PLDataSource * pDataSrc)
{

  // Check if the file is a valid PCX file or not
  Trace (2, "Decoding PCX.\n");

  m_PcxHeader = (LPPCXHEADER)pDataSrc->ReadNBytes(sizeof(tagPCXHEADER));

  if (m_PcxHeader->Manufacturer != PCX_MAGIC)
  {
    raiseError (PL_ERRWRONG_SIGNATURE, "Error decoding pcx: Not a PCX file.");
  }
  // Check for PCX run length encoding
  if (m_PcxHeader->Encoding != 1)
  {
    raiseError (PL_ERRWRONG_SIGNATURE, "File has unknown encoding scheme.");
  }

  // Check that we can handle this image format
  switch (m_PcxHeader->BitsPerPixel)
  {
    case 1:
      if (m_PcxHeader->ColorPlanes > 4)
      {
        raiseError(PL_ERRFORMAT_UNKNOWN, "Can't handle image with more than 4 planes.");
      }
      break;
    case 2:
    case 4:
    case 8:
      if (m_PcxHeader->ColorPlanes == 1 || m_PcxHeader->ColorPlanes == 3)
        break;
    default:
      raiseError(PL_ERRFORMAT_UNKNOWN, "Can't handle bits per pixel image with planes.");
      break;
  }

  int Width = (m_PcxHeader->Xmax - m_PcxHeader->Xmin) + 1;
  int Height = (m_PcxHeader->Ymax - m_PcxHeader->Ymin) + 1;
  SetBmpInfo (PLPoint(Width, Height), 32,
      PLPoint (m_PcxHeader->Hres, m_PcxHeader->Vres), false, false);
}

void PLPCXDecoder::GetImage (PLBmp & Bmp)
{
  int i, x, y;
  PLBYTE ColorMap[PCX_MAXCOLORS][3];
  PLBYTE * pcximage = NULL;
  PLBYTE * lpHead1 = NULL;
  PLBYTE * lpHead2 = NULL;
  PLBYTE * pcxplanes;
  PLBYTE * pcxpixels;
  PLBYTE c;
  int nbytes, count;

  // Check if the file is a valid PCX file or not
  Trace (2, "Decoding PCX.\n");

  try
  {
    Bmp.Create (*this);

    nbytes = m_PcxHeader->BytesPerLine * m_PcxHeader->ColorPlanes * GetHeight();
    lpHead1 = pcximage = (PLBYTE *)malloc(nbytes);
    try
    {
      while (nbytes > 0)
      {
        c = ReadByte(m_pDataSrc);
        if ((c & 0XC0) != 0XC0) // Repeated group
        {
          *pcximage++ = c;
          --nbytes;
          continue;
        }
        count = c & 0X3F; // extract count
        c = ReadByte(m_pDataSrc);
        if (count > nbytes)
        {
          raiseError(PL_ERRINTERNAL, "repeat count spans end of image.");
        }
        nbytes -= count;
        while (--count >=0)
          *pcximage++ = c;
      }
    }
    catch (PLTextException &e)
    {
      // Just in case BytesPerLine is bogus.
      // This will fall apart for images that have a palette after the
      // image data, however.
      if (e.GetCode() != PL_ERREND_OF_FILE)
        throw;
    }
    pcximage = lpHead1;

    for (i = 0; i < 16; i++)
    {
      ColorMap[i][0] = m_PcxHeader->ColorMap[i][0];
      ColorMap[i][1] = m_PcxHeader->ColorMap[i][1];
      ColorMap[i][2] = m_PcxHeader->ColorMap[i][2];
    }
    if (m_PcxHeader->BitsPerPixel == 8 && m_PcxHeader->ColorPlanes == 1)
    {
      PLBYTE colsig = ReadByte(m_pDataSrc);
/* It seems like valid PCXs exist with a bad color map signature...
      if (colsig != PCX_256_COLORS)
      {
        raiseError(PL_ERRINTERNAL, "bad color map signature.");
      }
*/
      for (i = 0; i < PCX_MAXCOLORS; i++)
      {
        ColorMap[i][0] = ReadByte(m_pDataSrc);
        ColorMap[i][1] = ReadByte(m_pDataSrc);
        ColorMap[i][2] = ReadByte(m_pDataSrc);
      }
    }
    if (m_PcxHeader->BitsPerPixel == 1 && m_PcxHeader->ColorPlanes == 1)
    {
      ColorMap[0][0] = ColorMap[0][1] = ColorMap[0][2] = 0;
      ColorMap[1][0] = ColorMap[1][1] = ColorMap[1][2] = 255;
    }

    lpHead2 = pcxpixels = (PLBYTE *)malloc(GetWidth() + m_PcxHeader->BytesPerLine * 8);
    // Convert the image
    Bmp.Lock(false, true);
    PLPixel32 ** pLineArray = Bmp.GetLineArray32();
    for (y = 0; y < GetHeight(); y++)
    {
      pcxpixels = lpHead2;
      pcxplanes = pcximage + (y * m_PcxHeader->BytesPerLine * m_PcxHeader->ColorPlanes);
      if (m_PcxHeader->ColorPlanes == 3 && m_PcxHeader->BitsPerPixel == 8)
      {
        // Deal with 24 bit color image
        for (x = 0; x < GetWidth(); x++)
        {
          PLPixel32 * pPixel = pLineArray[y];
          pPixel[x].Set (pcxplanes[x],
                         pcxplanes[m_PcxHeader->BytesPerLine + x],
                         pcxplanes[2*m_PcxHeader->BytesPerLine + x],
                         0xFF);
        }
        continue;
      }
      else if (m_PcxHeader->ColorPlanes == 1)
      {
        PCX_UnpackPixels(pcxpixels, pcxplanes, m_PcxHeader->BytesPerLine, m_PcxHeader->ColorPlanes, m_PcxHeader->BitsPerPixel);
      }
      else
      {
        PCX_PlanesToPixels(pcxpixels, pcxplanes, m_PcxHeader->BytesPerLine, m_PcxHeader->ColorPlanes, m_PcxHeader->BitsPerPixel);
      }
      for (x = 0; x < GetWidth(); x++)
      {
        i = pcxpixels[x];
        PLPixel32 * pPixel = pLineArray[y];
        pPixel[x].Set (ColorMap[i][0], ColorMap[i][1],
                       ColorMap[i][2], 0xFF);
      }
    }
    Bmp.Unlock();
  }

  catch (PLTextException)
  {
    if (lpHead1)
    {
      free(lpHead1);
      lpHead1 = NULL;
    }
    if (lpHead2)
    {
      free(lpHead2);
      lpHead2 = NULL;
    }
    throw;
  }

  if (lpHead1)
  {
    free(lpHead1);
    lpHead1 = NULL;
  }
  if (lpHead2)
  {
    free(lpHead2);
    lpHead2 = NULL;
  }
}

// Convert multi-plane format into 1 pixel per byte
// from unpacked file data bitplanes[] into pixel row pixels[]
// image Height rows, with each row having planes image planes each
// bytesperline bytes
void PLPCXDecoder::PCX_PlanesToPixels(PLBYTE * pixels, PLBYTE * bitplanes,
                                     short bytesperline, short planes, short bitsperpixel)
{
  int i, j;
  int npixels;
  PLBYTE * p;
  if (planes > 4)
  {
    raiseError(PL_ERRINTERNAL, "Can't handle more than 4 planes.");
  }
  if (bitsperpixel != 1)
  {
    raiseError(PL_ERRINTERNAL, "Can't handle more than 1 bit per pixel.");
  }

  // Clear the pixel buffer
  npixels = ((bytesperline-1) * 8) / bitsperpixel;
  p = pixels;
  while (--npixels >= 0)
    *p++ = 0;

  // Do the format conversion
  for (i = 0; i < planes; i++)
  {
    int pixbit, bits, mask;
    p = pixels;
    pixbit = (1 << i);  // pixel bit for this plane
    for (j = 0; j < bytesperline; j++)
    {
      bits = *bitplanes++;
      for (mask = 0X80; mask != 0; mask >>= 1, p++)
        if (bits & mask)
          *p |= pixbit;
    }
  }
}


// convert packed pixel format into 1 pixel per byte
// from unpacked file data bitplanes[] into pixel row pixels[]
// image Height rows, with each row having planes image planes each
// bytesperline bytes
void PLPCXDecoder::PCX_UnpackPixels(PLBYTE * pixels, PLBYTE * bitplanes,
                                   short bytesperline, short planes, short bitsperpixel)
{
  register int bits;
  if (planes != 1)
  {
    raiseError(PL_ERRINTERNAL, "Can't handle packed pixels with more than 1 plane.");
  }
  if (bitsperpixel == 8)  // 8 bits/pixels, no unpacking needed
  {
    while (bytesperline-- > 0)
      *pixels++ = *bitplanes++;
  }
  else if (bitsperpixel == 4)  // 4 bits/pixel, two pixels per byte
  {
    while (bytesperline-- > 0)
    {
      bits = *bitplanes++;
      *pixels++ = (PLBYTE)((bits >> 4) & 0X0F);
      *pixels++ = (PLBYTE)((bits) & 0X0F);
    }
  }
  else if (bitsperpixel == 2)  // 2 bits/pixel, four pixels per byte
  {
    while (bytesperline-- > 0)
    {
      bits = *bitplanes++;
      *pixels++ = (PLBYTE)((bits >> 6) & 0X03);
      *pixels++ = (PLBYTE)((bits >> 4) & 0X03);
      *pixels++ = (PLBYTE)((bits >> 2) & 0X03);
      *pixels++ = (PLBYTE)((bits) & 0X03);
    }
  }
  else if (bitsperpixel == 1)  // 1 bits/pixel, 8 pixels per byte
  {
    while (bytesperline-- > 0)
    {
      bits = *bitplanes++;
      *pixels++ = ((bits & 0X80) != 0);
      *pixels++ = ((bits & 0X40) != 0);
      *pixels++ = ((bits & 0X20) != 0);
      *pixels++ = ((bits & 0X10) != 0);
      *pixels++ = ((bits & 0X08) != 0);
      *pixels++ = ((bits & 0X04) != 0);
      *pixels++ = ((bits & 0X02) != 0);
      *pixels++ = ((bits & 0X01) != 0);
    }
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: plpcxdec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:53  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:56  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:04  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:22  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:26  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:31  황정영
|      2006.05.17
|
|      Revision 1.9  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/01/27 18:20:15  uzadow
|      Updated copyright message; corrected pcx decoder bug.
|
|      Revision 1.6  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.5  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.4  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.3  2001/10/06 15:32:22  uzadow
|      Removed types LPPLBYTE, DWORD, UCHAR, VOID and INT from the code.
|
|      Revision 1.2  2001/09/30 16:56:29  uzadow
|      Fixed bug decoding wierd pcx files.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.12  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.11  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.10  2000/10/27 11:30:06  uzadow
|      Fixed bug #117544
|
|      Revision 1.9  2000/10/23 22:03:07  uzadow
|      Bugfix for bug #116362 by Marcus J. Hodge
|
|      Revision 1.8  2000/09/21 14:35:13  Administrator
|      Fixed bug in pcx decoder
|
|      Revision 1.7  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.6  2000/03/30 21:23:43  Ulrich von Zadow
|      no message
|
|      Revision 1.5  2000/03/17 10:51:12  Ulrich von Zadow
|      Bugfix for b/w images.
|
|      Revision 1.4  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|
\--------------------------------------------------------------------
*/
