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
|      $Id: plstdpch.h,v 1.1 2009/12/09 05:33:58 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! Contains most system-specific includes and definitions. On windows
//! systems, it corresponds to stdafx.h. On other systems, the
//! appropriate data types and macros are declared here.

#ifdef INCL_PLSTDPCH
#error This file should only be included by .cpp files and not by headers.
#else
#define INCL_PLSTDPCH

#include "plobject.h"

#ifdef _WINDOWS
  #define VC_EXTRALEAN  // Exclude rarely-used stuff from Windows headers
  #include <windows.h>
#else
  #include <stdlib.h>
#endif

#include "pldebug.h"

#endif // #ifndef _STDPCH_H_

/*
/--------------------------------------------------------------------
|
|      $Log: plstdpch.h,v $
|      Revision 1.1  2009/12/09 05:33:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:06  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:59  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:26  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:44  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.5  2002/11/06 21:54:49  uzadow
|      cygwin build stuff, removed LSB_FIRST.
|
|      Revision 1.4  2002/02/24 13:00:40  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 15:32:22  uzadow
|      Removed types LPBYTE, DWORD, UCHAR, VOID and INT from the code.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.14  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.13  2000/11/07 15:40:46  jmbuena
|      Changes related to paintlibdefs.h and pixeldefs.h
|
|      Revision 1.12  2000/10/31 11:27:04  jmbuena
|      Removed PAINTLIB_INTERNAL and moved win definitions to windefs.h
|
|      Revision 1.11  2000/10/30 13:10:57  jmbuena
|      Only include config.h when PAINTLIB_INTERNAL is defined
|
|      Revision 1.10  2000/10/24 16:46:34  uzadow
|      Fixed build problems
|
|      Revision 1.9  2000/10/24 12:02:24  jmbuena
|      Include recursion protection
|
|      Revision 1.8  2000/05/27 16:34:05  Ulrich von Zadow
|      Linux compatibility changes
|
|      Revision 1.7  2000/03/31 11:53:30  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.6  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/08 15:43:58  Ulrich von Zadow
|      Changed ASSERT and PLASSERT_VALID so that they
|      disappear correctly in release mode.
|
|      Revision 1.4  1999/11/22 14:59:37  Ulrich von Zadow
|      no message
|
|      Revision 1.3  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
