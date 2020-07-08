/*
/--------------------------------------------------------------------
|
|      $Id: PicLook.cpp,v 1.1 2009/12/09 05:34:53 ¹ÚÁ¾¿î Exp $
|      application class implementation
|
|      PaintLib MDI MFC graphic files viewer
|
|      Copyright (c) 1998 Bernard Delmée courtesy of Dilys bvba.
|
\--------------------------------------------------------------------
*/

/*
 *  This program hacked from the 'DIBLOOK' MFC example, with
 *  the added capability of multi-format viewing besides BMP.
 *  This was a testbed for paintlib under Win32, really,
 *  and we also use the quick DrawDib bitmap engine.
 *  Reuse at will in derived work; just acknowledge their due credit
 *  to the authors of paintlib and the decoding libraries it relies on.
 */

#include "stdafx.h"
#include "piclook.h"
#include "docman.h"
#include "mainfrm.h"
//#include "childfrm.h"
#include "WinBmpEx.h"
#include "doc.h"
#include "view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#include "planydec.h"
#endif

// here for easy modification, Ulrich!
const char paintlib_url[] = "http://www.paintlib.de/paintlib/";

/////////////////////////////////////////////////////////////////////////////
// CPaintLibViewerApp

BEGIN_MESSAGE_MAP(CPaintLibViewerApp, CWinApp)
    //{{AFX_MSG_MAP(CPaintLibViewerApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintLibViewerApp construction
// Place all significant initialization in InitInstance

CPaintLibViewerApp::CPaintLibViewerApp()
{}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPaintLibViewerApp object


CPaintLibViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPaintLibViewerApp initialization

BOOL CPaintLibViewerApp::InitInstance()
{
    // Standard initialization
    SetRegistryKey( "PaintLib" );
    LoadStdProfileSettings(5);  // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    ASSERT(m_pDocManager == NULL);
    m_pDocManager = new CDocManagerEx;
    CMultiDocTemplate* pDocTemplate;
    // BMP aware, and more ...
    pDocTemplate = new CMultiDocTemplate(
                       IDR_BMPTYPE,
                       RUNTIME_CLASS(CPLViewerDoc),
                       RUNTIME_CLASS(CMDIChildWnd),    // standard MDI child frame
                       //RUNTIME_CLASS(CChildFrame),    // derived MDI child frame
                       RUNTIME_CLASS(CPLView));
    m_pDocManager->AddDocTemplate(pDocTemplate);

    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return false;
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();
    m_pMainWnd = pMainFrame;

    // enable file manager drag/drop and DDE Execute open
    m_pMainWnd->DragAcceptFiles();

    // Uncomment this to have PicLook register itself as default viewer
    // for image files.
    // EnableShellOpen();
    // RegisterShellFileTypes(true);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // MFC faq says this avoids 1st (empty) doc
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return false;

#ifdef _DEBUG
    PLPicDecoder::SetTraceConfig( 3, NULL );
#endif
    return true;
}


int CPaintLibViewerApp::GetScreenBPP() const
{
    HDC dc = ::GetWindowDC(NULL);
    int adjbit = ::GetDeviceCaps(dc, BITSPIXEL);
    int planes = ::GetDeviceCaps(dc, PLANES);
    ::ReleaseDC (NULL, dc);
    // TRACE( "video set at %d bpp\n", adjbit * planes );
    return adjbit * planes;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnCredits();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnUrl();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    bool m_bCreditsShown;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
    m_bCreditsShown = true;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_BN_CLICKED(ID_CREDITS, OnCredits)
    ON_WM_SETCURSOR()
    ON_BN_CLICKED(IDC_URL, OnUrl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPaintLibViewerApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // paintlib's url gets injected in a button field
    GetDlgItem( IDC_URL )->SetWindowText( paintlib_url );

    CenterWindow();
    // hide the credits at startup
    OnCredits();

    return true;
}

void CAboutDlg::OnCredits()
{
    m_bCreditsShown = ! m_bCreditsShown;

    RECT dlg_rect;
    GetWindowRect( &dlg_rect );
    // these size adjustments are scaled to 'dialog units'
    // to (hopefully) work at all resolutions...
    if (m_bCreditsShown)
        dlg_rect.bottom += 7 * HIWORD(::GetDialogBaseUnits());
    else
        dlg_rect.bottom -= 7 * HIWORD(::GetDialogBaseUnits());

    MoveWindow( &dlg_rect, true );
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (pWnd->GetSafeHwnd() ==  GetDlgItem( IDC_URL )->GetSafeHwnd())
    {
        //SetCursor( thisApp.LoadStandardCursor(IDC_CROSS) );
        SetCursor( thisApp.LoadCursor(IDC_CURSOR_HAND) );
        return true;
    }
    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CAboutDlg::OnUrl()
{
    (void) ShellExecute(NULL, "open", paintlib_url, NULL, NULL, SW_SHOWNORMAL);
}
/*
/--------------------------------------------------------------------
|
|      $Log: PicLook.cpp,v $
|      Revision 1.1  2009/12/09 05:34:53  ¹ÚÁ¾¿î
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
|      Revision 1.10  2001/10/06 15:31:10  uzadow
|      Fixed open file dialog bug.
|
|      Revision 1.9  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2001/01/15 15:05:32  uzadow
|      Added CBmp::ApplyFilter() and CBmp::CreateFilteredCopy()
|
|      Revision 1.7  2000/09/01 13:27:06  Administrator
|      Minor bugfixes
|
|      Revision 1.6  2000/03/28 21:05:04  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.5  2000/01/10 23:53:02  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  2000/01/04 22:06:37  Ulrich von Zadow
|      Added png encoder by Neville Richards.
|
|      Revision 1.3  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
--------------------------------------------------------------------
*/
