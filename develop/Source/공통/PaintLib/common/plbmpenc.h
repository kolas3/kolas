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
|      Windows bitmap file encoder. Encodes 1, 4, 8, 24 and 32 bpp
|      bitmaps to a 1, 4, 8 or 24 bpp uncompressed BMP file
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBMPENC
#define INCL_PLBMPENC

#ifndef INCL_PLPICENC
#include "plpicenc.h"
#endif

class PLBmpEncoder : public PLPicEncoder
{

public:
  //! Creates an encoder
  PLBmpEncoder
    ();

  //! Destroys a encoder
  ~PLBmpEncoder
    ();

  // todo: Notification not yet implemented for encoding - ms 3.6.99
  /*
  static void JNotification
    (j_common_ptr cinfo
    );
  */

protected:
  // encodes in bmp format
  void DoEncode
    ( PLBmp * pBmp,
      PLDataSink * pDataSink
    );

  int GetLineMemNeeded(PLLONG width, PLWORD BitsPerPixel);
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plbmpenc.h,v $
|      Revision 1.1  2009/12/09 05:33:48  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:57  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:53  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:03  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:21  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:25  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:31  황정영
|      2006.05.17
|
|      Revision 1.4  2002/02/24 13:00:18  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/12/08 12:32:00  uzadow
|      Added gif decoder by Michael Salzlechner.
|
|      Revision 1.3  2000/12/02 19:50:01  uzadow
|      Added Logging.
|
\--------------------------------------------------------------------
*/
