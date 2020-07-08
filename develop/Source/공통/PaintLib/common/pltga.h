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
|      $Id: pltga.h,v 1.1 2009/12/09 05:33:58 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! Defines structures and constants present in TGA files.

#ifndef INCL_PLTGA
#define INCL_PLTGA

typedef struct _TgaHeader
{
    PLBYTE   IdLength;            // Image ID Field Length
    PLBYTE   CmapType;            // Color Map Type
    PLBYTE   ImageType;           // Image Type

    PLWORD   CmapIndex;           // First Entry Index
    PLWORD   CmapLength;          // Color Map Length
    PLBYTE   CmapEntrySize;       // Color Map Entry Size

    PLWORD   X_Origin;            // X-origin of Image
    PLWORD   Y_Origin;            // Y-origin of Image
    PLWORD   ImageWidth;          // Image Width
    PLWORD   ImageHeight;         // Image Height
    PLBYTE   PixelDepth;          // Pixel Depth
    PLBYTE   ImagDesc;            // Image Descriptor
} TGAHEADER;

// Definitions for image types.
#define TGA_Null 0
#define TGA_Map 1
#define TGA_RGB 2
#define TGA_Mono 3
#define TGA_RLEMap 9
#define TGA_RLERGB 10
#define TGA_RLEMono 11
#define TGA_CompMap 32
#define TGA_CompMap4 33


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pltga.h,v $
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
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.3  2002/02/24 13:00:40  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
