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
|      $Id: plpicenc.h,v 1.1 2009/12/09 05:33:54 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/
// not quite ready for prime-time; bdelmee; 2/99

#ifndef INCL_PLPICENC
#define INCL_PLPICENC

#ifndef INCL_PLBITMAP
#include "plbitmap.h"
#endif

#ifndef INCL_PLDATASNK
#include "pldatasink.h"
#endif

// the details of the bitmap class are not needed here
class PLBmp;

//! PLPicEncoder is an abstract base class. It defines common routines
//! for all encoders. Encoders for specific file formats can be
//! derived from this class. Objects of this class interact with a
//! PLDataSink to encode bitmaps.
class PLPicEncoder : public PLObject
{


public:
  //! Empty constructor. The actual initialization takes place in a
  //! derived class.
  PLPicEncoder
    ();

  //! Destructor. Frees memory allocated.
  virtual ~PLPicEncoder
    ();

  //! Encodes a bitmap to a file and stores.
  virtual void MakeFileFromBmp(
    const char *,
    PLBmp*
    );

  //! Encodes a picture to a pre-existing data destination (sink).
  virtual void SaveBmp( PLBmp*, PLDataSink* );

  void SetTraceConfig( int Level, char * pszFName );

  // This function is needed by callbacks outside of any object,
  // so it's public and static. It should not be called from
  // outside of the library. - 3.6.99 MS
  static void raiseError( int Code, char * pszErr );

  void Trace( int TraceLevel, const char * pszMessage );

protected:
  //! Implements the actual encoding process. Uses variables local to the
  //! object to retrieve and store the data. Implemented in derived classes.
  //!
  virtual void DoEncode
    ( PLBmp* pBmp,
      PLDataSink* pDataSrc
      ) = 0;
};
#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpicenc.h,v $
|      Revision 1.1  2009/12/09 05:33:54  박종운
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
|      Revision 1.3  2002/02/24 13:00:26  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  2000/01/08 15:51:30  Ulrich von Zadow
|      Misc. modifications to png encoder.
|
|      Revision 1.5  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.4  1999/10/19 21:28:05  Ulrich von Zadow
|      Added jpeg encoder
|
|      Revision 1.3  1999/10/03 18:50:50  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
