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
|      $Id: plpicdec.h,v 1.1 2009/12/09 05:33:54 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPICDEC
#define INCL_PLPICDEC

#ifndef INCL_PLBITMAP
#include "plbitmap.h"
#endif

#ifndef INCL_PLDATASRC
#include "pldatasrc.h"
#endif

//! PLPicDecoder is an abstract base class. It defines common routines
//! for all decoders. Decoders for specific file formats can be
//! derived from this class. Objects of this class interact with a
//! PLDataSource to decode bitmaps.
class PLPicDecoder : public PLBmpInfo
{

public:
  //! Empty constructor. The actual initialization takes place in a
  //! derived class.
  PLPicDecoder
    ();

  //! Destructor. Frees memory allocated.
  virtual ~PLPicDecoder
    ();

  //! Decodes a picture in a file and stores the results in pBmp.
  //! BPPWanted is the number of bits per pixel in the bitmap
  //! returned. Valid values for BPPWanted are 8, 24, and 0.
  //! 0 means 'keep the input format'. Decoding a true-color image
  //! to 8 bpp is not supported. As an exception, BPPWanted can be
  //! 1 for b/w tiff files.
  virtual void MakeBmpFromFile
    ( const char * pszFName,
      PLBmp * pBmp,
      int BPPWanted = 0,
      PLIProgressNotification * pProgNot = NULL
    );

#ifdef _WINDOWS
  //! Decodes a picture in a resource by creating a resource data
  //! source and calling MakeBmp with this data source.
  //! BPPWanted is the number of bits per pixel in the bitmap
  //! returned. Valid values for BPPWanted are 8, 24, and 0.
  //! 0 means 'keep the input format'. Decoding a true-color image
  //! to 8 bpp is not supported. As an exception, BPPWanted can be
  //! 1 for b/w tiff files.
  //! ResType is the windows resource type (e.g. "JPEG") to be used.
  //!
  //! New optional parameter: hResModule (04.01.2000 ms)
  //! If a module handle is passed as hResModule, then the resource
  //! will be loaded from this module, otherwise, the resource
  //! will be searched for by AfxFindResourceHandle (! MFC dependent!)
  virtual void MakeBmpFromResource
    ( HINSTANCE hInstResource, int ResourceID,
      PLBmp * pBmp,
      int BPPWanted = 0,
      const char* ResType = NULL,
      HMODULE hResModule = 0
	  );
#endif

  //! Decodes a picture from a memory location which directly resembles
  //! the image file as it would be on disc. The result is stored in pBmp.
  //! BPPWanted is the number of bits per pixel in the bitmap
  //! returned. Valid values for BPPWanted are 8, 24, and 0.
  //! 0 means 'keep the input format'. Decoding a true-color image
  //! to 8 bpp is not supported. As an exception, BPPWanted can be
  //! 1 for b/w tiff files.
  virtual void PLPicDecoder::MakeBmpFromMemory
    ( unsigned char * ucMemSrc,
      int MemSrcSize,
      PLBmp * pBmp,
      int BPPWanted = 0,
      PLIProgressNotification * pProgNot = NULL
    );

  //! Decodes a picture by getting the encoded data from pDataSrc.
  //! Open should be called before, close after this routine is called.
  //! Stores the results in pBmp. BPPWanted is the number of bits
  //! per pixel in the bitmap returned. Valid values for BPPWanted
  //! are 8, 24, and 0. 0 means 'keep the input format'. Decoding a
  //! true-color image to 8 bpp is not supported.
  virtual void MakeBmp
    ( PLBmp * pBmp,
      int BPPWanted = 0
    );

  void OpenFile
    ( const char * pszFName, 
      PLIProgressNotification * pProgNot = NULL
    );

  virtual void Close
    ();

  //! Sets the amount and destination of debug traces output by the
  //! debug version. pszFName contains either a valid file name or is
  //! NULL. If it contains a file name, this file is used to store
  //! debug information. If pszFName is NULL, the destination is
  //! either the MSVC debug console or stderr depending on the
  //! version of the library. Valid values for Level are: <BR>
  //! <BR>
  //! 0: Trace only errors.<BR>
  //! 1: Trace top-level calls.<BR>
  //! 2: Trace picture format information<BR>
  //! 3: Trace all miscellaneous info.<BR>
  //! <BR>
  //! The trace configuration is global to all decoders.
  static void SetTraceConfig
    ( int Level,
      char * pszFName
    );

  //!
  virtual void Open (PLDataSource * pDataSrc)=0;

  //!
  virtual void GetImage (PLBmp & Bmp) = 0;

  // Used only by PLAnyPicDecoder
  void SetDataSrc (PLDataSource * pDataSrc);

  // This function is needed by callbacks outside of any object,
  // so it's public and static. It should not be called from
  // outside of the library.
  static void raiseError
    ( int Code,
      char * pszErr
    );

  //! Called to output status messages to the current debug console
  static void Trace
    ( int TraceLevel,
      const char * pszMessage
    );

protected:
  //! Implements the actual decoding process. Uses variables local to
  //! the object to retrieve and store the data. Implemented in
  //! derived classes.
  //!
  //! This routine should never be called. It's here so derived classes
  //! can override MakeDIB directly if they want to. (CAnyDecoder does
  //! this).
/*  virtual void DoDecode
    ( PLBmp * pBmp,
      PLDataSource * pDataSrc
    );
*/

  PLBYTE * unpackPictRow
    ( PLBYTE * pLineBuf,
      PLDataSource * pDataSrc,
      int Width,
      int rowBytes,
      int SrcBytes
    );

  PLBYTE ReadByte
    ( PLDataSource * pDataSrc
    );

  // Machine-independent routines for byte-order conversion.

  PLWORD ReadIWord
    ( PLDataSource * pDataSrc
    );

  PLWORD ReadMWord
    ( PLDataSource * pDataSrc
    );

  PLLONG ReadILong
    ( PLDataSource * pDataSrc
    );

  PLLONG ReadMLong
    ( PLDataSource * pDataSrc
    );

  PLDataSource * m_pDataSrc;

private:
  ///////////////////////////////////////////////////////////////////
  // Member variables.

  static int    m_TraceLevel;    // 0: Trace only errors.
                                 // 1: Trace top-level calls.
                                 // 2: Trace picture format info.
                                 // 3: Trace misc. info.
  static char * m_pszTraceFName; // Name of trace file. NULL if trace to
                                 // MSVC debug console.
};


inline PLBYTE PLPicDecoder::ReadByte
    ( PLDataSource * pDataSrc
    )
{

  return *(pDataSrc->ReadNBytes (1));
}


inline PLWORD PLPicDecoder::ReadIWord
    ( PLDataSource * pDataSrc
    )
{
  PLBYTE * pData = pDataSrc->Read2Bytes ();
  // This should work regardless of the destination byte order ;-)
  return pData[0] + (pData[1]<<8);
}


inline PLWORD PLPicDecoder::ReadMWord
    ( PLDataSource * pDataSrc
    )
{
  PLBYTE * pData = pDataSrc->Read2Bytes ();
  return pData[1] + (pData[0]<<8);
}


inline PLLONG PLPicDecoder::ReadILong
    ( PLDataSource * pDataSrc
    )
{
  PLBYTE * pData = pDataSrc->Read4Bytes ();
  return pData[0] + (pData[1]<<8) + (pData[2]<<16) + (pData[3]<<24);
}

inline PLLONG PLPicDecoder::ReadMLong
    ( PLDataSource * pDataSrc
    )
{
  PLBYTE * pData = pDataSrc->Read4Bytes ();
  return pData[3] + (pData[2]<<8) + (pData[1]<<16) + (pData[0]<<24);
}

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpicdec.h,v $
|      Revision 1.1  2009/12/09 05:33:54  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:17  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:56  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:05  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:32  황정영
|      2006.05.17
|
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/06 22:46:54  uzadow
|      Fixed major PLAnyDec bug
|
|      Revision 1.4  2002/03/03 16:29:55  uzadow
|      Re-added BPPWanted.
|
|      Revision 1.3  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.7  2000/03/30 21:24:15  Ulrich von Zadow
|      Added MakeBmpFromMemory() function by Markus Ewald
|
|      Revision 1.6  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  2000/01/11 21:40:30  Ulrich von Zadow
|      Added instance handle parameter to LoadFromResource()
|
|      Revision 1.4  2000/01/08 15:51:30  Ulrich von Zadow
|      Misc. modifications to png encoder.
|
|      Revision 1.3  1999/11/08 22:12:51  Ulrich von Zadow
|      Andreas Koepf: Added resource type as parameter to
|      MakeBmpFromResource
|
|
\--------------------------------------------------------------------
*/
