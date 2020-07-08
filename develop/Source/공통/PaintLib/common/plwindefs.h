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
|      $Id: plwindefs.h,v 1.1 2009/12/09 05:33:59 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! Windows Bitmap Struct Definitions
//!
//! Contains the definitions for the data structures used in
//! windows bitmap files. For windows apps, they are already
//! defined - but not with these names.
//!
//! The WINRGBQUAD, WINBITMAPFILEHEADER, WINBITMAPINFOHEADER,
//! and WINBITMAPINFO structs are defined as RGBQUAD etc. in
//! the windows header files. See the windows documentation
//! for details.

#ifndef INCL_PLWINDEFS
#define INCL_PLWINDEFS

typedef struct tagWINRGBQUAD
{
  PLBYTE    rgbBlue;
  PLBYTE    rgbGreen;
  PLBYTE    rgbRed;
  PLBYTE    rgbReserved;
} WINRGBQUAD;

#pragma pack(2)
typedef struct tagWINBITMAPFILEHEADER
{
  PLWORD    bfType;
  PLLONG    bfSize;
  PLWORD    bfReserved1;
  PLWORD    bfReserved2;
  PLLONG    bfOffBits;
} WINBITMAPFILEHEADER;
#pragma pack()

typedef struct tagWINBITMAPINFOHEADER
{
  PLLONG   biSize;
  PLLONG   biWidth;
  PLLONG   biHeight;
  PLWORD   biPlanes;
  PLWORD   biBitCount;
  PLLONG   biCompression;
  PLLONG   biSizeImage;
  PLLONG   biXPelsPerMeter;
  PLLONG   biYPelsPerMeter;
  PLLONG   biClrUsed;
  PLLONG   biClrImportant;
} WINBITMAPINFOHEADER;

typedef struct tagWINBITMAPINFO
{
  WINBITMAPINFOHEADER bmiHeader;
  WINRGBQUAD          bmiColors[1];
} WINBITMAPINFO;

typedef struct tagWINBITMAPCOREHEADER
{
  PLLONG    bcSize; 
  PLWORD    bcWidth; 
  PLWORD    bcHeight; 
  PLWORD    bcPlanes; 
  PLWORD    bcBitCount; 
} WINBITMAPCOREHEADER;

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plwindefs.h,v $
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
|      Revision 1.3  2002/02/24 13:00:42  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.9  2001/01/14 15:32:21  uzadow
|      Unix compatibility changes.
|
|      Revision 1.8  2000/12/13 23:40:16  uzadow
|      no message
|
|      Revision 1.7  2000/11/02 21:27:50  uzadow
|      Temporary build fix.
|
|      Revision 1.6  2000/10/31 11:27:47  jmbuena
|      Moved win definitions from stdpch.h
|
|      Revision 1.5  2000/10/28 11:59:46  uzadow
|      no message
|
|      Revision 1.4  2000/07/07 13:20:47  Ulrich von Zadow
|      Bugfix: Added #pragma pack to WINBITMAPFILEHEADER.
|
|      Revision 1.3  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
