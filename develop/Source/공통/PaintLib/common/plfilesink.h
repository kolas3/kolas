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
|      $Id: plfilesink.h,v 1.1 2009/12/09 05:33:50 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/
// not quite ready for prime-time; bdelmee; 2/99

#ifndef INCL_PLFILESNK
#define INCL_PLFILESNK

#ifndef INCL_PLDATASNK
#include "pldatasink.h"
#endif

#include <stdio.h>

//! This is a class which takes a file as a destination of picture data.
//!
//! Restriction: needs to allocate a buffer large enough to hold a complete
//! image file. But we don't know its size before it's actually encoded, so
//! we have to be generous or "accidents will happen"
//! A better implementation would rely on memory-mapped files or implement
//! dynamic buffering.
class PLFileSink : public PLDataSink
{

public:
  //!
  PLFileSink
    ();

  //!
  virtual ~PLFileSink
    ();

  //!
  virtual int Open
    ( const char * pszFName,
      int MaxFileSize
    );

  //!
  virtual void Close
    ();

private:
  FILE * m_pFile;
  PLBYTE * m_pDataBuf;
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilesink.h,v $
|      Revision 1.1  2009/12/09 05:33:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:14  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:57  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:54  박우람
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
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.4  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  2000/01/08 15:56:12  Ulrich von Zadow
|      Made sure change logging works in every file.
|
|
\--------------------------------------------------------------------
*/
