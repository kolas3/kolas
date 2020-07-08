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
|      $Id: plbmpdec.h,v 1.1 2009/12/09 05:33:48 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBMPDEC
#define INCL_PLBMPDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#ifndef INCL_PLWINDEFS
#include "plwindefs.h"
#endif

//! Windows bitmap file decoder. Decodes bitmap files (compressed and
//! uncompressed) of all bit depths. Returns an 8
//! or 32 bpp bitmap.
class PLBmpDecoder : public PLPicDecoder
{

public:
  //! Creates a decoder
  PLBmpDecoder
    ();

  //! Destroys a decoder
  virtual ~PLBmpDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the main image. 
  virtual void GetImage (PLBmp & Bmp);

  //!
  virtual void Close ();

private:

  // Decodes the bitmap file & info headers
  WINBITMAPINFOHEADER * getInfoHeader
    ( PLDataSource * pDataSrc,
      PLPixel32* Pal
    );

  // Decodes a 2-color bitmap. Ignores the palette & just uses
  // black & white as 'colors'
  void decode1bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    );

  // Decodes an uncompressed 16-color-bitmap.
  void decode4bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    );

  // Decodes an uncompressed 256-color-bitmap.
  void decode8bpp
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    );

  // Decodes a compressed 16-color-bitmap.
  void decodeRLE4
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    );

  // Decodes a compressed 256-color-bitmap.
  void decodeRLE8
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp
    );

  void decodeHiColor
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      WINBITMAPINFOHEADER * pBMI
    );

  // Decodes true-color bitmap
  void decodeTrueColor
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int SrcBPP
    );

  void decode15bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    );

  void decode16bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    );

  void decode24bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    );

  void decode32bppLine
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      PLBYTE * pDest
    );


  // Decodes two 4-bit pixels using a palette & puts them in pDest
  // and pDest+4.
  void decode2Nibbles
    ( PLBYTE * pDest,
      PLBYTE SrcByte
    );

  // Makes a private copy of the palette in the file and sets the
  // alpha channel. Returns the number of colors read.
  int readPalette
    ( WINBITMAPINFOHEADER * pBMI,     // Pointer to bitmapinfoheader in file.
      PLDataSource * pDataSrc,
      PLPixel32 * pPal,
      int RGBSize
    );

  PLPixel32 m_Pal[256];
  WINBITMAPINFOHEADER * m_pBMI;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plbmpdec.h,v $
|      Revision 1.1  2009/12/09 05:33:48  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:57  PDJ
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
|      Revision 1.7  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.6  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.5  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.4  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.3  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.2  2001/10/05 21:15:09  uzadow
|      Improved support for OS/2 bitmap decoding.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
\--------------------------------------------------------------------
*/
