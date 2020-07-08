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
|      $Id: planybmp.h,v 1.1 2009/12/09 05:33:47 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLANYBMP
#define INCL_PLANYBMP

#ifndef INCL_PLBITMAP
#include "plbitmap.h"
#endif

//! Manipulates uncompressed device- and platform-independent bitmaps.
//! Supported data formats are 8 and 32 bpp. The data is stored
//! sequentially without padding in the bitmap. The class implements exactly
//! the interface defined by PLBmp without additions.
class PLAnyBmp : public PLBmp
{

public:
  //! Creates an empty bitmap.
  PLAnyBmp
    ();

  //! Destroys the bitmap.
  virtual ~PLAnyBmp
    ();

  //! Copy constructor
  PLAnyBmp
    ( const PLBmp &Orig
    );

  //! Copy constructor
  PLAnyBmp
    ( const PLAnyBmp &Orig
    );

  //! Assignment operator.
  PLAnyBmp &operator=
    ( const PLBmp &Orig
    );

  //! Assignment operator.
  PLAnyBmp &operator=
    ( const PLAnyBmp &Orig
    );

  // PLAnyBmp information

  //! Returns the amount of memory used by the object.
  long GetMemUsed
    ();

  //! Returns number of bytes used per line.
  long GetBytesPerLine
    ();


  // Static functions

  //! Returns memory needed by a bitmap with the specified attributes.
  static long GetMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    );

  //! Returns memory needed by bitmap bits.
  static long GetBitsMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    );


protected:
  // Local functions

  // Create a new bitmap with uninitialized bits. Assumes no memory
  // is allocated yet.
  virtual void internalCreate
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    );

  // Deletes memory allocated by member variables.
  virtual void freeMembers
    ();

  // Initializes internal table of line addresses.
  virtual void initLineArray
    ();

  PLBYTE      * m_pBits;        // Pointer to the bits.
};

// Note that _both_ these copy constructors are needed. If only the 
// second one is there, MSVC generates a default copy 
// constructor anyway :-(.
inline PLAnyBmp::PLAnyBmp
    ( const PLAnyBmp &Orig
    )
    : PLBmp ()
{
  internalCopy (Orig);
}

inline PLAnyBmp::PLAnyBmp
    ( const PLBmp &Orig
    )
    : PLBmp ()
{
  internalCopy (Orig);
}

inline PLAnyBmp & PLAnyBmp::operator=
    ( const PLBmp &Orig
    )
{
  PLBmp::operator=(Orig);
  return *this;
}

inline PLAnyBmp & PLAnyBmp::operator=
    ( const PLAnyBmp &Orig
    )
{
  PLBmp::operator=(Orig);
  return *this;
}


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: planybmp.h,v $
|      Revision 1.1  2009/12/09 05:33:47  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:56  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:19  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/02/24 13:00:17  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.10  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.9  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.8  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.7  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  1999/12/14 12:30:13  Ulrich von Zadow
|      Corrected copy constructor and assignment operator.
|
|      Revision 1.5  1999/12/10 01:27:26  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.4  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
\--------------------------------------------------------------------
*/
