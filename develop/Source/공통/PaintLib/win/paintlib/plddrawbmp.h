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
|      $Id: plddrawbmp.h,v 1.1 2009/12/09 05:34:46 박종운 Exp $
|      Bitmap class using a directdraw surface.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_DDRAWBMP
#define INCL_DDRAWBMP

#include "plbitmap.h"

#include <ddraw.h>

//! 
class PLDDrawBmp : public PLBmp
{

public:
  //! Creates an empty bitmap.
  PLDDrawBmp
    ( IDirectDraw * pDDraw
    );

  //! Destroys the bitmap.
  virtual ~PLDDrawBmp
    ();

#ifdef _DEBUG
  virtual void AssertValid
    () const;    // Tests internal object state
#endif

  // PLDDrawBmp information

  //! Returns the amount of memory used by the object.
  virtual long GetMemUsed
    ();

  //! Returns number of bytes used per line.
  long GetBytesPerLine
    ();

  //! Returns the size of the bitmap in pixels
  SIZE GetSize
    ();

  //! Draws the bitmap.
//  virtual void Blt
//    ();

  //! Locks the DirectDraw surface and fills the line array. GetLineArray() 
  //! returns NULL if the surface isn't locked.
  virtual void Lock
    ( bool bReadable,
      bool bWriteable
    );

  //! Unlocks the DirectDraw surface.
  virtual void Unlock
    ();
 
  //! Gets a pointer to the directdraw surface used.
  IDirectDrawSurface * GetSurface
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

  //! Deletes memory allocated by member variables.
  virtual void freeMembers
    ();

  //! Initializes internal table of line addresses.
  virtual void initLineArray
    ();

  // Set color table pointer & pointer to bits
  virtual void initPointers
    ();

private:

  // Member variables.
  IDirectDrawSurface * m_pSurface;
  DDSURFACEDESC * m_pDDSurfaceDesc;
  IDirectDraw * m_pDDraw;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plddrawbmp.h,v $
|      Revision 1.1  2009/12/09 05:34:46  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:06  박대우
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
|      Revision 1.1  2006/11/29 03:34:45  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:58  황정영
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
|      Revision 1.1  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|
\--------------------------------------------------------------------
*/
