/*
/--------------------------------------------------------------------
|
|      $Id: plfilter.cpp,v 1.1 2009/12/09 05:33:38 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilter.h"
#include "plbitmap.h"
#include "planybmp.h"


PLFilter::PLFilter() : PLObject()
{

}

PLFilter::~PLFilter()
{

}

void PLFilter::ApplyInPlace(PLBmp * pBmp) const
{
  // Use a bitmap class that's guaranteed to work on all platforms as
  // temporary storage.
  PLAnyBmp TempBmp;  
  Apply (pBmp, &TempBmp);
  *pBmp = TempBmp;
}

void PLFilter::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  *pBmpDest = *pBmpSource;
  ApplyInPlace (pBmpDest);
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilter.cpp,v $
|      Revision 1.1  2009/12/09 05:33:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:49  PDJ
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
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.9  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.8  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.7  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  1999/12/14 12:29:47  Ulrich von Zadow
|      no message
|
|      Revision 1.5  1999/12/10 01:27:27  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.4  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
|
|      Revision 1.3  1999/11/27 18:45:48  Ulrich von Zadow
|      Added/Updated doc comments.
|
|      Revision 1.2  1999/10/21 18:48:03  Ulrich von Zadow
|      no message
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
