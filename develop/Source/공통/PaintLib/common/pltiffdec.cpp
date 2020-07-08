/*
/--------------------------------------------------------------------
|
|      $Id: pltiffdec.cpp,v 1.1 2009/12/09 05:33:58 박종운 Exp $
|      TIFF Decoder Class
|
|      TIFF file decoder. Uses LIBTIFF to do the actual conversion.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "pltiffdec.h"
#include "plexcept.h"

extern "C"
{
#include "tif_msrc.h"
}

char PLTIFFDecoder::m_szLastErr[256];

/////////////////////////////////////////////////////////////////////
//  helper class to setup libtiff callbacks

class _tiff_helper
{
public:
  _tiff_helper()
  {
    // Set up function pointers to error handling routines.
    TIFFSetErrorHandler (PLTIFFDecoder::Win32ErrorHandler);
    TIFFSetWarningHandler (PLTIFFDecoder::Win32WarningHandler);
  }

  // We could restore the internal (default) libtiff handlers,
  // but that is not generally useful
  ~_tiff_helper()
{}

};

static _tiff_helper _the_tiff_helper_singleton;

// Helper stuff to make sure photoshop alpha gets decoded too.
// Copied from tif_getimage.c. If anyone knows how to do this in
// an easier way, please tell me - uz.

typedef unsigned char u_char;

#define REPEAT8(op) REPEAT4(op); REPEAT4(op)
#define REPEAT4(op) REPEAT2(op); REPEAT2(op)
#define REPEAT2(op) op; op
#define CASE8(x,op)     \
    switch (x) {      \
    case 7: op; case 6: op; case 5: op; \
    case 4: op; case 3: op; case 2: op; \
    case 1: op;       \
    }
#define NOP

#define UNROLL8(w, op1, op2) {    \
    uint32 _x;        \
    for (_x = w; _x >= 8; _x -= 8) {  \
  op1;        \
  REPEAT8(op2);     \
    }         \
    if (_x > 0) {     \
  op1;        \
  CASE8(_x,op2);      \
    }         \
}

#define PACK4(r,g,b,a)  \
  ((uint32)(r)|((uint32)(g)<<8)|((uint32)(b)<<16)|((uint32)(a)<<24))

/*
 * 8-bit packed samples => RGBA w/ associated alpha
 * (known to have Map == NULL)
 */
static void putRGBAAcontig8bittile(
    TIFFRGBAImage* img,
    uint32* cp,
    uint32 x, uint32 y,
    uint32 w, uint32 h,
    int32 fromskew, int32 toskew,
    u_char* pp
)
{
    int samplesperpixel = img->samplesperpixel;

    (void) x; (void) y;
    fromskew *= samplesperpixel;
    while (h-- > 0) {
      UNROLL8(w, NOP,
              *cp++ = PACK4(pp[0], pp[1], pp[2], pp[3]);
              pp += samplesperpixel);
  cp += toskew;
  pp += fromskew;
    }
}

/////////////////////////////////////////////////////////////////////
// Class functions

PLTIFFDecoder::PLTIFFDecoder ()
  : PLPicDecoder(),
    m_pTif(0)
// Creates a decoder
{
}


PLTIFFDecoder::~PLTIFFDecoder ()
{
}

void PLTIFFDecoder::Open (PLDataSource * pDataSrc)
{
  PLASSERT (m_pTif == 0);
  m_pTif = TIFFOpenMem (pDataSrc->ReadEverything(), pDataSrc->GetFileSize(), 
                        NULL);
  if (!m_pTif)
    raiseError (PL_ERRWRONG_SIGNATURE, m_szLastErr);

  uint16  BitsPerSample;
  uint16  SamplesPerPixel;
  uint32  ImageHeight;
  uint32  ImageWidth;

  // get tagged image parameters
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_IMAGEWIDTH, &ImageWidth);
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_IMAGELENGTH, &ImageHeight);
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);

  int DestBPP = 32;
  if (!TIFFIsTiled(m_pTif))
  { 
    if (SamplesPerPixel == 1 && BitsPerSample ==1)
      DestBPP = 1;
    else if (SamplesPerPixel < 3 && BitsPerSample <= 8)
      DestBPP = 8;
  }
  
  int16  PhotometricInterpretation;
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_PHOTOMETRIC, &PhotometricInterpretation);
  bool bIsGreyscale = (PhotometricInterpretation == PHOTOMETRIC_MINISWHITE ||
                       PhotometricInterpretation == PHOTOMETRIC_MINISBLACK);
  bool bAlpha = (SamplesPerPixel == 4 && 
                 (bIsGreyscale || PhotometricInterpretation == PHOTOMETRIC_RGB || 
                  PhotometricInterpretation == PHOTOMETRIC_PALETTE));

  SetBmpInfo (PLPoint (ImageWidth, ImageHeight), DestBPP, getResolution (m_pTif), bAlpha, bIsGreyscale);

}

void PLTIFFDecoder::GetImage (PLBmp & Bmp)
{
  uint16  BitsPerSample;
  uint16  SamplePerPixel;

  // get tagged image parameters
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
  TIFFGetFieldDefaulted(m_pTif, TIFFTAG_SAMPLESPERPIXEL, &SamplePerPixel);

  // For the time being, paintlib bitmaps only actually support 8
  // or 32bpp; so the following mapping should cover all cases ...
  if ((SamplePerPixel == 1 || SamplePerPixel == 2) && BitsPerSample <= 8
      &&  !TIFFIsTiled(m_pTif))
    doLoColor(m_pTif, &Bmp);
  else
    // complicated decoding; use higher-level API
    // it will promote all images to 32bpp, though
    doHiColor(m_pTif, &Bmp, SamplePerPixel);
}

void PLTIFFDecoder::Close ()
{
  TIFFClose (m_pTif);
  m_pTif = 0;

  PLPicDecoder::Close();
}

// It'd be nicer to define a bunch of type-safe functions like:
//    uint32  PLTIFFDecoderEx::GetImageLength();
//    CString PLTIFFDecoderEx::GetImageDescription();
int PLTIFFDecoder::GetField( int tag_id, ... )
{
  va_list marker;
  int retv;

  va_start( marker, tag_id );     /* Initialize variable arguments. */
  retv = TIFFVGetFieldDefaulted( m_pTif, (ttag_t) tag_id, marker );
  va_end( marker );              /* Reset variable arguments.      */

  return retv;
}


const PLPoint PLTIFFDecoder::getResolution (TIFF* tif)
{
  float nResX;
  nResX = 0.0;
  TIFFGetField(tif,TIFFTAG_XRESOLUTION,&nResX);
  if(nResX <= 1.0)
    nResX = 0.0;

  float nResY;
  nResY = 0.0;
  TIFFGetField(tif,TIFFTAG_YRESOLUTION,&nResY);
  if(nResY <= 1.0)
    nResY = 0.0;
  uint16 resunit;
  if (!TIFFGetField (tif, TIFFTAG_RESOLUTIONUNIT, &resunit))
    resunit = RESUNIT_INCH;

  if (resunit != RESUNIT_INCH)
  {  // Centimeters...
    nResX /= 39.37f;
    nResY /= 39.37f;
  }

  return PLPoint (int (nResX+0.5), int (nResY+0.5));
}

void PLTIFFDecoder::doHiColor (TIFF * tif, PLBmp * pBmp, uint16 SamplePerPixel)
{
  int ok;
  PLULONG x, y;

  TIFFRGBAImage img;
  char emsg[1024];
  PLBYTE * pBits;

  ok = TIFFRGBAImageBegin(&img, tif, 0, emsg);

  if (ok == 0)
  {
    TIFFClose (tif);
    raiseError (PL_ERRWRONG_SIGNATURE, m_szLastErr);
  }

  bool bHasAlpha = pBmp->HasAlpha();
  try
  {
    PLASSERT (int(img.width) == pBmp->GetWidth());
    PLASSERT (int(img.height) == pBmp->GetHeight());
    PLASSERT (pBmp->GetBitsPerPixel() == 32);
    pBits = new PLBYTE [img.width*img.height*4];
    if (pBits == NULL)
      raiseError (PL_ERRNO_MEMORY, "Out of memory allocating TIFF buffer.");
  }
  catch (PLTextException)
  {
    TIFFClose (tif);
    throw;
  }

  // Hack for photoshop alpha channel support
  if (SamplePerPixel == 4 && img.bitspersample == 8 && img.photometric == 2)
  {
    img.put.contig = putRGBAAcontig8bittile;
  }

  ok = TIFFRGBAImageGet(&img, (uint32 *) pBits, img.width, img.height);
  if (!ok)
  {
    TIFFRGBAImageEnd(&img);
    TIFFClose(tif);
    raiseError (PL_ERRWRONG_SIGNATURE, m_szLastErr);
  }

  pBmp->Lock(false, true);
  PLPixel32 ** pLineArray = pBmp->GetLineArray32();

  // Correct the byte ordering. This could be replaced by appropriate 
  // putRGBAcontig... routines.
  for (y=0; y<img.height; y++)
  {
    PLBYTE * pSrc = pBits+(img.height-y-1)*img.width*4;
    PLPixel32 * pPixel = pLineArray[y];
    for  (x=0; x<img.width; x++)
    {
      pPixel->Set (*pSrc, *(pSrc+1), *(pSrc+2), *(pSrc+3));
      pPixel++;
      pSrc += 4;
    }
  }
  pBmp->Unlock();

  // Clean up.
  delete [] pBits;
  TIFFRGBAImageEnd(&img);
}

/*
 * TIFF decoding for 1, 4 and 8 bit(s) per pixel
 * bdelmee; 10/98
 */

/* check if color map holds old-style 8-bit values */
static int checkcmap(int n, uint16* r, uint16* g, uint16* b)
{
  while (n-- > 0)
    if (*r++ >= 256 || *g++ >= 256 || *b++ >= 256)
      return (16);

  return (8);
}

#define CVT(x)      (((x) * 255L) / ((1L<<16)-1))

void PLTIFFDecoder::doLoColor (TIFF * tif, PLBmp * pBmp)
{
  uint16  BitsPerSample;
  uint16  SamplePerPixel;
  int32  LineSize;
  int16  PhotometricInterpretation;
  int  row;
  PLBYTE  *pBits;

  TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
  TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &SamplePerPixel);
  TIFFGetFieldDefaulted(tif, TIFFTAG_PHOTOMETRIC, &PhotometricInterpretation);

  if (PhotometricInterpretation!=PHOTOMETRIC_MINISWHITE &&
      PhotometricInterpretation!=PHOTOMETRIC_MINISBLACK &&
      PhotometricInterpretation!=PHOTOMETRIC_PALETTE)
  {
    PhotometricInterpretation = PHOTOMETRIC_MINISWHITE;
    Trace(2,"unexpected PhotometricInterpretation, default to PHOTOMETRIC_MINISWHITE");
  }

  LineSize = TIFFScanlineSize(tif); //Number of bytes in one line

  PLPixel32 pPal[256];
  try
  {
    pBits = new PLBYTE [LineSize];
    if (pBits == NULL)
      raiseError (PL_ERRNO_MEMORY, "Out of memory allocating TIFF buffer.");
  }
  catch (PLTextException)
  {
    TIFFClose (tif);
    throw;
  }


  // phase one: build color map

  if /* monochrome (=bitonal) or grayscale */
  (PhotometricInterpretation == PHOTOMETRIC_MINISWHITE ||
      PhotometricInterpretation == PHOTOMETRIC_MINISBLACK)
  {
    int numColors = 1 << BitsPerSample;
    PLBYTE step = 255 / (numColors-1);
    PLBYTE *pb = (PLBYTE *) (pPal);
    int offset = sizeof(PLPixel32);
    if (PhotometricInterpretation == PHOTOMETRIC_MINISWHITE)
    {
      pb += (numColors-1) * sizeof(PLPixel32);
      offset = -offset;
    }
    // warning: the following ignores possible halftone hints
    for (int i = 0; i < numColors; ++i, pb += offset)
    {
      pb[PL_RGBA_RED] = pb[PL_RGBA_GREEN] = pb[PL_RGBA_BLUE] = i * step;
      pb[PL_RGBA_ALPHA] = 255;
    }

  }
  //PhotometricInterpretation = 2 image is RGB
  //PhotometricInterpretation = 3 image has a color palette
  else if (PhotometricInterpretation == PHOTOMETRIC_PALETTE)
  {
    uint16* red;
    uint16* green;
    uint16* blue;
    int16 i, Palette16Bits;

    // we get pointers to libtiff-owned colormaps
    i = TIFFGetField(tif, TIFFTAG_COLORMAP, &red, &green, &blue);

    //Is the palette 16 or 8 bits ?
    Palette16Bits = checkcmap(1<<BitsPerSample, red, green, blue) == 16;

    //load the palette in the DIB
    for (i = 0; i < 1<<BitsPerSample; ++i)
    {
      PLBYTE *pb = (PLBYTE *) ((pPal)+i);
      pb[PL_RGBA_RED  ] = (PLBYTE) (Palette16Bits ? CVT(  red[i]) :   red[i]);
      pb[PL_RGBA_GREEN] = (PLBYTE) (Palette16Bits ? CVT(green[i]) : green[i]);
      pb[PL_RGBA_BLUE ] = (PLBYTE) (Palette16Bits ? CVT( blue[i]) :  blue[i]);
      pb[PL_RGBA_ALPHA] = 255;
    }
  }
  else
    Trace( 2, "unexpected PhotometricInterpretation in PLTIFFDecoder::DoLoColor()" );

  // phase two: read image itself

  //generally, TIFF images are ordered from upper-left to bottom-right
  // we implicitly assume PLANARCONFIG_CONTIG
  pBmp->Lock(false, true);
  PLBYTE **pLineArray = pBmp->GetLineArray();

  if (BitsPerSample > 8)
    Trace( 2, "unexpected bit-depth in PLTIFFDecoder::DoLoColor()" );
  else for ( row = 0; row < GetHeight(); ++row )
    {
      uint16 x;
      int status = TIFFReadScanline( tif, pBits, row, 0 );
      if (status == -1 && row < GetHeight()  / 3)
      {
        delete [] pBits;
        // we should maybe free the BMP memory as well...
        raiseError (PL_ERRINTERNAL, m_szLastErr);
      }
      /*
      if (BitsPerSample == 1)  // go ahead, waste space ;-)
        for (x=0; x < imageWidth; ++x)
          pLineArray[row][x] = pBits[x / 8] & (128 >> (x & 7)) ? 1 : 0;
      else */
      if (BitsPerSample == 4)
      {
        for (x=0; x < GetWidth() / 2; ++x)
        {
          pLineArray[row][2*x  ] = (pBits[x] & 0xf0) >> 4;
          pLineArray[row][2*x+1] = (pBits[x] & 0x0f);
        }
        // odd number of pixels
        if (GetWidth() & 1)
          pLineArray[row][GetWidth()-1] = (pBits[x] & 0xf0) >> 4;
      }
      else //if (BitsPerSample == 8 || BitsPerSample == 1)
        if (SamplePerPixel == 1)
          wmemcpy( pLineArray[row], pBits, LineSize );
        else
        {
          // We've got an 8 bit image with an alpha channel.
          // Ignore the alpha channel.
          PLASSERT (BitsPerSample == 8);
          for (x=0; x < GetWidth(); ++x)
            pLineArray[row][x] = pBits[x*2];
        }
    }
  pBmp->Unlock();

  // propagate colormap
  pBmp->SetPalette (pPal);
  // clean up
  delete [] pBits;
}

/////////////////////////////////////////////////////////////////////
// Static functions used as Callbacks from the TIFF library

void PLTIFFDecoder::Win32ErrorHandler (const char* module, const char* fmt, va_list ap)
{
  int k = vsprintf(m_szLastErr, fmt, ap);
  if (k >= 0) strcat( m_szLastErr + k, "\n" );

  Trace (0, m_szLastErr);
  raiseError (PL_ERRFORMAT_UNKNOWN, m_szLastErr);

  return;
}

void PLTIFFDecoder::Win32WarningHandler (const char* module, const char* fmt, va_list ap)
{
  char szTemp[256];
  char szMessage[256];

  int k = vsprintf(szMessage, fmt, ap);
  if (k >= 0) strcat( szMessage + k, "\n" );

  if (module != NULL)
    sprintf (szTemp, "Warning in LIBTIFF(%s): %s\n", module, szMessage);
  else
    sprintf (szTemp, "Warning in LIBTIFF: %s\n", szMessage);

  Trace (2, szTemp);

  return;
}
/*
/--------------------------------------------------------------------
|
|      $Log: pltiffdec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:06  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:59  박우람
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
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.10  2002/11/02 19:37:41  uzadow
|      Fixed CMYK tiff handling.
|
|      Revision 1.9  2002/08/05 19:06:30  uzadow
|      no message
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/03/03 11:59:43  uzadow
|      Updated to work with cygwin.
|
|      Revision 1.6  2002/02/11 16:45:36  uzadow
|      Fixed bug decoding 16 bit per channel tiffs.
|
|      Revision 1.5  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.4  2001/10/17 13:19:47  uzadow
|      Added support for photoshop tiff alpha channels.
|
|      Revision 1.3  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.13  2001/09/15 14:37:34  uzadow
|      Fixed palette handling for tiff < 4 bpp.
|
|      Revision 1.12  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.11  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.10  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.9  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.8  2000/03/16 13:56:37  Ulrich von Zadow
|      Added pgm decoder by Jose Miguel Buenaposada Biencinto
|
|      Revision 1.7  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.5  2000/01/09 22:24:10  Ulrich von Zadow
|      Corrected tiff callback bug.
|
|      Revision 1.4  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
