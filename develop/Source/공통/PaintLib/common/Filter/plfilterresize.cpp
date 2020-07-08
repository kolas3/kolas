/*
/--------------------------------------------------------------------
|
|      $Id: plfilterresize.cpp,v 1.1 2009/12/09 05:33:42 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterresize.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PLFilterResize::PLFilterResize(int NewXSize, int NewYSize) 
: PLFilter(),
  m_NewXSize(NewXSize), 
  m_NewYSize(NewYSize)
{
}

PLFilterResize::~PLFilterResize()
{
}

void PLFilterResize::SetNewSize(int NewXSize, int NewYSize)
{
  m_NewXSize = NewXSize;
  m_NewYSize = NewYSize;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterresize.cpp,v $
|      Revision 1.1  2009/12/09 05:33:42  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:53  PDJ
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
|      Revision 1.1  2006/05/23 05:48:22  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.3  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.2  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
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
