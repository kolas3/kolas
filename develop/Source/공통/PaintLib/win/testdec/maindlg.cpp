/*
/--------------------------------------------------------------------
|
|      $Id: maindlg.cpp,v 1.1 2009/12/09 05:34:58 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "testdec.h"
#include "MainDlg.h"
#include "planydec.h"
#include "plexcept.h"
#include "planybmp.h"
#include "pldibsect.h"
#include "plbmpenc.h"
#include "filter/PLFilterQuantize.h"

#ifdef TEST_DDRAW
#include "plddrawbmp.h"
#include "plddhelper.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
// Worker thread to display the pics.

UINT FadePic (LPVOID pParam)
{
  CDibtestDlg * pDlg = (CDibtestDlg *) pParam;

  pDlg->DoWork();

  return 0;
}


/////////////////////////////////////////////////////////////////////
// CDibtestDlg dialog

CDibtestDlg::CDibtestDlg (char * pszDirName)
    : CDialog(CDibtestDlg::IDD, NULL)
{
  //{{AFX_DATA_INIT(CDibtestDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_pBmp = new PLDIBSection();
  m_pBigBmp = new PLWinBmp();

  InitializeCriticalSection(&m_Critical);
  m_pDIBWnd = new CMTDIBWnd (m_pBmp, &m_Critical);

  m_sDirName = pszDirName;
  if (m_sDirName.Right(1) != '\\')
    m_sDirName += '\\';

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

  delete m_pThreadCritical;

  DeleteCriticalSection(&m_Critical);

#ifdef TEST_DDRAW
  m_pDDraw->Release();
  m_pDDraw = NULL;
#endif
}

void CDibtestDlg::DoWork ()
{

  //---------------------- CBmp tests. ---------------------------

  PLAnyPicDecoder Decoder;
  Decoder.SetTraceConfig (2, NULL);  //"c:\paintlib.log");

  m_bStopDecoding = false;

  m_pThreadCritical = new CCriticalSection ();
  m_pThreadCritical->Lock();

  doCreateCopyTest (&Decoder);

  // Basic bitmap tests
  doBmpTest (98, 98, 1, IDB_TEST3);
  doBmpTest (100, 100, 8, IDB_TEST8);
  doBmpTest (101, 101, 8, 0);
  doBmpTest (103, 103, 32, 0);

  // This can be used if a specific file should be tested:
  //  displayFile (&Decoder, "c:\\prog\\PAINTLIB\\testpic\\tiff\\pics\\fax2d.tif");
  //  displayFile (&Decoder, "c:\\prog\\PAINTLIB\\testpic\\rgb32.tga");
//  displayDir (&Decoder, "f:\\prog\\paintlib\\testpic\\");
  displayDir (&Decoder, m_sDirName);

  if (!m_bStopDecoding)
    SetDlgItemText (IDS_FNAME, "Test finnished.");

  m_pThreadCritical->Unlock ();

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
ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////
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
    pWnd->ShowWindow(SW_HIDE);
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

  m_pThread = AfxBeginThread (FadePic, (LPVOID) this);
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

void CDibtestDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

}

void CDibtestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// Resize bitmap window so it fills the frame.
	CWnd * pWnd = GetDlgItem(IDS_BMPFRAME);
	CRect Rect;

	if(pWnd)
	{
		m_pDIBWnd->GetWindowRect(Rect);

		ScreenToClient(Rect);

		Rect.bottom = cy -10;
		Rect.right  = cx -10;

		m_pDIBWnd->MoveWindow (Rect, true);
	}
}

/////////////////////////////////////////////////////////////////////
// CDibtestDlg locals

void CDibtestDlg::displayDir (PLPicDecoder * pDecoder, const char * pszDirName)
// Tries to display all files in a directory and its
// subdirectories.
{
  WIN32_FIND_DATA FindData;
  HANDLE hSearch;
  bool bOK;
  char sz[256];

  TRACE ("Searching directory %s.\n", pszDirName);

  displayFilesInDir (pDecoder, pszDirName, "*.bmp");
  displayFilesInDir (pDecoder, pszDirName, "*.eps");
  displayFilesInDir (pDecoder, pszDirName, "*.jpg");
  displayFilesInDir (pDecoder, pszDirName, "*.pct");
  displayFilesInDir (pDecoder, pszDirName, "*.png");
  displayFilesInDir (pDecoder, pszDirName, "*.tga");
  displayFilesInDir (pDecoder, pszDirName, "*.tif");
  displayFilesInDir (pDecoder, pszDirName, "*.pcx");
  displayFilesInDir (pDecoder, pszDirName, "*.gif");

  // Recurse directories

  sprintf (sz, "%s*.*", pszDirName);
  hSearch = FindFirstFile (sz, &FindData);
  bOK = (hSearch != INVALID_HANDLE_VALUE);
  while (bOK)
  {
    if (FindData.cFileName[0] != '.' &&
        (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      sprintf (sz, "%s%s\\", pszDirName, FindData.cFileName);
      displayDir (pDecoder, sz);
    }
    if (m_bStopDecoding)
      return;
    bOK = (FindNextFile (hSearch, &FindData) != 0);
  }
  FindClose (hSearch);

}

void CDibtestDlg::displayFilesInDir (PLPicDecoder * pDecoder, const char * pszDirName,
                                     const char * pszFName)
// Tries to display all files in a directory matching
// the specified file name.
{
  WIN32_FIND_DATA FindData;
  HANDLE hSearch;
  bool bOK;
  char sz[256];

  sprintf (sz, "%s%s", pszDirName, pszFName);

  hSearch = FindFirstFile (sz, &FindData);
  bOK = (hSearch != INVALID_HANDLE_VALUE);
  while (bOK)
  {
    if (FindData.cFileName[0] != '.' &&
        !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      sprintf (sz, "%s%s", pszDirName, FindData.cFileName);
      displayFile (pDecoder, sz);
    }
    if (m_bStopDecoding)
      return;
    bOK = (FindNextFile (hSearch, &FindData) != 0);
  }
  FindClose (hSearch);
}

void CDibtestDlg::displayFile (PLPicDecoder * pDecoder, char * pszFName)
{
#ifdef TEST_DDRAW
  PLDDrawBmp NewBmp (m_pDDraw);
#else
  PLWinBmp NewBmp;
#endif

  try
  {
    pDecoder->MakeBmpFromFile (pszFName, &NewBmp);
    if (m_bStopDecoding)
      return;
    EnterCriticalSection (&m_Critical);

    *m_pBmp = NewBmp;
    LeaveCriticalSection (&m_Critical);

    m_pDIBWnd->NewDIBNotify ();
    m_pDIBWnd->UpdateWindow ();

    CString s ("Decoding Image: ");
    s += pszFName;
    SetWindowText(s);
  }
  catch (PLTextException e)
  {
    char sz[1024];
    sprintf (sz, "Error decoding %s: %s", pszFName, (const char *)e);
    PLPicDecoder::Trace (1, sz);

  }
  catch (CMemoryException e)
  {
    TRACE ("Error decoding %s: Out of memory.\n", pszFName);
  }
}

void CDibtestDlg::doCreateCopyTest (PLPicDecoder * pDecoder)
{
  PLDIBSection TestBmp, TestBmp1;

  EnterCriticalSection (&m_Critical);

  pDecoder->MakeBmpFromResource (AfxFindResourceHandle(MAKEINTRESOURCE (IDR_MONOCHROMETIFF), ""),
                                 IDR_MONOCHROMETIFF, &TestBmp);

  PLBmpEncoder Enc;
  TestBmp1.CreateCopy (TestBmp, 8);
  Enc.MakeFileFromBmp("c:\\test8.bmp", &TestBmp1);
  TestBmp.CreateCopy (TestBmp1, 32);
  Enc.MakeFileFromBmp("c:\\test32.bmp", &TestBmp);
  TestBmp1.SetQuantizationMode(PLDTH_NONE, PLDTHPAL_MEDIAN);
  TestBmp1.CreateCopy (TestBmp, 8);
  TestBmp.CreateCopy (TestBmp1, 1);
  Enc.MakeFileFromBmp("c:\\test1.bmp", &TestBmp);
  TestBmp1.CreateCopy (TestBmp, 32);
  TestBmp.CreateCopy (TestBmp1, 1);
  TestBmp1.CreateCopy (TestBmp, 32);
  m_pBmp->AlphaBlt (&TestBmp1, 0, 0);

  LeaveCriticalSection (&m_Critical);

  m_pDIBWnd->NewDIBNotify ();
  m_pDIBWnd->UpdateWindow ();

  // Comment this out if you need to look at the results.
  remove ("c:\\test1.bmp");
  remove ("c:\\test8.bmp");
  remove ("c:\\test32.bmp");
}


void CDibtestDlg::doBmpTest (int Width, int Height, int bpp, int ID)
// Tests the bitmap class used for defects by calling its methods
// with one type of bitmap.
{
  // Create empty
  EnterCriticalSection (&m_Critical);
  m_pBmp->Create (Width, Height, bpp, false, false);
  LeaveCriticalSection (&m_Critical);

  m_pDIBWnd->NewDIBNotify ();
  m_pDIBWnd->UpdateWindow ();

  // Load from resource
  if (ID)
    m_pBigBmp->CreateRes (AfxGetInstanceHandle(), ID);
  else
    m_pBigBmp->Create (Width, Height, bpp, false, false);

  // Copy constructor
  {
    PLWinBmp WinBmp (*m_pBigBmp);
    PLDIBSection DIBSect (*m_pBigBmp);
//    PLAnyBmp AnyBmp (*m_pBigBmp);
  }

  // Assignment
  {
    EnterCriticalSection (&m_Critical);
    *m_pBmp = *m_pBigBmp;
    PLAnyBmp AnyBmp;
    AnyBmp = *m_pBigBmp;
    *m_pBigBmp = AnyBmp;
    LeaveCriticalSection (&m_Critical);
  }

  m_pDIBWnd->NewDIBNotify ();
  m_pDIBWnd->UpdateWindow ();

  // Put in clipboard
  m_pBmp->ToClipboard();

  HBITMAP hBmp;

  // Create a PLWinBmp from a HBITMAP
  hBmp = m_pBmp->GetHandle();
  PLWinBmp TestBmp;
  TestBmp.CreateFromHBitmap(hBmp);

  // PLDIBSection tests
  hBmp = m_pBmp->GetHandle();
  BITMAPINFOHEADER * pBMI = m_pBmp->GetBMI();
  m_pBmp->Detach();
  DeleteObject (hBmp);
  free (pBMI);

}

void CDibtestDlg::OnDestroy()
{
  CDialog::OnDestroy();

  m_bStopDecoding = true;

  // Wait for the decoder thread to terminate.
  m_pThreadCritical->Lock (5000);
}
/*
/--------------------------------------------------------------------
|
|      $Log: maindlg.cpp,v $
|      Revision 1.1  2009/12/09 05:34:58  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:12  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:24  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:36  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:32  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:52  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:12  황정영
|      2006.05.17
|
|      Revision 1.26  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.25  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.24  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.23  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.22  2001/01/15 15:05:32  uzadow
|      Added CBmp::ApplyFilter() and CBmp::CreateFilteredCopy()
|
|      Revision 1.21  2001/01/15 12:10:13  uzadow
|      Added gif to list of file types.
|
|      Revision 1.20  2000/11/21 20:27:28  uzadow
|      Added crop test.
|
|      Revision 1.19  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.18  2000/10/28 12:00:35  uzadow
|      Added AnyBmp copy constructor tests.
|
|      Revision 1.17  2000/09/26 12:14:50  Administrator
|      Refactored quantization.
|
|      Revision 1.16  2000/09/21 14:35:12  Administrator
|      Fixed bug in pcx decoder
|
|      Revision 1.15  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.14  2000/08/13 12:11:44  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.13  2000/07/19 12:23:47  Ulrich von Zadow
|      Added CreateFromHBitmap test.
|
|      Revision 1.12  2000/05/28 11:10:18  Ulrich von Zadow
|      Enhanced test routines.
|
|      Revision 1.11  2000/03/15 17:31:27  Ulrich von Zadow
|      Added resizing code and icon by
|      Amos Di Gennaro.
|
|      Revision 1.10  2000/01/18 00:03:01  Ulrich von Zadow
|      Bugs.
|
|      Revision 1.9  2000/01/17 23:38:08  Ulrich von Zadow
|      MFC removal aftermath.
|
|      Revision 1.8  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.7  2000/01/10 23:53:03  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
\--------------------------------------------------------------------
*/
