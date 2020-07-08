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
|      $Id: MainFrm.h,v 1.1 2009/12/09 05:34:53 ¹ÚÁ¾¿î Exp $
|      highest-level frame window class declaration
|
|      Copyright (c) 1998 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#ifndef INCL_MFRM
#define INCL_MFRM

#ifndef __AFXEXT_H__
#include <afxext.h>      // for access to CToolBar and CStatusBar
#endif

class CMainFrame : public CMDIFrameWnd
{
    DECLARE_DYNAMIC(CMainFrame)
public:
    CMainFrame();

    // Implementation
public:
    virtual ~CMainFrame();
    void SetProgressInfo(UINT part);


protected:  // control bar embedded members
    CStatusBar  m_wndStatusBar;
    CToolBar  m_wndToolBar;

    // Generated message map functions
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, bool bMinimized);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    afx_msg void OnUpdatePanels(CCmdUI *pCmdUI);
};

#endif  // INCL_MFRM
/*
/--------------------------------------------------------------------
|
|      $Log: MainFrm.h,v $
|      Revision 1.1  2009/12/09 05:34:53  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:09  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:21  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:32  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:30  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:50  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:31:11  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.7  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.6  2000/03/28 21:05:04  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.5  2000/01/10 23:53:02  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
