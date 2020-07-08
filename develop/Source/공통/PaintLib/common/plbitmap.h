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
|      $Id: plbitmap.h,v 1.1 2009/12/09 05:33:47 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBITMAP
#define INCL_PLBITMAP

#include "plbmpinfo.h"
#include "plpoint.h"
#include "pldebug.h"
#include "plpixel32.h"
#include "plpixel24.h"

class PLFilter;

//! Device- and OS-independent bitmap class. Manipulates uncompressed
//! bitmaps of all color depths.
//!
//! This class is an abstract base class. It exists to define a
//! format-independent interface for bitmap manipulation and to
//! provide common routines. Derived classes must support at least
//! the color depths 1, 8 and 32 bpp. PLBmp defines a public interface
//! for general use and a protected interface for use by derived
//! classes.
//!
//! For 32 bpp, alpha channel information is stored in one byte
//! (PL_RGBA_ALPHA) of each 4-byte pixel. To allow for optimizations
//! when no alpha channel is present, a flag is set whenever the
//! alpha information is valid. The complete alpha channel of a
//! bitmap can be replaced by a different one by calling
//! SetAlphaChannel(). A 0 in an alpha channel entry is completely
//! transparent; a 255 is completely opaque.
class PLBmp : public PLBmpInfo
{

public:

  //! Empty constructor. Constructors in derived classes create a
  //! small empty bitmap to ensure that the object is always in a
  //! sane state.
  PLBmp
    ();

  //! Empty destructor.
  virtual ~PLBmp
    ();

  //! Assignment operator. Note that assignment between different derived
  //! classes is possible and results in a format conversion.
  PLBmp &operator=
    ( PLBmp const &Orig
    );

  //! Test for equality. This function actually tests every pixel, so
  //! it's not fast. It's meant mainly for use in asserts and such.
  bool const operator ==
    ( PLBmp const &Other
    );

#ifdef _DEBUG
  virtual void AssertValid
    () const;    // Tests internal object state
#endif

  // PLBmp creation

  //! Creates a new empty bitmap. Memory for the bits is allocated
  //! but not initialized. Previous contents of the bitmap object are
  //! discarded. If bAlphaChannel is true, the bitmap is assumed to
  //! contain a valid alpha channel.
  virtual void Create
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale, 
      const PLPoint& Resolution = PLPoint (0,0)
    );

  virtual void Create
    ( const PLBmpInfo& Info 
    );

  //! Creates a copy of rSrPLBmp, converting color depth if nessesary.
  //! Supports 1, 8 and 32 BPP. Alpha channel information is preserved.
  void CreateCopy
    ( const PLBmp & rSrPLBmp,
      int BPPWanted = 0
    );

  //! Creates a copy of rSrPLBmp, applying rFilter on the way. Depending
  //! on the filter called, this is often much faster than CreateCopy()
  //! followed by ApplyFilter().
  void CreateFilteredCopy (PLBmp & rSrPLBmp, const PLFilter & rFilter);

  // PLBmp manipulation

  //! Sets quality of conversion to 8 bpp. Valid parameters are
  //! defined in FilterQuantize.h.
  void SetQuantizationMode
    ( int DitherType,
      int DitherPaletteType
    );

  //! Fills the color table with a grayscale palette. This function
  //! is only useable for bitmaps containing a color table. Index 0
  //! contains black (0) and the last index contains white (255). The
  //! alpha channel is set to opaque (255) for every palette entry.
  void SetGrayPalette
    ();

  //! Sets the color table to pPal. The contents or pPal are copied.
  void SetPalette
    ( PLPixel32 * pPal
    );

  //! Sets one entry in the color table. The function may only be
  //! called if there is a color table stored with the bitmap. The
  //! color table entry is set to the red, green, blue, and alpha
  //! values specified.
  void SetPaletteEntry
    ( PLBYTE Entry,
      PLBYTE r,
      PLBYTE g,
      PLBYTE b,
      PLBYTE a
    );

  //! Sets one entry in the color table. The function may only be
  //! called if there is a color table stored with the bitmap. The
  //! color table entry is set to the red, green, blue, and alpha
  //! values specified.
  void SetPaletteEntry
    ( PLBYTE Entry,
      PLPixel32 Value
    );

  //! Replaces the alpha channel of the bitmap with a new one. This
  //! only works for bitmaps with 32 bpp. pAlphaBmp must point to an
  //! 8 bpp bitmap with the same dimensions as the object. The alpha
  //! channel information is physically copied into the bitmap.
  void SetAlphaChannel
    ( PLBmp * pAlphaBmp
    );

  //! Applies a filter to the bitmap.
  void ApplyFilter
    ( const PLFilter& Filter
    );

  //! Slow but simple function to set a single pixel. 32 bpp only.
  void SetPixel
    ( int x,
      int y,
      PLPixel32 pixel
    );

  //! Slow but simple function to get a single pixel. 32 bpp only.
  PLPixel32 GetPixel
    ( int x,
      int y
    ) const;

  //! Find the nearest color to cr in the palette used by this bitmap
  //! Only works for 8 bpp bitmaps.
  PLBYTE FindNearestColor
    ( PLPixel32 cr
    );

  // PLBmp information.
  //!
  virtual long GetMemUsed
    () = 0;

  //!
  int GetNumColors
    ();

  //!
  void SetHasAlpha
    (bool b
    );

  //! Returns number of bytes used per line.
  virtual long GetBytesPerLine
    () = 0;

  // PLBmp direct manipulation

  //! Returns the address of the color table of the bitmap or NULL if
  //! no color table exists. The color table is stored as an array of
  //! consecutive PLPixel32 objects.
  PLPixel32 * GetPalette
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLBYTE ** GetLineArray
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLPixel32 ** GetLineArray32
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLPixel24 ** GetLineArray24
    () const;

  //! Locks bitmap. GetLineArray() and other direct-access methods should
  //! only be called if the bitmap is locked. Lock and Unlock keep a lock
  //! count. In most cases (currently: all but PLDDrawBmp), the lock count
  //! will always be >= 1, so access is always possible.
  virtual void Lock
    ( bool bReadable,
      bool bWriteable
    );

  //! Unlocks the Bitmap surface.
  virtual void Unlock
    ();

  bool IsLocked
    () const;

  bool AlmostEqual
    ( const PLBmp& Bmp,
      int epsilon
    ) const;

  //! Sets the bitmap resolution in pixels per inch.
  void SetResolution (const PLPoint& Resolution);

protected:


  //! Create a new bitmap with uninitialized bits. (Assume no memory
  //! is allocated yet.)
  virtual void internalCreate
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    ) = 0;

  //! Delete memory allocated by member variables.
  virtual void freeMembers
    () = 0;

  //! Initialize internal table of line addresses.
  virtual void initLineArray
    () = 0;

  //! Creates a new PLBmp as copy of rSrPLBmp. Assumes there is no memory
  //! allocated yet.
  void internalCopy
    ( const PLBmp & rSrPLBmp
    );

  //! Can be called from internalCreate() to initialize object state.
  void initLocals
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale    
    );

  void create8BPPCopy
    ( const PLBmp & rSrPLBmp
    );

  void create1BPPCopy
    ( const PLBmp & rSrPLBmp
    );

  // Member variables
  PLPixel32 * m_pClrTab;      // Pointer to the color table.
  PLBYTE    ** m_pLineArray;   // Table of the starting addresses of
                             // the lines.
  int        m_LockCount;    // Number of times the bitmap was locked.
                             // Default is m_LockCount always >= 1, so
                             // access to bits is always possible.
  int m_DitherType;
  int m_DitherPaletteType;
};

inline PLBmp & PLBmp::operator=
    ( PLBmp const &Orig
    )
{
  if (this != &Orig)
    CreateCopy(Orig);
  return *this;
}

inline void PLBmp::SetPaletteEntry
    ( PLBYTE Entry,
      PLBYTE r,
      PLBYTE g,
      PLBYTE b,
      PLBYTE a
    )
{
  m_pClrTab[Entry].Set (r, g, b, a);
}

inline void PLBmp::SetPaletteEntry
    ( PLBYTE Entry,
      PLPixel32 Value
    )
{
  m_pClrTab[Entry] = Value;
}


inline PLPixel32 PLBmp::GetPixel (int x, int y) const
{
  PLASSERT (GetBitsPerPixel() == 32);
  return *((PLPixel32 *)GetLineArray()[y]+x);
}

inline void PLBmp::SetPixel (int x, int y, PLPixel32 pixel)
{
  PLASSERT (GetBitsPerPixel() == 32);
  *(GetLineArray32()[y]+x) = pixel;
}

inline void PLBmp::SetResolution(const PLPoint& Resolution)
{
  PLASSERT_VALID(this);
  m_Resolution = Resolution;
}

inline int PLBmp::GetNumColors
    ()
{
  PLASSERT_VALID (this);

  if (m_bpp == 32)
    return 1 << 24;
   else
    return 1 << m_bpp;
}


inline void PLBmp::SetHasAlpha
    (bool b
    )
{
  PLASSERT_VALID (this);
  m_bAlphaChannel = b;
}

// PLBmp direct manipulation

inline PLBYTE ** PLBmp::GetLineArray
    () const
{
  PLASSERT (m_pLineArray);
  return m_pLineArray;
}

inline PLPixel32 ** PLBmp::GetLineArray32
    () const
{
  PLASSERT (m_pLineArray);
  PLASSERT (m_bpp == 32);
  return (PLPixel32**)m_pLineArray;
}

inline PLPixel24 ** PLBmp::GetLineArray24
    () const
{
  PLASSERT (m_pLineArray);
  PLASSERT (m_bpp == 24);
  return (PLPixel24**)m_pLineArray;
}

inline PLPixel32 * PLBmp::GetPalette
    () const
    // Returns adress of the color table of the bitmap or NULL if no
    // color table exists.
{
  PLASSERT_VALID (this);

  return m_pClrTab;
}

inline bool PLBmp::IsLocked
    () const
{
  return (m_LockCount >= 1);
}


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plbitmap.h,v $
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
|      Revision 1.1  2007/04/06 09:31:03  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:21  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:24  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:30  황정영
|      2006.05.17
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.6  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.5  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.4  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.26  2001/09/15 14:30:20  uzadow
|      Fixed PLPixel32 initialization bug.
|
|      Revision 1.25  2001/09/13 20:47:36  uzadow
|      Removed commented-out lines.
|
|      Revision 1.24  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.23  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.22  2000/11/21 20:18:03  uzadow
|      Added operator ==
|
|      Revision 1.21  2000/11/07 15:40:46  jmbuena
|      Changes related to paintlibdefs.h and pixeldefs.h
|
|      Revision 1.20  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.19  2000/10/24 16:46:34  uzadow
|      Fixed build problems
|
|      Revision 1.18  2000/10/23 17:45:03  jmbuena
|      Linux compatibility changes
|
|      Revision 1.17  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|      Revision 1.16  2000/09/26 12:14:51  Administrator
|      Refactored quantization.
|
|      Revision 1.15  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.14  2000/07/11 17:11:00  Ulrich von Zadow
|      Added support for RGBA pixel ordering (Jose Miguel Buenaposada Biencinto).
|
|      Revision 1.13  2000/03/31 12:20:05  Ulrich von Zadow
|      Video invert filter (beta)
|
|      Revision 1.12  2000/03/31 11:53:30  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.11  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.10  1999/12/10 01:27:26  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.9  1999/12/09 16:35:22  Ulrich von Zadow
|      no message
|
|      Revision 1.8  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.7  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.6  1999/10/22 21:25:51  Ulrich von Zadow
|      Removed buggy octree quantization
|
\--------------------------------------------------------------------
*/
