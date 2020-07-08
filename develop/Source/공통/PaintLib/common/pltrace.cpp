/*
/--------------------------------------------------------------------
|
|      $Id: pltrace.cpp,v 1.1 2009/12/09 05:33:59 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/
#include "plstdpch.h"

#ifdef _DEBUG

#include <stdarg.h>
#include <stdio.h>

void PLTrace(const char * pszFormat, ...)
{
  va_list args;
  va_start(args, pszFormat);

  int nBuf;
  char szBuffer[4096];

  nBuf = vsprintf(szBuffer, pszFormat, args);
  PLASSERT(nBuf < 4096);

#ifndef _WINDOWS
  fprintf (stderr, szBuffer);
#else
  ::OutputDebugString (szBuffer);
#endif //_WINDOWS

  va_end(args);
}

#endif //_DEBUG

/*
/--------------------------------------------------------------------
|
|      $Log: pltrace.cpp,v $
|      Revision 1.1  2009/12/09 05:33:59  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:47  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:08  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:00  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:07  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:27  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.6  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.5  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.3  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
