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
|      $Id: plpcx.h,v 1.1 2009/12/09 05:33:53 박종운 Exp $
|
|      Defines structures and constants present in PCX files.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPCX
#define INCL_PLPCX

// PCX Image File
typedef struct tagPCXHEADER
{
  char Manufacturer;  // always 0X0A
  char Version;  // version number
  char Encoding;  // always 1
  char BitsPerPixel;  // color bits
  PLWORD Xmin, Ymin;  // image origin
  PLWORD Xmax, Ymax;  // image dimensions
  PLWORD Hres, Vres;  // resolution values
  PLBYTE ColorMap[16][3];  // color palette
  char Reserved;
  char ColorPlanes;  // color planes
  PLWORD BytesPerLine;  // line buffer size
  PLWORD PaletteType;  // grey or color palette
  char Filter[58];
}
*LPPCXHEADER;

#define PCX_MAGIC 0X0A  // PCX magic number
#define PCX_256_COLORS 0X0C  // magic number for 256 colors
#define PCX_HDR_SIZE 128  // size of PCX header
#define PCX_MAXCOLORS 256
#define PCX_MAXPLANES 4
#define PCX_MAXVAL 255

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plpcx.h,v $
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
|      Revision 1.3  2002/02/24 13:00:22  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.3  1999/12/15 21:16:30  Ulrich von Zadow
|      Removed references to gif in pcx decoder.
|
|
\--------------------------------------------------------------------
*/
