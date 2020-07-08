/*
/--------------------------------------------------------------------
|
|      $Id: plStreamSink.cpp,v 1.1 2009/12/09 05:34:46 박종운 Exp $
|      Copyright (c) 1996-2000 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "ole2.h"
#include "plStreamSink.h"
#include "plpaintlibdefs.h"

PLStreamSink::PLStreamSink()
  : PLDataSink(),
    m_pIStream (NULL),
    m_pDataBuf (NULL),
	  m_hMem (NULL )
{
}

PLStreamSink::~PLStreamSink()
{
  if (m_pDataBuf)
  {
    GlobalUnlock(m_hMem);
    m_pDataBuf = NULL;
  }

  if (m_hMem)
  {
    m_hMem = GlobalFree(m_hMem);
  }

  if (m_pIStream)
  {
    m_pIStream->Release();
    m_pIStream = NULL;
  }
}

int PLStreamSink::Open (int MaxFileSize)
{
	// Create the stream
	m_hMem = GlobalAlloc(GMEM_MOVEABLE, MaxFileSize);
  if (m_hMem == 0)
    throw PLTextException (PL_ERRNO_MEMORY, "PLStreamSink::Open - Error allocating memory.\n");

  if ((m_pDataBuf = (PLBYTE*)GlobalLock(m_hMem))==NULL)
    throw PLTextException (PL_ERRINTERNAL, "PLStreamSink::Open - Error locking memory.\n");
  PLDataSink::Open("HGlobalStream", m_pDataBuf, MaxFileSize);
  return 0;

/*
  if ((CreateStreamOnHGlobal(hMem, true, &m_pIStream) == S_OK) &&
    (m_pDataBuf = new PLBYTE [MaxFileSize]))
  {
    PLDataSink::Open("HGlobalStream", m_pDataBuf, MaxFileSize);
    return 0;
  }
  else
    throw PLTextException (PL_ERRINTERNAL, "Error creating HGlobalStream.\n");
*/
}

// now flush the data to the stream
void PLStreamSink::Close ()
{
  GlobalUnlock(m_hMem);
  m_pDataBuf = NULL;

  // by setting fDeleteOnRelease to true the stream owns the global memory block
  // and we don't have to call GlobalUnalloc.
  // So we can safely set m_hMem to 0.
  if (CreateStreamOnHGlobal(m_hMem, true, &m_pIStream) != S_OK)
    throw PLTextException (PL_ERRINTERNAL, "Error creating HGlobalStream.\n");
  m_hMem = 0;

  // Truncate the stream
  ULARGE_INTEGER uli_size; // Size of the written data, not of the author of paintlib
  uli_size.LowPart = GetDataSize();
  uli_size.HighPart = 0;
  m_pIStream->SetSize(uli_size);

  // reset pointer to beginning of stream
  LARGE_INTEGER li_NULL;
  li_NULL.LowPart = 0;
  li_NULL.HighPart = 0;
  m_pIStream->Seek(li_NULL,STREAM_SEEK_SET,NULL);
  PLDataSink::Close();
}


IStream * PLStreamSink::GetIStream()
{
  return m_pIStream;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plStreamSink.cpp,v $
|      Revision 1.1  2009/12/09 05:34:46  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:06  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:44  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:58  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.1  2000/09/01 14:19:46  Administrator
|      no message
|
|
\--------------------------------------------------------------------
*/
