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
|      $Id: plpngenc.h,v 1.1 2009/12/09 05:33:55 ¹ÚÁ¾¿î Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPNGENC
#define INCL_PLPNGENC

#ifndef INCL_PLPICENC
#include "plpicenc.h"
#endif

//! PNG file encoder. Uses LIBPNG to do the actual conversion.
class PLPNGEncoder : public PLPicEncoder
{

public:
  //! Creates an encoder
  PLPNGEncoder
    ();

  //! Destroys a encoder
  virtual ~PLPNGEncoder
    ();

  PLDataSink * GetDataSink
    ();

  //! Sets compression level for PNG, range is 0-9, 9 is hardest compression
  void SetCompressionLevel(unsigned short Level); // Inserted by René Dencker Eriksen (edr@mip.sdu.dk)

protected:

  //! Sets up LIBPNG environment and calls LIBPNG to encode an image.
  virtual void DoEncode
    ( PLBmp* pBmp,
      PLDataSink* pDataSnk
    );

private:
	PLBmp * m_pBmp;
	PLDataSink* m_pDataSnk;
	unsigned short m_CompressionLevel;

};


#endif  // INCL_PLPNGENC
/*
/--------------------------------------------------------------------
|
|      $Log: plpngenc.h,v $
|      Revision 1.1  2009/12/09 05:33:55  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:03  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:30:33  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.4  2002/11/18 14:45:00  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.3  2002/02/24 13:00:38  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  2000/01/08 15:53:12  Ulrich von Zadow
|      Moved several functions to the cpp file so applications don't
|      need the png directory in the include path.
|
|      Revision 1.1  2000/01/04 22:06:17  Ulrich von Zadow
|      Initial version by Neville Richards.
|
|
\--------------------------------------------------------------------
*/
