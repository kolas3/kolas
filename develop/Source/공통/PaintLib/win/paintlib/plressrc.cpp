/*
/--------------------------------------------------------------------
|
|      $Id: plressrc.cpp,v 1.1 2009/12/09 05:34:47 박종운 Exp $
|      Resource Data Source Class
|
|      This is a class which takes a windows resource as a source of
|      picture data.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include "plressrc.h"
#include "plexcept.h"
#include "plpaintlibdefs.h"

#include <stdio.h>

PLResourceSource::PLResourceSource
    ()
  : PLDataSource (NULL),  // No progress notification nessesary when
                         // reading from resources.
    m_hRsrc (NULL),
    m_hGlobal (NULL),
    m_pCurPos (NULL)
{
}

PLResourceSource::~PLResourceSource
    ()
{
  if (m_hGlobal)
    Close();
}

int PLResourceSource::Open (HINSTANCE lh_ResInst, int ResourceID, const char * pResType)
{
  if (!pResType)
    pResType= RT_RCDATA;

  m_hRsrc = FindResource (lh_ResInst, MAKEINTRESOURCE (ResourceID),
                          pResType);
  if (!m_hRsrc)
  {
    char sz[256];
    sprintf (sz, "FindResource Failed (Code %i).\n", GetLastError());
    PLTRACE (sz);
    return PL_ERRFILE_NOT_FOUND;
  }

  m_hGlobal = LoadResource (lh_ResInst, m_hRsrc);
  if (!m_hGlobal)
  {
    char sz[256];
    sprintf (sz, "LoadResource Failed (Code %i).\n", GetLastError());
    PLTRACE (sz);
    return PL_ERRFILE_NOT_FOUND;
  }

  PLBYTE * pBuffer;
  pBuffer = (PLBYTE *)LockResource (m_hGlobal);
  if (!pBuffer)
  {
    PLTRACE ("LockResource failed.\n");
    return (PL_ERRFILE_NOT_FOUND);
  }
  int DataLen = ::SizeofResource (lh_ResInst, m_hRsrc);

  // We've got the resource mapped to memory
  char sz[256];
  sprintf (sz, "Resource: %i", ResourceID);
  PLDataSource::Open (sz, DataLen);
  m_pCurPos = pBuffer;
  return 0;
}

void PLResourceSource::Close
    ()
{
  PLDataSource::Close();
  UnlockResource(m_hGlobal);
  FreeResource(m_hGlobal);
  m_hGlobal = NULL;
}

PLBYTE * PLResourceSource::ReadNBytes
    ( int n
    )
{
  PLDataSource::ReadNBytes(n);
  m_pCurPos += n;
  return m_pCurPos-n;
}

PLBYTE * PLResourceSource::ReadEverything
    ()
{
  return m_pCurPos;  // ;-)
}

// Read but don't advance file pointer.
PLBYTE * PLResourceSource::GetBufferPtr
    ( int MinBytesInBuffer
    )
{
  return m_pCurPos;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plressrc.cpp,v $
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
|      Revision 1.1  2006/11/29 03:34:46  황정영
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
|      Revision 1.8  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.7  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.6  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.5  2000/01/16 20:43:17  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/11 22:07:11  Ulrich von Zadow
|      Added instance handle parameter.
|
|      Revision 1.3  1999/11/02 21:20:06  Ulrich von Zadow
|      AfxFindResourceHandle statt AfxGetInstanceHandle
|      verwendet.
|
\--------------------------------------------------------------------
*/
