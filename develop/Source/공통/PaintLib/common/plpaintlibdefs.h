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
|      $Id: plpaintlibdefs.h,v 1.1 2009/12/09 05:33:53 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! Contains common datatype definitions.
//!

#ifndef INCL_PLPAINTLIBDEFS
#define INCL_PLPAINTLIBDEFS

typedef unsigned char       PLBYTE;
typedef unsigned int        PLUINT;
typedef unsigned short      PLWORD;         // This is 16 bit.
typedef long                PLLONG;         // This is 32 bit.
typedef unsigned long       PLULONG;

#ifndef _WINDOWS
#include <stdarg.h>

// max & min definitions
#include <algorithm> // STL

#ifndef NULL
#define NULL    0
#endif

#endif //#ifndef _WINDOWS


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpaintlibdefs.h,v $
|      Revision 1.1  2009/12/09 05:33:53  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
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
|      Revision 1.4  2002/02/24 13:00:22  uzadow
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
|      Revision 1.3  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.2  2000/11/09 22:23:36  uzadow
|      no message
|
|      Revision 1.1  2000/11/07 15:33:44  jmbuena
|      Windows type defines for non windows systems
|
|
\--------------------------------------------------------------------
*/
