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
|      $Id: pljpegenc.h,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLJPEGENC
#define INCL_PLJPEGENC

#ifndef INCL_PLPICENC
#include "plpicenc.h"
#endif

struct jpeg_compress_struct;
struct jpeg_error_mgr;

//! JPEG file encoder. Uses the independent JPEG group's library
//! to do the actual conversion.
class PLJPEGEncoder : public PLPicEncoder
{

public:
  //! Creates an encoder
  PLJPEGEncoder
    ();

  //! Destroys a encoder
  ~PLJPEGEncoder
    ();

  // todo: Notification not yet implemented for encoding - ms 3.6.99
  /*
  static void JNotification
    (j_common_ptr cinfo
    );
  */

  //! Set the compression quality on a scale from 0 to 100.
  void SetQuality(int iQuality);
  //! Enable or disable the generation of optimal Huffmann coding tables.
  void SetOptimizeCoding(bool bOptimizeCoding);
  //! Set the smoothing factor (<=100). 0 turns it off.
  void SetSmoothingFactor(int iSmoothingFactor);
  //! Set the resolution information (DPI) for the image.
  void SetDensity(unsigned int uiX, unsigned int uiY);

protected:
  // Sets up the jpeg library data source and error handler and
  // calls the jpeg encoder.
  void DoEncode
    ( PLBmp * pBmp,
      PLDataSink * pDataSink
    );

private:
  jpeg_compress_struct * m_pcinfo;  // IJPEG encoder state.
  jpeg_error_mgr       * m_pjerr;   // Custom error manager.

  int iQuality_;
  bool bOptimizeCoding_;
  int iSmoothingFactor_;
  unsigned int uiDensityX_;
  unsigned int uiDensityY_;

  // Assumes IJPEG decoder is already set up.
  void encodeRGB
    ( PLBmp * pBmp,
      int iScanLines
    );

};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: pljpegenc.h,v $
|      Revision 1.1  2009/12/09 05:33:51  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:16  박대우
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
|      Revision 1.3  2002/02/24 13:00:21  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/05/22 17:43:25  Ulrich von Zadow
|      Added SetQuality(), SetDensity(), SetOptimizeCoding() and
|      SetSmoothingFactor().
|
|      Revision 1.4  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.2  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.1  1999/10/19 21:28:05  Ulrich von Zadow
|      Added jpeg encoder
|
|
\--------------------------------------------------------------------
*/
