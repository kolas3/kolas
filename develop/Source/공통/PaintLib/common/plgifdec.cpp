/*
/--------------------------------------------------------------------
|
|      $Id: plgifdec.cpp,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|
|      TODO: Error handling.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|      Initial version by Michael Salzlechner
|
\-------------------------------------------------------------------
*/

#include "plstdpch.h"

#include "plgifdec.h"
#include "planybmp.h"

#include <stdio.h>

extern "C"
{
#define DOEXTERNGIF

// Workaround for a namespace clash of libgif with windows. This namespace clash
// is also the reason m_GifFile is declared void* in the header: Having an
// #undef DrawText in a file that might be included by paintlib clients is
// unacceptable.
#ifdef _WINDOWS
#undef DrawText
#endif
#include "gif_lib.h"
}

static int
    InterlacedOffset[] = { 0, 4, 2, 1 }, /* The way Interlaced image should. */
    InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PLGIFDecoder::PLGIFDecoder()
  : PLPicDecoder()
{
  m_pGifFile = 0;
//  SetFileExtensions("*.gif");
//  SetFileDescription("GIF File");
}

PLGIFDecoder::~PLGIFDecoder()
{

}

int GIF_Read_Data(GifFileType* pGifFile, GifByteType* pByteType, int length)
{
  PLBYTE *ptr;
  PLDataSource* pSourceInfo=(PLDataSource*)pGifFile->UserData;

  ptr = pSourceInfo->ReadNBytes(length);
  wmemcpy(pByteType,ptr,length);

  return length;
}

void PLGIFDecoder::Open (PLDataSource * pDataSrc)
{
  PLASSERT (m_pGifFile == 0);
  GifFileType * pGifFile= DGifOpen( (void*)pDataSrc, GIF_Read_Data);
  m_pGifFile = (void*)pGifFile;
  SetBmpInfo (PLPoint (pGifFile->SWidth, pGifFile->SHeight), 8, PLPoint (0,0), 0,0);
}

void PLGIFDecoder::GetImage (PLBmp & Bmp)
{
  GifFileType * pGifFile = (GifFileType *)m_pGifFile;
  GifRecordType RecordType;
  GifByteType *Extension;
  ColorMapObject *ColorMap = NULL;

  bool bTransparent = false;
  int iTransparentIndex = -1;

  int CurLine = 0;
  int i, Row, Col, Width, Height, Count, j, ExtCode;

  Bmp.Create (*this);

  PLBYTE ** pLineArray = Bmp.GetLineArray();

  CurLine = 0;
  while (CurLine < pGifFile->SHeight)
  {
    pLineArray[CurLine][0] = pGifFile->SBackGroundColor;
    CurLine++;
  }

  /* Scan the content of the GIF file and load the image(s) in: */
  do
  {
    if (DGifGetRecordType(pGifFile, &RecordType) == GIF_ERROR)
    {
      PrintGifError();
      PLASSERT (false);
    }

    switch (RecordType)
    {
      case IMAGE_DESC_RECORD_TYPE:
        if (DGifGetImageDesc(pGifFile) == GIF_ERROR)
        {
          PrintGifError();
          PLASSERT (false);
        }
        Row = pGifFile->Image.Top; /* Image Position relative to Screen. */
        Col = pGifFile->Image.Left;
        Width = pGifFile->Image.Width;
        Height = pGifFile->Image.Height;
        if (pGifFile->Image.Left + pGifFile->Image.Width > pGifFile->SWidth ||
            pGifFile->Image.Top + pGifFile->Image.Height > pGifFile->SHeight)
        {
          fprintf(stderr, "Image %d is not confined to screen dimension, aborted.\n");
          PLASSERT (false);
        }
        if (pGifFile->Image.Interlace)
        {
          /* Need to perform 4 passes on the images: */
          for (Count = i = 0; i < 4; i++)
            for (j = Row + InterlacedOffset[i]; j < Row + Height;
                 j += InterlacedJumps[i])
            {
              GifQprintf("\b\b\b\b%-4d", Count++);
              if (DGifGetLine(pGifFile, &pLineArray[j][Col], Width) == GIF_ERROR)
              {
                PrintGifError();
                PLASSERT (false);
              }
            }
        }
        else
        {
          for (i = 0; i < Height; i++)
          {
            GifQprintf("\b\b\b\b%-4d", i);
            if (DGifGetLine(pGifFile, &pLineArray[Row++][Col], Width) == GIF_ERROR)
            {
              PrintGifError();
              PLASSERT (false);
            }
          }
        }
        RecordType = TERMINATE_RECORD_TYPE;
        break;
      case EXTENSION_RECORD_TYPE:
        // Check for transparency, skip any other extension records.
        if (DGifGetExtension(pGifFile, &ExtCode, &Extension) == GIF_ERROR)
        {
          PrintGifError();
          PLASSERT (false);
        }
        if(Extension == NULL)
          break;
        do
        {
          if(ExtCode == GRAPHICS_EXT_FUNC_CODE)
          {
            if((Extension[1] & 1) == 1)
            {
              bTransparent = true;
              iTransparentIndex = Extension[4];
            }
          }
          if (DGifGetExtensionNext(pGifFile, &Extension) == GIF_ERROR)
          {
            PrintGifError();
            PLASSERT (false);
          }
        } while(Extension != NULL);
        break;
      case TERMINATE_RECORD_TYPE:
        break;
      default:        /* Should be trapped by DGifGetRecordType. */
        break;
    }
  } while (RecordType != TERMINATE_RECORD_TYPE);

  ColorMap = (pGifFile->Image.ColorMap ? pGifFile->Image.ColorMap :
              pGifFile->SColorMap);

  Bmp.SetHasAlpha(bTransparent);

  // Make sure the transparency information gets copied to the decoder.
  SetBmpInfo(Bmp);  

  PLBYTE* pb = (PLBYTE*)ColorMap->Colors;
  for (i = 0; i < ColorMap->ColorCount; i++)
  {
    Bmp.SetPaletteEntry(i, pb[0], pb[1], pb[2], i == iTransparentIndex ? 0:255);
    pb += 3;
  }
}

void PLGIFDecoder::Close ()
{
  DGifCloseFile((GifFileType*)m_pGifFile);
  m_pGifFile = 0;
  PLPicDecoder::Close();
}

/*
/--------------------------------------------------------------------
|
|      $Log: plgifdec.cpp,v $
|      Revision 1.1  2009/12/09 05:33:51  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:58  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:19  박우람
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
|      Revision 1.8  2002/11/18 14:44:53  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.7  2002/11/02 20:33:27  uzadow
|      Added gif transparency handling (Mark Richarme)
|
|      Revision 1.6  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.5  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.4  2002/03/16 15:23:15  uzadow
|      Fixed minor gif decoder bug.
|
|      Revision 1.3  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.6  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.5  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.4  2001/01/15 12:09:34  uzadow
|      Fixed crash decoding some gif files.
|
|      Revision 1.3  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.2  2000/12/17 20:45:57  uzadow
|      Fixed build problem with libungif.
|
|      Revision 1.1  2000/12/08 13:41:23  uzadow
|      Added gif support by Michael Salzlechner.
|
|
\--------------------------------------------------------------------
*/
