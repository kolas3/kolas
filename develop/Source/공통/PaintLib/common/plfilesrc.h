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
|      $Id: plfilesrc.h,v 1.1 2009/12/09 05:33:50 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLFILESRC
#define INCL_PLFILESRC

#ifndef INCL_PLDATASRC
#include "pldatasrc.h"
#endif

#ifdef _WINDOWS
#define PL_FILE_MAPPING
#endif

#include <stdio.h>

class PLIProgressNotification;

//! This is a class which takes a file as a source of picture data.
class PLFileSource : public PLDataSource
{

public:
  //!
  PLFileSource
    ( PLIProgressNotification * pNotification = NULL
    );

  //!
  virtual ~PLFileSource
    ();

  //!
  virtual int Open
    ( const char * pszFName
    );

  //!
  virtual void Close
    ();

  virtual PLBYTE * ReadNBytes
    ( int n
    );

  //! Read but don't advance file pointer.
  virtual PLBYTE * GetBufferPtr
    ( int MinBytesInBuffer
    );

  virtual PLBYTE * ReadEverything
    ();

private:
#ifdef PL_FILE_MAPPING
  HANDLE m_hf;    // File handle.
  HANDLE m_hm;    // Handle to file-mapping object.

#else
  bool bytesAvailable
    ( int n
    );

  void fillBuffer
    ( int n = 4096
    );

  FILE * m_pFile;
  PLBYTE * m_pBuffer;

  PLBYTE * m_pReadPos;
  int m_BytesReadFromFile;
#endif
  PLBYTE * m_pStartData;
  PLBYTE * m_pCurPos;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plfilesrc.h,v $
|      Revision 1.1  2009/12/09 05:33:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:58  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:55  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:04  황정영
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
|      Revision 1.4  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.2  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/09/13 20:46:45  uzadow
|      Removed 4096-byte limit for fillBuffer that was causing PLPNGEncoder
|      to fail under Linux.
|
|
\--------------------------------------------------------------------
*/
