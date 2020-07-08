/*
/--------------------------------------------------------------------
|
|      $Id: DocMan.cpp,v 1.1 2009/12/09 05:34:50 박종운 Exp $
|
|      Customised document manager (for MFC doc/view framework)
|      This code copied from MFC sources, except that we use our derived
|      CFileDialog for graphic files previewing and adapt file filtering
|      bdelmee; 4/99
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\-------------------------------------------------------------------
*/

/*
 *  Customised document manager (for MFC doc/view framework)
 *  This code copied from MFC sources, except that we use our derived
 *  CFileDialog for graphic files previewing and adapt file filtering
 *  bdelmee; 4/99
 */

#include "stdafx.h"
#include "WinBmpEx.h"
#include "DibCtl.h"
#include "Preview.h"
#include "DocMan.h"

// see what graphic formats are compiled in paintlib
#include "config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void AppendFileType(CString& filter, CString& catfilter,
                           LPCTSTR desc, LPCTSTR extn)
{
    filter += desc;
    filter += (TCHAR)'\0';
    filter += extn;
    filter += (TCHAR)'\0';
    if (! catfilter.IsEmpty())
        catfilter += ';';
    catfilter += extn;
}


/////////////////////////////////////////////////////////////////////////////
// CDocManagerEx

IMPLEMENT_DYNAMIC(CDocManagerEx, CDocManager)

BOOL CDocManagerEx::DoPromptFileName(CString& fileName,
                                     UINT nIDSTitle,
                                     DWORD lFlags,
                                     BOOL bOpenFileDialog,
                                     CDocTemplate* /* pTemplate */ )
{
    CPreviewFileDlg dlgFile(bOpenFileDialog);

    CString title;
    VERIFY(title.LoadString(nIDSTitle));

    // our program is but a viewer, so let's use read-only mode
    dlgFile.m_ofn.Flags |= lFlags | OFN_READONLY;

    CString strFilter;
    CString strGrfFlt;

    // instead of using the template, let's build our filter
    // according to the graphic formats for which support was
    // compiled in paintlib
#ifdef PL_SUPPORT_BMP
    AppendFileType(strFilter,strGrfFlt,"BMP file","*.bmp;*.dib;*.rle");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_JPEG
    AppendFileType(strFilter,strGrfFlt,"JPEG file","*.jpg;*.jpeg;*.jif");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PNG
    AppendFileType(strFilter,strGrfFlt,"PNG file","*.png");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PICT
    AppendFileType(strFilter,strGrfFlt,"PICT file","*.pct");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_TIFF
    AppendFileType(strFilter,strGrfFlt,"TIFF file","*.tif;*.eps");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_TGA
    AppendFileType(strFilter,strGrfFlt,"TARGA file","*.tga");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_WEMF
    AppendFileType(strFilter,strGrfFlt,"Windows Metafile","*.wmf");
    dlgFile.m_ofn.nMaxCustFilter++;
    AppendFileType(strFilter,strGrfFlt,"Windows Enhanced Metafile","*.emf");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PCX
    AppendFileType(strFilter,strGrfFlt,"PCX file","*.pcx");
    dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PGM
  AppendFileType(strFilter,strGrfFlt,"Portable Greymap File","*.pgm");
  dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PPM
  AppendFileType(strFilter,strGrfFlt,"Portable Pixmap File","*.ppm");
  dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_PSD
  AppendFileType(strFilter,strGrfFlt,"Photoshop File","*.psd");
  dlgFile.m_ofn.nMaxCustFilter++;
#endif
#ifdef PL_SUPPORT_GIF
  AppendFileType(strFilter,strGrfFlt,"GIF","*.gif");
  dlgFile.m_ofn.nMaxCustFilter++;
#endif

    // add default filter
    AppendFileType(strFilter,strGrfFlt,
                   "All supported graphic formats",(LPCTSTR) strGrfFlt);
    dlgFile.m_ofn.nMaxCustFilter++;
    dlgFile.m_ofn.nFilterIndex = dlgFile.m_ofn.nMaxCustFilter;

    // append the "*.*" all files filter
    CString allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
    AppendFileType(strFilter,strGrfFlt,allFilter,"*.*");
    //  dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = title;
    dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

    bool bResult = dlgFile.DoModal() == IDOK ? true : false;
    fileName.ReleaseBuffer();
    return bResult;
}
/*
/--------------------------------------------------------------------
|
|      $Log: DocMan.cpp,v $
|      Revision 1.1  2009/12/09 05:34:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:09  박대우
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
|      Revision 1.14  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.13  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.12  2001/10/16 17:51:32  uzadow
|      Added ppm support (Todd Harris)
|
|      Revision 1.11  2001/10/06 15:31:10  uzadow
|      Fixed open file dialog bug.
|
|      Revision 1.10  2000/12/08 12:32:00  uzadow
|      Added gif decoder by Michael Salzlechner.
|
|      Revision 1.9  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.8  2000/03/16 13:56:38  Ulrich von Zadow
|      Added pgm decoder by Jose Miguel Buenaposada Biencinto
|
|      Revision 1.7  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.6  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.5  1999/10/21 17:43:19  Ulrich von Zadow
|      Added pcx support by Meng Bo.
|
\-------------------------------------------------------------------
*/
