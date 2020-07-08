/*
/--------------------------------------------------------------------
|
|      $Id: pltest.cpp,v 1.1 2009/12/09 05:34:01 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "pltest.h"
#include "pldebug.h"

PLTest::PLTest(void)
: m_bOk(true),
  m_NumSucceeded(0),
  m_NumFailed(0)
{
}

PLTest::~PLTest(void)
{
}

// TODO: Make this into a macro that prints out the test that failed.
void PLTest::Test (bool b)
{
  if (b)
  {
    m_NumSucceeded++;
  }
  else
  {
    PLTRACE ("           ---->> failed\n");
    m_bOk = false;
    m_NumFailed++;
  }
// Comment in to stop when a test fails
// PLASSERT (b);
}


bool PLTest::IsOk ()
{
  return m_bOk;
}

void PLTest::SetFailed ()
{
  m_NumFailed++;
  m_bOk = false;
}

int PLTest::GetNumSucceeded () const
{
  return m_NumSucceeded;
}

int PLTest::GetNumFailed() const
{
  return m_NumFailed;
}

void PLTest::AggregateStatistics (const PLTest& ChildTest)
{
  m_NumSucceeded += ChildTest.GetNumSucceeded();
  m_NumFailed += ChildTest.GetNumFailed();
}

/*
/--------------------------------------------------------------------
|
|      $Log: pltest.cpp,v $
|      Revision 1.1  2009/12/09 05:34:01  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:47  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:09  PDJ
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
|      Revision 1.5  2002/10/20 22:11:53  uzadow
|      Partial doc update.
|      Fixed tests when images were not available.
|
|      Revision 1.4  2002/08/07 20:32:31  uzadow
|      Updated Makefile.in, fixed psd decoder layer stuff.
|
|      Revision 1.3  2002/08/06 20:13:37  uzadow
|      Cross-platform stuff.
|
|      Revision 1.2  2002/08/04 21:20:42  uzadow
|      no message
|
|      Revision 1.1  2002/08/04 20:12:34  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|      Major improvements in tests.
|
|
|
\--------------------------------------------------------------------
*/
