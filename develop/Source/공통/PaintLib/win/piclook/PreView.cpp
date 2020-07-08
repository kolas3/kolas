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


#include "stdafx.h"
#include "resource.h"
#include "WinBmpEx.h"
#include "DibCtl.h"
#include "Preview.h"

#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewFileDlg

IMPLEMENT_DYNAMIC(CPreviewFileDlg, CFileDialog)

CPreviewFileDlg::CPreviewFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                                 DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
        CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    m_ofn.Flags |= (OFN_EXPLORER | OFN_ENABLETEMPLATE);
    m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILEOPENPREVIEW);

    m_bPreview = true;
}


BEGIN_MESSAGE_MAP(CPreviewFileDlg, CFileDialog)
    //{{AFX_MSG_MAP(CPreviewFileDlg)
    ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    ON_WM_SETFOCUS()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPreviewFileDlg::OnInitDialog()
{
    CFileDialog::OnInitDialog();

    m_DIBStaticCtrl.SubclassDlgItem(IDC_IMAGE, this);
    GetDlgItem(IDC_PREVIEW)->SendMessage(BM_SETCHECK, (m_bPreview) ? 1 : 0);

    return true;  // return true unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return false
}


void CPreviewFileDlg::OnFileNameChange()
{
    CFileDialog::OnFileNameChange();
    if (m_bPreview)
        m_DIBStaticCtrl.LoadDib(GetPathName()); // the control will handle errors
}


void CPreviewFileDlg::OnPreview()
{
    m_bPreview = !m_bPreview;
    if (!m_bPreview)
        m_DIBStaticCtrl.RemoveDib(); // no preview
    else
        m_DIBStaticCtrl.LoadDib(GetPathName()); // the control will handle errors
}


BOOL CPreviewFileDlg::OnQueryNewPalette()
{
    m_DIBStaticCtrl.SendMessage(WM_QUERYNEWPALETTE);  // redo the palette if necessary
    return CFileDialog::OnQueryNewPalette();
}

void CPreviewFileDlg::OnPaletteChanged(CWnd* pFocusWnd)
{
    CFileDialog::OnPaletteChanged(pFocusWnd);
    m_DIBStaticCtrl.SendMessage(WM_PALETTECHANGED, (WPARAM)pFocusWnd->GetSafeHwnd());  // redo the palette if necessary
}


void CPreviewFileDlg::OnSetFocus(CWnd* pOldWnd)
{
    CFileDialog::OnSetFocus(pOldWnd);
    m_DIBStaticCtrl.SendMessage(WM_QUERYNEWPALETTE);  // redo the palette if necessary
}


#ifdef _DEBUG
void CPreviewFileDlg::Dump(CDumpContext& dc) const
{
    CFileDialog::Dump(dc);
    if (m_bPreview)
        dc << "preview is enabled\n";
    else
        dc << "preview is disabled\n";
}
#endif //_DEBUG
/*
/--------------------------------------------------------------------
|
|      $Log: PreView.cpp,v $
|      Revision 1.1  2009/12/09 05:34:54  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:10  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:21  PDJ
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
|      Revision 1.8  2001/10/06 15:31:10  uzadow
|      Fixed open file dialog bug.
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
|      Revision 1.4  1999/12/02 17:07:36  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
