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
|      $Id: pldibsect.h,v 1.1 2009/12/09 05:34:47 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_DIBSECT
#define INCL_DIBSECT

#ifndef INCL_WINBMP
#include "plwinbmp.h"
#endif

//! This is a windows DIBSection wrapped in a PLBmp-derived class.
//! It can be used just like a PLWinBmp can be used. In addition,
//! PLDIBSection can give access to the bitmap as a GDI bitmap handle.
//! This bitmap handle can be selected into a device context. All
//! normal GDI drawing functions can be used to write on the bitmap
//! in this way.
//!
//! Internally, PLDIBSections are stored with header and bits in two
//! separate buffers.
class PLDIBSection : public PLWinBmp
{

public:
  //! Creates an empty bitmap.
  PLDIBSection
    ();

  //! Destroys the bitmap.
  virtual ~PLDIBSection
    ();

  //! Copy constructor
  PLDIBSection
    ( const PLBmp &Orig
    );

  //! Copy constructor
  PLDIBSection
    ( const PLDIBSection &Orig
    );

  //! Assignment operator.
  PLDIBSection &operator=
    ( PLBmp const &Orig
    );

  //! Assignment operator.
  PLDIBSection &operator=
    ( PLDIBSection const &Orig
    );

#ifdef _DEBUG
  virtual void AssertValid
    () const;    // Tests internal object state
#endif

  //! Calling this function causes the windows DIBSection to be detached
  //! from the PLDIBSection object. The bitmap data are not deleted in
  //! this function. This means that the bitmap handle and
  //! the bitmap memory (bits and BMI) must be deleted by some other object.
  //! The PLDIBSection object is in the same state as after a constructor
  //! call after this function is called.
  virtual void Detach
    ();

  // PLDIBSection output

  //! Draws the bitmap on the given device context using
  //! BitBlt.
  virtual void Draw
    ( HDC hDC,
      int x,
      int y,
      DWORD rop = SRCCOPY
    );

  //! Draws a portion of the bitmap on the given device context
  virtual BOOL DrawExtract
    ( HDC hDC,
      POINT pntDest,
      RECT rcSrc
    );

  // PLDIBSection member access

  //! Returns a GDI handle to the bitmap. This handle can be selected
  //! into a DC and used in normal GDI operations.
  //! Under Windows NT, GDI operations can be queued. This means that
  //! a program running under NT must call GdiFlush() before the
  //! DIBSection can be used again after GetHandle() has been called.
  //! See the documentation for GdiFlush() for details.
  HBITMAP GetHandle
    ();


protected:

  // Protected callbacks

  //! Create a new empty DIB. Bits are uninitialized.
  //! Assumes that no memory is allocated before the call.
  virtual void internalCreate
    ( LONG Width,
      LONG Height,
      WORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    );

  // Creates a PLDIBSection from an existing bitmap pointer.
  // Assumes that no memory is allocated before the call.
  virtual void internalCreate
    ( BITMAPINFOHEADER* pBMI
    );

  //! Deletes memory allocated by member variables.
  virtual void freeMembers
    ();

  //! Creates a copy of the current bitmap in a global memory block
  //! and returns a handle to this block.
  virtual HANDLE createCopyHandle
    ();

  //! Set color table pointer & pointer to bits based on m_pBMI.
  virtual void initPointers
    ();


private:
  // Local functions

  // Member variables.
  HBITMAP  m_hBitmap;

  bool     m_bOwnsBitmap;
};

// Note that _both_ these copy constructors are needed. If only the 
// second one is there, the compiler generates a default copy 
// constructor anyway :-(.
inline PLDIBSection::PLDIBSection
    ( const PLDIBSection &Orig
    )
{
  // Delete everything the base class allocated.
  free(m_pBMI);
  m_pBMI = NULL;

  delete [] m_pLineArray;
  m_pLineArray = NULL;

  internalCopy (Orig);
}

inline PLDIBSection::PLDIBSection
    ( const PLBmp &Orig
    )
{
  // Delete everything the base class allocated.
  free(m_pBMI);
  m_pBMI = NULL;

  delete [] m_pLineArray;
  m_pLineArray = NULL;

  internalCopy (Orig);
}


inline PLDIBSection & PLDIBSection::operator=
    ( PLBmp const &Orig
    )
{
  PLBmp::operator=(Orig);
  return *this;
}

inline PLDIBSection & PLDIBSection::operator=
    ( PLDIBSection const &Orig
    )
{
  PLBmp::operator=(Orig);
  return *this;
}


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: pldibsect.h,v $
|      Revision 1.1  2009/12/09 05:34:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:23  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:46  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.3  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.12  2001/01/21 14:28:22  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.11  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.10  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.9  2000/07/19 12:23:15  Ulrich von Zadow
|      Changed HANDLE to HBITMAP.
|
|      Revision 1.8  2000/07/07 13:20:03  Ulrich von Zadow
|      Comments
|
|      Revision 1.7  2000/01/17 23:37:12  Ulrich von Zadow
|      Corrected bug in assignment operator.
|
|      Revision 1.6  2000/01/16 20:43:17  anonymous
|      Removed MFC dependencies
|
|
\--------------------------------------------------------------------
*/
