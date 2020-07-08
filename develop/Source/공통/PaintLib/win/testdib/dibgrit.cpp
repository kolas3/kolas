/*
/--------------------------------------------------------------------
|
|      $Id: dibgrit.cpp,v 1.1 2009/12/09 05:35:01 박종운 Exp $
|      Bitmap Graphic item class
|
|      A bitmap on a canvas.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "dibgrit.h"
#include "plbitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CDIBGrItem, CGrItem);


CDIBGrItem::CDIBGrItem
    ( int x,            // Position on the canvas
      int y,
      int w,            // Width
      int h,            // Height
      int z,            // Position in z-Order
      PLBYTE Opacity,     // Opacity of the object. 255 is completely
                        // opaque, 0 is completely transparent.
      PLBmp * pBmp
    )
  : CGrItem (x, y, w, h, z, Opacity)
{
//  ASSERT_VALID (pBmp);
  ASSERT (pBmp->GetBitsPerPixel() == 32);

  m_pBmp = pBmp;
}


CDIBGrItem::~CDIBGrItem
    ()
{
}

void CDIBGrItem::Draw
    ( PLBmp * pCanvas,
      CRect * pUpdateRect
    )
    // Responsible for drawing the object on the canvas.
{
//  ASSERT_VALID (pCanvas);
  ASSERT (pCanvas->GetBitsPerPixel() == 32);

//  ASSERT_VALID (m_pBmp);
  ASSERT (m_pBmp->GetBitsPerPixel() == 32);

  // Perform clipping.
  CRect ClipRect (pUpdateRect);
  clip (pCanvas, &ClipRect);

  // Draw
  if (m_w == m_pBmp->GetWidth() &&
      m_h == m_pBmp->GetHeight())
    drawClippedNoScale (pCanvas, &ClipRect);
  else
    if (m_pBmp->HasAlpha())
      drawClipped (pCanvas, &ClipRect);
     else
      drawClippedNoAlpha (pCanvas, &ClipRect);

  // If this fails, the procedure destroyed something.
//  ASSERT_VALID (pCanvas);
//  ASSERT_VALID (m_pBmp);
}


void CDIBGrItem::drawClippedNoScale
    ( PLBmp * pCanvas,
      CRect * pRect
    )
    // Draws the object. pRect must have been clipped already.
    // Assumes that no scaling is nessesary.
{
  PLBYTE * pDest;
  PLBYTE * pSrc;

  pCanvas->Lock (true, true);  // Need read access because of alpha
  PLBYTE ** pDestLineArray = pCanvas->GetLineArray();
  m_pBmp->Lock (true, false);
  PLBYTE ** pSrcLineArray = m_pBmp->GetLineArray();
  int SrcXOfs = (pRect->left-m_x); // Starting X-Coordinate in the
                                   // source bitmap. 0 unless clipped.

  for (int sy = pRect->top; sy<pRect->bottom; sy++)
  { // For each line

    // Set up pointers to beginnings of lines.
    pDest = pDestLineArray[sy] + pRect->left*4;
    pSrc = pSrcLineArray[sy-m_y] + SrcXOfs*4;

    if (m_pBmp->HasAlpha())
      drawAlphaLine (pDest, pSrc, pRect);
    else
      // No alpha channel.
      if (m_Opacity == 255)
        wmemcpy (pDest, pSrc, 4*pRect->Width());
      else
        drawFadeLine (pDest, pSrc, pRect);
  }
  __asm emms
  m_pBmp->Unlock();
  pCanvas->Unlock();
}

// Disable 'no EMMS instruction' warning. The calling function contains
// the emms instruction, so everything's ok.
#pragma warning (disable: 4799)  
inline void CDIBGrItem::drawAlphaLine
    ( PLBYTE * pDest,
      PLBYTE * pSrc,
      CRect * pRect
    )
    // Draws one line. No scaling. Assumes alpha channel exists.
{
  
  PLBYTE * pSrcStart = pSrc+(pRect->left-m_x)*4;
  __int64 Alpha = __int64(m_Opacity) + (__int64(m_Opacity)<<16)
                + (__int64(m_Opacity)<<32) + (__int64(m_Opacity)<<48);
  __int64 Negator = 0x00ff00ff00ff00ff;
  int LoopLen = pRect->right-pRect->left+1;

    __asm 
    {
      mov ecx, LoopLen

      mov edi, pDest

      // Set up constants.
      mov esi, pSrcStart

      movq mm7, Alpha
      movq mm6, Negator
      pxor mm0, mm0

          // Calculate current alpha.
          movzx eax, byte ptr [esi+PL_RGBA_ALPHA]
          mov ebx, eax
          shl ebx, 8
          or  ebx, eax
          shl ebx, 8

  startofloop:
      ALIGN 16
          or  ebx, eax
              // Load source, do alpha calc.
              movd mm1, [esi]
          movd mm3, ebx
                    add edi, 4
          punpcklbw mm3, mm0
              punpcklbw mm1, mm0
          pmullw mm3, mm7
                // Load destination, do neg. alpha calc.
                movd mm2, [edi-4]
                      add esi, 4
          psrlw mm3, 8
                punpcklbw mm2, mm0
              pmullw mm1, mm3
                pxor mm3, mm6
                pmullw mm2, mm3
			    dec ecx
			    je endofloop

          // Calculate alpha for next iteration
          movzx eax, byte ptr [esi+PL_RGBA_ALPHA]

      paddusw mm1, mm2
          mov ebx, eax
      psrlw mm1, 8
          shl ebx, 8

      // Write results back.
      packuswb mm1, mm0
          or  ebx, eax
      movd [edi-4], mm1
          shl ebx, 8
	  jmp startofloop
	endofloop:
      
    }
}
#pragma warning (default: 4799)  


inline void CDIBGrItem::drawFadeLine
    ( PLBYTE * pDest,
      PLBYTE * pSrc,
      CRect * pRect
    )
    // Draws one line. No scaling. Assumes alpha channel doesn't
    // exist.
{
  int negalpha;
  negalpha = 255-m_Opacity;

  for (int sx = pRect->left; sx<pRect->right; sx++)
  { // For each pixel
    pDest[PL_RGBA_RED] = (pDest[PL_RGBA_RED]*negalpha +
                       pSrc[PL_RGBA_RED]*m_Opacity)>>8;
    pDest[PL_RGBA_GREEN] = (pDest[PL_RGBA_GREEN]*negalpha +
                         pSrc[PL_RGBA_GREEN]*m_Opacity)>>8;
    pDest[PL_RGBA_BLUE] = (pDest[PL_RGBA_BLUE]*negalpha +
                        pSrc[PL_RGBA_BLUE]*m_Opacity)>>8;
    pDest += 4;
    pSrc += 4;
  }
}


void CDIBGrItem::drawClipped
    ( PLBmp * pCanvas,
      CRect * pRect
    )
    // Draws the object. pRect must have been clipped already.
{
  PLBYTE * pDest;
  PLBYTE * pSrc;
  PLBYTE * pSrcStart;
  PLBYTE * pAlpha = NULL;

  // Invisible.
  if (m_Opacity < 3)
    return;

  pCanvas->Lock (true, true);
  PLBYTE ** pDestLineArray = pCanvas->GetLineArray();
  m_pBmp->Lock (true, false);
  PLBYTE ** pSrcLineArray = m_pBmp->GetLineArray();

  // Calculate Loop invariants.
  double YScale = (double)(m_pBmp->GetHeight())/m_h;
  int ys = (int)(YScale*65536);
  int SrcLineNum;

  double XScale = (double)(m_pBmp->GetWidth())/m_w;
  int xs = (int)(XScale*65536)*4;

  __int64 Alpha = __int64(m_Opacity) + (__int64(m_Opacity)<<16)
                + (__int64(m_Opacity)<<32) + (__int64(m_Opacity)<<48);
  int SrcStart = ((pRect->left-m_x)*xs)<<16;
  __int64 Negator = 0x00ff00ff00ff00ff;
  int LoopStart = pRect->left-m_x;
  int LoopLen = pRect->right-pRect->left+1;

  for (int sy = pRect->top; sy<pRect->bottom; sy++)
  { // For each line

    pDest = pDestLineArray[sy] + pRect->left*4;
    SrcLineNum = (int)((sy-m_y)*ys)>>16;
    pSrc = pSrcLineArray[SrcLineNum];
    pSrcStart = pSrc+SrcStart;
  
    __asm 
    {
      mov ecx, LoopLen

      mov edi, pDest
      mov edx, 0

      // Set up constants.
      mov esi, pSrcStart

      movq mm7, Alpha
      movq mm6, Negator
      pxor mm0, mm0

          // Calculate current alpha.
          movzx eax, byte ptr [esi+PL_RGBA_ALPHA]
          mov ebx, eax
          shl ebx, 8
          or  ebx, eax
          shl ebx, 8

  startofloop:
      ALIGN 16
          or  ebx, eax
              // Load source, do alpha calc.
              movd mm1, [esi]
          movd mm3, ebx
                    add edi, 4
          punpcklbw mm3, mm0
              punpcklbw mm1, mm0
          pmullw mm3, mm7
                // Load destination, do neg. alpha calc.
                movd mm2, [edi-4]
                       // Calc source address for next iteration.
                      add edx, xs
                punpcklbw mm2, mm0
                      mov esi, edx
          psrlw mm3, 8
                      shr esi, 16
              pmullw mm1, mm3
                pxor mm3, mm6
                      add esi, pSrcStart
                pmullw mm2, mm3
                      and esi, 0xfffffffc

          // Calculate alpha for next iteration
          movzx eax, byte ptr [esi+PL_RGBA_ALPHA]

      paddusw mm1, mm2
          mov ebx, eax
      psrlw mm1, 8
          shl ebx, 8

      // Write results back.
      packuswb mm1, mm0
          or  ebx, eax
      movd [edi-4], mm1
          shl ebx, 8

      dec ecx
      jnz startofloop

    }
  }
  __asm         emms

  m_pBmp->Unlock();
  pCanvas->Unlock();
}

/* This is the unoptimized assembler version 2.0. */
/*
  int LoopStart = pRect->left-m_x;
  int LoopEnd = pRect->right-m_x;
  int Opacity = m_Opacity;
  __int64 Negator = 0x00ff00ff00ff00ff;
  __int64 Alpha = __int64(m_Opacity) + (__int64(m_Opacity)<<16)
                + (__int64(m_Opacity)<<32) + (__int64(m_Opacity)<<48);
  __int64 temp;
  
  __asm 
  {
    push si
    push di
    mov ecx, LoopStart
    mov edi, pDest
    movq mm7, Alpha

startofloop:

    // Calculate source address
    mov eax, xs
    mul ecx
    shr eax, 16     // This is 16 to compensate for fixed-point calculations
    shl eax, 2      // Every pixel is 4 bytes wide. 
                    // Can't do these two in one step because we need the last two 
                    // bits to be zero.
    mov esi, pSrc 
    add esi, eax

    // Calculate current alpha.
    movq mm3, mm7
    cmp bAlpha, 0
    jz endalphacalc
    mov ebx, 0
    mov bl, [esi+PL_RGBA_ALPHA]
    mov bh, bl
    mov edx, ebx
    shl edx, 16
    add ebx, edx
    movd mm3, ebx
    punpcklbw mm3, mm0
    pmullw mm3, mm7
    psrlw mm3, 8

endalphacalc:
    movd ebx, mm3  
    cmp bl, 4      // Completely transparent
    jb  endofloop
    cmp bl, 0xfc    
    jb  realalpha
    // Completely opaque
    mov eax, [esi]
    mov [edi], eax
    jmp endofloop

realalpha:
    // Die Konstante 0 speichern.
    pxor mm0, mm0

    // Load source, do alpha calc.
    movd mm1, [esi]
    punpcklbw mm1, mm0
    movq temp, mm1
    pmullw mm1, mm3
    movq temp, mm1
    psrlw mm1, 8

    // Load destination, do neg. alpha calc.
    movd mm2, [edi]
    punpcklbw mm2, mm0
    pxor mm3, Negator
    pmullw mm2, mm3
    psrlw mm2, 8

    paddusw mm1, mm2

    // Write results back.
    packuswb mm1, mm0
    movd [edi], mm1

endofloop:
    add edi, 4
    inc ecx
    cmp ecx, LoopEnd
    jl startofloop

    emms
    pop si
    pop di
  }
*/
/* Original C++ version
  int SrcOfs;
  int alpha;
  int negalpha;

  for (int sx = pRect->left-m_x; sx<pRect->right-m_x; sx++)
  { // For each pixel

    SrcOfs = (sx*xs)>>16;
    if (bAlpha)
      alpha = ((pSrc[SrcOfs*4+PL_RGBA_ALPHA]) * m_Opacity)>>8;
     else
      alpha = m_Opacity;
    negalpha = 255-alpha;
    pDest[PL_RGBA_RED] = (pDest[PL_RGBA_RED]*negalpha +
                       pSrc[SrcOfs*4+PL_RGBA_RED]*alpha)>>8;
    pDest[PL_RGBA_GREEN] = (pDest[PL_RGBA_GREEN]*negalpha +
                         pSrc[SrcOfs*4+PL_RGBA_GREEN]*alpha)>>8;
    pDest[PL_RGBA_BLUE] = (pDest[PL_RGBA_BLUE]*negalpha +
                        pSrc[SrcOfs*4+PL_RGBA_BLUE]*alpha)>>8;

    pDest += 4;

  }
*/


void CDIBGrItem::drawClippedNoAlpha
    ( PLBmp * pCanvas,
      CRect * pRect
    )
    // Draws the object. pRect must have been clipped already.
{
  PLBYTE * pDest;
  PLBYTE * pSrc;
  PLBYTE * pSrcStart;
  PLBYTE * pAlpha = NULL;

  // Invisible.
  if (m_Opacity < 3)
    return;

  pCanvas->Lock (false, true);
  PLBYTE ** pDestLineArray = pCanvas->GetLineArray();
  m_pBmp->Lock (true, false);
  PLBYTE ** pSrcLineArray = m_pBmp->GetLineArray();

  // Calculate Loop invariants.
  double YScale = (double)(m_pBmp->GetHeight())/m_h;
  int ys = (int)(YScale*65536);
  int SrcLineNum;

  double XScale = (double)(m_pBmp->GetWidth())/m_w;
  int xs = (int)(XScale*65536)*4;

  __int64 Alpha = __int64(m_Opacity) + (__int64(m_Opacity)<<16)
                + (__int64(m_Opacity)<<32) + (__int64(m_Opacity)<<48);
  int SrcStart = ((pRect->left-m_x)*xs)<<16;
  __int64 Negator = 0x00ff00ff00ff00ff;
  int LoopStart = pRect->left-m_x;
  int LoopLen = pRect->right-pRect->left+1;

  for (int sy = pRect->top; sy<pRect->bottom; sy++)
  { // For each line

    pDest = pDestLineArray[sy] + pRect->left*4;
    SrcLineNum = (int)((sy-m_y)*ys)>>16;
    pSrc = pSrcLineArray[SrcLineNum];
    pSrcStart = pSrc+SrcStart;
  
    __asm 
    {
      mov ecx, LoopLen

      mov edi, pDest
      mov edx, 0

      // Set up constants.
      mov esi, pSrcStart

      movq mm3, Alpha
      movq mm4, mm3
      movq mm6, Negator
      pxor mm4, mm6
      pxor mm0, mm0

  startofloop:
      ALIGN 16
              // Load source, do alpha calc.
              movd mm1, [esi]
                    add edi, 4
              punpcklbw mm1, mm0
                // Load destination, do neg. alpha calc.
                movd mm2, [edi-4]
                       // Calc source address for next iteration.
                      add edx, xs
                punpcklbw mm2, mm0
                      mov esi, edx
              pmullw mm1, mm3
                      shr esi, 16
                pmullw mm2, mm4
                      add esi, pSrcStart
                      and esi, 0xfffffffc

      paddusw mm1, mm2
      psrlw mm1, 8

      // Write results back.
      packuswb mm1, mm0
      movd [edi-4], mm1

      dec ecx
      jnz startofloop

    }
  }
  __asm         emms

  m_pBmp->Unlock();
  pCanvas->Unlock();
}
/*
/--------------------------------------------------------------------
|
|      $Log: dibgrit.cpp,v $
|      Revision 1.1  2009/12/09 05:35:01  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:14  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:26  PDJ
|      *** empty log message ***
|
|      Revision 1.2  2007/10/30 04:20:21  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:50  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:34  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:54  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:02  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.8  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.7  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.6  2001/09/15 17:00:35  uzadow
|      Fixed bug where too much memory is read.
|
|      Revision 1.5  2000/08/13 12:11:44  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.4  2000/01/16 20:43:19  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  1999/10/21 16:06:55  Ulrich von Zadow
|      Added #pragma warning off
|
|
--------------------------------------------------------------------
*/
