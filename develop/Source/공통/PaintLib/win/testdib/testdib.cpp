/*
/--------------------------------------------------------------------
|
|      $Id: testdib.cpp,v 1.1 2009/12/09 05:35:04 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "testdib.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDibtestApp

BEGIN_MESSAGE_MAP(CDibtestApp, CWinApp)
//{{AFX_MSG_MAP(CDibtestApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibtestApp construction

CDibtestApp::CDibtestApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDibtestApp object


CDibtestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDibtestApp initialization

BOOL CDibtestApp::InitInstance()
{
  // Standard initialization

  CDibtestDlg dlg (m_lpCmdLine);
  m_pMainWnd = &dlg;
  int nResponse = dlg.DoModal();

  return false;
}
/*
/--------------------------------------------------------------------
|
|      $Log: testdib.cpp,v $
|      Revision 1.1  2009/12/09 05:35:04  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:28  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:32:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:35:13  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:16  황정영
|      2006.05.17
|
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/01/10 23:53:03  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
--------------------------------------------------------------------
*/
