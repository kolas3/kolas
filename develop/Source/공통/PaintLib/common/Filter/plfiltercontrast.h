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
|      $Id: plfiltercontrast.h,v 1.1 2009/12/09 05:33:38 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERCONTRAST)
#define INCL_PLFILTERCONTRAST

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plfilter.h"
#include "plpaintlibdefs.h"

class PLBmp;

//! Enhances or reduces the image contrast using a linear mapping
//! between input and output. The zero point (i. e., the intensity
//! that is neither enhanced nor reduced) has to be provided.
//! (A Contrast filter is defined in the following way: It lowers all
//! intensity values below a given threshold, and it raises them
//! beyond. Most applications position the threshold at 50 %. This
//! does not always yield the best results, especially if you have an
//! unbalanced dark/light ratio of pixels. The best you can do here
//! is play around with the threshold.)
//! Works for 24 and 32 bpp bitmaps.
class PLFilterContrast : public PLFilter
{
public:
  //! contrast is the slope of the function. offset is the intensity
  //! at which the color stays the same. Above this value, intensities
  //! are increased. Below it, they are reduced. With offset 128 and
  //! contrast 1, the image stays unchanged.
  PLFilterContrast(double contrast,
                   PLBYTE offset);
  virtual ~PLFilterContrast();
  virtual void Apply(PLBmp *pBmpSource, PLBmp* pBmpDest) const;

protected:
  double m_contrast;
  double m_offset;
};
#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfiltercontrast.h,v $
|      Revision 1.1  2009/12/09 05:33:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:51  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:46  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:30:27  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:14  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:20  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:22  황정영
|      2006.05.17
|
|      Revision 1.3  2002/02/24 13:00:46  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.4  2001/01/13 14:44:52  uzadow
|      Doc update.
|
|      Revision 1.3  2001/01/12 23:30:01  uzadow
|      Doc update.
|
|      Revision 1.2  2001/01/12 23:04:37  uzadow
|      Comments.
|
|      Revision 1.1  2000/11/06 23:20:22  uzadow
|      Added Contrast, Intensity and Lightness filters by
|      Thomas Hirschmann
|
|
\--------------------------------------------------------------------
*/
