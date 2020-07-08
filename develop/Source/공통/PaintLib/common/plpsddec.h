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
|      $Id: plpsddec.h,v 1.1 2009/12/09 05:33:56 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPSDDEC
#define INCL_PLPSDDEC

#ifndef INCL_PLPICDEC
#include "plpicdec.h"
#endif

#include <vector>

class PLDataSource;
class PLPoint;

struct PLPSDHeader
{
  char     Signature[4];
  PLWORD   Version;
  PLBYTE   Reserved[6];
  PLWORD   Channels;
  int      Rows;
  int      Columns;
  PLWORD   BPP;
  PLWORD   Mode;
};

struct PLPSDLayerInfo;

//! Photoshop file decoder. Besides having the MakeBmpFromFile interface
//! that the other decoders have, the photoshop decoder loads the layers
//! of the image into separate bitmaps so they can be manipulated separately:
//! <pre>
//!   vector&lt;PLAnyBmp&gt; Bmp;
//!   PLAnyBmp BaseBmp);
//!   Decoder-&gt;OpenFile ("face.psd");
//!   int NumLayers = Decoder-&gt;GetNumLayers();
//!   for (int i=0; i&lt;NumLayers; i++)
//!   {
//!     GetNextLayer (Bmp[i]);
//!     LayerOffset = GetLayerOffset();
//!   }
//!   GetImage (&BaseBmp);
//!   Close();
//! </pre>
class PLPSDDecoder : public PLPicDecoder
{
public:
  //! Creates a decoder
  PLPSDDecoder
    ();

  //! Destroys a decoder
  virtual ~PLPSDDecoder
    ();

  //!
  virtual void Open (PLDataSource * pDataSrc);

  //! Fills the bitmap with the main image. This image is a flattened 
  //! version of the layers.
  virtual void GetImage (PLBmp & pBmp);

  //! Returns number of layers in the image.
  int GetNumLayers
    ();

  //! Fills the bitmap with the layer data.
  void GetNextLayer
    ( PLBmp& Bmp
    );

  //! Returns the origin of the layer data in the image.
  PLPoint GetLayerOffset
    ();

private:
  void readHeader
    ( PLDataSource * pDataSrc,
      PLPSDHeader * pPSDHeader
    );

  void readColorModeData 
    ( PLDataSource * pDataSrc
    );

  PLBmpInfo* createBmpInfo 
    ( int Mode,
      int Height,
      int Width,
      int Channels,
      const PLPoint& Resolution
    );

  void readImageResData 
    ( PLDataSource * pDataSrc,
      PLPoint& Resolution
    );

  void readLayerHeader 
    ( PLDataSource * pDataSrc
    );

  void skipLayerData
    ( PLDataSource * pDataSrc
    );

  void skipMaskData
    ( PLDataSource * pDataSrc
    );

  void readLayer 
    ( PLDataSource * pDataSrc, 
      PLBmp& Bmp,
      int Mode
    );

  void readImageData
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int Mode,
      int Height,
      int Width,
      int Channels 
    );

  void traceCompressionMethod
    ( PLWORD CompressionMethod
    );

  void readUncompressedImage 
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int Mode,
      int Height,
      int Width,
      int Channels 
    );

  void readRLEImage 
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int Mode,
      int Height,
      int Width,
      int Channels 
    );

  void readRLEChannel 
    ( PLDataSource * pDataSrc,
      PLBmp * pBmp,
      int Mode,
      int Height,
      int Width,
      int ChannelNum,
      int * pRowLengths 
    );

  void cleanup
    ();

  PLPSDHeader m_PSDHeader;
  PLPoint m_Resolution;
  PLPixel32 m_pPal[256];
  PLBYTE * m_pMiscDataStart;
  int m_MiscDataSize;
  int m_NumLayers;
  int m_LayersRead;
  std::vector<PLPSDLayerInfo *> m_pLayerInfo;
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plpsddec.h,v $
|      Revision 1.1  2009/12/09 05:33:56  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:05  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:26  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:44  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:33  황정영
|      2006.05.17
|
|      Revision 1.4  2002/11/18 14:45:10  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.3  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|
\--------------------------------------------------------------------
*/
