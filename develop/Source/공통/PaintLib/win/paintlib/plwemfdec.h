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
|      $Id: plwemfdec.h,v 1.1 2009/12/09 05:34:47 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
--------------------------------------------------------------------
*/

#ifndef INCL_WEMFDEC_H
#define INCL_WEMFDEC_H

#ifndef INCL_PICDEC
#include "plpicdec.h"
#endif

// This only makes sense for Windows
#ifdef _WINDOWS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//! Decoder for 16-Bit Windows Metafiles (*.wmf) and 16-Bit Adobe 
//! Placeable Metafiles (*,wmf) and 32-Bit Enhanced Windows 
//! Metafiles (*.emf) for Windows 95, Windows 98 and Windows NT 
//! >= 3.1.
//!
//! Comments by the author, Mario Westphal:<BR>
//!   <I>It can handle at least the 500 tested images I've got
//!   from various free and commercial clipart sources. If
//!   you find a WMF/EMF file it cannot handle, attach it to
//!   an email and send it to mw@mwlabs.de. I'll see what I
//!   can do. But, please, test it with another program
//!   before you send itin to see if it is really a valid 
//!   metafile.</I>
class PLWEMFDecoder : public PLPicDecoder  
{
public:
  //!
	PLWEMFDecoder();

  //!
	virtual ~PLWEMFDecoder();

  //! Fills the PLBmpInfo fields. To do this, the routine reads the complete
  //! file.
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the image. 
  virtual void GetImage (PLBmp & Bmp);

  //!
  void Close();

#ifdef _DEBUG
  virtual void AssertValid() const;
#endif

private:
	HENHMETAFILE m_hemf;
	HDC m_memdc;
	HDC m_dc;
	HBITMAP m_bm;
	HPALETTE m_holdpal;
  HGDIOBJ m_holdbm;
	LPENHMETAHEADER m_phdr;

};

#endif // _WINDOWS

#endif // INCL_WEMFDEC_H

/*
/--------------------------------------------------------------------
|
|      $Log: plwemfdec.h,v $
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
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:00  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.4  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|
\--------------------------------------------------------------------
*/
