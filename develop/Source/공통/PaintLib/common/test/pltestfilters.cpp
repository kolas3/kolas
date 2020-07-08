/*
/--------------------------------------------------------------------
|
|      $Id: pltestfilters.cpp,v 1.1 2009/12/09 05:34:02 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "pltestfilters.h"
#include "plpaintlibdefs.h"
#include "planybmp.h"

#include "plpixel8.h"

#include "Filter/plfilter.h"
#include "Filter/plfiltercontrast.h"
#include "Filter/plfiltercrop.h"
#include "Filter/plfilterintensity.h"
#include "Filter/plfilterlightness.h"
#include "Filter/plfilterquantize.h"
#include "Filter/plfilterresizebilinear.h"
#include "Filter/plfilterresizegaussian.h"
#include "Filter/plfilterflip.h"
#include "Filter/plfiltermirror.h"
#include "Filter/plfilterfillrect.h"
#include "Filter/plfilterfill.h"

#include "testfilterresizebilinear.h"

PLTestFilters::PLTestFilters(PLBmp* pBmp32, PLBmp* pBmp8)
  : m_pBmp32 (pBmp32),
    m_pBmp8 (pBmp8)
{
}

PLTestFilters::~PLTestFilters(void)
{
}

void PLTestFilters::RunTests ()
{
  PLTRACE ("  Testing contrast...\n");
  testFilterWithBmp (PLFilterContrast (1, 128), *m_pBmp32);

  PLTRACE ("  Testing crop...\n");
  PLTRACE ("    32 bpp...\n");
  testFilterWithBmp (PLFilterCrop (0,0,5,5), *m_pBmp32);
  PLTRACE ("    8 bpp...\n");
  testFilterWithBmp (PLFilterCrop (0,0,5,5), *m_pBmp8);

  PLTRACE ("  Testing intensity...\n");
  testFilterWithBmp (PLFilterIntensity (20, 128, 1), *m_pBmp32);

  PLTRACE ("  Testing lightness...\n");
  testFilterWithBmp (PLFilterLightness (0), *m_pBmp32);

  PLTRACE ("  Testing quantization...\n");
  PLTRACE ("    Testing median cut...\n");
  testQuant (PLDTHPAL_MEDIAN, PLDTH_NONE);
  PLTRACE ("    Testing popularity sort...\n");
  testQuant (PLDTHPAL_POPULARITY, PLDTH_NONE);
  PLTRACE ("    Testing default palette...\n");
  testQuant (PLDTHPAL_DEFAULT, PLDTH_NONE);
  PLTRACE ("    Testing user-defined palette...\n");
  testQuant (PLDTHPAL_USERDEFINED, PLDTH_NONE);
  PLTRACE ("    Testing ordered dithering...\n");
  testQuant (PLDTHPAL_MEDIAN, PLDTH_ORDERED);
  PLTRACE ("    Testing floyd-steinberg dithering...\n");
  testQuant (PLDTHPAL_MEDIAN, PLDTH_FS);

  {
    PLTestFilterResizeBilinear ResizeTester;
    ResizeTester.RunTests();
  }

  PLTRACE ("  Testing gauss filter...\n");
  testFilterWithBmp (PLFilterResizeGaussian (5,5,0.2), *m_pBmp32);

  PLTRACE ("  Testing flip...\n");
  testDoubleFilter (PLFilterFlip());

  PLTRACE ("  Testing mirror...\n");
  testDoubleFilter (PLFilterMirror());

  PLTRACE ("  Testing fillrect...\n");
  testFillRect ();

  PLTRACE ("  Testing fill...\n");
  testFill ();
}

void PLTestFilters::testFilterWithBmp (const PLFilter& Filter, PLBmp& SrcBmp)
{
  PLAnyBmp DestBmp;
  DestBmp.CreateFilteredCopy (SrcBmp, Filter);
  Test (SrcBmp.AlmostEqual (DestBmp, 4));

  DestBmp = SrcBmp;
  DestBmp.ApplyFilter (Filter);
  Test (SrcBmp.AlmostEqual (DestBmp, 8));
}

void PLTestFilters::testDoubleFilter (const PLFilter& Filter)
{
  PLAnyBmp DestBmp1;
  PLAnyBmp DestBmp2;
  const_cast<PLFilter&>(Filter).Apply (m_pBmp32, &DestBmp1);
  const_cast<PLFilter&>(Filter).Apply (&DestBmp1, &DestBmp2);
  Test (m_pBmp32->AlmostEqual (DestBmp2, 4));
}


void PLTestFilters::testQuant (int DthPal, int Dth)
{
  PLAnyBmp DestBmp8;
  PLAnyBmp DestBmp32;
  PLFilterQuantize Q (DthPal, Dth);
  if (DthPal == PLDTHPAL_USERDEFINED)
    Q.SetUserPalette(PLFilterQuantize::GetDefaultPalette ());
  Q.Apply(m_pBmp32, &DestBmp8);
  DestBmp32.CreateCopy (DestBmp8, 32);
  Test (m_pBmp32->AlmostEqual (DestBmp32, 4));
}

void PLTestFilters::testFillRect ()
{
  PLAnyBmp Bmp8;
  Bmp8.Create (4, 4, 8, false, true);
  Bmp8.ApplyFilter (PLFilterFillRect<PLPixel8> (PLPoint (0, 0), PLPoint (4, 4), PLPixel8(0)));
  Bmp8.ApplyFilter (PLFilterFillRect<PLPixel8> (PLPoint (1, 1), PLPoint (3, 3), PLPixel8(255)));
  PLPixel8 ** ppLines = (PLPixel8**)Bmp8.GetLineArray();
  for (int i=0; i<4; ++i)
  {
    PLASSERT (ppLines[0][i] == 0);
    PLASSERT (ppLines[3][i] == 0);
    PLASSERT (ppLines[i][0] == 0);
    PLASSERT (ppLines[i][3] == 0);
  }
  PLASSERT (ppLines[1][1] == 255);
  PLASSERT (ppLines[1][2] == 255);
  PLASSERT (ppLines[2][1] == 255);
  PLASSERT (ppLines[2][2] == 255);

  PLFilterFillRect<PLPixel32> Filter32(PLPoint (0, 0), PLPoint (15, 15), PLPixel32(0,0,0,0));
}

void PLTestFilters::testFill ()
{
  PLAnyBmp Bmp;
  Bmp.Create (5, 5, 32, true, false);
  PLPixel32 color(1,2,3,4);
  Bmp.ApplyFilter (PLFilterFill (color));
  PLBYTE ** ppLines = Bmp.GetLineArray();
  for (int y=0; y<5; y++)
    for (int x=0; x<5; x++)
    {
      PLPixel32 * pPix = (PLPixel32*)&(ppLines[y][x*4]);
      PLASSERT (*pPix == color);
    }
}

/*
/--------------------------------------------------------------------
|
|      $Log: pltestfilters.cpp,v $
|      Revision 1.1  2009/12/09 05:34:02  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:48  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:11  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:01  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:28  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.1  2002/08/04 20:12:34  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|      Major improvements in tests.
|
|
|
\--------------------------------------------------------------------
*/
