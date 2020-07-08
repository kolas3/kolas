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
|      $Id: pltester.h,v 1.1 2009/12/09 05:34:02 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLTESTER
#define INCL_PLTESTER

#include "pltest.h"
#include "config.h"

#ifdef _WINDOWS
#include <windows.h>
#else
#include "plpaintlibdefs.h"
#endif

#include "plbitmap.h"
#include "planybmp.h"

#ifdef _WINDOWS
#include "pldibsect.h"
// #include "plddrawbmp.h"
#endif

#include "plpicenc.h"
#include "plpicdec.h"
//class PLPicEncoder;
//class PLPicDecoder;
class PLFilter;

class PLTester: public PLTest
{

public:
  PLTester ();

  virtual ~PLTester();

  virtual void RunTests ();

private:
  void testBmpInfo (PLBmpInfo& BmpInfo);
  void testPLBmp (PLBmp& Bmp);
#ifdef _WINDOWS
  void testPLWinBmp (PLWinBmp& Bmp);
  void testLoadRes ();
  void testDIBSect (PLDIBSection& Bmp);
#endif
  void testPixels ();
  void testPoint ();
  void testRect ();

  void testCodec (const PLPicEncoder& Encoder, const PLPicDecoder& Decoder);
  void testCodecBmp (PLBmp& Bmp, const PLPicEncoder& Encoder,
                     const PLPicDecoder& Decoder, char * pszFName);
#ifdef PL_SUPPORT_TIFF
  void testCTIFFEx (PLBmp& Bmp);
#endif

  void setUpBmp (PLBmp& Bmp, int BPP);
  void setUp ();
  void tearDown ();
  void dumpBmp (PLBmp& bmp);
  bool almostEqual (PLBmp& bmp1, PLBmp& bmp2, int epsilon);


  PLAnyBmp m_AnyBmp8;
  PLAnyBmp m_AnyBmp24;
  PLAnyBmp m_AnyBmp32;

#ifdef _WINDOWS
  PLDIBSection m_DIBSect8;
  PLDIBSection m_DIBSect32;
#endif

//  PLDDrawBmp m_DDBmp8;
//  PLDDrawBmp m_DDBmp32;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pltester.h,v $
|      Revision 1.1  2009/12/09 05:34:02  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:48  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:11  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:01  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:28  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.21  2002/11/27 22:32:24  uzadow
|      Now works if TIFF, PNG and/or JPEG support are not available
|
|      Revision 1.20  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.19  2002/02/24 13:00:49  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.18  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.17  2001/10/03 14:00:29  uzadow
|      Much improved quality in FilterResizeBilinear.
|
|      Revision 1.16  2001/09/30 19:55:37  uzadow
|      Fixed bug for 8 bpp in PLFilterCrop.
|
|      Revision 1.15  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.14  2001/09/24 14:19:19  uzadow
|      Added PLPoint & PLRect tests.
|
|      Revision 1.13  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.12  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.11  2001/09/15 14:30:20  uzadow
|      Fixed PLPixel32 initialization bug.
|
|      Revision 1.10  2001/09/13 20:49:31  uzadow
|      Added fill filters.
|
|      Revision 1.9  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.8  2001/01/13 20:06:16  uzadow
|      Added Flip and Mirror filters.
|
|      Revision 1.7  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.6  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.5  2000/12/04 23:59:02  uzadow
|      Added filter tests.
|
|      Revision 1.4  2000/12/03 21:15:43  uzadow
|      Fixed png handling in configure; test/ adapted to linux
|
|      Revision 1.3  2000/12/02 19:42:30  uzadow
|      Added PLTIFFEncoderEx test.
|
|      Revision 1.2  2000/11/21 23:28:53  uzadow
|      Moved PLBmp tests from testdec to test.
|
|      Revision 1.1  2000/11/21 20:29:39  uzadow
|      Added test project.
|
|
\--------------------------------------------------------------------
*/
