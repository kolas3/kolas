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
|      $Id: dibwnd.h,v 1.1 2009/12/09 05:34:58 박종운 Exp $
|      Window containing a bitmap.
|
|      Child window containing a bitmap. Handles
|      scrolling and resize.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\----------------------------------------------------------------
*/

#ifndef INCL_DIBWND
#define INCL_DIBWND

#include "plwinbmp.h"

class CDIBWnd : public CWnd
{

DECLARE_DYNAMIC (CDIBWnd);
// Construction
public:
  CDIBWnd
    ( PLWinBmp * pBmp
    );

  virtual ~CDIBWnd
    ();

// Implementation
public:
  virtual void NewDIBNotify
    ();
    // Call this whenever the bitmap is changed outside of the object.

protected:
// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDIBWnd)
  //}}AFX_VIRTUAL

  // Generated message map functions
  //{{AFX_MSG(CDIBWnd)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnPaint();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

protected:
  void initScrollBars
    ( int cx,
      int cy
    );

  PLWinBmp   * m_pBmp;
  CSize       m_Size;
  CSize       m_BmpSize;
  CPoint      m_BmpPos;
  bool        m_bScrolling;

private:
  void initScrollBar
    ( int ID,
      int Page,
      int Range,
      int Pos
    );

  int handleScroll
    ( int ID,
      UINT nSBCode,
      UINT nPos,
      int Page,
      int Range,
      int Pos
    );

};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: dibwnd.h,v $
|      Revision 1.1  2009/12/09 05:34:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:12  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:24  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:36  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:32  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:52  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:12  황정영
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
