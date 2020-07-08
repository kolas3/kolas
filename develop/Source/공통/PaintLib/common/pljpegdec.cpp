/*
/--------------------------------------------------------------------
|
|      $Id: pljpegdec.cpp,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|      JPEG Decoder Class
|
|      JPEG file decoder. Uses the independent JPEG group's library
|      to do the actual conversion.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "pljpegdec.h"

#include "plexcept.h"

#include "jmemsrc.h"

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

  PLPicDecoder::raiseError (PL_ERRFORMAT_NOT_SUPPORTED, sz);
}

/////////////////////////////////////////////////////////////////////
// Class functions

PLJPEGDecoder::PLJPEGDecoder
    ()
    : PLPicDecoder(),
      m_bFast(true)
    // Creates a decoder
{
  cinfo.err = jpeg_std_error (&jerr);
  jerr.error_exit = error_exit;  // Register custom error manager.

  jpeg_create_decompress (&cinfo);
}


PLJPEGDecoder::~PLJPEGDecoder
    ()
{
  jpeg_destroy_decompress (&cinfo);
}

void PLJPEGDecoder::SetFast
    ( bool bFast
    )
    // true (the default) selects fast but sloppy decoding.
{
  m_bFast = bFast;
}

void PLJPEGDecoder::Open (PLDataSource * pDataSrc)
{
  try
  {
    // Jo Hagelberg 15.4.99: added progress notification callback
    JMETHOD( void, notify, (j_common_ptr));
    notify = JNotification;

    // Initialize custom data source.
    // Jo Hagelberg 15.4.99: added pDataSrc and notify
    jpeg_mem_src (&cinfo, pDataSrc->ReadEverything(),
                  pDataSrc->GetFileSize(), (void*)pDataSrc, notify);

    jpeg_read_header (&cinfo, true);

    int DestBPP = 32;
    bool bIsGreyscale = false;
    if (cinfo.out_color_space == JCS_GRAYSCALE)
    {
      DestBPP = 8;
      bIsGreyscale = true;
    }

	  PLPoint DPI;

	  DPI.x = cinfo.X_density;
	  DPI.y = cinfo.Y_density;
    
    if (DPI.x == 1 || DPI.y == 1) 
    {
      // Unknown DPI
		  DPI.x = 0;
	    DPI.y = 0;
    } 

    // Check for dots per centimeter and convert if nessesary.
    // We assume that 'unknown' density_unit (=0) means inches here.
    if (cinfo.density_unit == 2) 
    {
      DPI.x = int(2.54*DPI.x);
      DPI.y = int(2.54*DPI.y);
    }

    SetBmpInfo (PLPoint (cinfo.image_width, cinfo.image_height), DestBPP, DPI, false, bIsGreyscale);
  }
  catch (PLTextException)
  {
    jpeg_abort_decompress(&cinfo);
    throw;
  }


}

void PLJPEGDecoder::GetImage (PLBmp & Bmp)
{
  try
  {
    if (m_bFast)
    {
      cinfo.do_fancy_upsampling = false;
    }

    // Choose floating point DCT method.
    cinfo.dct_method = JDCT_FLOAT;

    jpeg_start_decompress (&cinfo);

    if (cinfo.out_color_space == JCS_GRAYSCALE)
      decodeGray (&Bmp);
    else
      decodeRGB (&Bmp);
    jpeg_finish_decompress (&cinfo);
  }
  catch (PLTextException)
  {
    jpeg_abort_decompress(&cinfo);
    throw;
  }
}


void PLJPEGDecoder::decodeRGB
    ( PLBmp * pBmp)
    // Assumes IJPEG decoder is already set up.
{
  int CurLine = 0;

#if ((PL_RGBA_RED!=RGB_RED)||(PL_RGBA_GREEN!=RGB_GREEN)||(PL_RGBA_BLUE!=RGB_BLUE)||(4!=RGB_PIXELSIZE))
  PLBYTE * pBuf;
  JSAMPARRAY ppBuf = &pBuf;
  pBuf = new PLBYTE[GetWidth()*sizeof (PLPixel32)];
#endif

  pBmp->Lock(false, true);

  int read = 0;
  while (CurLine < GetHeight())
  {
    // Correct pixel ordering if libjpeg is configured differently
    // than paintlib.
#if ((PL_RGBA_RED!=RGB_RED)||(PL_RGBA_GREEN!=RGB_GREEN)||(PL_RGBA_BLUE!=RGB_BLUE)||(4!=RGB_PIXELSIZE))
    PLPixel32 ** pLineArray = pBmp->GetLineArray32();
    jpeg_read_scanlines (&cinfo, ppBuf, 1);
    int i;
    for (i=0; i<GetWidth(); i++)
    {
      PLBYTE * pSrcPixel = pBuf+i*RGB_PIXELSIZE;
      PLPixel32 * pDestPixel = pLineArray[CurLine]+i;
      pDestPixel->SetR (pSrcPixel[RGB_RED]);
      pDestPixel->SetG (pSrcPixel[RGB_GREEN]);
      pDestPixel->SetB (pSrcPixel[RGB_BLUE]);
    }
    CurLine++;
#else
    PLBYTE ** pLineArray = pBmp->GetLineArray();
    read = jpeg_read_scanlines (&cinfo,&pLineArray[CurLine],32);
    CurLine+=min(read,(GetHeight()-CurLine));
    if(read==0)
      break;
#endif
  }
  pBmp->Unlock();
#if ((PL_RGBA_RED!=RGB_RED)||(PL_RGBA_GREEN!=RGB_GREEN)||(PL_RGBA_BLUE!=RGB_BLUE)||(4!=RGB_PIXELSIZE))
  delete[] pBuf;
#endif

}

void PLJPEGDecoder::decodeGray
    ( PLBmp * pBmp)
    // Assumes IJPEG decoder is already set up.
{
  PLBYTE * pDst;
  int CurLine = 0;
  PLBYTE * pBuf = new PLBYTE [GetWidth()];
  try
  {
    JSAMPARRAY ppBuf = &pBuf;

    pBmp->Lock(false, true);
    PLBYTE ** pLineArray = pBmp->GetLineArray();

    while (CurLine < GetHeight())
    {
      ppBuf = &pDst;
      *ppBuf = pLineArray[CurLine];
      jpeg_read_scanlines (&cinfo, ppBuf, 1);

      CurLine++;
    }
    pBmp->Unlock();
  }
  catch(...)
  {
    delete [] pBuf;
    throw;
  }
  delete [] pBuf;
}


/*
 * Jo Hagelberg 15.4.99
 * progress notification callback
 * since this is a static function we need pDataSrc from cinfo->client_data
 * progress is calculated (0...1) and PaintLib's Notification called
 */

void PLJPEGDecoder::JNotification (j_common_ptr cinfo)
{
  double       part;
  PLDataSource *pDataSrc;

  /* calculated according to jpeg lib manual
   * note: this may not be precice when using buffered image mode
   * todo: think hard of alternatives 4 this case ... :-)
   */
  part = ( (double)cinfo->progress->completed_passes +
           ((double)cinfo->progress->pass_counter/cinfo->progress->pass_limit) ) /
         (double)cinfo->progress->total_passes;

  // call Notification in PLDataSource
  pDataSrc = (PLDataSource*) cinfo->client_data;
  if (pDataSrc)
  {
    pDataSrc->OProgressNotification( part);
  }

}

/*
/--------------------------------------------------------------------
|
|      $Log: pljpegdec.cpp,v $
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
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.3  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.19  2001/09/15 17:12:40  uzadow
|      Added jpeg codec optimizations by Petr Kures.
|
|      Revision 1.18  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.17  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.16  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.15  2000/11/10 10:40:53  jmbuena
|      Fixed jpeg decoder bug on GNU/Linux
|
|      Revision 1.5  2000/11/08 13:36:32  jmbuena
|      Changes due to paintlib changes
|
|      Revision 1.14  2000/10/30 14:32:51  uzadow
|      Removed dependency on jinclude.h
|
|      Revision 1.13  2000/10/27 11:58:25  jmbuena
|      paintlib-config added, libjpeg and so on taked from the system ones, copied libs internal headers onto paintlib
|
|      Revision 1.12  2000/10/26 21:06:17  uzadow
|      Removed dependency on jpegint.h
|
|      Revision 1.11  2000/10/24 23:00:09  uzadow
|      Added byte order conversion.
|
|      Revision 1.10  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.9  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.8  1999/12/14 12:30:13  Ulrich von Zadow
|      Corrected copy constructor and assignment operator.
|
|      Revision 1.7  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
|
|      Revision 1.6  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.5  1999/10/19 21:25:16  Ulrich von Zadow
|      no message
|
|      Revision 1.4  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
