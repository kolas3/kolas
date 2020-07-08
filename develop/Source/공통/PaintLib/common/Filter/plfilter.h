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
|      $Id: plfilter.h,v 1.1 2009/12/09 05:33:38 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(AFX_FILTER_H__3CED06E1_1B40_11D3_B142_444553540000__INCLUDED_)
#define AFX_FILTER_H__3CED06E1_1B40_11D3_B142_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class PLBmp;

//! Base class for filters that operate on bitmaps. Derived classes need
//! to override either the ApplyInPlace or the Apply function.
class PLFilter : public PLObject  
{
public:
	//!
  PLFilter();
  //!
	virtual ~PLFilter() = 0;

  //! In-Place Apply. Applies the filter to pBmp. The base-class
  //! version copies the bitmap after calling Apply (pBmp, pTempBmp).
  virtual void ApplyInPlace(PLBmp * pBmp) const;  

  //! Applies the Filter to pBmpSource and stores the result in
  //! pBmpDest. The base-class version copies the bitmap before calling
  //! ApplyInPlace (pBmpDest).
  virtual void Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const;
};

#endif // !defined(AFX_FILTER_H__3CED06E1_1B40_11D3_B142_444553540000__INCLUDED_)

/*
/--------------------------------------------------------------------
|
|      $Log: plfilter.h,v $
|      Revision 1.1  2009/12/09 05:33:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:50  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:46  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:30:24  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:14  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:20  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:21  황정영
|      2006.05.17
|
|      Revision 1.2  2002/03/03 16:59:26  uzadow
|      Changed PLFilter so it's pure virtual.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.3  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.1  1999/10/21 16:05:17  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.1  1999/10/19 21:29:44  Ulrich von Zadow
|      Added filters.
|
|
\--------------------------------------------------------------------
*/
