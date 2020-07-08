/*
/--------------------------------------------------------------------
|
|      $Id: testpaintlib.cpp,v 1.1 2009/12/09 05:34:03 박종운 Exp $
|
|      Test suite for paintlib.
|
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifdef _WINDOWS
#include <windows.h>
#else
#include "plpaintlibdefs.h"
#endif

#include "pltester.h"

int main(int nargs, char** args)
{
  PLTester* pTester;
  pTester = new PLTester();
  pTester->RunTests();
  bool bOK = pTester->IsOk();
  delete pTester;

  if (bOK)
    return 0;
  else
    return 1;
}

/*
/--------------------------------------------------------------------
|
|      $Log: testpaintlib.cpp,v $
|      Revision 1.1  2009/12/09 05:34:03  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:48  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:13  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:03  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:29  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.8  2002/11/04 22:40:14  uzadow
|      Updated for gcc 3.1
|
|      Revision 1.7  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.6  2002/02/24 13:00:50  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.5  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.4  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.3  2001/01/15 15:05:31  uzadow
|      Added CBmp::ApplyFilter() and CBmp::CreateFilteredCopy()
|
|      Revision 1.2  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.1  2000/11/21 20:29:39  uzadow
|      Added test project.
|
|
|
\--------------------------------------------------------------------
*/
