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
/----------------------------------------------------------------
|
|      $Id: mtdibwnd.h,v 1.1 2009/12/09 05:35:04 박종운 Exp $
|      Window containing a bitmap. Thread-safe.
|
|      Child window containing a bitmap. Handles
|      scrolling and resize.
|
|      A critical section is set whenever the window needs to
|      access the bitmap data.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\----------------------------------------------------------------
*/

#ifndef INCL_MTDIBWND
#define INCL_MTDIBWND

#include "dibwnd.h"

class CMTDIBWnd : public CDIBWnd
{

DECLARE_DYNAMIC (CMTDIBWnd);
// Construction
public:
  CMTDIBWnd
    ( PLWinBmp * pBmp,
      CRITICAL_SECTION * pCritical
    );

// Implementation
public:
  virtual void NewDIBNotify();

protected:
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMTDIBWnd)
        //}}AFX_VIRTUAL

  // Generated message map functions
  //{{AFX_MSG(CMTDIBWnd)
  afx_msg void OnPaint();
        //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  CRITICAL_SECTION * m_pCritical;

};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: mtdibwnd.h,v $
|      Revision 1.1  2009/12/09 05:35:04  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:27  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:36  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:35:13  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:15  황정영
|      2006.05.17
|
|      Revision 1.3  2002/01/28 20:19:51  uzadow
|      no message
|
|
--------------------------------------------------------------------
*/
