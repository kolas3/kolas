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
|      $Id: plpixeldefs.h,v 1.1 2009/12/09 05:33:55 박종운 Exp $
|
|      Pixel format definitions for MS Visual C++
|
|      Copyright (c) 1996-1998 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/


#ifndef INCL_PLPIXELDEFS
#define INCL_PLPIXELDEFS

#undef PL_PIXEL_BGRA_ORDER  // MS Windows ordering
#undef PL_PIXEL_RGBA_ORDER // GNU/Linux (at least)

// Define the pixel format for RGB-bitmaps. Change this if you change
// the order of the color components. 

// We have this pixel byte ordering
#define PL_PIXEL_BGRA_ORDER

#ifdef PL_PIXEL_RGBA_ORDER
 // To access in RGBA order
 #define PL_RGBA_RED    0
 #define PL_RGBA_GREEN  1
 #define PL_RGBA_BLUE   2
 #define PL_RGBA_ALPHA  3
#elif defined(PL_PIXEL_BGRA_ORDER)
 #define PL_RGBA_BLUE   0
 #define PL_RGBA_GREEN  1
 #define PL_RGBA_RED    2
 #define PL_RGBA_ALPHA  3
#else
  #error Pixel RGB bytes order not selected
#endif

// Note that the preceeding #defines aren't static const ints because
// this breaks preprocessor tests in several places.

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plpixeldefs.h,v $
|      Revision 1.1  2009/12/09 05:33:55  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:44  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:02  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:05  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.2  2000/11/09 22:23:36  uzadow
|      no message
|
|
\--------------------------------------------------------------------
*/
