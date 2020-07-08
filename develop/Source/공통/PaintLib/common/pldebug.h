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
|      $Id: pldebug.h,v 1.1 2009/12/09 07:29:45 박종운 Exp $
|
|      Plattform-independent support for PLASSERT_VALID, PLTRACE and
|      PLASSERT.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLDEBUG
#define INCL_PLDEBUG

//------------- PLASSERT_VALID
#ifdef _DEBUG
  #define PLASSERT_VALID(pOb) (pOb)->AssertValid()
#else
  #define PLASSERT_VALID(pOb) do{} while (0)
#endif

//------------- TRACE
#ifdef _DEBUG
  void PLTrace(const char * pszFormat, ...);
  #define PLTRACE ::PLTrace
#else
  // This will be optimized away in release mode and still allow TRACE
  // to take a variable amount of arguments :-).
  inline void PLTrace (const char *, ...) { }
  #define PLTRACE  1 ? (void)0 : ::PLTrace
#endif

//------------- ASSERT

#ifdef _DEBUG
  #ifdef _WINDOWS
    #define PLASSERT(f)            \
      if (!(f))                    \
        {                          \
          PLTRACE ("Assertion failed at %s, %i\n", __FILE__, __LINE__); \
          __asm { int 3 }          \
        }
  #else
    #define PLASSERT(f)            \
      if (!(f))                    \
        {                          \
          PLTRACE ("Assertion failed at %s, %i\n", __FILE__, __LINE__); \
          abort();                 \
        }
  #endif
#else
  #define PLASSERT(f) do{}while (0)
#endif


//------------- CompilerAssert template for conditional
//              compile time error generation.

#define PLCOMPILER_ASSERT(cond) (void)sizeof(int[bool(cond)?1:-1]);


#endif // INCL_PLDEBUG

/*
/--------------------------------------------------------------------
|
|      $Log: pldebug.h,v $
|      Revision 1.1  2009/12/09 07:29:45  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:14  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:57  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:53  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:03  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:21  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:25  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:31  황정영
|      2006.05.17
|
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/12/04 23:55:40  uzadow
|      no message
|
|      Revision 1.2  2000/12/04 13:28:17  uzadow
|      Changed PLASSERT to use int 3 in windows builds.
|
|      Revision 1.1  2000/01/17 23:45:07  Ulrich von Zadow
|      MFC-Free version.
|
|
\--------------------------------------------------------------------
*/
