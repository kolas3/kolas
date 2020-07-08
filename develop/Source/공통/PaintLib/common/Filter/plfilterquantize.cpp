/*
/--------------------------------------------------------------------
|
|      $Id: plfilterquantize.cpp,v 1.1 2009/12/09 05:33:42 ¹ÚÁ¾¿î Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
|      Based on code:
|
|        Copyright (C) 1998 - 2000 by Punk Productions Electronic Entertainment.
|                                http://cust.nol.at/ppee
|        Content: Quantization/Dithering
|        Programmer: Nikolaus Brennig (virtualnik@nol.at)
|
|      and
|
|        DIBQuant version 1.0
|        Copyright (c) 1993 Edward McCreary.
|        All rights reserved.
|
|        Redistribution and use in source and binary forms are freely permitted
|        provided that the above copyright notice and attibution and date of work
|        and this paragraph are duplicated in all such forms.
|        THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
|        IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
|        WARRANTIES OF MERCHANTIBILILTY AND FITNESS FOR A PARTICULAR PURPOSE.
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterquantize.h"
#include "plbitmap.h"
#include "plexcept.h"

#ifndef  min
#define min(a, b)       ((a) < (b) ? (a) : (b))
#endif

#ifndef  max
#define max(a, b)       ((a) < (b) ? (b) : (a))
#endif

#define COLOR_MAX     32

PLFilterQuantize::PLFilterQuantize (int DitherPaletteType, int DitherType)
  : PLFilter(),
    m_DitherPaletteType (DitherPaletteType),
    m_DitherType (DitherType),
    m_ppHisto (NULL),
    m_pQuBoxes (NULL),
    m_pUserPal (NULL)
{
  initLUT();
}

PLFilterQuantize::~PLFilterQuantize()
{
  deleteLUT();
  if (m_pUserPal != NULL)
  {
    delete[] m_pUserPal;
    m_pUserPal = NULL;
  }
}

void PLFilterQuantize::Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  PLBYTE ** pDstLines = pBmpDest->GetLineArray();

  // Only works for 32 bpp bitmaps.
  PLASSERT (pBmpSource->GetBitsPerPixel() == 32);

  pBmpDest->Create (pBmpSource->GetWidth(), pBmpSource->GetHeight(), 8,
                    false, false, pBmpSource->GetResolution());

  if (m_DitherPaletteType != PLDTHPAL_DEFAULT &&
      m_DitherPaletteType != PLDTHPAL_USERDEFINED)
    genColorArray(pBmpSource);

  // Generate the palette.
  switch (m_DitherPaletteType)
  {
    case PLDTHPAL_MEDIAN:
      genMedianPalette (pBmpSource, pBmpDest);
      break;
    case PLDTHPAL_POPULARITY:
      genPopularityPalette(pBmpSource, pBmpDest);
      break;
    case PLDTHPAL_DEFAULT:
      genDefaultPalette(pBmpDest);
      break;
    case PLDTHPAL_USERDEFINED:
      // The destination bitmap contains already a palette (e.g. a custom halftone palette)
      if (m_pUserPal)
        pBmpDest->SetPalette(m_pUserPal);
      else
        genDefaultPalette(pBmpDest);    // Fallback to the default palette...
      break;
    default:
      // Incorrect palette type specified in constructor.
      PLASSERT (false);
  }

  ditherDestBmp(pBmpSource, pBmpDest);
}

void PLFilterQuantize::SetUserPalette(const PLPixel32* pPal)
{
  if (m_pUserPal != NULL)
    delete[] m_pUserPal;
  m_pUserPal = new PLPixel32[256];
  wmemcpy(m_pUserPal, pPal, sizeof(PLPixel32)*256);
}

// Initialize local variables
void PLFilterQuantize::initLUT()
{
  PLULONG i;

  m_pQuBoxes = new QUBOX[256];
  if (!m_pQuBoxes)
    throw (PLTextException (PL_ERRNO_MEMORY, "Out of memory."));

  PLULONG len = (long)COLOR_MAX*(long)COLOR_MAX*(long)COLOR_MAX;

  // buffer to store every color in image
  m_ppHisto = new HISTONODE*[len];
  for (i=0; i<len; i++)
    m_ppHisto[i] = NULL;
}

void PLFilterQuantize::deleteLUT()
{
  delete[] m_pQuBoxes;

  int len = (long)COLOR_MAX*(long)COLOR_MAX*(long)COLOR_MAX;

  int i;
  if (m_ppHisto)
    for (i=0; i<len; i++)
      if (m_ppHisto[i])
        delete m_ppHisto[i];

  delete[] m_ppHisto;
}


void PLFilterQuantize::genMedianPalette (PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  //--------------------------------------------------------------
  // build new palette with median cut algorithm
  // I didn't write this, if you know who did, please let me know!
  //                                               Ed McCreary
  //--------------------------------------------------------------

  /* force the counts in the corners to be zero */
  addColor (PLPixel32 (0,0,0,0), 0L);
  addColor (PLPixel32 (31,0,0,0), 0L);
  addColor (PLPixel32 (0,31,0,0), 0L);
  addColor (PLPixel32 (0,0,31,0), 0L);
  addColor (PLPixel32 (31,31,0,0), 0L);
  addColor (PLPixel32 (0,31,31,0), 0L);
  addColor (PLPixel32 (31,0,31,0), 0L);
  addColor (PLPixel32 (31,31,31,0), 0L);

  /* assign the 1st eight boxes to be the corners */
  makeBox (PLPixel32 (0,0,0,0), 0, 1L);
  makeBox (PLPixel32 (31,0,0,0), 1, 1L);
  makeBox (PLPixel32 (0,31,0,0), 2, 1L);
  makeBox (PLPixel32 (0,0,31,0), 3, 1L);
  makeBox (PLPixel32 (31,31,0,0), 4, 1L);
  makeBox (PLPixel32 (0,31,31,0), 5, 1L);
  makeBox (PLPixel32 (31,0,31,0), 6, 1L);
  makeBox (PLPixel32 (31,31,31,0), 7, 1L);

  /* set up 9th box to hold the rest of the world */
  m_pQuBoxes[8].Corner0 = PLPixel32 (0,0,0,0);
  m_pQuBoxes[8].Corner1 = PLPixel32 (31,31,31,0);
  squeeze(&m_pQuBoxes[8]);


  /* split the rest of the boxes */
  int i;
  for(i=9; i<256; i++)
  {
    /* find biggest box */
    int Biggest = 8;
    int j;
    for( j=8; j<i; j++ )
      if(m_pQuBoxes[j].count > m_pQuBoxes[Biggest].count)
        Biggest = j;
    if (m_pQuBoxes[Biggest].count > 1)
    {
      /* decide which side to split the box along, and split it */
      PLPixel32 Corner0 = m_pQuBoxes[Biggest].Corner0;
      PLPixel32 Corner1 = m_pQuBoxes[Biggest].Corner1;
      int dr = Corner1.GetR () - Corner0.GetR ();
      int dg = Corner1.GetG () - Corner0.GetG ();
      int db = Corner1.GetB () - Corner0.GetB ();
      m_pQuBoxes[i] = m_pQuBoxes[Biggest];      /* copy info over */

      // Determine at which color component to split.
      int ColComp;
      if(dr>=dg && dr>=db)
        ColComp = PL_RGBA_RED;
      else if(dg>=db)
        ColComp = PL_RGBA_GREEN;
      else
        ColComp = PL_RGBA_BLUE;

      split (&m_pQuBoxes[i],&m_pQuBoxes[Biggest], ColComp);
    }
    else
    { // The biggest box has size 1. This means we've found all the colors.
      // Set rest of the palette to black.
      for (j=i; j<256; j++)
      {
        m_pQuBoxes[j].Corner0 = PLPixel32 (0,0,0,0);
        m_pQuBoxes[j].Corner1 = PLPixel32 (0,0,0,0);
        m_pQuBoxes[j].Average = PLPixel32 (0,0,0,0);
        m_pQuBoxes[j].count = 0;
      }

      // And make the loop exit.
      i = 256;
    }
  } /* end of i loop, we've found 256 boxes. */


  /* Generate the actual palette */
  PLPixel32 * pClrTab = pBmpDest->GetPalette();

  for( i=0; i<256; i++ )
  {
    // For each color component, find the average of the two corners of the box and
    // scale by the amount of colors in the palette.
    PLBYTE r = PLBYTE(((long)(m_pQuBoxes[i].Corner0.GetR ()+m_pQuBoxes[i].Corner1.GetR ()))*255/(2*31));
    PLBYTE g = PLBYTE(((long)(m_pQuBoxes[i].Corner0.GetG ()+m_pQuBoxes[i].Corner1.GetG ()))*255/(2*31));
    PLBYTE b = PLBYTE(((long)(m_pQuBoxes[i].Corner0.GetB ()+m_pQuBoxes[i].Corner1.GetB ()))*255/(2*31));
    pClrTab[i].Set (r,g,b,0);
  }
}

// Split a box into two at the average color.
// (The actual split is done before the function is called. split() just
// calculates new upper and lower bounds for the two boxes.)
// ColComp is the color component that the split is done on, i.e. one of
// PL_RGBA_RED, PL_RGBA_GREEN, PL_RGBA_BLUE
void PLFilterQuantize::split (QUBOX * pBox0, QUBOX * pBox1, int ColComp) const
{
  // If we had good accessor functions for the color components, this
  // would be more readable.
  PLBYTE * pB0C0 = (PLBYTE *)&(pBox0->Corner0);  // Box0, Color 0
  PLBYTE * pB0C1 = (PLBYTE *)&(pBox0->Corner1);
  PLBYTE * pB0Ave = (PLBYTE *)&(pBox0->Average);
  PLBYTE * pB1C0 = (PLBYTE *)&(pBox1->Corner0);
  PLBYTE * pB1C1 = (PLBYTE *)&(pBox1->Corner1);
  PLBYTE * pB1Ave = (PLBYTE *)&(pBox1->Average);

  if (pB0C1[ColComp]-pB0C0[ColComp] == 2)
  { /* tight squeeze */
    pB0C1[ColComp] = pB0C0[ColComp];
    pB1C0[ColComp] = pB1C1[ColComp];
  }
  else
  { /* figure out where to split */
    PLBYTE j = pB1Ave[ColComp];
    if (j == pB1C1[ColComp])
      j--;
    pB1C1[ColComp] = j;
    pB0C0[ColComp] = j+1;
  }
  PLASSERT (pB0C0[ColComp] < 32);
  PLASSERT (pB1C0[ColComp] < 32);
  PLASSERT (pB0C1[ColComp] < 32);
  PLASSERT (pB1C1[ColComp] < 32);

  squeeze (pBox0);
  squeeze (pBox1);

}

// shrink a boxes extremes to fit tightly
// if a box is 1x1x1 change its count to 1
void PLFilterQuantize::squeeze(QUBOX * pBox) const
{
  PLPixel32 Corner0, Corner1;

  Corner0 = pBox->Corner0;
  Corner1 = pBox->Corner1;

  pBox->Corner0 = PLPixel32 (31,31,31,0);
  pBox->Corner1 = PLPixel32 (0,0,0,0);
  pBox->Average = PLPixel32 (0,0,0,0);

  int r,g,b;
  int rsum = 0;
  int gsum = 0;
  int bsum = 0;
  PLULONG count = 0;
  for (r=Corner0.GetR (); r<=Corner1.GetR (); r++)
  {
    for (g=Corner0.GetG (); g<=Corner1.GetG (); g++)
    {
      for (b=Corner0.GetB (); b<=Corner1.GetB (); b++)
      {
        int index = getColorTableIndex (PLPixel32 (r,g,b,0));
        HISTONODE * pNode = m_ppHisto[index];
        if (pNode)
        {
          if (pNode->count>0L)
          {
            pBox->Corner0 = PLPixel32 (min (r, (int)pBox->Corner0.GetR ()),
                                      min (g, (int)pBox->Corner0.GetG ()),
                                      min (b, (int)pBox->Corner0.GetB ()),
                                      0);
            pBox->Corner1 = PLPixel32 (max (r, (int)pBox->Corner1.GetR ()),
                                      max (g, (int)pBox->Corner1.GetG ()),
                                      max (b, (int)pBox->Corner1.GetB ()),
                                      0);
            rsum += r*pNode->count;
            gsum += g*pNode->count;
            bsum += b*pNode->count;

            count += pNode->count;
          }
        }
      }
    }
  }

  /* box is now shrunk */
  if (count)
    pBox->Average = PLPixel32 ((PLBYTE)(rsum / count),
                              (PLBYTE)(gsum / count),
                              (PLBYTE)(bsum / count),
                              0);

  pBox->count = min (count, (PLULONG)0xFFFFL);

  if (pBox->Corner0 == pBox->Corner1)
  { /* box is min size */
    pBox->count = 1;  /* so it won't get split again */
  }
}

void SwapLong (long* pa, long* pb)
{
  long tmp = *pa;
  *pa = *pb;
  *pb = tmp;
}

void PLFilterQuantize::genPopularityPalette (PLBmp * pBmpSource, PLBmp * pBmpDest) const
{
  PLULONG PalCount[256];
  wmemset (PalCount, 0, sizeof(PLULONG)*256);
  PLPixel32 * pPal = pBmpDest->GetPalette();

  /* force corners of rgb color cube out of the running */
  addColor (PLPixel32 (0,0,0,0), 0L);
  addColor (PLPixel32 (31,0,0,0), 0L);
  addColor (PLPixel32 (0,31,0,0), 0L);
  addColor (PLPixel32 (0,0,31,0), 0L);
  addColor (PLPixel32 (31,31,0,0), 0L);
  addColor (PLPixel32 (0,31,31,0), 0L);
  addColor (PLPixel32 (31,0,31,0), 0L);
  addColor (PLPixel32 (31,31,31,0), 0L);

  /* force feed the corners into the palette */
  pPal[0] = PLPixel32 (0,0,0,0);
  pPal[1] = PLPixel32 (31,0,0,0);
  pPal[2] = PLPixel32 (0,31,0,0);
  pPal[3] = PLPixel32 (0,0,31,0);
  pPal[4] = PLPixel32 (31,31,0,0);
  pPal[5] = PLPixel32 (0,31,31,0);
  pPal[6] = PLPixel32 (31,0,31,0);
  pPal[7] = PLPixel32 (31,31,31,0);

  int r,g,b;
  int IndexCache = -1;
  HISTONODE * pNode;
  HISTONODE * pNodeCache;
  for (r=0; r<31; r++)
  {
    for (g=0; g<31; g++)
    {
      for (b=0; b<31; b++)
      {
        int index = getColorTableIndex (PLPixel32 (r,g,b,0));
        if (index == IndexCache)
          pNode = pNodeCache;
        else
        {
          pNode = m_ppHisto[index];
          pNodeCache = pNode;
          IndexCache = index;
        }

        if (pNode)
        {
          if(pNode->count > PalCount[255])
          {
            PalCount[255] = pNode->count;
            pPal[255].Set (r,g,b,0);

            int i = 255;        /* bubble up */
            while(PalCount[i] > PalCount[i-1] && i>8)
            {
              SwapLong ((long *)&PalCount[i], (long*)&PalCount[i-1]);
              SwapLong ((long *)&pPal[i], (long *)&pPal[i-1]);
              i--;
            }
          }
        }
      }       /* end of current chain */
    }       /* end of r loop */
  }

  int i;
  for (i=0; i<256; i++)
  {
    pPal[i].SetR ((int(pPal[i].GetR ())*255)/31);
    pPal[i].SetG ((int(pPal[i].GetG ())*255)/31);
    pPal[i].SetB ((int(pPal[i].GetB ())*255)/31);
  }
}

void PLFilterQuantize::genColorArray (PLBmp * pBmpSource) const
{
  int x,y;
  int h = pBmpSource->GetHeight();
  int w = pBmpSource->GetWidth();
  PLPixel32 ** pSrcLines = pBmpSource->GetLineArray32();
  for (y=0; y<h; y++)
  {
    PLPixel32 * pSrcPixel = pSrcLines[y];
    for (x=0; x<w; x++)
    {
      addColor(*pSrcPixel, 1L);
      pSrcPixel ++;
    }
  }

}

PLBYTE DefaultPalette[] =
{
  0,0,0,0,
  191,0,0,0,
  0,191,0,0,
  191,191,0,0,
  0,0,191,0,
  191,0,191,0,
  0,191,191,0,
  192,192,192,0,
  192,220,192,0,
  166,202,240,0,
  128,0,0,0,
  0,128,0,0,
  128,128,0,0,
  0,0,128,0,
  128,0,128,0,
  0,128,128,0,
  0,81,0,0,
  81,81,0,0,
  133,81,0,0,
  177,81,0,0,
  217,81,0,0,
  255,81,0,0,
  0,133,0,0,
  81,133,0,0,
  133,133,0,0,
  177,133,0,0,
  217,133,0,0,
  255,133,0,0,
  0,177,0,0,
  81,177,0,0,
  133,177,0,0,
  177,177,0,0,
  217,177,0,0,
  255,177,0,0,
  0,217,0,0,
  81,217,0,0,
  133,217,0,0,
  177,217,0,0,
  217,217,0,0,
  255,217,0,0,
  0,255,0,0,
  81,255,0,0,
  133,255,0,0,
  177,255,0,0,
  217,255,0,0,
  255,255,0,0,
  0,0,81,0,
  81,0,81,0,
  133,0,81,0,
  177,0,81,0,
  217,0,81,0,
  255,0,81,0,
  0,81,81,0,
  81,81,81,0,
  133,81,81,0,
  177,81,81,0,
  217,81,81,0,
  255,81,81,0,
  0,133,81,0,
  81,133,81,0,
  133,133,81,0,
  177,133,81,0,
  217,133,81,0,
  55,133,81,0,
  0,177,81,0,
  81,177,81,0,
  133,177,81,0,
  177,177,81,0,
  217,177,81,0,
  255,177,81,0,
  0,217,81,0,
  81,217,81,0,
  133,217,81,0,
  177,217,81,0,
  217,217,81,0,
  255,217,81,0,
  0,255,81,0,
  81,255,81,0,
  133,255,81,0,
  177,255,81,0,
  217,255,81,0,
  255,255,81,0,
  0,0,133,0,
  81,0,133,0,
  133,0,133,0,
  177,0,133,0,
  217,0,133,0,
  255,0,133,0,
  0,81,133,0,
  81,81,133,0,
  133,81,133,0,
  177,81,133,0,
  217,81,133,0,
  255,81,133,0,
  0,133,133,0,
  81,133,133,0,
  133,133,133,0,
  177,133,133,0,
  217,133,133,0,
  255,133,133,0,
  0,177,133,0,
  81,177,133,0,
  133,177,133,0,
  177,177,133,0,
  217,177,133,0,
  255,177,133,0,
  0,217,133,0,
  81,217,133,0,
  133,217,133,0,
  177,217,133,0,
  217,217,133,0,
  255,217,133,0,
  0,255,133,0,
  81,255,133,0,
  133,255,133,0,
  177,255,133,0,
  217,255,133,0,
  255,255,133,0,
  0,0,177,0,
  81,0,177,0,
  133,0,177,0,
  177,0,177,0,
  217,0,177,0,
  255,0,177,0,
  0,81,177,0,
  81,81,177,0,
  133,81,177,0,
  177,81,177,0,
  217,81,177,0,
  255,81,177,0,
  0,133,177,0,
  81,133,177,0,
  133,133,177,0,
  177,133,177,0,
  217,133,177,0,
  255,133,177,0,
  0,177,177,0,
  81,177,177,0,
  133,177,177,0,
  177,177,177,0,
  217,177,177,0,
  255,177,177,0,
  0,217,177,0,
  81,217,177,0,
  133,217,177,0,
  177,217,177,0,
  217,217,177,0,
  255,217,177,0,
  0,255,177,0,
  81,255,177,0,
  133,255,177,0,
  177,255,177,0,
  217,255,177,0,
  255,255,177,0,
  0,0,217,0,
  81,0,217,0,
  133,0,217,0,
  177,0,217,0,
  217,0,217,0,
  255,0,217,0,
  0,81,217,0,
  81,81,217,0,
  133,81,217,0,
  177,81,217,0,
  217,81,217,0,
  255,81,217,0,
  0,133,217,0,
  81,133,217,0,
  133,133,217,0,
  177,133,217,0,
  217,133,217,0,
  255,133,217,0,
  0,177,217,0,
  81,177,217,0,
  133,177,217,0,
  177,177,217,0,
  217,177,217,0,
  255,177,217,0,
  0,217,217,0,
  81,217,217,0,
  133,217,217,0,
  177,217,217,0,
  217,217,217,0,
  255,217,217,0,
  0,255,217,0,
  81,255,217,0,
  133,255,217,0,
  177,255,217,0,
  217,255,217,0,
  255,255,217,0,
  0,0,255,0,
  81,0,255,0,
  133,0,255,0,
  177,0,255,0,
  217,0,255,0,
  255,0,255,0,
  0,81,255,0,
  81,81,255,0,
  133,81,255,0,
  177,81,255,0,
  217,81,255,0,
  255,81,255,0,
  0,133,255,0,
  81,133,255,0,
  133,133,255,0,
  177,133,255,0,
  217,133,255,0,
  255,133,255,0,
  0,177,255,0,
  81,177,255,0,
  133,177,255,0,
  177,177,255,0,
  217,177,255,0,
  255,177,255,0,
  0,217,255,0,
  81,217,255,0,
  133,217,255,0,
  177,217,255,0,
  217,217,255,0,
  255,217,255,0,
  0,255,255,0,
  81,255,255,0,
  133,255,255,0,
  177,255,255,0,
  217,255,255,0,
  196,196,196,0,
  199,199,199,0,
  202,202,202,0,
  205,205,205,0,
  208,208,208,0,
  211,211,211,0,
  214,214,214,0,
  217,217,217,0,
  220,220,220,0,
  223,223,223,0,
  226,226,226,0,
  229,229,229,0,
  232,232,232,0,
  235,235,235,0,
  238,238,238,0,
  241,241,241,0,
  244,244,244,0,
  247,247,247,0,
  250,250,250,0,
  253,253,253,0,
  155,207,207,0,
  255,251,240,0,
  160,160,164,0,
  128,128,128,0,
  255,0,0,0,
  0,255,0,0,
  255,255,0,0,
  0,0,255,0,
  255,0,255,0,
  0,255,255,0,
  255,255,255,0,
};

const PLPixel32* PLFilterQuantize::GetDefaultPalette ()
{
  return (PLPixel32*)DefaultPalette;
}

void PLFilterQuantize::genDefaultPalette (PLBmp * pBmpDest) const
{
  PLPixel32 * pPal = pBmpDest->GetPalette();
  wmemcpy (pPal, DefaultPalette, 256*4);
}

// Adds a RGB Color into the color histogram.
void PLFilterQuantize::addColor(PLPixel32 col, PLULONG count) const
{
  HISTONODE * pNode;

  PLULONG index = getShiftedColorTableIndex(col);
  count = min (count, (PLULONG)0xFFFFL);

  pNode = m_ppHisto[index];
  if(!pNode)  // new color
  {
    pNode = m_ppHisto[index] = new HISTONODE;
    pNode->index = -1;
    pNode->count = count;
  }
  else
  {
    PLULONG NewCount;
    NewCount = pNode->count;
    NewCount += count;
    pNode->count = min (NewCount, (PLULONG)0xFFFFL);
  }
}

// make a 1x1x1 box at index with color rgb count c
void PLFilterQuantize::makeBox(PLPixel32 col, int i, PLULONG c) const
{
  m_pQuBoxes[i].Corner0 = col;
  m_pQuBoxes[i].Corner1 = col;
  m_pQuBoxes[i].count = c;
}

int PLFilterQuantize::getColorTableIndex (PLPixel32 col) const
{
  int b = col.GetB ();
  int g = col.GetG ();
  int r = col.GetR ();
  return (b | g << 5 | r << 10);
}

int PLFilterQuantize::getShiftedColorTableIndex (PLPixel32 col) const
{
  int b = col.GetB ()>>3;
  int g = col.GetG ()>>3;
  int r = col.GetR ()>>3;
  return (b | g << 5 | r << 10);
}

void PLFilterQuantize::ditherDestBmp(PLBmp * pBmpSource, PLBmp * pBmpDest) const
{

  PLBYTE ** ppSrcLines = pBmpSource->GetLineArray();
  PLBYTE ** ppDestLines = pBmpDest->GetLineArray();

  int w = pBmpSource->GetWidth();
  int h = pBmpSource->GetHeight();

  // FS error accumulators.
  double * pOddErrors;
  double * pEvenErrors;
  double * pCurErrors;
  double * pNextErrors;

  // FS: current pixel as float values
  double r,g,b;

  if (m_DitherType == PLDTH_FS)
  {
    // Get memory for errors
    pOddErrors = new double[(w + 2) * 6];
    pEvenErrors = &pOddErrors[(w + 2) * 3];
    wmemset(pOddErrors, 0, (w + 2) * 6 * sizeof(double));
  }

  // Loop row by row, col by col, and of course dither...
  int y;
  int i;
  int SrcInc, DestInc;
  for (y=0; y<h; y++)
  {
    PLBYTE * pSrcLine = ppSrcLines[y];
    PLBYTE * pDestLine = ppDestLines[y];
    if (m_DitherType == PLDTH_FS)
    { // Floyd-Steinberg dithers in different directions in alternate rows.
      if (y&1) // odd row
      {
        // Start at last pixel in row.
        pSrcLine += w*4-4;
        pDestLine += w-1;
        SrcInc = -4;
        DestInc = -1;

        pCurErrors = pOddErrors + 3;
        pNextErrors = pEvenErrors + (w * 3);
      }
      else // even row
      {
        SrcInc = 4;
        DestInc = 1;

        pCurErrors = pEvenErrors + 3;
        pNextErrors = pOddErrors + (w * 3);
      }

      // Initialize the error constants for the first part of the row
      pNextErrors[0] = 0;
      pNextErrors[1] = 0;
      pNextErrors[2] = 0;
    }
    else
    {
      SrcInc = 4;
      DestInc = 1;
    }

    // go through the scanline...
    i = w;
    while(i--)
    {
      // Get RGB Values...
      PLPixel32 pixel = *(PLPixel32 *)pSrcLine;
      pSrcLine += SrcInc;

      // Now dither...
      switch (m_DitherType)
      {
/*        case PLDTH_JITTER:
          jitterPixel( i, y, &pixel);
          break;
*/
        case PLDTH_ORDERED:
          ditherPixelOrdered (i, y, &pixel);
          break;
        case PLDTH_FS:
          r = pixel.GetR ();
          g = pixel.GetG ();
          b = pixel.GetB ();
          ditherPixelFS(&r, &g, &b, pCurErrors);
          pixel.Set ((PLBYTE)r, (PLBYTE)g, (PLBYTE)b, 0);
          break;
      }

      // Now get the best possible paletteindex matching the RGB Vals.
      PLBYTE index = getNeighbor (pixel, pBmpDest->GetPalette());

      // FS dithering now calculates the error we've introduced by
      // approximating.
      if (m_DitherType == PLDTH_FS)
      {
        PLPixel32 * pPal = pBmpDest->GetPalette();
        double PalR = (double)(pPal[index].GetR ());
        double PalG = (double)(pPal[index].GetG ());
        double PalB = (double)(pPal[index].GetB ());

        double error;

        // Red component
        error = (r - PalR) / 16.0;
        pNextErrors[-3] = error;
        pNextErrors[+3] += error*3;
        pNextErrors[+0] += error*5;
        pCurErrors[+3] += error*7;

        // Green component
        error = (g - PalG) / 16.0;
        pNextErrors[-2]  = error;
        pNextErrors[+4] += error*3;
        pNextErrors[+1] += error*5;
        pCurErrors[+4] += error*7;

        // Blue component
        error = (b - PalB) / 16.0;
        pNextErrors[-1]  = error;
        pNextErrors[+5] += error*3;
        pNextErrors[+2] += error*5;
        pCurErrors[+5] += error*7;

        pNextErrors -= 3;
        pCurErrors += 3;
      }

      // Set the pixel in the destination bitmap
      *pDestLine = (PLBYTE)index;
      pDestLine += DestInc;
    }

  }

  // clean up.
  if (m_DitherType == PLDTH_FS )
    delete [] pOddErrors;

}


void PLFilterQuantize::jitterPixel (int i, int y, PLPixel32 * pPixel) const
{
}

void PLFilterQuantize::ditherPixelOrdered (int x, int y, PLPixel32 * pPixel) const
{
  PLBYTE * pPx = (PLBYTE *)pPixel;
  ditherCompOrdered (x, y, &pPx[PL_RGBA_RED]);
  ditherCompOrdered (x, y, &pPx[PL_RGBA_GREEN]);
  ditherCompOrdered (x, y, &pPx[PL_RGBA_BLUE]);

}

void PLFilterQuantize::ditherCompOrdered (int x, int y, PLBYTE * pComp) const
{
  const int DitherLUT[4][4] = { -7, 1, -5, 3, 5, -3, 7, -1, -4, 4, -6, 2, 8, 0, 6, -2 };

  int Comp = *pComp;
  Comp += DitherLUT [x&3][y&3]<<1;
  Comp = clip (Comp);
  *pComp = (PLBYTE)Comp;
}

void PLFilterQuantize::ditherPixelFS (double * pR, double * pG, double * pB, double * pCurErrors) const
{
  ditherCompFS (pR, pCurErrors[0]);
  ditherCompFS (pG, pCurErrors[1]);
  ditherCompFS (pB, pCurErrors[2]);
}

void PLFilterQuantize::ditherCompFS (double * pComp, double Error) const
{
  *pComp += Error;
  if (*pComp > 255) *pComp = 255;
  if (*pComp < 0) *pComp = 0;
}

// Returns nearest color to the color passed in that's present
// in the palette as a palette index.
PLBYTE PLFilterQuantize::getNeighbor (PLPixel32 Color, PLPixel32 * pPal) const
{
  HISTONODE * pNode;


  int TableIndex = getShiftedColorTableIndex (Color);

  pNode = m_ppHisto[TableIndex];

  if (!pNode)
  {
    m_ppHisto[TableIndex] = new HISTONODE;
    pNode = m_ppHisto[TableIndex];
    pNode->count = 0;
    pNode->index = -1;
  }

  int PalIndex = pNode->index;
  if( PalIndex == -1)
  {
    pNode->index = 0;

    PLPixel32 BestColor = pPal[0];
    int MinDist = colorDist (BestColor, Color);

    int c;
    for (c = 1; c<256; c++)
    {
      int Dist = colorDist (pPal[c], Color);
      if (Dist < MinDist)
      {
        pNode->index = c;
        MinDist = Dist;
      }
    }
    PalIndex = pNode->index;
  }
  return PalIndex;
}

int PLFilterQuantize::colorDist (PLPixel32 c0, PLPixel32 c1) const
{
  int dr = c0.GetR ()-c1.GetR ();
  int dg = c0.GetG ()-c1.GetG ();
  int db = c0.GetB ()-c1.GetB ();
  return dr*dr+dg*dg+db*db;
}

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterquantize.cpp,v $
|      Revision 1.1  2009/12/09 05:33:42  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:10  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:53  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:19  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:49  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:01  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:17  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:22  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:30:26  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.8  2002/11/04 22:40:13  uzadow
|      Updated for gcc 3.1
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
|      Revision 1.4  2001/10/16 17:12:27  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 15:32:22  uzadow
|      Removed types LPBYTE, DWORD, UCHAR, VOID and INT from the code.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2001/09/15 20:39:57  uzadow
|      Added Quantization to user-defined palette (Andreas Köpf)
|
|      Revision 1.6  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.5  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.4  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.3  2000/12/09 12:16:26  uzadow
|      Fixed several memory leaks.
|
|      Revision 1.2  2000/10/23 17:45:03  jmbuena
|      Linux compatibility changes
|
|      Revision 1.1  2000/09/26 12:14:49  Administrator
|      Refactored quantization.
|
|
|
\--------------------------------------------------------------------
*/
