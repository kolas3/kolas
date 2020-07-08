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
|      $Id: Doc.h,v 1.1 2009/12/09 05:34:50 ¹ÚÁ¾¿î Exp $
|      Document class declaration
|
|      Copyright (c) 1998 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#ifndef INCL_DOCUMENT
#define INCL_DOCUMENT

#include "WinBmpEx.h"

// Jo Hagelberg 15.4.99: inherit from PLIProgressNotification
#include "plprognot.h"


// Jo Hagelberg 15.4.99: inherit from PLIProgressNotification
class CPLViewerDoc : public CDocument, PLIProgressNotification
{
protected: // create from serialization only
    CPLViewerDoc();
    DECLARE_DYNCREATE(CPLViewerDoc)

    // Attributes
public:
    CSize GetDocSize()
    {
        return m_pDib->GetSize();
    }
    HPALETTE GetDocPalette()
    {
        return m_pDib->GetLogPalette();
    }

    // Operations
public:
    void ConvertTo8BPP(UINT iDitherPaletteType, UINT iDitherType);
    virtual void DeleteContents();
    PLWinBmpEx *GetDib()
    {
      return m_pDib;
    }
    // Jo Hagelberg 15.4.99: copied OnProgress from DibStatic
    void OnProgress( double Part);

    // Implementation
protected:
    virtual ~CPLViewerDoc();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

protected:
    PLWinBmpEx*	m_pDib;

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // virtual bool OnSaveDocument( LPCTSTR lpszPathName );

    // Generated message map functions
protected:
    //{{AFX_MSG(CPLViewerDoc)
    afx_msg void OnFileSave();
    afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif  // INCL_DOCUMENT
/*
/--------------------------------------------------------------------
|
|      $Log: Doc.h,v $
|      Revision 1.1  2009/12/09 05:34:50  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:08  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:20  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:31  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:31:09  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.9  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2000/03/31 11:53:31  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.7  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.6  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.5  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.4  1999/11/08 22:15:05  Ulrich von Zadow
|      Added File/SaveAs
|
|
\--------------------------------------------------------------------
*/
