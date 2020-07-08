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
|      $Id: plppmdec.h,v 1.1 2009/12/09 05:33:56 박종운 Exp $
|
|      Copyright (c) 2001 Ulrich von Zadow
|
|      Portable Pixmap Decoder Class
|
|      Originally by Todd Harris - Mar 2001
|      adadapted from pgm.h originally by
|			 Jose Miguel Buenaposada Biencinto. Mar 2000.
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPPMDEC
#define INCL_PLPPMDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#ifndef INCL_PLPPM
#include "plppm.h"
#endif

//! Portable Pixmap file decoder. Delivers a 24 bit true color image.
class PLPPMDecoder : public PLPicDecoder
{

public:
  //! Creates a decoder
  PLPPMDecoder
    ();

  //! Destroys a decoder
  virtual ~PLPPMDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //!
  virtual void GetImage (PLBmp & Bmp);

private:

  void readPpmHeader (PPMHEADER *pPpmHead,
	                    PLDataSource * pDataSrc);

  int  readASCIIDecimal(PLDataSource * pDataSrc);

  void skipComment(PLDataSource * pDataSrc);

  void skipPpmASCIISeparators(PLDataSource * pDataSrc);

  PLBYTE *readASCIILine(PLDataSource *pDataSrc);

  void readImage (PLBmp * pBmp,
                  PLDataSource * pDataSrc);

  void readData(PLBmp * pBmp,
                PLDataSource * pDataSrc);

  void expandASCIILine
    ( PLPixel32 * pDest,
      int MaxGrayValue,
      int Width,
      PLDataSource * pDataSrc
    );

  void expandByteLine
    ( PLPixel32 * pDest,
	    int MaxGrayValue,
	    int Width,
      PLDataSource * pDataSrc
    );


  PLPixel32 PLPPMDecoder::readASCIIPixel32
    ( int MaxGrayValue,
      PLDataSource * pDataSrc
    );

  PPMHEADER m_PPMHead;
  PLBYTE m_LastByte;
  bool m_UseLastByte;
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plppmdec.h,v $
|      Revision 1.1  2009/12/09 05:33:56  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:05  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:25  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.3  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.2  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.1  2001/10/16 17:51:32  uzadow
|      Added ppm support (Todd Harris)
|
|
\--------------------------------------------------------------------
*/
