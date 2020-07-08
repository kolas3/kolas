/*The zlib/libpng License

Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/
/*
/--------------------------------------------------------------------
|
|      $Id: pltgadec.h,v 1.1 2009/12/09 05:33:58 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLTGADEC
#define INCL_PLTGADEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#ifndef INCL_PLTGA
#include "pltga.h"
#endif

//! Targa file decoder. Decodes 8, 15, 16, 24 and 32 bpp
//! targa files (compressed and uncompressed) and returns an 8 or 32
//! bpp CBitmap. Preserves the alpha channel.
class PLTGADecoder : public PLPicDecoder
{


public:
  //! Creates a decoder
  PLTGADecoder
    ();

  //! Destroys a decoder
  virtual ~PLTGADecoder
    ();

   //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the image. 
  virtual void GetImage (PLBmp & Bmp);

private:

  // Fills TGAHEADER structure with the information in the file.
  void readTgaHeader
    ( TGAHEADER * pTgaHead,       // Pointer to TGA header structure
      PLDataSource * pDataSrc
    );

  // Reads the TGA palette and creates a windows palette.
  void readPalette
    ( int StartIndex,           // Index of first palette entry.
      int Length,               // Number of palette entries stored.
      int EntrySize,            // Size of palette entries in bits.
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );


  // Determines compression type and calls readData.
  void readImage
    ( TGAHEADER * pTgaHead,       // Pointer to TGA header structure
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Reads image data line-by-line.
  void readData
    ( TGAHEADER * pTgaHead,       // Pointer to TGA header structure
      bool bCompressed,
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Decodes one line of uncompressed image data.
  void expandUncompressedLine
    ( PLBYTE * pDest,
      int Width,
      bool bReversed,
      int bpp,
      PLDataSource * pDataSrc
    );

  // Decodes one line of compressed image data.
  void expandCompressedLine
    ( PLBYTE * pDest,
      int Width,
      bool bReversed,
      int bpp,
      PLDataSource * pDataSrc
    );

  // Reads one image pixel and returns it in RGBA format.
  PLPixel32 readPixel32
    ( int bpp,
      PLDataSource * pDataSrc
    );

  // Reads one image pixel and returns it in 8-bit format.
  PLBYTE readPixel8
    ( int bpp,
      PLDataSource * pDataSrc
    );

  TGAHEADER m_TgaHead;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pltgadec.h,v $
|      Revision 1.1  2009/12/09 05:33:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:06  PDJ
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
|      Revision 1.5  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
\--------------------------------------------------------------------
*/
