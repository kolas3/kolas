/*
/--------------------------------------------------------------------
|
|      $Id: drawhelp.cpp,v 1.1 2009/12/09 05:35:02 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#include "stdafx.h"

#include "drawhelp.h"
#include "dibgrit.h"


void DrawOnBmp
    ( PLBmp * pDestBmp,
      CRect * pDestRect,
      PLBmp * pSrPLBmp,
      PLBYTE Opacity
    )
{
  CDIBGrItem DIBItem (pDestRect->left, pDestRect->top,
                      pDestRect->Width(), pDestRect->Height(),
                      0, Opacity, pSrPLBmp);

  DIBItem.Draw (pDestBmp, pDestRect);
}
/*
/--------------------------------------------------------------------
|
|      $Log: drawhelp.cpp,v $
|      Revision 1.1  2009/12/09 05:35:02  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:14  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:27  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:51  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:36  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:56  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
--------------------------------------------------------------------
*/
