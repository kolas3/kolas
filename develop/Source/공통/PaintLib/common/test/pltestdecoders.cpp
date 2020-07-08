/*
/--------------------------------------------------------------------
|
|      $Id: pltestdecoders.cpp,v 1.1 2009/12/09 05:34:02 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "config.h"
#include "pltestdecoders.h"
#include "plpaintlibdefs.h"
#include "planybmp.h"

#include "planydec.h"
#include "testpsddecoder.h"

#include <string>

using namespace std;

#ifdef WIN32
const char szTestFileDir[] = "..\\..\\testpic\\";
#else
#include <sys/stat.h>

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/types.h>
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include <fcntl.h>
#include <unistd.h>

const char szTestFileDir[] = "testpic/";
#endif

//#define CREATE_TEST_DATA
//#define INCLUDE_SUBDIRS

PLTestDecoders::PLTestDecoders ()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_InfoFile = NULL;
}

PLTestDecoders::~PLTestDecoders ()
{
}

void PLTestDecoders::RunTests ()
{
  PLTRACE ("Running Decoder tests...\n");
  char szTestFileName[256];
  strcpy (szTestFileName, szTestFileDir);
  strcat (szTestFileName, "TestBmpList.txt");
#ifdef CREATE_TEST_DATA
  m_InfoFile = fopen (szTestFileName, "w");
  getDirInfo ("");
#else
  //TODO: Get length of file and allocate enough memory.
  char * pszInfoFile = new char[65535];
  m_InfoFile = fopen (szTestFileName, "r");
  if (m_InfoFile == 0)
  {
    PLTRACE ("Could not open test image information file %s.\n", szTestFileName);
    PLTRACE ("No decoder tests performed.\n");
    SetFailed();
  }
  else
  {
    int BytesRead = fread (pszInfoFile, 1, 65535, m_InfoFile);
    fclose (m_InfoFile);
    pszInfoFile[BytesRead] = 0;
    string sInfoFile(pszInfoFile);
    bool bDone = false;
    int EOLPos = 0;
    while (!bDone)
    {
      int LastEOLPos = EOLPos;
      EOLPos = sInfoFile.find ('\n', EOLPos+1);
      if (EOLPos != string::npos)
      {
        string sCurLine = sInfoFile.substr(LastEOLPos, EOLPos-LastEOLPos);
        EOLPos+=1;
        testImage (sCurLine);
      }
      else
      {
        bDone = true;
      }
    }
    PLTRACE ("Testing PSD decoder...\n");
    PLTestPSDDecoder PSDTester;
    PSDTester.RunTests();
  }
#endif
}

#ifndef WIN32
// this function is used to select all files that do not start with a dot
int selectFile(const struct dirent * theEntry)
{
  if (theEntry->d_name[0] == '.')
  {
    return 0;
  }
  return 1;
}
#endif

void PLTestDecoders::getDirInfo (const char * pszDirName)
{
  PLTRACE ("Searching directory %s%s.\n", szTestFileDir, pszDirName);

#ifdef WIN32
  WIN32_FIND_DATA FindData;

  char sz[256];
  sprintf (sz, "%s%s*.*", szTestFileDir, pszDirName);
  HANDLE hSearch = FindFirstFile (sz, &FindData);
  bool bOK = (hSearch != INVALID_HANDLE_VALUE);
  while (bOK)
  {
    if (FindData.cFileName[0] != '.')
      if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      { // Recurse directories
#ifdef INCLUDE_SUBDIRS
        sprintf (sz, "%s%s\\", pszDirName, FindData.cFileName);
        getDirInfo (sz);
#endif
      }
      else
      {
        addToInfoFile (pszDirName, FindData.cFileName);
      }
    bOK = (FindNextFile (hSearch, &FindData) != 0);
  }
  FindClose (hSearch);
#else
  struct dirent **namelist;
  int SourceFileCount = scandir(pszDirName, &namelist, selectFile, alphasort);
  for (int i=0; i < SourceFileCount; ++i)
  {
    addToInfoFile (pszDirName, namelist[i]->d_name);
  }
#endif
}

void PLTestDecoders::addToInfoFile (const char * pszDirName, const char * pszFName)
{
  PLAnyPicDecoder Decoder;
  try
  {
    char szFullName[1024];
    sprintf(szFullName, "%s%s%s", szTestFileDir, pszDirName, pszFName);
    Decoder.OpenFile (szFullName);
    PLAnyBmp Bmp;
    Decoder.MakeBmp(&Bmp);
    char sz[256];
    Decoder.AsString (sz, 256);
    fprintf (m_InfoFile, "%s%s: %s\n", pszDirName, pszFName, sz);
    Decoder.Close();
  }
  catch (PLTextException e)
  {
    if (e.GetCode() != PL_ERRACCESS_DENIED &&
        e.GetCode() != PL_ERRUNKNOWN_FILE_TYPE)
    {
      fprintf (m_InfoFile, "%s%s: { Error, code: %i}\n", pszDirName, pszFName, e.GetCode());
      Decoder.Close();
    }
  }
}

void PLTestDecoders::testImage (string sLine)
{
#ifndef WIN32
  // Remove \r if it's there
  int CRPos = sLine.find("\r");
  if (CRPos != string::npos)
  {
    sLine = sLine.substr(0, CRPos);
  }
#endif

  int ColonPos = sLine.find(':');
  string sFName = sLine.substr(0, ColonPos);
  PLTRACE ("    %s\n", sFName.c_str());
  string sExpectedInfo = sLine.substr(ColonPos+2);

  PLAnyPicDecoder Decoder;
  char szRealInfo[256];
  try
  {
    PLAnyBmp Bmp;
    Decoder.MakeBmpFromFile ((string(szTestFileDir)+sFName).c_str(), &Bmp);
    Bmp.AsString (szRealInfo, 256);
    Test (sExpectedInfo == szRealInfo);

    Decoder.OpenFile ((string(szTestFileDir)+sFName).c_str());
    Decoder.AsString (szRealInfo, 256);
    // The gif decoder doesn't give correct alpha info before the file is
    // actually decoded :-(.
    if (sFName.substr (sFName.find ("."),4) == ".gif")
    {
      int AlphaPos = sExpectedInfo.find ("Alpha:");
      sExpectedInfo.replace (AlphaPos, 8, "Alpha: 0");
    }
    Test (sExpectedInfo == szRealInfo);
    Decoder.MakeBmp(&Bmp);
    Decoder.Close();
  }
  catch (PLTextException &e)
  {
    if (e.GetCode() == PL_ERRFORMAT_NOT_COMPILED)
    {
      PLTRACE ("      --> Test skipped.\n");
    }
    else
    {
      sprintf (szRealInfo, "{ Error, code: %i}", e.GetCode());
      Test (sExpectedInfo == szRealInfo);
      Decoder.Close();
    }
  }
}


/*
/--------------------------------------------------------------------
|
|      $Log: pltestdecoders.cpp,v $
|      Revision 1.1  2009/12/09 05:34:02  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:48  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:10  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:01  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:28  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.13  2002/11/18 14:45:37  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.12  2002/11/06 22:47:00  uzadow
|      *** empty log message ***
|
|      Revision 1.11  2002/11/04 22:40:13  uzadow
|      Updated for gcc 3.1
|
|      Revision 1.10  2002/11/04 21:12:36  uzadow
|      *** empty log message ***
|
|      Revision 1.9  2002/11/02 23:03:29  uzadow
|      Fixed BmpInfo handling for transparent gifs
|
|      Revision 1.8  2002/11/02 22:38:28  uzadow
|      Fixed \\r handling in TestBmpList.txt, removed libtool from cvs
|
|      Revision 1.7  2002/10/20 22:11:53  uzadow
|      Partial doc update.
|      Fixed tests when images were not available.
|
|      Revision 1.6  2002/08/09 22:43:46  uzadow
|      no message
|
|      Revision 1.5  2002/08/07 18:50:39  uzadow
|      More cygwin compatibility changes.
|
|      Revision 1.4  2002/08/06 20:13:37  uzadow
|      Cross-platform stuff.
|
|      Revision 1.3  2002/08/05 20:43:12  uzadow
|      cygwin compatibility changes
|
|      Revision 1.2  2002/08/04 21:20:42  uzadow
|      no message
|
|      Revision 1.1  2002/08/04 20:12:34  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from
|      images without loading the whole image and proper greyscale support.
|      Major improvements in tests.
|
|
|
\--------------------------------------------------------------------
*/
