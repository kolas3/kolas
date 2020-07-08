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
|      $Id: maindlg.h,v 1.1 2009/12/09 05:34:58 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#include <afxmt.h>

#include "plwinbmp.h"
#include "mtdibwnd.h"
#include "plpicdec.h"
#include "pldibsect.h"

#ifdef TEST_DDRAW
#include <ddraw.h>
#endif

class CDibtestDlg : public CDialog
{
// Construction
public:
  CDibtestDlg
    ( char * pszDirName
    );

  ~CDibtestDlg
    ();

  void DoWork
    ();

// Dialog Data
  //{{AFX_DATA(CDibtestDlg)
  enum { IDD = IDD_DIBTEST_DIALOG };
          // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDibtestDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  HICON m_hIcon;

  // Generated message map functions
  //{{AFX_MSG(CDibtestDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  void displayDir
    ( PLPicDecoder * pDecoder,
      const char * pszDirName
    );
    // Tries to display all files in a directory and its
    // subdirectories.

  void displayFilesInDir
    ( PLPicDecoder * pDecoder,
      const char * pszDirName,
      const char * pszFName
    );
    // Tries to display all files in a directory matching
    // the specified file name.

  void displayFile
    ( PLPicDecoder * pDecoder,
      char * pszFName
    );
    // Tries to load a file & display it. Does error checking.

  void doCreateCopyTest
    ( PLPicDecoder * pDecoder
    );

  void doBmpTest
    ( int Width,
      int Height,
      int bpp,
      int ID
    );
    // Tests the CBmp class for defects by calling its methods
    // with one type of bitmap.

  PLDIBSection * m_pBmp;
  PLWinBmp     * m_pBigBmp;

  CMTDIBWnd   * m_pDIBWnd;

  CString       m_sDirName;

  CRITICAL_SECTION   m_Critical;
  CWinThread       * m_pThread;
  bool               m_bStopDecoding;
  CCriticalSection * m_pThreadCritical;

#ifdef TEST_DDRAW
  IDirectDraw * m_pDDraw;
#endif
};
/*
/--------------------------------------------------------------------
|
|      $Log: maindlg.h,v $
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
|      Revision 1.1  2007/04/06 09:31:33  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:53  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:02  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:12  황정영
|      2006.05.17
|
|      Revision 1.6  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.5  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/08/13 12:11:44  Administrator
|      Added experimental DirectDraw-Support
|
|
--------------------------------------------------------------------
*/
