/*
|
|      $Id: plfilesink.cpp,v 1.1 2009/12/09 05:33:50 박종운 Exp $
|      File "Data Sink" Class
|
|      This class a file as a destination for picture data.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

// not quite ready for prime-time; bdelmee; 2/99
// this is a very simple-minded implementation suitable for
// the needs of the TIFF encoder at this point

#include "plfilesink.h"
#include "plexcept.h"


PLFileSink::PLFileSink ()
: PLDataSink (),
  m_pFile (NULL),
  m_pDataBuf (NULL)
{
}

PLFileSink::~PLFileSink ()
{
  if (m_pFile)
    Close();
}

// Generic code assuming memory mapped files are not available.
int PLFileSink::Open (const char * pszFName, int MaxFileSize)
{

  // we could actually open the file in "Close()",
  // but if e.g we cannot create it, it's pointless to proceed
  if ((m_pFile = fopen (pszFName, "wb")) &&
      (m_pDataBuf = new PLBYTE [MaxFileSize]))
  {
    PLDataSink::Open(pszFName, m_pDataBuf, MaxFileSize);
    return 0;
  }
  else
    return -1;
}

// now flush the data to disk
void PLFileSink::Close ()
{
  int towrite = GetDataSize();
  int written = fwrite( m_pStartData, 1, towrite, m_pFile );
  PLASSERT( written == towrite );
  fclose( m_pFile );
  m_pFile = 0;

  if (m_pDataBuf)
  {
    delete[] m_pDataBuf;
    m_pDataBuf = NULL;
  }

  PLDataSink::Close();
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilesink.cpp,v $
|      Revision 1.1  2009/12/09 05:33:50  박종운
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
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.7  2000/12/20 19:17:46  uzadow
|      FileSink::Close() now calls the base class Close() so
|      the file sink can be reopened.
|
|      Revision 1.6  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.5  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
