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
|      $Id: PicLook.h,v 1.1 2009/12/09 05:34:54 ¹ÚÁ¾¿î Exp $
|      application class declaration
|
|      PaintLib MDI MFC graphic files viewer
|
|      Copyright (c) 1998 Bernard Delmée courtesy of Dilys bvba.
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PICLOOK
#define INCL_PICLOOK

#ifndef __AFXWIN_H__
#   error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"    // main symbols
#include "planydec.h"        // PaintLib multi-format decoder class

//  private message for palette negotiations
#define WM_DOREALIZE   (WM_USER + 0)

/////////////////////////////////////////////////////////////////////////////
// CPaintLibViewerApp :
// See PicLook.cpp for the implementation of this class
//

class CPaintLibViewerApp : public CWinApp
{
public:
    CPaintLibViewerApp();
    PLAnyPicDecoder *GetDecoder()
    {
        return &m_theDecoder;
    }

    // Overrides
    virtual BOOL InitInstance();
    
    int GetScreenBPP() const;

protected:
    // global decoder object (embedded in unique app instance)
    PLAnyPicDecoder  m_theDecoder;

    // Implementation

    //{{AFX_MSG(CPaintLibViewerApp)
    afx_msg void OnAppAbout();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif  // INCL_PICLOOK
/*
/--------------------------------------------------------------------
|
|      $Log: PicLook.h,v $
|      Revision 1.1  2009/12/09 05:34:54  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:09  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:21  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:33  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:31  È²Á¤¿µ
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
|      Revision 1.6  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/03/28 21:05:04  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.4  2000/01/10 23:53:02  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.3  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
