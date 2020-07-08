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
//! Table of the first 200 or so PICT opcodes with size & description.
//! Mostly stolen from pict2pbm. I hope they don't mind.

struct OpDef
{
  char * name;
  int    len;
  char * description;
};

// for reserved opcodes
#define res(length) { "reserved", (length), "reserved for Apple use" }

#define RGB_LEN 6

#define WORD_LEN -1

#define NA 0

static OpDef optable[] =
  {
    /* 0x00 */  { "NOP",               0, "nop" },
    /* 0x01 */  { "Clip",             NA, "clip" },
    /* 0x02 */  { "BkPat",             8, "background pattern" },
    /* 0x03 */  { "TxFont",            2, "text font (word)" },
    /* 0x04 */  { "TxFace",            1, "text face (byte)" },
    /* 0x05 */  { "TxMode",            2, "text mode (word)" },
    /* 0x06 */  { "SpExtra",           4, "space extra (fixed point)" },
    /* 0x07 */  { "PnSize",            4, "pen size (point)" },
    /* 0x08 */  { "PnMode",            2, "pen mode (word)" },
    /* 0x09 */  { "PnPat",             8, "pen pattern" },
    /* 0x0a */  { "FillPat",           8, "fill pattern" },
    /* 0x0b */  { "OvSize",            4, "oval size (point)" },
    /* 0x0c */  { "Origin",            4, "dh, dv (word)" },
    /* 0x0d */  { "TxSize",            2, "text size (word)" },
    /* 0x0e */  { "FgColor",           4, "foreground color (longword)" },
    /* 0x0f */  { "BkColor",           4, "background color (longword)" },
    /* 0x10 */  { "TxRatio",           8, "numerator (point), denominator (point)" },
    /* 0x11 */  { "Version",           1, "version (byte)" },
    /* 0x12 */  { "BkPixPat",         NA, "color background pattern" },
    /* 0x13 */  { "PnPixPat",         NA, "color pen pattern" },
    /* 0x14 */  { "FillPixPat",       NA, "color fill pattern" },
    /* 0x15 */  { "PnLocHFrac",        2, "fractional pen position" },
    /* 0x16 */  { "ChExtra",           2, "extra for each character" },
    /* 0x17 */  res(0),
    /* 0x18 */  res(0),
    /* 0x19 */  res(0),
    /* 0x1a */  { "RGBFgCol",    RGB_LEN, "RGB foreColor" },
    /* 0x1b */  { "RGBBkCol",    RGB_LEN, "RGB backColor" },
    /* 0x1c */  { "HiliteMode",        0, "hilite mode flag" },
    /* 0x1d */  { "HiliteColor", RGB_LEN, "RGB hilite color" },
    /* 0x1e */  { "DefHilite",         0, "Use default hilite color" },
    /* 0x1f */  { "OpColor",           6, "RGB OpColor for arithmetic modes" },
    /* 0x20 */  { "Line",              8, "pnLoc (point), newPt (point)" },
    /* 0x21 */  { "LineFrom",          4, "newPt (point)" },
    /* 0x22 */  { "ShortLine",         6, "pnLoc (point, dh, dv (-128 .. 127))" },
    /* 0x23 */  { "ShortLineFrom",     2, "dh, dv (-128 .. 127)" },
    /* 0x24 */  res(WORD_LEN),
    /* 0x25 */  res(WORD_LEN),
    /* 0x26 */  res(WORD_LEN),
    /* 0x27 */  res(WORD_LEN),
    /* 0x28 */  { "LongText",         NA, "txLoc (point), count (0..255), text" },
    /* 0x29 */  { "DHText",           NA, "dh (0..255), count (0..255), text" },
    /* 0x2a */  { "DVText",           NA, "dv (0..255), count (0..255), text" },
    /* 0x2b */  { "DHDVText",         NA, "dh, dv (0..255), count (0..255), text" },
    /* 0x2c */  res(WORD_LEN),
    /* 0x2d */  res(WORD_LEN),
    /* 0x2e */  res(WORD_LEN),
    /* 0x2f */  res(WORD_LEN),
    /* 0x30 */  { "frameRect",         8, "rect" },
    /* 0x31 */  { "paintRect",         8, "rect" },
    /* 0x32 */  { "eraseRect",         8, "rect" },
    /* 0x33 */  { "invertRect",        8, "rect" },
    /* 0x34 */  { "fillRect",          8, "rect" },
    /* 0x35 */  res(8),
    /* 0x36 */  res(8),
    /* 0x37 */  res(8),
    /* 0x38 */  { "frameSameRect",     0, "rect" },
    /* 0x39 */  { "paintSameRect",     0, "rect" },
    /* 0x3a */  { "eraseSameRect",     0, "rect" },
    /* 0x3b */  { "invertSameRect",    0, "rect" },
    /* 0x3c */  { "fillSameRect",      0, "rect" },
    /* 0x3d */  res(0),
    /* 0x3e */  res(0),
    /* 0x3f */  res(0),
    /* 0x40 */  { "frameRRect",        8, "rect" },
    /* 0x41 */  { "paintRRect",        8, "rect" },
    /* 0x42 */  { "eraseRRect",        8, "rect" },
    /* 0x43 */  { "invertRRect",       8, "rect" },
    /* 0x44 */  { "fillRRrect",        8, "rect" },
    /* 0x45 */  res(8),
    /* 0x46 */  res(8),
    /* 0x47 */  res(8),
    /* 0x48 */  { "frameSameRRect",    0, "rect" },
    /* 0x49 */  { "paintSameRRect",    0, "rect" },
    /* 0x4a */  { "eraseSameRRect",    0, "rect" },
    /* 0x4b */  { "invertSameRRect",   0, "rect" },
    /* 0x4c */  { "fillSameRRect",     0, "rect" },
    /* 0x4d */  res(0),
    /* 0x4e */  res(0),
    /* 0x4f */  res(0),
    /* 0x50 */  { "frameOval",         8, "rect" },
    /* 0x51 */  { "paintOval",         8, "rect" },
    /* 0x52 */  { "eraseOval",         8, "rect" },
    /* 0x53 */  { "invertOval",        8, "rect" },
    /* 0x54 */  { "fillOval",          8, "rect" },
    /* 0x55 */  res(8),
    /* 0x56 */  res(8),
    /* 0x57 */  res(8),
    /* 0x58 */  { "frameSameOval",     0, "rect" },
    /* 0x59 */  { "paintSameOval",     0, "rect" },
    /* 0x5a */  { "eraseSameOval",     0, "rect" },
    /* 0x5b */  { "invertSameOval",    0, "rect" },
    /* 0x5c */  { "fillSameOval",      0, "rect" },
    /* 0x5d */  res(0),
    /* 0x5e */  res(0),
    /* 0x5f */  res(0),
    /* 0x60 */  { "frameArc",         12, "rect, startAngle, arcAngle" },
    /* 0x61 */  { "paintArc",         12, "rect, startAngle, arcAngle" },
    /* 0x62 */  { "eraseArc",         12, "rect, startAngle, arcAngle" },
    /* 0x63 */  { "invertArc",        12, "rect, startAngle, arcAngle" },
    /* 0x64 */  { "fillArc",          12, "rect, startAngle, arcAngle" },
    /* 0x65 */  res(12),
    /* 0x66 */  res(12),
    /* 0x67 */  res(12),
    /* 0x68 */  { "frameSameArc",      4, "rect, startAngle, arcAngle" },
    /* 0x69 */  { "paintSameArc",      4, "rect, startAngle, arcAngle" },
    /* 0x6a */  { "eraseSameArc",      4, "rect, startAngle, arcAngle" },
    /* 0x6b */  { "invertSameArc",     4, "rect, startAngle, arcAngle" },
    /* 0x6c */  { "fillSameArc",       4, "rect, startAngle, arcAngle" },
    /* 0x6d */  res(4),
    /* 0x6e */  res(4),
    /* 0x6f */  res(4),
    /* 0x70 */  { "framePoly",        NA, "poly" },
    /* 0x71 */  { "paintPoly",        NA, "poly" },
    /* 0x72 */  { "erasePoly",        NA, "poly" },
    /* 0x73 */  { "invertPoly",       NA, "poly" },
    /* 0x74 */  { "fillPoly",         NA, "poly" },
    /* 0x75 */  res(NA),
    /* 0x76 */  res(NA),
    /* 0x77 */  res(NA),
    /* 0x78 */  { "frameSamePoly",     0, "poly (NYI)" },
    /* 0x79 */  { "paintSamePoly",     0, "poly (NYI)" },
    /* 0x7a */  { "eraseSamePoly",     0, "poly (NYI)" },
    /* 0x7b */  { "invertSamePoly",    0, "poly (NYI)" },
    /* 0x7c */  { "fillSamePoly",      0, "poly (NYI)" },
    /* 0x7d */  res(0),
    /* 0x7e */  res(0),
    /* 0x7f */  res(0),
    /* 0x80 */  { "frameRgn",         NA, "region" },
    /* 0x81 */  { "paintRgn",         NA, "region" },
    /* 0x82 */  { "eraseRgn",         NA, "region" },
    /* 0x83 */  { "invertRgn",        NA, "region" },
    /* 0x84 */  { "fillRgn",          NA, "region" },
    /* 0x85 */  res(NA),
    /* 0x86 */  res(NA),
    /* 0x87 */  res(NA),
    /* 0x88 */  { "frameSameRgn",      0, "region (NYI)" },
    /* 0x89 */  { "paintSameRgn",      0, "region (NYI)" },
    /* 0x8a */  { "eraseSameRgn",      0, "region (NYI)" },
    /* 0x8b */  { "invertSameRgn",     0, "region (NYI)" },
    /* 0x8c */  { "fillSameRgn",       0, "region (NYI)" },
    /* 0x8d */  res(0),
    /* 0x8e */  res(0),
    /* 0x8f */  res(0),
    /* 0x90 */  { "BitsRect",         NA, "copybits, rect clipped" },
    /* 0x91 */  { "BitsRgn",          NA, "copybits, rgn clipped" },
    /* 0x92 */  res(WORD_LEN),
    /* 0x93 */  res(WORD_LEN),
    /* 0x94 */  res(WORD_LEN),
    /* 0x95 */  res(WORD_LEN),
    /* 0x96 */  res(WORD_LEN),
    /* 0x97 */  res(WORD_LEN),
    /* 0x98 */  { "PackBitsRect",     NA, "packed copybits, rect clipped" },
    /* 0x99 */  { "PackBitsRgn",      NA, "packed copybits, rgn clipped" },
    /* 0x9a */  { "Opcode_9A",        NA, "the mysterious opcode 9A" },
    /* 0x9b */  res(WORD_LEN),
    /* 0x9c */  res(WORD_LEN),
    /* 0x9d */  res(WORD_LEN),
    /* 0x9e */  res(WORD_LEN),
    /* 0x9f */  res(WORD_LEN),
    /* 0xa0 */  { "ShortComment",      2, "kind (word)" },
    /* 0xa1 */  { "LongComment",      NA, "kind (word), size (word), data" }
  };
/*
/--------------------------------------------------------------------
|
|      $Log: ploptable.h,v $
|      Revision 1.1  2009/12/09 05:33:52  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:16  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:55  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:04  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:22  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:26  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:31  황정영
|      2006.05.17
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.3  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
--------------------------------------------------------------------
*/
