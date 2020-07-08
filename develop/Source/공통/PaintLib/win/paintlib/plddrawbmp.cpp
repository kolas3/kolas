/*
/--------------------------------------------------------------------
|
|      $Id: plddrawbmp.cpp,v 1.1 2009/12/09 05:34:46 박종운 Exp $
|      Bitmap class using a directdraw surface.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plddrawbmp.h"
#include "plexcept.h"
#include "plddhelper.h"

#include <stdio.h>

PLDDrawBmp::PLDDrawBmp
    ( IDirectDraw * pDDraw
    )
    : PLBmp(),
      m_pSurface (NULL),
      m_pDDSurfaceDesc (new DDSURFACEDESC),
      m_pDDraw(pDDraw)
    // Creates an empty bitmap.
{  
  internalCreate (0,0,32, false, false);
  PLASSERT_VALID(this);
}


PLDDrawBmp::~PLDDrawBmp
    ()
{
  // Free the memory.
  freeMembers();
  delete m_pDDSurfaceDesc;
}


#ifdef _DEBUG
void PLDDrawBmp::AssertValid
    () const
{
  // call inherited ASSERT_VALID first
  PLBmp::AssertValid();

  if (m_pSurface)
  {
    DDSURFACEDESC Desc;
    Desc.dwSize = sizeof(Desc); 
    m_pSurface->GetSurfaceDesc(&Desc);
    PLASSERT (int(Desc.dwHeight) == m_Size.y);
    PLASSERT (int(Desc.dwWidth) == m_Size.x);
  }
}
#endif


/////////////////////////////////////////////////////////////////////
// PLDDrawBmp information

long PLDDrawBmp::GetMemUsed
    ()
    // Returns the memory used by the object.
{
  PLASSERT_VALID (this);
  PLASSERT (m_pSurface);

  return GetBytesPerLine()*GetHeight();
}


long PLDDrawBmp::GetBytesPerLine
    ()
    // Returns number of bytes used per line.
{
  PLASSERT_VALID (this);
  PLASSERT (m_pSurface);

  switch (m_bpp)
  {
    case 32: 
      return GetWidth() * 4;
    default:
      PLASSERT (false);
      return 0;
  }
}

void PLDDrawBmp::Lock
    ( bool bReadable,
      bool bWriteable
    ) 
    // Locks the DirectDraw surface and fills the line array. GetLineArray() 
    // returns NULL if the surface isn't locked.
    // Always locks the whole surface.
{
  PLBmp::Lock(bReadable, bWriteable);

  PLASSERT_VALID (this);
  PLASSERT (m_pSurface);

  m_pDDSurfaceDesc->dwSize = sizeof(*m_pDDSurfaceDesc);
  DWORD dwFlags = 0;

  if (bReadable && !bWriteable)
    dwFlags |= DDLOCK_READONLY;
  if (!bReadable && bWriteable)
    dwFlags |= DDLOCK_WRITEONLY;
  HRESULT hr = m_pSurface->Lock(NULL, m_pDDSurfaceDesc, dwFlags, NULL);
  CheckForDDError (hr);
  initLineArray();
}

void PLDDrawBmp::Unlock
    () 
    // Unlocks the DirectDraw surface.
{
  PLBmp::Unlock();

  HRESULT hr = m_pSurface->Unlock (NULL);
  CheckForDDError (hr);
  delete[] m_pLineArray;
  initLineArray();
}

IDirectDrawSurface * PLDDrawBmp::GetSurface
  ()
  // Gets a pointer to the directdraw surface used.
{
  return m_pSurface;
}


/////////////////////////////////////////////////////////////////////
// Windows-specific interface

SIZE PLDDrawBmp::GetSize
    ()
    // Returns size in pixels
{
  SIZE sz;

  PLASSERT_VALID (this);

  sz.cx = GetWidth();
  sz.cy = GetHeight();

  return sz;
}




/////////////////////////////////////////////////////////////////////
// Protected callbacks

void PLDDrawBmp::internalCreate
    ( LONG Width,
      LONG Height,
      WORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    )
    // Create a new empty DIB. Bits are uninitialized.
    // Assumes that no memory is allocated before the call.
{
  PLASSERT (BitsPerPixel == 32);
  PLASSERT (!m_pSurface);

  if (Width != 0 || Height != 0)
  {
    DDSURFACEDESC Desc;
    ZeroMemory(&Desc,sizeof(Desc));
    Desc.dwSize = sizeof(Desc);
    Desc.dwWidth        = Width;
    Desc.dwHeight       = Height;
    Desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    Desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    Desc.ddpfPixelFormat.dwSize  = sizeof(DDPIXELFORMAT);
    Desc.ddpfPixelFormat.dwFlags = DDPF_RGB; 
    Desc.ddpfPixelFormat.dwRGBBitCount = 32;
    Desc.ddpfPixelFormat.dwRBitMask = 0x000000FF;
    Desc.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
    Desc.ddpfPixelFormat.dwBBitMask = 0x00FF0000;
    // DirectX ignores any alpha channel information.
    Desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0x00000000;

    HRESULT hr = m_pDDraw->CreateSurface(&Desc, &m_pSurface, NULL);
    CheckForDDError (hr);   // ---> Replace this by exception.
  }

  // Set color table pointer & pointer to bits.
  initPointers ();

  initLocals (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);

  PLASSERT_VALID (this);
}


void PLDDrawBmp::freeMembers
    ()
{
  if (m_pSurface)
    m_pSurface->Release();
  m_pSurface = NULL;
}


void PLDDrawBmp::initLineArray
    ()
{
  if (IsLocked())
  {
    m_pLineArray = new PLBYTE * [m_Size.y];
    PLBYTE * pBits = (PLBYTE *)(m_pDDSurfaceDesc->lpSurface);
    int i;
    for (i = 0; i < GetHeight(); i++)
    {
      m_pLineArray[i] = pBits;
      pBits += m_pDDSurfaceDesc->lPitch;
    }
  }
  else
    m_pLineArray = NULL;
}


void PLDDrawBmp::initPointers
    ()
    // Set color table pointer
{
  m_pClrTab = NULL;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plddrawbmp.cpp,v $
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
|      Revision 1.2  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.1  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|
\--------------------------------------------------------------------
*/
