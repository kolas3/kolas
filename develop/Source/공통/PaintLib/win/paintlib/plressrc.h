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
|      $Id: plressrc.h,v 1.1 2009/12/09 05:34:47 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_RESSRC
#define INCL_RESSRC

#ifndef INCL_DATASRC
#include "pldatasrc.h"
#endif
#include "plpaintlibdefs.h"

//! This is a class which takes a windows resource as a source of
//! picture data.
class PLResourceSource : public PLDataSource
{
public:
  //!
  PLResourceSource
    ();

  //!
  virtual ~PLResourceSource
    ();

  //!
  virtual int Open
    ( HINSTANCE lh_ResInst, int ResourceID,
      const char * pResType = NULL
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

  //! This is a legacy routine that interferes with progress notifications.
  //! Don't call it!
  virtual PLBYTE * ReadEverything
    ();

private:
  HRSRC   m_hRsrc;
  HGLOBAL m_hGlobal;
  PLBYTE * m_pCurPos;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plressrc.h,v $
|      Revision 1.1  2009/12/09 05:34:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:23  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.3  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/11 22:07:11  Ulrich von Zadow
|      Added instance handle parameter.
|
|      Revision 1.3  1999/11/02 21:20:14  Ulrich von Zadow
|      AfxFindResourceHandle statt AfxGetInstanceHandle
|      verwendet.
|
|
\--------------------------------------------------------------------
*/
