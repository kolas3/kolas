/*
/--------------------------------------------------------------------
|
|      $Id: pljpegenc.cpp,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|
|      JPEG file encoder. Uses the independent JPEG group's library
|      to do the actual conversion.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include <stdio.h>

extern "C"
{
#include "jpeglib.h"
}

#include "pljpegenc.h"
#include "plexcept.h"
#include "jmemdest.h"

// This is for RGB_RED, RGB_GREEN, RGB_BLUE, RGB_PIXELSIZE
#define JPEG_INTERNALS
#include <jmorecfg.h>


/////////////////////////////////////////////////////////////////////
// Error handling.

METHODDEF(void)
error_exit (j_common_ptr pcinfo)
// This procedure is called by the IJPEG library when an error
// occurs.
{
  /* Create the message string */
  char sz[256];
  (pcinfo->err->format_message) (pcinfo, sz);
  strcat (sz, "\n");

  PLPicEncoder::raiseError (PL_ERRFORMAT_NOT_SUPPORTED, sz);
}

/////////////////////////////////////////////////////////////////////
// Class functions

PLJPEGEncoder::PLJPEGEncoder
    ()
    : PLPicEncoder(),
      m_pcinfo ( new jpeg_compress_struct ),
      m_pjerr ( new jpeg_error_mgr ),   // Custom error manager.
      iQuality_ (0),
      bOptimizeCoding_ (false),
      iSmoothingFactor_ (0),
      uiDensityX_ (0),
      uiDensityY_ (0)
    // Creates an encoder
{
  m_pcinfo->err = jpeg_std_error (m_pjerr);
  m_pjerr->error_exit = error_exit;  // Register custom error manager.

  jpeg_create_compress (m_pcinfo);
}


PLJPEGEncoder::~PLJPEGEncoder
    ()
{
  jpeg_destroy_compress (m_pcinfo);
  delete m_pjerr;
  delete m_pcinfo;
}

void PLJPEGEncoder::DoEncode
    ( PLBmp * pBmp,
      PLDataSink * pDataSink
    )
{
  PLASSERT (pBmp->GetBitsPerPixel() == 32); // Only true-color supported for now.
  try
  {
  // todo: notification not yet implemented for encoders (3.6.99 MS)
  /*
    JMETHOD( void, notify, (j_common_ptr));
    notify = JNotification;
  */

    // Initialize custom data destination
    jpeg_mem_dest(m_pcinfo, pDataSink->GetBufferPtr(), pDataSink->GetMaxDataSize(), pDataSink);

    // Set Header Fields
    m_pcinfo->image_width = pBmp->GetWidth();
    m_pcinfo->image_height = pBmp->GetHeight();

    m_pcinfo->input_components = RGB_PIXELSIZE; // Constant from  libjpeg
    m_pcinfo->in_color_space = JCS_RGB;

    jpeg_set_defaults (m_pcinfo);

    m_pcinfo->X_density = pBmp->GetResolution().x;
    m_pcinfo->Y_density = pBmp->GetResolution().y;

    // on good FPUs (e.g. Pentium) this is the fastest and "best" DCT method
    m_pcinfo->dct_method = JDCT_FLOAT;

    // set up user settings
    if (iQuality_)
        jpeg_set_quality(m_pcinfo, iQuality_, true);
    m_pcinfo->optimize_coding = bOptimizeCoding_;
    m_pcinfo->smoothing_factor = iSmoothingFactor_;
    // density will be in DPI
    if (uiDensityX_ || uiDensityY_)
    {
        m_pcinfo->density_unit = 1;
        m_pcinfo->X_density = uiDensityX_;
        m_pcinfo->Y_density = uiDensityY_;
    }

    jpeg_start_compress (m_pcinfo,true);

    encodeRGB (pBmp, pBmp->GetHeight());

    jpeg_finish_compress (m_pcinfo);
  }
  catch (PLTextException)
  {
    jpeg_abort_compress(m_pcinfo);
    throw;
  }

}

// 　� RGB_PIXELSIZE constant from libjpeg !!!
// 　� RGB_RED, RGB_GREEN, RGB_BLUE constants from libjpeg !!!
void PLJPEGEncoder::encodeRGB
    ( PLBmp * pBmp,
      int iScanLines
    )
    // Assumes IJPEG decoder is already set up.
{

  PLBYTE * pBuf = NULL;
  int CurLine = 0;
  JSAMPARRAY ppBuf = &pBuf;

  pBmp->Lock(true, false);

#if ((PL_RGBA_RED!=RGB_RED)||(PL_RGBA_GREEN!=RGB_GREEN)||(PL_RGBA_BLUE!=RGB_BLUE)||(4!=RGB_PIXELSIZE))
  pBuf = new PLBYTE [pBmp->GetWidth()*RGB_PIXELSIZE];
#endif

  int written = 0;
  while (CurLine < iScanLines)
  {
#if ((PL_RGBA_RED!=RGB_RED)||(PL_RGBA_GREEN!=RGB_GREEN)||(PL_RGBA_BLUE!=RGB_BLUE)||(4!=RGB_PIXELSIZE))
    PLPixel32 ** pLineArray = pBmp->GetLineArray32();
    int i;
    for (i=0;i<pBmp->GetWidth();i++)
    {

      PLPixel32 * pSrcPixel = pLineArray[CurLine]+i;
      PLBYTE * pDestPixel = pBuf+i*RGB_PIXELSIZE;
      pDestPixel[RGB_RED] = pSrcPixel->GetR();
      pDestPixel[RGB_GREEN] = pSrcPixel->GetG();
      pDestPixel[RGB_BLUE] = pSrcPixel->GetB();
    }
    jpeg_write_scanlines (m_pcinfo, ppBuf, 1);
    CurLine++;
#else
    PLBYTE ** pLineArray = pBmp->GetLineArray();
    written = jpeg_write_scanlines (m_pcinfo,&pLineArray[CurLine], 32);
    CurLine+=written;

    if(!written)
      break;
#endif
  }
  pBmp->Unlock();
  if(pBuf)
  {
    delete[] pBuf;
  }
}

/////////////////////////////////////////////////////////////////////

void PLJPEGEncoder::SetQuality(int iQuality)
{
  iQuality_ = iQuality;
}

void PLJPEGEncoder::SetOptimizeCoding(bool bOptimizeCoding)
{
  bOptimizeCoding_ = bOptimizeCoding;
}

void PLJPEGEncoder::SetSmoothingFactor(int iSmoothingFactor)
{
  iSmoothingFactor_ = iSmoothingFactor;
}

void PLJPEGEncoder::SetDensity(unsigned int uiX, unsigned int uiY)
{
  uiDensityX_ = uiX;
  uiDensityY_ = uiY;
}

/*
/--------------------------------------------------------------------
|
|      $Log: pljpegenc.cpp,v $
|      Revision 1.1  2009/12/09 05:33:51  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:59  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:55  박우람
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
|      Revision 1.7  2002/12/11 21:17:48  uzadow
|      Fixed jpeg rgb order problem.
|
|      Revision 1.6  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.5  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.4  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.14  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.13  2001/09/15 17:12:40  uzadow
|      Added jpeg codec optimizations by Petr Kures.
|
|      Revision 1.12  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.11  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.10  2000/11/10 10:41:15  jmbuena
|      Fixed jpeg encoder bug on GNU/Linux
|
|      Revision 1.5  2000/11/08 13:36:32  jmbuena
|      Changes due to paintlib changes
|
|      Revision 1.9  2000/10/26 21:06:17  uzadow
|      Removed dependency on jpegint.h
|
|      Revision 1.8  2000/10/25 13:58:22  jmbuena
|      Fixed a non defined symbol problem for GNU/Linux
|
|      Revision 1.7  2000/10/24 23:00:09  uzadow
|      Added byte order conversion.
|
|      Revision 1.6  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.5  2000/07/11 17:11:01  Ulrich von Zadow
|      Added support for RGBA pixel ordering (Jose Miguel Buenaposada Biencinto).
|
|      Revision 1.4  2000/05/22 17:43:25  Ulrich von Zadow
|      Added SetQuality(), SetDensity(), SetOptimizeCoding() and
|      SetSmoothingFactor().
|
|      Revision 1.3  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.1  1999/10/19 21:28:05  Ulrich von Zadow
|      Added jpeg encoder
|
|
\--------------------------------------------------------------------
*/
