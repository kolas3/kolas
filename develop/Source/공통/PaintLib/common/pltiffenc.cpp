/*
/--------------------------------------------------------------------
|
|      $Id: pltiffenc.cpp,v 1.1 2009/12/09 05:33:58 박종운 Exp $
|
|      TIFF file encoder. Uses LIBTIFF to do the actual conversion.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include <stdarg.h>
#include "pltiffenc.h"
#include "plbitmap.h"
#include "plexcept.h"
extern "C"
{
#include "tiffio.h"   // for the tags definitions
#include "tif_msrc.h"
}


/////////////////////////////////////////////////////////////////////
// Class functions

// Creates an encoder
PLTIFFEncoder::PLTIFFEncoder()
  : PLPicEncoder()
{
    m_Compression = COMPRESSION_PACKBITS;
}



PLTIFFEncoder::~PLTIFFEncoder()
{}

void PLTIFFEncoder::SetCompression (PLWORD Compression)
{
    m_Compression = Compression;
}


void PLTIFFEncoder::DoEncode (PLBmp * pBmp, PLDataSink* pDataSnk)
{
  TIFF* tif = TIFFOpenMem (pDataSnk->m_pStartData,
                           pDataSnk->m_nMaxFileSize,
                           &(pDataSnk->m_nCurPos));
  PLASSERT( tif );
  /*
  if (!tif)
  raiseError (PL_ERRWRONG_SIGNATURE, PLTIFFDecoder::m_szLastErr);
  */

  // initialize TIFF "directory"
  SetBaseTags( tif, pBmp );

  DoTiffEncode( pBmp, tif );

  TIFFClose( tif );
}


void PLTIFFEncoder::DoTiffEncode (PLBmp* pBmp, TIFF* tif)
{
  int k;
  uint32 l, c, image_length, image_width;
  // iterate over data
  PLBYTE **pla = pBmp->GetLineArray();
  PLASSERT( pla );

  image_length = (uint32) pBmp->GetHeight();
  image_width  = (uint32) pBmp->GetWidth();
  switch (pBmp->GetBitsPerPixel())
  {
    case 8:
      {
        // first, save the colormap
        uint16 red[256];
        uint16 green[256];
        uint16 blue[256];

        PLPixel32 * pPal = pBmp->GetPalette();
        PLASSERT( pPal );
        for (int i = 0; i < pBmp->GetNumColors(); i++, pPal++)
        {
          red[i]   = pPal->GetR ();
          green[i] = pPal->GetG ();
          blue[i]  = pPal->GetB ();
        }
        SetField( tif, TIFFTAG_COLORMAP, red, green, blue );
      }
      // fall-through

    case 1:  // TODO: a bit of error checking
      for (l = 0; l < image_length; l++)
        k = TIFFWriteScanline( tif, pla[l], l, 0 );
      break;

    case 32:
      {
        // TODO: check whether (r,g,b) components come in the correct order here...
        PLBYTE* pBuf = new PLBYTE[3*image_width];
        for (l = 0; l < image_length; l++)
        {
          for (c = 0; c < image_width; c++)
          {
            pBuf[c*3 + 0] = pla[l][c*sizeof(PLPixel32) + PL_RGBA_RED];
            pBuf[c*3 + 1] = pla[l][c*sizeof(PLPixel32) + PL_RGBA_GREEN];
            pBuf[c*3 + 2] = pla[l][c*sizeof(PLPixel32) + PL_RGBA_BLUE];
          }
          k = TIFFWriteScanline( tif, pBuf, l, 0 );
        }
        delete [] pBuf;
      }
      break;

    default:
      PLASSERT(false);
  }
  // we could flush at this point, but TIFFClose will do it anyway
}


// According to the characteristics of the given bitmap,
// set the baseline tags

int PLTIFFEncoder::SetBaseTags (TIFF* tif, PLBmp* pBmp)
{
  PLASSERT( tif && pBmp );

  uint16 ui16 = 0;
  uint32 ui32 = 0;

  ui32 = pBmp->GetWidth();
  SetField( tif, TIFFTAG_IMAGEWIDTH,      ui32 );
  ui32 = pBmp->GetHeight();
  SetField( tif, TIFFTAG_IMAGELENGTH,     ui32 );
  // one strip = the whole image
  // SetField( tif, TIFFTAG_ROWSPERSTRIP,    ui32 );
  ui16 = pBmp->GetBitsPerPixel();
  if (ui16 > 8) ui16 = 8;
  SetField( tif, TIFFTAG_BITSPERSAMPLE,   ui16 );
  ui16 = pBmp->GetBitsPerPixel();
  ui16 = ui16 <= 8 ? 1 : 3;
  SetField( tif, TIFFTAG_SAMPLESPERPIXEL, ui16 );


  ui16 = m_Compression;
  SetField( tif, TIFFTAG_COMPRESSION,     ui16 );

  ui16 = PLANARCONFIG_CONTIG;
  SetField( tif, TIFFTAG_PLANARCONFIG,    ui16 );

  /*
   * The following tags are supposedly mandatory,
   * but libtiff seems to have sensible defaults for us
   *

  ui32 = 0;
  SetField( TIFFTAG_SUBFILETYPE,     ui32 );
  ?!?
  SetField( TIFFTAG_STRIPOFFSETS,    ui32 );
  ?!?
  SetField( TIFFTAG_STRIPBYTECOUNT,  ui32 );
   *
   *
   */

  float nRes = float(pBmp->GetResolution().x);
  SetField(tif,TIFFTAG_XRESOLUTION,nRes);
  nRes = float(pBmp->GetResolution().y);
  SetField(tif,TIFFTAG_YRESOLUTION,nRes);
  SetField (tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

  switch (pBmp->GetBitsPerPixel())
  {
    case 1:
      {
        // look at bi-level palette...
        PLBYTE* p = (PLBYTE*) pBmp->GetPalette();
        ui16 = p[PL_RGBA_RED] < p[PL_RGBA_RED + sizeof(PLPixel32)] &&
               p[PL_RGBA_GREEN] < p[PL_RGBA_GREEN + sizeof(PLPixel32)] &&
               p[PL_RGBA_BLUE] < p[PL_RGBA_BLUE + sizeof(PLPixel32)] ?
               PHOTOMETRIC_MINISBLACK : PHOTOMETRIC_MINISWHITE;
        SetField( tif, TIFFTAG_PHOTOMETRIC,   ui16 );
      }
      break;

    case 8:
      ui16 = PHOTOMETRIC_PALETTE;
      SetField( tif, TIFFTAG_PHOTOMETRIC,    ui16 );
      break;

    case 32:
      ui16 = PHOTOMETRIC_RGB;
      SetField( tif, TIFFTAG_PHOTOMETRIC,    ui16 );
      break;

    default:
      PLASSERT(false);
  }

  return 1; // should reflect the successful directory initialisation
}


// Set field in directory.

int PLTIFFEncoder::SetField( TIFF* tif, int tag_id, ... )
{
  int retv;
  va_list marker;

  va_start( marker, tag_id );     /* Initialize variable arguments. */
  retv = TIFFVSetField( tif, tag_id, marker );
  va_end( marker );               /* Reset variable arguments.      */

  return retv;
}


/*
/--------------------------------------------------------------------
|
|      $Log: pltiffenc.cpp,v $
|      Revision 1.1  2009/12/09 05:33:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:08  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:00  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:07  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:26  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.6  2002/01/27 18:20:16  uzadow
|      Updated copyright message; corrected pcx decoder bug.
|
|      Revision 1.5  2001/10/17 13:19:47  uzadow
|      Added support for photoshop tiff alpha channels.
|
|      Revision 1.4  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.14  2001/09/13 20:43:02  uzadow
|      Added tiff compression support.
|
|      Revision 1.13  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.12  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.11  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.10  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.9  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.8  1999/12/10 01:27:26  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.7  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.6  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.5  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
