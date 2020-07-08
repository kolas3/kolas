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

#if !defined(AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
#define AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PreviewFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewFileDlg dialog

class CPreviewFileDlg : public CFileDialog
{
    DECLARE_DYNAMIC(CPreviewFileDlg)

public:
    CPreviewFileDlg(BOOL bOpenFileDialog, // true for FileOpen, false for FileSaveAs
                    LPCTSTR lpszDefExt = NULL,
                    LPCTSTR lpszFileName = NULL,
                    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    LPCTSTR lpszFilter = NULL,
                    CWnd* pParentWnd = NULL);

    // Attributes
private:
    bool m_bPreview;
    CDIBStatic m_DIBStaticCtrl;

protected:
    //{{AFX_MSG(CPreviewFileDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPreview();
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    //}}AFX_MSG
    virtual void OnFileNameChange();

    // Implementation
#ifdef _DEBUG
    virtual void Dump(CDumpContext& dc) const;
#endif
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
/*
/--------------------------------------------------------------------
|
|      $Log: PreView.h,v $
|      Revision 1.1  2009/12/09 05:34:54  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:22  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:33  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:31  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:50  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:11  황정영
|      2006.05.17
|
|      Revision 1.7  2001/10/06 15:31:10  uzadow
|      Fixed open file dialog bug.
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
|      Revision 1.3  1999/12/02 17:07:36  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
