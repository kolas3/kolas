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
|      $Id: testdec.h,v 1.1 2009/12/09 05:35:00 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"    // main symbols

// CDibtestApp:
// See dibtest.cpp for the implementation of this class
//

class CDibtestApp : public CWinApp
{
public:
  CDibtestApp();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDibtestApp)
public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CDibtestApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/*
/--------------------------------------------------------------------
|
|      $Log: testdec.h,v $
|      Revision 1.1  2009/12/09 05:35:00  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:25  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:37  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:34  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:54  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:02  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2000/01/10 23:53:03  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
--------------------------------------------------------------------
*/
