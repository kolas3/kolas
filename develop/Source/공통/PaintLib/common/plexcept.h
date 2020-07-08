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
|      $Id: plexcept.h,v 1.1 2009/12/09 05:33:50 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLEXCEPT
#define INCL_PLEXCEPT


// Error codes
#define PL_ERROK              0            // No error
#define PL_ERRWRONG_SIGNATURE 1            // Expected file signature
                                           // not found.
#define PL_ERRFORMAT_UNKNOWN  2            // Sanity check failed.

#define PL_ERRPATH_NOT_FOUND  3            // <--
#define PL_ERRFILE_NOT_FOUND  4            // <-- Problems with the file
#define PL_ERRACCESS_DENIED   5            // <-- system.

#define PL_ERRFORMAT_NOT_SUPPORTED 6       // Known but unsupported
                                           // format.
#define PL_ERRINTERNAL        7            // Internal error of some sort.
#define PL_ERRUNKNOWN_FILE_TYPE    8       // Couldn't recognize the
                                           // file type.
#define PL_ERRDIB_TOO_LARGE   9            // Maximum size for 1 bmp was
                                           // exceeded.
#define PL_ERRNO_MEMORY      10            // Out of memory.
#define PL_ERREND_OF_FILE    11            // End of file reached before
                                           // end of image.
#define PL_ERRFORMAT_NOT_COMPILED 12       // Support for this file format
                                           // was #ifdef'ed out.

//! An object of this class is thrown by other classes when an error
//! occurs. It contains an error code and a string describing the
//! error. The error code is meant to be used internally in the
//! program; the descriptive string can be output to the user. Error
//! codes and strings do not correspond 1:1. The strings are more
//! precise.
class PLTextException : public PLObject
{

public:
  //! Creates an exception.
  PLTextException
    ( int Code,
      const char * pszErr
    );

  //! Copy constructor.
  PLTextException
    ( const PLTextException& ex
    );

  //! Destroys an exception
  virtual ~PLTextException
    ();

  //! Returns the code of the error that caused the exception. Valid
  //! error codes are:
  //!
  //! PL_ERRWRONG_SIGNATURE (1): Expected file signature not found.
  //!
  //! PL_ERRFORMAT_UNKNOWN (2): Unexpected data encountered. This
  //! probably indicates a corrupt file or an unknown file
  //! sub-format.
  //!
  //! PL_ERRPATH_NOT_FOUND (3), PL_ERRFILE_NOT_FOUND (4),
  //! PL_ERRACCESS_DENIED (5): Problems with the file system.
  //!
  //! PL_ERRFORMAT_NOT_SUPPORTED (6): Known but unsupported format.
  //!
  //! PL_ERRINTERNAL (7): Kaputt. Tell me about it.
  //!
  //! PL_ERRUNKNOWN_FILE_TYPE (8): Couldn't recognize the file type.
  //!
  //! PL_ERRDIB_TOO_LARGE (9): Maximum size for 1 bmp was exceeded.
  //! (See MAX_BITMAP_SIZE above for an explanation).
  //!
  //! PL_ERRNO_MEMORY (10): Out of memory.
  //!
  //! PL_ERREND_OF_FILE (11): End of file reached before end of image.
  virtual int GetCode
    ()
    const;


  //! This operator allows the exception to be treated as a string
  //! whenever needed. The string contains the error message.
  virtual operator const char *
    ()
    const;

private:
  int     m_Code;
  char *  m_pszErr;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plexcept.h,v $
|      Revision 1.1  2009/12/09 05:33:50  박종운
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
|      Revision 1.4  2002/11/18 14:44:46  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.3  2002/08/07 18:50:38  uzadow
|      More cygwin compatibility changes.
|
|      Revision 1.2  2002/03/03 16:29:55  uzadow
|      Re-added BPPWanted.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/01/17 23:38:07  Ulrich von Zadow
|      MFC removal aftermath.
|
|
\--------------------------------------------------------------------
*/
