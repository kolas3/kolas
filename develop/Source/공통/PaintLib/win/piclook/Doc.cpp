/*
/--------------------------------------------------------------------
|
|      $Id: Doc.cpp,v 1.1 2009/12/09 05:34:50 박종운 Exp $
|      DOC.CPP    Document class implementation (where the bitmaps live)
|
|      Copyright (c) 1998 Bernard Delm?
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "piclook.h"
#include "doc.h"

#include "MainFrm.h"      // Jo Hagelberg 15.4.99

#include "pltiffencex.h"
#include "pljpegenc.h"
#include "plpngenc.h"
#include "plbmpenc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLViewerDoc

IMPLEMENT_DYNCREATE(CPLViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CPLViewerDoc, CDocument)
    //{{AFX_MSG_MAP(CPLViewerDoc)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLViewerDoc construction/destruction

CPLViewerDoc::CPLViewerDoc()
{
    m_pDib = 0;
}

CPLViewerDoc::~CPLViewerDoc()
{
    DeleteContents();
}

BOOL CPLViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    // recycle document
    DeleteContents();

    m_pDib = new PLWinBmpEx;

    // have PaintLib perform I/O instead of Serialization
    bool bRet = true;
    try
    {
        // Jo Hagelberg 15.4.99: added "this" for progress notification
        thisApp.GetDecoder()->MakeBmpFromFile(lpszPathName, m_pDib, 0, this);
    }
    catch (PLTextException e)
    {
        AfxMessageBox(e);
        DeleteContents();
        bRet = false;
    }

    if (bRet)
    {
        SetPathName(lpszPathName);
        SetModifiedFlag(false);      // start off with unmodified
        char *cp = strrchr(lpszPathName,'\\');
        if (cp) // establish this image's directory as our new wd
        {
            *cp = '\0';
            SetCurrentDirectory(lpszPathName);
            // pretend we did not touch that string
            *cp = '\\';
        }
    }

    return bRet;    // Belgian resolution enhancement technology :-)
}

// Jo Hagelberg 15.4.99: copied OnProgress from DibStatic
// (bad design, anyway, this is just for testing ;-)

void CPLViewerDoc::OnProgress
( double Part
)
{
    //TRACE ("V %.4f\n", Part);
    // Jo Hagelberg 13.4.99
    // added real progress info
    UINT ipart = UINT( Part * 100.0);
    ((CMainFrame*)AfxGetMainWnd())->SetProgressInfo(ipart);
}

void CPLViewerDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    pCmdUI->Enable (m_pDib != NULL);
}

void CPLViewerDoc::OnFileSave()
{
    CFileDialog  SaveDlg (false, ".jpg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                          "JPEG File (*.jpg)|*.jpg|Windows Bitmap (*.bmp)|*.bmp|TIFF File (*.tif)|*.tif|PNG File (*.png)|*.png|",
                          AfxGetMainWnd());
    if (SaveDlg.DoModal() == IDOK)
    {
      CString sFName = SaveDlg.GetPathName();
      CString sExt = SaveDlg.GetFileExt();
      sExt.MakeLower();
      try
        {
          if (sExt == "jpg")
          {
              PLJPEGEncoder Encoder;
              Encoder.MakeFileFromBmp(sFName, m_pDib);
          }
          else if (sExt == "tif")
          {
              PLTIFFEncoder Encoder;
              Encoder.MakeFileFromBmp(sFName, m_pDib);
          }
          else if (sExt == "png")
          {
              PLPNGEncoder Encoder;
              Encoder.MakeFileFromBmp(sFName, m_pDib);
          }
          else if (sExt == "bmp")
          {
              PLBmpEncoder Encoder;
              Encoder.MakeFileFromBmp(sFName, m_pDib);
          }
          else
          {
              // Extension must be one of the supported types. If not,
              // something is wrong in the user interface code.
              ASSERT (false);
          }
        }
      catch(PLTextException &e)
        {
          CString bmpEx;
          bmpEx.Format("Error saving the file: %s", 
                       (LPCTSTR) e, 
                       (LPCTSTR) e.GetCode());
          AfxMessageBox(bmpEx);
        }
    }
}


//////////////////
// Delete contents of doc: delete the DIB
//
void CPLViewerDoc::DeleteContents()
{
    if (m_pDib) delete m_pDib;
    m_pDib = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPLViewerDoc diagnostics

#ifdef _DEBUG
void CPLViewerDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CPLViewerDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}

#endif //_DEBUG
/*
/--------------------------------------------------------------------
|
|      $Log: Doc.cpp,v $
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
|      Revision 1.16  2002/03/03 16:29:56  uzadow
|      Re-added BPPWanted.
|
|      Revision 1.15  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.14  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.13  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.12  2000/07/11 17:49:04  Ulrich von Zadow
|      Added save in bitmap format (Thomas Hirschmann).
|
|      Revision 1.11  2000/03/31 11:53:31  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.10  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.8  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.7  2000/01/04 22:06:37  Ulrich von Zadow
|      Added png encoder by Neville Richards.
|
|      Revision 1.6  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.5  1999/11/08 22:15:05  Ulrich von Zadow
|      Added File/SaveAs
|
|
\--------------------------------------------------------------------
*/

void CPLViewerDoc::ConvertTo8BPP(UINT iDitherPaletteType, UINT iDitherType)
{
  PLWinBmpEx* bpp8Copy = new PLWinBmpEx;
  bpp8Copy->SetQuantizationMode (iDitherType, iDitherPaletteType);
  bpp8Copy->CreateCopy((PLBmp&)*m_pDib, 8);
  delete m_pDib;
  m_pDib = bpp8Copy;
}
