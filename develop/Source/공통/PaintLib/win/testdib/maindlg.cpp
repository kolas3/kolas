/*
/--------------------------------------------------------------------
|
|      $Id: maindlg.cpp,v 1.1 2009/12/09 05:35:04 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "mmsystem.h"

#include "testdib.h"
#include "MainDlg.h"
#include "planydec.h"
#include "plpictdec.h"
#include "dibgrit.h"
#include "shadow.h"
#include "drawhelp.h"
#include "planybmp.h"
#include "plexcept.h"
#include "pldibsect.h"

#ifdef TEST_DDRAW
#include "plddrawbmp.h"
#include "plddhelper.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Worker thread to show the pics.

UINT FadePic (LPVOID pParam)
{
  CDibtestDlg * pDlg = (CDibtestDlg *) pParam;

  pDlg->DoWork();

  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CDibtestDlg dialog

CDibtestDlg::CDibtestDlg (char * pszDirName)
    : CDialog(CDibtestDlg::IDD, NULL)
{
  //{{AFX_DATA_INIT(CDibtestDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_pBigBmp = new PLWinBmp ();
  m_pBmp = new PLDIBSection ();

  InitializeCriticalSection(&m_Critical);
  m_pDIBWnd = new CMTDIBWnd (m_pBmp, &m_Critical);

  m_sDirName = pszDirName;
  if (m_sDirName.Right(1) != '\\')
    m_sDirName += '\\';

  m_x = 0;
  m_y = 0;

#ifdef TEST_DDRAW
  // Create the main DirectDraw object
  HRESULT hr = DirectDrawCreate(NULL, &m_pDDraw, NULL);
  CheckForDDError (hr);

  hr = m_pDDraw->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
  CheckForDDError (hr);
#endif
}

CDibtestDlg::~CDibtestDlg ()
{
  delete m_pDIBWnd;

  delete m_pBmp;
  delete m_pBigBmp;

  DeleteCriticalSection(&m_Critical);

#ifdef TEST_DDRAW
  m_pDDraw->Release();
  m_pDDraw = NULL;
#endif
}

void CDibtestDlg::DoWork ()
{

  PLAnyPicDecoder Decoder;
  CGrItemArray DIBArray;
  int i,j;
  CRect OldRect, NewRect;

  Decoder.SetTraceConfig (2, NULL); //"C:\\paintlib.log");

  createBkGrnd (&Decoder);
  m_pDIBWnd->InvalidateRect (NULL);
  m_pDIBWnd->UpdateWindow ();

  testResource ();

  // Load the files
  loadRes (&Decoder, &DIBArray, IDR_LOGO1);
  loadRes (&Decoder, &DIBArray, IDR_LOGO);

  CDIBGrItem * pAmmonitItem = ((CDIBGrItem*)DIBArray.GetItem(0));
  CDIBGrItem * pParserItem = ((CDIBGrItem*)DIBArray.GetItem(1));


  SetDlgItemText (IDS_FNAME, "Blt without alpha channel.");
  for (i=0; i<100; i++)
  {
    update (&DIBArray, &OldRect, &NewRect);

    // Move
    pAmmonitItem->m_x += 1;
    pAmmonitItem->m_y += 1;
    pParserItem->m_x -=1;
  }

  PLPixel32 ShadowColor;
  ShadowColor.Set (0x80, 0x80, 0xFF, 0x00);

  CShadowItem * pShadow;
  pShadow = new CShadowItem (200, 100,
                             400, 200,
                             1, 64,
                             &ShadowColor, NULL);
  DIBArray.AddItem (pShadow);

  // Set alpha channel
  setAlphaChannel (&DIBArray);

  SetDlgItemText (IDS_FNAME, "Blt with alpha channel.");
  pParserItem->m_Opacity = 50;

  for (i=0; i<100; i++)
  {
    update (&DIBArray, &OldRect, &NewRect);

    // Move
    pAmmonitItem->m_x += 1;
    pAmmonitItem->m_y -= 1;
    pAmmonitItem->m_Opacity -=2;

    pParserItem->m_x -=1;
    pParserItem->m_Opacity += 2;
  }

  SetDlgItemText (IDS_FNAME, "Blt with size changes.");

  for (j=0; j<DIBArray.GetSize(); j++)
  {
    CDIBGrItem * pItem = ((CDIBGrItem*)DIBArray.GetItem(j));
    pItem->m_w /= 2;
    pItem->m_h /= 2;
  }
  pAmmonitItem->m_Opacity = 255;

  // Remove shadow.

  DIBArray.RemoveAt (2);
  delete pShadow->m_pAlphaBmp;
  delete pShadow;

  int t1 = timeGetTime ();
  for (j= 0; j<10; j++)
  {
    pParserItem->m_x = 200;
    pParserItem->m_y = 100;
    pParserItem->m_w = pParserItem->GetBmp()->GetWidth();
    pParserItem->m_h = pParserItem->GetBmp()->GetHeight();
    pParserItem->m_Opacity = 155;

    for (i=0; i<99; i++)
    {
      update (&DIBArray, &OldRect, &NewRect);

      // Move
      pAmmonitItem->m_x += 1;
      pAmmonitItem->m_w -= 2;
      if (i & 1)
        pAmmonitItem->m_h -= 1;
      else
        pAmmonitItem->m_h -= 2;
      pAmmonitItem->m_Opacity -= 3;
//      pAmmonitItem->GetBmp()->Rotate (3.1415926535/90, 0);

      pParserItem->m_x -= 1;
      pParserItem->m_w += 3;
      pParserItem->m_h += 1;

      pParserItem->m_Opacity += 1;

    }
  }
  int t2 = timeGetTime ();


  // Delete all the bitmaps.
  for (i=0; i<2; i++)
  {
    PLBmp * pBmp = ((CDIBGrItem *)DIBArray.GetItem(i))->GetBmp();
    delete (pBmp);
  }

  char sz[256];
  sprintf (sz, "Test finished. Time: %.4f sec.", double(t2-t1)/1000);
  SetDlgItemText (IDS_FNAME, sz);

}


void CDibtestDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDibtestDlg)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDibtestDlg, CDialog)
//{{AFX_MSG_MAP(CDibtestDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_CREATE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibtestDlg message handlers

BOOL CDibtestDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Set the icon for this dialog.
  SetIcon(m_hIcon, true);      // Set big icon
  SetIcon(m_hIcon, false);    // Set small icon

  CRect Rect;

  // Resize bitmap window so it fills the frame.
  CWnd * pWnd = GetDlgItem(IDS_BMPFRAME);
  if (pWnd)
  {
    pWnd->GetWindowRect(Rect);
    ScreenToClient (Rect);
    Rect.InflateRect (-2, -2);
    m_pDIBWnd->MoveWindow (Rect, true);
  }

  // Create DIBs as canvas.
  int Width = Rect.right-Rect.left;
  int Height = Rect.bottom-Rect.top;

  m_pBigBmp->Create (Width, Height, 32, false, false);
  m_pBmp->Create (Width, Height, 32, false, false);

  AfxBeginThread (FadePic, (LPVOID) this);
  m_pDIBWnd->NewDIBNotify();

  return true;  // return true  unless you set the focus to a control
}


void CDibtestDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDibtestDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

int CDibtestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;

  m_pDIBWnd->Create (NULL, "DIBWnd", 0, CRect (0,0,10,10),
                     this, 2001);

  return 0;
}


/////////////////////////////////////////////////////////////////////
// CDibtestDlg locals


void CDibtestDlg::loadFile (PLPicDecoder * pDecoder, CGrItemArray * pDIBArray,
                            const char * pszFName)
// Tries to load a file. Does error checking.
{
  CString s ("Datei: ");
  CDIBGrItem * pDIBItem;

  s += pszFName;
  SetDlgItemText (IDS_FNAME, s);

  try
  {
    PLWinBmp * pNewBmp = new PLWinBmp();
    pDecoder->MakeBmpFromFile (pszFName, pNewBmp);

    pDIBItem = new CDIBGrItem (m_x, m_y, //161, 121,
                               pNewBmp->GetWidth(),
                               pNewBmp->GetHeight(),
                               1,
                               255,
                               pNewBmp);
    m_x += 480;
    m_y += 130;
    pDIBArray->AddItem (pDIBItem);
  }
  catch (PLTextException e)
  {
    char sz[256];
    TRACE ("Error decoding %s: %s", pszFName, (const char *)e);
    sprintf (sz, "Error: %s", (const char *)e);
    MessageBox (sz);
  }
  catch (CMemoryException e)
  {
    TRACE ("Error decoding %s: Out of memory.\n", pszFName);
  }
}


void CDibtestDlg::loadRes (PLPicDecoder * pDecoder, CGrItemArray * pDIBArray,
                           int ResourceID)
// Tries to load a resource. Does error checking.
{
  char sz[256];
  CDIBGrItem * pDIBItem;

  sprintf (sz, "Resource: ID %i", ResourceID);
  SetDlgItemText (IDS_FNAME, sz);

  try
  {
#ifdef TEST_DDRAW
    PLDDrawBmp * pNewBmp = new PLDDrawBmp(m_pDDraw);
#else
    PLWinBmp * pNewBmp = new PLWinBmp();
#endif
    pDecoder->MakeBmpFromResource (
					AfxFindResourceHandle(MAKEINTRESOURCE (ResourceID), ""),
					ResourceID, pNewBmp);

    pDIBItem = new CDIBGrItem (m_x, m_y, //161, 121,
                               pNewBmp->GetWidth(),
                               pNewBmp->GetHeight(),
                               1,
                               255,
                               pNewBmp);
    m_x += 480;
    m_y += 130;
    pDIBArray->AddItem (pDIBItem);
  }
  catch (PLTextException e)
  {
    char sz[256];
    TRACE ("Error decoding resource %x: %s", ResourceID, (const char *)e);
    sprintf (sz, "Error: %s", (const char *)e);
    MessageBox (sz);
  }
  catch (CMemoryException e)
  {
    TRACE ("Error decoding resource %x: Out of memory.\n", ResourceID);
  }
}


void CDibtestDlg::createBkGrnd (PLPicDecoder * pDecoder)
{
  PLWinBmp * pTileBmp = new PLWinBmp();
  try
  {
    pDecoder->MakeBmpFromResource (
					AfxFindResourceHandle(MAKEINTRESOURCE (IDR_CANVAS), ""),
					IDR_CANVAS, pTileBmp);
  }
  catch (PLTextException e)
  {
    char sz[256];
    TRACE ("Error decoding ID_CANVAS: %s", (const char *)e);
    sprintf (sz, "Error: %s", (const char *)e);
    MessageBox (sz);
  }
  catch (CMemoryException e)
  {
    TRACE ("Error decoding ID_CANVAS: Out of memory.\n");
  }

  int w = pTileBmp->GetWidth();
  int h = pTileBmp->GetHeight();
  int xmult = m_pBmp->GetWidth()/w;
  int ymult = m_pBmp->GetHeight()/h;
  CRect DestRect;

  EnterCriticalSection (&m_Critical);
  for (int x=0; x<=xmult; x++)
    for (int y=0; y<=ymult; y++)
    {
      DestRect = CRect (x*w, y*h, (x+1)*w, (y+1)*h);
      DrawOnBmp (m_pBmp, &DestRect, pTileBmp, 255);
    }
  *m_pBigBmp = *m_pBmp;
  //  m_pBigBmp->CreateCopy (*m_pBmp);
  LeaveCriticalSection (&m_Critical);

  delete pTileBmp;
}

void CDibtestDlg::setAlphaChannel (CGrItemArray * pDIBArray)
{
  PLPictDecoder AlphaDecoder (NULL);
  PLAnyBmp * pAlphaBmp = new PLAnyBmp();

  AlphaDecoder.MakeBmpFromResource (
					AfxFindResourceHandle(MAKEINTRESOURCE (IDR_LOGOALPH), ""),
					IDR_LOGOALPH, pAlphaBmp, 0);

  for (int j=0; j<2; j++)
  {
    CDIBGrItem * pItem = ((CDIBGrItem*)pDIBArray->GetItem(j));
    pItem->GetBmp()->SetAlphaChannel (pAlphaBmp);
  }

  CShadowItem * pShadow;
  pShadow = (CShadowItem *)pDIBArray->GetItem(2);
  pShadow->m_pAlphaBmp = pAlphaBmp;
  pShadow->m_pLineArray = pAlphaBmp->GetLineArray();
  pShadow->m_w = pAlphaBmp->GetWidth();
  pShadow->m_h = pAlphaBmp->GetHeight();

}


void CDibtestDlg::update (CGrItemArray * pDIBArray, CRect * pOldRect,
                          CRect * pNewRect)
{
  CRect UpdateRect;
  CDIBGrItem CanvasItem (0, 0,
                         m_pBigBmp->GetWidth(), m_pBigBmp->GetHeight(),
                         0, 255, m_pBigBmp);

  // Calculate update rectangle.
  *pOldRect = *pNewRect;
  pDIBArray->GetRect (pNewRect);
  UpdateRect.UnionRect (pOldRect, pNewRect);

  // Draw
  EnterCriticalSection (&m_Critical);
  CanvasItem.Draw (m_pBmp, &UpdateRect);
  pDIBArray->Draw (m_pBmp, &UpdateRect);
  LeaveCriticalSection (&m_Critical);

  m_pDIBWnd->InvalidateRect (&UpdateRect);
  m_pDIBWnd->UpdateWindow ();

}


void CDibtestDlg::testResource()
{
  PLWinBmp TestBmp;

  TestBmp.CreateRes (AfxGetInstanceHandle(),IDB_TEST3);
  TestBmp.CreateRes (AfxGetInstanceHandle(),IDB_TEST4);
  TestBmp.CreateRes (AfxGetInstanceHandle(),IDB_TEST8);
}
/*
/--------------------------------------------------------------------
|
|      $Log: maindlg.cpp,v $
|      Revision 1.1  2009/12/09 05:35:04  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:27  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:36  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:35:13  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.10  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.9  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.8  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.6  2000/08/13 12:11:44  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.5  2000/01/16 20:43:19  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/10 23:53:03  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
\--------------------------------------------------------------------
*/
