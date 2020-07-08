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
|      $Id: plpictdec.h,v 1.1 2009/12/09 05:33:54 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPICTDEC
#define INCL_PLPICTDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#include "qdraw.h"

class PLJPEGDecoder;

//! This class decodes macintosh PICT files with 1,2,4,8,16 and 32
//! bits per pixel as well as PICT/JPEG. If an alpha channel is
//! present in a 32-bit-PICT, it is decoded as well.
//! The PICT format is a general picture file format and can
//! contain a lot of other elements besides bitmaps. These elements
//! are ignored.
class PLPictDecoder : public PLPicDecoder
{


public:
  //! Creates a decoder. A JPEG decoder is needed to decode PICT
  //! files containing JPEGs.
  PLPictDecoder
    ( PLJPEGDecoder * pJPEGDecoder
    );

  //! Destroys a decoder.
  virtual ~PLPictDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the image.
  virtual void GetImage (PLBmp & Bmp);

private:
  // Decodes header and version information.
  // Sets m_Version.
  // Performs checks to make sure the data is really a pict file.
  void readHeader
    ( PLDataSource * pDataSrc,
      int& Version
    );

  // This is the main decoder loop. The functions reads opcodes,
  // skips some of them, and hands the rest to opcode-specific
  // functions. It stops decoding after the first opcode containing
  // bitmapped data.
  void interpretOpcodes
    ( PLDataSource * pDataSrc,
      int& Version
    );

  // Moves to an even byte position in the file and returns the
  // opcode found there.
  PLWORD readOpcode
    ( int Version,
      PLDataSource * pDataSrc
    );

  // Opcode functions

  // Skips clipping rectangle or region opcode.
  void clip
    ( PLDataSource * pDataSrc
    );

  // Skips pattern definition opcode.
  // Untested!!
  void pixPat
    ( PLDataSource * pDataSrc
    );

  // Skips polygon/region opcodes.
  // Untested!!
  void skipPolyOrRegion
    ( PLDataSource * pDataSrc
    );

  // Opcode: Bitmap/pixmap data clipped by a rectangle.
  void bitsRect
    ( PLDataSource * pDataSrc
    );

  // Opcode: Bitmap/pixmap data clipped by a region.
  void bitsRegion
    ( PLDataSource * pDataSrc
    );

  // DirectBitsRect opcode.
  void opcode9a
    ( PLDataSource * pDataSrc
    );

  void DecodeOp9a
    ( PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Long comment opcode. Skipped.
  void longComment
    ( PLDataSource * pDataSrc
    );

  // Header opcode. Contains resolution information.
  void headerOp
    ( PLDataSource * pDataSrc
    );

  // JPEG opcode. Invoke the JPEG decoder for this PICT.
  void jpegOp
    ( PLDataSource * pDataSrc
    );

  void DecodeJPEG
    ( PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Bitmap & Pixmap functions.

  // Allocates the output bitmap.
  void setBmpInfo
    ( MacpixMap PixMap
    );

  // Decode version 1 bitmap: 1 bpp.
  void doBitmap
    ( PLDataSource * pDataSrc
    );

  void DecodeBitmap
    ( PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Decode version 2 pixmap.
  void doPixmap
    ( PLDataSource * pDataSrc
    );

  void DecodePixmap
    ( PLBmp * pBmp,
      PLDataSource * pDataSrc
    );


  // This routine decompresses BitsRects with a packType of 4 (and
  // 32 bits per pixel). In this format, each line is separated
  // into 8-bit-bitplanes and then compressed via RLE. To decode,
  // the routine decompresses each line & then juggles the bytes
  // around to get pixel-oriented data.
  void unpack32bits
    ( MacRect* pBounds,
      PLWORD rowBytes,
      int NumBitPlanes,    // 3 if RGB, 4 if RGBA
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Decompression routine for 8 bpp. rowBytes is the number of
  // bytes each source row would take if it were uncompressed.
  // This _isn't_ equal to the number of pixels in the row - apple
  // pads the data to a word boundary and then compresses it. Of
  // course, we have to decompress the excess data and then throw it
  // away.
  void unpack8bits
    ( MacRect* pBounds,
      PLWORD rowBytes,
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Decompression routine for everything but 32 bpp. This routine
  // is slower than the two routines above since it has to deal
  // with a lot of special cases.
  void unpackbits
    ( MacRect* pBounds,
      PLWORD rowBytes,
      int pixelSize,         // Source bits per pixel.
      PLBmp * pBmp,
      PLDataSource * pDataSrc
    );

  // Skips unneeded packbits.
  void skipBits
    ( MacRect* pBounds,
      PLWORD rowBytes,
      int pixelSize,         // Source bits per pixel.
      PLDataSource * pDataSrc
    );

  // Expands one RLE unit to 32-bit pixel data.
  void expandBuf
    ( PLBYTE * pDestBuf,
      PLBYTE * pSrcBuf,
      int Width,       // Width in bytes for 8 bpp or less.
                       // Width in pixels for 16 bpp.
      int bpp          // bits per pixel
    );

  // Expands Width units to 8-bit pixel data.
  // Max. 8 bpp source format.
  void expandBuf8
    ( PLBYTE * pDestBuf,
      PLBYTE * pSrcBuf,
      int Width,       // Width in bytes.
      int bpp          // bits per pixel.
    );

  void readPixmap
    ( MacpixMap * pPixMap,
      PLDataSource * pDataSrc
    );

  // Reads a mac colour table into a windows palette.
  void readColourTable
    ( PLWORD * pNumColors,
      PLDataSource * pDataSrc,
      PLPixel32 * pPal
    );

  void readRect
    ( MacRect * pr,
      PLDataSource * pDataSrc
    );

  void dumpRect
    ( char * psz,
      MacRect * pr
    );

  void tracePixMapHeader
    ( int Level,
      MacpixMap * pPixMap
    );


  PLJPEGDecoder * m_pJPEGDecoder; // Used if a pict file contains a JPEG.

  enum PICTType {none, op9a, jpeg, pixmap, bitmap};
  PICTType m_PictType;

  // Pixmap header.
  MacpixMap m_PixMap;

  // pixmap/bitmap stuff
  int m_rowBytes;
  MacRect m_Bounds;
  bool m_bIsRegion;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plpictdec.h,v $
|      Revision 1.1  2009/12/09 05:33:54  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:18  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:02  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:57  박우람
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
|      Revision 1.8  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.7  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.6  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.5  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.4  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/03 13:58:21  uzadow
|      Removed references to config.h
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
\--------------------------------------------------------------------
*/
