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
|      $Id: plpngdec.h,v 1.1 2009/12/09 05:33:55 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#ifndef INCL_PLPNGDEC
#define INCL_PLPNGDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#ifndef INCL_PLPNG
#include "png.h"
#endif



//! PNG file decoder. Uses LibPng to do the actual decoding.
//! PNG supports many pixel formats not supported by paintlib. These
//! pixel formats are converted to the nearest paintlib equivalent.
//! Images with less or more than 8 bits per channel are converted to 
//! 8 bits per channel. Images with 16-bit palettes or grayscale 
//! images with an alpha channel are returned as full 32-bit RGBA 
//! bitmaps.
class PLPNGDecoder : public PLPicDecoder
{


public:
  //! Creates a decoder
  PLPNGDecoder
    ();

  //! Destroys a decoder
  virtual ~PLPNGDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the image. 
  virtual void GetImage (PLBmp & Bmp);

private:

  static void PLPNGDecoder::user_error_fn
    ( png_structp png_ptr,
      png_const_charp error_msg
    );

  static void PLPNGDecoder::user_warning_fn
    ( png_structp png_ptr,
      png_const_charp warning_msg
    );

  int m_color_type, m_bit_depth;
  png_structp m_png_ptr;
  png_infop m_info_ptr;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpngdec.h,v $
|      Revision 1.1  2009/12/09 05:33:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:03  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
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
|      Revision 1.5  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.4  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|
--------------------------------------------------------------------
*/
