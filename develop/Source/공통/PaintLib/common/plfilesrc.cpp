/*
/--------------------------------------------------------------------
|
|      $Id: plfilesrc.cpp,v 1.1 2009/12/09 05:33:50 박종운 Exp $
|      File Data Source Class
|
|      This is a class which takes a file as a source of picture data.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include "plfilesrc.h"
#include "plexcept.h"


PLFileSource::PLFileSource
    ( PLIProgressNotification * pNotification
    )
  : PLDataSource (pNotification),
#ifdef PL_FILE_MAPPING
    m_hf (INVALID_HANDLE_VALUE),    // File handle.
    m_hm (NULL),    // Handle to file-mapping object.
#else
    m_pFile (NULL),
    m_pBuffer (NULL),
    m_pReadPos (NULL),
    m_BytesReadFromFile(0),
#endif
    m_pStartData (NULL),
    m_pCurPos (NULL)
{
}

PLFileSource::~PLFileSource
    ()
{
#ifdef PL_FILE_MAPPING
  if (m_hf)
    Close();
#else
  if (m_pFile)
    Close();
#endif
}

int PLFileSource::Open
    ( const char * pszFName
    )
{
  int FileSize;

#ifdef PL_FILE_MAPPING
  PLBYTE * pBuffer = NULL;
  try
  {
    m_hf = CreateFile (pszFName, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if (m_hf == INVALID_HANDLE_VALUE)
    {
      m_hf = NULL;
      switch (GetLastError())
      {
        case ERROR_PATH_NOT_FOUND:
          // sprintf (sz, "Path not found.", pszFName);
          return PL_ERRPATH_NOT_FOUND;
        case ERROR_FILE_NOT_FOUND:
          // sprintf (sz, "File not found.", pszFName);
          return PL_ERRFILE_NOT_FOUND;
        case ERROR_ACCESS_DENIED:
          // sprintf (sz, "Access denied.", pszFName);
          return PL_ERRACCESS_DENIED;
        case ERROR_SHARING_VIOLATION:
          // sprintf (sz, "Sharing violation.", pszFName);
          return PL_ERRACCESS_DENIED;
        default:
          // sprintf (sz, "CreateFile returned %d.",
          //          pszFName, GetLastError());
          return PL_ERRFILE_NOT_FOUND;
      }
    }

    FileSize = ::GetFileSize (m_hf, NULL);

    m_hm = CreateFileMapping (m_hf, NULL, PAGE_READONLY, 0, 0, NULL);

    // This happens if the file is empty.
    if (m_hm == NULL)
    {  // raiseError (PL_ERRACCESS_DENIED, "CreateFileMapping failed.");
      if (m_hf) CloseHandle (m_hf);
      m_hf = NULL;
      return PL_ERRACCESS_DENIED;
    }

    pBuffer = (PLBYTE *) MapViewOfFile (m_hm, FILE_MAP_READ, 0, 0, 0);

    if (pBuffer == NULL)
      // raiseError (PL_ERRACCESS_DENIED, "MapViewOfFile failed.");
      return PL_ERRACCESS_DENIED;
    m_pStartData = pBuffer;
    m_pCurPos = pBuffer;

    // We've got the file mapped to memory.
    PLDataSource::Open (pszFName, FileSize);
  }
  catch (PLTextException)
  {
    // Clean up on error
    if (pBuffer) UnmapViewOfFile (pBuffer);
    if (m_hm) CloseHandle (m_hm);
    if (m_hf) CloseHandle (m_hf);
    throw;
  }
  return 0;

#else

  // Generic code assuming memory mapped files are not available.
  m_pFile = NULL;
  if (strcmp (pszFName, ""))
    m_pFile = fopen (pszFName, "rb");

  if (m_pFile == NULL)
  { // Crude...
    m_pFile = 0;
    return -1;
  }

  // Determine file size. Can this be done in an easier way using ANSI C?
  fseek (m_pFile, 0, SEEK_END);
  FileSize = ftell (m_pFile);
  fseek (m_pFile, 0, SEEK_SET);

  // Create a buffer for the file.
  m_pBuffer = new PLBYTE[FileSize];
  //  this only works if our implementation of "new" does not throw...
  if (m_pBuffer == 0)
  {
    fclose (m_pFile);
    return -1;
  }

  m_pReadPos = m_pBuffer;
  m_pCurPos = m_pBuffer;
  m_BytesReadFromFile = 0;
  PLDataSource::Open (pszFName, FileSize);
  fillBuffer ();
  return 0;
#endif
}

void PLFileSource::Close
    ()
{
#ifdef PL_FILE_MAPPING
  UnmapViewOfFile (m_pStartData);
  PLDataSource::Close ();
  CloseHandle (m_hm);
  CloseHandle (m_hf);
  m_hm = NULL;
  m_hf = NULL;
#else
  //Note that 'new' is used to get the memory, not alloc, so don't use free
  delete [] m_pBuffer;
  m_pBuffer = NULL;
  PLDataSource::Close ();
  fclose (m_pFile);
  m_pFile = NULL;
#endif
}

PLBYTE * PLFileSource::ReadNBytes
    ( int n
    )
{
  PLDataSource::ReadNBytes(n);
#ifndef PL_FILE_MAPPING
  if (!bytesAvailable(n))
    fillBuffer(n);
#endif
  m_pCurPos += n;
  return m_pCurPos-n;
}

//! Read but don't advance file pointer.
PLBYTE * PLFileSource::GetBufferPtr
    ( int MinBytesInBuffer
    )
{
  PLASSERT (MinBytesInBuffer < 4096);
#ifndef PL_FILE_MAPPING
  if (!bytesAvailable(MinBytesInBuffer))
    fillBuffer();
#endif
  return m_pCurPos;
}

PLBYTE * PLFileSource::ReadEverything
    ()
{
#ifdef PL_FILE_MAPPING
  return m_pCurPos;
#else
  int BytesToRead = GetFileSize()-m_BytesReadFromFile;
  int i = fread (m_pReadPos, 1, BytesToRead, m_pFile);
  PLASSERT (i==BytesToRead);
  m_BytesReadFromFile += BytesToRead;
  m_pReadPos += BytesToRead;
  return m_pCurPos;
#endif
}


#ifndef PL_FILE_MAPPING
void PLFileSource::fillBuffer
    ( int n
    )
{
  // bdelmee; code change for the sake of portability
  int BytesToRead = GetFileSize() - m_BytesReadFromFile;
  if ( BytesToRead > n )
    BytesToRead = n;
  int i = fread (m_pReadPos, 1, BytesToRead, m_pFile);
  PLASSERT (i==BytesToRead);
  m_BytesReadFromFile += BytesToRead;
  m_pReadPos += BytesToRead;
}

bool PLFileSource::bytesAvailable
    ( int n
    )
{
  if (m_pReadPos-m_pCurPos >= n)
    return true;
  else
    return false;
}
#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plfilesrc.cpp,v $
|      Revision 1.1  2009/12/09 05:33:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:58  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:54  박우람
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
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.10  2001/09/13 20:46:45  uzadow
|      Removed 4096-byte limit for fillBuffer that was causing PLPNGEncoder
|      to fail under Linux.
|
|      Revision 1.9  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.8  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.7  2000/09/01 13:27:07  Administrator
|      Minor bugfixes
|
|      Revision 1.6  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.4  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
