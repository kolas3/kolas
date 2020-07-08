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
|      $Id: plgifdec.h,v 1.1 2009/12/09 05:33:51 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLGIFDEC
#define INCL_PLGIFDEC


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plpicdec.h"

//! Compuserve gif file decoder using libungif to do the actual work. The
//! bitmap returned always has 8 bpp. If the gif has a transparent color,
//! the palette of the bitmap has alpha=0 for this color and HasAlpha=true.
//! Does not support loading more than one image from a multi-image gif.
//! (One issue with gifs is that you don't know whether the image has a 
//! transparent color until it's been decoded completely. For this reason,
//! calling HasAlpha for the decoder before GetImage() will always return
//! false.
class PLGIFDecoder : public PLPicDecoder  
{
public:
  PLGIFDecoder();
  virtual ~PLGIFDecoder();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //!
  virtual void GetImage (PLBmp & Bmp);

  //!
  virtual void Close ();

private:
  void * m_pGifFile;

};

#endif 

/*
/--------------------------------------------------------------------
|
|      $Log: plgifdec.h,v $
|      Revision 1.1  2009/12/09 05:33:51  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:58  PDJ
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
|      Revision 1.5  2002/11/02 20:33:27  uzadow
|      Added gif transparency handling (Mark Richarme)
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.1  2000/12/08 13:41:23  uzadow
|      Added gif support by Michael Salzlechner.
|
|
\--------------------------------------------------------------------
*/
