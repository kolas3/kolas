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
|      $Id: testpsddecoder.h,v 1.1 2009/12/09 05:34:03 박종운 Exp $
|
|      Copyright (c) 1996-1998 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLTESTPSDDECODER
#define INCL_PLTESTPSDDECODER

#include <string>

class PLTester;
class PLBmp;

class PLTestPSDDecoder
{

public:
  PLTestPSDDecoder ();

  virtual ~PLTestPSDDecoder();

  void RunTests ();

private:
  void test (const std::string& sFName);
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: testpsddecoder.h,v $
|      Revision 1.1  2009/12/09 05:34:03  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:49  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:13  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:03  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:29  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.3  2002/10/20 22:11:53  uzadow
|      Partial doc update.
|      Fixed tests when images were not available.
|
|      Revision 1.2  2001/10/21 17:54:40  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|
\--------------------------------------------------------------------
*/
