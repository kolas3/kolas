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
|      $Id: plwinbmp.h,v 1.1 2009/12/09 05:34:49 박종운 Exp $
|
|      Copyright (c) 1996-1998 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_WINBMP
#define INCL_WINBMP

#ifndef INCL_BITMAP
#include "plbitmap.h"
#endif

//! This is the windows version of PLBmp. The internal storage format
//! is a windows DIB. It supports all color depths allowed by
//! windows: 1, 4, 8, 16, 24, and 32 bpp. The bits are always accessible:
//! m_LockCount is always > 0 and IsLocked always returns true.
//!
//! The subset of the windows DIB format supported is as follows: The
//! DIB is stored so that header, palette, and bits are in one
//! buffer. The bottom line is stored first (biHeight must be > 0)
//! and the data is uncompressed (BI_RGB). Color tables for 16, 24,
//! and 32 bpp are not supported. biClrUsed is always 0. The palette
//! mode is DIB_RGB_COLORS. DIB_PAL_COLORS is not supported.
//!
//! Note that almost all real-life DIBs conform to this subset
//! anyway, so there shouldn't be any problems.
//!
//! <i>In the current version, some functions (notably CreateCopy) only
//! support 1, 8 and 32 bpp. Sorry!</i>
class PLWinBmp : public PLBmp
{

public:
  // Creates an empty bitmap.
  PLWinBmp ();

  //! Copy constructor
  PLWinBmp
    ( const PLWinBmp &Orig
    );

  //! Copy constructor
  PLWinBmp
    ( const PLBmp &Orig
    );

  //! Destroys the bitmap.
  virtual ~PLWinBmp ();

  //! Assignment operator.
  PLWinBmp &operator= (PLBmp const &Orig);

  //! Assignment operator.
  PLWinBmp &operator= (PLWinBmp const &Orig);

#ifdef _DEBUG
  virtual void AssertValid () const;    // Tests internal object state
#endif

  // PLWinBmp manipulation

  // Do a bitblt using the alpha channel of pSrPLBmp. Restricted to
  // 32 bpp.
  // Legacy routine. Use the Blt classes instead.
  void AlphaBlt (PLWinBmp * pSrPLBmp, int x, int y);

  // PLWinBmp information

  //! Returns the amount of memory used by the object.
  virtual long GetMemUsed ();

  //! Returns number of bytes used per line.
  virtual long GetBytesPerLine ();

  // Windows-specific interface

  //! Loads a bitmap from a windows resource (.rc or .res linked to
  //! the exe). Fails if the bitmap is compressed.
  virtual void CreateRes (HINSTANCE lh_ResInst, int ID);

  //! Takes a HBITMAP and converts it to a PLWinBmp.
  void CreateFromHBitmap (HBITMAP hBitMap);

  //! Takes an existing device-independent bitmap and converts it
  //! to a PLWinBmp.
  void CreateFromHDIBBitmap(BITMAPINFOHEADER* pBIH, HPALETTE hPal = NULL);

  //! Returns the size of the bitmap in pixels
  SIZE GetSize ();

  //! Access the windows bitmap structure. Using this structure, all
  //! standard DIB manipulations can be performed.
  BITMAPINFOHEADER * GetBMI ();

  // PLWinBmp output

  //! Draws the bitmap on the given device context using
  //! StretchDIBits.
  virtual void Draw (HDC hDC, int x, int y, DWORD rop = SRCCOPY);

  //! Draws the bitmap on the given device context using
  //! StretchDIBits. Scales the bitmap by Factor.
  virtual void StretchDraw (HDC hDC, int x, int y, double Factor, DWORD rop = SRCCOPY);

  //! Draws the bitmap on the given device context using
  //! StretchDIBits. Scales the bitmap so w is the width and
  //! h the height.
  virtual void StretchDraw (HDC hDC, int x, int y, int w, int h, DWORD rop = SRCCOPY);

  //! Draws a portion of the bitmap on the given device context
  virtual BOOL DrawExtract (HDC hDC, POINT pntDest, RECT rcSrc);

  //! Puts a copy of the bitmap in the clipboard
  void ToClipboard ();

  //! Reads the clipboard into the bitmap. uFormat can be either
  //! CF_BITMAP or CF_DIB.
  bool FromClipboard (UINT uFormat = CF_BITMAP);

  //! Gets a pointer to the bitmap bits. (Usually, using GetLineArray()
  //! is much easier!)
  BYTE * GetBits ();

  //! Copies the palette over from pSrPLBmp.
  void CopyPalette (PLWinBmp * pSrPLBmp);

  //! Traces the values in the palette via PLTRACE();
  void TracePalette();

  // Static functions

  //! Returns memory needed by a bitmap with the specified attributes.
  static long GetMemNeeded (LONG width, LONG height, WORD BitsPerPixel);

  //! Returns memory needed by bitmap bits.
  static long GetBitsMemNeeded (LONG width, LONG height, WORD BitsPerPixel);

  //! Returns memory needed by one line.
  static int GetLineMemNeeded (LONG width, WORD BitsPerPixel);

protected:

  // Protected callbacks

  //! Create a new empty DIB. Bits are uninitialized.
  //! Assumes that no memory is allocated before the call.
  virtual void internalCreate (LONG Width, LONG Height, WORD BitsPerPixel, 
      bool bAlphaChannel, bool bIsGreyscale);

  //! Creates a PLWinBmp from an existing bitmap pointer.
  //! Assumes that no memory is allocated before the call.
  virtual void internalCreate (BITMAPINFOHEADER* pBMI);

  //! Deletes memory allocated by member variables.
  virtual void freeMembers ();

  //! Initializes internal table of line addresses.
  virtual void initLineArray ();

  // Creates a copy of the current bitmap in a global memory block
  // and returns a handle to this block.
  virtual HANDLE createCopyHandle ();

  // Set color table pointer & pointer to bits based on m_pBMI.
  virtual void initPointers ();

  // Member variables.

  BITMAPINFOHEADER * m_pBMI;  // Pointer to picture format information.
  BYTE * m_pBits;


private:
};


// Note that _both_ these copy constructors are needed. If only the 
// second one is there, the compiler generates a default copy 
// constructor anyway :-(.
inline PLWinBmp::PLWinBmp
    ( const PLWinBmp &Orig
    )
    : PLBmp ()
{
  internalCopy (Orig);
}

inline PLWinBmp::PLWinBmp
    ( const PLBmp &Orig
    )
    : PLBmp ()
{
  internalCopy (Orig);
}

inline PLWinBmp & PLWinBmp::operator= ( PLBmp const &Orig)
{ 
  PLBmp::operator=(Orig);
  return *this;
}

inline PLWinBmp & PLWinBmp::operator= ( PLWinBmp const &Orig)
{
  PLBmp::operator=(Orig);
  return *this;
}


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plwinbmp.h,v $
|      Revision 1.1  2009/12/09 05:34:49  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:08  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:44  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:30  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:09  황정영
|      2006.05.17
|
|      Revision 1.2  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.14  2001/01/14 13:36:15  uzadow
|      Added PLAnyPicDecoder::GetFileFormat()
|
|      Revision 1.13  2000/11/21 20:29:39  uzadow
|      Added test project.
|
|      Revision 1.12  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.11  2000/10/12 21:59:34  uzadow
|      Added CreateFromHDIBBitmap() and CopyPalette() to PLWinBmp
|      Added CF_DIB support to PLWinBmp::FromClipboard() (Richard Hollis)
|
|      Revision 1.10  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.9  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.8  2000/01/17 23:37:12  Ulrich von Zadow
|      Corrected bug in assignment operator.
|
|      Revision 1.7  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.6  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
\--------------------------------------------------------------------
*/
