/*
/--------------------------------------------------------------------
|
|      $Id: plhsvconvert.cpp,v 1.1 2009/12/09 05:33:45 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plhsvconvert.h"

void fp_rgb_to_hsv (double *r,
                    double *g,
                    double *b)
{
  int red, green, blue;
  double h=0, s, v;
  int min, max;
  int delta;

  red = (int)*r;
  green = (int)*g;
  blue = (int)*b;

  if (red > green)
    {
      if (red > blue)
        max = red;
      else
        max = blue;

      if (green < blue)
        min = green;
      else
        min = blue;
    }
  else
    {
      if (green > blue)
        max = green;
      else
        max = blue;

      if (red < blue)
        min = red;
      else
        min = blue;
    }

  v = max;

  if (max != 0)
    s = ((max - min) * 255) / (double) max;
  else
    s = 0;

  if (s == 0)
    h = 0;
  else
    {
      delta = max - min;
      if (red == max)
        h = (green - blue) / (double) delta;
      else if (green == max)
        h = 2 + (blue - red) / (double) delta;
      else if (blue == max)
        h = 4 + (red - green) / (double) delta;
      h *= 42.5;

      if (h < 0)
        h += 255;
      if (h > 255)
        h -= 255;
    }

  *r = h;
  *g = s;
  *b = v;
}


void fp_hsv_to_rgb (double *h,
                    double *s,
                    double *v)
{
  double hue, saturation, value;
  double f, p, q, t;

  if (((int)*s) == 0)
    {
      *h = *v;
      *s = *v;
      *v = *v;
    }
  else
    {
      hue = *h * 6.0 / 255.0;
      saturation = *s / 255.0;
      value = *v / 255.0;

      f = hue - (int) hue;
      p = value * (1.0 - saturation);
      q = value * (1.0 - (saturation * f));
      t = value * (1.0 - (saturation * (1.0 - f)));

      switch ((int) hue)
        {
        case 0:
          *h = value * 255.0;
          *s = t * 255.0;
          *v = p * 255.0;
          break;
        case 1:
          *h = q * 255.0;
          *s = value * 255.0;
          *v = p * 255.0;
          break;
        case 2:
          *h = p * 255.0;
          *s = value * 255.0;
          *v = t * 255.0;
          break;
        case 3:
          *h = p * 255.0;
          *s = q * 255.0;
          *v = value * 255.0;
          break;
        case 4:
          *h = t * 255.0;
          *s = p * 255.0;
          *v = value * 255.0;
          break;
        case 5:
          *h = value * 255.0;
          *s = p * 255.0;
          *v = q * 255.0;
          break;
        }
    }
}

/*
/--------------------------------------------------------------------
|
|      $Log: plhsvconvert.cpp,v $
|      Revision 1.1  2009/12/09 05:33:45  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:11  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:54  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:50  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:18  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2000/10/12 21:56:12  uzadow
|      Moved local functions from VideoInvertFilter.cpp to
|      hsvconvert.*
|
|
|
\--------------------------------------------------------------------
*/
