/*
/--------------------------------------------------------------------
|
|      $Id: plpngdec.cpp,v 1.1 2009/12/09 05:33:55 박종운 Exp $
|      PNG Decoder Class
|
|      PNG file decoder. Uses LibPng to do the actual decoding.
|      PNG supports many pixel formats not supported by paintlib.
|      These pixel formats are converted to the nearest paintlib
|      equivalent. Images with less or more than 8 bits per channel
|      are converted to 8 bits per channel. Images with 16-bit
|      palettes or grayscale images with an alpha channel are
|      returned as full 32-bit RGBA bitmaps.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plpngdec.h"
#include "plexcept.h"
#include "png.h"


PLPNGDecoder::PLPNGDecoder
    ()
    : PLPicDecoder()
    // Creates a decoder
{
	// KOL.UDF.022 시큐어코딩 보완
	m_color_type = -1;
	m_bit_depth = -1;
}


PLPNGDecoder::~PLPNGDecoder
    ()
{
}



void my_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
  // todo : check data erasing
  PLBYTE *ptr;
  PLDataSource* pSourceInfo=(PLDataSource*)png_get_io_ptr(png_ptr);

  ptr = pSourceInfo->ReadNBytes(length);
  wmemcpy(data,ptr,length);
}

void PLPNGDecoder::Open (PLDataSource * pDataSrc)
{
  png_uint_32 width, height;
  int interlace_type;

  m_png_ptr = png_create_read_struct
              (PNG_LIBPNG_VER_STRING,
               (void *)NULL,
               user_error_fn,
               user_warning_fn);
  PLASSERT (m_png_ptr);

  m_info_ptr = png_create_info_struct(m_png_ptr);
  PLASSERT (m_info_ptr);

  png_set_read_fn(m_png_ptr, (void*)pDataSrc, my_read_data);

  /* The call to png_read_info() gives us all of the information from the
   * PNG file before the first IDAT (image data chunk).
   */
  png_read_info(m_png_ptr, m_info_ptr);

  png_get_IHDR(m_png_ptr, m_info_ptr, &width, &height, &m_bit_depth, &m_color_type,
               &interlace_type, NULL, NULL);

  int DestBPP;
  bool bHasAlpha;
  bool bIsGreyscale = false;
  switch (m_color_type)
  {
    case PNG_COLOR_TYPE_RGB:
      DestBPP = 32;
      bHasAlpha = false;
      break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
      DestBPP = 32;
      bHasAlpha = true;
      break;
    case PNG_COLOR_TYPE_GRAY:
      DestBPP = 8;
      bHasAlpha = false;
      bIsGreyscale = true;
      break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      DestBPP = 32;
      png_set_gray_to_rgb(m_png_ptr);
      png_set_expand(m_png_ptr);
      bHasAlpha = true;
      break;
    case PNG_COLOR_TYPE_PALETTE:
      if (m_bit_depth != 16)
        DestBPP = 8;
      else
      { // 16-bit palette image
        png_set_expand(m_png_ptr);
        DestBPP = 32;
      }
      bHasAlpha = false;
      break;
  }

  if (DestBPP == 32)
  { // Make sure we use the correct byte order
// Jose Miguel Buenaposada 2000/06/30
#ifdef PL_PIXEL_BGRA_ORDER
    png_set_bgr (m_png_ptr);
#else
#ifdef PL_PIXEL_RGBA_ORDER 
#else
#error Unsupported byte ordering!
#endif
#endif

    if (!bHasAlpha)
#if (PL_RGBA_ALPHA == 3)
      png_set_filler(m_png_ptr, 0xff, PNG_FILLER_AFTER);
#else
      png_set_filler(m_png_ptr, 0xff, PNG_FILLER_BEFORE);
#endif
  }

  SetBmpInfo (PLPoint (width, height), DestBPP, PLPoint(0,0), bHasAlpha, bIsGreyscale);

  PLULONG XRes, YRes;
  int UnitType;
  png_get_pHYs (m_png_ptr, m_info_ptr, &XRes, &YRes,
                &UnitType);
  if (UnitType == PNG_RESOLUTION_METER)
    m_Resolution = PLPoint (int (XRes/39.37f+0.5), int (YRes/39.37f+0.5));
}

void PLPNGDecoder::GetImage (PLBmp & Bmp)
{
  if (m_color_type == PNG_COLOR_TYPE_GRAY)
  {
    int i;
    int NumColors = 1<<(m_bit_depth);
    for (i=0; i<NumColors; i++)
    {
      int CurColor = (i*255)/(NumColors-1);
      Bmp.SetPaletteEntry(i, CurColor, CurColor, CurColor, 0xFF);
    }
  }

  if (m_color_type == PNG_COLOR_TYPE_PALETTE)
  {
    png_color* ppng_color_tab=NULL;

    int   i;
    int   nbColor=0;

    png_get_PLTE(m_png_ptr, m_info_ptr, &ppng_color_tab, &nbColor);

    for (i=0; i<nbColor; i++)
    {
      Bmp.SetPaletteEntry(i,
                            (*(ppng_color_tab+i)).red,
                            (*(ppng_color_tab+i)).green,
                            (*(ppng_color_tab+i)).blue,
                            0xFF);
    }
  }

  if (m_bit_depth == 16)
    png_set_strip_16(m_png_ptr);
  if (m_bit_depth < 8)
    png_set_packing(m_png_ptr);

  Bmp.Lock(false, true);
  PLBYTE ** pLineArray = Bmp.GetLineArray();
  png_read_image(m_png_ptr, pLineArray);
  Bmp.Unlock();

  /* read rest of file, and get additional chunks in info_ptr - REQUIRED */
  png_read_end(m_png_ptr, m_info_ptr);

  /* clean up after the read, and free any memory allocated - REQUIRED */
  png_destroy_read_struct(&m_png_ptr, &m_info_ptr, (png_infopp)NULL);
}


void PLPNGDecoder::user_error_fn
    ( png_structp png_ptr,
      png_const_charp error_msg
    )
{
  raiseError (PL_ERRFORMAT_UNKNOWN, (char *)error_msg);
}

void PLPNGDecoder::user_warning_fn
    ( png_structp png_ptr,
      png_const_charp warning_msg
    )
{
  PLTRACE ((char *)warning_msg);
}

/*
/--------------------------------------------------------------------
|
|      $Log: plpngdec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:02  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.6  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.5  2001/10/16 18:00:19  uzadow
|      Linux compatibility
|
|      Revision 1.4  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.11  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.10  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.9  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.8  2000/07/11 17:11:01  Ulrich von Zadow
|      Added support for RGBA pixel ordering (Jose Miguel Buenaposada Biencinto).
|
|      Revision 1.7  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  2000/01/04 18:35:23  Ulrich von Zadow
|      no message
|
|      Revision 1.5  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
|
|      Revision 1.4  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
