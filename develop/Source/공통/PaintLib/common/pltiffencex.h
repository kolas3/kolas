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
|      $Id: pltiffencex.h,v 1.1 2009/12/09 05:33:59 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/
// not quite ready for prime-time; bdelmee; 2/99

#ifndef INCL_PLTIFFENCEX
#define INCL_PLTIFFENCEX

#include "pltiffenc.h"

//! Basic tiff output is available via "PLTIFFEncoder::MakeFileFromBmp()".
//! This does not compress data or allow any informative tag to be set.
//! For those who need greater control, PLTIFFDecoderEx is provided.
class PLTIFFEncoderEx : public PLTIFFEncoder
{

public:
  //! Creates an encoder
  PLTIFFEncoderEx();

  //! Destroys an encoder
  virtual ~PLTIFFEncoderEx();

  //! Bind DataSink to Encoder (the link is the TIFF* member)
  bool Associate( PLDataSink* );

  //! Flush the output
  void Dissociate( void );

  //! SetBaseTabs and SetField make their base class equivalent usable,
  //! without requiring the user to know about the libtiff internals (TIFF*)
  int SetBaseTags( PLBmp* );

  //! SetBaseTabs and SetField make their base class equivalent usable,
  //! without requiring the user to know about the libtiff internals (TIFF*)
  int SetField( int tag_id, ... );

protected:
  //! Sets up LIBTIFF environment and calls LIBTIFF to encode an image.
  virtual void DoEncode( PLBmp*, PLDataSink* = 0 );

private:
  TIFF* m_TiffToken;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pltiffencex.h,v $
|      Revision 1.1  2009/12/09 05:33:59  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:47  박대우
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
|      Revision 1.1  2006/11/29 03:34:27  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.1  1999/10/19 21:30:42  Ulrich von Zadow
|      B. Delmee - Initial revision
|
|
--------------------------------------------------------------------
*/
