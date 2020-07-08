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
|      $Id: DibCtl.h,v 1.1 2009/12/09 05:34:50 박종운 Exp $
|
\--------------------------------------------------------------------
*/
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Jorge Lodos
// All rights reserved
//
// Distribute and use freely, except:
// 1. Don't alter or remove this notice.
// 2. Mark the changes you made
//
// Send bug reports, bug fixes, enhancements, requests, etc. to:
//    lodos@cigb.edu.cu
/////////////////////////////////////////////////////////////////////////////

//  adapted to paintlib by bdelmee@skynet.be, 1998

#ifndef INCL_DIBCTL
#define INCL_DIBCTL

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WinBmpEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDIBStatic window

class CDIBStatic : public CStatic
{
    // Construction
public:
    CDIBStatic() : m_bHasFile(false)
	{} ;
    virtual ~CDIBStatic()
	{} ;

    // Attributes
private:
    PLWinBmpEx	m_DIB;
    bool	m_bHasFile;

    // Operations
public:
    bool IsValidDib() const
    {
        return m_bHasFile;
    }
    bool LoadDib(LPCTSTR lpszFileName);
    void UpdateDib();
    void RemoveDib();

    bool DoRealizePalette(bool bForceBackGround);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDIBStatic)
    //}}AFX_VIRTUAL

protected:
    void ClearDib();
    void PaintDib(bool bDibValid);

    // Generated message map functions
protected:
    //{{AFX_MSG(CDIBStatic)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // INCL_DIBCTL
/*
/--------------------------------------------------------------------
|
|      $Log: DibCtl.h,v $
|      Revision 1.1  2009/12/09 05:34:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:20  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:31  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:09  황정영
|      2006.05.17
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/01/27 18:20:18  uzadow
|      Updated copyright message; corrected pcx decoder bug.
|
|      Revision 1.6  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.5  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
