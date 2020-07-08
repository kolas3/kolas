/*
/--------------------------------------------------------------------
|
|      $Id: MainFrm.cpp,v 1.1 2009/12/09 05:34:53 ¹ÚÁ¾¿î Exp $
|      highest-level frame window class implementation
|
|       Copyright (c) 1998 Bernard Delmée
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "piclook.h"
#include "mainfrm.h"
#include "doc.h"
#include "view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_PALETTECHANGED()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_GETMINMAXINFO()
    //}}AFX_MSG_MAP
    ON_UPDATE_COMMAND_UI_RANGE( ID_INDICATOR_ZRATIO, ID_BMPINFO, OnUpdatePanels )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
    {
        // same order as in the bitmap 'toolbar.bmp'
        ID_FILE_OPEN,
        ID_SEPARATOR,
        ID_FILE_PRINT,
        ID_SEPARATOR,
        ID_ZOOM_MODE,
        ID_SEPARATOR,
        ID_APP_ABOUT,
    };

static UINT BASED_CODE indicators[] =
    {
	ID_SEPARATOR,    // status line indicator
//	ID_PROGRESS,     // Jo Hagelberg: added for progress info
#ifndef USES_DRAWDIB
	ID_INDICATOR_ZRATIO,
	ID_INDICATOR_PIXELX,
	ID_INDICATOR_PIXELY,
#endif
        ID_BMPINFO
    };

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{}


CMainFrame::~CMainFrame()
{}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.Create(this) ||
            !m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
            !m_wndToolBar.SetButtons(buttons,
                                     sizeof(buttons)/sizeof(UINT)))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;  // fail to create
    }

    // TODO: Remove this if you don't want tool tips or a resizeable toolbar

    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
                             CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    // enable docking for frame window and toolbar, then hook them together
    EnableDocking( CBRS_ALIGN_ANY );
    m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
    DockControlBar( &m_wndToolBar );

    // CStatusBar
    if (!m_wndStatusBar.Create(this) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;  // fail to create
    }


    CenterWindow();

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame commands

/*
 *  Windows notifies top-level windows when a palette change occurs,
 *  so every app gets a chance to update its display. In our MDI case,
 *  we just propagate the message (in a private form) to the drawing children.
 */
void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
    CMDIFrameWnd::OnPaletteChanged(pFocusWnd);

    // always realize the palette for the active view
    CMDIChildWnd* pMDIChildWnd = MDIGetActive();
    if (pMDIChildWnd == NULL)
        return; // no active MDI child frame
    CView* pView = pMDIChildWnd->GetActiveView();
    ASSERT(pView != NULL);

    // notify all child windows that the palette has changed
    SendMessageToDescendants(WM_DOREALIZE, (WPARAM)pView->m_hWnd);
}


/*
 *  Big time: we're getting focus,
 *  so let's give the active view an opportunity to realize its palette.
 */
BOOL CMainFrame::OnQueryNewPalette()
{
    // always realize the palette for the active view
    CMDIChildWnd* pMDIChildWnd = MDIGetActive();
    if (pMDIChildWnd == NULL)
        return false; // no active MDI child frame (no new palette)
    CView* pView = pMDIChildWnd->GetActiveView();
    ASSERT(pView != NULL);

    // just notify the target view
    return (BOOL) pView->SendMessage(WM_DOREALIZE, (WPARAM)pView->m_hWnd);
    //return true;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CFrameWnd::OnGetMinMaxInfo(lpMMI);

    // Set min track size to whatever you want. This is a quick
    // hack to get around the problem of sizing the frame window
    // considering all the possibilities for tool bars and status
    // bars to be on, off, docked top, docked bottom, etc.

    lpMMI->ptMinTrackSize.x = lpMMI->ptMaxTrackSize.x * 2 / 5;
    lpMMI->ptMinTrackSize.y = lpMMI->ptMaxTrackSize.y * 2 / 5;
}


// JH 13.4.99: added progress info
//         part is in the range 0...100
//         toDo: can s.o. add a real progress bar?
void CMainFrame::SetProgressInfo(UINT part)
{
    CString progtext;
    progtext.Format( "%d%%", part);

    // bdelmee; 3/00; this used to have its own pane, but since this
    // is transient info, let's just update while decoder progresses...
    m_wndStatusBar.SetPaneText( 0, progtext, true);
}


// when there's no (active) view, this handler will get precedence
void CMainFrame::OnUpdatePanels(CCmdUI *pCmdUI)
{
    pCmdUI->SetText( "" );
}
/*
/--------------------------------------------------------------------
|
|      $Log: MainFrm.cpp,v $
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
|      Revision 1.8  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delmée.
|
|      Revision 1.6  2000/03/28 21:05:04  Ulrich von Zadow
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
