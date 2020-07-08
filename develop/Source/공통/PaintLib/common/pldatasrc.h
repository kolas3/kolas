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
|      $Id: pldatasrc.h,v 1.1 2009/12/09 05:33:49 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLDATASRC
#define INCL_PLDATASRC

#include "plexcept.h"
#include "plpaintlibdefs.h"

class PLIProgressNotification;

//! This is a base class for a source of picture data.
//! It defines methods to open, close, and read from data sources.
//! Does byte-order-conversions in the ReadByte, ReadWord, and
//! ReadLong routines.
class PLDataSource : public PLObject
{

public:

  //!
  PLDataSource
    ( PLIProgressNotification * pNotification = NULL
    );

  //!
  virtual ~PLDataSource
    ();

  //!
  virtual void Open
    ( const char * pszName,
      int    FileSize
    );

  //!
  virtual void Close
    ();

  //!
  char * GetName
    ();

  //! Read but don't advance file pointer.
  virtual PLBYTE * GetBufferPtr
    ( int MinBytesInBuffer
    ) = 0;

  //! This needs to be overridden in derived classes.
  virtual PLBYTE * ReadNBytes
    ( int n
    );

  //!
  int GetFileSize
    ();

  //! This is a legacy routine that interferes with progress notifications.
  //! Don't call it!
  virtual PLBYTE * ReadEverything
    () = 0;

  //!
  PLBYTE * Read1Byte
    ();

  //!
  PLBYTE * Read2Bytes
    ();

  //!
  PLBYTE * Read4Bytes
    ();

  // JH 15.4.99:
  //! handles progress notification from other libs
  void OProgressNotification
    ( double part
    );

  //!
  void AlignToWord
    ();

  //!
  void Skip
    ( int n
    );

  //! Test to see if we didn't go past the end of the file
  void CheckEOF
    ();

protected:

private:
  char * m_pszName;        // Name of the data source for diagnostic
                           // purposes.
  int    m_FileSize;
  int    m_BytesRead;
  bool   m_bSrcLSBFirst;   // Source byte order: true for intel order,
                           // false for Motorola et al. (MSB first).
  PLIProgressNotification * m_pNotification;
};


inline int PLDataSource::GetFileSize
    ()
{
  return m_FileSize;
}

inline PLBYTE * PLDataSource::Read1Byte
    ()
{
  return ReadNBytes (1);
}


inline PLBYTE * PLDataSource::Read2Bytes
    ()
{
  return ReadNBytes (2);
}


inline PLBYTE * PLDataSource::Read4Bytes
    ()
{
  return ReadNBytes (4);
}

inline void PLDataSource::AlignToWord
    ()
{
  if (m_BytesRead & 1)
    Read1Byte();
}


inline void PLDataSource::Skip
    ( int n
    )
{
  ReadNBytes (n);
}

inline void PLDataSource::CheckEOF
    ()
{

  if (m_FileSize < m_BytesRead)
  {
    throw PLTextException (PL_ERREND_OF_FILE,
          "End of file reached while decoding.\n");
  }
}

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pldatasrc.h,v $
|      Revision 1.1  2009/12/09 05:33:49  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:14  박대우
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
|      Revision 1.3  2002/02/24 13:00:21  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/10/24 22:59:34  uzadow
|      no message
|
|
\--------------------------------------------------------------------
*/
