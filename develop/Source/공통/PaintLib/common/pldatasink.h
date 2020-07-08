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
|      $Id: pldatasink.h,v 1.1 2009/12/09 05:33:49 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/
// not quite ready for prime-time; bdelmee; 2/99

#ifndef INCL_PLDATASNK
#define INCL_PLDATASNK

#include "plexcept.h"
#include "plpaintlibdefs.h"

#include <stdio.h>

//! This is a base class for a destination of picture data.
//! It defines methods to open, write to, close data sinks.
//!
//! Restriction: actually writes to a memory buffer that needs
//! to be foreseen large enough by user (descendant class)
class PLDataSink : public PLObject
{
  // I'd like to get rid of this, but now it is needed so the encoders
  // can maintain the actually useful length of the buffer
  friend class PLTIFFEncoder;
  friend class PLTIFFEncoderEx;

public:
  //!
  void Open
    ( const char* pszName,
      PLBYTE*   pData,
      size_t  MaxDataSize
    );

  //!
  virtual void Close
    ();

  //!
  char* GetName
    ();

  //!
  PLBYTE* GetBufferPtr
    ();

  //!
  size_t GetDataSize
    ();

  //!
  size_t GetMaxDataSize
    ();

  //!
  size_t WriteNBytes
    ( size_t   n,
	  PLBYTE* pData
    );

  void WriteByte
    ( PLBYTE Data
    );

  //!
  void Skip
    ( size_t n );

  //! Test to see if we didn't go past the "end of the file"
  void CheckEOF
    ();

protected:
  PLBYTE* m_pStartData;
  int   m_nCurPos;   // this SHOULD be an unsigned int (size_t), but TIFFLib wants an int

  //!
  PLDataSink
    ();

  //!
  virtual ~PLDataSink
   ();

private:
  char*  m_pszName;    // Name of the data source for diagnostic purposes
  size_t m_nMaxFileSize;
};


inline PLBYTE * PLDataSink::GetBufferPtr
    ()
{
	return m_pStartData + m_nCurPos;
}

inline size_t PLDataSink::GetMaxDataSize
    ()
{
  return  m_nMaxFileSize;
}

inline size_t PLDataSink::GetDataSize
    ()
{
  // return currently used buffer size
  return m_nCurPos;
}

inline void PLDataSink::CheckEOF
    ()
{
  //if (GetFileSize() > GetMaxFileSize())
  if (m_nCurPos > (int)m_nMaxFileSize)
  {
    throw PLTextException (PL_ERREND_OF_FILE,
          "Buffer overflow while encoding.\n");
  }
}

inline void PLDataSink::Skip
    ( size_t n )
{
	// maybe we should fill the gap with nulls
  m_nCurPos += n;
  CheckEOF ();
}

#endif	// INCL_PLDATASNK
/*
/--------------------------------------------------------------------
|
|      $Log: pldatasink.h,v $
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
|      Revision 1.4  2002/02/24 13:00:20  uzadow
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
|      Revision 1.5  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/08 15:56:12  Ulrich von Zadow
|      Made sure change logging works in every file.
|
|
\--------------------------------------------------------------------
*/
