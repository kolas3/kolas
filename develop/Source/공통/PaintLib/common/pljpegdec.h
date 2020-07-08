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
|      $Id: pljpegdec.h,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLJPEGDEC
#define INCL_PLJPEGDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#include <stdio.h>

extern "C"
{
#include "jpeglib.h"
}

//! JPEG file decoder. Uses the independent JPEG group's library
//! to do the actual conversion.
class PLJPEGDecoder : public PLPicDecoder
{


public:
  //! Creates a decoder
  PLJPEGDecoder
    ();

  //! Destroys a decoder
  ~PLJPEGDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the image. 
  virtual void GetImage (PLBmp & Bmp);

  //! true (the default) selects fast but sloppy decoding.
  void SetFast
    ( bool bFast
    );

  // Jo Hagelberg 15.4.99
  // this must be static as it's passed to extern C
  //! callback for jpeglib's progress notification
  static void JNotification
    (j_common_ptr cinfo
    );

private:
  // Assumes IJPEG decoder is already set up.
  void decodeGray
    ( PLBmp * pBmp);

  // Assumes IJPEG decoder is already set up.
  void decodeRGB
    ( PLBmp * pBmp);

  jpeg_decompress_struct cinfo;  // IJPEG decoder state.
  jpeg_error_mgr         jerr;   // Custom error manager.

  bool  m_bFast;  // true selects fast but sloppy decoding.
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pljpegdec.h,v $
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
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.7  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.6  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.4  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|
\--------------------------------------------------------------------
*/
