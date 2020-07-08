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
|      $Id: plfilterquantize.h,v 1.1 2009/12/09 05:33:42 ¹ÚÁ¾¿î Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERQUANTIZE)
#define INCL_PLFILTERQUANTIZE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "plfilter.h"
#include "plbitmap.h"

#define PLDTHPAL_MEDIAN          0       // Median Cut
#define PLDTHPAL_POPULARITY      1       // Popularity Sort
#define PLDTHPAL_DEFAULT         2       // Use Default Palette
#define PLDTHPAL_USERDEFINED     3       // Use Palette set by SetUserPalette()

#define PLDTH_NONE      0       // None
#define PLDTH_ORDERED   1       // Ordered Dithering
#define PLDTH_FS        2       // Floyd-Steinberg Dithering
//#define PLDTH_JITTER    3       // Jitter preprocessing


//! Returns an 8 bpp bitmap containing a best-fit representation of the
//! source 32 bpp bitmap.
class PLFilterQuantize : public PLFilter
{
public:
  //! DitherPaletteType and DitherType determine the type of quantization:
  //!
  //! PLDTHPAL_MEDIAN (0): Median cut
  //!
  //! PLDTHPAL_POPULARITY (1): Popularity sort
  //!
  //! PLDTHPAL_DEFAULT (2): Use default palette
  //!
  //! PLDTHPAL_USERDEFINED (3): Use palette defined by SetUserPalette();
  //!
  //!
  //! PLDTH_NONE (0): No dithering
  //!
  //! PLDTH_ORDERED (1): Ordered dithering
  //!
  //! PLDTH_FS (2): Floyd-Steinberg dithering
  PLFilterQuantize (int DitherPaletteType, int DitherType);
  //!
  virtual ~PLFilterQuantize();

  //!
  virtual void Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const;

  //! For PLDTHPAL_USERDEFINED, sets the palette to use.
  void SetUserPalette(const PLPixel32* pPal);

  static const PLPixel32* GetDefaultPalette ();

private:
  // Local structs defining the quantization buffer
  typedef struct tagQUBOX
  {
    PLPixel32 Corner0, Corner1;
    PLPixel32 Average;
    PLULONG count;
  } QUBOX;

  typedef struct tagHISTONODE
  {
    int   index;
    PLULONG count;
  } HISTONODE;

  // Private members
  void initLUT();
  void deleteLUT();

  void genMedianPalette (PLBmp * pBmpSource, PLBmp * pBmpDest) const;
  void split (QUBOX * pBox0, QUBOX * pBox1, int ColComp) const;
  void squeeze(QUBOX * pBox) const;
  void genPopularityPalette (PLBmp * pBmpSource, PLBmp * pBmpDest) const;
  void genColorArray(PLBmp * pBmpSource) const;
  void genDefaultPalette (PLBmp * pBmpSource) const;
  void addColor(PLPixel32 col, PLULONG count) const;
  void makeBox(PLPixel32 col, int i, PLULONG c) const;
  int getColorTableIndex (PLPixel32 col) const;
  int getShiftedColorTableIndex (PLPixel32 col) const;

  void ditherDestBmp(PLBmp * pBmpSource, PLBmp * pBmpDest) const;
  void jitterPixel (int i, int y, PLPixel32 * pPixel) const;
  void ditherPixelOrdered (int x, int y, PLPixel32 * pPixel) const;
  void ditherCompOrdered (int x, int y, PLBYTE * pComp) const;
  void ditherPixelFS(double * pR, double * pG, double * pB, double * pCurErrors) const;
  void ditherCompFS (double * pComp, double Error) const;
  PLBYTE getNeighbor (PLPixel32 Color, PLPixel32 * pPal) const;
  int colorDist (PLPixel32 c0, PLPixel32 c1) const;

  int clip (int c) const;

  int m_DitherPaletteType;
  int m_DitherType;
  PLPixel32* m_pUserPal;

  HISTONODE ** m_ppHisto; // Array of 32x32x32 color bins. Allocated
                          // sparsely to save memory.
  QUBOX * m_pQuBoxes;     // Array of 256 boxes, one for each color.
};

inline int PLFilterQuantize::clip (int c) const
{
  if (c > 255)
    return 255;
  if (c < 0)
    return 0;
  return c;
}

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterquantize.h,v $
|      Revision 1.1  2009/12/09 05:33:42  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:10  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:53  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:49  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:01  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:18  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:22  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:30:29  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.4  2002/11/18 14:45:24  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.3  2002/02/24 13:00:47  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/09/15 20:39:57  uzadow
|      Added Quantization to user-defined palette (Andreas Köpf)
|
|      Revision 1.3  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.2  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.1  2000/09/26 12:14:49  Administrator
|      Refactored quantization.
|
|
|
\--------------------------------------------------------------------
*/
