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
|      $Id: qdraw.h,v 1.1 2009/12/09 05:33:59 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! This file defines the data structures used in pict files. They
//! correspond to the appropriate MAC QuickDraw structs. See
//! QuickDraw docs for an explanation of these structs.

#ifndef INCL_PLQDRAW
#define INCL_PLQDRAW

struct MacRect
{
  PLWORD top;
  PLWORD left;
  PLWORD bottom;
  PLWORD right;
};

struct MacpixMap
{
  // Ptr baseAddr              // Not used in file.
  // short rowBytes            // read in seperatly.
  struct MacRect Bounds;
  PLWORD version;
  PLWORD packType;
  PLLONG packSize;
  PLLONG hRes;
  PLLONG vRes;
  PLWORD pixelType;
  PLWORD pixelSize;
  PLWORD cmpCount;
  PLWORD cmpSize;
  PLLONG planeBytes;
  PLLONG pmTable;
  PLLONG pmReserved;
};

struct MacRGBColour
{
  PLWORD red;
  PLWORD green;
  PLWORD blue;
};

struct MacPoint
{
  PLWORD x;
  PLWORD y;
};

struct MacPattern // Klaube
{
  PLBYTE pix[64];
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: qdraw.h,v $
|      Revision 1.1  2009/12/09 05:33:59  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:47  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:08  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:01  박우람
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
|      Revision 1.6  2002/02/24 13:00:42  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.5  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
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
