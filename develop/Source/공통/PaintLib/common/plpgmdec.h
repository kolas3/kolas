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
|      $Id: plpgmdec.h,v 1.1 2009/12/09 05:33:53 박종운 Exp $
|
|      portable graymap Decoder Class
|
|      Original author: Jose Miguel Buenaposada Biencinto.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPGMDEC
#define INCL_PLPGMDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#include "plpgm.h"

//! Portable Graymap file decoder. Delivers an 8 bpp grayscale image.
class PLPGMDecoder : public PLPicDecoder
{

public:
  //! Creates a decoder
  PLPGMDecoder
    ();

  //! Destroys a decoder
  virtual ~PLPGMDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //!
  virtual void GetImage (PLBmp & Bmp);

private:
  void readPgmHeader (PGMHEADER *pPgmHead,
	                    PLDataSource * pDataSrc);

  int  readASCIIDecimal(PLDataSource * pDataSrc);

  void skipComment(PLDataSource * pDataSrc);

  void skipPgmASCIISeparators(PLDataSource * pDataSrc);

  PLBYTE *readASCIILine(PLDataSource *pDataSrc);

  void readImage (PLBmp * pBmp,
                  PLDataSource * pDataSrc);

  void readData(PLBmp * pBmp,
                PLDataSource * pDataSrc);

  void expandASCIILine
    ( PLBYTE * pDest,
      int MaxGrayValue,
      int Width,
      PLDataSource * pDataSrc
    );

  void expandByteLine
    ( PLBYTE * pDest,
	    int MaxGrayValue,
	    int Width,
      PLDataSource * pDataSrc
    );

  PLBYTE PLPGMDecoder::readASCIIPixel8
    ( int MaxGrayValue,
      PLDataSource * pDataSrc
    );

  PLBYTE m_LastByte;
  bool m_UseLastByte;
  PGMHEADER m_PGMHeader;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpgmdec.h,v $
|      Revision 1.1  2009/12/09 05:33:53  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:17  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:56  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:05  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:32  황정영
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
|      Revision 1.4  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.3  2000/05/23 10:19:11  Ulrich von Zadow
|      Minor unix compatibility changes.
|
|      Revision 1.2  2000/03/21 10:51:16  Ulrich von Zadow
|      Documentation update.
|
|      Revision 1.1  2000/03/16 13:56:37  Ulrich von Zadow
|      Added pgm decoder by Jose Miguel Buenaposada Biencinto
|
|
\--------------------------------------------------------------------
*/
