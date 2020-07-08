/*
|
|      $Id: plmemsink.cpp,v 1.1 2009/12/09 05:33:52 박종운 Exp $
|      Memory "Data Sink" Class
|
|      This class uses memory as a destination for picture data.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include "plmemsink.h"
#include "plexcept.h"
#include "plpaintlibdefs.h"
#include "plexcept.h"

PLMemSink::PLMemSink ()
  : PLDataSink(),    
    m_pDataBuf (NULL)
{

}

PLMemSink::~PLMemSink ()
{  
  Close();
}

int PLMemSink::Open (const char * pszFName, int MaxFileSize)
{
  if (m_pDataBuf = new PLBYTE [MaxFileSize])
  {
    PLDataSink::Open(pszFName, m_pDataBuf, MaxFileSize);
    return 0;
  }
  else
    return -1;
}

void PLMemSink::Close ()
{
  if (m_pDataBuf)
  {
    delete [] m_pDataBuf;
    m_pDataBuf = NULL;
  }

  PLDataSink::Close();
}

PLBYTE* PLMemSink::GetBytes()
{
	if (m_pDataBuf)
		return m_pDataBuf;
	else
		throw PLTextException(PL_ERRINTERNAL, "Open was not called for CMemSink");
}


/*
/--------------------------------------------------------------------
|
|      $Log: plmemsink.cpp,v $
|      Revision 1.1  2009/12/09 05:33:52  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:59  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:55  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:04  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:22  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:26  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:31  황정영
|      2006.05.17
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.2  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.1  2000/10/12 21:57:26  uzadow
|      no message
|
|
|
\--------------------------------------------------------------------
*/
