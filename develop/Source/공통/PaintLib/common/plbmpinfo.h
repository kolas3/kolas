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
|      $Id: plbmpinfo.h,v 1.1 2009/12/09 05:33:48 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBMPINFO
#define INCL_PLBMPINFO

#include "plobject.h"
#include "plpoint.h"

//! This is a simple base class that holds bitmap metainformation. It is
//! used as a base class by PLPicDecoder and PLBmp.
class PLBmpInfo : public PLObject
{

public:
  //! 
  PLBmpInfo (const PLPoint& Size, int bpp, const PLPoint& Resolution, 
             bool bAlphaChannel, bool bIsGreyscale);

  //! 
  PLBmpInfo () {}

  //!
  PLBmpInfo (const char * pszInfo);

  // This class doesn't have any virtual functions and there's nothing to 
  // destroy, so we don't define a destructor  - and the default assignment
  // and equality operators work well too.

  //!
  const PLPoint& GetSize () const
  { return m_Size; }

  //!
  int GetWidth () const
  { return m_Size.x; }

  //!
  int GetHeight () const
  { return m_Size.y; }

  //!
  int GetBitsPerPixel () const
  { return m_bpp; }

  //!
  bool HasAlpha () const
  { return m_bAlphaChannel; }

  //!
  bool IsGreyscale () const
  { return m_bIsGreyscale; }


  //! Gets the bitmap resolution in pixels per inch. Returns 0 if the
  //! resolution is unknown.
  const PLPoint& GetResolution () const
  { return m_Resolution; }

  //!
  void AsString (char * psz, int len) const;
  
  //!
  bool operator == (const PLBmpInfo & Other) const;

protected:
  void SetBmpInfo (const PLPoint& Size, int bpp, const PLPoint& Resolution, 
                   bool bAlphaChannel, bool bIsGreyscale);
  void SetBmpInfo (const PLBmpInfo& SrcInfo);

  PLPoint m_Size;
  int     m_bpp;
  PLPoint m_Resolution;

  bool    m_bAlphaChannel;
  bool    m_bIsGreyscale;
};


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plbmpinfo.h,v $
|      Revision 1.1  2009/12/09 05:33:48  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:13  박대우
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
|      Revision 1.1  2002/08/04 20:17:57  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|      Major improvements in tests.
|      Update to VS .NET
|
|
\--------------------------------------------------------------------
*/
