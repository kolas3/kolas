/*
/--------------------------------------------------------------------
|
|      $Id: planydec.cpp,v 1.1 2009/12/09 05:33:47 박종운 Exp $
|      Picture Decoder Class
|
|      Class which decodes pictures with any known format. It auto-
|      detects the format to use and delegates the work to one of the
|      other decoder classes.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "planydec.h"

#include "config.h"
#include "plbitmap.h"
#include "plexcept.h"

// needed for BMP recognition even when this format is not decoded
#include "plwindefs.h"

#ifdef PL_SUPPORT_BMP
#include "plbmpdec.h"
#endif

#ifdef PL_SUPPORT_PICT
#include "plpictdec.h"
#endif

#ifdef PL_SUPPORT_TGA
#include "pltgadec.h"
#endif

#ifdef PL_SUPPORT_TIFF
#include "pltiffdec.h"
#endif

#ifdef PL_SUPPORT_JPEG
#include "pljpegdec.h"
#endif

#ifdef PL_SUPPORT_PNG
#include "plpngdec.h"
#endif

#ifdef PL_SUPPORT_WEMF
#include "plwemfdec.h"
#endif

#ifdef PL_SUPPORT_PCX
#include "plpcxdec.h"
#endif

#ifdef PL_SUPPORT_PGM
#include "plpgmdec.h"
#endif

#ifdef PL_SUPPORT_GIF
#include "plgifdec.h"
#endif

#ifdef PL_SUPPORT_PPM
#include "plppmdec.h"
#endif

#ifdef PL_SUPPORT_PSD
#include "plpsddec.h"
#endif

PLAnyPicDecoder::PLAnyPicDecoder ()
  : PLPicDecoder(),
    m_Type(PL_FT_UNKNOWN)
// Creates a decoder
{
  // Create sub-decoders...
  // KOL.UDF.022 시큐어코딩 보완
  m_Type = -1;
  m_pCurDec = NULL;
  m_pBmpDec = NULL;
  m_pPictDec = NULL;
  m_pTGADec = NULL;
  m_pTIFFDec = NULL;
  m_pJPEGDec = NULL;
  m_pPNGDec = NULL;
  m_pWEMFDec = NULL;
  m_pPCXDec = NULL;
  m_pPGMDec = NULL;
  m_pGIFDec = NULL;
  m_pPPMDec = NULL;
  m_pPSDDec = NULL;

#ifdef PL_SUPPORT_BMP
  m_pBmpDec = new PLBmpDecoder ();
#endif
#ifdef PL_SUPPORT_TGA
  m_pTGADec = new PLTGADecoder ();
#endif
#ifdef PL_SUPPORT_TIFF
  m_pTIFFDec = new PLTIFFDecoder ();
#endif
#ifdef PL_SUPPORT_JPEG
  m_pJPEGDec = new PLJPEGDecoder ();
#endif
#ifdef PL_SUPPORT_PICT
#ifdef PL_SUPPORT_JPEG
  m_pPictDec = new PLPictDecoder (m_pJPEGDec);
#else
  m_pPictDec = new PLPictDecoder (NULL);
#endif
#endif
#ifdef PL_SUPPORT_PNG
  m_pPNGDec = new PLPNGDecoder ();
#endif
#ifdef PL_SUPPORT_WEMF
  m_pWEMFDec = new PLWEMFDecoder();
#endif
#ifdef PL_SUPPORT_PCX
  m_pPCXDec = new PLPCXDecoder();
#endif
#ifdef PL_SUPPORT_PGM
  m_pPGMDec = new PLPGMDecoder();
#endif
#ifdef PL_SUPPORT_GIF
  m_pGIFDec = new PLGIFDecoder();
#endif
#ifdef PL_SUPPORT_PPM
  m_pPPMDec = new PLPPMDecoder();
#endif
#ifdef PL_SUPPORT_PSD
  m_pPSDDec = new PLPSDDecoder();
#endif
}


PLAnyPicDecoder::~PLAnyPicDecoder ()
{
#ifdef PL_SUPPORT_BMP
  delete m_pBmpDec;
#endif
#ifdef PL_SUPPORT_PICT
  delete m_pPictDec;
#endif
#ifdef PL_SUPPORT_TGA
  delete m_pTGADec;
#endif
#ifdef PL_SUPPORT_TIFF
  delete m_pTIFFDec;
#endif
#ifdef PL_SUPPORT_JPEG
  delete m_pJPEGDec;
#endif
#ifdef PL_SUPPORT_PNG
  delete m_pPNGDec;
#endif
#ifdef PL_SUPPORT_WEMF
  delete m_pWEMFDec;
#endif
#ifdef PL_SUPPORT_PCX
  delete m_pPCXDec;
#endif
#ifdef PL_SUPPORT_PGM
  delete m_pPGMDec;
#endif
#ifdef PL_SUPPORT_GIF
  delete m_pGIFDec;
#endif
#ifdef PL_SUPPORT_PPM
  delete m_pPPMDec;
#endif
#ifdef PL_SUPPORT_PSD
  delete m_pPSDDec;
#endif
}

void PLAnyPicDecoder::Open (PLDataSource * pDataSrc)
{
  m_Type = PL_FT_UNKNOWN;
#ifdef PL_SUPPORT_WEMF
  // If we have support for WMF and EMF, we check only the
  // file extension and let Windows do the rest
  char* strname = strupr(strdup(pDataSrc->GetName()));
  if (strname == NULL)
  {
    raiseError (PL_ERRNO_MEMORY,"Out of memory during strdup.");
  }
  strupr(strname);
  if (strstr(strname,".WMF") != NULL)
  {
    m_Type = PL_FT_WMF;
  }
  else if (strstr(strname,".EMF") != NULL)
  {
    m_Type = PL_FT_EMF;
  }
  ::free(strname);
#endif

  if (m_Type == PL_FT_UNKNOWN)
  {
    m_Type = getFileType (pDataSrc->GetBufferPtr (512),
                          pDataSrc->GetFileSize());
  }

  switch (m_Type)
  {
    case PL_FT_UNKNOWN:
      raiseError (PL_ERRUNKNOWN_FILE_TYPE, "Unknown file type.");
#ifdef PL_SUPPORT_BMP
    case PL_FT_WINBMP:
      Trace (2, "Windows bitmap recognized.\n");
      m_pCurDec = m_pBmpDec;
      break;
#endif
#ifdef PL_SUPPORT_PICT
    case PL_FT_MACPICT:
      Trace (2, "Mac PICT recognized.\n");
      m_pCurDec = m_pPictDec;
      break;
#endif
#ifdef PL_SUPPORT_TGA
    case PL_FT_TARGA:
      Trace (2, "TGA file recognized.\n");
      m_pCurDec = m_pTGADec;
      break;
#endif
#ifdef PL_SUPPORT_TIFF
    case PL_FT_EPSTIFF:
      Trace (2, "TIFF preview in EPS file recognized.\n");
      // skip eps information
      pDataSrc->ReadNBytes (epsLongVal(20+pDataSrc->GetBufferPtr(30)));
      m_pCurDec = m_pTIFFDec;
      break;
    case PL_FT_TIFF:
      Trace (2, "TIFF file recognized.\n");
      m_pCurDec = m_pTIFFDec;
      break;
#endif
#ifdef PL_SUPPORT_JPEG
    case PL_FT_JPEG:
      Trace (2, "JPEG file recognized.\n");
      m_pCurDec = m_pJPEGDec;
      break;
#endif
#ifdef PL_SUPPORT_PNG
    case PL_FT_PNG:
      Trace (2, "PNG file recognized.\n");
      m_pCurDec = m_pPNGDec;
      break;
#endif
#ifdef PL_SUPPORT_WEMF
    case PL_FT_WMF:
      Trace (2, "WMF file recognized.\n");
      m_pCurDec = m_pWEMFDec;
      break;
    case PL_FT_EMF:
      Trace (2, "EMF file recognized.\n");
      m_pCurDec = m_pWEMFDec;
      break;
#endif
#ifdef PL_SUPPORT_PCX
    case PL_FT_PCX:
      Trace (2, "PCX file recognized.\n");
      m_pCurDec = m_pPCXDec;
      break;
#endif
#ifdef PL_SUPPORT_GIF
    case PL_FT_GIF:
      Trace (2, "GIF file recognized.\n");
      m_pCurDec = m_pGIFDec;
      break;
#endif
#ifdef PL_SUPPORT_PGM
    case PL_FT_PGM:
      Trace (2, "PGM file recognized.\n");
      m_pCurDec = m_pPGMDec;
      break;
#endif
#ifdef PL_SUPPORT_PPM
    case PL_FT_PPM:
      Trace (2, "PPM file recognized.\n");
      m_pCurDec = m_pPPMDec;
      break;
#endif
#ifdef PL_SUPPORT_PSD
    case PL_FT_PSD:
      Trace (2, "PSD file recognized.\n");
      m_pCurDec = m_pPSDDec;
      break;
#endif
    default:
      raiseError (PL_ERRFORMAT_NOT_COMPILED,
                  "Library not compiled for this file type.");
      break;
  }
  m_pCurDec->Open (pDataSrc);
  SetBmpInfo (*m_pCurDec);
  m_pCurDec->SetDataSrc (pDataSrc);
}

void PLAnyPicDecoder::GetImage (PLBmp & Bmp)
{
  m_pCurDec->GetImage (Bmp);
  SetBmpInfo (*m_pCurDec);
}

void PLAnyPicDecoder::Close ()
{
  if (m_pDataSrc)
  {
    m_pCurDec->Close ();
    m_pDataSrc = 0;
  }
}


int PLAnyPicDecoder::GetFileFormat ()
{
  return m_Type;
}

/////////////////////////////////////////////////////////////////////
// Local functions

int PLAnyPicDecoder::getFileType (PLBYTE * pData, int DataLen)
// Check for file-format-specific data & return the file type if
// something fits.
{
  // Check for bitmap file signature: First 2 bytes are 'BM'.
  if ((pData[0] == 0x42 && pData[1] == 0x41) ||
      (pData[0] == 0x42 && pData[1] == 0x4d) ||
      (pData[0] == 0x49 && pData[1] == 0x43) ||
      (pData[0] == 0x43 && pData[1] == 0x49) ||
      (pData[0] == 0x43 && pData[1] == 0x50))
    return PL_FT_WINBMP;

  // Check for TGA file. The TGA format doesn't have a signature,
  // so the program checks for a meaningful TGA header.
  bool bCouldBeTGA = true;
  if (*(pData+1) > 1)
    bCouldBeTGA = false;
  PLBYTE TGAImgType = *(pData+2);
  if ((TGAImgType > 11) || (TGAImgType > 3 && TGAImgType < 9))
    bCouldBeTGA = false;
  PLBYTE TGAColMapDepth = *(pData+7);
  if (TGAColMapDepth != 8 && TGAColMapDepth != 15 &&
      TGAColMapDepth != 16 && TGAColMapDepth != 24 &&
      TGAColMapDepth != 32 && TGAColMapDepth != 0)
    bCouldBeTGA = false;
  PLBYTE TGAPixDepth = *(pData+16);
  if (TGAPixDepth != 8 && TGAPixDepth != 15 &&
      TGAPixDepth != 16 && TGAPixDepth != 24 &&
      TGAPixDepth != 32)
    bCouldBeTGA = false;
  if (bCouldBeTGA)
    return PL_FT_TARGA;

  // Check for GIF
  PLULONG GIFSig = *((PLULONG *)pData);
  if (GIFSig == 0x38464947)
    return PL_FT_GIF;

  // Check for TIFF
  PLULONG TIFFSig = *((PLULONG *)pData);
  if (TIFFSig == 0x002A4949 || TIFFSig == 0x2A004D4D)
    return PL_FT_TIFF;

  // Check for Mac PICT signature and Version.
  if (DataLen > 540)
  {
    PLBYTE * pPictSig = (PLBYTE *)(pData+0x20a);
    if ((pPictSig[0] == 0x00 && pPictSig[1] == 0x11 &&
         pPictSig[2] == 0x02 && pPictSig[3] == 0xFF) ||
        (pPictSig[0] == 0x00 && pPictSig[1] == 0x11 &&
         pPictSig[2] == 0x01) ||
        (pPictSig[0] == 0x11 && pPictSig[1] == 0x01 &&
         pPictSig[2] == 0x01 && pPictSig[3] == 0x00))
      return PL_FT_MACPICT;
  }

  // Check for JPEG/JFIF.
  if ((*pData == 0xFF) && (*(pData+1) == 0xD8) &&
      (*(pData+2) == 0xFF))
    return PL_FT_JPEG;

  // Check for PNG.
  if ((*pData == 0x89) && (*(pData+1) == 0x50) &&
      (*(pData+2) == 0x4E) && (*(pData+3) == 0x47))
    return PL_FT_PNG;

  // Check for TIFF wrapped in EPS
  PLULONG EPSSig = *((PLULONG *)pData);
  if ((EPSSig == 0xc6d3d0c5 || EPSSig == 0xc5d0d3c6) &&
      *(PLULONG *)(pData+20) && *(PLULONG *)(pData+24))
    return PL_FT_EPSTIFF;

  // Check for PCX
  // This check isn't really safe... should find other criteria.
  if (pData[0] == 0x0A && pData[2] == 0x01)
    return PL_FT_PCX;

  // Check for PGM
  if (pData[0] == 0x50 && ((pData[1] == 0x32)||(pData[1] == 0x35)))
    return PL_FT_PGM;

  // Check for PPM
  if (pData[0] == 0x50 && ((pData[1] == 0x33)||(pData[1] == 0x36)))
    return PL_FT_PPM;

  // Check for PSD
  if (!strncmp ((char *)pData, "8BPS", 4))
    return PL_FT_PSD;

  return PL_FT_UNKNOWN;
}


long PLAnyPicDecoder::epsLongVal (unsigned char *p)
{
  unsigned long retval = 0;
  int i;
  // this may look like an endian dependency but its not - EPS headers
  // are always this way round
  for (i = 0; i < 4; i++)
    retval = ((retval >> 8) & 0xffffffL) + (((long)*p++) << 24);
  return (long) retval;
}

/*
/--------------------------------------------------------------------
|
|      $Log: planydec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:56  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:19  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.11  2002/11/06 21:54:49  uzadow
|      cygwin build stuff, removed LSB_FIRST.
|
|      Revision 1.10  2002/11/02 23:03:03  uzadow
|      Fixed BmpInfo handling for transparent gifs
|
|      Revision 1.9  2002/08/07 18:50:38  uzadow
|      More cygwin compatibility changes.
|
|      Revision 1.8  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.7  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.6  2002/03/06 22:46:54  uzadow
|      Fixed major PLAnyDec bug
|
|      Revision 1.5  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.4  2001/10/16 17:51:32  uzadow
|      Added ppm support (Todd Harris)
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/05 21:15:09  uzadow
|      Improved support for OS/2 bitmap decoding.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.15  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.14  2001/01/15 12:09:02  uzadow
|      Fixed autodetection bug introduced yesterday.
|
|      Revision 1.13  2001/01/14 13:36:15  uzadow
|      Added PLAnyPicDecoder::GetFileFormat()
|
|      Revision 1.12  2000/12/08 12:32:00  uzadow
|      Added gif decoder by Michael Salzlechner.
|
|      Revision 1.11  2000/07/11 17:11:00  Ulrich von Zadow
|      Added support for RGBA pixel ordering (Jose Miguel Buenaposada Biencinto).
|
|      Revision 1.10  2000/03/31 11:53:29  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.9  2000/03/16 13:56:37  Ulrich von Zadow
|      Added pgm decoder by Jose Miguel Buenaposada Biencinto
|
|      Revision 1.8  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.7  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.6  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.5  1999/10/21 17:43:08  Ulrich von Zadow
|      Added pcx support by Meng Bo.
|
|      Revision 1.4  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
