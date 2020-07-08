/*
/--------------------------------------------------------------------
|
|      $Id: plfilterfill.cpp,v 1.1 2009/12/09 05:33:38 박종운 Exp $
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterfill.h"
#include "plfilterfillrect.h"
#include "plbitmap.h"

PLFilterFill::PLFilterFill(const PLPixel32& Color)
  : PLFilter(),
    m_Color (Color)
{
}

PLFilterFill::~PLFilterFill()
{
}

void PLFilterFill::ApplyInPlace(PLBmp * pBmp) const
{
  pBmp->ApplyFilter (PLFilterFillRect<PLPixel32> (PLPoint(0,0),
                                                PLPoint (pBmp->GetWidth(), pBmp->GetHeight()),
                                                m_Color));
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterfill.cpp,v $
|      Revision 1.1  2009/12/09 05:33:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:51  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:47  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:00  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:16  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:20  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:22  황정영
|      2006.05.17
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2001/09/13 20:48:42  uzadow
|      Added fill filters.
|
|      Revision 1.2  2001/09/13 10:39:31  uzadow
|      Added FilterFillRect
|
|      Revision 1.1  2001/09/06 14:20:13  uzadow
|      Moved FilterFill to paintlib
|
|      Revision 1.1.2.1  2001/09/04 11:20:40  uzadow
|      Initial version: test works, no main program yet.
|
|
\--------------------------------------------------------------------
*/

