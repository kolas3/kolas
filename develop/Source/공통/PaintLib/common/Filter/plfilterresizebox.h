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
|      $Id: plfilterresizebox.h,v 1.1 2009/12/09 05:33:44 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef FILTERResizeBOX_H
#define FILTERResizeBOX_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "plfilterresize.h"

//! Resizes a 32 bpp bitmap using a box filter to calculate results.
class PLFilterResizeBox : public PLFilterResize
{
public:
  //! 
  PLFilterResizeBox (int NewXSize, int NewYSize);
  //! 
  virtual void Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const;

private: 
};

#endif 

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterresizebox.h,v $
|      Revision 1.1  2009/12/09 05:33:44  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:54  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:49  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:01  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:18  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.5  2002/02/24 13:00:48  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.4  2001/09/30 16:57:26  uzadow
|      Improved speed of 2passfilter.h, code readability changes.
|
|      Revision 1.3  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.2  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.2  1999/11/27 18:45:49  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.1  1999/10/21 16:05:18  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.1  1999/10/19 21:29:45  Ulrich von Zadow
|      Added filters.
|
|
\--------------------------------------------------------------------
*/
