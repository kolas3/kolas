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
|      $Id: pltiffenc.h,v 1.1 2009/12/09 05:33:59 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLTIFFENC
#define INCL_PLTIFFENC

#ifndef INCL_PLPICENC
#include "plpicenc.h"
#endif

typedef struct tiff TIFF;

//! TIFF file encoder. Uses LIBTIFF to do the actual conversion.
class PLTIFFEncoder : public PLPicEncoder
{

public:
  //! Creates an encoder
  PLTIFFEncoder
    ();

  //! Destroys a encoder
  virtual ~PLTIFFEncoder
    ();

  void SetCompression (PLWORD Compression);

protected:
  //! Sets up LIBTIFF environment and calls LIBTIFF to encode an image.
  virtual void DoEncode
    ( PLBmp* pBmp,
      PLDataSink* pDataSnk
    );

  //! this one mostly useful for the "extended" decoder
  virtual void DoTiffEncode
    ( PLBmp* pBmp,
      TIFF* tif
    );

  //! According to the characteristics of the given bitmap,
  //! set the baseline tags
  int SetBaseTags( TIFF*, PLBmp* );

  //! Set tag value in directory; also, "pseudo-tags" are interpreted by
  //! LIBTIFF as modifiers to certain algorithms (compression....).
  //! We do _not_ support multiple images by file, nor tiling nor stripping.
  int SetField( TIFF*, int tag_id, ... );

private:
  PLWORD m_Compression;
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: pltiffenc.h,v $
|      Revision 1.1  2009/12/09 05:33:59  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:46  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:08  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:00  박우람
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
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.4  2002/02/24 13:00:41  uzadow
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
|      Revision 1.7  2001/09/13 20:43:02  uzadow
|      Added tiff compression support.
|
|      Revision 1.6  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|
\--------------------------------------------------------------------
*/
